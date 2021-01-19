#include<inavr.h>
#include<iom16.h>

int cnt=0; //variabila pentru întrerupere
long int FOsc=4006012; //Hz
int Fx=5000; //Hz
int N=1;

//întreruperea de overflow la timer1
#pragma vector=TIMER1_OVF_vect
__interrupt void T1int(void)
{
   cnt++;
}

void timer1_INIT()
{
/*
Modul Fast PWM: WGM13:10=1110;
Fãrã prescaler: CS12:10=001;
Compare Output Mode: 10
*/
TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS10);
TCCR1A|=(1<<COM1A1)|(1<<WGM11);
TIMSK|=(1<<TOIE1);
}


long long int duty=15;//duty*10
int nrCicli=0;
int fu=50; //fu*10 initial


void main()
{
  __enable_interrupt();
  int sw=0;
  //Alegerea pinului de ie.ire
  DDRD|=(1<<PD5);
  PORTD&=(1<<PD5);
  //Initializarea timer-ului
  timer1_INIT();
  nrCicli=FOsc/Fx/N; //nrCicli
  
  OCR1A=nrCicli*fu/1000;
  ICR1=nrCicli;
  
  
 
  while(1)
  {
   
    if(cnt==12)                                                                                                                                       
    {
     
      if(sw==0)
      {
        fu+=duty;
        OCR1A=fu*nrCicli/1000;
        if(OCR1A>=95*nrCicli/100);
           sw=1;
      }
      else
      {
        fu-=duty;
        OCR1A=fu*nrCicli/1000;
        if(OCR1A<5*nrCicli/100)
           sw=0;
      }
    cnt=0;   
    }

  }
  
  
}

