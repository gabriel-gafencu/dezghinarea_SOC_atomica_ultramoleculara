#include <inavr.h>
#include <iom16.h>
void timer0_init()
{
  // initializare TCCR0
  TCCR0 |= (1<<WGM00)|(1<<COM01)|(1<<WGM01)|(1<<CS00);
  // setare pinul OC0 ca output (este pinul PB3)
  DDRB |= (1<<PB3);
}
void main()
{
  unsigned char duty;
  duty = 191; // duty cycle = 45% si deoarece Timer 0 este pe 8 biti acesta poate numara pana la 255. Deci 45 din 255 = 114.75 = 115
  timer0_init();// initializarea timer-ului
  while(1) // crearea unei bucle infinite
  {
    OCR0 = duty; //Output Compare registrer
  }
}