#include "Stdafx.h"
#include "ACuoi_MocNap.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

InfoClientRecvSendData g_dataclient;
ItemListClienRecvSend g_mocapclient;

SEASON3B::CNewUIScrollBar* ListMocNap = nullptr;
SEASON3B::CNewUIScrollBar* ListItemNhan = nullptr;

int MaxListItemInPage = 6;

SEASON3B::CNewUIMocNap::CNewUIMocNap()
{
    this->m_pNewUIMng = nullptr;
    this->m_Pos = { 0, 0 };
	IsToolKit.ViewIndex = 0;
}

SEASON3B::CNewUIMocNap::~CNewUIMocNap()
{
	Release();
}

bool SEASON3B::CNewUIMocNap::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MOCNAP, this);

	SetPos(x, y);

	Show(false);

	return true;
}

void SEASON3B::CNewUIMocNap::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIMocNap::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

bool SEASON3B::CNewUIMocNap::Update()
{
	return true;
}

bool SEASON3B::CNewUIMocNap::UpdateMouseEvent()
{
	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 40, m_Pos.y + 5, 16, 16))
	{
		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_MOCNAP);
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH - 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUIMocNap::UpdateKeyEvent()
{
	if (IsVisible())
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_MOCNAP);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIMocNap::GetLayerDepth()
{
	return 3.4;
}

float SEASON3B::CNewUIMocNap::GetKeyEventOrder()
{
	return 3.4;
}

bool SEASON3B::CNewUIMocNap::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, WINDOW_WIDTH, WINDOW_HEIGHT, GlobalText[1277]);

	int TongSoDaNap = g_dataclient.TongNap;
	int MocDaNhan = g_dataclient.NhanMocNap;

	g_pUIForm->RenderBack(m_Pos.x + 8, m_Pos.y + 35, int(WINDOW_WIDTH - 33), 20);
	IsToolKit.ThisFont(m_Pos.x + 10, m_Pos.y + 39, 0xFFEECCE1, 0, 440, 0, RT3_SORT_CENTER, GlobalText[875], IsToolKit.QN(TongSoDaNap));

	g_pUIForm->RenderBack(m_Pos.x + 8, m_Pos.y + 55, int(WINDOW_WIDTH - 300), 150);
	
	g_pUIForm->RenderBack(m_Pos.x + 186, m_Pos.y + 55, int(WINDOW_WIDTH - 374), 150);
	g_pUIForm->RenderBack(m_Pos.x + 292, m_Pos.y + 55, int(WINDOW_WIDTH - 317), 150);

	RenderBitmap(SEASON3B::CNewUIJewelBank::IMAGE_BASE_BANK_BOX, m_Pos.x + 296, m_Pos.y + 57, 153, 141, 0.f, (320.f / 256.f) * 0, 1.0, 80.f / 128.f, 1, 1, 0);
	
	int DataListMocNap = g_dataclient.DanhSachMocNap.size();
	int MaxLineData = 10;

	if (ListMocNap == NULL)
	{
		ListMocNap = new CNewUIScrollBar();
		ListMocNap->Create(m_Pos.x + 175, m_Pos.y + 57, 141);
	}
	if (ListMocNap)
	{
		int maxPos = (DataListMocNap > MaxLineData) ? (DataListMocNap - MaxLineData) : 0;
		ListMocNap->SetMaxPos(maxPos);

		ListMocNap->MouseWheelWindow = CheckMouseIn(m_Pos.x + 175, m_Pos.y + 60, int(WINDOW_WIDTH - 300), 141);
		ListMocNap->Render();
		ListMocNap->UpdateMouseEvent();
		ListMocNap->Update();
	}

	int MixItemListPage = ListMocNap->GetCurPos();
	int MaxList = 0;
	
	if (g_dataclient.DanhSachMocNap.size() == 0)
	{
		IsToolKit.ThisFont(m_Pos.x, m_Pos.y + 10, HEX_COLOR_WHITE, 0, 144, 0, 3, GlobalText[3178]);
	}
	else
	{
		for (int i = MixItemListPage; i < g_dataclient.DanhSachMocNap.size(); i++)
		{
			if (MaxList >= MaxLineData)
				break;

			float TyleNap = (96.0 * TongSoDaNap) / g_dataclient.DanhSachMocNap[i].GiaTriNap;
			if (TyleNap > 96.0)
			{
				TyleNap = 96.0;
			}
			double Tyle = (static_cast<double>(TongSoDaNap) / g_dataclient.DanhSachMocNap[i].GiaTriNap) * 100;

			int PosY1 = m_Pos.y + 58 + (MaxList * 14.3);
			int PosY2 = m_Pos.y + 60 + (MaxList * 14.3);

			IsToolKit.ThisFont(m_Pos.x + 12, PosY1, 0, 0x000000FF, 160, 12, 1, " ");
			IsToolKit.ThisFont(m_Pos.x + 12, PosY1, 0, 0xCA0E0E69, TyleNap, 12, 1, " ");
			
			if (CheckMouseIn(m_Pos.x + 12, PosY1, 96, 12) == 1)
			{
				IsToolKit.ThisFont(m_Pos.x + 12, PosY1, 0, 0x0064B4FF, 96, 12, 1, " ");

				if (SEASON3B::IsRelease(VK_LBUTTON))
				{
					IsToolKit.KeySendStateViewMocNap(i);
				}
			}

			IsToolKit.ThisFont(m_Pos.x + 28, PosY2 - 2, HEX_COLOR_WHITE, 0, 90, 15, 1, GlobalText[876], IsToolKit.QN(g_dataclient.DanhSachMocNap[i].GiaTriNap));

			if (MocDaNhan < g_dataclient.DanhSachMocNap[i].IndexMocNap)
			{
				if (CheckMouseIn(m_Pos.x + 108, PosY2, 64, 12) == 1)
				{
					IsToolKit.ThisFont(m_Pos.x + 108, PosY1, 0, 0x0064B4FF, 64, 12, 1, " ");

					if (SEASON3B::IsRelease(VK_LBUTTON) && Tyle >= 100)
					{
						IsToolKit.KeySendStateMocNap(i);
					}
				}

				if (Tyle < 100)
				{
					IsToolKit.ThisFont(m_Pos.x + 108, PosY2, HEX_COLOR_WHITE, 0, 64, 0, 3, GlobalText[1249], Tyle);
				}
				else
				{
					IsToolKit.ThisFont(m_Pos.x + 108, PosY2, HEX_COLOR_WHITE, 0, 64, 0, 3, GlobalText[1247]);
				}
			}
			else
			{
				IsToolKit.ThisFont(m_Pos.x + 108, PosY2, HEX_COLOR_WHITE, 0, 64, 0, 3, GlobalText[1248]);
			}
			
			MaxList++;
			if (MaxList >= MaxLineData) break;
		}
	}


	int DataListItem = g_mocapclient.g_listmocnap.size();
	int MaxLineRecv = 6;

	if (ListItemNhan == NULL)
	{
		ListItemNhan = new CNewUIScrollBar();
		ListItemNhan->Create(m_Pos.x + 280, m_Pos.y + 57, 141);
	}
	if (ListItemNhan)
	{
		int maxPos = (DataListItem > MaxLineRecv) ? (DataListItem - MaxLineRecv) : 0;
		ListItemNhan->SetMaxPos(maxPos);

		ListItemNhan->MouseWheelWindow = CheckMouseIn(m_Pos.x + 280, m_Pos.y + 60, int(WINDOW_WIDTH - 300), 141);
		ListItemNhan->Render();
		ListItemNhan->UpdateMouseEvent();
		ListItemNhan->Update();
	}

	float PosXBoxItem = m_Pos.x + 190;
	float PosYBoxItem = m_Pos.y + 55;

	float KhoangCach = 40;
	float PosXBoxItemGoc = m_Pos.x + 190;
	int CountNgang = 0;
	int CountDoc = 0;
	int ItemListPage = ListItemNhan->GetCurPos();
	int RenderInfoItem = -1;

	for (int n = ItemListPage; n < g_mocapclient.g_listmocnap.size(); n++)
	{
		if (CheckMouseIn(PosXBoxItem + 4, PosYBoxItem + 6, 40, 40) == 1)
		{
			IsToolKit.ThisFont(PosXBoxItem + 4, PosYBoxItem + 6, 0, 0x00A0E8FF, 40, 40, 1, " ");
		}

		IsToolKit.RenderItemSlot(PosXBoxItem + 3, PosYBoxItem + 5, 2, 2, 0);

		g_pNewUISystem->RenderItem3DFree(PosXBoxItem + 8, PosYBoxItem,
			30,
			30,
			g_mocapclient.g_listmocnap[n].Index,
			g_mocapclient.g_listmocnap[n].Item.Level,
			g_mocapclient.g_listmocnap[n].Item.Option1,
			g_mocapclient.g_listmocnap[n].Item.ExtOption,
			false, g_mocapclient.g_listmocnap[n].SizeBMD, true);

		if (CheckMouseIn(PosXBoxItem + 4, PosYBoxItem + 6, 40, 40) == 1)
		{
			RenderInfoItem = n;
		}

		PosXBoxItem += KhoangCach;
		CountDoc++;
		CountNgang++;
		if (CountNgang >= 2)
		{
			PosXBoxItem = PosXBoxItemGoc;
			PosYBoxItem += KhoangCach;
			CountNgang = 0;
		}
		if (CountDoc >= MaxListItemInPage) break;
	}

	if (RenderInfoItem != -1)
	{
		ITEM tempItem;
		memset(&tempItem, 0, sizeof(ITEM));
		memcpy(&tempItem, &g_mocapclient.g_listmocnap[RenderInfoItem].Item, sizeof(tempItem));

		RenderItemInfo(MouseX, MouseY, &tempItem, false, 0, false, false);
	}

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	DisableAlphaBlend();
	return true;
}

void WsClientSend9A(const BYTE* SPK)
{
	g_dataclient.Clear();
	auto* mRecv = (WsClientMocNap*)SPK;
	g_dataclient.NhanMocNap = mRecv->NhanMocNap;
	g_dataclient.TongNap = mRecv->TongNap;
	for (int i = 0; i < mRecv->count; i++)
	{
		ListMocNapSend lpInfo = *(ListMocNapSend*)(((BYTE*)SPK) + sizeof(WsClientMocNap) + (sizeof(ListMocNapSend) * i));
		g_dataclient.DanhSachMocNap.push_back(lpInfo);
	}
}

void WsClientSend9B(BYTE* SPK)
{
	g_mocapclient.Clear();
	auto mRecv = reinterpret_cast<ClientListItemSend*>(SPK);
	g_mocapclient.WC = mRecv->WC;
	g_mocapclient.WP = mRecv->WP;
	g_mocapclient.GP = mRecv->GP;
	g_mocapclient.Ruud = mRecv->Ruud;
	g_mocapclient.CoinAtm = mRecv->CoinAtm;

	for (int i = 0; i < mRecv->count; i++)
	{
		auto lpInfo = reinterpret_cast<InfoListMocNapSend*>(reinterpret_cast<BYTE*>(SPK) + sizeof(ClientListItemSend) + (sizeof(InfoListMocNapSend) * i));

		InfoItemLocalSend infoItemLocal = { 0 };
		infoItemLocal.Count = lpInfo->Count;
		infoItemLocal.SizeBMD = lpInfo->SizeBMD;
		infoItemLocal.Index = lpInfo->Index;
		infoItemLocal.GetTypeItem = lpInfo->GetTypeItem;

		auto item = g_pNewItemMng->CreateItem(lpInfo->Item);
		if (item)
		{
			memcpy(&infoItemLocal.Item, item, sizeof(ITEM));
			infoItemLocal.Item.Durability = lpInfo->Dur;

			if (lpInfo->PeriodTime)
			{
				infoItemLocal.Item.bPeriodItem = 1;
				infoItemLocal.Item.lExpireTime = lpInfo->PeriodTime;
			}
			g_mocapclient.g_listmocnap.push_back(infoItemLocal);
		}
	}
}

void IsVisibleWindow()
{
	IsToolKit.KeySendStateOpenMocNap(0);
	IsToolKit.KeySendStateViewMocNap(IsToolKit.ViewIndex);
}