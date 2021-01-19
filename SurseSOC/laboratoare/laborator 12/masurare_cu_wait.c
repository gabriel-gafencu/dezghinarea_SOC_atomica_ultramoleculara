#include <inavr.h>
#include <iom16.h>
#include "usart.h"

//transmiterea unui întreg pe seriala
void integerTransmit (unsigned int p1, unsigned int p2, void * p3)
{
  int index=0;
  char aux[5];
  int x=*((int *)(p3));
  if(x<0)
  {
    USART_transmit('-');
    x*=(-1);
  }
  while(x!=0)
  {
    aux[index]=x%10+'0';
    index++;
    x=x/10;
  }
  while(p2>0 )
  {
    USART_transmit(aux[index-1]);
    index--;
    p2--;
  }
}

void main()
{
  // PD2 = pin de intrare (implicit)
  while (1)
  {
    // asteptam pana la primul front pozitiv
    while ((PIND & (1 << PD2)) == 0);
  
    // am ajuns la frontul pozitiv
    // pornim timer-ul cu prescaler 256, adica 4 MHz / 256
    TCNT1 = 0;
    TCCR1B |= (1 << CS12);
    
    // asteptam sa treaca frontul pozitiv pe care am ajuns
    while ((PIND & (1 << PD2)) == (1 << PD2));
    
    // asteptam sa treaca si frontul negativ de imediat dupa
    while ((PIND & (1 << PD2)) == 0);
    
    // am ajuns la frontul pozitiv urmator
    // oprim timer-ul
    TCCR1B &= ~(1 << CS12);
    
    // in TCNT1 avem valoarea lui N
    // => perioada semnalului este T = N * t, unde t = perioada timer-ului
    unsigned int T = TCNT1 / 4 * 256; // pentru ca 1 ciclu = 0.25 microsecunde
    
    // trimitem valoarea pe seriala
    // T se va obtine in microsecunde
    USART_initialize(BAUD_RATE); // rata de biti de pe seriala
    integerTransmit(0, 5, &T);
    USART_transmit('\r');
    USART_transmit('\n');
    
    __delay_cycles(400000);
  }
}