#include "usart.h"

// folosim volatile pentru date partajate (explicatie in caiet)
volatile char data;
volatile int flagDataReceived;

#pragma vector = USART_RXC_vect
__interrupt void ReceiveInterruptRoutine(void)
{
  data = UDR; //are ca efect secundar resetarea flag-ului RXC
  // daca primim un caracter si nu-l citim, flagul RXC ramane setat si intreruperea este reapelata la infinit
  flagDataReceived = 1;
}

void USART_initialize(unsigned short int baud_rate)
{
  // seteaza baud rate
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  
  // activeaza transmisia si receptia la iesire
  UCSRB = (1 << RXEN) | (1 << TXEN); 
  
  // seteaza pinul RXD de intrare
  DDRD &= ~(1 << PD0);
  // seteaza pinul TXD de iesire
  DDRD |= (1 << PD1);
  
  // activeaza intruperea 
  UCSRB |= (1 << RXCIE);
}
void USART_transmit(unsigned char data)
{
  // asteapta pana ce se termina de transmis toate datele si dupa trece la urmatoarele informatii
  while (!( UCSRA & (1 << UDRE)))
  {
    ;
  }
  UDR = data;
}

// functia returneaza 0 daca s-au preluat date salvate in variabila globala 'data' si -1 in caz contrar
int USART_receive(char *d)
{
  if(flagDataReceived == 1)
  {
    *d = data;
    flagDataReceived = 0;
    return 0;
  }
  return -1;
}

void transmit_uint(unsigned int x)
{
  unsigned char nr_cifre = 0; // in caz ca x se termina cu zerouri, trebuie sa tinem minte cate cifre are
  unsigned int x_inversat = 0;
  while(x)
  {
    ++nr_cifre;
    x_inversat = x_inversat * 10 + x % 10;
    x /= 10;
  }
  while(x_inversat)
  {
    USART_transmit(x_inversat % 10 + '0');
    x_inversat /= 10;
    --nr_cifre;
  }
  while(nr_cifre--)
    USART_transmit('0');
}

void transmit_ulong(unsigned long x)
{
  unsigned char nr_cifre = 0; // in caz ca x se termina cu zerouri, trebuie sa tinem minte cate cifre are
  unsigned long x_inversat = 0;
  while(x)
  {
    ++nr_cifre;
    x_inversat = x_inversat * 10 + x % 10;
    x /= 10;
  }
  while(x_inversat)
  {
    USART_transmit(x_inversat % 10 + '0');
    x_inversat /= 10;
    --nr_cifre;
  }
  while(nr_cifre--)
    USART_transmit('0');
}



// transmiterea unui sir de caractere pe seriala
void transmit_string(unsigned char* val)
{
  unsigned char i = 0;
  while(val[i])
  {
    USART_transmit(val[i]);
    ++i;
  }
}

// transmiterea unui numar hexazecimal pe seriala
void transmit_hexa (unsigned char nr_car, unsigned int x)
{
  char aux[4];
  unsigned char index=0;
  USART_transmit('0');
  USART_transmit('x');
  while(x)
  {
    aux[index] = x & 0x0F;
    if(aux[index]<=9)
    {
      aux[index]+='0';
    }
    else
    {
      aux[index]=aux[index]+'A'-10;
    }
    index++;
    x >>= 4;
  }
  while(nr_car > index)
  {
    USART_transmit('0');
    --nr_car;
  }    
  while(nr_car>0)
  {
    if(index > 0)
    {
      USART_transmit(aux[index-1]);
      index--;
    }
    nr_car--;
  }
}
