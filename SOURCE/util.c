#include "globals.h"
#include "util.h"
#include <stdio.h>

void print_mem() {
	for(int i = 0; i<MEM_SIZE; i+=16) {
		printf("%03X :"
		       " %02X %02X %02X %02X "
		       " %02X %02X %02X %02X "
		       " %02X %02X %02X %02X "
		       " %02X %02X %02X %02X "
		       "\n",
			i, 
			memory_g[i], memory_g[i+1], memory_g[i+2], memory_g[i+3],
			memory_g[i+4], memory_g[i+5], memory_g[i+6], memory_g[i+7],
			memory_g[i+8], memory_g[i+9], memory_g[i+10], memory_g[i+11],
			memory_g[i+12], memory_g[i+13], memory_g[i+14], memory_g[i+15]
		);
	}

}

void checkerboard() {
	for(int x = 0; x<DISP_W; x++) {
		for(int y = 0; y<DISP_H; y++) {
			if((x%2)^(y%2)) {
				ppu_setpx(x,y, CELL_ON);
			}else {
				ppu_setpx(x,y, CELL_OFF);
			}
		}
	}
}
