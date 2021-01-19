#include "functions.h"

__interrupt void ISR_TIMER0_ovf()
{
  TCNT0 = 177;
}