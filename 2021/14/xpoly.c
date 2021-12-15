//  https://adventofcode.com/2021/day/14

#include "polyList.h"

#include <stdio.h>
#include <stdlib.h>
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

    const int STARTING_POLY_COUNT = 20;
    const int RULE_COUNT = 100;
    const int DO_INSERTIONS = 10;
    const int LINE_LEN_MAX = 10;
    
    char rules[RULE_COUNT][3]; // "CH -> B"
    char elementTypes[10] = {'B','C','F','H','K','N','O','P','S','V'};
    char *ruleChars = malloc(LINE_LEN_MAX);

    if(ruleChars == NULL){
        perror("malloc");
    }
  
    // read the polymer template (initial set)
    int c;
    for(int i = 0; i < STARTING_POLY_COUNT; i++){
        // get character
        c = fgetc(fp);
        if(c == EOF){
            break;
        }
        // check if character is valid
        if(!isalpha(c)){
            if (fprintf(stderr, "invalid input: %c\n", c) < 0){
                perror("fprintf");
            }
            exit(EXIT_FAILURE);
        }
        // insert into the list
        insertLast(c);
	}
	if (ferror(fp)){
		perror("fgetc");
		exit(EXIT_FAILURE);
	}

    // read the pair insertion rules
    for(int i = -2; i < RULE_COUNT; i++){
        // read line
        if(fgets(ruleChars, LINE_LEN_MAX, fp) == NULL){
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
         // skip blank lines
        if(i < 0){
            continue;
        }
        // check if characters are valid and insert them
        // left element
        if( !isalpha(ruleChars[0]) ){
            if (fprintf(stderr, "invalid input1: %c\n", c) < 0){
                perror("fprintf");
            }
            exit(EXIT_FAILURE);
        }
        rules[i][0] = ruleChars[0];

        // right element
        if( !isalpha(ruleChars[1]) ){
            if (fprintf(stderr, "invalid input2: %c\n", c) < 0){
                perror("fprintf");
            }
            exit(EXIT_FAILURE);
        }
        rules[i][1] = ruleChars[1];

        // element to be inserted
        if( !isalpha(ruleChars[6]) ){
            if (fprintf(stderr, "invalid input3: %c\n", c) < 0){
                perror("fprintf");
            }
            exit(EXIT_FAILURE);
        }
        rules[i][2] = ruleChars[6];
    }
    if (fclose(fp)){
		perror("fclose");
		exit(EXIT_FAILURE);
	}
    free(ruleChars);

    // Apply pair insertion steps
    for(int i = 0; i < DO_INSERTIONS; i++){
        char left = cursorReset();
        char right;
        for(right = cursorNext(); right != EOF; right = cursorNext()){
            // find the element that needs to be inserted
            char newElement;
            for(int j = 0; j < RULE_COUNT; j++){
                if(rules[j][0] == left && rules[j][1] == right){
                    // found matching rule
                    newElement = rules[j][2];
                    break;
                }
            }
            //insert the new element
            if(insertInCursorPair(newElement)){
                if (fprintf(stderr, "insertInCursorPair: insertion failed") < 0){
                    perror("fprintf");
                }
                exit(EXIT_FAILURE);
            }
            left = right;
        }
    }

    // calculate and print stats and result
    {
        int len = length();
        if(printf("%i insertion step%s completed\n%i elements in polymer\n\n", DO_INSERTIONS, (DO_INSERTIONS > 1)?"s":"", len) < 0)
        {
            perror("printf");
            exit(EXIT_FAILURE);
        }
        
        int mostCommonElementCount = 0;
        int leastCommonElementCount = len;

        // count how many occurences there are of each element type
        for(int i = 0; i < sizeof(elementTypes); i++){
            char val = elementTypes[i];
            int valCount = countValue(elementTypes[i]);
            // check if this beats the current highest/ lowest
            if(valCount > mostCommonElementCount){
                mostCommonElementCount = valCount;
            }
            else if(valCount < leastCommonElementCount && valCount > 1){
                leastCommonElementCount = valCount;
            }

            if(printf("%c: %i\n", val, valCount) < 0){
                perror("printf");
                exit(EXIT_FAILURE);
            }
        }
        clearPolyList();

        // result = take the quantity of the most common element and subtract the quantity of the least common element
        int res = mostCommonElementCount - leastCommonElementCount;

        if(printf("\nresult: %i\n", res) < 0)
        {
            perror("printf");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}