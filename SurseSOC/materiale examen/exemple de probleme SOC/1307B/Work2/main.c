#include<inavr.h>
#include<iom16.h>
#include "usart.h"
int cnt=0; //variabila pentru întrerupere
long long int FOsc=4008506; //Hz
int Fx=1000;//Hz
int N=1; //fara prescaler
unsigned char c='0';
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

#pragma vector = USART_RXC_vect
__interrupt void interrupt_routine_USART_RXC(void)
{
  c=UDR; 
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

long int duty;
long long int DC;
long long int DCC;

void main()
{
  long long int nrCicli=(FOsc/Fx/N)*1000;
  
  //10.5a
  
  timer1_INIT();
  DDRD|=(1<<PD5);
  PORTD&=(1<<PD5);
  
  /*
  duty=(50L*1000)/100;
  DC=duty*nrCicli/1000000; 
  */
  
  
  //10.5b,c
  int fu=5;
  DC=fu*nrCicli/100000;
  
  
  USART_initialize(BAUD_RATE);
  __enable_interrupt();
  /*
  ICR1=nrCicli/1000;
  OCR1A=DC;
  
  while(1)
  {
    //pb 10.5a
    
    if(c=='+' || c=='-')
    {
    switch(c)
    {
  case '+':
    if(Fx<=9000)
    {
    Fx+=1000; 
    nrCicli=1000*FOsc/Fx/N; 
    DC=duty*nrCicli;
    ICR1=nrCicli/1000;
    DCC=DC/1000000;
    OCR1A=DCC; 
    c='0';
  }
    
    break;
    
  case '-':
    
    if(Fx>=2000)
    {
    Fx-=1000; 
    nrCicli=1000*FOsc/Fx/N; //
    DC=duty*nrCicli;
    ICR1=nrCicli/1000;
    OCR1A=DC/1000000;
    c='0';
    
  } 
    break;
    
    cnt=0;
    
  }
    
  }
    
  }
  }
    */
    
    //10.5b
    /*
    ICR1=nrCicli/1000;
    OCR1A=DC;    
    while(1)
    {
      
      
      switch(c)
      {
      case '+':
        if(fu<90)
        {
          fu+=5;
          DC=fu*nrCicli/100;
          OCR1A=DC/1000;
          c='0';   
        }
        
        break;
        
      case '-':
        if(fu>10)
        {
          fu-=5;
          DC=fu*nrCicli/100;
          OCR1A=DC/1000; 
          c='0';
        }
        break; 
        
      }
      cnt=0;
      
      
      
    }
  }
*/

//10.5C

ICR1=nrCicli/1000;
OCR1A=DC;  
while(1)
{
//c=USART_Receive();

switch(c)
{
    case '+':
if(fu<92&&Fx<14500)
{
fu+=3; 
Fx+=500;
nrCicli=FOsc/Fx/N*1000;
DC=fu*nrCicli/100;
ICR1=nrCicli/1000;
OCR1A=DC/1000; 
cnt=0;
c='0';
      }

break;

    case '-':
if(fu>8&&Fx>100)
{
fu-=3; 
Fx-=500;
nrCicli=FOsc/Fx/N*1000;
DC=fu*nrCicli/100;
ICR1=nrCicli/1000;
OCR1A=DC/1000;  
cnt=0;
c='0';
      }

break;
    } 
  }

}