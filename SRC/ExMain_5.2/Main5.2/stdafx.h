// stdafx.h : include file for standard system include files,
#pragma once

//warining
#pragma warning( disable : 4067 ) 
#pragma warning( disable : 4786 ) 
#pragma warning( disable : 4800 ) 
#pragma warning( disable : 4996 ) 
#pragma warning( disable : 4244 )
#pragma warning( disable : 4237 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4503 ) 
#pragma warning( disable : 4267 ) 
#pragma warning( disable : 4091 ) 
#pragma warning( disable : 4819 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4702 )
#pragma warning( disable : 4838 )
#pragma warning( disable : 5208 )
//#pragma warning( disable : 4482 )
//#pragma warning( disable : 4700 )
//#pragma warning( disable : 4748 )
//#pragma warning( disable : 4786 )
#pragma warning( disable : 28159 )
#pragma warning( disable : 26812 )

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN	
	
#ifndef _USE_32BIT_TIME_T
	#define _USE_32BIT_TIME_T
#endif //_USE_32BIT_TIME_T
#define ASG_ADD_MAP_KARUTAN
#define ASG_ADD_KARUTAN_MONSTERS
#define ASG_ADD_KARUTAN_NPC

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#define GET_ITEM(x,y) (((x)*512)+(y))
#pragma warning( push, 3 )

#include <windows.h>

//windows
#include <WinSock2.h>
#include <mmsystem.h>
#include <shellapi.h>

//c runtime
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <mbstring.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <conio.h>

#include <string>
#include <list>
#include <map>
#include <deque>
#include <algorithm>
#include <vector>
#include <queue>

#pragma warning( pop )

//opengl
#include <gl/glew.h>
#include <gl/gl.h>

//patch
//winmain
#include "Winmain.h"
#include "Defined_Global.h"

//client
#include "_define.h"
#include "_enum.h"
#include "_types.h"
#include "_struct.h"	
#include "w_WindowMessageHandler.h"
#include "_GlobalFunctions.h"
#include "_TextureIndex.h"	
#include "UIDefaultBase.h"
#include "NewUICommon.h"
#include "./Math/ZzzMathLib.h"
#include "ZzzOpenglUtil.h"

#define ITEM_SECTION	        0
#define ITEM_INDEX		        1
#define ITEM_LEVEL		        2
#define ITEM_MIXINDEX	        3
#define ITEM_COLORSET	        4
#define ITEM_COLORSEND	        5
#define ITEM_WINGEFFECT	        6

#define MAX_BUTTON_CREATE		15
#define MAX_CHAR_CREATE			32
#define MAX_TEXT_CREATE			256

#define MU_REMOVE_CLASS_CHECK	0
#define WIDE					1

#define OFF_MAP_EFFECT			0	//Off effect map

#define OFF_DEBUG_CONSOLE		0
#define NEW_MASTER_SKILL_TREE	1	//Su Dung Master Skill

#define M_CUSTOM_ITEM			1
#define M_CUSTOM_MONSTER		1
#define UPDATE_FPS_LIMIT		1
#define MAX_HWID_KEY			45

#define WinQuestTurn		    1
#define ChangePassTurn		    1
							    
#define Ex700			        0
#define RENDER_ITEM_3D		    0   //Chỉ Admin bật khi sửa màu item