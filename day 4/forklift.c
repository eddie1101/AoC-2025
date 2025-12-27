#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADJACENT_THRESHOLD 4

int can_reach(char* rolls, size_t r, size_t c, size_t num_rolls, size_t num_chars);
int has_roll(char* roll, size_t r, size_t c, size_t num_rolls, size_t num_chars);

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
  long linecount = 0;
  long num_chars;
  while ((readlen = getline(&line, &len, input)) > -1)
  {
    if (readlen > 0) num_chars = readlen - 1;
    memcpy(rolls + (linecount * num_chars), line, num_chars);
    linecount += 1;
  }

  printf("There are %ld total spaces to check.\n", linecount * num_chars);

  int sum = 0;
  for (int i = 0; i < linecount; i++)
  {
    for (int n = 0; n < num_chars; n++)
    {
      size_t idx = i * num_chars + n;
      if (rolls[idx] == '@')
      {
        sum += can_reach(rolls, i, n, linecount, num_chars);
      }
    }
  }

  printf("Can reach %d rolls.\n", sum);
  
  return 0;
}

int can_reach(char* rolls, size_t r, size_t c, size_t num_lines, size_t num_chars)
{
   int num_rolls = 0;
   for (int i = -1; i < 2; i++)
   {
     for (int n = -1; n < 2; n++)
     {
       if (i != 0 || n != 0)
       {
         num_rolls += has_roll(rolls, r + i, c + n, num_lines, num_chars);
       }
     }
   }
   return num_rolls < ADJACENT_THRESHOLD;
}

int has_roll(char* rolls, size_t r, size_t c, size_t num_lines, size_t num_chars)
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
