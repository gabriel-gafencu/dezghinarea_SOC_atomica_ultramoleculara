#ifndef MYPRINT_H
#define MY_PRINT_H

void integerTransmit (unsigned int p1, unsigned int p2, void * p3);
void hexadecimalTransmit (unsigned int p1, unsigned int p2, void * p3);
void doubleTransmit(unsigned int p1, unsigned int p2, void * p3);
void floatTransmit(unsigned int p1, unsigned int p2, void * p3);
void characterTransmit (unsigned int p1, unsigned int p2, void * p3);
void myprint(unsigned int tip, unsigned int nr_car, void * val);

#endif