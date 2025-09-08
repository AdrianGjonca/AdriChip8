#include "globals.h"
#include "keys.h"
#include <stdio.h>
#include <stdlib.h>

int shift_g = 0;
int jumpquirk_g = 0;
int adjustIR_g = 0;
int amigaquirk_g = 0;

int isWaiting_g = 0;
int waitingFor_g = 0;

#define DEBUG 0

static uint8_t value(int reg);
static void set(int reg, uint8_t val);
void draw(int nib2, int nib3, int nib4) {
	uint8_t output = 0;
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

		output |= ppu_setpx(x+0,y+i,_0);
		output |= ppu_setpx(x+1,y+i,_1);
		output |= ppu_setpx(x+2,y+i,_2);
		output |= ppu_setpx(x+3,y+i,_3);
		output |= ppu_setpx(x+4,y+i,_4);
		output |= ppu_setpx(x+5,y+i,_5);
		output |= ppu_setpx(x+6,y+i,_6);
		output |= ppu_setpx(x+7,y+i,_7);
	}
	
	//if(output) printf("COLLISION \n");

	set(0xF, output);
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
	if(isWaiting_g) {
		switch(waitingFor_g) {
			case 0x0: if(key0_g) isWaiting_g = 0; break;
			case 0x1: if(key1_g) isWaiting_g = 0; break;
			case 0x2: if(key2_g) isWaiting_g = 0; break;
			case 0x3: if(key3_g) isWaiting_g = 0; break;

			case 0x4: if(key4_g) isWaiting_g = 0; break;
			case 0x5: if(key5_g) isWaiting_g = 0; break;
			case 0x6: if(key6_g) isWaiting_g = 0; break;
			case 0x7: if(key7_g) isWaiting_g = 0; break;
					  
			case 0x8: if(key8_g) isWaiting_g = 0; break;
			case 0x9: if(key9_g) isWaiting_g = 0; break;
			case 0xA: if(keyA_g) isWaiting_g = 0; break;
			case 0xB: if(keyB_g) isWaiting_g = 0; break;

			case 0xC: if(keyC_g) isWaiting_g = 0; break;
			case 0xD: if(keyD_g) isWaiting_g = 0; break;
			case 0xE: if(keyE_g) isWaiting_g = 0; break;
			case 0xF: if(keyF_g) isWaiting_g = 0; break;
		}
		if(isWaiting_g) {
#if DEBUG
			printf("WAITING FOR KEY %01X\n", waitingFor_g);
#endif
			return;
		}else{
#if DEBUG
			printf("Key %01X has been recieved! Resuming execution.\n", waitingFor_g);
#endif
		}
	}

	//FETCH
	uint16_t operation = wordat(PC_g);
	PC_g += 2;

#if DEBUG
	printf("%03X : %04X\n", PC_g-2, operation);
	printf(
		"V0:%02X V1:%02X V2:%02X V3:%02X V4:%02X V5:%02X V6:%02X V7:%02X\n"
		"V8:%02X V9:%02X VA:%02X VB:%02X VC:%02X VD:%02X VE:%02X VF:%02X\n",
		V0_g, V1_g, V2_g, V3_g, V4_g, V5_g, V6_g, V7_g,
		V8_g, V9_g, VA_g, VB_g, VC_g, VD_g, VE_g, VF_g
	);

	printf("StackTop : %d\n", stack_top_g);
#endif

	//DECODE & EXECUTE
        int nib1 = (0xF000 & operation) >> 12;
	int nib2 = (0x0F00 & operation) >> 8;
	int nib3 = (0x00F0 & operation) >> 4;
	int nib4 = (0x000F & operation);

	uint8_t carry_val;

#if DEBUG
	printf("%01X %01X %01X %01X\n", nib1, nib2, nib3, nib4);
#endif

	int addr = 0x0FFF & operation;
	uint8_t nn = 0x00FF & operation;

	switch(nib1){
		case 0:
			if(nn == 0xE0) { //CLEAR
				ppu_clear();
#if DEBUG
				printf("Clear\n");
#endif
			}else if(nn == 0x00EE) { //RETURN
				PC_g = stack_g[stack_top_g];
				stack_top_g--;
#if DEBUG
				printf("Return\n");
#endif
			}else{ //MACHINE ROUTINE
				//TODO potentially something here?
			}
			break;	
		case 1: //JUMP NNN
			PC_g = addr;
#if DEBUG
			printf("Jumping to %03X\n", PC_g);
#endif
			break;
		case 2: //CALL NNN
			stack_g[stack_top_g+1] = PC_g;
			stack_top_g++;
			PC_g = addr;
#if DEBUG
			printf("Calling to %03X\n", PC_g);
#endif
			break;
		case 3: //SKIPIF VX == NN
#if DEBUG
			printf("Skip if V%01X == %02X\n", nib2, nn);
#endif
			if(value(nib2) == nn) {
				PC_g += 2;
#if DEBUG
				printf("Skipped\n");
#endif
			}
			break;
		case 4: //SKIPIF VX != NN
#if DEBUG
			printf("Skip if V%01X != %02X\n", nib2, nn);
#endif
			if(value(nib2) != nn) {
				PC_g += 2;
#if DEBUG
				printf("Skipped\n");
#endif
			}
			break;
		case 5: //SKIPIF VX == XY
#if DEBUG
			printf("Skip if V%01X == V%01X\n", nib2, nib3);
#endif
			if(value(nib2) == value(nib3)) {
				PC_g += 2;
#if DEBUG
				printf("Skipped\n");
#endif
			}
			break;
		case 6: //VX = NN
#if DEBUG
			printf("Set V%01X to %02X\n", nib2, nn);
#endif
			set(nib2, nn);
			break;
		case 7: //VX += NN
#if DEBUG
			printf("Add to V%01X value %02X\n", nib2, nn);
#endif
			set(nib2, value(nib2) + nn); //we discard the carry!!
			break;
		case 8: 
			switch(nib4) {
				case 0: //VX = VY
#if DEBUG
					printf("Set V%01X to V%01X\n", nib2, nib3);
#endif
					set(nib2, value(nib3));
					break;
				case 1: //VX |= VY
#if DEBUG
					printf("V%01X |= V%01X\n", nib2, nib3);
#endif
					set(nib2, value(nib2) | value(nib3));
					VF_g = 0;
					break;
				case 2: //VX &= VY
#if DEBUG
					printf("V%01X &= V%01X\n", nib2, nib3);
#endif
					set(nib2, value(nib2) & value(nib3));
					VF_g = 0;
					break;
				case 3: //VX ^= VY
#if DEBUG
					printf("V%01X ^= V%01X\n", nib2, nib3);
#endif
					set(nib2, value(nib2) ^ value(nib3));
					VF_g = 0;
					break;
				case 4: //VX += VY
#if DEBUG
					printf("V%01X += V%01X\n", nib2, nib3);
#endif
					int sum = (int) value(nib2) + (int) value(nib3);
					if(sum > 255) carry_val = 1;
					else carry_val = 0;
					set(nib2, (uint8_t)sum); //Intentional truncation
					VF_g = carry_val;
					break;
				case 5: //VX = VX - VY
#if DEBUG
					printf("V%01X -= V%01X\n", nib2, nib3);
#endif
					carry_val = value(nib3) <= value(nib2);
					set(nib2, value(nib2) - value(nib3));
					VF_g = carry_val;
					break;
				case 6: //RSHIFT 
					if(shift_g == 0) {
#if DEBUG
						printf("V%01X = V%01X >> 1\n", nib2, nib3);
#endif
						set(nib2, value(nib3));
					} else {
#if DEBUG
						printf("V%01X = V%01X >> 1\n", nib2, nib2);
#endif
					}
					carry_val = value(nib2) & 0x01;
					set(nib2, value(nib2) >> 1);
					VF_g = carry_val;
					break;				
				case 7: //VX = VY - VX
#if DEBUG
					printf("V%01X = V%01X - V%01X\n", nib2, nib3, nib2);
#endif
					set(nib2, value(nib3) - value(nib2));
					VF_g = value(nib3) > value(nib2);
					break;
				case 0xE://LSHIFT 
					if(shift_g == 0) {
#if DEBUG
						printf("V%01X = V%01X << 1\n", nib2, nib3);
#endif
						set(nib2, value(nib3));
					} else {
#if DEBUG
						printf("V%01X = V%01X << 1\n", nib2, nib2);
#endif
					}
					int shift = (int)value(nib2) << 1;
					carry_val = (shift & 0xFF00) != 0;
					set(nib2, shift);
					VF_g = carry_val;
					break;
			}
			break;
		case 9: //SKIPIF VX != XY
#if DEBUG
			printf("Skip if V%01X != V%01X\n", nib2, nib3);
#endif
			if(value(nib2) != value(nib3))
				PC_g += 2;
			break;

		case 0xA: //IR = addr
#if DEBUG
			printf("IR = %03X\n", addr);
#endif
			IR_g = addr;
			break;
		case 0xB: // PC = V0 + addr (or V0 + nn + VX)
			if(jumpquirk_g) { 
#if DEBUG
				printf("Jumping to V0 + %02X + V%01X\n", nn, nib2);
#endif
				PC_g = V0_g + nn + value(nib2);
			} else {
#if DEBUG
				printf("Jumping to V0 + %03X\n", addr);
#endif
				PC_g = V0_g + addr;
			}
			break;
		case 0xC: // VX = NN & rand()
#if DEBUG
			printf("V%01X = %02X & rand()\n", nib2, nn);
#endif
			set(nib2, nn & rand());
			break;
		case 0xD: // Draw(VY, VY, nib4, IR)
#if DEBUG
			printf("DRAW\n");
#endif
			draw(nib2, nib3, nib4);
			break;
		case 0xE:
			int is_pressed = 0;
			switch(value(nib2)) {
				case 0x0: is_pressed = key0_g; break;
				case 0x1: is_pressed = key1_g; break;
				case 0x2: is_pressed = key2_g; break;
				case 0x3: is_pressed = key3_g; break;

				case 0x4: is_pressed = key4_g; break;
				case 0x5: is_pressed = key5_g; break;
				case 0x6: is_pressed = key6_g; break;
				case 0x7: is_pressed = key7_g; break;
					  
				case 0x8: is_pressed = key8_g; break;
				case 0x9: is_pressed = key9_g; break;
				case 0xA: is_pressed = keyA_g; break;
				case 0xB: is_pressed = keyB_g; break;

				case 0xC: is_pressed = keyC_g; break;
				case 0xD: is_pressed = keyD_g; break;
				case 0xE: is_pressed = keyE_g; break;
				case 0xF: is_pressed = keyF_g; break;
			}

			if(nn == 0x9E) {
				if(is_pressed)
					PC_g += 2;
			}else if(nn = 0xA1) {
				if(!is_pressed)
					PC_g += 2;
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
						if(amigaquirk_g) VF_g = 1;
					}else {
						if(amigaquirk_g) VF_g = 0;
					}
					break;
				case 0x0A:
					waitingFor_g = value(nib2);
					isWaiting_g = 1;
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
					int i;
					for(i = 0; i <= nib2; i++, IR_g++) {
						memory_g[IR_g] = value(i);
					}
					if(adjustIR_g) IR_g-=i;
					break;
				case 0x65:
					int j;
					for(j = 0; j <= nib2; j++, IR_g++) {
						set(j, memory_g[IR_g]);
					}
					if(adjustIR_g) IR_g-=j;
					break;
			}
			break;
	}
#if DEBUG
	printf("\n");
#endif
}


