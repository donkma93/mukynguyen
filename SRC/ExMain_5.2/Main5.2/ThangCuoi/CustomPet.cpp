#include "stdafx.h"
#include "CustomPet.h"

cCustomPet gCustomPet;

cCustomPet::cCustomPet() {
	Init();
}

cCustomPet::~cCustomPet() {}

void cCustomPet::Init() {
	m_CustomPetInfo.clear();
}

void cCustomPet::Load(CUSTOM_PET_INFO* info) {
	for (int i = 0; i < MAX_PET_ITEM; ++i) {
		if (info[i].Index >= 0 && info[i].Index < MAX_PET_ITEM) {
			m_CustomPetInfo[info[i].ItemIndex] = info[i];
		}
	}
}

int cCustomPet::GetInfoByItem(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end() && it->second.PetType == 6) ? it->second.ItemIndex + MODEL_ITEM : -1;
}

int cCustomPet::GetInfoByItemInventory(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.IndexInventory + MODEL_ITEM : -1;
}

int cCustomPet::GetInfoPetType(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.PetType : -1;
}

int cCustomPet::GetInfoPetEffect(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.FenrrirEffect : 0;
}

float cCustomPet::GetScaleInv(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.Scale_Inve : -1;
}

float cCustomPet::GetScaleDrop(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.Scale_Drop : -1;
}

float cCustomPet::GetScaleView(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.Scale_View : -1;
}

float cCustomPet::GetScalePosX(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.Scale_X : -1;
}

float cCustomPet::GetScalePosY(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.Scale_Y : -1;
}

float cCustomPet::GetScaleRotationX(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.Scale_RotationX : -1;
}

float cCustomPet::GetScaleRotationY(int itemIndex) {
	auto it = m_CustomPetInfo.find(itemIndex);
	return (it != m_CustomPetInfo.end()) ? it->second.Scale_RotationY : -1;
}

bool cCustomPet::CheckCustomPetByItem(int itemIndex) {
	return m_CustomPetInfo.find(itemIndex) != m_CustomPetInfo.end();
}

void cCustomPet::PetFollowCharacter(OBJECT* o, float MaxPos)
{
	float v22;
	vec3_t MUFLOAT;
	vec3_t PetPos;
	vec3_t OwnerPos;
	vec3_t CurrentPos;
	int LootPosition = 1000;

	OwnerPos[0] = o->Owner->Position[0];
	OwnerPos[1] = o->Owner->Position[1];
	OwnerPos[2] = o->Owner->Position[2];

	PetPos[0] = OwnerPos[0] - o->Position[0];
	PetPos[1] = OwnerPos[1] - o->Position[1];
	PetPos[2] = OwnerPos[2] - o->Position[2];

	CurrentPos[0] = o->Owner->Position[0];
	CurrentPos[1] = o->Owner->Position[1];
	CurrentPos[2] = o->Owner->Position[2];

	float PetMapPos = PetPos[0] * PetPos[0] + PetPos[1] * PetPos[1];

	float diff1 = ((OwnerPos[0] > CurrentPos[0]) ? OwnerPos[0] - CurrentPos[0] : CurrentPos[0] - OwnerPos[0]);
	float diff2 = ((OwnerPos[1] > CurrentPos[1]) ? OwnerPos[1] - CurrentPos[1] : CurrentPos[1] - OwnerPos[1]);

	float angle = atan(diff2 / diff1) * 180 / PI;
	float angle2 = atan2(diff1, diff2) * 180 / PI;

	if (PetMapPos >= MaxPos * MaxPos)
	{
		float speed;
		if (PetMapPos >= (MaxPos * 2) * (MaxPos * 2))
		{
			speed = 14.0f;
			speed += 1.0f;
			o->Velocity = 0.89000002f;
		}
		else if (PetMapPos >= (MaxPos + (MaxPos / 2)) * (MaxPos + (MaxPos / 2)))
		{
			speed = 13.0f;
			o->Velocity = 0.69000002f;
		}
		else
		{
			speed = 10.0f;
			o->Velocity = 0.40000001f;
		}

		double MoveX = speed * (cos(angle * PI / 180));
		double MoveY = speed * (sin(angle * PI / 180));

		if (CurrentPos[0] < OwnerPos[0] && CurrentPos[1] < OwnerPos[1])
		{
			o->Position[0] += (float)MoveX;
			o->Position[1] += (float)MoveY;
			o->Angle[2] = 180;
			o->Angle[2] -= angle2;
		}
		else if (CurrentPos[0] < OwnerPos[0] && CurrentPos[1] > OwnerPos[1])
		{
			o->Position[0] += (float)MoveX;
			o->Position[1] -= (float)MoveY;
			o->Angle[2] = angle2;
		}
		else if (CurrentPos[0] > OwnerPos[0] && CurrentPos[1] > OwnerPos[1])
		{
			o->Position[0] -= (float)MoveX;
			o->Position[1] -= (float)MoveY;
			o->Angle[2] = 360;
			o->Angle[2] -= angle2;
		}
		else if (CurrentPos[0] > OwnerPos[0] && CurrentPos[1] < OwnerPos[1])
		{
			o->Position[0] -= (float)MoveX;
			o->Position[1] += (float)MoveY;
			o->Angle[2] = 180;
			o->Angle[2] += angle2;
		}

		if (this->CheckCustomPetByItem(o->Type - MODEL_ITEM))
		{
			o->CurrentAction = 2;
		}
	}
	else
	{
		if (this->CheckCustomPetByItem(o->Type - MODEL_ITEM))
		{
			o->CurrentAction = 1;
		}
	}
}
