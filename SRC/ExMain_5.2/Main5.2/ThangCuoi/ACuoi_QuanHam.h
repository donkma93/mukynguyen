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
	class CNewUIQuanHam : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)

			IMAGE_HD_LOGO_2 = 50200,
			IMAGE_HD_LOGO_START2,
		};

		private:

		

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;
		CNewUIButton			m_Btn[3];

		public:
		CNewUIQuanHam();
		virtual ~CNewUIQuanHam();

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

		void InitButtons();
		void SetPos(int x, int y);
		bool BtnProcess();

	private:
		void LoadImages();
		void UnloadImages();
	};
}
