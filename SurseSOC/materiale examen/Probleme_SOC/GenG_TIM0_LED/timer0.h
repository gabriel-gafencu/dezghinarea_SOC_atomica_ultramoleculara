#ifndef __TIMER0__
#define __TIMER0__

#include<iom16.h>
#include<inavr.h>

void TIMER0_init();
#pragma vector = TIMER0_OVF_vect
__interrupt void isr_TIMER0_OVF_vect();

#endif