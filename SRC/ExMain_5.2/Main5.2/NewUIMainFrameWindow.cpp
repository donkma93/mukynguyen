//////////////////////////////////////////////////////////////////////
// NewUIMainFrameWindow.cpp: implementation of the CNewUIMainFrameWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIMainFrameWindow.h"	// self
#include "NewUIOptionWindow.h"
#include "NewUISystem.h"
#include "UIBaseDef.h"
#include "DSPlaySound.h"
#include "ZzzInfomation.h"
#include "ZzzBMD.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzOpenData.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"
#include "CSItemOption.h"
#include "CSChaosCastle.h"
#include "MapManager.h"
#include "CharacterManager.h"
#include "SkillManager.h"
#include "GMDoppelGanger1.h"
#include "GMDoppelGanger2.h"
#include "GMDoppelGanger3.h"
#include "GMDoppelGanger4.h"
#include "./Time/CTimCheck.h"
#include "MonkSystem.h"
#include "ThangCuoi\ZzzToolKit.h"
#include "ThangCuoi\WideData.h"
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
#include "GameShop/InGameShopSystem.h"
#endif //PBG_ADD_INGAMESHOP_UI_MAINFRAME



extern int  MouseUpdateTime;
extern int  MouseUpdateTimeMax;
extern int SelectedCharacter;
extern int Attacking;
#if Ex700
POINT GetPos;

// === skill grid config ===
static const float SKILL_TILE_W = 25.f;//32.f;
static const float SKILL_TILE_H = 28.f;//38.f;

float TransForX(float num)
{
	return (float)(gPosWide.x_fScreenRate_x <= 1.25) ? num * gPosWide.x_fScreenRate_x : num * gPosWide.x_fScreenRate_y;
}

float TransForY(float num)
{
	return (float)num * gPosWide.x_fScreenRate_y;
}

float TransFor2X(float num)
{
	return (float)num * gPosWide.x_fScreenRate_x;
}
#endif

SEASON3B::CNewUIMainFrameWindow::CNewUIMainFrameWindow()
{
	m_bExpEffect = false;
	m_dwExpEffectTime = 0;
	m_dwPreExp = 0;
	m_dwGetExp = 0;
	m_bButtonBlink = false;
#if Ex700
	GetPos.x = 0;
	GetPos.y = 0;
#endif
}

SEASON3B::CNewUIMainFrameWindow::~CNewUIMainFrameWindow()
{
	Release();
}
#if Ex700
void SEASON3B::CNewUIMainFrameWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_menu04.tga", BITMAP_INTERFACE_LEFT_BEGIN, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu02-03.jpg", IMAGE_MENU_2_1, GL_LINEAR);

	LoadBitmap("Interface\\Ex700\\Main_IE.tga", IMAGE_MENU_1, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\guege_hp.tga", IMAGE_GAUGE_RED, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\guege_hp_green.tga", IMAGE_GAUGE_GREEN, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\guege_hp_1.tga", IMAGE_GAUGE_END, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\guege_mp.tga", IMAGE_GAUGE_BLUE, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\guege_mp_1.tga", IMAGE_GAUGE_END_MANA, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\Main_I4.jpg", IMAGE_GAUGE_SD, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\Main_I5.jpg", IMAGE_GAUGE_AG, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\RenderSkillForm.tga", IMAGE_GAUGE_END_FORM, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\StackButton0.tga",	IMAGE_GAUGE_BTN_01, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\StackButton1.tga",	IMAGE_GAUGE_BTN_02, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\StackButton2.tga",	IMAGE_GAUGE_BTN_03, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\StackButton3.tga",	IMAGE_GAUGE_BTN_04, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\StackButton4.tga",	IMAGE_GAUGE_BTN_05, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\StackButton5.tga",	IMAGE_GAUGE_BTN_06, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\MacroUI_Main.tga",	IMAGE_GAUGE_BTN_07, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\MacroUI_Menu.tga",	IMAGE_GAUGE_BTN_08, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\MacroUI_BSetup.tga",	IMAGE_GAUGE_BTN_09, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\MacroUI_BStart.tga",	IMAGE_GAUGE_BTN_10, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\MacroUI_Stop.tga",	IMAGE_GAUGE_BTN_11, GL_LINEAR);
	LoadBitmap("Interface\\Ex700\\Main_IE_Exp.tga",		IMAGE_GAUGE_BTN_12, GL_LINEAR);

	LoadBitmap("Interface\\newui_exbar.jpg", IMAGE_GAUGE_EXBAR, GL_LINEAR);
	LoadBitmap("Interface\\Exbar_Master.jpg", IMAGE_MASTER_GAUGE_BAR, GL_LINEAR);

	LoadBitmap("ACuoi\\newui_menu01A.tga", BITMAP_START_SPK + 114, GL_LINEAR);
	LoadBitmap("ACuoi\\newui_menu02A.tga", BITMAP_START_SPK + 115, GL_LINEAR);
	LoadBitmap("ACuoi\\newui_menu03A.tga", BITMAP_START_SPK + 116, GL_LINEAR);
}
#else
void SEASON3B::CNewUIMainFrameWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_menu01.jpg", IMAGE_MENU_1, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu02.jpg", IMAGE_MENU_2, GL_LINEAR);
	LoadBitmap("Interface\\partCharge1\\newui_menu03.jpg", IMAGE_MENU_3, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu04.tga", BITMAP_INTERFACE_LEFT_BEGIN, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu02-03.jpg", IMAGE_MENU_2_1, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_blue.jpg", IMAGE_GAUGE_BLUE, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_green.jpg", IMAGE_GAUGE_GREEN, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_red.jpg", IMAGE_GAUGE_RED, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_ag.jpg", IMAGE_GAUGE_AG, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_sd.jpg", IMAGE_GAUGE_SD, GL_LINEAR);
	LoadBitmap("Interface\\newui_exbar.jpg", IMAGE_GAUGE_EXBAR, GL_LINEAR);
	LoadBitmap("Interface\\Exbar_Master.jpg", IMAGE_MASTER_GAUGE_BAR, GL_LINEAR);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt05.jpg", IMAGE_MENU_BTN_CSHOP, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt01.jpg", IMAGE_MENU_BTN_CHAINFO, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt02.jpg", IMAGE_MENU_BTN_MYINVEN, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt03.jpg", IMAGE_MENU_BTN_FRIEND, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt04.jpg", IMAGE_MENU_BTN_WINDOW, GL_LINEAR, GL_CLAMP_TO_EDGE);

	LoadBitmap("ACuoi\\newui_menu01A.tga", BITMAP_START_SPK + 114, GL_LINEAR);
	LoadBitmap("ACuoi\\newui_menu02A.tga", BITMAP_START_SPK + 115, GL_LINEAR);
	LoadBitmap("ACuoi\\newui_menu03A.tga", BITMAP_START_SPK + 116, GL_LINEAR);
}
#endif
void SEASON3B::CNewUIMainFrameWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_MENU_1);
	DeleteBitmap(IMAGE_MENU_2);
	DeleteBitmap(IMAGE_MENU_3);
	DeleteBitmap(IMAGE_MENU_4);
	DeleteBitmap(IMAGE_MENU_2_1);
	DeleteBitmap(IMAGE_GAUGE_BLUE);
	DeleteBitmap(IMAGE_GAUGE_GREEN);
	DeleteBitmap(IMAGE_GAUGE_RED);
	DeleteBitmap(IMAGE_GAUGE_AG);
	DeleteBitmap(IMAGE_GAUGE_SD);
	DeleteBitmap(IMAGE_GAUGE_EXBAR);
	DeleteBitmap(IMAGE_MENU_BTN_CHAINFO);
	DeleteBitmap(IMAGE_MENU_BTN_MYINVEN);
	DeleteBitmap(IMAGE_MENU_BTN_FRIEND);
	DeleteBitmap(IMAGE_MENU_BTN_WINDOW);
	DeleteBitmap(BITMAP_START_SPK + 114);
	DeleteBitmap(BITMAP_START_SPK + 115);
	DeleteBitmap(BITMAP_START_SPK + 116);
	#if Ex700
	DeleteBitmap(IMAGE_GAUGE_END_FORM);
	DeleteBitmap(IMAGE_GAUGE_END_MANA);
	DeleteBitmap(IMAGE_GAUGE_BTN_01);
	DeleteBitmap(IMAGE_GAUGE_BTN_02);
	DeleteBitmap(IMAGE_GAUGE_BTN_03);
	DeleteBitmap(IMAGE_GAUGE_BTN_04);
	DeleteBitmap(IMAGE_GAUGE_BTN_05);
	DeleteBitmap(IMAGE_GAUGE_BTN_06);
	DeleteBitmap(IMAGE_GAUGE_BTN_07);
	DeleteBitmap(IMAGE_GAUGE_BTN_08);
	DeleteBitmap(IMAGE_GAUGE_BTN_09);
	DeleteBitmap(IMAGE_GAUGE_BTN_10);
	DeleteBitmap(IMAGE_GAUGE_BTN_11);
	#endif
}

bool SEASON3B::CNewUIMainFrameWindow::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
	if (NULL == pNewUIMng || NULL == pNewUI3DRenderMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MAINFRAME, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;
	m_pNewUI3DRenderMng->Add3DRenderObj(this, ITEMHOTKEYNUMBER_CAMERA_Z_ORDER);

	LoadImages();

	SetButtonInfo();

	Show(true);

	return true;
}

void SEASON3B::CNewUIMainFrameWindow::SetButtonInfo()
{
#if Ex700
	POINT mPos = { setPosCenterX(640), setPosDown(430) };
	SIZE Btn3D = { 19.f, 19.f };

	g_pUIForm->RenderCustomButton(&m_BtnCShop,		mPos.x + 45.f,		mPos.y + 20, 6, "");
	g_pUIForm->RenderCustomButton(&m_BtnChaInfo,	mPos.x + 69.5f,		mPos.y + 20, 6, "");
	g_pUIForm->RenderCustomButton(&m_BtnMyInven,	mPos.x + 93.5f,		mPos.y + 20, 6, "");
	g_pUIForm->RenderCustomButton(&m_BtnFriend,		mPos.x + 529.1f,	mPos.y + 20, 6, "");
	g_pUIForm->RenderCustomButton(&m_BtnQuest,		mPos.x + 553.5f,	mPos.y + 20, 6, "");
	g_pUIForm->RenderCustomButton(&m_BtnWindow,		mPos.x + 577.9f,	mPos.y + 20, 6, "");
#else
	int x_Next = setPosCenterX(640) + 489;
	int y_Next = setPosDown(480 - 51);

	switch (m_Resolution)
	{
		case 7:
		x_Next = setPosCenterX(640) + 489;
		y_Next = setPosDown(480 - 50);
		break;
		case 8:
		x_Next = setPosCenterX(640) + 489;
		y_Next = setPosDown(480 - 50);
		break;
		case 9:
		x_Next = setPosCenterX(640) + 489;
		y_Next = setPosDown(480 - 50);
		break;
		default:
		x_Next = setPosCenterX(640) + 489;
		y_Next = setPosDown(480 - 51);
		break;
	}

	int x_Add = 30;
	int y_Add = 41;
	m_BtnCShop.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnCShop.ChangeButtonImgState( true, IMAGE_MENU_BTN_CSHOP, true );
	m_BtnCShop.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Add;
	m_BtnCShop.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnCShop.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnCShop.ChangeToolTipText(GlobalText[2277], true);

	m_BtnChaInfo.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnChaInfo.ChangeButtonImgState( true, IMAGE_MENU_BTN_CHAINFO, true );
	m_BtnChaInfo.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);	
	x_Next += x_Add;
	m_BtnChaInfo.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnChaInfo.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnChaInfo.ChangeToolTipText(GlobalText[362], true);

	m_BtnMyInven.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnMyInven.ChangeButtonImgState( true, IMAGE_MENU_BTN_MYINVEN, true );
	m_BtnMyInven.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);	
	x_Next += x_Add;
	m_BtnMyInven.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnMyInven.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnMyInven.ChangeToolTipText(GlobalText[363], true);

	m_BtnFriend.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnFriend.ChangeButtonImgState( true, IMAGE_MENU_BTN_FRIEND, true );
	m_BtnFriend.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);	
	x_Next += x_Add;
	m_BtnFriend.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnFriend.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnFriend.ChangeToolTipText(GlobalText[1043], true);

	m_BtnWindow.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnWindow.ChangeButtonImgState( true, IMAGE_MENU_BTN_WINDOW, true );
	m_BtnWindow.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);	
	m_BtnWindow.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnWindow.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnWindow.ChangeToolTipText(GlobalText[1744], true);
#endif
}

void SEASON3B::CNewUIMainFrameWindow::Release()
{
	UnloadImages();

	if (m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->Remove3DRenderObj(this);
		m_pNewUI3DRenderMng = NULL;
	}

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

bool SEASON3B::CNewUIMainFrameWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
#if Ex700
	RenderLifeMana();
	RenderFrame();
	m_pNewUI3DRenderMng->RenderUI2DEffect(ITEMHOTKEYNUMBER_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);
	g_pSkillList->RenderCurrentSkillAndHotSkillList();
	EnableAlphaTest();
	RenderGuageSD();
	RenderGuageAG();
	RenderNumberBar();
	RenderButtons();
	RenderExperience();
	#else
	RenderFrame();
	m_pNewUI3DRenderMng->RenderUI2DEffect(ITEMHOTKEYNUMBER_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);
	g_pSkillList->RenderCurrentSkillAndHotSkillList();
	EnableAlphaTest();
	RenderLifeMana();
	RenderGuageSD();
	RenderGuageAG();
	RenderButtons();
	RenderExperience();
	RenderFrameBarLop();
#endif


	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIMainFrameWindow::Render3D()
{
	m_ItemHotKey.RenderItems();
}

void SEASON3B::CNewUIMainFrameWindow::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	g_pMainFrame->RenderHotKeyItemCount();

}

bool SEASON3B::CNewUIMainFrameWindow::IsVisible() const
{
	return CNewUIObj::IsVisible();
}

void SEASON3B::CNewUIMainFrameWindow::RenderFrame()
{
#if Ex700
	float W = TransForX(640);
	float H = TransForY(73);
	float X = (IsToolKit.GetPositionScreen() - (W / 2)) / 2;
	//float X = (TransForX(640) - W ) / 2; //
	float Y = TransForY(480 - 73);

	RenderBitmap(IMAGE_MENU_1, X, Y, W, H, 0.0, 0.0, 0.9129598141, 0.8629598618, 0, 0, 0.0);
#else
float width, height;
	float x, y;
	
	width = 256.f; height = 51.f;

	x = setPosCenterX(640);
	y = setPosDown(480 - 51);

	float SoChia = 1.875f;
	int SangPhai = 640;
	RenderBitmap(BITMAP_INTERFACE_LEFT_BEGIN, x-70, y, 70.28048780487805, 51, 0.0, 0.0, 0.8785000443, 0.6409999728, 1, 1, 0.0);
	RenderBitmap(BITMAP_INTERFACE_LEFT_BEGIN, x + SangPhai, y, 70.28048780487805, 51, 0.8785000443, 0.0, -0.8785000443, 0.6409999728, 1, 1, 0.0);

	SEASON3B::RenderImage(IMAGE_MENU_1, x, y, width, height);
	width = 128.f;

	x += 256.f;

	SEASON3B::RenderImage(IMAGE_MENU_2, x, y, width, height);
	width = 256.f;
	x += 128.f;
	SEASON3B::RenderImage(IMAGE_MENU_3, x, y, width, height);

	if(g_pSkillList->IsSkillListUp() == true)
	{
		width = 160.f; height = 40.f;

		x = setPosCenterX(640) + 222.f;
		SEASON3B::RenderImage(IMAGE_MENU_2_1, x, y, width, height);
	}
#endif
}

void SEASON3B::CNewUIMainFrameWindow::RenderFrameBarLop()
{
	float width, height;
	float x, y;

	width = 256.f; height = 51.f;

	x = setPosCenterX(640);
	y = setPosDown(480 - 51);

	SEASON3B::RenderImage(BITMAP_START_SPK + 114, x, y, width, height);
	width = 128.f;

	x += 256.f;

	SEASON3B::RenderImage(BITMAP_START_SPK + 115, x, y, width, height);
	width = 256.f;
	x += 128.f;
	SEASON3B::RenderImage(BITMAP_START_SPK + 116, x, y, width, height);
}

void SEASON3B::CNewUIMainFrameWindow::RenderLifeMana()
{
	DWORD wLifeMax = CharacterAttribute->ViewMaxHP;
	DWORD wLife = min(max(0, CharacterAttribute->ViewCurHP), wLifeMax);

	DWORD wManaMax = CharacterAttribute->ViewMaxMP;
	DWORD wMana = min(max(0, CharacterAttribute->ViewCurMP), wManaMax);

	if (wLifeMax > 0)
	{
		if (wLife > 0 && (wLife / (float)wLifeMax) < 0.2f)
		{
			PlayBuffer(SOUND_HEART);
		}
	}

	float fLife = 0.f;
	float fMana = 0.f;

	if (wLifeMax > 0)
	{
		fLife = (wLifeMax - wLife) / (float)wLifeMax;
	}
	if (wManaMax > 0)
	{
		fMana = (wManaMax - wMana) / (float)wManaMax;
	}

	float width, height;
	float x, y;
	float fY, fH, fV;

	// life
	#if Ex700
	width = 65.f;
	height = 58.f;
	x = setPosCenterX(640) + 107;
	y = setPosDown(480 - 71);
#else
	width = 45.f; 
	height = 39.f;
	x = setPosCenterX(640) + 158;
	y = setPosDown(480 - 48);
#endif


	fY = y + (fLife * height);
	fH = height - (fLife * height);
	fV = fLife;
#if Ex700
	RenderBitmap(IMAGE_GAUGE_END, x, y, width, height, 0.f, 0.f, width / 64.f, height / 64.f);
#endif
	if (g_isCharacterBuff((&Hero->Object), eDeBuff_Poison))
	{
		RenderBitmap(IMAGE_GAUGE_GREEN, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
	}
	else
	{
		RenderBitmap(IMAGE_GAUGE_RED, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
	}
	//ACuoi Fix HP Number
#if Ex700 == 0
SEASON3B::RenderNumber(x + 25, y + 10, wLife);
#endif
	char strTipText[256];
	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[358], IsToolKit.QN(wLife), IsToolKit.QN(wLifeMax));
		RenderTipText((int)x, (int)y - 18, strTipText);
	}

	// mana
	#if Ex700
	width = 65.f;
	height = 58.f;

	x = setPosCenterX(640) + 463.f;
	y = setPosDown(480 - 70);
#else
width = 45.f; 
	height = 39.f;


	x = setPosCenterX(640) + 256.f + 128.f + 53.f;
	y = setPosDown(480 - 48);
#endif
	fY = y + (fMana * height);
	fH = height - (fMana * height);
	fV = fMana;
#if Ex700
	RenderBitmap(IMAGE_GAUGE_END_MANA, x, y, width, height, 0.f, 0.f, width / 64.f, height / 64.f);
	#endif
	RenderBitmap(IMAGE_GAUGE_BLUE, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);

	//ACuoi mana
#if Ex700 == 0
SEASON3B::RenderNumber(x + 30, y + 10, wMana);
#endif
	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[359], IsToolKit.QN(wMana), IsToolKit.QN(wManaMax));
		RenderTipText((int)x, (int)y - 18, strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderNumberBar()
{
	#if Ex700
	//HP
	DWORD wHPMax = CharacterAttribute->ViewMaxHP;
	DWORD wHPMin = min(max(0, CharacterAttribute->ViewCurHP), wHPMax);
	float HPx = setPosCenterX(640) + 107;
	float HPy = setPosDown(480 - 71);
	SEASON3B::RenderNumber(HPx + 30, HPy + 18, wHPMin);
	//SD
	DWORD wSDMax = max(1, CharacterAttribute->ViewMaxSD);
	DWORD wSDMin = min(wSDMax, CharacterAttribute->ViewCurSD);
	float SDx = setPosCenterX(640) + 178.f;
	float SDy = setPosDown(480.f - 52.f);
	SEASON3B::RenderNumber(SDx + 38, SDy + 1, wSDMin, 0.81f);

	//Mana
	DWORD wMNMax = CharacterAttribute->ViewMaxMP;
	DWORD wMNMin = min(max(0, CharacterAttribute->ViewCurMP), wMNMax);
	float MNx = setPosCenterX(640) + 463.f;
	float MNy = setPosDown(480 - 70);
	SEASON3B::RenderNumber(MNx + 38.f, MNy + 16.f, wMNMin);

	//AG
	DWORD wAGMax = max(1, CharacterAttribute->ViewMaxBP);
	DWORD wAGMin = min(wAGMax, CharacterAttribute->ViewCurBP);
	float AGx = setPosCenterX(640) + 397.f;
	float AGy = setPosDown(480.f - 52.f);
	SEASON3B::RenderNumber(AGx + 38, AGy + 1, wAGMin, 0.81f);
	#endif
}

#if Ex700
void SEASON3B::CNewUIMainFrameWindow::RenderGuageAG()
{
	float x, y, width, height;

	DWORD dwMaxSkillMana = max(1, CharacterAttribute->ViewMaxBP);
	DWORD dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->ViewCurBP);

	float fSkillMana = 0.0f;
	if (dwMaxSkillMana > 0)
	{
		fSkillMana = (dwMaxSkillMana - dwSkillMana) / (float)dwMaxSkillMana;
	}

	width = 67.f;
	height = 8.8f;

	x = setPosCenterX(640) + 397.f;
	y = setPosDown(480.f - 52.f);

	float fW = width - (fSkillMana * width);
	float fU = fSkillMana * (width / (width + 5.f));

	RenderBitmap(IMAGE_GAUGE_AG, x + (fSkillMana * width), y, fW, height, fU, 0.f, fW / (width + 5.f), height / 16.f);

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];
		sprintf(strTipText, GlobalText[214], IsToolKit.QN(dwSkillMana), IsToolKit.QN(dwMaxSkillMana));
		RenderTipText((int)x - 20, (int)y - 18, strTipText);
	}
}
#else
void SEASON3B::CNewUIMainFrameWindow::RenderGuageAG()
{
	float x, y, width, height;
	float fY, fH, fV;

	#if MU_REMOVE_CLASS_CHECK
	DWORD dwMaxSkillMana,dwSkillMana;
	if(gCharacterManager.IsMasterLevel(Hero->Class) == true)
	{
		
		dwMaxSkillMana = max(1, Master_Level_Data.wMaxBP);
		dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->SkillMana);
	}
	else
	{
		dwMaxSkillMana = max(1, CharacterAttribute->SkillManaMax);
		dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->SkillMana);
	}
	#else
	DWORD dwMaxSkillMana = max(1, CharacterAttribute->ViewMaxBP);
	DWORD dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->ViewCurBP);
	#endif

	float fSkillMana = 0.0f;

	if(dwMaxSkillMana > 0)
	{
		fSkillMana = (dwMaxSkillMana - dwSkillMana) / (float)dwMaxSkillMana;
	}

	width = 16.f, height = 39.f;

	x = setPosCenterX(640) + 256 + 128 + 36;
	y = setPosDown(480.f - 49.f);

	fY = y + (fSkillMana * height);
	fH = height - (fSkillMana * height);
	fV = fSkillMana;

	RenderBitmap(IMAGE_GAUGE_AG, x, fY, width, fH, 0.f, fV*height/64.f, width/16.f, (1.0f - fV)*height/64.f);
	SEASON3B::RenderNumber(x + 10, y + 30, dwSkillMana);

	if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{	
		char strTipText[256];
        sprintf(strTipText, GlobalText[214], IsToolKit.QN(dwSkillMana), IsToolKit.QN(dwMaxSkillMana));
		RenderTipText((int)x - 20, (int)y - 18, strTipText);
    }
}
#endif

#if Ex700
void SEASON3B::CNewUIMainFrameWindow::RenderGuageSD()
{
	float x, y, width, height;
	float fY, fH, fV;
	DWORD wMaxShield, wShield;

	wMaxShield = max(1, CharacterAttribute->ViewMaxSD);
	wShield = min(wMaxShield, CharacterAttribute->ViewCurSD);

	float fShield = 0.0f;
	if (wMaxShield > 0)
	{
		fShield = (wMaxShield - wShield) / (float)wMaxShield;
	}

	width = 67.f;
	height = 8.8f;

	x = setPosCenterX(640) + 178.f;
	y = setPosDown(480.f - 52.f);

	float fX = x + (fShield * width);
	float fW = width - (fShield * width);
	float fU = fShield;

	RenderBitmap(IMAGE_GAUGE_SD, x, y, fW, height, 0.f, 0.f, fW / (width + 5.f), height / 16.f);

	//ACuoi SD
	height = 39.f;
	y = 480.f - 10.f - 39.f;
	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[2037], IsToolKit.QN(wShield), IsToolKit.QN(wMaxShield));

		RenderTipText((int)x - 20, (int)y - 18, strTipText);
	}
}
#else
void SEASON3B::CNewUIMainFrameWindow::RenderGuageSD()
{
	float x, y, width, height;
	float fY, fH, fV;
    DWORD wMaxShield,wShield;

	#if MU_REMOVE_CLASS_CHECK
	if(gCharacterManager.IsMasterLevel(Hero->Class) == true)
	{
		wMaxShield = max (1, Master_Level_Data.ViewMaxSD);
		wShield = min (wMaxShield, CharacterAttribute->ViewCurSD);
	}
	else
	{
		wMaxShield = max (1, CharacterAttribute->ViewMaxSD);
		wShield = min (wMaxShield, CharacterAttribute->ViewCurSD);
	}
	#else
	wMaxShield = max(1, CharacterAttribute->ViewMaxSD);
	wShield = min(wMaxShield, CharacterAttribute->ViewCurSD);
	#endif
	float fShield = 0.0f;

	if(wMaxShield > 0)
	{
		fShield = (wMaxShield - wShield) / (float)wMaxShield;
	}

	width = 16.f, 
	height = 39.f;

	x = setPosCenterX(640) + 204;
	y = setPosDown(480.f - 49.f);

	fY = y + (fShield * height);
	fH = height - (fShield * height);
	fV = fShield;

	RenderBitmap(IMAGE_GAUGE_SD, x, fY, width, fH, 0.f, fV*height/64.f, width/16.f, (1.0f - fV)*height/64.f);
	//ACuoi SD

	SEASON3B::RenderNumber(x + 15, y + 30, (int)wShield);
	height = 39.f;
	y = 480.f - 10.f - 39.f;
	if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{	
		char strTipText[256];

        sprintf(strTipText, GlobalText[2037], IsToolKit.QN(wShield), IsToolKit.QN(wMaxShield));

		RenderTipText((int)x - 20, (int)y - 18, strTipText);
    }
}
#endif

void SEASON3B::CNewUIMainFrameWindow::RenderExperience()
{
	__int64 wLevel;
	__int64 dwNexExperience;
	__int64 dwExperience;
	double x, y, width, height;
	
	#if Ex700
	DWORD m_dwCachedExp = 0;
	DWORD m_dwCachedNextExp = 0;
	float iWidth = 520.f;

	x = setPosCenterX(640) + 61.0f; 
	y = setPosDown(473); 
	width = 6; 
	height = 4;

	wLevel = CharacterAttribute->Level;

	dwNexExperience = CharacterAttribute->NextExperince;
	dwExperience = CharacterAttribute->Experience;

	if (dwExperience >= m_dwCachedExp || dwNexExperience != m_dwCachedNextExp)
	{
		m_dwCachedExp = dwExperience;
		m_dwCachedNextExp = dwNexExperience;
	}
	else
	{
		dwExperience = m_dwCachedExp;
		dwNexExperience = m_dwCachedNextExp;
	}

#else
	float iWidth = 629.f;
	x = setPosCenterX(640) + 2.f; y = setPosDown(473); width = 6; height = 4;

	#if MU_REMOVE_CLASS_CHECK
	if(gCharacterManager.IsMasterLevel(CharacterAttribute->Class) == true)
	{
		wLevel = (__int64)Master_Level_Data.nMLevel;
		dwNexExperience = (__int64)Master_Level_Data.lNext_MasterLevel_Experince;
		dwExperience = (__int64)Master_Level_Data.lMasterLevel_Experince;
	}
	else
	{
		wLevel = CharacterAttribute->Level;
		dwNexExperience = CharacterAttribute->NextExperince;
		dwExperience = CharacterAttribute->Experience;
	}
	#else
	wLevel = CharacterAttribute->Level;
	dwNexExperience = CharacterAttribute->NextExperince;
	dwExperience = CharacterAttribute->Experience;
	#endif
#endif
	WORD wPriorLevel = wLevel - 1;
	DWORD dwPriorExperience = 0;

	if (wPriorLevel > 0)
	{
		dwPriorExperience = (9 + wPriorLevel) * wPriorLevel * wPriorLevel * 10;

		if (wPriorLevel > 255)
		{
			int iLevelOverN = wPriorLevel - 255;
			dwPriorExperience += (9 + iLevelOverN) * iLevelOverN * iLevelOverN * 1000;
		}
	}

	float fNeedExp = dwNexExperience - dwPriorExperience;
	float fExp = dwExperience - dwPriorExperience;

	if (dwExperience < dwPriorExperience)
	{
		fExp = 0.f;
	}

	float fProgress = 0.f;
	if (fExp > 0.f && fNeedExp > 0.f)
	{
		fProgress = fExp / fNeedExp;
	}

	if (fProgress < 0.f)
	{
		fProgress = 0.f;
	}
	else if (fProgress > 1.f)
	{
		fProgress = 1.f;
	}

	width = fProgress * iWidth; height = 4.f;
	RenderBitmap(IMAGE_GAUGE_EXBAR, x, y, width, height, 0.f, 0.f, 6.f / 8.f, 4.f / 4.f);

	const __int64 currentLevelExperience = (dwExperience > dwPriorExperience) ? (dwExperience - dwPriorExperience) : 0;
	const __int64 requiredLevelExperience = (dwNexExperience > dwPriorExperience) ? (dwNexExperience - dwPriorExperience) : 0;
	width = iWidth; height = 4.f;

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[1748], IsToolKit.QN(dwExperience), IsToolKit.QN(dwNexExperience));
		RenderTipText(setPosCenterX(640) + 280, setPosDown(418), strTipText);
	}
#if Ex700
	float W = TransForX(640);
	float H = TransForY(73);
	float X = (IsToolKit.GetPositionScreen() - (W / 2)) / 2;
	float Y = TransForY(480 - 73);

	RenderBitmap(IMAGE_GAUGE_BTN_12, X, Y, W, H - 1.1f, 0.0, 0.0, 0.9129598141, 0.8629598618, 0, 0, 0.0);

	// Draw the value inside the main EXP frame.  The old position was only a few
	// pixels above the bottom edge, where the frame can cover it at some resolutions.
	char expText[96];
	sprintf_s(expText, sizeof(expText), "EXP: %I64d / %I64d", currentLevelExperience, requiredLevelExperience);
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 230, 110, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 190);
	g_pRenderText->RenderText((int)x, (int)y - 25, expText, (int)iWidth, 0, RT3_SORT_CENTER);

	int percent = 0;

	if (dwNexExperience > dwPriorExperience) 
	{
		percent = (int)(((double)(dwExperience - dwPriorExperience) /
			(double)(dwNexExperience - dwPriorExperience)) * 100.0);
	}

	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;

	SEASON3B::RenderNumber(x - 17, y - 2, percent, 0.75);
	#else
	SEASON3B::RenderNumber(x + 633.f, y - 4.f, (int)(fProgress * 10.f));
	#endif
}

void SEASON3B::CNewUIMainFrameWindow::RenderHotKeyItemCount()
{
	m_ItemHotKey.RenderItemCount();
}
#if Ex700
void SEASON3B::CNewUIMainFrameWindow::RenderButtons()
{
	POINT mPos = { setPosCenterX(640), setPosDown(430) };
	SIZE Btn3D = { 19.f, 19.f};

	g_pUIForm->RenderButtonBarFrameMain(IMAGE_GAUGE_BTN_01, mPos.x + 45,		mPos.y + 20, Btn3D.cx, Btn3D.cy, 1);
	g_pUIForm->RenderButtonBarFrameMain(IMAGE_GAUGE_BTN_02, mPos.x + 69.5f,		mPos.y + 20, Btn3D.cx, Btn3D.cy, 1);
	g_pUIForm->RenderButtonBarFrameMain(IMAGE_GAUGE_BTN_03, mPos.x + 93.5f,		mPos.y + 20, Btn3D.cx, Btn3D.cy, 1);
	g_pUIForm->RenderButtonBarFrameMain(IMAGE_GAUGE_BTN_04, mPos.x + 553.5f,	mPos.y + 20, Btn3D.cx, Btn3D.cy, 1);
	g_pUIForm->RenderButtonBarFrameMain(IMAGE_GAUGE_BTN_05, mPos.x + 529.1f,	mPos.y + 20, Btn3D.cx, Btn3D.cy, 1);
	g_pUIForm->RenderButtonBarFrameMain(IMAGE_GAUGE_BTN_06, mPos.x + 577.9f,	mPos.y + 20, Btn3D.cx, Btn3D.cy, 1);
}
#else
void SEASON3B::CNewUIMainFrameWindow::RenderButtons()
{
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_BtnCShop.Render();
#endif //defined PBG_ADD_INGAMESHOP_UI_MAINFRAME

	RenderCharInfoButton();
	m_BtnMyInven.Render();

	RenderFriendButton();

	m_BtnWindow.Render();
}
#endif
void SEASON3B::CNewUIMainFrameWindow::RenderCharInfoButton()
{
	m_BtnChaInfo.Render();

	if (g_QuestMng.IsQuestIndexByEtcListEmpty())
		return;

	if (g_Time.GetTimeCheck(5, 500))
		m_bButtonBlink = !m_bButtonBlink;

	if (m_bButtonBlink)
	{
		if (!(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_QUEST_PROGRESS_ETC)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)))
			RenderImage(IMAGE_MENU_BTN_CHAINFO, setPosCenterX(640) + 489 + 30, setPosDown(480 - 51), 30, 41, 0.0f, 41.f);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderFriendButton()
{
	m_BtnFriend.Render();

	int iBlinkTemp = g_pFriendMenu->GetBlinkTemp();
	BOOL bIsAlertTime = (iBlinkTemp % 24 < 12);

	if (g_pFriendMenu->IsNewChatAlert() && bIsAlertTime)
	{
		RenderFriendButtonState();
	}
	if (g_pFriendMenu->IsNewMailAlert())
	{
		if (bIsAlertTime)
		{
			RenderFriendButtonState();

			if (iBlinkTemp % 24 == 11)
			{
				g_pFriendMenu->IncreaseLetterBlink();
			}
		}
	}
	else if (g_pLetterList->CheckNoReadLetter())
	{
		RenderFriendButtonState();
	}

	g_pFriendMenu->IncreaseBlinkTemp();
}

void SEASON3B::CNewUIMainFrameWindow::RenderFriendButtonState()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND) == true)
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, setPosCenterX(640) + 489 + (30 * 3), setPosDown(480 - 51), 30, 41, 0.0f, 123.f);
	}
	else
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, setPosCenterX(640) + 489 + (30 * 3), setPosDown(480 - 51), 30, 41, 0.0f, 41.f);
	}
}

bool SEASON3B::CNewUIMainFrameWindow::UpdateMouseEvent()
{
	if (g_pNewUIHotKey->IsStateGameOver() == true)
	{
		return true;
	}

	if (BtnProcess() == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIMainFrameWindow::BtnProcess()
{
#if Ex700
	if (m_BtnQuest.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MYQUEST);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	else 
	#endif
	if (g_pNewUIHotKey->CanUpdateKeyEventRelatedMyInventory() == true)
	{
		if (m_BtnMyInven.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
	}
	else if (g_pNewUIHotKey->CanUpdateKeyEvent() == true)
	{
		if (m_BtnMyInven.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
		else if (m_BtnChaInfo.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);

			PlayBuffer(SOUND_CLICK01);

			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))

				return true;
		}
		else if (m_BtnFriend.UpdateMouseEvent() == true)
		{
			if (gMapManager.InChaosCastle() == true)
			{
				PlayBuffer(SOUND_CLICK01);
				return true;
			}

			int iLevel = CharacterAttribute->Level;

			if (iLevel < 6)
			{
				if (g_pChatListBox->CheckChatRedundancy(GlobalText[1067]) == FALSE)
				{
					g_pChatListBox->AddText("", GlobalText[1067], SEASON3B::TYPE_SYSTEM_MESSAGE);
				}
			}
			else
			{
				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_FRIEND);
			}
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
		else if (m_BtnWindow.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_WINDOW_MENU);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
		else if (m_BtnCShop.UpdateMouseEvent() == true)
		{
			if (g_pInGameShop->IsInGameShopOpen() == false)
				return false;

			if (g_InGameShopSystem->IsScriptDownload() == true)
			{
				if (g_InGameShopSystem->ScriptDownload() == false)
					return false;
			}

			if (g_InGameShopSystem->IsBannerDownload() == true)
			{
				g_InGameShopSystem->BannerDownload();
			}

			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INGAMESHOP) == false)
			{
				if (g_InGameShopSystem->GetIsRequestShopOpenning() == false)
				{
					SendRequestIGS_CashShopOpen(0);
					g_InGameShopSystem->SetIsRequestShopOpenning(true);

					g_pMainFrame->SetBtnState(MAINFRAME_BTN_PARTCHARGE, true);

				}
			}
			else
			{
				SendRequestIGS_CashShopOpen(1);
				g_pNewUISystem->Hide(SEASON3B::INTERFACE_INGAMESHOP);
			}

			return true;
		}
	}

	return false;
}

bool SEASON3B::CNewUIMainFrameWindow::UpdateKeyEvent()
{
	if (m_ItemHotKey.UpdateKeyEvent() == false)
	{
		return false;
	}
	return true;
}

bool SEASON3B::CNewUIMainFrameWindow::Update()
{
	if (m_bExpEffect == true)
	{
		if (timeGetTime() - m_dwExpEffectTime > 2000)
		{
			m_bExpEffect = false;
			m_dwExpEffectTime = 0;
			m_dwGetExp = 0;
		}
	}

	return true;
}

float SEASON3B::CNewUIMainFrameWindow::GetLayerDepth()
{
	return 10.6f;
}

float SEASON3B::CNewUIMainFrameWindow::GetKeyEventOrder()
{
	return 2.9f;
}

void SEASON3B::CNewUIMainFrameWindow::SetItemHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	m_ItemHotKey.SetHotKey(iHotKey, iItemType, iItemLevel);
}

int SEASON3B::CNewUIMainFrameWindow::GetItemHotKey(int iHotKey)
{
	return m_ItemHotKey.GetHotKey(iHotKey);
}

int SEASON3B::CNewUIMainFrameWindow::GetItemHotKeyLevel(int iHotKey)
{
	return m_ItemHotKey.GetHotKeyLevel(iHotKey);
}

void SEASON3B::CNewUIMainFrameWindow::UseHotKeyItemRButton()
{
	m_ItemHotKey.UseItemRButton();
}

void SEASON3B::CNewUIMainFrameWindow::UpdateItemHotKey()
{
	m_ItemHotKey.UpdateKeyEvent();
}

void SEASON3B::CNewUIMainFrameWindow::ResetSkillHotKey()
{
	g_pSkillList->Reset();
}

void SEASON3B::CNewUIMainFrameWindow::SetSkillHotKey(int iHotKey, int iSkillType)
{
	g_pSkillList->SetHotKey(iHotKey, iSkillType);
}

int SEASON3B::CNewUIMainFrameWindow::GetSkillHotKey(int iHotKey)
{
	return g_pSkillList->GetHotKey(iHotKey);
}

int SEASON3B::CNewUIMainFrameWindow::GetSkillHotKeyIndex(int iSkillType)
{
	return g_pSkillList->GetSkillIndex(iSkillType);
}

SEASON3B::CNewUIItemHotKey::CNewUIItemHotKey()
{
	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		m_iHotKeyItemType[i] = -1;
		m_iHotKeyItemLevel[i] = 0;
	}
}

SEASON3B::CNewUIItemHotKey::~CNewUIItemHotKey()
{

}

bool SEASON3B::CNewUIItemHotKey::UpdateKeyEvent()
{
	int iIndex = -1;

	if (SEASON3B::IsPress('Q') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_Q);
	}
	else if (SEASON3B::IsPress('W') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_W);
	}
	else if (SEASON3B::IsPress('E') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_E);
	}
	else if (SEASON3B::IsPress('R') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_R);
	}

	if (iIndex != -1)
	{
		ITEM* pItem = NULL;
		pItem = g_pMyInventory->FindItem(iIndex);
		if ((pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82))
		{
			std::list<eBuffState> secretPotionbufflist;
			secretPotionbufflist.push_back(eBuff_SecretPotion1);
			secretPotionbufflist.push_back(eBuff_SecretPotion2);
			secretPotionbufflist.push_back(eBuff_SecretPotion3);
			secretPotionbufflist.push_back(eBuff_SecretPotion4);
			secretPotionbufflist.push_back(eBuff_SecretPotion5);

			if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) != eBuffNone) {
				SEASON3B::CreateOkMessageBox(GlobalText[2530], RGBA(255, 30, 0, 255));
			}
			else {
				SendRequestUse(iIndex, 0);
			}
		}
		else

		{
			SendRequestUse(iIndex, 0);
		}
		return false;
	}

	return true;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyItemIndex(int iType, bool bItemCount)
{
	int iStartItemType = 0, iEndItemType = 0;

	switch (iType)
	{
	case HOTKEY_Q:
		if (!GetHotKeyCommonItem(iType, iStartItemType, iEndItemType))
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
				iStartItemType = ITEM_POTION + 6, iEndItemType = ITEM_POTION + 4;
			else
				iStartItemType = ITEM_POTION + 3, iEndItemType = ITEM_POTION + 0;
		}
		break;
	case HOTKEY_W:
		if (!GetHotKeyCommonItem(iType, iStartItemType, iEndItemType))
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
				iStartItemType = ITEM_POTION + 3, iEndItemType = ITEM_POTION + 0;
			else
				iStartItemType = ITEM_POTION + 6, iEndItemType = ITEM_POTION + 4;
		}
		break;
	case HOTKEY_E:
		if (!GetHotKeyCommonItem(iType, iStartItemType, iEndItemType))
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
				iStartItemType = ITEM_POTION + 3, iEndItemType = ITEM_POTION + 0;
			else if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
				iStartItemType = ITEM_POTION + 6, iEndItemType = ITEM_POTION + 4;
			else
				iStartItemType = ITEM_POTION + 8, iEndItemType = ITEM_POTION + 8;
		}
		break;
	case HOTKEY_R:
		if (!GetHotKeyCommonItem(iType, iStartItemType, iEndItemType))
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
				iStartItemType = ITEM_POTION + 3, iEndItemType = ITEM_POTION + 0;
			else if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
				iStartItemType = ITEM_POTION + 6, iEndItemType = ITEM_POTION + 4;
			else
				iStartItemType = ITEM_POTION + 37, iEndItemType = ITEM_POTION + 35;
		}
		break;
	}

	ITEM* pItem = nullptr;
	int iItemCount = 0;

	std::vector<CNewUIInventoryCtrl*> allControls;
	if (g_pMyInventory->GetInventoryCtrl())
		allControls.push_back(g_pMyInventory->GetInventoryCtrl());

	if (g_pMyInventoryExt)
	{
		for (int i = 0; i < CharacterAttribute->InventoryExtensions; ++i)
		{
			if (auto* ctrl = g_pMyInventoryExt->GetInventoryCtrlByIndex(i))
				allControls.push_back(ctrl);
		}
	}

	for (int i = iStartItemType; i >= iEndItemType; --i)
	{
		if (bItemCount)
		{
			for (auto* invCtrl : allControls)
			{
				int itemCount = invCtrl->GetNumberOfItems();
				for (int j = 0; j < itemCount; ++j)
				{
					pItem = invCtrl->GetItem(j);
					if (!pItem)
						continue;

					if ((pItem->Type == i && ((pItem->Level >> 3) & 15) == m_iHotKeyItemLevel[iType]) ||
						(pItem->Type == i && (pItem->Type >= ITEM_POTION + 0 && pItem->Type <= ITEM_POTION + 3)))
					{
						if (pItem->Type == ITEM_POTION + 9 || pItem->Type == ITEM_POTION + 10 || pItem->Type == ITEM_POTION + 20)
							iItemCount++;
						else
							iItemCount += pItem->Durability;
					}
				}
			}
		}
		else
		{
			int iIndex = -1;
			if (i >= ITEM_POTION + 0 && i <= ITEM_POTION + 3)
			{
				iIndex = g_pMyInventory->FindItemIndex(i);
			}
			else
			{
				iIndex = g_pMyInventory->FindItemIndex(i, m_iHotKeyItemLevel[iType]);
			}

			if (iIndex != -1)
			{
				pItem = g_pMyInventory->FindItem(iIndex);

				if (pItem && ((pItem->Type != ITEM_POTION + 7 && pItem->Type != ITEM_POTION + 10 && pItem->Type != ITEM_POTION + 20) || ((pItem->Level >> 3) & 15) == m_iHotKeyItemLevel[iType]))
				{
					return iIndex;
				}
			}
		}
	}

	if (bItemCount)
		return iItemCount;

	return -1;
}

bool SEASON3B::CNewUIItemHotKey::GetHotKeyCommonItem(IN int iHotKey, OUT int& iStart, OUT int& iEnd)
{
	switch (m_iHotKeyItemType[iHotKey])
	{
	case ITEM_POTION + 7:
	case ITEM_POTION + 8:
	case ITEM_POTION + 9:
	case ITEM_POTION + 10:
	case ITEM_POTION + 20:
	case ITEM_POTION + 46:
	case ITEM_POTION + 47:
	case ITEM_POTION + 48:
	case ITEM_POTION + 49:
	case ITEM_POTION + 50:
	case ITEM_POTION + 70:
	case ITEM_POTION + 71:
	case ITEM_POTION + 78:
	case ITEM_POTION + 79:
	case ITEM_POTION + 80:
	case ITEM_POTION + 81:
	case ITEM_POTION + 82:
	case ITEM_POTION + 94:
	case ITEM_POTION + 85:
	case ITEM_POTION + 86:
	case ITEM_POTION + 87:
	case ITEM_POTION + 133:
		if (m_iHotKeyItemType[iHotKey] != ITEM_POTION + 20 || m_iHotKeyItemLevel[iHotKey] == 0)
		{
			iStart = iEnd = m_iHotKeyItemType[iHotKey];
			return true;
		}
		break;
	default:
		if (m_iHotKeyItemType[iHotKey] >= ITEM_POTION + 35 && m_iHotKeyItemType[iHotKey] <= ITEM_POTION + 37)
		{
			iStart = ITEM_POTION + 37; iEnd = ITEM_POTION + 35;
			return true;
		}
		else if (m_iHotKeyItemType[iHotKey] >= ITEM_POTION + 38 && m_iHotKeyItemType[iHotKey] <= ITEM_POTION + 40)
		{
			iStart = ITEM_POTION + 40; iEnd = ITEM_POTION + 38;
			return true;
		}
		break;
	}
	return false;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyItemCount(int iType)
{
	return 0;
}

void SEASON3B::CNewUIItemHotKey::SetHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	if (iHotKey != -1 && CNewUIMyInventory::CanRegisterItemHotKey(iItemType) == true
		)
	{
		m_iHotKeyItemType[iHotKey] = iItemType;
		m_iHotKeyItemLevel[iHotKey] = iItemLevel;
	}
	else
	{
		m_iHotKeyItemType[iHotKey] = -1;
		m_iHotKeyItemLevel[iHotKey] = 0;
	}
}

int SEASON3B::CNewUIItemHotKey::GetHotKey(int iHotKey)
{
	if (iHotKey != -1)
	{
		return m_iHotKeyItemType[iHotKey];
	}

	return -1;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyLevel(int iHotKey)
{
	if (iHotKey != -1)
	{
		return m_iHotKeyItemLevel[iHotKey];
	}

	return 0;
}

void SEASON3B::CNewUIItemHotKey::RenderItems()
{
	float x, y, width, height;

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		int iIndex = GetHotKeyItemIndex(i);
		if (iIndex != -1)
		{
			ITEM* pItem = g_pMyInventory->FindItem(iIndex);
			if (pItem)
			{
				#if Ex700
				x = setPosCenterX(640) + 189.3f + (i * 29.5);
				y = setPosDown(443) + 2.f;
				width = 20; 
				height = 20;
				#else
				x = setPosCenterX(640) + 10 + (i * 38); y = setPosDown(443); width = 20; height = 20;
				#endif
				RenderItem3D(x - 3, y, width, height, pItem->Type, pItem->Level, 0, 0);
			}
		}
	}
}

void SEASON3B::CNewUIItemHotKey::RenderItemCount()
{
	float x, y, width, height;

	glColor4f(1.f, 1.f, 1.f, 1.f);

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		int iCount = GetHotKeyItemIndex(i, true);
		if (iCount > 0)
		{
			#if Ex700
			x = setPosCenterX(640) + 208 + (i * 29.5);
			y = setPosDown(457) + 2;
			width = 8;
			height = 9;
			SEASON3B::RenderNumber(x - 9, y, iCount, 0.80f);
			#else
			x = setPosCenterX(640) + 30 + (i * 38); 
			y = setPosDown(457); 
			width = 8; 
			height = 9;
			SEASON3B::RenderNumber(x - 9,  y, iCount, 0.85f);
			#endif
		}
	}
}

void SEASON3B::CNewUIItemHotKey::UseItemRButton()
{
	int x, y, width, height;

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		#if Ex700
		x = setPosCenterX(640) + 189.3f + (i * 29.5);
		y = setPosDown(443) + 2.f;
		width = 20;
		height = 20;
		#else
		x = setPosCenterX(640) + 10 + (i * 38); y = setPosDown(445); width = 20; height = 20;
		#endif
		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			if (MouseRButtonPush)
			{
				MouseRButtonPush = false;
				int iIndex = GetHotKeyItemIndex(i);
				if (iIndex != -1)
				{
					SendRequestUse(iIndex, 0);
					break;
				}
			}
		}
	}
}

SEASON3B::CNewUISkillList::CNewUISkillList()
{
	m_pNewUIMng = NULL;
	Reset();
}

SEASON3B::CNewUISkillList::~CNewUISkillList()
{
	Release();
}

bool SEASON3B::CNewUISkillList::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_SKILL_LIST, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;

	LoadImages();

	Show(true);

	return true;
}

void SEASON3B::CNewUISkillList::Release()
{
	if (m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->DeleteUI2DEffectObject(UI2DEffectCallback);
	}

	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUISkillList::Reset()
{
	m_bSkillList = false;
	m_bHotKeySkillListUp = false;

	m_bRenderSkillInfo = false;
	m_iRenderSkillInfoType = 0;
	m_iRenderSkillInfoPosX = 0;
	m_iRenderSkillInfoPosY = 0;

	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		m_iHotKeySkillType[i] = -1;
	}

	m_EventState = EVENT_NONE;
}

void SEASON3B::CNewUISkillList::LoadImages()
{
	LoadBitmap("Interface\\newui_skill.jpg", IMAGE_SKILL1, GL_LINEAR);
	LoadBitmap("Interface\\newui_skill2.jpg", IMAGE_SKILL2, GL_LINEAR);
	LoadBitmap("Interface\\newui_command.jpg", IMAGE_COMMAND, GL_LINEAR);
	LoadBitmap("Interface\\newui_skillbox.jpg", IMAGE_SKILLBOX, GL_LINEAR);
	#if Ex700
	LoadBitmap("Interface\\Ex700\\SelectSkill.tga", IMAGE_SKILLBOX_USE, GL_LINEAR);
	#else
	LoadBitmap("Interface\\newui_skillbox2.jpg", IMAGE_SKILLBOX_USE, GL_LINEAR);
	#endif
	LoadBitmap("Interface\\newui_non_skill.jpg", IMAGE_NON_SKILL1, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_skill2.jpg", IMAGE_NON_SKILL2, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_command.jpg", IMAGE_NON_COMMAND, GL_LINEAR);
	LoadBitmap("Interface\\newui_skill3.jpg", IMAGE_SKILL3, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_skill3.jpg", IMAGE_NON_SKILL3, GL_LINEAR);
}

void SEASON3B::CNewUISkillList::UnloadImages()
{
	DeleteBitmap(IMAGE_SKILL1);
	DeleteBitmap(IMAGE_SKILL2);
	DeleteBitmap(IMAGE_COMMAND);
	DeleteBitmap(IMAGE_SKILLBOX);
	DeleteBitmap(IMAGE_SKILLBOX_USE);
	DeleteBitmap(IMAGE_NON_SKILL1);
	DeleteBitmap(IMAGE_NON_SKILL2);
	DeleteBitmap(IMAGE_NON_COMMAND);
	DeleteBitmap(IMAGE_SKILL3);
	DeleteBitmap(IMAGE_NON_SKILL3);
}

bool SEASON3B::CNewUISkillList::UpdateMouseEvent()
{
#ifdef MOD_SKILLLIST_UPDATEMOUSE_BLOCK
	if(GFxProcess::GetInstancePtr()->GetUISelect() == 1)
	{
		return true;
	}
#endif //MOD_SKILLLIST_UPDATEMOUSE_BLOCK
	if (g_isCharacterBuff((&Hero->Object), eBuff_DuelWatch))
	{
		m_bSkillList = false;
		return true;
	}

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;
	BYTE bySkillMasterNumber = CharacterAttribute->SkillMasterNumber;

	float x, y, width, height;

	m_bRenderSkillInfo = false;

	if (bySkillNumber <= 0)
	{
		return true;
	}
#if Ex700
	x = setPosCenterX(640) + 310.0f;
	y = setPosDown(431.f) + 10.0f;
	width = 20.f;
	height = 28.f;
#else
	x = setPosCenterX(640) + 385.f; y = setPosDown(431.f); width = 32.f; height = 38.f;
#endif
	//Hover Click MainSkill -> Open the list of skills
	if (m_EventState == EVENT_NONE && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_HOVER_CURRENTSKILL;
		return true;
	}
	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL && (MouseLButtonPush == true || MouseLButtonDBClick == true)
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_DOWN_CURRENTSKILL;
		return false;
	}
	if (m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		if (MouseLButtonPush == false && MouseLButtonDBClick == false)
		{
			if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
			{
				m_bSkillList = !m_bSkillList;
				PlayBuffer(SOUND_CLICK01);
				m_EventState = EVENT_NONE;
				return false;
			}
			m_EventState = EVENT_NONE;
			return true;
		}
	}

	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL)
	{
		m_bRenderSkillInfo = true;
		m_iRenderSkillInfoType = Hero->CurrentSkill;
		m_iRenderSkillInfoPosX = x - 5;
		#if Ex700
		m_iRenderSkillInfoPosY = y - 30;
		#else
		m_iRenderSkillInfoPosY = y;
		#endif

		return false;
	}
	else if (m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		return false;
	}
#if Ex700
	x = setPosCenterX(640) + 330.f;
	y = setPosDown(431.f) + 10.f;
	width = 32.f * 5.f;
	height = 28.f;
	#else
	x = setPosCenterX(640) + 222.f; y = setPosDown(431.f); width = 32.f * 5.f; height = 38.f;
#endif

	if (m_EventState == EVENT_NONE && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_HOVER_SKILLHOTKEY;
		return true;
	}
	if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == true
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_DOWN_SKILLHOTKEY;
		return false;
	}
#if Ex700
	x = setPosCenterX(640) + 310.f;
	y = setPosDown(431.f) + 10.f;
	width = 22.5f;
	height = 28.f;
	#else
	x = setPosCenterX(640) + 190.f; y = setPosDown(431.f); width = 32.f; height = 38.f;
#endif
	int iStartIndex = (m_bHotKeySkillListUp == true) ? 6 : 1;
	for (int i = 0, iIndex = iStartIndex; i < 5; ++i, iIndex++)
	{
		x += width;

		if (iIndex == 10)
		{
			iIndex = 0;
		}
		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			if (m_iHotKeySkillType[iIndex] == -1)
			{
				if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY)
				{
					m_bRenderSkillInfo = false;
					m_iRenderSkillInfoType = -1;
				}
				if (m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY && MouseLButtonPush == false)
				{
					m_EventState = EVENT_NONE;
				}
				continue;
			}

			WORD bySkillType = CharacterAttribute->Skill[m_iHotKeySkillType[iIndex]];

			if (bySkillType == 0 || (bySkillType >= AT_SKILL_STUN && bySkillType <= AT_SKILL_REMOVAL_BUFF))
				continue;

			BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;

			if (bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
			{
				continue;
			}

			if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY)
			{
				m_bRenderSkillInfo = true;
				m_iRenderSkillInfoType = m_iHotKeySkillType[iIndex];
				m_iRenderSkillInfoPosX = x - 5;
				#if Ex700
				m_iRenderSkillInfoPosY = y - 30;
				#else
				m_iRenderSkillInfoPosY = y;
				#endif
				return true;
			}
			if (m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY)
			{
				if (MouseLButtonPush == false)
				{
					if (m_iRenderSkillInfoType == m_iHotKeySkillType[iIndex])
					{
						m_EventState = EVENT_NONE;
						m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];
						Hero->CurrentSkill = m_iHotKeySkillType[iIndex];
						PlayBuffer(SOUND_CLICK01);
						return false;
					}
					else
					{
						m_EventState = EVENT_NONE;
					}
				}
			}
		}
	}
#if Ex700
	x = setPosCenterX(640) + 330.f;
	y = setPosDown(431.f) + 10.f;
	width = 32.f * 5.f;
	height = 28.f;
#else
x = setPosCenterX(640) + 222.f; y = setPosDown(431.f); width = 32.f * 5.f; height = 38.f;
#endif
	if (m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY)
	{
		if (MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == false)
		{
			m_EventState = EVENT_NONE;
			return true;
		}
		return false;
	}

	if (m_bSkillList == false)
		return true;

	WORD bySkillType = 0;
#if Ex700
	bool bMouseOnSkillList = false;

	std::vector<int> skills;
	skills.reserve(MAX_MAGIC);
	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		int s = CharacterAttribute->Skill[i];
		if (s == 0 || (s >= AT_SKILL_STUN && s <= AT_SKILL_REMOVAL_BUFF))
			continue;
		BYTE t = SkillAttribute[s].SkillUseType;
		if (t == SKILL_USE_TYPE_MASTER || t == SKILL_USE_TYPE_MASTERLEVEL)
			continue;
		skills.push_back(i);
	}

	int totalSkills = (int)skills.size();
	int cols = 11;
	float tileW = SKILL_TILE_W;
	float tileH = SKILL_TILE_H;
	float totalWidth = tileW * cols;
	float baseX = (IsToolKit.GetPositionScreen() - totalWidth) / 2.0f;
	float baseY = setPosDown(390);
	float GetPosYIconHober = 23.f;

	EVENT_STATE PrevEventState = m_EventState;

	for (int idx = 0; idx < totalSkills; ++idx)
	{
		int col = idx % cols;
		int row = idx / cols;
		float x = baseX + col * tileW;
		float y = baseY + row * tileH - GetPosYIconHober;

		float iconW = 20.0f;
		float iconH = 28.0f;
		float iconOffsetX = 2.8f;

		if (SEASON3B::CheckMouseIn(x + iconOffsetX, y, iconW, iconH))
		{
			bMouseOnSkillList = true;

			if (m_EventState == EVENT_NONE && MouseLButtonPush == false)
			{
				m_EventState = EVENT_BTN_HOVER_SKILLLIST;
				break;
			}
			if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true)
			{
				m_EventState = EVENT_BTN_DOWN_SKILLLIST;
				break;
			}
			if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == false)
			{
				m_bRenderSkillInfo = true;
				m_iRenderSkillInfoType = skills[idx];
				m_iRenderSkillInfoPosX = x;
				m_iRenderSkillInfoPosY = y;
			}
			if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
				&& m_iRenderSkillInfoType == skills[idx])
			{
				m_EventState = EVENT_NONE;
				m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];
				Hero->CurrentSkill = skills[idx];
				m_bSkillList = false;
				PlayBuffer(SOUND_CLICK01);
				return false;
			}
		}
	}
#else
int iSkillCount = 0;
	bool bMouseOnSkillList = false;
	
	x = setPosCenterX(640) + 385.f; y = setPosDown(390); width = 32; height = 38;
	float fOrigX = setPosCenterX(640) + 385.f;
	
	EVENT_STATE PrevEventState = m_EventState;

	for(int i=0; i<MAX_MAGIC; ++i)
	{
		bySkillType = CharacterAttribute->Skill[i];

		if(bySkillType == 0 || ( bySkillType >= AT_SKILL_STUN && bySkillType <= AT_SKILL_REMOVAL_BUFF ))
			continue;

		BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;

		if(bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
		{
			continue;
		}


		if(iSkillCount == 18)
		{
			y -= height;
		}
		
		if(iSkillCount < 14)
		{
			int iRemainder = iSkillCount % 2;
			int iQuotient = iSkillCount / 2;
			
			if(iRemainder == 0)
			{
				x = fOrigX + iQuotient * width;
			}
			else
			{
				x = fOrigX - (iQuotient + 1) * width;
			}
		}
		else if(iSkillCount >= 14 && iSkillCount < 18)
		{
			x = fOrigX - (8 * width) - ((iSkillCount - 14) * width);
		}
		else
		{
			x = fOrigX - (12 * width) + ((iSkillCount - 17) * width);
		}

		iSkillCount++;
		
		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			bMouseOnSkillList = true;
			if(m_EventState == EVENT_NONE && MouseLButtonPush == false)
			{
				m_EventState = EVENT_BTN_HOVER_SKILLLIST;
				break;
			}
		}
		
		if(m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true
			&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_EventState = EVENT_BTN_DOWN_SKILLLIST;
			break;
		}
		
		if(m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == false
			&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_bRenderSkillInfo = true;
			m_iRenderSkillInfoType = i;
			m_iRenderSkillInfoPosX = x;
			m_iRenderSkillInfoPosY = y;
		}
		
		if(m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
			&& m_iRenderSkillInfoType == i && SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_EventState = EVENT_NONE;

			m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

			Hero->CurrentSkill = i;
			m_bSkillList = false;
	
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}
#endif
	if (PrevEventState != m_EventState)
	{
		if (m_EventState == EVENT_NONE || m_EventState == EVENT_BTN_HOVER_SKILLLIST)
			return true;
		return false;
	}

	if (Hero->m_pPet != NULL)
	{
		x = setPosCenterX(640) + 353.f; y = setPosDown(352); width = 32; height = 38;
		for (int i = AT_PET_COMMAND_DEFAULT; i < AT_PET_COMMAND_END; ++i)
		{
			if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
			{
				bMouseOnSkillList = true;

				if (m_EventState == EVENT_NONE && MouseLButtonPush == false)
				{
					m_EventState = EVENT_BTN_HOVER_SKILLLIST;
					return true;
				}
				if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true)
				{
					m_EventState = EVENT_BTN_DOWN_SKILLLIST;
					return false;
				}

				if (m_EventState == EVENT_BTN_HOVER_SKILLLIST)
				{
					m_bRenderSkillInfo = true;
					m_iRenderSkillInfoType = i;
					m_iRenderSkillInfoPosX = x;
					m_iRenderSkillInfoPosY = y;
				}
				if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
					&& m_iRenderSkillInfoType == i)
				{
					m_EventState = EVENT_NONE;

					m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

					Hero->CurrentSkill = i;
					m_bSkillList = false;
					PlayBuffer(SOUND_CLICK01);
					return false;
				}
			}
			x += width;
		}
	}

	if (bMouseOnSkillList == false && m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if (bMouseOnSkillList == false && MouseLButtonPush == false
		&& m_EventState == EVENT_BTN_DOWN_SKILLLIST)
	{
		m_EventState = EVENT_NONE;
		return false;
	}
	if (m_EventState == EVENT_BTN_DOWN_SKILLLIST)
	{
		if (MouseLButtonPush == false)
		{
			m_EventState = EVENT_NONE;
			return true;
		}
		return false;
	}

	return true;
}

bool SEASON3B::CNewUISkillList::UpdateKeyEvent()
{
	for (int i = 0; i < 9; ++i)
	{
		if (SEASON3B::IsPress('1' + i))
		{
			UseHotKey(i + 1);
		}
	}

	if (SEASON3B::IsPress('0'))
	{
		UseHotKey(0);
	}

	if (m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		if (SEASON3B::IsRepeat(VK_CONTROL))
		{
			for (int i = 0; i < 9; ++i)
			{
				if (SEASON3B::IsPress('1' + i))
				{
					SetHotKey(i + 1, m_iRenderSkillInfoType);
					SaveOptions();

					return false;
				}
			}

			if (SEASON3B::IsPress('0'))
			{
				SetHotKey(0, m_iRenderSkillInfoType);
				SaveOptions();

				return false;
			}
		}
	}

	if (SEASON3B::IsRepeat(VK_SHIFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			if (SEASON3B::IsPress('1' + i))
			{
				Hero->CurrentSkill = AT_PET_COMMAND_DEFAULT + i;
				return false;
			}
		}
	}

	return true;
}

bool SEASON3B::CNewUISkillList::IsArrayUp(BYTE bySkill)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == bySkill)
		{
			if (i == 0 || i > 5)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

bool SEASON3B::CNewUISkillList::IsArrayIn(BYTE bySkill)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == bySkill)
		{
			return true;
		}
	}

	return false;
}

void SEASON3B::CNewUISkillList::SetHotKey(int iHotKey, int iSkillType)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == iSkillType)
		{
			m_iHotKeySkillType[i] = -1;
			break;
		}
	}

	m_iHotKeySkillType[iHotKey] = iSkillType;
}

int SEASON3B::CNewUISkillList::GetHotKey(int iHotKey)
{
	return m_iHotKeySkillType[iHotKey];
}

int SEASON3B::CNewUISkillList::GetSkillIndex(int iSkillType)
{
	int iReturn = -1;
	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		if (CharacterAttribute->Skill[i] == iSkillType)
		{
			iReturn = i;
			break;
		}
	}

	return iReturn;
}

void SEASON3B::CNewUISkillList::UseHotKey(int iHotKey)
{
	if (m_iHotKeySkillType[iHotKey] != -1)
	{
		if (m_iHotKeySkillType[iHotKey] >= AT_PET_COMMAND_DEFAULT && m_iHotKeySkillType[iHotKey] < AT_PET_COMMAND_END)
		{
			if (Hero->m_pPet == NULL)
			{
				return;
			}
		}

		WORD wHotKeySkill = CharacterAttribute->Skill[m_iHotKeySkillType[iHotKey]];

		if (wHotKeySkill == 0)
		{
			return;
		}

		m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

		Hero->CurrentSkill = m_iHotKeySkillType[iHotKey];

		WORD bySkill = CharacterAttribute->Skill[Hero->CurrentSkill];


		if (
			g_pOption->IsAutoAttack() == true
			&& gMapManager.WorldActive != WD_6STADIUM
			&& gMapManager.InChaosCastle() == false
			&& (bySkill == AT_SKILL_TELEPORT || bySkill == AT_SKILL_TELEPORT_B))
		{
			SelectedCharacter = -1;
			Attacking = -1;
		}
	}
}

bool SEASON3B::CNewUISkillList::Update()
{
	if (IsArrayIn(Hero->CurrentSkill) == true)
	{
		if (IsArrayUp(Hero->CurrentSkill) == true)
		{
			m_bHotKeySkillListUp = true;
		}
		else
		{
			m_bHotKeySkillListUp = false;
		}
	}

	if (Hero->m_pPet == NULL)
	{
		if (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END)
		{
			Hero->CurrentSkill = 0;
		}
	}

	return true;
}
void SEASON3B::CNewUISkillList::RenderCurrentSkillAndHotSkillList()
{
	int i;
	float x, y, width, height;

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if (bySkillNumber > 0)
	{
		int iStartSkillIndex = 1;
		if (m_bHotKeySkillListUp)
		{
			iStartSkillIndex = 6;
		}
#if Ex700
		x = setPosCenterX(640) + 305.f;  // 190
		y = setPosDown(431) + 8.f;
		width = 22.5f; // 32
		height = 38; //38
#else
x = setPosCenterX(640) + 190; y = setPosDown(431); width = 32; height = 38;
#endif
		for (i = 0; i < 5; ++i)
		{
			x += width;

			int iIndex = iStartSkillIndex + i;
			if (iIndex == 10)
			{
				iIndex = 0;
			}

			if (m_iHotKeySkillType[iIndex] == -1)
			{
				continue;
			}

			if (m_iHotKeySkillType[iIndex] >= AT_PET_COMMAND_DEFAULT && m_iHotKeySkillType[iIndex] < AT_PET_COMMAND_END)
			{
				if (Hero->m_pPet == NULL)
				{
					continue;
				}
			}
#if Ex700
			float x1 = x + 3.7f;
			float y1 = y + 4.f;
			float w1 = width + 10.f;
			float h1 = height;

			RenderSkillIcon(m_iHotKeySkillType[iIndex], x + 6, y + 6, 20, 28);

			if (Hero->CurrentSkill == m_iHotKeySkillType[iIndex])
			{
				SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x1, y1, w1, h1);
			}
		}

		x = setPosCenterX(640) + 307.f; // 392
		y = setPosDown(431) + 14.f; // 437
		width = 20.f;
		height = 28.f;
#else
	if(Hero->CurrentSkill == m_iHotKeySkillType[iIndex])
			{
				SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
			}
			RenderSkillIcon(m_iHotKeySkillType[iIndex], x+6, y+6, 20, 28);
		}
		x = setPosCenterX(640) + 392; y = setPosDown(437); width = 20; height = 28;
#endif
		RenderSkillIcon(Hero->CurrentSkill, x, y, width, height);
	}
}
#if Ex700
bool SEASON3B::CNewUISkillList::Render()
{
	BYTE bySkillNumber = CharacterAttribute->SkillNumber;
	if (bySkillNumber > 0 && m_bSkillList)
	{
		std::vector<int> skills;
		skills.reserve(MAX_MAGIC);

		for (int i = 0; i < MAX_MAGIC; ++i)
		{
			int s = CharacterAttribute->Skill[i];
			if (s == 0 || (s >= AT_SKILL_STUN && s <= AT_SKILL_REMOVAL_BUFF))
				continue;
			BYTE t = SkillAttribute[s].SkillUseType;
			if (t == SKILL_USE_TYPE_MASTER || t == SKILL_USE_TYPE_MASTERLEVEL)
				continue;
			skills.push_back(i);
		}

		int totalSkills = (int)skills.size();
		int cols = 11;
		float width = SKILL_TILE_W;
		float height = SKILL_TILE_H;
		float totalWidth = width * cols;
		float baseX = (IsToolKit.GetPositionScreen() - totalWidth) / 2.0f;
		float baseY = setPosDown(390);

		float RenderX_Form = (IsToolKit.GetPositionScreen() - 280.f) / 2.0f;
		RenderBitmap(CNewUIMainFrameWindow::IMAGE_GAUGE_END_FORM,
			RenderX_Form, baseY - 28.f, 280.f, 60.f, 0.f, 0.f, 0.55f, 1.0);

		float GetPosYIconHober = 23.f;

		m_bRenderSkillInfo = false;

		for (int idx = 0; idx < totalSkills; ++idx)
		{
			int col = idx % cols;
			int row = idx / cols;
			float x = baseX + col * width;
			float y = baseY + row * height;

			float iconX = x + 2.8f;
			float iconY = y - GetPosYIconHober;
			float iconW = 20.f;
			float iconH = 28.f;

			bool isCurrentSkill = (skills[idx] == Hero->CurrentSkill);
			bool isHovered = SEASON3B::CheckMouseIn(iconX, iconY, iconW, iconH);

			if (isCurrentSkill || isHovered)
			{
				EnableAlphaTest();
				SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x + 0.9f, (y - GetPosYIconHober) - 1.4f, width, height);
				DisableAlphaBlend();
			}

			RenderSkillIcon(skills[idx], iconX, iconY, iconW, iconH);

			if (isHovered)
			{
				m_bRenderSkillInfo = true;
				m_iRenderSkillInfoType = skills[idx];
				m_iRenderSkillInfoPosX = x;
				m_iRenderSkillInfoPosY = baseY - 28.f;
			}
		}

		RenderPetSkill();
	}

	if (m_bRenderSkillInfo && m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER,
			UI2DEffectCallback, this, 0, 0);
		m_bRenderSkillInfo = false;
	}
	return true;
}
#else
bool SEASON3B::CNewUISkillList::Render()
{
	int i;
	float x, y, width, height;
	
	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if(bySkillNumber > 0)
	{
		if(m_bSkillList == true)
		{
			x = setPosCenterX(640) + 385; y = setPosDown(390); width = 32; height = 38;
			float fOrigX = setPosCenterX(640) + 385.f;
            int iSkillType  = 0;
			int iSkillCount = 0;

			for(i=0; i<MAX_MAGIC; ++i)
			{
                iSkillType = CharacterAttribute->Skill[i];

				if(iSkillType != 0 && (iSkillType < AT_SKILL_STUN || iSkillType > AT_SKILL_REMOVAL_BUFF))
				{
                    BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;
                  
                    if(bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
					{
                        continue;
					}

					if(iSkillCount == 18)
					{
						y -= height;
					}

					if(iSkillCount < 14)
					{
						int iRemainder = iSkillCount % 2;
						int iQuotient = iSkillCount / 2;

						if(iRemainder == 0)
						{
							x = fOrigX + iQuotient * width;
						}
						else
						{
							x = fOrigX - (iQuotient + 1) * width;
						}
					}
					else if(iSkillCount >= 14 && iSkillCount < 18)
					{
						x = fOrigX - (8 * width) - ((iSkillCount - 14) * width);
					}
					else
					{
						x = fOrigX - (12 * width) + ((iSkillCount - 17) * width);
					}

					iSkillCount++;

					if(i == Hero->CurrentSkill)
					{
						SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
					}
					else
					{
						SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y, width, height);	
					}
					
					RenderSkillIcon(i, x+6, y+6, 20, 28);
				}
			}
			RenderPetSkill();
		}
	}
	
	if(m_bRenderSkillInfo == true && m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);

		m_bRenderSkillInfo = false;
	}
	return true;
}
#endif




void SEASON3B::CNewUISkillList::RenderSkillInfo()
{
	::RenderSkillInfo(m_iRenderSkillInfoPosX + 15, m_iRenderSkillInfoPosY - 10, m_iRenderSkillInfoType);
}

float SEASON3B::CNewUISkillList::GetLayerDepth()
{
	return 5.2f;
}

WORD SEASON3B::CNewUISkillList::GetHeroPriorSkill()
{
	return m_wHeroPriorSkill;
}

void SEASON3B::CNewUISkillList::SetHeroPriorSkill(BYTE bySkill)
{
	m_wHeroPriorSkill = bySkill;
}

void SEASON3B::CNewUISkillList::RenderPetSkill()
{
	if (Hero->m_pPet == NULL)
	{
		return;
	}

	float x, y, width, height;
	x = setPosCenterX(640) + 353.f; y = setPosDown(352); width = 32; height = 38;
	for (int i = AT_PET_COMMAND_DEFAULT; i < AT_PET_COMMAND_END; ++i)
	{
		if (i == Hero->CurrentSkill)
		{
			SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
		}
		else
		{
			SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y, width, height);
		}

		RenderSkillIcon(i, x + 6, y + 6, 20, 28);
		x += width;
	}
}

void SEASON3B::CNewUISkillList::RenderSkillIcon(int iIndex, float x, float y, float width, float height, int TypeMuHelper)
{
	WORD bySkillType = CharacterAttribute->Skill[iIndex];

	if (TypeMuHelper == 1)
	{
		bySkillType = iIndex;
		if ((bySkillType == 0 || !gSkillManager.FindHeroSkill((ActionSkillType)bySkillType)))
		{
			return;
		}
	}

	if (iIndex >= AT_PET_COMMAND_DEFAULT)
	{
		bySkillType = iIndex;
	}

	bool bCantSkill = false;

	BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;
	int Skill_Icon = SkillAttribute[bySkillType].Magic_Icon;

	if (!gSkillManager.DemendConditionCheckSkill(bySkillType))
	{
		bCantSkill = true;
	}

	if (IsCanBCSkill(bySkillType) == false)
	{
		bCantSkill = true;
	}
	if (g_isCharacterBuff((&Hero->Object), eBuff_AddSkill) && bySkillUseType == SKILL_USE_TYPE_BRAND)
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_SPEAR && (Hero->Helper.Type<MODEL_HELPER + 2 || Hero->Helper.Type>MODEL_HELPER + 3) && Hero->Helper.Type != MODEL_HELPER + 37)
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_SPEAR && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;
		if ((iTypeL < ITEM_SPEAR || iTypeL >= ITEM_BOW) && (iTypeR < ITEM_SPEAR || iTypeR >= ITEM_BOW))
		{
			bCantSkill = true;
		}
	}

	if (bySkillType >= AT_SKILL_BLOCKING && bySkillType <= AT_SKILL_SWORD5 && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		bCantSkill = true;
	}

	if ((bySkillType == AT_SKILL_ICE_BLADE || (AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP + 4 >= bySkillType)) && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		bCantSkill = true;
	}
	int iEnergy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

	if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_PARTY_TELEPORT && PartyNumber <= 0)
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_PARTY_TELEPORT && (IsDoppelGanger1() || IsDoppelGanger2() || IsDoppelGanger3() || IsDoppelGanger4()))
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_DARK_HORSE ||
		bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_IMPROVED ||
		bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_ENHANCED)
	{
		BYTE byDarkHorseLife = 0;
		byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;
		if (byDarkHorseLife == 0 || Hero->Helper.Type != MODEL_HELPER + 4)
		{
			bCantSkill = true;

		}
	}

	if ((bySkillType == AT_SKILL_INFINITY_ARROW)
		|| (bySkillType == MASTER_SKILL_ADD_INFINITY_ARROW_IMPROVED)
		|| (bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER)
		|| (bySkillType == MASTER_SKILL_ADD_MAGIC_CIRCLE_ENHANCED)
		|| bySkillType == MASTER_SKILL_ADD_MAGIC_CIRCLE_IMPROVED)
	{
		if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
		{
			bCantSkill = true;
		}
		if (
			(g_isCharacterBuff((&Hero->Object), eBuff_InfinityArrow)) ||
			(g_isCharacterBuff((&Hero->Object), EFFECT_INFINITY_ARROW_IMPROVED)) //Mui ten vo tan Master
			|| (g_isCharacterBuff((&Hero->Object), eBuff_SwellOfMagicPower))
			|| (g_isCharacterBuff((&Hero->Object), EFFECT_MAGIC_CIRCLE_IMPROVED))
			|| (g_isCharacterBuff((&Hero->Object), EFFECT_MAGIC_CIRCLE_ENHANCED))
			)
		{
			bCantSkill = true;
		}
	}

	if (bySkillType == AT_SKILL_REDUCEDEFENSE
		|| bySkillType == MASTER_SKILL_ADD_FIRE_SLASH_IMPROVED
		|| bySkillType == MASTER_SKILL_ADD_FIRE_SLASH_ENHANCED
		)
	{
		DWORD Strength;
		const WORD wRequireStrength = 596;
		Strength = CharacterAttribute->ViewStrength + CharacterAttribute->AddStrength;
		if (Strength < wRequireStrength)
		{
			bCantSkill = true;
		}
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 && (iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) && (iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			bCantSkill = true;
		}
	}
	//ACuoi chan Skill images UI
	switch (bySkillType)
	{
		//case AT_SKILL_PIERCING:
	case AT_SKILL_PARALYZE:
	case MASTER_SKILL_ADD_ICE_ARROW_IMPROVED:
	{
		DWORD  Dexterity;
		const WORD wRequireDexterity = 646;
		Dexterity = CharacterAttribute->ViewDexterity + CharacterAttribute->AddDexterity;
		if (Dexterity < wRequireDexterity)
		{
			bCantSkill = true;
		}
	}
	break;
	}

	if (bySkillType == AT_SKILL_WHEEL
		|| bySkillType == MASTER_SKILL_ADD_TWISTING_SLASH_ENHANCED
		|| bySkillType == MASTER_SKILL_ADD_TWISTING_SLASH_IMPROVED1
		|| bySkillType == MASTER_SKILL_ADD_TWISTING_SLASH_IMPROVED2
		)
	{
		int iTypeL = CharacterMachine->Equipment[0].Type;
		int iTypeR = CharacterMachine->Equipment[1].Type;

		bool isLeftSword = (iTypeL >= 0 && !(iTypeL >= ITEM_STAFF && iTypeL < ITEM_STAFF + MAX_ITEM_INDEX));
		bool isRightStaff = (iTypeR >= ITEM_STAFF && iTypeR < ITEM_STAFF + MAX_ITEM_INDEX);
		bool isLeftStaff = (iTypeL >= ITEM_STAFF && iTypeL < ITEM_STAFF + MAX_ITEM_INDEX);

		if (!isLeftSword || isLeftStaff)
		{
			bCantSkill = true;
		}
	}

	if (gMapManager.InChaosCastle() == true)
	{
		if (bySkillType == AT_SKILL_DARK_HORSE ||
			bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_IMPROVED ||
			bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_ENHANCED ||
			bySkillType == AT_SKILL_RIDER || (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_TARGET))
		{
			bCantSkill = true;
		}
	}
	else
	{
		if (bySkillType == AT_SKILL_DARK_HORSE ||
			bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_IMPROVED ||
			bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_ENHANCED)
		{
			BYTE byDarkHorseLife = 0;
			byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;
			if (byDarkHorseLife == 0)
			{
				bCantSkill = true;
			}
		}
	}

	int iCharisma = CharacterAttribute->ViewLeadership + CharacterAttribute->AddCharisma;

	if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy, iCharisma))
	{
		bCantSkill = true;
	}


	if (!g_CMonkSystem.IsSwordformGlovesUseSkill(bySkillType))
	{
		bCantSkill = true;
	}
	if (g_CMonkSystem.IsRideNotUseSkill(bySkillType, Hero->Helper.Type))
	{
		bCantSkill = true;
	}

	ITEM* pLeftRing = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
	ITEM* pRightRing = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];

	if (g_CMonkSystem.IsChangeringNotUseSkill(pLeftRing->Type, pRightRing->Type, pLeftRing->Level, pRightRing->Level)
		&& (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER))
	{
		bCantSkill = true;
	}

	float fU, fV;
	int iKindofSkill = 0;

	if (g_csItemOption.Special_Option_Check() == false && (
		bySkillType == AT_SKILL_ICE_BLADE
		|| bySkillType == MASTER_SKILL_ADD_POWER_SLASH_IMPROVED
		|| (AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP + 4 >= bySkillType)))
	{
		bCantSkill = true;
	}

	if (g_csItemOption.Special_Option_Check(1) == false && (
		bySkillType == AT_SKILL_CROSSBOW ||
		bySkillType == MASTER_SKILL_ADD_TRIPLE_SHOT_IMPROVED ||
		bySkillType == MASTER_SKILL_ADD_TRIPLE_SHOT_ENHANCED ||
		(AT_SKILL_MANY_ARROW_UP <= bySkillType && AT_SKILL_MANY_ARROW_UP + 4 >= bySkillType)))
		bCantSkill = true;

	if (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_END)
	{
		fU = ((bySkillType - AT_PET_COMMAND_DEFAULT) % 8) * width / 256.f;
		fV = ((bySkillType - AT_PET_COMMAND_DEFAULT) / 8) * height / 256.f;
		iKindofSkill = KOS_COMMAND;
	}
	else if (bySkillType == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		fU = 4 * width / 256.f;
		fV = 0.f;
		iKindofSkill = KOS_COMMAND;
	}
	else if ((bySkillType >= AT_SKILL_ALICE_DRAINLIFE && bySkillType <= AT_SKILL_ALICE_THORNS))
	{
		fU = ((bySkillType - AT_SKILL_ALICE_DRAINLIFE) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType >= AT_SKILL_ALICE_SLEEP && bySkillType <= AT_SKILL_ALICE_BLIND)
	{
		fU = ((bySkillType - AT_SKILL_ALICE_SLEEP + 4) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_ALICE_BERSERKER)
	{
		fU = 10 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType >= AT_SKILL_ALICE_WEAKNESS && bySkillType <= AT_SKILL_ALICE_ENERVATION)
	{
		fU = (bySkillType - AT_SKILL_ALICE_WEAKNESS + 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType >= AT_SKILL_SUMMON_EXPLOSION && bySkillType <= AT_SKILL_SUMMON_REQUIEM)
	{
		fU = ((bySkillType - AT_SKILL_SUMMON_EXPLOSION + 6) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_SUMMON_POLLUTION)
	{
		fU = 11 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_BLOW_OF_DESTRUCTION)
	{
		fU = 7 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_GAOTIC)
	{
		fU = 3 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_RECOVER)
	{
		fU = 9 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_MULTI_SHOT)
	{
		if (gCharacterManager.GetEquipedBowType_Skill() == BOWTYPE_NONE)
		{
			bCantSkill = true;
		}

		fU = 0 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_FLAME_STRIKE)
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 && (iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) && (iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			bCantSkill = true;
		}

		fU = 1 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_GIGANTIC_STORM)
	{
		fU = 2 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_LIGHTNING_SHOCK)
	{
		fU = 2 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (AT_SKILL_LIGHTNING_SHOCK_UP <= bySkillType && bySkillType <= AT_SKILL_LIGHTNING_SHOCK_UP + 4)
	{
		fU = 6 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER)
	{
		fU = 8 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillUseType == AT_SKILL_FIREBALL)
	{
		fU = (width / 256.f) * (Skill_Icon % 12);
		fV = (height / 256.f) * ((Skill_Icon / 12) + 4);
		iKindofSkill = KOS_SKILL2;
	}

	else if (bySkillType >= AT_SKILL_THRUST)
	{
		fU = ((bySkillType - 260) % 12) * width / 256.f;
		fV = ((bySkillType - 260) / 12) * height / 256.f;
		iKindofSkill = KOS_SKILL3;
	}

	else if (bySkillType >= 57)
	{
		fU = ((bySkillType - 57) % 8) * width / 256.f;
		fV = ((bySkillType - 57) / 8) * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else
	{
		fU = ((bySkillType - 1) % 8) * width / 256.f;
		fV = ((bySkillType - 1) / 8) * height / 256.f;
		iKindofSkill = KOS_SKILL1;
	}
	int iSkillIndex = 0;
	switch (iKindofSkill)
	{
	case KOS_COMMAND:
	{
		iSkillIndex = IMAGE_COMMAND;
	}break;
	case KOS_SKILL1:
	{
		iSkillIndex = IMAGE_SKILL1;
	}break;
	case KOS_SKILL2:
	{
		iSkillIndex = IMAGE_SKILL2;
	}break;

	case KOS_SKILL3:
	{
		iSkillIndex = IMAGE_SKILL3;
	}
	break;

	}
	if (bCantSkill == true)
	{
		iSkillIndex += 6;
	}
	#if Ex700
	if (iSkillIndex != 0)
	{
		if (bySkillUseType == AT_SKILL_FIREBALL) //Skill Master
		{
			JCCoord GetXY;
			float MaxIconW = 18.0f;
			float MaxIconH = 22.0f;
			if (g_pMasterSkillTreeInterface->GetXYImgMaster(&GetXY, bySkillType))
			{
				if (bCantSkill == true)
				{
					SEASON3B::RenderImage(BITMAP_INTERFACE_MASTER_BEGIN + 3, x, y, MaxIconW, MaxIconH, GetXY.CalcX, GetXY.CalcY, 0.0390625, 0.053710938); //Non
				}
				else
				{
					SEASON3B::RenderImage(BITMAP_INTERFACE_MASTER_BEGIN + 2, x, y, MaxIconW, MaxIconH, GetXY.CalcX, GetXY.CalcY, 0.0390625, 0.053710938);
				}
			}
		}
		else
		{
			RenderBitmap(iSkillIndex, x, y, width - 2, height - 6, fU, fV, width / 256.f, height / 256.f);
		}
	}
	#else
	if (iSkillIndex != 0)
	{
		if (bySkillUseType == AT_SKILL_FIREBALL) //Skill Master
		{
			JCCoord GetXY;
			#if(NEW_MASTER_SKILL_TREE)
			if (g_pMasterSkillTreeInterface->GetXYImgMaster(&GetXY, bySkillType))
			{
				if (bCantSkill == true)
				{
					SEASON3B::RenderImage(BITMAP_INTERFACE_MASTER_BEGIN + 3, x, y, 20.0, 28.0, GetXY.CalcX, GetXY.CalcY, 0.0390625, 0.053710938); //Non
				}
				else
				{
					SEASON3B::RenderImage(BITMAP_INTERFACE_MASTER_BEGIN + 2, x, y, 20.0, 28.0, GetXY.CalcX, GetXY.CalcY, 0.0390625, 0.053710938);
				}
			}
			#endif
		}
		else
		{
			RenderBitmap(iSkillIndex, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
		}
	}
	#endif
	int iHotKey = -1;
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == iIndex)
		{
			iHotKey = i;
			break;
		}
	}

	if (iHotKey != -1)
	{
		glColor3f(1.f, 0.9f, 0.8f);
		#if Ex700
		SEASON3B::RenderNumber(x + 8.5f, y - 8.f, iHotKey, 0.75f);
		#else
		SEASON3B::RenderNumber(x+20, y+20, iHotKey);
		#endif
		glColor3f(1.f, 1.f, 1.f);
	}

	if ((bySkillType == AT_SKILL_GIANTSWING
		|| bySkillType == MASTER_SKILL_ADD_CHAIN_DRIVER_IMPROVED
		|| bySkillType == MASTER_SKILL_ADD_CHAIN_DRIVER_ENHANCED
		|| bySkillType == AT_SKILL_DRAGON_KICK
		|| bySkillType == MASTER_SKILL_ADD_DRAGON_SLAYER_IMPROVED
		|| bySkillType == MASTER_SKILL_ADD_DRAGON_SLAYER_ENHANCED
		|| bySkillType == AT_SKILL_PHOENIX_SHOT
		|| bySkillType == MASTER_SKILL_ADD_DRAGON_LORE_IMPROVED
		|| bySkillType == MASTER_SKILL_ADD_DRAGON_LORE_ENHANCED
		|| bySkillType == AT_SKILL_DRAGON_LOWER) && (bCantSkill))
		return;

	if ((bySkillType != AT_SKILL_INFINITY_ARROW)
		&& (bySkillType != MASTER_SKILL_ADD_INFINITY_ARROW_IMPROVED)
		&& (bySkillType != AT_SKILL_SWELL_OF_MAGICPOWER)
		&& (bySkillType != MASTER_SKILL_ADD_MAGIC_CIRCLE_IMPROVED)
		&& (bySkillType != MASTER_SKILL_ADD_MAGIC_CIRCLE_ENHANCED))

	{
		RenderSkillDelay(iIndex, x, y, width, height);
	}
}

void SEASON3B::CNewUISkillList::RenderSkillDelay(int iIndex, float x, float y, float width, float height)
{
	int iSkillDelay = CharacterAttribute->SkillDelay[iIndex];
	if (iSkillDelay > 0)
	{
		int iSkillType = CharacterAttribute->Skill[iIndex];

		if (iSkillType == AT_SKILL_PLASMA_STORM_FENRIR)
		{
			if (!CheckAttack())
			{
				return;
			}
		}

		int iSkillMaxDelay = SkillAttribute[iSkillType].Delay;

		auto fPersent = (float)(iSkillDelay / (float)iSkillMaxDelay);

		EnableAlphaTest();
		glColor4f(1.f, 0.5f, 0.5f, 0.5f);
		float fdeltaH = height * fPersent;
		RenderColor(x, y + height - fdeltaH, width, fdeltaH);
		EndRenderColor();
	}
}

bool SEASON3B::CNewUISkillList::IsSkillListUp()
{
	return m_bHotKeySkillListUp;
}

void SEASON3B::CNewUISkillList::ResetMouseLButton()
{
	MouseLButton = false;
	MouseLButtonPop = false;
	MouseLButtonPush = false;
}

void SEASON3B::CNewUISkillList::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	if (pClass)
	{
		auto* pSkillList = (CNewUISkillList*)(pClass);
		pSkillList->RenderSkillInfo();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetPreExp_Wide(__int64 dwPreExp)
{
	m_loPreExp = dwPreExp;
}

void SEASON3B::CNewUIMainFrameWindow::SetGetExp_Wide(__int64 dwGetExp)
{
	m_loGetExp = dwGetExp;

	if (m_loGetExp > 0)
	{
		m_bExpEffect = true;
		m_dwExpEffectTime = timeGetTime();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetPreExp(DWORD dwPreExp)
{
	m_dwPreExp = dwPreExp;
}

void SEASON3B::CNewUIMainFrameWindow::SetGetExp(DWORD dwGetExp)
{
	m_dwGetExp = dwGetExp;

	if (m_dwGetExp > 0)
	{
		m_bExpEffect = true;
		m_dwExpEffectTime = timeGetTime();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetBtnState(int iBtnType, bool bStateDown)
{
	switch (iBtnType)
	{
	case MAINFRAME_BTN_PARTCHARGE:
	{
		if (bStateDown)
		{
			m_BtnCShop.UnRegisterButtonState();
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CSHOP, 2);
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CSHOP, 3);
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CSHOP, 2);
			m_BtnCShop.ChangeImgIndex(IMAGE_MENU_BTN_CSHOP, 2);
		}
		else
		{
			m_BtnCShop.UnRegisterButtonState();
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CSHOP, 0);
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CSHOP, 1);
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CSHOP, 2);
			m_BtnCShop.ChangeImgIndex(IMAGE_MENU_BTN_CSHOP, 0);
		}
	}
	break;

	case MAINFRAME_BTN_CHAINFO:
	{
		if (bStateDown)
		{
			m_BtnChaInfo.UnRegisterButtonState();
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CHAINFO, 2);
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CHAINFO, 3);
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CHAINFO, 2);
			m_BtnChaInfo.ChangeImgIndex(IMAGE_MENU_BTN_CHAINFO, 2);

		}
		else
		{
			m_BtnChaInfo.UnRegisterButtonState();
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CHAINFO, 0);
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CHAINFO, 1);
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CHAINFO, 2);
			m_BtnChaInfo.ChangeImgIndex(IMAGE_MENU_BTN_CHAINFO, 0);
		}
	}
	break;
	case MAINFRAME_BTN_MYINVEN:
	{
		if (bStateDown)
		{
			m_BtnMyInven.UnRegisterButtonState();
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_MYINVEN, 2);
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_MYINVEN, 3);
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_MYINVEN, 2);
			m_BtnMyInven.ChangeImgIndex(IMAGE_MENU_BTN_MYINVEN, 2);
		}
		else
		{
			m_BtnMyInven.UnRegisterButtonState();
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_MYINVEN, 0);
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_MYINVEN, 1);
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_MYINVEN, 2);
			m_BtnMyInven.ChangeImgIndex(IMAGE_MENU_BTN_MYINVEN, 0);
		}
	}
	break;
	case MAINFRAME_BTN_FRIEND:
	{
		if (bStateDown)
		{
			m_BtnFriend.UnRegisterButtonState();
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_FRIEND, 2);
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_FRIEND, 3);
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_FRIEND, 2);
			m_BtnFriend.ChangeImgIndex(IMAGE_MENU_BTN_FRIEND, 2);
		}
		else
		{
			m_BtnFriend.UnRegisterButtonState();
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_FRIEND, 0);
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_FRIEND, 1);
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_FRIEND, 2);
			m_BtnFriend.ChangeImgIndex(IMAGE_MENU_BTN_FRIEND, 0);
		}
	}
	break;
	case MAINFRAME_BTN_WINDOW:
	{
		if (bStateDown)
		{
			m_BtnWindow.UnRegisterButtonState();
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_WINDOW, 2);
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_WINDOW, 3);
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_WINDOW, 2);
			m_BtnWindow.ChangeImgIndex(IMAGE_MENU_BTN_WINDOW, 2);
		}
		else
		{
			m_BtnWindow.UnRegisterButtonState();
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_WINDOW, 0);
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_WINDOW, 1);
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_WINDOW, 2);
			m_BtnWindow.ChangeImgIndex(IMAGE_MENU_BTN_WINDOW, 0);
		}
	}
	break;
	}
}
