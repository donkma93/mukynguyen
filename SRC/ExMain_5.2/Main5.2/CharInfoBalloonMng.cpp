#include "stdafx.h"
#include "CharInfoBalloonMng.h"

#include "CharInfoBalloon.h"


CCharInfoBalloonMng::CCharInfoBalloonMng()
{
	m_pCharInfoBalloon = NULL;
}

CCharInfoBalloonMng::~CCharInfoBalloonMng()
{
	Release();
}

void CCharInfoBalloonMng::Release()
{
	SAFE_DELETE_ARRAY(m_pCharInfoBalloon);
}

void CCharInfoBalloonMng::Create()
{
	if (NULL == m_pCharInfoBalloon)
		m_pCharInfoBalloon = new CCharInfoBalloon[5];

	for (int i = 0; i < 5; ++i)
		m_pCharInfoBalloon[i].Create(&CharactersClient[i]);
}

void CCharInfoBalloonMng::Render()
{
	if (NULL == m_pCharInfoBalloon)
		return;

	for (int i = 0; i < 5; ++i)
		m_pCharInfoBalloon[i].Render();
}

void CCharInfoBalloonMng::UpdateDisplay()
{
	if (NULL == m_pCharInfoBalloon)
		return;

	for (int i = 0; i < 5; ++i)
		m_pCharInfoBalloon[i].SetInfo();
}