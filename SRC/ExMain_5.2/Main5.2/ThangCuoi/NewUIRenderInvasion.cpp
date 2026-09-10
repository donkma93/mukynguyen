#include "Stdafx.h"
#include "NewUIRenderInvasion.h"

#include "NewUISystem.h"
#include "WSclientinline.h"
#include "ThangCuoi\ZzzToolKit.h"
#include "ThangCuoi\WideData.h"
#include "ThangCuoi\StructSendGs.h"
#include "NewUIScrollBar.h"

CNewUIRenderInvasionExt gInvasionExt;

SEASON3B::CNewUIScrollBar* CUIScrollBar = NULL;

SEASON3B::CNewUIRenderInvasion::CNewUIRenderInvasion()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	gInvasionExt.m_Data.clear();
}

SEASON3B::CNewUIRenderInvasion::~CNewUIRenderInvasion()
{
	Release();
}

bool SEASON3B::CNewUIRenderInvasion::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENINVASION, this);

	SetPos(x, y);

	Show(false);

	return true;
}

void SEASON3B::CNewUIRenderInvasion::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIRenderInvasion::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos + 200;
	m_Pos.y = y;

	gInvasionExt.m_Pos.x = IsToolKit.GetPositionScreen() - 160;
	gInvasionExt.m_Pos.y = y;
}

bool SEASON3B::CNewUIRenderInvasion::Update()
{
	return true;
}

bool SEASON3B::CNewUIRenderInvasion::UpdateMouseEvent()
{
	if (CheckMouseIn(gInvasionExt.m_Pos.x - 325, gInvasionExt.m_Pos.y, 160, 150))
		return false;
	return true;
}

bool SEASON3B::CNewUIRenderInvasion::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENINVASION);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIRenderInvasion::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIRenderInvasion::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIRenderInvasion::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	gInvasionExt.Initz();

	DisableAlphaBlend();
	return true;
}

void CNewUIRenderInvasionExt::GetInfoReceived(const BYTE* lpMsg)
{
	auto DataSPK = reinterpret_cast<const InvasionDataReceived*>(lpMsg);
	this->m_Data.clear();

	for (auto n = 0; n < DataSPK->Counter; ++n)
	{
		auto info = reinterpret_cast<const InvasionDataActive*>(
			lpMsg + sizeof(InvasionDataReceived) + sizeof(InvasionDataActive) * n
			);

		if (info->Counter.IsCount == 0 && info->Counter.IsMaxCount == 0)
		{
			continue;
		}

		InvasionActive g_Data;
		g_Data.MonsterIndex = info->MonsterIndex;
		g_Data.Counter = info->Counter;

		this->m_Data.push_back(g_Data);
	}

	g_pNewUIMenuOption->IsVisible(SEASON3B::INTERFACE_EVENINVASION);
}

void CNewUIRenderInvasionExt::GetUpdateMonster(const BYTE* lpMsg)
{
	auto DataSPK = reinterpret_cast<const InvasionMonsterReceived*>(lpMsg);

	for (auto it = this->m_Data.begin(); it != this->m_Data.end();)
	{
		if (it->MonsterIndex == DataSPK->MonsterIndex)
		{
			it->Counter.IsCount = DataSPK->Counter;

			if (it->Counter.IsCount == 0 && it->Counter.IsMaxCount == 0)
			{
				it = this->m_Data.erase(it);
			}
			break;
		}
		else
		{
			++it;
		}
	}
}

void CNewUIRenderInvasionExt::Initz()
{
	IsToolKit.ThisFont(gInvasionExt.m_Pos.x - 325, gInvasionExt.m_Pos.y, HEX_COLOR_WHITE, 200, 160, 145, 1, " ");

	this->ListMaxPer[0] = 10;
	this->DataList[0] = this->m_Data.size();

	if (CUIScrollBar == NULL)
	{
		CUIScrollBar = new SEASON3B::CNewUIScrollBar();
		CUIScrollBar->Create(gInvasionExt.m_Pos.x - 175, gInvasionExt.m_Pos.y + 10, 130);
	}

	if (CUIScrollBar)
	{
		int maxPos = (this->DataList[0] > this->ListMaxPer[0]) ? (this->DataList[0] - this->ListMaxPer[0]) : 0;
		CUIScrollBar->SetMaxPos(maxPos);

		CUIScrollBar->MouseWheelWindow = CheckMouseIn(gInvasionExt.m_Pos.x + 16, gInvasionExt.m_Pos.y + 10, 144, 130);
		CUIScrollBar->Render();
		CUIScrollBar->UpdateMouseEvent();
		CUIScrollBar->Update();
	}
	this->CurPos[0] = CUIScrollBar->GetCurPos();
	this->Count[0] = 0;

	EnableAlphaTest();

	if (this->m_Data.size() == 0)
	{
		IsToolKit.ThisFont(gInvasionExt.m_Pos.x - 317, gInvasionExt.m_Pos.y + 135, HEX_COLOR_WHITE, 0, 144, 0, 3, GlobalText[3178]);
	}
	else
	{
		for (int i = this->CurPos[0]; i < this->m_Data.size(); i++)
		{
			if (this->Count[0] >= this->ListMaxPer[0])
				break;

			float PosY = gInvasionExt.m_Pos.y + (this->Count[0] * 13);
			char* name = getMonsterName(this->m_Data[i].MonsterIndex);

			IsToolKit.ThisFont(gInvasionExt.m_Pos.x - 320,  PosY + 13, HEX_COLOR_Yellow, 0, 144, 0, 1, "%d.%s", i + 1, name);
			IsToolKit.ThisFont(gInvasionExt.m_Pos.x - 335, PosY + 13, HEX_COLOR_Yellow, 0, 144, 0, 4, "%d/%d", this->m_Data[i].Counter.IsCount, this->m_Data[i].Counter.IsMaxCount);
			this->Count[0]++;
		}
	}
	IsToolKit.ThisFont(gInvasionExt.m_Pos.x - 325, gInvasionExt.m_Pos.y, HEX_COLOR_WHITE, 0x00FBFF69, 160, 0, 3, GlobalText[3177]);

	DisableAlphaBlend();
}