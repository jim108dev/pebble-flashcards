#pragma once // Prevent errors from being included multiple times

#include <pebble.h>
#include <@smallstoneapps/data-processor/data-processor.h>
#include <pebble-packet/pebble-packet.h>
#include <pebble-events/pebble-events.h>
#include "util.h"
#include "types.h"

#define INBOX_SIZE app_message_inbox_size_maximum()
#define DOWNLOAD_KEY_MAX 100
#define DOWNLOAD_KEY_DATA 200
#define LOG_TAG 1
#define TIMEOUT 30000 //1/2 minute

typedef struct InputRecord
{
    char id[MAX_SMALL_TEXT_LEN];        //  16 bytes
    char text1[MAX_TEXT_LEN];           //  85 bytes
    char text2[MAX_TEXT_LEN];           //  85 bytes
} InputRecord;                          // 186 bytes

void download_init(DownloadSuccessCallback success, DownloadFailCallback fail);

void download_deinit();