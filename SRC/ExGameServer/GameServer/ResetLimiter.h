#pragma once

class ResetLitmitLock
{
	public:
	void gObjGHRSRunProc();
	void ThayDoiGioiHanRS();
	void ReadResetInfo();
	int GioiHanRS;
	int ResetGio;
	int ResetPhut;
	int ResetGiay;
	int ResetNgay;

	int ResetByDay[7];
};
extern ResetLitmitLock gResetLitmit;