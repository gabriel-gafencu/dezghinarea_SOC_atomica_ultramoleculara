#include<inavr.h>
#include<intrinsics.h>
#include<iom16.h>
#include"usart.h"
#include "ir_printf.h"
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
   USART_Init(MYUBRR);
  DDRD&=~(1<<PD2);
  TIMSK |= (1<<TOIE0);// intrerupere overflow
  TCCR0 = 0; // mod normal , fara prescaler , oc0 deconectat
  while(1)
  {
    value = (PIND & (1<<PD2));
    while( (PIND & (1<<PD2)) == value )
    {
      ;
    }
    value^= 1<<PD2;
    TCCR0 |= (1<<CS00);
    while( (PIND & (1<<PD2)) == value )
    {
      ;
    }
   
    cicli1 = TCNT0;
    cnt1=cnt;
    TCNT0 = 0;
    cnt=0;
   
    value^= 1<<PD2;
    while( (PIND & (1<<PD2)) == value )
    {
      ;
    }
   
   
    cicli0 = TCNT0;
    cicli1 = (cnt1*0xFF+cicli1);
    cicli0 = (cnt*0xFF+cicli0);
    
    if(!((value&1<<PD2)==1<<PD2))
    {
      cicli1^=cicli0;
      cicli0^=cicli1;
      cicli1^=cicli0;
   }
    TCCR0 = 0;
    USART_Transmit('1');
    USART_Transmit(':');
    PRINTF((void*)(&cicli1),LONG);
    USART_Transmit('\r');
    USART_Transmit('0');
    USART_Transmit(':');
    PRINTF((void*)(&cicli0),LONG);
    USART_Transmit('\r');
    cnt=0;
    
  }
}





