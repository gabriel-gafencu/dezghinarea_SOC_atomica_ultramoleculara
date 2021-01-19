//#define v1
//#define v2
#define v3

#ifdef v1
#include "usart.h"
#include "myprint.h"

volatile unsigned int nr_t;
#pragma vector = INT1_vect
__interrupt void ISR_INT0()
{
  // intreruperea se apeleaza la fiecare front pozitiv, deci de fiecare data trebuie sa preluam valoarea TCNT1 si sa-l resetam
  nr_t = TCNT1;
  TCNT1 = 0;
}

void timer1_init()
{
  // in modul normal => WGM1[3:0] = 4b'0000
  // fara prescaler => CS1[2:0] = 3'b001
  TCCR1B |= (1 << CS10);
}

void main()
{ 
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  // initializare TIMER/COUNTER1
  timer1_init();
  
  // activarea intreruperii externe INT1
  GICR |= (1 << INT1);
  
  // configurarea modului de activare a intreruperii INT1 (activare la front pozitiv)
  MCUCR |= (1 << ISC11) | (1 << ISC10);
  
  // activarea globala a intreruperilor
  __enable_interrupt();
  
  while(1)
  {
    if(nr_t) // daca nr_t este nenul, inseamna ca timer-ul a terminat de numarat o perioada a semnalului de intrare
    {
      // perioada MCU = 248.6382 ns
      unsigned int period = (unsigned int) (nr_t * 0.2486382); // in microsecunde
      if(period < 10)
        myprint(4,1,&period);
      else if(period < 100)
        myprint(4,2,&period);
      else if(period < 1000)
        myprint(4,3,&period);
      else if(period < 10000)
        myprint(4,4,&period);
      else
        myprint(4,5,&period);
      USART_transmit('u');
      USART_transmit('s');
      USART_transmit(0xA);
      USART_transmit(0xD);
    }
  }
}
#endif

// *********************************************************************************************************************************************

#ifdef v2
#include "usart.h"
#include "myprint.h"

#define ULONG_MAX 4294967296

unsigned char flag_TC1_is_counting = 0;
unsigned long sum_of_TC1_vals = 0;
unsigned int nr_of_TC1_vals = 0;
unsigned int temp;

#pragma vector = INT1_vect
__interrupt void ISR_INT1()
{
  temp = TCNT1;
  TCNT1 = 0;
  if(sum_of_TC1_vals < ULONG_MAX - temp)
  {
    sum_of_TC1_vals += temp;
    ++nr_of_TC1_vals;
  }
}

unsigned char TC0_nr_ovfs = 0;
unsigned char flag_one_sec = 0;

#pragma vector = TIMER0_OVF_vect
__interrupt void ISR_TC0_ovf()
{
  ++ TC0_nr_ovfs;
  if(TC0_nr_ovfs >= 16) // pentru a obtine 1s
  {
    flag_one_sec = 1;
    TC0_nr_ovfs = 0;
  }
}


void timer1_init()
{
  // in modul normal => WGM1[3:0] = 4b'0000
  // fara prescaler => CS1[2:0] = 3'b001
  // non-PWM mode cu OC1A/OC1B disconnected => COM1A[1:0] = COM1B[1:0] = 2'b00
  TCCR1B |= (1 << CS10);
}
void timer0_init()
{
  // in modul normal => WGM0[1:0] = 2b'00
  // prescaler 1024 => CS0[2:0] = 3'b101
  // Normal port operation, OC0 disconnected => COM0[1:0] = 2'b00
  TCCR0 |= (1 << CS02) | (1 << CS00);
  
  // activarea intreruperii de ovf pentru a contoriza aproximativ o secunda
  TIMSK |= (1 << TOIE0);
}

void main()
{ 
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  // initializare TIMER/COUNTER1
  timer1_init();
  
  // initializare TIMER/COUNTER0
  timer0_init();
  
  // activarea intreruperii externe INT1
  GICR |= (1 << INT1);
  
  // configurarea modului de activare a intreruperii INT1 (activare la front pozitiv)
  MCUCR |= (1 << ISC11) | (1 << ISC10);
  
  // activarea globala a intreruperilor
  __enable_interrupt();
  
  while(1)
  {
    if(flag_one_sec) // daca timer-ul a terminat de numarat o perioada a semnalului de intrare
    {
      // perioada MCU = 248.74 ns
      unsigned long mean_period = (unsigned long)(sum_of_TC1_vals / nr_of_TC1_vals + 1) * 24874 / 100000; // in microsecunde; + 1 pentru ca iau in calcul numarul de perioade numarate
      // unsigned int period = (unsigned int) ((TC1_val + 1) * 0.24874); 
      transmit_ulong(mean_period);
      
      USART_transmit('u');
      USART_transmit('s');
      USART_transmit(0xA);
      USART_transmit(0xD);
      
      // resetari
      flag_one_sec = 0;
      sum_of_TC1_vals = 0;
      nr_of_TC1_vals = 0;
    }
  }
}
#endif


// *********************************************************************************************************************************************

#ifdef v3
#include "usart.h"
#include "myprint.h"

#define ULONG_MAX 4294967296

unsigned char flag_TC1_is_counting = 0;
unsigned long sum_of_TC1_vals = 0;
unsigned int nr_of_TC1_vals = 0;
unsigned int temp;

#pragma vector = INT1_vect
__interrupt void ISR_INT1()
{
  sum_of_TC1_vals += TCNT1;
  TCNT1 = 0;
  ++nr_of_TC1_vals;
  
}

unsigned char TC0_nr_ovfs = 0;
unsigned char flag_one_sec = 0;

#pragma vector = TIMER0_OVF_vect
__interrupt void ISR_TC0_ovf()
{
  ++ TC0_nr_ovfs;
  if(TC0_nr_ovfs >= 16) // pentru a obtine 1s
  {
    flag_one_sec = 1;
    TC0_nr_ovfs = 0;
  }
}


void timer1_init()
{
  // in modul normal => WGM1[3:0] = 4b'0000
  // fara prescaler => CS1[2:0] = 3'b001
  // non-PWM mode cu OC1A/OC1B disconnected => COM1A[1:0] = COM1B[1:0] = 2'b00
  TCCR1B |= (1 << CS10);
}
void timer0_init()
{
  // in modul normal => WGM0[1:0] = 2b'00
  // prescaler 1024 => CS0[2:0] = 3'b101
  // Normal port operation, OC0 disconnected => COM0[1:0] = 2'b00
  TCCR0 |= (1 << CS02) | (1 << CS00);
  
  // activarea intreruperii de ovf pentru a contoriza aproximativ o secunda
  TIMSK |= (1 << TOIE0);
}

void main()
{ 
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  // initializare TIMER/COUNTER1
  timer1_init();
  
  // initializare TIMER/COUNTER0
  timer0_init();
  
  // activarea intreruperii externe INT1
  GICR |= (1 << INT1);
  
  // configurarea modului de activare a intreruperii INT1 (activare la front pozitiv)
  MCUCR |= (1 << ISC11) | (1 << ISC10);
  
  // activarea globala a intreruperilor
  __enable_interrupt();
  
  while(1)
  {
    if(flag_one_sec) // daca timer-ul a terminat de numarat o perioada a semnalului de intrare
    {
      // perioada MCU = 248.74 ns
      unsigned long mean_period = (unsigned long)(sum_of_TC1_vals / nr_of_TC1_vals + 1) * 24874 / 100000; // in microsecunde; + 1 pentru ca iau in calcul numarul de perioade numarate
      transmit_ulong(mean_period);
      
      USART_transmit('u');
      USART_transmit('s');
      USART_transmit(0xA);
      USART_transmit(0xD);
      
      // resetari
      flag_one_sec = 0;
      sum_of_TC1_vals = 0;
      nr_of_TC1_vals = 0;
    }
  }
}
#endif