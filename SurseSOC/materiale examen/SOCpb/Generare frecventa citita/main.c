#include <inavr.h> //include biblioteca inavr.h
#include <iom16.h> //include biblioteca iom16.h
#include "usart.h"
#include "genF.h"
unsigned char frecv[10];
unsigned long Fx;
unsigned long FOsc=4015102;
unsigned int fu=3;
unsigned long nrCicli;
unsigned long DC;
int N=64;
int index;
int flag;

#pragma vector = USART_RXC_vect
__interrupt void interrupt_routine_USART_RXC(void)
{
  unsigned char c=UDR;
  
  if(flag==0)
  {
    if(c!='@' && c!='k' && c!='m' && c!='M' && c!='K')
    {
      frecv[index]=c;
      index++;
    }
    else
    {
      
      
      for(int i =0;i<index;++i)
      {
        if(frecv[i]>='0' && frecv[i]<='9')
        {
          Fx=Fx*10 + (frecv[i]-'0');
          
        }
      }
      
      
      
      if(c=='k' || c=='K')
      {
        Fx*=1000; 
      }
      else
      {
        if(c=='M')
        {
          Fx*=1000000; 
        }
        else
        {
          if(c=='m')
          {
            Fx/=1000; 
          }
        }
        
      }
       flag=1;
    }
  }
   
}



/*
void main( void )
{
//generare frecventa citita de pe seriala
  flag=0;
  Fx=0;
  USART_initialize(BAUD_RATE);
  __enable_interrupt();
  index=0;
  
  
  while(1)
  {
    if(flag==1)
    {
      TCCR1A = ( 1<<COM1A1 | 1<<WGM11 );
      TCCR1B = ( 1<<WGM13 | 1<<WGM12 | 1<<CS11) ; // prescaler de 8
      TIMSK|=(1<<TOIE1);
      DDRD|=(1<<PD5);
      PORTD&=(1<<PD5);
      nrCicli=FOsc*1000/Fx/N;
      DC=fu*nrCicli/100000;
      OCR1A=DC;   
      ICR1=nrCicli/1000;
    }
    
    
  }
  
  
}
*/

void main(void)
{
  USART_initialize(BAUD_RATE);
  __enable_interrupt();
  
  TCCR1A = ( 1<<COM1A1 | 1<<WGM11 );
  TCCR1B = ( 1<<WGM13 | 1<<WGM12 | 1<<CS11 ) ; //prescaler de 64
  
  while(1)
  {
   
    if(TCNT1==200)
    {
       if(sw==1)
    {
      if(fu<92 && TCNT1==200)
      {
        fu+=3;
        DC=fu*nrCicli/100000;
        OCR1A=DC; 
        ICR1=nrCicli/1000;   
        TCNT1=0;
      }
      
      
    }
    else
    {
      
      if(fu>8 && TCNT1==200)
      {
        fu-=3;
        DC=fu*nrCicli/100000;
        OCR1A=DC; 
        ICR1=nrCicli/1000;   
        TCNT1=0;
      }
      
      
    }
      
    }
   
    
  }
  
}