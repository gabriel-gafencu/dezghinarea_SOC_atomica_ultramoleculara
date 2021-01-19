#include <iom16.h>
#include <inavr.h>
#include "usart.h"
__no_init unsigned int Timer1_currentValue;
__no_init unsigned int Timer1_numberOverflows;
#pragma vector = TIMER1_OVF_vect
__interrupt void T1_OVF()
{
Timer1_numberOverflows++;
}

void printFrecv(unsigned long frecv)
{
    unsigned long parteIntreaga=(unsigned long)frecv;
    unsigned long parteFractionara=(frecv-parteIntreaga*1000000)*1000;
    unsigned char buffer[3]={'\0'};
    int index=0;
  
     USART_transmit(parteIntreaga%10+'0');
     USART_transmit('.');
     
     while(parteFractionara)
     {
      buffer[index++]=parteFractionara%10+'0';
      parteFractionara/=10;
       
     }
     index-=1;
     while(index)
     {
     USART_transmit(buffer[index--]);
     }
     USART_transmit(0x0d);
     USART_transmit(0x0a);
   
  
     
}
unsigned long number;
unsigned long period;
unsigned long time_per_clock;
unsigned long frecv;
unsigned long parteFractionara;
int index;
unsigned char buffer[3];
int main( void )
{
  /*iniþializare usart*/
  USART_initialize(BAUD_RATE);
  /*iniþializare Timer 1*/
  /*Mod de funcþionare Normal*/
  /*Setare prescaler */
  TCCR1B|=(1<<CS10);
  /*Activare întrerupere*/
  TIMSK|=(1<<TOIE1);
  __enable_interrupt();
  /*iniþializare Watchdog*/
  /*Setare time-out de 65 ms*/
  WDTCR|= (1<<WDP1);
  if(Timer1_numberOverflows >0 || Timer1_currentValue>0)
  {
    /*calculul perioadei efective de time-out a watchdog-ului*/
     number=Timer1_numberOverflows*65535+Timer1_currentValue;
    /*calculul perioadei în us*/
     period=number*0.25;
    /*calculul perioadei unui ciclu de clock al watchdog-ului*/
     time_per_clock=(unsigned long)((period*1000000)/65536);
    /*calculul frecvenþei în MHz*/
    frecv=(unsigned long)((1000000./time_per_clock));
    parteFractionara=((1000000./time_per_clock) - frecv)*1000;
    
    while(frecv)
    {
     USART_transmit(frecv%10+'0'); 
     frecv/=10;
    }
    
   
    USART_transmit('.');
     
     if(parteFractionara)
     {
       
       while(parteFractionara)
     {
      buffer[index++]=parteFractionara%10+'0';
      parteFractionara/=10;
       
     }
       
     }
     else
     {
      index=3; 
     }
     
     index-=1;
     while(index)
     {
     USART_transmit(buffer[index--]);
     }
     USART_transmit(buffer[0]);
     USART_transmit('M');
     USART_transmit('H');
     USART_transmit('z');
     USART_transmit(0x0d);
     USART_transmit(0x0a);
     
     
    
    
    
    
  }
  Timer1_numberOverflows=0;
  Timer1_currentValue=0;
  TCNT1=0;
  /*Resetare WDT*/
  asm("WDR");
  /*Activare WDT*/
  WDTCR|=(1<<WDE);
  while(1)
  {
    Timer1_currentValue=TCNT1;
  }
return 0;
}