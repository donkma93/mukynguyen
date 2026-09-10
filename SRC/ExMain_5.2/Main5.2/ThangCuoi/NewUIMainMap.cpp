#include "Stdafx.h"
#include "NewUIMainMap.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
#include "MapManager.h"
#include "ZzzAI.h"
#include "Reconnect.h"
#include "CProtect.h"
#include "NewUISystem.h"
#include "ZzzOpenglUtil.h"

using namespace SEASON3B;
SEASON3B::CNewUIMainMap::CNewUIMainMap()
{
	m_pNewUIMng = nullptr;
	m_iMiniMapScale = 1;

	m_dwBuffState = 0;
	m_fMiniMapTexU = 0.0f;
	m_fMiniMapTexV = 0.0f;

	memset(&m_MiniMapPos, 0, sizeof(POINT));
	memset(&m_MiniMapFramePos, 0, sizeof(POINT));
	memset(&m_HeroPosInWorld, 0, sizeof(POINT));
	memset(&m_HeroPosInMiniMap, 0, sizeof(POINT));
	memset(&m_MiniMapScaleOffset, 0, sizeof(POINT));
}

SEASON3B::CNewUIMainMap::~CNewUIMainMap()
{
	Release();
}

bool SEASON3B::CNewUIMainMap::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MAINMAP, this);

	LoadImages();

	SetPos(x, y);

	Show(false);

	return true;
}

void SEASON3B::CNewUIMainMap::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}
void SEASON3B::CNewUIMainMap::SetPos(int x, int y)
{
	m_MiniMapFramePos.x = IsToolKit.GetPositionScreen() - 160;
	m_MiniMapFramePos.y = y + 200;

	m_MiniMapPos.x = m_MiniMapFramePos.x + 25;
	m_MiniMapPos.y = m_MiniMapFramePos.y + 28;
}

bool SEASON3B::CNewUIMainMap::Update()
{
	UpdateHeroPos();
	return true;
}
bool SEASON3B::CNewUIMainMap::UpdateMouseEvent()
{
	return true;
}

bool SEASON3B::CNewUIMainMap::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_MAINMAP);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIMainMap::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIMainMap::GetKeyEventOrder()
{
	return 3.4;
}

void SEASON3B::CNewUIMainMap::UpdateHeroPos()
{
	m_HeroPosInWorld.x = (Hero->PositionX) / m_iMiniMapScale;
	m_HeroPosInWorld.y = (256 - (Hero->PositionY)) / m_iMiniMapScale;

	m_MiniMapScaleOffset.x = max((m_HeroPosInWorld.x - (64 * m_iMiniMapScale)), 0);
	m_MiniMapScaleOffset.y = min(max((m_HeroPosInWorld.y - (64 * m_iMiniMapScale)), 0), 128);

	m_HeroPosInMiniMap.x = m_HeroPosInWorld.x - m_MiniMapScaleOffset.x + m_MiniMapPos.x;
	m_HeroPosInMiniMap.y = m_HeroPosInWorld.y - m_MiniMapScaleOffset.y + m_MiniMapPos.y;

	m_fMiniMapTexU = (float)(m_MiniMapScaleOffset.x) / (256.f / (float)m_iMiniMapScale);
	m_fMiniMapTexV = (float)(m_MiniMapScaleOffset.y) / (256.f / (float)m_iMiniMapScale);
}

void SEASON3B::CNewUIMainMap::RenderMapImageIndex(int Index)
{
	RenderBitmap(Index, (float)(m_MiniMapPos.x), (float)(m_MiniMapPos.y), 128.f, 128.f, m_fMiniMapTexU, m_fMiniMapTexV, 0.5f * m_iMiniMapScale, 0.5f * m_iMiniMapScale);
}

void CNewUIMainMap::EnterInsideMap()
{
	EnableAlphaTest();

	if (!UpdateRadarStates())
		return;
	
	float fPosX, fPosY;
	int IndexID = -1;

	float AddonPosX = 0.0f;
	float AddonPosY = 0.0f;

	glColor4f(1.f, 1.f, 1.f, 1.f);

	switch (gMapManager.WorldActive)
	{
		case 0:  RenderMapImageIndex(WOLRD_00);	AddonPosX = 4.3; AddonPosY = 3.4;	break;
		case 1:  RenderMapImageIndex(WOLRD_01);	AddonPosX = 3.8; AddonPosY = 10.0;	break;
		case 2:  RenderMapImageIndex(WOLRD_02);	AddonPosX = 4.3; AddonPosY = 3.4;	break;
		case 3:  RenderMapImageIndex(WOLRD_03);	AddonPosX = 4.3; AddonPosY = 3.4;	break;
		case 4:  RenderMapImageIndex(WOLRD_04);	AddonPosX = 4.3; AddonPosY = 10.0;	break;
		case 6:  RenderMapImageIndex(WOLRD_06);	AddonPosX = 6.0; AddonPosY = 3.4;	break;
		case 7:  RenderMapImageIndex(WOLRD_07);	AddonPosX = 4.3; AddonPosY = 10.0;	break;
		case 8:  RenderMapImageIndex(WOLRD_08);	AddonPosX = 2.0; AddonPosY = 8.0;	break;
		case 31: RenderMapImageIndex(WOLRD_31);	AddonPosX = 2.0; AddonPosY = 8.0;	break;
		case 33: RenderMapImageIndex(WOLRD_33);	AddonPosX = 2.0; AddonPosY = 8.0;	break;
		case 34: RenderMapImageIndex(WOLRD_34);	AddonPosX = 3.0; AddonPosY = 9.0;	break;
		case 37: RenderMapImageIndex(WOLRD_37);	AddonPosX = 5.0; AddonPosY = 8.0;	break;
		case 38: RenderMapImageIndex(WOLRD_38);	AddonPosX = 5.0; AddonPosY = 8.0;	break;
		case 41: RenderMapImageIndex(WOLRD_41);	AddonPosX = 7.0; AddonPosY = 9.0;	break;
		case 42: RenderMapImageIndex(WOLRD_42);	AddonPosX = 3.0; AddonPosY = 9.0;	break;
		case 51: RenderMapImageIndex(WOLRD_51);	AddonPosX = 3.0; AddonPosY = 6.0;	break;
		case 56: RenderMapImageIndex(WOLRD_56);	AddonPosX = 3.0; AddonPosY = 9.0;	break;
		case 57: RenderMapImageIndex(WOLRD_57);	AddonPosX = 3.0; AddonPosY = 9.0;	break;
		case 63: RenderMapImageIndex(WOLRD_63);	AddonPosX = 0.0; AddonPosY = 0.0;	break;
		case 80: RenderMapImageIndex(WOLRD_80);	AddonPosX = 3.0; AddonPosY = 6.0;	break;
		case 81: RenderMapImageIndex(WOLRD_81);	AddonPosX = 3.0; AddonPosY = 6.0;	break;
		case 82: RenderMapImageIndex(WOLRD_82);	AddonPosX = 3.0; AddonPosY = 6.0;	break;
		case 83: RenderMapImageIndex(WOLRD_83);	AddonPosX = 6.f; AddonPosY = 6.f;	break;
		case 84: RenderMapImageIndex(WOLRD_84);	AddonPosX = 1.f; AddonPosY = 9.f;	break;
		case 85: RenderMapImageIndex(WOLRD_85);	AddonPosX = 6.f; AddonPosY = 6.f;	break;
	}


	EnableAlphaTest();
	RenderImage(DOT_POINT_ME, (float)(m_HeroPosInMiniMap.x) - AddonPosX, (float)(m_HeroPosInMiniMap.y) - AddonPosY, 12, 12, 0.f, 0.f, 17.5f / 32.f, 17.5f / 32.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);

	for (int i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		CHARACTER* c = &CharactersClient[i];
		if (c != NULL && c->Object.Live && c != Hero && (c->Object.Kind == KIND_PLAYER || c->Object.Kind == KIND_MONSTER || c->Object.Kind == KIND_NPC))
		{
			OBJECT* o = &c->Object;

			if (g_isCharacterBuff(o, static_cast<eBuffState>(m_dwBuffState)))
			{
				glColor4f(0.f, 1.f, 0.f, 1.f);
			}
			else
			{
				glColor4f(0.8f, 0.f, 0.f, 1.f);
			}
			if (o->Kind == KIND_NPC)
			{
				IndexID = DOT_POINT_NPC;
			}
			else if (o->Kind == KIND_MONSTER)
			{
				IndexID = DOT_POINT_MONSTER;
			}
			else if (o->Kind == KIND_PLAYER)
			{
				IndexID = DOT_POINT_PLAYER;
			}

			fPosX = ((c->PositionX)) / m_iMiniMapScale - m_MiniMapScaleOffset.x + m_MiniMapPos.x;
			fPosY = (256 - (c->PositionY)) / m_iMiniMapScale - m_MiniMapScaleOffset.y + m_MiniMapPos.y;

			RenderImage(IndexID, fPosX - AddonPosX, fPosY - AddonPosY, 10, 10, 0.f, 0.f, 17.5f / 32.f, 17.5f / 32.f);

			if (CheckMouseIn((int)(fPosX - AddonPosX), (int)(fPosY - AddonPosY), 10, 10))
			{
				IsToolKit.ThisFont((int)(fPosX - AddonPosX), (int)(fPosY - AddonPosY), 0xFFFFFFFF, 0x0000FF96, 0, 0, 1, "%s", c->ID);
			}
			
			if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) && CheckMouseIn(m_MiniMapPos.x, m_MiniMapPos.y, 128, 128))
			{
				int LocalX = MouseX - m_MiniMapPos.x;
				int LocalY = MouseY - m_MiniMapPos.y;

				int WorldX = (LocalX + m_MiniMapScaleOffset.x) * m_iMiniMapScale;
				int WorldY = (256 - (LocalY + m_MiniMapScaleOffset.y)) * m_iMiniMapScale;

				IsToolKit.ThisFont(m_MiniMapPos.x, m_MiniMapPos.y - 14, HEX_COLOR_WHITE, 0, 0, 0, 1,
					"Pos: (%d, %d)", WorldX, WorldY);

				if (IsRelease(VK_RBUTTON))
				{
					POINT pos = { WorldX, WorldY };
					MUHelper::g_MuHelper.SimulateMoveToPos(pos);
				}
			}
		}
	}

	IsToolKit.ThisFont(m_MiniMapPos.x, m_MiniMapPos.y - 13, HEX_COLOR_WHITE, 0, 128, 0, 3, "%d, %d", Hero->PositionX, Hero->PositionY);

	DisableAlphaBlend();
}

bool SEASON3B::CNewUIMainMap::Render()
{
	return false;
}

void SEASON3B::CNewUIMainMap::LoadImages()
{
	LoadBitmap("ACuoi\\FaceMap\\newui_char_point.tga", DOT_POINT_ME, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\newui_char_point_npc.tga", DOT_POINT_NPC, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\newui_char_point_monster.tga", DOT_POINT_MONSTER, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\newui_char_point_player.tga", DOT_POINT_PLAYER, GL_LINEAR);

	LoadBitmap("ACuoi\\FaceMap\\World_00.tga", WOLRD_00, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_01.tga", WOLRD_01, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_02.tga", WOLRD_02, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_03.tga", WOLRD_03, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_04.tga", WOLRD_04, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_06.tga", WOLRD_06, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_07.tga", WOLRD_07, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_08.tga", WOLRD_08, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_31.tga", WOLRD_31, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_33.tga", WOLRD_33, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_34.tga", WOLRD_34, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_37.tga", WOLRD_37, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_38.tga", WOLRD_38, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_41.tga", WOLRD_41, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_42.tga", WOLRD_42, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_51.tga", WOLRD_51, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_56.tga", WOLRD_56, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_57.tga", WOLRD_57, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_63.tga", WOLRD_63, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_80.tga", WOLRD_80, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_81.tga", WOLRD_81, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_82.tga", WOLRD_82, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_83.tga", WOLRD_83, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_84.tga", WOLRD_84, GL_LINEAR);
	LoadBitmap("ACuoi\\FaceMap\\World_85.tga", WOLRD_85, GL_LINEAR);
}
void SEASON3B::CNewUIMainMap::UnloadImages()
{
	DeleteBitmap(DOT_POINT_ME);
	DeleteBitmap(DOT_POINT_NPC);
	DeleteBitmap(DOT_POINT_MONSTER);
	DeleteBitmap(DOT_POINT_PLAYER);

	DeleteBitmap(WOLRD_00);
	DeleteBitmap(WOLRD_01);
	DeleteBitmap(WOLRD_02);
	DeleteBitmap(WOLRD_03);
	DeleteBitmap(WOLRD_04);
	DeleteBitmap(WOLRD_06);
	DeleteBitmap(WOLRD_07);
	DeleteBitmap(WOLRD_08);
	DeleteBitmap(WOLRD_31);
	DeleteBitmap(WOLRD_33);
	DeleteBitmap(WOLRD_34);
	DeleteBitmap(WOLRD_37);
	DeleteBitmap(WOLRD_38);
	DeleteBitmap(WOLRD_41);
	DeleteBitmap(WOLRD_42);
	DeleteBitmap(WOLRD_51);
	DeleteBitmap(WOLRD_56);
	DeleteBitmap(WOLRD_57);
	DeleteBitmap(WOLRD_63);
	DeleteBitmap(WOLRD_80);
	DeleteBitmap(WOLRD_81);
	DeleteBitmap(WOLRD_82);
	DeleteBitmap(WOLRD_83);
	DeleteBitmap(WOLRD_84);
	DeleteBitmap(WOLRD_85);
}

bool SEASON3B::CNewUIMainMap::UpdateRadarStates()
{
	if (gMapManager.WorldActive == 52
		|| gMapManager.WorldActive == 9
		|| gMapManager.WorldActive == 53
		|| gMapManager.WorldActive == 64
		|| gMapManager.WorldActive == 66
		|| gMapManager.WorldActive >= 11 && gMapManager.WorldActive <= 32
		|| SceneFlag != 5
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_JEWELBANK)
		|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MUHELPER)
		)
	{
		return false;
	}
	return true;
}