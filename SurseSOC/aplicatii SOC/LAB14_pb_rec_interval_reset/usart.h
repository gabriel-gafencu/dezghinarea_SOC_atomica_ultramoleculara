#ifndef __USART__
#define __USART__

#include <inavr.h>
#include <iom16.h>

#define F_OSC 4000000
#define BAUD 19200
#define BAUD_RATE (F_OSC/16/BAUD - 1) //L79

void USART_initialize(unsigned short int baud_rate);
void USART_transmit(unsigned char data);
int USART_Receive(char *d);

#endif