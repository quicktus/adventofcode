//  https://adventofcode.com/2021/day/7

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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

    const int SUB_COUNT = 1000;
    int subs[SUB_COUNT]; // horizontal position of the submarines
    memset(subs, 0, SUB_COUNT*sizeof(int));

    // read values and insert them into the subs[] array in order
    int c = 0;
	int lastIndex = -1;
    int num = 0;
    while ((c = fgetc(fp)) != EOF){
        if( c == ',' ||  iscntrl(c) ){ // -> number complete
            // search subs[] from 0 to lastIndex for the first value higher than num
            int idx = 0;
            for(int i = 0; i <= lastIndex; i++){
                if(subs[i] > num){
                    idx = i;
                    break;
                }
                if(i == lastIndex){
                    // none found
                    idx = lastIndex + 1;
                }
            }
            // move all higher values to the right by 1
            for(int i = lastIndex; i >= idx; i--){   
                subs[i+1] = subs[i]; 
            }
            // insert num
            subs[idx] = num;
			// increment lastIndex
            lastIndex++;
            // reset num
            num = 0;
            // ignore all \r \n etc. at the end
            if( iscntrl(c) ){ 
                break;
            }
        } else if (!isdigit(c)){
            if (fprintf(stderr, "invalid input: %c\n", c) < 0){
                perror("fprintf");
            }
        } else {
            // convert and add c to num
            num = num * 10 + (c - '0');
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

    // find median
	int median = 0;
    if(SUB_COUNT % 2 == 0) {
			// if its an even amount of values take the mean between the two middle values
			median = (int) ( subs[(int)((SUB_COUNT-1) / 2)] + subs[(int)((SUB_COUNT-1) / 2  + 1)] ) / 2;
		} else {
			// if its an uneven amount of values take the middle value
			median = subs[(SUB_COUNT-1) / 2];
		}
    
    // calcualte total fuel consumption
    int totalFuel = 0;
    for(int i = 0; i < SUB_COUNT; i++){
        // sum of distances between sub and median
        totalFuel += (subs[i] > median) ? (subs[i] - median) : (median - subs[i]);
	}

    if(printf("The ideal position is at %i. The total fuel consumption of the swarm to get there is %i units.\n", median, totalFuel) < 0){
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}