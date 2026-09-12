#include "user.h"
#include "stdafx.h"

#define MAX_BOTONLINE	100
#define MAX_BOTONLINE_MARKET_ITEMS	24

struct BotOnlineBodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct BotOnlineMarketItem
{
	int num;
	int level;
	int opt;
	int price;
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
	char ShopTitle[36];
	BotOnlineMarketItem market[MAX_BOTONLINE_MARKET_ITEMS];
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
