#include <inavr.h> // include biblioteca inavr.h
#include <iom16.h> // include biblioteca iom16.h

unsigned char test = 0;

// facem corespondenta intre intreruperea INT1 si functia de tratare a ei
#pragma vector = INT1_vect
__interrupt void isr_INT1(void) // functia care se apeleaza la intreruperea INT1
{
  test += 1; // putem pune un breakpoint aici
}

int main( void )
{
  /* INT1 este pe pinul PD3 */
  DDRD = 0xFF; // seteaza toti pinii de pe portul D ca fiind de iesire
  PORTD = 0xFF; // scriere 1 logic pe toti bitii portului D
  
  // setam cand anume se genereaza intreruperea
  MCUCR |= ((1 << ISC11) | (1 << ISC10)); 
  /* Pe frontul pozitiv al lui INT1 
  se genereaza o cerere de întrerupere */
  
  GICR |= (1 << INT1); /* activeaza întreruperea externa INT1 */

  __enable_interrupt(); // activeaza întreruperea globala
  
  while (1)
  {
    PORTD = ~PORTD; // genereaza o întrerupere software
    // comuta toti bitii de pe portul D astfel incat sa se produca o intrerupere
    // cand trec din 0 in 1 (la frontul pozitiv), se genereaza intrerupere
  }
  
}
