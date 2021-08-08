#include "info_window.h"

static Window *s_window = NULL;
static TextLayer *s_main_layer;
static TextLayer *s_head_left_layer;
static TextLayer *s_head_right_layer;
static InfoConfig *s_config;
static char buf[30];

void click_handler(ClickRecognizerRef recognizer, void *context)
{
  s_config->action(s_config->extra);
}

void click_provider(void *context)
{
  window_single_click_subscribe(BUTTON_ID_SELECT, click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, click_handler);
}

void window_load(Window *window)
{
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_head_left_layer = text_layer_create(GRect(PADDING, 0, bounds.size.w/2-PADDING, HEAD_HEIGHT));

  text_layer_set_text(s_head_left_layer, s_config->head_left);
  text_layer_set_text_alignment(s_head_left_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(s_head_left_layer));

  s_head_right_layer = text_layer_create(GRect(bounds.size.w/2, 0, bounds.size.w/2-PADDING, HEAD_HEIGHT));

  text_layer_set_text(s_head_right_layer, s_config->head_right);
  text_layer_set_text_alignment(s_head_right_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_head_right_layer));

  GRect max_text_bounds = GRect(PADDING, HEAD_HEIGHT, bounds.size.w-PADDING, bounds.size.h);
  s_main_layer = text_layer_create(max_text_bounds);
  text_layer_set_font(s_main_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text(s_main_layer, s_config->main);
  text_layer_set_text_alignment(s_main_layer, GTextAlignmentLeft);

  GSize max_size = GSize(max_text_bounds.size.w - max_text_bounds.origin.x, max_text_bounds.size.h - max_text_bounds.origin.y);
  text_layer_set_size(s_main_layer, max_size);
  layer_add_child(window_layer, text_layer_get_layer(s_main_layer));
}

void info_window_set_head_left(char text[MAX_TEXT_LEN])
{
  text_layer_set_text(s_head_left_layer, text);
}

void info_window_set_head_right(char text[MAX_TEXT_LEN])
{
  text_layer_set_text(s_head_right_layer, text);
}

void info_window_set_main(char text[MAX_TEXT_LEN])
{
  text_layer_set_text(s_main_layer, text);
}

void window_unload(Window *window)
{
  text_layer_destroy(s_head_left_layer);
  text_layer_destroy(s_head_right_layer);
  text_layer_destroy(s_main_layer);
}

static void info_window_deinit()
{
  window_destroy(s_window);
  s_window = NULL;
  FREE_SAFE(s_config);
}

void info_window_init(InfoConfig *config)
{
  if (s_window != NULL)
  {
    window_stack_pop_all(true);
    info_window_deinit();
  }

  s_window = window_create();
  s_config = config;

  window_set_click_config_provider(s_window, click_provider);
  window_set_window_handlers(s_window, (WindowHandlers){
                                           .load = window_load,
                                           .unload = window_unload,
                                       });
  window_stack_push(s_window, true);
}