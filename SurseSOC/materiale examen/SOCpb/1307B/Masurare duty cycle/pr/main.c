#include<inavr.h>
#include<intrinsics.h>
#include<iom16.h>
#include"usart.h"

long cnt,cicli1,cicli0,frecv1,frecv2,cnt1;
#pragma vector = TIMER0_OVF_vect
__interrupt void timer0_ovf_int()
{
 cnt++;
}

void main(void)
{
  char value ;
  __enable_interrupt();
  USART_initialize(BAUD_RATE);
  DDRB=0;//<<PB2);
  TIMSK |= (1<<TOIE0);// intrerupere overflow
  TCCR0 = 0; // mod normal , fara prescaler , oc0 deconectat
  while(1)
  {
    value = (PINB & (1<<PB2));
    while( (PINB & (1<<PB2)) == value )
    {
      ;
    }
    value^= 1<<PB2;
    TCCR0 |= (1<<CS00);
    while( (PINB & (1<<PB2)) == value )
    {
      ;
    }
   
    cicli1 = TCNT0;
    cnt1=cnt;
    TCNT0 = 0;
    cnt=0;
   
    value^= 1<<PB2;
    while( (PINB & (1<<PB2)) == value )
    {
      ;
    }
   
   
    cicli0 = TCNT0;
    cicli1 = (cnt1*0xFF+cicli1);
    cicli0 = (cnt*0xFF+cicli0);
    
    if(!((value&1<<PB2)==1<<PB2))
    {
      cicli1^=cicli0;
      cicli0^=cicli1;
      cicli1^=cicli0;
   }
    TCCR0 = 0;
    USART_transmit('1');
    USART_transmit(':');
    //PRINTF((void*)(&cicli1),LONG);
    USART_transmit('\r');
    USART_transmit('0');
    USART_transmit(':');
   // PRINTF((void*)(&cicli0),LONG);
    USART_transmit('\r');
    cnt=0;
    
  }
}





