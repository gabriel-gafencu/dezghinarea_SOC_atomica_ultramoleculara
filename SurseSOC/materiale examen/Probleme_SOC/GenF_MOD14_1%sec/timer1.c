#include "timer1.h"

unsigned int softCount;

void TIMER1_init()
{
  TCCR1A |= (1 << WGM11) | (0 << WGM10);
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  
  TCCR1A |= (1 << COM1A1) | (0 << COM1A0);
  
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
  
  unsigned int wantedFrq = 20000; /*oscilatiiCLK pe secunda*/
  ICR1 = (F_OSC) / wantedFrq - 1;
  
  unsigned int duty = 1; /*procent*/
  OCR1A = ICR1 * duty / 100;
}

void TIMER0_init()
{
  TCCR0 |= (0 << WGM01) | (0 << WGM00); 
  TCCR0 |= (1 << CS02) | (0 << CS01) | (1 << CS00);
}

__interrupt void isr_TIMER0_OVF_vect()
{
  if(softCount <= 16)
    softCount++;
  else
    softCount = 0;
}
