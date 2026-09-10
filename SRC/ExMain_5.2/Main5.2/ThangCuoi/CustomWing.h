#pragma once
#include <map>

#define MAX_CUSTOM_WING 100

struct CUSTOM_WING_INFO {
    int Index;
    int ItemIndex;
    int DefenseConstA;
    int IncDamageConstA;
    int IncDamageConstB;
    int DecDamageConstA;
    int DecDamageConstB;
    int OptionIndex1, OptionValue1;
    int OptionIndex2, OptionValue2;
    int OptionIndex3, OptionValue3;
    int NewOptionIndex1, NewOptionValue1;
    int NewOptionIndex2, NewOptionValue2;
    int NewOptionIndex3, NewOptionValue3;
    int NewOptionIndex4, NewOptionValue4;
    int ModelType;
    char ModelName[32];
    int IsCape;
};

class CCustomWing {
public:
    CCustomWing();
    void Init();
    void Load(CUSTOM_WING_INFO* infoArray);
    void SetInfo(const CUSTOM_WING_INFO& info);

    CUSTOM_WING_INFO* GetInfo(int index);
    CUSTOM_WING_INFO* GetInfoByItem(int itemIndex);

    bool CheckCustomWing(int index);
    bool CheckCustomWingByItem(int itemIndex);
    bool CheckCustomWingByItemGlow(int itemIndex);
    bool CheckCustomWingByModelType(int itemIndex, int modelType);

    int GetCustomWingItem(int index);
    int GetCustomWingIndex(int itemIndex);
    int GetCustomWingDefense(int itemIndex, int itemLevel);
    int GetCustomWingIncDamage(int itemIndex, int itemLevel);
    int GetCustomWingDecDamage(int itemIndex, int itemLevel);
    int GetCustomWingOptionIndex(int itemIndex, int optionNumber);
    int GetCustomWingOptionValue(int itemIndex, int optionNumber);
    int GetCustomWingNewOptionIndex(int itemIndex, int optionNumber);
    int GetCustomWingNewOptionValue(int itemIndex, int optionNumber);
    int CheckCustomWingIsCape(int itemIndex);

    std::map<int, CUSTOM_WING_INFO> m_CustomWingInfo;
};

extern CCustomWing gCustomWing;
