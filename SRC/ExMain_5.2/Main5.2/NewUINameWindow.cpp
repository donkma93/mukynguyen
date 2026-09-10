#include "stdafx.h"
#include "NewUINameWindow.h"
#include "ZzzBmd.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "UIControls.h"
#include "CSChaosCastle.h"
#include "PersonalShopTitleImp.h"
#include "MatchEvent.h"
#include "MapManager.h"
#include <ThangCuoi/StructSendGs.h>
#include <NewUISystem.h>


using namespace SEASON3B;

SEASON3B::CNewUINameWindow::CNewUINameWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;

	m_bShowItemName = false;
}

SEASON3B::CNewUINameWindow::~CNewUINameWindow()
{
	Release();
}

bool SEASON3B::CNewUINameWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_NAME_WINDOW, this);

	SetPos(x, y);

	Show(true);

	return true;
}

void SEASON3B::CNewUINameWindow::Release()
{
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}
		
void SEASON3B::CNewUINameWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUINameWindow::UpdateMouseEvent()
{
	return true;
}

bool SEASON3B::CNewUINameWindow::UpdateKeyEvent()
{
	if(SEASON3B::IsPress(VK_MENU) == true)
	{
		m_bShowItemName = !m_bShowItemName;
	}

	return true;
}

bool SEASON3B::CNewUINameWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUINameWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderName();
    RenderTimes();
    matchEvent::RenderMatchTimes ();
    RenderBooleans();
	DrawPersonalShopTitleImp();
#if WinQuestTurn
	g_ExWinQuestSystem.DrawMiniInfo();
#endif
	DisableAlphaBlend();
	return true;
}
//ACuoi Show name
void SEASON3B::CNewUINameWindow::RenderName()
{
	if(g_bGMObservation == false)
	{
		for(int i=0; i<MAX_CHARACTERS_CLIENT; i++)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if(o->Live && o->Kind == KIND_PLAYER)
			{
				if(IsShopTitleVisible(c) == false)
				{
					CreateChat(c->ID,"",c);
				}
			}
			//else if (o->Live && o->Kind == KIND_MONSTER)
			//{
			//	CreateChat(c->ID, "", c);
			//}
		}
	}
	
    if(gMapManager.InChaosCastle() == true)
    {
        if(FindText(Hero->ID, "webzen") == false)
        {
            if(SelectedNpc != -1 || SelectedCharacter != -1 )
			{
				return;
			}
        }
    }

	if(SelectedItem != -1 || SelectedNpc != -1 || SelectedCharacter != -1)
	{
		if(SelectedNpc != -1)
		{
			CHARACTER *c = &CharactersClient[SelectedNpc];
			OBJECT *o = &c->Object;
			CreateChat(c->ID,"",c);
		}
		else if(SelectedCharacter != -1)
		{
			CHARACTER *c = &CharactersClient[SelectedCharacter];

			OBJECT *o = &c->Object;
			// Render Name Monster Red
			/*
			if(o->Kind == KIND_MONSTER)
			{
				g_pRenderText->SetTextColor(255, 230, 200, 255);
				g_pRenderText->SetBgColor(100, 0, 0, 255);
				g_pRenderText->RenderText(320, 2, c->ID, 0 ,0, RT3_WRITE_CENTER);
			}
			else
			{
				if(IsShopTitleVisible(c) == false)
				{
					CreateChat(c->ID,"",c);
				}
			}
			*/

			if (o->Kind != KIND_MONSTER)
			{
				if (IsShopTitleVisible(c) == false)
				{
					CreateChat(c->ID, "", c);
				}
			}
		}
		//ACuoi Render item info drop
		else if (SelectedItem != -1)
		{
			if (Items[SelectedItem].Item.Type == ITEM_POTION + 15)
			{
				RenderItemName(SelectedItem, &Items[SelectedItem].Object, Items[SelectedItem].Item.Level, Items[SelectedItem].Item.Option1, Items[SelectedItem].Item.ExtOption, true);
			}
			else
			{
				ItemConvert(&Items[SelectedItem].Item, Items[SelectedItem].Item.Level, Items[SelectedItem].Item.Option1, Items[SelectedItem].Item.ExtOption);
				RenderItemInfo(MouseX, MouseY, &Items[SelectedItem].Item, false);
			}
		}
		//else if(SelectedItem != -1)
		//{
		//	RenderItemName(SelectedItem,&Items[SelectedItem].Object,Items[SelectedItem].Item.Level,Items[SelectedItem].Item.Option1,Items[SelectedItem].Item.ExtOption,false);
		//}
	}

	if(m_bShowItemName || SEASON3B::IsRepeat(VK_MENU))
	{
		for(int i=0;i<MAX_ITEMS;i++)
		{
			OBJECT *o = &Items[i].Object;
			if(o->Live)
			{
				if(o->Visible && i!=SelectedItem)
				{
					RenderItemName(i,o,Items[i].Item.Level,Items[i].Item.Option1,Items[i].Item.ExtOption,true);
				}
			}
		}
	}	
}

float SEASON3B::CNewUINameWindow::GetLayerDepth()
{
	return 1.0f;
}

