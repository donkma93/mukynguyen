#pragma once

char* ConvertModuleFileName(char* name);
char* GetHardwareId();
#define CRASH_APPLICATION_MACRO _asm _emit 0xFF _asm _emit 0x25 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00

extern char* BGetItemName(int ItemType, int Level);