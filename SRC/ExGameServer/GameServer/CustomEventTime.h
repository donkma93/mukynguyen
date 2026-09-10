#include "Protocol.h"
#define MAX_EVENTTIME  42

#pragma pack(push, 1)
struct CUSTOM_EVENTTIME_INFO
{
	int Index;
	char Name[40];
	char Map[40];
	char Gate[40];
};

struct CUSTOM_EVENTTIME_DATA
{
	int index;
	int time;
	char Name[40];
	char Map[40];
};

struct PMSG_CUSTOM_EVENTTIME_SEND
{
	PSWMSG_HEAD header;
	int count;
	BYTE RegLineEvent;
};

struct PMSG_CUSTOM_EVENTTIME_RECV
{
	PSBMSG_HEAD header;
};
#pragma pack(pop)

struct CUSTOM_EVENTTIME_DATA_LEGACY
{
	int index;
	int time;
};

struct PMSG_CUSTOM_EVENTTIME_SEND_LEGACY
{
	PSWMSG_HEAD header;
	int count;
	BYTE RegLineEvent;
};

#pragma pack(push, 1)
struct DASHBOARD_ONLINE_PLAYER
{
	char name[11];
	char mapName[32];
	short level;
	int reset;
	int masterReset;
	int x;
	int y;
	int hp;
	int maxHp;
	int sd;
	int maxSd;
};

struct PMSG_DASHBOARD_PLAYERS_SEND
{
	PSWMSG_HEAD header;
	int count;
};
#pragma pack(pop)

class CCustomEventTime
{
	public:
	void Load(char* path);
	void GCReqEventTime(int Index, PMSG_CUSTOM_EVENTTIME_RECV* pMsg);
	void GCReqDashboardEvents(int Index);
	void GCReqOnlinePlayers(int Index);

	private:
	void Init();
	void SetInfo(CUSTOM_EVENTTIME_INFO info);
	void FillEventData(int n, CUSTOM_EVENTTIME_DATA* info);
	CUSTOM_EVENTTIME_INFO m_CustomEventInfo[MAX_EVENTTIME];
};
extern CCustomEventTime gCustomEventTime;
