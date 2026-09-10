#pragma once
#if ChangePassTurn
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "../UIControls.h"

namespace SEASON3B
{
	class CNewUIChangePass : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_CHAINFO_TEXTBOX = BITMAP_INTERFACE_NEW_CHAINFO_WINDOW_BEGIN,
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

		CUITextInputBox* m_pOldPassInput;
		CUITextInputBox* m_pNewPassInput;
		CUITextInputBox* m_pReNewPassInput;
		CUITextInputBox* m_pPersionCodeInput;

		bool m_bShowLengthWarningNew;
		bool m_bShowLengthWarningRNew;
		bool m_bShowNotMatchWarning;
		bool m_bShowInvalidCodeWarning;

		public:
		CNewUIChangePass();
		virtual ~CNewUIChangePass();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		void ClearTextBoxes();
		float GetLayerDepth();
		float GetKeyEventOrder();

		void InitButtons();
		void SetPos(int x, int y);
	};
}
#endif