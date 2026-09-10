#include "Stdafx.h"
#if WinQuestTurn
#include "NewUIQQ.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
#include <MapManager.h>
#include <Util.h>

ExWinQuestSystem g_ExWinQuestSystem;

POINT DrawQuest;
POINT CurPositionQ;
bool  DragItemQ;
DWORD ActionTimeQ;

SEASON3B::CNewUIQQ::CNewUIQQ()
{
    this->m_pNewUIMng = nullptr;
    this->m_Pos = { 0, 0 };
	IsToolKit.ViewIndex = 0;
	g_ExWinQuestSystem.CurrentQuestIndex = 0;
	ZeroMemory(&g_ExWinQuestSystem.ewdata, sizeof(g_ExWinQuestSystem.ewdata));
	g_ExWinQuestSystem.Finish = true;
}

SEASON3B::CNewUIQQ::~CNewUIQQ()
{
	Release();
}

bool SEASON3B::CNewUIQQ::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_WINQUEST, this);

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIQQ::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[0], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 200, m_Pos.y + 190, 52, 26, 1, 0, 1, 1u, "", "", 0);
	g_pUIForm->SetButtonInfo(&m_Btn[3], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 147, m_Pos.y + 190, 52, 26, 1, 0, 1, 1u, "", "", 0);

	m_Btn[1].ChangeButtonImgState(true, CNewUINPCDialogue::IMAGE_ND_BTN_L);
	m_Btn[1].ChangeButtonInfo(m_Pos.x + 130, m_Pos.y + 163, 17, 18);
	m_Btn[2].ChangeButtonImgState(true, CNewUINPCDialogue::IMAGE_ND_BTN_R);
	m_Btn[2].ChangeButtonInfo(m_Pos.x + 233, m_Pos.y + 163, 17, 18);
}

bool SEASON3B::CNewUIQQ::BtnProcess()
{
	int a = g_ExWinQuestSystem.OpenTab;

	if (m_Btn[0].UpdateMouseEvent())
	{
		if (g_ExWinQuestSystem.Finish != 0)
		{
			g_ExWinQuestSystem.CG_AcceptQuest();
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_WINQUEST);
			PlayBuffer(SOUND_CLICK01);
		}
		else
		{
			int a = g_ExWinQuestSystem.OpenTab;

			bool notEnoughLevel = (CharacterAttribute->Level < g_ExWinQuestSystem.ewdata[a].NeedLevel);
			bool notEnoughReset = (CharacterAttribute->ViewReset < g_ExWinQuestSystem.ewdata[a].NeedReset);
			bool notEnoughGrand = (CharacterAttribute->ViewMReset < g_ExWinQuestSystem.ewdata[a].NeedGrand);

			if (notEnoughLevel || notEnoughReset || notEnoughGrand)
			{
				g_pChatListBox->AddText("", GlobalText[3221], SEASON3B::TYPE_SYSTEM_MESSAGE);
			}
			else
			{
				g_pChatListBox->AddText("", GlobalText[3216], SEASON3B::TYPE_SYSTEM_MESSAGE);
			}

			return false;
		}

		return true;
	}


	if (m_Btn[1].UpdateMouseEvent()) // Prev
	{
		if (g_ExWinQuestSystem.ewdata[a].Start)
		{
			g_pChatListBox->AddText("", GlobalText[3217], SEASON3B::TYPE_SYSTEM_MESSAGE);
			return false;
		}

		if (g_ExWinQuestSystem.CurrentQuestIndex > 0)
		{
			g_ExWinQuestSystem.CurrentQuestIndex--;
			g_ExWinQuestSystem.QuestChange(g_ExWinQuestSystem.CurrentQuestIndex);

			char szText[64];
			wsprintf(szText, GlobalText[3218], g_ExWinQuestSystem.CurrentQuestIndex + 1);
			g_pChatListBox->AddText("", szText, SEASON3B::TYPE_SYSTEM_MESSAGE);
		}

		if (g_ExWinQuestSystem.CurrentQuestIndex <= 0) m_Btn[1].Lock(); else m_Btn[1].UnLock();
		m_Btn[2].UnLock();

		return true;
	}

	if (m_Btn[2].UpdateMouseEvent()) // Next
	{
		if (g_ExWinQuestSystem.ewdata[a].Start)
		{
			g_pChatListBox->AddText("", GlobalText[3217], SEASON3B::TYPE_SYSTEM_MESSAGE);
			return false;
		}

		int max = g_ExWinQuestSystem.ewdata[a].ExWQuestCurrent;
		if (g_ExWinQuestSystem.CurrentQuestIndex < (max - 1))
		{
			g_ExWinQuestSystem.CurrentQuestIndex++;
			g_ExWinQuestSystem.QuestChange(g_ExWinQuestSystem.CurrentQuestIndex);

			char szText[64];
			wsprintf(szText, GlobalText[3218], g_ExWinQuestSystem.CurrentQuestIndex + 1);
			g_pChatListBox->AddText("", szText, SEASON3B::TYPE_SYSTEM_MESSAGE);
		}

		if (g_ExWinQuestSystem.CurrentQuestIndex >= (max - 1)) m_Btn[2].Lock(); else m_Btn[2].UnLock();
		m_Btn[1].UnLock();

		return true;
	}

	if (g_ExWinQuestSystem.ewdata[a].Start && !g_ExWinQuestSystem.ewdata[a].b_done)
	{
		if (m_Btn[3].UpdateMouseEvent())
		{
			g_ExWinQuestSystem.QuestChange(111);
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_WINQUEST);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
	}

	return false;
}

void SEASON3B::CNewUIQQ::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIQQ::SetPos(int x, int y)
{
	m_Pos.x = x + gPosWide.x_GetAddPos;
	m_Pos.y = y + 73;

	DrawQuest.x = IsToolKit.GetPositionScreen() - 121;
	DrawQuest.y = 0;
	CurPositionQ.x = MouseX;
	CurPositionQ.y = MouseY;
	DragItemQ = false;
}

bool SEASON3B::CNewUIQQ::Update()
{
	return true;
}

bool SEASON3B::CNewUIQQ::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if (SEASON3B::IsRelease(VK_LBUTTON))
	{
		if (CheckMouseIn(m_Pos.x + 252, m_Pos.y + 7, 16, 16))
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_WINQUEST);
			return false;
		}

		if (!CheckMouseIn(m_Pos.x - 10, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT - 45))
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_WINQUEST);
			return false;
		}
	}

	if (CheckMouseIn(m_Pos.x - 10, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT - 45))
		return false;

	return true;
}

bool SEASON3B::CNewUIQQ::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_WINQUEST);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIQQ::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIQQ::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIQQ::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK, (m_Pos.x + 1), m_Pos.y + 20, float(WINDOW_WIDTH - 3), float(WINDOW_HEIGHT - 65));
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, float(WINDOW_WIDTH), 64.f);
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_LEFT, m_Pos.x, m_Pos.y + 64.f, 21.f, 116.f);
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_RIGHT, m_Pos.x + float(WINDOW_WIDTH - 31.f) + 10, m_Pos.y + 64.f, 21.f, 116.f);
	RenderImage(CNewUIJewelBank::IMAGE_BASE_WINDOW_BOTTOM, m_Pos.x, m_Pos.y + 180, float(WINDOW_WIDTH), 45.f);

	int iLineHeight = ((FontHeight / gPosWide.x_fScreenRate_y)) + 2;

	g_pRenderText->SetBgColor(NULL);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 10, GlobalText[3373], 270, 0, RT3_SORT_CENTER);

	g_pUIForm->RenderBack(m_Pos.x + 13, m_Pos.y + 40, 110, 60);

	g_pRenderText->SetBgColor(100, 0, 0, 255);
	g_pRenderText->RenderText(m_Pos.x + 16, m_Pos.y + 42, GlobalText[3203], 102, iLineHeight, RT3_SORT_CENTER);
	
	g_pUIForm->RenderBack(m_Pos.x + 13, m_Pos.y + 100, 110, 90);
	g_pRenderText->RenderText(m_Pos.x + 16, m_Pos.y + 102, GlobalText[3204], 102, iLineHeight, RT3_SORT_CENTER);

	g_pUIForm->RenderBack(m_Pos.x + 122, m_Pos.y + 40, 137, 150);
	
	char GetText[255];
	sprintf(GetText, "%s", g_ExWinQuestSystem.ewdata[g_ExWinQuestSystem.OpenTab].NameQuest);
	g_pRenderText->RenderText(m_Pos.x + 125, m_Pos.y + 42, GetText, 129, iLineHeight, RT3_SORT_CENTER);

	g_ExWinQuestSystem.Draw(m_Pos.x, m_Pos.y);

	m_Btn[0].Render();
	m_Btn[1].Render();
	m_Btn[2].Render();

	int a = g_ExWinQuestSystem.OpenTab;
	if (g_ExWinQuestSystem.ewdata[a].Start && !g_ExWinQuestSystem.ewdata[a].b_done)
	{
		m_Btn[3].Render();
	}

	InitTextButtons();

	DisableAlphaBlend();
	return true;
}


void SEASON3B::CNewUIQQ::InitTextButtons()
{
	int iLineHeight = ((FontHeight / gPosWide.x_fScreenRate_y)) + 2;
	g_pRenderText->SetBgColor(NULL);

	if (!g_ExWinQuestSystem.ewdata[g_ExWinQuestSystem.OpenTab].Start)
	{
		g_pRenderText->SetTextColor(255, 255, 64, 255);
		g_pRenderText->RenderText(m_Pos.x + 200, m_Pos.y + 197, GlobalText[3205], 52, iLineHeight, RT3_SORT_CENTER);
	}
	else
	{
		if (g_ExWinQuestSystem.Finish)
		{
			g_pRenderText->SetTextColor(255, 255, 64, 255);
			g_pRenderText->RenderText(m_Pos.x + 200, m_Pos.y + 197, GlobalText[3206], 52, iLineHeight, RT3_SORT_CENTER);
		}
		else
		{
			g_pRenderText->SetTextColor(64, 255, 64, 255);
			g_pRenderText->RenderText(m_Pos.x + 200, m_Pos.y + 197, GlobalText[3207], 52, iLineHeight, RT3_SORT_CENTER);

			g_pRenderText->SetTextColor(255, 64, 64, 255);
			g_pRenderText->RenderText(m_Pos.x + 147, m_Pos.y + 197, GlobalText[3219], 52, iLineHeight, RT3_SORT_CENTER);
		}
	}
}

void ExWinQuestSystem::QuestChange(int numchange)
{
	QUESTCHANGE_REQ pRequest{};
	pRequest.h.set(0xF3, 0xDA, sizeof(pRequest));
	pRequest.numchange = numchange;
	DataSend((LPBYTE)&pRequest, pRequest.h.size);
}

void ExWinQuestSystem::DrawMiniInfo()
{
	if (GetKeyState(VK_RBUTTON) & 0x8000 && GetTickCount() > gJwBanking.ActionTime + 1000)
	{
		if (IsToolKit.IsWorkZone(DrawQuest.x, DrawQuest.y, 120.f, 93.f))
		{
			if (!DragItemQ)
			{
				DragItemQ = true;
				CurPositionQ.x = MouseX;
				CurPositionQ.y = MouseY;
			}
		}
		if (DragItemQ)
		{
			DrawQuest.x += MouseX - CurPositionQ.x;
			DrawQuest.y += MouseY - CurPositionQ.y;
			CurPositionQ.x = MouseX;
			CurPositionQ.y = MouseY;
		}
	}
	else
	{
		if (DragItemQ)
		{
			DragItemQ = false;
		}
	}

	if (DrawQuest.x < 0)		
		DrawQuest.x = 0;
	if (DrawQuest.y < 0)
		DrawQuest.y = 0;

	float MaxPos = 735.f;

	if (DrawQuest.x > MaxPos)	DrawQuest.x = MaxPos;
	if (DrawQuest.y > 335)		DrawQuest.y = 335;

	float StartX = DrawQuest.x;
	float StartY = DrawQuest.y;
	int mLuck;
	char GetText[255];
	int a = 0;
	{
		if (this->ewdata[a].Start == TRUE)
		{
			if (this->ewdata[a].Start)
			{
				Finish = this->ewdata[a].b_done; // 1
			}
			else
			{
				Finish = this->ewdata[a].b_accept; // 0
			}

			IsToolKit.RenderToolTip(StartX, StartY, 120.f, 93.f);

			int iLineHeight = ((FontHeight / gPosWide.x_fScreenRate_y)) + 4;

			int row = 0;
			for (int m = 0; m < EW_MAX_MISSION; m++)
			{
				if (this->ewdata[a].MissionType[m] == Hunting)
				{
					mLuck = this->ewdata[0].MissionLevel[m];
					if (this->ewdata[a].MissionID[m] == 65535)
					{
						g_pRenderText->SetBgColor(0, 128, 200, 128);
						g_pRenderText->SetTextColor(255, 255, 255, 255);
						g_pRenderText->RenderText(StartX + 10, StartY + 10, GlobalText[3208], 100, iLineHeight, RT3_SORT_CENTER);
						continue;
					}

					if (this->ewdata[a].Count[m] >= this->ewdata[a].MissionCount[m])
					{
						continue;
					}

					if (CheckMouseIn(StartX + 13, (StartY + 20) + (12 * row), 100, 13))
					{
						g_pRenderText->SetBgColor(255, 255, 255, 80);
						g_pRenderText->SetTextColor(255, 255, 255, 255);
						g_pRenderText->RenderText(StartX + 10, (StartY + 19) + (12 * row), " ", 100, iLineHeight, RT3_SORT_CENTER);

						sprintf(GetText, GlobalText[3209], gMapManager.GetMapName(mLuck));
						g_pRenderText->SetBgColor(255, 255, 255, 0);
						g_pRenderText->SetTextColor(255, 255, 64, 255);
						g_pRenderText->RenderText(StartX + 4, StartY + 9 + (12 * 7), GetText, 110, iLineHeight, RT3_SORT_CENTER);
					}

					char* name = getMonsterName(this->ewdata[a].MissionID[m]);
					sprintf(GetText, "%d/%d", this->ewdata[a].Count[m], this->ewdata[a].MissionCount[m]);

					g_pRenderText->SetBgColor(255, 255, 255, 0);
					g_pRenderText->SetTextColor(255, 255, 255, 255);
					g_pRenderText->RenderText(StartX + 13, (StartY + 20) + (12 * row), name, 100, iLineHeight, 1);

					g_pRenderText->SetTextColor(255, 64, 64, 255);
					g_pRenderText->RenderText(StartX + 7, (StartY + 20) + (12 * row), GetText, 100, iLineHeight, 4);

					row++; 
				}
			}

			sprintf(GetText, "%s", g_ExWinQuestSystem.ewdata[g_ExWinQuestSystem.OpenTab].NameQuest);
			g_pRenderText->SetBgColor(94, 8, 16, 255);
			g_pRenderText->SetTextColor(255, 255, 255, 255);

			if (!Finish)
			{
				g_pRenderText->RenderText(StartX + 4, StartY + 6, GetText, 110, iLineHeight, 3);
				//gToolKit.StartBoss = true; 
			}

			if (row == 0)
			{
				g_pRenderText->RenderText(StartX + 4, StartY + 6, GetText, 110, iLineHeight, 3);
				g_pRenderText->SetBgColor(NULL);
				g_pRenderText->SetTextColor(255, 255, 0, 255);
				g_pRenderText->RenderText(StartX + 4, StartY + 20 + (10 * 2), GlobalText[3211], 110, iLineHeight, 3);
				//gToolKit.StartBoss = false;
				//IsToolKit.RenderMixEffect(StartX + 12, StartY + 35, 4, 1);
			}
		}
	}
}

void ExWinQuestSystem::Draw(int x, int y)
{
	int a = this->OpenTab;

	m_IsPos.x = x, m_IsPos.y = y;

	DWORD OnColor;
	
	if (this->ewdata[a].b_End)
	{
		IsToolKit.ThisFont(m_IsPos.x + 125, m_IsPos.y + 82, 0x0B6DDEFF, 0, 129, 0, 3, GlobalText[3212]);
	}

	if (this->ewdata[a].ExWQuestCurrent != 0)
	{
		IsToolKit.ThisFont(m_IsPos.x + 15, m_IsPos.y + 196, 0xFFFFFFFF, 0, 150, 0, 1, "%s %d %s", GlobalText[3210], this->ewdata[a].ExWQuestCurrent, GlobalText[3220]);
		IsToolKit.ThisFont(m_IsPos.x + 15, m_IsPos.y + 196, 0xFFFF00FF, 0, 150, 0, 1, "%s %d", GlobalText[3210], this->ewdata[a].ExWQuestCurrent);
		IsToolKit.ThisFont(m_IsPos.x + 15, m_IsPos.y + 196, 0xFFFFFFFF, 0, 150, 0, 1, "%s", GlobalText[3210]);
	}

	if (this->ewdata[a].NeedLevel > 400 || this->ewdata[a].NeedReset > 400 || this->ewdata[a].NeedGrand > 400)
	{
		IsToolKit.ThisFont(m_IsPos.x + 125, m_IsPos.y + 82, 0x0B6DDEFF, 0, 129, 0, 3, GlobalText[3212]);
	}
	else
	{
		if (this->ewdata[a].Start)
		{
			Finish = this->ewdata[a].b_done;
		}
		else
		{
			Finish = this->ewdata[a].b_accept;
		}

		IsToolKit.ThisFont(m_IsPos.x + 25, m_IsPos.y + 55, 0xFFFFFFFF, 0, 100, 0, 1, GlobalText[3213]);
		IsToolKit.ThisFont(m_IsPos.x + 25, m_IsPos.y + 68, 0xFFFFFFFF, 0, 100, 0, 1, GlobalText[3214]);
		IsToolKit.ThisFont(m_IsPos.x + 25, m_IsPos.y + 81, 0xFFFFFFFF, 0, 100, 0, 1, GlobalText[3215]);

		struct RequirementDisplay
		{
			int needValue;
			int userValue;
			int yOffset;
		};

		RequirementDisplay reqs[] =
		{
			{ this->ewdata[a].NeedLevel,  CharacterAttribute->Level,      55 },
			{ this->ewdata[a].NeedReset,  CharacterAttribute->ViewReset,  68 },
			{ this->ewdata[a].NeedGrand,  CharacterAttribute->ViewMReset, 81 }
		};

		for (const auto& req : reqs)
		{
			DWORD color = (req.userValue < req.needValue) ? 0xFF0000FF : 0x05EB3EFF;
			IsToolKit.ThisFont(m_IsPos.x + 13, m_IsPos.y + req.yOffset, color, 0, 100, 0, 4, "%d", req.needValue);
		}

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			if (this->ewdata[a].MissionType[m] == Hunting)
			{
				if (this->ewdata[a].b_Mission[m])
				{
					OnColor = 0xFFFFFFFF;
				}
				else
				{
					OnColor = 0xFF00FFFF;
				}
				if (this->ewdata[a].MissionType[m] == Hunting)
				{
					if (this->ewdata[a].MissionID[m] == 65535)
					{
						IsToolKit.ThisFont(m_IsPos.x + 40, m_IsPos.y + (10 * m), OnColor, 0, 100, 0, 3, GlobalText[3208]);
					}
					else
					{
						IsToolKit.ThisFont(m_IsPos.x + 22, m_IsPos.y + (13 * m) + 115, 0xFFFFFFFF, 0, 100, 0, 1, "%d - %s", m + 1, getMonsterName(this->ewdata[a].MissionID[m]));
						IsToolKit.ThisFont(m_IsPos.x + 13, m_IsPos.y + (13 * m) + 115, 0xFF3300FF, 0, 100, 0, 4, "%d", this->ewdata[a].MissionCount[m]);
					}
				}
			}
		}

		for (int r = 0; r < EW_MAX_REWARD; r++)
		{
			if (this->ewdata[a].RewardType[r] == ewZen || this->ewdata[a].RewardType[r] == ewFreePoint || this->ewdata[a].RewardType[r] == ewItem || this->ewdata[a].RewardType[r] == ewBonus)
			{
				if (this->ewdata[a].RewardType[r] == ewZen)
				{
					IsToolKit.ThisFont(m_IsPos.x + 128, m_IsPos.y + 55, 0xFFFFFFFF, 0, 100, 0, 1, "Zen :");
					IsToolKit.ThisFont(m_IsPos.x + 150, m_IsPos.y + 55, 0xFFFF00FF, 0, 100, 0, 4, "+%s", IsToolKit.QN(this->ewdata[a].RewardCount[r]));
				}
				else if (this->ewdata[a].RewardType[r] == ewFreePoint)
				{
					IsToolKit.ThisFont(m_IsPos.x + 128, m_IsPos.y + 68, 0xFFFFFFFF, 0, 100, 0, 1, "Point :");
					IsToolKit.ThisFont(m_IsPos.x + 150, m_IsPos.y + 68, 0xFFFF00FF, 0, 100, 0, 4, "+%s", IsToolKit.QN(this->ewdata[a].RewardCount[r]));
				}
				else if (this->ewdata[a].RewardType[r] == ewItem)
				{
					int GetIDMix = SafeGetItem(GET_ITEM(this->ewdata[a].RewardItemType[r], this->ewdata[a].RewardItemIndex[r]));
					//g_ConsoleDebug->Write(3, "LevelItemQuest: %d", this->ewdata[a].RewardItemLevel[r]);
					
					IsToolKit.ThisFont(m_IsPos.x + 128, m_IsPos.y + 94, 0xFFFFFFFF, 0, 100, 0, 1, "Item :");
					char szMessage[128];
					sprintf(szMessage, "%s", BGetItemName(GetIDMix, this->ewdata[a].RewardItemLevel[r] * 8));
					IsToolKit.ThisFont(m_IsPos.x + 150, m_IsPos.y + 94, 0xFFFF00FF, 0, 100, 0, 4, "%s", szMessage);

					g_pNewUISystem->RenderItem3DNew
					(
						m_IsPos.x + 175, m_IsPos.y + 120, 35, 35,
						GetIDMix,
						this->ewdata[a].RewardItemLevel[r] * 8,
						0,
						0,
						false, 1.0f, false
					);
				}
				else if (this->ewdata[a].RewardType[r] == ewBonus)
				{
					IsToolKit.ThisFont(m_IsPos.x + 128, m_IsPos.y + 81, 0xFFFFFFFF, 0, 100, 0, 1, "WCoinC :");
					IsToolKit.ThisFont(m_IsPos.x + 150, m_IsPos.y + 81, 0xFFFF00FF, 0, 100, 0, 4, "+%s", IsToolKit.QN(this->ewdata[a].RewardCount[r]));
				}
			}
		}
	}
}

void ExWinQuestSystem::GC_RecvMain(GC_MainPacket* aRecv)
{
	int a = 0;
	{
		strncpy(this->ewdata[a].NameQuest, aRecv->exQuest[a].NameQuest, 25);
		this->ewdata[a].b_accept = aRecv->exQuest[a].b_accept;
		this->ewdata[a].b_done = aRecv->exQuest[a].b_done;
		this->ewdata[a].b_End = aRecv->exQuest[a].b_End;
		this->ewdata[a].Start = aRecv->exQuest[a].Start;
		this->ewdata[a].Number = aRecv->exQuest[a].Number;
		this->ewdata[a].ExWQuestCurrent = aRecv->exQuest[a].ExWQuestCurrent;

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			this->ewdata[a].Count[m] = aRecv->exQuest[a].Count[m];
		}
		this->ewdata[a].b_Level = aRecv->exQuest[a].b_Level;
		this->ewdata[a].b_Reset = aRecv->exQuest[a].b_Reset;
		this->ewdata[a].b_Grand = aRecv->exQuest[a].b_Grand;

		this->ewdata[a].NeedLevel = aRecv->exQuest[a].NeedLevel;
		this->ewdata[a].NeedReset = aRecv->exQuest[a].NeedReset;
		this->ewdata[a].NeedGrand = aRecv->exQuest[a].NeedGrand;

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			this->ewdata[a].b_Mission[m] = aRecv->exQuest[a].b_Mission[m];
			this->ewdata[a].MissionType[m] = aRecv->exQuest[a].MissionType[m];
			this->ewdata[a].MissionID[m] = aRecv->exQuest[a].MissionID[m];
			this->ewdata[a].MissionLevel[m] = aRecv->exQuest[a].MissionLevel[m];
			this->ewdata[a].MissionCount[m] = aRecv->exQuest[a].MissionCount[m];
		}
		for (int r = 0; r < EW_MAX_REWARD; r++)
		{
			this->ewdata[a].RewardType[r] = aRecv->exQuest[a].RewardType[r];
			this->ewdata[a].RewardCount[r] = aRecv->exQuest[a].RewardCount[r];
			this->ewdata[a].RewardItemType[r] = aRecv->exQuest[a].RewardItemType[r];
			this->ewdata[a].RewardItemIndex[r] = aRecv->exQuest[a].RewardItemIndex[r];
			this->ewdata[a].RewardItemLevel[r] = aRecv->exQuest[a].RewardItemLevel[r];
		}
	}

	if (aRecv->isOpenWindow)
	{
		if (!g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_WINQUEST))
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_WINQUEST);
		}
	}
}
void ExWinQuestSystem::CG_AcceptQuest()
{
	CG_Accept_Done pReq = { 0 };
	pReq.h.set(0xFF, 0x0D, sizeof(pReq));
	pReq.Result = this->OpenTab;
	DataSend((LPBYTE)&pReq, pReq.h.size);
}

void ExWinQuestSystem::GC_MiniInfoRecv(GC_ExWinMiniInfo* aRecv)
{
	int a = 0;
	{
		strncpy(this->ewdata[a].NameQuest, aRecv->exQuest[a].NameQuest, 25);

		this->ewdata[a].Start = aRecv->exQuest[a].Start;

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			this->ewdata[a].Count[m] = aRecv->exQuest[a].ExWQuestCount[m];
			this->ewdata[a].MissionType[m] = aRecv->exQuest[a].MissionType[m];
			this->ewdata[a].MissionID[m] = aRecv->exQuest[a].MissionID[m];
			this->ewdata[a].MissionLevel[m] = aRecv->exQuest[a].MissionLevel[m];
			this->ewdata[a].MissionCount[m] = aRecv->exQuest[a].MissionCount[m];
		}
	}
}
void ExWinQuestSystem::GC_MiniInfoRecvEx(GC_ExWinMiniInfoEx* aRecv)
{
	int a = aRecv->a;
	int m = aRecv->m;

	if (a < 0 || a >= EW_MAX_ACT)
	{
		return;
	}

	if (m < 0 || m >= EW_MAX_MISSION)
	{
		return;
	}

	this->ewdata[a].Count[m]++;
}
void ExWinQuestSystem::GC_Start(GC_ExWinStart* aRecv)
{
	int a = aRecv->Act;
	int s = aRecv->Start;

	if (a < 0 || a >= EW_MAX_ACT)
	{
		return;
	}

	if (s == true)
	{
		this->ewdata[a].Start = true;
	}
	else
	{
		this->ewdata[a].Start = false;
	}
}
#endif