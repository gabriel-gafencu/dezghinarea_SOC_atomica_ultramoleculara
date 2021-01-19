#include <inavr.h>
#include <iom16.h>

// fara prescaler si cu __delay_cycles()
/*
#define NR_CYCLES 15625
void timer0_init()
{
// Mod FPWM => WGM0[1:0] = 2'b11
// Non-inverting mode => COM0[1:0] = 2'b10
// Fara Prescaler => CS0[2:0] = 3'b001
TCCR0 |= (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS00);

// setare pin OC0 ca output (este pinul PB3)
DDRB |= (1 << PB3);
}
void main()
{
unsigned char brightness = 0;

// initializare TIMER/COUNTER0
timer0_init();

while(1)
{
for(brightness=0; brightness<255; brightness++)
{
OCR0 = brightness;
__delay_cycles(NR_CYCLES);
    }
for(brightness=255; brightness>0; brightness--)
{
OCR0 = brightness;
__delay_cycles(NR_CYCLES);
    }
  }
}
*/

// fara prescaler, cu intrerupere si fara __delay_cycles()

#define NR_MAX_OVFS 61
volatile unsigned int nrOVFs = 0;
#pragma vector = TIMER0_OVF_vect
__interrupt void ISR_timer0_ovf()
{
  ++nrOVFs;
}

void timer0_init()
{
  // Mod FPWM => WGM0[1:0] = 2'b11
  // Non-inverting mode => COM0[1:0] = 2'b10
  // Fara Prescaler => CS0[2:0] = 3'b001
  TCCR0 |= (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS00);
  
  // activare intrerupere TIMER0_OVF
  TIMSK |= (1 << TOIE0);
  
  // setare pin OC0 ca output (este pinul PB3)
  DDRB |= (1 << PB3);
}
void main()
{  
  // initializare TIMER/COUNTER0
  timer0_init();
  
  // activarea globala a intreruperilor
  __enable_interrupt();
  
  unsigned char brightness = 0;
  unsigned char flagBrightness = 0; // 0 - creste, 1 - descreste
  while(1)
  {
    if(nrOVFs > NR_MAX_OVFS)
    {
      OCR0 = brightness;
      nrOVFs = 0;
      
      if(brightness == 255)
        OCR0 = brightness;
      if(!flagBrightness)
        ++brightness;
      else
        --brightness;
      
      // brightness = 0 cand era 255 si a fost incrementat sau cand era 1 si a fost decrementat
      if(brightness == 0)
        flagBrightness = 1 - flagBrightness;
    }
  }
}
