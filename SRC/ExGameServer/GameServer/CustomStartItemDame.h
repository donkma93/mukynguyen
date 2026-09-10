#pragma once

#define MAX_CLASS 7


struct CUSTOM_START_ITEM_DAME_INFO
{
	int ItemType;
	int ItemIndex;
	int ItemLevel;
	int ItemOption;
	int EffectBuff;
	int DupeDame;
	int TripDame;
	int CriticalDmg;
	int DameRate;
	int DefDameRate;
	int TimeBuff;
};

struct CustomStartItemDame_INFO
{
	int Class;
	std::vector<CUSTOM_START_ITEM_DAME_INFO> ItemDameStart;
};


class CCustomStartItemDame
{
	public:
	CCustomStartItemDame();
	void Init();
	void LoadItemDame(char* path);
	void CaclCharacter(LPOBJ lpObj, bool flag);
	CustomStartItemDame_INFO m_CustomStartItemDameInfo[MAX_CLASS];
};

extern CCustomStartItemDame gCustomStartItemDame;
