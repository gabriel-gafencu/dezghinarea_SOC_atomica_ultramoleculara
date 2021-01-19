#ifndef USART_H_
#define USART_H_

#define FOSC 4000000// CPU Clock speed
#define BAUD 9600//baud rate
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr);
unsigned char USART_Receive( void );
void USART_Transmit( unsigned char data );
void print(unsigned long data);

#endif