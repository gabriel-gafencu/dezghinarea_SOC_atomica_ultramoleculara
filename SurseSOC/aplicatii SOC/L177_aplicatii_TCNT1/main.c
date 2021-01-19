#include <iom16.h>
#include <inavr.h>
#include "usart.h"
#include "timer1.h"
void main()
{
  // initializam timer-ul
  timer0_init();
  
  // initializam modulul USART
  USART_initialize(BAUD_RATE);
  
  // setam pinul OC1A = PD5 de iesire
  DDRD |= (1 << PD5);
  
  // folosim intreruperea TIMER1_OVF pentru a comuta un LED la fiecare 1000 ovf si pt a retine nr de ovf
  // setam pinul PD2 de iesire
  DDRD |= (1 << PD2);
  // activarea globala a intreruperilor
  __enable_interrupt();
  
  // variabila in care retinem comanda de pe serial
  char command = 'x';
  // variabila in care retinem statusul actual al frecventei (cate incrementari/decrementari s-au facut)
  char nrIncDecFreq = 1; // 1 corespunde frecventei intiale de 1 kHz
  char nrIncDecDC = 1; // 1 corespunde DC initial = 10%
  ICR1 = 4000;
  // unsigned int temp; // folosit pentru a stoca temporar rezultatul unor calcule care depasesc 16 biti
  // OCR1A = ICR1 * 50 / 100; ? rezultatul temporar se pastreaza pe ceva mai mare de 2^16?
  OCR1A = ICR1 / 10; // pt OCR1A = ICR1 / 20, reactioneaza ciudat
  while(1)
  {
    USART_receive(&command);
    if(command == '+')
    {
      command = 'x'; // pentru a bloca incrementarea urmatoare
      if(nrIncDecFreq < 10)
        ++nrIncDecFreq;
      if(nrIncDecDC < 10)
        ++nrIncDecDC;
      ICR1 = 4000 / nrIncDecFreq;
      OCR1A = ICR1 / 10 * nrIncDecDC; // mai intai impartim, pentru a nu avea depasire
    }
    if(command == '-')
    {
      command = 'x';
      if(nrIncDecFreq > 1)
        --nrIncDecFreq;
      if(nrIncDecDC > 1)
        --nrIncDecDC;
      ICR1 = 4000 / nrIncDecFreq;
      OCR1A = ICR1 / 10 * nrIncDecDC;
    }
  }
}