#include "stdafx.h"
#include "CustomItem.h"
#include "MemScript.h"

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

void CCustomItem::Load(char* path)
{
    CMemScript* lpMemScript = new CMemScript;

    if (lpMemScript == nullptr)
    {
        printf("[ERROR] Memory allocation failed for CMemScript: %s\n", path);
        return;
    }

    if (lpMemScript->SetBuffer(path) == 0)
    {
        printf("[ERROR] Failed to set buffer for file: %s\n", path);
        printf("[ERROR] %s\n", lpMemScript->GetLastError());
        delete lpMemScript;
        return;
    }

    this->Init();

    try
    {
        static int CustomItemIndexCount = 0;

        while (true)
        {
            if (lpMemScript->GetToken() == TOKEN_END)
            {
                break;
            }

            if (strcmp("end", lpMemScript->GetString()) == 0)
            {
                break;
            }

            CUSTOM_ITEM_INFO info;
            memset(&info, 0, sizeof(info));

            info.Index = CustomItemIndexCount++;
            info.ItemIndex = lpMemScript->GetNumber();
            info.ColorR = lpMemScript->GetAsNumber();
            info.ColorG = lpMemScript->GetAsNumber();
            info.ColorB = lpMemScript->GetAsNumber();

            const char* modelName = lpMemScript->GetAsString();
            if (modelName == nullptr || strlen(modelName) == 0)
            {
                continue;
            }
            strcpy_s(info.ModelName, sizeof(info.ModelName), modelName);

            info.Color[0]   = lpMemScript->GetAsNumber();
            info.GetLine[0] = lpMemScript->GetAsNumber();

            info.Color[1]   = lpMemScript->GetAsNumber();
            info.GetLine[1] = lpMemScript->GetAsNumber();

            info.Color[2]   = lpMemScript->GetAsNumber();
            info.GetLine[2] = lpMemScript->GetAsNumber();

            info.ItemSet    = lpMemScript->GetAsNumber();

            this->SetInfo(info);
        }
    }
    catch (...)
    {
        printf("[ERROR] Exception occurred while parsing file: %s\n", path);
        printf("[ERROR] %s\n", lpMemScript->GetLastError());
    }

    delete lpMemScript;
}

void CCustomItem::SetInfo(CUSTOM_ITEM_INFO info)
{
    if (info.Index < 0 || info.Index >= MAX_CUSTOM_ITEM)
    {
        return;
    }

    if (strlen(info.ModelName) == 0)
    {
        return;
    }

    this->m_CustomItemInfo[info.Index] = info;
}
