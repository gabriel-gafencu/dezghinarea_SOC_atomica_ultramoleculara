#include "ir_printf.h"
#include "usart.h"

#define PRECISION 5 //numarul de cifre dupa virgula
void printHex(unsigned  int data)
{
	unsigned char buffer[5];
	unsigned char i,j=0;
	
	for(i = 0;i < 5;i++)
		buffer[i] = 0;
	if(!data)
	{
		USART_Transmit('0');
		USART_Transmit('x');
		USART_Transmit('0');
		return;
	}

	for(i = 0; i < 4;i++)
	{
		buffer[j] = (data&0xFFFF)>>12;
		data<<=4;
		if(!buffer[j])
		{
			if(j)
			{				
				if(buffer[j-1])
				{
					buffer[j] = '0';
					j++;
					continue;
				}
				j--;
			}
			
			continue;
		}
		if(buffer[j]<=9)
		{
			buffer[j]+='0';
		}
		else
		{
			buffer[j]+=('A'-10);
		}
		j++;
	}
	i = 0;
	USART_Transmit('0');
	USART_Transmit('x');
	while(buffer[i])
		USART_Transmit(buffer[i++]);
	
}
void printInt(int number)
{
	unsigned char buffer[7];
	unsigned char index = 0;
	long max=10;

	for(index = 0; index < 7; index++)
		buffer[index] = 0;

	index = 0;

	if(number<0)
	{
		index = 1;
		buffer[0] = '-';
		number = -number;
	}	
	
	while(1)
	{
		if(max <= number)
		{
			max*=10;
		}
		else
			break;
	}
	max/=10;
	while(max)
	{
		buffer[index++]=(number/max)+'0';
		number%=max;
		max/=10;
	}
	
	index = 0;
	while(buffer[index])
		USART_Transmit(buffer[index++]);
}

void printLong(long number)
{
	unsigned char buffer[11];
	unsigned char index = 0;
	long max=10;

	for(index = 0; index < 11; index++)
		buffer[index] = 0;

	index = 0;

	if(number<0)
	{
		index = 1;
		buffer[0] = '-';
		number = -number;
	}	
	
	while(1)
	{
		if(max <= number)
		{
			max*=10;
		}
		else
			break;
	}
	max/=10;
	while(max)
	{
		buffer[index++]=(unsigned char)(number/max)+'0';
		number%=max;
		max/=10;
	}
	
	index = 0;
	while(buffer[index])
		USART_Transmit(buffer[index++]);
}
void printString(unsigned char*str)
{
	while(*str)
		USART_Transmit(*str++);
}
void printFloat(float number)
{
	long integerPart = (long)number;
	float realPart = number - integerPart;
	unsigned char i=0;
	printLong(integerPart);
	USART_Transmit('.');
	if(realPart < 0)
		realPart = -realPart;
	for(;i<PRECISION;i++)
	{
		integerPart = (long)(realPart*10);
		realPart*=10;
                USART_Transmit((unsigned char)(integerPart%10+'0'));
	}	
}

void printUnsignedInt(unsigned int number)
{
	unsigned char buffer[6];
	unsigned char index = 0;
	long max=10;

	for(index = 0; index < 7; index++)
		buffer[index] = 0;

	index = 0;	
	while(1)
	{
		if(max <= number)
		{
			max*=10;
		}
		else
			break;
	}
	max/=10;
	while(max)
	{
		buffer[index++]=(number/max)+'0';
		number%=max;
		max/=10;
	}
	
	index = 0;
	while(buffer[index])
		USART_Transmit(buffer[index++]);
}




void PRINTF(void*data,enum TYPE type)
{
	switch(type)
	{
	case STRING :
		printString((unsigned char*)data);
		break;
	case LONG:
		printLong(*(long*)data);
		break;
	case INT:
		printInt((int)data);
		break;
	case FLOAT:
		printFloat(*(float *)data);
		break;
	case UNSIGNED:
		printUnsignedInt((unsigned int)data);
		break;
	case HEX:
		printHex((unsigned int)data);
		break;
	default:
		break;
	}
}

/*
Manuaul de utilizare
pentru int:
int x = 12555;(2 octeti)
PRINTF((void*)(x),INT);

pentru unsigned int:
unsigned int x = 12555;(2 octeti)
PRINTF((void*)(x),UNSIGNED);

pentru long:
long x = 854441122;(4 octeti)
PRINTF((void*)(&x),LONG);

pentru float:
float x = 21547.4577;(4 octeti)
PRINTF((void*)(&x),FLOAT);

pentru char*, sir terminat cu '\0' :

char* x = "test";
PRINTF((void*)(x),STRING);

pentru hex:
unsigned int x = 345;
PRINTF((void*)(x),HEX);

Copyright Raileanu,2014
All rights reserved
*/




