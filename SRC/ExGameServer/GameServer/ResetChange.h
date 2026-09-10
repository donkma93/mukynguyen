#pragma once

#define MAX_RESET_CHANGE 5
#include "Protocol.h"

struct RESETCHANGE_REQ
{
	PSBMSG_HEAD h;
	int	numchange;
};

struct RESETCHANGE_INFO
{
	int Index;
	int ReqReset;
	int Coin1;
	int LevelUpPoint;
	int TangQua;
	BYTE ItemLevel;
};

class CResetChange
{
public:
	CResetChange();
	void Init();
	void Load(char* path);
	void SetInfo(RESETCHANGE_INFO info);
	RESETCHANGE_INFO* GetInfo(int index);
	void ResetChange(int aIndex, RESETCHANGE_REQ* lpMsg);
	int SoLieuReset,SoLieuPoint;
	RESETCHANGE_INFO m_CustomResetChangeInfo[MAX_RESET_CHANGE];
};

extern CResetChange gResetChange;