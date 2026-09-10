#include "stdafx.h"
#include "MuHelperData.h"
#include <sstream>
#include "MemScript.h"
#include <ZzzCharacter.h>
#include "MuHelper.h"
#include <NewUISystem.h>

namespace MUHelper
{
	void ConfigDataSerDe::Serialize(const ConfigData& gameData, PRECEIVE_MUHELPER_DATA& netData)
	{
		return;
	}

	int ReadPrivateProfileInt(const char* section, const char* key, int def, const char* path)
	{
		return GetPrivateProfileIntA(section, key, def, path);
	}

    void LoadOfflineConfigINI(MUHelper::ConfigData& data)
    {
        CMemScript* lpMemScript = new CMemScript;

        if (!lpMemScript || !lpMemScript->SetBuffer((char*)".\\Data\\ACuoi\\Config\\Setting\\Config.ini"))
        {
            delete lpMemScript;
            return;
        }

        try
        {
            while (true)
            {
                if (lpMemScript->GetToken() == TOKEN_END)
                    break;

                if (strcmp(lpMemScript->GetString(), "end") == 0)
                    break;

                const char* name = lpMemScript->GetString();

                if (_stricmp(name, Hero->ID) != 0)
                {
                    lpMemScript->SkipLine();
                    continue;
                }

                data.iHuntingRange = lpMemScript->GetAsNumber();
                data.bLongRangeCounterAttack = lpMemScript->GetAsNumber() != 0;
                data.bReturnToOriginalPosition = lpMemScript->GetAsNumber() != 0;
                data.iMaxSecondsAway = lpMemScript->GetAsNumber();

                data.aiSkill[0] = lpMemScript->GetAsNumber();
                data.aiSkill[1] = lpMemScript->GetAsNumber();
                data.aiSkill[2] = lpMemScript->GetAsNumber();

                data.aiSkillCondition[0] = lpMemScript->GetAsNumber();
                data.aiSkillCondition[1] = lpMemScript->GetAsNumber();
                data.aiSkillCondition[2] = lpMemScript->GetAsNumber();

                data.aiSkillInterval[0] = lpMemScript->GetAsNumber();
                data.aiSkillInterval[1] = lpMemScript->GetAsNumber();
                data.aiSkillInterval[2] = lpMemScript->GetAsNumber();

                data.bUseCombo = lpMemScript->GetAsNumber() != 0;

                data.aiBuff[0] = lpMemScript->GetAsNumber();
                data.aiBuff[1] = lpMemScript->GetAsNumber();
                data.aiBuff[2] = lpMemScript->GetAsNumber();

                data.bBuffDuration = lpMemScript->GetAsNumber() != 0;
                data.bBuffDurationParty = lpMemScript->GetAsNumber() != 0;
                data.iBuffCastInterval = lpMemScript->GetAsNumber();

                data.bAutoHeal = lpMemScript->GetAsNumber() != 0;
                data.iHealThreshold = lpMemScript->GetAsNumber();
                data.bSupportParty = lpMemScript->GetAsNumber() != 0;
                data.bAutoHealParty = lpMemScript->GetAsNumber() != 0;
                data.iHealPartyThreshold = lpMemScript->GetAsNumber();

                data.bUseHealPotion = lpMemScript->GetAsNumber() != 0;
                data.iPotionThreshold = lpMemScript->GetAsNumber();

                data.bUseDrainLife = lpMemScript->GetAsNumber() != 0;
                data.bUseDarkRaven = lpMemScript->GetAsNumber() != 0;
                data.iDarkRavenMode = lpMemScript->GetAsNumber();

                data.bRepairItem = lpMemScript->GetAsNumber() != 0;
                data.StartOffline = lpMemScript->GetAsNumber() != 0;
                data.iObtainingRange = lpMemScript->GetAsNumber();
                data.bPickAllItems = lpMemScript->GetAsNumber() != 0;
                data.bPickSelectItems = lpMemScript->GetAsNumber() != 0;
                data.bPickJewel = lpMemScript->GetAsNumber() != 0;

                data.bPickZen = lpMemScript->GetAsNumber() != 0;
                data.bPickAncient = lpMemScript->GetAsNumber() != 0;
                data.bPickExcellent = lpMemScript->GetAsNumber() != 0;
                data.bPickExtraItems = lpMemScript->GetAsNumber() != 0;

                char extraPath[260];
                sprintf(extraPath, ".\\Data\\ACuoi\\Config\\Setting\\%s.ini", Hero->ID);

                FILE* fe = fopen(extraPath, "r");
                if (fe)
                {
                    char buffer[64];
                    while (fgets(buffer, sizeof(buffer), fe))
                    {
                        std::string line = buffer;
                        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
                        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

                        if (!line.empty())
                        {
                            data.aExtraItems.insert(line);
                        }
                    }
                    fclose(fe);
                }

                lpMemScript->SkipLine(); // skip any remaining values beyond 36
                break;
            }
        }
        catch (...)
        {
        }

        delete lpMemScript;
    }


	void ConfigDataSerDe::Deserialize(const PRECEIVE_MUHELPER_DATA& netData, ConfigData& gameData)
	{
        if (SceneFlag == MAIN_SCENE && Hero && Hero->ID[0] != '\0')
        {
            LoadOfflineConfigINI(gameData);
            g_MuHelper.Load(gameData);
            g_pNewUIMuHelper->LoadSavedConfig(gameData);
        }
	}
}
