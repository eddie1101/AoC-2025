#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ranges.h"

range* create_ranges(size_t num_ranges)
{
  range* r = (range*) malloc(num_ranges * sizeof(range));
  if (!r)
  {
    perror("could not allocate ranges");
    return NULL;
  }
  return r;
}

range_list* create_range_list()
{
  range_list* r = (range_list*) malloc(sizeof(range_list));
  if (!r)
  {
    perror("Could not allocate range_list");
    return NULL;
  }

  r->num_ranges = 0;
  r->ranges_size = 64;
  r->rs = create_ranges(r->ranges_size);
  
  return r;
}

int resize_ranges(range_list* r)
{
  range* temp = (range*) realloc(r->rs, r->ranges_size * 2 * sizeof(range));
  if (!temp)
  {
    perror("could not resize ranges");
    return -1;
  }

  r->rs = temp;
  r->ranges_size *= 2;
  return 0;
}

void add_range(range_list* r, long low, long high)
{
  if (r->num_ranges >= r->ranges_size - 1)
  {
    resize_ranges(r);
  }
  r->rs[r->num_ranges].low = low;
  r->rs[r->num_ranges].high = high;
}

range* get_range(range_list* r, size_t idx)
{
  return r->rs + idx;
}

range_list* read_ranges_file(char* filename)
{

  FILE* input = fopen(filename, "r");
  if (!input)
  {
    char* error_message = (char*) alloca(64);
    sprintf(error_message, "Could not open file \'%s\'", filename);
    perror(error_message);
    return NULL;
  }

  range_list* ranges = create_range_list();

  const size_t BUF_SIZE = 128;
  char* line = (char*) malloc(BUF_SIZE * sizeof(char));
  size_t len = BUF_SIZE;
  ssize_t read_status;
  while ((read_status = getline(&line, &len, input)) > -1)
  {
    char* lowc = strtok(line, "-");
    char* highc = strtok(NULL, "-");

    long low = atol(lowc);
    long high = atol(highc);

    add_range(ranges, low, high);
  }

  return ranges;
}


void print_ranges(range_list* ranges)
{
  for (size_t r = 0; r < ranges->num_ranges; r++)
  {
    range* range = get_range(ranges, r);
    printf("%ld-%ld\n", range->low, range->high);
  }
}
