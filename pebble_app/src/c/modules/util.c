#include "util.h"

void record_to_string(char buf[MAX_TEXT_LEN], Record r)
{
    char time_buf1[MAX_TEXT_LEN];
    time_to_string(time_buf1, r.start);
    
    char time_buf2[MAX_TEXT_LEN];
    time_to_string(time_buf2, r.stop);
    
    snprintf(buf, MAX_TEXT_LEN, "Record ('%s';'%.5s...';'%.5s...';%d,%ld,%ld)", r.id, r.text1, r.text2, r.feedback, r.start, r.stop);
}

void time_to_string(char buf[MAX_TEXT_LEN], time_t rawtime)
{
   struct tm *info;

   time( &rawtime );

   info = localtime( &rawtime );

   strftime(buf,MAX_TEXT_LEN,"%x - %I:%M%p", info);
}

uint8_t packet_get_uint8(DictionaryIterator *inbox_iter, int key)
{
    if (!packet_contains_key(inbox_iter, key))
    {
        return 0;
    }
    return dict_find(inbox_iter, key)->value->uint8;
}

char *textcpy(char * dest, const char * src){
    strncpy(dest, src, MAX_TEXT_LEN);
    dest[MAX_TEXT_LEN-1] = '\0';
    return dest;
}

char *small_textcpy(char * dest, const char * src){
    strncpy(dest, src, MAX_SMALL_TEXT_LEN);
    dest[MAX_SMALL_TEXT_LEN-1] = '\0';
    return dest;
}