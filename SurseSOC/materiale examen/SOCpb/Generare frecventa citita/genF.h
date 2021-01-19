#ifndef genf_h
#define genf_h

#pragma vector = USART_RXC_vect
__interrupt void interrupt_routine_USART_RXC(void);

#endif