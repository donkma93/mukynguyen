#pragma once
#include <array>
#include <vector>
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "MUHelper/MuHelper.h"
#include <_TextureIndex.h>
#include <NewUI3DRenderMng.h>

namespace SEASON3B
{
    class CNewUIMuHelperSkillList : public CNewUIObj
    {
        public:
        CNewUIMuHelperSkillList();
        ~CNewUIMuHelperSkillList();

        bool Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng);
        void Release();

        bool UpdateMouseEvent();
        bool UpdateKeyEvent();
        bool Update();
        bool Render();
        void RenderSkillInfo();
        float GetLayerDepth();

        void Reset();

        int UpdateMouseSkillList();
        void FilterByAttackSkills();
        void FilterByBuffSkills();
        POINT SkillListPos;
        static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);

    private:
        enum IMAGE_LIST
        {
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

        enum EVENT_STATE
        {
            EVENT_NONE = 0,

            EVENT_BTN_HOVER_SKILLLIST,
            EVENT_BTN_DOWN_SKILLLIST,
        };

        typedef struct
        {
            int skillId;
            POINT location;
            SIZE area;
            bool isVisible;
        } cSkillIcon;

        void LoadImages();
        void UnloadImages();

        void PrepareSkillsToRender();
        void RenderSkillIcon(int iIndex, float x, float y, float width, float height);

        bool IsAttackSkill(int iSkillType);
        bool IsBuffSkill(int iSkillType);
        bool IsHealingSkill(int iSkillType);
        bool IsDefenseSkill(int iSkillType);

        std::map<int, cSkillIcon> m_skillIconMap;

        CNewUIManager* m_pNewUIMng;
        CNewUI3DRenderMng* m_pNewUI3DRenderMng;

        bool m_bFilterByAttackSkills;
        bool m_bFilterByBuffSkills;
        bool m_bRenderSkillInfo;
        int m_iRenderSkillInfoType;
        int m_iRenderSkillInfoPosX;
        int m_iRenderSkillInfoPosY;
        std::vector<int> m_aiSkillsToRender;

        EVENT_STATE m_EventState;
    };
}
