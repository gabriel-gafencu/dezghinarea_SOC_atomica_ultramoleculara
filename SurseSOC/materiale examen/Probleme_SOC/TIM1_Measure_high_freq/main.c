#include"timer1.h"
#include"timer0.h"
#include"usart.h"

extern unsigned int tim0softCount;

extern unsigned int tim1_ovf;
extern unsigned int tim1_val;

void main( void )
{
  USART_init(BAUD_RATE);
  TIMER0_init();
  TIMER1_init();
  DDRD = (1 << PD1);
  __enable_interrupt();
  
  unsigned long long aux;
  while(1)
  {
    tim1_val = TCNT1;
    if(tim0softCount == 0)
    {
      tim0softCount++;
      aux = tim1_ovf;
      aux = aux * 65535 + tim1_val;
      tim1_ovf = 0;
      USART_transmit('<');
      USART_transmit_ulonglong(aux);
      USART_transmit('>');
      TCNT1 = 0;
    }
  }
}
