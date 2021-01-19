#include<iom16.h>
#include<inavr.h>
#include"usart.h"

__no_init unsigned int Timer1_currentValue;
__no_init unsigned int Timer1_numberOverflows;
__no_init unsigned int sendFlag;

#pragma vector = TIMER1_OVF_vect
__interrupt void isr_TIMER1_OVF_vect()
{
  Timer1_numberOverflows++;
}

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD = (1 << PD1);
  
  TIMSK |= (1 << TOIE1);
  __enable_interrupt();
  
  sendFlag++;
  if(sendFlag > 50)
    sendFlag = 0;
  
  unsigned long long aux = Timer1_numberOverflows;
  aux = aux * 65536 + Timer1_currentValue;
  aux = aux << 1; //aux - timp de resetare in microsecunde
  aux = (unsigned long long)32768 * 1000000 / aux;
  
  if(sendFlag == 0)
  {
  //USART_transmit('<');
  USART_transmit_ulonglong(aux);
  //USART_transmit('>');

  USART_transmit('\r');
  USART_transmit('\n');
  }
  
  Timer1_currentValue = 0;
  Timer1_numberOverflows = 0;
  
  WDTCR |= (0 << WDP2) | (0 << WDP1) | (1 << WDP0);
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  
  TCNT1 = 0;
  WDTCR |= (1 << WDE);
    
  while(1)
  {
    Timer1_currentValue = TCNT1;
  }
}
