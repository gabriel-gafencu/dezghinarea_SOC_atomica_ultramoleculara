#include <iom16.h>
#include <inavr.h>

int main( void )
{
  /*Activare întreruperi*/
  __enable_interrupt();
  
  /*initializare Watchdog*/
  /*Setare time-out de 16.3 ms*/
  WDTCR |= (~(1<<WDP0)) | (~(1<<WDP1)) | (~(1<<WDP2));
  
  /*Se blocheazã procesorul timp de 100000 de cicli*/
  // doar pentru a observa perioada in care procesorul sta degeaba
  __delay_cycles(100000); // asteapta 25 ms
  
  /*Resetare WDT*/
  __watchdog_reset();
  // asm("WDR");
  
  /*Activare WDT*/
  WDTCR |= (1<<WDE);
  
  // pinul PC1 de iesire
  DDRC |= (1<<DDC1);
  
  // scrie o valoare pe portul C
  PORTC |= (1<<PC1);
  
  while(1)
  {
    PORTC ^= (1<<PC1); // comutare (trigger) pin PC1
    __delay_cycles(1000); // asteapta 0.25 ms
  }
  
}