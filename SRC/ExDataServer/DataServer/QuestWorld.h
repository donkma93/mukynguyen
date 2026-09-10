#pragma once
#include "DataServerProtocol.h"
#define MAX_QUEST_WORLD_LIST 20

struct SDHP_QUEST_WORLD_RECV
{
	PSBMSG_HEAD header; // C1:10:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_QUEST_WORLD_SAVE_RECV
{
	PSWMSG_HEAD header; // C1:10:30
	WORD index;
	char account[11];
	char name[11];
	BYTE QuestWorldList[MAX_QUEST_WORLD_LIST][20];
};

struct SDHP_QUEST_WORLD_SEND
{
	PSWMSG_HEAD header; // C1:10:00
	WORD index;
	char account[11];
	char name[11];
	BYTE QuestWorldList[MAX_QUEST_WORLD_LIST][20];
};

class CQuestWorld
{
	public:
	void GDQuestWorldRecv(SDHP_QUEST_WORLD_RECV* lpMsg,int index);
	void GDQuestWorldSaveRecv(SDHP_QUEST_WORLD_SAVE_RECV* lpMsg);
};

extern CQuestWorld gQuestWorld;
