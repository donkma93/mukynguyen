#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "../UIControls.h"

namespace SEASON3B
{
	class CNewUIGiftCode : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			//IMAGE_BASE_WINDOW_BACK = CNewUIJewelBank::IMAGE_BASE_WINDOW_BACK,				//. newui_msgbox_back.jpg
			//IMAGE_BASE_WINDOW_TOP,
			//IMAGE_BASE_WINDOW_BOTTOM,
			//IMAGE_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,			//. newui_item_back02-l.tga	(21,320)
			//IMAGE_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,		//. newui_item_back02-r.tga	(21,320)

			IMAGE_CHAINFO_TEXTBOX = BITMAP_INTERFACE_NEW_CHAINFO_WINDOW_BEGIN,
			IMAGE_IGS_BUTTON = BITMAP_IGS_MSGBOX_BUTTON,//BITMAP_HERO_POSITION_INFO_BEGIN + 10,
			IMAGE_HD_LOGO_0 = 50500,
			IMAGE_HD_LOGO_1,
		};

	private:

		//enum PARTY_WINDOW_SIZE
		//{
		//	WINDOW_WIDTH = 190,
		//	WINDOW_HEIGHT = 200,
		//};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIButton			m_Btn[3];

		CUITextInputBox* m_pGiftCodeInput;
		CUITextInputBox* m_pNewGiftCodeInput;
		CUITextInputBox* m_pReNewGiftCodeInput;
		CUITextInputBox* m_pPersionCodeInput;

		bool m_bShowLengthWarningNew;
		bool m_bShowLengthWarningRNew;
		bool m_bShowNotMatchWarning;
		bool m_bShowInvalidCodeWarning;

	public:
		CNewUIGiftCode();
		virtual ~CNewUIGiftCode();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		int WINDOW_WIDTH;
		int WINDOW_HEIGHT;

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		void ClearTextBoxes();
		float GetLayerDepth();
		float GetKeyEventOrder();
		void LoadImages();
		void InitButtons();
		void SetPos(int x, int y);
	};
}
