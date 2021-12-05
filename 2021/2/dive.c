//  https://adventofcode.com/2021/day/2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
		if (fprintf(stderr, "%s\n", "provide an input file") < 0){
            perror("fprintf");
        }
        exit(EXIT_FAILURE);
	}

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int horizontalpos = 0;
    int depth = 0;
    char line[16];

    while(fgets(line, 16, fp) != NULL){
        // tokenize
        char *direction, *distance;
		if ((direction = strtok(line, " ")) == NULL){
            perror("strtok");
            exit(EXIT_FAILURE);
        }
		if ((distance = strtok(NULL, " ")) == NULL){
            perror("strtok");
            exit(EXIT_FAILURE);
        }
        
        // distance string to int
        int dist;
        long ldist;
        errno = 0;
        ldist = strtol(distance, NULL, 10);
        if(ldist > INT_MAX || ldist < INT_MIN){
            if (fprintf(stderr, "%s\n", "typecast failed, value out of range") < 0){
                perror("printf");
            }
            exit(EXIT_FAILURE);
        }
        if(errno != 0){
            perror("strtol");
            exit(EXIT_FAILURE);
        }
        dist = ldist;
        // adjust counters
        if(!strcmp(direction, "forward")){
			horizontalpos += dist;
		}	
        else if(!strcmp(direction, "up")){
			depth -= dist;
		}
        else if(!strcmp(direction, "down")){
			depth += dist;
		}
        // invalid direction
        else if (fprintf(stderr, "invalid direction: %s\n", direction) < 0){
			perror("printf");
		}
    }

    if (ferror(fp)){
		perror("fgets");
	}

    // multiply 
    int res = horizontalpos * depth;

    if(printf("%-20s %i \n%-20s %i \n%-20s %i \n", "horizontal position:", horizontalpos, "depth position:", depth, "result (multiplied):", res) < 0)
    {
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}