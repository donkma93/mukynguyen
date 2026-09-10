#pragma once

#define MAX_KANTURU_BATTLE_USER 15

class CKanturuBattleUser
{
public:
	CKanturuBattleUser()
	{
		this->ResetData();
	}

	void ResetData()
	{
		this->m_Index = -1;
		this->m_IsUse = 0;
	}

	void SetIndex(int aIndex)
	{
		this->m_Index = aIndex;
		this->m_IsUse = 1;
	}

	int GetIndex()
	{
		return this->m_Index;
	}

	int IsUseData()
	{
		return this->m_IsUse;
	}
private:
	int m_Index;
	int m_IsUse;
};

class CKanturuBattleUserMng
{
public:
	CKanturuBattleUserMng();
	void ResetAllData();
	bool AddUserData(int aIndex);
	bool DelUserData(int aIndex);
	void CheckUserState();
	void MoveAllUser(int gate);
	int GetUserCount();
	int GetMaxUser();
	int IsEmpty();
	int IsOverMaxUser();
public:
	CKanturuBattleUser m_BattleUser[MAX_KANTURU_BATTLE_USER];
	int m_BattleUserCount;
	int m_BattleMaxUser;
};

extern CKanturuBattleUserMng gKanturuBattleUserMng;
