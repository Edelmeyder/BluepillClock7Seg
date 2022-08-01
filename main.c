#include "stm32f10x.h"                  // Device header
#include "seos.h"

int main(void) {
	SEOS_init();
	while(1){
		SEOS_dispatch();
		SEOS_sleep();
	}
}
