#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
namespace SEASON3B
{
	class CNewUIRenderDameBoss : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_BASE_WINDOW_DMG_TOP_1 = BITMAP_START_SPK + 105,
			IMAGE_BASE_WINDOW_DMG_TOP_2 = BITMAP_START_SPK + 106,
		};

	private:

		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH = 190,
			WINDOW_HEIGHT = 200,
		};

		CNewUIManager* m_pNewUIMng;
		POINT		   m_Pos;

		public:

		CNewUIRenderDameBoss();
		virtual ~CNewUIRenderDameBoss();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void SetPos(int x, int y);
		void SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos);

	private:
		void LoadImages();
		void UnloadImages();
	};
}
