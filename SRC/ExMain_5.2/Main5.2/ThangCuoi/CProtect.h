#pragma once
#ifdef ENABLE_EDIT
#define PROTECT_IP 1
#else
#define PROTECT_IP 1
#include "NewUIEventTime.h"
#endif
#if PROTECT_IP
#define MAX_CUSTOM_ITEM				8192
#define MAX_LEN_CHAR                256

#include "CustomJewel.h"
#include "NewUIEventTime.h"
#include "CustomNpcName.h"
#include "CustomWing.h"
#include "ItemManager.h"
#include "CustomMonster.h"
#include "CustomMonsterGlow.h"
#include "CustomBow.h"
#include "CustomPet.h"
#include "CustomGloves.h"
#include "ItemTooltip.h"
#include "CustomIconBuff.h"
#include "CustomNpcNew.h"

#define WINNAME_UNI L"MU VIỆT NAM"

#define RENDER_UNICODE_0E L"Bạn đã vượt quá giới hạn của của sổ game.\nTối đa là được phép mở %d cửa sổ game thôi!"
#define ERROR_0E L"Lỗi 0x0E -> [%s]"

#define ERROR_01 L"Lỗi 0x00001"
#define RENDER_UNICODE_01 L"Lỗi khởi tạo giá trị khóa bảo mật CRC Plugin không được thực hiện!"

#define ERROR_02 L"Lỗi 0x00002"
#define RENDER_UNICODE_02 L"Lỗi chuỗi CRC không đúng với kích thước đã tạo!"

#define ERROR_03 L"Lỗi 0x00003"
#define RENDER_UNICODE_03 L"Lỗi khởi tạo Plugin bị sai!"

#define ERROR_04 L"Lỗi 0x00004"
#define RENDER_UNICODE_04 L"Lỗi khởi tạo giá trị khóa bảo mật CRC không được thực hiện!"

#define ERROR_05 L"Lỗi 0x00005"
#define RENDER_UNICODE_05 L"Lỗi xác định tiệp tin không đúng.\nYêu cầu cập nhập lại game đầy đủ với Launcher!"

#define ERROR_FF L"Lỗi 0x000FF"
#define RENDER_UNICODE_FF L"Dữ liệu truyền vào không đồng nhất! Vui lòng xem lại!\nĐể giải quyết nhanh chóng, hãy liên hệ Administrator\nZalo MU KỶ NGUYÊN: 0968 092 399 - Để Được Hỗ Trợ!"


struct MAIN_FILE_INFO
{
    char                        CustomerName[MAX_CHAR_CREATE];
    char                        ClientSerial[17];
    char                        ClientVersion[8];
    char                        IpAddress[MAX_CHAR_CREATE];
    WORD                        IpAddressPort;
    char                        WindowName[MAX_TEXT_CREATE];
    char                        ClientName[MAX_CHAR_CREATE];
    DWORD                       ClientCRC32;
    char                        ScreenShotPath[MAX_TEXT_CREATE];
    BYTE                        MaxGameInstances;
    DWORD                       ReconnectTime;
    float                       CameraDefault;
    char                        PluginName[MAX_CHAR_CREATE];
    DWORD                       PluginCRC32;
    char                        ServerName[4][32];
    float                       DefaultFPS;
    BYTE                        ButtonCharracter[10];
    BYTE                        EnableCsSkillsAllMaps;

    BYTE                        MenuButton[MAX_BUTTON_CREATE];

    CUSTOM_BOW_INFO             CustomBowInfo[MAX_CUSTOMMONSTER];
    BOSSCLASS_DATA	            CustomBossClass[MAX_BOSSCLASS];
    CUSTOMMONSTER_DATA          CustomMonsters[MAX_CUSTOMMONSTER];
    CUSTOM_ITEM_INFO            CustomItemInfo[MAX_CUSTOM_ITEM];
    CUSTOM_WING_INFO            CustomWingInfo[MAX_CUSTOM_WING];
    IsNameData                  CustomNPCName[MAX_CUSTOM_NPCNAME];
    CUSTOM_JEWEL_INFO           CustomJewelInfo[MAX_CUSTOM_JEWEL];
    CUSTOM_EVENT_INFO           CustomEventInfo[MAX_EVENTTIME];
    CUSTOM_PET_INFO             CustomPetInfo[MAX_PET_ITEM];
    CUSTOM_RF_GLOVES            CustomGloves[MAX_CUSTOM_GLOVES];
    CUSTOM_GLOW_INFO            m_CustomGlowInfo[MAX_CUSTOM_MONSTERGLOW];
    nInformation                m_TRSTooltipData[MaxLine];
    nText                       m_TRSTooltipText[MaxLine];

    RenderTooltipBuff           m_TooltipTRSDataEng[256];
    CustomModelNpcInfo          m_CustomModelInfo[50];
};

class CProtect
{
    public:
    CProtect();
    void CheckMaxGameInstances();
    void CheckPluginFile();
    void CheckClientFile();
	bool ReadMainFile(char* name);
	MAIN_FILE_INFO m_MainInfo;
    /*TOOLTIP_BMD m_ReadTooltipTRSData;*/
	DWORD m_ClientFileCRC;

    BYTE EncDecKey[2];

	char* GetScreenPath();
    void PreparetEnc();
    WORD AddressPort();
};

extern CProtect* gProtect;
extern unsigned char BuffRam[];
#endif
