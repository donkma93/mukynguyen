//////////////////////////////////////////////////////////////////////
// NewUIMoveCommandWindow.cpp: implementation of the CNewUIMoveCommandWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIMoveCommandWindow.h"
#include "NewUISystem.h"
#include "ZzzInterface.h"
#include "wsclientinline.h"
#include "ChangeRingManager.h"
#include "KeyGenerater.h"
#include "Local.h"
#include "ServerListManager.h"
#include "ZzzOpenData.h"
#include "MapManager.h"
#include "CharacterManager.h"
#include "ThangCuoi/CustomWing.h"
#include "ThangCuoi/CustomPet.h"
#include "ThangCuoi/ZzzToolKit.h"
#define MAX_WIDTH_MOVE 5
#define MAX_WIDTH_SCROLL 15
using namespace SEASON3B;

namespace
{
	const int MapNameCount = 6;

	const std::string MapName[MapNameCount] =
	{
		"Lorencia",
		"Noria",
		"Elbeland",
		"Dungeon",
		"Devias",
		"LostTower",
	};

	const bool IsLuckySeal(const std::string& name)
	{
		if (name.size() != 0) {
			for (int i = 0; i < MapNameCount; ++i) {
				if (name == MapName[i])
				{
					return true;
				}
			}
		}
		return false;
	}
};

CNewUIMoveCommandWindow::CNewUIMoveCommandWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;

	memset(&m_StartUISubjectName, 0, sizeof(POINT));
	memset(&m_StartMapNamePos, 0, sizeof(POINT));
	memset(&m_MapNameUISize, 0, sizeof(POINT));
	memset(&m_StrifePos, 0, sizeof(POINT));
	memset(&m_MapNamePos, 0, sizeof(POINT));
	memset(&m_ReqLevelPos, 0, sizeof(POINT));
	memset(&m_ReqZenPos, 0, sizeof(POINT));
	m_iSelectedMapName = -1;

	memset(&m_ScrollBarPos, 0, sizeof(POINT));
	memset(&m_ScrollBtnStartPos, 0, sizeof(POINT));
	memset(&m_ScrollBtnPos, 0, sizeof(POINT));
	m_iScrollBarHeightPixel = 0;
	m_iRenderStartTextIndex = 0;
	m_iSelectedTextIndex = -1;
	m_iScrollBtnInterval = 0;
	m_iScrollBarMiddleNum = 0;
	m_iScrollBarMiddleRemainderPixel = 0;
	m_iNumPage = 0;
	m_iCurPage = 0;
	m_iMousePosY = 0;
	m_bScrollBtnActive = false;
	m_iScrollBtnMouseEvent = MOVECOMMAND_MOUSEBTN_NORMAL;
}

CNewUIMoveCommandWindow::~CNewUIMoveCommandWindow()
{
	Release();
}

bool SEASON3B::CNewUIMoveCommandWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MOVEMAP, this);

	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void SEASON3B::CNewUIMoveCommandWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIMoveCommandWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_StrifePos.x = m_Pos.x + 20;
	switch (WindowWidth)
	{
		case 640:		m_MapNameUISize.x = 220; m_MapNamePos.x = m_Pos.x + 62; m_ReqLevelPos.x = m_Pos.x + 119; m_ReqZenPos.x = m_Pos.x + 159;		break;
		case 800:		m_MapNameUISize.x = 200; m_MapNamePos.x = m_Pos.x + 69; m_ReqLevelPos.x = m_Pos.x + 129; m_ReqZenPos.x = m_Pos.x + 174;		break;
		case 1024:		m_MapNameUISize.x = 180; m_MapNamePos.x = m_Pos.x + 64; m_ReqLevelPos.x = m_Pos.x + 119; m_ReqZenPos.x = m_Pos.x + 159;		break;
		case 1280:		m_MapNameUISize.x = 160; m_MapNamePos.x = m_Pos.x + 59; m_ReqLevelPos.x = m_Pos.x + 104; m_ReqZenPos.x = m_Pos.x + 139;		break;
		case 1366:		m_MapNameUISize.x = 150; m_MapNamePos.x = m_Pos.x + 56; m_ReqLevelPos.x = m_Pos.x + 101; m_ReqZenPos.x = m_Pos.x + 134;		break;
		case 1440:		m_MapNameUISize.x = 140; m_MapNamePos.x = m_Pos.x + 53; m_ReqLevelPos.x = m_Pos.x + 97; m_ReqZenPos.x = m_Pos.x + 129;		break;
		case 1600:		m_MapNameUISize.x = 120; m_MapNamePos.x = m_Pos.x + 46; m_ReqLevelPos.x = m_Pos.x + 86; m_ReqZenPos.x = m_Pos.x + 114;		break;
		case 1800:		m_MapNameUISize.x = 115; m_MapNamePos.x = m_Pos.x + 44; m_ReqLevelPos.x = m_Pos.x + 83; m_ReqZenPos.x = m_Pos.x + 110;		break;
		case 1920:		m_MapNameUISize.x = 110; m_MapNamePos.x = m_Pos.x + 38; m_ReqLevelPos.x = m_Pos.x + 70; m_ReqZenPos.x = m_Pos.x + 93;		break;
		case 2100:		m_MapNameUISize.x = 110; m_MapNamePos.x = m_Pos.x + 38; m_ReqLevelPos.x = m_Pos.x + 70; m_ReqZenPos.x = m_Pos.x + 93;		break;
		case 2560:		m_MapNameUISize.x = 110; m_MapNamePos.x = m_Pos.x + 38; m_ReqLevelPos.x = m_Pos.x + 70; m_ReqZenPos.x = m_Pos.x + 93;		break;
		default:
		
		break;
	}

	m_MapNameUISize.x += 10;

	m_listMoveInfoData = CMoveCommandData::GetInstance()->GetMoveCommandDatalist();
	m_iRealFontHeight = 8;

	m_MapNameUISize.y = 60 + (m_iRealFontHeight * MOVECOMMAND_MAX_RENDER_TEXTLINE);

	m_StartUISubjectName.x = m_Pos.x + m_MapNameUISize.x / 2;
	m_StartUISubjectName.y = m_Pos.y + 4;

	m_StartMapNamePos.x = m_Pos.x + 2;
	m_StartMapNamePos.y = m_Pos.y + 38;

	m_StrifePos.y = m_StartMapNamePos.y;

	m_MapNamePos.y = m_StartMapNamePos.y;
	m_ReqLevelPos.y = m_StartMapNamePos.y;
	m_ReqZenPos.y = m_StartMapNamePos.y;

	m_ScrollBarPos.x = m_Pos.x + 170;
	m_ScrollBarPos.y = m_Pos.y + m_StartMapNamePos.y - MOVECOMMAND_SCROLLBAR_TOP_HEIGHT - 11;

	m_ScrollBtnStartPos.x = m_ScrollBarPos.x - (MOVECOMMAND_SCROLLBTN_WIDTH / 2 - MOVECOMMAND_SCROLLBAR_TOP_WIDTH / 2);
	m_ScrollBtnStartPos.y = m_ScrollBarPos.y;
	m_ScrollBtnPos.x = m_ScrollBtnStartPos.x;
	m_ScrollBtnPos.y = m_ScrollBtnStartPos.y;

	m_iScrollBarHeightPixel = MOVECOMMAND_MAX_RENDER_TEXTLINE * m_iRealFontHeight;

	m_iScrollBarMiddleNum = (m_iScrollBarHeightPixel - (MOVECOMMAND_SCROLLBAR_TOP_HEIGHT * 2)) / MOVECOMMAND_SCROLLBAR_MIDDLE_HEIGHT;
	m_iScrollBarMiddleRemainderPixel = (m_iScrollBarHeightPixel - (MOVECOMMAND_SCROLLBAR_TOP_HEIGHT * 2)) % MOVECOMMAND_SCROLLBAR_MIDDLE_HEIGHT;

	m_iNumPage = 1 + (m_listMoveInfoData.size() / MOVECOMMAND_MAX_RENDER_TEXTLINE);

	m_iCurPage = 1;

	m_iTotalMoveScrBtnperStep = m_listMoveInfoData.size() - MOVECOMMAND_MAX_RENDER_TEXTLINE;
	m_iRemainMoveScrBtnperStep = m_iTotalMoveScrBtnperStep;
	m_iTotalMoveScrBtnPixel = m_iScrollBarHeightPixel - MOVECOMMAND_SCROLLBTN_HEIGHT;
	m_iRemainMoveScrBtnPixel = m_iTotalMoveScrBtnPixel;
	m_iMinMoveScrBtnPixelperStep = m_iTotalMoveScrBtnPixel / m_iTotalMoveScrBtnperStep;
	m_iMaxMoveScrBtnPixelperStep = m_iMinMoveScrBtnPixelperStep + 1;
	m_iTotalNumMaxMoveScrBtnperStep = m_iTotalMoveScrBtnPixel - (m_iTotalMoveScrBtnperStep * m_iMinMoveScrBtnPixelperStep);
	m_iTotalNumMinMoveScrBtnperStep = m_iTotalMoveScrBtnperStep - m_iTotalNumMaxMoveScrBtnperStep;
	m_icurMoveScrBtnPixelperStep = m_iMaxMoveScrBtnPixelperStep;
	m_iAcumMoveMouseScrollPixel = 0;

	if (m_iNumPage > 1)
	{
		m_bScrollBtnActive = true;
	}

	m_iRenderStartTextIndex = 0;
	m_iRenderEndTextIndex = m_iRenderStartTextIndex + MOVECOMMAND_MAX_RENDER_TEXTLINE;

	if (m_iRenderEndTextIndex > (int)m_listMoveInfoData.size())
	{
		m_iRenderEndTextIndex -= (m_iRenderEndTextIndex - m_listMoveInfoData.size());
	}
}

bool SEASON3B::CNewUIMoveCommandWindow::IsLuckySealBuff()
{
	if (g_isCharacterBuff((&Hero->Object), eBuff_Seal1)
		|| g_isCharacterBuff((&Hero->Object), eBuff_Seal2)
		|| g_isCharacterBuff((&Hero->Object), eBuff_Seal3)
		|| g_isCharacterBuff((&Hero->Object), eBuff_Seal4)
		|| g_isCharacterBuff((&Hero->Object), eBuff_Seal_HpRecovery)
		|| g_isCharacterBuff((&Hero->Object), eBuff_Seal_MpRecovery)
		|| g_isCharacterBuff((&Hero->Object), eBuff_AscensionSealMaster)
		|| g_isCharacterBuff((&Hero->Object), eBuff_WealthSealMaster)
		|| g_isCharacterBuff((&Hero->Object), eBuff_NewWealthSeal)
		|| g_isCharacterBuff((&Hero->Object), eBuff_PartyExpBonus)
		)
	{
		return true;
	}
	return false;
}

bool SEASON3B::CNewUIMoveCommandWindow::IsMapMove(const std::string& src)
{
	if (Hero->Object.Kind == KIND_PLAYER
		&& Hero->Object.Type == MODEL_PLAYER
		&& Hero->Object.SubType == MODEL_GM_CHARACTER)
	{
		return true;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_GMEffect))
	{
		return true;
	}

	if (IsLuckySealBuff() == false) {
		char lpszStr1[1024]; char* lpszStr2 = NULL;
		if (src.find(GlobalText[260]) != std::string::npos) {
			std::string temp = GlobalText[260];
			temp += ' ';
			wsprintf(lpszStr1, src.c_str());
			lpszStr2 = strtok(lpszStr1, temp.c_str());
			if (lpszStr2 == NULL) return false;

			SettingCanMoveMap();
			auto li = m_listMoveInfoData.begin();
			for (int i = 0; i < m_iRenderEndTextIndex; i++, li++) {
				if (!strcmp(lpszStr2, (*li)->_ReqInfo.szMainMapName)) {
					if ((*li)->_bCanMove == true) {
						return IsLuckySeal((*li)->_ReqInfo.szSubMapName);
					}
				}
			}
			return false;
		}
		else if (src.find("/move") != std::string::npos) {
			std::string temp = "/move";
			temp += ' ';
			wsprintf(lpszStr1, src.c_str());
			lpszStr2 = strtok(lpszStr1, temp.c_str());
			if (lpszStr2 == NULL) return false;

			SettingCanMoveMap();
			auto li = m_listMoveInfoData.begin();
			for (int i = 0; i < m_iRenderEndTextIndex; i++, li++) {
				if (!stricmp(lpszStr2, (*li)->_ReqInfo.szMainMapName)) {
					if ((*li)->_bCanMove == true) {
						return IsLuckySeal((*li)->_ReqInfo.szSubMapName);
					}
				}
			}
			return false;
		}
		else {
			return IsLuckySeal(src);
		}
	}
	return true;
}

void SEASON3B::CNewUIMoveCommandWindow::SetMoveCommandKey(DWORD dwKey)
{
	m_dwMoveCommandKey = dwKey;
}

DWORD SEASON3B::CNewUIMoveCommandWindow::GetMoveCommandKey()
{
	m_dwMoveCommandKey = g_KeyGenerater.GenerateKeyValue(m_dwMoveCommandKey);

	return m_dwMoveCommandKey;
}

void SEASON3B::CNewUIMoveCommandWindow::SetStrifeMap()
{
	std::list<CMoveCommandData::MOVEINFODATA*>::iterator li;

	if (!g_ServerListManager->IsNonPvP())
	{
		int anStrifeIndex[1] = { 42 };
		int i;
		for (li = m_listMoveInfoData.begin(); li != m_listMoveInfoData.end(); advance(li, 1))
		{
			(*li)->_bStrife = false;
			for (i = 0; i < 1; ++i)
			{
				if ((*li)->_ReqInfo.index == anStrifeIndex[i])
				{
					(*li)->_bStrife = true;
					break;
				}
			}
		}
	}
	else
	{
		for (li = m_listMoveInfoData.begin(); li != m_listMoveInfoData.end(); advance(li, 1))
			(*li)->_bStrife = false;
	}
}

void SEASON3B::CNewUIMoveCommandWindow::SettingCanMoveMap()
{
	int a = gMapManager.WorldActive;

	DWORD iZen;
	int iLevel, iReqLevel, iReqZen;

	auto li = m_listMoveInfoData.begin();
	for (int i = 0; i < m_iRenderEndTextIndex; i++, li++)
	{
		if (li == m_listMoveInfoData.end())
		{
			break;
		}
		if (i < m_iRenderStartTextIndex)
		{
			continue;
		}

		(*li)->_bCanMove = false;
		(*li)->_bSelected = false;

		iLevel = CharacterAttribute->Level;
		iZen = CharacterMachine->Gold;
		iReqLevel = (*li)->_ReqInfo.iReqLevel;
		iReqZen = (*li)->_ReqInfo.iReqZen;

		if ((gCharacterManager.GetBaseClass(CharacterAttribute->Class) == CLASS_MAGIC || gCharacterManager.GetBaseClass(CharacterAttribute->Class) == CLASS_DARK_LORD
			|| gCharacterManager.GetBaseClass(CharacterAttribute->Class) == CLASS_RAGEFIGHTER)
			&& (iReqLevel != 400))
		{
			iReqLevel = int(float(iReqLevel) * 2.f / 3.f);
		}

		if (iLevel >= iReqLevel && (int)iZen >= iReqZen && (int)Hero->PK < PVP_MURDERER1)
		{
			ITEM* pEquipedRightRing = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];
			ITEM* pEquipedLeftRing = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
			ITEM* pEquipedHelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
			ITEM* pEquipedWing = &CharacterMachine->Equipment[EQUIPMENT_WING];

			if (strcmp((*li)->_ReqInfo.szMainMapName, GlobalText[55]) == 0)
			{
				if (
					(
						pEquipedHelper->Type == ITEM_HELPER + 37
						//|| gCustomPet.GetInfoPetType(pEquipedHelper->Type) == 5 //Custom pet
						//|| gCustomPet.GetInfoPetType(pEquipedHelper->Type) == 6 //Custom pet
						|| pEquipedHelper->Type == ITEM_HELPER + 3
						|| pEquipedHelper->Type == ITEM_HELPER + 4
						|| pEquipedWing->Type == ITEM_HELPER + 30
						|| (pEquipedWing->Type >= ITEM_WING + 36 && pEquipedWing->Type <= ITEM_WING + 43 || gCustomWing.CheckCustomWingByItem(pEquipedWing->Type))
						|| (pEquipedWing->Type >= ITEM_WING && pEquipedWing->Type <= ITEM_WING + 6)
						|| (ITEM_WING + 130 <= pEquipedWing->Type && pEquipedWing->Type <= ITEM_WING + 134)
						|| (pEquipedWing->Type >= ITEM_WING + 49 && pEquipedWing->Type <= ITEM_WING + 50)
						|| (pEquipedWing->Type == ITEM_WING + 135) || (pEquipedWing->Type == ITEM_WING + 154))
					&& !(pEquipedHelper->Type == ITEM_HELPER + 2)
					&& (g_ChangeRingMgr->CheckBanMoveIcarusMap(pEquipedRightRing->Type, pEquipedLeftRing->Type) == false)
					)
				{
					(*li)->_bCanMove = true;
				}
				else
				{
					(*li)->_bCanMove = false;
				}
			}
			else if (strncmp((*li)->_ReqInfo.szMainMapName, GlobalText[37], 8) == 0)
			{
				if (pEquipedHelper->Type == ITEM_HELPER + 2 || pEquipedHelper->Type == ITEM_HELPER + 3)
				{
					(*li)->_bCanMove = false;
				}
				else
				{
					(*li)->_bCanMove = true;
				}
			}
			else if ((g_ServerListManager->IsNonPvP() == true) && (strcmp((*li)->_ReqInfo.szMainMapName, GlobalText[2686]) == 0))
			{
				(*li)->_bCanMove = true; //ACuoi Fix Vocanus
			}
			else
			{
				(*li)->_bCanMove = true;
			}
		}

		if ((*li)->_bCanMove && (*li)->_bStrife && 0 == Hero->m_byGensInfluence)
			(*li)->_bCanMove = false;
	}
}

bool SEASON3B::CNewUIMoveCommandWindow::BtnProcess()
{
	int iX, iY;

	if (CheckMouseIn(m_ScrollBtnPos.x, m_ScrollBtnPos.y, MOVECOMMAND_SCROLLBTN_WIDTH, MOVECOMMAND_SCROLLBTN_HEIGHT))
	{
		if (IsPress(VK_LBUTTON))
		{
			m_iScrollBtnMouseEvent = MOVECOMMAND_MOUSEBTN_CLICKED;
			m_iMousePosY = MouseY;
			m_iAcumMoveMouseScrollPixel = 0;
		}
	}

	if (IsRelease(VK_LBUTTON))
	{
		m_iScrollBtnMouseEvent = MOVECOMMAND_MOUSEBTN_NORMAL;
		m_iAcumMoveMouseScrollPixel = 0;
	}


	if (m_iScrollBtnMouseEvent == MOVECOMMAND_MOUSEBTN_CLICKED && m_icurMoveScrBtnPixelperStep > 0)
	{
		int iMoveValue = MouseY - m_iMousePosY;

		if (iMoveValue < 0)
		{
			if (MouseY <= m_ScrollBtnPos.y + (MOVECOMMAND_SCROLLBTN_HEIGHT / 2))
			{
				if (-(iMoveValue) > (m_iTotalMoveScrBtnPixel - m_iRemainMoveScrBtnPixel))
				{
					iMoveValue = -(m_iTotalMoveScrBtnPixel - m_iRemainMoveScrBtnPixel);
				}
				ScrollUp(-iMoveValue);
			}
		}
		else if (iMoveValue > 0)
		{
			if (MouseY >= m_ScrollBtnPos.y + (MOVECOMMAND_SCROLLBTN_HEIGHT / 2))
			{
				if (iMoveValue > m_iRemainMoveScrBtnPixel)
				{
					iMoveValue = m_iRemainMoveScrBtnPixel;
				}
				ScrollDown(iMoveValue);
			}
		}
		m_iMousePosY = MouseY;
	}

	if (CheckMouseIn(m_Pos.x + 20, m_Pos.y - 11, m_MapNameUISize.x, m_MapNameUISize.y) && IsPress(VK_LBUTTON))
	{
		SEASON3B::CNewUIInventoryCtrl::BackupPickedItem();
	}

	SettingCanMoveMap();

	if (CheckMouseIn(m_Pos.x + 20, m_Pos.y - 11, m_MapNameUISize.x, m_MapNameUISize.y))
	{
		if (m_icurMoveScrBtnPixelperStep > 0)
		{
			if (MouseWheel > 0)
			{
				ScrollUp(m_icurMoveScrBtnPixelperStep);
			}
			else if (MouseWheel < 0)
			{
				ScrollDown(m_icurMoveScrBtnPixelperStep);
			}
		}
		MouseWheel = 0;

		auto li = m_listMoveInfoData.begin();
		int iCurRenderTextIndex = 0;
		for (int i = 0; i < m_iRenderEndTextIndex; i++, li++)
		{
			if (li == m_listMoveInfoData.end())
			{
				break;
			}

			if (i < m_iRenderStartTextIndex)
			{
				continue;
			}
			iX = m_StartMapNamePos.x + 20;
			iY = m_StartMapNamePos.y + (m_iRealFontHeight * iCurRenderTextIndex) - 11;
			if (CheckMouseIn(iX, iY, m_MapNameUISize.x, m_iRealFontHeight))
			{
				if ((*li)->_bCanMove == true)
				{
					(*li)->_bSelected = true;

					if (SEASON3B::IsRelease(VK_LBUTTON))
					{
						if (IsTheMapInDifferentServer(gMapManager.WorldActive, (*li)->_ReqInfo.index))
						{
							SaveOptions();
						}
						SendRequestMoveMap(g_pMoveCommandWindow->GetMoveCommandKey(), (*li)->_ReqInfo.index);

						g_pNewUISystem->Hide(SEASON3B::INTERFACE_MOVEMAP);
						return true;
					}
				}
			}

			iCurRenderTextIndex++;

			if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(3, m_MapNameUISize.y - m_iRealFontHeight - 17, m_MapNameUISize.x - 5, m_iRealFontHeight))
			{
				g_pNewUISystem->Hide(SEASON3B::INTERFACE_MOVEMAP);
				return true;
			}
		}
	}
	return false;
}

bool SEASON3B::CNewUIMoveCommandWindow::UpdateMouseEvent()
{
	if (true == BtnProcess())
		return false;

	if (m_iScrollBtnMouseEvent == MOVECOMMAND_MOUSEBTN_CLICKED)
		return false;

	if (CheckMouseIn(m_Pos.x + 20, m_Pos.y - 11, m_MapNameUISize.x, m_MapNameUISize.y))
		return false;

	return true;
}

bool SEASON3B::CNewUIMoveCommandWindow::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			Show(false);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}
	return true;
}

bool SEASON3B::CNewUIMoveCommandWindow::Update()
{
	if (!IsVisible())
	{
		return true;
	}

	UpdateScrolling();

	return true;
}

void SEASON3B::CNewUIMoveCommandWindow::ScrollUp(int iMoveValue)
{
	if (m_iRemainMoveScrBtnperStep < m_iTotalMoveScrBtnperStep)
	{
		int iMovePixel = 0;
		m_iAcumMoveMouseScrollPixel -= iMoveValue;
		if ((-m_iAcumMoveMouseScrollPixel) < m_icurMoveScrBtnPixelperStep)
		{
			return;
		}
		else
		{
			RecursiveCalcScroll(m_iAcumMoveMouseScrollPixel, &iMovePixel, false);

			m_ScrollBtnPos.y += iMovePixel;
			m_iAcumMoveMouseScrollPixel -= iMovePixel;
		}
	}
}

void SEASON3B::CNewUIMoveCommandWindow::ScrollDown(int iMoveValue)
{
	if (m_iRemainMoveScrBtnperStep > 0)
	{
		int iMovePixel = 0;
		m_iAcumMoveMouseScrollPixel += iMoveValue;
		if (m_iAcumMoveMouseScrollPixel < m_icurMoveScrBtnPixelperStep)
		{
			return;
		}
		else
		{
			RecursiveCalcScroll(m_iAcumMoveMouseScrollPixel, &iMovePixel, true);
			m_iAcumMoveMouseScrollPixel -= iMovePixel;
			m_ScrollBtnPos.y += iMovePixel;
		}
	}
}

void SEASON3B::CNewUIMoveCommandWindow::RecursiveCalcScroll(IN int piScrollValue, OUT int* piMovePixel, bool bSign /* = true */)
{
	if (bSign == true)
	{ // DownScroll
		if (m_iRemainMoveScrBtnperStep > 0)
		{
			m_iRemainMoveScrBtnperStep--;
			m_iRemainMoveScrBtnPixel -= m_icurMoveScrBtnPixelperStep;
			piScrollValue -= m_icurMoveScrBtnPixelperStep;
			(*piMovePixel) += m_icurMoveScrBtnPixelperStep;

			if (m_iRemainMoveScrBtnperStep > m_iTotalNumMinMoveScrBtnperStep)
			{
				m_icurMoveScrBtnPixelperStep = m_iMaxMoveScrBtnPixelperStep;
			}
			else
			{
				m_icurMoveScrBtnPixelperStep = m_iMinMoveScrBtnPixelperStep;
			}

			if (piScrollValue >= m_icurMoveScrBtnPixelperStep)
			{
				RecursiveCalcScroll(piScrollValue, piMovePixel, bSign);
			}
		}
		else
		{
			(*piMovePixel) = piScrollValue;
		}
	}
	else
	{ // UpScroll
		if (m_iRemainMoveScrBtnperStep < m_iTotalMoveScrBtnperStep)
		{
			m_iRemainMoveScrBtnperStep++;
			m_iRemainMoveScrBtnPixel += m_icurMoveScrBtnPixelperStep;
			piScrollValue += m_icurMoveScrBtnPixelperStep;
			(*piMovePixel) -= m_icurMoveScrBtnPixelperStep;

			if (m_iRemainMoveScrBtnperStep >= m_iTotalNumMinMoveScrBtnperStep)
			{
				m_icurMoveScrBtnPixelperStep = m_iMaxMoveScrBtnPixelperStep;
			}
			else
			{
				m_icurMoveScrBtnPixelperStep = m_iMinMoveScrBtnPixelperStep;
			}

			if ((-piScrollValue) >= m_icurMoveScrBtnPixelperStep)
			{
				RecursiveCalcScroll(piScrollValue, piMovePixel, bSign);
			}
		}
		else
		{
			(*piMovePixel) = piScrollValue;
		}
	}

	return;
}


void SEASON3B::CNewUIMoveCommandWindow::UpdateScrolling()
{
	m_iRenderStartTextIndex = m_iTotalMoveScrBtnperStep - m_iRemainMoveScrBtnperStep;

	m_iRenderEndTextIndex = m_iRenderStartTextIndex + MOVECOMMAND_MAX_RENDER_TEXTLINE;

	if (m_iRenderEndTextIndex > (int)m_listMoveInfoData.size())
	{
		m_iRenderEndTextIndex -= (m_iRenderEndTextIndex - m_listMoveInfoData.size());
	}
}

void SEASON3B::CNewUIMoveCommandWindow::RenderBack(int x, int y, int width, int height)
{
	EnableAlphaTest();
	glColor4f(0.0, 0.0, 0.0, 0.8f);
	RenderColor(x + 3.f, y + 2.f, width - 7.f, height - 7, 0.0, 0);
	EndRenderColor();

	/*RenderImage(CNewUIRanking::IMAGE_TABLE_TOP_LEFT, x, y, 14.0, 14.0);
	RenderImage(CNewUIRanking::IMAGE_TABLE_TOP_RIGHT, (x + width) - 14.f, y, 14.0, 14.0);
	RenderImage(CNewUIRanking::IMAGE_TABLE_BOTTOM_LEFT, x, (y + height) - 14.f, 14.0, 14.0);
	RenderImage(CNewUIRanking::IMAGE_TABLE_BOTTOM_RIGHT, (x + width) - 14.f, (y + height) - 14.f, 14.0, 14.0);
	RenderImage(CNewUIRanking::IMAGE_TABLE_TOP_PIXEL, x + 6.f, y, (width - 12.f), 14.0);
	RenderImage(CNewUIRanking::IMAGE_TABLE_RIGHT_PIXEL, (x + width) - 14.f, y + 6.f, 14.0, (height - 14.f));
	RenderImage(CNewUIRanking::IMAGE_TABLE_BOTTOM_PIXEL, x + 6.f, (y + height) - 14.f, (width - 12.f), 14.0);
	RenderImage(CNewUIRanking::IMAGE_TABLE_LEFT_PIXEL, x, (y + 6.f), 14.0, (height - 14.f));*/
}
void SEASON3B::CNewUIMoveCommandWindow::RenderFrame()
{
	RenderImage(IMAGE_MOVECOMMAND_HEADER_TOP, (float)m_Pos.x, (float)m_Pos.y - 1, 210, 294);

	RenderImage(IMAGE_MOVECOMMAND_SCROLL_TOP, m_ScrollBarPos.x, m_ScrollBarPos.y, MOVECOMMAND_SCROLLBAR_TOP_WIDTH, MOVECOMMAND_SCROLLBAR_TOP_HEIGHT);		// TOP
	
	int icntText = 0;
	for (int i = 0; i < m_iScrollBarMiddleNum; i++)
	{
		icntText = i;
		RenderImage(IMAGE_MOVECOMMAND_SCROLL_MIDDLE, m_ScrollBarPos.x,
			m_ScrollBarPos.y + MOVECOMMAND_SCROLLBAR_TOP_HEIGHT + (i * MOVECOMMAND_SCROLLBAR_MIDDLE_HEIGHT),
			MOVECOMMAND_SCROLLBAR_TOP_WIDTH, MOVECOMMAND_SCROLLBAR_MIDDLE_HEIGHT);	// MIDDLE
	}
	if (m_iScrollBarMiddleRemainderPixel > 0)
	{
		RenderImage(IMAGE_MOVECOMMAND_SCROLL_MIDDLE, m_ScrollBarPos.x,
			m_ScrollBarPos.y + MOVECOMMAND_SCROLLBAR_TOP_HEIGHT + (icntText * MOVECOMMAND_SCROLLBAR_MIDDLE_HEIGHT),
			MOVECOMMAND_SCROLLBAR_TOP_WIDTH, m_iScrollBarMiddleRemainderPixel);	// MIDDLE ³ª¸ÓÁö
	}
	
	RenderImage(IMAGE_MOVECOMMAND_SCROLL_BOTTOM, m_ScrollBarPos.x, m_ScrollBarPos.y + m_iScrollBarHeightPixel - 5,
		MOVECOMMAND_SCROLLBAR_TOP_WIDTH, MOVECOMMAND_SCROLLBAR_TOP_HEIGHT);		// BOTTOM
	
	if (m_bScrollBtnActive == true)
	{
		if (m_iScrollBtnMouseEvent == MOVECOMMAND_MOUSEBTN_CLICKED)
		{
			glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
		}
		RenderImage(IMAGE_MOVECOMMAND_SCROLLBAR_ON, m_ScrollBtnPos.x, m_ScrollBtnPos.y,
			MOVECOMMAND_SCROLLBTN_WIDTH, MOVECOMMAND_SCROLLBTN_HEIGHT);
	}
	else
	{
		RenderImage(IMAGE_MOVECOMMAND_SCROLLBAR_OFF, m_ScrollBtnPos.x, m_ScrollBtnPos.y,
			MOVECOMMAND_SCROLLBTN_WIDTH, MOVECOMMAND_SCROLLBTN_HEIGHT);
	}

	int LW = 70, CW = 30, RW = 44, FW = 156;
	
	IsToolKit.ThisFont(m_Pos.x + 23,			m_StartUISubjectName.y - 1, 0xFFFFFFFF, 0x0080C080, FW, 0, 3, GlobalText[933]);
	IsToolKit.ThisFont(m_Pos.x + 23,			m_StartUISubjectName.y + 9, 0xFF0000FF, 0,			 LW, 0, 3, GlobalText[934]);
	IsToolKit.ThisFont(m_Pos.x + 23 + LW,		m_StartUISubjectName.y + 9, 0xFF0000FF, 0,			 CW, 0, 3, GlobalText[935]);
	IsToolKit.ThisFont(m_Pos.x + 23 + LW + CW,	m_StartUISubjectName.y + 9, 0xFF0000FF, 0,			 RW, 0, 3, GlobalText[936]);
}

bool SEASON3B::CNewUIMoveCommandWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	RenderFrame();

	auto li = m_listMoveInfoData.begin();
	int iX, iY;

	int iLevel = CharacterAttribute->Level;
	DWORD iZen = CharacterMachine->Gold;
	int iReqLevel;
	char szText[24];

	int iCurRenderTextIndex = 0;
	for (int i = 0; i < m_iRenderEndTextIndex; i++, li++)
	{
		if (li == m_listMoveInfoData.end())
		{
			break;
		}

		if (i < m_iRenderStartTextIndex)
		{
			continue;
		}

		iX = m_StartMapNamePos.x + 20;
		iY = m_StartMapNamePos.y + (m_iRealFontHeight * iCurRenderTextIndex) - 11;

		iReqLevel = (*li)->_ReqInfo.iReqLevel;
		if ((gCharacterManager.GetBaseClass(CharacterAttribute->Class) == CLASS_MAGIC 
			|| gCharacterManager.GetBaseClass(CharacterAttribute->Class) == CLASS_DARK_LORD
			|| gCharacterManager.GetBaseClass(CharacterAttribute->Class) == CLASS_RAGEFIGHTER)
			&& (iReqLevel != 400))
		{
			iReqLevel = int(float(iReqLevel) * 2.f / 3.f);
		}

		DWORD ReqColor = 0;

		if ((*li)->_bCanMove == true)
		{
			if ((*li)->_bSelected == true)
			{
				IsToolKit.ThisFont(iX, iY, 0, 0x0080C080, 144, m_iRealFontHeight, 3, " ");
			}

			ReqColor = 0xFFFFFFFF;

			IsToolKit.ThisFont(m_Pos.x + 23, iY, ReqColor, 0, 70, 0, 3, (*li)->_ReqInfo.szMainMapName);

			itoa(iReqLevel, szText, 10);
			IsToolKit.ThisFont(m_Pos.x + 93, iY, ReqColor, 0, 30, 0, 3, szText);

			itoa((*li)->_ReqInfo.iReqZen, szText, 10);
			IsToolKit.ThisFont(m_Pos.x + 123, iY, ReqColor, 0, 44, 0, 3, szText);
		}
		else
		{
			itoa(iReqLevel, szText, 10);

			if (iReqLevel > iLevel || (*li)->_ReqInfo.iReqZen > (int)iZen)
			{
				ReqColor = 0xFF331AFF;
			}
			else
			{
				ReqColor = 0xA42711FF;
			}

			IsToolKit.ThisFont(m_Pos.x + 23, iY, ReqColor, 0, 70, 0, 3, (*li)->_ReqInfo.szMainMapName);
			IsToolKit.ThisFont(m_Pos.x + 93, iY, ReqColor, 0, 30, 0, 3, szText);
			itoa((*li)->_ReqInfo.iReqZen, szText, 10);
			IsToolKit.ThisFont(m_Pos.x + 123, iY, ReqColor, 0, 44, 0, 3, szText);
		}

		iCurRenderTextIndex++;
	}

	IsToolKit.ThisFont(m_Pos.x + 23, m_MapNameUISize.y - m_iRealFontHeight - 20, 0xFFFFFFFF, 0x0080C080, 156, 0, 3, GlobalText[1002]);
	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIMoveCommandWindow::OpenningProcess()
{
	SetPos(m_Pos.x, m_Pos.y);
	SetStrifeMap();
	SettingCanMoveMap();

	m_iScrollBtnMouseEvent = MOVECOMMAND_MOUSEBTN_NORMAL;
	m_ScrollBtnPos.y = m_ScrollBtnStartPos.y;
	m_iRenderStartTextIndex = 0;

	m_iRenderEndTextIndex = m_iRenderStartTextIndex + MOVECOMMAND_MAX_RENDER_TEXTLINE;

	if (m_iRenderEndTextIndex > (int)m_listMoveInfoData.size())
	{
		m_iRenderEndTextIndex -= (m_iRenderEndTextIndex - m_listMoveInfoData.size());
	}
}

void SEASON3B::CNewUIMoveCommandWindow::ClosingProcess()
{

}

float SEASON3B::CNewUIMoveCommandWindow::GetLayerDepth()
{
	return 8.3f;
}

void SEASON3B::CNewUIMoveCommandWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_scrollbar_up.tga", IMAGE_MOVECOMMAND_SCROLL_TOP);
	LoadBitmap("Interface\\newui_scrollbar_m.tga", IMAGE_MOVECOMMAND_SCROLL_MIDDLE, GL_LINEAR);
	LoadBitmap("Interface\\newui_scrollbar_down.tga", IMAGE_MOVECOMMAND_SCROLL_BOTTOM);
	LoadBitmap("Interface\\newui_scroll_on.tga", IMAGE_MOVECOMMAND_SCROLLBAR_ON, GL_LINEAR);
	LoadBitmap("Interface\\newui_scroll_off.tga", IMAGE_MOVECOMMAND_SCROLLBAR_OFF, GL_LINEAR);
	LoadBitmap("ACuoi\\Minimap_positionB.tga", IMAGE_MOVECOMMAND_HEADER_TOP, GL_LINEAR);
}

void CNewUIMoveCommandWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_MOVECOMMAND_SCROLL_TOP);
	DeleteBitmap(IMAGE_MOVECOMMAND_SCROLL_MIDDLE);
	DeleteBitmap(IMAGE_MOVECOMMAND_SCROLL_BOTTOM);
	DeleteBitmap(IMAGE_MOVECOMMAND_SCROLLBAR_ON);
	DeleteBitmap(IMAGE_MOVECOMMAND_SCROLLBAR_OFF);
	DeleteBitmap(IMAGE_MOVECOMMAND_HEADER_TOP);
}

BOOL CNewUIMoveCommandWindow::IsTheMapInDifferentServer(const int iFromMapIndex, const int iToMapIndex) const
{
	BOOL bInOtherServer = FALSE;

	switch (iFromMapIndex)
	{
	case WD_30BATTLECASTLE:
	case WD_79UNITEDMARKETPLACE:
		bInOtherServer = TRUE;
		break;
	default:
		break;
	}

	switch (iToMapIndex)
	{
	case 24:
	case 44:
		bInOtherServer = TRUE;
		break;
	default:
		break;
	}

	return bInOtherServer;
}

int CNewUIMoveCommandWindow::GetMapIndexFromMovereq(const char* pszMapName)
{
	if (pszMapName == NULL)
		return -1;

	int iMapIndex = -1;
	std::list<CMoveCommandData::MOVEINFODATA*>::iterator li;
	for (li = m_listMoveInfoData.begin(); li != m_listMoveInfoData.end(); li++)
	{
		if (stricmp((*li)->_ReqInfo.szMainMapName, pszMapName) == 0 || stricmp((*li)->_ReqInfo.szSubMapName, pszMapName) == 0)
		{
			iMapIndex = (*li)->_ReqInfo.index;
			break;
		}
	}

	return iMapIndex;
}

