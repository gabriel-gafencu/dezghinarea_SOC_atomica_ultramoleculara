// aprindere LED o data la 1 s (0.5s OFF, 0.5s OFF)
#include <inavr.h>
#include <ioavr.h>

void main(void)
{
  // setam pinul de iesire ca fiind pinul 1 al portului D
  DDRD = (1 << PD1);
  
  // 1 ciclu instructiune dureaza 1 / 4MHz = 0.25 microsecunde
  // x cicli ............................... 0.5 secunde
  // x = 2000000 cicli
  // trebuie sa apelam delay_cicles() cu parametrul 2000000 de cicli
  
  while (1)
  {
    // aprindem LED-ul
    PORTD = 2; // se seteaz� valoarea �1� logic pe pinul de iesire 
              // 2(DEC)=0b00000010, ceea ce seteaz� pinul PD1 (al doilea pin din portul D) cu valoarea de �1� logic 
              // PORTD=0x02; 
              // echivalentul �n hexa
    
    // asteptam 0.5s
    __delay_cycles(2000000); // intrerupem executia pentru 0.5s (2000000 de cicli procesor)
    
    // stingem LED-ul
    PORTD = 0; // set�m toti pinii portului D pe �0� logic, inclusiv pinul PD1. 
              // Avem astfel 0 Volti la ie�irea acestuia ceea ce va determina stingerea ledului.
              // �n acest mod se realizeaz� operatia de toggle (�nchis/deschis)
    
    // iar asteptam 0.5s
    __delay_cycles(2000000); // intrerupem executia pentru 0.5s (2000000 de cicli procesor)
  }
}