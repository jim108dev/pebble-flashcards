#pragma once // Prevent errors from being included multiple times

#include <pebble.h>
#include "types.h"
#include "util.h"

//private
#define LOG_TAG 1
typedef struct DLogRecord
{
    char id[MAX_SMALL_TEXT_LEN];      //  20 bytes
    uint8_t feedback;         //   1 byte
    time_t start;             //   4 bytes
    time_t end;               //   4 bytes
} DLogRecord;                 //  29 bytes


//public
void dlog_init();

void dlog_log(Record record);

void dlog_deinit();


