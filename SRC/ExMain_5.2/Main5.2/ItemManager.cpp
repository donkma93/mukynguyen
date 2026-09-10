#include "stdafx.h"
#include "BoneManager.h"
#include "ItemManager.h"
#include "./Utilities/Log/muConsoleDebug.h"
#include "ThangCuoi/CustomBow.h"
#include <ThangCuoi/CustomGloves.h>

CCustomItem gCustomItem;

CCustomItem::CCustomItem()
{
	this->Init();
}

void CCustomItem::Init()
{
	for (int n = 0; n < MAX_CUSTOM_ITEM; n++)
	{
		this->m_CustomItemInfo[n].Index = -1;
	}
}
void CCustomItem::SetLoadItem(int Index, int ItemIndex, float colorR, float colorG, float colorB, char* ModelName, BYTE A1,int A, BYTE B1, int B, BYTE C1, int C, BYTE SetType)
{
	CUSTOM_ITEM_INFO info;

	info.Index = Index;
	info.ItemIndex = ItemIndex;

	info.ColorR = colorR / 255.0f;
	info.ColorG = colorG / 255.0f;
	info.ColorB = colorB / 255.0f;

	strcpy_s(info.ModelName, ModelName);

	info.Color[0] = A1;
	info.GetLine[0] = A;

	info.Color[1] = B1;
	info.GetLine[1] = B;

	info.Color[2] = C1;
	info.GetLine[2] = C;

	info.ItemSet = SetType;

	gCustomItem.m_CustomItemInfo[info.ItemIndex] = info;
}
void CCustomItem::Load(CUSTOM_ITEM_INFO* info) // OK
{
	for (int n = 0; n < MAX_CUSTOM_ITEM; n++)
	{
		this->SetInfo(info[n]);
	}
}
void CCustomItem::SetInfo(CUSTOM_ITEM_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOM_ITEM)
	{
		return;
	}
	SetLoadItem(info.Index
		, info.ItemIndex
		, info.ColorR, info.ColorG, info.ColorB
		, info.ModelName
		, info.Color[0], info.GetLine[0], info.Color[1]
		, info.GetLine[1], info.Color[2], info.GetLine[2]
		, info.ItemSet
	);
	//g_ConsoleDebug->Write(3,"%d %d %.f, %.f, %.f, %s | %d %d %d | %d %d %d | %d\n",
	//	info.Index, info.ItemIndex
	//	, info.ColorR, info.ColorG, info.ColorB
	//	, info.ModelName
	//	, info.GetLine[0], info.GetLine[1], info.GetLine[2], info.Color[0], info.Color[1], info.Color[2]
	//	, info.ItemSet);
}

BOOL CCustomItem::GetCustomItemColor(int ItemIndex, float* ItemColor) // OK
{
	std::map<int, CUSTOM_ITEM_INFO>::iterator it1 = this->m_CustomItemInfo.find(ItemIndex);
	std::map<int, CUSTOM_BOW_INFO>::iterator it2 = gCustomBow.m_CustomBowInfo.find(ItemIndex);
	if (it1 != this->m_CustomItemInfo.end())
	{
		ItemColor[0] = (float)(it1->second.ColorR / 255.0f);
		ItemColor[1] = (float)(it1->second.ColorG / 255.0f);
		ItemColor[2] = (float)(it1->second.ColorB / 255.0f);
		return 1;
	}
	else if (it2 != gCustomBow.m_CustomBowInfo.end())
	{
		ItemColor[0] = (float)(it2->second.ColorR / 255.0f);
		ItemColor[1] = (float)(it2->second.ColorG / 255.0f);
		ItemColor[2] = (float)(it2->second.ColorB / 255.0f);
		return 1;
	}
	else
	{
		for (std::map<int, CUSTOM_RF_GLOVES>::iterator it = gCustomBattleGloves.m_CustomGlovesInfo.begin(); it != gCustomBattleGloves.m_CustomGlovesInfo.end(); it++)
		{
			if (it->second.ItemIndex == ItemIndex)
			{
				ItemColor[0] = (float)(it->second.ColorR / 255.0f);
				ItemColor[1] = (float)(it->second.ColorG / 255.0f);
				ItemColor[2] = (float)(it->second.ColorB / 255.0f);
				return 1;
			}
			else if (it->second.TextureLeftIndex == ItemIndex)
			{
				ItemColor[0] = (float)(it->second.ColorR / 255.0f);
				ItemColor[1] = (float)(it->second.ColorG / 255.0f);
				ItemColor[2] = (float)(it->second.ColorB / 255.0f);
				return 1;
			}
			else if (it->second.TextureRightIndex == ItemIndex)
			{
				ItemColor[0] = (float)(it->second.ColorR / 255.0f);
				ItemColor[1] = (float)(it->second.ColorG / 255.0f);
				ItemColor[2] = (float)(it->second.ColorB / 255.0f);
				return 1;
			}
		}
	}
	return 0;
}

/*
BOOL CCustomItem::GetCustomItemColor(int ItemIndex, float* ItemColor) // OK
{
	std::map<int, CUSTOM_ITEM_INFO>::iterator it = this->m_CustomItemInfo.find(ItemIndex);

	if (it == this->m_CustomItemInfo.end())
	{
		return 0;
	}

	auto lpInfo = &(*it).second;

	ItemColor[0] = lpInfo->ColorR;
	ItemColor[1] = lpInfo->ColorG;
	ItemColor[2] = lpInfo->ColorB;

	return 1;
}
*/
void CCustomItem::GetItemColor(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon)
{
	if (this->GetCustomItemColor(Type, (float*)Light) == 0)
	{
		PartObjectColor(Type, Alpha, Bright, Light, ExtraMon);
	}
}

BOOL CCustomItem::ItemInfo(int ItemType, char TextList[][100], int TextListColor[50], int TextBold[50], int* TextNum)
{
	auto it = this->m_CustomItemInfo.find(ItemType);
	if (it == this->m_CustomItemInfo.end())
	{
		return FALSE;
	}

	if (it->second.ItemSet != 0)
	{
		sprintf(TextList[*TextNum], GlobalText[it->second.GetLine[0]]);
		TextListColor[*TextNum] = it->second.Color[0];
		TextBold[*TextNum] = false;
		(*TextNum)++;
	
		sprintf(TextList[*TextNum], GlobalText[it->second.GetLine[1]]);
		TextListColor[*TextNum] = it->second.Color[1];
		TextBold[*TextNum] = false;
		(*TextNum)++;
	
		sprintf(TextList[*TextNum], GlobalText[it->second.GetLine[2]]);
		TextListColor[*TextNum] = it->second.Color[2];
		TextBold[*TextNum] = false;
		(*TextNum)++;
	}

	return TRUE;
}

