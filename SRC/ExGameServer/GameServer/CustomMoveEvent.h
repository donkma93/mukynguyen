#include "StdAfx.h"
#pragma once
#include "Protocol.h"

struct MOVE_EVENT_REQ
{
	PSBMSG_HEAD h;
	int	MOVE_EVENT;
};

class CCustomMoveEvent
{
	public:
	void MOVE_EVENT(LPOBJ lpObj, MOVE_EVENT_REQ* lpMsg, int aIndex);
};
extern CCustomMoveEvent gCustomMoveEvent;
#pragma once
