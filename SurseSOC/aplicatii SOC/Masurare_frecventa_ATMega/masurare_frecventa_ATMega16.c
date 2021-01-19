#include <iom16.h>
#include <inavr.h>

#define NR_CYCLES 499994
void main()
{
  // comutam un semnal pe un pin si in f de perioada semnalului generat, calculam frecventa reala a MCU
  DDRD |= (1 << PD5);
  while(1)
  {
    PORTD ^= (1 << PD5);
    __delay_cycles(NR_CYCLES);
  }
}