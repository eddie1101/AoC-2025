#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum _direction
{
  RIGHT = 0,
  LEFT = 1
} direction;

int turn_dial_analytic(int, direction);
int turn_dial_brute(int, direction);

const int modulus = 100;

int dial = 50;  
int passwd = 0;

int dial_analytic = 50;
int dial_brute = 50;

int main(int argc, char** argv)
{
  char* input_filename = "input";
  if (argc > 1)
  {
    input_filename = argv[1];
  }

  FILE* input = fopen(input_filename, "r");

  if (!input)
  {
    perror("Could not open input file");
    return -1;
  }

  ssize_t read_status;
  size_t len = 5;
  char* line = (char*) malloc(5);
  char* line_no_endl = (char*) malloc(4);

  int zero_sum_analytic = 0;
  int zero_sum_brute = 0;
  
  while ((read_status = getline(&line, &len, input)) != -1)
  {
    memcpy(line_no_endl, line, 4);
    
    if (!(line_no_endl[3] <= 57 && line_no_endl[3] >= 48))
    {
      line_no_endl[3] = 32;
    }
    
    if (!(line_no_endl[2] <= 57 && line_no_endl[2] >= 48))
    {
      line_no_endl[2] = 32;
    }
    direction dir = line[0] == 'R' ? RIGHT : LEFT;
    int val = atoi(line + 1);

    int dial_analytic_before = dial_analytic;
    int dial_brute_before = dial_brute;

    int zero_counts_analytic = turn_dial_analytic(val, dir);
    int zero_counts_brute = turn_dial_brute(val, dir);

    if (zero_counts_analytic != zero_counts_brute)
    {
      printf("Given dial a(%d) and dial b(%d) and rotation %s, a counted %d and b counted %d, and a left dial at %d and b left dial at %d\n",
          dial_analytic_before, dial_brute_before, line_no_endl, zero_counts_analytic, zero_counts_brute, dial_analytic, dial_brute);
    }

    zero_sum_analytic += zero_counts_analytic;
    zero_sum_brute += zero_counts_brute;
    
  }

  // printf("%d\n", passwd);
  printf("Analytical: %d\nBrute: %d\n", zero_sum_analytic, zero_sum_brute);
  free(line);
  free(line_no_endl);

  return 0;   
}

int turn_dial_analytic(int amount, direction dir)
{
  int zero_counts = 0;
  int to_add = dir == RIGHT ? modulus - dial_analytic : dial_analytic;
  if (dial_analytic == 0)
  {
    to_add = 100;
  }

  if (to_add > amount)
  {
    if (dir == RIGHT)
    {
      dial_analytic = (dial_analytic + amount) % modulus;
    }
    else
    {
      dial_analytic = (dial_analytic - amount) % modulus;
    }
    return 0;
  }

  dial_analytic = 0;
  zero_counts += 1;
  amount -= to_add;

  int remainder = amount % modulus;
  zero_counts += amount / modulus;

  if (dir == RIGHT)
  {
    dial_analytic = remainder;
  }
  else
  {
    dial_analytic = (modulus - remainder) % modulus;
  }
  return zero_counts;
}

int turn_dial_brute(int amount, direction dir)
{
  int zero_counts = 0;
  for (int i = 0; i < amount; i++)
  {
    dial_brute = (dial_brute + (dir == RIGHT ? 1 : -1)) % modulus;
    if (dial_brute == 0) zero_counts += 1;
  }
  return zero_counts;
}
