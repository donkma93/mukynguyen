#include "stdafx.h"
#include "Util.h"
#include "User.h"
#include "CustomStartItemDame.h"
#include "CashShop.h"
#include "MemScript.h"
#include "Notice.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "ServerInfo.h"
#include "SocketItemType.h"

#include "ObjectManager.h"

CCustomStartItemDame gCustomStartItemDame;

CCustomStartItemDame::CCustomStartItemDame()
{
	this->Init();
}

void CCustomStartItemDame::Init()
{
	for (int n = 0; n < MAX_CLASS; n++)
	{
		this->m_CustomStartItemDameInfo[n].Class = -1;
		this->m_CustomStartItemDameInfo[n].ItemDameStart.clear();
	}
}

void CCustomStartItemDame::LoadItemDame(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}
					CUSTOM_START_ITEM_DAME_INFO Items;

					int Class = lpMemScript->GetNumber();

					Items.ItemType = lpMemScript->GetAsNumber();

					Items.ItemIndex = lpMemScript->GetAsNumber();

					Items.ItemLevel = lpMemScript->GetAsNumber();

					Items.ItemOption = lpMemScript->GetAsNumber();

					Items.EffectBuff = lpMemScript->GetAsNumber();

					Items.DupeDame = lpMemScript->GetAsNumber(); //

					Items.TripDame = lpMemScript->GetAsNumber();//

					Items.CriticalDmg = lpMemScript->GetAsNumber();//

					Items.DameRate = lpMemScript->GetAsNumber();//

					Items.DefDameRate = lpMemScript->GetAsNumber();

					Items.TimeBuff = lpMemScript->GetAsNumber();

					this->m_CustomStartItemDameInfo[Class].ItemDameStart.push_back(Items);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomStartItemDame::CaclCharacter(LPOBJ lpObj, bool flag)
{
	if (flag != 0)
	{
		return;
	}

	CItem* lpItem = &lpObj->Inventory[0]; // tay phải
	CItem* lpItem1 = &lpObj->Inventory[1]; // tay trái

	bool DualHandWeapon = 0;

	if (lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG || lpObj->Class == CLASS_RF)
	{
		if (lpItem->m_Index >= GET_ITEM(0, 0) && lpItem->m_Index < GET_ITEM(4, 0) &&
			lpItem1->m_Index >= GET_ITEM(0, 0) && lpItem1->m_Index < GET_ITEM(4, 0))
		{
			if (lpItem->m_IsValidItem != 0 && lpItem1->m_IsValidItem != 0)
			{
				DualHandWeapon = 1;
			}
		}
	}

	for (std::vector<CUSTOM_START_ITEM_DAME_INFO>::iterator it = this->m_CustomStartItemDameInfo[lpObj->Class].ItemDameStart.begin(); it != this->m_CustomStartItemDameInfo[lpObj->Class].ItemDameStart.end(); it++)
	{
		if (DualHandWeapon != 0)
		{
			if (lpObj->Inventory[0].m_Index == GET_ITEM(it->ItemType, it->ItemIndex))
			{
				if (lpItem->m_Level >= it->ItemLevel && lpItem->m_NewOption >= it->ItemOption || lpItem1->m_Level >= it->ItemLevel && lpItem1->m_NewOption >= it->ItemOption)
				{
					if (lpObj->Inventory[0].m_Index == lpObj->Inventory[1].m_Index)
					{
						lpObj->DoubleDamageRate += it->DupeDame;
						lpObj->TripleDamageRate += it->TripDame;
						lpObj->PhysiDamageMinRight += (it->DameRate) * 2;
						lpObj->PhysiDamageMaxRight += (it->DameRate) * 2;
						lpObj->MagicDamageMax += it->DameRate;
						lpObj->MagicDamageMin += it->DameRate;
						lpObj->DefensePvP += it->DefDameRate;
						lpObj->CriticalDamage += it->CriticalDmg;
					}
					else
					{
						lpObj->DoubleDamageRate += it->DupeDame;
						lpObj->TripleDamageRate += it->TripDame;
						lpObj->PhysiDamageMinRight += it->DameRate;
						lpObj->PhysiDamageMaxRight += it->DameRate;
						lpObj->MagicDamageMax += it->DameRate;
						lpObj->MagicDamageMin += it->DameRate;
						lpObj->DefensePvP += it->DefDameRate;
						lpObj->CriticalDamage += it->CriticalDmg;
					}
				}
			}
		}
		else if (lpItem->m_Index >= GET_ITEM(4, 0) && lpItem->m_Index < GET_ITEM(5, 0) && lpItem->m_Index != GET_ITEM(4, 15) && lpItem->m_Slot == 0)
		{
			if (lpObj->Inventory[0].m_Index == GET_ITEM(it->ItemType, it->ItemIndex))
			{
				if (lpItem->m_Level >= it->ItemLevel && lpItem->m_NewOption >= it->ItemOption || lpItem1->m_Level >= it->ItemLevel && lpItem1->m_NewOption >= it->ItemOption)
				{
					lpObj->DoubleDamageRate += it->DupeDame;
					lpObj->TripleDamageRate += it->TripDame;
					lpObj->PhysiDamageMinRight += it->DameRate;
					lpObj->PhysiDamageMaxRight += it->DameRate;
					lpObj->MagicDamageMax += it->DameRate;
					lpObj->MagicDamageMin += it->DameRate;
					lpObj->DefensePvP += it->DefDameRate;
					lpObj->CriticalDamage += it->CriticalDmg;
					continue;
				}
			}
		}
		else if (lpItem1->m_Index >= GET_ITEM(4, 0) && lpItem1->m_Index < GET_ITEM(5, 0) && lpItem1->m_Index != GET_ITEM(4, 7) && lpItem1->m_Slot == 1)
		{
			if (lpObj->Inventory[1].m_Index == GET_ITEM(it->ItemType, it->ItemIndex))
			{
				if (lpItem->m_Level >= it->ItemLevel && lpItem->m_NewOption >= it->ItemOption || lpItem1->m_Level >= it->ItemLevel && lpItem1->m_NewOption >= it->ItemOption)
				{
					lpObj->DoubleDamageRate += it->DupeDame;
					lpObj->TripleDamageRate += it->TripDame;
					lpObj->PhysiDamageMinLeft += it->DameRate;
					lpObj->PhysiDamageMaxLeft += it->DameRate;
					lpObj->MagicDamageMax += it->DameRate;
					lpObj->MagicDamageMin += it->DameRate;
					lpObj->DefensePvP += it->DefDameRate;
					lpObj->CriticalDamage += it->CriticalDmg;
				}
			}
		}
		else if (lpObj->Inventory[0].m_Index == GET_ITEM(it->ItemType, it->ItemIndex) && lpItem->m_Slot == 0)
		{
			if (lpItem->m_Level >= it->ItemLevel && lpItem->m_NewOption >= it->ItemOption)
			{
				lpObj->DoubleDamageRate += it->DupeDame;
				lpObj->TripleDamageRate += it->TripDame;
				lpObj->PhysiDamageMinRight += it->DameRate;
				lpObj->PhysiDamageMaxRight += it->DameRate;
				lpObj->MagicDamageMin += it->DameRate;
				lpObj->MagicDamageMax += it->DameRate;
				lpObj->DefensePvP += it->DefDameRate;
				lpObj->CriticalDamage += it->CriticalDmg;
			}
		}
	}
}
