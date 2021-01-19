#ifndef USART_H_
#define USART_H_

#define FOSC 4000000// CPU Clock speed
#define BAUD 19200//baud rate
#define MYUBRR FOSC/16/BAUD-1

enum TYPE
	{
		INT,
		LONG,
		STRING,
		FLOAT,
		UNSIGNED,
		HEX
	};
	

	void printInt(int number);
	void printLong(long number);
	void printString(unsigned char*str);
	void printFloat(float number);
	void PRINTF(void*data,enum TYPE type);
	void printUnsignedInt(unsigned int number);
	void printHex(unsigned  int data);

void USART_Init( unsigned int ubrr);
unsigned char USART_Receive( void );
void USART_Transmit( unsigned char data );
void print(unsigned long data);

#endif