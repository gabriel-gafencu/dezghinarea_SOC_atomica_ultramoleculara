//#define CU_WD
//#define CU_TIMER_FARA_INT
#define CU_TIMER_CU_INT

#ifdef CU_WD
#include <iom16.h>
#include "usart.h"

__no_init unsigned int nrR;
__no_init unsigned char flagWasWDR;
__no_init unsigned char flagWasFirstReset;
void main()
{
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  // daca a avut loc orice Power-on Reset sau JTAG Reset sau Brown-out Reset, initializez nrR cu 0 si tin minte reset-ul prin flagWasFirstReset
  if(MCUCSR & ((1 << PORF) | (1 << BORF) | (1 << JTRF)))
  {
    nrR = 0;
    
    // bitii trebuie resetati manual
    MCUCSR &= ~((1 << PORF) | (1 << BORF) | (1 << JTRF));
    
    // resetare flagWasWDR
    flagWasWDR = 0;
    
    // setare flagWasFirstReset
    flagWasFirstReset = 1;
  }
  
  // tin minte daca a avut loc un WDR pentru a compara durata intre reset-urile externe cu 1s
  if(MCUCSR & (1 << WDRF))
  {
    flagWasWDR = 1;
    
    // resetez bitul WDRF
    MCUCSR &= ~(1 << WDRF);
  }
  
  // daca a avut loc un Reset Extern, incrementez variabila __no_init nrR si transmit "early" sau "late"
  if(MCUCSR & (1 << EXTRF))
  {
    // Setare perioadã de time-out 1s
    WDTCR |= (1 << WDP2) | (1 << WDP1);
    
    // Activare watchdog timer
    WDTCR |=(1 << WDE);
    
    nrR = (nrR + 1) % 10;
    
    // transmit pe seriala variabila nrR
    USART_transmit('0' + nrR);
    
    // resetez bitul EXTRF
    MCUCSR &= ~(1 << EXTRF);
    
    // transmit "early" sau "late", daca acesta nu este primul ResetExtern
    if(!flagWasFirstReset)
    {
      char earlyMessage[] = "early";
      char lateMessage[] = "late";
      USART_transmit('-');
      if(flagWasWDR)
      {
        for(int i=0;i<4;++i)
          USART_transmit(lateMessage[i]);
        flagWasWDR = 0;
      }
      else
      {
        for(int i=0;i<5;++i)
          USART_transmit(earlyMessage[i]);
      }
    }
    
    // transmit '\n'
    USART_transmit(0xA);
    USART_transmit(0xD);
    
    // resetare flagWasFirstReset
    flagWasFirstReset = 0;
  }
  
  while(1)
  {
    
  }
}
#endif




// ***********************************************************************************************************************************************

#ifdef CU_TIMER_FARA_INT
#include <iom16.h>
#include "usart.h"

void timer1_start()
{
  // setam Timer/Counter 1 
  // in modul NORMAL => WGM1[3:0] = 4b'0000
  // cu prescaler 1024 => CS1[2:0] = 3'b101
  // non-PWM mode cu OC1A/OC1B disconnected => COM1A[1:0] = COM1B[1:0] = 2'b00
  TCCR1B |= (1 << CS10) | (1 << CS12);
}
void timer1_stop()
{
  // setam Timer/Counter 1 
  // in modul NORMAL => WGM1[3:0] = 4b'0000
  // fara clock => CS1[2:0] = 3'b000
  // non-PWM mode cu OC1A/OC1B disconnected => COM1A[1:0] = COM1B[1:0] = 2'b00
  TCCR1B = 0;
}

__no_init unsigned int nrR;
__no_init unsigned int flag_timer1_mission_complete;
__no_init unsigned char flag_was_first_reset;
void main()
{
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  // daca a avut loc orice Power-on Reset sau JTAG Reset sau Brown-out Reset, initializez nrR cu 0 si tin minte reset-ul prin flagWasFirstReset
  if(MCUCSR & ((1 << PORF) | (1 << BORF) | (1 << JTRF)))
  {
    nrR = 0;
    
    // bitii trebuie resetati manual
    MCUCSR &= ~((1 << PORF) | (1 << BORF) | (1 << JTRF));
    
    // setare flagWasFirstReset
    flag_was_first_reset = 1;
  }
  
  // daca a avut loc un Reset Extern, incrementez variabila __no_init nrR si transmit "early" sau "late", in functia de valoarea salvata anterior din timer1
  if(MCUCSR & (1 << EXTRF))
  {
    // pornesc timer1 si resetez flag-ul
    timer1_start();
    
    // iau in calcul Reset-ul Extern
    nrR = (nrR + 1) % 10;
    
    // transmit pe seriala variabila nrR
    USART_transmit('0' + nrR);
    
    // resetez bitul EXTRF
    MCUCSR &= ~(1 << EXTRF);
    
    // transmit "early" sau "late", daca acesta nu este primul ResetExtern
    if(!flag_was_first_reset)
    {
      char earlyMessage[] = "early";
      char lateMessage[] = "late";
      USART_transmit('-');
      if(flag_timer1_mission_complete)
      {
        for(int i=0;i<4;++i)
          USART_transmit(lateMessage[i]);
      }
      else
      {
        for(int i=0;i<5;++i)
          USART_transmit(earlyMessage[i]);
      }
    }
    
    // transmit '\n'
    USART_transmit(0xA);
    USART_transmit(0xD);
    
    // resetare flagWasFirstReset
    flag_was_first_reset = 0;
    
  }
  
  // resetare flag_timer1_mission_complete
  flag_timer1_mission_complete = 0;
  while(1)
  {
    // daca Timer/Counter 1 depasteste valoarea 3906, corespunzatoare unui interval de secunda, tin minte intr-un __no_init flag si-l opresc (nu-i mai dau clock)
    if(TCNT1 > 3906)
    {
      flag_timer1_mission_complete = 1;
      timer1_stop();
    } 
  }
}
#endif





// ************************************************************************************************************************************************

#ifdef CU_TIMER_CU_INT
#include <iom16.h>
#include <inavr.h>
#include "usart.h"

__no_init unsigned int nrR;

__no_init unsigned int MAX_nr_ovfs;
__no_init unsigned int MAX_val;

__no_init unsigned int last_nr_ovfs;
__no_init unsigned int last_val;

__no_init unsigned char flag_first_extern_reset;

unsigned int nr_ovfs = 0; // contorizam numarul de ovfs intr-o executie

#pragma vector = TIMER1_OVF_vect
__interrupt void ISR_TIMER1_OVF()
{
  ++nr_ovfs;
}

void timer1_start()
{  
  // setam Timer/Counter 1 
  // in modul NORMAL => WGM1[3:0] = 4b'0000
  // cu prescaler 1024 => CS1[2:0] = 3'b101
  // non-PWM mode cu OC1A/OC1B disconnected => COM1A[1:0] = COM1B[1:0] = 2'b00
  TCCR1B |= (1 << CS10) | (1 << CS12);
  
  // activarea intreruperii TIMER1_OVF
  TIMSK |= (1 << TOIE1);  
}

void main()
{
  unsigned long temp = 0;
  
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  if(MCUCSR & (1 << JTRF))
    USART_transmit('J');
  
  // daca a avut loc orice Power-on Reset sau JTAG Reset sau Brown-out Reset, initializez nrR cu 0 si tin minte reset-ul prin flagWasFirstReset
  if(MCUCSR & ((1 << PORF) | (1 << BORF) | (1 << JTRF)))
  {
    // initializez variabila de contorizare a Reset-urilor Externe
    nrR = 0;
    
    // flag-urile din MCUCSR trebuie resetati manual
    MCUCSR &= ~((1 << PORF) | (1 << BORF) | (1 << JTRF) | (1 << EXTRF)); // am resetat si EXTRF pentru ca la scrierea programului se seteaza si bitul EXTRF si s-ar fi trimis doi de 1 pe seriala (nu stiu de ce)

    // setez valorile default pentru MAX_ovfs si MAX_val ( corespunzatoare intervalului de 1s)
    MAX_nr_ovfs = 0;
    MAX_val = 3925;
    
    // resetez flag_first_extern_reset
    flag_first_extern_reset = 0;
  }
  
  // daca a avut loc un Reset Extern, incrementez variabila __no_init nrR si transmit "early" sau "late", in functia de valoarea salvata anterior din timer1
  if(MCUCSR & (1 << EXTRF))
  {
    USART_transmit('X');
    // pornesc timer1 si activez intreruperile la nivel global;
    timer1_start();
    __enable_interrupt();
    
    // iau in calcul Reset-ul Extern
    nrR = (nrR + 1) % 10;
    
    // transmit pe seriala variabila nrR
    USART_transmit('0' + nrR);
    
    // resetez bitul EXTRF
    MCUCSR &= ~(1 << EXTRF);
    
    // tin minte daca acesta este primul Reset Extern pentru ca in acest caz nu se poate calcul intervalul cerut
    if(!flag_first_extern_reset)
    {
      flag_first_extern_reset = 1;
    }
    else// transmit "early" sau "late" si intervalul de timp aferent, daca acesta nu este primul ResetExtern
    {
      char earlyMessage[] = "early";
      char lateMessage[] = "late";
      USART_transmit('-');
      
      // verific daca intervalul de timp dintre ultimele 2 reset-uri externe l-a depasit pe cel impus
      if((last_nr_ovfs > MAX_nr_ovfs) || (last_nr_ovfs == MAX_nr_ovfs && last_val > MAX_val))
      {
        for(int i=0;i<4;++i)
          USART_transmit(lateMessage[i]);
      }
      else
      {
        for(int i=0;i<5;++i)
          USART_transmit(earlyMessage[i]);
      }  
      
      // calculez si transmit si intervalul de timp dintre ultimele 2 reset-uri externe
      temp = last_nr_ovfs * 65536 + last_val;
      
      // calcul in ms (  temp *= 1024 * 248.75 / 1000000) 
      temp *= 0.25472; 
      
      // afisare interval de timp (inversare + trimitere pe serial)
      unsigned long temp_inversat = 0;
      while(temp)
      {
        temp_inversat = temp_inversat * 10 + temp % 10;
        temp /= 10;
      }
      USART_transmit('(');
      while(temp_inversat)
      {
        USART_transmit(temp_inversat % 10 + '0');
        temp_inversat /= 10;
      }
      USART_transmit('m');
      USART_transmit('s');
      USART_transmit(')');
    }
    
    // transmitere '\n'
    USART_transmit(0xA);
    USART_transmit(0xD);
  }
  
  unsigned int delta_t = 0;
  char aux;
  unsigned char pos_last_digit = 0;
  while(1)
  {
    // citesc cele 2 cifre de pe seriala
    if(USART_Receive(&aux) == 0)
    {
      if(aux >= '0' && aux <= '9')
      {
        delta_t = delta_t * 10 + aux - '0';
        ++pos_last_digit;
      }
    }
    
    if(pos_last_digit == 2)
    {
      // calcul MAX_ovfs si MAX_val in functie de delta_t (folosim un unsigned long pentru ca (3926 * delta_t) poate depasi maximul pe 16 biti)
      if(delta_t)
      {
        temp = 3926 * delta_t / 65536;
        MAX_nr_ovfs = temp;
        temp = 3926 * delta_t % 65536 - 1;
        MAX_val = temp;
      }
      
      // resetare delta_t si pos_last_digit pentru o eventuala noua citire
      pos_last_digit = 0;
      delta_t = 0;
    }
    
    // salvez mereu statusul timer-ului pentru a avea(aproximativ) ultima lui stare inainte de urmatorul reset
    last_val = TCNT1;
    last_nr_ovfs = nr_ovfs;
  }
}
#endif