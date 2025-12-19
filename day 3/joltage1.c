#include <stdio.h>
#include <stdlib.h>

int main()
{

  FILE* input = fopen("input", "r");

  size_t buf_size = 128;
  char* line = (char*) malloc(buf_size * sizeof(char));

  int linelen;
  int joltage_sum = 0;
  while ((linelen = getline(&line, &buf_size, input)) != -1)
  { 
    char max_val = 0;
    size_t max_idx;
    for (int i = 0; i < linelen; i++)
    {
      char val = line[i];
      if (val > max_val)
      {
        max_val = val;
        max_idx = i;
      }
    }

    // Account for \n
    if (max_idx == linelen - 2)
    {
      char max2 = 0;
      for (int i = 0; i < linelen - 2; i++)
      {
        char val = line[i];
        if (val > max2)
        {
          max2 = val;
        }  
      }
      max_val -= '0';
      max2 -= '0';
      joltage_sum += 10 * max2 + max_val;
    }
    else
    {
      char max2 = 0;      
      for (int i = max_idx + 1; i < linelen; i++)
      {
        char val = line[i];
        if (val > max2)
        {
          max2 = val;
        }
      }
      max_val -= '0';
      max2 -= '0';
      joltage_sum += 10 * max_val + max2;
    }
    
  }

  printf("%d\n", joltage_sum);

  free(line);
  return 0;
}
