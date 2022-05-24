#include "main.h"



static void start_process_item(uint8_t num_seen, uint8_t max_records)
{
  Record *record = malloc(sizeof(Record));
  pers_read_record(num_seen, record);

  DEBUG_RECORD(*record);

  CurrentRecord *current = malloc(sizeof(CurrentRecord));
  current->record = record;
  current->record->start = time(NULL);
  current->max_records = max_records;
  current->num_seen = num_seen;

  on_show_text1(current);
}

static void on_finish_record(uint8_t feedback, void *data)
{
  CurrentRecord *current = (CurrentRecord *)data;

  DEBUG_RECORD(*current->record);

  current->record->feedback = feedback;
  current->record->stop = time(NULL);

  pers_write_record(*current->record, current->num_seen-1);
  pers_write_num_seen(current->num_seen);
  pers_write_last_tested();
  dlog_log(*current->record);


  uint8_t max_records = current->max_records;
  uint8_t num_seen = current->num_seen;

  FREE_SAFE(current->record);
  FREE_SAFE(current);

  if (num_seen  == max_records)
  {
    dlog_deinit();
    show_last_window();
    return;
  }
  start_process_item(num_seen, max_records);
}

static void on_start_process(void *_)
{
  download_deinit();

  uint8_t max_records = pers_read_max_records();
  uint8_t num_seen = pers_read_num_seen();
  dlog_init();
  start_process_item(num_seen, max_records);
}

static void on_show_text1(void *data)
{
  CurrentRecord *current = (CurrentRecord *)data;

  DEBUG_RECORD(*current->record);

  current->num_seen++;

  InfoConfig *c = malloc(sizeof(InfoConfig));
  c->action = on_show_text2;
  c->extra = current;

  strcpy(c->head_left, current->record->id);

  sprint_progress(c->head_right, current->num_seen, current->max_records);
  strcpy(c->main, current->record->text1);

  info_window_init(c);
}

static void on_show_text2(void *data)
{
  CurrentRecord *current = (CurrentRecord *)data;

  DEBUG_RECORD(*current->record);

  InfoConfig *c = malloc(sizeof(InfoConfig));
  c->action = on_show_feedback;
  c->extra = current;

  strcpy(c->head_left, current->record->id);
  sprint_progress(c->head_right, current->num_seen, current->max_records);
  strcpy(c->main, current->record->text2);

  info_window_init(c);
}

static void on_show_feedback(void *data)
{
  CurrentRecord *current = (CurrentRecord *)data;

  DEBUG_RECORD(*current->record);

  MenuConfig *c = malloc(sizeof(MenuConfig));
  c->action = on_finish_record;
  c->extra = current;
  strcpy(c->labels[0], "Unclear");
  strcpy(c->labels[1], "Hard");
  strcpy(c->labels[2], "Good");
  strcpy(c->labels[3], "Easy");
  strcpy(c->labels[4], "Bury");

  c->max_items = 5;
  c->selected = current->record->feedback;

  menu_window_init(c);
}

static void show_first_window()
{
  InfoConfig *c = malloc(sizeof(InfoConfig));

  uint8_t max_records = pers_read_max_records();
  uint8_t num_seen = pers_read_num_seen();
  time_t uploaded_date = pers_read_uploaded_date();

  strcpy(c->head_left, "");
  sprint_progress(c->head_right, num_seen, max_records);

  if (max_records == 0)
  {
    strcpy(c->main, "No questions found. Please run 'pebble_upload.py'.");
    c->action = NULL;
    info_window_init(c);
    return;
  }

  if (num_seen == 0)
  {
    char buf[MAX_SMALL_TEXT_LEN];
    time_to_string(buf, uploaded_date);
    snprintf(c->main, sizeof(c->main), "Questions uploaded on the %s. Press 'select' to start.", buf);
    c->action = on_start_process;
    info_window_init(c);
    return;
  }

  if (num_seen < max_records)
  {
    sprint_progress(c->head_right, num_seen, max_records);
    snprintf(c->main, sizeof(c->main), "Only %d questions to go. Press 'select' to start.", max_records - num_seen);
    c->action = on_start_process;
    info_window_init(c);
  }

  if (num_seen == max_records)
  {
    sprint_progress(c->head_right, num_seen, max_records);
    strcpy(c->main, "All questions answered. Please run 'pebble_upload.py'.");
    c->action = NULL;
    info_window_init(c);
    return;
  }
}

static void show_last_window()
{
  InfoConfig *c = malloc(sizeof(InfoConfig));

  uint8_t max_records = pers_read_max_records();
  uint8_t current_num = pers_read_num_seen();

  strcpy(c->head_left, "The End");
  sprint_progress(c->head_right, current_num, max_records);
  strcpy(c->main, "All questions answered. Please run 'pebble_download.py'.");

  c->action = NULL;

  info_window_init(c);
}

static void download_success(Record *records, uint8_t max_records)
{
  
  for (int i = 0; i < max_records; i++)
  {
    int status_code = pers_write_record(records[i], i);
    if (status_code <= 0)
    {
      DEBUG("Persistend write error. Could only store %d items", i);
      max_records = i;
      break;
    }
  }
  download_deinit();

  pers_write_max_records(max_records);
  pers_write_num_seen(0);
  pers_write_uploaded_date();

  show_first_window();
}

static void download_fail(char msg[MAX_TEXT_LEN])
{
  info_window_set_main("Download failed. Please try 'pebble_upload.py' again");

  download_deinit();
}

static void init()
{
  AppLaunchReason appLaunchReason = launch_reason();

  if (appLaunchReason == APP_LAUNCH_PHONE)
  {
    pers_sweep();
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