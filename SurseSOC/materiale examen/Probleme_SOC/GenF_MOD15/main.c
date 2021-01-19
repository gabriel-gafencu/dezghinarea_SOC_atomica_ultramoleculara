#include "timer1.h"

void main( void )
{
  /*initializez timer*/
 TIMER1_init();
  
  /*setez PD5 (OC1A) ca pin de iesire*/
  DDRD = (1 << PD5);
  
  while(1)
    ;
}
