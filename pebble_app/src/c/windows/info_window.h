#pragma once

#include <pebble.h>
#include "../modules/types.h"
#include "../modules/util.h"

#define PADDING 0
#define HEAD_HEIGHT 20
#define DARK_BACKGROUND true

typedef void(DoneCallback)(void *data);

typedef struct InfoConfig
{
    char head_left[MAX_TEXT_LEN];
    char head_right[MAX_TEXT_LEN];
    char main[MAX_TEXT_LEN];
    DoneCallback *action;
    void *extra;

} InfoConfig;

void info_window_init(InfoConfig *config);
void info_window_set_head_left(char text[]);
void info_window_set_head_right(char text[]);
void info_window_set_main(char text[]);