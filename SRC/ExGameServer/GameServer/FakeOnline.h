
#pragma once

#include "User.h"
#include <fstream>

struct OFFEXP_DATA
{
	char Account[11];
	char Password[11];
	char Name[11];
	WORD SkillID;
	WORD UseBuffs[3];
	int TuNhatItem;
	int TuDongReset;
	int GateNumber;
	int MapX;
	int MapY;
	int PhamViTrain;
	int TimeReturn;
	int PostKhiDie;
	int PVPMode;
	int PartyMode;
	int MoveRange;
	int MoveSafeZone;
	int FakeDapDo;
};

class CFakeOnline
{
	public:
	CFakeOnline();
	void RestoreFakeOnline();
	OFFEXP_DATA* GetOffExpInfo(LPOBJ lpObj);
	OFFEXP_DATA* GetOffExpInfoByAccount(LPOBJ lpObj);
	void LoadFakeData(char* path);
	void FakeAttackProc(LPOBJ lpObj);
	void Attack(int UserIndex);
	void OnAttackAlreadyConnected(LPOBJ lpObj);
	int NhatItem(int aIndex);
	void QuayLaiToaDoGoc(int aIndex);
	void PostChatMSG(LPOBJ lpObj);
	int	IndexMsgMin;
	int	IndexMsgMax;
	int MoveRange;
	void SuDungMauMana(int aIndex);
	void TuDongBuffSkill(int aIndex);
	void TuDongDanhSkill(int aIndex);
	bool GetTargetPlayer(LPOBJ lpObj, int SkillNumber, int* MonsterIndex);
	bool GetTargetMonster(LPOBJ lpObj, int SkillNumber, int* MonsterIndex);
	void SendSkillAttack(LPOBJ lpObj, int aIndex, int SkillNumber);
	void SendMultiSkillAttack(LPOBJ lpObj, int aIndex, int SkillNumber);
	void SendDurationSkillAttack(LPOBJ lpObj, int aIndex, int SkillNumber);
	void SendRFSkillAttack(LPOBJ lpObj, int aIndex, int SkillNumber);
	void	GuiYCParty(int aIndex, int bIndex);
	void RandomMoveInSafeZone(LPOBJ lpObj);
	bool CFakeOnline::CheckFakeInAcc(LPOBJ lpObj);
	void CFakeOnline::PostDrop_OZ(LPOBJ lpObj);
	std::map<std::string, OFFEXP_DATA> m_Data;

	int FakeDapDo_Rate;
	int FakeDapDo_Speed;
	int FakeDapDo_Map;
	int FakeDapDo_Index;
	int FakeDapDo_Level_Min;
	int FakeDapDo_Level_Max;

	int AccountsRestored;
	DWORD TimeFakeLogIn;
};

extern CFakeOnline s_FakeOnline;
