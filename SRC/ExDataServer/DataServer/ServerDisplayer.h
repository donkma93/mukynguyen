// ServerDisplayer.h: interface for the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_LOG_TEXT_LINE 28
#define MAX_LOG_TEXT_SIZE 120

enum eLogColor
{
	LOG_BLACK = 0,
	LOG_RED,
	LOG_GREEN,
	LOG_BLUE,
	LOG_ORANGE,
	LOG_PURPLE,
	LOG_GRAY,
	LOG_DBLUE
};

struct LOG_DISPLAY_INFO
{
	char text[MAX_LOG_TEXT_SIZE];
	eLogColor color;
};

class CServerDisplayer
{
public:
	CServerDisplayer();
	virtual ~CServerDisplayer();
	void Init(HWND hWnd);
	void Run();
	void SetWindowName();
	void LogTextPaint();
	void LogAddText(eLogColor color,char* text,int size);
private:
	HWND m_hEditLog;
	HWND m_hwnd;
	HFONT m_font;
	HFONT SPK_gFont;
	HBRUSH m_brush[5];
	LOG_DISPLAY_INFO m_log[MAX_LOG_TEXT_LINE];
	int m_count;
	char m_DisplayerText[2][64];
};

extern CServerDisplayer gServerDisplayer;
