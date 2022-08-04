#ifndef _RTC_H
#define _RTC_H

// Library for the DS1302 RTC module with 3wire SPI communication

#include <stdint.h>

struct Time_HM {
	uint8_t hour;
	uint8_t minutes;
};

void RTC_init(void);

// Function that sends the time to the RTC module, parameters are hour and minutes, seconds are set to 0 on call
// If hour not in 0-23 or minutes not in 0-59 request will be ignored
void RTC_setTime(uint32_t, uint32_t);

// Function that gets the current time from the RTC and returns it on the struct, only hour and minutes are worked with for now
struct Time_HM RTC_getTime(void);

#endif
