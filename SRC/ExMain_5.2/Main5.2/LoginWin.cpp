//*****************************************************************************
// File: LoginWin.cpp
//*****************************************************************************

#include "stdafx.h"
#include "LoginWin.h"
#include "Input.h"
#include "UIMng.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "UIControls.h"
#include "ZzzScene.h"
#include "wsclientinline.h"
#include "DSPlaySound.h"
#include "./Utilities/Log/muConsoleDebug.h"
#include "ThangCuoi\CProtect.h"

#include "ServerListManager.h"
#include "ThangCuoi\WideData.h"
#include "ThangCuoi\Reconnect.h"
#define	LIW_ACCOUNT		0
#define	LIW_PASSWORD	1

#define LIW_OK			0
#define LIW_CANCEL		1

extern SPK_SHARED_INFO g_SPKSharedInfo;
extern int g_iChatInputType;
CLoginWin::CLoginWin()
{
	m_pIDInputBox = NULL;
	m_pPassInputBox = NULL;
	//m_bRenderSacePass = true;
}

CLoginWin::~CLoginWin()
{
	SAFE_DELETE(m_pIDInputBox);
	SAFE_DELETE(m_pPassInputBox);
}

void WriteStringRegedit(const char* szID, const char* szPass)
{
	HKEY hKey;
	if (RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
	{
		RegSetValueExA(hKey, "ID", 0, REG_SZ, (const BYTE*)szID, strlen(szID) + 1);
		RegSetValueExA(hKey, "PW", 0, REG_SZ, (const BYTE*)szPass, strlen(szPass) + 1);
		RegCloseKey(hKey);
	}
}
bool WriteDword(DWORD value) {
	HKEY	hKey = NULL;
	DWORD	dwDisp;
	DWORD	dwSize = sizeof(DWORD);

	if (ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp))
		return false;

	RegSetValueEx(hKey, "SavePass", 0L, REG_DWORD, (BYTE*)&value, dwSize);
	RegCloseKey(hKey);
	return true;
}
DWORD ReadSavePass()
{
	HKEY hKey;
	DWORD dwValue = 0;
	DWORD dwSize = sizeof(dwValue);

	if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		RegQueryValueExA(hKey, "SavePass", 0, NULL, (BYTE*)&dwValue, &dwSize);
		RegCloseKey(hKey);
	}

	return dwValue;
}

void CLoginWin::Create()
{
	CWin::Create(329, 245, BITMAP_LOG_IN + 7);

	m_asprInputBox[LIW_ACCOUNT].Create(156, 23, BITMAP_LOG_IN + 8);
	m_asprInputBox[LIW_PASSWORD].Create(156, 23, BITMAP_LOG_IN + 8);

	for (int i = 0; i < 2; ++i)
	{
		m_aBtn[i].Create(54, 30, BITMAP_BUTTON + i, 3, 2, 1);
		CWin::RegisterButton(&m_aBtn[i]);
	}

	SAFE_DELETE(m_pIDInputBox);

	m_pIDInputBox = new CUITextInputBox;
	m_pIDInputBox->Init(g_hWnd, 140, 14, MAX_ID_SIZE);
	m_pIDInputBox->SetBackColor(0, 0, 0, 255);
	m_pIDInputBox->SetTextColor(255, 255, 230, 210);
	m_pIDInputBox->SetFont(g_hFont);
	m_pIDInputBox->SetState(UISTATE_NORMAL);
	m_pIDInputBox->SetText(m_ID);

	SAFE_DELETE(m_pPassInputBox);

	m_pPassInputBox = new CUITextInputBox;
	m_pPassInputBox->Init(g_hWnd, 140, 14, MAX_PASSWORD_SIZE, TRUE);
	m_pPassInputBox->SetBackColor(0, 0, 0, 25);
	m_pPassInputBox->SetTextColor(255, 255, 230, 210);
	m_pPassInputBox->SetFont(g_hFont);
	m_pPassInputBox->SetState(UISTATE_NORMAL);

	SavePass = ReadSavePass();
	if (SavePass)
	{
		m_pPassInputBox->SetText(m_PW);
	}

	m_pIDInputBox->SetTabTarget(m_pPassInputBox);
	m_pPassInputBox->SetTabTarget(m_pIDInputBox);


	this->FirstLoad = 1;
}

void CLoginWin::PreRelease()
{
	for (int i = 0; i < 2; ++i)
		m_asprInputBox[i].Release();
}

void CLoginWin::SetPosition(int nXCoord, int nYCoord)
{
	CWin::SetPosition(nXCoord, nYCoord);

	m_asprInputBox[LIW_ACCOUNT].SetPosition(nXCoord + 109, nYCoord + 106);
	m_asprInputBox[LIW_PASSWORD].SetPosition(nXCoord + 109, nYCoord + 131);

	if (g_iChatInputType == 1)
	{
		m_pIDInputBox->SetPosition(int((m_asprInputBox[LIW_ACCOUNT].GetXPos() + 6) / gPosWide.x_fScreenRate_x),
			int((m_asprInputBox[LIW_ACCOUNT].GetYPos() + 6) / gPosWide.x_fScreenRate_y));

		m_pPassInputBox->SetPosition(int((m_asprInputBox[LIW_PASSWORD].GetXPos() + 6) / gPosWide.x_fScreenRate_x),
			int((m_asprInputBox[LIW_PASSWORD].GetYPos() + 6) / gPosWide.x_fScreenRate_y));
	}

	m_aBtn[LIW_OK].SetPosition(nXCoord + 150, nYCoord + 188);
	m_aBtn[LIW_CANCEL].SetPosition(nXCoord + 211, nYCoord + 188);

	
}

void CLoginWin::Show(bool bShow)
{
	CWin::Show(bShow);

	for (int i = 0; i < 2; ++i)
	{
		m_asprInputBox[i].Show(bShow);
		m_aBtn[i].Show(bShow);
	}
}

bool CLoginWin::CursorInWin(int nArea)
{
	if (!CWin::m_bShow)
		return false;

	switch (nArea)
	{
	case WA_MOVE:
		return false;
	}

	return CWin::CursorInWin(nArea);
}

void CLoginWin::UpdateWhileActive(double dDeltaTick)
{
	CInput& rInput = CInput::Instance();

	if (m_aBtn[LIW_OK].IsClick())
		RequestLogin();
	else if (m_aBtn[LIW_CANCEL].IsClick())
		CancelLogin();
	else if (CInput::Instance().IsKeyDown(VK_RETURN))
	{
		::PlayBuffer(SOUND_CLICK01);
		RequestLogin();
	}
	else if (CInput::Instance().IsKeyDown(VK_ESCAPE))
	{
		::PlayBuffer(SOUND_CLICK01);
		CancelLogin();
		CUIMng::Instance().SetSysMenuWinShow(false);
	}
}

void CLoginWin::UpdateWhileShow(double dDeltaTick)
{
	m_pIDInputBox->DoAction();
	m_pPassInputBox->DoAction();
}

inline int LoginSavePassX()
{
	int Value = 0;
	switch (m_Resolution)
	{
		case 0:  Value = 200; break;
		case 1:  Value = 200; break;
		case 2:  Value = 200; break;
		case 3:  Value = 194; break;
		case 4:  Value = 193; break;
		case 5:  Value = 191; break;
		case 6:  Value = 191; break;
		case 7:  Value = 187; break;
		case 8:  Value = 187; break;
		case 9:  Value = 187; break;
		case 10: Value = 187; break;
	}
	return Value;
}

inline int LoginSavePassY()
{
	int Value = 0;
	switch (m_Resolution)
	{
		case 0:  Value = 162; break;
		case 1:  Value = 162; break;
		case 2:  Value = 162; break;
		case 3:  Value = 162; break;
		case 4:  Value = 162; break;
		case 5:  Value = 158; break;
		case 6:  Value = 159; break;
		case 7:  Value = 157; break;
		case 8:  Value = 157; break;
		case 9:  Value = 157; break;
		case 10:  Value = 157; break;
	}
	return Value;
}
inline int LoginSavePassTextY()
{
	int Value = 0;
	switch (m_Resolution)
	{
		case 0:  Value = 165; break;
		case 1:  Value = 165; break;
		case 2:  Value = 165; break;
		case 3:  Value = 165; break;
		case 4:  Value = 165; break;
		case 5:  Value = 163; break;
		case 6:  Value = 165; break;
		case 7:  Value = 164; break;
		case 8:  Value = 164; break;
		case 9:  Value = 165; break;
		case 10:  Value = 166; break;
	}
	return Value;
}
void CLoginWin::RenderControls()
{
	if (this->FirstLoad == 1)
	{
		if (strlen(m_ID) > 0)
			CUIMng::Instance().m_LoginWin.GetPassInputBox()->GiveFocus();
		else
			CUIMng::Instance().m_LoginWin.GetIDInputBox()->GiveFocus();
		this->FirstLoad = 0;
	}

	CWin::RenderButtons();

	for (int i = 0; i < 2; ++i)
		m_asprInputBox[i].Render();

	m_pIDInputBox->Render();
	m_pPassInputBox->Render();

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
	g_pRenderText->RenderText(int((CWin::GetXPos() + 30) / gPosWide.x_fScreenRate_x),
		int((CWin::GetYPos() + 113) / gPosWide.x_fScreenRate_y), GlobalText[450]);
	g_pRenderText->RenderText(int((CWin::GetXPos() + 30) / gPosWide.x_fScreenRate_x),
		int((CWin::GetYPos() + 139) / gPosWide.x_fScreenRate_y), GlobalText[451]);
	g_pRenderText->RenderText(int((CWin::GetXPos() + 222) / gPosWide.x_fScreenRate_x),
		int((CWin::GetYPos() + LoginSavePassTextY()) / gPosWide.x_fScreenRate_y), GlobalText[3766]);

	unicode::t_char szServerName[MAX_TEXT_LENGTH];

	const char* apszGlobalText[4] = 
	{ 
		gProtect->m_MainInfo.ServerName[0], 
		gProtect->m_MainInfo.ServerName[1],
		gProtect->m_MainInfo.ServerName[2], 
		gProtect->m_MainInfo.ServerName[3]
	};
	sprintf(szServerName, apszGlobalText[g_ServerListManager->GetNonPVPInfo()],
		g_ServerListManager->GetSelectServerName(), g_ServerListManager->GetSelectServerIndex(), "");

	g_pRenderText->RenderText(int((CWin::GetXPos() + 111) / gPosWide.x_fScreenRate_x),
		int((CWin::GetYPos() + 80) / gPosWide.x_fScreenRate_y), szServerName);

	SavePass = ReadSavePass();

	if (SavePass == 1)
	{
		m_bRenderSacePass = true;
	}
	else
	{
		m_bRenderSacePass = false;
	}

	int ClickX = int((CWin::GetXPos() + LoginSavePassX()) / gPosWide.x_fScreenRate_x);
	int ClickY = int((CWin::GetYPos() + LoginSavePassY()) / gPosWide.x_fScreenRate_y);

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(ClickX, ClickY, 15, 15))
	{
		m_bRenderSacePass = !m_bRenderSacePass;

		if (m_bRenderSacePass)
		{
			WriteDword(1);
		}
		else
		{
			WriteDword(0);
		}
	}

	if (m_bRenderSacePass)
	{
		SEASON3B::RenderImage(SEASON3B::CNewUIOptionWindow::IMAGE_OPTION_BTN_CHECK, ClickX, ClickY, 15, 15, 0, 0);
	}
	else
	{
		SEASON3B::RenderImage(SEASON3B::CNewUIOptionWindow::IMAGE_OPTION_BTN_CHECK, ClickX, ClickY, 15, 15, 0, 15.f);
	}

	SEASON3B::RenderImage(SEASON3B::CNewUICryWolf::IMAGE_MVP_INTERFACE + 25, ClickX - 80, ClickY + 5, 13.f, 13.f);

	if (CheckMouseIn(ClickX - 80, ClickY + 5, 16, 17))
	{
		int TextNum = 0;

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		sprintf(TextList[TextNum++], "%s", GlobalText[1988]);

		for (int i = 1989; i <= 1994; i++)
		{
			sprintf(TextList[TextNum++], "%s", GlobalText[i]);
		}

		RenderTipTextList(ClickX - 180, MouseY, TextNum, 0);
	}
}

void CLoginWin::RequestLogin()
{
	if (CurrentProtocolState == REQUEST_JOIN_SERVER)
		return;

	CUIMng::Instance().HideWin(this);

	char szID[MAX_ID_SIZE + 1] = { 0, };
	char szPass[MAX_PASSWORD_SIZE + 1] = { 0, };
	m_pIDInputBox->GetText(szID, MAX_ID_SIZE + 1);
	m_pPassInputBox->GetText(szPass, MAX_PASSWORD_SIZE + 1);

	if (unicode::_strlen(szID) <= 0)
		CUIMng::Instance().PopUpMsgWin(MESSAGE_INPUT_ID);
	else if (unicode::_strlen(szPass) <= 0)
		CUIMng::Instance().PopUpMsgWin(MESSAGE_INPUT_PASSWORD);
	else
	{
		if (CurrentProtocolState == RECEIVE_JOIN_SERVER_SUCCESS)
		{
			g_ConsoleDebug->Write(MCD_NORMAL, "Login with the following account: %s / %s ", szID, szPass);

			memcpy(g_pReconnect->s_Data.ReconnectAccount, szID, 11);
			memcpy(g_pReconnect->s_Data.ReconnectPassword, szPass, 11); 

			g_ErrorReport.Write("> Login Request.\r\n");
			g_ErrorReport.Write("> Try to Login \"%s\"\r\n", szID);

			SavePass = ReadSavePass();

			if (SavePass == 1)
			{
				WriteStringRegedit(szID, szPass);
			}

			strncpy(g_SPKSharedInfo.AccountID, szID, 10);
			g_SPKSharedInfo.AccountID[10] = '\0';

			SendRequestLogIn(szID, szPass);

		}
	}
}

void CLoginWin::CancelLogin()
{
	ConnectConnectionServer();
	CUIMng::Instance().HideWin(this);
}
void CLoginWin::ConnectConnectionServer()
{
	LogIn = 0;
	CurrentProtocolState = REQUEST_JOIN_SERVER;
	CreateSocket(szServerIpAddress, g_ServerPort);
}