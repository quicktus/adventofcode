#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
		fprintf(stderr, "provide an input file\n");
		exit(EXIT_FAILURE);
	}

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
        }

    int counter = 0;
    int curr = 0;
    int prev = INT_MAX;
    char line[8];

    while(fgets(line, 8, fp) != NULL){
        // to number
        errno = 0;
        curr = strtol(line, NULL, 10);
        if(curr > INT_MAX || curr < INT_MIN){
            perror("typecast fails, value out of range");
            exit(EXIT_FAILURE);
        }
        if(errno != 0){
            perror("strtol");
            exit(EXIT_FAILURE);
        }
        // increment if greater that prev
        if(curr > prev){counter++;}
        // keep curr as prev
        prev = curr;
    }
    if(printf("Count: %i\n", counter) < 0)
    {
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}