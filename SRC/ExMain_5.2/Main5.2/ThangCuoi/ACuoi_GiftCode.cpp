#include "Stdafx.h"
#include "ACuoi_GiftCode.h"
#include "WSclientinline.h"
#include "ZzzToolKit.h"
#include "WideData.h"

/// ------------------------------------------------------------------------
/// Class xử lý UI GiftCode (Season3B)
/// - Cho phép nhập GiftCode để gửi về GS
/// - UI gồm: khung, textbox, nút xác nhận
/// - Đã loại bỏ cờ cảnh báo không dùng
/// ------------------------------------------------------------------------
SEASON3B::CNewUIGiftCode::CNewUIGiftCode()
{
	m_pNewUIMng = nullptr;
	m_pGiftCodeInput = nullptr; // chỉ cần 1 textbox nhập GiftCode

	m_Pos.x = 0;
	m_Pos.y = 0;

	WINDOW_WIDTH = 380;
	WINDOW_HEIGHT = 190;
}

SEASON3B::CNewUIGiftCode::~CNewUIGiftCode()
{
	Release(); // Giải phóng khi huỷ đối tượng
}

bool SEASON3B::CNewUIGiftCode::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (pNewUIMng == nullptr)
		return false;

	// Gắn UI manager
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_GIFTCODE, this);

	SetPos(x, y);
	InitButtons();
	LoadImages();
	// Khởi tạo ô nhập GiftCode
	m_pGiftCodeInput = new CUITextInputBox;
	m_pGiftCodeInput->Init(g_hWnd, 80, 10, 10, false); // false = không ẩn text
	m_pGiftCodeInput->SetPosition(m_Pos.x + 165, m_Pos.y + 192);
	m_pGiftCodeInput->SetTextColor(255, 255, 0, 255);
	m_pGiftCodeInput->SetBackColor(0, 0, 0, 128);     // nền tối mờ, dễ nhìn
	m_pGiftCodeInput->SetFont(g_hFont);
	m_pGiftCodeInput->SetState(UISTATE_NORMAL);

	Show(false);
	return true;
}

void SEASON3B::CNewUIGiftCode::Release()
{
	// Giải phóng textbox
	SAFE_DELETE(m_pGiftCodeInput);

	// Hủy liên kết với UI manager
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = nullptr;
	}
}

void SEASON3B::CNewUIGiftCode::ClearTextBoxes()
{
	// Reset text nhập về rỗng
	if (m_pGiftCodeInput)
		m_pGiftCodeInput->SetText("");
}

void SEASON3B::CNewUIGiftCode::SetPos(int x, int y)
{
	m_Pos.x = (IsToolKit.GetPositionScreen() - WINDOW_WIDTH) / 2;
	m_Pos.y = (IsToolKit.GetCreatePosHeight() - (WINDOW_HEIGHT + 70.0f)) / 2;
}

bool SEASON3B::CNewUIGiftCode::Update()
{
	if (!IsVisible())
		return true;

	// Cập nhật trạng thái input box
	if (m_pGiftCodeInput)
		m_pGiftCodeInput->DoAction();

	return true;
}

bool SEASON3B::CNewUIGiftCode::UpdateMouseEvent()
{
	if (!IsVisible())
	{
		// Nếu UI bị ẩn nhưng textbox còn giữ focus -> reset về cửa sổ game
		if (m_pGiftCodeInput && m_pGiftCodeInput->HaveFocus())
			SetFocus(g_hWnd);
		return false;
	}

	if (m_Btn[0].UpdateMouseEvent()) // Nút xác nhận
	{
		char GiftCode[11] = { 0 };

		if (m_pGiftCodeInput)
			m_pGiftCodeInput->GetText(GiftCode);

		if (strlen(GiftCode) > 0)
		{
			char zChat[64];
			sprintf(zChat, "/code %s", GiftCode);

			// Gửi qua hàm chuẩn
			SendChat(zChat);
			// Reset UI
			m_pGiftCodeInput->SetText("");
			if (m_pGiftCodeInput->HaveFocus())SetFocus(g_hWnd);g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_GIFTCODE);
			PlayBuffer(SOUND_CLICK01);

		}
		return true;
	}
	// Xử lý nút thoát (dấu X)
	if (SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn((m_Pos.x + WINDOW_WIDTH) - 40, m_Pos.y + 5, 16, 16))
	{
		if (m_pGiftCodeInput && m_pGiftCodeInput->HaveFocus())
			SetFocus(g_hWnd);

		g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_GIFTCODE);
	}

	// Nếu click trong vùng UI thì không cho click xuyên
	if (CheckMouseIn(m_Pos.x, m_Pos.y, WINDOW_WIDTH - 10, WINDOW_HEIGHT + 70))
		return false;

	return true;
}

bool SEASON3B::CNewUIGiftCode::UpdateKeyEvent()
{
	if (IsVisible())
	{
		// Nhấn ESC để đóng
		if (SEASON3B::IsPress(VK_ESCAPE))
		{
			if (m_pGiftCodeInput && m_pGiftCodeInput->HaveFocus())
				SetFocus(g_hWnd);

			g_pNewUIMenuOption->Hide(SEASON3B::INTERFACE_GIFTCODE);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

float SEASON3B::CNewUIGiftCode::GetLayerDepth()
{
	return 3.4f;
}

float SEASON3B::CNewUIGiftCode::GetKeyEventOrder()
{
	return 3.4f;
}

void SEASON3B::CNewUIGiftCode::LoadImages()
{
	LoadBitmap("Interface\\ACuoi\\Gifcode.tga", IMAGE_HD_LOGO_0, GL_LINEAR);
}

bool SEASON3B::CNewUIGiftCode::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pUIForm->NewRenderForm(m_Pos.x, m_Pos.y, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, "Giftcode Tân Thủ");
	//g_pUIForm->RenderBack(m_Pos.x + 20, m_Pos.y + 40, 257.f, 25);
	//g_pUIForm->RenderBack(m_Pos.x + 5, m_Pos.y + 10, 257.f, 260);

	RenderBitmap((SEASON3B::CNewUIGiftCode::IMAGE_HD_LOGO_0), 252,127, 588, 322, 0.f, 0.f, 1.0, 128.f / 128.f, 1, 1, 0);

	IsToolKit.ThisFont(m_Pos.x, m_Pos.y + 30, 0xFFDE26FF, 0, 360, 0, RT3_SORT_CENTER, "Để Trống Hòm Đồ Trước Khi Nhận Code");

	m_Btn[0].Render();
	// Render nút bấm
	m_Btn[0].Render();

	// Render textbox nhập liệu
	if (m_pGiftCodeInput)
		m_pGiftCodeInput->Render();

	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUIGiftCode::InitButtons()
{
	// Nút xác nhận GiftCode
	g_pUIForm->SetButtonInfo(&m_Btn[0],CNewUIGiftCode::IMAGE_IGS_BUTTON,m_Pos.x + 165,m_Pos.y + 212,52, 26,1, 0, 1, 1u,"Nhận Code", "", 0
	);
}
