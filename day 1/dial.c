#include <stdlib.h>
#include <stdio.h>

int main()
{
  int position = 50;  
  int count = 0;

  FILE* input = fopen("input", "r");

  ssize_t read_status;
  size_t len = 5;
  char* line = (char*) malloc(5);
  
  while ((read_status = getline(&line, &len, input)) != -1)
  {    
    const int value = atoi(line + 1);
    const int mod_value = value % 100;
    count += value / 100;
    if (line[0] == 'R') {
        position += mod_value;
        if (position > 99) {
            count++;
            position = position - 100;
        }
    }
    else {
        const int old_position = position;
        position -= mod_value;
        if (position < 0) {
            if (old_position != 0) count++;
            position = position + 100;
        }
        if (position == 0) count++;
    }    
  }

  printf("%d\n", count);
  free(line);

  return 0;   
  
}
