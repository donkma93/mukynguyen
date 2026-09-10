#include "Stdafx.h"
#include "NewUIResetChange.h"
#include "NewUISystem.h"
#include "WSclientinline.h"
#include "ThangCuoi\ZzzToolKit.h"
#include "ThangCuoi\WideData.h"

cResetChange gResetChange;

SEASON3B::CNewUIResetChange::CNewUIResetChange()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_iNumCurOpenTab = 0;

	gResetChange.MaxPosition.x = static_cast<float>((640 - 270)) / 2;
	gResetChange.MaxPosition.y = static_cast<float>((430 - 260)) / 2;

	gResetChange.CurPosition.x = MouseX;
	gResetChange.CurPosition.y = MouseY;

	gResetChange.DragItem = false;

	WINDOW_WIDTH = 310;
	WINDOW_HEIGHT = 215;
}

SEASON3B::CNewUIResetChange::~CNewUIResetChange()
{
	Release();
}

bool SEASON3B::CNewUIResetChange::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENRSCHANGE, this);

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIResetChange::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIResetChange::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

void SEASON3B::CNewUIResetChange::InitButtons()
{
	for (int x = 0; x < 5; x++)
	{
		g_pUIForm->SetButtonInfo(&m_Btn[x], IMAGE_IGS_BUTTON, m_Pos.x + 211, m_Pos.y + 74 + (x * 28), 52, 26, 1, 0, 1, 1u, GlobalText[3840], "", 0);
	}
}

bool SEASON3B::CNewUIResetChange::Update()
{
	if (IsVisible())
	{
		int iNumCurOpenTab = m_TabBtn.UpdateMouseEvent();

		if (iNumCurOpenTab == RADIOGROUPEVENT_NONE)
			return true;

		m_iNumCurOpenTab = iNumCurOpenTab;
	}
	return true;
}

bool SEASON3B::CNewUIResetChange::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 40, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENRSCHANGE);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH - 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUIResetChange::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENRSCHANGE);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIResetChange::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIResetChange::GetKeyEventOrder()
{
	return 3.4;
}

void SEASON3B::CNewUIResetChange::DrawStringData(BYTE ID, int AddY)
{
	char SystemText[255];
	sprintf_s(SystemText, sizeof(SystemText), GlobalText[3890], gRank.ReqResetChange[ID], IsToolKit.QN(gRank.ReqResetUpPoint[ID]), IsToolKit.QN(gRank.ReqResetCoin[ID]));
	IsToolKit.ThisFont(m_Pos.x + 30, m_Pos.y + 80 + (AddY), 0xFFFF00FF, 0, 180, 0, RT3_SORT_LEFT, SystemText);
}

bool SEASON3B::CNewUIResetChange::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, GlobalText[3784]);
	g_pUIForm->RenderBack(m_Pos.x + 20, m_Pos.y + 40, 257.f, 25);
	g_pUIForm->RenderBack(m_Pos.x + 20, m_Pos.y + 65, 257.f, 160);

	for (int x = 0; x < 5; x++)
	{
		DrawStringData(x, 29 * x);
	}

	IsToolKit.ThisFont(m_Pos.x, m_Pos.y + 46, 0x00A2E8FF, 0, 310, 0, RT3_SORT_CENTER, GlobalText[3891]);

	IsToolKit.ThisFont(m_Pos.x + 30, m_Pos.y + 230, 0xC80000FF, 0, 180, 0, RT3_SORT_LEFT, GlobalText[3892]);
	IsToolKit.ThisFont(m_Pos.x + 30, m_Pos.y + 245, 0xFAC864FF, 0, 180, 0, RT3_SORT_LEFT, GlobalText[3894]);
	IsToolKit.ThisFont(m_Pos.x + 30, m_Pos.y + 260, 0xFAC864FF, 0, 200, 0, RT3_SORT_LEFT, GlobalText[3893]);

	for (int x = 0; x < 5; x++)
	{
		m_Btn[x].Render();
	}

	DisableAlphaBlend();
	return true;
}

bool SEASON3B::CNewUIResetChange::ReCallBackProcess(BYTE index, int Enum)
{
	if (m_Btn[index].UpdateMouseEvent())
	{
		SendRequestDataSendType(0xF3, 0xD9, index + 1);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
}
bool SEASON3B::CNewUIResetChange::BtnProcess()
{
	for (int x = 0; x < 5; x++)
	{
		this->ReCallBackProcess(x, 0);
	}
	return false;
}
