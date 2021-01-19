#include <inavr.h>
#include <iom16.h>
#include "usart.h"
unsigned char flag=0;
unsigned long value=0;
float valoare;
unsigned char low,high;

#pragma vector = ADC_vect
__interrupt void OnADC(void)
{
  flag = 1;
}

void main(void)
{
 
  DDRA &= ~(1<<PA1);
  USART_Init(MYUBRR);
  __enable_interrupt();
  //ADMUX |= (1<<REFS0);//teoretic referinta de 5v
  ADMUX=(1<<REFS1)|(1<<REFS0)|(1<<MUX0);//intrare
  ADCSRA |= (1<<ADEN);//enable adc
  ADCSRA |= (1<<ADPS2);//prescaler 16
  ADCSRA |= (1<<ADIE); //activare intrerupere
  ADCSRA |= (1<<ADSC);//start conversie
  
  while(1)
  {
    if(flag)
    {
      low = ADCL;
      high = ADCH;
      value = ((high&0x03)<<8)|low;
      valoare = (value*5)/1024.;
      //parteIntreaga=(unsigned long)valoare;
      //parteFractionara=(valoare-parteIntreaga)*1000;
      
      
      if(valoare>=0 && valoare<=1.5)
      {
         PRINTF((void*)(&valoare),FLOAT);
         USART_Transmit('V');
         USART_Transmit(0xd);
         USART_Transmit(0xa);
      }
     
     
      flag = 0;
      ADCSRA |= (1<<ADSC);
    }
  }
}
/*

#include <inavr.h>
#include <iom16.h>
#include "usart.h"

#define RANGE 50000   // 10000
#define STEP RANGE/1024.0
#define PRECISION 100*/
/*
	Ground sursa a 2-a conectat la pin 11 pe placa
	VCC sursa a 2-a conectat la pin PA1(39 pe placa)
*/
	/*
int voltage;
int SC_flag=0;

#pragma vector=ADC_vect
__interrupt void ADC_INT(void)
{
   voltage=(int)ADC;
   SC_flag=1;
}


void PRINT_freq(int number, int precizion);

void main( void )
{
  int voltaj;
  DDRA=(0<<PA1);
  __enable_interrupt();
  USART_Init(MYUBRR);
  ADMUX=(1<<REFS1)|(1<<REFS0)|(1<<MUX0);//intrare
  ADCSRA=(1<<ADIE)|(1<<ADPS1)|(1<<ADEN);//ADEN AD enable ADSC porneste o noua canversie pentru single ended
  ADCSRA|=(1<<ADSC);
  while(1)
  {
    if(SC_flag)
    {
      voltaj=(int)(voltage*STEP)/100;
      PRINT_freq(voltaj,2);
      ADCSRA|=(1<<ADSC);
      SC_flag=0;
      
    }
  }
}


void PRINT_freq(int number, int precizion)
{
  unsigned char sir[10]={0,0,0,0,0,0,0,0,0,0,};
  int n=0;
  if(number>PRECISION)
  {
  while(number)
  {
     if(n==precizion)
     {
      sir[n]='.'; 
      n++;
     }
     sir[n]=((number%10)+'0');
     number/=10;
     n++;
     
  }
  n--;
  while(n>-1)
  {
     USART_transmit(sir[n]);
     n--;
  }

  }
  else
  {
   while(number)
     {
         sir[n]=((number%10)+'0');
         number/=10;
         n++;
       
    }
   while(n<2)
   {
    sir[n]='0';
    n++;
   }
   sir[n]='.'; 
   sir[n+1]='0';
   n+=2;
  }
  n--;
  while(n>-1)
  {
     USART_transmit(sir[n+1]);
     n--;
  }
  USART_transmit('V');
  USART_transmit(0x0D);
  USART_transmit(0x0A);
}
*/