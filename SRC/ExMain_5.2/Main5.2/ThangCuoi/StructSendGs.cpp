#include "Stdafx.h"
#include "ThangCuoi\StructSendGs.h"
#include "_struct.h"
#include <ZzzCharacter.h>
#include <NewUISystem.h>

cMuOffHelper gMuHelper;
cOffStore gOffStore;
float g_fScreenRate1_x = 0;
float g_fScreenRate1_y = 0;
SPK_SHARED_INFO g_SPKSharedInfo = { 0 };
GetCoinReceived pGetCoin;

GetValueSpeedAttackClass pGetSPeedClass;

void GCNewCharacterInfoRecv(const BYTE* lpMsg)
{
    auto DataSPK = reinterpret_cast<const PMSG_NEW_CHARACTER_INFO_SEND*>(lpMsg);
    g_ConsoleDebug->Write(3, "0xF3, 0xE0 PMSG_NEW_CHARACTER_INFO_SEND size: %d bytes", sizeof(*DataSPK));

    CharacterAttribute->Level               = DataSPK->Level;
    CharacterAttribute->LevelUpPoint        = DataSPK->LevelUpPoint;
    CharacterAttribute->Experience          = DataSPK->Experience;
    CharacterAttribute->NextExperince       = DataSPK->NextExperience;
    CharacterAttribute->Strength            = DataSPK->Strength;
    CharacterAttribute->Dexterity           = DataSPK->Dexterity;
    CharacterAttribute->Vitality            = DataSPK->Vitality;
    CharacterAttribute->Energy              = DataSPK->Energy;
    CharacterAttribute->Charisma            = DataSPK->Leadership;
    CharacterAttribute->Life                = DataSPK->Life;
    CharacterAttribute->LifeMax             = DataSPK->MaxLife;
    CharacterAttribute->Mana                = DataSPK->Mana;
    CharacterAttribute->ManaMax             = DataSPK->MaxMana;
    CharacterAttribute->SkillMana           = DataSPK->BP;
    CharacterAttribute->SkillManaMax        = DataSPK->MaxBP;
    CharacterAttribute->Shield              = DataSPK->Shield;
    CharacterAttribute->ShieldMax           = DataSPK->MaxShield;
    CharacterAttribute->AddPoint            = DataSPK->FruitAddPoint;
    CharacterAttribute->MaxAddPoint         = DataSPK->MaxFruitAddPoint;
    CharacterAttribute->wMinusPoint         = DataSPK->FruitSubPoint;
    CharacterAttribute->wMaxMinusPoint      = DataSPK->MaxFruitSubPoint;

    CharacterAttribute->ViewReset           = DataSPK->ViewReset;
    CharacterAttribute->ViewMReset          = DataSPK->ViewMReset;
    CharacterAttribute->ViewPoint           = DataSPK->ViewPoint;
    CharacterAttribute->ViewCurHP           = DataSPK->ViewCurHP;
    CharacterAttribute->ViewMaxHP           = DataSPK->ViewMaxHP;
    CharacterAttribute->ViewCurMP           = DataSPK->ViewCurMP;
    CharacterAttribute->ViewMaxMP           = DataSPK->ViewMaxMP;
    CharacterAttribute->ViewCurBP           = DataSPK->ViewCurBP;
    CharacterAttribute->ViewMaxBP           = DataSPK->ViewMaxBP;
    CharacterAttribute->ViewCurSD           = DataSPK->ViewCurSD;
    CharacterAttribute->ViewMaxSD           = DataSPK->ViewMaxSD;
    CharacterAttribute->ViewStrength        = DataSPK->ViewStrength;
    CharacterAttribute->ViewDexterity       = DataSPK->ViewDexterity;
    CharacterAttribute->ViewVitality        = DataSPK->ViewVitality;
    CharacterAttribute->ViewEnergy          = DataSPK->ViewEnergy;
    CharacterAttribute->ViewLeadership      = DataSPK->ViewLeadership;
    CharacterAttribute->ViewMaxReset        = DataSPK->ViewMaxReset;
    CharacterAttribute->InventoryExtensions = DataSPK->InventoryExtensions;

    CharacterAttribute->AddStrength         = DataSPK->ViewAddPoint[0];
    CharacterAttribute->AddDexterity        = DataSPK->ViewAddPoint[1];
    CharacterAttribute->AddVitality         = DataSPK->ViewAddPoint[2];
    CharacterAttribute->AddEnergy           = DataSPK->ViewAddPoint[3];
    CharacterAttribute->AddCharisma         = DataSPK->ViewAddPoint[3];


    CharacterAttribute->ManaClassReg[0] = DataSPK->ManaClassReg[0]; // DK
    CharacterAttribute->ManaClassReg[1] = DataSPK->ManaClassReg[1]; // DW
    CharacterAttribute->ManaClassReg[2] = DataSPK->ManaClassReg[2]; // EF
    CharacterAttribute->ManaClassReg[3] = DataSPK->ManaClassReg[3]; // MG
    CharacterAttribute->ManaClassReg[4] = DataSPK->ManaClassReg[4]; // DL
    CharacterAttribute->ManaClassReg[5] = DataSPK->ManaClassReg[5]; // SU
    CharacterAttribute->ManaClassReg[6] = DataSPK->ManaClassReg[6]; // RF A
    CharacterAttribute->ManaClassReg[7] = DataSPK->ManaClassReg[7]; // RF B

    if (Hero && Hero->ID[0] != '\0')
    {
        strncpy(g_SPKSharedInfo.CharacterName, Hero->ID, 10);
        g_SPKSharedInfo.CharacterName[10] = '\0';
    }

    g_ConsoleDebug->Write(MCD_RECEIVE, "0xE0 [CharacterInfoRecv]");
}

void GCNewCharacterCalcSend(const BYTE* lpMsg)
{
    auto DataSPK = reinterpret_cast<const PMSG_NEW_CHARACTER_CALC_SEND*>(lpMsg);
    g_ConsoleDebug->Write(3, "0xF3, 0xE1 PMSG_NEW_CHARACTER_CALC_SEND size: %d bytes", sizeof(*DataSPK));

    CharacterAttribute->ViewCurHP       = DataSPK->ViewCurHP;
    CharacterAttribute->ViewMaxHP       = DataSPK->ViewMaxHP;
    CharacterAttribute->ViewCurMP       = DataSPK->ViewCurMP;
    CharacterAttribute->ViewMaxMP       = DataSPK->ViewMaxMP;
    CharacterAttribute->ViewCurBP       = DataSPK->ViewCurBP;
    CharacterAttribute->ViewMaxBP       = DataSPK->ViewMaxBP;
    CharacterAttribute->ViewCurSD       = DataSPK->ViewCurSD;
    CharacterAttribute->ViewMaxSD       = DataSPK->ViewMaxSD;
    CharacterAttribute->AddStrength     = DataSPK->ViewAddStrength;
    CharacterAttribute->AddDexterity    = DataSPK->ViewAddDexterity;
    CharacterAttribute->AddVitality     = DataSPK->ViewAddVitality;
    CharacterAttribute->AddEnergy       = DataSPK->ViewAddEnergy;
    CharacterAttribute->AddCharisma     = DataSPK->ViewAddLeadership;

    CharacterAttribute->ViewExperience      = DataSPK->Experience;
    CharacterAttribute->ViewNextExperience  = DataSPK->NextExperience;

    CharacterAttribute->AttackSpeed     = DataSPK->ViewPhysiSpeed;
    CharacterAttribute->MagicSpeed      = DataSPK->ViewMagicSpeed;

    CharacterAttribute->Strength        = DataSPK->Strength;
    CharacterAttribute->Dexterity       = DataSPK->Dexterity;
    CharacterAttribute->Vitality        = DataSPK->Vitality;
    CharacterAttribute->Energy          = DataSPK->Energy;
    CharacterAttribute->Charisma        = DataSPK->Leadership;
    CharacterAttribute->Life            = DataSPK->Life;
    CharacterAttribute->LifeMax         = DataSPK->MaxLife;
    CharacterAttribute->Mana            = DataSPK->Mana;
    CharacterAttribute->ManaMax         = DataSPK->MaxMana;
    CharacterAttribute->SkillMana       = DataSPK->BP;
    CharacterAttribute->SkillManaMax    = DataSPK->MaxBP;
    CharacterAttribute->Shield          = DataSPK->Shield;
    CharacterAttribute->ShieldMax       = DataSPK->MaxShield;

    CharacterAttribute->ViewStrength    = DataSPK->ViewStrength;
    CharacterAttribute->ViewDexterity   = DataSPK->ViewDexterity;
    CharacterAttribute->ViewVitality    = DataSPK->ViewVitality;
    CharacterAttribute->ViewEnergy      = DataSPK->ViewEnergy;
    CharacterAttribute->ViewLeadership  = DataSPK->ViewLeadership;
    CharacterAttribute->InventoryExtensions = DataSPK->InventoryExtensions;

    g_ConsoleDebug->Write(MCD_RECEIVE, "F3:E1 [GCNewCharacterCalcSend]");
}

void GCRecvCoin(const BYTE* lpMsg)
{
    auto DataSPK = reinterpret_cast<const GetReceivedCoinData*>(lpMsg);
    for (int SPK = 0; SPK < MAX_COIN_POINT; SPK++)
    {
        pGetCoin.ThisCoin[SPK] = DataSPK->ThisCoin[SPK];
    }
}

void GCRecvSpeedAttack(const BYTE* lpMsg)
{
    auto DataSPK = reinterpret_cast<const GetValueSpeedAttack*>(lpMsg);
    for (int SPK = 0; SPK < 14; SPK++)
    {
        pGetSPeedClass.ThisValue[SPK] = DataSPK->ThisValue[SPK];
        //g_ConsoleDebug->Write(3, "F3:ED [%d]" , pGetSPeedClass.ThisValue[SPK]);
    }
}

const char* ItemNameBank[20] =
{
    "Ngọc Ước Nguyện",
    "Ngọc Tâm Linh",
    "Ngọc Sinh Mệnh",
    "Ngọc Sáng Tạo",
    "Đá Hộ Mệnh",
    "Đá Nguyên Thủy",
    "Đá Tạo Hóa",
    "Ngọc Hỗn Nguyên",
    "Đá Cấp Thấp",
    "Đá Cấp Cao",
    "Ngọc Cấp Độ",
    "Ngọc Kỹ Năng",
    "Ngọc May Mắn",
    "Ngọc Tính Năng",
    "Ngọc Hoàn Hảo",
    "Ngọc Socket",
    "Ngọc Thần 1",
    "Ngọc Thần 2",
    "Ngọc Full",
    "Ngọc Nâng Cấp"
};


void cOffStore::RecvPShop(const BYTE* Data)
{
    auto DataSPK = reinterpret_cast<const RecvOffTradeType*>(Data);
    this->ShopList = 1;
    this->TypeShop = DataSPK->Type;
}

void cOffStore::PShopActiveRecv(const BYTE* Data)
{
    auto DataSPK = reinterpret_cast<const LoadOffTradeData*>(Data);
    this->ShopActive = DataSPK->Active;
    this->TypeShop = DataSPK->Type;
}

void GCCTCMiniShowWindowRecv(const BYTE* Data)
{
    auto lpMsg = reinterpret_cast<const EventCastleSendClient*>(Data);

    IsToolKit.EnterSerectArea = lpMsg->GuildWinOld;

    IsToolKit.ValueCoin[0] = lpMsg->ValueCoin[0];
    IsToolKit.ValueCoin[1] = lpMsg->ValueCoin[1];
    IsToolKit.ValueCoin[2] = lpMsg->ValueCoin[2];

    memcpy(IsToolKit.GuildNameOld, lpMsg->GuildNameOld, sizeof(IsToolKit.GuildNameOld));

    if (lpMsg->OpenWindow == 1 && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CASTLEEVENT))
    {
        g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CASTLEEVENT);
    }
}

void GCCTCMiniKillRecv(const BYTE* Data)
{
    auto lpMsg = reinterpret_cast<const EventCastleKillSendClient*>(Data);

    IsToolKit.EventCurrentTime = lpMsg->EventCurrentTime;

    IsToolKit.StartTick = GetTickCount();

    for (int n = 0; n < 3; n++)
    {
        IsToolKit.EventGate[n]          = lpMsg->EventGate[n];
        IsToolKit.EventClassMonster[n]  = lpMsg->EventClassMonster[n];
    }
}