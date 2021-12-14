//  https://adventofcode.com/2021/day/13

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc != 2) {
		if(fprintf(stderr, "%s\n", "provide an input file") < 0){
		    perror("fprintf");
        }
        exit(EXIT_FAILURE);
	}

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    const int DOT_COUNT = 793; 
    const int FOLD_COUNT = 12;
    const int DO_FOLDS = 1;
    const int LINE_LEN_MAX = 20;
    
    int paperSize[2] = {1400, 900}; //axis (0 = x)
    short dots[DOT_COUNT][2]; // dot coorinates x, y
    short folds[FOLD_COUNT][2]; // value, axis (0 = x)
    char *coordChars= malloc(LINE_LEN_MAX);
    int finalDotCount = DOT_COUNT;

    if(coordChars == NULL){
        perror("malloc");
    }
  
    for(int i = 0; i < DOT_COUNT; i++){
        // read the dot coordinates
        if(fgets(coordChars, LINE_LEN_MAX, fp) == NULL){
            if(feof(fp)){
                if(fprintf(stderr, "%s\n", "input file incomplete") < 0){
                    perror("fprintf");
                }
            } 
            else {
                perror("fgets");
            }
            exit(EXIT_FAILURE);
        }
        // tokenize string
        char *xtok = strtok(coordChars, ",");
        char *ytok = strtok(NULL, "\r\n");
        // convert to int
        errno = 0;
        short x = strtol(xtok, NULL, 10);
        short y = strtol(ytok, NULL, 10);
        if(errno != 0){
            perror("strtol");
            exit(EXIT_FAILURE);
        }
        // assure coords are within paper range
        if(x > paperSize[0]-1 || x < 0){
            if (fprintf(stderr, "x coordinate value out of range [0,%i]: %i\n", paperSize[0]-1,x) < 0){
                perror("fprintf");
            }
        }
        if(y > paperSize[1]-1 || y < 0){
            if (fprintf(stderr, "y coordinate value out of range [0,%i]: %i\n", paperSize[1]-1,y) < 0){
                perror("fprintf");
            }
        }
        // insert dot
        dots[i][0] = x;
        dots[i][1] = y;
    }
    
    for(int i = -1; i <= FOLD_COUNT-1; i++){
        // read the fold instructions
        if(fgets(coordChars, LINE_LEN_MAX, fp) == NULL){
            if(feof(fp)){
                if(fprintf(stderr, "%s\n", "input file incomplete") < 0){
                    perror("fprintf");
                }
            } 
            else {
                perror("fgets");
            }
            exit(EXIT_FAILURE);
        }
        // skip blank line
        if(i == -1){
            continue;
        }
        // tokenize string
        char *axistok = strtok(coordChars, "fold along ");
        strtok(axistok, "=");
        char *valtok = strtok(NULL, "=");
        // convert to int
        errno = 0;
        short val = strtol(valtok, NULL, 10);
        if(errno != 0){
            perror("strtol");
            exit(EXIT_FAILURE);
        }
        // insert dot
        folds[i][0] = val;
        folds[i][1] = strncmp(axistok, "x", 1)?1:0; // x = 0; y = 1
    }
    if (fclose(fp)){
		perror("fclose");
		exit(EXIT_FAILURE);
	}
    free(coordChars);

    // fold once
    int i = 0; 
    short foldedDots[finalDotCount][2];
    int fdidx = 0;
    int foldVal = folds[i][0];
    int foldAxis = folds[i][1]; // x = 0; y = 1

    for(int j = 0; j < finalDotCount; j++){
        // for each point
        int dup = 0;
        if(dots[j][foldAxis] > foldVal ){
            // dot is on the upper half - fold it
            dots[j][foldAxis] -= (dots[j][foldAxis] - foldVal) *2;
        }
        for(int k = 0; k <= fdidx; k++){
            if(foldedDots[k][0] == dots[j][0] && foldedDots[k][1] == dots[j][1]){
                //folded value exists already
                dup = 1;
            }
        }
        //transfer to new table, unless its a duplicate
        if(!dup){
            foldedDots[fdidx][0] = dots[j][0];
            foldedDots[fdidx][1] = dots[j][1];
            fdidx++;
        }
    }
    finalDotCount = fdidx;

    for(int k = 0; k < fdidx; k++){
        printf("%i,%i\n",foldedDots[k][0], foldedDots[k][1]);  
    }
    

    if(printf("After completing %i fold instruction%s on the transparent paper, %i dots are visible.\n", DO_FOLDS, (DO_FOLDS > 1)?"s":"", finalDotCount) < 0)
    {
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}