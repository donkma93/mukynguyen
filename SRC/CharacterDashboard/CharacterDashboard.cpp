#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <windowsx.h>
#include <algorithm>
#include <atomic>
#include <thread>
#include <vector>
#include <string>
#include <cstdio>
#include "SharedProtocol.h"

#pragma comment(lib, "Ws2_32.lib")

namespace
{
    // Keep the panel close to the compact in-game reference window.
    const int kWindowWidth = 350;
    const int kWindowHeight = 405;
    const int kTitleHeight = 42;
    const int kTabHeight = 38;
    const int kRowHeight = 68;

    HANDLE g_mapping = NULL;
    CharacterDashboard::Registry* g_registry = NULL;
    HANDLE g_eventMapping = NULL;
    CharacterDashboard::EventRegistry* g_eventRegistry = NULL;
    HANDLE g_singleInstance = NULL;
    HMODULE g_mainResourceModule = NULL;
    HICON g_gameLogo = NULL;
    char g_iniPath[MAX_PATH] = { 0 };
    HANDLE g_eventStopEvent = NULL;
    std::thread g_eventQueryThread;
    std::atomic<bool> g_eventQueryRunning(false);
    int g_activeTab = 0;
    int g_playerScroll = 0;
    int g_eventScroll = 0;
    std::vector<CharacterDashboard::PlayerStatus> g_rows;
    std::vector<CharacterDashboard::EventStatus> g_events;
    DWORD g_eventSnapshotTick = 0;
    bool g_isMaximized = false;
    RECT g_restoreBounds = { 0 };
    int g_drawWidth = kWindowWidth;
    int g_drawHeight = kWindowHeight;

    COLORREF Rgb(int red, int green, int blue) { return RGB(red, green, blue); }

#pragma pack(push, 1)
    struct DirectEventWire
    {
        LONG index;
        LONG secondsUntilStart;
    };
#pragma pack(pop)

    void BuildIniPath()
    {
        if (GetModuleFileNameA(NULL, g_iniPath, MAX_PATH) == 0)
        {
            return;
        }
        char* slash = strrchr(g_iniPath, '\\');
        if (slash != NULL)
        {
            strcpy_s(slash + 1, MAX_PATH - static_cast<size_t>(slash + 1 - g_iniPath), "Dashboard.ini");
        }
    }

    void SavePersistedRows()
    {
        if (g_iniPath[0] == '\0')
        {
            return;
        }

        char text[32] = { 0 };
        sprintf_s(text, "%u", static_cast<unsigned>(g_rows.size()));
        WritePrivateProfileStringA("Dashboard", "CharacterCount", text, g_iniPath);

        for (DWORD i = 0; i < CharacterDashboard::kMaxPlayers; ++i)
        {
            char section[32] = { 0 };
            sprintf_s(section, "Character%lu", i);
            WritePrivateProfileStringA(section, NULL, NULL, g_iniPath);
        }

        for (size_t i = 0; i < g_rows.size(); ++i)
        {
            char section[32] = { 0 };
            char value[32] = { 0 };
            const CharacterDashboard::PlayerStatus& row = g_rows[i];
            sprintf_s(section, "Character%u", static_cast<unsigned>(i));
            WritePrivateProfileStringA(section, "Name", row.name, g_iniPath);
            WritePrivateProfileStringA(section, "Map", row.mapName, g_iniPath);

            sprintf_s(value, "%lu", row.level); WritePrivateProfileStringA(section, "Level", value, g_iniPath);
            sprintf_s(value, "%lu", row.reset); WritePrivateProfileStringA(section, "Reset", value, g_iniPath);
            sprintf_s(value, "%lu", row.masterReset); WritePrivateProfileStringA(section, "MasterReset", value, g_iniPath);
            sprintf_s(value, "%lu", row.currentHp); WritePrivateProfileStringA(section, "CurrentHp", value, g_iniPath);
            sprintf_s(value, "%lu", row.maximumHp); WritePrivateProfileStringA(section, "MaximumHp", value, g_iniPath);
            sprintf_s(value, "%lu", row.currentSd); WritePrivateProfileStringA(section, "CurrentSd", value, g_iniPath);
            sprintf_s(value, "%lu", row.maximumSd); WritePrivateProfileStringA(section, "MaximumSd", value, g_iniPath);
            sprintf_s(value, "%ld", row.positionX); WritePrivateProfileStringA(section, "PositionX", value, g_iniPath);
            sprintf_s(value, "%ld", row.positionY); WritePrivateProfileStringA(section, "PositionY", value, g_iniPath);
            sprintf_s(value, "%lu", row.flags); WritePrivateProfileStringA(section, "State", value, g_iniPath);
        }
    }

    void LoadPersistedRows()
    {
        if (g_registry == NULL || g_iniPath[0] == '\0')
        {
            return;
        }

        const DWORD count = std::min<DWORD>(GetPrivateProfileIntA("Dashboard", "CharacterCount", 0, g_iniPath),
            CharacterDashboard::kMaxPlayers);
        for (DWORD i = 0; i < count; ++i)
        {
            char section[32] = { 0 };
            sprintf_s(section, "Character%lu", i);
            CharacterDashboard::PlayerStatus& row = g_registry->players[i];
            ZeroMemory(&row, sizeof(row));
            GetPrivateProfileStringA(section, "Name", "", row.name, sizeof(row.name), g_iniPath);
            if (row.name[0] == '\0')
            {
                continue;
            }
            GetPrivateProfileStringA(section, "Map", "", row.mapName, sizeof(row.mapName), g_iniPath);
            row.processId = 1; // A saved snapshot is not tied to a running process.
            row.lastUpdateTick = GetTickCount();
            row.flags = GetPrivateProfileIntA(section, "State", CharacterDashboard::PlayerOffline, g_iniPath);
            row.level = GetPrivateProfileIntA(section, "Level", 0, g_iniPath);
            row.reset = GetPrivateProfileIntA(section, "Reset", 0, g_iniPath);
            row.masterReset = GetPrivateProfileIntA(section, "MasterReset", 0, g_iniPath);
            row.currentHp = GetPrivateProfileIntA(section, "CurrentHp", 0, g_iniPath);
            row.maximumHp = GetPrivateProfileIntA(section, "MaximumHp", 0, g_iniPath);
            row.currentSd = GetPrivateProfileIntA(section, "CurrentSd", 0, g_iniPath);
            row.maximumSd = GetPrivateProfileIntA(section, "MaximumSd", 0, g_iniPath);
            row.positionX = static_cast<LONG>(GetPrivateProfileIntA(section, "PositionX", 0, g_iniPath));
            row.positionY = static_cast<LONG>(GetPrivateProfileIntA(section, "PositionY", 0, g_iniPath));
        }
    }

    void ClearPersistedRows()
    {
        if (g_registry != NULL)
        {
            HANDLE mutex = CreateMutexA(NULL, FALSE, CharacterDashboard::kMutexName);
            if (mutex != NULL)
            {
                WaitForSingleObject(mutex, 50);
            }
            ZeroMemory(g_registry->players, sizeof(g_registry->players));
            if (mutex != NULL)
            {
                ReleaseMutex(mutex);
                CloseHandle(mutex);
            }
        }
        if (g_iniPath[0] != '\0')
        {
            DeleteFileA(g_iniPath);
        }
    }

    void EnsureEventServerSettings()
    {
        if (g_iniPath[0] == '\0')
        {
            return;
        }

        char address[64] = { 0 };
        GetPrivateProfileStringA("EventServer", "Address", "", address, sizeof(address), g_iniPath);
        if (address[0] == '\0')
        {
            WritePrivateProfileStringA("EventServer", "Address", "127.0.0.1", g_iniPath);
        }
        if (GetPrivateProfileIntA("EventServer", "Port", 0, g_iniPath) == 0)
        {
            WritePrivateProfileStringA("EventServer", "Port", "55901", g_iniPath);
        }
    }

    bool WaitForSocket(SOCKET socket, bool read, long timeoutMilliseconds)
    {
        fd_set sockets;
        FD_ZERO(&sockets);
        FD_SET(socket, &sockets);
        timeval timeout = { timeoutMilliseconds / 1000, (timeoutMilliseconds % 1000) * 1000 };
        return select(0, read ? &sockets : NULL, read ? NULL : &sockets, NULL, &timeout) > 0;
    }

    bool SendExact(SOCKET socket, const BYTE* data, int length)
    {
        int offset = 0;
        while (offset < length)
        {
            const int result = send(socket, reinterpret_cast<const char*>(data + offset), length - offset, 0);
            if (result > 0)
            {
                offset += result;
                continue;
            }
            if (result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK && WaitForSocket(socket, false, 800))
            {
                continue;
            }
            return false;
        }
        return true;
    }

    bool ReceiveExact(SOCKET socket, BYTE* data, int length)
    {
        int offset = 0;
        while (offset < length)
        {
            const int result = recv(socket, reinterpret_cast<char*>(data + offset), length - offset, 0);
            if (result > 0)
            {
                offset += result;
                continue;
            }
            if (result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK && WaitForSocket(socket, true, 800))
            {
                continue;
            }
            return false;
        }
        return true;
    }

    void BuildDirectEventName(LONG index, CharacterDashboard::EventStatus& event)
    {
        switch (index)
        {
        case 0: strcpy_s(event.name, "Blood Castle"); strcpy_s(event.mapName, "Blood Castle"); break;
        case 1: strcpy_s(event.name, "Devil Square"); strcpy_s(event.mapName, "Devil Square"); break;
        case 2: strcpy_s(event.name, "Chaos Castle"); strcpy_s(event.mapName, "Chaos Castle"); break;
        case 3: strcpy_s(event.name, "Castle Event"); strcpy_s(event.mapName, "Castle Siege"); break;
        default:
            sprintf_s(event.name, "Invasion %ld", index - 3);
            strcpy_s(event.mapName, "Server");
            break;
        }
    }

    bool QueryEventsFromGameServer(std::vector<CharacterDashboard::EventStatus>& events)
    {
        char address[64] = { 0 };
        GetPrivateProfileStringA("EventServer", "Address", "127.0.0.1", address, sizeof(address), g_iniPath);
        const unsigned short port = static_cast<unsigned short>(GetPrivateProfileIntA("EventServer", "Port", 55901, g_iniPath));

        addrinfo hints = { 0 };
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        char portText[8] = { 0 };
        sprintf_s(portText, "%u", static_cast<unsigned>(port));
        addrinfo* result = NULL;
        if (getaddrinfo(address, portText, &hints, &result) != 0 || result == NULL)
        {
            return false;
        }

        SOCKET socket = INVALID_SOCKET;
        for (addrinfo* candidate = result; candidate != NULL; candidate = candidate->ai_next)
        {
            socket = ::socket(candidate->ai_family, candidate->ai_socktype, candidate->ai_protocol);
            if (socket == INVALID_SOCKET)
            {
                continue;
            }
            u_long nonBlocking = 1;
            ioctlsocket(socket, FIONBIO, &nonBlocking);
            const int connectResult = connect(socket, candidate->ai_addr, static_cast<int>(candidate->ai_addrlen));
            if (connectResult == 0 || (WSAGetLastError() == WSAEWOULDBLOCK && WaitForSocket(socket, false, 800)))
            {
                int connectError = 0;
                int connectErrorSize = sizeof(connectError);
                getsockopt(socket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&connectError), &connectErrorSize);
                if (connectError == 0)
                {
                    break;
                }
            }
            closesocket(socket);
            socket = INVALID_SOCKET;
        }
        freeaddrinfo(result);
        if (socket == INVALID_SOCKET)
        {
            return false;
        }

        const BYTE request[] = { 0xC1, 0x04, 0xF3, 0xE8 };
        if (!SendExact(socket, request, sizeof(request)))
        {
            closesocket(socket);
            return false;
        }

        // A raw GameServer connection first emits its normal F1 welcome
        // packet.  It is unrelated to the Event request, so skip it (and any
        // other control packet) until the F3:E8 response arrives.
        for (int packetNumber = 0; packetNumber < 4; ++packetNumber)
        {
            BYTE header[3] = { 0 };
            if (!ReceiveExact(socket, header, sizeof(header)))
            {
                break;
            }
            const bool shortPacket = header[0] == 0xC1 || header[0] == 0xC3;
            const bool longPacket = header[0] == 0xC2 || header[0] == 0xC4;
            const int packetSize = shortPacket ? header[1] :
                longPacket ? ((static_cast<int>(header[1]) << 8) | header[2]) : 0;
            if (packetSize < (shortPacket ? 3 : 5) || packetSize > 4096)
            {
                break;
            }
            std::vector<BYTE> packet(packetSize);
            memcpy(packet.data(), header, sizeof(header));
            if (!ReceiveExact(socket, packet.data() + sizeof(header), packetSize - static_cast<int>(sizeof(header))))
            {
                break;
            }
            if (!longPacket || packet[3] != 0xF3 || packet[4] != 0xE8)
            {
                continue;
            }

            LONG count = 0;
            memcpy(&count, packet.data() + 5, sizeof(count));
            count = std::max<LONG>(0, std::min<LONG>(count, CharacterDashboard::kMaxEvents));
            if (packetSize < 10 + count * static_cast<LONG>(sizeof(DirectEventWire)))
            {
                break;
            }
            events.clear();
            for (LONG i = 0; i < count; ++i)
            {
                DirectEventWire wire = { 0 };
                memcpy(&wire, packet.data() + 10 + i * sizeof(DirectEventWire), sizeof(wire));
                if (wire.secondsUntilStart < 0)
                {
                    continue;
                }
                CharacterDashboard::EventStatus event = { 0 };
                BuildDirectEventName(wire.index, event);
                event.secondsUntilStart = wire.secondsUntilStart;
                events.push_back(event);
            }
            closesocket(socket);
            return true;
        }
        closesocket(socket);
        return false;
    }

    void PublishDirectEvents(const std::vector<CharacterDashboard::EventStatus>& events)
    {
        if (g_eventRegistry == NULL)
        {
            return;
        }
        HANDLE mutex = CreateMutexA(NULL, FALSE, CharacterDashboard::kMutexName);
        if (mutex != NULL)
        {
            WaitForSingleObject(mutex, 50);
        }
        ZeroMemory(g_eventRegistry->events, sizeof(g_eventRegistry->events));
        const DWORD count = std::min<DWORD>(static_cast<DWORD>(events.size()), CharacterDashboard::kMaxEvents);
        for (DWORD i = 0; i < count; ++i)
        {
            g_eventRegistry->events[i] = events[i];
        }
        g_eventRegistry->eventCount = count;
        g_eventRegistry->lastUpdateTick = GetTickCount();
        if (mutex != NULL)
        {
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }
    }

    void EventQueryLoop()
    {
        WSADATA winsock = { 0 };
        if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0)
        {
            return;
        }
        while (WaitForSingleObject(g_eventStopEvent, 0) == WAIT_TIMEOUT)
        {
            std::vector<CharacterDashboard::EventStatus> events;
            if (QueryEventsFromGameServer(events))
            {
                PublishDirectEvents(events);
            }
            WaitForSingleObject(g_eventStopEvent, 5000);
        }
        WSACleanup();
    }

    void StartEventServerMonitor()
    {
        if (g_eventRegistry == NULL || g_eventQueryRunning.exchange(true))
        {
            return;
        }
        EnsureEventServerSettings();
        g_eventStopEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
        if (g_eventStopEvent == NULL)
        {
            g_eventQueryRunning = false;
            return;
        }
        g_eventQueryThread = std::thread(EventQueryLoop);
    }

    void StopEventServerMonitor()
    {
        if (!g_eventQueryRunning.exchange(false))
        {
            return;
        }
        SetEvent(g_eventStopEvent);
        if (g_eventQueryThread.joinable())
        {
            g_eventQueryThread.join();
        }
        CloseHandle(g_eventStopEvent);
        g_eventStopEvent = NULL;
    }

    void LoadGameLogo()
    {
        char mainPath[MAX_PATH] = { 0 };
        if (GetModuleFileNameA(NULL, mainPath, MAX_PATH) == 0)
        {
            return;
        }

        char* slash = strrchr(mainPath, '\\');
        if (slash == NULL)
        {
            return;
        }
        strcpy_s(slash + 1, MAX_PATH - static_cast<size_t>(slash + 1 - mainPath), "Main.exe");

        // Main.exe already embeds the current game icon as resource 101.  Load
        // that resource directly so the dashboard always follows the game logo.
        g_mainResourceModule = LoadLibraryExA(mainPath, NULL,
            LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
        if (g_mainResourceModule != NULL)
        {
            HICON sourceLogo = static_cast<HICON>(LoadImageA(g_mainResourceModule, MAKEINTRESOURCEA(101),
                IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
            if (sourceLogo != NULL)
            {
                // Copy the icon before releasing Main.exe, otherwise the
                // running dashboard would lock the game executable during a build.
                g_gameLogo = CopyIcon(sourceLogo);
                DestroyIcon(sourceLogo);
            }
            FreeLibrary(g_mainResourceModule);
            g_mainResourceModule = NULL;
        }
    }

    class PaintBrush
    {
    public:
        explicit PaintBrush(COLORREF colour) : brush(CreateSolidBrush(colour)) {}
        ~PaintBrush() { DeleteObject(brush); }
        HBRUSH brush;
    };

    class PaintFont
    {
    public:
        PaintFont(int height, int weight, const wchar_t* face) : font(CreateFontW(height, 0, 0, 0, weight, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, face)) {}
        ~PaintFont() { DeleteObject(font); }
        HFONT font;
    };

    void Fill(HDC dc, const RECT& rect, COLORREF colour)
    {
        PaintBrush brush(colour);
        FillRect(dc, &rect, brush.brush);
    }

    void Text(HDC dc, int x, int y, const char* value, COLORREF colour, HFONT font, UINT format = 0)
    {
        if (value == NULL)
        {
            return;
        }
        // Event data is stored as UTF-8 by the game.  Drawing it through the
        // ANSI API corrupts Vietnamese characters ("ThÃ©..."), so decode UTF-8
        // first and fall back to the local code page only for legacy data.
        int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value, -1, NULL, 0);
        UINT codePage = CP_UTF8;
        if (length == 0)
        {
            codePage = CP_ACP;
            length = MultiByteToWideChar(codePage, 0, value, -1, NULL, 0);
        }
        if (length == 0)
        {
            return;
        }
        std::vector<wchar_t> wideText(static_cast<size_t>(length));
        MultiByteToWideChar(codePage, codePage == CP_UTF8 ? MB_ERR_INVALID_CHARS : 0,
            value, -1, wideText.data(), length);
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, colour);
        HGDIOBJ old = SelectObject(dc, font);
        RECT rect = { x, y, g_drawWidth - 18, y + 24 };
        DrawTextW(dc, wideText.data(), -1, &rect, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | format);
        SelectObject(dc, old);
    }

    bool IsOnline(const CharacterDashboard::PlayerStatus& status, DWORD now)
    {
        return status.flags == CharacterDashboard::PlayerOnline &&
            static_cast<DWORD>(now - status.lastUpdateTick) < 3000;
    }

    bool HasSameVisualState(const CharacterDashboard::PlayerStatus& left,
        const CharacterDashboard::PlayerStatus& right, DWORD now)
    {
        return left.processId == right.processId &&
            left.flags == right.flags &&
            IsOnline(left, now) == IsOnline(right, now) &&
            memcmp(left.name, right.name, sizeof(left.name)) == 0 &&
            memcmp(left.mapName, right.mapName, sizeof(left.mapName)) == 0 &&
            left.level == right.level &&
            left.reset == right.reset &&
            left.masterReset == right.masterReset &&
            left.currentHp == right.currentHp &&
            left.maximumHp == right.maximumHp &&
            left.currentSd == right.currentSd &&
            left.maximumSd == right.maximumSd &&
            left.positionX == right.positionX &&
            left.positionY == right.positionY;
    }

    bool RefreshRows()
    {
        std::vector<CharacterDashboard::PlayerStatus> nextRows;
        if (g_registry == NULL || g_registry->magic != CharacterDashboard::kMagic ||
            g_registry->version != CharacterDashboard::kVersion)
        {
            const bool changed = !g_rows.empty();
            g_rows.clear();
            return changed;
        }

        HANDLE mutex = CreateMutexA(NULL, FALSE, CharacterDashboard::kMutexName);
        if (mutex != NULL)
        {
            WaitForSingleObject(mutex, 50);
        }
        for (DWORD i = 0; i < CharacterDashboard::kMaxPlayers; ++i)
        {
            const CharacterDashboard::PlayerStatus& row = g_registry->players[i];
            if (row.processId != 0 && row.name[0] != '\0')
            {
                // Older running clients may still leave an entry behind after
                // a restart.  Never render or persist a second copy of the
                // same character; the newest/online entry wins.
                std::vector<CharacterDashboard::PlayerStatus>::iterator existing = std::find_if(nextRows.begin(), nextRows.end(),
                    [&row](const CharacterDashboard::PlayerStatus& candidate)
                    {
                        return lstrcmpiA(candidate.name, row.name) == 0;
                    });
                if (existing == nextRows.end())
                {
                    nextRows.push_back(row);
                }
                else if ((!IsOnline(*existing, GetTickCount()) && IsOnline(row, GetTickCount())) ||
                    row.lastUpdateTick > existing->lastUpdateTick)
                {
                    *existing = row;
                }
            }
        }
        if (mutex != NULL)
        {
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }

        const DWORD now = GetTickCount();
        std::sort(nextRows.begin(), nextRows.end(), [now](const CharacterDashboard::PlayerStatus& left,
            const CharacterDashboard::PlayerStatus& right)
        {
            return IsOnline(left, now) != IsOnline(right, now) ? IsOnline(left, now) : left.name < right.name;
        });
        bool changed = g_rows.size() != nextRows.size();
        if (!changed)
        {
            for (size_t i = 0; i < g_rows.size(); ++i)
            {
                if (!HasSameVisualState(g_rows[i], nextRows[i], now))
                {
                    changed = true;
                    break;
                }
            }
        }
        // Keep heartbeat timestamps current even when there is nothing to repaint.
        g_rows.swap(nextRows);

        if (changed)
        {
            SavePersistedRows();
        }

        const int contentHeight = static_cast<int>(g_rows.size()) * kRowHeight;
        const int viewportHeight = kWindowHeight - kTitleHeight - kTabHeight - 24;
        g_playerScroll = std::max(0, std::min(g_playerScroll, contentHeight - viewportHeight));
        return changed;
    }

    LONG SecondsUntilEvent(const CharacterDashboard::EventStatus& event, DWORD now)
    {
        const DWORD elapsed = static_cast<DWORD>(now - g_eventSnapshotTick) / 1000;
        return event.secondsUntilStart <= static_cast<LONG>(elapsed) ? 0 : event.secondsUntilStart - elapsed;
    }

    bool RefreshEvents()
    {
        std::vector<CharacterDashboard::EventStatus> nextEvents;
        DWORD nextTick = 0;
        if (g_eventRegistry == NULL || g_eventRegistry->magic != CharacterDashboard::kEventMagic ||
            g_eventRegistry->version != CharacterDashboard::kVersion)
        {
            const bool changed = !g_events.empty();
            g_events.clear();
            return changed;
        }

        HANDLE mutex = CreateMutexA(NULL, FALSE, CharacterDashboard::kMutexName);
        if (mutex != NULL)
        {
            WaitForSingleObject(mutex, 50);
        }
        const DWORD count = std::min<DWORD>(g_eventRegistry->eventCount, CharacterDashboard::kMaxEvents);
        nextTick = g_eventRegistry->lastUpdateTick;
        for (DWORD i = 0; i < count; ++i)
        {
            nextEvents.push_back(g_eventRegistry->events[i]);
        }
        if (mutex != NULL)
        {
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }

        const DWORD now = GetTickCount();
        std::sort(nextEvents.begin(), nextEvents.end(), [now, nextTick](const CharacterDashboard::EventStatus& left,
            const CharacterDashboard::EventStatus& right)
        {
            const DWORD elapsed = static_cast<DWORD>(now - nextTick) / 1000;
            const LONG leftTime = left.secondsUntilStart <= static_cast<LONG>(elapsed) ? 0 : left.secondsUntilStart - elapsed;
            const LONG rightTime = right.secondsUntilStart <= static_cast<LONG>(elapsed) ? 0 : right.secondsUntilStart - elapsed;
            return leftTime != rightTime ? leftTime < rightTime : strcmp(left.name, right.name) < 0;
        });

        bool changed = g_events.size() != nextEvents.size() || g_eventSnapshotTick != nextTick;
        if (!changed && !g_events.empty())
        {
            changed = memcmp(g_events.data(), nextEvents.data(), g_events.size() * sizeof(CharacterDashboard::EventStatus)) != 0;
        }
        g_events.swap(nextEvents);
        g_eventSnapshotTick = nextTick;
        const int contentHeight = static_cast<int>(g_events.size()) * 37;
        const int viewportHeight = kWindowHeight - kTitleHeight - kTabHeight - 36;
        g_eventScroll = std::max(0, std::min(g_eventScroll, contentHeight - viewportHeight));
        return changed;
    }

    void DrawGauge(HDC dc, int x, int y, int width, int height, DWORD value, DWORD maximum, bool online, COLORREF colour)
    {
        RECT track = { x, y, x + width, y + height };
        Fill(dc, track, online ? Rgb(32, 35, 45) : Rgb(64, 65, 70));
        FrameRect(dc, &track, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
        if (!online || maximum == 0)
        {
            return;
        }
        const int filled = static_cast<int>(std::min<DWORD>(value, maximum) * width / maximum);
        if (filled > 0)
        {
            RECT fill = { x + 1, y + 1, x + filled - 1, y + height - 1 };
            Fill(dc, fill, colour);
        }
    }

    void DrawRow(HDC dc, const CharacterDashboard::PlayerStatus& row, int top, DWORD now, HFONT regular, HFONT bold, HFONT small)
    {
        const bool online = IsOnline(row, now);
        RECT background = { 15, top, g_drawWidth - 15, top + kRowHeight - 4 };
        Fill(dc, background, online ? Rgb(22, 25, 33) : Rgb(29, 30, 34));
        FrameRect(dc, &background, reinterpret_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH)));

        Text(dc, 25, top + 6, row.name, online ? Rgb(238, 241, 248) : Rgb(180, 181, 184), bold);
        Text(dc, 25, top + 24, online ? "ONLINE" : "OFFLINE", online ? Rgb(77, 219, 133) : Rgb(139, 142, 148), small);

        char stats[96] = { 0 };
        sprintf_s(stats, "Lv %lu   RR %lu   MR %lu", row.level, row.reset, row.masterReset);
        Text(dc, 119, top + 7, stats, online ? Rgb(230, 197, 119) : Rgb(146, 146, 148), regular);

        char location[96] = { 0 };
        sprintf_s(location, "%s  %ld, %ld", row.mapName[0] == '\0' ? "Unknown map" : row.mapName,
            row.positionX, row.positionY);
        Text(dc, 119, top + 25, location, online ? Rgb(188, 201, 224) : Rgb(133, 134, 138), small);

        // The two colours communicate HP (red) and SD (blue) without labels.
        DrawGauge(dc, 25, top + 46, 140, 9, row.currentHp, row.maximumHp, online, Rgb(211, 63, 70));
        DrawGauge(dc, 184, top + 46, 140, 9, row.currentSd, row.maximumSd, online, Rgb(65, 144, 231));
    }

    void DrawControl(HDC dc, int left, int right, const char* caption, COLORREF colour, HFONT font)
    {
        RECT rect = { left, 7, right, kTitleHeight - 8 };
        PaintBrush fill(Rgb(45, 14, 16));
        PaintBrush border(Rgb(137, 49, 39));
        FillRect(dc, &rect, fill.brush);
        FrameRect(dc, &rect, border.brush);
        RECT highlight = { left + 1, 8, right - 1, 9 };
        Fill(dc, highlight, Rgb(185, 73, 52));
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, colour);
        HGDIOBJ old = SelectObject(dc, font);
        DrawTextA(dc, caption, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX);
        SelectObject(dc, old);
    }

    void DrawTabIcon(HDC dc, int centerX, int centerY, bool eventTab, COLORREF colour)
    {
        PaintBrush fill(colour);
        HGDIOBJ oldBrush = SelectObject(dc, fill.brush);
        HGDIOBJ oldPen = SelectObject(dc, CreatePen(PS_SOLID, 1, colour));
        if (!eventTab)
        {
            // Three simple silhouettes: the same visual language as the player icon in the reference.
            Ellipse(dc, centerX - 5, centerY - 7, centerX + 1, centerY - 1);
            Ellipse(dc, centerX + 2, centerY - 6, centerX + 8, centerY);
            RoundRect(dc, centerX - 9, centerY, centerX + 5, centerY + 7, 3, 3);
            RoundRect(dc, centerX + 1, centerY + 1, centerX + 10, centerY + 8, 3, 3);
        }
        else
        {
            RoundRect(dc, centerX - 9, centerY - 8, centerX + 9, centerY + 9, 2, 2);
            RECT cutout = { centerX - 7, centerY - 3, centerX + 7, centerY + 7 };
            Fill(dc, cutout, Rgb(31, 24, 24));
            MoveToEx(dc, centerX - 5, centerY - 10, NULL);
            LineTo(dc, centerX - 5, centerY - 5);
            MoveToEx(dc, centerX + 5, centerY - 10, NULL);
            LineTo(dc, centerX + 5, centerY - 5);
            MoveToEx(dc, centerX - 5, centerY + 2, NULL);
            LineTo(dc, centerX - 1, centerY + 6);
            LineTo(dc, centerX + 6, centerY - 2);
        }
        DeleteObject(SelectObject(dc, oldPen));
        SelectObject(dc, oldBrush);
    }

    void DrawTab(HDC dc, int left, int right, bool selected, bool eventTab)
    {
        RECT tab = { left, kTitleHeight + 5, right, kTitleHeight + kTabHeight - 5 };
        Fill(dc, tab, selected ? Rgb(89, 29, 24) : Rgb(22, 24, 30));
        PaintBrush border(selected ? Rgb(205, 140, 55) : Rgb(57, 61, 70));
        FrameRect(dc, &tab, border.brush);
        if (selected)
        {
            RECT topLine = { left + 1, kTitleHeight + 6, right - 1, kTitleHeight + 8 };
            Fill(dc, topLine, Rgb(221, 157, 65));
        }
        DrawTabIcon(dc, (left + right) / 2, kTitleHeight + 19, eventTab,
            selected ? Rgb(246, 217, 158) : Rgb(137, 153, 178));
    }

    void FormatEventTime(LONG seconds, char* output, size_t outputSize)
    {
        if (seconds <= 0)
        {
            strcpy_s(output, outputSize, "NOW");
            return;
        }
        const LONG days = seconds / 86400;
        const LONG hours = (seconds / 3600) % 24;
        const LONG minutes = (seconds / 60) % 60;
        const LONG secs = seconds % 60;
        if (days > 0)
        {
            sprintf_s(output, outputSize, "%ldd %02ld:%02ld", days, hours, minutes);
        }
        else
        {
            sprintf_s(output, outputSize, "%02ld:%02ld:%02ld", hours, minutes, secs);
        }
    }

    void DrawEventList(HDC dc, HFONT regular, HFONT bold, HFONT small)
    {
        const int headerTop = kTitleHeight + kTabHeight + 10;
        Text(dc, 25, headerTop, "EVENT", Rgb(216, 159, 91), small);
        Text(dc, 145, headerTop, "MAP", Rgb(216, 159, 91), small);
        Text(dc, 253, headerTop, "START", Rgb(216, 159, 91), small);
        RECT line = { 19, headerTop + 18, g_drawWidth - 19, headerTop + 19 };
        Fill(dc, line, Rgb(103, 42, 33));

        const DWORD now = GetTickCount();
        const int firstRow = headerTop + 25 - g_eventScroll;
        for (size_t i = 0; i < g_events.size(); ++i)
        {
            const int top = firstRow + static_cast<int>(i) * 37;
            if (top + 34 < kTitleHeight + kTabHeight || top >= g_drawHeight - 18)
            {
                continue;
            }
            const LONG seconds = SecondsUntilEvent(g_events[i], now);
            const bool urgent = seconds > 0 && seconds <= 300;
            RECT background = { 18, top, g_drawWidth - 18, top + 32 };
            Fill(dc, background, urgent ? Rgb(63, 25, 24) : Rgb(22, 25, 32));
            FrameRect(dc, &background, reinterpret_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH)));
            Text(dc, 25, top + 7, g_events[i].name, urgent ? Rgb(255, 196, 152) : Rgb(235, 236, 240), bold);
            Text(dc, 145, top + 8, g_events[i].mapName, Rgb(170, 189, 214), small);
            char timeText[32] = { 0 };
            FormatEventTime(seconds, timeText, sizeof(timeText));
            Text(dc, 253, top + 7, timeText, seconds == 0 ? Rgb(87, 221, 131) :
                urgent ? Rgb(255, 124, 114) : Rgb(231, 204, 132), regular);
        }

        if (g_events.empty())
        {
            Text(dc, 0, 190, "Waiting for the server event schedule...", Rgb(144, 149, 160), regular, DT_CENTER);
        }
    }

    void DrawDashboard(HDC dc)
    {
        RECT client = { 0, 0, g_drawWidth, g_drawHeight };
        Fill(dc, client, Rgb(10, 12, 16));

        PaintFont title(16, FW_BOLD, L"Segoe UI");
        PaintFont bold(13, FW_SEMIBOLD, L"Segoe UI");
        PaintFont regular(12, FW_NORMAL, L"Segoe UI");
        PaintFont small(10, FW_NORMAL, L"Segoe UI");

        // Compact MU-inspired chrome: a dark red metal header, brass pin-lines
        // and a small crest keep the information-dense window readable.
        RECT header = { 0, 0, client.right, kTitleHeight };
        Fill(dc, header, Rgb(48, 12, 14));
        RECT innerHeader = { 3, 3, client.right - 3, kTitleHeight - 4 };
        Fill(dc, innerHeader, Rgb(76, 20, 19));
        RECT topPinLine = { 4, 3, client.right - 4, 4 };
        Fill(dc, topPinLine, Rgb(204, 139, 61));
        RECT lowerPinLine = { 3, kTitleHeight - 5, client.right - 3, kTitleHeight - 3 };
        Fill(dc, lowerPinLine, Rgb(170, 47, 36));

        if (g_gameLogo != NULL)
        {
            DrawIconEx(dc, 12, 6, g_gameLogo, 30, 30, 0, NULL, DI_NORMAL);
        }
        else
        {
            PaintBrush crestFill(Rgb(116, 30, 25));
            HGDIOBJ oldBrush = SelectObject(dc, crestFill.brush);
            HGDIOBJ oldPen = SelectObject(dc, CreatePen(PS_SOLID, 1, Rgb(229, 159, 61)));
            Ellipse(dc, 13, 8, 43, 37);
            DeleteObject(SelectObject(dc, oldPen));
            SelectObject(dc, oldBrush);
            RECT crestCore = { 17, 12, 39, 33 };
            Fill(dc, crestCore, Rgb(79, 18, 17));
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, Rgb(241, 173, 64));
            HGDIOBJ oldFont = SelectObject(dc, title.font);
            DrawTextA(dc, "M", -1, &crestCore, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            SelectObject(dc, oldFont);
        }

        Text(dc, 54, 7, "DASHBOARD", Rgb(252, 236, 196), bold.font);
        Text(dc, 55, 22, "LIVE CHARACTER MONITOR", Rgb(213, 156, 92), small.font);
        RECT liveDot = { client.right - 145, 18, client.right - 140, 23 };
        PaintBrush liveBrush(Rgb(74, 207, 125));
        FillRect(dc, &liveDot, liveBrush.brush);
        Text(dc, client.right - 136, 15, "LIVE", Rgb(141, 220, 163), small.font);
        DrawControl(dc, client.right - 96, client.right - 66, "-", Rgb(211, 205, 197), regular.font);
        DrawControl(dc, client.right - 63, client.right - 33, "[]", Rgb(211, 205, 197), small.font);
        DrawControl(dc, client.right - 30, client.right - 4, "X", Rgb(245, 143, 131), regular.font);

        RECT tabs = { 0, kTitleHeight, client.right, kTitleHeight + kTabHeight };
        Fill(dc, tabs, Rgb(13, 16, 21));
        RECT tabTopLine = { 0, kTitleHeight, client.right, kTitleHeight + 1 };
        Fill(dc, tabTopLine, Rgb(16, 11, 13));
        DrawTab(dc, 15, 164, g_activeTab == 0, false);
        DrawTab(dc, 184, client.right - 15, g_activeTab == 1, true);

        if (g_activeTab == 0)
        {
            const int firstRow = kTitleHeight + kTabHeight + 12 - g_playerScroll;
            const DWORD now = GetTickCount();
            for (size_t i = 0; i < g_rows.size(); ++i)
            {
                const int top = firstRow + static_cast<int>(i) * kRowHeight;
                if (top + kRowHeight >= kTitleHeight + kTabHeight && top < g_drawHeight)
                {
                    DrawRow(dc, g_rows[i], top, now, regular.font, bold.font, small.font);
                }
            }
            if (g_rows.empty())
            {
                Text(dc, 0, 205, "No character has logged in yet", Rgb(144, 149, 160), regular.font, DT_CENTER);
                Text(dc, 0, 231, "Open Main.exe or keep this dashboard running.", Rgb(94, 99, 110), small.font, DT_CENTER);
            }
        }
        else
        {
            DrawEventList(dc, regular.font, bold.font, small.font);
        }
        RECT footer = { 15, g_drawHeight - 15, client.right - 15, g_drawHeight - 13 };
        Fill(dc, footer, Rgb(73, 26, 25));
    }

    void ToggleMaximize(HWND window)
    {
        if (!g_isMaximized)
        {
            GetWindowRect(window, &g_restoreBounds);
            MONITORINFO monitor = { sizeof(monitor) };
            GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST), &monitor);
            SetWindowPos(window, HWND_TOPMOST, monitor.rcWork.left, monitor.rcWork.top,
                monitor.rcWork.right - monitor.rcWork.left, monitor.rcWork.bottom - monitor.rcWork.top, SWP_SHOWWINDOW);
            g_isMaximized = true;
        }
        else
        {
            SetWindowPos(window, HWND_TOPMOST, g_restoreBounds.left, g_restoreBounds.top,
                g_restoreBounds.right - g_restoreBounds.left, g_restoreBounds.bottom - g_restoreBounds.top, SWP_SHOWWINDOW);
            g_isMaximized = false;
        }
    }

    LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
            SetTimer(window, 1, 1000, NULL);
            return 0;
        case WM_TIMER:
        {
            const bool playerChanged = RefreshRows();
            const bool eventChanged = RefreshEvents();
            // The event tab must redraw once per second for its countdown. The
            // off-screen buffer keeps that update completely flicker-free.
            if ((g_activeTab == 0 && playerChanged) || (g_activeTab == 1 && (eventChanged || !g_events.empty())))
            {
                InvalidateRect(window, NULL, FALSE);
            }
            return 0;
        }
        case WM_MOUSEWHEEL:
            if (g_activeTab == 0)
            {
                g_playerScroll -= GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA * 48;
            }
            else
            {
                g_eventScroll -= GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA * 37;
            }
            RefreshRows();
            RefreshEvents();
            InvalidateRect(window, NULL, FALSE);
            return 0;
        case WM_NCHITTEST:
        {
            POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(window, &point);
            RECT client = { 0 };
            GetClientRect(window, &client);
            if (point.y < kTitleHeight && point.x < client.right - 100)
            {
                return HTCAPTION;
            }
            return HTCLIENT;
        }
        case WM_LBUTTONUP:
        {
            const int x = GET_X_LPARAM(lParam);
            const int y = GET_Y_LPARAM(lParam);
            RECT client = { 0 };
            GetClientRect(window, &client);
            if (y < kTitleHeight && x >= client.right - 30)
            {
                DestroyWindow(window);
            }
            else if (y < kTitleHeight && x >= client.right - 63)
            {
                ToggleMaximize(window);
            }
            else if (y < kTitleHeight && x >= client.right - 96)
            {
                ShowWindow(window, SW_MINIMIZE);
            }
            else if (y >= kTitleHeight && y < kTitleHeight + kTabHeight)
            {
                const int nextTab = x < 174 ? 0 : 1;
                if (g_activeTab != nextTab)
                {
                    g_activeTab = nextTab;
                    InvalidateRect(window, NULL, FALSE);
                }
            }
            return 0;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC dc = BeginPaint(window, &paint);
            RECT client = { 0 };
            GetClientRect(window, &client);
            g_drawWidth = client.right;
            g_drawHeight = client.bottom;

            // All drawing happens off-screen first, then one blit presents a
            // complete frame. This eliminates the visible clear/repaint flash.
            HDC backBuffer = CreateCompatibleDC(dc);
            HBITMAP bitmap = CreateCompatibleBitmap(dc, g_drawWidth, g_drawHeight);
            HGDIOBJ oldBitmap = SelectObject(backBuffer, bitmap);
            DrawDashboard(backBuffer);
            BitBlt(dc, 0, 0, g_drawWidth, g_drawHeight, backBuffer, 0, 0, SRCCOPY);
            SelectObject(backBuffer, oldBitmap);
            DeleteObject(bitmap);
            DeleteDC(backBuffer);
            EndPaint(window, &paint);
            return 0;
        }
        case WM_DESTROY:
            KillTimer(window, 1);
            ClearPersistedRows();
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(window, message, wParam, lParam);
    }

    bool ConnectRegistry()
    {
        g_mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
            sizeof(CharacterDashboard::Registry), CharacterDashboard::kMappingName);
        if (g_mapping == NULL)
        {
            return false;
        }
        g_registry = reinterpret_cast<CharacterDashboard::Registry*>(MapViewOfFile(g_mapping,
            FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CharacterDashboard::Registry)));
        if (g_registry == NULL)
        {
            CloseHandle(g_mapping);
            g_mapping = NULL;
            return false;
        }

        g_eventMapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
            sizeof(CharacterDashboard::EventRegistry), CharacterDashboard::kEventMappingName);
        if (g_eventMapping != NULL)
        {
            g_eventRegistry = reinterpret_cast<CharacterDashboard::EventRegistry*>(MapViewOfFile(g_eventMapping,
                FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CharacterDashboard::EventRegistry)));
            if (g_eventRegistry != NULL)
            {
                HANDLE eventMutex = CreateMutexA(NULL, FALSE, CharacterDashboard::kMutexName);
                if (eventMutex != NULL)
                {
                    WaitForSingleObject(eventMutex, 50);
                }
                if (g_eventRegistry->magic != CharacterDashboard::kEventMagic ||
                    g_eventRegistry->version != CharacterDashboard::kVersion)
                {
                    ZeroMemory(g_eventRegistry, sizeof(CharacterDashboard::EventRegistry));
                    g_eventRegistry->magic = CharacterDashboard::kEventMagic;
                    g_eventRegistry->version = CharacterDashboard::kVersion;
                }
                if (eventMutex != NULL)
                {
                    ReleaseMutex(eventMutex);
                    CloseHandle(eventMutex);
                }
            }
        }

        HANDLE mutex = CreateMutexA(NULL, FALSE, CharacterDashboard::kMutexName);
        if (mutex != NULL)
        {
            WaitForSingleObject(mutex, 50);
        }
        bool hasCharacterRows = false;
        if (g_registry->magic != CharacterDashboard::kMagic || g_registry->version != CharacterDashboard::kVersion)
        {
            ZeroMemory(g_registry, sizeof(CharacterDashboard::Registry));
            g_registry->magic = CharacterDashboard::kMagic;
            g_registry->version = CharacterDashboard::kVersion;
        }
        for (DWORD i = 0; i < CharacterDashboard::kMaxPlayers; ++i)
        {
            if (g_registry->players[i].name[0] != '\0')
            {
                hasCharacterRows = true;
                break;
            }
        }
        if (!hasCharacterRows)
        {
            LoadPersistedRows();
        }
        if (mutex != NULL)
        {
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }
        return true;
    }
}

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    g_singleInstance = CreateMutexA(NULL, TRUE, CharacterDashboard::kDashboardMutexName);
    if (g_singleInstance == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
    {
        HWND existing = FindWindowA("ACuoiCharacterDashboard", NULL);
        if (existing != NULL)
        {
            ShowWindow(existing, SW_RESTORE);
            SetForegroundWindow(existing);
        }
        return 0;
    }

    BuildIniPath();

    if (!ConnectRegistry())
    {
        return 1;
    }

    LoadGameLogo();
    StartEventServerMonitor();

    WNDCLASSEXA windowClass = { 0 };
    windowClass.cbSize = sizeof(windowClass);
    windowClass.hInstance = instance;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hIcon = g_gameLogo;
    windowClass.hIconSm = g_gameLogo;
    windowClass.lpszClassName = "ACuoiCharacterDashboard";
    RegisterClassExA(&windowClass);

    HWND window = CreateWindowExA(WS_EX_APPWINDOW, windowClass.lpszClassName, "Dashboard",
        WS_POPUP | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, kWindowWidth, kWindowHeight,
        NULL, NULL, instance, NULL);
    if (window == NULL)
    {
        return 1;
    }
    SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    ShowWindow(window, SW_SHOW);
    RefreshRows();
    // Ensure the settings file exists even before the first character logs in.
    SavePersistedRows();
    RefreshEvents();

    MSG message;
    while (GetMessage(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    StopEventServerMonitor();
    if (g_registry != NULL)
    {
        UnmapViewOfFile(g_registry);
    }
    if (g_mapping != NULL)
    {
        CloseHandle(g_mapping);
    }
    if (g_eventRegistry != NULL)
    {
        UnmapViewOfFile(g_eventRegistry);
    }
    if (g_eventMapping != NULL)
    {
        CloseHandle(g_eventMapping);
    }
    if (g_singleInstance != NULL)
    {
        ReleaseMutex(g_singleInstance);
        CloseHandle(g_singleInstance);
    }
    if (g_gameLogo != NULL)
    {
        DestroyIcon(g_gameLogo);
    }
    return static_cast<int>(message.wParam);
}
