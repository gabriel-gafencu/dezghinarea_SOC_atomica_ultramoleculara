
#include <iom16.h>
#include "usart.h"

void timer1_init()
{
  // setam Timer/Counter 1 
  // in modul FPWM cu TOP = ICR1 => WGM1[3:0] = 4b'1110
  // fara prescaler => CS1[2:0] = 3'b001
  // non-inverting mode => COM1A[1:0] = 2'b10
  TCCR1A |= (1 << WGM11) | (1 << COM1A1);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10);
}

void main()
{
  char stringErrorMessage[] = "Sir eronat!", aux = 'x';
  unsigned int i;
  unsigned int pos;
  unsigned int freq100;
  unsigned int DC;
  unsigned char flagError;
  
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  // initializare timer1
  timer1_init();
  
  // setam pinul OC1A (PD5) de iesire
  DDRD |= (1 << PD5);
  
  while(1)
  {
    flagError = 0;
    freq100 = 0;
    DC = 0;
    pos = 0;
    
    // citirea intregului sir de configurare de pe serial
    while(pos < 13 && !flagError)
    {
      if(USART_Receive(&aux) == 0)
      {
        USART_transmit(aux);
        switch(pos)
        {
        case 0:
          if(aux >= '1' && aux <= '9') 
            freq100 = freq100 * 10 + aux - '0';
          else
            flagError = 1;
          break;
        case 2:
        case 3:
          if(aux >= '0' && aux <= '9') 
            freq100 = freq100 * 10 + aux - '0';
          else
            flagError = 1;
          break;
        case 4:
        case 8:
        case 11:
          if(aux != ' ')
            flagError = 1;
          break;
        case 1:          
          if(aux != '.')
            flagError = 1;
          break;
        case 5:
          if(aux != 'k' && aux != 'K')
            flagError = 1;
          break;
        case 6:
          if(aux != 'H')
            flagError = 1;
          break;
        case 7:
          if(aux != 'z')
            flagError = 1;
          break;
        case 9:
        case 10:
          if(aux >= '0' && aux <= '9') 
            DC = DC * 10 + aux - '0';
          else
            flagError = 1;
          break;
        case 12:
          if(aux != '%' || DC == 0) // DC nu are voie sa fie 0, pentru ca frecventa este nenula
            flagError = 1;
          break;
        }
        ++pos;
      }
    }
    if(flagError)
    {
      // golesc buffer-ul de intrare
      while(USART_Receive(&aux) == 0)
        ;
      
      // afisez mesajul de eroare
      i=0;
      USART_transmit('-');
      while(stringErrorMessage[i]!='\0')
      { 
        USART_transmit(stringErrorMessage[i]);
        i++;
      }
    }
    else
    {
      // varianta 1, in care nu folosesc o variabila de tip unsigned long
      /*
      // generam semnalul corespunzator cu Timer1
      // transpunem totul in Hz si tinem cont de factorul de scalare al frecventei de pe serial
      ICR1 = 4021428 / (freq100 * 10) - 1;
      // OCR1A controleaza duty cycle-ul
      // pt. f = 1.00 kHz, ICR1 este maxim ~= 4000 si schimbam ordinea operatiilor pentru o precizie mai buna (pt. 16 biti, maximul este de 65535)
      // aplicam si o corectie
      OCR1A = ICR1 / 100 * DC - 1;
      // corectie + bonus
      OCR1A = OCR1A + (ICR1 % 100) * DC / 100 + ICR1 / 200;
      */ 
      unsigned long temp;
      temp = 4021428 / (freq100 * 10) - 1;
      ICR1 = temp;
      temp = temp * DC / 100 - 1;
      OCR1A = temp;
    }
    
    // transmit '\n'
    USART_transmit(0x0d);
    USART_transmit(0x0a);
  }
}