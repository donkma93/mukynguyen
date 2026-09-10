#pragma once
#if WinQuestTurn
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIGuildInfoWindow.h"
#include "NewUIInventoryCtrl.h"

namespace SEASON3B
{
	class CNewUIQQ : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{
			IMAGE_WINDOW_TAB_BTN = CNewUIGuildInfoWindow::IMAGE_GUILDINFO_TAB_BUTTON,

			IMAGE_TABLE_TOP_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,			
			IMAGE_TABLE_TOP_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,		
			IMAGE_TABLE_BOTTOM_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,	
			IMAGE_TABLE_BOTTOM_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,	
			IMAGE_TABLE_TOP_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,		
			IMAGE_TABLE_BOTTOM_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,	
			IMAGE_TABLE_LEFT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,		
			IMAGE_TABLE_RIGHT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,	

			IMAGE_IGS_BUTTON = BITMAP_IGS_MSGBOX_BUTTON,//BITMAP_HERO_POSITION_INFO_BEGIN + 10,
		};

		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH = 270,
			WINDOW_HEIGHT = 270,
		};

		CNewUIManager*			m_pNewUIMng;
		POINT					m_Pos;

		CNewUIButton			m_Btn[5];

		CNewUIQQ();
		virtual ~CNewUIQQ();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();
		void InitButtons();
		bool BtnProcess();
		void InitTextButtons();
		void SetPos(int x, int y);
	};

}
#endif