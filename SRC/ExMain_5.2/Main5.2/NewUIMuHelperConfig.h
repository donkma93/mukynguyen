#pragma once

#include <array>
#include <vector>

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "MUHelper/MuHelper.h"

namespace SEASON3B
{
    class CNewUIMuHelperConfig : public CNewUIObj
    {
        public:
        CNewUIMuHelperConfig();
        ~CNewUIMuHelperConfig();

        bool Create(CNewUIManager* pNewUIMng, int x, int y);
        void Release();

        bool Render();
        bool Update();
        bool UpdateMouseEvent();
        bool UpdateKeyEvent();

        float GetLayerDepth();
        float GetKeyEventOrder();

        void Toggle(int iPage);
        void Save();
        void Reset();
        void ApplySavedConfig();
        void InitConfig();

        enum ESkillSlot
        {
            SUB_PAGE_SKILL2_CONFIG = 2,
            SUB_PAGE_SKILL3_CONFIG,
            SUB_PAGE_POTION_CONFIG_ELF,
            SUB_PAGE_POTION_CONFIG_SUMMY,
            SUB_PAGE_POTION_CONFIG,
            SUB_PAGE_PARTY_CONFIG,
            SUB_PAGE_PARTY_CONFIG_ELF
        };

        private:
        enum IMAGE_LIST
        {
            IMAGE_BASE_WINDOW_BACK = BITMAP_INTERFACE_NEW_MESSAGEBOX_BEGIN + 3,				
            IMAGE_BASE_WINDOW_TOP = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN,			
            IMAGE_BASE_WINDOW_LEFT = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN + 2,		
            IMAGE_BASE_WINDOW_RIGHT = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN + 3,		
            IMAGE_BASE_WINDOW_BOTTOM = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN + 4,	
            IMAGE_BASE_WINDOW_BTN_EXIT = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN + 17,	
            //--
            IMAGE_TABLE_SQUARE = BITMAP_INTERFACE_NEW_INVENTORY_BASE_BEGIN,	//. newui_item_box.tga
            IMAGE_TABLE_TOP_LEFT,		
            IMAGE_TABLE_TOP_RIGHT,		
            IMAGE_TABLE_BOTTOM_LEFT,	
            IMAGE_TABLE_BOTTOM_RIGHT,	
            IMAGE_TABLE_TOP_PIXEL,		
            IMAGE_TABLE_BOTTOM_PIXEL,	
            IMAGE_TABLE_LEFT_PIXEL,		
            IMAGE_TABLE_RIGHT_PIXEL,	
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

            IMAGE_OPTION_BTN_CHECK = BITMAP_OPTION_BEGIN + 5,
            IMAGE_OPTION_VOLUME_BACK = BITMAP_OPTION_BEGIN + 8,
            IMAGE_OPTION_VOLUME_COLOR = BITMAP_OPTION_BEGIN + 9,

        };

        static constexpr int WINDOW_WIDTH = 190;
        static constexpr int WINDOW_HEIGHT = 429;

        public:
        void InitText();
        void InitButtons();
        void SetPos(int x, int y);
        void RenderBackPane(int x, int y, int width, int height, const char* pszHeader);
        void RenderHpLevel(int x, int y, int width, int height, int level, const char* pszLabel);
        void LoadImages();
        void UnloadImages();

        CNewUIManager* m_pNewUIMng;

        POINT m_Pos;
        CNewUICheckBox m_BtnPreConHuntRange;
        CNewUICheckBox m_BtnPreConAttacking;

        CNewUICheckBox m_BtnSubConMoreThanTwo;
        CNewUICheckBox m_BtnSubConMoreThanThree;
        CNewUICheckBox m_BtnSubConMoreThanFour;
        CNewUICheckBox m_BtnSubConMoreThanFive;

        CNewUICheckBox m_BtnPartyHeal;
        CNewUICheckBox m_BtnPartyDuration;
        CUITextInputBox m_BuffTimeInput;

        CNewUIButton m_BtnSave;
        CNewUIButton m_BtnReset;
        CNewUIButton m_BtnClose;

        int m_iCurrentPage;
        int m_iCurrentHealThreshold;
        int m_iCurrentPartyHealThreshold;
        int m_iCurrentPotionThreshold;
    };

}
