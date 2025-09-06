#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

int shift_g = 0;
int jumpquirk_g = 0;

static uint8_t value(int reg);
void draw(int nib2, int nib3, int nib4) {
	int x = value(nib2) % DISP_W;
	int y = value(nib3) % DISP_H;
	
	for(int i = 0; i < nib4; i++) {
		uint8_t row = memory_g[IR_g + i];
		cell_t _0 = 128 & row ? CELL_ON : CELL_OFF;
		cell_t _1 = 64  & row ? CELL_ON : CELL_OFF;
		cell_t _2 = 32  & row ? CELL_ON : CELL_OFF;
		cell_t _3 = 16  & row ? CELL_ON : CELL_OFF;
		cell_t _4 = 8   & row ? CELL_ON : CELL_OFF;
		cell_t _5 = 4   & row ? CELL_ON : CELL_OFF;
		cell_t _6 = 2   & row ? CELL_ON : CELL_OFF;
		cell_t _7 = 1   & row ? CELL_ON : CELL_OFF;

		ppu_setpx(x+0,y+i,_0);
		ppu_setpx(x+1,y+i,_1);
		ppu_setpx(x+2,y+i,_2);
		ppu_setpx(x+3,y+i,_3);
		ppu_setpx(x+4,y+i,_4);
		ppu_setpx(x+5,y+i,_5);
		ppu_setpx(x+6,y+i,_6);
		ppu_setpx(x+7,y+i,_7);

	}


}

uint8_t value(int reg) {
	switch(reg) {
		case 0x0: return V0_g;
		case 0x1: return V1_g;
		case 0x2: return V2_g;
		case 0x3: return V3_g;
		case 0x4: return V4_g;
		case 0x5: return V5_g;
		case 0x6: return V6_g;
		case 0x7: return V7_g;
		case 0x8: return V8_g;
		case 0x9: return V9_g;
		case 0xA: return VA_g;
		case 0xB: return VB_g;
		case 0xC: return VC_g;
		case 0xD: return VD_g;
		case 0xE: return VE_g;
		case 0xF: return VF_g;
	}
}

void set(int reg, uint8_t val) {
	switch(reg) {
		case 0x0: V0_g = val; break;
		case 0x1: V1_g = val; break;
		case 0x2: V2_g = val; break;
		case 0x3: V3_g = val; break;
		case 0x4: V4_g = val; break;
		case 0x5: V5_g = val; break;
		case 0x6: V6_g = val; break;
		case 0x7: V7_g = val; break;
		case 0x8: V8_g = val; break;
		case 0x9: V9_g = val; break;
		case 0xA: VA_g = val; break;
		case 0xB: VB_g = val; break;
		case 0xC: VC_g = val; break;
		case 0xD: VD_g = val; break;
		case 0xE: VE_g = val; break;
		case 0xF: VF_g = val; break;
	}
}

void cpu_cycle() {
	//FETCH
	uint16_t operation = wordat(PC_g);
	PC_g += 2;

	//printf("%03X : %04X\n", PC_g-2, operation);

	//DECODE & EXECUTE
	int nib1 = (0xF000 & operation) >> 12;
	int nib2 = (0x0F00 & operation) >> 8;
	int nib3 = (0x00F0 & operation) >> 4;
	int nib4 = (0x000F & operation);

	//printf("%01X %01X %01X %01X\n", nib1, nib2, nib3, nib4);

	int addr = 0x0FFF & operation;
	uint8_t nn = 0x00FF & operation;

	switch(nib1){
		case 0:
			if(operation == 0x00E0) { //CLEAR
				ppu_clear(); 
			}else if(operation == 0x00EE) { //RETURN
				PC_g = stack_g[stack_top_g];
				stack_top_g--;
			}else{ //MACHINE ROUTINE
				//TODO potentially something here?
			}
			break;	
		case 1: //JUMP NNN
			PC_g = addr;
			//printf("Jumping to %03X\n", PC_g);
			break;
		case 2: //CALL NNN
			stack_g[stack_top_g+1] = PC_g;
			stack_top_g++;
			PC_g = addr;
			break;
		case 3: //SKIPIF VX = NN
			if(value(nib2) == nn) 
				PC_g += 2;
			break;
		case 4: //SKIPIF VX != NN
			if(value(nib2) != nn)
				PC_g += 2;
			break;
		case 5: //SKIPIF VX == XY
			if(value(nib2) == value(nib3))
				PC_g += 2;
			break;
		case 6: //VX = NN
			set(nib2, nn);
			break;
		case 7: //VX += NN
			set(nib2, value(nib2) + nn); //we discard the carry!!
			break;
		case 8: 
			switch(nib4) {
				case 0: //VX = VY
					set(nib2, nib3);
					break;
				case 1: //VX |= VY
					set(nib2, value(nib2) | value(nib3));
					break;
				case 2: //VX &= VY
					set(nib2, value(nib2) & value(nib3));
					break;
				case 3: //VX ^= VY
					set(nib2, value(nib2) ^ value(nib3));
					break;
				case 4: //VX += VY
					int sum = (int) value(nib2) + (int) value(nib3);
					if(sum > 255) VF_g = 1;
					else VF_g = 0;
					set(nib2, (uint8_t)sum); //Intentional truncation
					break;
				case 5: //VX = VX - VY
					set(nib2, value(nib2) - value(nib3));
					VF_g = value(nib2) > value(nib3);
					break;
				case 6: //RSHIFT 
					if(shift_g == 0)
						set(nib2, value(nib3));
					VF_g = nib2 & 0x01;
					set(nib2, value(nib2) >> 1);
					break;				
				case 7: //VX = VY - VX
					set(nib2, value(nib3) - value(nib2));
					VF_g = value(nib3) > value(nib2);
					break;
				case 0xE://LSHIFT 
					if(shift_g == 0)
						set(nib2, value(nib3));
					int shift = value(nib2) << 1;
					VF_g = shift & 0xFF00 != 0;
					set(nib2, shift);
					break;
			}
			break;
		case 9: //SKIPIF VX != XY
			if(value(nib2) != value(nib3))
				PC_g += 2;
			break;

		case 0xA: //IR = addr
			IR_g = addr;
			break;
		case 0xB: // PC = V0 + addr (or V0 + nn + VX)
			if(jumpquirk_g) 
				PC_g = V0_g + addr;
			else
				PC_g = V0_g + nn + value(nib2);
			break;
		case 0xC: // VX = NN & rand()
			set(nib2, nn & rand());
			break;
		case 0xD: // Draw(VY, VY, nib4, IR)
			draw(nib2, nib3, nib4);
			break;
		case 0xE:
			if(nn == 0x9E) {
				//TODO SKIP IF KEY
			}else if(nn = 0xA1) {
				//TODO SKIP IF NOT KEY
			}
			break;
		case 0xF:
			switch(nn) {
				case 0x07:
					set(nib2, DT_g);
					break;
				case 0x15:
					DT_g = value(nib2);
					break;
				case 0x18:
					ST_g = value(nib2);
					break;
				case 0x1E:
					IR_g += value(nib2);
					if(IR_g > 0xFF) {
						IR_g % 0x100;
						VF_g = 1;
					}else {
						VF_g = 0;
					}
					break;
				case 0x0A:
					while(1);
					//TODO WAIT FOR KEY
					break;
				case 0x29:
					IR_g = value(nib2) * 5 + FONT_START;
					break;
				case 0x33:
					memory_g[IR_g] = value(nib2) / 100;
					memory_g[IR_g+1] = value(nib2) / 10 % 10;
					memory_g[IR_g+2] = value(nib2) % 10;
					break;
				case 0x55:
					memory_g[IR_g] = value(0);
					memory_g[IR_g+1] = value(1);
					memory_g[IR_g+2] = value(2);
					memory_g[IR_g+3] = value(3);
					memory_g[IR_g+4] = value(4);
					memory_g[IR_g+5] = value(5);
					memory_g[IR_g+6] = value(6);
					memory_g[IR_g+7] = value(7);
					//TODO Old support
					break;
				case 0x65:
					set(0, memory_g[IR_g]);
					set(1, memory_g[IR_g+1]);
					set(2, memory_g[IR_g+2]);
					set(3, memory_g[IR_g+3]);
					set(4, memory_g[IR_g+4]);
					set(5, memory_g[IR_g+5]);
					set(6, memory_g[IR_g+6]);
					set(7, memory_g[IR_g+7]);
					break;
			}
			break;
	}
}


