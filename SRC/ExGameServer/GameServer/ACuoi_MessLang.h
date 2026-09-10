#pragma once

#include "Protocol.h"
#include "User.h"

struct MESSAGE_NEW
{
	int Index;
	char Message[256];
};

class cMessageNew
{
	public:
	void LoadConfig(char* FilePath);
	std::map<int, MESSAGE_NEW> m_MessageInfoBP;
	char* GetMessage(int index);
};

extern cMessageNew gMessageNew;
