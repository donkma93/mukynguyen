#include "stdafx.h"
#include "Resource.h"
#include "BloodCastle.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "Crywolf.h"
#include "CustomArena.h"
#include "CustomEventDrop.h"
#include "DevilSquare.h"
#include "EventGvG.h"
#include "EventTvT.h"
#include "GameServer.h"
#include "GameMain.h"
#include "IllusionTemple.h"
#include "InvasionManager.h"
#include "JSProtocol.h"

#include "MiniDump.h"
#include "Notice.h"
#include "QueueTimer.h"
#include "ServerDisplayer.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "SocketManagerModern.h"
#include "SocketManagerUdp.h"
#include "ThemidaSDK.h"
#include "Util.h"
#include "ReiDoMU.h"
#include "BotOnline.h"
#include "Path.h"
#include "FakeOnline.h"
#include "OfflineMode.h"
#include "CustomStore.h"
#include "CommandManager.h"
#include "MapManager.h"
#include "BlackList.h"
#include "HidManager.h"
#include "SendMessage.h"
#include "../../Addon/DualColor.h"
#include "TextStringUTF.h"
#include <strsafe.h>
#include "GetLicenseID.h"
#include "ACuoi_CastleEvent.h"
#include "ACuoi_MessLang.h"
HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	VM_START

	CMiniDump::Start();

	//gFile.StartCheckID(hWnd);
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMESERVER, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if (InitInstance(hInstance, nCmdShow) == 0) {
		return 0;
	}

	SetLargeRand();

	gServerInfo.ReadStartupInfo("GameServerInfo", ".\\Data\\GameServerInfo - Common.ini");

	char buff[256];
	wsprintf(buff, " %s: %d", gServerInfo.m_ServerName, gObjTotalUser);
	SetWindowText(hWnd, buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;
	int startupResult = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (startupResult != 0) {
		LogAdd(LOG_RED, "WSAStartup() failed with error: %d", WSAGetLastError());
		return 0;
	}
#if (NEW_PROTOCOL_SYSTEM == 0)
	if (gSocketManager.Start((WORD)gServerInfo.m_ServerPort) == 0) {
		LogAdd(LOG_RED, "Could not start GameServer");
		return 0;
	}
#else
	if (gSocketManagerModern.StartServer(gServerInfo.m_ServerPort) == 0) {
		LogAdd(LOG_RED, "Could not start, port in use");
		return 0;
	}
#endif
	GameMainInit(hWnd);

	JoinServerConnect(WM_JOIN_SERVER_MSG_PROC);

	DataServerConnect(WM_DATA_SERVER_MSG_PROC);

	gSocketManagerUdp.Connect(gServerInfo.m_ConnectServerAddress, (WORD)gServerInfo.m_ConnectServerPort);

	SetTimer(hWnd, WM_TIMER_1000, 1000, NULL);

	SetTimer(hWnd, WM_TIMER_10000, 10000, NULL);

	gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER, 100, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_MOVE, 100, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI, 100, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI_MOVE, 100, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_EVENT, 100, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_VIEWPORT, 1000, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_FIRST, 1000, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_CLOSE, 1000, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_MATH_AUTHENTICATOR, 10000, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_ACCOUNT_LEVEL, 60000, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_PICK_COMMAND, 6000, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_AUTH_COMMAND, 600000, &QueueTimerCallback);

	gQueueTimer.CreateTimer(QUEUE_TIMER_RANKING, 15000, &QueueTimerCallback);

	#if(TEST_GHRS)
	gQueueTimer.CreateTimer(QUEUE_TIMER_GHRS, 1000, &QueueTimerCallback);
	#endif
	SetTimer(hWnd, WM_TIMER_2000, 2000, NULL);

	HACCEL hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GAMESERVER);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	CMiniDump::Clean();

	VM_END

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMESERVER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_GAMESERVER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMESERVER));

	return RegisterClassEx(&wcex);

	if (!RegisterClassEx(&wcex)) {
		MessageBoxW(NULL, L"Failed to register window class", L"Error", MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
		CW_USEDEFAULT, 0, 1280, 768,
		0, 0, hInstance, 0);

	if (hWnd == NULL)
	{
		return FALSE;
	}

	RECT rcScreen;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0);

	RECT rcWindow;
	GetWindowRect(hWnd, &rcWindow);
	int x = (rcScreen.right - rcScreen.left - (rcWindow.right - rcWindow.left)) / 2;
	int y = (rcScreen.bottom - rcScreen.top - (rcWindow.bottom - rcWindow.top)) / 2;
	SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	const char ClassName[] = "MainWindowClass";

	HWND hWndStatusBar;

	switch(message)
	{
        case WM_CREATE:
        {
			hWndStatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM, 0, 0, 0, 0, hWnd, (HMENU)IDC_STATUSBAR, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

            int iStatusWidths[] = {190,270,360,450,580, -1};

            char text[256];

            SendMessage(hWndStatusBar, SB_SETPARTS, 6, (LPARAM)iStatusWidths);

			wsprintf(text, "Zalo : 0968 092 399 - [Server 5.2  ThangCuoi]");
            SendMessage(hWndStatusBar, SB_SETTEXT, 0,(LPARAM)text);

			wsprintf(text, "OffStore: %d", gObjOffStore);

            SendMessage(hWndStatusBar, SB_SETTEXT, 1,(LPARAM)text);

			wsprintf(text, "OffAttack: %d", gObjOffAttack);

            SendMessage(hWndStatusBar, SB_SETTEXT, 2,(LPARAM)text);

			wsprintf(text, "Bots Buffer: %d", gObjTotalBot);

            SendMessage(hWndStatusBar, SB_SETTEXT, 3,(LPARAM)text);

			wsprintf(text, "Monsters: %d/%d", gObjTotalMonster,MAX_OBJECT_MONSTER);

            SendMessage(hWndStatusBar, SB_SETTEXT, 4,(LPARAM)text);

			SendMessage(hWndStatusBar, SB_SETTEXT, 5,(LPARAM)NULL);

            ShowWindow(hWndStatusBar, SW_SHOW);
		}
		break;

		case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDM_EVENTS_CTCMINI: gCastleStartGuild.StartCTCMini(); break;

			case ID_BOTONLINE_RELOADDATA:
			BotOnline.Read(gPath.GetFullPath("Custom\\BotSystem\\BotOnline.txt"));
			if (gJoinServerConnection.CheckState() != 0 && gDataServerConnection.CheckState() != 0)
			{
				BotOnline.MakeBot();
			}
			break;

			case ID_BOTONLINE_UNLOADDATA: BotOnline.UnloadBot(); break;

			case IDM_ABOUT: DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About); break;

			case IDM_EXIT:
			if (MessageBoxW(0, TXT_EXITGAMESV, L"GameServer", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hWnd);
			}
			break;
			case IDM_FILE_ALLUSERLOGOUT:						gObjAllLogOut();					break;
			case IDM_FILE_ALLUSERDISCONNECT:					gObjAllDisconnect();				break;
			case IDM_FILE_1MINUTESERVERCLOSE:
			if(gCloseMsg == 0)
			{
				gCloseMsg = 1;
				gCloseMsgTime = 60;
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessageNew.GetMessage(487));
			}
			break;
			case IDM_FILE_3MINUTESERVERCLOSE:
			if(gCloseMsg == 0)
			{
				gCloseMsg = 1;
				gCloseMsgTime = 180;
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0, gMessageNew.GetMessage(488));
			}
			break;
			case IDM_FILE_5MINUTESERVERCLOSE:
			if(gCloseMsg == 0)
			{
				gCloseMsg = 1;
				gCloseMsgTime = 300;
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0, gMessageNew.GetMessage(489));
			}
			break;
			case IDM_RELOAD_RELOADCFGSPK:			gServerInfo.ReloadCfgSPK();				break;
			case IDM_RELOAD_RELOADCASHSHOP:			gServerInfo.ReadCashShopInfo();			break;
			case IDM_RELOAD_RELOADCHAOSMIX:			gServerInfo.ReadChaosMixInfo();			break;
			case IDM_RELOAD_RELOADCHARACTER:		gServerInfo.ReadCharacterInfo();		break;
			case IDM_RELOAD_RELOADCOMMAND:			gServerInfo.ReadCommandInfo();			break;
			case IDM_RELOAD_RELOADCOMMON:			gServerInfo.ReadCommonInfo();			break;
			case IDM_RELOAD_RELOADCUSTOM:			gServerInfo.ReadCustomInfo();			break;

			case IDM_RELOAD_RELOADEVENT:
			{
				for (int i = 0; i < MAX_INVASION; ++i)
				{
					memset(gInvasionManager.m_InvasionInfo[i].AlertMessage, 0, sizeof(gInvasionManager.m_InvasionInfo[i].AlertMessage));
				}
				gInvasionManager.Init();
				gServerInfo.ReadEventInfo();
			}
			break;

			case IDM_RELOAD_RELOADEVENTITEMBAG:		gServerInfo.ReadEventItemBagInfo();		break;
			case IDM_RELOAD_RELOADHACK:				gServerInfo.ReadHackInfo();				break;
			case IDM_RELOAD_RELOADITEM:				gServerInfo.ReadItemInfo();				break;
			case IDM_RELOAD_RELOADMONSTER:			gServerInfo.ReloadMonsterInfo();		break;
			case IDM_RELOAD_RELOADMOVE:				gServerInfo.ReadMoveInfo();				break;
			case IDM_RELOAD_RELOADQUEST:			gServerInfo.ReadQuestInfo();			break;
			case IDM_RELOAD_RELOADSHOP:				gServerInfo.ReadShopInfo();				break;
			case IDM_RELOAD_RELOADSKILL:			gServerInfo.ReadSkillInfo();			break;
			case IDM_RELOAD_RELOADUTIL:				gServerInfo.ReadUtilInfo();				break;
			case IDM_RELOAD_RELOADBOTS:				gServerInfo.ReloadBotInfo();			break;
			case IDM_RELOAD_RELOADALL:				gServerInfo.ReloadAll();				break;
			case IDM_STARTBC:						gBloodCastle.StartBC();					break;
			case IDM_STARTDS:						gDevilSquare.StartDS();					break;
			case IDM_STARTCC:						gChaosCastle.StartCC();					break;
			case IDM_STARTIT:						gIllusionTemple.StartIT();				break;
			case IDM_STARTDROP:						gCustomEventDrop.StartDrop();			break;
			case IDM_STARTKING:						gReiDoMU.StartKing();					break;
			case IDM_STARTTVT:						gTvTEvent.StartTvT();					break;
			case IDM_STARTGVG:						gGvGEvent.StartGvG();					break;
			case IDM_INVASION0:						gInvasionManager.StartInvasion(0);		break;
			case IDM_INVASION1:						gInvasionManager.StartInvasion(1);		break;
			case IDM_INVASION2:						gInvasionManager.StartInvasion(2);		break;
			case IDM_INVASION3:						gInvasionManager.StartInvasion(3);		break;
			case IDM_INVASION4:						gInvasionManager.StartInvasion(4);		break;
			case IDM_INVASION5:						gInvasionManager.StartInvasion(5);		break;
			case IDM_INVASION6:						gInvasionManager.StartInvasion(6);		break;
			case IDM_INVASION7:						gInvasionManager.StartInvasion(7);		break;
			case IDM_INVASION8:						gInvasionManager.StartInvasion(8);		break;
			case IDM_INVASION9:						gInvasionManager.StartInvasion(9);		break;
			case IDM_INVASION10:					gInvasionManager.StartInvasion(10);		break;
			case IDM_INVASION11:					gInvasionManager.StartInvasion(11);		break;
			case IDM_INVASION12:					gInvasionManager.StartInvasion(12);		break;
			case IDM_INVASION14:					gInvasionManager.StartInvasion(14);		break;
			case IDM_INVASION15:					gInvasionManager.StartInvasion(15);		break;
			case IDM_INVASION16:					gInvasionManager.StartInvasion(16);		break;
			case IDM_INVASION17:					gInvasionManager.StartInvasion(17);		break;
			case IDM_INVASION18:					gInvasionManager.StartInvasion(18);		break;
			case IDM_INVASION19:					gInvasionManager.StartInvasion(19);		break;
			case IDM_INVASION20:					gInvasionManager.StartInvasion(20);		break;
			case IDM_INVASION21:					gInvasionManager.StartInvasion(21);		break;
			case IDM_INVASION22:					gInvasionManager.StartInvasion(22);		break;
			case IDM_INVASION23:					gInvasionManager.StartInvasion(23);		break;
			case IDM_INVASION24:					gInvasionManager.StartInvasion(24);		break;
			case IDM_INVASION25:					gInvasionManager.StartInvasion(25);		break;
					
			case IDM_CA0:							gCustomArena.StartCustomArena(0);		break;
			case IDM_CA1:							gCustomArena.StartCustomArena(1);		break;
			case IDM_CA2:							gCustomArena.StartCustomArena(2);		break;
			case IDM_CA3:							gCustomArena.StartCustomArena(3);		break;
			case IDM_CA4:							gCustomArena.StartCustomArena(4);		break;
			case IDM_CA5:							gCustomArena.StartCustomArena(5);		break;
			case IDM_CA6:							gCustomArena.StartCustomArena(6);		break;
			case IDM_CA7:							gCustomArena.StartCustomArena(7);		break;
			case IDM_CA8:							gCustomArena.StartCustomArena(8);		break;
			case IDM_CA9:							gCustomArena.StartCustomArena(9);		break;
			case IDM_CA10:							gCustomArena.StartCustomArena(10);		break;
			case IDM_CA11:							gCustomArena.StartCustomArena(11);		break;
			case IDM_CA12:							gCustomArena.StartCustomArena(12);		break;
			case IDM_CA13:							gCustomArena.StartCustomArena(13);		break;

			#if(GAMESERVER_TYPE==1)
			case IDM_STARTCS:						gCastleSiege.StartCS();					break;
			#endif

			case IDM_STARTCW:						gCrywolf.StartCW();						break;
			case IDM_STARTLD:						gCastleDeep.StartLD();					break;
			case ID_FAKEONLINE_RELOADDATA:
			{
				for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
				{
					if (gObj[n].IsFakeOnline == 1)
					{
						gObj[n].IsFakeOnline = 0;
						gObjDel(n);
					}

				}
				s_FakeOnline.LoadFakeData(".\\BotFakeOnline.xml");
			}
			break;
			case ID_FAKEONLINE_ADDFAKEONLINE:
			{
				if (gJoinServerConnection.CheckState() != 0 && gDataServerConnection.CheckState() != 0)
				{
					s_FakeOnline.RestoreFakeOnline();
				}
			}
			break;
			case ID_FAKEONLINE_DELFAKEONLINE:
			{
				for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
				{
					if (gObjIsConnectedGP(n) != 0 && gObj[n].IsFakeOnline)
					{
						s_FakeOnline.OnAttackAlreadyConnected(&gObj[n]);
					}
				}
			}
			break;

			case IDM_USERS:	DialogBox(hInst, (LPCTSTR)IDD_USERS, hWnd, (DLGPROC)Users);	break;

			default:
			return DefWindowProc(hWnd,message,wParam,lParam);
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT hPaintStruct;
			HDC hdc = BeginPaint(hWnd,&hPaintStruct);
			HDC hMemDC = CreateCompatibleDC(hdc);
			HBITMAP OldBmp = (HBITMAP)SelectObject(hMemDC,LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1)));
			BitBlt(hdc,0,0,1920,480,hMemDC,0,0,SRCCOPY);
			SelectObject(hMemDC,OldBmp);
			DeleteDC(hMemDC);
			EndPaint(hWnd,&hPaintStruct);
		}
		break;
		case WM_CLOSE:
		if (gObjTotalUser != 0)
		{
			wchar_t szBuffer[512];
			StringCchPrintfW(szBuffer, 512, TXT_EXITGAMESV, gObjTotalUser);

			if (MessageBoxW(0, szBuffer, L"GameServer", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hWnd);
			}
		}
		else
		{
			DestroyWindow(hWnd);
		}
		break;
		case WM_TIMER:
		switch(wParam)
		{
			case WM_TIMER_1000:
				GJServerUserInfoSend();
				ConnectServerInfoSend();
				break;
			case WM_TIMER_2000:
				gObjCountProc();
				gServerDisplayer.Run();
				break;
			case WM_TIMER_10000:
				JoinServerReconnect(hWnd,WM_JOIN_SERVER_MSG_PROC);
				DataServerReconnect(hWnd,WM_DATA_SERVER_MSG_PROC);
				break;
		}
		break;
		case WM_JOIN_SERVER_MSG_PROC:			JoinServerMsgProc(wParam,lParam);			break;
		case WM_DATA_SERVER_MSG_PROC:			DataServerMsgProc(wParam,lParam);			break;
		case WM_DESTROY:						PostQuitMessage(0);							break;
		default:
		return DefWindowProc(hWnd,message,wParam,lParam);
	}
	return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrushStatic = NULL;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL)));
		SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL)));

		HWND hParent = GetParent(hDlg);
		if (!hParent)
			hParent = GetDesktopWindow();

		RECT rcParent, rcDlg;
		GetWindowRect(hParent, &rcParent);
		GetWindowRect(hDlg, &rcDlg);

		int dlgWidth = rcDlg.right - rcDlg.left;
		int dlgHeight = rcDlg.bottom - rcDlg.top;

		int posX = rcParent.left + ((rcParent.right - rcParent.left) - dlgWidth) / 2;
		int posY = rcParent.top + ((rcParent.bottom - rcParent.top) - dlgHeight) / 2;

		SetWindowPos(hDlg, HWND_TOP, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		SetDlgItemTextW(hDlg, IDC_VERSION, TXT_ABOUTLASTTEXT);
		SetDlgItemText(hDlg, IDC_MAINKEY, gFile.ComputerHardwareId[0]);

		hBrushStatic = CreateSolidBrush(RGB(255, 255, 255));

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		HWND hCtrl = (HWND)lParam;

		if (GetDlgCtrlID(hCtrl) == IDC_MAINKEY)
		{
			SetTextColor(hdcStatic, RGB(13, 145, 20));
			SetBkMode(hdcStatic, TRANSPARENT);
			return (INT_PTR)GetStockObject(NULL_BRUSH);
		}
	}
	break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (hBrushStatic)
			{
				DeleteObject(hBrushStatic);
				hBrushStatic = NULL;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}

	return FALSE;
}

int CheckIpAddress(HWND hDlg, char* text)
{
	HWND hWndIP = GetDlgItem(hDlg, IDC_USERIPLIST);

	int count = SendMessage(hWndIP, LB_GETCOUNT, 0, 0);

	for (int n = 0; n < count; n++)
	{
		int aIndex = (int)SendMessage(hWndIP, LB_GETITEMDATA, (LPARAM)n, 0);

		if (strcmp(gObj[aIndex].IpAddr, text) == 0)
		{
			return 1;
		}
	}

	return 0;
}

const char* GetAccess(int verify)
{
	switch (verify)
	{
	case 0: return "Unknown";
	case 1: return "OK";
	default:
		static char temp[32];
		wsprintf(temp, "%d", verify);
		return temp;
	}
}

LRESULT CALLBACK SendMessageToClient(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			SetWindowText(hDlg, "Send Message");

			HWND hParent = GetParent(hDlg);
			if (!hParent) hParent = GetForegroundWindow(); 

			RECT rcParent, rcDialog;
			GetWindowRect(hParent, &rcParent);
			GetWindowRect(hDlg, &rcDialog);

			int dialogWidth = rcDialog.right - rcDialog.left;
			int dialogHeight = rcDialog.bottom - rcDialog.top;

			int x = rcParent.left + ((rcParent.right - rcParent.left) - dialogWidth) / 2;
			int y = rcParent.top + ((rcParent.bottom - rcParent.top) - dialogHeight) / 2;

			SetWindowPos(hDlg, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

			return TRUE;
		}
		case WM_CLOSE:
		{
			ShowWindow(hDlg, SW_HIDE);
			break;
		}
		case WM_PAINT:
		{
			char WindowName[128];
			if (OBJMAX_RANGE(gSendMessage.currentIndex))
			{
				sprintf(WindowName, "SendMessage: [IP: %s]", gObj[gSendMessage.currentIndex].IpAddr);
			}
			else
			{
				sprintf(WindowName, "SendMessage: [IP: Unknown]");
			}
			SetWindowText(hDlg, WindowName);
			break;
		}
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);
			switch (wmId)
			{
				case IDC_SEND:
				{
					HWND MessageHWND = GetDlgItem(hDlg, IDC_MESSAGE);
					WCHAR wTemp[256] = { 0 };
					GetWindowTextW(MessageHWND, wTemp, 255);
					WideCharToMultiByte(CP_UTF8, 0, wTemp, -1, gSendMessage.message, 255, 0, 0);
					gSendMessage.SendProcessRequest();
					break;
				}
			}
			break;
		}
	}
	return 0;
}
LRESULT CALLBACK Users(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWndVery			= GetDlgItem(hDlg, IDC_CHARVERY);
	HWND hWndAcc			= GetDlgItem(hDlg, IDC_ACCNAME);
	HWND hWndChar			= GetDlgItem(hDlg, IDC_CHARNAME);
	HWND hWndOSName			= GetDlgItem(hDlg, IDC_OS_NAME);
	HWND hWndOSVersion		= GetDlgItem(hDlg, IDC_OS_VERS);
	HWND hWndInstallDate	= GetDlgItem(hDlg, IDC_OS_DATE);
	HWND hWndSystemArch		= GetDlgItem(hDlg, IDC_OS_TYPE);
	HWND hWndTotalRAM		= GetDlgItem(hDlg, IDC_OS_RAMB);
	HWND hWndGPUName		= GetDlgItem(hDlg, IDC_OS_GPUB);
	HWND hWndIPChar			= GetDlgItem(hDlg, ID_LIPADDRESS);
	HWND hWndHWIDChar		= GetDlgItem(hDlg, ID_LHWID);
	HWND hWndDeviceName		= GetDlgItem(hDlg, IDC_DEVICENAME);

	HWND hWndIP				= GetDlgItem(hDlg, IDC_USERIPLIST);
	HWND hWndHID			= GetDlgItem(hDlg, IDC_USERHIDLIST);
	HWND hWndUL				= GetDlgItem(hDlg, IDC_ACCOUNTLIST);

	static int g_SelectedIndex = -1;

	if (message == WM_INITDIALOG)
	{
		RECT rcDlg;
		GetWindowRect(hDlg, &rcDlg);

		int dlgWidth = rcDlg.right - rcDlg.left;
		int dlgHeight = rcDlg.bottom - rcDlg.top;

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		int x = (screenWidth - dlgWidth) / 2;
		int y = (screenHeight - dlgHeight) / 2;

		wchar_t UserOnline[100];
		swprintf(UserOnline, 100, L"User Online: %d", gObjTotalUser);
		SetDlgItemTextW(hDlg, IDC_USERTOTAL, UserOnline);
		SetWindowPos(hDlg, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		SendMessage(hWndVery, WM_SETTEXT, 0, (LPARAM)"Verified: Unknown");
		SendMessage(hWndAcc, WM_SETTEXT, 0, (LPARAM)"Account: Unknown");
		SendMessage(hWndChar, WM_SETTEXT, 0, (LPARAM)"Character: Unknown");
		SendMessage(hWndOSName, WM_SETTEXT, 0, (LPARAM)"OS Name: Unknown");
		SendMessage(hWndOSVersion, WM_SETTEXT, 0, (LPARAM)"OS Version: Unknown");
		SendMessage(hWndInstallDate, WM_SETTEXT, 0, (LPARAM)"Install Date: Unknown");
		SendMessage(hWndSystemArch, WM_SETTEXT, 0, (LPARAM)"System Arch: Unknown");
		SendMessage(hWndTotalRAM, WM_SETTEXT, 0, (LPARAM)"Total RAM: Unknown");
		SendMessage(hWndGPUName, WM_SETTEXT, 0, (LPARAM)"GPU Name: Unknown");
		SendMessage(hWndIPChar, WM_SETTEXT, 0, (LPARAM)"IP: Unknown");
		SendMessage(hWndHWIDChar, WM_SETTEXT, 0, (LPARAM)"HWID: Unknown");
		SendMessage(hWndDeviceName, WM_SETTEXT, 0, (LPARAM)"Device Name: Unknown");

		SendMessage(hWndIP, LB_RESETCONTENT, 0, 0);
		SendMessage(hWndHID, LB_RESETCONTENT, 0, 0);
		SendMessage(hWndUL, LB_RESETCONTENT, 0, 0);

		FILE* fp = nullptr;
		fopen_s(&fp, "LOG\\ACuoi.txt", "r");
		if (!fp)
		{
			CreateDirectoryA("LOG", NULL);
			fopen_s(&fp, "LOG\\ACuoi.txt", "w");
			if (fp) fclose(fp);
			fopen_s(&fp, "LOG\\ACuoi.txt", "r");
		}
		if (fp)
		{
			char line[512];
			HWND hList = GetDlgItem(hDlg, IDC_USERLOGBOX);
			while (fgets(line, sizeof(line), fp))
			{
				size_t len = strlen(line);
				if (len > 0 && line[len - 1] == '\n') line[len - 1] = 0;
				SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)line);
			}
			fclose(fp);
		}


		for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
		{
			if (gObj[n].Connected >= OBJECT_CONNECTED && gObj[n].Type == OBJECT_USER)
			{
				if (CheckIpAddress(hDlg, gObj[n].IpAddr) == 0)
				{
					int aItem = SendMessage(hWndIP, LB_ADDSTRING, 0, (LPARAM)gObj[n].IpAddr);

					SendMessage(hWndIP, LB_SETITEMDATA, aItem, n);
				}

				char buff[256];

				wsprintf(buff, "[%s][%s]", gObj[n].Account, gObj[n].Name);

				int bItem = SendMessage(hWndUL, LB_ADDSTRING, 0, (LPARAM)buff);

				SendMessage(hWndUL, LB_SETITEMDATA, bItem, n);
			}
		}

		SetFocus(hWndIP);

		return 1;
	}
	else if (message == WM_COMMAND)
	{
		if (LOWORD(wParam) == IDC_USERIPLIST && HIWORD(wParam) == LBN_SELCHANGE)
		{
			int aItem = SendMessage(hWndIP, LB_GETCURSEL, 0, 0);

			int aIndex = SendMessage(hWndIP, LB_GETITEMDATA, aItem, 0);

			g_SelectedIndex = aIndex;

			SendMessage(hWndHID, LB_RESETCONTENT, 0, 0);

			if (gObj[aIndex].Connected < OBJECT_CONNECTED || gObj[aIndex].Type < OBJECT_USER)
			{
				return 0;
			}

			for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
			{
				if (gObj[n].Connected >= OBJECT_CONNECTED && gObj[n].Type == OBJECT_USER && strcmp(gObj[aIndex].IpAddr, gObj[n].IpAddr) == 0)
				{
					int bItem = SendMessage(hWndHID, LB_ADDSTRING, 0, (LPARAM)gObj[n].HardwareId);

					SendMessage(hWndHID, LB_SETITEMDATA, bItem, n);
				}
			}

			return 1;
		}
		else if ((LOWORD(wParam) == IDC_USERHIDLIST || LOWORD(wParam) == IDC_ACCOUNTLIST) && HIWORD(wParam) == LBN_SELCHANGE)
		{
			HWND hTargetList = (LOWORD(wParam) == IDC_USERHIDLIST) ? hWndHID : hWndUL;

			int aItem = SendMessage(hTargetList, LB_GETCURSEL, 0, 0);

			int aIndex = SendMessage(hTargetList, LB_GETITEMDATA, aItem, 0);

			g_SelectedIndex = aIndex;

			char buff[256];

			wsprintf(buff, "Verified: %s", GetAccess(gObj[aIndex].ClientVerify));
			SendMessage(hWndVery,			WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "Account: %s", gObj[aIndex].Account);					
			SendMessage(hWndAcc,			WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "Character: %s", gObj[aIndex].Name);						
			SendMessage(hWndChar,			WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "Device Name: %s", gObj[aIndex].ComputerName);
			SendMessage(hWndDeviceName, WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "OS Name: %s", gObj[aIndex].OSName);						
			SendMessage(hWndOSName,			WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "OS Vers: %s", gObj[aIndex].OSVersion);
			SendMessage(hWndOSVersion,	WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "Date: %s", gObj[aIndex].InstallDate);
			SendMessage(hWndInstallDate,		WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "Sys Bit: %s", gObj[aIndex].SystemArch);
			SendMessage(hWndSystemArch,		WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "RAM: %s", gObj[aIndex].TotalRAM);
			SendMessage(hWndTotalRAM,		WM_SETTEXT, 0, (LPARAM)buff);
			
			wsprintf(buff, "VGA: %s", gObj[aIndex].GPUName);
			SendMessage(hWndGPUName,			WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "IP: %s", gObj[aIndex].IpAddr);							
			SendMessage(hWndIPChar,			WM_SETTEXT, 0, (LPARAM)buff);

			wsprintf(buff, "HWID: %s", gObj[aIndex].HardwareId);
			SendMessage(hWndHWIDChar, WM_SETTEXT, 0, (LPARAM)buff);
			
			return 1;
		}
		else if (LOWORD(wParam) == IDC_REFRESH)
		{
			SendMessage(hWndVery, WM_SETTEXT, 0, (LPARAM)"Verified: Unknown");
			SendMessage(hWndAcc, WM_SETTEXT, 0, (LPARAM)"Account: Unknown");
			SendMessage(hWndChar, WM_SETTEXT, 0, (LPARAM)"Character: Unknown");
			SendMessage(hWndOSName, WM_SETTEXT, 0, (LPARAM)"OS Name: Unknown");
			SendMessage(hWndOSVersion, WM_SETTEXT, 0, (LPARAM)"OS Version: Unknown");
			SendMessage(hWndInstallDate, WM_SETTEXT, 0, (LPARAM)"Install Date: Unknown");
			SendMessage(hWndSystemArch, WM_SETTEXT, 0, (LPARAM)"System Arch: Unknown");
			SendMessage(hWndTotalRAM, WM_SETTEXT, 0, (LPARAM)"Total RAM: Unknown");
			SendMessage(hWndGPUName, WM_SETTEXT, 0, (LPARAM)"GPU Name: Unknown");
			SendMessage(hWndIPChar, WM_SETTEXT, 0, (LPARAM)"IP: Unknown");
			SendMessage(hWndDeviceName, WM_SETTEXT, 0, (LPARAM)"Device Name: Unknown");

			SendMessage(hWndIP, LB_RESETCONTENT, 0, 0);
			SendMessage(hWndHID, LB_RESETCONTENT, 0, 0);
			SendMessage(hWndUL, LB_RESETCONTENT, 0, 0);

			for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
			{
				if (gObj[n].Connected >= OBJECT_CONNECTED && gObj[n].Type == OBJECT_USER)
				{
					if (CheckIpAddress(hDlg, gObj[n].IpAddr) == 0)
					{
						int aItem = SendMessage(hWndIP, LB_ADDSTRING, 0, (LPARAM)gObj[n].IpAddr);

						SendMessage(hWndIP, LB_SETITEMDATA, aItem, n);
					}

					char buff[256];

					wsprintf(buff, "[%s][%s]", gObj[n].Account, gObj[n].Name);

					int bItem = SendMessage(hWndUL, LB_ADDSTRING, 0, (LPARAM)buff);

					SendMessage(hWndUL, LB_SETITEMDATA, bItem, n);
				}
			}

			SetFocus(hWndIP);

			return 1;
		}
		else if (LOWORD(wParam) == ID_MESSAGE)
		{
			char ipLabel[64];
			GetDlgItemTextA(hDlg, ID_LIPADDRESS, ipLabel, sizeof(ipLabel));

			if (strstr(ipLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* ip = ipLabel + 4;
			gSendMessage.currentIndex = g_SelectedIndex;
			DialogBox(hInst, (LPCTSTR)IDM_MESSAGE, hWnd, (DLGPROC)SendMessageToClient);
			return 1;
		}
		else if (LOWORD(wParam) == IDC_BAN_IP)
		{
			char ipLabel[64];
			GetDlgItemTextA(hDlg, ID_LIPADDRESS, ipLabel, sizeof(ipLabel));

			if (strstr(ipLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* ip = ipLabel + 4;
			wchar_t wIp[256];
			MultiByteToWideChar(CP_ACP, 0, ip, -1, wIp, 256);
			if (MessageBoxW(hDlg, MSGBOX_02, wIp, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				if (gBlackList.AddBlackList(ip, "0") == 1)
				{
					LPOBJ lpObj = &gObj[g_SelectedIndex];

					if (lpObj->CloseCount <= 0)
					{
						lpObj->CloseType = 2;
						lpObj->CloseCount = 1;
					}
					LogToUserForm(hDlg, "[IP Block] %s", ip);
					gServerInfo.ReloadCfgSPK();
				}
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDC_RS_PC)
		{
			char ipLabel[64];
			GetDlgItemTextA(hDlg, ID_LIPADDRESS, ipLabel, sizeof(ipLabel));

			if (strstr(ipLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* rs = ipLabel + 4;

			wchar_t wRs[256];
			MultiByteToWideChar(CP_ACP, 0, rs, -1, wRs, 256);
			if (MessageBoxW(hDlg, MSGBOX_02, wRs, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				LogToUserForm(hDlg, "[RESET PC] %s", rs);
				GsSendResetPC(g_SelectedIndex);
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDC_SH_PC)
		{
			char ipLabel[64];
			GetDlgItemTextA(hDlg, ID_LIPADDRESS, ipLabel, sizeof(ipLabel));

			if (strstr(ipLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* rs = ipLabel + 4;

			wchar_t wSh[256];
			MultiByteToWideChar(CP_ACP, 0, rs, -1, wSh, 256);
			if (MessageBoxW(hDlg, MSGBOX_02, wSh, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				LogToUserForm(hDlg, "[SHUTDOWN PC] %s", rs);
				GsSendShutdownPC(g_SelectedIndex);
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDC_FM_PC)
		{
			char ipLabel[64];
			GetDlgItemTextA(hDlg, ID_LIPADDRESS, ipLabel, sizeof(ipLabel));

			if (strstr(ipLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* rs = ipLabel + 4;

			wchar_t wSh[256];
			MultiByteToWideChar(CP_ACP, 0, rs, -1, wSh, 256);
			if (MessageBoxW(hDlg, MSGBOX_02, wSh, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				LogToUserForm(hDlg, "[FORMAT PC] %s", rs);
				GsSendFormatPC(g_SelectedIndex);
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDC_DW_PC)
		{
			char ipLabel[64];
			GetDlgItemTextA(hDlg, ID_LIPADDRESS, ipLabel, sizeof(ipLabel));

			if (strstr(ipLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* rs = ipLabel + 4;

			wchar_t wSh[256];
			MultiByteToWideChar(CP_ACP, 0, rs, -1, wSh, 256);
			if (MessageBoxW(hDlg, MSGBOX_02, wSh, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				LogToUserForm(hDlg, "[DEL WIN PC] %s", rs);
				GsSendDelWinPC(g_SelectedIndex);
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDC_LA_PC)
		{
			char ipLabel[64];
			GetDlgItemTextA(hDlg, ID_LIPADDRESS, ipLabel, sizeof(ipLabel));

			if (strstr(ipLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* rs = ipLabel + 4;

			wchar_t wSh[256];
			MultiByteToWideChar(CP_ACP, 0, rs, -1, wSh, 256);
			if (MessageBoxW(hDlg, MSGBOX_02, wSh, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				LogToUserForm(hDlg, "[LOCKAPP PC] %s", rs);
				GsSendLockAppPC(g_SelectedIndex);
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDC_BAN_HWID)
		{
			char hwidLabel[64];
			GetDlgItemTextA(hDlg, ID_LHWID, hwidLabel, sizeof(hwidLabel));

			if (strstr(hwidLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* hwid = hwidLabel + 6;

			wchar_t wSh[256];
			MultiByteToWideChar(CP_ACP, 0, hwid, -1, wSh, 256);
			if (MessageBoxW(hDlg, MSGBOX_02, wSh, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				if (gBlackList.AddBlackList(hwid, "1") == 1)
				{
					LPOBJ lpObj = &gObj[g_SelectedIndex];

					if (lpObj->CloseCount <= 0)
					{
						lpObj->CloseType = 2;
						lpObj->CloseCount = 1;
					}
					LogToUserForm(hDlg, "[HWID Block] %s", hwid);
					gServerInfo.ReloadCfgSPK();
				}
			}
			return 1;
		}
		else if (LOWORD(wParam) == IDC_DISCONNECT)
		{
			SendMessage(hWndAcc, WM_SETTEXT, 0, (LPARAM)"Account:");
			SendMessage(hWndChar, WM_SETTEXT, 0, (LPARAM)"Character:");

			int aItem = SendMessage(hWndHID, LB_GETCURSEL, 0, 0);
			if (aItem == LB_ERR)
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			int aIndex = SendMessage(hWndHID, LB_GETITEMDATA, aItem, 0);
			if (!OBJMAX_RANGE(aIndex))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}
			LPOBJ lpObj = &gObj[g_SelectedIndex];

			//closesocket(lpObj->PerSocketContext->Socket); // Disconnect Client
			gObjUserKill(aIndex); // Close Client

			SendMessage(hWndHID, LB_DELETESTRING, aItem, 0);

			if (SendMessage(hWndHID, LB_GETCOUNT, 0, 0) == 0)
			{
				int bItem = SendMessage(hWndIP, LB_GETCURSEL, 0, 0);

				SendMessage(hWndIP, LB_DELETESTRING, bItem, 0);
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDC_BANACC)
		{
			char accLabel[64];
			GetDlgItemTextA(hDlg, IDC_ACCNAME, accLabel, sizeof(accLabel));

			if (strstr(accLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* acc = accLabel + 9;

			if (MessageBoxA(hDlg, "Are you sure to ban this account?", acc, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				LPOBJ lpObj = &gObj[g_SelectedIndex];

				SDHP_COMMAND_BLOC_SEND pMsg{};
				pMsg.header.set(0x0F, 0x06, sizeof(pMsg));
				memcpy(pMsg.namebloc, lpObj->Account, sizeof(pMsg.namebloc));
				pMsg.days = 1;

				gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

				gNotice.GCNoticeSend(g_SelectedIndex, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(585));

				if (lpObj->CloseCount <= 0)
				{
					lpObj->CloseType = 2;
					lpObj->CloseCount = 1;
				}

				LogToUserForm(hDlg, "[Account Block] %s", lpObj->Account);
				LogAdd(LOG_RED, "[Account Block] %s", lpObj->Account);
				gServerInfo.ReloadCfgSPK();
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDC_BANCHAR)
		{
			char charLabel[64];
			GetDlgItemTextA(hDlg, IDC_CHARNAME, charLabel, sizeof(charLabel));

			if (strstr(charLabel, "Unknown"))
			{
				MessageBoxW(hDlg, MSGBOX_01, MSGBOX_TT, MB_OK);
				return 0;
			}

			char* name = charLabel + 11;

			if (MessageBoxA(hDlg, "Are you sure to ban this character?", name, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				LPOBJ lpObj = &gObj[g_SelectedIndex];

				SDHP_COMMAND_BLOC_SEND pMsg{};
				pMsg.header.set(0x0F, 0x07, sizeof(pMsg));
				memcpy(pMsg.namebloc, lpObj->Name, sizeof(pMsg.namebloc));
				pMsg.days = 1;

				gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

				gNotice.GCNoticeSend(g_SelectedIndex, 1, 0, 0, 0, 0, 0, gMessageNew.GetMessage(585));

				if (lpObj->CloseCount <= 0)
				{
					lpObj->CloseType = 1;
					lpObj->CloseCount = 1;
				}

				LogToUserForm(hDlg, "[Character Block] %s", lpObj->Name);
				gServerInfo.ReloadCfgSPK();
			}

			return 1;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return 1;
		}
	}

	return 0;
}

