#include "stdafx.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ServerDisplayer.h"
#include "Util.h"

#include "User.h"
#include "Path.h"
#include "ServerInfo.h"
#include "CustomEventTime.h"
#include "MemScript.h"
#include "Notice.h"
#include "InvasionManager.h"

CCustomEventTime gCustomEventTime;

void CCustomEventTime::GCReqEventTime(int Index, PMSG_CUSTOM_EVENTTIME_RECV* lpMsg)
{
    // The event schedule is public read-only data.  Allow the standalone
    // dashboard to request it directly without a character login.
    if (gServerInfo.m_CustomEventTimeSwitch == 0)
    {
        return;
    }

    BYTE send[4096];
    PMSG_CUSTOM_EVENTTIME_SEND pMsg{};
    pMsg.header.set(0xF3, 0xE8, 0);

    int size = sizeof(pMsg);
    pMsg.count = 0;

    CUSTOM_EVENTTIME_DATA info;

    pMsg.RegLineEvent = 0;

    int invasionCount = 0;

    for (int i = 0; i < 20; ++i)
    {
        if (strlen(gInvasionManager.m_InvasionInfo[i].AlertMessage) > 0)
        {
            invasionCount++;
        }
    }

    pMsg.RegLineEvent = invasionCount + 4;

    //LogAdd(LOG_RED, "RegLineEvent: %d", pMsg.RegLineEvent);

    for (int n = 0; n < 30; ++n)
    {
        info.index = n;

        if (n == 0)       info.time = gServerDisplayer.EventBc;
        else if (n == 1)  info.time = gServerDisplayer.EventDs;
        else if (n == 2)  info.time = gServerDisplayer.EventCc;
        else if (n == 3)  info.time = gServerDisplayer.EventCTCMini;
        else              info.time = gServerDisplayer.EventInvasion[n - 4];

        memcpy(&send[size], &info, sizeof(info));
        size += sizeof(info);
        pMsg.count++;
    }

    pMsg.header.size[0] = SET_NUMBERHB(size);
    pMsg.header.size[1] = SET_NUMBERLB(size);

    memcpy(send, &pMsg, sizeof(pMsg));
    DataSend(Index, send, size);
}
