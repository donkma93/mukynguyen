#include "stdafx.h"
#include "Util.h"
#include <ZzzInventory.h>
#include <Utilities/Log/muConsoleDebug.h>
#include <winioctl.h>

char* ConvertModuleFileName(char* name) // OK
{
	static char buff[MAX_PATH] = { 0 };

	for (int n = strlen(name); n > 0; n--)
	{
		if (name[n] == '\\')
		{
			strcpy_s(buff, sizeof(buff), &name[(n + 1)]);
			break;
		}
	}

	return buff;
}

char* BGetItemName(int ItemType, int Level)
{
	char ItemNameGet[100];
	memset(ItemNameGet, 0, sizeof(ItemNameGet));
	GetItemName(ItemType, (GET_ITEMOPT_LEVEL(Level)), (char*)ItemNameGet);

	std::string a(ItemNameGet);

	return strdup(a.c_str());

}
bool GetPhysicalDriveSerialNumber(int PhysicalDriveNumber, char* PhysicalDriveSerial, int PhysicalDriveSerialSize)
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


char* GetHardwareId() //OK
{
	char PhysicalDriveSerial[256];
	memset(PhysicalDriveSerial, 0, sizeof(PhysicalDriveSerial));

	for (int n = 0; n < 5; n++)
	{
		if (GetPhysicalDriveSerialNumber(n, PhysicalDriveSerial, sizeof(PhysicalDriveSerial)) != 0)
		{
			break;
		}
	}

	DWORD ComputerHardwareId1 = *(DWORD*)(&PhysicalDriveSerial[0x00]) ^ *(DWORD*)(&PhysicalDriveSerial[0x10]) ^ 0xFC3E59E2;
	DWORD ComputerHardwareId2 = *(DWORD*)(&PhysicalDriveSerial[0x04]) ^ *(DWORD*)(&PhysicalDriveSerial[0x14]) ^ 0x43AC7E86;
	DWORD ComputerHardwareId3 = *(DWORD*)(&PhysicalDriveSerial[0x08]) ^ *(DWORD*)(&PhysicalDriveSerial[0x18]) ^ 0xEA86C5CA;
	DWORD ComputerHardwareId4 = *(DWORD*)(&PhysicalDriveSerial[0x0C]) ^ *(DWORD*)(&PhysicalDriveSerial[0x1C]) ^ 0x7D3C15BB;
	DWORD ComputerHardwareId5 = *(DWORD*)(&PhysicalDriveSerial[0x20]) ^ *(DWORD*)(&PhysicalDriveSerial[0x24]) ^ 0x9AB712EF;

	static char HardwareId[MAX_HWID_KEY];
	wsprintf(HardwareId, "%08X-%08X-%08X-%08X-%08X", ComputerHardwareId1, ComputerHardwareId2, ComputerHardwareId3, ComputerHardwareId4, ComputerHardwareId5);
	return HardwareId;
}
