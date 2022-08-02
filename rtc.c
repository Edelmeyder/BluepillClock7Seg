#include "rtc.h"
#include "stm32f10x.h"


void _sendData(uint8_t);
uint8_t _receiveData(void); 


void RTC_init() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enables port B clock
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // Enables SPI2 clock
	
	// Pin B15:MOSI/MISO bidirectional as alternate function - Pin B13:SCL as alternate function - Pin B12:SS output software controlled
	GPIOB->CRH = (GPIOB->CRH & 0xBFB3FFFF) | 0xB0B30000;
	GPIOB->ODR &= (uint16_t)~(1<<12); // Sets SS to low
	
	SPI2->CR1 = 0xC3EC; /*
	Bidirectional 1-line
	No CRC
	8-bit dataframe LSF
	NSS pin free
	Master mode
	Clock low on idle
	BR = 5
	SPI enabled
	*/
}

void _sendData(uint8_t data){
	SPI2->DR = data; // Loads data to Tx
	while((SPI2->SR & (1<<1)) == 0); // Waits for transmission to end (TXE=1)
}

uint8_t _receiveData(){
	SPI2->CR1 &= (uint16_t)~(1<<14); // Starts receiving data
	while((SPI2->SR & (1<<0)) == 0); // Waits for reception to end (RXNE=1)
	return (uint8_t)SPI2->DR;
}
