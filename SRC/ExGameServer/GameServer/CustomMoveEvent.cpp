#include "StdAfx.h"
#include "CustomMoveEvent.h"

CCustomMoveEvent gCustomMoveEvent;

void CCustomMoveEvent::MOVE_EVENT(LPOBJ lpUser, MOVE_EVENT_REQ* lpMsg, int aIndex)
{
    if (lpMsg->MOVE_EVENT >= 1 && lpMsg->MOVE_EVENT <= 41)
    {
        gObjMoveGate(aIndex, 799 + lpMsg->MOVE_EVENT);
    }
}
