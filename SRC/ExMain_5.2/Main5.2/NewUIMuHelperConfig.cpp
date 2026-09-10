#include "stdafx.h"

#include <algorithm>
#include <vector>
#include <array>

#include "UIControls.h"
#include "NewUISystem.h"
#include "NewUIMuHelperConfig.h"
#include "CharacterManager.h"
#include "MUHelper/MuHelper.h"
#include "SkillManager.h"
#include "ThangCuoi/ZzzToolKit.h"
#include "ThangCuoi/WideData.h"
#include "wsclientinline.h"

using namespace MUHelper;

constexpr int MAX_NUMBER_DIGITS = 3;

ConfigData _TempConfig;

SEASON3B::CNewUIMuHelperConfig::CNewUIMuHelperConfig()
{
    m_pNewUIMng = NULL;
    m_Pos.x = 0;
    m_Pos.y = 0;
    m_iCurrentPage = -1;
    m_iCurrentHealThreshold = 50;
    m_iCurrentPartyHealThreshold = 50;
    m_iCurrentPotionThreshold = 50;
}

SEASON3B::CNewUIMuHelperConfig::~CNewUIMuHelperConfig()
{
    Release();
}

bool SEASON3B::CNewUIMuHelperConfig::Create(CNewUIManager* pNewUIMng, int x, int y)
{
    if (NULL == pNewUIMng)
        return false;

    m_pNewUIMng = pNewUIMng;
    m_pNewUIMng->AddUIObj(INTERFACE_MUHELPER_EXT, this);

    SetPos(x, y);

    LoadImages();

    InitButtons();

    InitText();

    Show(false);

    return true;
}

void SEASON3B::CNewUIMuHelperConfig::Release()
{
    UnloadImages();

    if (m_pNewUIMng)
    {
        m_pNewUIMng->RemoveUIObj(this);
        m_pNewUIMng = NULL;
    }
}

void SEASON3B::CNewUIMuHelperConfig::SetPos(int x, int y)
{
    m_Pos.x = x;
    m_Pos.y = y;
}

void SEASON3B::CNewUIMuHelperConfig::InitText()
{
    m_BuffTimeInput.Init(g_hWnd, 17, 15, MAX_NUMBER_DIGITS, false);
    m_BuffTimeInput.SetTextColor(255, 0, 0, 0);
    m_BuffTimeInput.SetBackColor(255, 255, 255, 255);
    m_BuffTimeInput.SetFont(g_hFont);
    m_BuffTimeInput.SetState(UISTATE_NORMAL);
    m_BuffTimeInput.SetOption(UIOPTION_NUMBERONLY);
}

void SEASON3B::CNewUIMuHelperConfig::InitButtons()
{
    m_BtnPreConHuntRange.CheckBoxImgState(IMAGE_MACROUI_HELPER_OPTIONBUTTON);
    m_BtnPreConHuntRange.CheckBoxInfo(m_Pos.x + 17, m_Pos.y + 78, 15, 15);
    m_BtnPreConHuntRange.ChangeText(GlobalText[3555]);

    m_BtnPreConAttacking.CheckBoxImgState(IMAGE_MACROUI_HELPER_OPTIONBUTTON);
    m_BtnPreConAttacking.CheckBoxInfo(m_Pos.x + 17, m_Pos.y + 93, 15, 15);
    m_BtnPreConAttacking.ChangeText(GlobalText[3556]);

    m_BtnSubConMoreThanTwo.CheckBoxImgState(IMAGE_MACROUI_HELPER_OPTIONBUTTON);
    m_BtnSubConMoreThanTwo.CheckBoxInfo(m_Pos.x + 17, m_Pos.y + 143, 15, 15);
    m_BtnSubConMoreThanTwo.ChangeText(GlobalText[3557]);

    m_BtnSubConMoreThanThree.CheckBoxImgState(IMAGE_MACROUI_HELPER_OPTIONBUTTON);
    m_BtnSubConMoreThanThree.CheckBoxInfo(m_Pos.x + 17, m_Pos.y + 158, 15, 15);
    m_BtnSubConMoreThanThree.ChangeText(GlobalText[3558]);

    m_BtnSubConMoreThanFour.CheckBoxImgState(IMAGE_MACROUI_HELPER_OPTIONBUTTON);
    m_BtnSubConMoreThanFour.CheckBoxInfo(m_Pos.x + 17 + 78, m_Pos.y + 143, 15, 15);
    m_BtnSubConMoreThanFour.ChangeText(GlobalText[3559]);

    m_BtnSubConMoreThanFive.CheckBoxImgState(IMAGE_MACROUI_HELPER_OPTIONBUTTON);
    m_BtnSubConMoreThanFive.CheckBoxInfo(m_Pos.x + 17 + 78, m_Pos.y + 158, 15, 15);
    m_BtnSubConMoreThanFive.ChangeText(GlobalText[3560]);

    m_BtnPartyHeal.CheckBoxImgState(IMAGE_OPTION_BTN_CHECK);
    m_BtnPartyHeal.CheckBoxInfo(m_Pos.x + 17, m_Pos.y + 78, 15, 15);
    m_BtnPartyHeal.ChangeText(GlobalText[3539]);

    m_BtnPartyDuration.CheckBoxImgState(IMAGE_OPTION_BTN_CHECK);
    m_BtnPartyDuration.CheckBoxInfo(m_Pos.x + 17, m_Pos.y + 168, 15, 15);
    m_BtnPartyDuration.ChangeText(GlobalText[3540]);

    m_BtnSave.ChangeButtonImgState(1, IMAGE_IGS_BUTTON, 1, 0, 1);
    m_BtnSave.ChangeButtonInfo(m_Pos.x + 120, m_Pos.y + 388, 52, 26);
    m_BtnSave.ChangeText(GlobalText[3503]);
    m_BtnSave.MoveTextPos(0, -1);
    m_BtnSave.ChangeToolTipText("", TRUE);

    m_BtnReset.ChangeButtonImgState(1, IMAGE_IGS_BUTTON, 1, 0, 1);
    m_BtnReset.ChangeButtonInfo(m_Pos.x + 65, m_Pos.y + 388, 52, 26);
    m_BtnReset.ChangeText(GlobalText[3504]);
    m_BtnReset.MoveTextPos(0, -1);
    m_BtnReset.ChangeToolTipText("", TRUE);

    m_BtnClose.ChangeButtonImgState(1, IMAGE_BASE_WINDOW_BTN_EXIT, 0, 0, 0);
    m_BtnClose.ChangeButtonInfo(m_Pos.x + 20, m_Pos.y + 388, 36, 29);
    m_BtnClose.ChangeText("");
    m_BtnClose.ChangeToolTipText(GlobalText[388], TRUE);
}

bool SEASON3B::CNewUIMuHelperConfig::Render()
{
    EnableAlphaTest();
    glColor4f(1.f, 1.f, 1.f, 1.f);

    DWORD TextColor = g_pRenderText->GetTextColor();

    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetTextColor(HEX_COLOR_WHITE);
    g_pRenderText->SetBgColor(0);

    RenderImage(IMAGE_BASE_WINDOW_BACK, m_Pos.x + 2, m_Pos.y + 3, float(WINDOW_WIDTH - 4), float(WINDOW_HEIGHT - 3));
    RenderImage(IMAGE_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, float(WINDOW_WIDTH), 64.f);
    RenderImage(IMAGE_BASE_WINDOW_LEFT, m_Pos.x, m_Pos.y + 64.f, 21.f, float(WINDOW_HEIGHT) - 64.f - 45.f);
    RenderImage(IMAGE_BASE_WINDOW_RIGHT, m_Pos.x + float(WINDOW_WIDTH) - 21.f, m_Pos.y + 64.f, 21.f, float(WINDOW_HEIGHT) - 64.f - 45.f);
    RenderImage(IMAGE_BASE_WINDOW_BOTTOM, m_Pos.x, m_Pos.y + float(WINDOW_HEIGHT) - 45.f, float(WINDOW_WIDTH), 45.f);

    g_pRenderText->SetFont(g_hFont);

    if (m_iCurrentPage == SUB_PAGE_POTION_CONFIG_ELF)
    {
        g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 13, GlobalText[3553], 190, 0, RT3_SORT_CENTER); // "Auto Recovery"
        RenderBackPane(m_Pos.x + 12, m_Pos.y + 55, 165, 45, GlobalText[3545]); // "Auto Potion"
        RenderHpLevel(m_Pos.x + 32, m_Pos.y + 80, 124.f, 16.f, m_iCurrentPotionThreshold, GlobalText[3547]); // "HP Status"

        RenderBackPane(m_Pos.x + 12, m_Pos.y + 120, 165, 45, GlobalText[3546]); // "Auto Hea"
        RenderHpLevel(m_Pos.x + 32, m_Pos.y + 145, 124.f, 16.f, m_iCurrentHealThreshold, GlobalText[3547]); // "HP Status"
    }
    else if (m_iCurrentPage == SUB_PAGE_POTION_CONFIG_SUMMY)
    {
        g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 13, GlobalText[3553], 190, 0, RT3_SORT_CENTER); // "Auto Recovery"
        RenderBackPane(m_Pos.x + 12, m_Pos.y + 55, 165, 45, GlobalText[3545]); // "Auto Potion"
        RenderHpLevel(m_Pos.x + 32, m_Pos.y + 80, 124.f, 16.f, m_iCurrentPotionThreshold, GlobalText[3547]); // "HP Status"

        RenderBackPane(m_Pos.x + 12, m_Pos.y + 120, 165, 45, GlobalText[3517]); // "Drain Life"
        RenderHpLevel(m_Pos.x + 32, m_Pos.y + 145, 124.f, 16.f, m_iCurrentHealThreshold, GlobalText[3547]); // "HP Status"
    }
    else if (m_iCurrentPage == SUB_PAGE_POTION_CONFIG)
    {
        g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 13, GlobalText[3553], 190, 0, RT3_SORT_CENTER); // "Auto Recovery"
        RenderBackPane(m_Pos.x + 12, m_Pos.y + 55, 165, 45, GlobalText[3545]); // "Auto Potion"

        RenderHpLevel(m_Pos.x + 32, m_Pos.y + 80, 124.f, 16.f, m_iCurrentPotionThreshold, GlobalText[3547]);
    }
    else if (m_iCurrentPage == SUB_PAGE_SKILL2_CONFIG
        || m_iCurrentPage == SUB_PAGE_SKILL3_CONFIG)
    {
        g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 13, GlobalText[3552], 190, 0, RT3_SORT_CENTER); // "Activation Skil"
        RenderBackPane(m_Pos.x + 12, m_Pos.y + 55, 165, 45, GlobalText[3543]);  // "Pre-con"
        m_BtnPreConHuntRange.Render();
        m_BtnPreConAttacking.Render();

        RenderBackPane(m_Pos.x + 12, m_Pos.y + 120, 165, 45, GlobalText[3544]); // "Sub-con"
        m_BtnSubConMoreThanTwo.Render();
        m_BtnSubConMoreThanThree.Render();
        m_BtnSubConMoreThanFour.Render();
        m_BtnSubConMoreThanFive.Render();
    }

    else if (m_iCurrentPage == SUB_PAGE_PARTY_CONFIG)
    {
        g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 13, GlobalText[3554], 190, 0, RT3_SORT_CENTER); // "Party"
        //g_pRenderText->SetTextColor(TextColor);
        RenderBackPane(m_Pos.x + 12, m_Pos.y + 55, 165, 45, GlobalText[3549]); // Buff Support
        m_BtnPartyDuration.Render();
        g_pRenderText->RenderText(m_Pos.x + 50, m_Pos.y + 97, GlobalText[3551], 124, 0, RT3_SORT_LEFT); // "Time Space of Casting Buff"
        RenderImage(IMAGE_MACROUI_HELPER_INPUTNUMBER, m_Pos.x + 125, m_Pos.y + 93, 20, 15);
        m_BuffTimeInput.Render();
        g_pRenderText->RenderText(m_Pos.x + 146, m_Pos.y + 97, "Giây", 124, 0, RT3_SORT_LEFT); // "s"
    }
    else if (m_iCurrentPage == SUB_PAGE_PARTY_CONFIG_ELF)
    {
        g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 13, GlobalText[3554], 190, 0, RT3_SORT_CENTER); // "Party"
        //g_pRenderText->SetTextColor(TextColor);
        RenderBackPane(m_Pos.x + 12, m_Pos.y + 55, 165, 70, GlobalText[3548]); // Heal Support
        m_BtnPartyHeal.Render();
        RenderHpLevel(m_Pos.x + 32, m_Pos.y + 100, 124.f, 16.f, m_iCurrentPartyHealThreshold, GlobalText[3550]); // "HP Status of Party Members"

        RenderBackPane(m_Pos.x + 12, m_Pos.y + 145, 165, 45, GlobalText[3549]); // Buff Support
        m_BtnPartyDuration.Render();
        g_pRenderText->RenderText(m_Pos.x + 50, m_Pos.y + 187, GlobalText[3551], 124, 0, RT3_SORT_LEFT); // "Time Space of Casting Buff"
        RenderImage(IMAGE_MACROUI_HELPER_INPUTNUMBER, m_Pos.x + 125, m_Pos.y + 183, 20, 15);
        m_BuffTimeInput.Render();
        g_pRenderText->RenderText(m_Pos.x + 146, m_Pos.y + 187, "Giây", 124, 0, RT3_SORT_LEFT); // "s"
    }

    m_BtnSave.Render();
    m_BtnReset.Render();
    m_BtnClose.Render();

    DisableAlphaBlend();

    return true;
}

void SEASON3B::CNewUIMuHelperConfig::RenderHpLevel(int x, int y, int width, int height, int level, const char* pszLabel)
{
    RenderImage(IMAGE_OPTION_VOLUME_BACK, x, y, 124.f, 16.f);
    if (level > 0)
    {
        RenderImage(IMAGE_OPTION_VOLUME_COLOR, x, y, 124.f * 0.1f * (level), 16.f);
    }
    g_pRenderText->RenderText(x, y + 18, pszLabel, width, 0, RT3_SORT_CENTER);
}

void SEASON3B::CNewUIMuHelperConfig::RenderBackPane(int x, int y, int width, int height, const char* pszHeader)
{
    DWORD TextColor = g_pRenderText->GetTextColor();
    int headerWidth = 65;

    EnableAlphaTest();
    glColor4f(0.0, 0.0, 0.0, 0.4f);
    RenderColor(x + 3.f, y + 2.f, headerWidth - 7.f, 18.f, 0.0, 0);  // shade for top box
    RenderColor(x + 3.f, y + 2.f + 18.f, width - 7.f, height - 7.f, 0.0, 0);  // shade for bottom box
    EndRenderColor();

    // Top box (tab) without bottom line
    RenderImage(IMAGE_TABLE_TOP_LEFT, x, y, 14.0, 14.0);                                // Top-left corner of the tab
    RenderImage(IMAGE_TABLE_TOP_RIGHT, (x + headerWidth) - 14.f, y, 14.0, 14.0);        // Top-right corner of the tab
    RenderImage(IMAGE_TABLE_TOP_PIXEL, x + 6.f, y, (headerWidth - 12.f), 14.0);         // Top edge of the tab
    RenderImage(IMAGE_TABLE_RIGHT_PIXEL, (x + headerWidth) - 14.f, y + 6.f, 14.0, 14.0); // Right edge of the tab

    // Bottom box without top line
    RenderImage(IMAGE_TABLE_TOP_RIGHT, (x + width) - 14.f, y + 18.f, 14.0, 14.0);       // Main box top-right corner
    RenderImage(IMAGE_TABLE_BOTTOM_LEFT, x, (y + height + 18.f) - 14.f, 14.0, 14.0);    // Main box bottom-left corner
    RenderImage(IMAGE_TABLE_BOTTOM_RIGHT, (x + width) - 14.f, (y + height + 18.f) - 14.f, 14.0, 14.0); // Main box bottom-right corner
    RenderImage(IMAGE_TABLE_TOP_PIXEL, x + 2.f, y + 18.f, (width - 12.f), 14.0);        // Top edge of main box
    RenderImage(IMAGE_TABLE_RIGHT_PIXEL, (x + width) - 14.f, y + 24.f, 14.0, (height - 14.f)); // Right edge of main box
    RenderImage(IMAGE_TABLE_BOTTOM_PIXEL, x + 6.f, (y + height + 18.f) - 14.f, (width - 12.f), 14.0); // Bottom edge of main box

    // Left line to connect top box and bottom box
    RenderImage(IMAGE_TABLE_LEFT_PIXEL, x, y + 6.f, 14.0, (height));             // Connecting left edge

    // Header inside top box
    g_pRenderText->SetTextColor(TextColor);
    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->RenderText(x + 10.f, y + 6.f, pszHeader, headerWidth, 0, RT3_SORT_LEFT);
}

void SEASON3B::CNewUIMuHelperConfig::LoadImages()
{
    LoadBitmap("Interface\\MacroUI\\MacroUI_RangeMinus.tga", IMAGE_MACROUI_HELPER_RAGEMINUS, GL_LINEAR, GL_CLAMP, 1, 0);
    LoadBitmap("Interface\\MacroUI\\MacroUI_OptionButton.tga", IMAGE_MACROUI_HELPER_OPTIONBUTTON, GL_LINEAR, GL_CLAMP, 1, 0);
    LoadBitmap("Interface\\MacroUI\\MacroUI_InputNumber.tga", IMAGE_MACROUI_HELPER_INPUTNUMBER, GL_LINEAR, GL_CLAMP, 1, 0);
    LoadBitmap("Interface\\MacroUI\\MacroUI_InputString.tga", IMAGE_MACROUI_HELPER_INPUTSTRING, GL_LINEAR, GL_CLAMP, 1, 0);
    //--
    LoadBitmap("Interface\\InGameShop\\Ingame_Bt03.tga", IMAGE_IGS_BUTTON, GL_LINEAR, GL_CLAMP, 1, 0);
}

void SEASON3B::CNewUIMuHelperConfig::UnloadImages()
{
    DeleteBitmap(IMAGE_MACROUI_HELPER_RAGEMINUS);
    DeleteBitmap(IMAGE_MACROUI_HELPER_OPTIONBUTTON);
    DeleteBitmap(IMAGE_MACROUI_HELPER_INPUTNUMBER);
    DeleteBitmap(IMAGE_MACROUI_HELPER_INPUTSTRING);
    //--
    DeleteBitmap(IMAGE_IGS_BUTTON);
}

bool SEASON3B::CNewUIMuHelperConfig::Update()
{
    if (IsVisible())
    {
        if (m_iCurrentPage == SUB_PAGE_SKILL2_CONFIG || m_iCurrentPage == SUB_PAGE_SKILL3_CONFIG)
        {
            int iSkillIndex = m_iCurrentPage == SUB_PAGE_SKILL2_CONFIG ? 1 : 2; // SKill 2 : Skill 3

            if (m_BtnPreConHuntRange.UpdateMouseEvent())
            {
                m_BtnPreConHuntRange.RegisterBoxState(true);
                m_BtnPreConAttacking.RegisterBoxState(!m_BtnPreConHuntRange.GetBoxState());

                // Clear other precondition bits and set the bit for "Hunt Range"
                _TempConfig.aiSkillCondition[iSkillIndex] =
                    (_TempConfig.aiSkillCondition[iSkillIndex] & MUHELPER_SKILL_PRECON_CLEAR) |
                    ON_MOBS_NEARBY;
            }
            else if (m_BtnPreConAttacking.UpdateMouseEvent())
            {
                m_BtnPreConAttacking.RegisterBoxState(true);
                m_BtnPreConHuntRange.RegisterBoxState(!m_BtnPreConAttacking.GetBoxState());

                // Clear other precondition bits and set the bit for "Attacking"
                _TempConfig.aiSkillCondition[iSkillIndex] =
                    (_TempConfig.aiSkillCondition[iSkillIndex] & MUHELPER_SKILL_PRECON_CLEAR) |
                    ON_MOBS_ATTACKING;
            }
            else if (m_BtnSubConMoreThanTwo.UpdateMouseEvent())
            {
                m_BtnSubConMoreThanTwo.RegisterBoxState(true);
                m_BtnSubConMoreThanThree.RegisterBoxState(false);
                m_BtnSubConMoreThanFour.RegisterBoxState(false);
                m_BtnSubConMoreThanFive.RegisterBoxState(false);

                // Clear other bits and set the bit for "More Than Two Mobs"
                _TempConfig.aiSkillCondition[iSkillIndex] =
                    (_TempConfig.aiSkillCondition[iSkillIndex] & MUHELPER_SKILL_SUBCON_CLEAR) |
                    ON_MORE_THAN_TWO_MOBS;
            }
            else if (m_BtnSubConMoreThanThree.UpdateMouseEvent())
            {
                m_BtnSubConMoreThanTwo.RegisterBoxState(false);
                m_BtnSubConMoreThanThree.RegisterBoxState(true);
                m_BtnSubConMoreThanFour.RegisterBoxState(false);
                m_BtnSubConMoreThanFive.RegisterBoxState(false);

                // Clear other bits and set the bit for "More Than Three Mobs"
                _TempConfig.aiSkillCondition[iSkillIndex] =
                    (_TempConfig.aiSkillCondition[iSkillIndex] & MUHELPER_SKILL_SUBCON_CLEAR) |
                    ON_MORE_THAN_THREE_MOBS;
            }
            else if (m_BtnSubConMoreThanFour.UpdateMouseEvent())
            {
                m_BtnSubConMoreThanTwo.RegisterBoxState(false);
                m_BtnSubConMoreThanThree.RegisterBoxState(false);
                m_BtnSubConMoreThanFour.RegisterBoxState(true);
                m_BtnSubConMoreThanFive.RegisterBoxState(false);

                // Clear other bits and set the bit for "More Than Four Mobs"
                _TempConfig.aiSkillCondition[iSkillIndex] =
                    (_TempConfig.aiSkillCondition[iSkillIndex] & MUHELPER_SKILL_SUBCON_CLEAR) |
                    ON_MORE_THAN_FOUR_MOBS;
            }
            else if (m_BtnSubConMoreThanFive.UpdateMouseEvent())
            {
                m_BtnSubConMoreThanTwo.RegisterBoxState(false);
                m_BtnSubConMoreThanThree.RegisterBoxState(false);
                m_BtnSubConMoreThanFour.RegisterBoxState(false);
                m_BtnSubConMoreThanFive.RegisterBoxState(true);

                // Clear other bits and set the bit for "More Than Five Mobs"
                _TempConfig.aiSkillCondition[iSkillIndex] =
                    (_TempConfig.aiSkillCondition[iSkillIndex] & MUHELPER_SKILL_SUBCON_CLEAR) |
                    ON_MORE_THAN_FIVE_MOBS;
            }
        }

        if (m_iCurrentPage == SUB_PAGE_PARTY_CONFIG_ELF)
        {
            if (m_BtnPartyHeal.UpdateMouseEvent())
            {
                _TempConfig.bAutoHealParty = m_BtnPartyHeal.GetBoxState();
            }
        }

        if (m_iCurrentPage == SUB_PAGE_PARTY_CONFIG || m_iCurrentPage == SUB_PAGE_PARTY_CONFIG_ELF)
        {
            if (m_BtnPartyDuration.UpdateMouseEvent())
            {
                _TempConfig.bBuffDurationParty = m_BtnPartyDuration.GetBoxState();
            }
        }

        if (m_BtnClose.UpdateMouseEvent())
        {
            g_pNewUISystem->Hide(INTERFACE_MUHELPER_EXT);
        }
        else if (m_BtnSave.UpdateMouseEvent())
        {
            Save();
            g_pNewUISystem->Hide(INTERFACE_MUHELPER_EXT);
        }
        else if (m_BtnReset.UpdateMouseEvent())
        {
            Reset();
        }
    }
    return true;
}

bool SEASON3B::CNewUIMuHelperConfig::UpdateMouseEvent()
{
    // Ignore events outside MU Helper window
    if (!CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        return true;
    }

    if (CheckMouseIn(m_Pos.x + 33 - 8, m_Pos.y + 80, 124 + 8, 16))
    {
        int iOldValue = m_iCurrentPotionThreshold;
        if (MouseWheel > 0)
        {
            MouseWheel = 0;
            m_iCurrentPotionThreshold++;
            if (m_iCurrentPotionThreshold > 10)
            {
                m_iCurrentPotionThreshold = 10;
            }
        }
        else if (MouseWheel < 0)
        {
            MouseWheel = 0;
            m_iCurrentPotionThreshold--;
            if (m_iCurrentPotionThreshold < 0)
            {
                m_iCurrentPotionThreshold = 0;
            }
        }
        if (IsRepeat(VK_LBUTTON))
        {
            int x = MouseX - (m_Pos.x + 33);
            if (x < 0)
            {
                m_iCurrentPotionThreshold = 0;
            }
            else
            {
                float fValue = (10.f * x) / 124.f;
                m_iCurrentPotionThreshold = (int)fValue + 1;
            }
        }
    }

    if (m_iCurrentPage == SUB_PAGE_POTION_CONFIG_ELF || m_iCurrentPage == SUB_PAGE_POTION_CONFIG_SUMMY)
    {
        if (CheckMouseIn(m_Pos.x + 33 - 8, m_Pos.y + 145, 124 + 8, 16))
        {
            int iOldValue = m_iCurrentHealThreshold;
            if (MouseWheel > 0)
            {
                MouseWheel = 0;
                m_iCurrentHealThreshold++;
                if (m_iCurrentHealThreshold > 10)
                {
                    m_iCurrentHealThreshold = 10;
                }
            }
            else if (MouseWheel < 0)
            {
                MouseWheel = 0;
                m_iCurrentHealThreshold--;
                if (m_iCurrentHealThreshold < 0)
                {
                    m_iCurrentHealThreshold = 0;
                }
            }
            if (IsRepeat(VK_LBUTTON))
            {
                int x = MouseX - (m_Pos.x + 33);
                if (x < 0)
                {
                    m_iCurrentHealThreshold = 0;
                }
                else
                {
                    float fValue = (10.f * x) / 124.f;
                    m_iCurrentHealThreshold = (int)fValue + 1;
                }
            }
        }
    }
    else if (m_iCurrentPage == SUB_PAGE_PARTY_CONFIG_ELF)
    {
        if (CheckMouseIn(m_Pos.x + 32 - 8, m_Pos.y + 100, 124 + 8, 16))
        {
            int iOldValue = m_iCurrentPartyHealThreshold;
            if (MouseWheel > 0)
            {
                MouseWheel = 0;
                m_iCurrentPartyHealThreshold++;
                if (m_iCurrentPartyHealThreshold > 10)
                {
                    m_iCurrentPartyHealThreshold = 10;
                }
            }
            else if (MouseWheel < 0)
            {
                MouseWheel = 0;
                m_iCurrentPartyHealThreshold--;
                if (m_iCurrentPartyHealThreshold < 0)
                {
                    m_iCurrentPartyHealThreshold = 0;
                }
            }
            if (IsRepeat(VK_LBUTTON))
            {
                int x = MouseX - (m_Pos.x + 33);
                if (x < 0)
                {
                    m_iCurrentPartyHealThreshold = 0;
                }
                else
                {
                    float fValue = (10.f * x) / 124.f;
                    m_iCurrentPartyHealThreshold = (int)fValue + 1;
                }
            }
        }
        else if (CheckMouseIn(m_BuffTimeInput.GetPosition_x(), m_BuffTimeInput.GetPosition_y(), 20, 15))
        {
            m_BuffTimeInput.GiveFocus();
        }
        else
        {
            SetFocus(g_hWnd);
        }
    }
    else if (m_iCurrentPage == SUB_PAGE_PARTY_CONFIG)
    {
        if (CheckMouseIn(m_BuffTimeInput.GetPosition_x(), m_BuffTimeInput.GetPosition_y(), 20, 15))
        {
            m_BuffTimeInput.GiveFocus();
        }
        else
        {
            SetFocus(g_hWnd);
        }
    }

    return false;
}

bool SEASON3B::CNewUIMuHelperConfig::UpdateKeyEvent()
{
    if (IsVisible())
    {
        if (IsPress(VK_ESCAPE) == true)
        {
            g_pNewUISystem->Hide(INTERFACE_MUHELPER_EXT);
            //PlayBuffer(SOUND_CLICK01);

            return false;
        }
    }
    return true;
}

float SEASON3B::CNewUIMuHelperConfig::GetLayerDepth()
{
    return 3.4;
}

float SEASON3B::CNewUIMuHelperConfig::GetKeyEventOrder()
{
    return 3.4;
}

void SEASON3B::CNewUIMuHelperConfig::Toggle(int iPageId)
{
    int iPrevPage = m_iCurrentPage;
    m_iCurrentPage = iPageId;

    if (IsVisible() && m_iCurrentPage == iPrevPage)
    {
        m_iCurrentPage = -1;
        this->Show(false);
        return;
    }

    if (m_iCurrentPage == SUB_PAGE_SKILL2_CONFIG || m_iCurrentPage == SUB_PAGE_SKILL3_CONFIG)
    {
        int iSkillIndex = m_iCurrentPage == SUB_PAGE_SKILL2_CONFIG ? 1 : 2;
        m_BtnPreConHuntRange.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MOBS_NEARBY);
        m_BtnPreConAttacking.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MOBS_ATTACKING);
        m_BtnSubConMoreThanTwo.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MORE_THAN_TWO_MOBS);
        m_BtnSubConMoreThanThree.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MORE_THAN_THREE_MOBS);
        m_BtnSubConMoreThanFour.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MORE_THAN_FOUR_MOBS);
        m_BtnSubConMoreThanFive.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MORE_THAN_FIVE_MOBS);
    }
    else if (m_iCurrentPage == SUB_PAGE_POTION_CONFIG || m_iCurrentPage == SUB_PAGE_POTION_CONFIG_ELF || m_iCurrentPage == SUB_PAGE_POTION_CONFIG_SUMMY)
    {
        m_iCurrentPotionThreshold = _TempConfig.iPotionThreshold / 10;
        m_iCurrentHealThreshold = _TempConfig.iHealThreshold / 10;
    }
    else if (m_iCurrentPage == SUB_PAGE_PARTY_CONFIG)
    {
        m_BtnPartyDuration.CheckBoxInfo(m_Pos.x + 17, m_Pos.y + 78, 15, 15);
        m_BtnPartyDuration.RegisterBoxState(_TempConfig.bBuffDurationParty);
        m_iCurrentPartyHealThreshold = _TempConfig.iHealPartyThreshold / 10;

        char wsBuffTime[MAX_NUMBER_DIGITS + 1] = { 0 };
        snprintf(wsBuffTime, MAX_NUMBER_DIGITS + 1, "%d", _TempConfig.iBuffCastInterval);
        m_BuffTimeInput.SetText(wsBuffTime);
        m_BuffTimeInput.SetPosition(m_Pos.x + 127, m_Pos.y + 97);
    }
    else if (m_iCurrentPage == SUB_PAGE_PARTY_CONFIG_ELF)
    {
        m_BtnPartyHeal.RegisterBoxState(_TempConfig.bAutoHealParty);

        m_BtnPartyDuration.CheckBoxInfo(m_Pos.x + 17, m_Pos.y + 168, 15, 15);
        m_BtnPartyDuration.RegisterBoxState(_TempConfig.bBuffDurationParty);
        m_iCurrentPartyHealThreshold = _TempConfig.iHealPartyThreshold / 10;

        char wsBuffTime[MAX_NUMBER_DIGITS + 1] = { 0 };
        snprintf(wsBuffTime, MAX_NUMBER_DIGITS + 1, "%d", _TempConfig.iBuffCastInterval);
        m_BuffTimeInput.SetText(wsBuffTime);
        m_BuffTimeInput.SetPosition(m_Pos.x + 127, m_Pos.y + 187);
    }

    this->Show(true);
}

void SEASON3B::CNewUIMuHelperConfig::Save()
{
    char wsNumberInput[MAX_NUMBER_DIGITS + 1]{};

    m_BuffTimeInput.GetText(wsNumberInput, sizeof(wsNumberInput));
    _TempConfig.iBuffCastInterval = CNewUIMuHelper::GetIntFromTextInput(wsNumberInput);

    _TempConfig.iPotionThreshold = m_iCurrentPotionThreshold * 10;
    _TempConfig.iHealThreshold = m_iCurrentHealThreshold * 10;
    _TempConfig.iHealPartyThreshold = m_iCurrentPartyHealThreshold * 10;
}

void SEASON3B::CNewUIMuHelperConfig::ApplySavedConfig()
{
    m_iCurrentPotionThreshold = _TempConfig.iPotionThreshold / 10;
    m_iCurrentHealThreshold = _TempConfig.iHealThreshold / 10;
    m_iCurrentPartyHealThreshold = _TempConfig.iHealPartyThreshold / 10;
}

// Called by the "Initialization" button from the main page
void SEASON3B::CNewUIMuHelperConfig::InitConfig()
{
    _TempConfig.iPotionThreshold = 40;
    _TempConfig.iHealThreshold = 60;
    _TempConfig.iBuffCastInterval = 0;
    _TempConfig.iHealPartyThreshold = 60;
    _TempConfig.bAutoHealParty = false;
    _TempConfig.bBuffDurationParty = false;
}

// Called by the "Initialization" button from the sub page
void SEASON3B::CNewUIMuHelperConfig::Reset()
{
    if (m_iCurrentPage == SUB_PAGE_SKILL2_CONFIG 
        || m_iCurrentPage == SUB_PAGE_SKILL3_CONFIG)
    {
        int iSkillIndex = m_iCurrentPage == SUB_PAGE_SKILL2_CONFIG ? 1 : 2;

        _TempConfig.aiSkillCondition[iSkillIndex] = ON_MOBS_NEARBY | ON_MORE_THAN_TWO_MOBS;
        _TempConfig.aiSkillCondition[iSkillIndex] = ON_MOBS_NEARBY | ON_MORE_THAN_TWO_MOBS;

        m_BtnPreConHuntRange.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MOBS_NEARBY);
        m_BtnPreConAttacking.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MOBS_ATTACKING);
        m_BtnSubConMoreThanTwo.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MORE_THAN_TWO_MOBS);
        m_BtnSubConMoreThanThree.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MORE_THAN_THREE_MOBS);
        m_BtnSubConMoreThanFour.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MORE_THAN_FOUR_MOBS);
        m_BtnSubConMoreThanFive.RegisterBoxState(_TempConfig.aiSkillCondition[iSkillIndex] & ON_MORE_THAN_FIVE_MOBS);
    }
    else if (m_iCurrentPage == SUB_PAGE_POTION_CONFIG 
        || m_iCurrentPage == SUB_PAGE_POTION_CONFIG_ELF 
        || m_iCurrentPage == SUB_PAGE_POTION_CONFIG_SUMMY)
    {
        _TempConfig.iPotionThreshold = 0;
        _TempConfig.iHealThreshold = 0;

        m_iCurrentPotionThreshold = _TempConfig.iPotionThreshold / 10;
        m_iCurrentHealThreshold = _TempConfig.iHealThreshold / 10;
    }
}
