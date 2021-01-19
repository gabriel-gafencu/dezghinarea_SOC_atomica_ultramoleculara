#ifndef __USART__
#define __USART__
#include <inavr.h>
#include <iom16.h>
#define F_OSC 4000000
#define BAUD 19200
#define BAUD_RATE (F_OSC/16/BAUD - 1)
void USART_initialize(unsigned short int baud_rate);
void USART_transmit(unsigned char data);
unsigned char USART_Receive( void );
void integerTransmit (unsigned int p1, unsigned int p2, void * p3);
#pragma vector = USART_RXC_vect
__interrupt void interrupt_routine_USART_RXC(void);
#pragma vector = USART_TXC_vect
__interrupt void interrupt_routine_USART_TXC(void);

#endif