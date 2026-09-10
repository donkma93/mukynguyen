#include "stdafx.h"
#include "CustomWing.h"

CCustomWing gCustomWing;

CCustomWing::CCustomWing() {
    Init();
}

void CCustomWing::Init() {
    m_CustomWingInfo.clear();
}

void CCustomWing::Load(CUSTOM_WING_INFO* infoArray) {
    for (int i = 0; i < MAX_CUSTOM_WING; ++i) {
        SetInfo(infoArray[i]);
    }
}

void CCustomWing::SetInfo(const CUSTOM_WING_INFO& info) {
    if (info.Index >= 0 && info.Index < MAX_CUSTOM_WING) {
        m_CustomWingInfo[info.Index] = info;
    }
}

CUSTOM_WING_INFO* CCustomWing::GetInfo(int index) {
    auto it = m_CustomWingInfo.find(index);
    return (it != m_CustomWingInfo.end() && it->second.Index == index) ? &it->second : nullptr;
}

CUSTOM_WING_INFO* CCustomWing::GetInfoByItem(int itemIndex) {
    for (auto& [_, info] : m_CustomWingInfo) {
        if (info.ItemIndex == itemIndex) {
            return &info;
        }
    }
    return nullptr;
}

bool CCustomWing::CheckCustomWing(int index) {
    return GetInfo(index) != nullptr;
}

bool CCustomWing::CheckCustomWingByItem(int itemIndex) {
    return GetInfoByItem(itemIndex) != nullptr;
}

bool CCustomWing::CheckCustomWingByItemGlow(int itemIndex) {
    return CheckCustomWingByItem(itemIndex);
}

bool CCustomWing::CheckCustomWingByModelType(int itemIndex, int modelType) {
    for (auto& [_, info] : m_CustomWingInfo) {
        if (info.ItemIndex == itemIndex && info.ModelType == modelType) {
            return true;
        }
    }
    return false;
}

int CCustomWing::GetCustomWingItem(int index) {
    CUSTOM_WING_INFO* info = GetInfo(index);
    return info ? info->ItemIndex : 0;
}

int CCustomWing::GetCustomWingIndex(int itemIndex) {
    CUSTOM_WING_INFO* info = GetInfoByItem(itemIndex);
    return info ? info->Index : 0;
}

int CCustomWing::GetCustomWingDefense(int itemIndex, int itemLevel) {
    CUSTOM_WING_INFO* info = GetInfoByItem(itemIndex);
    return info ? (info->DefenseConstA * itemLevel) : 0;
}

int CCustomWing::GetCustomWingIncDamage(int itemIndex, int itemLevel) {
    CUSTOM_WING_INFO* info = GetInfoByItem(itemIndex);
    return info ? ((info->IncDamageConstA + itemLevel * info->IncDamageConstB) - 100) : 0;
}

int CCustomWing::GetCustomWingDecDamage(int itemIndex, int itemLevel) {
    CUSTOM_WING_INFO* info = GetInfoByItem(itemIndex);
    return info ? (100 - (info->DecDamageConstA - itemLevel * info->DecDamageConstB)) : 0;
}

int CCustomWing::GetCustomWingOptionIndex(int itemIndex, int optionNumber) {
    CUSTOM_WING_INFO* info = GetInfoByItem(itemIndex);
    if (!info) return 0;

    switch (optionNumber) {
    case 0: return info->OptionIndex1;
    case 1: return info->OptionIndex2;
    case 2: return info->OptionIndex3;
    default: return 0;
    }
}

int CCustomWing::GetCustomWingOptionValue(int itemIndex, int optionNumber) {
    CUSTOM_WING_INFO* info = GetInfoByItem(itemIndex);
    if (!info) return 0;

    switch (optionNumber) {
    case 0: return info->OptionValue1;
    case 1: return info->OptionValue2;
    case 2: return info->OptionValue3;
    default: return 0;
    }
}

int CCustomWing::GetCustomWingNewOptionIndex(int itemIndex, int optionNumber) {
    CUSTOM_WING_INFO* info = GetInfoByItem(itemIndex);
    if (!info) return 0;

    switch (optionNumber) {
    case 0: return info->NewOptionIndex1;
    case 1: return info->NewOptionIndex2;
    case 2: return info->NewOptionIndex3;
    case 3: return info->NewOptionIndex4;
    default: return 0;
    }
}

int CCustomWing::GetCustomWingNewOptionValue(int itemIndex, int optionNumber) {
    CUSTOM_WING_INFO* info = GetInfoByItem(itemIndex);
    if (!info) return 0;

    switch (optionNumber) {
    case 0: return info->NewOptionValue1;
    case 1: return info->NewOptionValue2;
    case 2: return info->NewOptionValue3;
    case 3: return info->NewOptionValue4;
    default: return 0;
    }
}

int CCustomWing::CheckCustomWingIsCape(int ItemIndex)
{
    if (ItemIndex < GET_ITEM(12, 0) || ItemIndex >= GET_ITEM(13, 0))
    {
        return 0;
    }

    std::map<int, CUSTOM_WING_INFO>::iterator it = gCustomWing.m_CustomWingInfo.find(ItemIndex - MODEL_ITEM);

    if (it == gCustomWing.m_CustomWingInfo.end())
    {
        return 0;
    }

    return it->second.IsCape;
}
