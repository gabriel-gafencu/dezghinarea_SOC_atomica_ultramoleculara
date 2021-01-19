#include<iom16.h>
#include<inavr.h>

unsigned int softFlag;

#pragma vector = TIMER0_COMP_vect
__interrupt void isr_TIMER0_COMP_vect()
{
  softFlag = 1;
}

void main( void )
{
  TCCR0 = (1 << WGM01) | (0 << WGM00) | (1 << CS02) | (0 << CS01) | (1 << CS00);
  OCR0 = 206;
  TIMSK |= (1 << OCIE0);
  __enable_interrupt();
  
  TCCR1A |= (1 << COM1A1) | (0 << COM1A0) | (1 << WGM11) | (0 << WGM10);
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
  ICR1 = 502;
  OCR1A = 502 * 5 / 100;
  
  unsigned char direction = 1;
  unsigned int duty = 5;
  unsigned long aux;
  
  DDRD = (1 << PD5);
  
  while(1)
  {
    if(softFlag == 1)
    {
      softFlag = 0;
      if(direction == 0)
      {
        duty = duty - 5;
      }
      else
      {
        duty = duty + 5;

      }
      if(duty > 95)
        direction = 0;
      if(duty < 5)
        direction = 1;
      
      aux = ICR1;
      aux = aux * duty / 100;
      OCR1A = aux;
    }
  }
}
