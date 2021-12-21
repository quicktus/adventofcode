//  https://adventofcode.com/2021/day/19

#include <limits.h>
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
  const int DISTINCT_BEACONS_MAX = 800;
  const int BEACONS_PER_SCANNER_MAX = 28;

  // beacon coordinates (xyz) and fingerprint, which consists of:
  // squared distance, min squared offset, max squared offset
  int beacon[DISTINCT_BEACONS_MAX][6];
  int tempBeacon[BEACONS_PER_SCANNER_MAX][6];
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
    if (coordChars[1] == '-') {
      continue;
    }
    // calculate the fingerprint
    beacon[idx][3] = 0;       // squared distance
    beacon[idx][4] = INT_MAX; // min squared offset
    beacon[idx][5] = 0;       // max squared offset
    // read the three axes
    char *tok = strtok(coordChars, ",");
    for (int axis = 0; axis < 3; axis++) {
      // convert to int
      int c = strtol(tok, NULL, 10);
      beacon[idx][axis] = c;
      // square
      c = c * c;
      // add to the squared distance
      beacon[idx][3] += c;
      // set min squared offset
      if (c < beacon[idx][4]) {
        beacon[idx][4] = c;
      }
      // set max squared offset
      if (c > beacon[idx][4]) {
        beacon[idx][4] = c;
      }
      tok = strtok(NULL, ",");
    }
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
      // skip the scanner name divider
      if (coordChars[1] == '-') {
        continue;
      }
      // calculate the fingerprint
      tempBeacon[tempidx][3] = 0;       // squared distance
      tempBeacon[tempidx][4] = INT_MAX; // min squared offset
      tempBeacon[tempidx][5] = 0;       // max squared offset
      // read the three axes
      char *tok = strtok(coordChars, ",");
      for (int axis = 0; axis < 3; axis++) {
        // convert to int
        int c = strtol(tok, NULL, 10);
        tempBeacon[tempidx][axis] = c;
        // square
        c = c * c;
        // add to the squared distance
        tempBeacon[tempidx][3] += c;
        // set min squared offset
        if (c < tempBeacon[tempidx][4]) {
          tempBeacon[tempidx][4] = c;
        }
        // set max squared offset
        if (c > tempBeacon[tempidx][4]) {
          tempBeacon[tempidx][4] = c;
        }
        tok = strtok(NULL, ",");
      }
      tempidx++;
      // prevent going out of bounds
      if (tempidx >= BEACONS_PER_SCANNER_MAX) {
        if (fprintf(stderr, "%s\n", "BEACONS_PER_SCANNER_MAX too low.") < 0) {
          perror("fprintf");
        }
        exit(EXIT_FAILURE);
      }
    }

    
    /* // find its sd relative to scanner 0
    int delta;
    int matches;
    int bestDelta;
    int bestMatches = 12;

    for (int i = 0; i < idx; i++) {
      for (int j = 0; j < tempidx; j++) {
        // calculate the offset
        delta = beacon[i] - tempBeacon[j];
        // see if the offset tempBeacons matches 12 or more of the beacons
        matches = 0;
        for (int k = 0; k < tempidx; k++) {
          if ((tempidx - 1 - k) >= (bestMatches - matches)) {
            // even if all other beacons matched, the total won't be higher
            break;
          }
          for (int l = 0; l < idx; l++) {
            if (beacon[l] == (tempBeacon[k] + delta)) {
              matches++;
            }
          }
        }
        if (matches > bestMatches) {
          bestMatches = matches;
          bestDelta = delta;
        }
      }
    }

    // append the newly discovered beacons
    for (int j = 0; j < tempidx; j++) {
      // apply the offset
      tempBeacon[j] += bestDelta;
      int match = 0;
      for (int i; i < idx; i++) {
        // for each beacon in beacon
        // see if it matches the current beacon
        if (beacon[i] == tempBeacon[j]) {
          match = 1;
        }
      }
      if (!match) {
        // beacon is not a known beacon - add it
        beacon[idx] = tempBeacon[j];
        idx++;
        // prevent going out of bounds
        if (idx == DISTINCT_BEACONS_MAX) {
          if (fprintf(stderr, "%s\n", "DISTINCT_BEACONS_MAX too low.") < 0) {
            perror("fprintf");
          }
          exit(EXIT_FAILURE);
        }
      }
    } */

    // check if there are no more scanners left to read
    if (feof(fp)) {
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

  if (printf("total amount of beacons: %i\n", idx) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}