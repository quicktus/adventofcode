//  https://adventofcode.com/2021/day/11

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>  


# define ROW_COUNT 10
# define COL_COUNT 10
# define SIM_DURATION 100
# define ANIMATE 1

static void flash(int map[ROW_COUNT][COL_COUNT], int row, int col){
	//set the cell value to 100 to signify it has been flashed and should not be flashed a second time in this step.
	map[row][col] = 100; 
	// increase the energy level of all 8 surrounding octopuses by 1.
	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
			if(i != 0 || j != 0){ //skip the middle
				if(row+i != -1 && col+j != -1 && row+i != ROW_COUNT && col+j != COL_COUNT){
					map[row+i][col+j]++;					
				}
			}
		}
	}
	// if this raised their individual energy level above 9, it also needs to flash.
	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
			if(i != 0 || j != 0){ //skip the middle
				if(row+i != -1 && col+j != -1 && row+i != ROW_COUNT && col+j != COL_COUNT && 
				map[row+i][col+j] > 9 && map[row+i][col+j] < 100){ //if it is >=100, its flash has been simulated already
					flash(map, row+i, col+j);
				}
			}
		}
	}	
}

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

	int totalFlashes = 0;
	int octomap[ROW_COUNT][COL_COUNT];


	// read the starting values from the input file and fill in the octomap
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
			octomap[row][col] = (int)c - '0';
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

	if(ANIMATE){
		// clear the console
		if(printf("%*s", 10000, "") < 0){
			perror("printf");
			exit(EXIT_FAILURE);
		}
		if(printf("\x1b[H") < 0){
			perror("printf");
			exit(EXIT_FAILURE);
		}
	}

	// simulate
	for(int i = 0; i < SIM_DURATION; i++){
		// First, the energy level of each octopus increases by 1
		for(int row = 0; row < ROW_COUNT; row++){
			for(int col = 0; col < COL_COUNT; col++){
				octomap[row][col]++;
			}
		}

		// Then, any octopus with an energy level higher than 9 flashes.
		for(int row = 0; row < ROW_COUNT; row++){
			for(int col = 0; col < COL_COUNT; col++){
				if(octomap[row][col] > 9 && octomap[row][col] < 100){ //if it is >=100, its flash has been simulated already
					flash(octomap, row, col);
				}
			}
		}

		// Count flashes and reset the energy levels (cell value)
		for(int row = 0; row < ROW_COUNT; row++){
			for(int col = 0; col < COL_COUNT; col++){
				if(octomap[row][col] > 9){
					octomap[row][col] = 0;
					totalFlashes++;
				}
			}
		}
	
		if(ANIMATE){
			// print the current state
			for(int row = 0; row < ROW_COUNT; row++){
				for(int col = 0; col < COL_COUNT; col++){
					if(printf("%c", "#*'       "[octomap[row][col]]) < 0){
						perror("printf");
						exit(EXIT_FAILURE);
					}
				}
				if(printf("%s", "\n") < 0){
					perror("printf");
					exit(EXIT_FAILURE);
				}
			}
			if(printf("step:%5i\n\n", i+1) < 0){
				perror("printf");
				exit(EXIT_FAILURE);
			}
			if(fflush(stdout) == EOF){
				perror("fflush");
				exit(EXIT_FAILURE);
			}
			// wait
			int ms = 75;
			struct timespec ts;
			ts.tv_sec = ms / 1000;
			ts.tv_nsec = (ms % 1000) * 1000000;
			int res; 
			do {
				if( (res = nanosleep(&ts, &ts)) ){
					perror("nanosleep");
				}
			} while (res && errno == EINTR);
			// bring cursor back
			if(i != SIM_DURATION-1){
				if(printf("\x1b[H") < 0){
					perror("printf");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
 
    if(printf("The total number of flashes after %i steps is %i.\n", SIM_DURATION, totalFlashes) < 0){
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}