#include "pers.h"

void pers_read_record(uint8_t num, Record *buf)
{
  DEBUG("Read record at position %d", num);
  persist_read_data(num + RECORDS_OFFSET, buf, sizeof(Record));
}

int pers_write_record(Record record, uint num)
{
  return persist_write_data(num + RECORDS_OFFSET, &record, sizeof(Record));
}

void pers_sweep()
{
  for (uint8_t i = 0; i < 255; i++)
  {
    persist_delete(i);
  }
}

//settings
uint8_t pers_read_max_records()
{
  uint8_t buf = 0;
  persist_read_data(MAX_RECORDS_KEY, &buf, sizeof(uint8_t));
  return buf;
}

uint8_t pers_read_num_seen()
{
  uint8_t buf = 0;
  persist_read_data(CURRENT_NUM_KEY, &buf, sizeof(uint8_t));
  return buf;
}

time_t pers_read_uploaded_date()
{
  time_t buf = 0;
  persist_read_data(UPLOADED_DATE_KEY, &buf, sizeof(time_t));
  return buf;
}

time_t pers_read_last_tested()
{
  time_t buf = 0;
  persist_read_data(LAST_TESTED_KEY, &buf, sizeof(time_t));
  return buf;
}

void pers_write_max_records(uint8_t n)
{
  persist_write_data(MAX_RECORDS_KEY, &n, sizeof(uint8_t));
}

void pers_write_num_seen(uint8_t n)
{
  persist_write_data(CURRENT_NUM_KEY, &n, sizeof(uint8_t));
}

void pers_write_last_tested()
{
  time_t t = time(NULL);
  persist_write_data(LAST_TESTED_KEY, &t, sizeof(time_t));
}

void pers_write_uploaded_date()
{
  time_t t = time(NULL);
  persist_write_data(UPLOADED_DATE_KEY, &t, sizeof(time_t));
}
