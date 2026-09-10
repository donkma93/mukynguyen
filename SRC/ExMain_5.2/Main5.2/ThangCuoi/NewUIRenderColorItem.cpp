#include "Stdafx.h"
#include "NewUIRenderColorItem.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"
#include <ZzzLodTerrain.h>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <ZzzEffect.h>

namespace fs = std::filesystem;

bool IsSetExlItem = false;
bool IsSetAncItem = false;
BYTE ReadyToSave = 0;

SEASON3B::CNewUIRenderColorItem::CNewUIRenderColorItem()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;

	for (int a = 0; a < 10; a++)
	{
		this->RenderItem[a] = 0;
		this->fRenderItem[a] = 0;
	}
}

SEASON3B::CNewUIRenderColorItem::~CNewUIRenderColorItem()
{
	Release();
}

bool SEASON3B::CNewUIRenderColorItem::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_RENDER_COLOR, this);

	SetPos(x, y);

	InitButtons();

	Show(false);

	return true;
}

void SEASON3B::CNewUIRenderColorItem::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIRenderColorItem::SetPos(int x, int y)
{
	m_Pos.x = gPosWide.x_GetAddPos;// x + gPosWide.x_GetAddPos;
	m_Pos.y = y + 80;
}

bool SEASON3B::CNewUIRenderColorItem::Update()
{
	return true;
}

bool SEASON3B::CNewUIRenderColorItem::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 89, m_Pos.y + 129, 15, 15))
	{
		IsSetExlItem = !IsSetExlItem;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 89, m_Pos.y + 149, 15, 15))
	{
		IsSetAncItem = !IsSetAncItem;
	}

	POINT ptExitBtn1 = { m_Pos.x + 248, m_Pos.y + 5 };

	if (SEASON3B::IsRelease(VK_LBUTTON))
	{
		if (CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 16, 16))// || !CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 86, WINDOW_HEIGHT + 126))
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_RENDER_COLOR);
		}
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH + 86, WINDOW_HEIGHT + 126))
		return false;
	return true;
}

bool SEASON3B::CNewUIRenderColorItem::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_RENDER_COLOR);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIRenderColorItem::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIRenderColorItem::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIRenderColorItem::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	float ExtForm = 80.0f;
	RenderImage(IMAGE_BASE_WINDOW_BACK,		m_Pos.x + 1, m_Pos.y + 5, 268.f, 186.f + ExtForm);
	RenderImage(IMAGE_BASE_WINDOW_TOP,		m_Pos.x, m_Pos.y, 270.f, 64.f);
	RenderImage(IMAGE_BASE_WINDOW_LEFT,		m_Pos.x, m_Pos.y + 64.f, 21.f, 83.f + ExtForm);
	RenderImage(IMAGE_BASE_WINDOW_RIGHT,	m_Pos.x + 249, m_Pos.y + 64.f, 21.f, 83.f + ExtForm);
	RenderImage(IMAGE_BASE_WINDOW_BOTTOM,	m_Pos.x, m_Pos.y + (147.f + ExtForm), 270.f, 45.f);

	g_pRenderText->SetBgColor(NULL);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 10, "Sửa Màu Item Sever", 270, 0, 3);

	this->RenderItem[3] = (this->RenderItem[0] * MAX_ITEM_INDEX) + this->RenderItem[1];
	this->RenderItem[5] = this->RenderItem[1];

	int iLineHeight = ((FontHeight / gPosWide.x_fScreenRate_y)) + 4;
	int SetPosTextX = 46;

	float GetWToolTip = 67.0f;
	float GetWTextTip = 61.0f;
	float NextForm = 32.0f;

	IsToolKit.RenderToolTipExt(m_Pos.x + 23 + GetWToolTip * 0, m_Pos.y + SetPosTextX + (NextForm * 0), GetWTextTip, 28, iLineHeight, "Section", 1, RenderItem[ITEM_SECTION]);
	IsToolKit.RenderToolTipExt(m_Pos.x + 23 + GetWToolTip * 0, m_Pos.y + SetPosTextX + (NextForm * 1), GetWTextTip, 28, iLineHeight, "Item Index", 1, RenderItem[ITEM_INDEX]);
	IsToolKit.RenderToolTipExt(m_Pos.x + 23 + GetWToolTip * 0, m_Pos.y + SetPosTextX + (NextForm * 2), GetWTextTip, 28, iLineHeight, "Item Lever", 1, RenderItem[ITEM_LEVEL]);
	IsToolKit.RenderToolTipExt(m_Pos.x + 23 + GetWToolTip * 0, m_Pos.y + SetPosTextX + (NextForm * 3), GetWTextTip, 28, iLineHeight, "Mix Index", 1, RenderItem[ITEM_MIXINDEX]);
	IsToolKit.RenderToolTipExt(m_Pos.x + 23 + GetWToolTip * 0, m_Pos.y + SetPosTextX + (NextForm * 4), GetWTextTip, 28, iLineHeight, "Color Set", 1, RenderItem[ITEM_COLORSET]);
	IsToolKit.RenderToolTipExt(m_Pos.x + 23 + GetWToolTip * 0, m_Pos.y + SetPosTextX + (NextForm * 5), GetWTextTip, 28, iLineHeight, "Set Blend", 1, RenderItem[6]);
	IsToolKit.RenderToolTipExt(m_Pos.x + 23 + GetWToolTip * 1, m_Pos.y + SetPosTextX + (NextForm * 5), GetWTextTip, 28, iLineHeight, "Effect", 1, RenderItem[7]);
	IsToolKit.RenderToolTipExt(m_Pos.x + 23 + GetWToolTip * 2, m_Pos.y + SetPosTextX + (NextForm * 5), GetWTextTip, 28, iLineHeight, "Alpha", 2, 0, fRenderItem[0]);

	char szItem[255] = { 0 };
	BYTE GetIndexLevel = RenderItem[ITEM_LEVEL] * 8;
	int level = (GetIndexLevel >> 3) & 15;
	GetItemName(RenderItem[ITEM_MIXINDEX], level, szItem);
	char szMessage[128];
	sprintf(szMessage, "%s", szItem);

	IsToolKit.RenderToolTipExt(m_Pos.x + 90.f, m_Pos.y + 48.f, 150.f, 77.f, iLineHeight, szMessage, 0, 0);

	for (int x = 0; x < 17; x++)
	{
		m_Btn[x].Render();
	}
	
	BYTE CreateExl = 0;
	BYTE CreateAnc = 0;

	if (IsSetExlItem) 
	{
		RenderImage(CNewUIOptionWindow::IMAGE_OPTION_BTN_CHECK, m_Pos.x + 89.0f, m_Pos.y + 129.3, 15, 15, 0, 0);
		CreateExl = 1;
	}
	else 
	{
		RenderImage(CNewUIOptionWindow::IMAGE_OPTION_BTN_CHECK, m_Pos.x + 89.0f, m_Pos.y + 129.3, 15, 15, 0, 15.f);
		CreateExl = 0;
	}

	if (IsSetAncItem)
	{
		RenderImage(CNewUIOptionWindow::IMAGE_OPTION_BTN_CHECK, m_Pos.x + 89.0f, m_Pos.y + 149.3f, 15, 15, 0, 0);
		CreateAnc = 5;
	}
	else 
	{
		RenderImage(CNewUIOptionWindow::IMAGE_OPTION_BTN_CHECK, m_Pos.x + 89.0f, m_Pos.y + 149.3f, 15, 15, 0, 15.f);
		CreateAnc = 0;
	}

	g_pRenderText->SetBgColor(NULL);
	g_pRenderText->SetTextColor(0, 255, 0, 255);
	g_pRenderText->RenderText(m_Pos.x + 106, m_Pos.y + 131, "Item Exc", 150, iLineHeight, 1);

	g_pRenderText->SetTextColor(0, 240, 240, 255);
	g_pRenderText->RenderText(m_Pos.x + 106, m_Pos.y + 151, "Item Thần", 150, iLineHeight, 1);

	if (ReadyToSave == 1)
	{
		g_pRenderText->SetTextColor(255, 255, 0, 255);
		sprintf(szItem, "Saved! Item Color: %s", szMessage);
		g_pRenderText->RenderText(m_Pos.x + 93, m_Pos.y + 115, szItem, 150, iLineHeight, 1);
	}
	else if (ReadyToSave == 2)
	{
		g_pRenderText->SetTextColor(255, 0, 0, 255);
		g_pRenderText->RenderText(m_Pos.x + 93, m_Pos.y + 115, "Select Wing first!", 150, iLineHeight, 1);
	}
	else
	{
		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->RenderText(m_Pos.x + 93, m_Pos.y + 115, "Shift + Click to Fast Add +10 Num", 150, iLineHeight, 1);
	}

	g_pNewUISystem->RenderItem3DNew
	(
		m_Pos.x + 155,
		m_Pos.y + 75,
		30,
		30,
		this->RenderItem[3],
		GetIndexLevel,
		CreateExl,
		CreateAnc,
		false, 1.0, true
	);

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIRenderColorItem::CreateInitButtons(int a, int x, int y, bool Ext)
{
	if (Ext)
	{
		m_Btn[a].ChangeButtonImgState(true, CNewUICharacterInfoWindow::IMAGE_CHAINFO_BTN_STAT_TRU);
		m_Btn[a].ChangeButtonInfo(m_Pos.x + x, m_Pos.y + y, 16, 15);
		m_Btn[a + 1].ChangeButtonImgState(true, CNewUICharacterInfoWindow::IMAGE_CHAINFO_BTN_STAT);
		m_Btn[a + 1].ChangeButtonInfo(m_Pos.x + x + 49, m_Pos.y + y, 16, 15);
	}
	else
	{
		m_Btn[a].ChangeButtonImgState(true, CNewUICharacterInfoWindow::IMAGE_CHAINFO_BTN_STAT_TRU);
		m_Btn[a].ChangeButtonInfo(m_Pos.x + x, m_Pos.y + y, 16, 15);
		m_Btn[a + 1].ChangeButtonImgState(true, CNewUICharacterInfoWindow::IMAGE_CHAINFO_BTN_STAT);
		m_Btn[a + 1].ChangeButtonInfo(m_Pos.x + x + 41, m_Pos.y + y, 16, 15);
	}
}
void SEASON3B::CNewUIRenderColorItem::InitButtons()
{
	int SetNewPos = 59;
	int SetNewSubPos = 32;

	CreateInitButtons(0, 25, SetNewPos + (SetNewSubPos * 0));
	CreateInitButtons(2, 25, SetNewPos + (SetNewSubPos * 1));
	CreateInitButtons(4, 25, SetNewPos + (SetNewSubPos * 2));
	CreateInitButtons(6, 25, SetNewPos + (SetNewSubPos * 4));
	CreateInitButtons(11, 25, SetNewPos + (SetNewSubPos * 5));
	CreateInitButtons(13, 25 + 67 * 1, SetNewPos + (SetNewSubPos * 5));
	CreateInitButtons(15, 25 + 67 * 2, SetNewPos + (SetNewSubPos * 5));

	g_pUIForm->SetButtonInfo(&m_Btn[10], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 90,  m_Pos.y + 180, 52, 26, 1, 0, 1, 1u, "Make", "Create current items", 0);
	g_pUIForm->SetButtonInfo(&m_Btn[8],  CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 144, m_Pos.y + 180, 52, 26, 1, 0, 1, 1u, "Lưu", "Save config", 0);
	g_pUIForm->SetButtonInfo(&m_Btn[9],  CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 198, m_Pos.y + 180, 52, 26, 1, 0, 1, 1u, "Reset", "Reset config", 0);
}

bool SEASON3B::CNewUIRenderColorItem::GetButtonPrev(int Item, int Step, int Min)
{
	if (RenderItem[Item] > Min)
	{
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			if (RenderItem[Item] >= Step)
				RenderItem[Item] -= Step;
			else
				RenderItem[Item] = Min;
		}
		else
		{
			RenderItem[Item]--;
		}
		return true;
	}
	return false;
}

bool SEASON3B::CNewUIRenderColorItem::GetButtonNext(int Item, int Step, int Max)
{
	if (RenderItem[Item] < Max)
	{
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			RenderItem[Item] += Step;
			if (RenderItem[Item] > Max)
				RenderItem[Item] = Max;
		}
		else
		{
			RenderItem[Item]++;
		}
		return true;
	}
	return false;
}

bool SEASON3B::CNewUIRenderColorItem::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonPrev(0, 10, 0))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[1].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonNext(0, 10, 15))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[2].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonPrev(1, 10, 0))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[3].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonNext(1, 10, 512))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[4].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonPrev(2, 10, 0))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[5].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonNext(2, 10, 15))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}
	//
	if (m_Btn[6].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonPrev(4, 10, 0))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[7].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonNext(4, 10, 512))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}
	//
	
	if (m_Btn[8].UpdateMouseEvent())
	{
		for (int x = 0; x < 7; x++)
		{
			if (RenderItem[0] == x)
			{
				ReadyToSave = 1;
				SaveRenderColorToFile(x);
			}
		}

		if (RenderItem[0] >= 7 && RenderItem[0] <= 11)
		{
			ReadyToSave = 1;
			SaveRenderColorToFile(7);
		}

		PlayBuffer(SOUND_CLICK01);
		
		return true;
	}

	if (m_Btn[9].UpdateMouseEvent())
	{
		ReadyToSave = 0;

		for (int a = 0; a < 7; a++)
		{
			RenderItem[a] = 0;
		}

		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[10].UpdateMouseEvent())
	{
		char szCmd[32];

		for (int x = 0; x < 7; x++)
		{
			if (RenderItem[0] == x)
			{
				sprintf(szCmd, "/make %d %d %d 0 0 0 %d %d", RenderItem[0], RenderItem[1], RenderItem[2], IsSetExlItem * 63, IsSetAncItem);
				SendChat(szCmd);
			}
		}

		if (RenderItem[0] >= 7 && RenderItem[0] <= 11)
		{
			sprintf(szCmd, "/makeset %d %d 0 0 0 %d %d", RenderItem[1], RenderItem[2], IsSetExlItem * 63, IsSetAncItem);
			SendChat(szCmd);
		}

		if (RenderItem[0] == 12 || RenderItem[0] == 13)
		{
			sprintf(szCmd, "/make %d %d %d 0 0 0 %d %d", RenderItem[0], RenderItem[1], RenderItem[2], IsSetExlItem * 63, IsSetAncItem);
			SendChat(szCmd);
		}

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (m_Btn[11].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonPrev(6, 10, -10))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[12].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonNext(6, 10, 10))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}
	//
	if (m_Btn[13].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonPrev(7, 10, 0))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (m_Btn[14].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (GetButtonNext(7, 10, 255))
			PlayBuffer(SOUND_CLICK01);
		return true;
	}


	if (m_Btn[15].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (fRenderItem[0] > -10.0f)
		{
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				fRenderItem[0] -= 1.0f;
			}
			else
			{
				fRenderItem[0] -= 0.10f;
			}

			if (fRenderItem[0] < -10.0f)
				fRenderItem[0] = -10.0f;

			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	if (m_Btn[16].UpdateMouseEvent())
	{
		ReadyToSave = 0;
		if (fRenderItem[0] < 100.0f)
		{
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				fRenderItem[0] += 1.0f;
			}
			else
			{
				fRenderItem[0] += 0.10f;
			}

			if (fRenderItem[0] > 100.0f)
				fRenderItem[0] = 100.0f;

			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}
}


void SEASON3B::CNewUIRenderColorItem::SaveRenderColorToFile(BYTE a)
{
	std::string folder = "Data/ACuoi/ColorRender";
	std::string path;

	if (!fs::exists(folder))
	{
		fs::create_directory(folder);
	}
	
	if (a == 0)
	{
		path = folder + "/ITEM_SWORD.txt";
	}
	else if (a == 1)
	{
		path = folder + "/ITEM_AXE.txt";
	}
	else if (a == 2)
	{
		path = folder + "/ITEM_MACE.txt";
	}
	else if (a == 3)
	{
		path = folder + "/ITEM_SPEAR.txt";
	}
	else if (a == 4)
	{
		path = folder + "/ITEM_BOW.txt";
	}
	else if (a == 5)
	{
		path = folder + "/ITEM_STAFF.txt";
	}
	else if (a == 6)
	{
		path = folder + "/ITEM_SHIELD.txt";
	}
	else if (a >= 7 && a <= 11)
	{
		path = folder + "/ITEM_SET.txt";
	}
	else if (a == 12)
	{
		path = folder + "/ITEM_WING.txt";
	}
	else if (a == 13)
	{
		path = folder + "/ITEM_HELPER.txt";
	}
	else
	{
		return;
	}

	std::ifstream infile(path);
	std::stringstream buffer;
	std::string line;
	int currentAaa = RenderItem[5];
	int currentBb = RenderItem[4];
	bool found = false;

	while (std::getline(infile, line))
	{
		if (line.find("case " + std::to_string(currentAaa) + ":") != std::string::npos)
		{
			buffer << "case " << currentAaa << ": Color = " << currentBb << "; break;\n";
			found = true;
		}
		else
		{
			buffer << line << "\n";
		}
	}
	infile.close();

	if (!found)
	{
		buffer << "case " << currentAaa << ": Color = " << currentBb << "; break;\n";
	}

	std::ofstream outfile(path, std::ios::trunc);
	outfile << buffer.str();
	outfile.close();
}

bool SEASON3B::CNewUIRenderColorItem::IsRenderWeapon(int Type)
{
	return 	
		Type == MODEL_SWORD + 51
		|| Type == MODEL_SWORD + 61
		|| Type == MODEL_SWORD + 83

		|| Type == MODEL_MACE + 25

		|| Type == MODEL_BOW + 30

		|| Type == MODEL_STAFF + 50
		;
}

static const std::unordered_map<int, int> BlendMeshMap =
{
	{ MODEL_SWORD + 51, -2 },
	{ MODEL_SWORD + 61, 2 },
	{ MODEL_SWORD + 83, 2 },


	{ MODEL_MACE + 25, 3 },


	{ MODEL_BOW + 30, 2 },


	{ MODEL_STAFF + 50, 0 },
};

void SEASON3B::CNewUIRenderColorItem::RenderSetBlendEffect(OBJECT* o)
{
	auto it = BlendMeshMap.find(o->Type);
	int GetSection = MODEL_ITEM + (RenderItem[ITEM_SECTION] * MAX_ITEM_INDEX);

	if (it != BlendMeshMap.end())
	{
		o->BlendMesh = it->second;
	}
	#if RENDER_ITEM_3D
	//else if (o->Type >= MODEL_SWORD && o->Type <= MODEL_ETC + 512 && !IsRenderWeapon(o->Type))
	else if (o->Type >= GetSection && o->Type <= GetSection + RenderItem[ITEM_INDEX])// && !IsRenderWeapon(o->Type))
	{
		o->BlendMesh = this->RenderItem[6];
	}
	#endif
}

static const std::unordered_map<int, int> RenderEffectMap =
{
	{ MODEL_SWORD + 51, 0 },
	{ MODEL_SWORD + 61, 0 },
	{ MODEL_SWORD + 83, 3 },

	{ MODEL_MACE + 25, 0 },

	{ MODEL_BOW + 30, 2 },

	{ MODEL_STAFF + 50, 107 },
};

void SEASON3B::CNewUIRenderColorItem::RenderCEfect(BMD* b, OBJECT* o, int Type, float Alpha, int RenderType)
{
	int Key = -1;

	auto it = RenderEffectMap.find(o->Type);
	if (it != RenderEffectMap.end())
	{
		Key = it->second;
	}
	#if RENDER_ITEM_3D
	else if (IsRenderWeapon(o->Type))
	{
		Key = this->RenderItem[7];
	}
	#endif
	//ACuoi Render Item
	switch (Key)
	{
		case 107:
		{
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			o->BlendMeshLight = absf((sinf(WorldTime * 0.001f)));
			o->BlendMesh = 2;
			b->RenderMesh(2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE, 1.f, 2, o->BlendMeshLight, -WorldTime * 0.0005f, o->BlendMeshTexCoordV);
		}
		break;
		case 0:
		{
			b->BeginRender(1.f);
			glColor4f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2], 0.8f);
			b->RenderMesh(1, RENDER_TEXTURE, 0.1f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			glColor4f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2], 0.5f);
			b->RenderMesh(3, RENDER_TEXTURE, 0.1f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			glColor3f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2]);
			b->RenderMesh(0, RENDER_TEXTURE, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE, 1.f, 2, o->BlendMeshLight, -WorldTime * 0.0005f, o->BlendMeshTexCoordV);
			b->EndRender();
		}
		break;

		case 1:
		{
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			o->BlendMesh = 2;
			o->BlendMeshLight = absf((sinf(WorldTime * 0.001f)));
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 2:
		{
		}
		break;

		case 3:
		{
			vec3_t Light;
			VectorCopy(b->BodyLight, Light);
			Vector(0.85f * Light[0], 0.85f * Light[1], 1.2f * Light[2], b->BodyLight);
			VectorCopy(Light, b->BodyLight);
		}
		break;

		case 5:
		{
			Vector(0.8f, 0.6f, 1.f, b->BodyLight);
			Vector(1.f, 1.f, 1.f, b->BodyLight);
		}
		break;

		case 7:
		{
			Vector(1.f, 0.7f, 0.5f, b->BodyLight);
			glColor3fv(b->BodyLight);
			b->RenderMesh(2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

			static float s_iTexAni = 0;
			s_iTexAni += FPS_ANIMATION_FACTOR;
			if (s_iTexAni > 15)
				s_iTexAni = 0;
			float fU = ((int)s_iTexAni / 4) * 0.25f;
			Vector(0.9f, 0.6f, 0.3f, b->BodyLight);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, fU, o->BlendMeshTexCoordV, o->HiddenMesh);
			Vector(1.f, 1.f, 1.f, b->BodyLight);
		}
		break;

		case 8:
		{
			Vector(1.f, 1.f, 1.f, b->BodyLight);
			glColor3fv(b->BodyLight);
			b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			float Luminosity = absf(sinf(WorldTime * 0.001f)) * 0.3f;
			Vector(0.1f + Luminosity, 0.1f + Luminosity, 0.1f + Luminosity, b->BodyLight);
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Luminosity = absf(sinf(WorldTime * 0.001f)) * 0.8f;
			Vector(0.0f + Luminosity, 0.0f + Luminosity, 0.0f + Luminosity, b->BodyLight);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_3RDWING_LAYER);
		}
		break;

		case 9:
		{
			Vector(1.f, 1.f, 1.f, b->BodyLight);
			glColor3fv(b->BodyLight);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME6, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 10:
		{
			Vector(1.f, 1.f, 1.f, b->BodyLight);
			glColor3fv(b->BodyLight);
			b->RenderMesh(1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 11:
		{
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_METAL, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_LIGHTMAP | RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, WorldTime * 0.0001f, BITMAP_CHROME);
			b->RenderMesh(1, RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 12:
		{
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_METAL, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_LIGHTMAP | RENDER_TEXTURE, 1.f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -WorldTime * 0.0001f, BITMAP_CHROME);
			b->RenderMesh(1, RENDER_TEXTURE, Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 13:
		{
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_METAL, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_LIGHTMAP | RENDER_TEXTURE, 1.f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -WorldTime * 0.0001f, BITMAP_CHROME);
			b->RenderMesh(1, RENDER_TEXTURE, Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 14:
		{
			b->RenderMesh(0, RENDER_TEXTURE, Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_LIGHTMAP | RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -WorldTime * 0.0001f, BITMAP_CHROME);
		}
		break;

		case 15:
		{
			vec3_t Light;
			VectorCopy(b->BodyLight, Light);
			Vector(0.8f * Light[0], 0.f, 0.8f * Light[2], b->BodyLight);
			b->RenderMesh(0, RENDER_LIGHTMAP | RENDER_TEXTURE, 1.f, 0, o->BlendMeshLight, -WorldTime * 0.0002f, o->BlendMeshTexCoordV, BITMAP_CHROME);
			VectorCopy(Light, b->BodyLight);
			b->RenderMesh(0, RENDER_CHROME | RENDER_BRIGHT, 1.f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME + 1);
			VectorCopy(Light, b->BodyLight);
		}
		break;

		case 16:
		{
			b->RenderMesh(1, RENDER_DARK | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, -WorldTime * 0.0002f, o->BlendMeshTexCoordV, BITMAP_CHROME);

			vec3_t vPos, vRelativePos;
			Vector(0.f, 0.f, 0.f, vRelativePos);
			b->TransformPosition(BoneTransform[8], vRelativePos, vPos, true);
			float fLumi = (sinf(WorldTime * 0.004f) + 1.0f) * 0.05f;
			Vector(0.8f + fLumi, 0.8f + fLumi, 0.3f + fLumi, o->Light);
			CreateSprite(BITMAP_LIGHT, vPos, 1.5f, o->Light, o, 0.5f);
			b->TransformPosition(BoneTransform[10], vRelativePos, vPos, true);
			CreateSprite(BITMAP_LIGHT, vPos, 0.5f, o->Light, o, 0.5f);
			b->TransformPosition(BoneTransform[11], vRelativePos, vPos, true);
			CreateSprite(BITMAP_LIGHT, vPos, 0.5f, o->Light, o, 0.5f);
		}
		break;

		case 17:
		{
		}
		break;

		case 18:
		{
			vec3_t Light;
			VectorCopy(b->BodyLight, Light);
			b->BeginRender(1.f);
			glColor3f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2]);
			b->RenderMesh(3, RENDER_TEXTURE, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE, sinf(WorldTime * 0.01f), 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_CHROME | RENDER_TEXTURE, 1.f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, WorldTime * 0.001f, BITMAP_CHROME);

			float Luminosity = sinf(WorldTime * 0.001f) * 0.5f + 0.5f;
			Vector(Light[0] * Luminosity, Light[0] * Luminosity, Light[0] * Luminosity, b->BodyLight);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, 1.f, 2, o->BlendMeshLight, WorldTime * 0.0001f, -WorldTime * 0.0005f);
			b->EndRender();
		}
		break;

		case 19:
		{
			b->RenderMesh(1, RENDER_TEXTURE, 1.f, 1, o->BlendMeshLight, WorldTime * 0.0001f, WorldTime * 0.0005f);
		}
		break;

		case 20:
		{
			vec3_t Light;
			VectorCopy(b->BodyLight, Light);
			float time = WorldTime * 0.001f;
			float Luminosity = sinf(time) * 0.5f + 0.3f;
			Vector(Light[0] * Luminosity, Light[0] * Luminosity, Light[0] * Luminosity, b->BodyLight);
			b->RenderMesh(2, RENDER_TEXTURE, 1.f, 2, o->BlendMeshLight, time, -WorldTime * 0.0005f);
		}
		break;

		case 21:
		{
		}
		break;

		case 22:
		{
			Vector(0.3f, 0.8f, 1.f, b->BodyLight);
			b->RenderMesh(0, RENDER_BRIGHT | RENDER_CHROME, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 23:
		{
			o->BlendMeshLight = sinf(WorldTime * 0.001f) * 0.6f + 0.4f;
			b->BeginRender(1.f);
			b->RenderMesh(0, RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->EndRender();
		}
		break;

		case 24:
		{
			Vector(0.1f, 0.3f, 1.f, b->BodyLight);
			o->BlendMesh = 0;
			o->BlendMeshLight = sinf(WorldTime * 0.001f) * 0.6f + 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.6f, 0.8f, 1.f, b->BodyLight);
			o->BlendMesh = 1;
			o->BlendMeshLight = 1.f;
			b->RenderMesh(1, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, WorldTime * 0.0003f, BITMAP_CHROME);
		}
		break;

		case 25:
		{
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			o->BlendMesh = 1;
			Vector(1.f, 0.f, 0.2f, b->BodyLight);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.f, 1.f, 1.f, b->BodyLight);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(4, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			float fV = (((int)(WorldTime * 0.05) % 16) / 4) * 0.25f;
			b->RenderMesh(5, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 5, o->BlendMeshLight, o->BlendMeshTexCoordU, fV);
		}
		break;

		case 26:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 27:
		{
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			o->BlendMesh = 1;
			o->BlendMeshLight = sinf(WorldTime * 0.001f) * 0.6f + 0.4f;
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 28:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 29:
		{
			o->BlendMesh = 1;
			o->BlendMeshLight = 1.f;
			Vector(1.f, 0.5f, 0.5f, b->BodyLight);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 30:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			o->BlendMesh = 0;
			o->BlendMeshLight = fabs(sinf(WorldTime * 0.001f)) + 0.1f;
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SOCKETSTAFF);
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SOCKETSTAFF);
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SOCKETSTAFF);
		}
		break;

		case 31:
		{
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 32:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 33:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 34:
		{
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			o->BlendMesh = 2;
			o->BlendMeshLight = absf(sinf(WorldTime * 0.001f));
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 35:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 36:
		{
			glColor3f(1.f, 1.f, 1.f);
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->StreamMesh = 1;
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -(int)WorldTime % 2000 * 0.0005f);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 37:
		{
		}
		break;

		case 38:
		{
		}
		break;

		case 39:
		{
		}
		break;

		case 40:
		{
		}
		break;

		case 41:
		{
			float fLumi = (sinf(WorldTime * 0.001f) + 1.5f) * 0.25f;
			float PlaySpeed = b->Actions[b->CurrentAction].PlaySpeed;
			b->PlayAnimation(&o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, 2.f / 7.f, o->Position, o->Angle);
			vec3_t Light; vec3_t vRelativePos; vec3_t vWorldPos;
			Vector(0.f, 0.f, 0.f, vRelativePos);
			Vector(1.f, 0.f, 0.0f, Light);
			b->TransformPosition(BoneTransform[1], vRelativePos, vWorldPos, true);
			CreateSprite(BITMAP_LIGHT, vWorldPos, 3.f, Light, o, 0.f);
		}
		break;

		case 42:
		{
		}
		break;

		case 43:
		{
		}
		break;

		case 44:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.5f) * 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ITEM_EFFECT_DBSTONE_R);
		}
		break;

		case 45:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.5f) * 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ITEM_EFFECT_HELLHORN_R);
		}
		break;

		case 46:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.5f) * 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ITEM_EFFECT_PFEATHER_R);
		}
		break;

		case 47:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.5f) * 0.4f;
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ITEM_EFFECT_DEYE_R);
			b->RenderMesh(1, RENDER_CHROME | RENDER_BRIGHT, 0.2f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 48:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.3f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FREETICKET_R);
		}
		break;

		case 49:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHAOSCARD_R);
		}
		break;

		case 50:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM1_R);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 51:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM2_R);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 52:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM3_R);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 53:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM4_R);
		}
		break;

		case 54:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM5_R);
		}
		break;

		case 55:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.5f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_LUCKY_CHARM_EFFECT53);
		}
		break;
		
		case 56:
		{
			b->RenderMesh(0, RENDER_CHROME4, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 57:
		{
		}
		break;

		case 58:
		{
		}
		break;

		case 59:
		{
		}
		break;

		case 60:
		{
		}
		break;

		case 61:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.3f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FREETICKET_R);
		}
		break;

		case 62:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM5_R);
		}
		break;

		case 63:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM7);
		}
		break;

		case 64:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM8);
		}
		break;

		case 65:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM9);
		}
		break;

		case 66:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM10);
		}
		break;

		case 67:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM11);
		}
		break;

		case 68:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.f) * 0.5f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM12);
		}
		break;

		case 69:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.3f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DOPPLEGANGGER_FREETICKET);
		}
		break;

		case 70:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.3f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BARCA_FREETICKET);
		}
		break;

		case 71:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.3f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BARCA7TH_FREETICKET);
		}
		break;

		case 72:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHARACTERCARD_R);
		}
		break;

		case 73:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_NEWCHAOSCARD_GOLD_R);
		}
		break;

		case 74:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_NEWCHAOSCARD_RARE_R);
		}
		break;

		case 75:
		{
			float fLumi = (sinf(WorldTime * 0.0015f) + 1.2f) * 0.4f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_NEWCHAOSCARD_MINI_R);
		}
		break;

		case 76:
		{
			glColor3f(1.f, 1.f, 1.f);
			Vector(1.f, 1.f, 1.f, b->BodyLight);
			b->RenderMesh(0, RENDER_TEXTURE, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_CHROME | RENDER_BRIGHT, 0.3f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;
		case 77:
		{
			Vector(1.f, 1.f, 1.f, b->BodyLight);
			b->RenderMesh(1, RENDER_CHROME | RENDER_BRIGHT, Alpha, 1, 0.2f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			glColor3f(1.f, 1.f, 1.f);
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 0, sinf(WorldTime * 0.005f), o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_CHROME4 | RENDER_BRIGHT, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;
		case 78:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 1, 1.f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;
		case 79:
		{
			float fLumi = (sinf(WorldTime * 0.002f) + 0.5f) * 0.5f;
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;
		case 80:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 1, 1.f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 3, 1.f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;
		case 81:
		{
			b->BeginRender(1.f);
			vec3_t Light;
			VectorCopy(b->BodyLight, Light);
			Vector(Light[0] * 0.3f, Light[1] * 0.3f, Light[2] * 0.3f, b->BodyLight);
			glColor3f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2]);
			b->RenderMesh(2, RENDER_COLOR, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			VectorCopy(Light, b->BodyLight);
			glColor3f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2]);
			b->RenderMesh(2, RENDER_CHROME | RENDER_BRIGHT, 1.f, 2, o->BlendMeshLight, o->BlendMeshTexCoordU, WorldTime * 0.01f, BITMAP_CHROME);
			b->RenderMesh(0, RENDER_TEXTURE, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE, 1.f, 1, o->BlendMeshLight, (float)(rand() % 10) * 0.1f, (float)(rand() % 10) * 0.1f);
			float Luminosity = sinf(WorldTime * 0.001f) * 0.4f + 0.6f;
			Vector(Light[0] * Luminosity, Light[0] * Luminosity, Light[0] * Luminosity, b->BodyLight);
			glColor3f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2]);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, 1.f, 2, o->BlendMeshLight, WorldTime * 0.0001f, WorldTime * 0.0005f);
			b->EndRender();
		}
		break;
		case 82:
		{
			b->BeginRender(1.f);
			glColor4f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2], 0.8f);
			b->RenderMesh(1, RENDER_TEXTURE, 0.8f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			glColor4f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2], 0.5f);
			b->RenderMesh(3, RENDER_TEXTURE, 0.5f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			glColor3f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2]);
			b->RenderMesh(0, RENDER_TEXTURE, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE, 1.f, 2, o->BlendMeshLight, WorldTime * 0.0005f, o->BlendMeshTexCoordV);
			b->RenderMesh(3, RENDER_TEXTURE, 1.f, 3, o->BlendMeshLight, (float)(rand() % 10) * 0.1f, (float)(rand() % 10) * 0.1f);
			b->EndRender();
		}
		break;

		case 83:
		{
			
		}
		break;
		case 84:
		{
			Vector(0.75f, 0.55f, 0.5f, b->BodyLight);
			b->RenderMesh(0, RENDER_CHROME4 | RENDER_BRIGHT, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.f, 1.f, 1.f, b->BodyLight);
			b->RenderMesh(0, RENDER_CHROME | RENDER_TEXTURE | RENDER_BRIGHT, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 85:
		{
			b->BeginRender(1.f);
			if (o->HiddenMesh == 1)
			{
				glColor3f(1.f, 1.f, 1.f);
				Vector(1.f, 1.f, 1.f, b->BodyLight);
				b->RenderMesh(0, RENDER_TEXTURE, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				Vector(0.1f, 0.5f, 1.f, b->BodyLight);
				b->RenderMesh(0, RENDER_METAL | RENDER_BRIGHT, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(0, RENDER_CHROME | RENDER_BRIGHT, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			else if (o->HiddenMesh == 0)
			{
				glColor3f(1.f, 1.f, 1.f);
				Vector(1.f, 1.f, 1.f, b->BodyLight);
				b->RenderMesh(1, RENDER_TEXTURE, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				Vector(0.1f, 0.5f, 1.f, b->BodyLight);
				b->RenderMesh(1, RENDER_METAL | RENDER_BRIGHT, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(1, RENDER_CHROME | RENDER_BRIGHT, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			b->EndRender();
		}
		break;
		case 86:
		{
			b->BeginRender(1.f);
			if (o->HiddenMesh == 1)
			{
				glColor3f(1.f, 1.f, 1.f);
				Vector(1.f, 1.f, 1.f, b->BodyLight);
				b->RenderMesh(0, RENDER_TEXTURE, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			else if (o->HiddenMesh == 0)
			{
				glColor3f(1.f, 1.f, 1.f);
				Vector(1.f, 1.f, 1.f, b->BodyLight);
				b->RenderMesh(1, RENDER_TEXTURE, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			b->EndRender();
		}
		break;

		case 87:
		{
			float Luminosity = sinf(WorldTime * 0.0008f) * 0.7f + 0.5f;
			b->RenderMesh(2, RENDER_TEXTURE, Alpha, 2, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE, Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			glColor3f(0.43f, 0.14f, 0.6f);
			b->RenderMesh(3, RENDER_BRIGHT | RENDER_CHROME, Alpha, 3, o->BlendMeshLight, WorldTime * 0.0001f, WorldTime * 0.0005f);
			glColor3f(1.f, 1.f, 1.f);
		}
		break;

		case 88:
		{
			b->RenderMesh(0, RENDER_TEXTURE, 1.f, 0, o->BlendMeshLight, WorldTime * 0.0005f, WorldTime * 0.0005f);
			o->HiddenMesh = 0;
			b->StreamMesh = 1;
			b->RenderMesh(1, RENDER_TEXTURE, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, WorldTime * 0.0005f);
		}
		break;

		case 89:
		{
			float Luminosity = sinf(WorldTime * 0.002f) * 0.3f + 0.5f;
			vec3_t Light;
			VectorCopy(b->BodyLight, Light);
			Vector(Light[0] * 0.3f, Light[1] * 0.8f, Light[1] * 1.f, b->BodyLight);
			VectorCopy(Light, b->BodyLight);
		}
		break;

		case 90:
		{
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordU);
			float Luminosity = sinf(WorldTime * 0.0015f) * 0.03f + 0.3f;
			Vector(Luminosity, Luminosity, Luminosity + 0.1f, b->BodyLight);
			b->RenderMesh(2, RENDER_TEXTURE, o->Alpha, -2, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordU);
			o->Alpha = 0.5f;
			float Luminosity4 = sinf(WorldTime * 0.0025f) * 0.5f + 0.7f;
			Vector(0.4f, 0.4f, 0.8f, b->BodyLight);
			b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, -2, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordU);
		}
		break;

		case 91:
		{
			if (b->HideSkin == true)
			{
				b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
		}
		break;

		case 92:
		{
			if (b->HideSkin == true)
			{
				b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
		}
		break;

		case 93:
		{
			o->HiddenMesh = 1;
			b->RenderMesh(1, RENDER_TEXTURE, 0.5f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_LAVA);
			b->RenderMesh(1, RENDER_TEXTURE, 0.7f, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, WorldTime * 0.0009f);
		}
		break;

		case 94:
		{
		}
		break;

		case 95:
		{
			o->HiddenMesh = 1;
			b->RenderMesh(1, RENDER_TEXTURE, 1.0f, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME5, 0.5f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 96:
		{
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 1, o->BlendMeshLight, WorldTime * 0.0009f, WorldTime * 0.0009f);
		}
		break;

		case 97:
		{
			b->BeginRender(1.f);
			glColor3f(1.f, 1.f, 1.f);
			Vector(1.f, 1.f, 1.f, b->BodyLight);
			b->RenderMesh(0, RENDER_TEXTURE, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.f, 0.5f, 1.f, b->BodyLight);
			b->RenderMesh(1, RENDER_CHROME | RENDER_BRIGHT, 1.f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->EndRender();
		}
		break;
		case 98:
		{
			
		}
		break;
		case 99:
		{
			vec3_t EndRelative, EndPos;
			Vector(0.f, 0.f, 0.f, EndRelative);
			b->TransformPosition(o->BoneTransform[0], EndRelative, EndPos, true);
			Vector(0.4f, 0.6f, 0.8f, o->Light);
			CreateSprite(BITMAP_LIGHT, EndPos, 6.0f, o->Light, o, 0.5f);
			float Luminosity = sinf(WorldTime * 0.05f) * 0.4f + 0.9f;
			Vector(Luminosity * 0.3f, Luminosity * 0.5f, Luminosity * 0.8f, o->Light);
			CreateSprite(BITMAP_LIGHT, EndPos, 2.0f, o->Light, o);

			vec3_t Light;
			VectorCopy(b->BodyLight, Light);
			Vector(0.9f, 0.7f, 1.0f, b->BodyLight);
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FENRIR_THUNDER);
			b->RenderMesh(0, RENDER_BRIGHT | RENDER_METAL, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FENRIR_THUNDER);
			VectorCopy(Light, b->BodyLight);
		}
		break;

		case 100:
		{
			b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			float Luminosity = (sinf(WorldTime * 0.003f) + 1) * 0.3f + 0.3f;
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		break;
		case 101:
		{
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		break;
		case 102:
		{
			b->RenderMesh(0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;
		case 103:
		{
			b->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		break;
		case 104:
		{
			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_BRIGHT | RENDER_CHROME4, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			float Luminosity = (sinf(WorldTime * 0.003f) + 1) * 0.3f + 0.6f;
			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		break;

		case 105:
		{
			b->BeginRender(1.f);
			glColor4f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2], 0.8f);
			b->RenderMesh(1, RENDER_TEXTURE, 0.8f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			glColor4f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2], 0.5f);
			b->RenderMesh(3, RENDER_TEXTURE, 0.5f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			glColor3f(b->BodyLight[0], b->BodyLight[1], b->BodyLight[2]);
			b->RenderMesh(0, RENDER_TEXTURE, 1.f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(2, RENDER_TEXTURE, 1.f, 2, o->BlendMeshLight, WorldTime * 0.0005f, o->BlendMeshTexCoordV);
			b->EndRender();

			b->RenderMesh(0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(0, RENDER_BRIGHT | RENDER_CHROME4, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			float Luminosity = (sinf(WorldTime * 0.003f) + 1) * 0.3f + 0.6f;

			b->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Luminosity = sinf(WorldTime * 0.0008f) * 0.7f + 0.5f;
			b->RenderMesh(2, RENDER_TEXTURE, Alpha, 2, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(1, RENDER_TEXTURE, Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			glColor3f(0.43f, 0.14f, 0.6f);
			b->RenderMesh(3, RENDER_BRIGHT | RENDER_CHROME, Alpha, 3, o->BlendMeshLight, WorldTime * 0.0001f, WorldTime * 0.0005f);
			glColor3f(1.f, 1.f, 1.f);
		}
		break;
	}
}
