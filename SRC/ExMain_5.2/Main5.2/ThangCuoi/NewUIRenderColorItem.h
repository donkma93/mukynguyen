#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "ACuoi_JewelBank.h"
#include "NewUIMyInventory.h"

namespace SEASON3B
{
	class CNewUIRenderColorItem : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{
			IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)
		};

		private:

		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH = 190,
			WINDOW_HEIGHT = 200,
		};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIButton			m_Btn[20];

		public:
		CNewUIRenderColorItem();
		virtual ~CNewUIRenderColorItem();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void CreateInitButtons(int a, int x, int y, bool Ext = false);
		void InitButtons();
		bool GetButtonPrev(int Item, int Step, int Min);
		bool GetButtonNext(int Item, int Step, int Max);
		void SetPos(int x, int y);
		bool BtnProcess();
		int RenderItem[10];
		float fRenderItem[10];
		bool IsRenderWeapon(int Type);
		void SaveRenderColorToFile(BYTE a);
		void RenderCEfect(BMD* b, OBJECT* o, int Type, float Alpha, int RenderType);
		void RenderSetBlendEffect(OBJECT* o);
	};
}
