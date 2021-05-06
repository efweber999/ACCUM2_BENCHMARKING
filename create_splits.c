//
// May 2021 - Gene Weber
// Generates a file named "splits" used for pre-splitting the ci table.
// Pass the number of desired splits in as the single command line argument.
// The entries in the ci table are hexidecimal numbers. This program generates a series
// of hex numbers as split points.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Error message and exit for check_input function.
void exit_message(char prog_name[]) {
  printf("\nArgument to %s must be an integer greater than 1 and less than 1,000,000\n\n", prog_name);
  exit(1);
}

// Verify the the input is an integer >1. Return integer if valid, else exit.
// Assumes this will be run on machines where int is at least 4 bytes.
int check_input(char number[], char prog_name[]) {
  int i;

  // Check that there is an argument provided.
  if (number == NULL)
    exit_message(prog_name);

  // Scan through array ensuring all characters are in the 0 - 9 range, and number not greater than 999,999.
  for (i = 0; number[i] != 0; i++) {
    if ((!isdigit(number[i])) || ( i > 5))
      exit_message(prog_name);
  }

  // number array is all 0-9 chars, so convert to int.
  i = atoi(number);

  // Check that value is >1
  if ( i > 1 )
    return i;
  else
    exit_message(prog_name);
}

int main(int argc, char *argv[]) {
  FILE *fp;
  int extra, num_splits, index;
  double fraction, wobble;
  unsigned long long int split_size, split_out;

  // Check that a valid number for splits has been provided.
  num_splits = check_input(argv[1], argv[0]);

  // Check that variable can hold 64 bits.
  if ( sizeof(split_out) < 8 ) {
    printf("\nCannot be run on a machine with an unsigned long long int smaller than 8 bytes\n");
    exit(1);
  }

  // Calculate the size of the step between splits.
  split_size = 0xFFFFFFFFFFFFFFFF / num_splits;

  // Calculate fractional remainder.
  fraction = (double)(0xFFFFFFFFFFFFFFFF % num_splits) / (double)num_splits;

  // Set up a print loop. Add an extra 1 based on multiples of the remainder fraction.
  fp = fopen("splits", "w");
  wobble = fraction;
  split_out = split_size;
  fprintf(fp, "%016llx\n", split_out);
  for (index = 2; index < num_splits; index++) {
    extra = 0;
    if (wobble >= 1) {
      extra = 1;
      wobble = (wobble - 1) + fraction;
    }
    else {
      wobble = wobble + fraction;
    }
    split_out = split_out + split_size + extra;
    fprintf(fp, "%016llx\n", split_out);
  }
  fclose(fp);

  return 0;
}
