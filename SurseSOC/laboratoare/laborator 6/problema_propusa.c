#include <inavr.h>
#include <iom16.h>

void Init()
{
  // Timer Clock = CPU Clock (fãrã prescalare)
  TCCR0 |= (1<<CS00);
  
  // Mode = Fast PWM
  TCCR0 |= (1<<WGM00) | (1<<WGM01);
  
  // PWM neinversat
  TCCR0 |= (1<<COM01);
  
  // Setare pin OC0 (adica PB3) ca output
  DDRB |= (1<<PB3);
}

void SetOutput(unsigned int duty)
{
  OCR0 = duty; // Output Compare registrer
}

void Wait() // este necesarã o functie de asteptare
{
  __delay_cycles(3200); // asteptare 6.4 ms
}

void main()
{
  unsigned int brightness=0;
  
  // initializare Timer/Counter0
  Init();
  
  while(1)
  {
    // luminozitatea variaza crescator intre 0 si 254 (de la 0V la 5V)
    for(brightness = 0; brightness < 255; brightness++)
    {
      // TC0 va numara pana la valoarea "brightness" curenta
      SetOutput(brightness);

      // inainte de a trece la o noua valoare a luminozitatii, se asteapta 6.4 ms
      Wait();
    }

    // luminozitatea variaza descrescator de la 255 la 1 (de la 5V la 0V)
    for(brightness = 255; brightness > 0; brightness--)
    {
      // TC0 va numara pana la valoarea "brightness" curenta
      SetOutput(brightness);

      // inainte de a trece la o noua valoare a luminozitatii, se asteapta 6.4 ms
      Wait();
    }
  }
}
