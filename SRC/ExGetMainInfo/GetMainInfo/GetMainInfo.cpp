#include "stdafx.h"
#include "..\..\Include\CCRC32.H"
#include "ThemidaSDK.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <io.h> 
#include <fcntl.h>
#include "CustomJewel.h"
#include "CustomEventTime.h"
#include "CustomNpcName.h"
#include "CustomWing.h"
#include "CustomItem.h"
#include "CustomMonster.h"
#include "CustomMonsterGlow.h"
#include "CustomBow.h"
#include "CustomPet.h"
#include "CustomGloves.h"
#include "ItemTooltip.h"
#include "CustomIconBuff.h"
#include "CustomNpcNew.h"

unsigned char xORKey[2] =
{
    0x20, 0x20
};

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


void LoadAndCheckFile(const std::string& filepath, std::ofstream& fout, const std::string& NameFileOut)
{
    std::ifstream file(filepath);
    std::string tab;

    if (NameFileOut.length() >= 17) {
        tab = "\t";
    }
    else if (NameFileOut.length() > 11) {
        tab = "\t\t";
    }
    else {
        tab = "\t\t\t";
    }

    fout << NameFileOut << tab << "= " << (file ? "FOUND OK!" : "NOT FOUND!") << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
    SetConsoleTitleW(L"GetMain MU 5.2 Bởi ACuoi");
    HWND consoleWindow = GetConsoleWindow();

    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style &= ~WS_MAXIMIZEBOX;
    style &= ~WS_MINIMIZEBOX;
    SetWindowLong(consoleWindow, GWL_STYLE, style);

    int windowWidth = 600;
    int windowHeight = 700;

    MoveWindow(consoleWindow, -5, 0, windowWidth, windowHeight, TRUE);

    CLEAR_START
    ENCODE_START

    MAIN_FILE_INFO info;

    CreateDirectory("..\\Client", 0);
    CreateDirectory("..\\Client\\Data", 0);
    CreateDirectory("..\\Client\\Data\\ACuoi", 0);

    memset(&info, 0, sizeof(info));
    cout << "==============================================================" << endl;
    cout << "==================Ban Quyen 5.2 ThangCuoi=====================" << endl;
    cout << "=====================Zalo:BQT MU KỶ NGUYÊN========================" << endl;
    cout << "============Cung Cap Sever Mu online Lien He Ngay=============" << endl;

    GetPrivateProfileString("ThangCuoi", "CustomerName", "", info.CustomerName, sizeof(info.CustomerName), ".\\GetMain.ini");
    std::cout << " Server Name:\t	" << info.CustomerName << std::endl;

    GetPrivateProfileString("ThangCuoi", "ClientSerial", "", info.ClientSerial, sizeof(info.ClientSerial), ".\\GetMain.ini");
    std::cout << " Client Serial:\t	" << info.ClientSerial << std::endl;

    GetPrivateProfileString("ThangCuoi", "ClientVersion", "", info.ClientVersion, sizeof(info.ClientVersion), ".\\GetMain.ini");
    std::cout << " Client Version:\t" << info.ClientVersion << std::endl;

    std::cout << "==============================================================" << std::endl;

    GetPrivateProfileString("ThangCuoi", "IpAddress", "", info.IpAddress, sizeof(info.IpAddress), ".\\GetMain.ini");
    std::cout << " Direct IP:\t\t" << info.IpAddress << std::endl;

    info.IpAddressPort = GetPrivateProfileInt("ThangCuoi", "IpAddressPort", 44405, ".\\GetMain.ini");
    std::cout << " Direct Port:\t\t" << info.IpAddressPort << std::endl;

    GetPrivateProfileString("ThangCuoi", "WindowName", "[MU]", info.WindowName, sizeof(info.WindowName), ".\\GetMain.ini");
    std::cout << " Window Title:\t\t" << info.WindowName << std::endl;

    GetPrivateProfileString("ThangCuoi", "ClientName", "", info.ClientName, sizeof(info.ClientName), ".\\GetMain.ini");
    std::cout << " Client Main:\t\t" << info.ClientName << std::endl;

    GetPrivateProfileString("ThangCuoi", "ScreenShotPath", "", info.ScreenShotPath, sizeof(info.ScreenShotPath), ".\\GetMain.ini");
    std::cout << " SnapShot Folder:\t" << info.ScreenShotPath << std::endl;

    GetPrivateProfileString("ThangCuoi", "PluginName", "", info.PluginName, sizeof(info.PluginName), ".\\GetMain.ini");
    std::cout << " PluginName:\t\t" << info.PluginName << std::endl;

    for (int i = 0; i < 4; ++i)
    {
        char keyList[32];
        char keyName[32];
        sprintf(keyName, "ServerName_%d", i + 1);
        GetPrivateProfileString("ThangCuoi", keyName, "", info.ServerName[i], sizeof(info.ServerName[i]), ".\\GetMain.ini");
    }

    info.MaxGameInstances    = GetPrivateProfileInt("ThangCuoi", "MaxGameInstances",    0, ".\\GetMain.ini");
    info.ReconnectTime       = GetPrivateProfileInt("ThangCuoi", "ReconnectTime",       0, ".\\GetMain.ini");
    info.CameraDefault       = GetPrivateProfileInt("ThangCuoi", "CameraDefault",       0, ".\\GetMain.ini");
    info.DefaultFPS          = GetPrivateProfileInt("ThangCuoi", "DefaultFPS",          0, ".\\GetMain.ini");
    info.ButtonCharracter[0] = GetPrivateProfileInt("ThangCuoi", "ButtonCharracter",    0, ".\\GetMain.ini");

    info.ButtonCharracter[1] = GetPrivateProfileInt("ThangCuoi", "ButtonShopJwBless",   0, ".\\GetMain.ini");
    info.ButtonCharracter[2] = GetPrivateProfileInt("ThangCuoi", "ButtonShopJwSoul",    0, ".\\GetMain.ini");
    info.ButtonCharracter[3] = GetPrivateProfileInt("ThangCuoi", "ButtonShopChaos",     0, ".\\GetMain.ini");

    info.ButtonCharracter[4] = GetPrivateProfileInt("ThangCuoi", "ButtonShopWcoinC",    0, ".\\GetMain.ini");
    info.ButtonCharracter[5] = GetPrivateProfileInt("ThangCuoi", "ButtonShopWcoinP",    0, ".\\GetMain.ini");
    info.ButtonCharracter[6] = GetPrivateProfileInt("ThangCuoi", "ButtonShopWcoinG",    0, ".\\GetMain.ini");

    info.ButtonCharracter[8] = GetPrivateProfileInt("ThangCuoi", "ButtonShopZens",      0, ".\\GetMain.ini");

    info.EnableCsSkillsAllMaps = GetPrivateProfileInt("ThangCuoi", "EnableCsSkillsAllMaps", 0, ".\\GetMain.ini");

    for (int i = 0; i < 15; ++i)
    {
        char key[32];
        sprintf(key, "MENU_BUTTON_%02d", i + 1);
        info.MenuButton[i] = GetPrivateProfileInt("ThangCuoi", key, 0, ".\\GetMain.ini");
    }
  
    gCustomModelNpc.Load("Data\\CustomModelNpc.txt");
    memcpy(info.m_CustomModelInfo, gCustomModelNpc.m_CustomModelInfo, sizeof(info.m_CustomModelInfo));

    gCBuffIcon.Load("Data\\BuffEffect.txt", 0);
    memcpy(info.m_TooltipTRSDataEng, gCBuffIcon.m_TooltipENG, sizeof(info.m_TooltipTRSDataEng));

    gItemToolTip.Load("Data\\ItemToolTips\\JCItemToolTip.txt", 1);
    memcpy(info.m_TRSTooltipData, gItemToolTip.n_TRSTooltipData, sizeof(info.m_TRSTooltipData));

    gItemToolTip.Load("Data\\ItemToolTips\\JCTextTooltip.txt", 2);
    memcpy(info.m_TRSTooltipText, gItemToolTip.n_TRSTooltipText, sizeof(info.m_TRSTooltipText));

    gCustomBow.Load("Data\\CustomBowCross.txt");
    memcpy(info.CustomBowInfo, gCustomBow.m_CustomBow, sizeof(info.CustomBowInfo));

    gCustomMonsterGlow.Load("Data\\CustomMonsterGlow.txt");
    memcpy(info.m_CustomGlowInfo, gCustomMonsterGlow.m_CustomGlowInfo, sizeof(info.m_CustomGlowInfo));

    gCustomPet.Load("Data\\CustomPet.txt");
    memcpy(info.CustomPetInfo, gCustomPet.m_CustomPetInfo, sizeof(info.CustomPetInfo));

    gMonsters.Load("Data\\CustomMonster.txt");
    memcpy(info.CustomMonsters, gMonsters.m_Monsters, sizeof(info.CustomMonsters));
    memcpy(info.CustomBossClass, gMonsters.m_BossClass, sizeof(info.CustomBossClass)); //Custom Boss Class

    gCustomGloves.Load("Data\\CustomClaws.txt");
    memcpy(info.CustomGloves, gCustomGloves.m_CustomGlovesInfo, sizeof(info.CustomGloves));
    gCustomItem.Load("Data\\CustomItem.txt");
    memcpy(info.CustomItemInfo, gCustomItem.m_CustomItemInfo, sizeof(info.CustomItemInfo));

    gCustomWing.Load("Data\\CustomWing.txt");
    memcpy(info.CustomWingInfo, gCustomWing.m_CustomWingInfo, sizeof(info.CustomWingInfo));
    gNPCName.Load("Data\\CustomNameNpc.txt");
    memcpy(info.CustomNPCName, gNPCName.m_CustomNpcName, sizeof(info.CustomNPCName));

    gCustomJewel.Load("Data\\CustomJewel.txt");
    memcpy(info.CustomJewelInfo, gCustomJewel.m_CustomJewelInfo, sizeof(info.CustomJewelInfo));

    gEventTimeText.Load("Data\\CustomEventTime.txt");
    memcpy(info.CustomEventInfo, gEventTimeText.m_CustomEventInfo, sizeof(info.CustomEventInfo));

    CCRC32 CRC32;

    char ClientNameCrC[MAX_PATH] = { 0 };
    sprintf(ClientNameCrC, "..\\Client\\%s", info.ClientName);

    if (CRC32.FileCRC(ClientNameCrC, &info.ClientCRC32, 1024) == 0)
    {
        info.ClientCRC32 = 0;
    }

    char PluginCrC[MAX_PATH] = { 0 };
    sprintf(PluginCrC, "..\\Client\\%s", info.PluginName);
    if (CRC32.FileCRC(PluginCrC, &info.PluginCRC32, 1024) == 0)
    {
        info.PluginCRC32 = 0;
    }

    DWORD MainEXECRC, PlayerCRC;

    if (CRC32.FileCRC("..\\Client\\Main.exe", &MainEXECRC, 1024) == 0) { MainEXECRC = 0; }
    if (CRC32.FileCRC("..\\Client\\Data\\Player\\player.bmd", &PlayerCRC, 1024) == 0) { PlayerCRC = 0; }

    std::cout << " " << std::endl;
    std::cout << "==============================================================" << std::endl;
    std::cout << "===================== MAIN CRC DETECTOR ======================" << std::endl;
    std::cout << "==============================================================" << std::endl;
    std::cout << " " << std::endl;

    std::cout << " Main\t\t= 0x" << std::uppercase << std::hex << (DWORD)MainEXECRC << std::endl;
    std::cout << " Player\t\t= 0x" << std::uppercase << std::hex << (DWORD)PlayerCRC << std::endl;

    std::ofstream fout(".\\ACuoi_CRCFILE.ini", std::ios_base::out | std::ios_base::trunc);

    cout << "==============================================================" << endl;
    cout << "==================Ban Quyen 5.2 ThangCuoi=====================" << endl;
    cout << "=====================Zalo:BQT MU KỶ NGUYÊN========================" << endl;
    cout << "============Cung Cap Sever Mu online Lien He Ngay=============" << endl;
    fout << "ACuoi_MEXE\t\t\t= 0x" << std::uppercase << std::hex << (DWORD)MainEXECRC << std::endl;
    fout << "ACuoi_PBMD\t\t\t= 0x" << std::uppercase << std::hex << (DWORD)PlayerCRC << std::endl;
    fout << ";==============================================================" << std::endl;


    LoadAndCheckFile("Data\\CustomModelNpc.txt",                        fout, "CustomModelNpc");
    LoadAndCheckFile("Data\\BuffEffect.txt",                            fout, "BuffEffect");
    LoadAndCheckFile("Data\\ItemToolTips\\JCItemToolTip.txt",           fout, "JCItemToolTip");
    LoadAndCheckFile("Data\\ItemToolTips\\JCTextTooltip.txt",           fout, "JCTextTooltip");
    LoadAndCheckFile("Data\\CustomClaws.txt",                           fout, "CustomClaws");
    LoadAndCheckFile("Data\\CustomPet.txt",                             fout, "CustomPet");
    LoadAndCheckFile("Data\\CustomBowCross.txt",                        fout, "CustomBowCross");
    LoadAndCheckFile("Data\\CustomMonsterGlow.txt",                     fout, "CustomMonsterGlow");
    LoadAndCheckFile("Data\\CustomMonster.txt",                         fout, "CustomMonster");
    LoadAndCheckFile("Data\\CustomItem.txt",                            fout, "CustomItem");
    LoadAndCheckFile("Data\\CustomWing.txt",                            fout, "CustomWing");
    LoadAndCheckFile("Data\\CustomNameNpc.txt",                         fout, "CustomNameNpc");
    LoadAndCheckFile("Data\\CustomJewel.txt",                           fout, "CustomJewel");
    LoadAndCheckFile("Data\\CustomEventTime.txt",                       fout, "CustomEventTime");

    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    const char* daysOfWeek[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

    fout << ";==============================================================" << std::endl;
    fout << "; " << daysOfWeek[localTime->tm_wday] << ", " << std::put_time(localTime, "%H:%M:%S %d/%m/%Y") << std::endl;
    fout << ";==============================================================" << std::endl;

    fout.close();

    for (int i = 0; i < sizeof(MAIN_FILE_INFO); i++)
    {
        ((BYTE*)&info)[i] ^= xORKey[i % 2];
    }

    HANDLE file = CreateFile("..\\Client\\Data\\ACuoi\\ServerData.bmd", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, 0);

    if (file == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    DWORD OutSize = 0;

    if (WriteFile(file, &info, sizeof(MAIN_FILE_INFO), &OutSize, 0) == 0)
    {
        CloseHandle(file);
        return 0;
    }

    CloseHandle(file);

    std::cout << " " << std::endl;
    std::cout << "===============================================================" << std::endl;
    std::cout << "= Getmain File has been successfully copied to the path       =" << std::endl;
    std::cout << "= Patch folder: Client/Data/ACuoi/ServerData.bmd                =" << std::endl;
    std::cout << "= Press any key to close the program.                         =" << std::endl;
    std::cout << "===============================================================" << std::endl;

    ENCODE_END

    CLEAR_END
   // Sleep(3000);
    getchar();
    return 0;
}
