#ifndef __TIMER1__
#define __TIMER1__

#include <iom16.h>
#include <inavr.h>

#define F_OSC 4010400

void TIMER1_init();

void TIMER0_init();
#pragma vector = TIMER0_OVF_vect
__interrupt void isr_TIMER0_OVF_vect();

#endif