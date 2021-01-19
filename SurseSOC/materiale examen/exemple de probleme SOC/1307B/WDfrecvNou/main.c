#include <inavr.h>
#include <iom16.h>


/* Generati un semnal de 1Khz cu factor de umplere care variaza crescator si descrescator
* cu pasi de 5% intre 5 si 95% la fiecare 2 secunde si transmitere pe seriala
*/

#define F_OSC 4000000
#define BAUD 19200
#define BAUD_RATE (F_OSC/16/BAUD - 1)

#define frecventa 6666 //cloci      frecventa = 4 000 000/ frecventa data de prof;
volatile long timp=6006; //perioada dintre transmisiuni - 1s timp = 4 000 000/ frecventa (pt o secunda)   daca vrem pentru 2 sec inmultim cu 2 sau pentru 0.5 sec impartim la 2

long cnt=0;

char data;
void USART_initialize(unsigned short int baud_rate)
{
	/* set baud rate */
	UBRRH = (unsigned char)(baud_rate >> 8);
	UBRRL = (unsigned char)(baud_rate & 0xFF);

	UCSRB = (1 << RXEN) | (1 << TXEN); /* enable transmission and reception */

	/* set TXD pin direction: output */
	DDRD |= (1 << PD1);

	/* set RXD pin direction: input */
	DDRD &= ~(1 << PD0);

	/* enable interrupt on receive */
	UCSRB |= (1 << RXCIE);
}

void USART_transmit(unsigned char data)
{
	/* wait until previous data transmitted, and then put current data on the line */
	while (!( UCSRA & (1 << UDRE)))
	{
		;
	}
	UDR = data;
}

//transmiterea unui intreg pe seriala

void integerTransmit (unsigned int p1, unsigned int p2,  unsigned int p3) // p3- este valoarea , p2 este nr de cifre care vreau sa il afisez, p1- poti sa il stergi !!! DAR NU ACUM !!
{
  int index=0;
  char aux[5];
   unsigned int x=p3;
  if(x<0)
  {
    USART_transmit('-');
    x*=(-1);
  }
  while(x!=0)
    {
      aux[index]=x%10+'0';//ia ultima cifra si o trans in char
      index++;
      x=x/10;//elimina ultima cifra
    }
  
    while(p2>0 )
    {
      USART_transmit(aux[index-1]);
      index--;
      p2--;
    }
  
}

#pragma vector=TIMER1_OVF_vect
__interrupt void T1int(void)
{
  cnt++;
}

void main( void )
{

  unsigned int duty_cycle=5;		// duty_cycle reprezinta procentul de la care pornim ... adica daca e  5%-95% duty_cycle e 5 ...
  signed char pas=3;				// pasul reprezinta cu cat se umple semnalul nostru in unitatea de timp ....  5% pentru ex asta 
  
  TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS10);//fara prescaler si scriu valoarea de TOP in ICR1
  TCCR1A|=(1 << COM1A1) |(1<<WGM11); 
  TIMSK |= (1<<TOIE1);
  
  DDRD |= (1 << PD5);
  PORTD |= (1 << PD5);
  
  USART_initialize(BAUD_RATE);
 
  ICR1=666; //calculat     4MHz / frecventa data de prof  ..... 
  
  
  OCR1A=(unsigned int)ICR1/100*duty_cycle;//calcul dc pentru timer       factorul de umplere pentru aparat
 
  
  __enable_interrupt(); 
  
  while (1)
  {
    //SAU timp=2000=2s
    if(cnt>=timp)
    {
      duty_cycle+=pas;//treci la dc urmator
      if(duty_cycle>95||duty_cycle<5)//daca au fost depasite limitele
      {
        pas=-pas;//schimba directia pasului
        duty_cycle+=pas; //anuleaza ultimul pas pt ca a cauzat iesirea din limite
        duty_cycle+=pas; //pentru a nu avea de doua ori aceasi valoare
      }
      OCR1A=(unsigned short)ICR1/100*duty_cycle;//calcul dc pentru timer       factorul de umplere pentru aparat
      
      
      if(duty_cycle<10)
      {
        integerTransmit(0,1,duty_cycle);//transmitere pe seriala
      }
      else
      {
        integerTransmit(0,2,duty_cycle);//transmitere pe seriala
      }
      
      USART_transmit(0x0d);
      USART_transmit(0x0a);
      cnt=0;//resetare conter timp
    }
  }
}
