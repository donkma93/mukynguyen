#include "Stdafx.h"
#include "NewUIRanking.h"
#include "StructSendGs.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
#include "CustomReadGuildClient.h"
#include <GuildCache.h>

Ranking gRanking;
int lastSentTab = -1;
static MARK_t TempRankingGuildMark;

SEASON3B::CNewUIRanking::CNewUIRanking()
{
	m_pNewUIMng			= nullptr;
	m_Pos.x = m_Pos.y	= 0;
	m_iNumCurOpenTab	= 0;
	this->Page			= 1;
}

SEASON3B::CNewUIRanking::~CNewUIRanking()
{
	Release();
}

bool SEASON3B::CNewUIRanking::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_RANKING, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIRanking::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIRanking::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

void SEASON3B::CNewUIRanking::InitButtons()
{
	m_TabBtn.CreateRadioGroup(7, IMAGE_WINDOW_TAB_BTN, TRUE);
	m_TabBtn.ChangeRadioButtonInfo(true, m_Pos.x + 30.f, m_Pos.y + 38.f, 56, 22);
	m_TabBtn.ChangeFrame(m_iNumCurOpenTab);	
}

void SEASON3B::CNewUIRanking::LoadImages()
{
	/*LoadBitmap("Interface\\ACuoi\\newui_item_form_480_top.tga", IMAGE_BASE_WINDOW_TOP, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_form_480_bottom.tga", IMAGE_BASE_WINDOW_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_form_80_top.tga", IMAGE_BASE_WINDOW_TOP_80, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_form_80_bottom.tga", IMAGE_BASE_WINDOW_BOTTOM_80, GL_LINEAR);
	LoadBitmap("Interface\\ACuoi\\newui_item_bank_back.jpg", IMAGE_BASE_WINDOW_BACK, GL_LINEAR);*/
}

void SEASON3B::CNewUIRanking::UnloadImages()
{
	//DeleteBitmap(IMAGE_BASE_WINDOW_TOP);
	//DeleteBitmap(IMAGE_BASE_WINDOW_BOTTOM);
	//DeleteBitmap(IMAGE_BASE_WINDOW_TOP_80);
	//DeleteBitmap(IMAGE_BASE_WINDOW_BOTTOM_80);
	//DeleteBitmap(IMAGE_BASE_WINDOW_BACK);
}

bool SEASON3B::CNewUIRanking::Update()
{
	if (IsVisible())
	{
		int iNumCurOpenTab = m_TabBtn.UpdateMouseEvent();

		if (iNumCurOpenTab == RADIOGROUPEVENT_NONE)
			return true;

		m_iNumCurOpenTab = iNumCurOpenTab;
		
	}
	return true;
}

bool SEASON3B::CNewUIRanking::UpdateMouseEvent()
{
	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 40, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_RANKING);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH - 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUIRanking::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_RANKING);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIRanking::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIRanking::GetKeyEventOrder()
{
	return 3.4;
}

void Ranking::GsSendMain(DGCharTopAll* lpMsg)
{
	for (int i = 0; i < MAXTOP; ++i)
	{
		auto& src  = lpMsg->HA[i];
		auto& dest = this->RankingChar[i];

		strncpy(dest.Name,     src.Name, 11);
		strncpy(dest.Is_SvSub, src.Is_SvSub, 9);

		dest.Is_Class             = src.Is_Class;
		dest.Is_cLevel            = src.Is_cLevel;
		dest.Is_ResetCount        = src.Is_ResetCount;
		dest.Is_MasterResetCount  = src.Is_MasterResetCount;
		dest.Is_ResetTime         = src.Is_ResetTime;
		dest.Is_TotalPoint        = src.Is_TotalPoint;
		dest.Is_VipStars          = src.Is_VipStars;
	}
}
void Ranking::RecvInfoTP(DGCharTopTP* lpMsg)
{
	for (int i = 0; i < MAXTOP; ++i)
	{
		auto& src  = lpMsg->HA[i];
		auto& dest = RankingCharTP[i];

		strncpy(dest.Name, src.Name, 11);
		dest.Is_Class   = src.Is_Class;
		dest.Is_WcoinC  = src.Is_WcoinC;
		dest.Is_WcoinP  = src.Is_WcoinP;
		dest.Is_WcoinG  = src.Is_WcoinG;
		dest.Is_WcoinR  = src.Is_WcoinR;
		dest.Is_CoinAtm = src.Is_CoinAtm;
	}
}

void Ranking::RecvInfoTG(DGCharTopTG* lpMsg)
{
	for (int i = 0; i < MAXTOP; ++i)
	{
		auto& src = lpMsg->HA[i];
		auto& dest = RankingCharTG[i];

		strncpy(dest.G_Master, src.G_Master, 11);
		strncpy(dest.G_Name, src.G_Name, 9);
		strncpy(dest.Is_JoinDate, src.Is_JoinDate, 20);

		dest.Is_GLevel = src.Is_GLevel;
		dest.Is_GScore = src.Is_GScore;
		dest.Is_GMem = src.Is_GMem;
		dest.KillBoss = src.KillBoss;
		dest.TotalReset = src.TotalReset;
		dest.TotalPoint = src.TotalPoint;

		memcpy(dest.GuildMark, src.GuildMark, 32); 
	}
}

void Ranking::RecvInfoPK(DGCharTopPK* lpMsg)
{
	for (int i = 0; i < MAXTOP; ++i)
	{
		auto& src  = lpMsg->HA[i];
		auto& dest = RankingCharPK[i];

		strncpy(dest.Name,         src.Name,         11);
		strncpy(dest.Is_JoinDate,  src.Is_JoinDate,  22);

		dest.Is_Class       = src.Is_Class;
		dest.Is_Kills       = src.Is_Kills;
		dest.Is_Deads       = src.Is_Deads;
		dest.Is_OnlineHours = src.Is_OnlineHours;
		dest.Is_Win         = src.Is_Win;
		dest.Is_Lose        = src.Is_Lose;
		dest.Is_Kboss       = src.Is_Kboss;
	}
}

void Ranking::RecvInfoET(DGCharTopET* lpMsg)
{
	for (int i = 0; i < MAXTOP; ++i)
	{
		auto& src  = lpMsg->HA[i];
		auto& dest = RankingCharET[i];

		strncpy(dest.Name, src.Name, 11);
		dest.Is_Class   = src.Is_Class;
		dest.Is_BloodCS = src.Is_BloodCS;
		dest.Is_ChaosCS = src.Is_ChaosCS;
		dest.Is_DevilCS = src.Is_DevilCS;
	}
}

void Ranking::RecvInfoMT(DGCharTopMT* lpMsg)
{
	for (int i = 0; i < MAXTOP; ++i)
	{
		auto& src  = lpMsg->HA[i];
		auto& dest = RankingCharMT[i];

		strncpy(dest.Name, src.Name, 11);
		dest.cLevel = src.cLevel;
		dest.cPoint = src.cPoint;
		dest.cExp   = src.cExp;
	}
}

void Ranking::RecvInfoWQ(DGCharTopWQ* lpMsg)
{
	for (int i = 0; i < MAXTOP; ++i)
	{
		auto& src = lpMsg->tp[i];
		auto& dest = RankingCharWQ[i];

		strncpy(dest.Name, src.Name, 11);
		dest.Start = src.Start;
		dest.Number = src.Number;
		dest.CurQuest = src.CurQuest;
	}
}

void HandleTabChange(int currentTabIndex)
{
	if (currentTabIndex != lastSentTab)
	{
		static const BYTE tabCodes[] = { 0x01, 0x02, 0xF4, 0x05, 0x03, 0x06, 0x07 };
		if (currentTabIndex >= 0 && currentTabIndex < sizeof(tabCodes))
		{
			SendRequestDataSend(0x7A, tabCodes[currentTabIndex]);
			lastSentTab = currentTabIndex;
		}
	}
}
void OnTabChanged(int newTabIndex)
{
	HandleTabChange(newTabIndex);
}

void ConvertGuildMark32To64(BYTE* dest64, const BYTE* src32)
{
	if (!dest64 || !src32)
		return;

	for (int i = 0; i < 32; ++i)
	{
		dest64[i * 2] = (src32[i] >> 4) & 0x0F; // high nibble
		dest64[i * 2 + 1] = src32[i] & 0x0F;        // low nibble
	}
}

void SEASON3B::CNewUIRanking::DrawTabs(int sTapIndex)
{
	const int TabCount = 7;
	const int StartX = 30;
	const int Spacing = 57;

	for (int i = 0; i < TabCount; i++)
	{
		DWORD Color = (sTapIndex == i) ? 0xFF8000FF : 0xC3C3C3FF;
		IsToolKit.ThisFont(m_Pos.x + StartX + (i * Spacing), m_Pos.y + 46, Color, 0, 56, 0, 3, GlobalText[3833 + i]);
	}
}

bool SEASON3B::CNewUIRanking::Render()
{
	int sTapIndex = m_iNumCurOpenTab;
	OnTabChanged(sTapIndex);
	SEASON3B::CNewUIRanking::IsRender();	

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	DrawTabs(sTapIndex);

	DisableAlphaBlend();
	return false;
}

void SEASON3B::CNewUIRanking::IsRender()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT - 20, GlobalText[3781]);

	m_TabBtn.Render();

	g_pUIForm->RenderBack(m_Pos.x + 20, m_Pos.y + 61, int(WINDOW_WIDTH - 60), 20);
	g_pUIForm->RenderBack(m_Pos.x + 20, m_Pos.y + 80, int(WINDOW_WIDTH - 60), 220);

	int sTapIndex = m_iNumCurOpenTab;
	int AddPosY = 65;
	DWORD HoverLine = 0x0080C080;
	int GetWHoverLine = WINDOW_WIDTH - 72;
	int iLineHeight = ((FontHeight / gPosWide.x_fScreenRate_y)) + 1;

	switch(sTapIndex)
	{
		case 0: // TOP ALL
		{
			for (int m = 0; m < 8; m++) 
			{
				IsToolKit.ThisFont(m_Pos.x + 25 + (m * 51), m_Pos.y + AddPosY, 0xFFB400FF, 0, 50, 0, RT3_SORT_CENTER, GlobalText[3900 + m]);
			}
			DWORD Color;
			char NameID[11], TopID[3], TopLevel[10], TopPoint[10], TopTimeReset[255], NameSub[9], TopVip[10];

			for (int i = 0; i < 20; i++)
			{
				int up = i;

				if (gRanking.RankingChar[up].Name[0] == NULL)
					continue;

				memcpy(NameID, gRanking.RankingChar[up].Name, sizeof(NameID) - 1);
				NameID[sizeof(NameID) - 1] = '\0';

				memcpy(NameSub, gRanking.RankingChar[up].Is_SvSub, sizeof(NameSub) - 1);
				NameSub[sizeof(NameSub) - 1] = '\0';

				Color = (i <= 2) ? 0xFF8040FF : HEX_COLOR_WHITE;

				int mYear = 0;
				int mMonth = 0;
				int mDays = 0;
				int hours = 0;
				int minutes = 0;
				int seconds = 0;
				time_t mTime; tm* lpTime = NULL;
				mTime = gRanking.RankingChar[up].Is_ResetTime;
				lpTime = localtime(&mTime);
				if (lpTime)
				{
					mYear = 1952 + lpTime->tm_year;	mMonth = 1 + lpTime->tm_mon; mDays = lpTime->tm_mday; hours = lpTime->tm_hour; minutes = lpTime->tm_min; seconds = lpTime->tm_sec;
				}

				int yOffset = (m_Pos.y + AddPosY + 20) + (i * 10);

				sprintf(TopID, "%02d", up + 1);
				sprintf(TopLevel, "%d/%d/%d", gRanking.RankingChar[up].Is_cLevel, gRanking.RankingChar[up].Is_ResetCount, gRanking.RankingChar[up].Is_MasterResetCount);
				sprintf(TopPoint, "%s", IsToolKit.QN(gRanking.RankingChar[up].Is_TotalPoint));
				sprintf(TopTimeReset, "%02d:%02d:%02d", hours, minutes, seconds);
				sprintf(TopVip, "%d", gRanking.RankingChar[up].Is_VipStars);

				if (CheckMouseIn(m_Pos.x + 25, yOffset, GetWHoverLine, 9))
				{
					g_pUIForm->RenderHover(m_Pos.x + 25, yOffset, GetWHoverLine, (float)iLineHeight, HoverLine);
				}

				IsToolKit.ThisFont(m_Pos.x + 25,		yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 51,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, NameID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 102,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, CharacterCode(gRanking.RankingChar[up].Is_Class));
				IsToolKit.ThisFont(m_Pos.x + 25 + 153,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopLevel);
				IsToolKit.ThisFont(m_Pos.x + 25 + 204,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopPoint);
				IsToolKit.ThisFont(m_Pos.x + 25 + 255,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, NameSub);
				IsToolKit.ThisFont(m_Pos.x + 25 + 307,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopTimeReset);
				IsToolKit.ThisFont(m_Pos.x + 25 + 359,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopVip);
			}
		}
		break;

		case 1: // TOP COIN
		{
			for (int i = 0; i < 8; i++)
			{
				IsToolKit.ThisFont(m_Pos.x + 25 + (i * 51), m_Pos.y + AddPosY, 0xFFB400FF, 0, 50, 0, RT3_SORT_CENTER, GlobalText[3908 + i]);
			}

			DWORD Color;
			char NameID[11], TopID[3],TopWC[12], TopWP[12], TopWG[12], TopWR[12], TopAm[12];
			for (int i = 0; i < 20; i++)
			{
				int up = i;
				if (gRanking.RankingCharTP[up].Name[0] == NULL)
					continue;

				memcpy(NameID, gRanking.RankingCharTP[up].Name, sizeof(NameID) - 1);
				NameID[sizeof(NameID) - 1] = '\0';

				Color = (i <= 2) ? 0xFF8040FF : HEX_COLOR_WHITE;
				int yOffset = (m_Pos.y + AddPosY + 20) + (i * 10);

				sprintf(TopID, "%02d", up + 1);
				sprintf(TopWC, "%s", IsToolKit.QN(gRanking.RankingCharTP[up].Is_WcoinC));
				sprintf(TopWP, "%s", IsToolKit.QN(gRanking.RankingCharTP[up].Is_WcoinP));
				sprintf(TopWG, "%s", IsToolKit.QN(gRanking.RankingCharTP[up].Is_WcoinG));
				sprintf(TopWR, "%s", IsToolKit.QN(gRanking.RankingCharTP[up].Is_WcoinR));
				sprintf(TopAm, "%s", IsToolKit.QN(gRanking.RankingCharTP[up].Is_CoinAtm));

				if (CheckMouseIn(m_Pos.x + 25, yOffset, GetWHoverLine, 9))
				{
					g_pUIForm->RenderHover(m_Pos.x + 25, yOffset, GetWHoverLine, (float)iLineHeight, HoverLine);
				}

				IsToolKit.ThisFont(m_Pos.x + 25, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 51, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, NameID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 102, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, CharacterCode(gRanking.RankingCharTP[up].Is_Class));
				IsToolKit.ThisFont(m_Pos.x + 25 + 153, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopWC);
				IsToolKit.ThisFont(m_Pos.x + 25 + 204, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopWP);
				IsToolKit.ThisFont(m_Pos.x + 25 + 255, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopWG);
				IsToolKit.ThisFont(m_Pos.x + 25 + 307, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopWR);
				IsToolKit.ThisFont(m_Pos.x + 25 + 359, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopAm);
			}
		}
		break;

		case 2: // TOP GUILD
		{
			for (int i = 0; i < 8; i++)
			{
				IsToolKit.ThisFont(m_Pos.x + 25 + (i * 51), m_Pos.y + AddPosY, 0xFFB400FF, 0, 50, 0, RT3_SORT_CENTER, GlobalText[3916 + i]);
			}
			DWORD Color;
			char Guild_Name[11];
			char Guild_Master[11];
			char TopID[3], TopGLvl[12], TopScr[12], TopGMem[12], TopGRs[12];
			for (int i = 0; i < 20; i++)
			{
				int up = i;
				if (gRanking.RankingCharTG[up].G_Master[0] == NULL)
					continue;

				CHARACTER* c = &CharactersClient[i];
				memcpy(Guild_Name, gRanking.RankingCharTG[up].G_Name, sizeof(Guild_Name) - 1);
				Guild_Name[sizeof(Guild_Name) - 1] = '\0';

				memcpy(Guild_Master, gRanking.RankingCharTG[up].G_Master, sizeof(Guild_Master) - 1);
				Guild_Master[sizeof(Guild_Master) - 1] = '\0';

				Color = (i <= 2) ? 0xFF8040FF : HEX_COLOR_WHITE;
				int yOffset = (m_Pos.y + AddPosY + 20) + (i * 10);

				char OnMyDate[20];
				char ReversedDateTime[20];
				memcpy(OnMyDate, gRanking.RankingCharTG[up].Is_JoinDate, sizeof(OnMyDate) - 1);
				OnMyDate[sizeof(OnMyDate) - 1] = '\0';
				sprintf(ReversedDateTime, "%c%c-%c%c-%c%c%c%c",
					OnMyDate[8], OnMyDate[9],
					OnMyDate[5], OnMyDate[6],
					OnMyDate[0], OnMyDate[1],
					OnMyDate[2], OnMyDate[3]);

				sprintf(TopID, "%02d", up + 1);
				sprintf(TopGLvl, "%s", IsToolKit.QN(gRanking.RankingCharTG[up].Is_GLevel));
				sprintf(TopScr, "%s", IsToolKit.QN(gRanking.RankingCharTG[up].Is_GScore));
				sprintf(TopGMem, "%s", IsToolKit.QN(gRanking.RankingCharTG[up].Is_GMem));
				sprintf(TopGRs, "%s", IsToolKit.QN(gRanking.RankingCharTG[up].TotalReset));

				if (CheckMouseIn(m_Pos.x + 25, yOffset, GetWHoverLine, 9))
				{
					g_pUIForm->RenderHover(m_Pos.x + 25, yOffset, GetWHoverLine, (float)iLineHeight, HoverLine);
				}

				IsToolKit.ThisFont(m_Pos.x + 25,		yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 51,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Guild_Name);
				IsToolKit.ThisFont(m_Pos.x + 25 + 102,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopGLvl);
				IsToolKit.ThisFont(m_Pos.x + 25 + 153,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Guild_Master);
				IsToolKit.ThisFont(m_Pos.x + 25 + 204,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopScr);
				IsToolKit.ThisFont(m_Pos.x + 25 + 255,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopGMem);
				IsToolKit.ThisFont(m_Pos.x + 25 + 307,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, ReversedDateTime);
				IsToolKit.ThisFont(m_Pos.x + 25 + 359,	yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopGRs);

				int TempGuildMarkIndex = MAX_MARKS - 1;
				memset(GuildMark[TempGuildMarkIndex].Mark, 0, sizeof(GuildMark[TempGuildMarkIndex].Mark));
				ConvertGuildMark32To64(GuildMark[TempGuildMarkIndex].Mark, gRanking.RankingCharTG[up].GuildMark);
				GuildMark[TempGuildMarkIndex].Key = 9900 + up;

				CreateGuildMark(TempGuildMarkIndex);
				RenderBitmap(BITMAP_GUILD, (float)m_Pos.x + 30, (float)yOffset - 0.5f, 8.f, 8.f);
			}
		}
		break;

		case 3: //TOP KILLL
		{
			for (int i = 0; i < 8; i++)
			{
				IsToolKit.ThisFont(m_Pos.x + 25 + (i * 51), m_Pos.y + AddPosY, 0xFFB400FF, 0, 50, 0, RT3_SORT_CENTER, GlobalText[3924 + i]);
			}

			DWORD Color;
			char NameID[11], TopID[3], Is_Kboss[10], Is_KillDead[10], Is_Duel[10], Is_OnlineHours[255], OnMyDate[20];
			for (int i = 0; i < 20; i++)
			{
				int up = i;

				if (gRanking.RankingCharPK[up].Name[0] == NULL)
					continue;

				memcpy(NameID, gRanking.RankingCharPK[up].Name, sizeof(NameID) - 1);
				NameID[sizeof(NameID) - 1] = '\0';
				
				Color = (i <= 2) ? 0xFF8040FF : HEX_COLOR_WHITE;
				int yOffset = (m_Pos.y + AddPosY + 20) + (i * 10);

				char ReversedDateTime[20];
				memcpy(OnMyDate, gRanking.RankingCharPK[up].Is_JoinDate, sizeof(OnMyDate) - 1);
				OnMyDate[sizeof(OnMyDate) - 1] = '\0';
				sprintf(ReversedDateTime, "%c%c-%c%c-%c%c%c%c",
					OnMyDate[8], OnMyDate[9],
					OnMyDate[5], OnMyDate[6],
					OnMyDate[0], OnMyDate[1],
					OnMyDate[2], OnMyDate[3]);

				sprintf(TopID, "%02d", up + 1);
				sprintf(Is_Kboss, "%s", IsToolKit.QN(gRanking.RankingCharPK[up].Is_Kboss));
				sprintf(Is_KillDead, "%s/%s", IsToolKit.QN(gRanking.RankingCharPK[up].Is_Kills), IsToolKit.QN(gRanking.RankingCharPK[up].Is_Deads));
				sprintf(Is_Duel, "%s/%s", IsToolKit.QN(gRanking.RankingCharPK[up].Is_Win), IsToolKit.QN(gRanking.RankingCharPK[up].Is_Lose));
				sprintf(Is_OnlineHours, "%s %s", IsToolKit.QN(gRanking.RankingCharPK[up].Is_OnlineHours), GlobalText[1279]);

				if (CheckMouseIn(m_Pos.x + 25, yOffset, GetWHoverLine, 9))
				{
					g_pUIForm->RenderHover(m_Pos.x + 25, yOffset, GetWHoverLine, (float)iLineHeight, HoverLine);
				}

				IsToolKit.ThisFont(m_Pos.x + 25, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 51, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, NameID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 102, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, CharacterCode(gRanking.RankingCharPK[up].Is_Class));
				IsToolKit.ThisFont(m_Pos.x + 25 + 153, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Is_Kboss);
				IsToolKit.ThisFont(m_Pos.x + 25 + 204, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Is_KillDead);
				IsToolKit.ThisFont(m_Pos.x + 25 + 255, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Is_Duel);
				IsToolKit.ThisFont(m_Pos.x + 25 + 307, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Is_OnlineHours);
				IsToolKit.ThisFont(m_Pos.x + 25 + 359, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, ReversedDateTime);

			}
		}
		break;

		case 4: //TOP EVENT
		{
			for (int i = 0; i < 8; i++)
			{
				IsToolKit.ThisFont(m_Pos.x + 25 + (i * 51), m_Pos.y + AddPosY, 0xFFB400FF, 0, 50, 0, RT3_SORT_CENTER, GlobalText[3932 + i]);
			}
			DWORD Color;
			char NameID[11], TopID[3], Is_BloodCS[10], Is_ChaosCS[10], Is_DevilCS[10];
			for (int i = 0; i < 20; i++)
			{
				int up = i;

				if (gRanking.RankingCharET[up].Name[0] == NULL)
					continue;

				memcpy(NameID, gRanking.RankingCharET[up].Name, sizeof(NameID) - 1);
				NameID[sizeof(NameID) - 1] = '\0';

				Color = (i <= 2) ? 0xFF8040FF : HEX_COLOR_WHITE;
				int yOffset = (m_Pos.y + AddPosY + 20) + (i * 10);

				sprintf(TopID, "%02d", up + 1);
				sprintf(Is_BloodCS, "%s", IsToolKit.QN(gRanking.RankingCharET[up].Is_BloodCS));
				sprintf(Is_ChaosCS, "%s", IsToolKit.QN(gRanking.RankingCharET[up].Is_ChaosCS));
				sprintf(Is_DevilCS, "%s", IsToolKit.QN(gRanking.RankingCharET[up].Is_DevilCS));

				if (CheckMouseIn(m_Pos.x + 25, yOffset, GetWHoverLine, 9))
				{
					g_pUIForm->RenderHover(m_Pos.x + 25, yOffset, GetWHoverLine, (float)iLineHeight, HoverLine);
				}

				IsToolKit.ThisFont(m_Pos.x + 25, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 51, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, NameID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 102, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, CharacterCode(gRanking.RankingCharET[up].Is_Class));
				IsToolKit.ThisFont(m_Pos.x + 25 + 153, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Is_BloodCS);
				IsToolKit.ThisFont(m_Pos.x + 25 + 204, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Is_ChaosCS);
				IsToolKit.ThisFont(m_Pos.x + 25 + 255, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Is_DevilCS);
				IsToolKit.ThisFont(m_Pos.x + 25 + 307, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, "0");
				IsToolKit.ThisFont(m_Pos.x + 25 + 359, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, "0");
			}
		}
		break;

		case 5: //TOP MASTER
		{
			for (int i = 0; i < 5; i++)
			{
				IsToolKit.ThisFont(m_Pos.x + 25 + (i * 51), m_Pos.y + AddPosY, 0xFFB400FF, 0, 50, 0, RT3_SORT_CENTER, GlobalText[3940 + i]);
			}
			DWORD Color;
			char NameID[11], TopID[3], cLevel[10], cPoint[10], cExp[20];
			for (int i = 0; i < 20; i++)
			{
				int up = i;

				if (gRanking.RankingCharMT[up].Name[0] == NULL)
					continue;

				memcpy(NameID, gRanking.RankingCharMT[up].Name, sizeof(NameID) - 1);
				NameID[sizeof(NameID) - 1] = '\0';

				Color = (i <= 2) ? 0xFF8040FF : HEX_COLOR_WHITE;
				int yOffset = (m_Pos.y + AddPosY + 20) + (i * 10);

				sprintf(TopID, "%02d", up + 1);
				sprintf(cLevel, "%s", IsToolKit.QN(gRanking.RankingCharMT[up].cLevel));
				sprintf(cPoint, "%s", IsToolKit.QN(gRanking.RankingCharMT[up].cPoint));
				sprintf(cExp, "%s", IsToolKit.QN(gRanking.RankingCharMT[up].cExp));

				if (CheckMouseIn(m_Pos.x + 25, yOffset, GetWHoverLine - 200, 9))
				{
					g_pUIForm->RenderHover(m_Pos.x + 25, yOffset, GetWHoverLine - 200, (float)iLineHeight, HoverLine);
				}

				IsToolKit.ThisFont(m_Pos.x + 25, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 51, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, NameID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 102, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, cLevel);
				IsToolKit.ThisFont(m_Pos.x + 25 + 153, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, cPoint);

				int CurrentExp = gRanking.RankingCharMT[up].cExp;
				float ratio = static_cast<float>(CurrentExp) / 2147483647.0f;

				if (ratio > 1.0f) ratio = 1.0f;
				if (ratio < 0.0f) ratio = 0.0f;

				float iExp = ratio * 45.0f;
				int percent = static_cast<int>(ratio * 100.0f);

				RenderBitmap(SEASON3B::CNewUIPartyInfoWindow::IMAGE_UI_BARHP, m_Pos.x + 25 + 204, (float)yOffset - 6.8f, 48, 21, 0.f, 0.f, 1.0, 64.f / 256.f, 1, 1, 0);
				RenderImage(SEASON3B::CNewUIPartyInfoWindow::IMAGE_PARTY_HPBAR, m_Pos.x + 26 + 204, (float)yOffset + 2.f, iExp, 3);

				IsToolKit.ThisFont(m_Pos.x + 25 + 204, yOffset, Color, 0, 48, 0, RT3_SORT_CENTER, "%d%%", percent);

			}
		}
		break;

		case 6: //TOP QUEST
		{
			for (int i = 0; i < 6; i++)
			{
				IsToolKit.ThisFont(m_Pos.x + 25 + (i * 51), m_Pos.y + AddPosY, 0xFFB400FF, 0, 50, 0, RT3_SORT_CENTER, GlobalText[3948 + i]);
			}

			DWORD Color;
			char TopID[3], NameID[11], Start[20], Number[12], CurQuest[12];

			for (int i = 0; i < MAXTOP; i++)
			{
				if (gRanking.RankingCharWQ[i].Name[0] == 0)
					continue;

				int yOffset = (m_Pos.y + AddPosY + 20) + (i * 10);
				Color = (i <= 2) ? 0xFF8040FF : HEX_COLOR_WHITE;

				sprintf(TopID, "%02d", i + 1);
				sprintf(NameID, "%s", gRanking.RankingCharWQ[i].Name);
				sprintf(Number, "%d", gRanking.RankingCharWQ[i].Number);
				sprintf(CurQuest, "%d", gRanking.RankingCharWQ[i].CurQuest);

				if (gRanking.RankingCharWQ[i].Start == 1)
					strcpy(Start, GlobalText[1847]);
				else
					strcpy(Start, GlobalText[1846]);

				if (CheckMouseIn(m_Pos.x + 25, yOffset, GetWHoverLine, 9))
				{
					g_pUIForm->RenderHover(m_Pos.x + 25, yOffset, GetWHoverLine, (float)iLineHeight, HoverLine);
				}

				IsToolKit.ThisFont(m_Pos.x + 25, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, TopID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 51, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, NameID);
				IsToolKit.ThisFont(m_Pos.x + 25 + 102, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, "---");
				IsToolKit.ThisFont(m_Pos.x + 25 + 153, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Start);
				IsToolKit.ThisFont(m_Pos.x + 25 + 204, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, Number);
				IsToolKit.ThisFont(m_Pos.x + 25 + 255, yOffset, Color, 0, 50, 0, RT3_SORT_CENTER, CurQuest);
			}
		}
		break;
	}

	DisableAlphaBlend();
}

