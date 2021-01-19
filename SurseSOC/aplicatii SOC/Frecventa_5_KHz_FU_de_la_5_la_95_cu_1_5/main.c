#include "inavr.h"
#include "iom16.h"
#include "usart.h"

unsigned int no_overflows;
unsigned char flag;
#pragma vector=TIMER1_OVF_vect
__interrupt void T1int(void)
{
  no_overflows++;
  if (no_overflows == 5000)
  {
    flag = 1;
    no_overflows = 0;
  }
}

void timer1_init()
{
  TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);  
  
  TIMSK |= (1 << TOIE1);
  DDRD |= (1 << PD5);
}
void main(void)
{
  unsigned int DCx10 = 50;
  unsigned char directie = 'S';
  unsigned int parte_zecimala;
  unsigned long front;
  timer1_init();
  ICR1 = 799;
  front = ICR1 * DCx10 / 1000;
  OCR1A = (unsigned int) front;
  USART_initialize(BAUD_RATE);
  __enable_interrupt();
  while(1)
  {
    if (flag)
    {
      flag = 0;
      if (directie == 'S')
      {
        if (DCx10 + 15 <= 950)
          DCx10 += 15;
        else
        {
          directie = 'J';
          DCx10 -= 15;
        }
      }
      else
      {
        if ( DCx10 - 15 >= 50)
          DCx10 -= 15;
        else
        {
          directie = 'S';
          DCx10 += 15;
        }
      }
      front = (unsigned long)ICR1 * DCx10 / 1000;
      OCR1A = (unsigned int) front;
      USART_transmit('F');
      USART_transmit('U');
      USART_transmit('=');
      transmit_uint(DCx10 / 10);
      parte_zecimala = DCx10 % 10;
      if (parte_zecimala)
      {
        USART_transmit('.');  
        transmit_uint(parte_zecimala);
      }
      USART_transmit('%');  
      USART_transmit('\r');  
      USART_transmit('\n');  
    }
  }
}