#pragma once
#include "Win.h"

class CBlackWin : public CWin  
{
	public:
	void Create();
	bool CursorInWin(int nArea);

	protected:
	double	m_dFadeDeltaTickSum;
	void UpdateWhileActive(double dDeltaTick);
};
