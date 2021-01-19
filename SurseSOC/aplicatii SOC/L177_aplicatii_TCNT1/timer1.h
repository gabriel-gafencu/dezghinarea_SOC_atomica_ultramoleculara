#ifndef __TIMER1__
#define __TIMER1__

#pragma vector = TIMER1_OVF_vect
__interrupt void ISR_TIMER1_OVF();

void timer0_init();

#endif