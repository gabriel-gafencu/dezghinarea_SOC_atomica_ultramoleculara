// Sa se genereze un semnal dreptunghiular de o frecventa de 5 KHz, cu un
// factor de umplere ce variaza intre 5% si 95%, avansand cu 5% la fiecare 0.5s.
// Factorul de umplere curent se va trimite pe seriala in forma <FU-XX%>.

#include <iom16.h>
#include <inavr.h>
#include "usart.h"
#include "myprint.h"

// numarul de overflow-uri ale TIMER0 folosite pentru a masura 0.5s
unsigned int nrOverflows = 0;

// directia de modificare a factorului de umplere
short int dir = 1; // (implicit, +)

unsigned int fu = 5; // factorul curent de umplere
char* s1 = "<FU-";
char* s2 = "%>";

#pragma vector = TIMER0_OVF_vect
__interrupt void interrupt_routine_TIMER0_OVF(void)
{
  ++nrOverflows;
  // pentru a ajunge la 0.5s (=2000 de cicli Timer0 cu prescaler 1/1024), 
  // Timer0 are nevoie de 7 overflow-uri
  if (nrOverflows == 7) 
  {
    while (TCNT0 < 215); // si o ultima parcurgere de 215 numarari
    OCR1A += 40 * dir; // modificam factorul de umplere
    fu += 5 * dir;
    
    // daca am ajuns la factor de umplere de 95% (respectiv 5%), schimbam directia
    if (OCR1A == 760 || OCR1A == 40) 
    {
      dir = -dir;
    }
    
    // trimitem factorul curent de umplere pe seriala
    myprint(3, 4, s1); // <FU-
    USART_transmit(fu / 10 + '0'); // prima cifra
    USART_transmit(fu % 10 + '0'); // a doua cifra
    myprint(3, 4, s2); // %>
      
    // resetam numaratorul si numarul de overflow-uri
    nrOverflows = 0;
    TCNT0 = 0;
  }
}

void main(void)
{  
  // activam intreruperile globale
  __enable_interrupt();
  
  // initializam transmisia seriala
  USART_initialize(BAUD_RATE);
  
  // configurare pin de iesire pentru semnalul generat
  DDRD |= (1 << PD5);
  
  // configurare Timer/Counter1
  // mod Clear on Compare, non-inverted
  TCCR1A |= (1 << COM1A1); 
  
  // Fast PWM, TOP = ICR1
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11); 
  
  // calculam valoarea de pus in ICR1
  // ICR1 = perioada de generat / perioada clock-ului intern
  // ICR1 = numarul de perioade de clock ale microcontroller-ului necesare pentru a forma o perioada de generata
  ICR1 = 800;
  OCR1A = 40; // initial, F.U. = 5% (5% din 800 = pasul de incrementare / decrementare a lui F.U.)
  
  // pornim TC1, fara prescaler
  TCNT1 = 0;
  TCCR1B |= (1 << CS10);
  
  // configuram Timer/Counter0, pentru a masura 0.5s
  TCNT0 = 0;
  TIMSK |= (1 << TOV0); // pornim intreruperea de overflow
  TCCR0 |= (1 << CS02) | (1 << CS00); // folosim prescaler 1/1024

  while(1) {}
}