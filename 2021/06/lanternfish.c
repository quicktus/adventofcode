//  https://adventofcode.com/2021/day/6

#include <ctype.h>
#include <errno.h>
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

  const int DAYS_TO_SIM = 80;
  int fish[7][2]; // fish count [weekday it creates on][newborn(0) or adult(1)]

  memset(fish, 0, 7 * 2 * sizeof(int));

  // fill in the fish table
  int c = 0;
  while ((c = fgetc(fp)) != EOF) {
    if (c == ',' || iscntrl(c)) {
      continue;
    }
    if (!isdigit(c)) {
      if (fprintf(stderr, "invalid input: %c\n", c) < 0) {
        perror("fprintf");
      }
      continue;
    }
    int fishDay = (int)c - '0'; // evil char to int conversion trick
    fish[fishDay % 7]
        [(fishDay < 7) ? 1 : 0]++; // increment the fish count [weekday it
                                   // creates on][newborn(0) or adult(1)]
  }
  if (ferror(fp) != 0) {
    perror("fgetc");
    exit(EXIT_FAILURE);
  }
  if (fclose(fp)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  // simulate days
  int weekday = 0;
  for (int currentDay = 0; currentDay < DAYS_TO_SIM; currentDay++) {
    // add newborns for the weekday they will create on
    fish[(weekday + 2) % 7][0] += fish[weekday][1];
    // add current weekday's newborns to the adults (so they will create next
    // cycle)
    fish[weekday][1] += fish[weekday][0];
    // reset current weekday's newborns
    fish[weekday][0] = 0;
    // increment weekday
    weekday = (weekday + 1) % 7;
  }

  // tally fish
  int totalFish = 0;
  for (int i = 0; i < 7; i++) {
    totalFish += fish[i][0] + fish[i][1];
  }

  if (printf("After %i days, there will be a total of %i lanternfish.\n",
             DAYS_TO_SIM, totalFish) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}