
#include "stdafx.h"
#include "CharInfoBalloon.h"
#include "ZzzOpenglUtil.h"
#include "ZzzInterface.h"
#include "UIControls.h"
#include "CharacterManager.h"
#include "ThangCuoi\WideData.h"
#include <ThangCuoi/ZzzToolKit.h>

CCharInfoBalloon::CCharInfoBalloon()
{
	m_pCharInfo = NULL;
	m_dwNameColor = 0;
	m_szName[0] = '\0';
	m_szGuild[0] = '\0';
	m_szClass[0] = '\0';
}

CCharInfoBalloon::~CCharInfoBalloon()
{
}

void CCharInfoBalloon::Create(CHARACTER* pCharInfo)
{
	CSprite::Create(118, 54, BITMAP_LOG_IN + 7, 0, NULL, 57, 54);

	m_pCharInfo = pCharInfo;
	m_dwNameColor = 0;
	memset(m_szName, 0, sizeof(char) * 64);
	memset(m_szGuild, 0, sizeof(char) * 64);
	memset(m_szClass, 0, sizeof(char) * 64);
}

void CCharInfoBalloon::Render()
{
	if (NULL == m_pCharInfo || !CSprite::m_bShow)
		return;

	CSprite::Render();

	vec3_t afPos;
	VectorCopy(m_pCharInfo->Object.Position, afPos);
	afPos[2] += 350.f;
	int nPosX, nPosY;
	::Projection(afPos, &nPosX, &nPosY);

	CSprite::SetPosition(int(nPosX * gPosWide.x_fScreenRate_x),
		int(nPosY * gPosWide.x_fScreenRate_y));

	g_pRenderText->SetFont(g_hFont);

	int nTextPosX = int(CSprite::GetXPos() / gPosWide.x_fScreenRate_x) + 2;
	
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(m_dwNameColor);
	g_pRenderText->RenderText(nTextPosX - 5, int((CSprite::GetYPos() + 4) / gPosWide.x_fScreenRate_y), m_szName, (CSprite::GetWidth() / gPosWide.x_fScreenRate_x) + 10, 0, RT3_SORT_CENTER);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
	g_pRenderText->RenderText(nTextPosX - 5, int((CSprite::GetYPos() + 20) / gPosWide.x_fScreenRate_y), m_szGuild, (CSprite::GetWidth() / gPosWide.x_fScreenRate_x) + 10, 0, RT3_SORT_CENTER);
	g_pRenderText->SetTextColor(CLRDW_BR_ORANGE);
	g_pRenderText->RenderText(nTextPosX - 5, int((CSprite::GetYPos() + 36) / gPosWide.x_fScreenRate_y), m_szClass, ((CSprite::GetWidth()) / gPosWide.x_fScreenRate_x) + 10, 0, RT3_SORT_CENTER);


	if (m_pCharInfo->CtlCode & CTLCODE_80MANAGER_MOVE_CHAR)
	{
		g_pRenderText->SetTextColor(255, 200, 180, 255);

		for (int i = 0; i < 3; ++i)
		{
			if (i == 0)
			{
				g_pRenderText->SetFont(g_hFont);
			}
			else
			{
				g_pRenderText->SetFont(g_hFont);
			}
			g_pRenderText->RenderText(nTextPosX, int((CSprite::GetYPos() - 46 + i * 16) / gPosWide.x_fScreenRate_y), GlobalText[1241 + i], CSprite::GetWidth() / gPosWide.x_fScreenRate_x, 0, RT3_SORT_CENTER);
		}
	}
}

void CCharInfoBalloon::SetInfo()
{
	if (NULL == m_pCharInfo)
		return;

	if (!m_pCharInfo->Object.Live)
	{
		CSprite::m_bShow = false;
		return;
	}

	CSprite::m_bShow = true;

	if (m_pCharInfo->CtlCode & CTLCODE_01BLOCKCHAR)
		m_dwNameColor = ARGB(255, 0, 255, 255);
	else if (m_pCharInfo->CtlCode
		& (CTLCODE_02BLOCKITEM | CTLCODE_10ACCOUNT_BLOCKITEM))
		m_dwNameColor = CLRDW_BR_ORANGE;
	else if (m_pCharInfo->CtlCode & CTLCODE_04FORTV)
		m_dwNameColor = CLRDW_WHITE;
	else if (m_pCharInfo->CtlCode & CTLCODE_08OPERATOR)
		m_dwNameColor = ARGB(255, 255, 0, 0);
	else if (m_pCharInfo->CtlCode & CTLCODE_20OPERATOR)
		m_dwNameColor = ARGB(255, 255, 0, 0);
	else
		m_dwNameColor = CLRDW_WHITE;

	strcpy(m_szName, m_pCharInfo->ID);

	int nText = 0;
	switch (m_pCharInfo->GuildStatus)
	{
		case 255:	nText = 488;	break;
		case 0:		nText = 1330;	break;
		case 32:	nText = 1302;	break;
		case 64:	nText = 1301;	break;
		case 128:	nText = 1300;	break;
	}
	sprintf(m_szGuild, "(%s)", GlobalText[nText]);
	sprintf(m_szClass, "%s %d", gCharacterManager.GetCharacterClassText(m_pCharInfo->Class), m_pCharInfo->Level);
}