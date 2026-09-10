#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"


struct RESET_INFO_RECV
{
	PSBMSG_HEAD h;
	int ResetThuong;
	int ResetVip;
	int GHRS;
	int Reset;
	int Level;
	int Money;
	int Point;
	int VipLevel;
	int VipMoney;
	int VipPoint;
	int Coin[2];
	int Ngoc[5];
};

struct GC_RESET_INFO
{
	int m_ResetThuong;
	int m_ResetVip;
	int m_GHRS;
	int m_Reset;
	int m_Level;
	int m_Money;
	int m_Point;
	int m_VipLevel;
	int m_VipMoney;
	int m_VipPoint;
	int m_Coin[2];
	int m_Ngoc[5];
};

namespace SEASON3B
{
	class CNewUIAutoReset : public CNewUIObj
	{
	public:
		//enum IMAGE_LIST
		//{
		//	IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
		//	IMAGE_BASE_WINDOW_TOP,
		//	IMAGE_BASE_WINDOW_BOTTOM,
		//	IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
		//	IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)

		//	IMAGE_HD_LOGO_0 = 50000,
		//	IMAGE_HD_LOGO_START,
		//};

	private:



		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;
		CNewUIButton			m_Btn[2];

	public:
		CNewUIAutoReset();
		virtual ~CNewUIAutoReset();

		int WINDOW_WIDTH;
		int WINDOW_HEIGHT;

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void RenderText();
		void InitButtons();
		void SetPos(int x, int y);
		bool BtnProcess();
		GC_RESET_INFO m_AutoResetInfo;
		bool m_bResetVip;
		int rsthuong;
		int rsvip;
		void GCResetAutoInfo(const BYTE* lpMsg);
	private:
		void LoadImages();
		void UnloadImages();

	};
}
