#include "stdafx.h"
#include "CustomNpcNew.h"
#include <Utilities/Log/muConsoleDebug.h>

CustomModelNPC gCustomModelNPC;

CustomModelNPC::CustomModelNPC()
{
	Init();
}

void CustomModelNPC::Init()
{
	m_CustomModelInfo.clear();
}

void CustomModelNPC::Load(CustomModelNpcInfo* info)
{
	for (int n = 0; n < 50; ++n)
	{
		if (info[n].Index >= 0 && info[n].Index < 50)
		{
			m_CustomModelInfo.push_back(info[n]);

            /*g_ConsoleDebug->Write(3,
                "[CustomModelNPC] Index=%d, MonsterID=%d, Class=%d, SetItemID=%d, LevelItemID=%d, WingItemID=%d, Action=%d, Scale=%.2f, BoundingBoxMax=%.2f, Weapon1=%d, Weapon2=%d, Type=%d",
                info[n].Index,
                info[n].MonsterID,
                info[n].Class,
                info[n].SetItemID,
                info[n].LevelItemID,
                info[n].WingItemID,
                info[n].CurrentAction,
                info[n].Scale,
                info[n].BoundingBoxMax,
                info[n].Weapon[0],
                info[n].Weapon[1],
                info[n].Type);*/
		}
	}
}

bool CustomModelNPC::IsMonster(int MonsterID) const
{
	for (const auto& data : m_CustomModelInfo)
	{
		if (data.MonsterID == MonsterID && data.Type == 1)
			return true;
	}
	return false;
}


void CustomModelNPC::MakeNpcModel(CHARACTER* c, int MonsterID)
{
    for (const auto& info : gCustomModelNPC.m_CustomModelInfo)
    {
        if (info.MonsterID == MonsterID)
        {
            OBJECT* o = &c->Object;

            c->Class = info.Class;

            if (info.WingItemID != -1)
            {
                c->Wing.Type = MODEL_WING + info.WingItemID;
            }

            if (info.Class == 3)
            {
                c->BodyPart[BODYPART_HELM].Type = MODEL_BODY_HELM + 15;
            }
            else
            {
                c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + info.SetItemID;
            }
            c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + info.SetItemID;
            c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + info.SetItemID;
            c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + info.SetItemID;
            c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + info.SetItemID;

            c->BodyPart[BODYPART_HELM].Level = info.LevelItemID;
            c->BodyPart[BODYPART_ARMOR].Level = info.LevelItemID;
            c->BodyPart[BODYPART_PANTS].Level = info.LevelItemID;
            c->BodyPart[BODYPART_GLOVES].Level = info.LevelItemID;
            c->BodyPart[BODYPART_BOOTS].Level = info.LevelItemID;

            if (info.Weapon[0] != -1)
            {
                c->Weapon[0].Type = MODEL_ITEM + info.Weapon[0];
            }

            if (info.Weapon[1] != -1)
            {
                c->Weapon[1].Type = MODEL_ITEM + info.Weapon[1];
            }

            o->Scale = info.Scale;

            if (info.CurrentAction != -1)
            {
                o->CurrentAction = info.CurrentAction;
            }

            if (info.BoundingBoxMax != -1)
            {
                o->BoundingBoxMax[2] += info.BoundingBoxMax;
            }

            return;
        }
    }
}