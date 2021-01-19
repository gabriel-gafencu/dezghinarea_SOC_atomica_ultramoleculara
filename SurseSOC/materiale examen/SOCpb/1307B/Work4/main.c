#include<inavr.h>
#include<iom16.h>
#include "usart.h"
int cnt=0; //variabila pentru întrerupere
double aux_per;  // va fi 2 %
double durata_pas;
double aux2=0.05;
char aux[20];

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
 int aux_iana;   // in aux_iana vom avea factorul de umplere
void main()
{
  USART_initialize(BAUD_RATE);
  
  //Alegerea pinului de ie.ire
  DDRD|=(1<<PD5);
  PORTD&=(1<<PD5);
  //Initializarea timer-ului
  timer1_INIT();
  //Valoarea care se încarca în registru în functie de frecventa necesara
 // ICR1=4000;
  //Factor de umplere de 50%
  //OCR1A=ICR1>>1;
  
  double frecv=5;  // kHz
  double perioada=1/frecv; // ms
  ICR1 = perioada/0.00025;  
  OCR1A = aux2*ICR1; //factor de umplere de 5% pt hi
  
  // Calculam aux_per de 2% din perioada (ICR1, nu se modifica ! )
  
  aux_per = 0.02*ICR1;
  
 
  
  // (95-5)/2 = 45 pasi
  
  durata_pas = 500/(double)45/perioada;   // in ms; 11/0.2=55
  
  //durata_pas=30000;
  
  int flag=0;
  //pornirea întreruperii
  __enable_interrupt();
   
 
   
  
  while(1)
  {
    if(cnt == (int)durata_pas)
    {
      cnt = 0;
      //USART_transmit(0x0d);     // CR
     // USART_transmit(0x0a);     // LF -> cele 2 = '\n'
      USART_transmit('F');
      USART_transmit('U');
      USART_transmit('=');
      aux_iana = aux2*100;
      myprint (0, 2, &aux_iana);
      USART_transmit('%');
      if(flag == 0)
      {
        aux2 += 0.02;
        OCR1A = aux2*ICR1;
        if(OCR1A >= 0.95 * ICR1)
          flag=1;
        
      }
      else
      {
          aux2 -= 0.02;
          OCR1A = aux2*ICR1;
          if(OCR1A <= 0.05 * ICR1)
            flag=0;
      }
    }
  }
}