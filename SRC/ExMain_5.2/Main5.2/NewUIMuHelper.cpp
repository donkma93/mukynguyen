#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <array>
#include "UIControls.h"
#include "NewUISystem.h"
#include "NewUIMuHelper.h"
#include "CharacterManager.h"
#include "MUHelper/MuHelper.h"
#include "SkillManager.h"
#include "ThangCuoi/ZzzToolKit.h"
#include "ThangCuoi/WideData.h"
#include "wsclientinline.h"

using namespace MUHelper;
extern ConfigData _TempConfig;

constexpr int BITMAP_DISTANCE_BEGIN = BITMAP_INTERFACE_CRYWOLF_BEGIN + 33;
constexpr int MAX_NUMBER_DIGITS = 3;
constexpr int MAX_HUNTING_RANGE = 6;
constexpr int MAX_OBTAINING_RANGE = 8;

SEASON3B::CNewUIMuHelper::CNewUIMuHelper()
{
    m_pNewUIMng = NULL;
    m_Pos.x = m_Pos.y = 0;
    m_ButtonList.clear();

    m_iCurrentOpenTab = 0;

    m_iSelectedSkillSlot = 0;
    m_aiSelectedSkills.fill(-1);

    m_iCurrentOpenSubWin = 0;
    m_bSubWinOpen = false;
}

SEASON3B::CNewUIMuHelper::~CNewUIMuHelper()
{
    Release();
}

bool SEASON3B::CNewUIMuHelper::Create(CNewUIManager* pNewUIMng, int x, int y)
{
    if (NULL == pNewUIMng)
        return false;

    m_pNewUIMng = pNewUIMng;
    m_pNewUIMng->AddUIObj(INTERFACE_MUHELPER, this);

    SetPos(x, y);

    LoadImages();

    InitButtons();

    InitCheckBox();

    InitImage();

    InitText();

    InitTextboxInput();

    Show(false);

    return true;
}

void SEASON3B::CNewUIMuHelper::Release()
{
    UnloadImages();

    if (m_pNewUIMng)
    {
        m_pNewUIMng->RemoveUIObj(this);
        m_pNewUIMng = NULL;
    }
}

void SEASON3B::CNewUIMuHelper::SetPos(int x, int y)
{
    m_Pos.x = x;
    m_Pos.y = y;
}

void SEASON3B::CNewUIMuHelper::InitButtons()
{
    std::list<unicode::t_string> ltext;
    ltext.push_back(GlobalText[3500]);
    ltext.push_back(GlobalText[3501]);
    ltext.push_back(GlobalText[3590]);

    m_TabBtn.CreateRadioGroup(2, IMAGE_WINDOW_TAB_BTN, TRUE);
    m_TabBtn.ChangeRadioText(ltext);
    m_TabBtn.ChangeRadioButtonInfo(true, m_Pos.x + 10.f, m_Pos.y + 48.f, 56, 22);
    m_TabBtn.ChangeFrame(m_iCurrentOpenTab);

    InsertButton(IMAGE_CHAINFO_BTN_STAT, m_Pos.x + 56, m_Pos.y + 78, 16, 15, 0, 0, 0, 0, "", "", BUTTON_ID_HUNT_RANGE_ADD, 0);
    InsertButton(IMAGE_MACROUI_HELPER_RAGEMINUS, m_Pos.x + 56, m_Pos.y + 97, 16, 15, 0, 0, 0, 0, "", "", BUTTON_ID_HUNT_RANGE_MINUS, 0);
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 132, m_Pos.y + 191, 38, 24, 1, 0, 1, 1, GlobalText[3502], "", BUTTON_ID_SKILL2_CONFIG, 0); //-- skill 2
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 132, m_Pos.y + 243, 38, 24, 1, 0, 1, 1, GlobalText[3502], "", BUTTON_ID_SKILL3_CONFIG, 0); //-- skill 3
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 132, m_Pos.y + 84, 38, 24, 1, 0, 1, 1, GlobalText[3502], "", BUTTON_ID_POTION_CONFIG_ELF, 0); //-- Buff
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 132, m_Pos.y + 79, 38, 24, 1, 0, 1, 1, GlobalText[3502], "", BUTTON_ID_POTION_CONFIG_SUMMY, 0); //-- potion
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 132, m_Pos.y + 84, 38, 24, 1, 0, 1, 1, GlobalText[3502], "", BUTTON_ID_POTION_CONFIG, 0); //-- potion
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 17, m_Pos.y + 234, 38, 24, 1, 0, 1, 1, GlobalText[3502], "", BUTTON_ID_PARTY_CONFIG, 0); //-- potion
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 17, m_Pos.y + 234, 38, 24, 1, 0, 1, 1, GlobalText[3502], "", BUTTON_ID_PARTY_CONFIG_ELF, 0); //-- potion

    InsertButton(IMAGE_CHAINFO_BTN_STAT, m_Pos.x + 56, m_Pos.y + 78, 16, 15, 0, 0, 0, 0, "", "", BUTTON_ID_PICK_RANGE_ADD, 1);
    InsertButton(IMAGE_MACROUI_HELPER_RAGEMINUS, m_Pos.x + 56, m_Pos.y + 97, 16, 15, 0, 0, 0, 0, "", "", BUTTON_ID_PICK_RANGE_MINUS, 1);
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 132, m_Pos.y + 208, 38, 24, 1, 0, 1, 1, GlobalText[3505], "", BUTTON_ID_ADD_OTHER_ITEM, 1); //-- Buff
    InsertButton(IMAGE_CLEARNESS_BTN, m_Pos.x + 132, m_Pos.y + 309, 38, 24, 1, 0, 1, 1, GlobalText[3506], "", BUTTON_ID_DELETE_OTHER_ITEM, 1); //-- Buff
    //--
    InsertButton(IMAGE_IGS_BUTTON, m_Pos.x + 120, m_Pos.y + 388, 52, 26, 1, 0, 1, 1, GlobalText[3503], "", BUTTON_ID_SAVE_CONFIG, -1);
    InsertButton(IMAGE_IGS_BUTTON, m_Pos.x + 65, m_Pos.y + 388, 52, 26, 1, 0, 1, 1, GlobalText[3504], "", BUTTON_ID_INIT_CONFIG, -1);
    InsertButton(IMAGE_BASE_WINDOW_BTN_EXIT, m_Pos.x + 20, m_Pos.y + 388, 36, 29, 0, 0, 0, 0, "", GlobalText[388], BUTTON_ID_EXIT_CONFIG, -1);

    RegisterBtnCharacter(0xFF, BUTTON_ID_HUNT_RANGE_ADD);
    RegisterBtnCharacter(0xFF, BUTTON_ID_HUNT_RANGE_MINUS);
    RegisterBtnCharacter(0xFF, BUTTON_ID_SKILL2_CONFIG);
    RegisterBtnCharacter(0xFF, BUTTON_ID_PICK_RANGE_ADD);
    RegisterBtnCharacter(0xFF, BUTTON_ID_PICK_RANGE_MINUS);
    RegisterBtnCharacter(0xFF, BUTTON_ID_ADD_OTHER_ITEM);
    RegisterBtnCharacter(0xFF, BUTTON_ID_DELETE_OTHER_ITEM);
    RegisterBtnCharacter(0xFF, BUTTON_ID_SAVE_CONFIG);
    RegisterBtnCharacter(0xFF, BUTTON_ID_INIT_CONFIG);
    RegisterBtnCharacter(0xFF, BUTTON_ID_EXIT_CONFIG);

    RegisterBtnCharacter(DK, BUTTON_ID_SKILL3_CONFIG);
    RegisterBtnCharacter(DK, BUTTON_ID_POTION_CONFIG);

    RegisterBtnCharacter(DW, BUTTON_ID_SKILL3_CONFIG);
    RegisterBtnCharacter(DW, BUTTON_ID_POTION_CONFIG);
    RegisterBtnCharacter(DW, BUTTON_ID_PARTY_CONFIG);

    RegisterBtnCharacter(MG, BUTTON_ID_SKILL3_CONFIG);
    RegisterBtnCharacter(MG, BUTTON_ID_POTION_CONFIG);
    RegisterBtnCharacter(DL, BUTTON_ID_POTION_CONFIG);

    RegisterBtnCharacter(RF, BUTTON_ID_SKILL3_CONFIG);
    RegisterBtnCharacter(RF, BUTTON_ID_POTION_CONFIG);

    RegisterBtnCharacter(FE, BUTTON_ID_SKILL3_CONFIG);
    RegisterBtnCharacter(FE, BUTTON_ID_POTION_CONFIG_ELF);
    RegisterBtnCharacter(FE, BUTTON_ID_PARTY_CONFIG_ELF);

    RegisterBtnCharacter(SM, BUTTON_ID_SKILL3_CONFIG);
    RegisterBtnCharacter(SM, BUTTON_ID_POTION_CONFIG_SUMMY);
}

void SEASON3B::CNewUIMuHelper::InitCheckBox()
{
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 79, m_Pos.y + 80, 15, 15, 0, GlobalText[3507], CHECKBOX_ID_POTION, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 122, 15, 15, 0, GlobalText[3508], CHECKBOX_ID_LONG_DISTANCE, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 137, 15, 15, 0, GlobalText[3509], CHECKBOX_ID_ORIG_POSITION, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 94, m_Pos.y + 174, 15, 15, 0, GlobalText[3510], CHECKBOX_ID_SKILL2_DELAY, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 94, m_Pos.y + 191, 15, 15, 0, GlobalText[3511], CHECKBOX_ID_SKILL2_CONDITION, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 94, m_Pos.y + 226, 15, 15, 0, GlobalText[3510], CHECKBOX_ID_SKILL3_DELAY, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 94, m_Pos.y + 243, 15, 15, 0, GlobalText[3511], CHECKBOX_ID_SKILL3_CONDITION, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 226, 15, 15, 0, GlobalText[3512], CHECKBOX_ID_COMBO, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 276, 15, 15, 0, GlobalText[3513], CHECKBOX_ID_BUFF_DURATION, 0);

    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 218, 15, 15, 0, GlobalText[3514], CHECKBOX_ID_USE_PET, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 218, 15, 15, 0, GlobalText[3515], CHECKBOX_ID_PARTY, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 79, m_Pos.y + 97, 15, 15, 0, GlobalText[3516], CHECKBOX_ID_AUTO_HEAL, 0);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 79, m_Pos.y + 97, 15, 15, 0, GlobalText[3517], CHECKBOX_ID_DRAIN_LIFE, 0);

    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 79, m_Pos.y + 80, 15, 15, 0, GlobalText[3518], CHECKBOX_ID_REPAIR_ITEM, 1);
    //InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 79, m_Pos.y + 100, 15, 15, 0, "Treo", CHECKBOX_ID_START_OFFLINE, 1);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 17, m_Pos.y + 125, 15, 15, 0, GlobalText[3519], CHECKBOX_ID_PICK_ALL, 1);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 17, m_Pos.y + 152, 15, 15, 0, GlobalText[3520], CHECKBOX_ID_PICK_SELECTED, 1);

    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 22, m_Pos.y + 170, 15, 15, 0, GlobalText[3521], CHECKBOX_ID_PICK_JEWEL, 1);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 85, m_Pos.y + 170, 15, 15, 0, GlobalText[3522], CHECKBOX_ID_PICK_ANCIENT, 1);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 22, m_Pos.y + 185, 15, 15, 0, GlobalText[3523], CHECKBOX_ID_PICK_ZEN, 1);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 85, m_Pos.y + 185, 15, 15, 0, GlobalText[3524], CHECKBOX_ID_PICK_EXCELLENT, 1);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 22, m_Pos.y + 200, 15, 15, 0, GlobalText[3525], CHECKBOX_ID_ADD_OTHER_ITEM, 1);
    //--

    InsertCheckBox(IMAGE_MACROUI_HELPER_OPTIONBUTTON, m_Pos.x + 94, m_Pos.y + 235, 15, 15, 0, GlobalText[3533], CHECKBOX_ID_DR_ATTACK_CEASE, 0);
    InsertCheckBox(IMAGE_MACROUI_HELPER_OPTIONBUTTON, m_Pos.x + 30, m_Pos.y + 235, 15, 15, 0, GlobalText[3534], CHECKBOX_ID_DR_ATTACK_AUTO, 0);
    InsertCheckBox(IMAGE_MACROUI_HELPER_OPTIONBUTTON, m_Pos.x + 30, m_Pos.y + 250, 15, 15, 0, GlobalText[3535], CHECKBOX_ID_DR_ATTACK_TOGETHER, 0);

    //--
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 80, 15, 15, 0, GlobalText[3591], CHECKBOX_ID_AUTO_ACCEPT_FRIEND, 2);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 97, 15, 15, 0, GlobalText[3592], CHECKBOX_ID_AUTO_ACCEPT_GUILD, 2);
    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 125, 15, 15, 0, GlobalText[3593], CHECKBOX_ID_AUTO_DEFEND, 2);

    InsertCheckBox(IMAGE_CHECKBOX_BTN, m_Pos.x + 18, m_Pos.y + 142, 15, 15, 0, "Auto Reset", CHECKBOX_ID_AUTO_RESET, 2);

    RegisterBoxCharacter(0xFF, CHECKBOX_ID_POTION);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_LONG_DISTANCE);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_ORIG_POSITION);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_SKILL2_DELAY);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_SKILL2_CONDITION);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_BUFF_DURATION);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_REPAIR_ITEM);
   //RegisterBoxCharacter(0xFF, CHECKBOX_ID_START_OFFLINE);

    RegisterBoxCharacter(0xFF, CHECKBOX_ID_PICK_ALL);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_PICK_SELECTED);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_PICK_JEWEL);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_PICK_ANCIENT);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_PICK_ZEN);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_PICK_EXCELLENT);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_ADD_OTHER_ITEM);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_AUTO_ACCEPT_FRIEND);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_AUTO_DEFEND);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_AUTO_ACCEPT_GUILD);
    RegisterBoxCharacter(0xFF, CHECKBOX_ID_AUTO_RESET);

    RegisterBoxCharacter(DK, CHECKBOX_ID_SKILL3_DELAY);
    RegisterBoxCharacter(DK, CHECKBOX_ID_SKILL3_CONDITION);
    RegisterBoxCharacter(DK, CHECKBOX_ID_COMBO);

    RegisterBoxCharacter(DW, CHECKBOX_ID_SKILL3_DELAY);
    RegisterBoxCharacter(DW, CHECKBOX_ID_SKILL3_CONDITION);
    RegisterBoxCharacter(DW, CHECKBOX_ID_PARTY);

    RegisterBoxCharacter(MG, CHECKBOX_ID_SKILL3_DELAY);
    RegisterBoxCharacter(MG, CHECKBOX_ID_SKILL3_CONDITION);

    RegisterBoxCharacter(DL, CHECKBOX_ID_USE_PET);
    RegisterBoxCharacter(DL, CHECKBOX_ID_DR_ATTACK_CEASE);
    RegisterBoxCharacter(DL, CHECKBOX_ID_DR_ATTACK_AUTO);
    RegisterBoxCharacter(DL, CHECKBOX_ID_DR_ATTACK_TOGETHER);

    RegisterBoxCharacter(FE, CHECKBOX_ID_AUTO_HEAL);
    RegisterBoxCharacter(FE, CHECKBOX_ID_SKILL3_DELAY);
    RegisterBoxCharacter(FE, CHECKBOX_ID_SKILL3_CONDITION);
    RegisterBoxCharacter(FE, CHECKBOX_ID_PARTY);

    RegisterBoxCharacter(SM, CHECKBOX_ID_SKILL3_DELAY);
    RegisterBoxCharacter(SM, CHECKBOX_ID_SKILL3_CONDITION);
    RegisterBoxCharacter(SM, CHECKBOX_ID_DRAIN_LIFE);

    RegisterBoxCharacter(RF, CHECKBOX_ID_SKILL3_DELAY);
    RegisterBoxCharacter(RF, CHECKBOX_ID_SKILL3_CONDITION);
}

void SEASON3B::CNewUIMuHelper::InitImage()
{
    InsertIcon(BITMAP_INTERFACE_NEW_SKILLICON_BEGIN + 4, m_Pos.x + 17, m_Pos.y + 171, 32, 38, SKILL_SLOT_SKILL1, 0);
    InsertIcon(BITMAP_INTERFACE_NEW_SKILLICON_BEGIN + 4, m_Pos.x + 61, m_Pos.y + 171, 32, 38, SKILL_SLOT_SKILL2, 0);
    InsertIcon(BITMAP_INTERFACE_NEW_SKILLICON_BEGIN + 4, m_Pos.x + 61, m_Pos.y + 222, 32, 38, SKILL_SLOT_SKILL3, 0);
    InsertIcon(BITMAP_INTERFACE_NEW_SKILLICON_BEGIN + 4, m_Pos.x + 21, m_Pos.y + 293, 32, 38, SKILL_SLOT_BUFF1, 0);
    InsertIcon(BITMAP_INTERFACE_NEW_SKILLICON_BEGIN + 4, m_Pos.x + 55, m_Pos.y + 293, 32, 38, SKILL_SLOT_BUFF2, 0);
    InsertIcon(BITMAP_INTERFACE_NEW_SKILLICON_BEGIN + 4, m_Pos.x + 89, m_Pos.y + 293, 32, 38, SKILL_SLOT_BUFF3, 0);

    InsertIcon(IMAGE_MACROUI_HELPER_INPUTNUMBER, m_Pos.x + 140, m_Pos.y + 137, 20, 15, TEXTBOX_IMG_DISTANCE_TIME, 0);
    InsertIcon(IMAGE_MACROUI_HELPER_INPUTNUMBER, m_Pos.x + 140, m_Pos.y + 174, 20, 15, TEXTBOX_IMG_SKILL1_TIME, 0);
    InsertIcon(IMAGE_MACROUI_HELPER_INPUTNUMBER, m_Pos.x + 140, m_Pos.y + 226, 20, 15, TEXTBOX_IMG_SKILL2_TIME, 0);
    InsertIcon(IMAGE_MACROUI_HELPER_INPUTSTRING, m_Pos.x + 34, m_Pos.y + 216, 94, 15, TEXTBOX_IMG_ADD_EXTRA_ITEM, 1);

    RegisterIconCharacter(0xFF, SKILL_SLOT_SKILL1);
    RegisterIconCharacter(0xFF, SKILL_SLOT_SKILL2);
    RegisterIconCharacter(0xFF, SKILL_SLOT_BUFF1);
    RegisterIconCharacter(0xFF, SKILL_SLOT_BUFF2);
    RegisterIconCharacter(0xFF, SKILL_SLOT_BUFF3);
    RegisterIconCharacter(0xFF, TEXTBOX_IMG_DISTANCE_TIME);
    RegisterIconCharacter(0xFF, TEXTBOX_IMG_SKILL1_TIME);
    RegisterIconCharacter(0xFF, TEXTBOX_IMG_ADD_EXTRA_ITEM);

    RegisterIconCharacter(DK, SKILL_SLOT_SKILL3);
    RegisterIconCharacter(DK, TEXTBOX_IMG_SKILL2_TIME);
    RegisterIconCharacter(DW, SKILL_SLOT_SKILL3);
    RegisterIconCharacter(DW, TEXTBOX_IMG_SKILL2_TIME);
    RegisterIconCharacter(FE, SKILL_SLOT_SKILL3);
    RegisterIconCharacter(FE, TEXTBOX_IMG_SKILL2_TIME);
    RegisterIconCharacter(MG, SKILL_SLOT_SKILL3);
    RegisterIconCharacter(MG, TEXTBOX_IMG_SKILL2_TIME);
    RegisterIconCharacter(SM, SKILL_SLOT_SKILL3);
    RegisterIconCharacter(SM, TEXTBOX_IMG_SKILL2_TIME);
    RegisterIconCharacter(RF, SKILL_SLOT_SKILL3);
    RegisterIconCharacter(RF, TEXTBOX_IMG_SKILL2_TIME);
}

void SEASON3B::CNewUIMuHelper::InitText()
{
    InsertText(m_Pos.x + 18, m_Pos.y + 78, GlobalText[3526], 1, 0); // Range
    InsertText(m_Pos.x + 127, m_Pos.y + 141, GlobalText[3527], 3, 0); // Distance
    InsertText(m_Pos.x + 162, m_Pos.y + 141, "Giây", 4, 0);

    InsertText(m_Pos.x + 18, m_Pos.y + 160, GlobalText[3529], 5, 0); // Basic Skill
    InsertText(m_Pos.x + 66, m_Pos.y + 160, GlobalText[3530], 7, 0); // Activation Skill 1
    InsertText(m_Pos.x + 162, m_Pos.y + 178, "Giây", 8, 0);
    InsertText(m_Pos.x + 66, m_Pos.y + 212, GlobalText[3531], 9, 0); // Activation Skill 2

    InsertText(m_Pos.x + 162, m_Pos.y + 230, "Giây", 10, 0);
    InsertText(m_Pos.x + 18, m_Pos.y + 78, GlobalText[3532], 11, 1); // Range

    RegisterTextCharacter(0xFF, 1);
    RegisterTextCharacter(0xFF, 2);
    RegisterTextCharacter(0xFF, 3);
    RegisterTextCharacter(0xFF, 4);
    RegisterTextCharacter(0xFF, 5);
    RegisterTextCharacter(0xFF, 7);
    RegisterTextCharacter(0xFF, 8);
    RegisterTextCharacter(0xFF, 11);
    RegisterTextCharacter(0xFF, 12);

    RegisterTextCharacter(DK, 9);
    RegisterTextCharacter(DK, 10);
    RegisterTextCharacter(DW, 9);
    RegisterTextCharacter(DW, 10);
    RegisterTextCharacter(FE, 9);
    RegisterTextCharacter(FE, 10);
    RegisterTextCharacter(MG, 9);
    RegisterTextCharacter(MG, 10);
    RegisterTextCharacter(SM, 9);
    RegisterTextCharacter(SM, 10);
    RegisterTextCharacter(RF, 9);
    RegisterTextCharacter(RF, 10);
}

void SEASON3B::CNewUIMuHelper::InitTextboxInput()
{
    char wsInitText[MAX_NUMBER_DIGITS + 1];

    m_DistanceTimeInput.Init(g_hWnd, 17, 15, MAX_NUMBER_DIGITS, false);
    m_DistanceTimeInput.SetPosition(m_Pos.x + 142, m_Pos.y + 140);
    m_DistanceTimeInput.SetTextColor(255, 0, 0, 0);
    m_DistanceTimeInput.SetBackColor(255, 255, 255, 255);
    m_DistanceTimeInput.SetFont(g_hFont);
    m_DistanceTimeInput.SetState(UISTATE_NORMAL);
    m_DistanceTimeInput.SetOption(UIOPTION_NUMBERONLY);
    snprintf(wsInitText, MAX_NUMBER_DIGITS + 1, "%d", _TempConfig.iMaxSecondsAway);
    m_DistanceTimeInput.SetText(wsInitText);

    m_Skill2DelayInput.Init(g_hWnd, 17, 15, MAX_NUMBER_DIGITS, false);
    m_Skill2DelayInput.SetPosition(m_Pos.x + 142, m_Pos.y + 177);
    m_Skill2DelayInput.SetTextColor(255, 0, 0, 0);
    m_Skill2DelayInput.SetBackColor(255, 255, 255, 255);
    m_Skill2DelayInput.SetFont(g_hFont);
    m_Skill2DelayInput.SetState(UISTATE_NORMAL);
    m_Skill2DelayInput.SetOption(UIOPTION_NUMBERONLY);
    snprintf(wsInitText, MAX_NUMBER_DIGITS + 1, "%d", _TempConfig.aiSkillInterval[1]);
    m_Skill2DelayInput.SetText(wsInitText);

    m_Skill3DelayInput.Init(g_hWnd, 17, 15, MAX_NUMBER_DIGITS, false);
    m_Skill3DelayInput.SetPosition(m_Pos.x + 142, m_Pos.y + 229);
    m_Skill3DelayInput.SetTextColor(255, 0, 0, 0);
    m_Skill3DelayInput.SetBackColor(255, 255, 255, 255);
    m_Skill3DelayInput.SetFont(g_hFont);
    m_Skill3DelayInput.SetState(UISTATE_NORMAL);
    m_Skill3DelayInput.SetOption(UIOPTION_NUMBERONLY);
    snprintf(wsInitText, MAX_NUMBER_DIGITS + 1, "%d", _TempConfig.aiSkillInterval[2]);
    m_Skill3DelayInput.SetText(wsInitText);

    m_ItemInput.Init(g_hWnd, 88, 15, MAX_ITEM_NAME, false);
    m_ItemInput.SetPosition(m_Pos.x + 36, m_Pos.y + 219);
    m_ItemInput.SetTextColor(255, 0, 0, 0);
    m_ItemInput.SetBackColor(255, 255, 255, 255);
    m_ItemInput.SetFont(g_hFont);
    m_ItemInput.SetState(UISTATE_HIDE);

    m_ItemFilter.SetSize(160, 70);
    m_ItemFilter.SetPosition(m_Pos.x + 20, m_Pos.y + 238 + m_ItemFilter.GetHeight());
}

bool SEASON3B::CNewUIMuHelper::Update()
{
    if (IsVisible())
    {
        int iNumCurOpenTab = m_TabBtn.UpdateMouseEvent();

        if (iNumCurOpenTab == RADIOGROUPEVENT_NONE)
            return true;

        m_iCurrentOpenTab = iNumCurOpenTab;

        if (m_iCurrentOpenTab == 0)
        {
            m_DistanceTimeInput.SetState(UISTATE_NORMAL);
            m_Skill2DelayInput.SetState(UISTATE_NORMAL);
            m_Skill3DelayInput.SetState(UISTATE_NORMAL);
            m_ItemInput.SetState(UISTATE_HIDE);

            m_DistanceTimeInput.GiveFocus();
        }
        else if (m_iCurrentOpenTab == 1)
        {
            m_DistanceTimeInput.SetState(UISTATE_HIDE);
            m_Skill2DelayInput.SetState(UISTATE_HIDE);
            m_Skill3DelayInput.SetState(UISTATE_HIDE);
            m_ItemInput.SetState(UISTATE_NORMAL);

            m_ItemInput.GiveFocus();
        }
    }
    return true;
}

bool SEASON3B::CNewUIMuHelper::UpdateMouseEvent()
{
    if (!CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        return true;
    }

    int iButtonId = UpdateMouseBtnList();
    if (iButtonId != -1)
    {
        g_ConsoleDebug->Write(MCD_NORMAL, "[MU Helper] Clicked [%d]", iButtonId);

        if (iButtonId == BUTTON_ID_HUNT_RANGE_ADD)
        {
            ApplyHuntRangeUpdate(1);
        }
        else if (iButtonId == BUTTON_ID_HUNT_RANGE_MINUS)
        {
            ApplyHuntRangeUpdate(-1);
        }
        else if (iButtonId == BUTTON_ID_PICK_RANGE_ADD)
        {
            ApplyLootRangeUpdate(1);
        }
        else if (iButtonId == BUTTON_ID_PICK_RANGE_MINUS)
        {
            ApplyLootRangeUpdate(-1);
        }
        else if (iButtonId == BUTTON_ID_ADD_OTHER_ITEM)
        {
            SaveExtraItem();
        }
        else if (iButtonId == BUTTON_ID_DELETE_OTHER_ITEM)
        {
            RemoveExtraItem();
        }
        else if (iButtonId == BUTTON_ID_SKILL2_CONFIG)
        {
            m_CheckBoxList[CHECKBOX_ID_SKILL2_DELAY].box->RegisterBoxState(false);
            m_CheckBoxList[CHECKBOX_ID_SKILL2_CONDITION].box->RegisterBoxState(true);
            ApplyConfigFromCheckbox(CHECKBOX_ID_SKILL2_CONDITION, true);
            g_pNewUIMuHelperExt->Toggle(SUB_PAGE_SKILL2_CONFIG);
        }
        else if (iButtonId == BUTTON_ID_SKILL3_CONFIG)
        {
            m_CheckBoxList[CHECKBOX_ID_SKILL3_DELAY].box->RegisterBoxState(false);
            m_CheckBoxList[CHECKBOX_ID_SKILL3_CONDITION].box->RegisterBoxState(true);
            ApplyConfigFromCheckbox(CHECKBOX_ID_SKILL3_CONDITION, true);
            g_pNewUIMuHelperExt->Toggle(SUB_PAGE_SKILL3_CONFIG);
        }
        else if (iButtonId == BUTTON_ID_POTION_CONFIG_ELF)
        {
            g_pNewUIMuHelperExt->Toggle(SUB_PAGE_POTION_CONFIG_ELF);
        }
        else if (iButtonId == BUTTON_ID_POTION_CONFIG_SUMMY)
        {
            g_pNewUIMuHelperExt->Toggle(SUB_PAGE_POTION_CONFIG_SUMMY);
        }
        else if (iButtonId == BUTTON_ID_POTION_CONFIG)
        {
            g_pNewUIMuHelperExt->Toggle(SUB_PAGE_POTION_CONFIG);
        }
        else if (iButtonId == BUTTON_ID_PARTY_CONFIG)
        {
            g_pNewUIMuHelperExt->Toggle(SUB_PAGE_PARTY_CONFIG);
        }
        else if (iButtonId == BUTTON_ID_PARTY_CONFIG_ELF)
        {
            g_pNewUIMuHelperExt->Toggle(SUB_PAGE_PARTY_CONFIG_ELF);
        }
        else if (iButtonId == BUTTON_ID_EXIT_CONFIG)
        {
            g_pNewUISystem->Hide(INTERFACE_MUHELPER);
            SetFocus(g_hWnd);
        }
        else if (iButtonId == BUTTON_ID_INIT_CONFIG)
        {
            InitConfig();
        }
        else if (iButtonId == BUTTON_ID_SAVE_CONFIG)
        {
            //SaveConfig();
            AllDataOfflineSave();
            g_pNewUISystem->Hide(INTERFACE_MUHELPER);
            SetFocus(g_hWnd);
        }

        return false;
    }

    int iCheckboxId = UpdateMouseBoxList();
    if (iCheckboxId != -1)
    {
        auto element = m_CheckBoxList[iCheckboxId];
        auto state = element.box->GetBoxState();
        g_ConsoleDebug->Write(MCD_NORMAL, "[MU Helper] Clicked checkbox [%d] state[%d]", iCheckboxId, state);

        if (iCheckboxId == CHECKBOX_ID_SKILL2_DELAY)
        {
            bool bState = m_CheckBoxList[CHECKBOX_ID_SKILL2_DELAY].box->GetBoxState();
            if (bState == true)
            {
                m_CheckBoxList[CHECKBOX_ID_SKILL2_CONDITION].box->RegisterBoxState(false);
                g_pNewUISystem->Hide(INTERFACE_MUHELPER_EXT);
            }
        }
        else if (iCheckboxId == CHECKBOX_ID_SKILL2_CONDITION)
        {
            bool bState = m_CheckBoxList[CHECKBOX_ID_SKILL2_CONDITION].box->GetBoxState();
            if (bState == true)
            {
                m_CheckBoxList[CHECKBOX_ID_SKILL2_DELAY].box->RegisterBoxState(false);
            }
            else
            {
                g_pNewUISystem->Hide(INTERFACE_MUHELPER_EXT);
            }
        }
        else if (iCheckboxId == CHECKBOX_ID_SKILL3_DELAY)
        {
            bool bState = m_CheckBoxList[CHECKBOX_ID_SKILL3_DELAY].box->GetBoxState();
            if (bState == true)
            {
                m_CheckBoxList[CHECKBOX_ID_SKILL3_CONDITION].box->RegisterBoxState(false);
                g_pNewUISystem->Hide(INTERFACE_MUHELPER_EXT);
            }
        }
        else if (iCheckboxId == CHECKBOX_ID_SKILL3_CONDITION)
        {
            bool bState = m_CheckBoxList[CHECKBOX_ID_SKILL3_CONDITION].box->GetBoxState();
            if (bState == true)
            {
                m_CheckBoxList[CHECKBOX_ID_SKILL3_DELAY].box->RegisterBoxState(false);
            }
            else
            {
                g_pNewUISystem->Hide(INTERFACE_MUHELPER_EXT);
            }
        }
        else if (iCheckboxId == CHECKBOX_ID_DR_ATTACK_CEASE)
        {
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_CEASE].box->RegisterBoxState(true);
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_AUTO].box->RegisterBoxState(false);
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_TOGETHER].box->RegisterBoxState(false);
        }
        else if (iCheckboxId == CHECKBOX_ID_DR_ATTACK_AUTO)
        {
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_AUTO].box->RegisterBoxState(true);
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_CEASE].box->RegisterBoxState(false);
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_TOGETHER].box->RegisterBoxState(false);
        }
        else if (iCheckboxId == CHECKBOX_ID_DR_ATTACK_TOGETHER)
        {
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_TOGETHER].box->RegisterBoxState(true);
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_CEASE].box->RegisterBoxState(false);
            m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_AUTO].box->RegisterBoxState(false);
        }

        ApplyConfigFromCheckbox(iCheckboxId, state);

        return false;
    }

    if (IsRelease(VK_LBUTTON))
    {
        int iPrevIndex = m_iSelectedSkillSlot;
        int iIconIndex = UpdateMouseIconList();

        if (iIconIndex != -1 && iIconIndex < MAX_SKILLS_SLOT)
        {
            g_ConsoleDebug->Write(MCD_NORMAL, "[MU Helper] Clicked skill slot [%d]", iIconIndex);
            m_iSelectedSkillSlot = iIconIndex;

            bool bPrevVisible = g_pNewUISystem->IsVisible(INTERFACE_MUHELPER_SKILL_LIST);

            if (iIconIndex == SKILL_SLOT_SKILL1
                || iIconIndex == SKILL_SLOT_SKILL2
                || iIconIndex == SKILL_SLOT_SKILL3)
            {
                g_pNewUIMuHelperSkillList->FilterByAttackSkills();
            }
            else
            {
                g_pNewUIMuHelperSkillList->FilterByBuffSkills();
            }

            if (iIconIndex == iPrevIndex && bPrevVisible)
            {
                g_pNewUISystem->Hide(INTERFACE_MUHELPER_SKILL_LIST);
            }
            else
            {
                g_pNewUISystem->Show(INTERFACE_MUHELPER_SKILL_LIST);
            }

            return false;
        }
        else if (iIconIndex == TEXTBOX_IMG_DISTANCE_TIME)
        {
            m_DistanceTimeInput.GiveFocus();
        }
        else if (iIconIndex == TEXTBOX_IMG_SKILL1_TIME)
        {
            m_Skill2DelayInput.GiveFocus();
        }
        else if (iIconIndex == TEXTBOX_IMG_SKILL2_TIME)
        {
            m_Skill3DelayInput.GiveFocus();
        }
        else if (iIconIndex == TEXTBOX_IMG_ADD_EXTRA_ITEM)
        {
            m_ItemInput.GiveFocus();
        }
    }
    if (IsRelease(VK_RBUTTON))
    {
        int iSlotIndex = UpdateMouseIconList();
        if (iSlotIndex != -1)
        {
            g_ConsoleDebug->Write(MCD_NORMAL, "[MU Helper] Clicked slot slot [%d]", iSlotIndex);
            m_aiSelectedSkills[iSlotIndex] = -1;

            auto cboxCombo = m_CheckBoxList[CHECKBOX_ID_COMBO];
            if (cboxCombo.box->GetBoxState() == true)
            {
                cboxCombo.box->RegisterBoxState(false);
                _TempConfig.bUseCombo = false;
            }

            return false;
        }
    }

    if (m_iCurrentOpenTab == 1)
    {
        m_ItemFilter.DoAction();
    }

    return false;
}

bool SEASON3B::CNewUIMuHelper::UpdateKeyEvent()
{
    if (IsVisible())
    {
        if (IsPress(VK_ESCAPE) == true)
        {
            g_pNewUISystem->Hide(INTERFACE_MUHELPER);
            g_pNewUISystem->Hide(INTERFACE_MUHELPER_SKILL_LIST);
            //PlayBuffer(SOUND_CLICK01);
            SetFocus(g_hWnd);

            return false;
        }
    }
    return true;
}

void SEASON3B::CNewUIMuHelper::ApplyConfigFromCheckbox(int iCheckboxId, bool bState)
{
    switch (iCheckboxId) 
    {
    case CHECKBOX_ID_POTION:        _TempConfig.bUseHealPotion = bState;        break;

    case CHECKBOX_ID_LONG_DISTANCE:
        _TempConfig.bLongRangeCounterAttack = bState;
        break;

    case CHECKBOX_ID_ORIG_POSITION:
        _TempConfig.bReturnToOriginalPosition = bState;
        break;

    case CHECKBOX_ID_SKILL2_DELAY:
        _TempConfig.aiSkillCondition[1] &= ~ON_CONDITION;
        _TempConfig.aiSkillCondition[1] = bState
            ? (_TempConfig.aiSkillCondition[1] | ON_TIMER)
            : (_TempConfig.aiSkillCondition[1] & ~ON_TIMER);
        break;

    case CHECKBOX_ID_SKILL2_CONDITION:
        _TempConfig.aiSkillCondition[1] &= ~ON_TIMER;
        _TempConfig.aiSkillCondition[1] = bState
            ? (_TempConfig.aiSkillCondition[1] | ON_CONDITION)
            : (_TempConfig.aiSkillCondition[1] & ~ON_CONDITION);
        break;

    case CHECKBOX_ID_SKILL3_DELAY:
        _TempConfig.aiSkillCondition[2] &= ~ON_CONDITION;
        _TempConfig.aiSkillCondition[2] = bState
            ? (_TempConfig.aiSkillCondition[2] | ON_TIMER)
            : (_TempConfig.aiSkillCondition[2] & ~ON_TIMER);
        break;

    case CHECKBOX_ID_SKILL3_CONDITION:
        _TempConfig.aiSkillCondition[2] &= ~ON_TIMER;
        _TempConfig.aiSkillCondition[2] = bState
            ? (_TempConfig.aiSkillCondition[2] | ON_CONDITION)
            : (_TempConfig.aiSkillCondition[2] & ~ON_CONDITION);
        break;

    case CHECKBOX_ID_COMBO:
        auto cboxCombo = m_CheckBoxList[CHECKBOX_ID_COMBO];

        if (bState == true)
        {
            if (m_aiSelectedSkills[0] <= 0 || m_aiSelectedSkills[1] <= 0 || m_aiSelectedSkills[2] <= 0)
            {
                g_pChatListBox->AddText("", GlobalText[3565], SEASON3B::TYPE_ERROR_MESSAGE);
                cboxCombo.box->RegisterBoxState(false);
            }
        }
        
        _TempConfig.bUseCombo = cboxCombo.box->GetBoxState();
        break;

    case CHECKBOX_ID_BUFF_DURATION:         _TempConfig.bBuffDuration = bState;                 break;
    case CHECKBOX_ID_USE_PET:               _TempConfig.bUseDarkRaven = bState;                 break;
    case CHECKBOX_ID_DR_ATTACK_CEASE:       _TempConfig.iDarkRavenMode = PET_ATTACK_CEASE;      break;
    case CHECKBOX_ID_DR_ATTACK_AUTO:        _TempConfig.iDarkRavenMode = PET_ATTACK_AUTO;       break;
    case CHECKBOX_ID_DR_ATTACK_TOGETHER:    _TempConfig.iDarkRavenMode = PET_ATTACK_TOGETHER;   break;
    case CHECKBOX_ID_PARTY:                 _TempConfig.bSupportParty = bState;                 break;
    case CHECKBOX_ID_AUTO_HEAL:             _TempConfig.bAutoHeal = bState;                     break;
    case CHECKBOX_ID_DRAIN_LIFE:            _TempConfig.bUseDrainLife = bState;                 break;
    case CHECKBOX_ID_REPAIR_ITEM:           _TempConfig.bRepairItem = bState;                   break;
    //case CHECKBOX_ID_START_OFFLINE:         _TempConfig.StartOffline = bState;                  break;

    case CHECKBOX_ID_PICK_ALL:
        auto cboxPickSelected = m_CheckBoxList[CHECKBOX_ID_PICK_SELECTED];
        if (cboxPickSelected.box->GetBoxState())
        {
            cboxPickSelected.box->RegisterBoxState(false);
        }
        _TempConfig.bPickAllItems = bState;
        break;

    case CHECKBOX_ID_PICK_SELECTED:
        auto cboxPickAll = m_CheckBoxList[CHECKBOX_ID_PICK_ALL];
        if (cboxPickAll.box->GetBoxState())
        {
            cboxPickAll.box->RegisterBoxState(false);
        }
        _TempConfig.bPickSelectItems = bState;
        break;

    case CHECKBOX_ID_PICK_JEWEL:        _TempConfig.bPickJewel = bState;        break;
    case CHECKBOX_ID_PICK_ANCIENT:      _TempConfig.bPickAncient = bState;      break;
    case CHECKBOX_ID_PICK_ZEN:          _TempConfig.bPickZen = bState;          break;
    case CHECKBOX_ID_PICK_EXCELLENT:    _TempConfig.bPickExcellent = bState;    break;
    case CHECKBOX_ID_ADD_OTHER_ITEM:    _TempConfig.bPickExtraItems = bState;   break;
    case CHECKBOX_ID_AUTO_DEFEND:       _TempConfig.bUseSelfDefense = bState;   break;

    default:        break;
    }
}

void SEASON3B::CNewUIMuHelper::ApplyConfigFromSkillSlot(int iSlot, int iSkill)
{
    if (iSlot < 3)
    {
        _TempConfig.aiSkill[iSlot] = iSkill;
    }
    else
    {
        _TempConfig.aiBuff[iSlot - SKILL_SLOT_BUFF1] = iSkill;
    }
}

void SEASON3B::CNewUIMuHelper::ApplyHuntRangeUpdate(int iDelta)
{
    _TempConfig.iHuntingRange += iDelta;
    if (_TempConfig.iHuntingRange < 0)
    {
        _TempConfig.iHuntingRange = 0;
    }
    if (_TempConfig.iHuntingRange > MAX_HUNTING_RANGE)
    {
        _TempConfig.iHuntingRange = MAX_HUNTING_RANGE;
    }
}

void SEASON3B::CNewUIMuHelper::ApplyLootRangeUpdate(int iDelta)
{
    _TempConfig.iObtainingRange += iDelta;
    if (_TempConfig.iObtainingRange < 1)
    {
        _TempConfig.iObtainingRange = 1;
    }
    if (_TempConfig.iObtainingRange > MAX_OBTAINING_RANGE)
    {
        _TempConfig.iObtainingRange = MAX_OBTAINING_RANGE;
    }
}

void SEASON3B::CNewUIMuHelper::SaveExtraItem()
{
    wchar_t wExtraItem[MAX_ITEM_NAME + 1] = { 0 };
    m_ItemInput.GetText(wExtraItem, sizeof(wExtraItem));

    if (wcslen(wExtraItem) == 0)
        return;

    char utf8Item[MAX_ITEM_NAME * 3 + 1] = { 0 };
    int len = WideCharToMultiByte(CP_UTF8, 0, wExtraItem, -1, utf8Item, sizeof(utf8Item), NULL, NULL);

    if (len > 1)
    {
        std::string itemStr(utf8Item);

        m_ItemFilter.AddText(itemStr.c_str());
        m_ItemFilter.Scrolling(-m_ItemFilter.GetBoxSize());

        _TempConfig.aExtraItems.insert(itemStr);
    }

    m_ItemInput.SetText("");
}

void SEASON3B::CNewUIMuHelper::RemoveExtraItem()
{
    FILTERLIST_TEXT* pText = m_ItemFilter.GetSelectedText();
    if (pText)
    {
        _TempConfig.aExtraItems.erase(std::string(pText->m_szPattern));
        m_ItemFilter.DeleteText(pText->m_szPattern);
    }
}

int SEASON3B::CNewUIMuHelper::GetIntFromTextInput(char* pwsInput)
{
    char* end;

    int value = static_cast<int>(strtol(pwsInput, &end, 10));  // Base 10

    if (*end != L'\0')
    {
        return 0;
    }

    return value;
}

void SEASON3B::CNewUIMuHelper::Reset()
{
    _TempConfig.iHuntingRange = 0;

    _TempConfig.iMaxSecondsAway = 0;
    _TempConfig.bLongRangeCounterAttack = false;
    _TempConfig.bReturnToOriginalPosition = false;

    _TempConfig.aiSkill.fill(0);
    _TempConfig.bUseCombo           = false;

    _TempConfig.aiSkillInterval.fill(0);

    _TempConfig.aiSkillCondition.fill(0);

    _TempConfig.aiBuff.fill(0);

    _TempConfig.bBuffDuration       = false;
    _TempConfig.bBuffDurationParty  = false;
    _TempConfig.iBuffCastInterval   = 0;

    _TempConfig.bAutoHeal           = false;
    _TempConfig.iHealThreshold      = 0;
    _TempConfig.bUseDrainLife       = false;
    _TempConfig.bUseHealPotion      = false;
    _TempConfig.iPotionThreshold    = 0;
    _TempConfig.bSupportParty       = false;
    _TempConfig.bAutoHealParty      = false;
    _TempConfig.iHealPartyThreshold = 0;

    _TempConfig.bUseDarkRaven       = false;
    _TempConfig.iDarkRavenMode      = PET_ATTACK_CEASE;
    _TempConfig.bRepairItem         = false;

    _TempConfig.iObtainingRange     = 0;
    _TempConfig.bPickAllItems       = false;
    _TempConfig.bPickSelectItems    = false;
    _TempConfig.bPickZen            = false;
    _TempConfig.bPickJewel          = false;
    _TempConfig.bPickExcellent      = false;
    _TempConfig.bPickAncient        = false;
    _TempConfig.bPickExtraItems     = false;
    _TempConfig.StartOffline        = false;
    _TempConfig.aExtraItems.clear();

    ApplyConfig();
}

void SEASON3B::CNewUIMuHelper::LoadSavedConfig(const ConfigData& config)
{
    _TempConfig = config;
    ApplyConfig();
}

void SEASON3B::CNewUIMuHelper::ApplyConfig()
{
    g_MuHelper.Load(_TempConfig);

    m_aiSelectedSkills[0] = _TempConfig.aiSkill[0] ? _TempConfig.aiSkill[0] : -1;
    m_aiSelectedSkills[1] = _TempConfig.aiSkill[1] ? _TempConfig.aiSkill[1] : -1;
    m_aiSelectedSkills[2] = _TempConfig.aiSkill[2] ? _TempConfig.aiSkill[2] : -1;
    m_aiSelectedSkills[3] = _TempConfig.aiBuff[0] ? _TempConfig.aiBuff[0] : -1;
    m_aiSelectedSkills[4] = _TempConfig.aiBuff[1] ? _TempConfig.aiBuff[1] : -1;
    m_aiSelectedSkills[5] = _TempConfig.aiBuff[2] ? _TempConfig.aiBuff[2] : -1;

    m_CheckBoxList[CHECKBOX_ID_POTION].box->RegisterBoxState(_TempConfig.bUseHealPotion);
    m_CheckBoxList[CHECKBOX_ID_AUTO_HEAL].box->RegisterBoxState(_TempConfig.bAutoHeal);
    m_CheckBoxList[CHECKBOX_ID_DRAIN_LIFE].box->RegisterBoxState(_TempConfig.bUseDrainLife);
    m_CheckBoxList[CHECKBOX_ID_LONG_DISTANCE].box->RegisterBoxState(_TempConfig.bLongRangeCounterAttack);
    m_CheckBoxList[CHECKBOX_ID_ORIG_POSITION].box->RegisterBoxState(_TempConfig.bReturnToOriginalPosition);

    m_CheckBoxList[CHECKBOX_ID_SKILL2_DELAY].box->RegisterBoxState(_TempConfig.aiSkillCondition[1] & ON_TIMER);
    m_CheckBoxList[CHECKBOX_ID_SKILL2_CONDITION].box->RegisterBoxState(_TempConfig.aiSkillCondition[1] & ON_CONDITION);
    m_CheckBoxList[CHECKBOX_ID_SKILL3_DELAY].box->RegisterBoxState(_TempConfig.aiSkillCondition[2] & ON_TIMER);
    m_CheckBoxList[CHECKBOX_ID_SKILL3_CONDITION].box->RegisterBoxState(_TempConfig.aiSkillCondition[2] & ON_CONDITION);
    m_CheckBoxList[CHECKBOX_ID_COMBO].box->RegisterBoxState(_TempConfig.bUseCombo);

    char wsTempNum[MAX_NUMBER_DIGITS + 1];
    memset(wsTempNum, 0, sizeof(wsTempNum));
    snprintf(wsTempNum, MAX_NUMBER_DIGITS + 1, "%d", _TempConfig.iMaxSecondsAway);
    m_DistanceTimeInput.SetText(wsTempNum);

    memset(wsTempNum, 0, sizeof(wsTempNum));
    snprintf(wsTempNum, MAX_NUMBER_DIGITS + 1, "%d", _TempConfig.aiSkillInterval[1]);
    m_Skill2DelayInput.SetText(wsTempNum);

    memset(wsTempNum, 0, sizeof(wsTempNum));
    snprintf(wsTempNum, MAX_NUMBER_DIGITS + 1, "%d", _TempConfig.aiSkillInterval[2]);
    m_Skill3DelayInput.SetText(wsTempNum);

    m_CheckBoxList[CHECKBOX_ID_BUFF_DURATION].box->RegisterBoxState(_TempConfig.bBuffDuration);
    m_CheckBoxList[CHECKBOX_ID_PARTY].box->RegisterBoxState(_TempConfig.bSupportParty);

    m_CheckBoxList[CHECKBOX_ID_USE_PET].box->RegisterBoxState(_TempConfig.bUseDarkRaven);
    m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_CEASE].box->RegisterBoxState(_TempConfig.iDarkRavenMode == PET_ATTACK_CEASE);
    m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_AUTO].box->RegisterBoxState(_TempConfig.iDarkRavenMode == PET_ATTACK_AUTO);
    m_CheckBoxList[CHECKBOX_ID_DR_ATTACK_TOGETHER].box->RegisterBoxState(_TempConfig.iDarkRavenMode == PET_ATTACK_TOGETHER);

    m_CheckBoxList[CHECKBOX_ID_REPAIR_ITEM].box->RegisterBoxState(_TempConfig.bRepairItem);
   //m_CheckBoxList[CHECKBOX_ID_START_OFFLINE].box->RegisterBoxState(_TempConfig.StartOffline);
    m_CheckBoxList[CHECKBOX_ID_PICK_ALL].box->RegisterBoxState(_TempConfig.bPickAllItems);
    m_CheckBoxList[CHECKBOX_ID_PICK_SELECTED].box->RegisterBoxState(_TempConfig.bPickSelectItems);
    m_CheckBoxList[CHECKBOX_ID_PICK_JEWEL].box->RegisterBoxState(_TempConfig.bPickJewel);
    m_CheckBoxList[CHECKBOX_ID_PICK_ZEN].box->RegisterBoxState(_TempConfig.bPickZen);
    m_CheckBoxList[CHECKBOX_ID_PICK_EXCELLENT].box->RegisterBoxState(_TempConfig.bPickExcellent);
    m_CheckBoxList[CHECKBOX_ID_PICK_ANCIENT].box->RegisterBoxState(_TempConfig.bPickAncient);
    m_CheckBoxList[CHECKBOX_ID_ADD_OTHER_ITEM].box->RegisterBoxState(_TempConfig.bPickExtraItems);

    m_CheckBoxList[CHECKBOX_ID_AUTO_ACCEPT_FRIEND].box->RegisterBoxState(_TempConfig.bAutoAcceptFriend);
    m_CheckBoxList[CHECKBOX_ID_AUTO_ACCEPT_GUILD].box->RegisterBoxState(_TempConfig.bAutoAcceptGuild);
    m_CheckBoxList[CHECKBOX_ID_AUTO_DEFEND].box->RegisterBoxState(_TempConfig.bUseSelfDefense);

    m_ItemFilter.Clear();
    for (const auto& item : _TempConfig.aExtraItems)
    {
        m_ItemFilter.AddText(item.c_str());
    }
}

void SEASON3B::CNewUIMuHelper::InitConfig()
{
    Reset();

    g_pNewUIMuHelperExt->InitConfig();
}

void SEASON3B::CNewUIMuHelper::SaveConfig()
{
    char wsNumberInput[MAX_NUMBER_DIGITS + 1]{};

    m_DistanceTimeInput.GetText(wsNumberInput, sizeof(wsNumberInput));
    _TempConfig.iMaxSecondsAway = GetIntFromTextInput(wsNumberInput);

    m_Skill2DelayInput.GetText(wsNumberInput, sizeof(wsNumberInput));
    _TempConfig.aiSkillInterval[1] = GetIntFromTextInput(wsNumberInput);

    m_Skill3DelayInput.GetText(wsNumberInput, sizeof(wsNumberInput));
    _TempConfig.aiSkillInterval[2] = GetIntFromTextInput(wsNumberInput);

    _TempConfig.aiSkill[0] = m_aiSelectedSkills[0] > 0 ? m_aiSelectedSkills[0] : 0;
    _TempConfig.aiSkill[1] = m_aiSelectedSkills[1] > 0 ? m_aiSelectedSkills[1] : 0;
    _TempConfig.aiSkill[2] = m_aiSelectedSkills[2] > 0 ? m_aiSelectedSkills[2] : 0;
    _TempConfig.aiBuff[0] = m_aiSelectedSkills[3] > 0 ? m_aiSelectedSkills[3] : 0;
    _TempConfig.aiBuff[1] = m_aiSelectedSkills[4] > 0 ? m_aiSelectedSkills[4] : 0;
    _TempConfig.aiBuff[2] = m_aiSelectedSkills[5] > 0 ? m_aiSelectedSkills[5] : 0;

    g_MuHelper.Save(_TempConfig);
}

float SEASON3B::CNewUIMuHelper::GetLayerDepth()
{
    return 3.4;
}

float SEASON3B::CNewUIMuHelper::GetKeyEventOrder()
{
    return 3.4;
}

void SEASON3B::CNewUIMuHelper::Show(bool bShow)
{
    CNewUIObj::Show(bShow);

    if (bShow == false)
    {
        if (g_pNewUIMuHelperExt)
            g_pNewUIMuHelperExt->Show(false);

        if (g_pNewUIMuHelperSkillList)
            g_pNewUIMuHelperSkillList->Show(false);
    }

    SetFocus(g_hWnd);
}

bool SEASON3B::CNewUIMuHelper::Render()
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

    g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 13, GlobalText[3536], 190, 0, RT3_SORT_CENTER);

    RenderBack(m_Pos.x + 12, m_Pos.y + 340, 165, 46);

    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 347, GlobalText[3537], 0, 0, RT3_SORT_CENTER);

    g_pRenderText->SetTextColor(0xFF00B4FF);
    g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 365, GlobalText[3538], 0, 0, RT3_SORT_CENTER);

    g_pRenderText->SetTextColor(TextColor);

    m_TabBtn.Render();

    if (m_iCurrentOpenTab == 1)
    {
        RenderBack(m_Pos.x + 12, m_Pos.y + 73, 68, 50);
        RenderBack(m_Pos.x + 75, m_Pos.y + 73, 102, 50);
        RenderBack(m_Pos.x + 12, m_Pos.y + 120, 165, 30);
        RenderBack(m_Pos.x + 12, m_Pos.y + 147, 165, 195);
        RenderBack(m_Pos.x + 16, m_Pos.y + 235, 158, 75);

        RenderImage(BITMAP_DISTANCE_BEGIN + _TempConfig.iObtainingRange, m_Pos.x + 29, m_Pos.y + 92, 15, 19, 0.f, 0.f, 15.f / 16.f, 19.f / 32.f);

        m_ItemFilter.Render();
    }
    else if (m_iCurrentOpenTab == 2)
    {
        RenderBack(m_Pos.x + 12, m_Pos.y + 73, 165, 50);
        RenderBack(m_Pos.x + 12, m_Pos.y + 120, 165, 222);
    }
    else
    {
        RenderBack(m_Pos.x + 12, m_Pos.y + 73, 68, 50);
        RenderBack(m_Pos.x + 75, m_Pos.y + 73, 102, 50);
        RenderBack(m_Pos.x + 12, m_Pos.y + 120, 165, 39);
        RenderBack(m_Pos.x + 12, m_Pos.y + 156, 165, 120);
        RenderBack(m_Pos.x + 12, m_Pos.y + 273, 165, 69);

        RenderImage(BITMAP_DISTANCE_BEGIN + _TempConfig.iHuntingRange, m_Pos.x + 29, m_Pos.y + 92, 15, 19, 0.f, 0.f, 15.f / 16.f, 19.f / 32.f);
    }

    RenderBoxList();
    RenderIconList();
    RenderTextList();
    RenderBtnList();

    if (m_iCurrentOpenTab == 0)
    {
        m_DistanceTimeInput.Render();
        m_Skill2DelayInput.Render();

        if (gCharacterManager.GetBaseClass(Hero->Class) != CLASS_DARK_LORD)
        {
            m_Skill3DelayInput.Render();
        }
    }
    else if (m_iCurrentOpenTab == 1)
    {
        m_ItemInput.Render();
    }

    DisableAlphaBlend();

    return true;
}

void SEASON3B::CNewUIMuHelper::RenderBack(int x, int y, int width, int height)
{
    EnableAlphaTest();
    glColor4f(0.0, 0.0, 0.0, 0.4f);
    RenderColor(x + 3.f, y + 2.f, width - 7.f, height - 7, 0.0, 0);
    EndRenderColor();

    RenderImage(IMAGE_TABLE_TOP_LEFT, x, y, 14.0, 14.0);
    RenderImage(IMAGE_TABLE_TOP_RIGHT, (x + width) - 14.f, y, 14.0, 14.0);
    RenderImage(IMAGE_TABLE_BOTTOM_LEFT, x, (y + height) - 14.f, 14.0, 14.0);
    RenderImage(IMAGE_TABLE_BOTTOM_RIGHT, (x + width) - 14.f, (y + height) - 14.f, 14.0, 14.0);
    RenderImage(IMAGE_TABLE_TOP_PIXEL, x + 6.f, y, (width - 12.f), 14.0);
    RenderImage(IMAGE_TABLE_RIGHT_PIXEL, (x + width) - 14.f, y + 6.f, 14.0, (height - 14.f));
    RenderImage(IMAGE_TABLE_BOTTOM_PIXEL, x + 6.f, (y + height) - 14.f, (width - 12.f), 14.0);
    RenderImage(IMAGE_TABLE_LEFT_PIXEL, x, (y + 6.f), 14.0, (height - 14.f));
}

void SEASON3B::CNewUIMuHelper::LoadImages()
{
    LoadBitmap("Interface\\MacroUI\\MacroUI_RangeMinus.tga", IMAGE_MACROUI_HELPER_RAGEMINUS, GL_LINEAR, GL_CLAMP, 1, 0);
    LoadBitmap("Interface\\MacroUI\\MacroUI_OptionButton.tga", IMAGE_MACROUI_HELPER_OPTIONBUTTON, GL_LINEAR, GL_CLAMP, 1, 0);
    LoadBitmap("Interface\\MacroUI\\MacroUI_InputNumber.tga", IMAGE_MACROUI_HELPER_INPUTNUMBER, GL_LINEAR, GL_CLAMP, 1, 0);
    LoadBitmap("Interface\\MacroUI\\MacroUI_InputString.tga", IMAGE_MACROUI_HELPER_INPUTSTRING, GL_LINEAR, GL_CLAMP, 1, 0);
    //--
    LoadBitmap("Interface\\InGameShop\\Ingame_Bt03.tga", IMAGE_IGS_BUTTON, GL_LINEAR, GL_CLAMP, 1, 0);
}

void SEASON3B::CNewUIMuHelper::UnloadImages()
{
    DeleteBitmap(IMAGE_MACROUI_HELPER_RAGEMINUS);
    DeleteBitmap(IMAGE_MACROUI_HELPER_OPTIONBUTTON);
    DeleteBitmap(IMAGE_MACROUI_HELPER_INPUTNUMBER);
    DeleteBitmap(IMAGE_MACROUI_HELPER_INPUTSTRING);
    //--
    DeleteBitmap(IMAGE_IGS_BUTTON);
}

void SEASON3B::CNewUIMuHelper::RegisterButton(int Identifier, CButtonTap button)
{
    m_ButtonList.insert(std::pair<int, CButtonTap>(Identifier, button));
}

void SEASON3B::CNewUIMuHelper::RegisterBtnCharacter(BYTE class_character, int Identifier)
{
    auto li = m_ButtonList.find(Identifier);

    if (li != m_ButtonList.end())
    {
        CButtonTap* cBTN = &li->second;
        if (class_character >= 0 && class_character < MAX_CLASS)
        {
            cBTN->class_character[class_character] = TRUE;
        }
        else
        {
            memset(cBTN->class_character, 1, sizeof(cBTN->class_character));
        }
    }
}

void SEASON3B::CNewUIMuHelper::InsertButton(int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, std::string btname, std::string tooltiptext, int Identifier, int iNumTab)
{
    CButtonTap cBTN;
    auto* button = new CNewUIButton();

    button->ChangeButtonImgState(1, imgindex, overflg, isimgwidth, bClickEffect);
    button->ChangeButtonInfo(x, y, sx, sy);

    button->ChangeText(btname);
    button->ChangeToolTipText(tooltiptext, TRUE);

    if (MoveTxt)
    {
        button->MoveTextPos(0, -1);
    }

    cBTN.btn = button;
    cBTN.iNumTab = iNumTab;
    memset(cBTN.class_character, 0, sizeof(cBTN.class_character));

    RegisterButton(Identifier, cBTN);
}

void SEASON3B::CNewUIMuHelper::RenderBtnList()
{
    auto li = m_ButtonList.begin();

    for (; li != m_ButtonList.end(); li++)
    {
        CButtonTap* cBTN = &li->second;

        if ((cBTN->class_character[gCharacterManager.GetBaseClass(Hero->Class)]) && (cBTN->iNumTab == m_iCurrentOpenTab || cBTN->iNumTab == -1))
        {
            cBTN->btn->Render();
        }
    }
}

int SEASON3B::CNewUIMuHelper::UpdateMouseBtnList()
{
    auto li = m_ButtonList.begin();

    for (; li != m_ButtonList.end(); li++)
    {
        CButtonTap* cBTN = &li->second;

        if ((cBTN->class_character[gCharacterManager.GetBaseClass(Hero->Class)]) && (cBTN->iNumTab == m_iCurrentOpenTab || cBTN->iNumTab == -1))
        {
            if (cBTN->btn->UpdateMouseEvent())
            {
                return li->first;
            }
        }
    }
    return -1;
}

//===============================================================================================================
//===============================================================================================================

void SEASON3B::CNewUIMuHelper::RegisterBoxCharacter(BYTE class_character, int Identifier)
{
    auto li = m_CheckBoxList.find(Identifier);

    if (li != m_CheckBoxList.end())
    {
        CheckBoxTap* cBOX = &li->second;

        if (class_character >= 0 && class_character < MAX_CLASS)
        {
            cBOX->class_character[class_character] = TRUE;
        }
        else
        {
            memset(cBOX->class_character, 1, sizeof(cBOX->class_character));
        }
    }
}

void SEASON3B::CNewUIMuHelper::RegisterCheckBox(int Identifier, CheckBoxTap button)
{
    m_CheckBoxList.insert(std::pair<int, CheckBoxTap>(Identifier, button));
}

void SEASON3B::CNewUIMuHelper::InsertCheckBox(int imgindex, int x, int y, int sx, int sy, bool overflg, std::string btname, int Identifier, int iNumTab)
{
    CheckBoxTap cBOX;

    auto* cbox = new CNewUICheckBox;

    cbox->CheckBoxImgState(imgindex);
    cbox->CheckBoxInfo(x, y, sx, sy);

    cbox->ChangeText(btname);
    cbox->RegisterBoxState(overflg);

    cBOX.box = cbox;
    cBOX.iNumTab = iNumTab;
    memset(cBOX.class_character, 0, sizeof(cBOX.class_character));

    RegisterCheckBox(Identifier, cBOX);
}

void SEASON3B::CNewUIMuHelper::RenderBoxList()
{
    auto li = m_CheckBoxList.begin();

    for (; li != m_CheckBoxList.end(); li++)
    {
        CheckBoxTap* cBOX = &li->second;

        if ((cBOX->class_character[gCharacterManager.GetBaseClass(Hero->Class)]) && (cBOX->iNumTab == m_iCurrentOpenTab || cBOX->iNumTab == -1))
        {
            cBOX->box->Render();
        }
    }
}

int SEASON3B::CNewUIMuHelper::UpdateMouseBoxList()
{
    auto li = m_CheckBoxList.begin();

    for (; li != m_CheckBoxList.end(); li++)
    {
        CheckBoxTap* cBOX = &li->second;

        if ((cBOX->class_character[gCharacterManager.GetBaseClass(Hero->Class)]) && (cBOX->iNumTab == m_iCurrentOpenTab || cBOX->iNumTab == -1))
        {
            if (cBOX->box->UpdateMouseEvent())
            {
                return li->first;
            }
        }
    }
    return -1;
}

//===============================================================================================================
//===============================================================================================================

void SEASON3B::CNewUIMuHelper::RenderIconList()
{
    auto li = m_IconList.begin();

    for (; li != m_IconList.end(); li++)
    {
        cTexture* cImage = &li->second;

        if ((cImage->class_character[gCharacterManager.GetBaseClass(Hero->Class)]) && (cImage->iNumTab == m_iCurrentOpenTab || cImage->iNumTab == -1))
        {
            RenderImage(cImage->s_ImgIndex, cImage->m_Pos.x, cImage->m_Pos.y, cImage->m_Size.x, cImage->m_Size.y);

            if (li->first < MAX_SKILLS_SLOT)
            {
                if (m_aiSelectedSkills[li->first] >= 0 && m_aiSelectedSkills[li->first] < MAX_SKILLS)
                {
                    g_pSkillList->RenderSkillIcon(m_aiSelectedSkills[li->first], cImage->m_Pos.x + 6, cImage->m_Pos.y + 6, 20, 28, 1);
                }
            }
        }
    }
}

int SEASON3B::CNewUIMuHelper::UpdateMouseIconList()
{
    auto li = m_IconList.begin();

    for (; li != m_IconList.end(); li++)
    {
        cTexture* cImage = &li->second;

        if ((cImage->class_character[gCharacterManager.GetBaseClass(Hero->Class)]) && (cImage->iNumTab == m_iCurrentOpenTab || cImage->iNumTab == -1))
        {
            if (CheckMouseIn(cImage->m_Pos.x, cImage->m_Pos.y, cImage->m_Size.x, cImage->m_Size.y))
            {
                return li->first;
            }
        }
    }

    return -1;
}

void SEASON3B::CNewUIMuHelper::RegisterIconCharacter(BYTE class_character, int Identifier)
{
    auto li = m_IconList.find(Identifier);

    if (li != m_IconList.end())
    {
        cTexture* cImage = &li->second;

        if (class_character >= 0 && class_character < MAX_CLASS)
        {
            cImage->class_character[class_character] = TRUE;
        }
        else
        {
            memset(cImage->class_character, 1, sizeof(cImage->class_character));
        }
    }
}

void SEASON3B::CNewUIMuHelper::RegisterIcon(int Identifier, cTexture button)
{
    m_IconList.insert(std::pair<int, cTexture>(Identifier, button));
}

void SEASON3B::CNewUIMuHelper::InsertIcon(int imgindex, int x, int y, int sx, int sy, int Identifier, int iNumTab)
{
    cTexture cImage;

    cImage.s_ImgIndex = imgindex;
    cImage.m_Pos.x = x;
    cImage.m_Pos.y = y;
    cImage.m_Size.x = sx;
    cImage.m_Size.y = sy;
    cImage.iNumTab = iNumTab;

    memset(cImage.class_character, 0, sizeof(cImage.class_character));

    RegisterIcon(Identifier, cImage);
}

//===============================================================================================================
//===============================================================================================================

void SEASON3B::CNewUIMuHelper::RenderTextList()
{
    auto li = m_TextNameList.begin();

    for (; li != m_TextNameList.end(); li++)
    {
        cTextName* cImage = &li->second;

        if ((cImage->class_character[gCharacterManager.GetBaseClass(Hero->Class)]) && (cImage->iNumTab == m_iCurrentOpenTab || cImage->iNumTab == -1))
        {
            g_pRenderText->RenderText(cImage->m_Pos.x, cImage->m_Pos.y, cImage->m_Name.c_str());
        }
    }
}

void SEASON3B::CNewUIMuHelper::RegisterTextCharacter(BYTE class_character, int Identifier)
{
    auto li = m_TextNameList.find(Identifier);

    if (li != m_TextNameList.end())
    {
        cTextName* cImage = &li->second;

        if (class_character >= 0 && class_character < MAX_CLASS)
        {
            cImage->class_character[class_character] = TRUE;
        }
        else
        {
            memset(cImage->class_character, 1, sizeof(cImage->class_character));
        }
    }
}

void SEASON3B::CNewUIMuHelper::RegisterText(int Identifier, cTextName button)
{
    m_TextNameList.insert(std::pair<int, cTextName>(Identifier, button));
}

void SEASON3B::CNewUIMuHelper::InsertText(int x, int y, std::string Name, int Identifier, int iNumTab)
{
    cTextName cText;

    cText.m_Pos.x = x;
    cText.m_Pos.y = y;
    cText.m_Name = Name;
    cText.iNumTab = iNumTab;

    memset(cText.class_character, 0, sizeof(cText.class_character));
    RegisterText(Identifier, cText);
}

void SEASON3B::CNewUIMuHelper::AssignSkill(int iSkill)
{
    if (m_iSelectedSkillSlot != -1 && m_iSelectedSkillSlot < MAX_SKILLS_SLOT)
    {
        if (!IsSkillAssigned(iSkill))
        {
            m_aiSelectedSkills[m_iSelectedSkillSlot] = iSkill;
            ApplyConfigFromSkillSlot(m_iSelectedSkillSlot, iSkill);

            g_ConsoleDebug->Write(MCD_NORMAL, "[MU Helper] Assign m_aiSelectedSkills[%d] = %d", m_iSelectedSkillSlot, iSkill);
        }
        else
        {
            int iPrevIndex = GetSkillIndex(iSkill);
            m_aiSelectedSkills[iPrevIndex] = -1;
            m_aiSelectedSkills[m_iSelectedSkillSlot] = iSkill;

            auto cboxCombo = m_CheckBoxList[CHECKBOX_ID_COMBO];
            if (cboxCombo.box->GetBoxState() == true)
            {
                cboxCombo.box->RegisterBoxState(false);
                _TempConfig.bUseCombo = false;
            }
        }
    }
}

bool SEASON3B::CNewUIMuHelper::IsSkillAssigned(int iSkill)
{
    return std::find(m_aiSelectedSkills.begin(), m_aiSelectedSkills.end(), iSkill) != m_aiSelectedSkills.end();
}

int SEASON3B::CNewUIMuHelper::GetSkillIndex(int iSkill)
{
    auto it = std::find(m_aiSelectedSkills.begin(), m_aiSelectedSkills.end(), iSkill);

    if (it != m_aiSelectedSkills.end()) {
        return std::distance(m_aiSelectedSkills.begin(), it);
    }

    return -1;
}

void SEASON3B::CNewUIMuHelper::AutoReset()
{
    SendRequestDataSend(0xFD, 0x32);
}

inline void SEASON3B::CNewUIMuHelper::WritePrivateProfileInt(const char* section, const char* key, int value, const char* path)
{
    char buffer[32];
    sprintf(buffer, "%d", value);
    WritePrivateProfileString(section, key, buffer, path);
}

void SEASON3B::CNewUIMuHelper::AllDataOfflineSave()
{
    this->SaveConfig();

    const char* path = ".\\Data\\ACuoi\\Config\\Setting\\Config.ini";
    const char* currentName = Hero->ID;
    const auto& cfg = MUHelper::g_MuHelper.m_config;

    std::vector<std::string> lines;
    FILE* fp = fopen(path, "r");

    if (fp)
    {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fp))
        {
            std::string line = buffer;
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            lines.push_back(line);
        }
        fclose(fp);
    }

    char newLine[1024];
    sprintf(newLine, "%s %d %d %d %d", currentName,
        cfg.iHuntingRange,
        cfg.bLongRangeCounterAttack ? 1 : 0,
        cfg.bReturnToOriginalPosition ? 1 : 0,
        cfg.iMaxSecondsAway);

    for (int i = 0; i < 3; ++i) sprintf(newLine + strlen(newLine), " %u", cfg.aiSkill[i]);
    for (int i = 0; i < 3; ++i) sprintf(newLine + strlen(newLine), " %u", cfg.aiSkillCondition[i]);
    for (int i = 0; i < 3; ++i) sprintf(newLine + strlen(newLine), " %u", cfg.aiSkillInterval[i]);

    sprintf(newLine + strlen(newLine), " %d", cfg.bUseCombo ? 1 : 0);
    for (int i = 0; i < 3; ++i) sprintf(newLine + strlen(newLine), " %u", cfg.aiBuff[i]);

    int safe_iHealThreshold = (cfg.iHealThreshold >= 0 && cfg.iHealThreshold <= 100) ? cfg.iHealThreshold : 50;
    int safe_iHealPartyThreshold = (cfg.iHealPartyThreshold >= 0 && cfg.iHealPartyThreshold <= 100) ? cfg.iHealPartyThreshold : 50;
    int safe_iPotionThreshold = (cfg.iPotionThreshold >= 0 && cfg.iPotionThreshold <= 100) ? cfg.iPotionThreshold : 50;

    sprintf(newLine + strlen(newLine), " %d %d %d %d %d %d %d %d %d",
        cfg.bBuffDuration ? 1 : 0,
        cfg.bBuffDurationParty ? 1 : 0,
        cfg.iBuffCastInterval,
        cfg.bAutoHeal ? 1 : 0,
        safe_iHealThreshold,
        cfg.bSupportParty ? 1 : 0,
        cfg.bAutoHealParty ? 1 : 0,
        safe_iHealPartyThreshold,
        cfg.bUseHealPotion ? 1 : 0);

    sprintf(newLine + strlen(newLine), " %d %d %d %d %d %d %d %d %d",
        safe_iPotionThreshold,
        cfg.bUseDrainLife ? 1 : 0,
        cfg.bUseDarkRaven ? 1 : 0,
        cfg.iDarkRavenMode,
        cfg.bRepairItem ? 1 : 0,
        cfg.StartOffline ? 1 : 0,
        cfg.iObtainingRange,
        cfg.bPickAllItems ? 1 : 0,
        cfg.bPickSelectItems ? 1 : 0);

    sprintf(newLine + strlen(newLine), " %d %d %d %d %d",
        cfg.bPickJewel ? 1 : 0,
        cfg.bPickZen ? 1 : 0,
        cfg.bPickAncient ? 1 : 0,
        cfg.bPickExcellent ? 1 : 0,
        cfg.bPickExtraItems ? 1 : 0);

    bool found = false;
    for (auto& line : lines)
    {
        if (_strnicmp(line.c_str(), currentName, strlen(currentName)) == 0 &&
            (line[strlen(currentName)] == ' ' || line[strlen(currentName)] == '\t'))
        {
            line = newLine;
            found = true;
            break;
        }
    }

    if (!found)
    {
        auto it = std::find_if(lines.begin(), lines.end(), [](const std::string& line)
            {
                return _stricmp(line.c_str(), "end") == 0;
            });

        if (it != lines.end())
            lines.insert(it, newLine);
        else
        {
            lines.push_back(newLine);
            lines.push_back("end");
        }
    }

    bool hasEnd = std::any_of(lines.begin(), lines.end(), [](const std::string& line) {
        return _stricmp(line.c_str(), "end") == 0;
        });

    if (!hasEnd)
        lines.push_back("end");

    FILE* fw = fopen(path, "w");
    if (!fw) return;

    for (const auto& line : lines)
        fprintf(fw, "%s\n", line.c_str());

    fclose(fw);

    char extraPath[260];
    sprintf(extraPath, ".\\Data\\ACuoi\\Config\\Setting\\%s.ini", currentName);
    FILE* fe = fopen(extraPath, "w");
    if (!fe) return;

    for (const auto& item : cfg.aExtraItems)
    {
        fprintf(fe, "%s\n", item.c_str());
    }

    fclose(fe);
}
