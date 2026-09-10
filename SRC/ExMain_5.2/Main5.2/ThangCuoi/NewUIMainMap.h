#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"

namespace SEASON3B
{
	class CNewUIMainMap : public CNewUIObj
	{
		public:
		enum IMAGE_LIST
		{
			DOT_POINT_PLAYER	= BITMAP_START_SPK + 39,
			DOT_POINT_NPC		= BITMAP_START_SPK + 40,
			DOT_POINT_MONSTER	= BITMAP_START_SPK + 41,
			DOT_POINT_ME		= BITMAP_START_SPK + 42,
			WOLRD_00			= BITMAP_START_SPK + 43,
			WOLRD_01			= BITMAP_START_SPK + 44,
			WOLRD_02			= BITMAP_START_SPK + 45,
			WOLRD_03			= BITMAP_START_SPK + 46,
			WOLRD_04			= BITMAP_START_SPK + 47,
			WOLRD_06			= BITMAP_START_SPK + 48,
			WOLRD_07			= BITMAP_START_SPK + 49,
			WOLRD_08			= BITMAP_START_SPK + 50,
			WOLRD_31			= BITMAP_START_SPK + 51,
			WOLRD_33			= BITMAP_START_SPK + 52,
			WOLRD_34			= BITMAP_START_SPK + 53,
			WOLRD_37			= BITMAP_START_SPK + 54,
			WOLRD_38			= BITMAP_START_SPK + 55,
			WOLRD_41			= BITMAP_START_SPK + 56,
			WOLRD_42			= BITMAP_START_SPK + 57,
			WOLRD_56			= BITMAP_START_SPK + 58,
			WOLRD_51			= BITMAP_START_SPK + 59,
			WOLRD_57			= BITMAP_START_SPK + 60,
			WOLRD_63			= BITMAP_START_SPK + 61,
			WOLRD_66			= BITMAP_START_SPK + 62,
			WOLRD_80			= BITMAP_START_SPK + 63,
			WOLRD_81			= BITMAP_START_SPK + 64,
			WOLRD_82			= BITMAP_START_SPK + 65,
			WOLRD_83			= BITMAP_START_SPK + 66,
			WOLRD_84			= BITMAP_START_SPK + 67,
			WOLRD_85			= BITMAP_START_SPK + 68,
			WOLRD_86			= BITMAP_START_SPK + 69,
			WOLRD_87			= BITMAP_START_SPK + 70
		};

		CNewUIMainMap();
		virtual ~CNewUIMainMap();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();
		void UpdateHeroPos();
		void SetPos(int x, int y);
		void RenderMapImageIndex(int Index);
		bool UpdateRadarStates();
		void EnterInsideMap();

		float		m_fMiniMapTexU;
		float		m_fMiniMapTexV;
		int			m_iMiniMapScale;
		DWORD		m_dwBuffState;

		private:
		CNewUIManager*	m_pNewUIMng;
		POINT			m_MiniMapPos;
		POINT			m_MiniMapFramePos;
		POINT			m_HeroPosInWorld;
		POINT			m_HeroPosInMiniMap;
		POINT			m_MiniMapScaleOffset;

		void LoadImages();
		void UnloadImages();
	};
}
