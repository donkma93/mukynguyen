// NewUIPersonalInventory.h: interface for the CNewUIViewItem class.
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NewUIBase.h"
#include "NewUIInventoryCtrl.h"
#include "NewUIMessageBox.h"
#include "NewUI3DRenderMng.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	class CNewUIViewItem : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_INVENTORY_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_INVENTORY_BACK_TOP = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN,	//"newui_item_back01.tga"
			IMAGE_INVENTORY_BACK_TOP2,	//"newui_item_back04.tga"
			IMAGE_INVENTORY_BACK_LEFT,	//"newui_item_back02-L.tga"
			IMAGE_INVENTORY_BACK_RIGHT,	//"newui_item_back02-R.tga"
			IMAGE_INVENTORY_BACK_BOTTOM,	//"newui_item_back03.tga"
			IMAGE_INVENTORY_ITEM_BOOT,	//"newui_item_boots.tga"
			IMAGE_INVENTORY_ITEM_HELM,	//"newui_item_cap.tga"
			IMAGE_INVENTORY_ITEM_FAIRY,	//"newui_item_fairy.tga"
			IMAGE_INVENTORY_ITEM_WING,	//"newui_item_wing.tga"
			IMAGE_INVENTORY_ITEM_RIGHT,	//"newui_item_weapon(L).tga"
			IMAGE_INVENTORY_ITEM_LEFT,	//"newui_item_weapon(R).tga"
			IMAGE_INVENTORY_ITEM_ARMOR,	//"newui_item_upper.tga"
			IMAGE_INVENTORY_ITEM_GLOVES,	//"newui_item_gloves.tga"
			IMAGE_INVENTORY_ITEM_PANTS,	//"newui_item_lower.tga"
			IMAGE_INVENTORY_ITEM_RING,	//"newui_item_ring.tga"
			IMAGE_INVENTORY_ITEM_NECKLACE,	//"newui_item_necklace.tga"
		};

	private:

		enum
		{
			INVENTORY_WIDTH = 190,
			INVENTORY_HEIGHT = 429,
		};
		typedef struct tagEQUIPMENT_ITEM
		{
			int x, y;
			int width, height;
			DWORD dwBgImage;
		} EQUIPMENT_ITEM;

		CNewUIManager*			m_pNewUIMng;
		POINT m_Pos;

		EQUIPMENT_ITEM m_EquipmentSlots[MAX_EQUIPMENT_INDEX];


	public:
		CNewUIViewItem();
	
		bool Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng, int x, int y);

		void SetPos(int x, int y);
		const POINT& GetPos() const;
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		
		bool IsVisible() const;

		float GetLayerDepth();	//. 4.2f

		void SetEquipmentSlotInfo();

		void RenderFrame(int pX, int pY) const;
		void RenderEquippedItem();
		void RenderInventoryDetails() const;
		bool BtnProcess();
	};
}

