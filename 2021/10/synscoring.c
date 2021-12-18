//  https://adventofcode.com/2021/day/10

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


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

  int totalScore = 0;
  int lineCorrupted = 0;
  char *expected = malloc(100);
  int idx = -1;

  int c = 0;
  while ((c = fgetc(fp)) != EOF) {
    // if the line is found to be corrupted, skip to the next line
    if (lineCorrupted && !iscntrl(c)) {
      continue;
    }

    // if there is an opening bracket, add the matching closing bracket to the
    // array
    if (c == '<') {
      idx++;
      expected[idx] = '>';
    } else if (c == '{') {
      idx++;
      expected[idx] = '}';
    } else if (c == '[') {
      idx++;
      expected[idx] = ']';
    } else if (c == '(') {
      idx++;
      expected[idx] = ')';
    }

    // if theres a closing bracket, there must be an opening bracket.
    // the closing bracket must match the most recent unclosed opening bracket.
    // otherwhise, it's a syntax error.
    else if (c == ')') {
      if (expected[idx] != c || idx == -1) {
        lineCorrupted = 1;
        totalScore += 3;
      } else {
        idx--;
      }
    } else if (c == ']') {
      if (expected[idx] != c || idx == -1) {
        lineCorrupted = 1;
        totalScore += 57;
      } else {
        idx--;
      }
    } else if (c == '}') {
      if (expected[idx] != c || idx == -1) {
        lineCorrupted = 1;
        totalScore += 1197;
      } else {
        idx--;
      }
    } else if (c == '>') {
      if (expected[idx] != c || idx == -1) {
        lineCorrupted = 1;
        totalScore += 25137;
      } else {
        idx--;
      }
    } else if (iscntrl(c)) {
      // end of line reached, reset the index and corruption indicatior
      idx = -1;
      lineCorrupted = 0;
    } else {
      if (fprintf(stderr, "invalid input: %c\n", c) < 0) {
        perror("fprintf");
      }
    }
  }
  if (ferror(fp)) {
    perror("fgetc");
    exit(EXIT_FAILURE);
  }
  if (fclose(fp)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
  free(expected);

  if (printf("The total syntax error score for those errors is %i.\n",
             totalScore) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}