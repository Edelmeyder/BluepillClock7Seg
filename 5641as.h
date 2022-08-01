#ifndef _5641AS_H
#define _5641AS_H

#include "stm32f10x.h"
#include <stdint.h>

#define SEVSEG_PORT GPIOA
#define SEVSEG_CLOCK RCC_APB2ENR_IOPAEN

void SEVSEG_init(void);
void SEVSEG_set(uint32_t, uint32_t);
void SEVSEG_update(void);

#endif
