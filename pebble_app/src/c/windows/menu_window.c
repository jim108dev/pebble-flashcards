#include <pebble.h>
#include "menu_window.h"

static Window *s_window = NULL;
static MenuLayer *s_menu_layer;
static MenuConfig *s_config;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context)
{
    return s_config->max_items;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context)
{
    menu_cell_basic_draw(ctx, cell_layer, s_config->labels[cell_index->row], NULL, NULL);
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context)
{
    return ITEM_HEIGHT;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context)
{
    s_config->action(cell_index->row, s_config->extra);
}

void invert_colors(MenuLayer *layer)
{
    GColor white = GColorWhite;
    GColor black = GColorBlack;

    menu_layer_set_normal_colors(layer, black, white);
    menu_layer_set_highlight_colors(layer, white, black);
}

static void window_load(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    s_menu_layer = menu_layer_create(bounds);

    if (DARK_BACKGROUND)
    {
        invert_colors(s_menu_layer);
    }
    menu_layer_pad_bottom_enable(s_menu_layer, false);

    menu_layer_set_click_config_onto_window(s_menu_layer, window);
    menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
                                                     .get_num_rows = get_num_rows_callback,
                                                     .draw_row = draw_row_callback,
                                                     .get_cell_height = get_cell_height_callback,
                                                     .select_click = select_callback,
                                                 });

    layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
    menu_layer_set_selected_index(s_menu_layer, (MenuIndex){.section = 0, .row = s_config->selected}, MenuRowAlignCenter, false);
}

static void window_unload(Window *window)
{
    menu_layer_destroy(s_menu_layer);
}

static void menu_window_deinit()
{
    window_destroy(s_window);
    s_window = NULL;
    FREE_SAFE(s_config);
}

void menu_window_init(MenuConfig *config)
{
    if (s_window != NULL)
    {
        window_stack_pop_all(true);
        menu_window_deinit();
    }

    s_config = config;
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers){
                                             .load = window_load,
                                             .unload = window_unload,
                                         });
    window_stack_push(s_window, true);
}
