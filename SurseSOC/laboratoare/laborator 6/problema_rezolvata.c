#include <inavr.h>
#include <iom16.h>

typedef unsigned short uint8_t;

void timer0_init()
{
  // initializare TCCR0
  
  // modul fast PWM => WGM01:0 = 1
  TCCR0 |= (1<<WGM00) | (1<<WGM01);
  
  // PWM non-inverted => COM01:0 = 2
  TCCR0 |= (1<<COM01);
  
  // folosire clock intern ATMega (4 MHz)
  TCCR0 |= (1<<CS00);
  
  // setare pinul OC0 ca output (este pinul PB3)
  DDRB |= (1<<PB3);
}
void main()
{
  uint8_t duty;
  duty = 115; // duty cycle = 45% si deoarece Timer 0 este pe 8 biti acesta poate numãra pânã la 255. 
  			// Deci 45% din 255 = 114.75 = 115
  
  timer0_init();// initializarea timer-ului
  
  while(1) // crearea unei bucle infinite
  {
    OCR0 = duty; // Output Compare Register
    // Timer/Counter0 va numara de la 0 la 255
    // cand numaratoarea (TCNT0) ajunge la valoarea din OCR0 (duty), se declanseaza iesirea OC0 (trece din 1 in 0)
    // apoi, cand se ajunge la 255, overflow, si Timer1 se reseteaza (inclusiv OC0)
  }
}