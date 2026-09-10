#include "stdafx.h"
#include "CommandManager.h"
#include "BProtect.h"
#include "GensSystem.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Skill.h"
#include "SkillManager.h"
#include "EffectManager.h"
#include "DSProtocol.h"
#include "GameMain.h"
#if(ANTIHACKGS)

CBProtect gBProtect;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBProtect::CBProtect() // OK
{
	ZeroMemory(Speed, sizeof(Speed));
	ZeroMemory(Animation, sizeof(Animation));
	ZeroMemory(AttackCountSpeed, sizeof(AttackCountSpeed));
	ZeroMemory(SetTimeRsAttack, sizeof(SetTimeRsAttack));
	this->AttackDelay_Enable = 0;
	this->AttackDelay_Debug = 0;
	//===Move
	this->m_AntiMoveSpeed.Enabled = 0;
	this->m_AntiMoveSpeed.GateMove = -1;
	this->m_AntiMoveSpeed.SpeedNormal = 8;
	this->m_AntiMoveSpeed.SpeedPet = 9;
	this->m_AntiMoveSpeed.SpeedWing = 9;
	//=== ANimagtionPlayer
	this->Animation_Enable = 0;
	this->Animation_Debug = 0;
	this->m_AntiAnimation.clear();
	ZeroMemory(SetTimeRsAnimation, sizeof(SetTimeRsAnimation));
	ZeroMemory(AnimationCountSpeed, sizeof(AnimationCountSpeed));
	ZeroMemory(AnimationSkillLast, sizeof(AnimationSkillLast));
	ZeroMemory(SetTimeAnimation, sizeof(SetTimeAnimation));
}

CBProtect::~CBProtect() // OK
{

}


void CBProtect::LoadConfig(char* path)
{
	ZeroMemory(Speed, sizeof(Speed));
	ZeroMemory(Animation, sizeof(Animation));
	ZeroMemory(AttackCountSpeed, sizeof(AttackCountSpeed));
	ZeroMemory(SetTimeRsAttack, sizeof(SetTimeRsAttack));
	this->AttackDelay_Enable = 0;
	//===Move
	this->m_AntiMoveSpeed.Enabled = 0;
	this->m_AntiMoveSpeed.GateMove = -1;
	this->m_AntiMoveSpeed.SpeedNormal = 8;
	this->m_AntiMoveSpeed.SpeedPet = 9;
	this->m_AntiMoveSpeed.SpeedWing = 9;
	//=== ANimagtionPlayer
	this->Animation_Enable = 0;
	this->m_AntiAnimation.clear();
	ZeroMemory(SetTimeRsAnimation, sizeof(SetTimeRsAnimation));
	ZeroMemory(AnimationCountSpeed, sizeof(AnimationCountSpeed));
	ZeroMemory(SetTimeAnimation, sizeof(SetTimeAnimation));
	ZeroMemory(AnimationSkillLast, sizeof(AnimationSkillLast));

	int Count1 = 0;
	this->m_AntiAttackDelay.clear();

	//========================
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);
	if (res.status != pugi::status_ok) {
		ErrorMessageBox("File %s load fail. Error: %s", path, res.description());
		return;
	}
	pugi::xml_node oSpeedCheck = file.child("SpeedCheck");



	//====MOve
	pugi::xml_node eAntiMoveSpeed = oSpeedCheck.child("AntiMoveSpeed");
	//--
	this->m_AntiMoveSpeed.Enabled = eAntiMoveSpeed.attribute("Enabled").as_int();
	this->m_AntiMoveSpeed.Debug = eAntiMoveSpeed.attribute("Debug").as_int();
	this->m_AntiMoveSpeed.GateMove = eAntiMoveSpeed.attribute("GateMove").as_int();
	this->m_AntiMoveSpeed.SpeedNormal = eAntiMoveSpeed.attribute("SpeedNormal").as_int();
	this->m_AntiMoveSpeed.SpeedPet = eAntiMoveSpeed.attribute("SpeedPet").as_int();
	this->m_AntiMoveSpeed.SpeedWing = eAntiMoveSpeed.attribute("SpeedWing").as_int();
	//==
	//====Animation
	pugi::xml_node eAntiAnimation = oSpeedCheck.child("AntiAnimation");
	//--
	this->Animation_Enable = eAntiAnimation.attribute("Enabled").as_bool();
	this->Animation_Debug = eAntiAnimation.attribute("Debug").as_bool();
	int AnimationCount = 0;
	for (pugi::xml_node rAntiAnimation = eAntiAnimation.child("Config"); rAntiAnimation; rAntiAnimation = rAntiAnimation.next_sibling()) {
		ANTIANIMATION_DATA info;
		info.Index = AnimationCount;
		info.SkillIndex = rAntiAnimation.attribute("SkillIndex").as_int();
		info.AnimationSpeed = rAntiAnimation.attribute("AnimationSpeed").as_float();
		info.LimitSpeed = rAntiAnimation.attribute("LimitSpeed").as_int();
		info.XDameMinMS = rAntiAnimation.attribute("XDameMinMS").as_int();
		info.TypeSkill = rAntiAnimation.attribute("TypeSkill").as_int();
		info.MaxCount = rAntiAnimation.attribute("MaxCount").as_int();
		this->m_AntiAnimation.insert(std::pair<int, ANTIANIMATION_DATA>(info.Index, info));
		AnimationCount++;
	}
	//==
	pugi::xml_node eAntiAttackDelay = oSpeedCheck.child("AntiAttackDelay");
	this->AttackDelay_Enable = eAntiAttackDelay.attribute("Enabled").as_bool();
	this->AttackDelay_Debug = eAntiAttackDelay.attribute("Debug").as_bool();
	for (pugi::xml_node rAntiAttackDelay = eAntiAttackDelay.child("Config"); rAntiAttackDelay; rAntiAttackDelay = rAntiAttackDelay.next_sibling()) {
		ANTIATTACKDELAY_DATA info;
		info.Index = Count1;
		info.MinSpeed = rAntiAttackDelay.attribute("MinSpeed").as_int();
		info.MaxSpeed = rAntiAttackDelay.attribute("MaxSpeed").as_int();
		info.MaxCount = rAntiAttackDelay.attribute("MaxCount").as_int();
		info.MsClass[0] = rAntiAttackDelay.attribute("DW").as_int();
		info.MsClass[1] = rAntiAttackDelay.attribute("DK").as_int();
		info.MsClass[2] = rAntiAttackDelay.attribute("ELF").as_int();
		info.MsClass[3] = rAntiAttackDelay.attribute("MG").as_int();
		info.MsClass[4] = rAntiAttackDelay.attribute("DL").as_int();
		info.MsClass[5] = rAntiAttackDelay.attribute("SUM").as_int();
		info.MsClass[6] = rAntiAttackDelay.attribute("RF").as_int();
		this->m_AntiAttackDelay.insert(std::pair<int, ANTIATTACKDELAY_DATA>(info.Index, info));
		Count1++;
	}
	//--

	LogAdd(LOG_BLUE, "[BProtect]Load Config Attack[%d]/Skill[%d]", Count1, AnimationCount);

}


bool CBProtect::GetAttackDelayBySpeed(int Speed, ANTIATTACKDELAY_DATA* lpInfo) // OK
{
	for (std::map<int, ANTIATTACKDELAY_DATA>::iterator it = this->m_AntiAttackDelay.begin(); it != this->m_AntiAttackDelay.end(); it++)
	{
		if (Speed >= it->second.MinSpeed && (Speed <= it->second.MaxSpeed || it->second.MaxSpeed == -1))
		{
			(*lpInfo) = it->second;
			return 1;
		}
	}

	return 0;
}
//===Animation
bool CBProtect::GetAnimationBySkill(int SkillIndex, ANTIANIMATION_DATA* lpInfo) // OK
{
	for (std::map<int, ANTIANIMATION_DATA>::iterator it = this->m_AntiAnimation.begin(); it != this->m_AntiAnimation.end(); it++)
	{
		if (SkillIndex == it->second.SkillIndex || it->second.SkillIndex == -1)
		{
			(*lpInfo) = it->second;
			return 1;
		}
	}

	return 0;
}

int CBProtect::GetAttackMS(int aIndex, int Number) // OK
{
	int ALLMSTime = 0;
	for (std::map<int, CalSpeedNew>::iterator it = this->m_AttackDamageSize[Number].begin(); it != this->m_AttackDamageSize[Number].end(); it++)
	{
		if (aIndex == it->second.aIndex)
		{
			ALLMSTime += it->second.MSTime;
		}
	}

	return ALLMSTime;
}


void CBProtect::KiemTraDiChuyen(int aIndex, int NewX, int NewY)
{
	if (!this->m_AntiMoveSpeed.Enabled || gObj[aIndex].Type != OBJECT_USER || gObj[aIndex].m_OfflineMode != 0) {
		return;

	}

	if (gObj[aIndex].CloseType != -1)
	{
		gObjMoveGate(aIndex, 17);
		return;
	}

	if ((GetTickCount() - gObj[aIndex].BProtectMove_Time) > 1000 || gObj[aIndex].Map != gObj[aIndex].BProtect_MAP)
	{
		gObj[aIndex].BProtectMove_Time = GetTickCount();
		gObj[aIndex].BProtect_MAP = gObj[aIndex].Map;
		gObj[aIndex].BProtect_X = NewX;
		gObj[aIndex].BProtect_Y = NewY;
	}

	int PhamViDiChuyen = sqrt(pow(((float)NewX - (float)gObj[aIndex].BProtect_X), 2) + pow(((float)NewY - (float)gObj[aIndex].BProtect_Y), 2));

	int PhamViChoPhep = 0;

	if (gObj[aIndex].Inventory[7].IsItem())
	{
		PhamViChoPhep = this->m_AntiMoveSpeed.SpeedWing;
	}
	else if (gObj[aIndex].Inventory[8].m_Index == 6660 || gObj[aIndex].Inventory[8].m_Index == 6693) //Chien Ma, Soi TInh
	{
		PhamViChoPhep = this->m_AntiMoveSpeed.SpeedPet;
	}
	else
	{
		PhamViChoPhep = this->m_AntiMoveSpeed.SpeedNormal;
	}

	if (PhamViDiChuyen > PhamViChoPhep)
	{
		if (this->m_AntiMoveSpeed.GateMove != -1)
		{
			gObjMoveGate(aIndex, this->m_AntiMoveSpeed.GateMove);
		}
		gLog.Output(LOG_HACK, "[AntiSpeedMove][%s][%s]Move : [%d > %d] ToaDoCu: (%d/%d) ToaDoMoi: (%d/%d)", gObj[aIndex].Account, gObj[aIndex].Name, PhamViDiChuyen, PhamViChoPhep, gObj[aIndex].BProtect_X, gObj[aIndex].BProtect_Y, NewX, NewY);
	}
	if (this->m_AntiMoveSpeed.Debug == 1)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "[AntiSpeedMove][%s][%s]Move : [%d > %d] ToaDoCu: (%d/%d) ToaDoMoi: (%d/%d)", gObj[aIndex].Account, gObj[aIndex].Name, PhamViDiChuyen, PhamViChoPhep, gObj[aIndex].BProtect_X, gObj[aIndex].BProtect_Y, NewX, NewY);
	}
	return;
}
BOOL CBProtect::AntiDelayAttack(int aIndex)
{
	if (!this->AttackDelay_Enable || gObj[aIndex].Type != OBJECT_USER || gObj[aIndex].m_OfflineMode != 0) {
		return 1;

	}
	if (gObj[aIndex].CloseType != -1) {

		return 0;
	}

	ANTIATTACKDELAY_DATA HackAttacklInfo;

	if (this->GetAttackDelayBySpeed(gObj[aIndex].PhysiSpeed, &HackAttacklInfo) == 0)
	{
		return 1;
	}
	int Number = aIndex - OBJECT_START_USER;
	DWORD CTimeAttack = GetTickCount() - gBProtect.Speed[Number].m_Time;

	DWORD SkillTimeAttack = GetTickCount() - gObj[aIndex].BProtectAnimationTime;
	DWORD AnimationTime = (GetTickCount() - this->SetTimeAnimation[Number]);
	if (SkillTimeAttack < 500 || AnimationTime < 500)
	{
		return 0;
	}

	gBProtect.Speed[Number].m_Time = GetTickCount();
	//===============================

	if (GetTickCount() - this->SetTimeRsAttack[Number] > 1000)
	{
		this->SetTimeRsAttack[Number] = GetTickCount();
		this->AttackCountSpeed[Number] = 0;
		this->m_AttackDamageSize[Number].clear();
	}
	CalSpeedNew infoSpeed;
	infoSpeed.Index = this->AttackCountSpeed[Number];
	infoSpeed.aIndex = aIndex;
	infoSpeed.MSTime = CTimeAttack;
	this->m_AttackDamageSize[Number].insert(std::pair<int, CalSpeedNew>(infoSpeed.Index, infoSpeed));
	this->AttackCountSpeed[Number]++;
	//================================
	int MSTrungBinh = (float)(this->GetAttackMS(aIndex, Number) / this->m_AttackDamageSize[Number].size());
	int CountSize = this->m_AttackDamageSize[Number].size();

	int count = 1000 / HackAttacklInfo.MsClass[gObj[aIndex].Class];


	if (count > HackAttacklInfo.MaxCount)
	{
		count = HackAttacklInfo.MaxCount;
	}
	if (this->AttackDelay_Debug == 1)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "AT : [%d]/[%d] (Count: %d/%d)", MSTrungBinh, HackAttacklInfo.MsClass[gObj[aIndex].Class], CountSize, count);
	}

	if ((CountSize - count) > 30)
	{
		return 0;
	}
	if (CountSize > count + 1)
	{
		return 0;
	}

	if (MSTrungBinh < HackAttacklInfo.MsClass[gObj[aIndex].Class])
	{
		return 0;
	}
	return 1;
}
//========= Animation Speed  New By Cuong beo =)) ==========//
BOOL CBProtect::CheckAnimationSkill(int aIndex, int SkillIndex, int Type)
{

	if (!this->Animation_Enable || gObj[aIndex].Type != OBJECT_USER || gObj[aIndex].m_OfflineMode != 0) {
		return 1;
	}

	if (gObj[aIndex].CloseType != -1) {

		return 0;
	}
	LPOBJ lpUser = &gObj[aIndex];

	int Number = aIndex - OBJECT_START_USER;

	ANTIANIMATION_DATA SpeedSkill;

	if (SkillIndex == 0)
	{
		return 1;
	}
	if (this->Animation_Debug && this->GetAnimationBySkill(SkillIndex, &SpeedSkill) == 0)
	{
		SpeedSkill.SkillIndex = -1;
		SpeedSkill.TypeSkill = -1;
		SpeedSkill.XDameMinMS = 0;
		SpeedSkill.AnimationSpeed = 6.0;
		SpeedSkill.LimitSpeed = 0;
		SpeedSkill.MaxCount = 30;
	}
	//===== Animation Anti
	//=======================================
	if (SpeedSkill.TypeSkill != -1 && SpeedSkill.TypeSkill != Type)
	{
		return 1;
	}

	int TimeCheck = (GetTickCount() - this->SetTimeAnimation[Number]);
	this->SetTimeAnimation[Number] = GetTickCount();

	if (this->AnimationSkillLast[Number] != SkillIndex && TimeCheck < 100)
	{
		return 0;
	}
	this->AnimationSkillLast[Number] = SkillIndex;


	DWORD CTimeAttack = GetTickCount() - gBProtect.Speed[Number].m_Time;
	if (CTimeAttack < 500)
	{
		return 0;
	}

	if (GetTickCount() - this->SetTimeRsAnimation[Number] > 1000)
	{
		this->SetTimeRsAnimation[Number] = GetTickCount();
		this->AnimationCountSpeed[Number] = 0;
	}

	float CalAnimation = 1000.0f;



	if (gObj[aIndex].Class == CLASS_WIZARD)
	{
		CalAnimation = (float)1000.0f / (((lpUser->PhysiSpeed) * 0.0142333198777464) + SpeedSkill.AnimationSpeed);
	}
	else
	{
		CalAnimation = (float)1000.0f / (((lpUser->PhysiSpeed) * 0.0109365655658977) + SpeedSkill.AnimationSpeed);
	}

	if (CalAnimation < SpeedSkill.LimitSpeed)
	{
		CalAnimation = SpeedSkill.LimitSpeed;
	}

	int count = 1000.0f / CalAnimation;



	if (count > SpeedSkill.MaxCount)
	{
		count = SpeedSkill.MaxCount;
	}
	if (this->Animation_Debug == 1) {
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "[%d]Skill %d Ms(%d) %d (%d/%d)", Type, SkillIndex, TimeCheck, (int)CalAnimation, this->AnimationCountSpeed[Number], count);
	}
	if (lpUser->MultiSkillCount == 0)
	{
		this->AnimationCountSpeed[Number]++;
	}
	//fixxxxxxxxxxxxxxxx
	if ((this->AnimationCountSpeed[Number] - count) > 20)
	{
		return 0;
	}


	if (this->AnimationCountSpeed[Number] > count + 1)
	{
		return 0;
	}
	if (SpeedSkill.XDameMinMS != 0)
	{
		if ((int)CalAnimation > SpeedSkill.XDameMinMS && TimeCheck == 0)
		{
			return 0;
		}
	}
	//////////////////////////////////////////////////
	if (TimeCheck == 0 && SpeedSkill.LimitSpeed > 0)
	{
		return 0;
	}

	if (TimeCheck < (int)CalAnimation && TimeCheck != 0)
	{
		return 0;
	}


	return 1;
	//========================================

}

BOOL CBProtect::AntiAutoSkill(int bIndex)
{

	//============ Auto Skill Target
	if (OBJECT_RANGE(bIndex) != 0 && gObj[bIndex].Type == OBJECT_USER)
	{
		if (gMap[gObj[bIndex].Map].CheckAttr(gObj[bIndex].X, gObj[bIndex].Y, 1) != 0)
		{
			return 0;
		}
	}
	return 1;
	//=============================
}

//====NEW TEST
void CBProtect::BQuetDupe(int aIndex)
{
	if (gObj[aIndex].Type != OBJECT_USER || gObj[aIndex].m_OfflineMode != 0) {
		return;
	}
	bool CheckNameOK = true;
	//=== Check Ten Nhan Vat (Hack Disk, Clone Dupe)
	int count = strlen(gObj[aIndex].Name);

	if (count > 10)
	{
		GCCloseClientSend(aIndex, 2); //Kick Khoi Game
		return;
	}
	for (int i = 0; i < count; ++i) {

		if (gObj[aIndex].Name[i] < 97 && gObj[aIndex].Name[i] > 122 && gObj[aIndex].Name[i] < 65 && gObj[aIndex].Name[i] > 90 && gObj[aIndex].Name[i] < 48 && gObj[aIndex].Name[i] > 57)
		{
			GCCloseClientSend(aIndex, 2); //Kick Khoi Game
			return;
		}
	}
	//====Scan
	if (gObj[aIndex].Transaction == 1 && gObj[aIndex].Interface.type == 0)
	{
		gObjInventoryRollback(aIndex);
	}
}

bool CBProtect::CheckUserMove(LPOBJ lpObj)
{
	// Kiểm tra trạng thái không hợp lệ
	if (
		lpObj->Interface.use != 0 ||                       // Đang dùng interface khác
		lpObj->Interface.type == INTERFACE_CHAOS_BOX ||
		lpObj->Interface.type == INTERFACE_TRADE ||
		lpObj->Interface.type == INTERFACE_WAREHOUSE ||
		lpObj->Interface.type == INTERFACE_PERSONAL_SHOP ||
		lpObj->Interface.type == INTERFACE_CASH_SHOP ||
		lpObj->PShopOpen != 0 ||                          // Đang mở shop cá nhân
		lpObj->ChaosLock != 0 ||                          // Đang khóa chaos
		lpObj->DieRegen != 0 ||                           // Đang chết
		lpObj->Teleport != 0 ||                           // Đang dịch chuyển
		lpObj->SkillSummonPartyTime != 0                  // Đang sử dụng kỹ năng triệu hồi
		)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Vui lòng đóng tất cả cửa sổ đang mở.");
		return false;
	}

	return true;
}
#endif



//------------------------------------------------