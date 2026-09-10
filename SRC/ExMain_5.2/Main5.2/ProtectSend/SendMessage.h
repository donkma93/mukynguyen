#pragma once

#include <WSclient.h>

struct PMSG_SEND_MESSAGE_REQ_RECV
{
	PWMSG_HEAD header;
	char message[256];
};

class CSendMessage
{
public:
	void RecvSendMessage(PMSG_SEND_MESSAGE_REQ_RECV *lpMsg);
	char message[256];
};
extern CSendMessage gSendMessage;