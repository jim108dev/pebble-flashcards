#pragma once // Prevent errors from being included multiple times

#include <pebble.h> // Pebble SDK symbols
#include "types.h"
#include "util.h"

#define RECORDS_OFFSET 10
#define MAX_RECORDS_KEY 0
#define CURRENT_NUM_KEY 1
#define UPLOADED_DATE_KEY 2
#define LAST_TESTED_KEY 3

void pers_read_record(uint8_t num, Record *buf);

void pers_read_all(Record *buf);

int pers_write_record(Record record, uint num);

void pers_sweep();

uint8_t pers_read_max_records();
uint8_t pers_read_num_seen();
time_t pers_read_uploaded_date();
time_t pers_read_last_tested();

void pers_write_max_records(uint8_t n);
void pers_write_num_seen(uint8_t n);
void pers_write_last_tested();
void pers_write_uploaded_date();