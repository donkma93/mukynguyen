#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
#include "ThangCuoi/StructSendGs.h"
namespace SEASON3B
{
	class CNewUIPKSystem : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{
			CREATE_NEWFONT = 34500,
		};

		CNewUIPKSystem();
		virtual ~CNewUIPKSystem();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void SetPos(int x, int y);

		private:
		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH = 190,
			WINDOW_HEIGHT = 200,
		};
	
		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;

		void LoadImages();
		void UnloadImages();
	};

}

class PKSystemSet
{
	public:
	std::vector<DATA_NOTICEPK> m_DataNoticePK;
	void RenderPKForm();
	float guiX;
	float guiY;
	short curX;
	short curY;
	bool  dragging;
	DWORD ActionTime;

};
extern PKSystemSet gPKSys;
