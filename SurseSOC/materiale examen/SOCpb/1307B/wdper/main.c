//calculati si trimiteti pe seriala in formatul xxxx.x ms valoarea tipica a wd
//in cazul utilizarii prescalerului WDP2-0=100
#include <iom16.h>
#include <inavr.h>
#include "usart.h"
#include "fct.h"
__no_init unsigned int Timer1_currentValue;
__no_init unsigned int Timer1_numberOverflows;
#pragma vector = TIMER1_OVF_vect
__interrupt void T1_OVF()
{
  Timer1_numberOverflows++;
}

unsigned char vect[10];
int j =9;

unsigned char vectF[10];
int i=9;

int nrCifre(unsigned long nr)
{
 int nrC=0;

 while(nr)
 {
   nr/=10;
  nrC++; 
 }
 return nrC;
  
}



void main( void )
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
  /*Setare time-out de 32.5 ms*/
  WDTCR|= (1<<WDP0);
  WDTCR|= (1<<WDP1);
 
  WDTCR|= (1<<WDP2);
  int nrC=0;
  int rest=0;

  
  if(Timer1_numberOverflows >0 || Timer1_currentValue>0)
  {
    /*calculul perioadei efective de time-out a watchdog-ului*/
    unsigned long number=Timer1_numberOverflows*65535+Timer1_currentValue;
    /*calculul perioadei în ms*/
    unsigned long parteIntreagaPeriod=(unsigned long)number/4000.0;
    unsigned long parteFractionaraPeriod=(number/4000.0-parteIntreagaPeriod)*10;
    vect[j--]=parteFractionaraPeriod%10+'0';
    vect[j--]='.';
    
     nrC=nrCifre(parteIntreagaPeriod);
     
     while(parteIntreagaPeriod)
     {
       vect[j--]=parteIntreagaPeriod%10+'0';
       parteIntreagaPeriod/=10;
     }
      rest=4-nrC;
      while(rest)
      {
       vect[j--]='0';
       rest--;
        
      }
    
      for(j=j+1;j<10;++j)
      {
        USART_transmit(vect[j]);
      }
      
      USART_transmit('m');
      USART_transmit('s');
      USART_transmit(' ');
      
    
      unsigned long time_per_clock=(unsigned long)(number*250/2097152); 
      unsigned long parteIntreagaFrecv=(unsigned long)1000000/time_per_clock;
      long double nrr=100000000/time_per_clock;
      unsigned long parteFractionaraFrecv=(unsigned long)(100000000/time_per_clock-100*parteIntreagaFrecv);
    
      vectF[i--]=parteFractionaraFrecv%10+'0';
      parteFractionaraFrecv%=10;
      vectF[i--]=parteFractionaraFrecv%10+'0';
      vectF[i--]='.';
      
      
       nrC=nrCifre(parteIntreagaFrecv);
     
     while(parteIntreagaFrecv)
     {
       vectF[i--]=parteIntreagaFrecv%10+'0';
       parteIntreagaFrecv/=10;
     }
      rest=4-nrC;
      while(rest)
      {
       vectF[i--]='0';
       rest--;
        
      }
    
      for(i=i+1;i<10;++i)
      {
        USART_transmit(vectF[i]);
      }
      
      USART_transmit('k');
      USART_transmit('H');
      USART_transmit('z');
      USART_transmit(' ');
      
      
      
  
   
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
 
}