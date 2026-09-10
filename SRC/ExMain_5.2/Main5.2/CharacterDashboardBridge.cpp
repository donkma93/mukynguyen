#include "stdafx.h"
#include "CharacterDashboardBridge.h"
#include "MapManager.h"
#include "ZzzCharacter.h"
#include "wsclientinline.h"
#include "ThangCuoi/NewUIEventTime.h"
#include "..\\..\\CharacterDashboard\\SharedProtocol.h"

namespace
{
    HANDLE g_dashboardMapping = NULL;
    CharacterDashboard::Registry* g_dashboardRegistry = NULL;
    HANDLE g_eventMapping = NULL;
    CharacterDashboard::EventRegistry* g_eventRegistry = NULL;
    DWORD g_lastPublishTick = 0;

    void LockRegistry(HANDLE mutex)
    {
        if (mutex != NULL)
        {
            WaitForSingleObject(mutex, 50);
        }
    }

    void UnlockRegistry(HANDLE mutex)
    {
        if (mutex != NULL)
        {
            ReleaseMutex(mutex);
        }
    }

    CharacterDashboard::PlayerStatus* GetMySlot(const char* characterName, bool createIfMissing)
    {
        if (g_dashboardRegistry == NULL)
        {
            return NULL;
        }

        const DWORD processId = GetCurrentProcessId();
        CharacterDashboard::PlayerStatus* freeSlot = NULL;
        CharacterDashboard::PlayerStatus* oldestSlot = &g_dashboardRegistry->players[0];

        // A restarted Main.exe has a different PID.  Prefer the existing row
        // for this character, so restarting the game refreshes that row rather
        // than creating a second copy of the same character.
        if (characterName != NULL && characterName[0] != '\0')
        {
            for (DWORD i = 0; i < CharacterDashboard::kMaxPlayers; ++i)
            {
                CharacterDashboard::PlayerStatus* slot = &g_dashboardRegistry->players[i];
                if (slot->name[0] != '\0' && lstrcmpiA(slot->name, characterName) == 0)
                {
                    slot->processId = processId;
                    return slot;
                }
            }
        }

        for (DWORD i = 0; i < CharacterDashboard::kMaxPlayers; ++i)
        {
            CharacterDashboard::PlayerStatus* slot = &g_dashboardRegistry->players[i];
            if (slot->processId == processId)
            {
                if (characterName == NULL || characterName[0] == '\0' || lstrcmpiA(slot->name, characterName) == 0)
                {
                    return slot;
                }
                slot->flags = CharacterDashboard::PlayerOffline;
                slot->processId = 1;
                continue;
            }
            if (slot->processId == 0 && freeSlot == NULL)
            {
                freeSlot = slot;
            }
            if (slot->lastUpdateTick < oldestSlot->lastUpdateTick)
            {
                oldestSlot = slot;
            }
        }

        if (!createIfMissing)
        {
            return NULL;
        }

        CharacterDashboard::PlayerStatus* slot = freeSlot != NULL ? freeSlot : oldestSlot;
        ZeroMemory(slot, sizeof(*slot));
        slot->processId = processId;
        return slot;
    }

    void StartDashboardProcess()
    {
        char dashboardPath[MAX_PATH] = { 0 };
        if (GetModuleFileName(NULL, dashboardPath, MAX_PATH) == 0)
        {
            return;
        }

        char* slash = strrchr(dashboardPath, '\\');
        if (slash == NULL)
        {
            return;
        }
        strcpy(slash + 1, "CharacterDashboard.exe");
        if (GetFileAttributes(dashboardPath) == INVALID_FILE_ATTRIBUTES)
        {
            return;
        }

        STARTUPINFO startupInfo;
        PROCESS_INFORMATION processInfo;
        ZeroMemory(&startupInfo, sizeof(startupInfo));
        ZeroMemory(&processInfo, sizeof(processInfo));
        startupInfo.cb = sizeof(startupInfo);

        char commandLine[MAX_PATH + 32] = { 0 };
        sprintf(commandLine, "\"%s\" --auto", dashboardPath);
        if (CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo))
        {
            CloseHandle(processInfo.hThread);
            CloseHandle(processInfo.hProcess);
        }
    }

    void PublishEvents(DWORD now)
    {
        if (g_eventRegistry == NULL)
        {
            return;
        }

        CUSTOM_EVENTTIME_DISPLAY sourceEvents[CharacterDashboard::kMaxEvents];
        ZeroMemory(sourceEvents, sizeof(sourceEvents));
        const int eventCount = gCETime.CopyEventTimeDisplay(sourceEvents, CharacterDashboard::kMaxEvents);

        HANDLE mutex = CreateMutex(NULL, FALSE, CharacterDashboard::kMutexName);
        LockRegistry(mutex);
        bool changed = g_eventRegistry->eventCount != static_cast<DWORD>(eventCount);
        if (!changed)
        {
            for (int i = 0; i < eventCount; ++i)
            {
                const CharacterDashboard::EventStatus& current = g_eventRegistry->events[i];
                if (current.secondsUntilStart != sourceEvents[i].SecondsUntilStart ||
                    strncmp(current.name, sourceEvents[i].Name, sizeof(current.name)) != 0 ||
                    strncmp(current.mapName, sourceEvents[i].Map, sizeof(current.mapName)) != 0)
                {
                    changed = true;
                    break;
                }
            }
        }

        if (changed)
        {
            ZeroMemory(g_eventRegistry->events, sizeof(g_eventRegistry->events));
            for (int i = 0; i < eventCount; ++i)
            {
                CharacterDashboard::EventStatus& destination = g_eventRegistry->events[i];
                strncpy(destination.name, sourceEvents[i].Name, sizeof(destination.name) - 1);
                strncpy(destination.mapName, sourceEvents[i].Map, sizeof(destination.mapName) - 1);
                destination.secondsUntilStart = sourceEvents[i].SecondsUntilStart;
            }
            g_eventRegistry->eventCount = eventCount;
            g_eventRegistry->lastUpdateTick = now;
        }
        UnlockRegistry(mutex);
        if (mutex != NULL)
        {
            CloseHandle(mutex);
        }
    }
}

void InitializeCharacterDashboard()
{
    if (g_dashboardRegistry != NULL)
    {
        return;
    }

    g_dashboardMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
        sizeof(CharacterDashboard::Registry), CharacterDashboard::kMappingName);
    if (g_dashboardMapping == NULL)
    {
        return;
    }

    g_dashboardRegistry = reinterpret_cast<CharacterDashboard::Registry*>(MapViewOfFile(
        g_dashboardMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CharacterDashboard::Registry)));
    if (g_dashboardRegistry == NULL)
    {
        CloseHandle(g_dashboardMapping);
        g_dashboardMapping = NULL;
        return;
    }

    g_eventMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
        sizeof(CharacterDashboard::EventRegistry), CharacterDashboard::kEventMappingName);
    if (g_eventMapping != NULL)
    {
        g_eventRegistry = reinterpret_cast<CharacterDashboard::EventRegistry*>(MapViewOfFile(
            g_eventMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CharacterDashboard::EventRegistry)));
        if (g_eventRegistry != NULL)
        {
            HANDLE mutex = CreateMutex(NULL, FALSE, CharacterDashboard::kMutexName);
            LockRegistry(mutex);
            if (g_eventRegistry->magic != CharacterDashboard::kEventMagic ||
                g_eventRegistry->version != CharacterDashboard::kVersion)
            {
                ZeroMemory(g_eventRegistry, sizeof(CharacterDashboard::EventRegistry));
                g_eventRegistry->magic = CharacterDashboard::kEventMagic;
                g_eventRegistry->version = CharacterDashboard::kVersion;
            }
            UnlockRegistry(mutex);
            if (mutex != NULL)
            {
                CloseHandle(mutex);
            }
        }
    }

    HANDLE mutex = CreateMutex(NULL, FALSE, CharacterDashboard::kMutexName);
    LockRegistry(mutex);
    if (g_dashboardRegistry->magic != CharacterDashboard::kMagic ||
        g_dashboardRegistry->version != CharacterDashboard::kVersion)
    {
        ZeroMemory(g_dashboardRegistry, sizeof(CharacterDashboard::Registry));
        g_dashboardRegistry->magic = CharacterDashboard::kMagic;
        g_dashboardRegistry->version = CharacterDashboard::kVersion;
    }
    UnlockRegistry(mutex);
    if (mutex != NULL)
    {
        CloseHandle(mutex);
    }

    StartDashboardProcess();
}

void UpdateCharacterDashboard()
{
    if (g_dashboardRegistry == NULL)
    {
        return;
    }

    const DWORD now = GetTickCount();
    if (now - g_lastPublishTick < 200)
    {
        return;
    }
    g_lastPublishTick = now;

    const bool online = Hero != NULL && CharacterAttribute != NULL && Hero->ID[0] != '\0' &&
        gMapManager.WorldActive >= 0 &&
        gMapManager.WorldActive != WD_54CHARACTERSCENE &&
        gMapManager.WorldActive != WD_55LOGINSCENE &&
        gMapManager.WorldActive != WD_73NEW_LOGIN_SCENE &&
        gMapManager.WorldActive != WD_74NEW_CHARACTER_SCENE;
    HANDLE mutex = CreateMutex(NULL, FALSE, CharacterDashboard::kMutexName);
    LockRegistry(mutex);
    CharacterDashboard::PlayerStatus* slot = GetMySlot(online ? Hero->ID : NULL, online);
    if (slot != NULL && online)
    {
        slot->lastUpdateTick = now;
        slot->flags = CharacterDashboard::PlayerOnline;

        if (online)
        {
            strncpy(slot->name, Hero->ID, sizeof(slot->name) - 1);
            slot->name[sizeof(slot->name) - 1] = '\0';
            const char* mapName = gMapManager.GetMapName(gMapManager.WorldActive);
            if (mapName != NULL)
            {
                strncpy(slot->mapName, mapName, sizeof(slot->mapName) - 1);
                slot->mapName[sizeof(slot->mapName) - 1] = '\0';
            }
            slot->level = CharacterAttribute->Level;
            slot->reset = CharacterAttribute->ViewReset;
            slot->masterReset = CharacterAttribute->ViewMReset;
            slot->currentHp = CharacterAttribute->ViewCurHP;
            slot->maximumHp = CharacterAttribute->ViewMaxHP;
            slot->currentSd = CharacterAttribute->ViewCurSD;
            slot->maximumSd = CharacterAttribute->ViewMaxSD;
            slot->positionX = Hero->PositionX;
            slot->positionY = Hero->PositionY;

            // Clean stale rows from older builds/restarts.  A character name
            // must have exactly one dashboard row, even if its process ID has
            // changed or another stale mapping entry remains.
            for (DWORD i = 0; i < CharacterDashboard::kMaxPlayers; ++i)
            {
                CharacterDashboard::PlayerStatus* other = &g_dashboardRegistry->players[i];
                if (other != slot && other->name[0] != '\0' && lstrcmpiA(other->name, slot->name) == 0)
                {
                    ZeroMemory(other, sizeof(*other));
                }
            }
        }
    }
    UnlockRegistry(mutex);
    if (mutex != NULL)
    {
        CloseHandle(mutex);
    }

    // Event names/times for the standalone dashboard come from GameServer
    // directly.  Main no longer publishes GetMain CustomEventTime.txt.
}

void ShutdownCharacterDashboard()
{
    if (g_dashboardRegistry == NULL)
    {
        return;
    }

    HANDLE mutex = CreateMutex(NULL, FALSE, CharacterDashboard::kMutexName);
    LockRegistry(mutex);
    // Keep the saved character row, but mark it offline as Main.exe exits.
    // The dashboard will retain its details and render its HP/SD in grey.
    const char* characterName = Hero != NULL && Hero->ID[0] != '\0' ? Hero->ID : NULL;
    CharacterDashboard::PlayerStatus* slot = GetMySlot(characterName, false);
    if (slot != NULL)
    {
        slot->flags = CharacterDashboard::PlayerOffline;
        slot->lastUpdateTick = GetTickCount();
    }
    UnlockRegistry(mutex);
    if (mutex != NULL)
    {
        CloseHandle(mutex);
    }

    UnmapViewOfFile(g_dashboardRegistry);
    CloseHandle(g_dashboardMapping);
    if (g_eventRegistry != NULL)
    {
        UnmapViewOfFile(g_eventRegistry);
    }
    if (g_eventMapping != NULL)
    {
        CloseHandle(g_eventMapping);
    }
    g_dashboardRegistry = NULL;
    g_dashboardMapping = NULL;
    g_eventRegistry = NULL;
    g_eventMapping = NULL;
}
