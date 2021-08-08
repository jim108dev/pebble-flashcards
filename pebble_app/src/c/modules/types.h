#pragma once // Prevent errors from being included multiple times

#include <pebble.h> // Pebble SDK symbols

#define MAX_SMALL_TEXT_LEN    20
#define MAX_TEXT_LEN          80

typedef struct Record
{
    char id[MAX_SMALL_TEXT_LEN];        //  20 bytes
    char text1[MAX_TEXT_LEN];           //  80 bytes
    char text2[MAX_TEXT_LEN];           //  80 bytes
    uint8_t feedback;                   //   1 byte
    time_t start;                       //   4 bytes
    time_t stop;                        //   4 bytes
} Record;                               // 189 bytes

typedef void (*DownloadSuccessCallback)(Record records[], uint8_t max_records);
typedef void (*DownloadFailCallback)(char message[MAX_TEXT_LEN]);

typedef struct CurrentRecord
{
    uint8_t num;
    uint8_t max;
    Record *record;

} CurrentRecord;