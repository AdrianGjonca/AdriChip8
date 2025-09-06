#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void setup_memory() {
	memcpy(&memory_g[0] + FONT_START, &FONT_DATA[0], FONT_END-FONT_START+1);
}

void load_program(const char * program_loc) {
	FILE * file_ptr;
	file_ptr = fopen(program_loc, "rb");
	if(file_ptr == NULL){
		printf("ERROR: %s does not exist!\nTerminating Program\n", program_loc);
		exit(1);
	}

	//May be better way of handling this. Investigate later!
	fseek(file_ptr, 0L, SEEK_END);
	int file_size = ftell(file_ptr);
	rewind(file_ptr);


	fread(&memory_g[PROGRAM_START], 1, file_size, file_ptr);

}
