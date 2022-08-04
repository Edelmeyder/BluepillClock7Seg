#include "rtc.h"
#include "stm32f10x.h"
#include "utils.h"

void _sendData(uint8_t);
uint8_t _receiveData(void); 
void _toBCDHour(uint32_t*);
void _toBCD(uint32_t*);
void _toInt(struct Time_HM*);


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

void RTC_setTime(uint32_t hour, uint32_t minutes) {
	if (hour < 24 & minutes < 60) {
		hour = (hour % 12 == 0) ? 12 : hour % 12;
		_toBCDHour(&hour);
		GPIOB->BSRR = (1<<12); // Raises CE/SS on the RTC module
		UTILS_delay_us(5);
		_sendData(0x84); // Sends the hour write address
		_sendData((uint8_t)hour); 
		GPIOB->BRR = (1<<12); // Lowers CE/SS on the RTC module
		UTILS_delay_us(5);
		_toBCD(&minutes);
		GPIOB->BSRR = (1<<12); // Raises CE/SS on the RTC module
		UTILS_delay_us(5);
		_sendData(0x82); // Sends the minutes write address
		_sendData((uint8_t)minutes); 
		GPIOB->BRR = (1<<12); // Lowers CE/SS on the RTC module
		UTILS_delay_us(5);
	}
}

struct Time_HM RTC_getTime() {
	struct Time_HM time;
	GPIOB->BSRR = (1<<12); // Raises CE/SS on the RTC module
	UTILS_delay_us(5);
	_sendData(0x85); // Sends the hour read address
	time.hour = _receiveData();
	GPIOB->BRR = (1<<12); // Lowers CE/SS on the RTC module
	UTILS_delay_us(5);
	GPIOB->BSRR = (1<<12); // Raises CE/SS on the RTC module
	UTILS_delay_us(5);
	_sendData(0x83); // Sends the minutes read address
	time.hour = _receiveData();
	GPIOB->BRR = (1<<12); // Lowers CE/SS on the RTC module
	UTILS_delay_us(5);
	_toInt(&time);
	return time;
}

void _sendData(uint8_t data){
	SPI2->CR1 &= ~(1<<0);
	SPI2->DR = data; // Loads data to Tx
	while((SPI2->SR & (1<<1)) == 0); // Waits for transmission to end (TXE=1)
}

uint8_t _receiveData(){
	SPI2->CR1 |= (1<<0);
	SPI2->CR1 &= (uint16_t)~(1<<14); // Starts receiving data
	while((SPI2->SR & (1<<0)) == 0); // Waits for reception to end (RXNE=1)
	SPI2->CR1 |= (1<<14); // Stops receiving data
	return (uint8_t)SPI2->DR;
}

void _toBCDHour(uint32_t *h) {
	*h = (1<<7) | (*h/10<<4) | (*h%10);
}

void _toBCD(uint32_t *m) {
	*m = (*m/10<<4) | (*m%10);
}

void _toInt(struct Time_HM *t) {
	t->hour &= ~(1<<7);
	t->hour = (t->hour>>4) * 10 + (t->hour & 0x0F);
	t->minutes = (t->minutes>>4) * 10 + (t->minutes & 0x0F);
}
