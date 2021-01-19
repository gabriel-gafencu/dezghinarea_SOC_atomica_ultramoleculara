#ifndef functii_h
#define functii_h
#define CRC16_CCITT 0x1021 
#define LSBF 0
#define MSBF 1


unsigned int crc16(unsigned int polinom16, unsigned int init_val_16,unsigned int adr_start,unsigned int len, int ord);
unsigned char min(unsigned char x, unsigned char y);
unsigned int lungimeFunctie(unsigned int adr);
void myprint(unsigned int tip, unsigned int nr_car, void * val);
void integerTransmit (unsigned int p1, unsigned int p2, void * p3);
void hexadecimalTransmit (unsigned int p1, unsigned int p2, void * p3);
void doubleTransmit(unsigned int p1, unsigned int p2, void * p3);
void floatTransmit(unsigned int p1, unsigned int p2, void * p3);
void characterTransmit (unsigned int p1, unsigned int p2, void * p3);

#endif