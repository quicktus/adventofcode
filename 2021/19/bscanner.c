//  https://adventofcode.com/2021/day/19

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN_MAX 32
#define DISTINCT_BEACONS_MAX 800
#define BEACONS_PER_SCANNER_MAX 27
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
  // apply rotation by switching values between the axes into a temporary vector
  // printf("%5i | %5i | %5i\n", *x, *y, *z);
  int v[3];
  v[0] = *x * rots[rotidx][0][0] + *y * rots[rotidx][0][1] +
         *z * rots[rotidx][0][2];
  v[1] = *x * rots[rotidx][1][0] + *y * rots[rotidx][1][1] +
         *z * rots[rotidx][1][2];
  v[2] = *x * rots[rotidx][2][0] + *y * rots[rotidx][2][1] +
         *z * rots[rotidx][2][2];
  // write the rotated values
  *x = v[0];
  *y = v[1];
  *z = v[2];
  // printf("%5i | %5i | %5i\n\n", *x, *y, *z);
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

  int bs[DISTINCT_BEACONS_MAX][3];
  int tempbs[BEACONS_PER_SCANNER_MAX][3];

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
      bs[idx][axis] = c;
      tok = strtok(NULL, ",");
    }
    idx++;
  }

  // add the rest of the scanners
  for (int scannerc = 0; !feof(fp); scannerc++) {
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
        tempbs[tempidx][axis] = c;

        tok = strtok(NULL, ",");
      }
      tempidx++;
      // prevent going out of bounds
      if (tempidx > BEACONS_PER_SCANNER_MAX) {
        if (fprintf(stderr, "%s\n", "BEACONS PER SCANNER MAX is too low.") <
            0) {
          perror("fprintf");
        }
        exit(EXIT_FAILURE);
      }
    }

    int isAligned = 0;
    // for each beacon in tempbs
    for (int j = 0; j < tempidx && !isAligned; j++) {
      // for each possible rotation
      for (int rotidx = 0; rotidx < 24 && !isAligned; rotidx++) {
        printf("rot: %i\n", rotidx);
        // do rotation
        int pos[3];
        for (int axis = 0; axis < 3; axis++) {
          pos[axis] = tempbs[j][axis];
        }
        rotate(&pos[0], &pos[1], &pos[2], rotidx);
        // for each beacon in bs
        for (int i = 0; i < idx && !isAligned; i++) {
          // calculate coordinate offset
          int os[3];
          for (int axis = 0; axis < 3; axis++) {
            os[axis] = bs[i][axis] - pos[axis];
          }
          // check alignment
          int alignmentc = 0;
          int newbs[tempidx][3];
          int newidx = 0;
          for (int k = 0; k < tempidx; k++) {
            if ((tempidx - k - 1) <
                (ALIGNMENT_CERTAINTY_THRESHOLD - alignmentc)) {
              // if the remaining beacons are fewer than the remaining amount
              // needed to cross the threshold - stop evaluating
              break;
            }
            // add rotation
            for (int axis = 0; axis < 3; axis++) {
              pos[axis] = tempbs[k][axis];
            }
            rotate(&pos[0], &pos[1], &pos[2], rotidx);
            // add offset
            for (int axis = 0; axis < 3; axis++) {
              pos[axis] += os[axis];
            }
            for (int m = 0; m < idx; m++) {
              // see if the transformed beacon has a match
              printf("%5i %5i | %5i %5i | %5i %5i", bs[m][0], pos[0], bs[m][1],
                     pos[1], bs[m][2], pos[2]);
              if (bs[m][0] == pos[0] && //
                  bs[m][1] == pos[1] && //
                  bs[m][2] == pos[2]) {
                // the beacon matches
                alignmentc++;
                printf("   a = %i\n", alignmentc);
                // skip the rest of the evaluations
                break;
              } else if (m == idx - 1) {
                // there were no matches. add it to the new beacons list.
                for (int axis = 0; axis < 3; axis++) {
                  newbs[newidx][axis] = pos[axis];
                }
                newidx++;
              }
              printf("\n");
            }
          }
          if (alignmentc >= ALIGNMENT_CERTAINTY_THRESHOLD) {
            // correct rotation and coordinate offset found
            isAligned = 1;
            // add all previously unknown beacons to the array
            for (int i = 0; i < newidx; i++) {
              for (int axis = 0; axis < 3; axis++) {
                bs[idx][axis] = newbs[i][axis];
              }
              idx++;
              // prevent going out of bounds
              if (idx == DISTINCT_BEACONS_MAX) {
                if (fprintf(stderr, "%s\n",
                            "DISTINCT BEACONS MAX is too low.") < 0) {
                  perror("fprintf");
                }
                exit(EXIT_FAILURE);
              }
            }
          }
        }
      }
    }

    if (!isAligned) {
      if (fprintf(stderr, "scanner %i could not be aligned\n", scannerc + 1) <
          0) {
        perror("fprintf");
      }
      //exit(EXIT_FAILURE);
    }

    if (printf("%2i scanners aligned.\r", scannerc + 1) < 0) {
      perror("fprintf");
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