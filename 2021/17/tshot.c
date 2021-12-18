//  https://adventofcode.com/2021/day/17

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIM 0

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

  const int LINE_LEN_MAX = 45;
  char *targetChars;
  int vx = 0;  // initial velocity in x the direction (forward)
  int vy = 0;  // initial velocity in y the direction (upward)
  int dx;      // distance to the far end of the x target range
  int dy;      // distance to the far end of the y target range
  int res = 0; // highest reachable y position at the peak of the trajectory

  // get the target area values from the input file
  targetChars = malloc(LINE_LEN_MAX);
  if (targetChars == NULL) {
    perror("malloc");
  }
  fgets(targetChars, LINE_LEN_MAX, fp);
  if (ferror(fp)) {
    perror("fgetc");
    exit(EXIT_FAILURE);
  }
  if (fclose(fp)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
  // tokenize string and convert to integers
  char *cp;
  strtok(targetChars, ".");
  if ((cp = strtok(NULL, ".,")) == NULL) {
    if (fprintf(stderr, "%s\n", "input file incomplete") < 0) {
      perror("fprintf");
    }
    exit(EXIT_FAILURE);
  }
  dx = strtol(cp, NULL, 10);

  strtok(NULL, "=");
  if ((cp = strtok(NULL, ".")) == NULL) {
    if (fprintf(stderr, "%s\n", "input file incomplete") < 0) {
      perror("fprintf");
    }
    exit(EXIT_FAILURE);
  }
  dy = strtol(cp, NULL, 10);

  free(targetChars);

  // calculate vx
  for (int i = 0; (i + vx + 1) <= dx; vx++) {
    i += vx;
  }
  vx--;

  // calculate vy
  vy = -dy - 1;

  // calculate peak height
  for (int i = vy; i >= 0; i--) {
    res += i;
  }

  // simulation
  if (SIM) {
    int simvx = vx;
    int simvy = vy;
    int simx = 0;
    int simy = 0;
    for (int i = 0; i != 1;) {
      // The probe's x position increases by its x velocity.
      simx += simvx;
      // The probe's y position increases by its y velocity.
      simy += simvy;
      // The probe's x velocity changes by 1 toward the value 0
      if (simvx >= 0)
        simvx--;
      // The probe's y velocity decreases by 1.
      simvy--;

      if (simx > dx || simy < dy) {
        i = 1;
      } else {
        printf("%i,%i\n", simx, simy);
      }
    }
    printf("\n");
  }

  if (printf("highest trickshot\nvelocity x: %i\nvelocity y: %i\npeak height: "
             "%i\n",
             vx, vy, res) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
