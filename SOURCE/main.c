#include <SDL2/SDL.h>
#include "renderer.h"

#include <stdio.h>
#include <time.h>
#include "globals.h"
#include "util.h"

clock_t next = 0;

void onEvent(SDL_Event event) {

}

void onFrame() {
	cpu_cycle();
	ppu_update();
	while(next <= clock()) {
		if(DT_g>0) DT_g -= 1;
		if(ST_g>0) ST_g -= 1;
		next = clock() + (CLOCKS_PER_SEC/60);
	}
	//SDL_Delay(100);
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

	printf("Initializing system registers.\n");
	setup_registers();

	//print_mem();

	//checkerboard();

	printf("Initializing SDL2 environment.\n");
	renderer_init();
	renderer_loop(onEvent,onFrame);
	
	return 0;
}
