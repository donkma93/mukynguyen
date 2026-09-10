#include "Stdafx.h"
#if ChangePassTurn
#include "NewUIChangePass.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

SEASON3B::CNewUIChangePass::CNewUIChangePass()
{
	m_pNewUIMng = nullptr;
	m_pNewPassInput = nullptr;
	m_pOldPassInput = nullptr;
	m_pReNewPassInput = nullptr;
	m_pPersionCodeInput = nullptr;

	m_Pos.x = 0;
	m_Pos.y = 0;

	m_bShowLengthWarningNew = false;
	m_bShowLengthWarningRNew = false;
	m_bShowNotMatchWarning = false;
	m_bShowInvalidCodeWarning = false;
}

SEASON3B::CNewUIChangePass::~CNewUIChangePass()
{
	Release();
}

bool SEASON3B::CNewUIChangePass::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CHANGEPASS, this);

	SetPos(x, y);

	InitButtons();	

	m_pOldPassInput = new CUITextInputBox;
	m_pOldPassInput->Init(g_hWnd, 80, 10, 10, true);
	m_pOldPassInput->SetPosition(m_Pos.x + 90, m_Pos.y + 66);
	m_pOldPassInput->SetTextColor(255, 255, 255, 255);
	m_pOldPassInput->SetBackColor(0, 255, 255, 255);
	m_pOldPassInput->SetFont(g_hFont);
	m_pOldPassInput->SetState(UISTATE_NORMAL);
	
	m_pNewPassInput = new CUITextInputBox;
	m_pNewPassInput->Init(g_hWnd, 80, 10, 10, false);
	m_pNewPassInput->SetPosition(m_Pos.x + 90, m_Pos.y + 100);
	m_pNewPassInput->SetTextColor(255, 255, 255, 255);
	m_pNewPassInput->SetBackColor(0, 255, 255, 255);
	m_pNewPassInput->SetFont(g_hFont);
	m_pNewPassInput->SetState(UISTATE_NORMAL);
	
	m_pReNewPassInput = new CUITextInputBox;
	m_pReNewPassInput->Init(g_hWnd, 80, 10, 10, false);
	m_pReNewPassInput->SetPosition(m_Pos.x + 90, m_Pos.y + 134);
	m_pReNewPassInput->SetTextColor(255, 255, 255, 255);
	m_pReNewPassInput->SetBackColor(0, 255, 255, 255);
	m_pReNewPassInput->SetFont(g_hFont);
	m_pReNewPassInput->SetState(UISTATE_NORMAL);
	
	m_pPersionCodeInput = new CUITextInputBox;
	m_pPersionCodeInput->Init(g_hWnd, 80, 10, 7, false);
	m_pPersionCodeInput->SetPosition(m_Pos.x + 90, m_Pos.y + 168);
	m_pPersionCodeInput->SetTextColor(255, 255, 255, 0);
	m_pPersionCodeInput->SetBackColor(0, 255, 255, 255);
	m_pPersionCodeInput->SetFont(g_hFont);
	m_pPersionCodeInput->SetState(UISTATE_NORMAL);

	Show(false);

	return true;
}

void SEASON3B::CNewUIChangePass::Release()
{
	SAFE_DELETE(m_pOldPassInput);
	SAFE_DELETE(m_pNewPassInput);
	SAFE_DELETE(m_pNewPassInput);
	SAFE_DELETE(m_pPersionCodeInput);

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIChangePass::ClearTextBoxes()
{
	if (m_pOldPassInput) m_pOldPassInput->SetText("");
	if (m_pNewPassInput) m_pNewPassInput->SetText("");
	if (m_pReNewPassInput) m_pReNewPassInput->SetText("");
	if (m_pPersionCodeInput) m_pPersionCodeInput->SetText("");

	m_bShowLengthWarningNew = false;
	m_bShowLengthWarningRNew = false;
	m_bShowNotMatchWarning = false;
	m_bShowInvalidCodeWarning = false;
}

void SEASON3B::CNewUIChangePass::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos;
	m_Pos.y = y + 50;
}

bool SEASON3B::CNewUIChangePass::Update()
{
	if (!IsVisible())
		return true;

	if (m_pOldPassInput) m_pOldPassInput->DoAction();
	if (m_pNewPassInput) m_pNewPassInput->DoAction();
	if (m_pReNewPassInput) m_pReNewPassInput->DoAction();
	if (m_pPersionCodeInput) m_pPersionCodeInput->DoAction();

	return true;
}

bool SEASON3B::CNewUIChangePass::UpdateMouseEvent()
{
	if (!IsVisible())
	{
		if (m_pOldPassInput && m_pOldPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pNewPassInput && m_pNewPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pReNewPassInput && m_pReNewPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pPersionCodeInput && m_pPersionCodeInput->HaveFocus()) SetFocus(g_hWnd);
		return false;
	}

	if (m_Btn[0].UpdateMouseEvent())
	{
		char oldPass[11] = { 0 };
		char newPass[11] = { 0 };
		char reNewPass[11] = { 0 };
		char reNewCode[8] = { 0 };

		if (m_pOldPassInput) m_pOldPassInput->GetText(oldPass);
		if (m_pNewPassInput) m_pNewPassInput->GetText(newPass);
		if (m_pReNewPassInput) m_pReNewPassInput->GetText(reNewPass);

		if (m_pPersionCodeInput) m_pPersionCodeInput->GetText(reNewCode);

		if (strlen(reNewCode) == 0 && CharacterAttribute != nullptr)
		{
			strncpy(reNewCode, CharacterAttribute->PersonalCode, sizeof(reNewCode));
			reNewCode[sizeof(reNewCode) - 1] = '\0';
		}

		if (strlen(reNewCode) != 7)
		{
			m_bShowInvalidCodeWarning = true;
			return true;
		}
		else
		{
			m_bShowInvalidCodeWarning = false;
		}

		if (strlen(newPass) < 4 || strlen(newPass) > 10)
		{
			m_bShowLengthWarningNew = true;
			return true;
		}
		else
		{
			m_bShowLengthWarningNew = false;
		}

		if (strlen(reNewPass) < 4 || strlen(reNewPass) > 10)
		{
			m_bShowLengthWarningRNew = true;
			return true;
		}
		else
		{
			m_bShowLengthWarningRNew = false;
		}

		if (strcmp(newPass, reNewPass) != 0)
		{
			m_bShowNotMatchWarning = true;
			return true;
		}
		else
		{
			m_bShowNotMatchWarning = false;
		}

		struct CSENDGS_DOIMK
		{
			PSBMSG_HEAD Head;
			BYTE TYPE;
			char PASS_OLD[11];
			char PASS_NEW[11];
			char PASS_NEW1[11];
			char CODE_NEW1[8];
		};

		CSENDGS_DOIMK pMsg{};
		pMsg.Head.set(0xFF, 0x91, sizeof(pMsg));
		pMsg.TYPE = 0x01;

		memcpy(pMsg.PASS_OLD, oldPass, sizeof(pMsg.PASS_OLD));
		memcpy(pMsg.PASS_NEW, newPass, sizeof(pMsg.PASS_NEW));
		memcpy(pMsg.PASS_NEW1, reNewPass, sizeof(pMsg.PASS_NEW1));
		memcpy(pMsg.CODE_NEW1, reNewCode, sizeof(pMsg.CODE_NEW1));

		DataSend((LPBYTE)&pMsg, pMsg.Head.size);
		PlayBuffer(SOUND_CLICK01);

		if (m_pOldPassInput && m_pOldPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pNewPassInput && m_pNewPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pReNewPassInput && m_pReNewPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pPersionCodeInput && m_pPersionCodeInput->HaveFocus()) SetFocus(g_hWnd);
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_CHANGEPASS);
		return true;
	}

	POINT ptExitBtn1 = { m_Pos.x + 168, m_Pos.y + 5 };

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 16, 16))
	{
		if (m_pOldPassInput && m_pOldPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pNewPassInput && m_pNewPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pReNewPassInput && m_pReNewPassInput->HaveFocus()) SetFocus(g_hWnd);
		if (m_pPersionCodeInput && m_pPersionCodeInput->HaveFocus()) SetFocus(g_hWnd);
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_CHANGEPASS);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 86, WINDOW_HEIGHT + 49))
		return false;

	return true;
}


bool SEASON3B::CNewUIChangePass::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			if (m_pOldPassInput && m_pOldPassInput->HaveFocus()) SetFocus(g_hWnd);
			if (m_pNewPassInput && m_pNewPassInput->HaveFocus()) SetFocus(g_hWnd);
			if (m_pReNewPassInput && m_pReNewPassInput->HaveFocus()) SetFocus(g_hWnd);
			if (m_pPersionCodeInput && m_pPersionCodeInput->HaveFocus()) SetFocus(g_hWnd);

			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_CHANGEPASS);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}

	return true;
}

float SEASON3B::CNewUIChangePass::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIChangePass::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIChangePass::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderImage(CNewUICharacterInfoWindow::IMAGE_CHAINFO_BACK, m_Pos.x + 1, m_Pos.y + 5, 186.f, 259.f);
	RenderImage(CNewUICharacterInfoWindow::IMAGE_CHAINFO_TOP, m_Pos.x, m_Pos.y, 190.f, 64.f);
	RenderImage(CNewUICharacterInfoWindow::IMAGE_CHAINFO_LEFT, m_Pos.x, m_Pos.y + 64.f, 21.f, 156.f);
	RenderImage(CNewUICharacterInfoWindow::IMAGE_CHAINFO_RIGHT, m_Pos.x + 169, m_Pos.y + 64.f, 21.f, 156.f);
	RenderImage(CNewUICharacterInfoWindow::IMAGE_CHAINFO_BOTTOM, m_Pos.x, m_Pos.y + 220.f, 190.f, 45.f);

	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 10, GlobalText[3790], 190, 0, RT3_SORT_CENTER);

	RenderImage(IMAGE_CHAINFO_TEXTBOX, m_Pos.x + 12, m_Pos.y + 60, 170.f, 21.f);
	RenderImage(IMAGE_CHAINFO_TEXTBOX, m_Pos.x + 12, m_Pos.y + 94, 170.f, 21.f);
	RenderImage(IMAGE_CHAINFO_TEXTBOX, m_Pos.x + 12, m_Pos.y + 128, 170.f, 21.f);
	RenderImage(IMAGE_CHAINFO_TEXTBOX, m_Pos.x + 12, m_Pos.y + 162, 170.f, 21.f);

	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 66,  GlobalText[1946], 190, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 100, GlobalText[1947], 190, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 18, m_Pos.y + 134, GlobalText[1971], 190, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 168, GlobalText[1983], 190, 0, RT3_SORT_LEFT);

	g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 194, GlobalText[1944], 190, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 205, GlobalText[1979], 190, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 216, GlobalText[1985], 190, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 227, GlobalText[1986], 190, 0, RT3_SORT_LEFT);

	char GetText[255];
	sprintf(GetText, "%s %s", GlobalText[1978], g_SPKSharedInfo.AccountID);
	g_pRenderText->SetTextColor(255, 255, 0, 255);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 35, GetText, 190);

	sprintf(GetText, "%s %s", GlobalText[1983], CharacterAttribute->PersonalCode);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 46, GetText, 190);

	sprintf(GetText, "%s", GlobalText[1978]);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 35, GetText, 190);

	sprintf(GetText, "%s", GlobalText[1983]);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 46, GetText, 190);
	
	g_pRenderText->SetTextColor(255, 0, 0, 255);

	if (m_bShowLengthWarningNew)
	{
		g_pRenderText->RenderText(m_Pos.x + 13, m_Pos.y + 117, GlobalText[1941], 190);
	}

	if (m_bShowLengthWarningRNew)
	{
		g_pRenderText->RenderText(m_Pos.x + 13, m_Pos.y + 150, GlobalText[1942], 190);
	}

	if (m_bShowNotMatchWarning)
	{
		g_pRenderText->RenderText(m_Pos.x + 13, m_Pos.y + 150, GlobalText[1943], 190);
	}

	if (m_bShowInvalidCodeWarning)
	{
		g_pRenderText->RenderText(m_Pos.x + 13, m_Pos.y + 185, GlobalText[1943], 1984);
	}

	m_Btn[0].Render();

	if (m_pOldPassInput)		m_pOldPassInput->Render();
	if (m_pNewPassInput)		m_pNewPassInput->Render();
	if (m_pReNewPassInput)		m_pReNewPassInput->Render();
	if (m_pPersionCodeInput)	m_pPersionCodeInput->Render();

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIChangePass::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[0], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 124, m_Pos.y + 34, 52, 26, 1, 0, 1, 1u, GlobalText[1940],"", 0);
}

#endif