#ifndef __USART__
#define __USART__
#include <inavr.h>
#include <iom16.h>
#define F_OSC 4000000
#define BAUD 19200
#define BAUD_RATE (F_OSC/16/BAUD - 1)
#define LSBF 0
#define MSBF 1


void USART_initialize(unsigned short int baud_rate);
void USART_transmit(unsigned char data);
unsigned char USART_Receive( void );
unsigned int crc16Char(unsigned int polinom16, unsigned int init_val_16,unsigned char* adr_start,unsigned int len, int i);
unsigned int crc16wtable(unsigned int init_val_16,unsigned int adr_start, unsigned int len);
unsigned int lungimeNumar(unsigned int i);
void hexadecimalTransmit (unsigned int p1, unsigned int p2, void * p3);
void characterTransmit (unsigned int p1, unsigned int p2, void * p3);
#endif