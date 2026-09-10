#pragma once
#include "User.h"

struct BuffData {
    int IDBuff;
    int Timer;
    int Val1;
    int Val2;
    int Val3;
    int Val4;
};
struct IsReadData {
    BuffData BuffsSection0[32]; 
    BuffData BuffsSection1[32]; 
    int SkillCountSection0;
    int SkillCountSection1;
};

class AddBuffer {
public:
    IsReadData IsReadDataX;
    void AddBuffer::Read(char* FilePath);
    bool CommandAddBuff(LPOBJ lpObj);
    bool CommandDelBuff(LPOBJ lpObj);
};
extern AddBuffer gAddBuffer;