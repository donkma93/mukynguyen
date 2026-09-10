#include "Stdafx.h"
#include "ZzzToolKit.h"

SPK_ToolKitMain IsToolKit; 

char* SPK_ToolKitMain::QN(SPK Number)
{
	if (Number == 0) {
		return strdup("0");
	}

	char OutPut[40];
	char Temp[40];

	int Index = 0;
	int CommaCounter = 0;

	while (Number > 0) {
		Temp[Index++] = '0' + (Number % 10);
		Number /= 10;

		if (++CommaCounter == 3 && Number > 0) {
			Temp[Index++] = ',';
			CommaCounter = 0;
		}
	}

	int OutPutIndex = 0;
	while (Index > 0) {
		OutPut[OutPutIndex++] = Temp[--Index];
	}
	OutPut[OutPutIndex] = '\0';

	return strdup(OutPut);
}

char* MapName[] = {
	"Lorencia",					//0
	"Dungeon",					//1
	"Davias",					//2
	"Noria",					//3
	"LostTower",				//4
	"Exile",					//5
	"Arena",					//6
	"Atlans",					//7
	"Tarkan",					//8
	"Devil Square",				//9
	"Icarus",					//10
	"Blood Castle 1",			//11
	"Blood Castle 2",			//12
	"Blood Castle 3",			//13
	"Blood Castle 4",			//14
	"Blood Castle 5",			//15
	"Blood Castle 6",			//16
	"Blood Castle 7",			//17
	"Chaos Castle 1",			//18
	"Chaos Castle 2",			//19
	"Chaos Castle 3",			//20
	"Chaos Castle 4",			//21
	"Chaos Castle 5",			//22
	"Chaos Castle 6",			//23
	"Kalima 1",					//24
	"Kalima 2",					//25
	"Kalima 3",					//26
	"Kalima 4",					//27
	"Kalima 5",					//28
	"Kalima 6",					//29
	"Valley of Loren",			//30
	"Land of Trials",			//31
	"Devil Square",				//32
	"Aida",						//33
	"Crywolf Fortress",			//34
	"NULL35",					//35
	"Kalima 7",					//36
	"Kantru",					//37
	"Kantru K4",				//38
	"Kantru 3",					//39
	"Silent Map",				//40
	"Doanh Trại Barack",		//41
	"Balgass Refuge",			//42
	"NULL43",					//43
	"NULL44",					//44
	"Illusion Temple 1",		//45
	"Illusion Temple 2",		//46
	"Illusion Temple 3",		//47
	"Illusion Temple 4",		//48
	"Illusion Temple 5",		//49
	"Illusion Temple 6",		//50
	"Elbeland",					//51
	"Blood Castle 8",			//52
	"Chaos Castle 7",			//53
	"NULL54",					//54
	"NULL55",					//55
	"Swamp of Calmness",		//56
	"Raklion",					//57
	"Raklion Boss",				//58
	"NULL59",					//59
	"NULL60",					//60
	"NULL61",					//61
	"Village's Santa",			//62
	"Vulcanus",					//63
	"Duel Arena",				//64
	"Doppelganger 1",			//65
	"Doppelganger 2",			//66
	"Doppelganger 3",			//67
	"Doppelganger 4",			//68
	"Empire Fortress 1",		//69
	"Empire Fortress 2",		//70
	"Empire Fortress 3",		//71
	"Empire Fortress 4",		//72
	"NULL73",					//73
	"NULL74",					//74
	"NULL75",					//75
	"NULL76",					//76
	"NULL77",					//77
	"NULL78",					//78
	"NULL79",					//79
	"Karuntan 1",				//80
	"Karuntan 2",				//81
	"NULL82",					//82
	"NULL83",					//83
	"NULL84",					//84
	"NULL85",					//85
	"Nars",					//86
	"NULL87",					//87
	"NULL88",					//88
	"Acheron",					//89
	"Debanter",					//90
	"NULL91",					//91
	"NULL92",					//92
	"NULL93",					//93
	"NULL94",					//94
	"NULL95",					//95
	"Thần Ma Chiến",			//96
	"Chiến Trường Cổ",			//97
	"NULL98",					//98
	"NULL99",					//99
	"NULL100" };					//100