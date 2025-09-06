#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

#define KiB 1024
#define MEM_SIZE (4 * KiB)
#define STACK_SIZE 32

//TODO SUPER-CHIP later on
#define DISP_W 64
#define DISP_H 32
#define DISP_SIZE (DISP_W * DISP_H)

#define FONT_START 0x50
#define FONT_END 0x9F
#define PROGRAM_START 0x200

typedef int cell_t;
#define CELL_ON 1
#define CELL_OFF 0

//EXTERNS: internal_globals.c
extern cell_t screen_g [DISP_SIZE];
extern uint8_t memory_g [MEM_SIZE];
extern uint16_t stack_g [STACK_SIZE];
extern int stack_top_g;
extern uint16_t IR_g;
extern uint16_t PC_g;
extern uint8_t V0_g;
extern uint8_t V1_g;
extern uint8_t V2_g;
extern uint8_t V3_g;
extern uint8_t V4_g;
extern uint8_t V5_g;
extern uint8_t V6_g;
extern uint8_t V7_g;
extern uint8_t V8_g;
extern uint8_t V9_g;
extern uint8_t VA_g;
extern uint8_t VB_g;
extern uint8_t VC_g;
extern uint8_t VD_g;
extern uint8_t VE_g;
extern uint8_t VF_g;
extern uint8_t DT_g;
extern uint8_t ST_g;

//EXTERNS: font.c
extern const uint8_t FONT_DATA [];

//EXTERNS: memory.c
extern void setup_memory();
extern void load_program();

//EXTERNS: ppu.c
extern void ppu_update();
extern void ppu_setpx(int x, int y, cell_t cell);
#endif
