#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIGuildInfoWindow.h"
#include "NewUIInventoryCtrl.h"

namespace SEASON3B
{
	class CUIFormSystemRender : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{
			IMAGE_TABLE_TOP_LEFT		= CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,
			IMAGE_TABLE_TOP_RIGHT		= CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,
			IMAGE_TABLE_BOTTOM_LEFT		= CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,
			IMAGE_TABLE_BOTTOM_RIGHT	= CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,
			IMAGE_TABLE_TOP_PIXEL		= CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,
			IMAGE_TABLE_BOTTOM_PIXEL	= CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,
			IMAGE_TABLE_LEFT_PIXEL		= CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,
			IMAGE_TABLE_RIGHT_PIXEL		= CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,
			IMAGE_IGS_BUTTON			= BITMAP_START_SPK + 104,
			IMAGE_BASE_WINDOW_BACK		= BITMAP_START_SPK_FORM,
			IMAGE_BASE_WINDOW_TOP,
			IMAGE_BASE_WINDOW_LEFT,
			IMAGE_BASE_WINDOW_RIGHT,
			IMAGE_BASE_WINDOW_BOTTOM,
			IMAGE_BASE_WINDOW_TOP_80,
			IMAGE_BASE_WINDOW_BOTTOM_80,
		};

		CNewUIManager*			m_pNewUIMng;
		POINT					m_Pos;

		CUIFormSystemRender();
		virtual ~CUIFormSystemRender();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();
		void SetPos(int x, int y);
		void LoadImages();
		void UnloadImages();
		void NewRenderForm(float x, float y, float w, float h, const char* zText);
		void RenderBack(int x, int y, int width, int height, float a = 0.7f);
		void RenderHover(float x, float y, float w, float h, DWORD c = 0x00000000);

		void RenderButton3D(int GetID, float x, float y, float w, float h, BYTE Mode);
		void RenderButtonBarFrameMain(int GetID, float x, float y, float w, float h, BYTE Mode);
		void RenderToolTip(float x, float y, float w, float h);
		void RenderCustomButton(CNewUIButton* m_Btn, float x, float y, BYTE Mode, const char* zString);
		void SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos);
	};

}