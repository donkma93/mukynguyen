#include "stdafx.h"
#include "SendMessage.h"
#include "SocketManager.h"
#include "Util.h"

CSendMessage gSendMessage;

CSendMessage::CSendMessage()
{
	this->currentIndex = -1;
}

void CSendMessage::SendProcessRequest()
{
	if (this->currentIndex == -1)
	{
		return;
	}

	BYTE send[8192];

	PMSG_SEND_MESSAGE_REQ_SEND pMsg{};

	pMsg.header.set(0xB0, sizeof(pMsg));
	memcpy(pMsg.message, gSendMessage.message, sizeof(pMsg.message));

	LogAdd(LOG_BLUE, "Gửi tin nhắn thành công!");

	gSocketManager.DataSend(this->currentIndex, (BYTE *)&pMsg, sizeof(pMsg));
}