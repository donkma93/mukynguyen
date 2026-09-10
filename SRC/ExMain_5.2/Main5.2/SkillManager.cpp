// SkillManager.cpp: implementation of the CSkillManager class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillManager.h"
#include "CharacterManager.h"
#include "MapManager.h"
#include "ZzzCharacter.h"
#include "ThangCuoi/CustomPet.h"
#include <ZzzInterface.h>

CSkillManager gSkillManager;
extern bool CheckAttack();

bool CSkillManager::FindHeroSkill(ActionSkillType eSkillType) 
{
	for (int i = 0; i < CharacterAttribute->SkillNumber; ++i)
	{
		if (CharacterAttribute->Skill[i] == eSkillType)
		{
			return true;
		}
	}
	return false;
}

void CSkillManager::GetSkillInformation( int iType, int iLevel, char *lpszName, int *piMana, int *piDistance, int *piSkillMana)
{
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];
	if ( lpszName)
	{
		strcpy( lpszName, p->Name);
	}
	if ( piMana)
	{
		*piMana = p->Mana;
	}
	if ( piDistance)
	{
		*piDistance = p->Distance;
	}
	if (piSkillMana)
	{
		*piSkillMana = p->AbilityGuage;
	}
}

void CSkillManager::GetSkillInformation_Energy(int iType, int *piEnergy)
{
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];

	if(piEnergy)
	{
		if(p->Energy == 0)
		{
			*piEnergy = 0;
		}
		else
		{
			*piEnergy = 20 + (p->Energy*p->Level*4/100);

			if( iType == AT_SKILL_SUMMON_EXPLOSION || iType == AT_SKILL_SUMMON_REQUIEM ){
				*piEnergy = 20 + (p->Energy*p->Level*3/100);
			}

			if( gCharacterManager.GetBaseClass(Hero->Class) == CLASS_KNIGHT ) {
				*piEnergy = 10 + (p->Energy*p->Level*4/100);
			}
		}
	}

}

void CSkillManager::GetSkillInformation_Charisma(int iType, int *piCharisma)
{
	
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];

	if(piCharisma)
	{
		*piCharisma = p->Charisma;
	}

}

void CSkillManager::GetSkillInformation_Damage(int iType, int *piDamage)
{
	
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];

	if(piDamage)
	{
		*piDamage = p->Damage;
	}

}

float CSkillManager::GetSkillDistance(int Index, CHARACTER* c)
{
    auto Distance = (float)( SkillAttribute[Index].Distance );

	if( Index == AT_SKILL_BLOW_UP + 4 ) 
	{
		if(Distance != 3)
			int aaa = 0;
	}

    if(c != NULL)
    {
        if(c->Helper.Type == MODEL_HELPER+4 || gCustomPet.GetInfoPetType(c->Helper.Type - MODEL_ITEM) == 6)
		{
            //Distance += 2; fix khoang cach danh khi cuoi ngua DL aLongfix5)
		}
    }

    return Distance;
}

bool CSkillManager::CheckSkillDelay(int SkillIndex)
{
	int Skill = CharacterAttribute->Skill[SkillIndex];

	//ACuoi chan skill chem xoay
	if (Skill == AT_SKILL_WHEEL)
	{
		int typeL = Hero->Weapon[0].Type;
		int typeR = Hero->Weapon[1].Type;

		bool isLeftStaff = (typeL >= MODEL_STAFF && typeL < MODEL_STAFF + MAX_ITEM_INDEX);
		bool isRightStaff = (typeR >= MODEL_STAFF && typeR < MODEL_STAFF + MAX_ITEM_INDEX);
		bool isLeftSword = (typeL >= MODEL_SWORD && typeL < MODEL_STAFF);

		if (isLeftSword)
		{
			return true;
		}

		if (isLeftStaff || isRightStaff)
		{
			return false;
		}

		return false;
	}

	int Delay = SkillAttribute[Skill].Delay;

	if (!CheckAttack())
	{
		switch (Skill)
		{
			case AT_SKILL_GIANTSWING:
			case AT_SKILL_DRAGON_LOWER:
			case MASTER_SKILL_ADD_CHAIN_DRIVER_IMPROVED:
			case MASTER_SKILL_ADD_CHAIN_DRIVER_ENHANCED:
			case MASTER_SKILL_ADD_DRAGON_LORE_IMPROVED:
			case MASTER_SKILL_ADD_DRAGON_LORE_ENHANCED:
			case AT_SKILL_PHOENIX_SHOT:
			case MASTER_SKILL_ADD_DRAGON_SLAYER_IMPROVED:
			case MASTER_SKILL_ADD_DRAGON_SLAYER_ENHANCED:
			case AT_SKILL_DRAGON_KICK:
				return false;
		}
	}

	if (Skill == AT_SKILL_INFINITY_ARROW || Skill == MASTER_SKILL_ADD_INFINITY_ARROW_IMPROVED)
	{
		return true;
	}

	if (Delay > 0)
	{
		if (CharacterAttribute->SkillDelay[SkillIndex] > 0)
			return false;

		int iCharisma = 0;
		GetSkillInformation_Charisma(Skill, &iCharisma);
		if (iCharisma > (CharacterAttribute->Charisma + CharacterAttribute->AddCharisma))
			return false;

		CharacterAttribute->SkillDelay[SkillIndex] = Delay;

		if (Skill == AT_SKILL_DRAGON_KICK || Skill == AT_SKILL_PHOENIX_SHOT)
		{
			LetHeroStop();
		}
	}

	return true;
}

void CSkillManager::CalcSkillDelay( int time )
{
	int iSkillNumber;
	iSkillNumber = CharacterAttribute->SkillNumber+2;
	iSkillNumber = min(iSkillNumber, MAX_SKILLS);

    for ( int i=0; i<iSkillNumber; ++i )
    {
		if ( CharacterAttribute->SkillDelay[i] <= 0 ) 
			continue;

        CharacterAttribute->SkillDelay[i] -= time;
        if ( CharacterAttribute->SkillDelay[i]<0 )
        {
            CharacterAttribute->SkillDelay[i] = 0;
        }
    }
}

BYTE CSkillManager::GetSkillMasteryType( int iType )
{
    BYTE MasteryType = 255;
    SKILL_ATTRIBUTE* p = &SkillAttribute[iType];

    MasteryType = p->MasteryType;

    return MasteryType;
}

int CSkillManager::MasterSkillToBaseSkillIndex(int iMasterSkillIndex)
{
	int iBaseSkillIndex = 0;
	switch (iMasterSkillIndex)
	{
	case MASTER_SKILL_ADD_EVIL_SPIRIT_IMPROVED1:
	case MASTER_SKILL_ADD_EVIL_SPIRIT_IMPROVED2:
	{
		iBaseSkillIndex = AT_SKILL_EVIL;
	}
	break;
	case MASTER_SKILL_ADD_MANA_SHIELD_IMPROVED:
	case MASTER_SKILL_ADD_MANA_SHIELD_ENHANCED:
	case MASTER_SKILL_ADD_MANA_SHIELD_MASTERED:
	{
		iBaseSkillIndex = AT_SKILL_WIZARDDEFENSE;
	}
	break;
	/*case AT_SKILL_HELL_FIRE_UP:
	case AT_SKILL_HELL_FIRE_UP+1:
	case AT_SKILL_HELL_FIRE_UP+2:
	case AT_SKILL_HELL_FIRE_UP+3:
	case AT_SKILL_HELL_FIRE_UP+4:
	{
		iBaseSkillIndex = AT_SKILL_BLAST_HELL;
	}
	break;*/
	/*case AT_SKILL_ICE_UP:
	case AT_SKILL_ICE_UP + 1:
	case AT_SKILL_ICE_UP + 2:
	case AT_SKILL_ICE_UP+3:
	case AT_SKILL_ICE_UP+4:
	{
		iBaseSkillIndex = AT_SKILL_BLAST_FREEZE;
	}
	break;*/
	/*case AT_SKILL_TORNADO_SWORDA_UP:
	case AT_SKILL_TORNADO_SWORDA_UP + 1:
	case AT_SKILL_TORNADO_SWORDA_UP + 2:
	case AT_SKILL_TORNADO_SWORDA_UP + 3:
	case AT_SKILL_TORNADO_SWORDA_UP + 4:
	case AT_SKILL_TORNADO_SWORDB_UP:
	case AT_SKILL_TORNADO_SWORDB_UP + 1:
	case AT_SKILL_TORNADO_SWORDB_UP + 2:
	case AT_SKILL_TORNADO_SWORDB_UP + 3:
	case AT_SKILL_TORNADO_SWORDB_UP + 4:
	{
		iBaseSkillIndex = AT_SKILL_WHEEL;
	}
	break;*/
	case MASTER_SKILL_ADD_DEATH_STAB_IMPROVED:
	case MASTER_SKILL_ADD_DEATH_STAB_ENHANCED:
	case MASTER_SKILL_ADD_DEATH_STAB_MASTERED:
	{
		iBaseSkillIndex = AT_SKILL_ONETOONE;
	}
	break;
	/*case AT_SKILL_ANGER_SWORD_UP:
	case AT_SKILL_ANGER_SWORD_UP + 1:
	case AT_SKILL_ANGER_SWORD_UP + 2:
	case AT_SKILL_ANGER_SWORD_UP + 3:
	case AT_SKILL_ANGER_SWORD_UP + 4:
	{
		iBaseSkillIndex = AT_SKILL_FURY_STRIKE;
	}
	break;*/
	case MASTER_SKILL_ADD_GREATER_LIFE_IMPROVED:
	case MASTER_SKILL_ADD_GREATER_LIFE_ENHANCED:
	case MASTER_SKILL_ADD_GREATER_LIFE_MASTERED:
	{
		iBaseSkillIndex = AT_SKILL_VITALITY;
	}
	break;
	case MASTER_SKILL_ADD_HEAL_IMPROVED:
	{
		iBaseSkillIndex = AT_SKILL_HEALING;
	}
	break;
	case MASTER_SKILL_ADD_GREATER_DEFENSE_IMPROVED:
	case MASTER_SKILL_ADD_GREATER_DEFENSE_ENHANCED:
	{
		iBaseSkillIndex = AT_SKILL_DEFENSE;
	}
	break;
	case MASTER_SKILL_ADD_GREATER_DAMAGE_IMPROVED:
	case MASTER_SKILL_ADD_GREATER_DAMAGE_ENHANCED:
	{
		iBaseSkillIndex = AT_SKILL_ATTACK;
	}
	break;
	case MASTER_SKILL_ADD_TRIPLE_SHOT_ENHANCED:
	case MASTER_SKILL_ADD_TRIPLE_SHOT_IMPROVED:
	{
		iBaseSkillIndex = AT_SKILL_CROSSBOW;
	}
	break;

	case MASTER_SKILL_ADD_FIRE_SLASH_IMPROVED:
	{
		iBaseSkillIndex = AT_SKILL_REDUCEDEFENSE;
	}
	break;
	case MASTER_SKILL_ADD_POWER_SLASH_IMPROVED:
	{
		iBaseSkillIndex = AT_SKILL_ICE_BLADE;
	}
	break;
	case MASTER_SKILL_ADD_BLAST_IMPROVED1:
	case MASTER_SKILL_ADD_BLAST_IMPROVED2:
	{
		iBaseSkillIndex = AT_SKILL_BLAST;
	}
	break;

	case MASTER_SKILL_ADD_EARTHQUAKE_IMPROVED:
	case MASTER_SKILL_ADD_EARTHQUAKE_ENHANCED:

	{
		iBaseSkillIndex = AT_SKILL_DARK_HORSE;
	}
	break;

	case MASTER_SKILL_ADD_FIRE_BURST_IMPROVED:
	case MASTER_SKILL_ADD_FIRE_BURST_ENHANCED:

	{
		iBaseSkillIndex = AT_SKILL_LONGPIER_ATTACK;
	}
	break;

	case MASTER_SKILL_ADD_FIRE_SCREAM_IMPROVED:
	case MASTER_SKILL_ADD_FIRE_SCREAM_ENHANCED:
	{
		iBaseSkillIndex = AT_SKILL_DARK_SCREAM;
	}
	break;
	case MASTER_SKILL_ADD_SLEEP_IMPROVED:
	{
		iBaseSkillIndex = AT_SKILL_ALICE_SLEEP;
	}
	break;
	case MASTER_SKILL_ADD_CHAIN_LIGHTNING_IMPROVED:
	{
		iBaseSkillIndex = AT_SKILL_ALICE_CHAINLIGHTNING;
	}
	break;
	case MASTER_SKILL_ADD_RED_STORM_IMPROVED:
	{
		iBaseSkillIndex = AT_SKILL_ALICE_LIGHTNINGORB;
	}
	break;

	case MASTER_SKILL_ADD_DRAIN_LIFE_IMPROVED:
	case MASTER_SKILL_ADD_DRAIN_LIFE_ENHANCED:
	{
		iBaseSkillIndex = AT_SKILL_ALICE_DRAINLIFE;
	}
	break;
	default:
	{
		iBaseSkillIndex = iMasterSkillIndex;
	}
	}

	return iBaseSkillIndex;
}

bool CSkillManager::skillVScharactorCheck( const DemendConditionInfo& basicInfo, const DemendConditionInfo& heroInfo )
{
	if( basicInfo <= heroInfo )
	{
		return true;
	}
	return false;
}
	
bool CSkillManager::DemendConditionCheckSkill(WORD SkillType)
{
	if( SkillType >= MAX_SKILLS ) 
	{
		return false;
	}

	if( (true == gMapManager.IsEmpireGuardian()) && (SkillType == AT_SKILL_TELEPORT_B || SkillType == AT_SKILL_TELEPORT) )
	{
		return false;
	}

 	if(SkillAttribute[SkillType].Energy == 0)
 	{
 		return true;
 	}

	SkillType = MasterSkillToBaseSkillIndex(SkillType);

	bool result = true;	
		
	DemendConditionInfo BasicCharacterInfo;
		
	BasicCharacterInfo.SkillLevel     = SkillAttribute[SkillType].Level;
	BasicCharacterInfo.SkillStrength  = SkillAttribute[SkillType].Strength;
	BasicCharacterInfo.SkillDexterity = SkillAttribute[SkillType].Dexterity;
	BasicCharacterInfo.SkillVitality  = 0;
	BasicCharacterInfo.SkillEnergy = (20 + ( SkillAttribute[SkillType].Energy * SkillAttribute[SkillType].Level ) * 0.04);
	BasicCharacterInfo.SkillCharisma = SkillAttribute[SkillType].Charisma;
		
	DemendConditionInfo HeroCharacterInfo;
		
	HeroCharacterInfo.SkillLevel     = CharacterMachine->Character.Level; 
	HeroCharacterInfo.SkillStrength  = CharacterMachine->Character.Strength + CharacterMachine->Character.AddStrength;
	HeroCharacterInfo.SkillDexterity = CharacterMachine->Character.Dexterity + CharacterMachine->Character.AddDexterity; 
	HeroCharacterInfo.SkillVitality  = CharacterMachine->Character.Vitality + CharacterMachine->Character.AddVitality; 
	HeroCharacterInfo.SkillEnergy    = CharacterMachine->Character.Energy + CharacterMachine->Character.AddEnergy; 
	HeroCharacterInfo.SkillCharisma  = CharacterMachine->Character.Charisma + CharacterMachine->Character.AddCharisma;
		
	result = skillVScharactorCheck( BasicCharacterInfo, HeroCharacterInfo );

	return result;	
}

