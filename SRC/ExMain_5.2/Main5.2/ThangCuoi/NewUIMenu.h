#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "ACuoi_JewelBank.h"
namespace SEASON3B
{
	class CNewUIMenuOption : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{

			IMAGE_BASE_WINDOW_BACK		= CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_LEFT		= CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			IMAGE_BASE_WINDOW_RIGHT		= CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)
			IMAGE_BASE_WINDOW_BTN_EXIT	= CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,			//. newui_exit_00.tga
			IMAGE_WINDOW_TAB_BTN		= CNewUIGuildInfoWindow::IMAGE_GUILDINFO_TAB_BUTTON,

			IMAGE_IGS_BUTTON = BITMAP_START_SPK + 104,
		};
		

		private:
		enum BUTTON_EVENT
		{
			BUTTON_OPTION_1 = 0,
		};

		int WINDOW_WIDTH;
		int WINDOW_HEIGHT;

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;
		int						m_iNumCurOpenTab;
		CNewUIButton*			m_Button;

		CNewUIButton			m_Btn[20];

		public:
		BYTE BtnMaxOption;
		CNewUIMenuOption();
		virtual ~CNewUIMenuOption();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void InitButtons();
		void SetPos(int x, int y);

		bool BtnProcess();
		void SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos);
		private:
		void LoadImages();
		void UnloadImages();
		int CalcAdjustedY();
		bool m_ButtonVisible[MAX_BUTTON_CREATE];
	};

}