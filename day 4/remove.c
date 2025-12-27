#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADJACENT_THRESHOLD 4

int remove_rolls(char* rolls);
int can_reach(char* rolls, size_t r, size_t c);
int has_roll(char* roll, size_t r, size_t c);

size_t num_lines = 0;
size_t num_chars = 0;

int main(int argc, char** argv)
{

  //Cmdline argument handling
  char* filename = "input";

  if (argc > 1)
  {
    filename = argv[1];
  }

  //File input handling
  FILE* input = fopen(filename, "r");

  if (!input)
  {
    printf("Could not open file %s: ", filename);
    perror("");
    return -1;
  }
  
  fseek(input, 0L, SEEK_END);
  size_t filesize = ftell(input);
  rewind(input);

  char* rolls = (char*) malloc(filesize);
  char* line = NULL;
  size_t len = 0;
  ssize_t readlen = 0;
  while ((readlen = getline(&line, &len, input)) > -1)
  {
    if (readlen > 0) num_chars = readlen - 1;
    memcpy(rolls + (num_lines * num_chars), line, num_chars);
    num_lines += 1;
  }

  printf("There are %ld total spaces in the input.\n", num_lines * num_chars);

  long total = 0;
  long removed = -1;
  while (removed != 0)
  {
    removed = remove_rolls(rolls);
    total += removed;
    printf("Removed %ld rolls.\n", removed);
  }

  printf("Can reach %d rolls by iteratively removing them.\n", total);
  
  return 0;
}

int remove_rolls(char* rolls)
{
  int sum = 0;
  for (int i = 0; i < num_lines; i++)
  {
    for (int n = 0; n < num_chars; n++)
    {
      size_t idx = i * num_chars + n;
      if (rolls[idx] == '@')
      {
        sum += can_reach(rolls, i, n);
      }
    }
  }
  return sum;
}

int can_reach(char* rolls, size_t r, size_t c)
{
   int num_rolls = 0;
   for (int i = -1; i < 2; i++)
   {
     for (int n = -1; n < 2; n++)
     {
       if (i != 0 || n != 0)
       {
         num_rolls += has_roll(rolls, r + i, c + n);
       }
     }
   }
   int reached = num_rolls < ADJACENT_THRESHOLD;
   if (reached)
   {
     rolls[r * num_lines + c] = 'x';
   }
   return reached;
}

int has_roll(char* rolls, size_t r, size_t c)
{
  if (r < 0 || r >= num_lines) // outside lateral edge
  {
    return 0;
  }

  if (c < 0 || c >= num_chars) // outside side edge
  {
    return 0;
  }

  return rolls[r * num_chars + c] == '@';
  
}
