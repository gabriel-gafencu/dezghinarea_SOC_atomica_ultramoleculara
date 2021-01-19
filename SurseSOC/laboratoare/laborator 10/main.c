#include <iom16.h>
#include <inavr.h>
#include <string.h>
#include "usart.h"
#include "myprint.h"

void main( void )
{
  USART_initialize(BAUD_RATE);
  unsigned int tip, nr_car;
  
  // transmiterea numarului -2.13 pe seriala
  double a = -2.13;
  double *val = &a;
  tip = 2;
  nr_car = 2;
  myprint(tip, nr_car, val);
  
  // transmiterea textului "Numarul 1.2345 cu 2 zecimale este 1.23."
  USART_transmit(0x0d); // CR (Carriage Return)
  USART_transmit(0x0a); // LF (Line Feed)
  
  // "Numarul "
  char s1[] = "Numarul ";
  tip = 3;
  nr_car = strlen(s1);
  myprint(tip, nr_car, s1);
  
  // 1.2345
  double n1 = 1.2345;
  double* pn1 = &n1;
  tip = 2;
  nr_car = 4;
  myprint(tip, nr_car, pn1);
  
  // " cu "
  char s2[] = " cu ";
  tip = 3;
  nr_car = strlen(s2);
  myprint(tip, nr_car, s2);
  
  // 2
  int n2 = 2;
  int* pn2 = &n2;
  tip = 0;
  nr_car = 1;
  myprint(tip, nr_car, pn2);
  
  // " zecimale este "
  char s3[] = " zecimale este ";
  tip = 3;
  nr_car = strlen(s3);
  myprint(tip, nr_car, s3);
  
  // 1.23
  double n3 = 1.23;
  double* pn3 = &n3;
  tip = 2;
  nr_car = 2;
  myprint(tip, nr_car, pn3);
  
  while (1) {}
}