//  https://adventofcode.com/2021/day/19

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_LEN_MAX 32
#define DISTINCT_BEACONS_MAX 800
#define BEACONS_PER_SCANNER_MAX 28

static const short rots[6][4][3] = {
    // north
    {
        {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
        {{0, 0, 1}, {0, 1, 0}, {-1, 0, 0}},
        {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}},
        {{0, 0, -1}, {0, 1, 0}, {1, 0, 0}},
    },
    // east
    {
        {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}},
        {{0, 1, 0}, {0, 0, -1}, {-1, 0, 0}},
        {{0, 1, 0}, {1, 0, 0}, {0, 0, -1}},
        {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}},
    },
    // south
    {
        {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
        {{0, 0, -1}, {0, -1, 0}, {-1, 0, 0}},
        {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},
        {{0, 0, 1}, {0, -1, 0}, {1, 0, 0}},
    },
    // west
    {
        {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}},
        {{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}},
        {{0, -1, 0}, {-1, 0, 0}, {0, 0, -1}},
        {{0, -1, 0}, {0, 0, -1}, {1, 0, 0}},
    },
    // up
    {
        {{1, 0, 0}, {0, 0, -1}, {0, 1, 0}},
        {{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},
        {{-1, 0, 0}, {0, 0, 1}, {0, 1, 0}},
        {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},
    },
    // down
    {{{1, 0, 0}, {0, 0, 1}, {0, -1, 0}},
     {{0, 0, 1}, {-1, 0, 0}, {0, -1, 0}},
     {{-1, 0, 0}, {0, 0, -1}, {0, -1, 0}},
     {{0, 0, 1}, {1, 0, 0}, {0, -1, 0}}}};

typedef struct beacon beacon;

struct beacon {
  int coords[3]; // beacon coordinates
  int in1;       // index of closest neighbor
  int in2;       // index of second-cosest neighbor
  int dn1;       // squared distance to closest neighbor
  int dn2;       // squared distance to second-cosest neighbor
};

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

  beacon bs[DISTINCT_BEACONS_MAX];
  beacon tempbs[BEACONS_PER_SCANNER_MAX];

  char *coordChars;
  if ((coordChars = malloc(LINE_LEN_MAX)) == NULL) {
    perror("malloc");
  }

  // read in scanner 0
  int idx = 0;
  while (fgets(coordChars, LINE_LEN_MAX, fp) != NULL) {
    // stop at blank line
    if (coordChars[0] == '\r') {
      break;
    }
    // skip scanner name divider
    if (coordChars[1] == '-') {
      continue;
    }
    // read the three axes
    char *tok = strtok(coordChars, ",");
    for (int axis = 0; axis < 3; axis++) {
      // convert to int
      int c = strtol(tok, NULL, 10);
      bs[idx]->coords[axis] = c;
      tok = strtok(NULL, ",");
    }
    idx++;
  }

  // find and set the distances for the two closest neighbours
  for (int i = 0; i < idx; i++) {
    bs[i]->dn1 = INT_MAX;
    bs[i]->dn2 = INT_MAX;
    for (int j = 0; j < idx; j++) {
      int xdist = bs[i]->coords[0] - bs[j]->coords[0];
      int ydist = bs[i]->coords[1] - bs[j]->coords[1];
      int zdist = bs[i]->coords[2] - bs[j]->coords[2];
      int dist = (xdist * xdist) + (ydist * ydist) + (zdist * zdist);
      if (dist != 0 && dist < bs[i]->dn1) {
        bs[i]->dn2 = bs[i]->dn1;
        bs[i]->in2 = bs[i]->in1;
        bs[i]->dn1 = dist;
        bs[i]->in1 = j;
      } else if (dist != 0 && dist < bs[i]->dn2) {
        bs[i]->dn2 = dist;
        bs[i]->in2 = j;
      }
    }
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
      // read the three axes
      char *tok = strtok(coordChars, ",");
      for (int axis = 0; axis < 3; axis++) {
        // convert to int
        int c = strtol(tok, NULL, 10);
        tempbs[tempidx]->coords[axis] = c;

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

    // find and set the distances for the two closest neighbours
    for (int i = 0; i < tempidx; i++) {
      tempbs[i]->dn1 = INT_MAX;
      tempbs[i]->dn2 = INT_MAX;
      for (int j = 0; j < tempidx; j++) {
        int xdist = tempbs[i]->coords[0] - tempbs[j]->coords[0];
        int ydist = tempbs[i]->coords[1] - tempbs[j]->coords[1];
        int zdist = tempbs[i]->coords[2] - tempbs[j]->coords[2];
        int dist = (xdist * xdist) + (ydist * ydist) + (zdist * zdist);
        if (dist != 0 && dist < tempbs[i]->dn1) {
          tempbs[i]->dn2 = tempbs[i]->dn1;
          tempbs[i]->in2 = tempbs[i]->in1;
          tempbs[i]->dn1 = dist;
          tempbs[i]->in1 = j;
        } else if (dist != 0 && dist < tempbs[i]->dn2) {
          tempbs[i]->dn2 = dist;
          tempbs[i]->in2 = j;
        }
      }
    }

    // find a beacon with matching neighbour distances from scanner 0
    int matchbs;
    int matchtempbs;
    for (int i = 0; i < idx; i++) {
      for (int j = 0; j < tempidx; j++) {
        if (bs[i]->dn1 == tempbs[j]->dn1 && bs[i]->dn2 == tempbs[j]->dn2) {
          // found one
          matchbs = i;
          matchtempbs = j;
          // end loop
          i = MAX_INT;
          j = MAX_INT;
        }
      }
    }

    // find the rotation and coordinate offset

    ///////////////////////////////////////////////////////

    for (int i = 0; i <= 24; i++) {
      int pos0[] = bs[matchbs]->coords;
      int pos1[] = tempbs[matchtempbs]->coords;
      int rot[] = rots[i];
      int vec[3];

      // rotate
      vec[0] = pos0[0] * rot[0][0] + vec[1] * rot[0][1] + vec[2] * rot[0][2];
      vec[1] = pos0[0] * rot[1][0] + vec[1] * rot[1][1] + vec[2] * rot[1][2];
      vec[2] = pos0[0] * rot[2][0] + vec[1] * rot[2][1] + vec[2] * rot[2][2];
      pos0 = vec;

      // move
      pos0[0] = pos1[0] - pos0[0];
      pos0[1] = pos1[1] - pos0[1];
      pos0[2] = pos1[2] - pos0[2];

      // test alignment
    }

    ///////////////////////////////////////////////////////

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
    }
    * /

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