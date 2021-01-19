#include <inavr.h>
#include <iom16.h>
#include "usart.h"

// contorizeaza numarul de apasari ale tastei RESET
__no_init int nrApasari;

void main()
{
  // la reset de power on, setam variabila globala pe 0
  if ((MCUCSR & (1 << PORF)) == (1 << PORF))
  {
    // reset de la power on (adica placa a fost bagata in priza)
    nrApasari = 0;
    MCUCSR &= 0xFE; // resetam bit-ul PORF
  }
  else if ((MCUCSR & (1 << EXTRF)) == (1 << EXTRF))
  {
    // reset de la buton (adica am apasat noi pe reset)
    ++nrApasari;
    if (nrApasari == 10)
    {
      nrApasari = 0;
    }
    
    USART_initialize(BAUD_RATE); // rata de biti de pe seriala
   
    // trimitem valoarea pe seriala
    USART_transmit(nrApasari + '0');
    USART_transmit('\r');
    USART_transmit('\n');
  }
  
  // blocam aplicatia pana la urmatorul reset
  while(1){}
}