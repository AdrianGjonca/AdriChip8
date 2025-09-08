#include <SDL2/SDL.h>
#include "renderer.h"

#include <stdio.h>
#include <time.h>
#include "globals.h"
#include "util.h"
#include "keys.h"

clock_t next_tick = 0;
clock_t next_9inst = 0;

// 1 2 3 C
// 4 5 6 D
// 7 8 9 E
// A 0 B F
//
// 1 2 3 4
// q w e r
// a s d f
// z x c v


void onEvent(SDL_Event event) {
	if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		SDL_KeyboardEvent kevent = event.key;
		SDL_Keycode keycode = kevent.keysym.sym;
		int setval = event.type == SDL_KEYDOWN;

		switch(keycode) {
			case '1': key1_g = setval; break;
			case '2': key2_g = setval; break;
			case '3': key3_g = setval; break;
			case '4': keyC_g = setval; break;

			case 'q': key4_g = setval; break;
			case 'w': key5_g = setval; break;
			case 'e': key6_g = setval; break;
			case 'r': keyD_g = setval; break;

			case 'a': key7_g = setval; break;
			case 's': key8_g = setval; break;
			case 'd': key9_g = setval; break;
			case 'f': keyE_g = setval; break;

			case 'z': keyA_g = setval; break;
			case 'x': key0_g = setval; break;
			case 'c': keyB_g = setval; break;
			case 'v': keyF_g = setval; break;
		}
	}
}

void onFrame() {
	while(next_tick <= clock()) {
		if(DT_g>0) DT_g -= 1;
		if(ST_g>0) ST_g -= 1;
		next_tick = clock() + (CLOCKS_PER_SEC/60);
	}

	while(next_9inst <= clock()) {
		cpu_cycle();
		next_9inst = clock() + (CLOCKS_PER_SEC/500);
	}

	speaker_g = ST_g > 0;
	//printf("%d %d\n",(int) ST_g, speaker_g);

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

	printf("Initializing system registers.\n");
	setup_registers();

	//print_mem();

	//checkerboard();

	printf("Initializing SDL2 environment.\n");
	renderer_init();
	renderer_loop(onEvent,onFrame);
	
	return 0;
}
