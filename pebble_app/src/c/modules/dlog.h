#pragma once // Prevent errors from being included multiple times

#include <pebble.h>
#include "types.h"
#include "util.h"

//private
#define LOG_TAG 1
typedef struct OutputRecord
{
    char id[MAX_SMALL_TEXT_LEN];      //  16 bytes
    uint8_t feedback;                 //   1 byte
    time_t start;                     //   4 bytes
    time_t end;                       //   4 bytes
} OutputRecord;                       //  25 bytes


//public
void dlog_init();

void dlog_log(Record record);

void dlog_deinit();


