#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
#include "ACuoi_JewelBank.h"

namespace SEASON3B
{
	class CNewUINewShop : public CNewUIObj
	{
		public:

		CNewUIManager*			m_pNewUIMng;
		CNewUI3DRenderMng*		m_pNewUI3DRenderMng;
		CNewUIInventoryCtrl*	m_pNewInventoryCtrl;
		CUIPhotoViewer m_PhotoChar;

		POINT					m_Pos;
		CNewUIButton			m_Btn[20];
		CNewUIButton			m_Buy[9];

		int DataListShop;

		struct SolvedNewShop
		{
			PSBMSG_HEAD header;
			DWORD ThaoTac;
		};

		struct PMSG_CASHSHOP_SEND
		{
			PSWMSG_HEAD header;
			BYTE count;
		};

		struct PMSG_LISTITEMSHOP_SEND
		{
			PSWMSG_HEAD header;
			BYTE count;
		};

		struct ListShopSend
		{
			int IndexShop;
			char Name[90];
		};

		std::vector<ListShopSend> DanhSachCashShop;

		struct INFO_SHOP_ITEM
		{
			float SizeBMD;
			int PriceType;
			int Price;
			int Class[7];
			short Index;
			BYTE TypeItem;
			ITEM Item;
		};

		std::vector<INFO_SHOP_ITEM> ListItemCashShop;

		struct LISTITEMCASHSHOP_SENDINFO
		{
			float SizeBMD;
			int PriceType;
			int Price;
			int Class[7];
			short Index;
			BYTE TypeItem;
			BYTE Dur;
			BYTE Item[12];
			int  PeriodTime;
		};

		struct XULY_CGPACKET_SSHOP
		{
			PSBMSG_HEAD header;
			DWORD Shop;
			DWORD ViTri;
		};

		int WINDOW_WIDTH;
		int WINDOW_HEIGHT;

		char* MyClassID[7] =
		{
			"DW",
			"DK",
			"ELF",
			"MG",
			"DL",
			"SM",
			"RF"
		};
		char* MoneyType[6] =
		{
			"Zen",
			"WC",
			"WP",
			"WG",
			"Ruud",
			"ATM",
		};

		CNewUINewShop();
		virtual ~CNewUINewShop();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool Render();
		bool Update();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();

		float GetLayerDepth();
		float GetKeyEventOrder();
		void SendMenuKey();
		void SendPacketToProtocol11(BYTE* lpMsg);
		void SendPacketToProtocol12(BYTE* lpMsg);
		static void XacNhanMuaShop(LPVOID pClass);
		void SetPos(int x, int y);
		void CreateCharacterPhoto();
		void UpdateCharacterPhoto();
		void RenderWindowShop();
		void RenderItem3DShop();
		void InitButtons();
		void InsertInventoryItemsFromPlayer();
	};

}