#ifndef __USART__
#define __USART__

#include<iom16.h>
#include<inavr.h>


#define F_OSC 4010400
#define BAUD 19200
#define BAUD_RATE (F_OSC/BAUD/16 - 1)

void USART_init(int baud_rate);
void USART_transmit(unsigned char c);
unsigned char USART_receive(void);
void USART_transmit_uint(unsigned int n);
void USART_transmit_ulonglong(unsigned long long n);

#endif