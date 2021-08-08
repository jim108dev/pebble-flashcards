#pragma once // Prevent errors from being included multiple times

#include <pebble.h>
#include "time.h"

#include "modules/types.h"
#include "modules/util.h"
#include "modules/download.h"
#include "modules/dlog.h"
#include "modules/pers.h"
#include "windows/info_window.h"
#include "windows/menu_window.h"

static void garbage_collection();
static void on_finish_record(uint8_t feedback, void *data);
static void on_show_text1(void *data);
static void on_show_text2(void *data);
static void on_show_feedback(void *data);
static void show_first_window();
static void show_last_window();
static void on_download_success(Record *records, uint8_t max_records);
static void on_download_fail(char *msg);
static void init();
static void deinit();