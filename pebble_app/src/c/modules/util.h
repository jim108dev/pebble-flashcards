#pragma once // Prevent errors from being included multiple times

#include "types.h"
#include <pebble-packet/pebble-packet.h>
#include <@smallstoneapps/data-processor/data-processor.h>


//Replaced __FILE__ by __FUNCTION__ because it is more helpful.
#define DISABLE_LOGGING false

#if DISABLE_LOGGING
#define LOG(...)
#define DEBUG(...)
#define INFO(...)
#define WARN(...)
#define ERROR(...)
#else
#define LOG(...) app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define DEBUG(...) app_log(APP_LOG_LEVEL_DEBUG, __FUNCTION__, __LINE__, __VA_ARGS__)
#define INFO(...) app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define WARN(...) app_log(APP_LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define ERROR(...) app_log(APP_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#endif

#define DEBUG_RECORD(record) char debug_buf[100]; record_to_string(debug_buf, record); DEBUG(debug_buf);

#define FREE_SAFE(ptr) if (NULL != ptr) { free(ptr); ptr = NULL; }

void record_to_string(char buf[MAX_TEXT_LEN], Record r);

void time_to_string(char buf[MAX_TEXT_LEN], time_t rawtime);
int textcpy(char * dest, const char * src);
int small_textcpy(char * dest, const char * src);
int sprint_progress(char text[MAX_SMALL_TEXT_LEN], uint8_t num, uint8_t max);
bool ll_free_callback(void *object, void *context);
int dp_fill_text(char *dest, ProcessingState* state);
int dp_fill_small_text(char *dest, ProcessingState* state);