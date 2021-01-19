#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <iom16.h>

#pragma vector = TIMER0_OVF_vect
__interrupt void ISR_TIMER0_ovf();

#endif