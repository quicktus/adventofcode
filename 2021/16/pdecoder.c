//  https://adventofcode.com/2021/day/16

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRANSMISSION_LEN 1404

static int parseAndCount(const char *str, int idx){
    // end of transmission - end of recursion
	if(str[idx] == '\0' || idx > (TRANSMISSION_LEN*4 - 4) ){
		return 0;
	}
	int ret = 0;
	int typeid = 0;
	int cursor = 0;
	// get the version number
	for(; cursor < 3; cursor++){
		ret = ret << 1;
		ret += str[idx+cursor] - '0';
	}
	// get the type ID
	for(; cursor < 6; cursor++){
		typeid = typeid << 1;
		typeid += str[idx+cursor] - '0';
	}
	// move the cursor to the first bit of the next packet based on the type of the current packet 
	if(typeid == 4){
		// literal value 
		// while it is one, meaning there are subsequent groups move the cursor to the next group
		while(str[idx+cursor] == '1'){
			cursor += 5;
		}
		// skip the last group +1 bit
		cursor += 5;
	} 
	else {
		// operator packet

		// move by the remaining length of the header +1 bits
		int remainder = (str[idx+cursor] == '0') ? 15 : 11;
		cursor += remainder + 1;
	}
    // the cursor is now one bit behind the packet
	// parse the next packet for its version number and add it to the current count
	ret += parseAndCount(str, idx+cursor);
	return ret;
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

	const char BIN[16][5] = {"0000", "0001", "0010", "0011", "0100", "0101","0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110","1111"};

	int versionSum = 0;
	char binaryStr[TRANSMISSION_LEN*4 + 1];
	
    // read the transmission and convert it to a binary string
	int c = fgetc(fp);
	int idx = 0;
	while( c != EOF ){
		// make sure the characters are as expected
		if(iscntrl(c)){
			continue;
		}
		if(!isxdigit(c)){
			if (fprintf(stderr, "invalid input: %c\n", c) < 0){
				perror("fprintf");
			}
			exit(EXIT_FAILURE);
		}
		// convert hex char to int
		int i;
		if (c >= '0' && c <= '9') {
			i = c - '0';
		} else if (c >= 'A' && c <= 'F') {
			i = c - 'A' + 10;
		}
		// append the number to the binaryStr
		for(int j = 0; j < 4; j++){
			binaryStr[idx+j] = BIN[i][j];
		}
		idx += 4;
		// get next char
		c = fgetc(fp);
	}
	if (ferror(fp)){
		perror("fgetc");
		exit(EXIT_FAILURE);
	}
	if (fclose(fp)){
		perror("fclose");
		exit(EXIT_FAILURE);
	}

	// end the String on \0
	binaryStr[idx] = '\0';
	// parse the transmission
	versionSum = parseAndCount(binaryStr, 0);

    if(printf("The sum of all packet version numbers in the transmission is %i\n", versionSum) < 0)
    {
		perror("printf");
        exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}
