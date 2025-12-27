#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 128

typedef struct
{
  long low;
  long high;
} range;

int main(int argc, char** argv)
{
  char* filename = "input";
  if (argc > 1)
  {
    filename = argv[1];
  }

  FILE* input = fopen(filename, "r");
  if (!input)
  {
    char* error_message;
    sprintf(error_message, "Could not open file \'%s\':", filename);
    perror(error_message);
    free(error_message);
    return -1;
  }
  
  size_t range_list_size = 128;
  size_t num_ranges = 0;
  range* ranges = (range*) malloc(range_list_size * sizeof(range));
  
  char* line = (char*) malloc(BUF_SIZE);
  size_t to_read = BUF_SIZE;
  ssize_t read_status;
  while ((read_status = getline(&line, &to_read, input)) > -1)
  {
    if (read_status == 1)
    {
      // printf("Finished parsing %ld ranges.\n\n", num_ranges);
      break;
    }
    
    char* low = strtok(line, "-");
    char* high = strtok(NULL, "-");

    ranges[num_ranges].low = atol(low);
    ranges[num_ranges].high = atol(high);

    // printf("Registered range #%ld: %s-%s", num_ranges, low, high);

    num_ranges += 1;
    if (num_ranges == range_list_size - 1)
    {
      range_list_size *= 2;
      range* temp = (range*) realloc(ranges, range_list_size * sizeof(range));
      if (!temp)
      {
        perror("Could not resize ranges array");
      }
      else
      {
        ranges = temp;
      }
    }
  }

  // The plan is to iteratively consolidate overlapping ranges until no more overlaps
  // exist, at which point the total number of IDs can be trivially calculated from the
  // remaining ranges.

  for (size_t range_idx = 0; range_idx < num_ranges; range_idx++)
  {
    for (size_t cmp = 0; cmp < num_ranges; cmp++)
    {
      long low = ranges[range_idx].low, high = ranges[range_idx].high;
      long olow = ranges[cmp].low, ohigh = ranges[cmp].high;

      if ((low == 0 && high == 0) || (olow == 0 && ohigh == 0)) continue;
      
      long new_low = low, new_high = high;
      int consume_range = 0;
      if (low >= olow && low <= ohigh)
      {
        new_low = olow;
        consume_range = 1;
      }
      if (high >= olow && high <= ohigh)
      {
        new_high = ohigh;
        consume_range = 1;
      }
      if (low <= olow && high >= ohigh)
      {
        consume_range = 1;
      }
      if (consume_range)
      {
        ranges[cmp].low = 0;
        ranges[cmp].high = 0;
        printf("Range #%ld %ld-%ld overlaps with range #%ld %ld-%ld!\n", range_idx, low, high, cmp, olow, ohigh);
        printf("Consolidating into range %ld-%ld\n\n", new_low, new_high);
      }
      ranges[range_idx].low = new_low;
      ranges[range_idx].high = new_high;
    }
  }

  int count = 0;
  for (size_t i = 0; i < num_ranges; i++)
  {
    if (ranges[i].low != 0 || ranges[i].high != 0) count += 1;
  }
  printf("Computing fresh IDs with %d remaining ranges.\n", count);

  long sum = 0;
  for (size_t i = 0; i < num_ranges; i++)
  {
    sum += ranges[i].high - ranges[i].low;
  }

  printf("There are %ld fresh IDs within the provided ranges.\n", sum);
  
  free(line);
  free(ranges);
}
