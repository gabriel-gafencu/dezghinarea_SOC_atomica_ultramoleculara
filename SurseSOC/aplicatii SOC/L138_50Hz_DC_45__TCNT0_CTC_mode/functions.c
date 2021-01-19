#include "functions.h"

volatile unsigned char dutyFlag = 1;
__interrupt void ISR_TIMER0_output_compare()
{
  if(!dutyFlag)
  {
    OCR0 = 140;
    dutyFlag = 1;
  }
  else
  {
    OCR0 = 170;
    dutyFlag = 0;
  }
}