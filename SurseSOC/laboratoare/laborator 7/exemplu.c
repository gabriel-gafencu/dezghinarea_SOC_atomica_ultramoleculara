#include <inavr.h>
#include <iom16.h>

int cnt=0; //variabila pentru �ntrerupere

//�ntreruperea de overflow la timer1
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
  F�r� prescaler: CS12:10=001;
  Compare Output Mode: 10
  */
  TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS10);
  TCCR1A|=(1<<COM1A1)|(1<<WGM11);
  TIMSK|=(1<<TOIE1);
}

void main()
{
  //Alegerea pinului de ie�ire
  DDRD|=(1<<PD5);
  PORTD&=(1<<PD5);
  
  //Ini�ializarea timer-ului
  timer1_INIT();
  
  //Valoarea care se �ncarc� �n registru �n func�ie de frecven�a necesar�
  //ICR1 = 4000;
  ICR1=4021; // 4021 = valoarea calculata
  
  //Factor de umplere de 50%
  OCR1A=ICR1>>1;
  
  //pornirea �ntreruperii
  __enable_interrupt();
  
  while(1)
  {
  }
}