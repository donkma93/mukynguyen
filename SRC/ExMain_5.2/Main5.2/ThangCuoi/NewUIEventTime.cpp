#include "Stdafx.h"
#include "NewUIEventTime.h"

#include "NewUISystem.h"
#include "NewUIMuHelper.h"
#include "CharacterManager.h"
#include "DSPlaySound.h"
#include "MemScript.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
CCustomEventTime gCETime;

SEASON3B::CNewUIEventTime::CNewUIEventTime()
{
	gCETime.Init();
	gCETime.ClearCustomEventTime();
	m_pNewUIMng = nullptr;
	m_Pos.x = m_Pos.y = 0;

	WINDOW_WIDTH = 340;
	WINDOW_HEIGHT = 0;
}

SEASON3B::CNewUIEventTime::~CNewUIEventTime()
{
	Release();
}

void CCustomEventTime::Init() // OK
{
	for (int n = 0; n < MAX_EVENTTIME; n++)
	{
		this->m_CustomEventInfo[n].Index = -1;
	}
}
void CCustomEventTime::Load(CUSTOM_EVENT_INFO* info) // OK
{
	for (int n = 0; n < MAX_EVENTTIME; n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomEventTime::SetInfo(CUSTOM_EVENT_INFO info) // OK
{
	if (info.Index < 0 || info.Index >= MAX_EVENTTIME)
	{
		return;
	}

	this->m_CustomEventInfo[info.Index] = info;
}
void CCustomEventTime::ClearCustomEventTime() // OK
{
	for (int n = 0; n < MAX_EVENTTIME; n++)
	{
		gCustomEventTime[n].index = -1;
		gCustomEventTime[n].time = -1;
		gCustomEventTime[n].Name[0] = 0;
		gCustomEventTime[n].Map[0] = 0;
	}
	this->count = 0;
	this->EventTimeEnable = 0;
}

int CCustomEventTime::CopyEventTimeDisplay(CUSTOM_EVENTTIME_DISPLAY* output, int capacity) const
{
	if (output == NULL || capacity <= 0)
	{
		return 0;
	}

	const int lineCount = (this->RegLineEvent < MAX_EVENTTIME) ? this->RegLineEvent : MAX_EVENTTIME;
	int written = 0;
	for (int i = 0; i < lineCount && written < capacity; ++i)
	{
		if (this->gCustomEventTime[i].time < 0)
		{
			continue;
		}

		CUSTOM_EVENTTIME_DISPLAY& event = output[written++];
		ZeroMemory(&event, sizeof(event));
		const char* name = this->gCustomEventTime[i].Name[0] ? this->gCustomEventTime[i].Name : this->m_CustomEventInfo[i].Name;
		const char* map = this->gCustomEventTime[i].Map[0] ? this->gCustomEventTime[i].Map : this->m_CustomEventInfo[i].Map;
		strncpy(event.Name, name, sizeof(event.Name) - 1);
		strncpy(event.Map, map, sizeof(event.Map) - 1);
		event.SecondsUntilStart = this->gCustomEventTime[i].time;
	}
	return written;
}
void CCustomEventTime::DrawEventTimePanelWindow(int x, int y)
{
	IsToolKit.ThisFont(x - 25, y + 40, 0xFFFF00FF, 0, 150, 0, RT3_SORT_CENTER, GlobalText[3851]);
	IsToolKit.ThisFont(x + 100, y + 40, 0xFFFF00FF, 0, 150, 0, RT3_SORT_CENTER, GlobalText[3852]);
	IsToolKit.ThisFont(x + 200, y + 40, 0xFFFF00FF, 0, 150, 0, RT3_SORT_CENTER, GlobalText[3854]);

	char text2[30];
	int totalseconds;
	int hours;
	int minutes;
	int seconds;
	int days;
	DWORD Color;
	int line = 10;

	int GetW = g_pNewUIEventTime->WINDOW_WIDTH - 51;
	int iLineHeight = ((FontHeight / gPosWide.x_fScreenRate_y));
	for (int i = 0; i < gCETime.RegLineEvent; i++)
	{
		if (this->gCustomEventTime[i].time <= -1)
		{
			continue;
		}

		if (this->gCustomEventTime[i].time == 0)
		{
			wsprintf(text2, GlobalText[3856]);
		}
		else
		{
			totalseconds = this->gCustomEventTime[i].time;
			hours = totalseconds / 3600;
			minutes = (totalseconds / 60) % 60;
			seconds = totalseconds % 60;

			if (hours > 23)
			{
				days = hours / 24;
				wsprintf(text2, GlobalText[3857], days);
			}
			else
			{
				wsprintf(text2, GlobalText[3858], hours, minutes, seconds);
			}
		}
		if (this->gCustomEventTime[i].time <= -1)
		{
			Color = 0xFF0000FF;
		}
		else if (this->gCustomEventTime[i].time == 0)
		{
			Color = 0x35D964FF;
		}
		else if (this->gCustomEventTime[i].time < 300)
		{
			Color = 0xFF0000FF;
		}
		else
		{
			Color = HEX_COLOR_WHITE;
		}
			
		if (CheckMouseIn(x + 15, y + 63 + (i * 12), 429, 9))
		{
			g_pUIForm->RenderHover(x + 15, y + 63 + (i * 12), GetW, iLineHeight, 0x0080C080);
		}

		const char* name = this->gCustomEventTime[i].Name[0] ? this->gCustomEventTime[i].Name : this->m_CustomEventInfo[i].Name;
		const char* map = this->gCustomEventTime[i].Map[0] ? this->gCustomEventTime[i].Map : this->m_CustomEventInfo[i].Map;
		IsToolKit.ThisFont(x + 25, y + 63 + (i * 12), Color, 0, 150, 0, RT3_SORT_LEFT, name);
		IsToolKit.ThisFont(x + 100, y + 63 + (i * 12), Color, 0, 150, 0, RT3_SORT_CENTER, map);
		IsToolKit.ThisFont(x + 200, y + 63 + (i * 12), Color, 0, 150, 0, RT3_SORT_CENTER, text2);

		for (int m = 0; m < gCETime.RegLineEvent; m++)
		{
			if (i == m)
			{
				if (this->gCustomEventTime[i].time < 300)
				{
					if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(x + 20, y + 63 + (i * 12), GetW, 9))
					{
						SendRequestDataSendType(0xF3, 0x27, i + 1);
						g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENTITME);
					}
				}
			}
		}
	}
}

void CCustomEventTime::GCReqEventTime(const BYTE* lpMsg)
{
	auto DataSPK = reinterpret_cast<const PMSG_CUSTOM_EVENTTIME_RECV*>(lpMsg);

	this->count			= DataSPK->count;
	this->RegLineEvent	= DataSPK->RegLineEvent;
	if (this->count < 0 || this->count > MAX_EVENTTIME)
	{
		this->count = 0;
		return;
	}
	if (this->RegLineEvent > MAX_EVENTTIME)
	{
		this->RegLineEvent = MAX_EVENTTIME;
	}

	for (auto n = 0; n < this->count; ++n)
	{
		auto lpInfo = reinterpret_cast<const CUSTOM_EVENTTIME_DATA*>(lpMsg + sizeof(PMSG_CUSTOM_EVENTTIME_RECV) + sizeof(CUSTOM_EVENTTIME_DATA) * n);

		this->gCustomEventTime[n].index = lpInfo->index;
		this->gCustomEventTime[n].time = lpInfo->time;
		strncpy(this->gCustomEventTime[n].Name, lpInfo->Name, sizeof(this->gCustomEventTime[n].Name) - 1);
		this->gCustomEventTime[n].Name[sizeof(this->gCustomEventTime[n].Name) - 1] = 0;
		strncpy(this->gCustomEventTime[n].Map, lpInfo->Map, sizeof(this->gCustomEventTime[n].Map) - 1);
		this->gCustomEventTime[n].Map[sizeof(this->gCustomEventTime[n].Map) - 1] = 0;

		if (lpInfo->index >= 28 && lpInfo->time != -1)
		{
			this->Arena = 1;
		}
	}

	this->EventTimeEnable = 1;
}

bool SEASON3B::CNewUIEventTime::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENTITME, this);

	SetPos(x, y);

	Show(false);

	return true;
}

void SEASON3B::CNewUIEventTime::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIEventTime::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos;
	m_Pos.y = y;
}


bool SEASON3B::CNewUIEventTime::Update()
{
	return true;
}

bool SEASON3B::CNewUIEventTime::UpdateMouseEvent()
{
	float GetPosX = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	float GetPosY = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((GetPosX + WINDOW_WIDTH) - 40, GetPosY + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENTITME);
	}

	if (CheckMouseIn(GetPosX, GetPosY, WINDOW_WIDTH - 10, WINDOW_HEIGHT + 70))
		return false;
	return true;
}

bool SEASON3B::CNewUIEventTime::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_EVENTITME);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIEventTime::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIEventTime::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIEventTime::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	SendRequestDataSend(0xF3, 0xE8);

	WINDOW_HEIGHT = (gCETime.RegLineEvent * 12) + 12;

	float GetPosX = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	float GetPosY = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;

	g_pUIForm->NewRenderForm(GetPosX, GetPosY, WINDOW_WIDTH, WINDOW_HEIGHT, GlobalText[3782]);

	g_pUIForm->RenderBack(GetPosX + 13, GetPosY + 36, int(WINDOW_WIDTH - 44), int(22));
	g_pUIForm->RenderBack(GetPosX + 13, GetPosY + 58, int(WINDOW_WIDTH - 44), int(WINDOW_HEIGHT));
	
	gCETime.DrawEventTimePanelWindow(GetPosX, GetPosY);

	DisableAlphaBlend();
	return true;
}

