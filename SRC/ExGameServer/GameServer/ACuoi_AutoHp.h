#pragma once
#include "User.h"

class CAUTOHP
{
	public:
	void AutoHp(LPOBJ lpObj);
	void BomMana(LPOBJ lpObj);
	bool CommandHp(LPOBJ lpObj);
	void MainProc();
};

extern CAUTOHP gAUTOHP;