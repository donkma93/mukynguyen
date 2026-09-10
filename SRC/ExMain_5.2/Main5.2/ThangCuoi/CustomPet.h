#pragma once
#include <map>

#define MAX_PET_ITEM 300

struct CUSTOM_PET_INFO {
	int Index;
	int ItemIndex;
	int IndexInventory;
	int PetType;
	float Scale_View;
	float Scale_Drop;
	float Scale_Inve;
	float Scale_X;
	float Scale_Y;
	float Scale_RotationX;
	float Scale_RotationY;
	int FenrrirEffect;
	char ModelName[50];
};

class cCustomPet {
public:
	cCustomPet();
	~cCustomPet();

	void Init();
	void Load(CUSTOM_PET_INFO* info);

	int GetInfoByItem(int itemIndex);
	int GetInfoByItemInventory(int itemIndex);
	int GetInfoPetType(int itemIndex);
	int GetInfoPetEffect(int itemIndex);

	float GetScaleInv(int itemIndex);
	float GetScaleDrop(int itemIndex);
	float GetScaleView(int itemIndex);
	float GetScalePosX(int itemIndex);
	float GetScalePosY(int itemIndex);
	float GetScaleRotationX(int itemIndex);
	float GetScaleRotationY(int itemIndex);

	bool CheckCustomPetByItem(int itemIndex);
	void PetFollowCharacter(OBJECT* o, float maxPos);

	std::map<int, CUSTOM_PET_INFO> m_CustomPetInfo;
};

extern cCustomPet gCustomPet;
