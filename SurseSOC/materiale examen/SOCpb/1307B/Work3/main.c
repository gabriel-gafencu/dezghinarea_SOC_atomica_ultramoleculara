#include <iom16.h>
#include <inavr.h>
#include "header.h"
#include "usart.h"

void timer1_INIT()
{
  /*
  Modul Normal: WGM13:10=0000;
  Cu prescaler: CS12:10=010;
  */
  TCCR1B|=(1<<CS11);
 
}


__no_init unsigned int HI;
__no_init unsigned int stare;

void main()
{
  DDRD|=(1<<PD5);
  
  if(HI==0)
  {
    PORTD&=~(HI<<PD5);
  }
  else
  {
    PORTD |=(HI<<PD5);
  }
  
  
  
  TCNT1=0;
  
   if((MCUCSR>>WDRF)&1)
  {
        
    switch(stare)
    {
     
    case 0:
      WDTCR&= ~(1<<WDP0);
      WDTCR|= (1<<WDP1);
      WDTCR&= ~(1<<WDP2);
      HI=1;
      stare=1;

      break;
      
    case 1:
      WDTCR&= ~(1<<WDP0);
      WDTCR&= ~(1<<WDP1);
      WDTCR&= ~(1<<WDP2);
      HI=1;
      stare=2;
      
      break;
        
    case 2:
      WDTCR|= (1<<WDP0);
      WDTCR&= ~(1<<WDP1);
      WDTCR&= ~(1<<WDP2);
      HI=0;
      stare=3;
      
      break;
        
    case 3:
      WDTCR&= ~(1<<WDP0);
      WDTCR&= ~(1<<WDP1);
      WDTCR&= ~(1<<WDP2);
      HI=0;
      stare=0;
      break;
      
    default:
      stare=0;
      
    }
  }

  asm("WDR");
  WDTCR|=(1<<WDE);  
  while(1)
   {
       
   }
}
  
 
  
