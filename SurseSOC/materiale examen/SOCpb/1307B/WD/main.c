#include <iom16.h>
#include <inavr.h>
__no_init unsigned int Timer1_currentValue;
__no_init unsigned int Timer1_numberOverflows;
#pragma vector = TIMER1_OVF_vect
__interrupt void T1_OVF()
{
  Timer1_numberOverflows++;
}
int main( void )
{
  /*ini�ializare usart*/
  
  /*ini�ializare Timer 1*/
  /*Mod de func�ionare Normal*/
  /*Setare prescaler */
  TCCR1B|=(1<<CS10);
  /*Activare �ntrerupere*/
  TIMSK|=(1<<TOIE1);
  __enable_interrupt();
  /*ini�ializare Watchdog*/
  /*Setare time-out de 32.5 ms*/
  WDTCR|= (1<<WDP0);
  if(Timer1_numberOverflows >0 || Timer1_currentValue>0)
  {
    /*calculul perioadei efective de time-out a watchdog-ului*/
    unsigned long number=Timer1_numberOverflows*65535+Timer1_currentValue;
    /*calculul perioadei �n ms*/
    unsigned long period=number*0.00025;
    /*calculul perioadei unui ciclu de clock al watchdog-ului*/
    unsigned long time_per_clock=(unsigned long)((period*1000000)/32768);
    /*calculul frecven�ei �n Hz*/
    unsigned long frecv=(unsigned long)((1000000./time_per_clock));
    
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