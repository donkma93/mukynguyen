#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
namespace SEASON3B
{
	class CNewUIRenderCoin : public CNewUIObj
	{
		public:
		CNewUIRenderCoin();
		virtual ~CNewUIRenderCoin();

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

		CNewUIManager*	m_pNewUIMng;
		POINT			m_Pos;
	};
}
