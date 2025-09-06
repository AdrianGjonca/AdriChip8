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
		}
	}
}

void ppu_setpx(int x, int y, cell_t cell) {
	int i = x + (y * DISP_W);
	screen_g[i] = cell;
}
