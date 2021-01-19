// Genereaza o frecventa de 50Hz folosind Timer/Counter0, factor de umplere 45%

#include <inavr.h>
#include <iom16.h>

typedef unsigned short uint8_t;

// folosim overflow-ul timer-ului pentru a seta TCNT0 sa inceapa de la 178
#pragma vector = TIMER0_OVF_vect
__interrupt void Timer0_ovf()
{
  TCNT0 = 178;
}

void timer0_init()
{
  // initializare TCCR0
  
  // modul fast PWM => WGM01:0 = 1
  TCCR0 |= (1<<WGM00) | (1<<WGM01);
  
  // PWM non-inverted => COM01:0 = 2
  TCCR0 |= (1<<COM01);
  
  // folosire prescaler 1/1024 (pentru a ne apropia de 50Hz)
  TCCR0 |= (1<<CS00 | 1<<CS02);

  /*
  Pentru a obtine 50Hz, adica o perioada de 20ms,
  trebuie sa incepem numararea de la valoarea 178.
  Pentru asta, la overflow, setam manual TCNT0 pe 178.
  Adica, Timer/Counter-ul va numara practic doar 78 de valori.
  Factorul de umplere va insemna, deci, doar 35 de valori numarate,
  adica vom seta OCR0 ca fiind 213.
  */
  
  // setare pinul OC0 ca output (este pinul PB3)
  DDRB |= (1<<PB3);
}
void main()
{
  // activam intreruperile globale
  __enable_interrupt();
  
  // activam intreruperea TOV0
  TIMSK |= (1 << TOIE0);
  
  uint8_t duty;
  duty = 213; // duty cycle = 45% si deoarece Timer0 este pe 8 biti acesta poate numãra pânã la 255. 
  			// deoarece am inceput numaratoarea de la 178, factorul de umplere se va calcula doar
  			// in intervalul [178, 255]. 45% din acest interval este valoarea 213
  
  timer0_init(); // initializarea timer-ului
  
  while(1) // crearea unei bucle infinite
  {
    OCR0 = duty; // Output Compare Register
    // Timer/Counter0 va numara de la 178 la 255
    // cand numaratoarea (TCNT0) ajunge la valoarea din OCR0 (duty), se declanseaza iesirea OC0 (trece din 1 in 0)
    // apoi, cand se ajunge la 255, overflow, si Timer1 se reseteaza (inclusiv OC0);
    // pentru ca aici am folosit intreruperea, la resetare, fortam TCNT0 sa inceapa de la 178
  }
}