#include "stdafx.h"
#include "HackCheck.h"
#include "ServerInfo.h"
#include "ThemidaSDK.h"

BYTE EncDecKeySend[2];
BYTE EncDecKeyRead[2];
char MHPCustomerName[32];

void DecryptData(BYTE* lpMsg,int size)
{
	//if(EncDecKeyRead[0] != 0 || EncDecKeyRead[1] != 0)
	//{
	//	MHPDecryptData(lpMsg,size);
	//}

	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^ EncDecKeySend[0]) - EncDecKeySend[1];
	}
}

void EncryptData(BYTE* lpMsg,int size)
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+ EncDecKeySend[1])^ EncDecKeySend[0];
	}

	//if(EncDecKeyRead[0] != 0 || EncDecKeyRead[1] != 0)
	//{
	//	MHPEncryptData(lpMsg,size);
	//}
}

void MHPDecryptData(BYTE* lpMsg,int size)
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^ EncDecKeyRead[0]) - EncDecKeyRead[1];
	}
}

void MHPEncryptData(BYTE* lpMsg,int size)
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+ EncDecKeyRead[1]) ^ EncDecKeyRead[0];
	}
}

void InitHackCheck()
{
	VM_START

	WORD EncDecKey = 0;

	for(int n=0;n < sizeof(gServerInfo.m_CustomerName);n++)
	{
		EncDecKey += (BYTE)(gServerInfo.m_CustomerName[n]^gServerInfo.m_ServerSerial[(n%sizeof(gServerInfo.m_ServerSerial))]);
	}

	EncDecKeySend[0] = (BYTE)0xF1;
	EncDecKeySend[1] = (BYTE)0x1A; 

	EncDecKeySend[0] += LOBYTE(EncDecKey);
	EncDecKeySend[1] += HIBYTE(EncDecKey);

	GetPrivateProfileString("MHPServerInfo","CustomerName","",MHPCustomerName,sizeof(MHPCustomerName),"..\\Data\\Hack\\MHPServer.ini");
	EncDecKeyRead[0] = GetPrivateProfileInt("MHPServerInfo", "EncDecKey1", 0, "..\\Data\\Hack\\MHPServer.ini");
	EncDecKeyRead[1] = GetPrivateProfileInt("MHPServerInfo", "EncDecKey2", 0, "..\\Data\\Hack\\MHPServer.ini");

	if(EncDecKeyRead[0] != 0 || EncDecKeyRead[1] != 0)
	{
		WORD MHPEncDecKey = 0;

		for(int n=0;n < sizeof(MHPCustomerName);n++)
		{
			MHPEncDecKey += MHPCustomerName[n];
		}

		EncDecKeyRead[0] += LOBYTE(MHPEncDecKey);
		EncDecKeyRead[1] += HIBYTE(MHPEncDecKey);
	}

	VM_END
}
