#ifndef IR_PRINTF_H
#define IR_PRINTF_H


	enum TYPE
	{
		INT,
		LONG,
		STRING,
		FLOAT,
		UNSIGNED,
		HEX
	};
	

	void printInt(int number);
	void printLong(long number);
	void printString(unsigned char*str);
	void printFloat(float number);
	void PRINTF(void*data,enum TYPE type);
	void printUnsignedInt(unsigned int number);
	void printHex(unsigned  int data);
#endif