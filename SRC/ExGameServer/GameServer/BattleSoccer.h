#pragma once

#include "BattleGround.h"
#include "User.h"

#define MAX_BATTLESOCCER_GROUND 1

class CBattleSoccer : public CBattleGround
{
	public:
	CBattleSoccer();
	void SetGoalMove(int set_code);
	int GetGoalMove();
	void SetGoalLeftRect(int x, int y, int xl, int yl);
	void SetGoalRightRect(int x, int y, int xl, int yl);

	int m_goalmove;
	int m_BallIndex;	
	RECT m_GoalLeft;	
	RECT m_GoalRight;	
	GUILD_INFO_STRUCT* m_RedTeam;	
	GUILD_INFO_STRUCT* m_BlueTeam;	
};

extern CBattleSoccer* gBSGround[MAX_BATTLESOCCER_GROUND];
