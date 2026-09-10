#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
namespace SEASON3B
{
	class CNewUIJewelBank : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{

			IMAGE_BASE_WINDOW_BACK = BITMAP_START_SPK + 107,
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_BOX,
			IMAGE_BASE_BANK_BOX,
			IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)
			IMAGE_BASE_WINDOW_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,		//. newui_exit_00.tga

			IMAGE_WINDOW_TAB_BTN = CNewUIGuildInfoWindow::IMAGE_GUILDINFO_TAB_BUTTON,

		};

		private:
		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH = 190,
			WINDOW_HEIGHT = 200,
		};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;
		int						m_iNumCurOpenTab;

		public:
		void Render3DItem();

		CNewUIJewelBank();
		virtual ~CNewUIJewelBank();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void SetPos(int x, int y);

		private:
		void LoadImages();
		void UnloadImages();
	};

}