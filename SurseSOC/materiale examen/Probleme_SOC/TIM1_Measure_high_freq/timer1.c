#include"timer1.h"

unsigned int tim1_ovf;
unsigned int tim1_val;

void TIMER1_init()
{
  TCCR1B = (1 << CS12) | (1 << CS11) | (1 << CS10);
  TIMSK |= (1 << TOIE1);
  DDRB = (0 << PB1);
}

__interrupt void isr_TIMER1_OVF_vect()
{
  tim1_ovf++;
}