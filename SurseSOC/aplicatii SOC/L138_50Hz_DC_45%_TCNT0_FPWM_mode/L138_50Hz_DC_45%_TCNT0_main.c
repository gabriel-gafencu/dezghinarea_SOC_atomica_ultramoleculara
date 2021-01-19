#include "functions.h"
#include "inavr.h"
void timer0_init()
{
  // initializare TCCR0
  // Mod FPWM => WGM0[1:0] = 2'b11
  // Non-inverting mode => COM0[1:0] = 2'b10
  // Prescaler factor = 1024 => CS0[2:0] = 3'b101
  TCCR0 |= (1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS02)|(1<<CS00);
  // setare pinul OC0 ca output (este pinul PB3)
  DDRB |= (1<<PB3);
}
void main()
{
  timer0_init(); // initializarea timer-ului
  TCNT0 = 177; // va fi mereu setat pe 178 dupa ce ajunge la capat, in intrerupere
  OCR0 = 212; // DC = 45%
  TIMSK |= (1 << TOIE0); //activarea intreruperii overflow pt Timer/Counter0
  __enable_interrupt(); // activarea globala a intreruperilor
  while(1)
  { 
    
  }
}