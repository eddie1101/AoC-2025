#pragma once

#include <stdlib.h>

typedef struct
{
  long low;
  long high;
} range;

typedef struct
{
  range* rs;
  size_t num_ranges;
  size_t ranges_size;
} range_list;

range* create_range();
range_list* create_range_list();
int resize_ranges(range_list*);
void add_range(range_list*, long, long);
range* get_range(range_list*, size_t);
range_list* read_ranges_file(char*);
void print_ranges(range_list*);
