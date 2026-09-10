// ServerDisplayer.h: interface for the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//SuperKhung Render Màu
#define Spk_vRed		RGB(139, 000, 000) // Đỏ Huyết
#define Spk_vWhite		RGB(255, 255, 255) // Trắng
#define Spk_vBlue		RGB(000, 000, 255) // Xanh Dương
#define Spk_vGreen		RGB(000, 255, 000) // Xanh Lá Sáng
#define Spk_vDarkGreen	RGB(000, 100, 000) // Xanh Lá Tối
#define Spk_vSilver		RGB(120, 120, 120) // Màu Bạc
#define Spk_vBlack		RGB(000, 000, 000) // Màu Đen
#define Spk_vPink		RGB(224, 224, 224) // Màu Hồng
//Font
#define SPK_BOLD 700
#define SPK_NORL 100
//Rút Gọn Code:
#define MAX_LOG_TEXT_LINE 41
#define MAX_LOG_TEXT_SIZE 100

#define MAX_LOGCONNECT_TEXT_LINE 38
#define MAX_LOGCONNECT_TEXT_SIZE 55

#define MAX_LOGGLOBAL_TEXT_LINE 8
#define MAX_LOGGLOBAL_TEXT_SIZE 100

enum eLogColor
{
	LOG_BLACK = 0,
	LOG_RED = 1,
	LOG_GREEN = 2,
	LOG_BLUE = 3,
	//MC bot
	LOG_BOT = 4,
	LOG_USER = 5,
	LOG_EVENT = 6,
	LOG_ALERT = 7,
	LOG_DARKGREEN = 8,
	LOG_ORGAN = 9,
};

struct LOG_DISPLAY_INFO
{
	char text[MAX_LOG_TEXT_SIZE];
	eLogColor color;
};

struct LOGCONNECT_DISPLAY_INFO
{
	char text[MAX_LOGCONNECT_TEXT_SIZE];
	eLogColor color;
};

struct LOGGLOBAL_DISPLAY_INFO
{
	char text[MAX_LOGGLOBAL_TEXT_SIZE];
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
	void LogTextPaintConnect();
	void PaintEventTime();
	void PaintInvasionTime();
	void PaintCustomArenaTime();

	void LogAddText(eLogColor color, char* text, int size);
	void LogAddTextConnect(eLogColor color, char* text, int size);
	void LogAddTextGlobal(eLogColor color, char* text, int size);
	int EventBc;
	int EventDs;
	int EventCc;
	int EventIt;
	int EventCustomBonus;
	int EventMoss;
	int EventKing;
	int EventDrop;
	int EventTvT;
	int EventGvG;
	int EventInvasion[30];
	int EventCustomArena[30];
	int EventCs;
	int EventCsState;
	int EventCastleDeep;
	int EventCryWolf;
	int EventCryWolfState;
	#if(EVENT_PK)
	int EventBsV;
	#endif
	#if(THANMA)
	int BEventThanMa;
	#endif
	int EventCTCMini;

	void CServerDisplayer::ShowEventDisplay(RECT rect, HDC hdc, LPCWSTR StringName, int EventName, int X1, int X2, int Y);

	HWND m_hwnd;
	HFONT m_font;
	HBRUSH m_brush[5];
	LOG_DISPLAY_INFO m_log[MAX_LOG_TEXT_LINE];
	LOGCONNECT_DISPLAY_INFO m_logConnect[MAX_LOGCONNECT_TEXT_LINE];
	LOGGLOBAL_DISPLAY_INFO m_logGlobal[MAX_LOGGLOBAL_TEXT_LINE];
	int m_count;
	int m_countConnect;
	int m_countGlobal;
	int m_FontHeight;
};

extern CServerDisplayer gServerDisplayer;
extern unsigned char RamCharError[];
