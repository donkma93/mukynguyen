#include "stdafx.h"
#include "ServerDisplayer.h"
#include "CustomArena.h"
#include "GameMain.h"
#include "Log.h"
#include "Protect.h"
#include "resource.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "User.h"
#include "../../Addon/DualColor.h"
#include "InvasionManager.h"

CServerDisplayer gServerDisplayer;

CServerDisplayer::CServerDisplayer()
{
	this->EventBc = -1;
	AddFontResourceExA("..\\..\\..\\Tool\\Font.ttf", FR_PRIVATE, 0);

	for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	{
		memset(&this->m_log[n], 0, sizeof(this->m_log[n]));
	}

	this->m_font = CreateFont(
		17, 0, 0, 0,
		100, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		5,
		DEFAULT_PITCH | FF_DONTCARE,
		"Roboto Condensed"
	);
	this->m_brush[0] = CreateSolidBrush(Spk_vBlack);
	this->m_brush[1] = CreateSolidBrush(Spk_vBlue);
	this->m_brush[2] = CreateSolidBrush(Spk_vRed);
	this->m_brush[3] = CreateSolidBrush(Spk_vWhite);
	this->m_brush[4] = CreateSolidBrush(Spk_vPink);
}


CServerDisplayer::~CServerDisplayer()
{
	DeleteObject(this->m_font);
	DeleteObject(this->m_brush[0]);
	DeleteObject(this->m_brush[1]);
	DeleteObject(this->m_brush[2]);
	DeleteObject(this->m_brush[3]);
	DeleteObject(this->m_brush[4]);
}

void CServerDisplayer::Init(HWND hWnd)
{
	PROTECT_START

	this->m_hwnd = hWnd;

	PROTECT_FINAL

	gLog.AddLog(1,"LOG");
	gLog.AddLog(gServerInfo.m_WriteChatLog,"LOG\\CHAT_LOG");
	gLog.AddLog(gServerInfo.m_WriteCommandLog,"LOG\\COMMAND_LOG");
	gLog.AddLog(gServerInfo.m_WriteTradeLog,"LOG\\TRADE_LOG");
	gLog.AddLog(gServerInfo.m_WriteConnectLog,"LOG\\CONNECT_LOG");
	gLog.AddLog(gServerInfo.m_WriteHackLog,"LOG\\HACK_LOG");
	gLog.AddLog(gServerInfo.m_WriteCashShopLog,"LOG\\CASH_SHOP_LOG");
	gLog.AddLog(gServerInfo.m_WriteChaosMixLog,"LOG\\CHAOS_MIX_LOG");
}

void CServerDisplayer::Run()
{
	this->LogTextPaint();
	this->SetWindowName();
	this->PaintInvasionTime();
	this->PaintCustomArenaTime();
	this->PaintEventTime();

	#if(GAMESERVER_TYPE !=1)
	this->LogTextPaintConnect();
	#endif
}

void CServerDisplayer::SetWindowName()
{
	char buff[256];
	wsprintf(buff, " %s: %d", gServerInfo.m_ServerName, gObjTotalUser);
	SetWindowText(this->m_hwnd, buff);

	HWND hWndStatusBar = GetDlgItem(this->m_hwnd, IDC_STATUSBAR);
	RECT rect;
	GetClientRect(this->m_hwnd,&rect);
	RECT rect2;
	GetClientRect(hWndStatusBar,&rect2);
	MoveWindow(hWndStatusBar,0,rect.bottom-rect2.bottom+rect2.top,rect.right,rect2.bottom-rect2.top,true);
	int iStatusWidths[] = {220,300,390,480,610, -1};
	char text[256];
	SendMessage(hWndStatusBar, SB_SETPARTS, 6, (LPARAM)iStatusWidths);
	wsprintf(text, (const char*)RamCharError);
	SendMessage(hWndStatusBar, SB_SETTEXT, 0,(LPARAM)text);
	wsprintf(text, "OffStore: %d", gObjOffStore);
	SendMessage(hWndStatusBar, SB_SETTEXT, 1,(LPARAM)text);
	wsprintf(text, "OffAttack: %d", gObjOffAttack);
	SendMessage(hWndStatusBar, SB_SETTEXT, 2,(LPARAM)text);
	wsprintf(text, "Bots Buffer: %d", gObjTotalBot);
	SendMessage(hWndStatusBar, SB_SETTEXT, 3,(LPARAM)text);
	wsprintf(text, "Monsters: %d/%d", gObjTotalMonster,MAX_OBJECT_MONSTER);
	SendMessage(hWndStatusBar, SB_SETTEXT, 4,(LPARAM)text);
	SendMessage(hWndStatusBar, SB_SETTEXT, 5,(LPARAM)NULL);
	ShowWindow(hWndStatusBar, SW_SHOW);
}

void CServerDisplayer::ShowEventDisplay(RECT rect, HDC hdc, LPCWSTR StringName, int EventName, int X1, int X2, int Y) //ThangCuoi
{
	wchar_t EventOfName[50];
	wchar_t RestName[50] = { 0 };
	int totalseconds, hours, minutes, seconds, days;

	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintfW(EventOfName, L"%s", StringName);

	if (EventName == -1)
	{
		wcscpy_s(RestName, L"OFF");
	}
	else if (EventName == 0)
	{
		wcscpy_s(RestName, L"ON");
	}
	else
	{
		totalseconds = EventName;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;
		swprintf_s(RestName, L"%02d:%02d:%02d", hours, minutes, seconds);
	}

	TextOutW(hdc, X1, rect.top + Y, EventOfName, wcslen(EventOfName));

	if (EventName == -1)
	{
		SetTextColor(hdc, Spk_vRed);
	}
	else if (EventName == 0)
	{
		SetTextColor(hdc, Spk_vBlack);
	}
	else if (EventName < 300)
	{
		SetTextColor(hdc, Spk_vDarkGreen);
	}
	else
	{
		SetTextColor(hdc, RGB(0, 0, 0));
	}

	TextOutW(hdc, X2 - 15, rect.top + Y, RestName, wcslen(RestName));
}


void CServerDisplayer::PaintEventTime()
{
	#if(GAMESERVER_TYPE != 1 )
	RECT rect;
	GetClientRect(this->m_hwnd, &rect);
	int posX1 = rect.right - 225;
	int posX2 = rect.right - 60;

	rect.left = rect.right - 230;
	rect.right = rect.right - 5;
	rect.bottom = rect.bottom - 20;
	rect.top = 450;

	HDC hdc = GetDC(this->m_hwnd);
	int OldBkMode = SetBkMode(hdc, TRANSPARENT);
	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font);

	SetTextColor(hdc, RGB(153, 0, 76));
	LPCWSTR TitleMainEvent = L"Sự Kiện Game";
	TextOutW(hdc, rect.left + 5, rect.top, TitleMainEvent, wcslen(TitleMainEvent));

	this->ShowEventDisplay(rect, hdc, L"Huyết Lâu",		        this->EventBc,		posX1, posX2, 17);
	this->ShowEventDisplay(rect, hdc, L"Quảng Trường Quỷ",		this->EventDs,		posX1, posX2, 33);
	this->ShowEventDisplay(rect, hdc, L"Hỗn Nguyên Lâu",	    this->EventCc,		posX1, posX2, 49);
	this->ShowEventDisplay(rect, hdc, L"Công Thành Mini",		this->EventCTCMini, posX1, posX2, 65);

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
	#endif
}

void CServerDisplayer::PaintInvasionTime()
{
	#if(GAMESERVER_TYPE != 1)
	RECT rect;
	GetClientRect(this->m_hwnd, &rect);
	int posX1 = rect.right - 225;
	int posX2 = rect.right - 60;
	rect.left = rect.right - 230;
	rect.right = rect.right + 15;
	rect.bottom = rect.bottom - 20;
	rect.top = 80;
	HDC hdc = GetDC(this->m_hwnd);
	int OldBkMode = SetBkMode(hdc, TRANSPARENT);
	FillRect(hdc, &rect, this->m_brush[4]);
	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font);
	WCHAR text1[MAX_PATH];
	char text2[255];
	int totalseconds, hours, minutes, seconds, days;

	SetTextColor(hdc, RGB(0, 173, 181));

	LPCWSTR TitleMainEvent = L"Boss Sự Kiện";
	TextOutW(hdc, rect.left + 5, rect.top + 2, TitleMainEvent, wcslen(TitleMainEvent));

	for (int n = 0; n < 20; n++)
	{
		SetTextColor(hdc, RGB(0, 102, 204));

		MultiByteToWideChar(CP_UTF8, 0, gInvasionManager.m_InvasionInfo[n].AlertMessage, -1, text1, MAX_PATH);

		TextOutW(hdc, posX1, rect.top + 20 + (15 * n), text1, wcslen(text1));

		if (this->EventInvasion[n] == -1)
		{
			wsprintf(text2, "");
		}
		else if (this->EventInvasion[n] == 0)
		{
			wsprintf(text2, "Online");
		}
		else
		{
			totalseconds = this->EventInvasion[n];
			hours = totalseconds / 3600;
			minutes = (totalseconds / 60) % 60;
			seconds = totalseconds % 60;
			if (hours > 23)
			{
				days = hours / 24;
				wsprintf(text2, "%d ngày", days);
			}
			else
			{
				wsprintf(text2, "%02d:%02d", hours, minutes);
			}
		}

		if (this->EventInvasion[n] == -1)
		{
			SetTextColor(hdc, Spk_vRed);
		}
		else if (this->EventInvasion[n] == 0)
		{
			SetTextColor(hdc, Spk_vBlue);
		}
		else if (this->EventInvasion[n] < 300)
		{
			SetTextColor(hdc, Spk_vDarkGreen);
		}
		else
		{
			SetTextColor(hdc, RGB(000, 000, 000));
		}
		TextOut(hdc, posX2 - 15, rect.top + 20 + (15 * n), text2, strlen(text2));
	}
	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
	#endif
}

void CServerDisplayer::PaintCustomArenaTime()
{
	#if(GAMESERVER_TYPE==1)
	RECT rect;
	GetClientRect(this->m_hwnd,&rect);

	int posX1 = rect.right-445;
	int posX2 = rect.right-355;
	rect.left	= rect.right-450;
	rect.right	= rect.right-5;
	rect.top	= 85;
	rect.bottom = 322;

	HDC hdc = GetDC(this->m_hwnd);
	int OldBkMode = SetBkMode(hdc,TRANSPARENT);
	FillRect(hdc,&rect,this->m_brush[4]);
	HFONT OldFont = (HFONT)SelectObject(hdc,this->m_font);

	char text1[20];
	char text2[30];
	char text3[30];
	char text4[30];
	int totalseconds;
	int hours;
	int minutes;
	int seconds;
	int days;

	SetTextColor(hdc,RGB(0, 173, 181));
	TextOut(hdc,rect.left+5,rect.top + 12,"EVENTS:",7);

	SetTextColor(hdc,RGB(0,102,204));
	wsprintf(text1, "Loren Deep:");

	if (this->EventCastleDeep == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventCastleDeep == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds	= this->EventCastleDeep;
		hours			= totalseconds/3600;
		minutes			= (totalseconds/60) % 60;  
		seconds			= totalseconds % 60;

		if (hours > 23)
		{
			days = hours/24;
			wsprintf(text2, "%d ngay", days);
		}
		else
		{
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	TextOut(hdc, posX1, rect.top + 30, text1, strlen(text1));
	if (this->EventCastleDeep == -1)
	{
		SetTextColor(hdc,RGB(255,0,0));
	}
	else if (this->EventCastleDeep == 0)
	{
		SetTextColor(hdc,RGB(0,190,0));
	}
	else if (this->EventCastleDeep < 300)
	{
		SetTextColor(hdc,RGB(0,190,0));
	}
	else
	{
		SetTextColor(hdc,RGB(0, 0, 0));
	}
	TextOut(hdc, posX2 + 10, rect.top + 30, text2, strlen(text2));

	SetTextColor(hdc,RGB(0,102,204));
	wsprintf(text1, "CryWolf: ");

	if (this->EventCryWolf == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventCryWolf == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds	= this->EventCryWolf;
		hours			= totalseconds/3600;
		minutes			= (totalseconds/60) % 60;  
		seconds			= totalseconds % 60;

		if (hours > 23)
		{
			days = hours/24;
			wsprintf(text2, "%d ngay", days);
		}
		else
		{
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	TextOut(hdc, posX1, rect.top + 50, text1,strlen(text1));
	if (this->EventCryWolf == -1)
	{
		SetTextColor(hdc,RGB(255,0,0));
	}
	else if (this->EventCryWolf == 0)
	{
		SetTextColor(hdc,RGB(0,190,0));
	}
	else if (this->EventCryWolf < 300)
	{
		SetTextColor(hdc,RGB(0,190,0));
	}
	else
	{
		SetTextColor(hdc, RGB(0,0,0));
	}
	TextOut(hdc, posX2 + 10, rect.top + 50,text2,strlen(text2));

	SetTextColor(hdc,RGB(0,102,204));
	wsprintf(text1, "Castle Siege: ");

	if (this->EventCs == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventCs == 0)
	{
		wsprintf(text2, "Adjust Date");
		wsprintf(text3, " ");
		wsprintf(text4, " ");
	}
	else
	{
		totalseconds	= this->EventCs;
		hours			= totalseconds/3600;
		minutes			= (totalseconds/60) % 60;  
		seconds			= totalseconds % 60;

		if (hours > 23)
		{
			days = hours/24;
			wsprintf(text4, "- Next Stage: %d ngay", days);
		}
		else
		{
			wsprintf(text4, "- Next Stage: %02d:%02d:%02d", hours, minutes, seconds);
		}

		if(this->EventCsState == -1)
			wsprintf(text3, "- Stage %d: None", this->EventCsState);
		if(this->EventCsState == 0)
			wsprintf(text3, "- Stage %d: Idle 1", this->EventCsState);
		if(this->EventCsState == 1)
			wsprintf(text3, "- Stage %d: Guild Register", this->EventCsState);
		if(this->EventCsState == 2)
			wsprintf(text3, "- Stage %d: Idle 2", this->EventCsState);
		if(this->EventCsState == 3)
			wsprintf(text3, "- Stage %d: Mark Register", this->EventCsState);
		if(this->EventCsState == 4)
			wsprintf(text3, "- Stage %d: Idle 3", this->EventCsState);
		if(this->EventCsState == 5)
			wsprintf(text3, "- Stage %d: Notify", this->EventCsState);
		if(this->EventCsState == 6)
			wsprintf(text3, "- Stage %d: Ready Siege", this->EventCsState);
		if(this->EventCsState == 7)
			wsprintf(text3, "- Stage %d: Started Siege", this->EventCsState);
		if(this->EventCsState == 8)
			wsprintf(text3, "- Stage %d: Ended Siege", this->EventCsState);
		if(this->EventCsState == 9)
			wsprintf(text3, "- Stage %d: End All", this->EventCsState);
			
		if (this->EventCs)
			wsprintf(text2, "Stage %d", this->EventCsState);
	}

	TextOut(hdc, posX1, rect.top + 70, text1, strlen(text1));
	if (this->EventCs == -1)
	{
		SetTextColor(hdc,RGB(255,0,0));
	}
	else
	{
		SetTextColor(hdc, RGB(0,0,0));
	}
	TextOut(hdc,posX2 + 10,rect.top +70, text2,strlen(text2));
	TextOut(hdc,posX1 + 10,rect.top +90, text3,strlen(text3));
	TextOut(hdc,posX1 + 10,rect.top +110,text4,strlen(text4));

	SelectObject(hdc,OldFont);
	SetBkMode(hdc,OldBkMode);
	ReleaseDC(this->m_hwnd,hdc);
	#endif
}

void CServerDisplayer::LogTextPaint()
{
	RECT rect;
	GetClientRect(this->m_hwnd,&rect);
	rect.top = rect.top + 80;
	HDC hdc = GetDC(this->m_hwnd);
	HFONT OldFont = (HFONT)SelectObject(hdc,this->m_font);
	int OldBkMode = SetBkMode(hdc,TRANSPARENT);
	FillRect(hdc,&rect,this->m_brush[3]);
	#define MaxLineText 40
	int line = MaxLineText;
	int count = (((this->m_count-1)>=0)?(this->m_count-1):(MaxLineText -1));
	for(int n=0;n < MaxLineText ;n++)
	{
		switch(this->m_log[count].color)
		{
			case LOG_BLACK:		SetTextColor(hdc,Spk_vBlack);		break;
			case LOG_RED:		SetTextColor(hdc,Spk_vRed);			break;
			case LOG_GREEN:		SetTextColor(hdc,Spk_vGreen);		break;
			case LOG_DARKGREEN:	SetTextColor(hdc,Spk_vDarkGreen);	break;
			case LOG_BLUE:		SetTextColor(hdc,Spk_vBlue);		break;
		}
		int SPK_Size = strlen(this->m_log[count].text); 
		WCHAR SPK_CharUnicode[300];
		int nn = MultiByteToWideChar(CP_UTF8, 0, this->m_log[count].text, SPK_Size, SPK_CharUnicode, 100);
		if(nn > 1)
		{
			TextOutW(hdc, rect.left + 10, (rect.top + (line * 15)) - 10, SPK_CharUnicode, nn);
			line--;
		}
		count = (((--count)>=0)?count:(MaxLineText-1));
	}
	SelectObject(hdc,OldFont);
	ReleaseDC(this->m_hwnd,hdc);
}
void CServerDisplayer::LogTextPaintConnect()
{
	RECT rect;

	GetClientRect(this->m_hwnd,&rect);

	rect.left	= rect.right	-	560;
	rect.right	= rect.right	-	230;
	rect.bottom = rect.bottom	-	22;
	rect.top	= rect.top + 80;

	HDC hdc = GetDC(this->m_hwnd);
	int OldBkMode = SetBkMode(hdc,TRANSPARENT);
	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font);
	FillRect(hdc, &rect, this->m_brush[4]);
	SetTextColor(hdc,RGB(153,0,76));

	LPCWSTR TitleMainEvent = L"Nhật Ký Đăng Nhập";
	TextOutW(hdc, rect.left + 5, rect.top + 5, TitleMainEvent, wcslen(TitleMainEvent));

	int H = MAX_LOGCONNECT_TEXT_LINE;
	int line = H;
	int count = (((this->m_countConnect - 1) >= 0) ? (this->m_countConnect - 1) : (H - 1));

	for (int n = 0; n < H; n++)
	{
		switch (this->m_logConnect[count].color)
		{
			case 0:		SetTextColor(hdc,RGB(0,0,0));		break;
			case 1:		SetTextColor(hdc,RGB(255,0,0));		break;
			case 2:		SetTextColor(hdc,RGB(0,190,0));		break;
			case 3:		SetTextColor(hdc,RGB(0,0,255));		break;
			case 4:		SetTextColor(hdc,RGB(255,0,64));	SetBkMode(hdc,TRANSPARENT);		break;
			case 5:		SetTextColor(hdc,RGB(254,154,46));	SetBkMode(hdc,TRANSPARENT);		break;
			case 6:		SetTextColor(hdc,RGB(0,102,204));	SetBkMode(hdc,TRANSPARENT);		break;
			case 7:		SetTextColor(hdc,RGB(153,0,76));	SetBkMode(hdc, TRANSPARENT);	break;
		}

		int size = strlen(this->m_logConnect[count].text);

		WCHAR text_unicode[300];
		int nn = MultiByteToWideChar(CP_UTF8, 0, this->m_logConnect[count].text, size, text_unicode, 100);
		if (nn > 1)
		{
			TextOutW(hdc, rect.left + 10, (rect.top + 15 + (line * 15)), text_unicode, nn);
			line--;
		}
		count = (((--count) >= 0) ? count : (H - 1));
	}

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::LogAddText(eLogColor color,char* text,int size)
{
	PROTECT_START
	size = ((size>=MAX_LOG_TEXT_SIZE)?(MAX_LOG_TEXT_SIZE-1):size);
	memset(&this->m_log[this->m_count].text,0,sizeof(this->m_log[this->m_count].text));
	memcpy(&this->m_log[this->m_count].text,text,size);
	this->m_log[this->m_count].color = color;
	this->m_count = (((++this->m_count)>=MAX_LOG_TEXT_LINE)?0:this->m_count);
	PROTECT_FINAL
	gLog.Output(LOG_GENERAL,"%s",&text[9]);
}

void CServerDisplayer::LogAddTextConnect(eLogColor color,char* text,int size)
{
	PROTECT_START
	size = ((size>=MAX_LOGCONNECT_TEXT_SIZE)?(MAX_LOGCONNECT_TEXT_SIZE-1):size);
	memset(&this->m_logConnect[this->m_countConnect].text,0,sizeof(this->m_logConnect[this->m_countConnect].text));
	memcpy(&this->m_logConnect[this->m_countConnect].text,text,size);
	this->m_logConnect[this->m_countConnect].color = color;
	this->m_countConnect = (((++this->m_countConnect)>=MAX_LOGCONNECT_TEXT_LINE)?0:this->m_countConnect);
	PROTECT_FINAL
	gLog.Output(LOG_GENERAL,"%s",&text[9]);
}

void CServerDisplayer::LogAddTextGlobal(eLogColor color,char* text,int size)
{
	PROTECT_START
	size = ((size>=MAX_LOGGLOBAL_TEXT_SIZE)?(MAX_LOGGLOBAL_TEXT_SIZE-1):size);
	memset(&this->m_logGlobal[this->m_countGlobal].text,0,sizeof(this->m_logGlobal[this->m_countGlobal].text));
	memcpy(&this->m_logGlobal[this->m_countGlobal].text,text,size);
	this->m_logGlobal[this->m_countGlobal].color = color;
	this->m_countGlobal = (((++this->m_countGlobal)>=MAX_LOGGLOBAL_TEXT_LINE)?0:this->m_countGlobal);
	PROTECT_FINAL
}

unsigned char RamCharError[] =
{
	0x5A, 0x61, 0x6C, 0x6F, 0x3A, 0x20, 0x30, 0x37, 0x37, 0x35, 0x20, 0x38, 0x33, 0x38, 0x20, 0x38,
	0x35, 0x38, 0x20, 0x2D, 0x20, 0x5B, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x35, 0x2E, 0x32, 0x20, 0x62, 0x79,
	0x20, 0x53, 0x50, 0x4B, 0x5D, 0x00
};