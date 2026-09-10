#include "stdafx.h"
#include "ItemTooltip.h"
#include "Util.h"
#include "./Utilities/Log/muConsoleDebug.h"

nCInfo GInfo;

typedef char tooltiparray[100];

nCInfo::nCInfo()
{
	this->m_CustomInfo.clear();
	this->m_CustomDescripcionInfo.clear();
}

nCInfo::~nCInfo()
{
}


void nCInfo::loadnInformation(nInformation* info)
{
	for (int n = 0; n < MaxLine; n++)
	{
		if (info[n].ItemIndex < 0 || info[n].ItemIndex > 7680)
			continue;

		this->m_CustomInfo.insert(std::pair<int, nInformation>(info[n].ItemIndex, info[n]));
	}
}

void nCInfo::loadnText(nText* info)
{
	for (int n = 0; n < MaxLine; n++)
	{
		if (info[n].Index < 0 || info[n].Index > MaxLine)
			continue;
		this->m_CustomDescripcionInfo.insert(std::pair<int, nText>(info[n].Index, info[n]));
	}
}


char* nGetTextLine(int TextIndex)
{
	auto it = GInfo.m_CustomDescripcionInfo.find(TextIndex);
	if (it != GInfo.m_CustomDescripcionInfo.end())
	{
		return it->second.Text;
	}
	return "no found";
}

int nCInfo::nInformationOP(int CurrenLine, int ItemSearch)
{
	std::map<int, nInformation>::iterator it = GInfo.m_CustomInfo.find(ItemSearch);

	if (it != GInfo.m_CustomInfo.end())
	{
		extern char TextList[50][100];
		extern int TextListColor[50];
		extern int TextBold[50];

		wsprintf(TextList[CurrenLine], "%s", it->second.OptionName);
		TextListColor[CurrenLine] = it->second.OptionColor;
		TextBold[CurrenLine++] = 1;

		wsprintf(TextList[CurrenLine++], "\n");

		for (int i = 0; i < 15; i++)
		{
			if (it->second.TextIndex[i] == -1)
				continue;

			int index = it->second.TextIndex[i];
			const char* line = nGetTextLine(index);
			wsprintf(TextList[CurrenLine], "%s", line);
			TextListColor[CurrenLine] = it->second.TextColor[i];
			TextBold[CurrenLine++] = 0;
		}
	}

	return CurrenLine;

}


