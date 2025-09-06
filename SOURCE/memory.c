#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void setup_memory() {
	memcpy(&memory_g[0] + FONT_START, &FONT_DATA[0], FONT_END-FONT_START+1);
}

void setup_registers() {
	PC_g = PROGRAM_START;
}

void load_program(const char * program_loc) {
	FILE * file_ptr;
	file_ptr = fopen(program_loc, "rb");
	if(file_ptr == NULL){
		fprintf(stderr, "ERROR: %s does not exist!\nTerminating Program\n", program_loc);
		exit(1);
	}

	//May be better way of handling this. Investigate later!
	fseek(file_ptr, 0L, SEEK_END);
	int file_size = ftell(file_ptr);
	rewind(file_ptr);


	fread(&memory_g[PROGRAM_START], 1, file_size, file_ptr);

}

uint16_t wordat(int address) {
	return (uint16_t)memory_g[address] << 8 | (uint16_t)memory_g[address+1] ;
}
