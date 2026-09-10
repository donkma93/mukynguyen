#include "stdafx.h"
#include "ThangCuoi\WideData.h"
#include "./Utilities/Log/muConsoleDebug.h"
#include <NewUIOptionWindow.h>
#include <NewUISystem.h>
GetPosWide gPosWide;
float IsScaleWideX;

void GetPosWide::SetupPosScreen(float a)
{
	this->x_fScreenRate_x = a;
	this->x_fScreenRate_y = a;
	int GoAddWidth = WindowWidth / this->x_fScreenRate_y;
	this->x_GetAddPos = GoAddWidth / 2 - 320;	
}

void mPosScreen(int ACuoi)
{
	bool FirstLoad = (GetPrivateProfileIntA("FontConfig", "WindowsSD", 0, "./Config.ini") != 0);
	if (FirstLoad)
	{
		switch (ACuoi)
		{
			case 0:		WindowWidth = 1024;	WindowHeight = 576;		IsScaleWideX = 1.20f;	break;
			case 1:		WindowWidth = 1024;	WindowHeight = 576;		IsScaleWideX = 1.20f;	break;
			case 2:		WindowWidth = 1024;	WindowHeight = 576;		IsScaleWideX = 1.20f;	break;
			case 3:		WindowWidth = 1280;	WindowHeight = 720;		IsScaleWideX = 1.50f;	break;
			case 4:		WindowWidth = 1366;	WindowHeight = 768;		IsScaleWideX = 1.60f;	break;
			case 5:		WindowWidth = 1440;	WindowHeight = 810;		IsScaleWideX = 1.69f;	break;
			case 6:		WindowWidth = 1600;	WindowHeight = 900;		IsScaleWideX = 1.87f;	break;
			case 7:		WindowWidth = 1800;	WindowHeight = 1012;	IsScaleWideX = 2.10f;	break;
			case 8:		WindowWidth = 1920;	WindowHeight = 1012;	IsScaleWideX = 2.10f;	break;
			case 9:		WindowWidth = 2100;	WindowHeight = 1012;	IsScaleWideX = 2.10f;	break;
			case 10:	WindowWidth = 2560;	WindowHeight = 1012;	IsScaleWideX = 2.10f;	break;
			default:	WindowWidth = 1024;	WindowHeight = 576;		IsScaleWideX = 1.20f;	break;
		}
	}
	else
	{
		switch (ACuoi)
		{
			case 0:		WindowWidth = 640;	WindowHeight = 480;		IsScaleWideX = 1.20f;	break;
			case 1:		WindowWidth = 800;	WindowHeight = 600;		IsScaleWideX = 1.25f;	break;
			case 2:		WindowWidth = 1024;	WindowHeight = 768;		IsScaleWideX = 1.60f;	break;
			case 3:		WindowWidth = 1280;	WindowHeight = 720;		IsScaleWideX = 1.50f;	break;
			case 4:		WindowWidth = 1366;	WindowHeight = 768;		IsScaleWideX = 1.60f;	break;
			case 5:		WindowWidth = 1440;	WindowHeight = 810;		IsScaleWideX = 1.69f;	break;
			case 6:		WindowWidth = 1600;	WindowHeight = 900;		IsScaleWideX = 1.87f;	break;
			case 7:		WindowWidth = 1800;	WindowHeight = 1012;	IsScaleWideX = 2.10f;	break;
			case 8:		WindowWidth = 1920;	WindowHeight = 1012;	IsScaleWideX = 2.10f;	break;
			case 9:		WindowWidth = 2100;	WindowHeight = 1012;	IsScaleWideX = 2.10f;	break;
			case 10:	WindowWidth = 2560;	WindowHeight = 1012;	IsScaleWideX = 2.10f;	break;
			default:	WindowWidth = 1024;	WindowHeight = 768;		IsScaleWideX = 1.20f;	break;
		}
	}

	if (ACuoi >= 0)
	{
		gPosWide.SetupPosScreen(IsScaleWideX);
	}
	else
	{
		gPosWide.x_fScreenRate_x = (float)WindowWidth / 640;
		gPosWide.x_fScreenRate_y = (float)WindowHeight / 480;
	}
}