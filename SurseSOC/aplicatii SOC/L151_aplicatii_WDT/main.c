//#define APP1
//#define APP2
#define APP3
// *****************************************************************************
// Aplcatia 1: demonstrarea functionarii WDT-ului
// *****************************************************************************

#ifdef APP1
#include <iom16.h>
#include <inavr.h>
#define DELAY_CYCLES 2097152 //cam 0.5s
void main()
{
  //__disable_interrupt();
  // Setare perioadã de time-out 2.1s
  WDTCR |= (1 << WDP2) | (1 << WDP1) | (1 << WDP0);
  
  // Activare watchdog timer
  WDTCR |=(1 << WDE);
  
  // Setare PD5 ca pin de iesire
  DDRD |= (1 << PD5);
  
  //__enable_interrupt();
  // Setare PD5 pe 1 logic <=> aprindere LED
  PORTD |= (1 << PD5);
  
  // folosim __delay_cycles pentru a observa LED-ul aprins
  __delay_cycles(DELAY_CYCLES);
  
  // Setare PD5 pe 0 logic <=> stigere LED
  PORTD &= ~(1 << PD5);
  
  // bucla infinita
  while(1)
  {
    
  }
}
#endif

// *****************************************************************************
// Aplicatia 2: Sa se calculeze perioada efectiva a WD-ului
// *****************************************************************************

#ifdef APP2
#include <iom16.h>
#include <inavr.h>
#define NR_CYCLES 2000
void main()
{
  // falfaim un LED de 2 ori cand RESET-ul este de la WDT
  DDRD |= (1 << PD5);
  if(MCUCSR & (1 << WDRF))
  {
    PORTD ^= (1 << PD5);
    __delay_cycles(1000000);
    PORTD ^= (1 << PD5);
    __delay_cycles(1000000);
    PORTD ^= (1 << PD5);
    __delay_cycles(1000000);
    PORTD ^= (1 << PD5);
  }
  __delay_cycles(16000000); // pauza de 4s
  
  // initializare time-out Watchdog (L152)
  // WDP[2:0] = 3'b000; //time-out de 16.3ms (16384 cicli WDT)
  // WDTCR |= (1 << WDP0); // time-out de 32.5 ms (32768 cicli WDT)
  // WDTCR |= (1 << WDP0) | (1 << WDP1); // time-out de 0.13s (131072 cicli WDT)
  // WDTCR |= (1 << WDP0) | (1 << WDP1)| (1 << WDP2); // time-out de 2.1s (2097152 cicli WDT)
  
  // scoatem pe PD6 un semnal cu o frecventa data de __delay_cycles()
  // durata intre primul front pozitiv si ultimul front negativ al acestui semnal va fi direct prop. cu perioada time-out
  DDRD |= (1 << PD6);
  
  //Resetare WDT
  __watchdog_reset(); // sau asm("WDR");
  
  //Activare WDT
  WDTCR |= (1 << WDE);
  
  while(1)
  {
    PORTD ^= (1 << PD6);
    __delay_cycles(NR_CYCLES - 6); // -6 pentru a lua in calcul si celelalte instructiuni din while(1)
  }
}
#endif

// *****************************************************************************
// Aplicatia3:: Sa se calculeze frecventa efectiva a WD-ului
// *****************************************************************************

#ifdef APP3

#include <iom16.h>
#include <inavr.h>
#include "usart.h"
__no_init unsigned int Timer1_currentValue;
__no_init unsigned int Timer1_numberOverflows;

#pragma vector = TIMER1_OVF_vect
__interrupt void T1_OVF()
{
  Timer1_numberOverflows++;
}

void timer1_init()
{
  // initializare Timer/Counter 1 
  // in modul NORMAL => WGM1[3:0] = 4b'0000
  // fara prescaler => CS1[2:0] = 3'b001
  TCCR1B |= (1 << CS10);
  
  // activarea intreruperii ovf
  TIMSK |= (1 << TOIE1);
}

    void main( void )
{
  __disable_interrupt();
  
  // initializare usart
  USART_initialize(BAUD_RATE);
  
  // initializare time-out Watchdog (L152)
  // WDP[2:0] = 3'b000; //time-out de 16.3ms (16384 cicli WDT)
 //  WDTCR |= (1 << WDP0); // time-out de 32.5 ms (32768 cicli WDT)
  // WDTCR |= (1 << WDP1); // time-out de 65 ms (65536) cicli WDT)
  // WDTCR |= (1 << WDP0) | (1 << WDP1); // time-out de 0.13s (131072 cicli WDTs)
  // WDTCR |= (1 << WDP0) | (1 << WDP1)| (1 << WDP2); // time-out de 2.1s (2097152 cicli WDTs)
  
  if(Timer1_numberOverflows > 0 || Timer1_currentValue > 0)
  {
    // calculul perioadei efective de time-out a watchdog-ului in ms
    unsigned long number = Timer1_numberOverflows * 65536 + Timer1_currentValue;
    unsigned long long period = (unsigned long long) number * 2487 / 10000000;
    unsigned char zecimala = (unsigned long long) number * 2487 / 1000000 % 10;
    
    // calculul perioadei unui ciclu de clock al watchdog-ului in ns (pentru o precizie mai buna)
    // tin cont de numarul de cicli corespunzator perioadei de time-out alese
    unsigned long time_per_clock = (unsigned long)((period * 1000000) / 16384);
    
    // calculul frecventei în kHz
    unsigned long frecv = (unsigned long)((1000000 / time_per_clock));
    USART_transmit('f');
    USART_transmit('=');
    // integerTransmit(4,&frecv);
    transmit_ulong(frecv);
    
    USART_transmit('k');
    USART_transmit('H');
    USART_transmit('z');
    
    // afisam si perioada de time-out
    USART_transmit(',');
    USART_transmit('T');
    USART_transmit('=');
    // integerTransmit(2,&period);
    transmit_ulong(period);
    USART_transmit('.');
    USART_transmit(zecimala + '0');
    
    USART_transmit('m');
    USART_transmit('s');
    USART_transmit(0x0d);
    USART_transmit(0x0a);
  }
  Timer1_numberOverflows = 0;
  Timer1_currentValue = 0;
 
  // resetare WDT
  // asm("WDR");
  
  // activare WDT si initializare Timer 1
  timer1_init();
  WDTCR |= (1 << WDE);
  TCNT1 = 0; // pentru a porni numaratorile cam in acelasi timp
  
  // activarea globala a intreruperilor
  __enable_interrupt();
  
  while(1)
  {
    Timer1_currentValue = TCNT1;
  }
}
#endif