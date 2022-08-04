#include "utils.h"

void UTILS_delay_us(uint32_t us) {
	volatile uint16_t l, i;
	for(i = 0 ; i < us ; i++) {
		for(l = 0; l < 12 ; l++) {
		}
	}
}
