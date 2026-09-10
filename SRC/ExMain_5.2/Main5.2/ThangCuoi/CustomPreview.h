#pragma once

#include "StructSendGs.h"
#define MAX_CUSTOM_PREVIEW_CHAR 5
#define MAX_CUSTOM_PREVIEW 75

struct CUSTOM_PET_VIEW_CHAR_LIST
{
    char name[10]{};
    int WingIndex{ -1 };
    BYTE slot{ 0xFF };

    void Reset()
    {
        memset(name, 0, sizeof(name));
        WingIndex = -1;
        slot = 0xFF;
    }
};

struct CUSTOM_PET_VIEW_LIST
{
    char name[11]{};
    int WingIndex{ -1 };
    BYTE slot{ 0xFF };
    WORD index{ 0xFFFF };

    void Reset()
    {
        memset(name, 0, sizeof(name));
        WingIndex = -1;
        slot = 0xFF;
        index = 0xFFFF;
    }
};

class CCustomPreview
{
public:
    CCustomPreview();
    void ClearCustomPreviewCharList();
    void InsertCustomPreviewCharList(int slot, const char* name, int wing);
    CUSTOM_PET_VIEW_CHAR_LIST* GetCustomPreviewCharList(const char* name);

    void ClearCustomPreviewList(int slot);
    void InsertCustomPreviewList(int slot, const char* name, int wing, WORD index);
    CUSTOM_PET_VIEW_LIST* GetCustomPreviewList(int index);
    void DeleteCustomPreview(int index);

    void PreviewCharListRecv(const BYTE* lpMsg);
    void PreviewCharRecv(const BYTE* lpMsg);

private:
    CUSTOM_PET_VIEW_CHAR_LIST m_CustomPreviewCharList[MAX_CUSTOM_PREVIEW_CHAR];
    CUSTOM_PET_VIEW_LIST m_CustomPreviewList[MAX_CUSTOM_PREVIEW];
};

extern CCustomPreview gPreview;
