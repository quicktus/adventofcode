//  https://adventofcode.com/2021/day/3

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    if (argc != 2) {
		fprintf(stderr, "%s\n", "provide an input file");
		exit(EXIT_FAILURE);
	}

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
        }

    const int SIZE = 12;
    int onesCount[SIZE];
    int gammaRate = 0;
    int epsilonRate = 0;
    int lineCount = 0;
    int value = 0;
    int position = 0;


    while((value = fgetc(fp)) != EOF){
        // line break
        if(value == '\n'){
            lineCount++;
            position = 0;
            continue;
        }
        // increment counter if digit is 1
        onesCount[position] += value - 0x30; //0x30 is zero, 0x31 is one
        position++;   
    }
    if (ferror(fp)){
		perror("fgetc");
	}
    if (fclose(fp)){
		perror("fclose");
		exit(EXIT_FAILURE);
	}

    // for each digit
    for(int i = 0; i <= SIZE; i++){
        // leftshift gammaRate
        gammaRate = gammaRate << 1;
        // if the majority of values is 1, set gammaRate to one at this position
        if(onesCount[i] > (lineCount >> 1)){
            gammaRate++;
        }
    }

    // invert gammaRate to get epsilonRate
    epsilonRate = gammaRate  ^ 0xFFF;

    // multiply 
    int powerconsumption = gammaRate * epsilonRate;

    if(printf("%-17s %i \n%-17s %i \n%-17s %i \n", "gamma rate:", gammaRate, "epsilon rate:", epsilonRate, "power consumption:", powerconsumption) < 0)
    {
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}