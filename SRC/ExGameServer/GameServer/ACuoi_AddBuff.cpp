#include "stdafx.h"
#include "ACuoi_AddBuff.h"
#include "EffectManager.h"
#include "Notice.h"
#include "MemScript.h"
#include "Util.h"
#include <fstream>
#include <sstream>
#include <string>
#include "ServerInfo.h"
#include "DSProtocol.h"
#include <cctype>
#include "ACuoi_MessLang.h"

AddBuffer gAddBuffer;

void AddBuffer::Read(char* FilePath)
{
    int currentSection = -1;
    std::ifstream file(FilePath);
    if (!file.is_open())
    {
        ErrorMessageBox("Error openm file", FilePath);
        return;
    }

    this->IsReadDataX.SkillCountSection0 = 0;
    this->IsReadDataX.SkillCountSection1 = 0;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '/')
        {
            continue;
        }

        if (line == "end")
        {
            currentSection = -1;
            continue;
        }

        if (std::isdigit(line[0]) && line.find(' ') == std::string::npos)
        {
            currentSection = std::stoi(line);
            continue;
        }

        std::stringstream ss(line);
        BuffData buff;
        if (ss >> buff.IDBuff >> buff.Timer >> buff.Val1 >> buff.Val2 >> buff.Val3 >> buff.Val4)
        {
            if (currentSection == 0 && this->IsReadDataX.SkillCountSection0 < 32)
            {
                this->IsReadDataX.BuffsSection0[this->IsReadDataX.SkillCountSection0] = buff;
                this->IsReadDataX.SkillCountSection0++;
            }
            else if (currentSection == 1 && this->IsReadDataX.SkillCountSection1 < 32)
            {
                this->IsReadDataX.BuffsSection1[this->IsReadDataX.SkillCountSection1] = buff;
                this->IsReadDataX.SkillCountSection1++;
            }
        }
    }
    file.close();
}

bool AddBuffer::CommandAddBuff(LPOBJ lpObj)
{
    if ((GetTickCount() - lpObj->ClickClientSend) < 5 * 1000)
    {
        gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(38));
        return false;
    }

    for (int i = 0; i < this->IsReadDataX.SkillCountSection0; i++)
    {
        BuffData& buff = this->IsReadDataX.BuffsSection0[i];
        gEffectManager.AddEffect(lpObj, 1, buff.IDBuff, (int)(buff.Timer * 60), buff.Val1, buff.Val2, buff.Val3, buff.Val4);
    }
    for (int i = 0; i < this->IsReadDataX.SkillCountSection1; i++)
    {
        BuffData& buff = this->IsReadDataX.BuffsSection1[i];
        gEffectManager.AddEffect(lpObj, 1, buff.IDBuff, (int)time(0) + (buff.Timer * 60), buff.Val1, buff.Val2, buff.Val3, buff.Val4);
    }

    gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(207));
    lpObj->ClickClientSend = GetTickCount();
    return true;
}

bool AddBuffer::CommandDelBuff(LPOBJ lpObj)
{
    if ((GetTickCount() - lpObj->ClickClientSend) < 5 * 1000)
    {
        gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(38));
        return false;
    }

    for (int i = 0; i < MAX_EFFECT_LIST; i++)
    {
        CEffect* lpEffect = &lpObj->Effect[i];

        if (lpEffect->IsEffect())
        {
            gEffectManager.RemoveEffect(lpObj, lpEffect);
            gEffectManager.GCPeriodicEffectSend(lpObj, 0, lpEffect->m_group, 1, 0, lpEffect->m_index);
            lpEffect->Clear();
        }
    }

    gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(353));
    lpObj->ClickClientSend = GetTickCount();
    return true;
}