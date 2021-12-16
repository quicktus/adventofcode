//  https://adventofcode.com/2021/day/15

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

    const int ROW_COUNT = 100;
    const int COL_COUNT = 100;

    int caveMap[ROW_COUNT][COL_COUNT];
    int distance[ROW_COUNT][COL_COUNT];
	short visited[ROW_COUNT][COL_COUNT];

	memset(distance, 127, ROW_COUNT*COL_COUNT*sizeof(int));
	memset(visited, 0, ROW_COUNT*COL_COUNT*sizeof(short));
	
    // read the risk levels from the input file and fill in the cavemap
	int c = 0;
	for(int row = 0; row < ROW_COUNT; row++){
		for(int col = 0; col < COL_COUNT; col++){
    		// get character
			c = fgetc(fp);
			// skip \r\n 
			while(iscntrl(c) && c != EOF){
				c = fgetc(fp);
			}
			// at EOF check if fgetc failed or if the file is too short
			if(c == EOF){
				if (ferror(fp)){
					perror("fgetc");	
				}
				else if (fprintf(stderr, "input too short in row %i\n", row+1) < 0){
					perror("fprintf");
				}
				exit(EXIT_FAILURE);
			}
			if(!isdigit(c)){
				if (fprintf(stderr, "invalid input: %c\n", c) < 0){
					perror("fprintf");
				}
        		exit(EXIT_FAILURE);
			}
			// apply char to int conversion trick and insert into the array
			caveMap[row][col] = (int)c - '0';
		}
	}
	if (fclose(fp)){
		perror("fclose");
		exit(EXIT_FAILURE);
	}

	// set the starting point to 0
	distance[0][0] = 0;

	int row, col;
	while(!visited[ROW_COUNT-1][COL_COUNT-1]){
		// set the lowest unvisited node as visited
		int lowest = INT_MAX;
		for(int i = 0; i < ROW_COUNT; i++){
			for(int j = 0; j < COL_COUNT; j++){
				if(!visited[i][j] && distance[i][j] < lowest){
					lowest = distance[i][j];
					row = i;
					col = j;
				}
			}
		}
		visited[row][col] = 1;
		// update its neighbours distance if routing via the current node lowers it
		// above
		if(row+1 < ROW_COUNT && distance[row+1][col] > (distance[row][col] + caveMap[row+1][col]) ){
			distance[row+1][col] = (distance[row][col] + caveMap[row+1][col]);
		}
		// below
		if(row-1 > -1 && distance[row-1][col] > (distance[row][col] + caveMap[row-1][col]) ){
			distance[row-1][col] = (distance[row][col] + caveMap[row-1][col]);
		}
		// right
		if(col+1 < COL_COUNT && distance[row][col+1] > (distance[row][col] + caveMap[row][col+1]) ){
			distance[row][col+1] = (distance[row][col] + caveMap[row][col+1]);
		}
		//left
		if(col-1 > -1 && distance[row][col-1] > (distance[row][col] + caveMap[row][col-1]) ){
			distance[row][col-1] = (distance[row][col] + caveMap[row][col-1]);
		}
	}

    if(printf("The lowest total risk of any path from the top left to the bottom right is %i\n", distance[ROW_COUNT-1][COL_COUNT-1]) < 0)
    {
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}
