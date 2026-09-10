//*****************************************************************************
// File: UIMapName.cpp
//*****************************************************************************

#include "stdafx.h"
#include "UIMapName.h"
#include "MapManager.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"

#include "UIWindows.h"
#ifdef ASG_ADD_GENS_SYSTEM
#include "ZzzInventory.h"
#endif	// ASG_ADD_GENS_SYSTEM
#include "ThangCuoi\WideData.h"



#define	UIMN_SHOW_TIME			5000
#define	UIMN_ALPHA_VARIATION	0.015f

#define	UIMN_IMG_WIDTH			166.0f
#define	UIMN_IMG_HEIGHT			45.0f

#ifdef ASG_ADD_GENS_SYSTEM
#define UIMN_STRIFE_HEIGHT		28.0f
#endif	// ASG_ADD_GENS_SYSTEM

#define	UIMN_IMG_POS_X		((::GetScreenWidth() * gPosWide.x_fScreenRate_x - UIMN_IMG_WIDTH) / 2.0f) + gPosWide.x_GetAddPos + 65
#define	UIMN_IMG_POS_Y		(220.0f * gPosWide.x_fScreenRate_y)

#ifdef ASG_ADD_GENS_SYSTEM
#define UIMN_STRIFE_POS_Y	(UIMN_IMG_POS_Y - UIMN_STRIFE_HEIGHT)
#endif	// ASG_ADD_GENS_SYSTEM

CUIMapName::CUIMapName()
{
	InitImgPathMap();
}

CUIMapName::~CUIMapName()
{

}

void CUIMapName::InitImgPathMap()
{
	std::string strFolderName = "Local\\" + g_strSelectedML + "\\ImgsMapName\\";

	m_mapImgPath[0] = strFolderName + "00-Lorencia.tga";
	m_mapImgPath[1] = strFolderName + "01-Dungeon.tga";
	m_mapImgPath[2] = strFolderName + "02-Davias.tga";
	m_mapImgPath[3] = strFolderName + "03.Noria.tga";
	m_mapImgPath[4] = strFolderName + "04-Losttower.tga";

	m_mapImgPath[6] = strFolderName + "06-Stadium.tga";
	m_mapImgPath[7] = strFolderName + "07-Atlans.tga";
	m_mapImgPath[8] = strFolderName + "08-Tarkan.tga";
	m_mapImgPath[9] = strFolderName + "09-DeviSquare.tga";
	m_mapImgPath[10] = strFolderName + "10-Icarus.tga";
	m_mapImgPath[11] = strFolderName + "11-BloodCastle.tga";
	m_mapImgPath[12] = strFolderName + "11-BloodCastle.tga";
	m_mapImgPath[13] = strFolderName + "11-BloodCastle.tga";
	m_mapImgPath[14] = strFolderName + "11-BloodCastle.tga";
	m_mapImgPath[15] = strFolderName + "11-BloodCastle.tga";
	m_mapImgPath[16] = strFolderName + "11-BloodCastle.tga";
	m_mapImgPath[17] = strFolderName + "11-BloodCastle.tga";
	m_mapImgPath[18] = strFolderName + "12-ChaosCastle.tga";
	m_mapImgPath[19] = strFolderName + "12-ChaosCastle.tga";
	m_mapImgPath[20] = strFolderName + "12-ChaosCastle.tga";
	m_mapImgPath[21] = strFolderName + "12-ChaosCastle.tga";
	m_mapImgPath[22] = strFolderName + "12-ChaosCastle.tga";
	m_mapImgPath[23] = strFolderName + "12-ChaosCastle.tga";
	m_mapImgPath[24] = strFolderName + "24-Kalima.tga";
	m_mapImgPath[25] = strFolderName + "24-Kalima.tga";
	m_mapImgPath[26] = strFolderName + "24-Kalima.tga";
	m_mapImgPath[27] = strFolderName + "24-Kalima.tga";
	m_mapImgPath[28] = strFolderName + "24-Kalima.tga";
	m_mapImgPath[29] = strFolderName + "24-Kalima.tga";
	m_mapImgPath[30] = strFolderName + "30-LorenValey.tga";
	m_mapImgPath[31] = strFolderName + "31-Ordeal.tga";

	m_mapImgPath[33] = strFolderName + "33-Aida.tga";
	m_mapImgPath[34] = strFolderName + "34-Crywolf.tga";

	m_mapImgPath[36] = strFolderName + "36-LostKalima.tga";
	m_mapImgPath[37] = strFolderName + "37-Kantru.tga";
	m_mapImgPath[38] = strFolderName + "37-Kantru.tga";
	m_mapImgPath[39] = strFolderName + "37-Kantru.tga";

	m_mapImgPath[41] = strFolderName + "41-Barrack.tga";
	m_mapImgPath[42] = strFolderName + "42-Refuge.tga";

	m_mapImgPath[45] = strFolderName + "45-IllusionTemple.tga";
	m_mapImgPath[46] = strFolderName + "45-IllusionTemple.tga";
	m_mapImgPath[47] = strFolderName + "45-IllusionTemple.tga";
	m_mapImgPath[48] = strFolderName + "45-IllusionTemple.tga";
	m_mapImgPath[49] = strFolderName + "45-IllusionTemple.tga";
	m_mapImgPath[50] = strFolderName + "45-IllusionTemple.tga";

	m_mapImgPath[51] = strFolderName + "51-Elbeland.tga";
	m_mapImgPath[52] = strFolderName + "11-BloodCastle.tga";
	m_mapImgPath[53] = strFolderName + "12-ChaosCastle.tga";

	m_mapImgPath[56] = strFolderName + "56-SwampOfCalmness.tga";
	m_mapImgPath[57] = strFolderName + "57-Raklion.tga";
	m_mapImgPath[58] = strFolderName + "58-RaklionBoss.tga";

	m_mapImgPath[62] = strFolderName + "62-SantaTown.tga";

	m_mapImgPath[63] = strFolderName + "63-PkField.tga";

	m_mapImgPath[64] = strFolderName + "64-DuelArena.tga";

	m_mapImgPath[65] = strFolderName + "65-DoppelGanger.tga";

	m_mapImgPath[66] = strFolderName + "65-DoppelGanger.tga";

	m_mapImgPath[67] = strFolderName + "65-DoppelGanger.tga";

	m_mapImgPath[68] = strFolderName + "65-DoppelGanger.tga";

	m_mapImgPath[69] = strFolderName + "69-EmpireGuardian.tga";

	m_mapImgPath[70] = strFolderName + "69-EmpireGuardian.tga";

	m_mapImgPath[71] = strFolderName + "69-EmpireGuardian.tga";

	m_mapImgPath[72] = strFolderName + "69-EmpireGuardian.tga";

	m_mapImgPath[79] = strFolderName + "79-LorenMarket.tga";

#ifdef ASG_ADD_MAP_KARUTAN
	m_mapImgPath[80] = strFolderName + "80-Karutan.tga";
	m_mapImgPath[81] = strFolderName + "80-Karutan.tga";
#endif	// ASG_ADD_MAP_KARUTAN

	m_mapImgPath[82] = strFolderName + "82-WarGuild.tga";
	m_mapImgPath[83] = strFolderName + "83-Nars.tga"; // Nars
	m_mapImgPath[84] = strFolderName + "84-Archeron.tga"; // Archeron
	m_mapImgPath[85] = strFolderName + "85-Debenter.tga"; // Debenter
}

void CUIMapName::Init()
{
	m_eState = HIDE;
	m_nOldWorld = -1;
	m_dwOldTime = ::timeGetTime();
	m_dwDeltaTickSum = 0;
	m_fAlpha = 1.0f;
#ifdef ASG_ADD_GENS_SYSTEM
	m_bStrife = false;
#endif	// ASG_ADD_GENS_SYSTEM
}

void CUIMapName::ShowMapName()
{
	m_eState = FADEIN;
	m_fAlpha = 0.2f;
	m_dwDeltaTickSum = 0;

	if (gMapManager.WorldActive == WD_40AREA_FOR_GM)
	{
		m_eState = HIDE;
		return;
	}

	if (m_nOldWorld != gMapManager.WorldActive)
	{
		char szImgPath[128];
		::strcpy(szImgPath, m_mapImgPath[gMapManager.WorldActive].data());

		DeleteBitmap(BITMAP_INTERFACE_EX + 45);
		LoadBitmap(szImgPath, BITMAP_INTERFACE_EX + 45);

		m_nOldWorld = gMapManager.WorldActive;

#ifdef ASG_ADD_GENS_SYSTEM
		m_bStrife = ::IsStrifeMap(gMapManager.WorldActive);
#endif	// ASG_ADD_GENS_SYSTEM
	}
}

void CUIMapName::Update()
{
	DWORD dwNowTime = ::timeGetTime();
	DWORD dwDeltaTick = dwNowTime - m_dwOldTime;

	switch (m_eState)
	{
	case FADEIN:
		m_fAlpha += UIMN_ALPHA_VARIATION;
		if (1.0f <= m_fAlpha)
		{
			m_eState = SHOW;
			m_fAlpha = 1.0f;
		}
		break;

	case SHOW:
		m_dwDeltaTickSum += dwDeltaTick;
		if (m_dwDeltaTickSum > UIMN_SHOW_TIME)
		{
			m_eState = FADEOUT;
			m_dwDeltaTickSum = 0;
		}
		break;

	case FADEOUT:
		m_fAlpha -= UIMN_ALPHA_VARIATION;
		if (0.0f >= m_fAlpha)
		{
			m_eState = HIDE;
			m_fAlpha = 0.0f;
		}
		break;
	}

	m_dwOldTime = dwNowTime;
}

void CUIMapName::Render()
{
	Update();

	if (HIDE == m_eState)
		return;

	::EnableAlphaTest();
	::glColor4f(1.0f, 1.0f, 1.0f, m_fAlpha);

#ifdef ASG_ADD_GENS_SYSTEM
	if (m_bStrife)
		::RenderBitmap(BITMAP_INTERFACE_EX + 47, UIMN_IMG_POS_X, UIMN_STRIFE_POS_Y, UIMN_IMG_WIDTH, UIMN_STRIFE_HEIGHT, 0.0f, 0.0f, UIMN_IMG_WIDTH / 256.0f, UIMN_STRIFE_HEIGHT / 32.0f, false, false);
#endif	// ASG_ADD_GENS_SYSTEM
	::RenderBitmap(BITMAP_INTERFACE_EX + 45, UIMN_IMG_POS_X, UIMN_IMG_POS_Y + 70, UIMN_IMG_WIDTH, UIMN_IMG_HEIGHT, 0.0f, 0.0f, UIMN_IMG_WIDTH / 256.0f, 90.f / 128.0f, false, false);

	::glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	::DisableAlphaBlend();
}