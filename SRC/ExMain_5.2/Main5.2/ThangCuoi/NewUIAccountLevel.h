#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "ACuoi_JewelBank.h"

namespace SEASON3B
{
	class CNewUIAccountLevel : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{

			IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)
			IMAGE_BASE_WINDOW_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,		//. newui_exit_00.tga
			IMAGE_WINDOW_TAB_BTN = CNewUIGuildInfoWindow::IMAGE_GUILDINFO_TAB_BUTTON,

			IMAGE_TABLE_TOP_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,			//. newui_item_table01(L).tga (14,14)
			IMAGE_TABLE_TOP_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,		//. newui_item_table01(R).tga (14,14)
			IMAGE_TABLE_BOTTOM_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,	//. newui_item_table02(L).tga (14,14)
			IMAGE_TABLE_BOTTOM_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,	//. newui_item_table02(R).tga (14,14)
			IMAGE_TABLE_TOP_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,		//. newui_item_table03(up).tga (1, 14)
			IMAGE_TABLE_BOTTOM_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,	//. newui_item_table03(dw).tga (1,14)
			IMAGE_TABLE_LEFT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,		//. newui_item_table03(L).tga (14,1)
			IMAGE_TABLE_RIGHT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,	//. newui_item_table03(R).tga (14,1)

			IMAGE_IGS_BUTTON = BITMAP_IGS_MSGBOX_BUTTON,//BITMAP_HERO_POSITION_INFO_BEGIN + 10,

			IMAGE_IGS_LOGOV1 = BITMAP_START_SPK + 35,
			IMAGE_IGS_LOGOV2,
			IMAGE_IGS_LOGOV3,
		};


		private:

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;

		CNewUIButton			m_Btn[5];

		int WINDOW_WIDTH;
		int WINDOW_HEIGHT;

		public:
		CNewUIAccountLevel();
		virtual ~CNewUIAccountLevel();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void InitButtons();
		void InitText();
		void SetPos(int x, int y);
		bool BtnProcess();
		private:
		void LoadImages();
		void UnloadImages();
	};

}