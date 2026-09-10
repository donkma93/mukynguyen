#include "stdafx.h"
#include "GetLicenseID.h"
#include "ThemidaSDK.h"
#include "Protect.h"
#include "resource.h"

CreateLicenseFile gFile;

bool CreateLicenseFile::LoadLicenseAndApply()
{
	unsigned char xORKey[5] = { 0x65, 0xCB, 0x96, 0x4B, 0x21 };

	HANDLE hFile = CreateFileA("..\\License.spk", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == 0 || fileSize > 44)
	{
		CloseHandle(hFile);
		return false;
	}

	BYTE buffer[45] = { 0 };
	DWORD bytesRead = 0;

	if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL) || bytesRead != fileSize)
	{
		CloseHandle(hFile);
		return false;
	}

	for (DWORD i = 0; i < bytesRead; i++)
	{
		buffer[i] ^= xORKey[i % 5];
	}

	buffer[bytesRead] = 0;

	strncpy(this->ComputerHardwareId[0], (char*)buffer, 44);
	this->ComputerHardwareId[0][44] = 0;

	CloseHandle(hFile);
	return true;
}

bool CreateLicenseFile::CheckingGetComputerHardwareId(LPCSTR HDWID)
{
	CLEAR_START
	VM_START

	if (!this->LoadLicenseAndApply())
	{
		MessageBoxA(0, "License key load failed", "ERROR", MB_ICONERROR);
		ExitProcess(0);
	}

	for (int x = 0; x < 25; x++)
	{
		if (!strcmp(HDWID, ComputerHardwareId[x]))
		{
			return true;
		}
	}
	return false;

	VM_END
	CLEAR_END
}

bool CreateLicenseFile::GetPhysicalDriveSerialNumber(int PhysicalDriveNumber, char* PhysicalDriveSerial, int PhysicalDriveSerialSize)
{
	char PhysicalDrivePath[MAX_PATH];
	wsprintf(PhysicalDrivePath, "\\\\.\\PhysicalDrive%d", PhysicalDriveNumber);

	HANDLE PhysicalDriveHandle = CreateFile(PhysicalDrivePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (PhysicalDriveHandle == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	DWORD BytesReturned;
	STORAGE_PROPERTY_QUERY StoragePropertyQuery{};
	STORAGE_DESCRIPTOR_HEADER StorageDescriptorHeader{};

	StoragePropertyQuery.PropertyId = StorageDeviceProperty;
	StoragePropertyQuery.QueryType = PropertyStandardQuery;

	if (!DeviceIoControl(PhysicalDriveHandle, IOCTL_STORAGE_QUERY_PROPERTY,
		&StoragePropertyQuery, sizeof(StoragePropertyQuery),
		&StorageDescriptorHeader, sizeof(StorageDescriptorHeader),
		&BytesReturned, 0))
	{
		CloseHandle(PhysicalDriveHandle);
		return 0;
	}

	DWORD BufferSize = StorageDescriptorHeader.Size;
	BYTE* PhysicalDriveBuff = new BYTE[BufferSize];
	memset(PhysicalDriveBuff, 0, BufferSize);

	if (!DeviceIoControl(PhysicalDriveHandle, IOCTL_STORAGE_QUERY_PROPERTY,
		&StoragePropertyQuery, sizeof(StoragePropertyQuery),
		PhysicalDriveBuff, BufferSize,
		&BytesReturned, 0))
	{
		CloseHandle(PhysicalDriveHandle);
		delete[] PhysicalDriveBuff;
		return 0;
	}

	STORAGE_DEVICE_DESCRIPTOR* descriptor = (STORAGE_DEVICE_DESCRIPTOR*)PhysicalDriveBuff;
	DWORD offset = descriptor->SerialNumberOffset;

	if (offset == 0 || offset >= BufferSize)
	{
		CloseHandle(PhysicalDriveHandle);
		delete[] PhysicalDriveBuff;
		return 0;
	}

	__try
	{
		char* pSerial = (char*)(PhysicalDriveBuff + offset);
		int serialLen = 0;

		for (DWORD i = 0; i < BufferSize - offset && serialLen < PhysicalDriveSerialSize - 1; i++)
		{
			char c = pSerial[i];
			if (c == 0 || c == ' ' || c == '\n' || c == '\r') break;
			PhysicalDriveSerial[serialLen++] = c;
		}

		PhysicalDriveSerial[serialLen] = 0;

		CloseHandle(PhysicalDriveHandle);
		delete[] PhysicalDriveBuff;
		return 1;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		CloseHandle(PhysicalDriveHandle);
		delete[] PhysicalDriveBuff;
		return 0;
	}
}

char* CreateLicenseFile::GetHardwareId()
{
	char PhysicalDriveSerial[256];
	memset(PhysicalDriveSerial, 0, sizeof(PhysicalDriveSerial));

	for (int n = 0; n < 5; n++)
	{
		if (this->GetPhysicalDriveSerialNumber(n, PhysicalDriveSerial, sizeof(PhysicalDriveSerial)) != 0)
		{
			break;
		}
	}

	DWORD ComputerHardwareId1 = *(DWORD*)(&PhysicalDriveSerial[0x00]) ^ *(DWORD*)(&PhysicalDriveSerial[0x10]) ^ 0xFC3E59E2;
	DWORD ComputerHardwareId2 = *(DWORD*)(&PhysicalDriveSerial[0x04]) ^ *(DWORD*)(&PhysicalDriveSerial[0x14]) ^ 0x43AC7E86;
	DWORD ComputerHardwareId3 = *(DWORD*)(&PhysicalDriveSerial[0x08]) ^ *(DWORD*)(&PhysicalDriveSerial[0x18]) ^ 0xEA86C5CA;
	DWORD ComputerHardwareId4 = *(DWORD*)(&PhysicalDriveSerial[0x0C]) ^ *(DWORD*)(&PhysicalDriveSerial[0x1C]) ^ 0x7D3C15BB;
	DWORD ComputerHardwareId5 = *(DWORD*)(&PhysicalDriveSerial[0x20]) ^ *(DWORD*)(&PhysicalDriveSerial[0x24]) ^ 0x9AB712EF;
	
	memset(HardwareId, 0, sizeof(HardwareId));
	wsprintf(HardwareId, "%08X-%08X-%08X-%08X-%08X",
		ComputerHardwareId1, ComputerHardwareId2, ComputerHardwareId3, ComputerHardwareId4, ComputerHardwareId5);
	return HardwareId;
}

void CreateLicenseFile::ErrorMessageBox()
{
	CLEAR_START
	VM_START

	memset(this->ComputerHardware, 0, sizeof(this->ComputerHardware));
	MessageBox(0, "You have not registered the license key.", "IDKey", MB_ICONSTOP | MB_OK);
	this->SafeExitProcess();
	VM_END
	CLEAR_END
}

inline void CreateLicenseFile::SafeExitProcess()
{
	CLEAR_START
	VM_START

	while (true)
	{
		TerminateProcess(GetCurrentProcess(), 0);
		CRASH_APPLICATION_MACRO
	}

	VM_END
	CLEAR_END
}

void CreateLicenseFile::StartCheckID(HWND hWnd)
{
	char HardwareId[45];
	memcpy(HardwareId, gFile.GetHardwareId(), sizeof(HardwareId));

	if (!gFile.CheckingGetComputerHardwareId(HardwareId))
	{
		gFile.ErrorMessageBox();
	}
}