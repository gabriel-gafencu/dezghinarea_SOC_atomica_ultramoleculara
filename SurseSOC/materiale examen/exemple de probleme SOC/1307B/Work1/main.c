#include<inavr.h>
#include<iom16.h>
#include "usart.h"
int cnt=0; //variabila pentru întrerupere
double FOsc=4006996.637; //Hz
double Fx=1000;//Hz
int N=1; //fara prescaler

//întreruperea de overflow la timer1
#pragma vector=TIMER1_OVF_vect
__interrupt void T1int(void)
{
  cnt++;
}
//configurarea timer 1
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

/*
void main()
{
  cnt=0;
  //factor de umplere variabil (+-)5%, intre 5% si 95% in 5 secunde
  double nrCicli=FOsc/Fx/N;
  double step=(double)nrCicli/100*5;
  double sw=0;
  //Alegerea pinului de ie.ire
  DDRD|=(1<<PD5);
  PORTD&=(1<<PD5);
  //Initializarea timer-ului
  timer1_INIT();
  //Valoarea care se încarca în registru în functie de frecventa necesara
  ICR1=nrCicli;
  OCR1A=step;

  
  //pornirea întreruperii
  __enable_interrupt();
  
  while(1)
  {
    if(cnt==312)
    {
      if(sw==0)
      {
       OCR1A+=step; 
        
       if(OCR1A>=(double)95/100*nrCicli)
       sw=1; 
        
      }else
      {
        
       OCR1A-=step;
       if(OCR1A<=(double)5/100*nrCicli)
       sw = 0;
       
      }
      cnt=0;
       
    }
      
  }
}*/

char c;
void main()
{
  double nrCicli=FOsc/Fx/N;
  double duty=(double)50/100;
  double DC=duty*nrCicli; 
  //USART_initialize(BAUD_RATE);
  
  ICR1=nrCicli;
  OCR1A=DC;
  
  while(1)
  {/*
   c=USART_Receive();
   //pb 10.5a
   switch(c)
   {
   case '+':
     if(Fx<9000)
     {
      Fx+=1000; 
      nrCicli=FOsc/Fx/N;
      DC=duty*nrCicli;
      ICR1=nrCicli;
      OCR1A=DC;    
     }
     
     break;
     
   case '-':
     
     if(Fx>2000)
     {
      Fx-=1000; 
      nrCicli=FOsc/Fx/N;
      DC=duty*nrCicli;
      ICR1=nrCicli;
      OCR1A=DC;
       
     } 
     break;
     
     //cnt=0;
   
   }
   
   
    */
  }
}