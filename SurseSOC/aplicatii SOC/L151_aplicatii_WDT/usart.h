#ifndef __USART__
#define __USART__

#define F_OSC 4000000
#define BAUD 19200
#define BAUD_RATE (F_OSC/16/BAUD - 1) //L79

void USART_initialize(unsigned short int baud_rate);
void USART_transmit(unsigned char data);
int USART_receive(char *d);
void transmit_ulong(unsigned long x);

#endif