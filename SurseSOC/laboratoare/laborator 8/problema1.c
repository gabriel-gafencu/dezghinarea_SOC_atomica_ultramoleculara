#include <iom16.h>
#include <inavr.h>

#define DELAY_CYCLES 32768

int main( void )
{
  /*Dezactivare întreruperi*/
  __disable_interrupt();
  
  /*Setare perioadã de time-out 0.26s */
  WDTCR |= (1<<WDP2); // WDP = 1 0 0 (conform tabelului)
  
  /*Activare watchdog timer*/
  WDTCR |= (1<<WDE);
  
  /*Setare PD6 ca pin de iesire*/
  DDRD |= (1<<PD6);
  
  /*Activare întreruperi*/
  __enable_interrupt();
  
  /*Setare PD6 pe 1 logic*/
  PORTD |= (1<<PD6);
  
  /*Intarziem procesorul cu un numar de DELAY_CICLES cicli*/
  // intarzierea este de 8.19 ms
  __delay_cycles(DELAY_CYCLES);
  
  /*Setare PD6 pe 0 logic*/
  PORTD &= ~(1<<PD6);
  
  /*Asteptam interventia watchdog-ului*/
  while(1);
  
  // watchdog-ul se reseteaza dupa 0.26s, deci dupa reset, vom avea iarasi 1 logic pe pinul PD6
}