//  https://adventofcode.com/2021/day/19

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

  const int LINE_LEN_MAX = 32;
  const int DISTINCT_BEACONS_MAX = 700;
  const int BEACONS_PER_SCANNER_MAX = 30;

  int beaconCoords[DISTINCT_BEACONS_MAX][3];
  int tempBeaconCoords[BEACONS_PER_SCANNER_MAX][3];
  int beaconCount = 0;
  int idx = 0;
  char *coordChars;

  if ((coordChars = malloc(LINE_LEN_MAX)) == NULL) {
    perror("malloc");
  }

  // read in scanner 0
  while (fgets(coordChars, LINE_LEN_MAX, fp) != NULL) {
    // stop at blank line
    if (coordChars[0] == '\r') {
      break;
    }
    // skip scanner name divider
    if (strncmp(coordChars, "---", 3) == 0) {
      continue;
    }
    // read the three values
    char *tok = strtok(coordChars, ",");
    for (int axis = 0; axis < 3; axis++) {
      beaconCoords[idx][axis] = strtol(tok, NULL, 10);
      printf("%i ", beaconCoords[idx][axis]);
      tok = strtok(NULL, ",");
    }
    printf("\n");
    idx++;
  }

  // add the rest of the scanners
  while (1) {
    // read the next scanner
    int tempidx = 0;
    while (fgets(coordChars, LINE_LEN_MAX, fp) != NULL) {
      // stop at blank line
      if (coordChars[0] == '\r') {
        break;
      }
      // skip scanner name divider
      if (strncmp(coordChars, "---", 3) == 0) {
        continue;
      }
      char *tok = strtok(coordChars, ",");
      for (int axis = 0; axis < 3; axis++) {
        tempBeaconCoords[idx][axis] = strtol(tok, NULL, 10);
        printf("%i ", tempBeaconCoords[idx][axis]);
        tok = strtok(NULL, ",");
      }
      printf("\n");
      tempidx++;
      // prevent going out of bounds
      if (tempidx == BEACONS_PER_SCANNER_MAX + 1) {
        if (fprintf(stderr, "%s\n", "BEACONS_PER_SCANNER_MAX too low.") < 0) {
          perror("fprintf");
        }
        exit(EXIT_FAILURE);
      }
    }
    // find its position relative to scanner 0
    int delta[3];
    for (int i; i < idx; i++) {
      for (int j; j < tempidx; j++) {
        // calculate the offsets
        for (int axis = 0; axis < 3; axis++) {
          delta[axis] = beaconCoords[j][axis] - tempBeaconCoords[i][axis];
        }
        // see if the points in tempBeaconCoords match 12 or more of the
        // beaconCoords
        int matches = 0;
        for (int k; k < tempidx; k++) {
          
        }
      }
    }

    // append the newly discovered beacons
    for (int j; j < tempidx; j++) {
      // apply the offset
      for (int axis = 0; axis < 3; axis++) {
        tempBeaconCoords[i][axis] += delta[axis];
      }
      // for each beacon in beaconCoords
      int match = 0;
      for (int i; i < idx; i++) {
        // see if it matches the current beacon
        int m = 0;
        for (int axis = 0; axis < 3; axis++) {
          if (beaconCoords[i][axis] == tempBeaconCoords[i][axis]) {
            m++;
          }
        }
        if (m == 3) {
          // match found
          match = 1;
        }
      }
      if (!match) {
        // beacon is not in a known beacon - add it
        for (int axis = 0; axis < 3; axis++) {
          beaconCoords[idx][axis] == tempBeaconCoords[i][axis]
        }
        idx++;
        // prevent going out of bounds
        if (idx == DISTINCT_BEACONS_MAX + 1) {
          if (fprintf(stderr, "%s\n", "DISTINCT_BEACONS_MAX too low.") < 0) {
            perror("fprintf");
          }
          exit(EXIT_FAILURE);
        }
      }
    }

    // check if there are no more scanners left to read
    if (feof(fp)) {
      printf("idx: %i  tempidx: %i", idx, tempidx);
      break;
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
  free(coordChars);

  if (printf("total amount of beacons: %i\n", beaconCount) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
