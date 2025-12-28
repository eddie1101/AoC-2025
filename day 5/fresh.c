#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ecl.h>

#define BUF_SIZE 128
#define MAX_FILENAME 128

int main(int argc, char** argv)
{
  char* filename = "input";
  if (argc > 1)
  {
    filename = malloc(MAX_FILENAME * sizeof(char));
    strncpy(filename, argv[1], MAX_FILENAME);
  }

  FILE* input = fopen(filename, "r");
  if (!input)
  {
    const char* error_fmt = "Could not open file \'%s\'";
    char* error_message = (char*) malloc(strnlen(error_fmt, 25) + strnlen(filename, MAX_FILENAME) * sizeof(char));
    sprintf(error_message, error_fmt, filename);
    perror(error_message);
    return -1;
  }

  range_list* ranges = read_ranges_file(input);
  print_ranges(ranges);
  
  char* line = (char*) malloc(BUF_SIZE * sizeof(char));
  size_t to_read = BUF_SIZE;
  ssize_t read_status = 0;  
  long total = 0;
  while ((read_status = getline(&line, &to_read, input)) > -1)
  {
    long id = atol(line);
    int is_fresh = 0;  
    for (size_t i = 0; i < ranges->num_ranges; i++)
    {
      range* r = get_range(ranges, i);
      if (id >= r->low && id <= r->high)
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
