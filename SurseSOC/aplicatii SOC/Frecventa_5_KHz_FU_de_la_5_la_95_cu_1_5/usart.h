#ifndef __USART__
#define __USART__

#include <inavr.h>
#include <iom16.h>

#define F_OSC 4000000
#define BAUD 19200
#define BAUD_RATE (F_OSC/16/BAUD - 1) //L79

void USART_initialize(unsigned short int baud_rate);
void USART_transmit(unsigned char data);
int USART_receive(char *d);
void transmit_uint(unsigned int x);
void transmit_ulong(unsigned long x);
void transmit_string (unsigned char* val);
void transmit_hexa (unsigned char nr_car, unsigned int x);

#endif