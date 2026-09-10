// ServerDisplayer.cpp: implementation of the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerDisplayer.h"
#include "AccountManager.h"
#include "JoinServerProtocol.h"
#include "Log.h"
#include "Protect.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "../../Addon/DualColor.h"
#include "Resource.h"
#include <Richedit.h>
#pragma comment(lib, "Riched20.lib") 

CServerDisplayer gServerDisplayer;

CServerDisplayer::CServerDisplayer() // OK
{
	AddFontResourceExA("..\\Tool\\Font.ttf", FR_PRIVATE, 0);

	for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	{
		memset(&this->m_log[n], 0, sizeof(this->m_log[n]));
	}

	this->SPK_gFont = CreateFont(
		17, 0, 0, 0,
		100, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		5,
		DEFAULT_PITCH | FF_DONTCARE,
		"Roboto Condensed"
	);
	this->m_brush[0] = CreateSolidBrush(Spk_vBlack);	// Đen
	this->m_brush[1] = CreateSolidBrush(Spk_vBlue);		// Xanh
	this->m_brush[2] = CreateSolidBrush(Spk_vRed);		// Nền tiêu đề
	this->m_brush[3] = CreateSolidBrush(Spk_vWhite);	// Nền nội dung
	this->m_brush[4] = CreateSolidBrush(Spk_vDarkGreen);// Nền nội dung
}

CServerDisplayer::~CServerDisplayer() // OK
{
	DeleteObject(this->SPK_gFont);
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
		LoadLibraryA("Msftedit.dll");
	this->m_hEditLog = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"RICHEDIT50W",
		L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
		0, 80, 586, 479,
		this->m_hwnd,
		(HMENU)2000,
		GetModuleHandle(NULL),
		NULL
	);

	SendMessage(this->m_hEditLog, WM_SETFONT, (WPARAM)this->SPK_gFont, TRUE);


	HWND hReloadButton = CreateWindowW(L"BUTTON", L"Reload Config", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 585, 80, 167, 30, this->m_hwnd, (HMENU)IDM_RELOAD_RELOAD, GetModuleHandle(NULL), NULL);
	SendMessageW(hReloadButton, WM_SETFONT, (WPARAM)this->SPK_gFont, TRUE);
	HWND hAboutButton = CreateWindowW(L"BUTTON", L"Support Sever", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 585, 111, 167, 30, this->m_hwnd, (HMENU)IDM_ABOUT, GetModuleHandle(NULL), NULL);
	SendMessageW(hAboutButton, WM_SETFONT, (WPARAM)this->SPK_gFont, TRUE);
	HWND hExitButton = CreateWindowW(L"BUTTON", L"Exit JoinSever", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 585, 142, 167, 30, this->m_hwnd, (HMENU)IDM_EXIT, GetModuleHandle(NULL), NULL);
	SendMessageW(hExitButton, WM_SETFONT, (WPARAM)this->SPK_gFont, TRUE);

	CreateDirectory("../7.Log", 0);
	CreateDirectory("../7.Log/Log_JS", 0);
	CreateDirectory("../7.Log/Log_JS/LogText", 0);
	CreateDirectory("../7.Log/Log_JS/LogAccount", 0);
	gLog.AddLog(1, "../7.Log/Log_JS/LogText");
	gLog.AddLog(1, "../7.Log/Log_JS/LogAccount");
}

void CServerDisplayer::Run() // OK
{
	this->SetWindowName();
}

void CServerDisplayer::SetWindowName() // OK
{
	char buff[256];
	wsprintf(buff,"%s",JS_TieuDe);
	SetWindowText(this->m_hwnd,buff);
}
void CServerDisplayer::LogTextPaint() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd,&rect);

	rect.top = 100;

	HDC hdc = GetDC(this->m_hwnd);
	HFONT OldFont = (HFONT)SelectObject(hdc,this->SPK_gFont);
	int OldBkMode = SetBkMode(hdc,TRANSPARENT);

	FillRect(hdc,&rect,this->m_brush[3]);

	int line = MAX_LOG_TEXT_LINE;

	int count = (((this->m_count-1)>=0)?(this->m_count-1):(MAX_LOG_TEXT_LINE-1));

	for(int n=0;n < MAX_LOG_TEXT_LINE;n++)
	{
		switch(this->m_log[count].color)
		{
			case LOG_BLACK:		SetTextColor(hdc,Spk_vBlack);		break;
			case LOG_RED:		SetTextColor(hdc,Spk_vRed);			break;
			case LOG_GREEN:		SetTextColor(hdc,Spk_vGreen);		break;
			case LOG_BLUE:		SetTextColor(hdc,Spk_vBlue);		break;
		}
		int SPK_Size = strlen(this->m_log[count].text);
		WCHAR SPK_CharUnicode[100];
		int nn = MultiByteToWideChar(CP_UTF8, 0, this->m_log[count].text, SPK_Size, SPK_CharUnicode, 100);
		if(nn > 1)
		{
			TextOutW(hdc, rect.left + 10, (rect.top + 5 + (line * 15)), SPK_CharUnicode, nn);
			line--;
		}

		count = (((--count)>=0)?count:(MAX_LOG_TEXT_LINE-1));
	}
	SelectObject(hdc,OldFont);
	ReleaseDC(this->m_hwnd,hdc);
}

void CServerDisplayer::LogAddText(eLogColor color, char* text, int size)
{
	size = ((size >= MAX_LOG_TEXT_SIZE) ? (MAX_LOG_TEXT_SIZE - 1) : size);
	memset(&this->m_log[this->m_count].text, 0, sizeof(this->m_log[this->m_count].text));
	memcpy(&this->m_log[this->m_count].text, text, size);
	this->m_log[this->m_count].color = color;
	this->m_count = (((++this->m_count) >= MAX_LOG_TEXT_LINE) ? 0 : this->m_count);

	if (this->m_hEditLog && IsWindow(this->m_hEditLog))
	{
		int wlen = MultiByteToWideChar(CP_UTF8, 0, text, size, NULL, 0);
		std::wstring wtext(wlen, 0);
		MultiByteToWideChar(CP_UTF8, 0, text, size, &wtext[0], wlen);
		wtext += L"\r\n";

		CHARRANGE cr;
		cr.cpMin = 0;
		cr.cpMax = 0;
		SendMessageW(this->m_hEditLog, EM_EXSETSEL, 0, (LPARAM)&cr);

		CHARFORMAT2W cf;
		memset(&cf, 0, sizeof(cf));
		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR;

		switch (color)
		{
			case LOG_RED:    cf.crTextColor = RGB(192, 57, 43); break;
			case LOG_GREEN:  cf.crTextColor = RGB(0, 128, 0); break;
			case LOG_BLUE:   cf.crTextColor = RGB(0, 0, 255); break;
			case LOG_ORANGE: cf.crTextColor = RGB(243, 156, 18); break;
			case LOG_PURPLE: cf.crTextColor = RGB(155, 89, 182); break;
			case LOG_GRAY:   cf.crTextColor = RGB(128, 128, 128); break;
			case LOG_BLACK:  cf.crTextColor = RGB(0, 0, 0); break;
			case LOG_DBLUE:  cf.crTextColor = RGB(13, 29, 92); break;
			default:         cf.crTextColor = RGB(0, 0, 0); break;
		}

		SendMessageW(this->m_hEditLog, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
		SendMessageW(this->m_hEditLog, EM_REPLACESEL, FALSE, (LPARAM)wtext.c_str());

		DWORD lineCount = SendMessageW(this->m_hEditLog, EM_GETLINECOUNT, 0, 0);
		if (lineCount > 1000)
		{
			CHARRANGE del;
			del.cpMin = SendMessageW(this->m_hEditLog, EM_LINEINDEX, 1000, 0);
			del.cpMax = -1;
			SendMessageW(this->m_hEditLog, EM_EXSETSEL, 0, (LPARAM)&del);
			SendMessageW(this->m_hEditLog, EM_REPLACESEL, FALSE, (LPARAM)L"");
		}
	}

	gLog.Output(LOG_GENERAL, "%s", &text[9]);
}