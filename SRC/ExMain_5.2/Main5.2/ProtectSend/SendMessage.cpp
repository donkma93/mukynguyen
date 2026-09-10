#include "stdafx.h"
#include "SendMessage.h"
#include "CCRC32.h"
#include "resource.h"
CSendMessage gSendMessage;

HWND hWndMain = NULL;
HWND hEditBox = NULL;

LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

DWORD WINAPI SendMessageThread(LPVOID)
{
    if (hWndMain != NULL)
    {
        wchar_t szWideMessage[256] = { 0 };
        MultiByteToWideChar(CP_UTF8, 0, gSendMessage.message, -1, szWideMessage, 256);

        int len = GetWindowTextLengthW(hEditBox);
        SendMessageW(hEditBox, EM_SETSEL, len, len);
        SendMessageW(hEditBox, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
        SendMessageW(hEditBox, EM_REPLACESEL, FALSE, (LPARAM)szWideMessage);
        return 0;
    }

    HINSTANCE hInstance = GetModuleHandle(NULL);
    const wchar_t* className = L"CustomMessageWindow";
    int width = 600 * 70 / 100;
    int height = 400 * 70 / 100;

    static bool classRegistered = false;
    if (!classRegistered)
    {
        WNDCLASSW wc = {};
        wc.lpfnWndProc = MessageWndProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = className;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);
        classRegistered = true;
    }

    hWndMain = CreateWindowExW(
        0,
        className,
        L"Server Message",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        0, 0,
        width, height,
        NULL, NULL, hInstance, NULL
    );

    if (!hWndMain) return 1;

    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    SendMessageW(hWndMain, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessageW(hWndMain, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    hEditBox = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        0, 0, width - 40, height - 60,
        hWndMain,
        NULL,
        hInstance,
        NULL
    );

    HFONT hFont = CreateFontW(
        17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Tahoma"
    );
    SendMessageW(hEditBox, WM_SETFONT, (WPARAM)hFont, TRUE);

    wchar_t szWideMessage[256] = { 0 };
    MultiByteToWideChar(CP_UTF8, 0, gSendMessage.message, -1, szWideMessage, 256);
    SendMessageW(hEditBox, WM_SETTEXT, 0, (LPARAM)szWideMessage);

    ShowWindow(hWndMain, SW_SHOW);
    UpdateWindow(hWndMain);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    hWndMain = NULL;
    hEditBox = NULL;
    DeleteObject(hFont);
    return 0;
}

void MemoryCpy(DWORD offset, void* value, DWORD size) // OK
{
	DWORD OldProtect;
	VirtualProtect((void*)offset, size, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy((void*)offset, value, size);
	VirtualProtect((void*)offset, size, OldProtect, &OldProtect);
}
void CSendMessage::RecvSendMessage(PMSG_SEND_MESSAGE_REQ_RECV *lpMsg)
{
    MemoryCpy((DWORD)gSendMessage.message, lpMsg->message, sizeof(gSendMessage.message));
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendMessageThread, 0, 0, NULL);
}
