#ifndef __CRC__
#define __CRC__

#include<iom16.h>
#include<inavr.h>

#define CRC16_CCITT 0x1021
enum BitOrder { LSBF, MSBF };

unsigned int crc16(unsigned int polinom16, unsigned int init_val_16, unsigned int adr_start,unsigned int len, enum BitOrder ord);
unsigned int crc16wtable(unsigned int init_val_16,unsigned int adr_start, unsigned int len);

#endif