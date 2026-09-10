#pragma once
#include <winioctl.h>
class CreateLicenseFile
{
	public:
	char ComputerHardwareId[25][45];
	char ComputerHardware[45];
	char HardwareId[45];

	bool LoadLicenseAndApply();
	bool CheckingGetComputerHardwareId(LPCSTR HDWID);
	bool GetPhysicalDriveSerialNumber(int PhysicalDriveNumber, char* PhysicalDriveSerial, int PhysicalDriveSerialSize);
	char* GetHardwareId();
	void ErrorMessageBox();
	inline void SafeExitProcess();
	void StartCheckID(HWND hWnd);
};
extern CreateLicenseFile gFile;