#ifndef __USART__
#define __USART__

#include <inavr.h>
#include <iom16.h>

#define F_OSC 4000000 // frecventa oscilatorului intern (4 MHz)
#define BAUD 19200 // rata baud
#define BAUD_RATE (F_OSC/16/BAUD - 1) // rata baud care trebuie scrisa in registrul UBRR, conform formulei

void USART_initialize(unsigned short int baud_rate);
void USART_transmit(unsigned char data);
unsigned char USART_Receive(void);

//#pragma vector = USART_RXC_vect
//__interrupt void interrupt_routine_USART_RXC(void);

#endif