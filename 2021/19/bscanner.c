//  https://adventofcode.com/2021/day/19

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN_MAX 32
#define DISTINCT_BEACONS_MAX 800
#define BEACONS_PER_SCANNER_MAX 28
#define ALIGNMENT_CERTAINTY_THRESHOLD 12

static const short rots[24][3][3] = {
    // north
    {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
    {{0, 0, 1}, {0, 1, 0}, {-1, 0, 0}},
    {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}},
    {{0, 0, -1}, {0, 1, 0}, {1, 0, 0}},
    // east
    {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}},
    {{0, 1, 0}, {0, 0, -1}, {-1, 0, 0}},
    {{0, 1, 0}, {1, 0, 0}, {0, 0, -1}},
    {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}},
    // south
    {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
    {{0, 0, -1}, {0, -1, 0}, {-1, 0, 0}},
    {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},
    {{0, 0, 1}, {0, -1, 0}, {1, 0, 0}},
    // west
    {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}},
    {{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}},
    {{0, -1, 0}, {-1, 0, 0}, {0, 0, -1}},
    {{0, -1, 0}, {0, 0, -1}, {1, 0, 0}},
    // up
    {{1, 0, 0}, {0, 0, -1}, {0, 1, 0}},
    {{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},
    {{-1, 0, 0}, {0, 0, 1}, {0, 1, 0}},
    {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},
    // down
    {{1, 0, 0}, {0, 0, 1}, {0, -1, 0}},
    {{0, 0, 1}, {-1, 0, 0}, {0, -1, 0}},
    {{-1, 0, 0}, {0, 0, -1}, {0, -1, 0}},
    {{0, 0, 1}, {1, 0, 0}, {0, -1, 0}}};

static void rotate(int *x, int *y, int *z, int rotidx) {
  int r[3][3];
  memcpy(r, rots[rotidx], (3 * 3 * sizeof(int)));
  // apply rotation by switching values between the axes into a temporary vector
  int v[3];
  v[0] = *x * r[0][0] + *y * r[0][1] + *z * r[0][2];
  v[1] = *x * r[1][0] + *y * r[1][1] + *z * r[1][2];
  v[2] = *x * r[2][0] + *y * r[2][1] + *z * r[2][2];
  // write the rotated values
  *x = v[0];
  *y = v[1];
  *z = v[2];
}

typedef struct beacon beacon;

struct beacon {
  int coords[3]; // beacon coordinates
  int in1;       // index of closest neighbour
  int in2;       // index of second-cosest neighbour
  int dn1;       // squared distance to closest neighbour
  int dn2;       // squared distance to second-cosest neighbour
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
      bs[idx].coords[axis] = c;
      tok = strtok(NULL, ",");
    }
    idx++;
  }

  // find and set the distances for the two closest neighbours
  for (int i = 0; i < idx; i++) {
    bs[i].dn1 = INT_MAX;
    bs[i].dn2 = INT_MAX;
    for (int j = 0; j < idx; j++) {
      int xdist = bs[i].coords[0] - bs[j].coords[0];
      int ydist = bs[i].coords[1] - bs[j].coords[1];
      int zdist = bs[i].coords[2] - bs[j].coords[2];
      int dist = (xdist * xdist) + (ydist * ydist) + (zdist * zdist);
      if (dist != 0 && dist < bs[i].dn1) {
        bs[i].dn2 = bs[i].dn1;
        bs[i].in2 = bs[i].in1;
        bs[i].dn1 = dist;
        bs[i].in1 = j;
      } else if (dist != 0 && dist < bs[i].dn2) {
        bs[i].dn2 = dist;
        bs[i].in2 = j;
      }
    }
  }

  // add the rest of the scanners
  while (1) {
    int scannerc = 0;
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
        tempbs[tempidx].coords[axis] = c;

        tok = strtok(NULL, ",");
      }
      tempidx++;
      // prevent going out of bounds
      if (tempidx >= BEACONS_PER_SCANNER_MAX) {
        if (fprintf(stderr, "%s\n", "BEACONS PER SCANNER MAX is too low.") <
            0) {
          perror("fprintf");
        }
        exit(EXIT_FAILURE);
      }
    }

    // find and set the distances for the two closest neighbours
    for (int i = 0; i < tempidx; i++) {
      tempbs[i].dn1 = INT_MAX;
      tempbs[i].dn2 = INT_MAX;
      for (int j = 0; j < tempidx; j++) {
        int xdist = tempbs[i].coords[0] - tempbs[j].coords[0];
        int ydist = tempbs[i].coords[1] - tempbs[j].coords[1];
        int zdist = tempbs[i].coords[2] - tempbs[j].coords[2];
        int dist = (xdist * xdist) + (ydist * ydist) + (zdist * zdist);
        if (dist != 0 && dist < tempbs[i].dn1) {
          tempbs[i].dn2 = tempbs[i].dn1;
          tempbs[i].in2 = tempbs[i].in1;
          tempbs[i].dn1 = dist;
          tempbs[i].in1 = j;
        } else if (dist != 0 && dist < tempbs[i].dn2) {
          tempbs[i].dn2 = dist;
          tempbs[i].in2 = j;
        }
      }
    }

    // find a beacon with matching neighbour distances from scanner 0
    int matchbs = -1;
    int matchtempbs = -1;
    for (int i = 0; i < idx; i++) {
      for (int j = 0; j < tempidx; j++) {
        printf("%6i %6i | %6i %6i\n", bs[i].dn1, tempbs[j].dn1, bs[i].dn2,
               tempbs[j].dn2);
        if (bs[i].dn1 == tempbs[j].dn1 && bs[i].dn2 == tempbs[j].dn2) {
          // found one
          matchbs = i;
          matchtempbs = j;
          // end loop
          i = INT_MAX;
          j = INT_MAX;
        }
      }
    }

    if (matchbs == -1) {
      if (fprintf(stderr, "%s\n",
                  "no beacon with matching neighbour distances was found") <
          0) {
        perror("fprintf");
      }
      exit(EXIT_FAILURE);
    }

    // find the rotation and coordinate offset
    int rotidx;
    int os[3];
    for (rotidx = 0; rotidx <= 24; rotidx++) {
      // rotate current beacon
      int cbpos[3];
      for (int axis = 0; axis < 3; axis++) {
        cbpos[axis] = tempbs[matchtempbs].coords[axis];
      }
      rotate(&cbpos[0], &cbpos[1], &cbpos[2], rotidx);
      // rotate n1
      int n1pos[3];
      for (int axis = 0; axis < 3; axis++) {
        n1pos[axis] = tempbs[tempbs[matchtempbs].in1].coords[axis];
      }
      rotate(&n1pos[0], &n1pos[1], &n1pos[2], rotidx);
      // rotate n2
      int n2pos[3];
      for (int axis = 0; axis < 3; axis++) {
        n2pos[axis] = tempbs[tempbs[matchtempbs].in2].coords[axis];
      }
      rotate(&n2pos[0], &n2pos[1], &n2pos[2], rotidx);

      // calculate coordinate offset
      os[0] = bs[matchbs].coords[0] - cbpos[0];
      os[1] = bs[matchbs].coords[1] - cbpos[1];
      os[2] = bs[matchbs].coords[2] - cbpos[2];

      // apply coordinate offset
      for (int axis = 0; axis < 3; axis++) {
        n1pos[axis] += os[axis];
        n2pos[axis] += os[axis];
      }

      // test alignment of neighbours
      int success = 1;
      for (int axis = 0; axis < 3; axis++) {
        if (n1pos[axis] != bs[bs[matchbs].in1].coords[axis] ||
            n2pos[axis] != bs[bs[matchbs].in2].coords[axis]) {
          success = 0;
        }
      }
      //  in case of success: do full alignment test
      if (success) {
        beacon newbs[tempidx];
        int newidx = 0;
        int alignmentc = 0;
        for (int j = 0; j < tempidx; j++) {
          if ((tempidx - j) < (ALIGNMENT_CERTAINTY_THRESHOLD - alignmentc)) {
            // even if all remaining beacons have matches, the total won't be
            // high enough to pass the test - stop evaluating
            break;
          }
          // add offset
          int pos[3];
          for (int axis = 0; axis < 3; axis++) {
            pos[axis] = tempbs[j].coords[axis] + os[axis];
          }
          for (int i = 0; i < idx; i++) {
            // see if the offset beacon has a match
            if (bs[i].coords[0] == pos[0] && //
                bs[i].coords[1] == pos[1] && //
                bs[i].coords[2] == pos[2]) {
              // the beacon matches
              alignmentc++;
              // skip the rest of the evaluations
              break;
            } else if (i == idx - 1) {
              // there are no matches. add it to the new beacons list.
              for (int axis = 0; axis < 3; axis++) {
                newbs[newidx].coords[axis] = pos[axis];
              }
              newidx++;
            }
          }
        }
        if (alignmentc >= ALIGNMENT_CERTAINTY_THRESHOLD) {
          // correct rotation and coordinate offset found
          // add all previously unknown beacons to the array
          for (int i = 0; i < newidx; i++) {
            bs[idx] = newbs[i];
            idx++;
            // prevent going out of bounds
            if (idx == DISTINCT_BEACONS_MAX) {
              if (fprintf(stderr, "%s\n", "DISTINCT BEACONS MAX is too low.") <
                  0) {
                perror("fprintf");
              }
              exit(EXIT_FAILURE);
            }
          }
          // calculate new neighbours
          // theoretically all neighbours must be recalculated here
          for (int i = idx - newidx; i < idx; i++) {
            bs[i].dn1 = INT_MAX;
            bs[i].dn2 = INT_MAX;
            for (int j = 0; j < idx; j++) {
              int xdist = bs[i].coords[0] - bs[j].coords[0];
              int ydist = bs[i].coords[1] - bs[j].coords[1];
              int zdist = bs[i].coords[2] - bs[j].coords[2];
              int dist = (xdist * xdist) + (ydist * ydist) + (zdist * zdist);
              if (dist != 0 && dist < bs[i].dn1) {
                bs[i].dn2 = bs[i].dn1;
                bs[i].in2 = bs[i].in1;
                bs[i].dn1 = dist;
                bs[i].in1 = j;
              } else if (dist != 0 && dist < bs[i].dn2) {
                bs[i].dn2 = dist;
                bs[i].in2 = j;
              }
            }
          }
          // no need to check other rotations - stop evaluation
          break;
        }
      }
    }

    scannerc++;
    if (printf("%2i scanners aligned.\r", scannerc) < 0) {
      perror("fprintf");
    }
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

  if (printf("\ntotal amount of beacons: %i\n", idx) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}