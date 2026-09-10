#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "WSClient.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
namespace SEASON3B
{
	class CNewUIRanking : public CNewUIObj
	{
		public:

		enum IMAGE_LIST
		{
			IMAGE_WINDOW_TAB_BTN = CNewUIGuildInfoWindow::IMAGE_GUILDINFO_TAB_BUTTON,
		};

		public:
		CNewUIRanking();
		virtual ~CNewUIRanking();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		void SEASON3B::CNewUIRanking::IsRender();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void InitButtons();
		void SetPos(int x, int y);
		void DrawTabs(int sTapIndex);
		int Page;

		private:

		enum PWINDOW_SIZE
		{
			WINDOW_WIDTH = 480,
			WINDOW_HEIGHT = 270,
		};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;
		int						m_iNumCurOpenTab;
		void LoadImages();
		void UnloadImages();

	};

}
