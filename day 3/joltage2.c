#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

long get_max_joltage(char* line, size_t linelen);
char largest_off(char* line, size_t linelen, int left, int right, int* idx);

int main()
{
  FILE* input = fopen("input", "r");

  size_t buf_size = 128;
  char* line = (char*) malloc(buf_size * sizeof(char));
  char* working_buffer = (char*) malloc(buf_size * sizeof(char));

  size_t linelen;
  long joltage_sum = 0;
  while ((linelen = getline(&line, &buf_size, input)) != -1)
  { 
    if (linelen > 0)
    {
      memcpy(working_buffer, line, linelen - 1); //Trim off \n
      joltage_sum += get_max_joltage(working_buffer, linelen - 1);
    }
  }

  printf("%ld\n", joltage_sum);

  free(line);
  free(working_buffer);
  return 0;
}

long get_max_joltage(char* line, size_t len)
{
  long max_joltage = 0;
  int batteries_remaining = 12;
  int left_idx = 0;
  
  for (; batteries_remaining > 0; batteries_remaining -= 1)
  {
    char joltage = largest_off(line, len, left_idx, batteries_remaining - 1, &left_idx);
    max_joltage += (joltage - '0') * (long) pow(10l, (long) batteries_remaining);
    batteries_remaining -= 1;
  }
  return max_joltage;
}

char largest_off(char* line, size_t len, int left, int right, int* idx)
{
  char max_val = 0;
  for (int i = left; i < len - right; i++)
  {
    char val = line[i];
    if (val > max_val)
    {
      max_val = val;
      *idx = i;
    }
  }
  return max_val;
}
