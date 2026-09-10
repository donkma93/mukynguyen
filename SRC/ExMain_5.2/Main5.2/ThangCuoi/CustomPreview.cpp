#include "stdafx.h"
#include "CustomPreview.h"
#include "./Utilities/Log/muConsoleDebug.h"

CCustomPreview gPreview;

CCustomPreview::CCustomPreview()
{
    ClearCustomPreviewCharList();
    ClearCustomPreviewList(0);
}

void CCustomPreview::ClearCustomPreviewCharList()
{
    for (auto& item : m_CustomPreviewCharList)
        item.Reset();
}

void CCustomPreview::ClearCustomPreviewList(int slot)
{
    for (int i = slot; i < MAX_CUSTOM_PREVIEW; ++i)
        m_CustomPreviewList[i].Reset();
}

void CCustomPreview::InsertCustomPreviewCharList(int slot, const char* name, int wing)
{
    if (slot < 0 || slot >= MAX_CUSTOM_PREVIEW_CHAR)
        return;

    auto& item = m_CustomPreviewCharList[slot];
    item.WingIndex = wing;
    item.slot = static_cast<BYTE>(slot);
    std::memcpy(item.name, name, sizeof(item.name));
}

CUSTOM_PET_VIEW_CHAR_LIST* CCustomPreview::GetCustomPreviewCharList(const char* name)
{
    for (auto& item : m_CustomPreviewCharList)
    {
        if (strncmp(item.name, name, sizeof(item.name)) == 0)
            return &item;
    }
    return nullptr;
}

void CCustomPreview::InsertCustomPreviewList(int slot, const char* name, int wing, WORD index)
{
    for (int i = 0; i < MAX_CUSTOM_PREVIEW; ++i)
    {
        if (m_CustomPreviewList[i].index == 0xFFFF)
        {
            auto& item = m_CustomPreviewList[i];
            item.WingIndex = wing;
            item.slot = static_cast<BYTE>(i);
            item.index = index;
            std::memcpy(item.name, name, sizeof(item.name));
            return;
        }
    }
}

CUSTOM_PET_VIEW_LIST* CCustomPreview::GetCustomPreviewList(int index)
{
    for (auto& item : m_CustomPreviewList)
    {
        if (item.index != 0xFFFF && item.index == index)
            return &item;
    }
    return nullptr;
}

void CCustomPreview::DeleteCustomPreview(int index)
{
    for (auto& item : m_CustomPreviewList)
    {
        if (item.index != 0xFFFF && item.index == index)
            item.Reset();
    }
}

void CCustomPreview::PreviewCharListRecv(const BYTE* lpMsg)
{
    auto DataSPK = reinterpret_cast<const RecvPreviewCharListCount*>(lpMsg);
    ClearCustomPreviewCharList();

    for (int n = 0; n < DataSPK->count; ++n)
    {
        auto lpInfo = reinterpret_cast<const RecvPreviewCharlistItem*>(
            lpMsg + sizeof(RecvPreviewCharListCount) + sizeof(RecvPreviewCharlistItem) * n
            );

        char SafeName[11]{};
        std::memcpy(SafeName, lpInfo->Name, sizeof(SafeName) - 1);

        InsertCustomPreviewCharList(n, SafeName, lpInfo->WingIndex);
    }
}

void CCustomPreview::PreviewCharRecv(const BYTE* lpMsg)
{
    auto DataSPK = reinterpret_cast<const RecvPreviewCharlistPort*>(lpMsg);

    ClearCustomPreviewList(DataSPK->Viewport == 1 ? 1 : 0);

    for (int n = 0; n < DataSPK->count; ++n)
    {
        auto lpInfo = reinterpret_cast<const RecvPreviewCharSetPort*>(
            lpMsg + sizeof(RecvPreviewCharlistPort) + sizeof(RecvPreviewCharSetPort) * n
            );

        char SafeName[11]{};
        std::memcpy(SafeName, lpInfo->Name, sizeof(SafeName) - 1);

        InsertCustomPreviewList(DataSPK->Viewport == 1 ? n + 1 : n, SafeName, lpInfo->WingIndex, lpInfo->index);
    }
}
