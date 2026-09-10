#include "stdafx.h"
#include "CCRC32.h"

CCRC32::CCRC32(void)
{
	this->Initialize();
}

void CCRC32::Initialize(void)
{
	unsigned long ulPolynomial = 0x04C11DB7;

	for (int iCodes = 0; iCodes <= 0xFF; iCodes++)
	{
		this->ulTable[iCodes] = this->Reflect(iCodes, 8) << 24;

		for (int iPos = 0; iPos < 8; iPos++)
		{
			this->ulTable[iCodes] = (this->ulTable[iCodes] << 1)
				^ ((this->ulTable[iCodes] & (1 << 31)) ? ulPolynomial : 0);
		}

		this->ulTable[iCodes] = this->Reflect(this->ulTable[iCodes], 32);
	}
}

unsigned long CCRC32::Reflect(unsigned long ulReflect, const char cChar)
{
	unsigned long ulValue = 0;

	for (int iPos = 1; iPos < (cChar + 1); iPos++)
	{
		if (ulReflect & 1)
		{
			ulValue |= (1 << (cChar - iPos));
		}
		ulReflect >>= 1;
	}

	return ulValue;
}

void CCRC32::PartialCRC(unsigned long* ulCRC, const unsigned char* sData, unsigned long ulDataLength)
{
	while (ulDataLength--)
	{
		*(unsigned long*)ulCRC =
			((*(unsigned long*)ulCRC) >> 8) ^ this->ulTable[((*(unsigned long*)ulCRC) & 0xFF) ^ *sData++];
	}
}

void CCRC32::FullCRC(const unsigned char* sData, unsigned long ulDataLength, unsigned long* ulOutCRC)
{
	*(unsigned long*)ulOutCRC = 0xffffffff; //Initilaize the CRC.
	this->PartialCRC(ulOutCRC, sData, ulDataLength);
	*(unsigned long*)ulOutCRC ^= 0xffffffff; //Finalize the CRC.
}

unsigned long CCRC32::FullCRC(const unsigned char* sData, unsigned long ulDataLength)
{
	unsigned long ulCRC = 0xffffffff; //Initilaize the CRC.
	this->PartialCRC(&ulCRC, sData, ulDataLength);
	return(ulCRC ^ 0xffffffff); //Finalize the CRC and return.
}

bool CCRC32::FileCRC(const char* sFileName, unsigned long* ulOutCRC, unsigned long ulBufferSize)
{
	*(unsigned long*)ulOutCRC = 0xffffffff; //Initilaize the CRC.

	FILE* fSource = NULL;
	unsigned char* sBuf = NULL;
	int iBytesRead = 0;

	if (fopen_s(&fSource, sFileName, "rb") != NULL)
	{
		return false; //Failed to open file for read access.
	}

	if (!(sBuf = (unsigned char*)malloc(ulBufferSize))) //Allocate memory for file buffering.
	{
		fclose(fSource);
		return false; //Out of memory.
	}

	while ((iBytesRead = fread(sBuf, sizeof(char), ulBufferSize, fSource)))
	{
		this->PartialCRC(ulOutCRC, sBuf, iBytesRead);
	}

	free(sBuf);
	fclose(fSource);

	*(unsigned long*)ulOutCRC ^= 0xffffffff; //Finalize the CRC.

	return true;
}

bool CCRC32::FileCRC(const char* sFileName, unsigned long* ulOutCRC)
{
	return this->FileCRC(sFileName, ulOutCRC, 1048576);
}