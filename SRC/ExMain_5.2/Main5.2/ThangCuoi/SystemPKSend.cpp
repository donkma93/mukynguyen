#include "Stdafx.h"
#include "SystemPKSend.h"

#include "NewUISystem.h"
#include "NewUIMuHelper.h"
#include "CharacterManager.h"
#include "DSPlaySound.h"
#include "ThangCuoi\StructSendGs.h"
#include "NewUIRenderNumber.h"
#include "wsclientinline.h"
#include "ThangCuoi\ZzzToolKit.h"
#include "MapManager.h"

SEASON3B::CNewUIPKSystem::CNewUIPKSystem()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;

	gPKSys.guiX = 53;
	gPKSys.guiY = 150;
	gPKSys.curX = MouseX;
	gPKSys.curY = MouseY;
	gPKSys.dragging = false;
}

SEASON3B::CNewUIPKSystem::~CNewUIPKSystem()
{
	Release();
}

bool SEASON3B::CNewUIPKSystem::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_PKSYSTEM, this);

	LoadImages();

	SetPos(x, y);

	Show(false);

	return true;
}

void SEASON3B::CNewUIPKSystem::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIPKSystem::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void SEASON3B::CNewUIPKSystem::LoadImages()
{
}
void SEASON3B::CNewUIPKSystem::UnloadImages()
{
}

bool SEASON3B::CNewUIPKSystem::Update()
{
	if (IsVisible())
	{
		int iNumCurOpenTab = m_TabBtn.UpdateMouseEvent();

		if (iNumCurOpenTab == RADIOGROUPEVENT_NONE)
			return true;
	}
	return true;
}

bool SEASON3B::CNewUIPKSystem::UpdateMouseEvent()
{
	POINT ptExitBtn1 = { m_Pos.x + 249, m_Pos.y + 7 };

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_PKSYSTEM);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 80, WINDOW_HEIGHT + 39))
		return false;
	return true;
}

bool SEASON3B::CNewUIPKSystem::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_PKSYSTEM);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIPKSystem::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIPKSystem::GetKeyEventOrder()
{
	return 3.4;
}

void PKSystemSet::RenderPKForm()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	int maxLines = 6;
	int totalLines = gPKSys.m_DataNoticePK.size();
	if (totalLines > maxLines) {
		totalLines = maxLines;
	}

	float ScaleHeight = 10 + (totalLines * 12);

	if (GetKeyState(VK_RBUTTON) & 0x8000 && GetTickCount() > this->ActionTime + 1000)
	{
		if (IsToolKit.IsWorkZone(this->guiX - 50, this->guiY, 110, ScaleHeight))
		{
			if (!this->dragging)
			{
				this->dragging = true;
				this->curX = MouseX;
				this->curY = MouseY;
			}
		}
		if (this->dragging)
		{
			this->guiX += MouseX - this->curX;
			this->guiY += MouseY - this->curY;
			this->curX = MouseX;
			this->curY = MouseY;
		}
	}
	else
	{
		if (this->dragging)
		{
			this->dragging = false;
		}
	}

	if (this->guiX < 0)
		this->guiX = 0;
	if (this->guiY < 20)
		this->guiY = 20;

	if (this->guiX > 640 - 110)
		this->guiX = 640 - 110;
	if (this->guiY > 480 - 10 - 51)
		this->guiY = 480 - ScaleHeight - 51;

	float X = this->guiX;
	float Y = this->guiY;

	for (int i = 0; i < totalLines; i++)
	{
		auto& data = gPKSys.m_DataNoticePK[i];
		std::string fullMessage = data.Mess;

		size_t firstSpace = fullMessage.find(' ');
		size_t secondSpace = fullMessage.find(' ', firstSpace + 1);

		std::string firstName, secondName;
		int mapID = -1;

		if (firstSpace != std::string::npos && secondSpace != std::string::npos)
		{
			firstName = fullMessage.substr(0, firstSpace);
			secondName = fullMessage.substr(firstSpace + 1, secondSpace - firstSpace - 1);
			std::string mapIDStr = fullMessage.substr(secondSpace + 1);

			if (firstName.length() > 10) firstName = firstName.substr(0, 10);
			if (secondName.length() > 10) secondName = secondName.substr(0, 10);

			mapID = atoi(mapIDStr.c_str());
		}
		else
		{
			firstName = "???";
			secondName = "???";
			mapID = -1;
		}

		if (!data.IsFading && data.Time <= GetTickCount())
		{
			data.IsFading = true;
		}

		if (data.IsFading)
		{
			data.Alpha -= 10;
			if (data.Alpha < 0) data.Alpha = 0;
		}

		int drawAlpha = data.Alpha << 24;

		IsToolKit.ThisFont(X - 49, Y + 4 + (i * 13), drawAlpha | 0xFF9500FF, 200, 0, 0, 1, GlobalText[3749], firstName.c_str(), secondName.c_str(), gMapManager.GetMapName(mapID));
		IsToolKit.ThisFont(X - 49, Y + 4 + (i * 13), drawAlpha | 0x00FFFFFF, 0, 0, 0, 1, GlobalText[3748], firstName.c_str(), secondName.c_str());
		IsToolKit.ThisFont(X - 49, Y + 4 + (i * 13), drawAlpha | 0x00FF00FF, 0, 0, 0, 1, GlobalText[3747], firstName.c_str(), secondName.c_str());
		IsToolKit.ThisFont(X - 49, Y + 4 + (i * 13), drawAlpha | 0x00FFFFFF, 0, 0, 0, 1, GlobalText[3371], firstName.c_str());
		IsToolKit.ThisFont(X - 49, Y + 4 + (i * 13), drawAlpha | 0x00FF00FF, 0, 0, 0, 1, GlobalText[3372], firstName.c_str());

		data.Delay += 5;

		if (data.IsFading && data.Alpha == 0)
		{
			gPKSys.m_DataNoticePK.erase(gPKSys.m_DataNoticePK.begin() + i);
			totalLines--;
			i--;
		}
	}

	DisableAlphaBlend();
}

bool SEASON3B::CNewUIPKSystem::Render()
{
	return false;
}
PKSystemSet gPKSys;