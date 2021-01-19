#ifndef __TIMER0__
#define __TOMER0__

#include<iom16.h>
#include<inavr.h>
#include"timer1.h"

void TIMER0_init();
#pragma vector = TIMER0_COMP_vect
__interrupt void isr_TIMER0_COMP_vect();

#endif