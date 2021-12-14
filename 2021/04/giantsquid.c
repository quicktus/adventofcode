//  https://adventofcode.com/2021/day/4

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

    int drawnNums[100];
    int board[5][5];
    int maxRounds = 100;
    int score = -1;

    char *drawnChars;
    if((drawnChars = malloc(300)) == NULL){
        perror("malloc");
    }

    // read the drawn numbers
    if(fgets(drawnChars, 300, fp) == NULL){
        if(feof(fp)){
            if(fprintf(stderr, "%s\n", "input file incomplete") < 0){
		        perror("fprintf");
            }
        } else {
            perror("fgets");
        }
        exit(EXIT_FAILURE);
    }

    // convert drawn numbers to int array
    int i = 0;
    char *tok = strtok(drawnChars, ",");
    while(tok != NULL){
        errno = 0;
        long num = strtol(tok, NULL, 10);
        if(num > 99 || num < 0){
            if (fprintf(stderr, "drawn number out of range [0,99]: %li\n", num) < 0){
                perror("fprintf");
            }
        }
        if(errno != 0){
            perror("strtol");
            exit(EXIT_FAILURE);
        }
        drawnNums[i] = num;

        i++;
        tok = strtok(NULL, ",");
    }
    free(drawnChars);

    char *rowChars;
    if((rowChars = malloc(20)) == NULL){
        perror("malloc");
    }
    int row = 0;
    int col = 0;
    while(1){
        // read row
        if(fgets(rowChars, 20, fp) == NULL){
            if(feof(fp)){
                break;
            } else {
                perror("fgets");
                exit(EXIT_FAILURE);
            }
        }
        if(row == 0){
             // skip blank lines
            if(fgets(rowChars, 20, fp) == NULL){
                if(feof(fp)){
                    break;
                } else {
                    perror("fgets");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // insert row into int matrix
        char *tok = strtok(rowChars, " ");
        while(tok != NULL){
            errno = 0;
            long num = strtol(tok, NULL, 10);
            if(num > 99 || num < 0){
                if (fprintf(stderr, "board number out of range [0,99]: %li\n", num) < 0){
                    perror("fprintf");
                }
            }
            if(errno != 0){
                perror("strtol");
                exit(EXIT_FAILURE);
            }
            board[row][col] = num;
            col++;
            tok = strtok(NULL, " ");
        }
        row++;
        col = 0;

        if(row == 5){
            // board complete, reset row index
            row = 0;

            // test board
            // set up row / col counters
            int rowScore[5] = {0};
            int colScore[5] = {0};

            // play rounds until maxRounds-1 is met i.e. a previous board is equal or better
            for(int currentRound = 1; currentRound < maxRounds; currentRound++){
                int currentNum = drawnNums[currentRound-1];

                // search for current number in board
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(board[i][j] == currentNum){
                            // found it, adjust scores
                            rowScore[i]++;
                            colScore[j]++;
                            if(rowScore[i] == 5 || colScore[j] == 5){
                                // board won!
                                // set new maxRounds
                                maxRounds = currentRound;
                                // calculate the score
                                score = 0;
                                for(int m = 0; m < 5; m++){
                                    for(int n = 0; n < 5; n++){
                                        short dup = 0;
                                        for(int o = 0; o < currentRound; o++){
                                            if(board[m][n] == drawnNums[o]){
                                                dup = 1;
                                                break;
                                            }
                                        }
                                        if(!dup){
                                            score += board[m][n];
                                        }
                                    }
                                }
                                score *= currentNum;
                            } else {
                                // skip the rest of the checks
                                i = 5; 
                                j = 5;
                            }
                        }
                    }
                }
            }
        }
    }
    if (fclose(fp)){
		perror("fclose");
		exit(EXIT_FAILURE);
	}
    free(rowChars);

    if(printf("The fastest winning board wins after %i rounds with a score of %i!\n", maxRounds, score) < 0)
    {
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}