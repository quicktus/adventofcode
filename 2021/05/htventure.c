//  https://adventofcode.com/2021/day/5

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

static int strtocoordint(const char *str) {
  errno = 0;
  long num = strtol(str, NULL, 10);
  if (num > 999 || num < 0) {
    if (fprintf(stderr, "coord value out of map range [0,999]: %li\n", num) <
        0) {
      perror("fprintf");
    }
  }
  if (errno != 0) {
    perror("strtol");
    exit(EXIT_FAILURE);
  }
  int ret = num;
  return ret;
}

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

  const int SIZE = 1000;
  int map[SIZE][SIZE];
  int counter = 0;

  char *vectorString = malloc(32); // e.g. "603,145 -> 603,309"
  if (vectorString == NULL) {
    perror("malloc");
  }

  memset(map, 0, SIZE * SIZE * sizeof(int));

  // read the vectors
  while (fgets(vectorString, 32, fp) != NULL) {

    // tokenize individual numbers
    char *coords1 = strtok(vectorString, " -> ");
    char *coords2 = strtok(NULL, " -> ");

    char *x1s = strtok(coords1, ",");
    char *y1s = strtok(NULL, ",");
    char *x2s = strtok(coords2, ",");
    char *y2s = strtok(NULL, "\n");
    // printf("%s,%s -> %s,%s\n", x1s, y1s, x2s, y2s);

    // convert to valid coordinate integers
    int x1 = strtocoordint(x1s);
    int y1 = strtocoordint(y1s);
    int x2 = strtocoordint(x2s);
    int y2 = strtocoordint(y2s);
    // printf("%i,%i -> %i,%i\n", x1, y1, x2, y2);

    // ignore diagonals
    if (!(x1 == x2 || y1 == y2)) {
      continue;
    }

    // adjust map accordingly
    for (int i = MIN(x1, x2); i <= MAX(x1, x2); i++) {
      for (int j = MIN(y1, y2); j <= MAX(y1, y2); j++) {
        map[i][j]++;
      }
    }
  }
  if (!feof(fp)) {
    perror("fgets");
    exit(EXIT_FAILURE);
  }
  if (fclose(fp)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  free(vectorString);

  // count occurences of overlapping
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      if (map[x][y] > 1) {
        counter++;
      }
    }
  }

  // print map
  const int SCALE = 20;
  for (int x = 0; x < SIZE; x += SCALE) {
    for (int y = 0; y < SIZE; y += SCALE) {
      int areaCounter = 0;
      for (int i = 0; i < SCALE; i++) {
        for (int j = 0; j < SCALE; j++) {
          areaCounter += map[x + i][y + j];
        }
      }
      char c = NULL;
      if (areaCounter <= (SCALE * SCALE * 1 / 16)) {
        c = ' ';
      } else if (areaCounter <= (SCALE * SCALE * 1 / 8)) {
        c = '.';
      } else if (areaCounter <= (SCALE * SCALE * 1 / 6)) {
        c = 'o';
      } else if (areaCounter <= (SCALE * SCALE * 1 / 5)) {
        c = 'O';
      } else {
        c = '0';
      }
      if (printf("%c ", c) < 0) {
        perror("printf");
        exit(EXIT_FAILURE);
      }
    }
    if (printf("\n") < 0) {
      perror("printf");
      exit(EXIT_FAILURE);
    }
  }

  if (printf("There are %i coordinates where two or more lines overlap.\n",
             counter) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}