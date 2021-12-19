//  https://adventofcode.com/2021/day/18

#include "sfnum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    if (fprintf(stderr, "%s\n", "provide an input file") < 0) {
      perror("fprintf");
    }
    exit(EXIT_FAILURE);
  }

  FILE *fp;
  if ((fp = fopen(argv[1], "r")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  const int LINE_LEN_MAX = 64;
  char *numChars;
  int magnitude;

  numChars = malloc(LINE_LEN_MAX);
  if (numChars == NULL) {
    perror("malloc");
  }

  // get the first line
  fgets(numChars, LINE_LEN_MAX, fp);
  // convert it to the first num which starts of the sum
  struct num *sumNum = newNum(numChars);

  // sequentially add all other nums
  while (fgets(numChars, LINE_LEN_MAX, fp) != NULL) {
    // convert line to num
    struct num *currentNum = newNum(numChars);
    // add current num to the sum
    sumNum = addNum(sumNum, currentNum);
  }
  if (ferror(fp)) {
    perror("fgetc");
    exit(EXIT_FAILURE);
  }
  if (fclose(fp)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
  free(numChars);

  // calculate magnitude of the final sum
  magnitude = magnitudeNum(sumNum);

  // print the results
  if (printf("final sum:\n") < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }

  printNum(sumNum);
  clearNum(sumNum);

  if (printf("\nmagnitude:\n%i\n", magnitude) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
