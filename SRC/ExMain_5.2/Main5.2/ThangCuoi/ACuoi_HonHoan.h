#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "ACuoi_JewelBank.h"
#include "NewUIMyInventory.h"

namespace SEASON3B
{
	class CNewUIHonHoan : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{
			IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)

			IMAGE_HD_LOGO_0 = 50100,
			IMAGE_HD_LOGO_1,
			IMAGE_HD_LOGO_2,
			IMAGE_HD_LOGO_3,
			IMAGE_HD_LOGO_4,
		};

		private:

		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH = 190,
			WINDOW_HEIGHT = 200,
		};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIButton			m_Btn[3];

		public:
		CNewUIHonHoan();
		virtual ~CNewUIHonHoan();

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

		void SendStartLoad();
		void SendPackButton();
		void RenderHonHoanEffect(CHARACTER* c, OBJECT* o);
		void RenderText(int a, int b);

		private:
		void LoadImages();
		void UnloadImages();
	};
}
