#pragma once

#include <array>
#include <vector>

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "MUHelper/MuHelper.h"

namespace SEASON3B
{
    class CNewUIMuHelper : public CNewUIObj
    {
    public:
        CNewUIMuHelper();
        ~CNewUIMuHelper();
        void AllDataOfflineSave();
        bool Create(CNewUIManager* pNewUIMng, int x, int y);
        void Release();

        void Show(bool bShow);
        bool Render();
        bool Update();
        bool UpdateMouseEvent();
        bool UpdateKeyEvent();

        float GetLayerDepth();
        float GetKeyEventOrder();
        void AutoReset();
        void Reset();
        void LoadSavedConfig(const MUHelper::ConfigData& config);
        void AssignSkill(int iSkill);
        static int GetIntFromTextInput(char* pstrInput);
        inline void WritePrivateProfileInt(const char* section, const char* key, int value, const char* path);

        enum ESkillSlot
        {
            SUB_PAGE_SKILL2_CONFIG         = 2,
            SUB_PAGE_SKILL3_CONFIG         = 3,
            SUB_PAGE_POTION_CONFIG_ELF     = 4,
            SUB_PAGE_POTION_CONFIG_SUMMY   = 5,
            SUB_PAGE_POTION_CONFIG         = 6,
            SUB_PAGE_PARTY_CONFIG          = 7,
            SUB_PAGE_PARTY_CONFIG_ELF      = 8
        };

        enum ECheckBoxId : uint16_t
        {
            CHECKBOX_ID_POTION               = 0,
            CHECKBOX_ID_LONG_DISTANCE        = 1,
            CHECKBOX_ID_ORIG_POSITION        = 2,
            CHECKBOX_ID_SKILL2_DELAY         = 3,
            CHECKBOX_ID_SKILL2_CONDITION     = 4,
            CHECKBOX_ID_SKILL3_DELAY         = 5,
            CHECKBOX_ID_SKILL3_CONDITION     = 6,
            CHECKBOX_ID_COMBO                = 7,
            CHECKBOX_ID_BUFF_DURATION        = 8,
            CHECKBOX_ID_USE_PET              = 9,
            CHECKBOX_ID_PARTY                = 10,
            CHECKBOX_ID_AUTO_HEAL            = 11,
            CHECKBOX_ID_DRAIN_LIFE           = 12,
            CHECKBOX_ID_REPAIR_ITEM          = 13,
            CHECKBOX_ID_START_OFFLINE        = 14,
            CHECKBOX_ID_PICK_ALL             = 15,
            CHECKBOX_ID_PICK_SELECTED        = 16,
            CHECKBOX_ID_PICK_JEWEL           = 17,
            CHECKBOX_ID_PICK_ANCIENT         = 18,
            CHECKBOX_ID_PICK_ZEN             = 19,
            CHECKBOX_ID_PICK_EXCELLENT       = 20,
            CHECKBOX_ID_ADD_OTHER_ITEM       = 21,
            CHECKBOX_ID_AUTO_ACCEPT_FRIEND   = 22,
            CHECKBOX_ID_AUTO_DEFEND          = 23,
            CHECKBOX_ID_AUTO_ACCEPT_GUILD    = 24,
            CHECKBOX_ID_DR_ATTACK_CEASE      = 25,
            CHECKBOX_ID_DR_ATTACK_AUTO       = 26,
            CHECKBOX_ID_DR_ATTACK_TOGETHER   = 27,
            CHECKBOX_ID_AUTO_RESET           = 28
        };

        enum EButtonId : uint16_t
        {
            BUTTON_ID_HUNT_RANGE_ADD       = 0,
            BUTTON_ID_HUNT_RANGE_MINUS     = 1,
            BUTTON_ID_SKILL2_CONFIG        = 2,
            BUTTON_ID_SKILL3_CONFIG        = 3,
            BUTTON_ID_POTION_CONFIG_ELF    = 4,
            BUTTON_ID_POTION_CONFIG_SUMMY  = 5,
            BUTTON_ID_POTION_CONFIG        = 6,
            BUTTON_ID_PARTY_CONFIG         = 7,
            BUTTON_ID_PARTY_CONFIG_ELF     = 8,
            BUTTON_ID_PICK_RANGE_ADD       = 9,
            BUTTON_ID_PICK_RANGE_MINUS     = 10,
            BUTTON_ID_ADD_OTHER_ITEM       = 11,
            BUTTON_ID_DELETE_OTHER_ITEM    = 12,
            BUTTON_ID_SAVE_CONFIG          = 13,
            BUTTON_ID_INIT_CONFIG          = 14,
            BUTTON_ID_EXIT_CONFIG          = 15
        };


        enum ESkillSlotImg : uint16_t
        {
            SKILL_SLOT_SKILL1   = 0,
            SKILL_SLOT_SKILL2   = 1,
            SKILL_SLOT_SKILL3   = 2,
            SKILL_SLOT_BUFF1    = 3,
            SKILL_SLOT_BUFF2    = 4,
            SKILL_SLOT_BUFF3    = 5
        };

        enum ETextBoxImg : uint16_t
        {
            TEXTBOX_IMG_DISTANCE_TIME   = 6,
            TEXTBOX_IMG_SKILL1_TIME     = 7,
            TEXTBOX_IMG_SKILL2_TIME     = 8,
            TEXTBOX_IMG_ADD_EXTRA_ITEM  = 9
        };

    private:
        enum IMAGE_LIST
        {
            IMAGE_BASE_WINDOW_BACK = BITMAP_INTERFACE_NEW_MESSAGEBOX_BEGIN + 3,				//. newui_msgbox_back.jpg
            IMAGE_BASE_WINDOW_TOP = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN,			//. newui_item_back01.tga	(190,64)
            IMAGE_BASE_WINDOW_LEFT = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN + 2,		//. newui_item_back02-l.tga	(21,320)
            IMAGE_BASE_WINDOW_RIGHT = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN + 3,		//. newui_item_back02-r.tga	(21,320)
            IMAGE_BASE_WINDOW_BOTTOM = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN + 4,	//. newui_item_back03.tga	(190,45)
            IMAGE_BASE_WINDOW_BTN_EXIT = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN + 17,		//. newui_exit_00.tga
            //--
            IMAGE_TABLE_SQUARE = BITMAP_INTERFACE_NEW_INVENTORY_BASE_BEGIN,	//. newui_item_box.tga
            IMAGE_TABLE_TOP_LEFT,			//. newui_item_table01(L).tga (14,14)
            IMAGE_TABLE_TOP_RIGHT,			//. newui_item_table01(R).tga (14,14)
            IMAGE_TABLE_BOTTOM_LEFT,		//. newui_item_table02(L).tga (14,14)
            IMAGE_TABLE_BOTTOM_RIGHT,	//. newui_item_table02(R).tga (14,14)
            IMAGE_TABLE_TOP_PIXEL,			//. newui_item_table03(up).tga (1, 14)
            IMAGE_TABLE_BOTTOM_PIXEL,	//. newui_item_table03(dw).tga (1,14)
            IMAGE_TABLE_LEFT_PIXEL,		//. newui_item_table03(L).tga (14,1)
            IMAGE_TABLE_RIGHT_PIXEL,		//. newui_item_table03(R).tga (14,1)
            //--
            IMAGE_WINDOW_TAB_BTN = BITMAP_GUILDINFO_BEGIN,
            //--
            IMAGE_MACROUI_HELPER_RAGEMINUS = BITMAP_INTERFACE_MACROUI_BEGIN,		// newui_position02.tga			(70, 25)
            IMAGE_MACROUI_HELPER_OPTIONBUTTON = BITMAP_INTERFACE_MACROUI_BEGIN + 1,		// newui_position02.tga			(70, 25)
            IMAGE_MACROUI_HELPER_INPUTNUMBER = BITMAP_INTERFACE_MACROUI_BEGIN + 2,
            IMAGE_MACROUI_HELPER_INPUTSTRING = BITMAP_INTERFACE_MACROUI_BEGIN + 3,
            //-- Buttons
            IMAGE_CHAINFO_BTN_STAT = BITMAP_INTERFACE_NEW_CHAINFO_WINDOW_BEGIN + 1,
            IMAGE_CLEARNESS_BTN = BITMAP_CURSEDTEMPLE_BEGIN + 4,
            IMAGE_IGS_BUTTON = BITMAP_IGS_MSGBOX_BUTTON,
            IMAGE_CHECKBOX_BTN = BITMAP_OPTION_BEGIN + 5,

            //-- Skills
            IMAGE_SKILL1 = BITMAP_INTERFACE_NEW_SKILLICON_BEGIN,
            IMAGE_SKILL2,
            IMAGE_COMMAND,
            IMAGE_SKILL3,
            IMAGE_SKILLBOX,
            IMAGE_SKILLBOX_USE,
            IMAGE_NON_SKILL1,
            IMAGE_NON_SKILL2,
            IMAGE_NON_COMMAND,
            IMAGE_NON_SKILL3,
        };

        enum CLASS_LIST_ 
        {
            DW = 0,
            DK,
            FE,
            MG,
            DL,
            SM,
            RF,
        };

        static constexpr int MAX_SKILLS_SLOT = 6;
        static constexpr int WINDOW_WIDTH = 190;
        static constexpr int WINDOW_HEIGHT = 429;

        typedef struct
        {
            int iNumTab;
            BYTE class_character[MAX_CLASS];
            CNewUIButton* btn;
        } CButtonTap;

        typedef struct
        {
            int iNumTab;
            BYTE class_character[MAX_CLASS];
            CNewUICheckBox* box;
        } CheckBoxTap;

        typedef struct
        {
            int iNumTab;
            int s_ImgIndex;
            POINT m_Pos;
            POINT m_Size;
            BYTE class_character[MAX_CLASS];
        } cTexture;

        typedef struct
        {
            int iNumTab;
            POINT m_Pos;
            std::string m_Name;
            BYTE class_character[MAX_CLASS];
        } cTextName;

        typedef std::map<int, CButtonTap> cButtonMap;
        typedef std::map<int, CheckBoxTap> cCheckBoxMap;
        typedef std::map<int, cTexture> cTextureMap;
        typedef std::map<int, cTextName> cTextNameMap;

        public:

        void RenderBtnList();
        int UpdateMouseBtnList();
        void RegisterBtnCharacter(BYTE class_character, int Identifier);
        void RegisterButton(int Identifier, CButtonTap button);
        void InsertButton(int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt,std::string btname,std::string tooltiptext, int Identifier, int iNumTab);
        //--
        void RenderBoxList();
        int UpdateMouseBoxList();
        void RegisterBoxCharacter(BYTE class_character, int Identifier);
        void RegisterCheckBox(int Identifier, CheckBoxTap button);
        void InsertCheckBox(int imgindex, int x, int y, int sx, int sy, bool overflg,std::string btname, int Identifier, int iNumTab);
        //--
        void RenderIconList();
        int UpdateMouseIconList();
        void RegisterIconCharacter(BYTE class_character, int Identifier);
        void RegisterIcon(int Identifier, cTexture button);
        void InsertIcon(int imgindex, int x, int y, int sx, int sy, int Identifier, int iNumTab);
        //--
        void RenderTextList();
        void RegisterTextCharacter(BYTE class_character, int Identifier);
        void RegisterText(int Identifier, cTextName button);
        void InsertText(int x, int y,std::string Name, int Identifier, int iNumTab);
        void InitText();
        void InitImage();
        void InitButtons();
        void InitCheckBox();
        void InitTextboxInput();
        void SetPos(int x, int y);
        void RenderBack(int x, int y, int width, int height);

        int GetSkillIndex(int iSkill);
        bool IsSkillAssigned(int iSkill);
        
        void InitConfig();
        void SaveConfig();
        void ApplyConfig();

        void LoadImages();
        void UnloadImages();

        void ApplyConfigFromCheckbox(int iCheckboxId, bool bState);
        void ApplyConfigFromSkillSlot(int iSlot, int iSkill);
        void ApplyHuntRangeUpdate(int iDelta);
        void ApplyLootRangeUpdate(int iDelta);
        void SaveExtraItem();
        void RemoveExtraItem();

        private:
        CNewUIManager* m_pNewUIMng;
        CUITextInputBox m_DistanceTimeInput;
        CUITextInputBox m_Skill2DelayInput;
        CUITextInputBox m_Skill3DelayInput;
        CUITextInputBox m_ItemInput;
        CUIExtraItemListBox m_ItemFilter;

        POINT m_Pos;
        CNewUIRadioGroupButton m_TabBtn;
        int m_iCurrentOpenTab;
        int m_iCurrentOpenSubWin;
        bool m_bSubWinOpen;
        cButtonMap m_ButtonList;
        cCheckBoxMap m_CheckBoxList;
        cTextNameMap m_TextNameList;
        cTextureMap m_IconList;
        int m_iSelectedSkillSlot;
        std::array<int, MAX_SKILLS_SLOT> m_aiSelectedSkills;
    };
}
