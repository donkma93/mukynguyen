#pragma once

#include "Protocol.h"

struct PMSG_INVENTORY_EQUIPMENT_RECV
{
	PSBMSG_HEAD header; // C1:BF:20
	BYTE slot;
	BYTE result;
};

struct PMSG_INVENTORY_EQUIPMENT_SEND
{
	PSBMSG_HEAD header; // C1:BF:20
	BYTE slot;
	BYTE result;
};

class CInventoryEquipment
{
public:
	void MainProc();
	bool CheckInventoryEquipment(LPOBJ lpObj,int ItemIndex);
	void InsertInventoryEquipment(LPOBJ lpObj);
	void CGInventoryEquipmentRecv(PMSG_INVENTORY_EQUIPMENT_RECV* lpMsg,int aIndex);
};

extern CInventoryEquipment gInventoryEquipment;
