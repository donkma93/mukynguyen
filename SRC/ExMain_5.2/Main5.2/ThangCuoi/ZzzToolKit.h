#pragma once
#include "CProtect.h"
#include "d3d9types.h"
#include <fstream>
#define VERSION "5.2"
#define CHECK_ITEM(x) (((x)<0)?-1:((x)>=MAX_ITEM)?-1:x)
#define CUR_FPS_PC 60
#define MAX_CALC_FPS_SEX 2000
#define LODWORD(qw) \
	((DWORD)(qw))
typedef unsigned __int64 SPK;
struct POINT_F
{
	float x;
	float y;
	float w;
	float h;
};

class SPK_ToolKitMain
{
	public:
	bool ShadowFlower;
	int Type;
	bool AutoCtrl;
	char* QN(SPK Number);
	void ThisFont(int PosX, int PosY, DWORD color, DWORD bkcolor, int Width, int Height, BYTE Align, LPCTSTR Text, ...);
	void ThisFontBig(int PosX, int PosY, DWORD color, DWORD bkcolor, int Width, int Height, BYTE Align, LPCTSTR Text, ...);
	bool IsWorkZone(float x, float y, float h, float w);
	void KeySendState(BYTE Folder, BYTE Data);
	void KeySendStateStore(int Type);
	void KeySendStateMocNap(DWORD c);
	void KeySendStateViewMocNap(DWORD c);
	void KeySendStateOpenMocNap(DWORD c);
	int CheckClass();
	int GetPositionScreen();
	int GetCreatePosHeight();

	POINT_F VisiblePos(int index);
	D3DCOLOR VisibleCor(int index);
	void WindowRename();
	void RenderItemKit(int x, int y, int a, int b);
	void RenderToolTip(float x, float y, float w, float h);
	void RenderToolTipExt(float x, float y, float w, float h, int tHi, const char* text, int Enable, int Val, float fVal = 0.0f);
	DWORD ViewExpMax;
	BYTE ViewIndex;
	int FontHeightRead;
	BYTE NextClass;
	POINT GetAccessPos;
	BYTE IsLocker = 0;
	void RenderItemSlot(int X, int Y, int Doc, int Ngang, int Use);
	void WindowFont();
	int BmdSPK();
	void CreateRenderButton(int ID, int Time, float x, float y, float w, float h);
	void RenderMixEffect(float x, float y, int w, int h);
	bool GetIndexMonster(int index);

	DWORD StartTick;
	BYTE EnterSerectArea;
	int ValueCoin[3];
	int	EventCurrentTime;
	BYTE EventGate[3];
	BYTE EventClassMonster[3];
	char GuildNameOld[9];
	DWORD GetAddPoint[5];
};
extern SPK_ToolKitMain IsToolKit;
extern bool m_AutoCtrl;
extern bool m_MiniMapOn;
int SafeGetItem(int index);
char* CharacterCode(int a);
bool IsValidPasswordChar(const char* str);
DWORD GetGoldValue(DWORD Gold);
bool IsFilteredWeapon(int WeaponType);
std::string EncryptXOR(const std::string& input);