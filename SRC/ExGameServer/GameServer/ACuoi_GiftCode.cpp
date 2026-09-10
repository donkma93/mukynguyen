// CustomGift.cpp: implementation of the CCustomGift class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "Log.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"
#include "ACuoi_GiftCode.h"

CCustomGift gCustomGift;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomGift::CCustomGift() // OK
{
    this->m_GiftListInfo.clear();
    this->ItemList.clear();
    this->BuffList.clear();
    this->m_MessageInfoBP.clear();
}

CCustomGift::~CCustomGift() // OK
{
}

void CCustomGift::LoadConfig(char* FilePath)
{
    this->EnableGift = false;
    this->m_MessageInfoBP.clear();
    this->m_GiftListInfo.clear();
    this->ItemList.clear();
    this->BuffList.clear();

    pugi::xml_document file;
    pugi::xml_parse_result res = file.load_file(FilePath);
    if (res.status != pugi::status_ok) {
        ErrorMessageBox("File %s load fail. Error: %s", FilePath, res.description());
        return;
    }

    pugi::xml_node oGiftCode = file.child("GiftCode");

    this->EnableGift = oGiftCode.attribute("Enable").as_int();

    pugi::xml_node Message = oGiftCode.child("Message");
    for (pugi::xml_node msg = Message.child("Msg"); msg; msg = msg.next_sibling())
    {
        MESSAGE_INFO_GIFT info;
        info.Index = msg.attribute("Index").as_int();
        strcpy_s(info.Message, msg.attribute("Text").as_string());
        this->m_MessageInfoBP.insert(std::pair<int, MESSAGE_INFO_GIFT>(info.Index, info));
    }

    pugi::xml_node DieuKienGift = oGiftCode.child("DieuKien");
    for (pugi::xml_node YeuCau = DieuKienGift.child("YeuCau"); YeuCau; YeuCau = YeuCau.next_sibling())
    {
        GIFT_LIST_INFO YeuCauGift;
        YeuCauGift.Index = YeuCau.attribute("Index").as_int();
        YeuCauGift.Class = YeuCau.attribute("Class").as_int();
        YeuCauGift.Enable[0] = YeuCau.attribute("Vip0").as_int();
        YeuCauGift.Enable[1] = YeuCau.attribute("Vip1").as_int();
        YeuCauGift.Enable[2] = YeuCau.attribute("Vip2").as_int();
        YeuCauGift.Enable[3] = YeuCau.attribute("Vip3").as_int();
        YeuCauGift.MinLevel = YeuCau.attribute("MinLv").as_int();
        YeuCauGift.MaxLevel = YeuCau.attribute("MaxLv").as_int();
        YeuCauGift.MinReset = YeuCau.attribute("MinRS").as_int();
        YeuCauGift.MaxReset = YeuCau.attribute("MaxRS").as_int();
        YeuCauGift.LevelUpPoint = YeuCau.attribute("Point").as_int();
        YeuCauGift.Money = YeuCau.attribute("Zen").as_int();
        YeuCauGift.Coin1 = YeuCau.attribute("WC").as_int();
        YeuCauGift.Coin2 = YeuCau.attribute("WP").as_int();
        YeuCauGift.Coin3 = YeuCau.attribute("WG").as_int();
        const char* codeValue = YeuCau.attribute("Code").as_string();
        strcpy_s(YeuCauGift.Code, sizeof(YeuCauGift.Code), codeValue);
        YeuCauGift.GioiHan = YeuCau.attribute("GioiHan").as_int();
        this->m_GiftListInfo.insert(std::pair<int, GIFT_LIST_INFO>(YeuCauGift.Index, YeuCauGift));
    }

    pugi::xml_node oQuaGift = oGiftCode.child("QuaGift");
    for (pugi::xml_node oCaiDatQua = oQuaGift.child("CaiDatQua"); oCaiDatQua; oCaiDatQua = oCaiDatQua.next_sibling())
    {
        GIFT_LIST_ITEM_INFO oQua;
        oQua.Index = oCaiDatQua.attribute("Index").as_int();
        oQua.MaxCount = oCaiDatQua.attribute("SLIteem").as_int();
        oQua.ItemIndex = oCaiDatQua.attribute("ItemIndex").as_int();
        oQua.Level = oCaiDatQua.attribute("Level").as_int();
        oQua.Dur = oCaiDatQua.attribute("Dur").as_int();
        oQua.Option1 = oCaiDatQua.attribute("Skill").as_int();
        oQua.Option2 = oCaiDatQua.attribute("Luck").as_int();
        oQua.Option3 = oCaiDatQua.attribute("Option").as_int();
        oQua.NewOption = oCaiDatQua.attribute("Exc").as_int();
        oQua.SetOption = oCaiDatQua.attribute("Anc").as_int();
        oQua.SocketCount = oCaiDatQua.attribute("SocketCount").as_int();
        oQua.Duration = oCaiDatQua.attribute("Time").as_int();

        this->ItemList.insert(std::pair<int, GIFT_LIST_ITEM_INFO>(oQua.Index, oQua));
    }

    pugi::xml_node oBuffGift = oGiftCode.child("BuffGift");
    for (pugi::xml_node oCaiDatBuff = oBuffGift.child("CaiDatQua"); oCaiDatBuff; oCaiDatBuff = oCaiDatBuff.next_sibling())
    {
        GIFT_LIST_BUFF_INFO oBuff;
        oBuff.Index = oCaiDatBuff.attribute("Index").as_int();
        oBuff.Effect = oCaiDatBuff.attribute("Effect").as_int();
        oBuff.Type = oCaiDatBuff.attribute("Type").as_int();
        oBuff.Power[0] = oCaiDatBuff.attribute("Power1").as_int();
        oBuff.Power[1] = oCaiDatBuff.attribute("Power2").as_int();
        oBuff.Power[2] = oCaiDatBuff.attribute("Power3").as_int();
        oBuff.Power[3] = oCaiDatBuff.attribute("Power4").as_int();
        oBuff.Duration = oCaiDatBuff.attribute("Time").as_int();

        this->BuffList.insert(std::pair<int, GIFT_LIST_BUFF_INFO>(oBuff.Index, oBuff));
    }
}

char* CCustomGift::GetMessage(int index)
{
    std::map<int, MESSAGE_INFO_GIFT>::iterator it = this->m_MessageInfoBP.find(index);
    if (it == this->m_MessageInfoBP.end())
    {
        char Error[256];
        wsprintf(Error, "Could not find message %d!", index);
        return Error;
    }
    else
    {
        return it->second.Message;
    }
}

GIFT_LIST_INFO* CCustomGift::GetConfigListInfo(int Info)
{
    std::map<int, GIFT_LIST_INFO>::iterator it = this->m_GiftListInfo.find(Info);
    if (it == this->m_GiftListInfo.end())
    {
        return 0;
    }
    else
    {
        return &it->second;
    }
}

bool CCustomGift::CommandGift(LPOBJ lpObj, char* arg) // OK
{
    if (this->EnableGift == 0)
    {
        gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(0));
        return 0;
    }

    char mode[32] = { 0 };
    gCommandManager.GetString(arg, mode, sizeof(mode), 0);
    bool found = false;

    for (std::map<int, GIFT_LIST_INFO>::iterator it = this->m_GiftListInfo.begin(); it != this->m_GiftListInfo.end(); ++it)
    {
        if (strcmp(mode, it->second.Code) == 0) // So sánh chuỗi
        {
            found = true;

            strncpy(this->m_CurrentCode, it->second.Code, sizeof(this->m_CurrentCode) - 1);
            this->m_CurrentCode[sizeof(this->m_CurrentCode) - 1] = '\0';

            // Kiểm tra Class trước
            if (it->second.Class != -1 && lpObj->Class != it->second.Class)
            {
                gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(10));
                return false;
            }

            if (it->second.Enable[lpObj->AccountLevel] == 0)
            {
                continue;
            }

            if (it->second.MinLevel != -1 && lpObj->Level < it->second.MinLevel)
            {
                gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(1));
                return false;
            }

            if (it->second.MaxLevel != -1 && lpObj->Level > it->second.MaxLevel)
            {
                gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(2));
                return false;
            }

            if (it->second.MinReset != -1 && lpObj->Reset < it->second.MinReset)
            {
                gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(3));
                return false;
            }

            if (it->second.MaxReset != -1 && lpObj->Reset > it->second.MaxReset)
            {
                gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(4));
                return false;
            }

            if (gItemManager.CheckItemInventorySpace(lpObj, 8, 8) == 0)
            {
                gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(5));
                return false;
            }

            this->CheckGiftCode(lpObj);
            return true;
        }
    }
    if (!found)
    {
        gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(6));
    }

    return 0;
}

void CCustomGift::CheckGiftCode(LPOBJ lpObj)
{
    SDHP_COMMAND_GIFT_SEND pMsg;

    pMsg.header.set(0x0F, 0x30, sizeof(pMsg));

    pMsg.index = lpObj->Index;
    memcpy(pMsg.Name, lpObj->Name, sizeof(pMsg.Name));

    strncpy(pMsg.GiftCode, this->m_CurrentCode, sizeof(pMsg.GiftCode) - 1);
    pMsg.GiftCode[sizeof(pMsg.GiftCode) - 1] = '\0';
    pMsg.GioiHan = 0; // Mặc định
    for (std::map<int, GIFT_LIST_INFO>::iterator it = this->m_GiftListInfo.begin(); it != this->m_GiftListInfo.end(); ++it)
    {
        if (strcmp(it->second.Code, this->m_CurrentCode) == 0)
        {
            pMsg.GioiHan = it->second.GioiHan;
            break;
        }
    }
    gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCustomGift::DGCommandGiftRecv(SDHP_COMMAND_GIFT_RECV* lpMsg) // OK
{
    if (gObjIsConnected(lpMsg->index) == 0)
    {
        return;
    }

    LPOBJ lpObj = &gObj[lpMsg->index];

    GIFT_LIST_INFO* currentGift = nullptr;

    for (std::map<int, GIFT_LIST_INFO>::iterator it = this->m_GiftListInfo.begin(); it != this->m_GiftListInfo.end(); ++it)
    {
        if (strcmp(it->second.Code, this->m_CurrentCode) == 0)
        {
            currentGift = &(it->second);
            break;
        }
    }

    if (currentGift == nullptr)
    {
        gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(6));
        return;
    }

    if (strcmp(lpMsg->Name, lpObj->Name) != 0)
    {
        gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(10));
        return;
    }

    if (currentGift->GioiHan != 0 && lpMsg->Result == 0)
    {
        gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(7));
        return;
    }

    // Xử lý LevelUpPoint
    if (currentGift->LevelUpPoint > 0)
    {
        lpObj->LevelUpPoint += currentGift->LevelUpPoint;
        GCNewCharacterInfoSend(lpObj);
        GDCharacterInfoSaveSend(lpObj->Index);
    }

    // Xử lý Money
    if (currentGift->Money > 0)
    {
        if (gObjCheckMaxMoney(lpObj->Index, currentGift->Money) == 0)
        {
            lpObj->Money = MAX_MONEY;
        }
        else
        {
            lpObj->Money += currentGift->Money;
        }
        GCMoneySend(lpObj->Index, lpObj->Money);
    }

    // Xử lý Coin
    if (currentGift->Coin1 > 0 || currentGift->Coin2 > 0 || currentGift->Coin3 > 0)
    {
        GDSetCoinSend(lpObj->Index, currentGift->Coin1, currentGift->Coin2, currentGift->Coin3, 0, 0, "GiftCode");
        //gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(8), currentGift->Coin1, currentGift->Coin2, currentGift->Coin3);
    }

    // Xử lý Items
    bool itemAdded = false;
    for (std::multimap<int, GIFT_LIST_ITEM_INFO>::iterator ir = this->ItemList.begin(); ir != this->ItemList.end(); ++ir)
    {
        if (ir->second.Index == currentGift->Index)
        {
            BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

            for (int u = 0; u < ir->second.SocketCount && u < MAX_SOCKET_OPTION; u++)
            {
                ItemSocketOption[u] = 0xFE;
            }

            for (int count = 0; count < ir->second.MaxCount; count++)
            {
                GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, ir->second.ItemIndex,
                    ir->second.Level, ir->second.Dur, ir->second.Option1,
                    ir->second.Option2, ir->second.Option3, lpObj->Index,
                    ir->second.NewOption, ir->second.SetOption, 0, 0,
                    ItemSocketOption, 0xFF,
                    ((ir->second.Duration > 0) ? ((DWORD)time(0) + ir->second.Duration) : 0));
                itemAdded = true;
                /* LogAdd(LOG_RED, "[CommandGift][%s][%s] - Added Item: Index=%d, ItemIndex=%d, Count=%d",
                     lpObj->Account, lpObj->Name, ir->second.Index, ir->second.ItemIndex, count + 1);*/
            }
        }
    }

    if (!itemAdded)
    {
        gLog.Output(LOG_COMMAND, "[CommandGift][%s][%s] - No items found for Gift Index: %d", lpObj->Account, lpObj->Name, currentGift->Index);
    }

    // Xử lý Buffs
    for (std::multimap<int, GIFT_LIST_BUFF_INFO>::iterator ir = this->BuffList.begin(); ir != this->BuffList.end(); ++ir)
    {
        if (ir->second.Index == currentGift->Index)
        {
            if (gEffectManager.GetInfo(ir->second.Effect) == 0)
            {
                continue;
            }

            gEffectManager.AddEffect(lpObj,
                (ir->second.Type == 0) ? 0 : 1,
                ir->second.Effect,
                (ir->second.Type == 0) ? ir->second.Duration : (int)(time(0) + ir->second.Duration),
                ir->second.Power[0], ir->second.Power[1],
                ir->second.Power[2], ir->second.Power[3]);
        }
    }

    if (currentGift->GioiHan != 0)
    {
        this->GiftCodeSend(lpObj, this->m_CurrentCode);
    }

    GCServerCommandSend(lpObj->Index, 0, lpObj->X, lpObj->Y);
    gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->GetMessage(9));
    gLog.Output(LOG_COMMAND, "[CommandGift][%s][%s] - (Active Gift: %d %s)",
        lpObj->Account, lpObj->Name, currentGift->Index, currentGift->Code);
}

void CCustomGift::GiftCodeSend(LPOBJ lpObj, const char* giftCode)
{
    SDHP_COMMAND_GIFT_SEND pMsg;

    pMsg.header.set(0x0F, 0x31, sizeof(pMsg));

    pMsg.index = lpObj->Index;
    memcpy(pMsg.Name, lpObj->Name, sizeof(pMsg.Name));
    strncpy(pMsg.GiftCode, giftCode, sizeof(pMsg.GiftCode) - 1);
    pMsg.GiftCode[sizeof(pMsg.GiftCode) - 1] = '\0';
    pMsg.GioiHan = 0; // Mặc định

    gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}