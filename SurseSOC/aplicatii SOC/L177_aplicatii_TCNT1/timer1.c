#include <inavr.h>
#include <iom16.h>
#include "timer1.h"

unsigned int nrOVFs; // retinem de care ori timer-ul a dat ovf
__interrupt void ISR_TIMER1_OVF()
{
  ++nrOVFs;
  if(!(nrOVFs % 1000))
    PORTD ^= (1 << PD2);
}

void timer0_init()
{
  // setam Timer/Counter 1 
  // in modul FPWM cu TOP = ICR1 => WGM1[3:0] = 4b'1110
  // fara prescaler => CS1[2:0] = 3'b001
  // non-inverting mode => COM1A[1:0] = 2'b10
  TCCR1A |= (1 << WGM11) | (1 << COM1A1);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10);
  
  // activarea intreruperii TIMER1_OVF
  TIMSK |= (1 << TOIE1);
}