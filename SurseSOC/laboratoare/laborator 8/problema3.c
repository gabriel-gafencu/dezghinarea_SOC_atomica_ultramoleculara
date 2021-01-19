#include <iom16.h>
#include <inavr.h>
#include "usart.h"
#include "myprint.h"

// valoarea curenta a timer-ului 1 pana la overflow
__no_init unsigned int Timer1_currentValue;

// numarul de overflow-uri date de timer
__no_init unsigned int Timer1_numberOverflows;

// rutina de tratare a overflow-ului timer-ului
// aici se intra cand timer-ul a ajuns la valoarea maxima si watchdog-ul inca nu a dat reset
#pragma vector = TIMER1_OVF_vect
__interrupt void T1_OVF()
{
  // incrementam numarul de overflow-uri
  Timer1_numberOverflows++;
}

int main( void )
{
  /*initializare USART*/
  USART_initialize(BAUD_RATE);
  
  /*Activare întrerupere Timer Overflow 1*/
  TIMSK |= (1<<TOIE1);
  
  // activare intrerupere globala
  __enable_interrupt();
  
  /*initializare Watchdog*/
  /*Setare time-out de 32.5 ms*/
  WDTCR |= (1<<WDP0);
  
  // daca watchdog-ul a dat reset (deci avem ceva in variabilele __no_init)
  if(Timer1_numberOverflows >0 || Timer1_currentValue>0)
  {
    // folosim ce a numarat pana acum timer-ul pentru a calcula cat a stat (efectiv) watchdog-ul pana sa dea reset
    
    /*calculul perioadei efective de time-out a watchdog-ului*/
    // valoarea maxima numarabila pe 16 biti (Timer1) e 65535 (2^16 - 1)
    // rezulta numarul de cicli procesor (masurati la clock-ul lui ATMega de 4 MHz) in care watchdog-ul a asteptat
    unsigned long number = Timer1_numberOverflows * 65535 + Timer1_currentValue;
    
    /*calculul perioadei în ms*/
    // period = cat dureaza, in cicli procesor ATMega, pana watchdog-ul da reset
    // 1 ciclu procesor (la 4 MHz) dureaza 250 ns = 0.00025 ms
    unsigned long period = number * 0.00025;
    
    /*calculul perioadei unui ciclu de clock al watchdog-ului*/
    // perioada de timeout = 32.5 ms (ideal) = 32768 cicli de WDT
    // noi am calculat durata efectiva (in ms) a 32768 cicli de WDT (pana s-a resetat)
    // stiind numarul de cicli, obtinem perioada unui ciclu (reala, afectata de eroare)
    // pentru ca impartirea da numar subunitar, se scaleaza cu 6 zerouri
    unsigned long time_per_clock = (unsigned long)((period * 1000000) / 32768);
    
    /*calculul frecventei în Hz*/
    // se revine la scalarea initiala prin inmultire cu 10^6
    unsigned long frecv = (unsigned long)((1000000. / time_per_clock));
    
    /*trimitere pe seriala*/
    integerTransmit(0, 4, &frecv);
    USART_transmit('\r');
    USART_transmit('\n');
  }
  
  // valorile initiale pentru variabilele folosite
  Timer1_numberOverflows = 0;
  Timer1_currentValue = 0;
  TCNT1 = 0;
  
  /*Resetare WDT*/
  asm("WDR");
  
  /*initializare Timer 1*/
  /*Mod de functionare Normal*/
  /*Setare prescaler la 1 (clock-ul placutei, de 4MHz) */
  TCCR1B |= (1<<CS10);
  // la setarea clock-ului, a pornit timer-ul -> incepe sa numere
  
  /*Activare WDT*/
  WDTCR |= (1<<WDE);
  // a pornit si watchdog-ul -> dupa 32.5 ms, va da reset
  
  // asteptam interventia watchdog-ului pentru reset, intr-o bucla infinita
  while(1)
  {
    // setam valoarea curenta a timer-ului
    Timer1_currentValue = TCNT1;
  }
  
}