//  https://adventofcode.com/2021/day/8

#include <stdio.h>
#include <stdlib.h>
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

    const int OUTPUT_SIGNAL_COUNT = 4;
	int total = 0, counter1 = 0, counter4 = 0, counter7 = 0, counter8 = 0;
	char *displayChars;
	
    if( (displayChars = malloc(100)) == NULL ){
        perror("malloc");
    }

	// read the ten unique signal patterns and the four output signals in each line
	// e.g. "acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf"
	while(fgets(displayChars, 100, fp) != NULL) {
		// tokenize twice by pipe to get just the second part (the output signals)	
		strtok(displayChars, "|");
		char *outputvals = strtok(NULL, "|");
		// tokenize to get the individual signals	
		char *tok;
		int i = 1;
		while(i <= 4){
			if(i == 1) {
				tok = strtok(outputvals, " ");
			} 
			else if(i == OUTPUT_SIGNAL_COUNT) {
				tok = strtok(NULL, "\r\n");
			}
			else {
				tok = strtok(NULL, " ");
			}
			
			// if strlength is either 2, 4, 3 or 7 (i.e. the digits 1, 4, 7, and 8) 
			// increment the  corrsponding counter
			int sl = strlen(tok);
			if(sl == 2) {
				counter1++;
			} 
			else if(sl == 4) {
				counter4++;
			} 
			else if(sl == 3) {
				counter7++;
			} 
			else if(sl == 7) {
				counter8++;
			}

			i++;
		}
	}
	if(!feof(fp)) {
		perror("fgets");
		exit(EXIT_FAILURE);
	}
	
	total = counter1 + counter4 + counter7 + counter8;

    if(printf("digit 1: %3i times\ndigit 4: %3i times\ndigit 7: %3i times\ndigit 8: %3i times\n\nIn the output values, the digits 1, 4, 7, and 8 appear a total of %i times.\n", counter1, counter4, counter7, counter8, total) < 0){
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}