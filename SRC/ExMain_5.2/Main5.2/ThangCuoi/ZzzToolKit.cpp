#include "Stdafx.h"
#include "ZzzToolKit.h"
#include "UIControls.h"
#include "StructSendGs.h"
#include "wsclientinline.h"
#include "MapManager.h"
#include <d3d9.h>
#include "WideData.h"
#include "NewUIInventoryCtrl.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <windows.h> 
#include "..\\..\\Util\\CCRC32.H"
#include <string>

SPK_ToolKitMain IsToolKit; 

void SPK_ToolKitMain::WindowRename()
{
	char WindowName[500];
	#ifdef ENABLE_EDIT
	if (SceneFlag == 5)
	{
		sprintf_s(WindowName, sizeof(WindowName), "%s - %s || Level: %d || Reset: %d/%d || FPS : %.1f || Mouse: %d %d [%d] - MapID: %d [World%d]"
			, gProtect->m_MainInfo.WindowName
			, Hero->ID
			, CharacterAttribute->Level
			, CharacterAttribute->ViewReset
			, CharacterAttribute->ViewMaxReset
			, FPS_AVG
			, MouseX, MouseY, MouseLButtonPush
			, gMapManager.WorldActive
			, gMapManager.WorldActive + 1
		);
		SetWindowText(g_hWnd, WindowName);
	}
	else
	{
		sprintf_s(WindowName, sizeof(WindowName), "%s || FPS : %.1f || Mouse: %d %d [%d] - MapID: %d [World%d]"
			, gProtect->m_MainInfo.WindowName
			, FPS_AVG
			, MouseX, MouseY, MouseLButtonPush
			, gMapManager.WorldActive
			, gMapManager.WorldActive + 1
		);
		SetWindowText(g_hWnd, WindowName);
	}
	#else
	if (SceneFlag == 5)
	{
		sprintf_s(WindowName, sizeof(WindowName), "%s || NV : %s || LV: %d || RS: %d/%d || FPS : %.f || MapID: %d [World%d]"
			, gProtect->m_MainInfo.WindowName
			, Hero->ID
			, CharacterAttribute->Level
			, CharacterAttribute->ViewReset
			, CharacterAttribute->ViewMaxReset
			, FPS_AVG
			, gMapManager.WorldActive
			, gMapManager.WorldActive + 1
		);
		SetWindowText(g_hWnd, WindowName);
	}
	else
	{
		sprintf_s(WindowName, sizeof(WindowName), "%s || FPS : %.f || MapID: %d [World%d]"
			, gProtect->m_MainInfo.WindowName
			, FPS_AVG
			, gMapManager.WorldActive
			, gMapManager.WorldActive + 1
		);
		SetWindowText(g_hWnd, WindowName);
	}
	#endif
}

POINT_F SPK_ToolKitMain::VisiblePos(int index)
{
	char buf[32];
	char keyX[32];
	char keyY[32];
	char keyW[32];
	char keyH[32];

	sprintf_s(keyX, "BODY_X%d", index);
	sprintf_s(keyY, "BODY_Y%d", index);
	sprintf_s(keyW, "BODY_W%d", index);
	sprintf_s(keyH, "BODY_H%d", index);

	GetPrivateProfileStringA("ACuoi", keyX, "0.0", buf, sizeof(buf), "./Config.ini");
	float x = (float)atof(buf);

	GetPrivateProfileStringA("ACuoi", keyY, "0.0", buf, sizeof(buf), "./Config.ini");
	float y = (float)atof(buf);

	GetPrivateProfileStringA("ACuoi", keyW, "0.0", buf, sizeof(buf), "./Config.ini");
	float w = (float)atof(buf);

	GetPrivateProfileStringA("ACuoi", keyH, "0.0", buf, sizeof(buf), "./Config.ini");
	float h = (float)atof(buf);

	return { x, y, w, h };
}

D3DCOLOR SPK_ToolKitMain::VisibleCor(int index)
{
	char buf[128];
	char keyColor[32];

	sprintf_s(keyColor, "COLOR%d", index);
	GetPrivateProfileStringA("ACuoi", keyColor, "255, 255, 255, 255", buf, sizeof(buf), "./Config.ini");

	int r, g, b, a;
	sscanf_s(buf, "%d, %d, %d, %d", &r, &g, &b, &a);

	return D3DCOLOR_RGBA(b, g, r, a);
}
int SPK_ToolKitMain::GetPositionScreen()
{
	int a = 0;
	switch (m_Resolution)
	{
		case 0: a = 854; break;
		case 1: a = 854; break;
		case 2: a = 854; break;
		case 3: a = 854; break;
		case 4: a = 854; break;
		case 5: a = 854; break;
		case 6: a = 854; break;
		case 7: a = 854; break;
		case 8: a = 915; break;
		case 9: a = 995; break;
		case 10: a = 1218; break;
		default:a = 640;
		break;
	}
	return a;
}

int SPK_ToolKitMain::GetCreatePosHeight()
{
	return 430;
}

#define ClassLang 0
char* CharacterCode(int a)
{
	switch (a)
	{
		#if ClassLang
		case 0:  return "Dark Wizard";     
		case 1:  return "Soul Master";     
		case 2:  return "Grand Master";    

		case 16: return "Dark Knight";     
		case 17: return "Blade Knight";    
		case 18: return "Blade Master";    

		case 32: return "Fairy Elf";       
		case 33: return "Muse Elf";        
		case 34: return "High Elf";        

		case 48: return "Magic Gladiator"; 
		case 50: return "Duel Master";     

		case 64: return "Dark Lord";       
		case 66: return "Lord Emperor";    

		case 80: return "Summoner";        
		case 81: return "Bloody Summoner"; 
		case 82: return "Dimension Master";

		case 96: return "Rage Fighter";    
		case 98: return "Fist Master";     
		#else
		case 0:  return "Phù Thủy";
		case 1:  return "Pháp Sư";
		case 2:  return "Thiên Sứ";

		case 16: return "Chiến Binh";
		case 17: return "Kỵ Sĩ";
		case 18: return "Thiên Kiếm";

		case 32: return "Tiên Nữ";
		case 33: return "Thánh Nữ";
		case 34: return "Thiên Nữ";

		case 48: return "Đấu Sĩ";
		case 50: return "Thiên Tướng";

		case 64: return "Chúa Tể";
		case 66: return "Thiên Vương";

		case 80: return "Thuật Sĩ";
		case 81: return "Thuật Sư";
		case 82: return "Phục Ma";

		case 96: return "Thiết Binh";
		case 98: return "Quyền Vương";
		#endif
	}
	return "unknown";
}

void SPK_ToolKitMain::WindowFont()
{
	//ACuoi set font face
	char FontFace[100];
	char FontWeightStr[10];
	char FontHeightStr[50];
	char FontAliasStr[50];

	GetPrivateProfileStringA("FontConfig", "FontName", "Tahoma", FontFace, sizeof(FontFace), "./Config.ini");
	GetPrivateProfileStringA("FontConfig", "FontWeight", "1", FontWeightStr, sizeof(FontWeightStr), "./Config.ini");
	GetPrivateProfileStringA("FontConfig", "FontHeight", "13", FontHeightStr, sizeof(FontHeightStr), "./Config.ini");
	GetPrivateProfileStringA("FontConfig", "FontAlias", "3", FontAliasStr, sizeof(FontAliasStr), "./Config.ini");

	int FontWeight = atoi(FontWeightStr);
	this->FontHeightRead = atoi(FontHeightStr);
	int FontAliasRead = atoi(FontAliasStr);
	int FontW = (FontWeight == 0) ? 100 : 700;

	switch (m_Resolution)
	{
		case 0: FontHeight = 11; FontW = 100; break;
		case 1: FontHeight = 11; FontW = 100; break;
		case 2: FontHeight = 12; FontW = 100; break;
		case 3: FontHeight = this->FontHeightRead; break;
		case 4: FontHeight = this->FontHeightRead; break;
		case 5: FontHeight = this->FontHeightRead; break;
		case 6: FontHeight = this->FontHeightRead; break;
		case 7: FontHeight = this->FontHeightRead; break;
		case 8: FontHeight = this->FontHeightRead; break;
		case 9: FontHeight = this->FontHeightRead; break;
		case 10: FontHeight = this->FontHeightRead; break;
	}
	g_hFont = CreateFont(FontHeight, 0, 0, 0, FontW, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, FontAliasRead, DEFAULT_PITCH | FF_DONTCARE, FontFace);
	g_hFontBig = CreateFont(20, 0, 0, 0, FontW, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, FontAliasRead, DEFAULT_PITCH | FF_DONTCARE, FontFace);

	g_ConsoleDebug->Write(MCD_NORMAL, "Resolution: %d / FontHeight:%d", m_Resolution, FontHeight);
	g_ConsoleDebug->Write(MCD_NORMAL, "Screen: %dx%d", WindowWidth, WindowHeight);
	g_ConsoleDebug->Write(MCD_NORMAL, "WideRate X:%.2f / WideRate Y:%.2f", gPosWide.x_fScreenRate_x, gPosWide.x_fScreenRate_y);

}

char* SPK_ToolKitMain::QN(SPK Number)
{
	if (Number == 0)
	{
		return strdup("0");
	}

	char OutPut[40];
	char Temp[40];

	int Index = 0;
	int CommaCounter = 0;

	while (Number > 0)
	{
		Temp[Index++] = '0' + (int)(Number % 10);
		Number /= 10;

		if (++CommaCounter == 3 && Number > 0)
		{
			Temp[Index++] = ',';
			CommaCounter = 0;
		}
	}

	int OutPutIndex = 0;
	while (Index > 0)
	{
		OutPut[OutPutIndex++] = Temp[--Index];
	}
	OutPut[OutPutIndex] = '\0';

	return strdup(OutPut);
}

void SPK_ToolKitMain::ThisFont(int PosX, int PosY, DWORD color, DWORD bkcolor, int Width, int Height, BYTE Align, LPCTSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (Len <= 0) return; 

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(
		(color >> 24) & 0xFF, // Alpha
		(color >> 16) & 0xFF, // Red
		(color >> 8) & 0xFF,  // Green
		color & 0xFF          // Blue
	);
	g_pRenderText->SetBgColor(
		(bkcolor >> 24) & 0xFF,
		(bkcolor >> 16) & 0xFF,
		(bkcolor >> 8) & 0xFF,
		bkcolor & 0xFF
	);

	char* Context = nullptr;
	char* Line = strtok_s(Buff, "\n", &Context);

	while (Line != NULL)
	{
		g_pRenderText->RenderText(PosX, PosY, Line, Width, Height, Align);
		PosY += Height + 2;
		Line = strtok_s(NULL, "\n", &Context);
	}
}
void SPK_ToolKitMain::ThisFontBig(int PosX, int PosY, DWORD color, DWORD bkcolor, int Width, int Height, BYTE Align, LPCTSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	if (Len <= 0) return; 

	g_pRenderText->SetFont(g_hFontBig);
	g_pRenderText->SetTextColor(
		(color >> 24) & 0xFF, // Alpha
		(color >> 16) & 0xFF, // Red
		(color >> 8) & 0xFF,  // Green
		color & 0xFF          // Blue
	);
	g_pRenderText->SetBgColor(
		(bkcolor >> 24) & 0xFF,
		(bkcolor >> 16) & 0xFF,
		(bkcolor >> 8) & 0xFF,
		bkcolor & 0xFF
	);

	char* Context = nullptr;
	char* Line = strtok_s(Buff, "\n", &Context);

	while (Line != NULL)
	{
		g_pRenderText->RenderText(PosX, PosY, Line, Width, Height, Align);
		PosY += Height + 2;
		Line = strtok_s(NULL, "\n", &Context);
	}
	g_pRenderText->SetFont(g_hFont);
}


bool SPK_ToolKitMain::IsWorkZone(float x, float y, float h, float w)
{
	return MouseX >= x && MouseX <= x + h && MouseY >= y && MouseY <= y + w;
}

void SPK_ToolKitMain::KeySendState(BYTE Folder, BYTE Data)
{
	PMSG_TICKET_SEND pMsg{};
	pMsg.header.set(Folder, Data, sizeof(pMsg));
	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void SPK_ToolKitMain::KeySendStateStore(int Type)
{
	SendOffTradeType pMsg{};
	pMsg.header.set(0xF3, 0xEB, sizeof(pMsg));
	pMsg.Type = Type;
	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void SPK_ToolKitMain::KeySendStateMocNap(DWORD c)
{
	SendRequestAction pMsg{};
	pMsg.header.set(0xD3, 0x9A, sizeof(pMsg));
	pMsg.Action = g_dataclient.DanhSachMocNap[c].IndexMocNap;
	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void SPK_ToolKitMain::KeySendStateViewMocNap(DWORD c)
{
	IsToolKit.ViewIndex = c;
	SendRequestAction pMsg{};
	pMsg.header.set(0xD3, 0x9B, sizeof(pMsg));
	pMsg.Action = g_dataclient.DanhSachMocNap[c].IndexMocNap;
	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void SPK_ToolKitMain::KeySendStateOpenMocNap(DWORD c)
{
	SendRequestAction pMsg{};
	pMsg.header.set(0xD3, 0x9C, sizeof(pMsg));
	pMsg.Action = 1;
	DataSend((LPBYTE)&pMsg, pMsg.header.size);
}

int SPK_ToolKitMain::CheckClass()
{
	return (Hero->Class == 0 || Hero->Class == 1 || Hero->Class == 2 ||
		Hero->Class == 3 || Hero->Class == 4 || Hero->Class == 5 ||
		Hero->Class == 6 || Hero->Class == 8 || Hero->Class == 9 ||
		Hero->Class == 10 || Hero->Class == 13);
}

void SPK_ToolKitMain::RenderItemKit(int x, int y, int a, int b)
{
	EndBitmap();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);
	gluPerspective2(1.f, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();


	RenderItem3D(x, y, 20, 20, a, b, 0, 0, false);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	BeginBitmap();
}

void SPK_ToolKitMain::RenderToolTipExt(float x, float y, float w, float h, int tHi, const char *text, int Enable, int Val, float fVal)
{
	EnableAlphaTest();

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	RenderColor(x - 1, y - 1, w + 1, 1);         
	RenderColor(x - 1, y - 1, 1, h + 1);         
	RenderColor(x - 1 + w + 1, y - 1, 1, h + 1); 
	RenderColor(x - 1, y - 1 + h + 1, w + 2, 1); 

	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	RenderColor(x, y, w, h);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);

	g_pRenderText->SetBgColor(50, 0, 0, 255);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(x, y - 1, text, w, tHi, 3);

	if (Enable == 1)
	{
		char szMessage[128];
		sprintf(szMessage, "%d", Val);
		g_pRenderText->SetBgColor(0, 0, 0, 0);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->RenderText(x, y + h * 0.5f, szMessage, w, tHi, 3);
	}
	else if (Enable == 2)
	{
		char szMessage[128];
		sprintf(szMessage, "%.2f", fVal);
		g_pRenderText->SetBgColor(0, 0, 0, 0);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->RenderText(x, y + h * 0.5f, szMessage, w, tHi, 3);
	}
}

void SPK_ToolKitMain::RenderToolTip(float x, float y, float w, float h)
{
	EnableAlphaTest();

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	RenderColor(x - 1, y - 1, w + 1, 1);
	RenderColor(x - 1, y - 1, 1, h + 1);
	RenderColor(x - 1 + w + 1, y - 1, 1, h + 1);
	RenderColor(x - 1, y - 1 + h + 1, w + 2, 1);

	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	RenderColor(x, y, w, h);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void SPK_ToolKitMain::RenderItemSlot(int X, int Y, int Doc, int Ngang, int Use)
{
	float v6;
	float v7;
	GLfloat red;
	GLfloat green;
	int j;
	int i;

	for (i = 0; i < Doc; ++i)
	{
		for (j = 0; j < Ngang; ++j)
		{
			EnableAlphaTest();
			glEnable(GL_ALPHA_TEST);
			glColor4f(0.3, 0.3, 0.3, 0.60000002);
			v7 = (float)(Y + 20 * i);
			v6 = (float)(X + 20 * j);
			RenderColor(LODWORD(v6), LODWORD(v7), 20.0, 20.0, 0.0, 0);
			glEnable(GL_TEXTURE_2D);
			glColor3f(1.0, 1.0, 1.0);
			green = (float)(Y + 20 * i);
			red = (float)(X + 20 * j);
			SEASON3B::RenderImage(BITMAP_INTERFACE_NEW_INVENTORY_BASE_BEGIN, LODWORD(red), LODWORD(green), 21.0, 21.0);
			glDisable(GL_BLEND);		
			glEnable(GL_ALPHA_TEST);
			DisableAlphaBlend();
			glColor3f(1.f, 1.f, 1.f);
		}
	}
}

DWORD lastToggleTime = 0;
bool isHoverState = true;

void SPK_ToolKitMain::CreateRenderButton(int ID, int Time, float x, float y, float w, float h)
{
	static DWORD lastSendTime = 0;
	char szCmd[64];

	int MaxCoinSend = 1000000000;
	if (pGetCoin.ThisCoin[0] <= MaxCoinSend / 100)
	{
		if (SEASON3B::CheckMouseIn((int)x, (int)y, (int)w, (int)h))
		{
			RenderBitmap(ID, x, y, w, h, 0.f, (64.f / 128.f) * 1, 1.0, 64.f / 128.f, 1, 1, 0);

			if (SEASON3B::IsPress(VK_LBUTTON))
			{
				DWORD currentTime = GetTickCount();

				if (currentTime - lastSendTime < (Time * 1000))
				{
					g_pChatListBox->AddText("", "Wait please...", SEASON3B::TYPE_SYSTEM_MESSAGE);
					return;
				}

				lastSendTime = currentTime;

				if (pGetCoin.ThisCoin[0] <= MaxCoinSend)
				{
					sprintf(szCmd, "/wc %s %d %d", Hero->ID, 1, MaxCoinSend);
					SendChat(szCmd);
				}

				if (pGetCoin.ThisCoin[1] <= MaxCoinSend)
				{
					sprintf(szCmd, "/wc %s %d %d", Hero->ID, 2, MaxCoinSend);
					SendChat(szCmd);
				}

				if (pGetCoin.ThisCoin[2] <= MaxCoinSend)
				{
					sprintf(szCmd, "/wc %s %d %d", Hero->ID, 3, MaxCoinSend);
					SendChat(szCmd);
				}

				if (pGetCoin.ThisCoin[3] <= MaxCoinSend)
				{
					sprintf(szCmd, "/wc %s %d %d", Hero->ID, 4, MaxCoinSend);
					SendChat(szCmd);
				}

				if (pGetCoin.ThisCoin[4] <= MaxCoinSend)
				{
					sprintf(szCmd, "/wc %s %d %d", Hero->ID, 5, MaxCoinSend);
					SendChat(szCmd);
				}

				return;
			}
		}
		else
		{
			DWORD currentTime = GetTickCount();
			if (currentTime - lastToggleTime >= 500)
			{
				isHoverState = !isHoverState;
				lastToggleTime = currentTime;
			}
			if (isHoverState)
			{
				RenderBitmap(ID, x, y, w, h, 0.f, (64.f / 128.f) * 1, 1.0, 64.f / 128.f, 1, 1, 0);
			}
			else
			{
				RenderBitmap(ID, x, y, w, h, 0.f, (64.f / 128.f) * 0, 1.0, 64.f / 128.f, 1, 1, 0);
			}
		}
	}
}

unsigned int CRC32(const char* data, size_t length)
{
	unsigned int crc = 0xFFFFFFFF;
	unsigned int table[256];

	for (unsigned int i = 0; i < 256; ++i)
	{
		unsigned int crc_value = i;
		for (unsigned int j = 8; j > 0; --j)
		{
			if (crc_value & 1)
				crc_value = (crc_value >> 1) ^ 0xEDB88320;
			else
				crc_value >>= 1;
		}
		table[i] = crc_value;
	}

	for (size_t i = 0; i < length; ++i)
	{
		unsigned char byte = data[i];
		crc = (crc >> 8) ^ table[(crc & 0xFF) ^ byte];
	}

	return ~crc;
}

bool CheckFileCRC(const std::string& filename, unsigned int expectedCRC)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file)
	{
		return false;
	}

	std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	if (fileData.empty())
	{
		return false;
	}

	unsigned int calculatedCRC = CRC32(fileData.c_str(), fileData.size());

	if (calculatedCRC != expectedCRC)
	{
		return false;
	}

	return true;
}

std::string xorEncryptDecrypt(const std::string& data, char key) {
	std::string output = data;
	for (size_t i = 0; i < output.size(); ++i) {
		output[i] ^= key; 
	}
	return output;
}

int SPK_ToolKitMain::BmdSPK()
{
	std::string filePath = "\x44\x61\x74\x61\x2F\x50\x6C\x61\x79\x65\x72\x2F\x50\x6C\x61\x79\x65\x72\x2E\x62\x6D\x64";
	unsigned int expectedCRC = 0x7D56FCEB;

	if (!CheckFileCRC(filePath, expectedCRC))
	{
		std::string encodedMessage = "\x18\x52\x5D\x71\x52\x75\x66\x55\x68\x56\x3A\x2C\x66\x57\x51\x1A\x47\x6A\x52\x50\x6E\x2C\x69\x5D";
		std::string decodedMessage = xorEncryptDecrypt(encodedMessage, 0xAA);

		MessageBox(NULL, decodedMessage.c_str(), "ACuoi", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}

	return 0;
}

WORD AdjustedDamage(WORD value)
{
	return (value > 255) ? (255 + (value % 256)) : value;
}
int SafeGetItem(int index)
{
	return CHECK_ITEM(index);
}

bool IsValidPasswordChar(const char* str)
{
	for (int i = 0; str[i] != '\0'; ++i)
	{
		unsigned char c = (unsigned char)str[i];
		if (c < 0x20 || c > 0x7E)
		{
			return false;
		}
	}
	return true;
}

void SPK_ToolKitMain::RenderMixEffect(float x, float y, int w, int h)
{
	EnableAlphaBlend();

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			float fx = x + j * 20 + (rand() % 20);
			float fy = y + i * 20 + (rand() % 20);

			glColor3f((float)(rand() % 6 + 6) * 0.1f, (float)(rand() % 4 + 4) * 0.1f, 0.2f);
			float Rotate = (float)((int)(WorldTime) % 100) * 20.f;
			float Scale = 5.f + (rand() % 10);

			RenderBitmapRotate(BITMAP_SHINY, fx, fy, Scale, Scale, 0);
			RenderBitmapRotate(BITMAP_SHINY, fx, fy, Scale, Scale, Rotate);
			RenderBitmapRotate(BITMAP_SHINY + 1, fx, fy, Scale * 3.f, Scale * 3.f, Rotate);
			RenderBitmapRotate(BITMAP_LIGHT, fx, fy, Scale * 6.f, Scale * 6.f, 0);
		}
	}

	DisableAlphaBlend();
}

bool SPK_ToolKitMain::GetIndexMonster(int index)
{
	switch (index)
	{
		case 750: return true;
		case 751: return true;
		case 752: return true;
		case 753: return true;
		case 754: return true;
		case 755: return true;
		case 756: return true;
	}

	return false;
}

DWORD GetGoldValue(DWORD Gold)
{
	if (Gold <= 99999)
	{
		return 0xFFFFFFFF;
	}
	else if (Gold <= 999999)
	{
		return 0x1DCC2EFF;
	}
	else if (Gold < 7000000)
	{
		return 0xFF6F00FF;
	}
	else
	{
		return 0xFF0000FF;
	}
}

bool IsFilteredWeapon(int WeaponType)
{
	if (WeaponType >= MODEL_SWORD && WeaponType <= MODEL_SHIELD + 512) 
	{
		return true;
	}

	return false;
}


std::string EncryptXOR(const std::string& input)
{
	std::string output = input;
	for (size_t i = 0; i + 1 < output.size(); i += 2)
	{
		output[i] ^= 0xA;
		output[i + 1] ^= 0xF;
	}
	return output;
}