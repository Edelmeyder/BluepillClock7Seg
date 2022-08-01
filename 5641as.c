#include "5641as.h"

#include <stdint.h>

const static uint32_t numbers[] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, // 0 1 2 3 4
	0x6D, 0x7D, 0x07, 0x7F, 0x6F 	// 5 6 7 8 9
};

static uint32_t display[4] = {0, 0, 0, 0}, i = 0;

void SEVSEG_init() {
	display[0] = 0;
	display[1] = 1;
	display[2] = 2;
	display[3] = 3;
	
	
	RCC->APB2ENR |= SEVSEG_CLOCK; // Enables port A clock
	SEVSEG_PORT->CRL = 0x33333333; // Sets pins A 0-11 as outputs 
	SEVSEG_PORT->CRH = (SEVSEG_PORT->CRH & 0xFFFF3333) | 0x00003333;
}

void SEVSEG_set(uint32_t p, uint32_t n) {
	display[p%4] = n % 10;
}

void SEVSEG_update() {
	SEVSEG_PORT->ODR = ((0xF & ~(1<<i))<<8) | numbers[display[i]];
	i = (i + 1) % 4;
}
