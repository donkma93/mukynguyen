#include "stdafx.h"
#include "TrayModeNew.h"
#include "resource.h"
#include "CProtect.h"
#include "MapManager.h"
#include "wsclientinline.h"

TrayMode gTrayMode;

void TrayMode::Load()
{
	this->TempWindowProc	= NULL;
	this->TempIconProc		= NULL;
	this->InTray			= false;
}
void TrayMode::SwitchState()
{
	if( IsWindowVisible(g_hWnd) )
	{
		if(GetForegroundWindow() == g_hWnd)
		{
			ShowWindow(g_hWnd, SW_HIDE);
			UpdateWindow(g_hWnd);
			this->ShowNotify(true);
			this->ShowMessage(NIIF_INFO, gProtect->m_MainInfo.WindowName, "Game Da Duoc Thu Nho");
		}
	}
	else
	{
		ShowWindow(g_hWnd, SW_SHOW);
		UpdateWindow(g_hWnd);
		this->ShowNotify(false);
	}
}
void TrayMode::ShowNotify(bool Mode)
{
	this->InTray = Mode;
	NOTIFYICONDATA Icon		= { 0 };
	Icon.cbSize				= sizeof(NOTIFYICONDATA);
	Icon.uID				= TRAYMODE_ICON_ID;
	Icon.uFlags				= NIF_ICON|NIF_MESSAGE|NIF_TIP;
	Icon.hIcon				= (HICON)LoadImage(this->Instance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	this->TempIconProc		= Icon.hIcon;
	Icon.hWnd				= g_hWnd;
	Icon.uCallbackMessage	= TRAYMODE_ICON_MESSAGE;
	strcpy_s(Icon.szTip, sizeof(Icon.szTip), gProtect->m_MainInfo.WindowName);
	if(SceneFlag == MAIN_SCENE)
	{
		char Text[120];
		sprintf(Text, "\n=====================\nName: %s\nLevel: %d\nLocation: %s (%d, %d)\n=====================", Hero->ID, CharacterAttribute->Level, gMapManager.GetMapName(gMapManager.WorldActive), Hero->PositionX, Hero->PositionY);
		strcat_s(Icon.szTip, Text);
	}
	if( Mode )
	{
		Shell_NotifyIcon(NIM_ADD, &Icon);
	}
	else
	{
		Shell_NotifyIcon(NIM_DELETE, &Icon);
	}
	DeleteObject(Icon.hIcon);
}

void TrayMode::ShowMessage(DWORD Type, char * Title, char * Message)
{
	NOTIFYICONDATA Icon		= { 0 };
	Icon.cbSize				= sizeof(NOTIFYICONDATA);
	Icon.uID				= TRAYMODE_ICON_ID;
	Icon.hWnd				= g_hWnd;
	Icon.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_INFO;
	Icon.hIcon				= this->TempIconProc;
	Icon.uCallbackMessage	= TRAYMODE_ICON_MESSAGE;
	Icon.dwInfoFlags		= Type;
	Icon.uTimeout			= 5000;
	strcpy(Icon.szInfo, Message);
	strcpy(Icon.szInfoTitle, Title);
	Shell_NotifyIcon(NIM_MODIFY, &Icon);
}
LRESULT TrayMode::Window(HWND Window, DWORD Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case TRAYMODE_ICON_MESSAGE:
		{
			switch (lParam)
			{
			case WM_LBUTTONDBLCLK:
			{
				gTrayMode.SwitchState();
			}
			break;
			}
		}
		break;
	}
	return CallWindowProc((WNDPROC)gTrayMode.TempWindowProc, Window, Message, wParam, lParam);
}