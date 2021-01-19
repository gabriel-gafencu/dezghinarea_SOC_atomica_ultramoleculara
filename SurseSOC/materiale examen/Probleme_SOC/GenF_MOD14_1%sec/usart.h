#ifndef __USART__
#define __USART__

#include<iom16.h>
#include<inavr.h>
#include"timer1.h"

#define BAUD 19200
#define BAUD_RATE (F_OSC/BAUD/16 - 1)

void USART_init(int baud_rate);
void USART_transmit(unsigned char c);
unsigned char USART_receive(void);
void USART_transmit_uint(unsigned int n);

#endif