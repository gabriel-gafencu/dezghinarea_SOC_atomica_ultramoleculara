#include<iom16.h>
#include<inavr.h>
#include"usart.h"

__no_init unsigned int overflowsNo;
__no_init unsigned int timerVal;

#pragma vector = TIMER1_OVF_vect
__interrupt void isr_TIMER1_OVF_vect()
{
  overflowsNo++; 
}

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1);
  
  unsigned long long timePass = overflowsNo;
  timePass *= 65536;
  timePass += timerVal;
  timePass = timePass * 249 / 1000;
  
  unsigned long long freq = 524288;
  freq = freq * 1000000 / timePass;
  
  USART_transmit_ulonglong(freq);
  USART_transmit('\r');
  USART_transmit('\n');
  
  overflowsNo = 0;
  timerVal = 0;
  
  WDTCR |= (1 << WDP2) | (0 << WDP1) | (1 << WDP0);
  TIMSK |= (1 << TOIE1);
  __enable_interrupt();
  TCCR1B |= (1 << CS10);
  WDTCR |= (1 << WDE);
  
  while(1)
    timerVal = TCNT1;
}
