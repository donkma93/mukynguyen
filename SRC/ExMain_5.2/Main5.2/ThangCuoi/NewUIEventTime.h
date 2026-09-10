#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
#include "ACuoi_JewelBank.h"
namespace SEASON3B
{
	class CNewUIEventTime : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{

			IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)
			IMAGE_BASE_WINDOW_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,			//. newui_exit_00.tga
		};

		private:

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;

		public:
		CNewUIEventTime();
		virtual ~CNewUIEventTime();

		int WINDOW_WIDTH;
		int	WINDOW_HEIGHT;

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();
		void InitButtons();
		void SetPos(int x, int y);
	};
}


#define MAX_EVENTTIME 42

struct PMSG_CUSTOM_EVENTTIME_RECV
{
	PSWMSG_HEAD h;
	int count;
	BYTE RegLineEvent;
};

struct CUSTOM_EVENTTIME_DATA
{
	int index;
	int time;
};

struct CUSTOM_EVENT_INFO
{
	int Index;
	char Name[40];
	char Map[40];
	char Gate[40];
};

// A read-only view used by the standalone character dashboard.  Keeping this
// separate from the protocol structs avoids exposing the event UI internals.
struct CUSTOM_EVENTTIME_DISPLAY
{
    char Name[40];
    char Map[40];
    int SecondsUntilStart;
};

class CCustomEventTime
{
public:
	void Load(CUSTOM_EVENT_INFO* info);
	void SetInfo(CUSTOM_EVENT_INFO info);
	void ClearCustomEventTime();
	void GCReqEventTime(const BYTE* lpMsg);
	void DrawEventTimePanelWindow(int x, int y);
	int CopyEventTimeDisplay(CUSTOM_EVENTTIME_DISPLAY* output, int capacity) const;
	DWORD ClickTime;
	void Init();
	POINT CustomPos;
	int count;
	BYTE RegLineEvent;
	private:
	CUSTOM_EVENT_INFO m_CustomEventInfo[MAX_EVENTTIME];
	CUSTOM_EVENTTIME_DATA gCustomEventTime[MAX_EVENTTIME];
	int EventTimeLoad;
	int EventTimeEnable;
	int EventTimeTickCount;
	int Page;
	int Arena;
	CUSTOM_EVENT_INFO Alldata;
	std::vector<CUSTOM_EVENT_INFO> m_EventInfoList;
};
extern CCustomEventTime gCETime;
