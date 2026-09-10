#pragma once

// System Include
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <wchar.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <Dbghelp.h>
#include <iostream>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>

#include <stdlib.h>
#include <map>
#include <fstream>
#include <string>

using namespace std;
#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "Shlwapi.lib")

#define MAX_CUSTOM_GLOVES		50
#define MAX_PET_ITEM			300

#define LUC_CHIEN				1
#define MAX_CUSTOM_WING			100
#define MAX_CUSTOM_MONSTERGLOW	100


#define MAX_BUTTON_CREATE		15
#define MAX_CHAR_CREATE			32
#define MAX_TEXT_CREATE			256

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
#define GET_ITEM(x,y)               (((x)*512)+(y))
