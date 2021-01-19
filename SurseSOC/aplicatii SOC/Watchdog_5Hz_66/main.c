#include <inavr.h>
#include <iom16.h>

// starea in care se afla forma de unda (pe 1 - front pozitiv, sau pe 0 - front negativ)
__no_init unsigned char stare;

// f = 5 Hz -> T = 200 ms
// F.U. = 66% 
// => T1 = 132 ms (WDP = 0 1 1)
// T0 = 68 ms (WDP = 0 1 0)

int main(void)
{
  // setam pinul PD5 ca pin de iesire
  DDRD |= (1 << PD5);
  
  // valoare initiala pentru Timer1
  TCNT1 = 0;
  
  // pornim timer-ul Timer/Counter1 cu prescaler 1/64
  TCCR1B |= (1 << CS11) | (1 << CS10);
  
  if (stare == 1) // pe frontul pozitiv
  {
    TCNT1 = 0; // resetare Timer1
    PORTD |= (1 << PD5); // scriem valoarea 1 logic (HIGH) pe pinul PD5
    while (TCNT1 < 2125); // asteptam inca 34 ms (2125 de numarari ale TC1) pentru corectia frecventei
    WDTCR |= (1 << WDP1) | (1 << WDP0); // setam perioada de timeout de 132 ms
    stare = 0;
  }
  else
  {
    TCNT1 = 0; // resetare Timer1
    PORTD &= ~(1 << PD5); // scriem valoarea 0 logic (LOW) pe pinul PD5
    while (TCNT1 < 1188); // asteptam inca 19 ms (1188 de numarari ale TC1) pentru corectia frecventei
    WDTCR |= (1 << WDP1); // setam perioada de timeout de 68 ms
    stare = 1;
  }
  
  // pornim Watchdog-ul
  WDTCR |= (1 << WDE);
  
  while(1) {}
}