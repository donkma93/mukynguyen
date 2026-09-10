#include "Stdafx.h"
#include "NewUIChangeClass.h"
#include "CharacterManager.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

SEASON3B::CNewUIChangeClass::CNewUIChangeClass()
{
	m_pNewUIMng = nullptr;
	m_Pos.x = 0;
	m_Pos.y = 0;
	CurrentClass = 0;
	IsToolKit.NextClass = 0;
	m_bDragging = false;
	m_iLastMouseX = 0;

	WINDOW_WIDTH = 288;
	WINDOW_HEIGHT = 200;
}

SEASON3B::CNewUIChangeClass::~CNewUIChangeClass()
{
	Release();
}

bool SEASON3B::CNewUIChangeClass::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CHANGECLASS, this);

	LoadImages();

	SetPos(x, y);

	InitButtons();	
	InitLeftRender();
	InitRightRender();

	Show(false);

	return true;
}

void SEASON3B::CNewUIChangeClass::InitLeftRender()
{
	m_PhotoCharLeft.Init(0);
	m_PhotoCharLeft.SetOption(UIPHOTOVIEWER_CANCONTROL);
	m_PhotoCharLeft.SetAngle(100.f);
	m_PhotoCharLeft.SetZoom(1.0f);
	m_PhotoCharLeft.SetAutoupdatePlayer(FALSE);
	m_PhotoCharLeft.SetPosition(m_Pos.x + 27, m_Pos.y + 22);
	m_PhotoCharLeft.SetSize(78, 112);
	m_PhotoCharLeft.SetClass(CurrentClass);
	m_PhotoCharLeft.SetAnimation(AT_STAND1);

	CHARACTER* c = m_PhotoCharLeft.GetPhotoChar();
	int skin = gCharacterManager.GetSkinModelIndex(CurrentClass);

	c->BodyPart[BODYPART_HELM].Type = MODEL_BODY_HELM + skin;
	c->BodyPart[BODYPART_ARMOR].Type = MODEL_BODY_ARMOR + skin;
	c->BodyPart[BODYPART_PANTS].Type = MODEL_BODY_PANTS + skin;
	c->BodyPart[BODYPART_BOOTS].Type = MODEL_BODY_BOOTS + skin;
	c->BodyPart[BODYPART_GLOVES].Type = MODEL_BODY_GLOVES + skin;

	SetChangeClass(c);
}

void SEASON3B::CNewUIChangeClass::InitRightRender()
{
	m_PhotoCharRight.Init(0);
	m_PhotoCharRight.SetOption(UIPHOTOVIEWER_CANCONTROL);
	m_PhotoCharRight.SetAngle(100.f);
	m_PhotoCharRight.SetZoom(1.0f);
	m_PhotoCharRight.SetAutoupdatePlayer(FALSE);
	m_PhotoCharRight.SetPosition(m_Pos.x + 169, m_Pos.y + 22);
	m_PhotoCharRight.SetSize(78, 112);
	m_PhotoCharRight.SetClass(CurrentClass);
	m_PhotoCharRight.SetAnimation(AT_STAND1);

	CHARACTER* c = m_PhotoCharRight.GetPhotoChar();
	int skin = gCharacterManager.GetSkinModelIndex(CurrentClass);

	c->BodyPart[BODYPART_HELM].Type = MODEL_BODY_HELM + skin;
	c->BodyPart[BODYPART_ARMOR].Type = MODEL_BODY_ARMOR + skin;
	c->BodyPart[BODYPART_PANTS].Type = MODEL_BODY_PANTS + skin;
	c->BodyPart[BODYPART_BOOTS].Type = MODEL_BODY_BOOTS + skin;
	c->BodyPart[BODYPART_GLOVES].Type = MODEL_BODY_GLOVES + skin;

	SetChangeClass(c);
}

void SEASON3B::CNewUIChangeClass::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}
void SEASON3B::CNewUIChangeClass::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

bool SEASON3B::CNewUIChangeClass::Update()
{
	return true;
}
bool SEASON3B::CNewUIChangeClass::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	g_dwActiveUIID = 0;
	m_PhotoCharLeft.DoMouseAction();

	if (m_PhotoCharLeft.GetCurrentZoom() > 1.02f)
	{
		m_PhotoCharLeft.SetZoom(1.02f);
	}

	m_PhotoCharRight.DoMouseAction();

	if (m_PhotoCharRight.GetCurrentZoom() > 1.02f)
	{
		m_PhotoCharRight.SetZoom(1.02f);
	}

	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 40, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_CHANGECLASS);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH - 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUIChangeClass::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_CHANGECLASS);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIChangeClass::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIChangeClass::GetKeyEventOrder()
{
	return 3.4;
}

void SEASON3B::CNewUIChangeClass::RenderLeft()
{
	switch (CharacterAttribute->Class)
	{
		case 0: case 8: case 24: CurrentClass = 0; break;
		case 1: case 9: case 25: CurrentClass = 1; break;
		case 2: case 10: case 26: CurrentClass = 2; break;
		case 3: case 27: CurrentClass = 3; break;
		case 4: case 28: CurrentClass = 4; break;
		case 5: case 13: case 29: CurrentClass = 5; break;
		case 6: case 30: CurrentClass = 6; break;
	}

	RenderBitmap(IMAGE_HD_LOGO_0, m_Pos.x + 22.f, m_Pos.y + 42.f, 78, 112, 0.f, 0.f, 256.f / 256.f, 256.f / 384.f);

	m_PhotoCharLeft.SetClass(CharacterAttribute->Class);

	CHARACTER* c = m_PhotoCharLeft.GetPhotoChar();
	int skin = gCharacterManager.GetSkinModelIndex(IsToolKit.NextClass);

	c->BodyPart[BODYPART_HEAD].Type = MODEL_BODY_HELM + skin;
	c->BodyPart[BODYPART_HELM].Type = MODEL_BODY_HELM + skin;
	c->BodyPart[BODYPART_ARMOR].Type = MODEL_BODY_ARMOR + skin;
	c->BodyPart[BODYPART_PANTS].Type = MODEL_BODY_PANTS + skin;
	c->BodyPart[BODYPART_BOOTS].Type = MODEL_BODY_BOOTS + skin;
	c->BodyPart[BODYPART_GLOVES].Type = MODEL_BODY_GLOVES + skin;

	DeleteCloth(c, &c->Object);
	SetChangeClass(c);

	m_PhotoCharLeft.Render();

	IsToolKit.ThisFont(m_Pos.x + 22, m_Pos.y + 146, 0xFFEECCE1, 0x420202EF, 78, 0, RT3_SORT_CENTER, "%s", gCharacterManager.GetCharacterClassText(CharacterAttribute->Class));
	g_pUIForm->RenderBack(m_Pos.x + 20, m_Pos.y + 40, 83, 120, 0);

	m_Btn[0].Render();
}

void SEASON3B::CNewUIChangeClass::RenderRight()
{
	if (IsToolKit.NextClass == CurrentClass)
	{
		for (int i = 0; i <= 6; ++i)
		{
			if (i != CurrentClass)
			{
				IsToolKit.NextClass = i;
				break;
			}
		}
	}

	RenderBitmap(IMAGE_HD_LOGO_0, m_Pos.x + 169.f, m_Pos.y + 42.f, 78, 112, 0.f, 0.f, 256.f / 256.f, 256.f / 384.f);

	m_PhotoCharRight.SetClass(IsToolKit.NextClass);

	CHARACTER* c = m_PhotoCharRight.GetPhotoChar();
	int skin = gCharacterManager.GetSkinModelIndex(IsToolKit.NextClass);

	c->BodyPart[BODYPART_HEAD].Type   = MODEL_BODY_HELM  + skin;
	c->BodyPart[BODYPART_HELM].Type = MODEL_BODY_HELM + skin;
	c->BodyPart[BODYPART_ARMOR].Type = MODEL_BODY_ARMOR + skin;
	c->BodyPart[BODYPART_PANTS].Type = MODEL_BODY_PANTS + skin;
	c->BodyPart[BODYPART_BOOTS].Type = MODEL_BODY_BOOTS + skin;
	c->BodyPart[BODYPART_GLOVES].Type = MODEL_BODY_GLOVES + skin;

	DeleteCloth(c, &c->Object);
	SetChangeClass(c);

	m_PhotoCharRight.Render();

	IsToolKit.ThisFont(m_Pos.x + 169, m_Pos.y + 146, 0xFFEECCE1, 0x420202EF, 78, 0, RT3_SORT_CENTER, "%s", gCharacterManager.GetCharacterClassText(IsToolKit.NextClass));
	g_pUIForm->RenderBack(m_Pos.x + 167, m_Pos.y + 40, 83, 120, 0);

	m_Btn[1].Render();
}



void SEASON3B::CNewUIChangeClass::RenderTextInfo()
{
	DWORD mColor = 0x00FF90FF;

	if (IsToolKit.NextClass < 0 || IsToolKit.NextClass >= 7)
		return;

	static const int textIndex[][3] = {
		{ 3231, 3232, 3233 },
		{ 3234, 3235, 3236 },
		{ 3237, 3238, -1 },
		{ 3239, 3240, 3241 },
		{ 3242, 3243, 3244 },
		{ 3245, 3246, -1 },
		{ 3247, 3248, -1 }
	};

	int yOffset = 165;

	for (int i = 0; i < 3; ++i)
	{
		int idx = textIndex[IsToolKit.NextClass][i];
		if (idx == -1)
			continue;

		IsToolKit.ThisFont(m_Pos.x + 20, m_Pos.y + yOffset, mColor, 0, 0, 0, RT3_SORT_LEFT, GlobalText[idx]);
		yOffset += 11;
	}
}


bool SEASON3B::CNewUIChangeClass::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, GlobalText[3789]);

	RenderLeft();

	RenderRight();

	g_pUIForm->RenderBack(m_Pos.x + 10, m_Pos.y + 160, 252, 85);

	RenderTextInfo();

	RenderImage(CNewUIMasterSkillTree::IMAGE_MASTER_INTERFACE + 6, m_Pos.x + 121, m_Pos.y + 70, 28.f, 7.f);

	m_Btn[2].Render();

	IsToolKit.ThisFont(m_Pos.x + 20, m_Pos.y + 211, 0xFFDE26FF, 0, 270, 0, RT3_SORT_LEFT, "%s %s %s", GlobalText[3201], GlobalText[3230], "WCoinC");
	IsToolKit.ThisFont(m_Pos.x + 20, m_Pos.y + 211, 0xFFDE26FF, 0, 270, 0, RT3_SORT_LEFT, "%s %s", GlobalText[3201], GlobalText[3230]);
	IsToolKit.ThisFont(m_Pos.x + 20, m_Pos.y + 211, 0xFFFFFFFF, 0, 270, 0, RT3_SORT_LEFT, "%s", GlobalText[3201]);

	IsToolKit.ThisFont(m_Pos.x + 20, m_Pos.y + 222, 0xFF3300FF, 0, 270, 0, RT3_SORT_LEFT, GlobalText[3202]);

	DisableAlphaBlend();
	return true;
}
void SEASON3B::CNewUIChangeClass::InitButtons()
{
	g_pUIForm->SetButtonInfo(&m_Btn[2], CNewUIResetChange::IMAGE_IGS_BUTTON, m_Pos.x + 109, m_Pos.y + 90, 52, 26, 1, 0, 1, 1u, GlobalText[1940], "", 0);

	m_Btn[0].ChangeButtonImgState(true, CNewUINPCDialogue::IMAGE_ND_BTN_L);
	m_Btn[0].ChangeButtonInfo(m_Pos.x + 151, m_Pos.y + 140, 17, 18);
	m_Btn[1].ChangeButtonImgState(true, CNewUINPCDialogue::IMAGE_ND_BTN_R);
	m_Btn[1].ChangeButtonInfo(m_Pos.x + 247, m_Pos.y + 140, 17, 18);
}

bool SEASON3B::CNewUIChangeClass::BtnProcess()
{
	if (m_Btn[0].UpdateMouseEvent()) 
	{
		int temp = IsToolKit.NextClass;
		while (temp > 0)
		{
			temp--;
			if (temp != CurrentClass)
			{
				IsToolKit.NextClass = temp;
				PlayBuffer(SOUND_CLICK01);
				break;
			}
		}

		if (IsToolKit.NextClass == 0) m_Btn[0].Lock(); else m_Btn[0].UnLock();
		if (IsToolKit.NextClass < 6) m_Btn[1].UnLock();
		return true;
	}
	else if (m_Btn[1].UpdateMouseEvent()) 
	{
		int temp = IsToolKit.NextClass;
		while (temp < 6)
		{
			temp++;
			if (temp != CurrentClass)
			{
				IsToolKit.NextClass = temp;
				PlayBuffer(SOUND_CLICK01);
				break;
			}
		}

		if (IsToolKit.NextClass == 6) m_Btn[1].Lock(); else m_Btn[1].UnLock();
		if (IsToolKit.NextClass > 0) m_Btn[0].UnLock();
		return true;
	}

	if (m_Btn[2].UpdateMouseEvent())
	{
		CreateMessageBox(MSGBOX_LAYOUT_CLASS(CChangeClassConfirmLayout));
		g_pNewUISystem->Hide(INTERFACE_CHANGECLASS);
		return true;
	}

	return false;
}


void SEASON3B::CNewUIChangeClass::LoadImages()
{
	LoadBitmap("Interface\\ACuoi\\Class-1.jpg", IMAGE_HD_LOGO_0, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("ACuoi\\BgID.tga", IMAGE_HD_LOGO_1, GL_LINEAR);
}

void SEASON3B::CNewUIChangeClass::UnloadImages()
{
	DeleteBitmap(IMAGE_HD_LOGO_0);
	DeleteBitmap(IMAGE_HD_LOGO_1);
}