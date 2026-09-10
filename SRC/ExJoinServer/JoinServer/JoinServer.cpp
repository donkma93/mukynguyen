#include "stdafx.h"
#include "resource.h"
#include "JoinServer.h"
#include "AccountManager.h"
#include "AllowableIpList.h"
#include "MiniDump.h"
#include "Protect.h"
#include "QueryManager.h"
#include "ServerDisplayer.h"
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
char CustomerHardwareId[36];
BOOL CaseSensitive;
BOOL MD5Encryption;
RegJoin gRegJoin;
char SPKGLPW[11];

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	VM_START

	CMiniDump::Start();
	//gFile.StartCheckID(hWnd);
	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_JOINSERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	GetPrivateProfileString("JoinServerInfo","CustomerName","",CustomerName,sizeof(CustomerName),".\\JoinServer.ini");

	GetPrivateProfileString("JoinServerInfo","CustomerHardwareId","",CustomerHardwareId,sizeof(CustomerHardwareId),".\\JoinServer.ini");

	#if(PROTECT_STATE==1)

	#if(JOINSERVER_UPDATE>=801)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S8_JOIN_SERVER);
	#elif(JOINSERVER_UPDATE>=601)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S6_JOIN_SERVER);
	#elif(JOINSERVER_UPDATE>=401)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S4_JOIN_SERVER);
	#else
	gProtect.StartAuth(AUTH_SERVER_TYPE_S2_JOIN_SERVER);
	#endif

	#endif

	char buff[256];
	wsprintf(buff,"%s",JS_TieuDe);
	SetWindowText(hWnd,buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		char JoinServerODBC[32] = {0};

		char JoinServerUSER[32] = {0};

		char JoinServerPASS[32] = {0};


		GetPrivateProfileString("JoinServerInfo","JoinServerODBC","",JoinServerODBC,sizeof(JoinServerODBC),".\\JoinServer.ini");

		GetPrivateProfileString("JoinServerInfo","JoinServerUSER","",JoinServerUSER,sizeof(JoinServerUSER),".\\JoinServer.ini");

		GetPrivateProfileString("JoinServerInfo","JoinServerPASS","",JoinServerPASS,sizeof(JoinServerPASS),".\\JoinServer.ini");

		GetPrivateProfileString("JoinServerInfo","SPKGLPW", "0968092399", SPKGLPW, sizeof(SPKGLPW), ".\\JoinServer.ini");

		WORD JoinServerPort = GetPrivateProfileInt("JoinServerInfo","JoinServerPort",55970,".\\JoinServer.ini");

		char ConnectServerAddress[16] = {0};

		GetPrivateProfileString("JoinServerInfo","ConnectServerAddress","127.0.0.1",ConnectServerAddress,sizeof(ConnectServerAddress),".\\JoinServer.ini");

		WORD ConnectServerPort = GetPrivateProfileInt("JoinServerInfo","ConnectServerPort",55557,".\\JoinServer.ini");

		CaseSensitive = GetPrivateProfileInt("JoinServerInfo","CaseSensitive",0,".\\JoinServer.ini");

		gRegJoin.RegNewAccount = GetPrivateProfileInt("JoinServerInfo","RegNewAccount",0,".\\JoinServer.ini");

		gRegJoin.ChieuDaiKyTu  = GetPrivateProfileInt("JoinServerInfo","ChieuDaiKyTu",4,".\\JoinServer.ini");

		MD5Encryption = GetPrivateProfileInt("JoinServerInfo","MD5Encryption",0,".\\JoinServer.ini");

		if(gQueryManager.Connect(JoinServerODBC,JoinServerUSER,JoinServerPASS) == 0)
		{
			LogAdd(LOG_RED, "Không Thể Kết Nối Tới Data SQL");
			LogAdd(LOG_RED, "Có Thể Chưa Chạy Reg32 Và Reg64 Hoặc Tên DB Không Đúng");
			LogAdd(LOG_RED, "Hãy Kiểm Tra Lại Cấu Hình Tên ODBC Phải Khớp DB SQL");
			LogAdd(LOG_RED, "Hoặc Liên Hệ Zalo SĐT: BQT MU KỶ NGUYÊN (MU KỶ NGUYÊN) Để Được Hỗ Trợ");
		}
		else
		{
			if(gSocketManager.Start(JoinServerPort) == 0)
			{
				gQueryManager.Disconnect();
			}
			else
			{
				if(gSocketManagerUdp.Connect(ConnectServerAddress,ConnectServerPort) == 0)
				{
					gSocketManager.Clean();

					gQueryManager.Disconnect();
				}
				else
				{
					gAllowableIpList.Load("AllowableIpList.txt");

					SetTimer(hWnd,TIMER_1000,1000,0);
				}
			}
		}
	}
	else
	{
		LogAdd(LOG_RED,"Lỗi: %d chưa biết.",WSAGetLastError());//
	}

	SetTimer(hWnd,TIMER_2000,2000,0);

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_JOINSERVER);

	MSG msg;

	while(GetMessage(&msg,0,0,0) != 0)
	{
		if(TranslateAccelerator(msg.hwnd,hAccelTable,&msg) == 0)
		{
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
	WNDCLASSEX wcex = { 0 }; // Inicializa a struct com 0 em todos os membros

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_JOINSERVER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_JOINSERVER);
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

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case IDM_EXIT:
					if (ShowConfirmDialog(hWnd, "Ban Co Dong Y Tat Join Sever Khong?") == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				case IDM_RELOAD_RELOAD:
					LogAdd(LOG_BLUE, "JoinServer loaded successfully");
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case TIMER_1000:
					JoinServerLiveProc();
					gAccountManager.DisconnectProc();
					break;
				case TIMER_2000:
					gServerDisplayer.Run();
					break;
				default:
					break;
			}
			break;
		case WM_CLOSE:
			if (ShowConfirmDialog(hWnd, "Ban Co Dong Y Tat Join Sever Khong?") == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT hPaintStruct;
			HDC hdc = BeginPaint(hWnd,&hPaintStruct);
			HDC hMemDC = CreateCompatibleDC(hdc);
			HBITMAP OldBmp = (HBITMAP)SelectObject(hMemDC,LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1)));
			BitBlt(hdc,0,0,1920,80,hMemDC,0,0,SRCCOPY);
			SelectObject(hMemDC,OldBmp);
			DeleteDC(hMemDC);
			EndPaint(hWnd,&hPaintStruct);
		}
		break;
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