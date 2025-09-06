#include <stdio.h>
#include "globals.h"
#include "util.h"

int main(int argc, char * argv[]) {
	if(argc == 1) {
		printf("Error: What should I run!\nTerminating Program\n");
		return 1;
	}
	if(argc > 2) {
		printf("Error: Too many arguments!\nTerminating Program\n");
		return 1;
	}

	setup_memory();
	load_program(argv[1]);
	print_mem();

	return 0;
}
