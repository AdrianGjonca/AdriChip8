#include <SDL2/SDL.h>
#include "renderer.h"

#include <stdio.h>
#include "globals.h"
#include "util.h"

void onEvent(SDL_Event event) {

}

void onFrame() {
	ppu_update();
}

int main(int argc, char * argv[]) {
	if(argc == 1) {
		fprintf(stderr, "Error: What should I run!\nTerminating Program\n");
		return 1;
	}
	if(argc > 2) {
		fprintf(stderr, "Error: Too many arguments!\nTerminating Program\n");
		return 1;
	}

	printf("Initializing system memory.\n");
	setup_memory();
	load_program(argv[1]);
	//print_mem();

	checkerboard();

	printf("Initializing SDL2 environment.\n");
	renderer_init();
	renderer_loop(onEvent,onFrame);
	
	return 0;
}
