#include <inavr.h> //aceasta biblioteca contine prototipurile functiilor delay_cycles(), enable_interrupt(),disable_interrupt() etc.
#include <ioavr.h> //contine definitiile functiilor pentru input/output
int main(void)
{
  DDRB = DDRB | (1 << PB7); //seteaza pinul PB7 ca pin de iesire
  while(1)
  {
    PORTB = PORTB | (1 << PB7); // se seteaza valoarea '1' logic pe pinul de iesire PB7
    __delay_cycles(2000000); //numarul de cicli echivalenti pentru 0,5s
    PORTB = PORTB & (~(1 << PB7));// se seteaza valoarea '0' logic pe pinul de iesire PB7
    __delay_cycles(2000000);
  }
 return 0;
}