// NewUIBuffWindow.cpp: implementation of the CNewUIBuffWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIBuffWindow.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "ZzzTexture.h"
#include "ZzzInventory.h"
#include "UIControls.h"
#include "NewUICommonMessageBox.h"
#include <ThangCuoi/CustomIconBuff.h>
#include <ThangCuoi/ZzzToolKit.h>

using namespace SEASON3B;

namespace
{
	const float BUFF_IMG_WIDTH = 20.0f;
	const float BUFF_IMG_HEIGHT = 28.0f;
	const int BUFF_MAX_LINE_COUNT = 16;
	const int BUFF_IMG_SPACE = 7;
};

SEASON3B::CNewUIBuffWindow::CNewUIBuffWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
}

SEASON3B::CNewUIBuffWindow::~CNewUIBuffWindow()
{
	Release();
}

bool SEASON3B::CNewUIBuffWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_BUFF_WINDOW, this);

	SetPos(x, y);

	LoadImages();

	Show(true);

	return true;
}

void SEASON3B::CNewUIBuffWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIBuffWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void SEASON3B::CNewUIBuffWindow::SetPos(int iScreenWidth)
{
	if (iScreenWidth == 640) {
		SetPos(220, 15);
	}
	//else if (iScreenWidth == 450) {
	//	SetPos(125, 15);
	//}
	//else if (iScreenWidth == 373) {
	//	SetPos(86, 15);
	//}
	//else if (iScreenWidth == 260) {
	//	SetPos(30, 15);
	//}
	//else {
	//	SetPos(220, 15);
	//}
}

void SEASON3B::CNewUIBuffWindow::BuffSort(std::list<eBuffState>& buffstate)
{
	OBJECT* pHeroObject = &Hero->Object;

	int iBuffSize = g_CharacterBuffSize(pHeroObject);

	for (int i = 0; i < iBuffSize; ++i)
	{
		eBuffState eBuffType = g_CharacterBuff(pHeroObject, i);

		if (SetDisableRenderBuff(eBuffType))	continue;

		if (eBuffType != eBuffNone) {
			eBuffClass eBuffClassType = g_IsBuffClass(eBuffType);

			if (eBuffClassType == eBuffClass_Buff) {
				buffstate.push_front(eBuffType);
			}
			else if (eBuffClassType == eBuffClass_DeBuff) {
				buffstate.push_back(eBuffType);
			}
			else {
				assert(!"SetDisableRenderBuff");
			}
		}
	}
}

bool SEASON3B::CNewUIBuffWindow::SetDisableRenderBuff(const eBuffState& _BuffState)
{
	switch (_BuffState)
	{
#ifdef PBG_ADD_PKSYSTEM_INGAMESHOP
	case eDeBuff_MoveCommandWin:
#endif //PBG_ADD_PKSYSTEM_INGAMESHOP
	case eDeBuff_FlameStrikeDamage:
	case eDeBuff_GiganticStormDamage:
	case eDeBuff_LightningShockDamage:
	case eDeBuff_Discharge_Stamina:
		return true;
	default:
		return false;
	}
	return false;
}

bool SEASON3B::CNewUIBuffWindow::UpdateMouseEvent()
{
	float x = 0.0f, y = 0.0f;
	int buffwidthcount = 0, buffheightcount = 0;

	std::list<eBuffState> buffstate;
	BuffSort(buffstate);

	std::list<eBuffState>::iterator iter;
	for (iter = buffstate.begin(); iter != buffstate.end(); )
	{
		auto tempiter = iter;
		++iter;
		eBuffState buff = (*tempiter);

		x = m_Pos.x + (buffwidthcount * (BUFF_IMG_WIDTH - BUFF_IMG_SPACE));
		y = m_Pos.y + (buffheightcount * (BUFF_IMG_HEIGHT - BUFF_IMG_SPACE));

		if (SEASON3B::CheckMouseIn(x, y, BUFF_IMG_WIDTH, BUFF_IMG_HEIGHT))
		{
			if (buff == eBuff_InfinityArrow)
			{
				if (SEASON3B::IsRelease(VK_RBUTTON))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CInfinityArrowCancelMsgBoxLayout));
				}
			}
			else if (buff == eBuff_SwellOfMagicPower)
			{
				if (SEASON3B::IsRelease(VK_RBUTTON))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CBuffSwellOfMPCancelMsgBoxLayOut));
				}
			}

			return false;
		}

		if (++buffwidthcount >= 16) {
			buffwidthcount = 0;
			++buffheightcount;
		}
	}
	return true;
}

bool SEASON3B::CNewUIBuffWindow::UpdateKeyEvent()
{
	return true;
}

bool SEASON3B::CNewUIBuffWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIBuffWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderBuffStatus(BUFF_RENDER_ICON);

	RenderBuffStatus(BUFF_RENDER_TOOLTIP);

	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIBuffWindow::RenderBuffStatus(BUFF_RENDER renderstate)
{
	OBJECT* pHeroObject = &Hero->Object;

	float x = 0.0f, y = 0.0f;
	int buffwidthcount = 0, buffheightcount = 0;

	std::list<eBuffState> buffstate;
	BuffSort(buffstate);

	std::list<eBuffState>::iterator iter;
	int IndexTime = 0;

	for (iter = buffstate.begin(); iter != buffstate.end(); )
	{
		auto tempiter = iter;
		++iter;
		eBuffState buff = (*tempiter);

		x = (m_Pos.x + 100) + (buffwidthcount * (BUFF_IMG_WIDTH - BUFF_IMG_SPACE));
		y = (m_Pos.y - 13) + (buffheightcount * (BUFF_IMG_HEIGHT - BUFF_IMG_SPACE));

		if (renderstate == BUFF_RENDER_ICON)
		{
			RenderBuffIcon(buff, x, y, BUFF_IMG_WIDTH, BUFF_IMG_HEIGHT);
			if (gIconBuff.UserBuff[IndexTime].Index != NULL)
			{
				if (gIconBuff.UserBuff[IndexTime].Index != 28)
				{
					float pBuffHp = 100.0f;

					if (gIconBuff.UserBuff[IndexTime].MaxTime > 0 && gIconBuff.UserBuff[IndexTime].Time > 0)
					{
						pBuffHp = (float)(gIconBuff.UserBuff[IndexTime].Time) * 100.0f / (float)(gIconBuff.UserBuff[IndexTime].MaxTime);
					}

					EnableAlphaTest();
					glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
					RenderColor(x, (y + BUFF_IMG_HEIGHT) - 12, ((BUFF_IMG_WIDTH - 9) * pBuffHp / 100.0f), 2.0f, 0.0f, 0);
					EnableAlphaTest();
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}
		else if (renderstate == BUFF_RENDER_TOOLTIP)
		{
			//ACuoi Buff Icon
			if (SEASON3B::CheckMouseIn(x, y, BUFF_IMG_WIDTH - 9, BUFF_IMG_HEIGHT - 10)) 
			{
				float fTooltip_x = x + (BUFF_IMG_WIDTH / 2);
				float fTooltip_y = y + BUFF_IMG_WIDTH + 40;
				eBuffClass buffclass = g_IsBuffClass(buff);
				gIconBuff.RenderTipBuff(buff, IndexTime, fTooltip_x, fTooltip_y);
			}
		}

		if (++buffwidthcount >= 30)
		{
			buffwidthcount = 0;
			++buffheightcount;
		}

		++IndexTime;
	}
}

void SEASON3B::CNewUIBuffWindow::RenderBuffIcon(eBuffState& eBuffType, float x, float y, float width, float height)
{
	int iWidthIndex, iHeightIndex;
	float u, v;

	float Chia = 1.8f;
	if (eBuffType < 81) // eBuff_Berserker
	{
		iWidthIndex = (eBuffType - 1) % 10;
		iHeightIndex = (eBuffType - 1) / 10;

		u = iWidthIndex * width / 256.f;
		v = iHeightIndex * height / 256.f;
		//ACuoi Buff Icon
		RenderBitmap(IMAGE_BUFF_STATUS, x, y, width / Chia, height / Chia, u, v, width / 256.f, height / 256.f);
	}
	else
	{
		// eBuff_Santa
		iWidthIndex = (eBuffType - 81) % 10; // eBuff_Berserker
		iHeightIndex = (eBuffType - 81) / 10; // eBuff_Berserker

		u = iWidthIndex * width / 256.f;
		v = iHeightIndex * height / 256.f;
		// ACuoi Buff Icon
		RenderBitmap(IMAGE_BUFF_STATUS2, x, y, width / Chia, height / Chia, u, v, width / 256.f, height / 256.f);
	}
}

void SEASON3B::CNewUIBuffWindow::RenderBuffTooltip(eBuffClass& eBuffClassType, eBuffState& eBuffType, float x, float y)
{
	int TextNum = 0;
	//::memset(TextList[0], 0, sizeof(char) * 30 * 100);
	::memset(TextList, 0, sizeof(TextList));
	::memset(TextListColor, 0, sizeof(int) * 30);
	::memset(TextBold, 0, sizeof(int) * 30);

	std::list<std::string> tooltipinfo;
	g_BuffToolTipString(tooltipinfo, eBuffType);

	for (std::list<std::string>::iterator iter = tooltipinfo.begin(); iter != tooltipinfo.end(); ++iter)
	{
		std::string& temp = *iter;

		unicode::_sprintf(TextList[TextNum], temp.c_str());

		if (TextNum == 0)
		{
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = true;
		}
		else
		{
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = false;
		}

		TextNum += 1;
	}

	std::string bufftime;
	g_BuffStringTime(eBuffType, bufftime);

	if (bufftime.size() != 0)
	{
		unicode::_sprintf(TextList[TextNum], GlobalText[2533], bufftime.c_str());
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		TextNum += 1;
	}

	SIZE TextSize = { 0, 0 };
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), TextList[0], 1, &TextSize);
	RenderTipTextList(x, y, TextNum, 0);
}

float SEASON3B::CNewUIBuffWindow::GetLayerDepth()	//. 5.3f
{
	return 0.95f;
}

void SEASON3B::CNewUIBuffWindow::OpenningProcess()
{

}

void SEASON3B::CNewUIBuffWindow::ClosingProcess()
{

}

void SEASON3B::CNewUIBuffWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_statusicon.jpg", IMAGE_BUFF_STATUS, GL_LINEAR);
	LoadBitmap("Interface\\newui_statusicon2.jpg", IMAGE_BUFF_STATUS2, GL_LINEAR);
}

void SEASON3B::CNewUIBuffWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_BUFF_STATUS2);
	DeleteBitmap(IMAGE_BUFF_STATUS);
}
