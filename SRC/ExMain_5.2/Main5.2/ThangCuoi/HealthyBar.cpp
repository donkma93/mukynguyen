#include "Stdafx.h"
#include "StructSendGs.h"
#include "UIControls.h"
#include "ZzzInventory.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ACuoi_DanhHieu.h"
#include "ACuoi_QuanHam.h"
#include "ACuoi_TuLuyen.h"
#include "ACuoi_HonHoan.h"
#include "NewUIPartyInfoWindow.h"
#include "MapManager.h"
#include "NewUIMessageBox.h"
#include "UIGuildInfo.h"
#include "ZzzToolKit.h"
#include "NewUIAccountLevel.h"
#include <ZzzLodTerrain.h>

RankUserClass gRank;
ClassLifeBar gClassLifeBar;

inline float PosDanhHieuY()
{
	float a = 0;
	switch (m_Resolution)
	{
		case 0: a = 63; break;
		case 1: a = 63; break;
		case 2: a = 62; break;
		case 3: a = 52; break;
		case 4: a = 49; break;
		case 5: a = 43; break;
		case 6: a = 43; break;
		case 7: a = 39; break;
		case 8: a = 39; break;
		case 9: a = 39; break;
		case 10: a = 39; break;
	}
	return a;
}
inline float PosQuanHamY()
{
	float a = 0;
	switch (m_Resolution)
	{
	case 0: a = 63; break;
	case 1: a = 63; break;
	case 2: a = 62; break;
	case 3: a = 52; break;
	case 4: a = 49; break;
	case 5: a = 43; break;
	case 6: a = 43; break;
	case 7: a = 39; break;
	case 8: a = 39; break;
	case 9: a = 39; break;
	case 10: a = 39; break;
	}
	return a;
}
inline float PosTuLuyenY()
{
	float a = 0;
	switch (m_Resolution)
	{
	case 0: a = 63; break;
	case 1: a = 63; break;
	case 2: a = 62; break;
	case 3: a = 52; break;
	case 4: a = 49; break;
	case 5: a = 43; break;
	case 6: a = 43; break;
	case 7: a = 39; break;
	case 8: a = 39; break;
	case 9: a = 39; break;
	case 10: a = 39; break;
	}
	return a;
}
void RankUserClass::RenderShowLogRank(int index, int PosX, int PosY)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int GetRankLevel = 0;
	int GetRankLevel1 = 0;
	int GetRankLevel2 = 0;
	int GetAccountLevel = 0;

	if (g_bRenderImageLogo)
	{
		GetRankLevel = gRank.m_Data[index].rDanhHieu;
		GetRankLevel1 = gRank.m_Data[index].rQuanHam;
		GetRankLevel2 = gRank.m_Data[index].rTuLuyen;
		//ACuoi Chỉnh Rank F8
		if (GetRankLevel >= 1 && GetRankLevel <= 50)
		{
			RenderBitmap((SEASON3B::CNewUIDanhHieu::IMAGE_HD_LOGO_START + GetRankLevel) - 1, PosX - 67, PosY - PosDanhHieuY() - 6, 135, 30, 0.f, 0.f, 1.0, 64.f / 64.f, 1, 1, 0);
		}
		if (GetRankLevel1 >= 1 && GetRankLevel1 <= 50)
		{
			RenderBitmap((SEASON3B::CNewUIQuanHam::IMAGE_HD_LOGO_START2 + GetRankLevel1) - 1, PosX - 26, PosY - PosQuanHamY() - 62, 65, 45, 0.f, 0.f, 1.0, 64.f / 64.f, 1, 1, 0);
		}
		if (GetRankLevel2 >= 1 && GetRankLevel2 <= 50)
		{
			RenderBitmap((SEASON3B::CNewUITuLuyen::IMAGE_HD_LOGO_START3 + GetRankLevel2) - 1, PosX - 59, PosY - PosTuLuyenY() - 35, 115, 30, 0.f, 0.f, 1.0, 64.f / 64.f, 1, 1, 0);
		}

		//// Todo VIP Logo OK
		//GetAccountLevel = gRank.m_Data[index].AccountType;
		//
		//if (GetAccountLevel >= 3)
		//{
		//	RenderBitmap(SEASON3B::CNewUIAccountLevel::IMAGE_IGS_LOGOV1, PosX - 11, PosY - 65, 40, 12, 0.f, 0.f, 1.0, 64.f / 256.f, 1, 1, 0);
		//}
	}
	DisableAlphaBlend();
}

void RankUserClass::RenderBarName()
{
	if (gMapManager.InChaosCastle(gMapManager.WorldActive) == true)
	{
		return;
	}

	for (int j = 0; j < MAX_CHARACTERS_CLIENT; ++j)
	{
		CHARACTER* c = &CharactersClient[j];
		OBJECT* o = &c->Object;

		if (c->Dead > 0.f || !o->Live || o->Kind != KIND_PLAYER)
		{
			continue;
		}

		vec3_t Position;
		int ScreenX, ScreenY;

		Vector(o->Position[0], o->Position[1], o->Position[2] + o->BoundingBoxMax[2] + 100.f, Position);
		Projection(Position, &ScreenX, &ScreenY);
		int AddPosY = 0;

		if (c->GuildMarkIndex >= 0 && GuildMark[c->GuildMarkIndex].UnionName[0])
		{
			AddPosY = 1;
		}
		RenderShowLogRank(c->Key, ScreenX, ScreenY - AddPosY);
	}

	DisableAlphaBlend();
	glColor3f(1.f, 1.f, 1.f);
}

void RankUserClass::GCReqRankLevelUser(const BYTE* SPK)
{
	auto DataSPK = reinterpret_cast<const PMSG_CUSTOM_RANKUSER*>(SPK);
	if (!DataSPK)
	{
		return;
	}

	auto& IsUser		= m_Data[DataSPK->iIndex];

	IsUser.m_Index		= DataSPK->iIndex;
	IsUser.m_Level		= DataSPK->iLevel;
	IsUser.rDanhHieu	= DataSPK->rDanhHieu;
	IsUser.rQuanHam		= DataSPK->rQuanHam;
	IsUser.rTuLuyen		= DataSPK->rTuLuyen;
	IsUser.rHonHoan		= DataSPK->rHonHoan;
	IsUser.AccountType	= DataSPK->AccountType;

	//g_ConsoleDebug->Write(3, "rHonHoan: %d", IsUser.rHonHoan);
	
	for (auto i = 0; i < 5; ++i)
	{
		ReqResetChange[i] = DataSPK->ReqResetChange[i];
		ReqResetUpPoint[i] = DataSPK->ReqResetUpPoint[i];
		ReqResetCoin[i] = DataSPK->ReqResetCoin[i];
	}

	std::memcpy(IsUser.szName, DataSPK->szName, sizeof(IsUser.szName));
}

void ClassLifeBar::Clear()
{
	for (auto& SPK : this->gNewHealthBar)
	{
		SPK.index	= 0xFFFF;
		SPK.type	= 0;
		SPK.rate	= 0;
		SPK.rate2	= 0;
		SPK.Level	= 0;
		SPK.Life	= 0;
		SPK.MonsID	= 0;
	}
}

void ClassLifeBar::Insert(WORD index, BYTE type, BYTE rate, BYTE rate2, short Level, float Life, WORD MonsID)
{
	for (auto& SPK : this->gNewHealthBar)
	{
		if (SPK.index == 0xFFFF)
		{
			SPK.index	= index;
			SPK.type	= type;
			SPK.rate	= rate;
			SPK.rate2	= rate2;
			SPK.Level	= Level;
			SPK.Life	= Life;
			SPK.MonsID	= MonsID;
			return;
		}
	}
}

ClassLifeData* ClassLifeBar::Get(WORD index, BYTE type)
{
	for (auto& SPK : gNewHealthBar)
	{
		if (SPK.index == index && SPK.type == type)
		{
			return &SPK;
		}
	}
	return nullptr;
}

void ClassLifeBar::Receive(const BYTE* lpMsg)
{
	auto DataSPK = reinterpret_cast<const RecvLifeCount*>(lpMsg);
	this->Clear();

	for (auto n = 0; n < DataSPK->count; ++n)
	{
		auto lpInfo = reinterpret_cast<const RecvLifeData*>(
			lpMsg + sizeof(RecvLifeCount) + sizeof(RecvLifeData) * n
			);

		Insert(lpInfo->index, lpInfo->type, lpInfo->rate, lpInfo->rate2, lpInfo->Level, lpInfo->Life, lpInfo->MonsID);
	}
}

void GetHPColorByRate(int rate, float& r, float& g, float& b)
{
	static float blinkTimer = 0.0f;
	blinkTimer += FPS_ANIMATION_FACTOR;

	if (rate > 80)
	{
		r = 0.0f;  g = 0.6f;  b = 0.0f;
	}
	else if (rate > 60)
	{
		r = 0.5f;  g = 0.7f;  b = 0.0f;
	}
	else if (rate > 50)
	{
		r = 1.0f;  g = 1.0f;  b = 0.0f;
	}
	else
	{
		float speed = 0.15f;
		if (rate <= 40) speed = 0.20f;
		if (rate <= 30) speed = 0.25f;
		if (rate < 30) speed = 0.30f;

		float brightness = 0.5f + 0.5f * sinf(blinkTimer * speed);
		r = brightness;
		g = brightness * 0.2f;
		b = brightness * 0.2f;
	}
}


void ClassLifeBar::Render()
{
	const float Width = 38.f;
	char Text[100], RateHP[100];

	if (!g_bRenderNameMonster || SelectedCharacter == -1)
		return;

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	CHARACTER* c = &CharactersClient[SelectedCharacter];
	OBJECT* o = &c->Object;
	vec3_t Position;
	int ScreenX, ScreenY;

	Vector(o->Position[0], o->Position[1], o->Position[2] + o->BoundingBoxMax[2] + 100.f, Position);
	Projection(Position, &ScreenX, &ScreenY);
	ScreenX -= static_cast<int>(Width / 2);

	if (!o->Live || c->Object.Kind == KIND_NPC)
		return;

	ClassLifeData* lpNewHealthBar = Get(c->Key, c->Object.Kind);
	if (lpNewHealthBar == nullptr)
		return;

	int LifePercent = lpNewHealthBar->rate / 1.8;
	int ShieldPercent = lpNewHealthBar->rate2 / 1.8;
	float iHP = static_cast<float>(LifePercent);

	if (c->Dead != 0)
	{
		iHP -= c->Dead;
		if (iHP < 0)
			iHP = 0;
	}

	int ModelMonster = gCustomModelNPC.IsMonster(c->MonsterIndex);
	int MonsterClass = IsToolKit.GetIndexMonster(c->MonsterIndex);

	if (MonsterClass || ModelMonster || c->MonsterIndex == 34 || c->Object.SubType == MODEL_SKELETON1 || c->Object.SubType == MODEL_SKELETON2 || c->Object.SubType == MODEL_SKELETON3 || c->Object.Kind == KIND_MONSTER && c->Object.Type != MODEL_PLAYER && c->Object.Kind != KIND_TRAP)
	{
		char DisplayText[128];
		sprintf(DisplayText, "%s - %d%%", c->ID, lpNewHealthBar->rate);

		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->RenderText(ScreenX - 86, ScreenY - 7, DisplayText, 200, 0, RT3_SORT_CENTER);

		IsToolKit.RenderToolTip(ScreenX - 15.f, ScreenY + 3.f, 55.f, 3.2f);
		EnableAlphaTest();

		float r, g, b;
		GetHPColorByRate(lpNewHealthBar->rate, r, g, b);
		glColor3f(r, g, b);
		RenderColor((ScreenX - 17) + 2, (ScreenY - 6) + 9, iHP, 3);
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else
	{
		if (!c->SafeZone)
		{
			IsToolKit.RenderToolTip(ScreenX - 15.f, ScreenY + 3.f, 55.f, 3.2f);
			RenderImage(SEASON3B::CNewUIPartyInfoWindow::IMAGE_PARTY_HPBAR, (ScreenX - 17) + 2, (ScreenY - 6) + 9, iHP, 3);
			RenderImage(SEASON3B::CNewUIMessageBoxMng::IMAGE_MSGBOX_PROGRESS_BAR, (ScreenX - 17) + 2, (ScreenY - 6) + 9, ShieldPercent, 3);
		}
	}

	DisableAlphaBlend();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


