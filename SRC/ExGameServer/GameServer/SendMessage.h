#pragma once
#include "Protocol.h"
struct PMSG_SEND_MESSAGE_REQ_SEND
{
	PWMSG_HEAD header;
	char message[256];
};

class CSendMessage
{
public:
	CSendMessage();
	void SendProcessRequest();

	int currentIndex;
	char message[256];
};
extern CSendMessage gSendMessage;