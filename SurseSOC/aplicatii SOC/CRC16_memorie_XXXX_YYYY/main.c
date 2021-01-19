// Se primeste pe seriala un sir de forma: 0xXXXX-0xXXXX
// Sa se calculeze CRC-ul zonei de memorie dintre acele adrese.

#include <iom16.h>
#include <inavr.h>
#include "usart.h"
#include "CRC16.h"
#include "myprint.h"

char data;
char flag;

// rutina de intrerupere la primirea de date pe seriala
#pragma vector = USART_RXC_vect
__interrupt void interrupt_routine_USART_RXC(void)
{
  // cand am primit date, le salvam in variabila data
  data = UDR;
  
  // setam flagul pe 1, adica exista ceva pe seriala
  flag = 1;
}

void main( void )
{
  // initializam interfata seriala
  USART_initialize(BAUD_RATE);
  
  // activam intreruperile globale
  __enable_interrupt();
  
  unsigned int i; // indicele caracterului primit pe seriala
  char c; // caracterul primit de pe seriala
  
  unsigned int start_adr = 0; // adresa de inceput
  unsigned int stop_adr = 0; // adresa de sfarsit
  
  i = 1;
  while(1)
  {    
    if (USART_Receive(&c) == 0) // primim data de pe serial (daca exista)
    {  
      if (c == '\n' || c == '\r') // ignoram caracterele albe de newline si carriage return
      {
        continue;
      }
      switch(i)
      {
      case 1: // 0 -> ignoram
      case 2: // x -> ignoram
        break;
      case 3: // prima cifra hexa (adresa de inceput)
        start_adr += (c <= '9' && c >= '0') ? ((c - '0') << 12) : ((c - 'A' + 10) << 12);
        break;
      case 4: // a doua cifra hexa (adresa de inceput)
        start_adr += (c <= '9' && c >= '0') ? ((c - '0') << 8) : ((c - 'A' + 10) << 8);
        break;
      case 5: // a treia cifra hexa (adresa de inceput)
        start_adr += (c <= '9' && c >= '0') ? ((c - '0') << 4) : ((c - 'A' + 10) << 4);
        break;
      case 6: // a patra cifra hexa (adresa de inceput)
        start_adr += (c <= '9' && c >= '0') ? (c - '0') : (c - 'A' + 10);
        break;
      case 7: // '-' -> ignoram
      case 8: // 0 -> ignoram
      case 9: // x -> ignoram
        break;
      case 10: // prima cifra hexa (adresa de sfarsit)
        stop_adr += (c <= '9' && c >= '0') ? ((c - '0') << 12) : ((c - 'A' + 10) << 12);
        break;
      case 11: // a doua cifra hexa (adresa de sfarsit)
        stop_adr += (c <= '9' && c >= '0') ? ((c - '0') << 8) : ((c - 'A' + 10) << 8);
        break;
      case 12: // a treia cifra hexa (adresa de sfarsit)
        stop_adr += (c <= '9' && c >= '0') ? ((c - '0') << 4) : ((c - 'A' + 10) << 4);
        break;
      case 13: // a patra cifra hexa (adresa de sfarsit)
        stop_adr += (c <= '9' && c >= '0') ? (c - '0') : (c - 'A' + 10);
      }
    }
    else
    {
      continue;
    }
    
    if (i == 13) // daca am primit toate 11 caractere
    {
      i = 1; // o luam de la capat cu citirea
      
      // calculam CRC-ul dintre cele 2 adrese
      unsigned int my_crc16 = crc16(CRC16_CCITT, 0, start_adr, (stop_adr - start_adr), MSBF);
      
      start_adr = stop_adr = 0;
      
      // il trimitem pe seriala
      myprint(0, 5, &my_crc16);
    }
    else
    {
      ++i;
    }
  }
}