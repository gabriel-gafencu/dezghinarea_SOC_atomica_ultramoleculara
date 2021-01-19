#include <iom16.h>
#include <inavr.h>

unsigned int nrOVFs; // retinem de care ori timer-ul a dat ovf
#pragma vector = TIMER1_OVF_vect
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
void main()
{
  // initializam timer-ul
  timer0_init();
  
  // setam pinul OC1A = PD5 de iesire
  DDRD |= (1 << PD5);
  
  // f = 1kHz, fara prescaler => TOP = ICR1 = 4000
  ICR1 = 4020; // datorita impreciziei clock-ului, necesita o ajustare
  
  // DC = 50% => OCR1A = ICR1/2;
  OCR1A = ICR1 / 2;
  
  // folosim intreruperea TIMER1_OVF pentru a comuta un LED la fiecare 1000 ovf si pt a retine nr de ovf
  // setam pinul PD2 de iesire
  DDRD |= (1 << PD2);
  // activarea globala a intreruperilor
  __enable_interrupt();
  while(1)
  {
    
  }
}