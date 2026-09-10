#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "ACuoi_JewelBank.h"

namespace SEASON3B
{
	class CNewUICastleEvent : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{
			IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)

			IMAGE_HD_LOGO_0 = BITMAP_START_SPK + 119,
		};

		private:

		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH = 190,
			WINDOW_HEIGHT = 200,
		};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIButton			m_Btn[5];

		public:
		CNewUICastleEvent();
		virtual ~CNewUICastleEvent();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();
		void RenderTime();
		void InitButtons();
		void SetPos(int x, int y);
		bool BtnProcess();
		void CGMoveCTCMini(int Type);
		void RenderText();
		private:
		void LoadImages();
		void UnloadImages();
	};
}
