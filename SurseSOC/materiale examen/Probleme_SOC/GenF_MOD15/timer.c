#include "timer1.h"

void TIMER1_init()
{
  /*selectare mod functionare*/
  TCCR1A |= (1 << WGM11) | (1 << WGM10);
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  
  /*selectare compare output mode*/
  TCCR1A |= (0 << COM1A1) | (1 << COM1A0);
  
  /*selectare prescaller*/
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
  
  /*setez valoare registru OCRA*/
  /*wantedFreq : frecventa dorita*/
  unsigned int wantedFreq = 1000;
  /*F_OSC >> 1 : OC1A face toggle de 2x / perioada*/
  unsigned int ocrVal = (F_OSC >> 1) / wantedFreq - 1;
  OCR1AH = (ocrVal >> 8);
  OCR1AL = ocrVal;
}
