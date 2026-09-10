#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <array>
#include "UIControls.h"
#include "NewUISystem.h"
#include "NewUIMuHelperSkillList.h"
#include "CharacterManager.h"
#include "MUHelper/MuHelper.h"
#include "SkillManager.h"
#include "ThangCuoi/ZzzToolKit.h"
#include "ThangCuoi/WideData.h"
#include "wsclientinline.h"

SEASON3B::CNewUIMuHelperSkillList::CNewUIMuHelperSkillList()
{
    Reset();
}

SEASON3B::CNewUIMuHelperSkillList::~CNewUIMuHelperSkillList()
{
    Release();
}

bool SEASON3B::CNewUIMuHelperSkillList::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
    if (NULL == pNewUIMng)
        return false;

    m_pNewUIMng = pNewUIMng;
    m_pNewUIMng->AddUIObj(INTERFACE_MUHELPER_SKILL_LIST, this);

    m_pNewUI3DRenderMng = pNewUI3DRenderMng;

    LoadImages();

    SkillListPos.x = IsToolKit.GetPositionScreen() - 200;
    SkillListPos.y = 0;

    Show(false);

    return true;
}

void SEASON3B::CNewUIMuHelperSkillList::Release()
{
    if (m_pNewUI3DRenderMng)
    {
        m_pNewUI3DRenderMng->DeleteUI2DEffectObject(UI2DEffectCallback);
    }

    UnloadImages();

    if (m_pNewUIMng)
    {
        m_pNewUIMng->RemoveUIObj(this);
        m_pNewUIMng = NULL;
    }
}

void SEASON3B::CNewUIMuHelperSkillList::Reset()
{
    m_bRenderSkillInfo = false;
    m_iRenderSkillInfoType = 0;
    m_iRenderSkillInfoPosX = 0;
    m_iRenderSkillInfoPosY = 0;

    m_EventState = EVENT_NONE;
}

void SEASON3B::CNewUIMuHelperSkillList::LoadImages()
{
    LoadBitmap("Interface\\newui_skill.jpg", IMAGE_SKILL1, GL_LINEAR);
    LoadBitmap("Interface\\newui_skill2.jpg", IMAGE_SKILL2, GL_LINEAR);
    LoadBitmap("Interface\\newui_command.jpg", IMAGE_COMMAND, GL_LINEAR);
    LoadBitmap("Interface\\newui_skillbox.jpg", IMAGE_SKILLBOX, GL_LINEAR);
	#if Ex700
	LoadBitmap("Interface\\Ex700\\SelectSkill.tga", IMAGE_SKILLBOX_USE, GL_LINEAR);
	#else
	LoadBitmap("Interface\\newui_skillbox2.jpg", IMAGE_SKILLBOX_USE, GL_LINEAR);
	#endif
    LoadBitmap("Interface\\newui_non_skill.jpg", IMAGE_NON_SKILL1, GL_LINEAR);
    LoadBitmap("Interface\\newui_non_skill2.jpg", IMAGE_NON_SKILL2, GL_LINEAR);
    LoadBitmap("Interface\\newui_non_command.jpg", IMAGE_NON_COMMAND, GL_LINEAR);
    LoadBitmap("Interface\\newui_skill3.jpg", IMAGE_SKILL3, GL_LINEAR);
    LoadBitmap("Interface\\newui_non_skill3.jpg", IMAGE_NON_SKILL3, GL_LINEAR);
}

void SEASON3B::CNewUIMuHelperSkillList::UnloadImages()
{
    DeleteBitmap(IMAGE_SKILL1);
    DeleteBitmap(IMAGE_SKILL2);
    DeleteBitmap(IMAGE_COMMAND);
    DeleteBitmap(IMAGE_SKILLBOX);
    DeleteBitmap(IMAGE_SKILLBOX_USE);
    DeleteBitmap(IMAGE_NON_SKILL1);
    DeleteBitmap(IMAGE_NON_SKILL2);
    DeleteBitmap(IMAGE_NON_COMMAND);
    DeleteBitmap(IMAGE_SKILL3);
    DeleteBitmap(IMAGE_NON_SKILL3);
}

bool SEASON3B::CNewUIMuHelperSkillList::UpdateMouseEvent()
{
    if (IsRelease(VK_LBUTTON))
    {
        int skillId = UpdateMouseSkillList();
        if (skillId != -1)
        {
            g_ConsoleDebug->Write(MCD_NORMAL, "[MU Helper] Clicked skill [%d]", skillId);
            g_pNewUIMuHelper->AssignSkill(skillId);
            Show(false);
            return false;
        }

        return true;
    }

    return false;
}

bool SEASON3B::CNewUIMuHelperSkillList::UpdateKeyEvent()
{
    if (IsVisible())
    {
        if (IsPress(VK_ESCAPE) == true)
        {
            g_pNewUISystem->Hide(INTERFACE_MUHELPER_SKILL_LIST);
            SetFocus(g_hWnd);
            //PlayBuffer(SOUND_CLICK01);

            return false;
        }
    }
    return true;
}

void SEASON3B::CNewUIMuHelperSkillList::PrepareSkillsToRender()
{
    m_aiSkillsToRender.clear();
    m_skillIconMap.clear();

    BYTE skillNumber = CharacterAttribute->SkillNumber;
    if (skillNumber > 0)
    {
        for (int i = 0; i < MAX_MAGIC; ++i)
        {
            int iSkillType = CharacterAttribute->Skill[i];

            if (iSkillType != 0 && (iSkillType < AT_SKILL_STUN || iSkillType > AT_SKILL_REMOVAL_BUFF))
            {
                BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;

                if (bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
                {
                    continue;
                }

                if ((m_bFilterByAttackSkills && IsAttackSkill(iSkillType))
                    || (m_bFilterByBuffSkills && IsBuffSkill(iSkillType)))
                {
                    m_aiSkillsToRender.push_back(iSkillType);
                }
            }
        }
    }
}

bool SEASON3B::CNewUIMuHelperSkillList::Update()
{
    for (const auto& [skillId, icon] : m_skillIconMap)
    {
        if (!icon.isVisible)
            continue;

        RECT rc = { icon.location.x, icon.location.y,
                    icon.location.x + icon.area.cx,
                    icon.location.y + icon.area.cy };

        if (PtInRect(&rc, { MouseX, MouseY }))
        {
            m_bRenderSkillInfo = true;


            m_iRenderSkillInfoType = -1;
            for (int i = 0; i < MAX_MAGIC; ++i)
            {
                if (CharacterAttribute->Skill[i] == skillId)
                {
                    m_iRenderSkillInfoType = i;
                    break;
                }
            }

            m_iRenderSkillInfoPosX = icon.location.x;
            m_iRenderSkillInfoPosY = icon.location.y;
            break;
        }
    }
    return true;
}

bool SEASON3B::CNewUIMuHelperSkillList::Render()
{
    float baseX = SkillListPos.x - 22;
    float baseY = m_bFilterByAttackSkills ? 171 : 293;
    float fOrigY = baseY - 150;

    float width = 32.0f;
    float height = 38.0f;
    const int maxSkillsPerColumn = 10;

    int skillIndex = 0;
    m_skillIconMap.clear();

    for (int iSkillType : m_aiSkillsToRender)
    {
        int column = skillIndex / maxSkillsPerColumn;
        int row = skillIndex % maxSkillsPerColumn;

        float x = baseX - column * (width);
        float y = fOrigY + row * height;

        RenderImage(IMAGE_SKILLBOX, x, y, width, height);
        g_pSkillList->RenderSkillIcon(iSkillType, x + 6, y + 6, 20, 28, 1);

        m_skillIconMap[iSkillType] = {
            iSkillType,
            { static_cast<LONG>(x), static_cast<LONG>(y) },
            { static_cast<LONG>(width), static_cast<LONG>(height) },
            true
        };

        skillIndex++;
    }

    if (m_bRenderSkillInfo && m_pNewUI3DRenderMng)
    {
        m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);
        m_bRenderSkillInfo = false;
    }

    return true;
}


void SEASON3B::CNewUIMuHelperSkillList::RenderSkillInfo()
{
    ::RenderSkillInfo(m_iRenderSkillInfoPosX + 15, m_iRenderSkillInfoPosY + 80, m_iRenderSkillInfoType);
}

float SEASON3B::CNewUIMuHelperSkillList::GetLayerDepth()
{
    return 5.2f;
}

void SEASON3B::CNewUIMuHelperSkillList::RenderSkillIcon(int iSkillType, float x, float y, float width, float height)
{
    float fU, fV;
    int iKindofSkill = 0;

    BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;
    int Skill_Icon = SkillAttribute[iSkillType].Magic_Icon;

    if (iSkillType >= AT_PET_COMMAND_DEFAULT && iSkillType <= AT_PET_COMMAND_END)
    {
        fU = ((iSkillType - AT_PET_COMMAND_DEFAULT) % 8) * width / 256.f;
        fV = ((iSkillType - AT_PET_COMMAND_DEFAULT) / 8) * height / 256.f;
        iKindofSkill = KOS_COMMAND;
    }
    else if (iSkillType == AT_SKILL_PLASMA_STORM_FENRIR)
    {
        fU = 4 * width / 256.f;
        fV = 0.f;
        iKindofSkill = KOS_COMMAND;
    }
    else if ((iSkillType >= AT_SKILL_ALICE_DRAINLIFE && iSkillType <= AT_SKILL_ALICE_THORNS))
    {
        fU = ((iSkillType - AT_SKILL_ALICE_DRAINLIFE) % 8) * width / 256.f;
        fV = 3 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType >= AT_SKILL_ALICE_SLEEP && iSkillType <= AT_SKILL_ALICE_BLIND)
    {
        fU = ((iSkillType - AT_SKILL_ALICE_SLEEP + 4) % 8) * width / 256.f;
        fV = 3 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_ALICE_BERSERKER)
    {
        fU = 10 * width / 256.f;
        fV = 3 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType >= AT_SKILL_ALICE_WEAKNESS && iSkillType <= AT_SKILL_ALICE_ENERVATION)
    {
        fU = (iSkillType - AT_SKILL_ALICE_WEAKNESS + 8) * width / 256.f;
        fV = 3 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType >= AT_SKILL_SUMMON_EXPLOSION && iSkillType <= AT_SKILL_SUMMON_REQUIEM)
    {
        fU = ((iSkillType - AT_SKILL_SUMMON_EXPLOSION + 6) % 8) * width / 256.f;
        fV = 3 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_SUMMON_POLLUTION)
    {
        fU = 11 * width / 256.f;
        fV = 3 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_BLOW_OF_DESTRUCTION)
    {
        fU = 7 * width / 256.f;
        fV = 2 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_GAOTIC)
    {
        fU = 3 * width / 256.f;
        fV = 8 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_RECOVER)
    {
        fU = 9 * width / 256.f;
        fV = 2 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_MULTI_SHOT)
    {
        fU = 0 * width / 256.f;
        fV = 8 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_FLAME_STRIKE)
    {
        int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
        int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

        fU = 1 * width / 256.f;
        fV = 8 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_GIGANTIC_STORM)
    {
        fU = 2 * width / 256.f;
        fV = 8 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_LIGHTNING_SHOCK)
    {
        fU = 2 * width / 256.f;
        fV = 3 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (AT_SKILL_LIGHTNING_SHOCK_UP <= iSkillType && iSkillType <= AT_SKILL_LIGHTNING_SHOCK_UP + 4)
    {
        fU = 6 * width / 256.f;
        fV = 8 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType == AT_SKILL_SWELL_OF_MAGICPOWER)
    {
        fU = 8 * width / 256.f;
        fV = 2 * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else if (bySkillUseType == 4)
    {
        fU = (width / 256.f) * (Skill_Icon % 12);
        fV = (height / 256.f) * ((Skill_Icon / 12) + 4);
        iKindofSkill = KOS_SKILL2;
    }
    else if (iSkillType >= AT_SKILL_THRUST)
    {
        fU = ((iSkillType - 260) % 12) * width / 256.f;
        fV = ((iSkillType - 260) / 12) * height / 256.f;
        iKindofSkill = KOS_SKILL3;
    }
    else if (iSkillType >= 57)
    {
        fU = ((iSkillType - 57) % 8) * width / 256.f;
        fV = ((iSkillType - 57) / 8) * height / 256.f;
        iKindofSkill = KOS_SKILL2;
    }
    else
    {
        fU = ((iSkillType - 1) % 8) * width / 256.f;
        fV = ((iSkillType - 1) / 8) * height / 256.f;
        iKindofSkill = KOS_SKILL1;
    }
    int iTextureId = 0;
    switch (iKindofSkill)
    {
    case KOS_COMMAND:
    {
        iTextureId = IMAGE_COMMAND;
    }break;
    case KOS_SKILL1:
    {
        iTextureId = IMAGE_SKILL1;
    }break;
    case KOS_SKILL2:
    {
        iTextureId = IMAGE_SKILL2;
    }break;
    case KOS_SKILL3:
    {
        iTextureId = IMAGE_SKILL3;
    }break;
    }

    if (iTextureId != 0)
    {
        RenderBitmap(iTextureId, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
    }
}

bool SEASON3B::CNewUIMuHelperSkillList::IsAttackSkill(int iSkillType)
{
    if (IsBuffSkill(iSkillType))
    {
        return false;
    }

    if (IsDefenseSkill(iSkillType))
    {
        return false;
    }

    if (IsHealingSkill(iSkillType))
    {
        return false;
    }

    return true;
}

bool SEASON3B::CNewUIMuHelperSkillList::IsBuffSkill(int iSkillType)
{
    switch (iSkillType)
    {
            // DK SPK Fix Skill
        case AT_SKILL_VITALITY:
        case MASTER_SKILL_ADD_GREATER_LIFE_IMPROVED:
        case MASTER_SKILL_ADD_GREATER_LIFE_ENHANCED:
            return true;

            // DW => SPK OK ( 2 buff )
        case AT_SKILL_WIZARDDEFENSE:
        case MASTER_SKILL_ADD_MANA_SHIELD_IMPROVED:
        case MASTER_SKILL_ADD_MANA_SHIELD_ENHANCED:

        case AT_SKILL_SWELL_OF_MAGICPOWER:
        case MASTER_SKILL_ADD_MAGIC_CIRCLE_IMPROVED:
        case MASTER_SKILL_ADD_MAGIC_CIRCLE_ENHANCED:
            return true;

            // Elf buffs
        case AT_SKILL_INFINITY_ARROW:
        case MASTER_SKILL_ADD_INFINITY_ARROW_IMPROVED:

        case AT_SKILL_DEFENSE:
        case MASTER_SKILL_ADD_GREATER_DEFENSE_IMPROVED:
        case MASTER_SKILL_ADD_GREATER_DEFENSE_ENHANCED:

        case AT_SKILL_HEALING:
        case MASTER_SKILL_ADD_HEAL_IMPROVED:

        case AT_SKILL_ATTACK:
        case MASTER_SKILL_ADD_GREATER_DAMAGE_IMPROVED:
        case MASTER_SKILL_ADD_GREATER_DAMAGE_ENHANCED:
            return true;

        // DL buffs
        case AT_SKILL_ADD_CRITICAL:
        case MASTER_SKILL_ADD_GREATER_CRITICAL_DAMAGE_IMPROVED:
        case MASTER_SKILL_ADD_GREATER_CRITICAL_DAMAGE_ENHANCED:
        return true;

            // SM buffs
        case AT_SKILL_ALICE_BERSERKER:
        case MASTER_SKILL_ADD_SWORD_POWER_IMPROVED:
        case MASTER_SKILL_ADD_SWORD_POWER_ENHANCED:
        case MASTER_SKILL_ADD_SWORD_POWER_MASTERED:

        case AT_SKILL_ALICE_THORNS:
            return true;

            // RF Buff
        case AT_SKILL_ATT_UP_OURFORCES:

        case AT_SKILL_HP_UP_OURFORCES:
        case AT_SKILL_DEF_UP_OURFORCES:
        case MASTER_SKILL_ADD_GREATER_DEFENSE_SUCCESS_RATE_IMPROVED:
        case MASTER_SKILL_ADD_GREATER_DEFENSE_SUCCESS_RATE_ENHANCED:
        case MASTER_SKILL_ADD_FITNESS_IMPROVED:
        return true;
    }

    return false;
}

bool SEASON3B::CNewUIMuHelperSkillList::IsHealingSkill(int iSkillType)
{
    // To-do: Complete list of healing skills

    switch (iSkillType)
    {
        case AT_SKILL_HEALING:
        case AT_SKILL_ALICE_DRAINLIFE:
        return true;
    }

    return false;
}

bool SEASON3B::CNewUIMuHelperSkillList::IsDefenseSkill(int iSkillType)
{
    switch (iSkillType)
    {
    case AT_SKILL_DEFENSE:
        return true;
    }

    return false;
}

void SEASON3B::CNewUIMuHelperSkillList::FilterByAttackSkills()
{
    m_bFilterByAttackSkills = true;
    m_bFilterByBuffSkills = false;

    PrepareSkillsToRender();
}

void SEASON3B::CNewUIMuHelperSkillList::FilterByBuffSkills()
{
    m_bFilterByBuffSkills = true;
    m_bFilterByAttackSkills = false;

    PrepareSkillsToRender();
}

void SEASON3B::CNewUIMuHelperSkillList::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
    if (auto* pThis = reinterpret_cast<CNewUIMuHelperSkillList*>(pClass))
    {
        pThis->RenderSkillInfo();
    }
}

int SEASON3B::CNewUIMuHelperSkillList::UpdateMouseSkillList()
{
    auto li = m_skillIconMap.begin();

    for (; li != m_skillIconMap.end(); li++)
    {
        cSkillIcon* pIcon = &li->second;

        if (CheckMouseIn(pIcon->location.x, pIcon->location.y, pIcon->area.cx, pIcon->area.cy))
        {
            return li->first;
        }
    }

    return -1;
}
