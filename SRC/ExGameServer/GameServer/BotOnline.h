#include "user.h"
#include "stdafx.h"

#define MAX_BOTONLINE	100

struct BotOnlineBodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct BotOnlineStruct
{
	int index;
	int Class;
	int Rank;
	char Name[11];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	bool Enabled;
	BotOnlineBodyItems body[9];
	BYTE ItemCount;
};

class ObjBotOnline
{
	public:
	bool Enabled;
	void Read(char * FilePath);
	void MakeBot();
	void UnloadBot();
	BotOnlineStruct bot[MAX_BOTONLINE];
};
extern ObjBotOnline BotOnline;
