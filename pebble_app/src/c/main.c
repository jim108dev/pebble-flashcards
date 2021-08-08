#include "main.h"

static void start_process_item(uint8_t num, uint8_t max)
{
  Record *record = malloc(sizeof(Record));
  pers_read_single(num, record);

  DEBUG_RECORD(*record);

  CurrentRecord *current = malloc(sizeof(CurrentRecord));
  current->record = record;
  current->max = max;
  current->num = num;

  on_show_text1(current);
}

static void on_finish_record(uint8_t feedback, void *data)
{
  CurrentRecord *current = (CurrentRecord *)data;

  DEBUG_RECORD(*current->record);

  current->record->feedback = feedback;
  current->record->stop = time(NULL);

  pers_write(*current->record, current->num);
  dlog_log(*current->record);

  uint8_t num = current->num + 1;
  uint8_t max = current->max;

  FREE_SAFE(current->record);
  FREE_SAFE(current);

  if (num == max)
  {
    dlog_deinit();
    show_last_window();
    return;
  }
  start_process_item(num, max);
}

static void on_start_process(void *data)
{
  uint8_t max = pers_read_max_records();
  if (max > 0)
  {
    dlog_init();
    start_process_item(0, max);
  }
}

static void on_show_text1(void *data)
{
  DEBUG("-");
  CurrentRecord *current = (CurrentRecord *)data;

  DEBUG_RECORD(*current->record);

  current->record->start = time(NULL);

  InfoConfig *c = malloc(sizeof(InfoConfig));
  c->action = on_show_text2;
  c->extra = current;
  strcpy(c->main, current->record->text1);
  snprintf(c->status, sizeof(c->status), "(%d / %d)", current->num + 1, current->max);

  info_window_init(c);
}

static void on_show_text2(void *data)
{
  DEBUG("-");
  CurrentRecord *current = (CurrentRecord *)data;

  DEBUG_RECORD(*current->record);

  InfoConfig *c = malloc(sizeof(InfoConfig));
  c->action = on_show_feedback;
  c->extra = current;

  strcpy(c->main, current->record->text2);
  snprintf(c->status, sizeof(c->status), "(%d / %d)", current->num + 1, current->max);

  info_window_init(c);
}

static void on_show_feedback(void *data)
{
  CurrentRecord *current = (CurrentRecord *)data;

  DEBUG_RECORD(*current->record);

  MenuConfig *c = malloc(sizeof(MenuConfig));
  c->action = on_finish_record;
  c->extra = current;
  strcpy(c->labels[0], "Again");
  strcpy(c->labels[1], "Hard");
  strcpy(c->labels[2], "Good");
  strcpy(c->labels[3], "Easy");

  c->max_items = 4;
  c->selected = current->record->feedback;

  menu_window_init(c);
}

static void show_first_window()
{
  InfoConfig *c = malloc(sizeof(InfoConfig));

  uint8_t max_records = pers_read_max_records();

  if (max_records > 0)
  {
    snprintf(c->main, sizeof(c->main), "%d records found. Press select to start.", max_records);
    c->action = on_start_process;
  }
  else
  {
    snprintf(c->main, sizeof(c->main), "%d records found. Please run 'pebble_upload.py'.", max_records);
    c->action = NULL;
  }
  strcpy(c->status, "");

  info_window_init(c);
}

static void show_last_window()
{
  InfoConfig *c = malloc(sizeof(InfoConfig));

  uint8_t max_records = pers_read_max_records();

  snprintf(c->main, sizeof(c->main), "%d questions evaluated. Press select to start again.", max_records);

  strcpy(c->status, "The End");

  c->action = on_start_process;

  info_window_init(c);
}

static void download_success(Record *records, uint8_t max_records)
{
  int records_size = 0;
  for (int i = 0; i < max_records; i++)
  {
    int status_code = pers_write(records[i], i);
    if (status_code <= 0){
      DEBUG("Persistend write error. Could only store %d items", i);
      max_records = i;
      break;
    }
  }
  download_deinit();

  pers_write_max_records(max_records);

  show_first_window();
}

static void download_fail(char msg[MAX_TEXT_LEN])
{
  info_window_set_main("Download failed. Please try 'pebble_upload.py' again or press select for demo mode");

  download_deinit();
}

static void init()
{
  AppLaunchReason appLaunchReason = launch_reason();

  if (appLaunchReason == APP_LAUNCH_PHONE)
  {
    //pers_sweep();
    show_first_window();
    download_init(download_success, download_fail);
    // If app was launched by phone and close to last app is disabled, always exit to the watchface instead of to the menu
    exit_reason_set(APP_EXIT_ACTION_PERFORMED_SUCCESSFULLY);
    return;
  }

  show_first_window();
}

static void deinit()
{
  dlog_deinit();
}

int main()
{
  init();
  app_event_loop();
  deinit();
}