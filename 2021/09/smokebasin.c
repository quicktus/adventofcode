//  https://adventofcode.com/2021/day/9

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

    const int ROW_COUNT = 5;
	const int COL_COUNT = 10;

	int sumrisklvl = 0;
	int heightmap[ROW_COUNT][COL_COUNT];

	// read values from the input file and fill in the heightmap
	int c = 0;
	for(int row = 0; row < ROW_COUNT; row++){
		for(int col = 0; col < COL_COUNT; col++){
    		// get character
			c = fgetc(fp);
			// skip \r\n 
			while(iscntrl(c) && c != EOF){
				c = fgetc(fp);
			}
			// break at EOF
			if(c == EOF){
				if (fprintf(stderr, "input too short in row %i\n", row+1) < 0){
					perror("fprintf");
				}
				break;
			}
			if(!isdigit(c)){
				if (fprintf(stderr, "invalid input: %c\n", c) < 0){
					perror("fprintf");
				}
        		exit(EXIT_FAILURE);
			}
			// apply char to int conversion trick and insert into the array
			heightmap[row][col] = (int)c - '0';
		}
	}
	if (ferror(fp)){
		perror("fgetc");
		exit(EXIT_FAILURE);
	}
	if (fclose(fp)){
		perror("fclose");
		exit(EXIT_FAILURE);
	}
	
	// check each field if it is a lowpoint and calculate sumrisklvl
	for(int row = 0; row < ROW_COUNT; row++){
		for(int col = 0; col < COL_COUNT; col++){
			int val = heightmap[row][col];
			//check upper field
			if(row != 0 && val >= heightmap[row-1][col]){
				continue;
			}
			//check lower field
			if(row != ROW_COUNT-1 && val >= heightmap[row+1][col]){
				continue;
			}
			//check left field
			if(col != 0 && val >= heightmap[row][col-1]){
				continue;
			}
			//check right
			if(col != COL_COUNT-1 && val >= heightmap[row][col+1]){
				continue;
			}
			sumrisklvl += val + 1;
		}
	}

    if(printf("The sum of the risk levels of all low points on the heightmap is %i.\n", sumrisklvl) < 0){
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}