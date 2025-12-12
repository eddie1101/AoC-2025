#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int num_digits(long num);
int is_id_invalid(long id);

void test_is_id_invalid(long id)
{
  int invalid = is_id_invalid(id);

  printf("%ld is Invalid (Not composed of only sequences): %d\n", id, invalid);
}

int main()
{
  FILE* input = fopen("input", "r");

  size_t line_size = 128;
  char* line = (char*) malloc(line_size);

  long sum = 0;
  int read_size = 0;
  while ((read_size = getdelim(&line, &line_size, ',', input)) != -1)
  {
    
    char* range_min_ch = strtok(line, "-");
    char* range_max_ch = strtok(NULL, "-");

    long range_min = atol(range_min_ch);
    long range_max = atol(range_max_ch);

    for (long i = range_min; i <= range_max; i++)
    {
      if (is_id_invalid(i))
      {
        sum += i;
      }
    }
  } 

  fclose(input);
  free(line);

  printf("The total of all the invalid IDs is %ld\n", sum);
  return 0;
  
}

int num_digits(long num)
{
  int num_digits = 0;
  while(num != 0)
  {
    num /= 10;
    num_digits += 1;
  }
  return num_digits;
}

void fill(char* to_fill, size_t size, char filler)
{
  for(size_t i = 0; i < size; i++)
  {
    to_fill[i] = filler;
  }
}

int is_id_invalid(long id)
{
  int digits = num_digits(id);

  char* id_string = (char*) malloc(digits + 1);
  sprintf(id_string, "%ld", id);

  const int buf_size = 16;
  char* sequence = (char*) malloc(buf_size);
  char* compare = (char*) malloc(buf_size);

  fill(sequence, buf_size, '\0');
  fill(sequence, buf_size, '\0');

  int invalid = 0;
  for (int sequence_size = 1; sequence_size <= digits / 2; sequence_size++)
  {
    if (digits % sequence_size == 0)
    {
      int num_sequences = digits / sequence_size;
      int num_successful_compares = 0;
      memcpy(sequence, id_string, sequence_size);
      for (int id_idx = 0; id_idx < digits; id_idx+=sequence_size)
      {
        memcpy(compare, id_string + id_idx, sequence_size);
        int sequence_val = atoi(sequence);
        int compare_val = atoi(compare);
        if (sequence_val == compare_val)
        {
          num_successful_compares += 1;
        }
      }

      if (num_successful_compares == num_sequences)
      {
        invalid = 1;
        break;
      }
      
      fill(sequence, buf_size, '\0');
      fill(compare, buf_size, '\0');
    }
  }

  free(sequence);
  free(compare);
  free(id_string);

  return invalid;

}
