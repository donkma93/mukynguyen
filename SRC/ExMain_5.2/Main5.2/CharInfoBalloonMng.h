#if !defined(AFX_CHARINFOBALLOONMNG_H__37129186_F7FE_4FBC_87BD_189E01191E8F__INCLUDED_)
#define AFX_CHARINFOBALLOONMNG_H__37129186_F7FE_4FBC_87BD_189E01191E8F__INCLUDED_

#pragma once

class CCharInfoBalloon;

class CCharInfoBalloonMng  
{
protected:
	CCharInfoBalloon*	m_pCharInfoBalloon;

public:
	CCharInfoBalloonMng();
	virtual ~CCharInfoBalloonMng();

	void Release();
	void Create();
	void Render();
	void UpdateDisplay();
};

#endif 
