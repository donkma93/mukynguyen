// ConnectServer.cpp: implementation of the WinMain class.
// Revisado: 02/10/23 19:52 GMT-3
// By: Qubit
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ConnectServer.h"
#include "MiniDump.h"
#include "Protect.h"
#include "ServerDisplayer.h"
#include "ServerList.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "ThemidaSDK.h"
#include "Util.h"
#include "../../Addon/DualColor.h"
#include "GetLicenseID.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;

char CustomerName[32];
long MaxIpConnection;
char Version[9];

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	VM_START

	CMiniDump::Start();

	//gFile.StartCheckID(hWnd);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CONNECTSERVER, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if (InitInstance(hInstance, nCmdShow) == 0)
	{
		return 0;
	}

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) == 0)
	{
		WORD ConnectServerPortTCP = GetPrivateProfileInt(_T("ConnectServerInfo"), _T("ConnectServerPortTCP"), 44405, _T(".\\ConnectServer.ini"));

		WORD ConnectServerPortUDP = GetPrivateProfileInt(_T("ConnectServerInfo"), _T("ConnectServerPortUDP"), 55557, _T(".\\ConnectServer.ini"));

		GetPrivateProfileString(_T("ConnectServerInfo"), _T("Version"), _T(""), Version, sizeof(Version), _T(".\\ConnectServer.ini"));

		MaxIpConnection = GetPrivateProfileInt(_T("ConnectServerInfo"), _T("MaxIpConnection"), 0, _T(".\\ConnectServer.ini"));

		if (gSocketManager.Start(ConnectServerPortTCP) != 0 && gSocketManagerUdp.Start(ConnectServerPortUDP) != 0)
		{
			gServerList.Load(_T("ServerList.xml"));

			SetTimer(hWnd, TIMER_1000, 1000, 0);

			SetTimer(hWnd, TIMER_5000, 5000, 0);

			SetTimer(hWnd, TIMER_2000, 2000, 0);

			HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONNECTSERVER));

			MSG msg;

			while (GetMessage(&msg, 0, 0, 0) != 0)
			{
				if (TranslateAccelerator(msg.hwnd, hAccelTable, &msg) == 0)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			CMiniDump::Clean();

			VM_END

				return (int)msg.wParam;
		}
	}
	else
	{
		LogAdd(LOG_RED, _T("WSAStartup() failed with error: %d"), WSAGetLastError());
	}

	CMiniDump::Clean();

	VM_END

		return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CONNECTSERVER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_CONNECTSERVER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		0,
		768,
		600,
		0,
		0,
		hInstance,
		0
	);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;

			case IDM_EXIT:
				if (ShowConfirmDialog(hWnd, "Ban Co Dong Y Tat Connectsever Khong ?") == IDYES)
				{
					DestroyWindow(hWnd);
				}
				break;
		
			case IDM_RELOAD_RELOADSERVERLIST:
			gServerList.Load("ServerList.xml");
			LogAdd(LOG_BLUE, "[ServerList] ServerList loaded successfully");
			break;
			default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		case WM_TIMER:
		switch (wParam)
		{
			case TIMER_1000:
			gServerList.MainProc();
			break;
			case TIMER_2000:
			gServerDisplayer.Run();
			break;
			case TIMER_5000:
			ConnectServerTimeoutProc();
			break;
			default:
			break;
		}
		break;
		case WM_CLOSE:
		if (ShowConfirmDialog(hWnd, "Ban Co Dong Y Tat Connectsever Khong ?") == IDYES)
		{
			DestroyWindow(hWnd);
		}
		break;
		case WM_DESTROY:
		PostQuitMessage(0);
		case WM_PAINT:
		{
			PAINTSTRUCT hPaintStruct;
			HDC hdc = BeginPaint(hWnd,&hPaintStruct);
			HDC hMemDC = CreateCompatibleDC(hdc);
			HBITMAP OldBmp = (HBITMAP)SelectObject(hMemDC,LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1)));
			BitBlt(hdc,0,0,1920,80,hMemDC,0,0,SRCCOPY);//tamando do bitmap
			SelectObject(hMemDC,OldBmp);
			DeleteDC(hMemDC);
			EndPaint(hWnd,&hPaintStruct);
		}
		break;
		default:
		return DefWindowProc(hWnd, message, wParam, lParam);
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


INT_PTR CALLBACK MyYesNoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL)));
		SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL)));

		if (lParam != NULL)
		{
			SetDlgItemTextA(hDlg, IDC_STATIC_TEXT, (LPCSTR)lParam);
		}

		{
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
		}

		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDYES || LOWORD(wParam) == IDNO)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}


int ShowConfirmDialog(HWND hWndParent, const char* message)
{
	return DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_YESNO), hWndParent, MyYesNoProc, (LPARAM)message);
}