#include<iom16.h>
#include<inavr.h>

void main( void )
{
  DDRD |= (1 << PD5);
  
  //OCR1A = 23438;
  TCCR1A = (1 << WGM11) | (1 << WGM10) | (0 << COM1A1) | (1 << COM1A0);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);
  OCR1A = 40000;
  
  while(1)
    ;
}
