#pragma once

class CLifeStone
{
public:
	bool CreateLifeStone(int aIndex);
	bool DeleteLifeStone(int aIndex);
	bool SetReSpawnUserXY(int aIndex);
	void LifeStoneAct(int aIndex);
};

extern CLifeStone gLifeStone;
