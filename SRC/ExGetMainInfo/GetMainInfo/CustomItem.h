// CustomItem.h: interface for the CCustomItem class.
//
#pragma once

#define MAX_CUSTOM_ITEM 2000

struct CUSTOM_ITEM_INFO
{
	int Index;
	int ItemIndex;
	float ColorR;
	float ColorG;
	float ColorB;
	char ModelName[32];
	BYTE Color[3];
	int GetLine[3];
	BYTE ItemSet;
};

class CCustomItem
{
	public:
	CCustomItem();
	void Init();
	void Load(char* path);
	void SetInfo(CUSTOM_ITEM_INFO info);
	CUSTOM_ITEM_INFO m_CustomItemInfo[MAX_CUSTOM_ITEM];
	int ItemID[2];
};

extern CCustomItem gCustomItem;
