#pragma once

#include "NewUIBase.h"
#include "NewUIInventoryCtrl.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	class CNewUIInventoryExtension : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_NPCSHOP_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,	// Reference
			IMAGE_NPCSHOP_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP2,
			IMAGE_NPCSHOP_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_NPCSHOP_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_NPCSHOP_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,
			IMAGE_INVENTORY_EXIT_BTN = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,
			IMAGE_EXTENSION_EMPTY = BITMAP_INTERFACE_NEW_INVENTORY_EXT_BEGIN,
			IMAGE_EXTENSION_TABLE,
			IMAGE_EXTENSION_NO1,
			IMAGE_EXTENSION_NO2,
			IMAGE_EXTENSION_NO3,
			IMAGE_EXTENSION_NO4,
		};

	private:
		enum
		{
			WIDTH = 190,
			HEIGHT = 429,

			HEIGHT_PER_EXT = 87,
			EXT_BORDER = 3,
		};

		CNewUIManager* m_pNewUIMng;
		CNewUIInventoryCtrl* m_extensions[MAX_INVENTORY_EXT_COUNT];
		POINT m_Pos;

		CNewUIButton m_BtnExit;
		CNewUIButton			m_Btn[4];
	protected:
		void SetButtonInfo();
	public:
		CNewUIInventoryExtension();
		virtual ~CNewUIInventoryExtension();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		void SetPos(int x, int y);

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		void InitButtons();
		void InitButtonsText();
		float GetLayerDepth();	//. 2.5f

		bool InsertItem(int iIndex, BYTE* pbyItemPacket) const;
		void DeleteItem(int iIndex) const;
		void DeleteAllItems() const;
		ITEM* FindItem(int iLinealPos);
		int FindItemIndex(short sType, int iLevel);
		CNewUIInventoryCtrl* GetOwnerOf(const CNewUIPickedItem* pPickedItem) const;
		CNewUIInventoryCtrl* TryGetExtensionByInventoryIndex(int iIndex) const;
		CNewUIInventoryCtrl* FindInventoryByItemKey(DWORD dwItemKey) const;
		CNewUIInventoryCtrl* GetInventoryCtrlByIndex(int i) const;
		int FindEmptySlot(IN int cx, IN int cy) const;
	private:
		void Init();

		void LoadImages();
		void UnloadImages();

		bool InventoryProcess();

		void RenderFrame() const;
		void RenderTexts() const;
	};
}