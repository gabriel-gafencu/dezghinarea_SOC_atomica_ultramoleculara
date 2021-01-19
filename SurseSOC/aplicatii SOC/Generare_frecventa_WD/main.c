#include "iom16.h"
#include "inavr.h"

__no_init unsigned char front_semnal;

#pragma vector = TIMER0_COMP_vect
__interrupt void ISR_TIMER0_COMP()
{
  // Activare WDT
  WDTCR |= (1 << WDE);
}

void timer0_init()
{
  // initializare TCCR0
  // Mod Normal => WGM0[1:0] = 2'b00
  // Prescaler factor = 1024 => CS0[2:0] = 3'b101
  TCCR0 |= (1<<CS02)|(1<<CS00);
  
  // activarea locala a intreruperii TIMER0_COMP
  TIMSK |= (1 << OCIE0);
}

void main()
{  
  // semnalul generat va fi scos pe pinul PD3
  DDRD |= (1 << PD3);
  DDRB |= (1 << PB0);
  
  // initializare time-out Watchdog (L152)
  // WDP[2:0] = 3'b000; //time-out de 16.3ms (16384 cicli WDT)
  // WDTCR |= (1 << WDP0); // time-out de 32.5 ms (32768 cicli WDT)
  // WDTCR |= (1 << WDP0) | (1 << WDP1); // time-out de 0.13s (131072 cicli WDT)
  // WDTCR |= (1 << WDP0) | (1 << WDP1)| (1 << WDP2); // time-out de 2.1s (2097152 cicli WDT)
  
  timer0_init();
  __enable_interrupt();
  if(front_semnal)
  {
    PORTD |= (1 << PD3);
    PORTB |= (1 << PB0);
    WDTCR |= (1 << WDP1); // time-out de 65 ms (65536) cicli WDT)
    OCR0 = 60;
  }
  else
  {
    PORTD &= ~(1 << PD3); 
    PORTB &= ~(1 << PB0);
    WDTCR |= (1 << WDP0); // time-out de 32.5 ms (32768 cicli WDT)
    OCR0 = 30;
  }
  front_semnal = !front_semnal;
  
  //WDTCR |= (1 << WDE);
  
  while(1)
  {
    // gol
  }
}