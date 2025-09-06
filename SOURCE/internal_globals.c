#include <stdint.h>
#include "globals.h"

cell_t screen_g [DISP_SIZE];
uint8_t memory_g [MEM_SIZE];

uint16_t stack_g [STACK_SIZE];
int stack_top_g;

uint16_t IR_g;
uint16_t PC_g;

uint8_t V0_g;
uint8_t V1_g;
uint8_t V2_g;
uint8_t V3_g;
uint8_t V4_g;
uint8_t V5_g;
uint8_t V6_g;
uint8_t V7_g;
uint8_t V8_g;
uint8_t V9_g;
uint8_t VA_g;
uint8_t VB_g;
uint8_t VC_g;
uint8_t VD_g;
uint8_t VE_g;
uint8_t VF_g;

uint8_t DT_g;
uint8_t ST_g;
