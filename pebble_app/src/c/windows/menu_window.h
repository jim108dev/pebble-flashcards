#pragma once // Prevent errors from being included multiple times

#include <pebble.h>
#include "../modules/util.h"

typedef void(ActionCallback)(uint8_t feedback, void *data);

typedef struct MenuConfig
{
    char labels[10][30];
    uint8_t max_items;
    ActionCallback *action;
    uint8_t selected;
    void *extra;
} MenuConfig;

#define ITEM_HEIGHT 30

void menu_window_init(MenuConfig config);