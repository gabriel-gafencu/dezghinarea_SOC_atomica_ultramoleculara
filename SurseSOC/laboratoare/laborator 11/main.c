// Se primeste pe seriala un sir de forma: x.x KHz yy %
// Sa se genereze un semnal de frecventa x.x KHz, cu factorul de umplere yy %.
// x.x este intre 2 si 8 KHz

#include <iom16.h>
#include <inavr.h>
#include "usart.h"

char data;
char flag;

// rutina de intrerupere la primirea de date pe seriala
#pragma vector = USART_RXC_vect
__interrupt void interrupt_routine_USART_RXC(void)
{
  // cand am primit date, le salvam in variabila data
  data = UDR;
  
  // setam flagul pe 1, adica exista ceva pe seriala
  flag = 1;
}

void main( void )
{
  // initializam interfata seriala
  USART_initialize(BAUD_RATE);
  
  // activam intreruperile globale
  __enable_interrupt();
  
  unsigned int i; // indicele caracterului primit pe seriala
  char c; // caracterul primit de pe seriala
  unsigned int x1, x0; // cele 2 componente ale frecventei primite
  // x1 = parte intreaga
  // x0 = parte fractionara
  unsigned int duty_cycle; // factorul de umplere
  
  // configurare pin de iesire pentru semnalul generat
  DDRD |= (1 << PD5);
  
  // configurare Timer/Counter1
  // mod Clear on Compare, non-inverted
  TCCR1A |= (1 << COM1A1); 
  
  // Fast PWM, TOP = ICR1
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11); 
  
  i = 1;
  while(1)
  {
    if (USART_Receive(&c) == 0) // primim data de pe serial (daca exista)
    {  
      if (c == '\n' || c == '\r') // ignoram caracterele albe de newline si carriage return
      {
        continue;
      }
      switch(i)
      {
      case 1: // partea intreaga
        x1 = c - '0';
        break;
      case 2: // punct -> ignoram
      case 3: // partea fractionara
        x0 = c - '0';
        break;
      case 4: // spatiu -> ignoram
      case 5: // K
      case 6: // H
      case 7: // z
      case 8: // spatiu -> ignoram
      case 9: // cifra zecilor a factorului de umplere
        duty_cycle = (c - '0') * 10;
        break;
      case 10: // cifra zecilor a factorului de umplere
        duty_cycle += (c - '0');
        break;
      case 11: // spatiu -> ignoram
      case 12: // procent -> ignoram
        break;
      }
    }
    else
    {
      continue;
    }
    
    if (i == 12) // daca am primit toate 12 caractere
    {
      i = 1; // o luam de la capat cu citirea
      TCNT1 = 0;
      
      // si configuram timer-ul corespunzator a ceea ce am primit
      // calculam valoarea de pus in ICR1
      // ICR1 = perioada de generat / perioada clock-ului intern
      // ICR1 = numarul de perioade de clock ale microcontroller-ului necesare pentru a forma o perioada de generata
      ICR1 = 40000 / (x1 * 10 + x0);
      
      // calculam valoarea de pus in OCR1A
      // OCR1A = ICR1 * duty_cycle / 100
      OCR1A = (400 * duty_cycle) / (10 * x1 + x0);
      
      // pornim TC1, fara prescaler
      TCCR1B |= (1 << CS10);
    }
    else
    {
      ++i;
    }
  }
}