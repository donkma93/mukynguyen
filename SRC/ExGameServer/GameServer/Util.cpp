// Revised: 31/05/2023 23:00 GMT-3

#include "stdafx.h"
#include "Util.h"
#include "GameMain.h"
#include "HackCheck.h"
#include "ItemManager.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "ThemidaSDK.h"
#include "Viewport.h"
#include "SocketManagerModern.h"
#include "resource.h"

std::mt19937 seed;
std::uniform_int_distribution<int> dist;
short RoadPathTable[MAX_ROAD_PATH_TABLE] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0 };

int SafeGetItem(int index)
{
	return CHECK_ITEM(index);
}

flt GetRoundValue(float value)
{
	float integral;

	if(modf(value,&integral) > 0.5f)
	{
		return ceil(value);
	}

	return floor(value);
}

BYTE GetNewOptionCount(BYTE NewOption)
{
	BYTE count = 0;

	for(int n=0;n < MAX_EXC_OPTION;n++)
	{
		if((NewOption & (1 << n)) != 0)
		{
			count++;
		}
	}

	return count;
}

BYTE GetSocketOptionCount(BYTE SocketOption[5])
{
	BYTE count = 0;

	for(int n=0;n < MAX_SOCKET_OPTION;n++)
	{
		if(SocketOption[n] != 0xFF)
		{
			count++;
		}
	}

	return count;
}

BYTE GetPathPacketDirPos(int px,int py)
{
	if(px <= -1 && py <= -1)
	{
		return 0;
	}
	else if(px <= -1 && py == 0)
	{
		return 7;
	}
	else if(px <= -1 && py >= 1)
	{
		return 6;
	}
	else if(px == 0 && py <= -1)
	{
		return 1;
	}
	else if(px == 0 && py >= 1)
	{
		return 5;
	}
	else if(px >= 1 && py <= -1)
	{
		return 2;
	}
	else if(px >= 1 && py == 0)
	{
		return 3;
	}
	else if(px >= 1 && py >= 1)
	{
		return 4;
	}

	return 0;
}

void PacketArgumentDecrypt(char* out_buff,char* in_buff,int size)
{
	BYTE XorTable[3] = {0xFC,0xCF,0xAB};

	for(int n=0;n < size;n++)
	{
		out_buff[n] = in_buff[n]^XorTable[n%3];
	}
}

void ErrorMessageBox(char* message,...)
{
	VM_START

	char buff[256];

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	MessageBox(0,buff,"Error",MB_OK | MB_ICONERROR);

	VM_END

	ExitProcess(0);
}

void LogAdd(eLogColor color,char* text,...)
{
	tm today;
	time_t ltime;
	time(&ltime);

	if(localtime_s(&today,&ltime) != 0)
	{
		return;
	}

	char time[32];

	if(asctime_s(time,sizeof(time),&today) != 0)
	{
		return;
	}

	char temp[1024];

	va_list arg;
	va_start(arg,text);
	vsprintf_s(temp,text,arg);
	va_end(arg);

	char log[1024];

	wsprintf(log,"%.8s %s",&time[11],temp);

	gServerDisplayer.LogAddText(color,log,strlen(log));
}

void LogAddConnect(eLogColor color,char* text,...)
{
	tm today;
	time_t ltime;
	time(&ltime);

	if(localtime_s(&today,&ltime) != 0)
	{
		return;
	}

	char time[32];

	if(asctime_s(time,sizeof(time),&today) != 0)
	{
		return;
	}

	char temp[1024];

	va_list arg;
	va_start(arg,text);
	vsprintf_s(temp,text,arg);
	va_end(arg);

	char log[1024];

	wsprintf(log,"%.8s %s",&time[11],temp);

	gServerDisplayer.LogAddTextConnect(color,log,strlen(log));
}

bool DataSend(int aIndex,BYTE* lpMsg,DWORD size)
{
	if (gObj[aIndex].m_OfflineMode == 1)
	{
		return 0;
	}
	#if (NEW_PROTOCOL_SYSTEM==1)
		gSocketManagerModern.PacketSend(aIndex, ProtocolHead::BOTH_MESSAGE, lpMsg,size);
		return 1;
	#else
		return gSocketManager.DataSend(aIndex,lpMsg,size);
	#endif
}

void DataSendAll(BYTE* lpMsg,int size)
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0)
		{
			DataSend(n,lpMsg,size);
		}
	}
}

bool DataSendSocket(SOCKET socket,BYTE* lpMsg,DWORD size)
{
	if(socket == INVALID_SOCKET)
	{
		return 0;
	}

	EncryptData(lpMsg,size);

	int count=0,result=0;

	while(size > 0)
	{
		if((result=send(socket,(char*)&lpMsg[count],size,0)) == SOCKET_ERROR)
		{
			if(WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return 0;
			}
		}
		else
		{
			count += result;
			size -= result;
		}
	}

	return 1;
}

void MsgSendV2(LPOBJ lpObj,BYTE* lpMsg,int size)
{
	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state != VIEWPORT_NONE && lpObj->VpPlayer2[n].type == OBJECT_USER && OBJECT_BOTS)
		{
			DataSend(lpObj->VpPlayer2[n].index,lpMsg,size);
		}
	}
}

void SendMonsterV2Msg(LPOBJ lpObj, LPBYTE lpMsg, int size)
{
    for( int i=0; i< MAX_VIEWPORT; i++ )
    {
        if( lpObj->VpPlayer2[i].type == OBJECT_USER )
        {
            if( lpObj->VpPlayer2[i].state )
            {
                if( (lpObj->Connected > 1) && (lpObj->Live) )
                {
                    DataSend(lpObj->VpPlayer2[i].index, lpMsg, size);
                }
                else
                {
                    lpObj->VpPlayer2[i].index = -1;
                    lpObj->VpPlayer2[i].state  = VIEWPORT_NONE;
                    lpObj->VPCount2--;
                }
            }  
        }
 
    }
}

void CloseClient(int aIndex)
{
	#if(NEW_PROTOCOL_SYSTEM==1)
		gSocketManagerModern.DisconenctClient(aIndex);
	#else
		gSocketManager.Disconnect(aIndex);
	#endif
}

void PostMessage1(char* name,char* message,char* text)
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	wsprintf(buff,message,name,text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {0};

	wsprintf(buff,message,text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_WHISPER_SEND pMsg;

	pMsg.header.set(0x02,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void PostMessage2(char* name,char* message,char* text)
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	wsprintf(buff,message,name,text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'~'};

	wsprintf(&buff[1],message,text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void PostMessage3(char* name,char* message,char* text)
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	wsprintf(buff,message,name,text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'@'};

	wsprintf(&buff[1],message,text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void PostMessage4(char* name,char* message,char* text)
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	wsprintf(buff,message,name,text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'$'};

	wsprintf(&buff[1],message,text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void PostMessagePK(char* name, char* message, char* text)
{
	char buff[256] = { '~' };
	wsprintf(&buff[1], message, name, text);
	buff[255] = '\0';

	int size = (int)strlen(buff);
	if (size >= sizeof(PMSG_CHAT_SEND::message)) {
		size = sizeof(PMSG_CHAT_SEND::message) - 1;
	}

	PMSG_CHAT_SEND pMsg = {};
	pMsg.header.set(0x00, sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1)));

	memcpy(pMsg.name, "[PK]", sizeof(pMsg.name));
	memcpy(pMsg.message, buff, size);
	pMsg.message[size] = '\0';

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}
void PostMessageUserON(char* name, char* message)
{
	char buff[256] = { '@' };
	wsprintf(&buff[1], message, name);
	buff[255] = '\0';

	int size = (int)strlen(buff);
	if (size >= sizeof(PMSG_CHAT_SEND::message)) {
		size = sizeof(PMSG_CHAT_SEND::message) - 1;
	}

	PMSG_CHAT_SEND pMsg{};
	pMsg.header.set(0x00, sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1)));

	strncpy(pMsg.name, "[MU]", sizeof(pMsg.name));
	pMsg.name[sizeof(pMsg.name) - 1] = '\0';

	memcpy(pMsg.message, buff, size);
	pMsg.message[size] = '\0';

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; ++n)
	{
		if (gObjIsConnectedGP(n))
		{
			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}


void PostMessageNew(char* name, char* message, char* text)
{
	char buff[256] = { 0 };

	wsprintf(buff, message, name, text);
	buff[255] = '\0';

	int size = (int)strlen(buff);
	if (size >= sizeof(PMSG_CHAT_WHISPER_SEND::message)) {
		size = sizeof(PMSG_CHAT_WHISPER_SEND::message) - 1;
	}

	PMSG_CHAT_WHISPER_SEND pMsg = { 0 };
	pMsg.header.set(0x02, sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1)));

	sprintf_s(pMsg.name, sizeof(pMsg.name), "[POST]");
	memcpy(pMsg.message, buff, size);
	pMsg.message[size] = '\0';

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}


void SetLargeRand()
{
	std::random_device rd;
	seed = std::mt19937(rd());
	dist = std::uniform_int_distribution<int>(0, 2147483647);
}

long GetLargeRand()
{
	return dist(seed);
}

bool CheckValid(char* temp)
{
	if (strlen(temp) < 1 || strcmp(temp, "Unknown"))
		return false;
	return true;
}

char* GetIpByDialog(HWND hDlg)
{
	int len = GetWindowTextLengthA(GetDlgItem(hDlg, ID_LIPADDRESS));
	if (len <= 0)
		return nullptr;

	char* IpValue = (char*)GlobalAlloc(GPTR, len + 1);
	if (!IpValue)
		return nullptr;

	GetDlgItemTextA(hDlg, ID_LIPADDRESS, IpValue, len + 1);
	return IpValue;
}

char* GetHwidByDialog(HWND hDlg)
{
	int len = GetWindowTextLengthA(GetDlgItem(hDlg, ID_LHWID));
	if (len <= 0)
		return nullptr;

	char* Hwid = (char*)GlobalAlloc(GPTR, len + 1);
	if (!Hwid)
		return nullptr;

	GetDlgItemTextA(hDlg, ID_LHWID, Hwid, len + 1);
	return Hwid;
}

void LogToUserForm(HWND hDlg, const char* fmt, ...)
{
	char buffer[512];
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	va_end(args);

	SYSTEMTIME time;
	GetLocalTime(&time);

	char timestamp[64];
	sprintf_s(timestamp, "[%02d:%02d:%02d %02d/%02d/%04d]",
		time.wHour, time.wMinute, time.wSecond,
		time.wDay, time.wMonth, time.wYear);

	char final[600];
	sprintf_s(final, "%s %s", timestamp, buffer);

	FILE* fp = nullptr;
	fopen_s(&fp, "LOG\\ACuoi.txt", "a+");
	if (fp)
	{
		fprintf(fp, "%s\n", final);
		fclose(fp);
	}

	if (IsWindow(hDlg))
	{
		HWND hList = GetDlgItem(hDlg, IDC_USERLOGBOX);
		if (hList)
		{
			SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)final);
		}
	}
}

const char* GetDataMsg(char* a, char* b)
{
	char buffer[256];
	sprintf_s(buffer, sizeof(buffer), a, b);
	return buffer; 
}
bool CheckUnknown(HWND hDlg, int ctrlId)
{
	char label[64];
	GetDlgItemTextA(hDlg, ctrlId, label, sizeof(label));
	return (strstr(label, "Unknown") != nullptr);
}

void InLog(char* String, ...)
{
	LogAdd(LOG_RED, String);
}