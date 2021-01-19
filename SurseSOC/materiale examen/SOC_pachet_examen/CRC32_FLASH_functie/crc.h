#ifndef __CRC__
#define __CRC__

#include<iom16.h>
#include<inavr.h>

#define CRC32_CCITT 0x4C11DB7
enum BitOrder { LSBF, MSBF };


unsigned long crc32(unsigned long polinom32, unsigned long init_val_32, unsigned int adr_start,unsigned int len, enum BitOrder ord);
unsigned long crc32wtable(unsigned int init_val_32, unsigned int adr_start, unsigned int len);

#endif