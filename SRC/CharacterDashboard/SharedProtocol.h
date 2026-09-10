#pragma once

// This file intentionally contains only fixed-width Win32 types so the 32-bit
// game client and a 32/64-bit dashboard can share the exact same layout.
#include <windows.h>

namespace CharacterDashboard
{
    static const DWORD kMagic = 0x48445343; // "CSDH"
    static const DWORD kEventMagic = 0x56455343; // "CSEV"
    static const DWORD kVersion = 1;
    static const DWORD kMaxPlayers = 64;
    static const DWORD kMaxEvents = 42;
    static const char kMappingName[] = "Local\\ACuoi.CharacterDashboard.Registry.v1";
    static const char kEventMappingName[] = "Local\\ACuoi.CharacterDashboard.Events.v1";
    static const char kMutexName[] = "Local\\ACuoi.CharacterDashboard.RegistryLock.v1";
    static const char kDashboardMutexName[] = "Local\\ACuoi.CharacterDashboard.Window.v1";

    enum PlayerFlags
    {
        PlayerNone = 0,
        PlayerOnline = 1,
        PlayerOffline = 2,
    };

#pragma pack(push, 1)
    struct PlayerStatus
    {
        DWORD processId;
        DWORD lastUpdateTick;
        DWORD flags;
        char name[11];
        char mapName[32];
        DWORD level;
        DWORD reset;
        DWORD masterReset;
        DWORD currentHp;
        DWORD maximumHp;
        DWORD currentSd;
        DWORD maximumSd;
        LONG positionX;
        LONG positionY;
    };

    struct Registry
    {
        DWORD magic;
        DWORD version;
        PlayerStatus players[kMaxPlayers];
    };

    struct EventStatus
    {
        char name[40];
        char mapName[40];
        LONG secondsUntilStart;
    };

    struct EventRegistry
    {
        DWORD magic;
        DWORD version;
        DWORD lastUpdateTick;
        DWORD eventCount;
        EventStatus events[kMaxEvents];
    };
#pragma pack(pop)
}
