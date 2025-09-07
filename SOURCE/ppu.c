#include "globals.h"
#include "renderer.h"

void ppu_update() {
	for(int i = 0; i<DISP_SIZE; i++) {
		int x = i % DISP_W;
		int y = i / DISP_W;
		cell_t cell = screen_g[i];
		switch(cell) {
			case CELL_ON:
				blot_white(x,y);
				break;
			case CELL_OFF:
				blot_black(x,y);
				break;
		}
	}
}

uint8_t ppu_setpx(int x, int y, cell_t cell) {
	uint8_t output = 0;
	if(x >= 0 && y >= 0 && x < DISP_W && y < DISP_H) {
		int i = x + (y * DISP_W);
		int to_set = cell ^ screen_g[i];
		if(cell == CELL_OFF && to_set == CELL_ON)
			output = 1;
		screen_g[i] = to_set;
	}
	return output;
}

void ppu_clear() {
	for(int i = 0; i<DISP_SIZE; i++) {
		screen_g[i] = CELL_OFF;
	}
}
