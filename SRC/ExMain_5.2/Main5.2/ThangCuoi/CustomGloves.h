#pragma once

#define MAX_CUSTOM_GLOVES 50
#include <map>

struct CUSTOM_RF_GLOVES
{
	int Index, ItemIndex;
	char ModelName[32];
	int TextureLeftIndex;
	char TextureLeftName[32];
	int TextureRightIndex;
	char TextureRightName[32];
	int ColorR, ColorG, ColorB;
};

class cCustomGloves
{
public:
	cCustomGloves();
	void Load(CUSTOM_RF_GLOVES* info);
	bool CheckGloves(int ItemID);
	int GetItemIndex(int ItemIndex);

	std::map<int, CUSTOM_RF_GLOVES> m_CustomGlovesInfo;
};

extern cCustomGloves gCustomBattleGloves;
