#pragma once
#if(ANTIHACKGS)
struct SpeedPlayer
{
	DWORD m_Tick;
	DWORD m_DCount;
	DWORD m_Time;
	DWORD m_CheckTick;
};

struct AnimationPlayer
{
	DWORD m_DCount;
	DWORD m_XDameCount;
	DWORD m_XDameTick;
};
struct ANTIATTACKDELAY_DATA
{
	int Index;
	int MinSpeed;
	int MaxSpeed;
	int MaxCount;
	int MsClass[7];
};


struct CalSpeedNew
{
	int Index;
	int aIndex;
	int MSTime;
};

struct ANTIMOVESPEED_DATA
{
	int Enabled;
	int Debug;
	int SpeedNormal;
	int SpeedPet;
	int SpeedWing;
	int GateMove;

};

struct ANTIANIMATION_DATA
{
	int Index;
	int SkillIndex;
	int TypeSkill;
	float AnimationSpeed;
	int XDameMinMS;
	int LimitSpeed;
	int MaxCount;
};

class CBProtect
{
public:
	CBProtect();
	virtual ~CBProtect();


	void	LoadConfig(char* path);

	char* GetMessage(int index);
	bool	CheckPhamViSkill(int aIndex, int bIndex, int SkillIndex);
	BOOL 	AntiDelayAttack(int aIndex);
	SpeedPlayer Speed[1000];
	AnimationPlayer Animation[1000];
	DWORD	CheckTimeAttack[1000];

	int GetAttackMS(int aIndex, int Number); //Attack Thuong
	void 	KiemTraDiChuyen(int aIndex, int NewX, int NewY);
	ANTIMOVESPEED_DATA  m_AntiMoveSpeed;
	//==
	bool AttackDelay_Enable;
	bool AttackDelay_Debug;
	///=== ANimation
	bool Animation_Enable;
	bool Animation_Debug;
	BOOL	    CheckAnimationSkill(int aIndex, int SkillIndex, int Type);
	bool CheckUserMove(LPOBJ lpObj);
	//
	BOOL	    AntiAutoSkill(int bIndex);
	void 		BQuetDupe(int aIndex);
private:
	std::map<int, ANTIATTACKDELAY_DATA> m_AntiAttackDelay;
	bool GetAttackDelayBySpeed(int Speed, ANTIATTACKDELAY_DATA* lpInfo);
	bool KiemTraDelayInSkill(LPOBJ lpObj, int SkillIndex);
	//====Animation
	std::map<int, ANTIANIMATION_DATA>   m_AntiAnimation;
	bool GetAnimationBySkill(int SkillIndex, ANTIANIMATION_DATA* lpInfo);
	//=====================
	std::map<int, CalSpeedNew> m_AttackDamageSize[1000];
	int AttackCountSpeed[1000];
	DWORD SetTimeRsAttack[1000];

	//=====================
	int AnimationCountSpeed[1000];
	int AnimationSkillLast[1000];
	DWORD SetTimeAnimation[1000];
	DWORD SetTimeRsAnimation[1000];

};

extern CBProtect gBProtect;
#endif