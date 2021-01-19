#include <iom16.h>
#include <inavr.h>
#include "usart.h"

//generare frecventa de 5KHz cu fu=66%;
//perioada de transmitere a unui char pe seriala ~ 500us

void timer1_INIT()
{
  /*
  Modul Normal: WGM13:10=0000;
  Cu prescaler: CS12:10=010;
  */
  TCCR1B|=(1<<CS11);
  TCNT1=0;
 
}


__no_init unsigned int HI;

void main()
{
  int nrChars=0;
  USART_initialize(BAUD_RATE);
  DDRD  |=(1<<PD5);
  
  if(HI==1)
  {
    PORTD |=(HI<<PD5);
  }
  else
  {
    PORTD &=~(HI<<PD5);
    
  }
 
  //TCNT1=0;
  
   if((MCUCSR>>WDRF)&1)
  {
      
   if(HI==1)
   {
     
   timer1_INIT();
   while(TCNT1<2750);    //8430
  /*nrChars=11;
   while(nrChars)
   {
    USART_transmit('*');
    nrChars--;
   }*/
     
   WDTCR|= (1<<WDP0);
   WDTCR|= (0<<WDP1);
   WDTCR|= (0<<WDP2); 
   HI=0;  
   }
   else
   {
     
     timer1_INIT();
     while(TCNT1<5750); //4160
     /* nrChars=22;
      while(nrChars--)
      {
        USART_transmit('*'); 
      }*/
      WDTCR|= (0<<WDP0);
      WDTCR|= (1<<WDP1);
      WDTCR|= (0<<WDP2);
      HI=1;
    }
  }
  
  asm("WDR");
  WDTCR|=(1<<WDE);  
  while(1)
   {
       
   }
}