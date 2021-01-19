#include<iom16.h>
#include<inavr.h>

void main( void )
{
  TCCR0 |= (1 << WGM01) | (1 << COM00) | (1 << CS00);
  DDRB |= (1 << PB3);
 
  
  while(1)
    ;
}
