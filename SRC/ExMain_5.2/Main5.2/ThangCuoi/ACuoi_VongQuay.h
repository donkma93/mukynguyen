#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
#include "ACuoi_JewelBank.h"
namespace SEASON3B
{
	class CNewUISpinPush : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{

			IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			
			IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		
			IMAGE_BASE_WINDOW_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,		
			IMAGE_WINDOW_TAB_BTN = CNewUIGuildInfoWindow::IMAGE_GUILDINFO_TAB_BUTTON,

			IMAGE_TABLE_TOP_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,			
			IMAGE_TABLE_TOP_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,		
			IMAGE_TABLE_BOTTOM_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,	
			IMAGE_TABLE_BOTTOM_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,	
			IMAGE_TABLE_TOP_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,		
			IMAGE_TABLE_BOTTOM_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,	
			IMAGE_TABLE_LEFT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,		
			IMAGE_TABLE_RIGHT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,	

			IMAGE_IGS_BUTTON = BITMAP_IGS_MSGBOX_BUTTON,

			DRAW_ARROW_RED = BITMAP_START_SPK + 100,
		};


	private:
		enum BUTTON_EVENT
		{
			BUTTON_OPTION_1 = 0,
		};
		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH = 190,
			WINDOW_HEIGHT = 200,
		};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;
		int						m_iNumCurOpenTab;

		CNewUIButton			m_Btn[5];
	public:

		struct RecvInfoItemData
		{
			BYTE Dur;
			BYTE Item[12];
			int  PeriodTime;
		};

		typedef struct tagEQUIPMENT_ITEM
		{
			int x, y;
			int width, height;
			DWORD dwBgImage;
		} EQUIPMENT_ITEM;

		ITEM CharItemInfo[MAX_EQUIPMENT];
		std::vector<ITEM> m_CharItemInfo;

		EQUIPMENT_ITEM m_EquipmentSlots[MAX_EQUIPMENT_INDEX];

		CNewUISpinPush();
		virtual ~CNewUISpinPush();

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

		void SetEquipmentSlotInfo();
		void GenerateCharItemInfoFromSpin();

		private:
		void LoadImages();
		void UnloadImages();
	};

}