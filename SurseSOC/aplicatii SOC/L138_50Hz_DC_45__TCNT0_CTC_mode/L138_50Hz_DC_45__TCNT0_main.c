#include "functions.h"
#include "inavr.h"
void timer0_init()
{
  // initializare TCCR0
  // Mod CTC => WGM0[1:0] = 2'b10
  // Toogle OC0 => COM0[1:0] = 2'b01
  // Prescaler factor = 256 => CS0[2:0] = 3'b100
  TCCR0 |= (1<<WGM01)|(1<<COM00)|(1<<CS02);
  // setare pinul OC0 ca output (este pinul PB3)
  DDRB |= (1<<PB3);
}
void main()
{
  timer0_init();// initializarea timer-ului
  OCR0 = 140; //Output Compare registrer; initial ia valoarea 140, dupa care este reactualizat 
  
  // prima varianta, in care OCR0 este reactualizat in intrerupere cu valoarea 170, apoi iarasi cu 140 s.a.m.d., dupa fiecare Output Compare Match
  
  TIMSK |= (1 << OCIE0); //activarea intreruperii Output Compare pt Timer/Counter0
  __enable_interrupt(); // activarea globala a intreruperilor
  while(1)
  { 
    
  }
  
  // a doua varianta, in care OCR0 este reactualizat corespunzator, dupa ce TCNT0 depaseste o anumita valoare (de ex, 100)
  // bazat pe faptul ca nu e nevoie ca reactualizarea sa se faca fix la Output Compare Match
  
  /*
  volatile unsigned char dutyFlag = 1;
  volatile unsigned char needToChangeOCR0 = 0;
  // crearea unei bucle infinite 
  while(1) 
  {
    if(TCNT0 > 100)
    {
      if(needToChangeOCR0)
      {
        if(dutyFlag)
        {
          OCR0 = 140;
          dutyFlag = 0;
        }
        else
        {
          OCR0 = 170;
          dutyFlag = 1;
        }
        needToChangeOCR0 = 0;
      }
    }
    else
    {
      needToChangeOCR0 = 1;
    }
  }
  */
}