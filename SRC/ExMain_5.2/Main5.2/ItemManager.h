#pragma once

#include <map>
#include "MemScript.h"

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
    BOOL GetCustomItemColor(int ItemIndex, float* ItemColor);
    void GetItemColor(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon = false);
    void SetInfo(CUSTOM_ITEM_INFO info);
    void Load(CUSTOM_ITEM_INFO* info);
    std::map<int, CUSTOM_ITEM_INFO> m_CustomItemInfo;

    void SetLoadItem(int Index, int ItemIndex, float colorR, float colorG, float colorB, char* ModelName, BYTE A1, int A, BYTE B1, int B, BYTE C1, int C, BYTE SetType);

    BOOL ItemInfo(int ItemType, char TextList[][100], int TextListColor[50], int TextBold[50], int* TextNum);

};

extern CCustomItem gCustomItem;
