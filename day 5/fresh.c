#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 128

typedef struct
{
  size_t low;
  size_t high;
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
      printf("Finished parsing %ld ranges.\n\n", num_ranges);
      break;
    }
    
    char* low = strtok(line, "-");
    char* high = strtok(NULL, "-");

    ranges[num_ranges].low = atol(low);
    ranges[num_ranges].high = atol(high);

    printf("Registered range #%ld: %s-%s", num_ranges, low, high);

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

  long total = 0;
  while ((read_status = getline(&line, &to_read, input)) > -1)
  {
    long id = atol(line);
    int is_fresh = 0;  
    for (size_t i = 0; i < num_ranges; i++)
    {
      range r = ranges[i];
      if (id >= r.low && id <= r.high)
      {
        is_fresh = 1;
        break;
      }
    }
    total += is_fresh;
  }

  printf("There are %ld fresh IDs.\n", total);

  free(line);
  free(ranges);
}
