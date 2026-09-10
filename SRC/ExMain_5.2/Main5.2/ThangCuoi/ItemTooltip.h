#pragma once
#define MaxLine 2000

#define ITEM_SECTION_SWORD         0  
#define ITEM_SECTION_AXE           1  
#define ITEM_SECTION_MACE          2  
#define ITEM_SECTION_SPEAR         3  
#define ITEM_SECTION_BOW           4  
#define ITEM_SECTION_STAFF         5  
#define ITEM_SECTION_SHIELD        6  
#define ITEM_SECTION_HELM          7  
#define ITEM_SECTION_ARMOR         8  
#define ITEM_SECTION_PANTS         9  
#define ITEM_SECTION_GLOVES        10 
#define ITEM_SECTION_BOOTS         11 
#define ITEM_SECTION_WING          12 
#define ITEM_SECTION_HELPER        13 
#define ITEM_SECTION_EVENT         14 
#define ITEM_SECTION_SPECIAL       15 

struct nInformation
{
	int ItemIndex;
	//--
	char OptionName[32];
	int OptionColor;
	//--
	int TextIndex[15];
	int TextColor[15];
};

struct nText
{
	int Index;
	char Text[254];
};

class nCInfo
{
public:
	nCInfo();
	virtual ~nCInfo();
	void loadnInformation(nInformation * info);
	void loadnText(nText * info);
	int nInformationOP(int CurrenLine, int ItemSearch);
	void IniInformation();
public:
	std::map<int, nInformation> m_CustomInfo;
	std::map<int, nText> m_CustomDescripcionInfo;
};

extern nCInfo GInfo;