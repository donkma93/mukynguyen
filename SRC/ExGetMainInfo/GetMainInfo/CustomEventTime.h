#pragma once
#define MAX_EVENTTIME 42

struct CUSTOM_EVENT_INFO
{
	int Index;
	char Name[40];
	char Map[40]; 
	char Gate[40];
};

class CEventTimeText
{
	public:
	CEventTimeText();
	void Init();
	void Load(char* path);
	void SetInfo(CUSTOM_EVENT_INFO info);
	CUSTOM_EVENT_INFO m_CustomEventInfo[MAX_EVENTTIME];
};
extern CEventTimeText gEventTimeText;