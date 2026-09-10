#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "wsclient.h"
namespace SEASON3B
{
	class CNewUIRelife : public CNewUIObj
	{
		private:
		enum PARTY_WINDOW_SIZE
		{
			WINDOW_WIDTH	= 250,
			WINDOW_HEIGHT	= 200,
		};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CNewUIRadioGroupButton	m_TabBtn;
		CNewUIButton			m_Btn[1];

		public:
		BYTE BtnMaxOption;
		CNewUIRelife();
		virtual ~CNewUIRelife();

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
		void Render3DItem();
		bool BtnProcess();
	};
}
#define MAXSIZERL	10
#define MAXSUBSIZE	5


struct RelifeCheckReQuest
{
	PSBMSG_HEAD h;
	int			Number;
};

struct PSBMSG_JewelBankRevX
{
	PSBMSG_HEAD h;
	int JewelIndex[MAXSIZERL];
};

struct RelifeCheckUpdate
{
	PBMSG_HEADX h;
	int m_TaiSinh;
	int m_DieuKienLevel;
	int CheckDieuKien[MAXSIZERL];
	int ResetLife;
};

struct RelifePointUpdate
{
	PBMSG_HEADX h;
	int CheckDiemTaiSinh[MAXSIZERL];
};

struct RelifeCheckReqInfoPart1
{
	PBMSG_HEADX h;
	int CheckDieuKienNgocVal[MAXSIZERL][MAXSUBSIZE];
};

class SpkRelife
{
public:
	bool DrawMainRelife;
	float MainWidth;
	float StartY;
	float StartX;
	DWORD ClickTick;

	int m_TaiSinh;
	int m_DieuKienLevel;
	int mCheckDieuKien[MAXSIZERL];
	int ResetLife;
	int mCheckDiemTaiSinh[MAXSIZERL];
	int CheckDieuKienJw[MAXSIZERL][MAXSUBSIZE];

	void SendMenuButton();
	void TaiSinhCheckRecv(const BYTE* SPK);
	void TaiSinhDiemRecv(const BYTE* SPK);
	void SaveFormGs(const BYTE* SPK);
	void GetRelifeDataDraw(float x, float y, int SlotJw1, int SlotJw2, int SlotJw3, int SlotJw4, int SlotJw5, int gmCheckDieuKien, int gmCheckRelife);
};
extern SpkRelife G_SpkRelife;