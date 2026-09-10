#include "stdafx.h"
#include "ResetLimiter.h"
#include "ServerInfo.h"
#include "ACuoi_MessLang.h"
#include "Notice.h"
#include "Util.h"
#include <fstream>
using namespace std;

ResetLitmitLock gResetLitmit;

void ResetLitmitLock::gObjGHRSRunProc()
{
	this->ThayDoiGioiHanRS();
}

void ResetLitmitLock::ThayDoiGioiHanRS()
{
	tm* today;
	time_t ltime;
	time(&ltime);
	today = localtime(&ltime);

	if (today->tm_sec == this->ResetGiay &&
		today->tm_min == this->ResetPhut &&
		today->tm_hour == this->ResetGio)
	{
		int DayOfWeek = today->tm_wday;
		int ResetAdd = this->ResetByDay[(DayOfWeek + 6) % 7];
		const char* dayNames[7] = {
			"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
		};

		this->GioiHanRS += ResetAdd;

		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessageNew.GetMessage(551), ResetAdd, this->GioiHanRS);

		LogAdd(LOG_DARKGREEN, "Giới Hạn Reset: %d %02d:%02d:%02d +%d (%s)",
			this->GioiHanRS, this->ResetGio, this->ResetPhut, this->ResetGiay,
			ResetAdd, dayNames[(DayOfWeek + 6) % 7]);

		WritePrivateProfileString(
			"CURRENT",
			"GioiHanRS",
			std::to_string(this->GioiHanRS).c_str(),
			"..//Data//Util//ACuoi_GHRS.ini"
		);

		return;
	}
}

void ResetLitmitLock::ReadResetInfo()
{
	this->GioiHanRS = GetPrivateProfileInt("CURRENT", "GioiHanRS", 0, "..//Data//Util//ACuoi_GHRS.ini");

	this->ResetGio = GetPrivateProfileInt("CONFIG", "Gio", 0, "..//Data//Util//ACuoi_GHRS.ini");
	this->ResetPhut = GetPrivateProfileInt("CONFIG", "Phut", 0, "..//Data//Util//ACuoi_GHRS.ini");
	this->ResetGiay = GetPrivateProfileInt("CONFIG", "Giay", 0, "..//Data//Util//ACuoi_GHRS.ini");

	this->ResetByDay[0] = GetPrivateProfileInt("CONFIG", "T2", 0, "..//Data//Util//ACuoi_GHRS.ini");
	this->ResetByDay[1] = GetPrivateProfileInt("CONFIG", "T3", 0, "..//Data//Util//ACuoi_GHRS.ini");
	this->ResetByDay[2] = GetPrivateProfileInt("CONFIG", "T4", 0, "..//Data//Util//ACuoi_GHRS.ini");
	this->ResetByDay[3] = GetPrivateProfileInt("CONFIG", "T5", 0, "..//Data//Util//ACuoi_GHRS.ini");
	this->ResetByDay[4] = GetPrivateProfileInt("CONFIG", "T6", 0, "..//Data//Util//ACuoi_GHRS.ini");
	this->ResetByDay[5] = GetPrivateProfileInt("CONFIG", "T7", 0, "..//Data//Util//ACuoi_GHRS.ini");
	this->ResetByDay[6] = GetPrivateProfileInt("CONFIG", "CN", 0, "..//Data//Util//ACuoi_GHRS.ini");

	tm* today;
	time_t ltime;
	time(&ltime);
	today = localtime(&ltime);

	int todayIndex = (today->tm_wday + 6) % 7;
	int todayValue = this->ResetByDay[todayIndex];

	const char* dayNames[7] = {
		"Thứ 2", "Thứ 3", "Thứ 4", "Thứ 5", "Thứ 6", "Thứ 7", "Chủ Nhật"
	};

	LogAdd(LOG_DARKGREEN, "GHRS Hôm Nay Là : %d Lần - (%s) - %02d:%02d:%02d Reset : +%d Lần",
		this->GioiHanRS,
		dayNames[todayIndex],
		this->ResetGio, this->ResetPhut, this->ResetGiay,
		todayValue);

}