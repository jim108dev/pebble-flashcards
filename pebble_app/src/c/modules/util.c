#include "util.h"

void record_to_string(char buf[MAX_TEXT_LEN], Record r)
{
    char time_buf1[MAX_TEXT_LEN];
    time_to_string(time_buf1, r.start);
    
    char time_buf2[MAX_TEXT_LEN];
    time_to_string(time_buf2, r.stop);
    
    snprintf(buf, MAX_TEXT_LEN, "Record ('%s';'%.5s...';'%.5s...';%d;%ld;%ld)", r.id, r.text1, r.text2, r.feedback, r.start, r.stop);
}

void time_to_string(char buf[MAX_TEXT_LEN], time_t rawtime)
{
    struct tm *info;

    time(&rawtime);

    info = localtime(&rawtime);

    strftime(buf, MAX_TEXT_LEN, "%x - %I:%M%p", info);
}

uint8_t packet_get_uint8(DictionaryIterator *inbox_iter, int key)
{
    if (!packet_contains_key(inbox_iter, key))
    {
        return 0;
    }
    return dict_find(inbox_iter, key)->value->uint8;
}

int textcpy(char *dest, const char *src)
{
    return snprintf(dest, MAX_SMALL_TEXT_LEN, "%s", src);
}

int small_textcpy(char *dest, const char *src)
{
    return snprintf(dest, MAX_TEXT_LEN, "%s", src);
}

int sprint_progress(char text[MAX_SMALL_TEXT_LEN], uint8_t num, uint8_t max)
{
    return snprintf(text, MAX_SMALL_TEXT_LEN, "%d/%d", num, max);
}


bool ll_free_callback(void *object, void *context){
  FREE_SAFE(object);
  return true;
}

int dp_fill_text(char *dest, ProcessingState* state){
    char *buf = data_processor_get_string(state);
    int n = textcpy(dest, buf);
    free(buf);
    return n;
}

int dp_fill_small_text(char *dest, ProcessingState* state){
    char *buf = data_processor_get_string(state);
    int n = small_textcpy(dest, buf);
    free(buf);
    return n;
}