#include "seos.h"
#include "stm32f10x.h"
#include "5641as.h"

void SEOS_schedule(void);

static uint32_t ledCounter = 0, ledFlag = 0;

void SysTick_Handler(void) {
	SEOS_schedule();
}

void SEOS_init(void){
	if(SysTick_Config(SystemCoreClock / 1000)) {
		//ERROR HANDLING
	}
	SEVSEG_init();
	RCC->APB2ENR |= 0xFC; //Enable GPIO ports clocks
	GPIOC->CRH = 0x44344444; //PC13 as output
}

void SEOS_schedule(){
	if(++ledCounter == LEDTICKS) {
		ledCounter = 0;
		ledFlag = 1;
	}
}

void SEOS_dispatch(void){
	if(ledFlag) {
		ledFlag = 0;
		GPIOC->ODR ^= (1<<13);
		SEVSEG_update();
	}
}
void SEOS_sleep(void){
	__WFE();
}
