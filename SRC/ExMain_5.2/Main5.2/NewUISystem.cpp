
#include "stdafx.h"
#include "NewUISystem.h"
#include "NewUIMessageBox.h"
#include "wsclientinline.h"
#include "PersonalShopTitleImp.h"
#include "MapManager.h"
#include "./Utilities/Log/muConsoleDebug.h"

#include "ThangCuoi/WideData.h"
#include "ThangCuoi/ZzzToolKit.h"
using namespace SEASON3B;

CNewUISystem::CNewUISystem()
{
	m_pNewUI3DRenderMng = nullptr;
	m_pNewPurchaseShopInventory = nullptr;
	m_pNewNameWindow = nullptr;
	m_pNewMyQuestInfoWindow = nullptr;
	m_pNewKanturuInfoWindow = nullptr;
	m_pNewDuelWindow = nullptr;
	m_pNewDuelWatchMainFrameWindow = nullptr;
	m_pNewDuelWatchUserListWindow = nullptr;
	m_pNewUIMng = nullptr;
	m_pNewUIHotKey = nullptr;
	m_pNewChatLogWindow = nullptr;
	m_pNewSlideWindow = nullptr;
	m_pNewGuildMakeWindow = nullptr;
	m_pNewFriendWindow = nullptr;
	m_pNewMainFrameWindow = nullptr;
	m_pNewSkillList = nullptr;
	m_pNewChatInputBox = nullptr;
	m_pNewItemMng = nullptr;
	m_pNewMyInventory = nullptr;
	m_pNewMyInventoryExt = nullptr;
	m_pNewNPCShop = nullptr;
	m_pNewPetInfoWindow = nullptr;
	m_pNewMixInventory = nullptr;
	m_pNewCastleWindow = nullptr;
	m_pNewGuardWindow = nullptr;
	m_pNewGatemanWindow = nullptr;
	m_pNewGateSwitchWindow = nullptr;
	m_pNewStorageInventory = nullptr;
	m_pNewStorageInventoryExt = nullptr;
	m_pNewGuildInfoWindow = nullptr;
	m_pNewMyShopInventory = nullptr;
	m_pNewCharacterInfoWindow = nullptr;
	m_pNewPartyInfoWindow = nullptr;
	m_pNewPartyListWindow = nullptr;
	m_pNewNPCQuest = nullptr;
	m_pNewEnterBloodCastle = nullptr;
	m_pNewEnterDevilSquare = nullptr;
	m_pNewBloodCastle = nullptr;
	m_pNewTrade = nullptr;
	m_pNewKanturu2ndEnterNpc = nullptr;
	m_pNewCatapultWindow = nullptr;
	m_pNewChaosCastleTime = nullptr;
	m_pNewBattleSoccerScore = nullptr;
	m_pNewQuickCommandWindow = nullptr;
	m_pNewCommandWindow = nullptr;
	m_pNewWindowMenu = nullptr;
	m_pNewOptionWindow = nullptr;
	m_pNewMoveCommandWindow = nullptr;
	m_pNewHeroPositionInfo = nullptr;
	m_pNewHelpWindow = nullptr;
	m_pNewItemExplanationWindow = nullptr;
	m_pNewSetItemExplanation = nullptr;
	m_pNewSiegeWarfare = nullptr;
	m_pNewItemEnduranceInfo = nullptr;
	m_pNewBuffWindow = nullptr;
	m_pNewCryWolfInterface = nullptr;
	m_pNewMaster_SkillTree_Interface = nullptr;
	m_pNewCursedTempleResultWindow = nullptr;
	m_pNewCursedTempleWindow = nullptr;
	m_pNewCursedTempleEnterWindow = nullptr;
	m_pNewGoldBowman = nullptr;
	m_pNewGoldBowmanLena = nullptr;
	m_pNewLuckyCoinRegistration = nullptr;
	m_pNewExchangeLuckyCoinWindow = nullptr;
	m_pNewDuelWatchWindow = nullptr;
	m_pNewInGameShop = nullptr;
	m_pNewDoppelGangerWindow = nullptr;
	m_pNewDoppelGangerFrame = nullptr;
	m_pNewNPCDialogue = nullptr;
	m_pNewQuestProgress = nullptr;
	m_pNewQuestProgressByEtc = nullptr;
	m_pNewEmpireGuardianNPC = nullptr;
	m_pNewEmpireGuardianTimer = nullptr;
	m_pNewMiniMap = nullptr;
#ifdef PBG_MOD_STAMINA_UI
	m_pNewUIStamina = NULL;
#endif //PBG_MOD_STAMINA_UI
	m_pNewGensRanking = nullptr;
	m_pNewUnitedMarketPlaceWindow = nullptr;

	m_pNewUIMuHelper = nullptr;
	m_pNewUIMuHelperConfig = nullptr;
	m_pNewUIMuHelperSkillList = nullptr;
	m_pNewUIMenuOption = nullptr;
	m_pNewUIRanking = nullptr;
	m_pNewUIJewelBank = nullptr;
	m_pNewUIPKSystem = nullptr;
	m_pNewUIEventTime = nullptr;
	m_pNewUIRelife = nullptr;
	m_pNewUIResetChange = nullptr;
	m_pNewUIDanhHieu = nullptr;
	m_pNewUIQuanHam = nullptr;
	m_pNewUITuLuyen = nullptr;
	m_pNewUIHonHoan = nullptr;
	m_pNewUIRenderDameBoss = nullptr;
	m_pNewUIRenderInvasion = nullptr;
	m_pNewUIRenderCoin = nullptr;
	m_pNewUIViewItem = nullptr;
	m_pNewUINewShop = nullptr;
	m_pNewUIAccountLevel = nullptr;
	m_pNewUIMainMap = nullptr;
	m_pNewUISpinPush = nullptr;
	m_pNewUIMocNap = nullptr;
	m_pNewUIChangeClass = nullptr;
	m_pNewUIChangePass = nullptr;
	m_pNewUIQQ = nullptr;
	m_pNewUICastleEvent = nullptr;
	m_pNewUIRenderColorItem = nullptr;
	m_pUIFormSystemRender = nullptr;
	m_pNewUIAutoReset = nullptr;
}

CNewUISystem::~CNewUISystem()
{
	Release();
}

bool CNewUISystem::Create()
{
	m_pNewUIMng = new CNewUIManager;

	m_pNewUI3DRenderMng = new CNewUI3DRenderMng;
	if (false == m_pNewUI3DRenderMng->Create(m_pNewUIMng))
		return false;

	m_pNewChatLogWindow = new CNewUIChatLogWindow;
	if (false == m_pNewChatLogWindow->Create(m_pNewUIMng, 0, GetWindowsY() - 50 - 47, 6))
		return false;

	m_pNewOptionWindow = new CNewUIOptionWindow;
	if (m_pNewOptionWindow->Create(m_pNewUIMng, (640 - 270) / 2, (430 - 266) / 2) == false)
	{
		return false;
	}

	m_pNewSlideWindow = new CNewUISlideWindow;
	if (m_pNewSlideWindow->Create(m_pNewUIMng) == false)
	{
		return false;
	}

	if (g_MessageBox->Create(m_pNewUIMng) == false)
		return false;

	return true;
}

void CNewUISystem::Release()
{
	UnloadMainSceneInterface();

	SAFE_DELETE(m_pNewSlideWindow);
	SAFE_DELETE(m_pNewOptionWindow);
	SAFE_DELETE(m_pNewChatLogWindow);
	SAFE_DELETE(m_pNewUI3DRenderMng);

	m_pNewUIMng->RemoveAllUIObjs();

	SAFE_DELETE(m_pNewUIMng);
}

bool CNewUISystem::LoadMainSceneInterface()
{
	g_MessageBox->Show(true);
	m_pNewChatLogWindow->Show(true);
	m_pNewSlideWindow->Show(true);

	m_pNewItemMng = new CNewUIItemMng;

	m_pNewChatInputBox = new CNewUIChatInputBox;

	if (false == m_pNewChatInputBox->Create(m_pNewUIMng, m_pNewChatLogWindow, 0, setPosDown(480 - 51 - 47)))
	{
		return false;
	}
	SetFocus(g_hWnd);

	m_pNewUIHotKey = new CNewUIHotKey;
	if (false == m_pNewUIHotKey->Create(m_pNewUIMng))
		return false;

	m_pUIFormSystemRender = new CUIFormSystemRender;
	if (m_pUIFormSystemRender->Create(m_pNewUIMng, 0, 0) == false)
		return false;

	m_pNewMainFrameWindow = new CNewUIMainFrameWindow;
	if (m_pNewMainFrameWindow->Create(m_pNewUIMng, m_pNewUI3DRenderMng) == false)
		return false;

	m_pNewSkillList = new CNewUISkillList;
	if (m_pNewSkillList->Create(m_pNewUIMng, m_pNewUI3DRenderMng) == false)
		return false;

	m_pNewFriendWindow = new CNewUIFriendWindow;
	if (m_pNewFriendWindow->Create(m_pNewUIMng) == false)
		return false;

	m_pNewMyInventory = new CNewUIMyInventory;
	if (false == m_pNewMyInventory->Create(m_pNewUIMng, m_pNewUI3DRenderMng, setPosRight(640 - 190), 0))
		return false;

	m_pNewMyInventoryExt = new CNewUIInventoryExtension;
	if (false == m_pNewMyInventoryExt->Create(m_pNewUIMng, setPosRight(640 - 190 * 2), 0))
		return false;

	m_pNewNPCShop = new CNewUINPCShop;
	if (false == m_pNewNPCShop->Create(m_pNewUIMng, setPosRight(640 - 190 * 2), 0))
		return false;

	m_pNewPetInfoWindow = new CNewUIPetInfoWindow;
	if (false == m_pNewPetInfoWindow->Create(m_pNewUIMng, setPosRight(640 - (190 * 2)), 0))
		return false;

	m_pNewMixInventory = new CNewUIMixInventory;
	if (m_pNewMixInventory->Create(m_pNewUIMng, setPosRight(260), 0) == false)
		return false;

	m_pNewCastleWindow = new CNewUICastleWindow;
	if (m_pNewCastleWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewGuardWindow = new CNewUIGuardWindow;
	if (m_pNewGuardWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewGatemanWindow = new CNewUIGatemanWindow;
	if (m_pNewGatemanWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewGateSwitchWindow = new CNewUIGateSwitchWindow;
	if (m_pNewGateSwitchWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewStorageInventory = new CNewUIStorageInventory;
	if (m_pNewStorageInventory->Create(m_pNewUIMng, setPosRight(260), 0) == false)
		return false;

	m_pNewStorageInventoryExt = new CNewUIStorageInventoryExt;
	if (m_pNewStorageInventoryExt->Create(m_pNewUIMng, setPosRight(260 - 190), 0) == false)
		return false;

	m_pNewGuildInfoWindow = new CNewUIGuildInfoWindow;
	if (m_pNewGuildInfoWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewGuildMakeWindow = new CNewUIGuildMakeWindow;
	if (m_pNewGuildMakeWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	CreatePersonalItemTable();

	m_pNewMyShopInventory = new CNewUIMyShopInventory;
	if (m_pNewMyShopInventory->Create(m_pNewUIMng, setPosRight(640 - 190 * 2), 0) == false)
		return false;

	m_pNewPurchaseShopInventory = new CNewUIPurchaseShopInventory;
	if (m_pNewPurchaseShopInventory->Create(m_pNewUIMng, setPosRight(640 - 190 * 2), 0) == false)
		return false;

	m_pNewCharacterInfoWindow = new CNewUICharacterInfoWindow;
	if (m_pNewCharacterInfoWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewMyQuestInfoWindow = new CNewUIMyQuestInfoWindow;
	if (m_pNewMyQuestInfoWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewPartyInfoWindow = new CNewUIPartyInfoWindow;
	if (m_pNewPartyInfoWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewPartyListWindow = new CNewUIPartyListWindow;
	if (m_pNewPartyListWindow->Create(m_pNewUIMng, setPosRight(640 - 79), 14) == false)
		return false;

	m_pNewNPCQuest = new CNewUINPCQuest;
	if (m_pNewNPCQuest->Create(m_pNewUIMng, m_pNewUI3DRenderMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewEnterBloodCastle = new CNewUIEnterBloodCastle;
	if (m_pNewEnterBloodCastle->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewEnterDevilSquare = new CNewUIEnterDevilSquare;
	if (m_pNewEnterDevilSquare->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewBloodCastle = new CNewUIBloodCastle;
	if (m_pNewBloodCastle->Create(m_pNewUIMng, setPosRight(640 - 127), setPosDown(480 - 132)) == false)
		return false;

	m_pNewTrade = new CNewUITrade;
	if (m_pNewTrade->Create(m_pNewUIMng, setPosRight(260), 0) == false)
		return false;

	m_pNewKanturu2ndEnterNpc = new CNewUIKanturu2ndEnterNpc;
	if (m_pNewKanturu2ndEnterNpc->Create(m_pNewUIMng, setPosCenterX(230), 20) == false)
	{
		return false;
	}

	m_pNewKanturuInfoWindow = new CNewUIKanturuInfoWindow;
	if (m_pNewKanturuInfoWindow->Create(m_pNewUIMng, setPosRight(541), setPosDown(351)) == false)
	{
		return false;
	}

	m_pNewChaosCastleTime = new CNewUIChaosCastleTime;
	if (m_pNewChaosCastleTime->Create(m_pNewUIMng, setPosRight(640 - 127), setPosDown(480 - 132)) == false)
		return false;

	m_pNewBattleSoccerScore = new CNewUIBattleSoccerScore;
	if (m_pNewBattleSoccerScore->Create(m_pNewUIMng, setPosRight(509), setPosDown(359)) == false)
		return false;

	m_pNewCommandWindow = new CNewUICommandWindow;
	if (m_pNewCommandWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewCatapultWindow = new CNewUICatapultWindow;
	if (m_pNewCatapultWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
	{
		return false;
	}

	m_pNewWindowMenu = new CNewUIWindowMenu;
	if (m_pNewWindowMenu->Create(m_pNewUIMng, setPosDown(640 - 112), setPosDown(480 - 196)) == false)
	{
		return false;
	}

	m_pNewMoveCommandWindow = new CNewUIMoveCommandWindow;//

	if (m_pNewMoveCommandWindow->Create(m_pNewUIMng, 1, 1) == false)
		return false;

	m_pNewQuickCommandWindow = new CNewUIQuickCommandWindow;
	if (m_pNewQuickCommandWindow->Create(m_pNewUIMng, 0, 0) == false)
	{
		return false;
	}

	m_pNewHeroPositionInfo = new CNewUIHeroPositionInfo;
	if (m_pNewHeroPositionInfo->Create(m_pNewUIMng, 0, 0) == false)
	{
		return false;
	}

	m_pNewHelpWindow = new CNewUIHelpWindow;
	if (m_pNewHelpWindow->Create(m_pNewUIMng, 0, 0) == false)
	{
		return false;
	}

	m_pNewItemExplanationWindow = new CNewUIItemExplanationWindow;
	if (m_pNewItemExplanationWindow->Create(m_pNewUIMng, 0, 0) == false)
	{
		return false;
	}

	m_pNewSetItemExplanation = new CNewUISetItemExplanation;
	if (m_pNewSetItemExplanation->Create(m_pNewUIMng, 0, 0) == false)
	{
		return false;
	}
	
	m_pNewDuelWindow = new CNewUIDuelWindow;
	if (m_pNewDuelWindow->Create(m_pNewUIMng, setPosRight(509), setPosDown(359)) == false)
	{
		return false;
	}

	m_pNewNameWindow = new CNewUINameWindow;
	if (m_pNewNameWindow->Create(m_pNewUIMng, 0, 0) == false)
	{
		return false;
	}

	m_pNewSiegeWarfare = new CNewUISiegeWarfare;
	if (m_pNewSiegeWarfare->Create(m_pNewUIMng, setPosRight(486), setPosDown(234)) == false)
		return false;

	m_pNewItemEnduranceInfo = new CNewUIItemEnduranceInfo;
	if (m_pNewItemEnduranceInfo->Create(m_pNewUIMng, 2, 26) == false)
	{
		return false;
	}

	m_pNewBuffWindow = new CNewUIBuffWindow;
	if (m_pNewBuffWindow->Create(m_pNewUIMng, 220, 15) == false)
	{
		return false;
	}

	m_pNewCursedTempleEnterWindow = new CNewUICursedTempleEnter;
	if (m_pNewCursedTempleEnterWindow->Create(m_pNewUIMng, setPosCenterX(230), setPosCenterY(252)) == false)
	{
		return false;
	}

	m_pNewCursedTempleWindow = new CNewUICursedTempleSystem;
	if (m_pNewCursedTempleWindow->Create(m_pNewUIMng, 0, 0) == false)
	{
		return false;
	}

	m_pNewCursedTempleResultWindow = new CNewUICursedTempleResult;
	if (m_pNewCursedTempleResultWindow->Create(m_pNewUIMng, setPosCenterX(230), setPosCenterY(282)) == false)
	{
		return false;
	}

	m_pNewCryWolfInterface = new CNewUICryWolf;
	if (m_pNewCryWolfInterface->Create(m_pNewUIMng, 0, 0) == false)
		return false;

#if(NEW_MASTER_SKILL_TREE)
	m_pNewMaster_SkillTree_Interface = new CNewUIMasterSkillTree;
	if (m_pNewMaster_SkillTree_Interface->Create() == false)
		return false;
#endif
	m_pNewMiniMap = new CNewUIMiniMap;
	if (m_pNewMiniMap->Create(m_pNewUIMng, 0, 0) == false)
		return false;

	m_pNewGoldBowman = new CNewUIGoldBowmanWindow;
	if (m_pNewGoldBowman->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewGoldBowmanLena = new CNewUIGoldBowmanLena;
	if (m_pNewGoldBowmanLena->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewLuckyCoinRegistration = new CNewUIRegistrationLuckyCoin;
	if (m_pNewLuckyCoinRegistration->Create(m_pNewUIMng, setPosRight(640 - (190 * 2)), 0) == false)
		return false;

	m_pNewExchangeLuckyCoinWindow = new CNewUIExchangeLuckyCoin;
	if (m_pNewExchangeLuckyCoinWindow->Create(m_pNewUIMng, setPosRight(640 - (190 * 2)), 0) == false)
		return false;

	m_pNewDuelWatchWindow = new CNewUIDuelWatchWindow;
	if (m_pNewDuelWatchWindow->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewDuelWatchMainFrameWindow = new CNewUIDuelWatchMainFrameWindow;
	if (m_pNewDuelWatchMainFrameWindow->Create(m_pNewUIMng, m_pNewUI3DRenderMng) == false)
		return false;

	m_pNewDuelWatchUserListWindow = new CNewUIDuelWatchUserListWindow;
	if (m_pNewDuelWatchUserListWindow->Create(m_pNewUIMng, setPosRight(640 - 57), setPosDown(480 - 51)) == false)
		return false;

	m_pNewInGameShop = new CNewUIInGameShop;
	if (m_pNewInGameShop->Create(m_pNewUIMng, 0, 0) == false)
		return false;

	m_pNewDoppelGangerWindow = new CNewUIDoppelGangerWindow;
	if (m_pNewDoppelGangerWindow->Create(m_pNewUIMng, m_pNewUI3DRenderMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewDoppelGangerFrame = new CNewUIDoppelGangerFrame;
	if (m_pNewDoppelGangerFrame->Create(m_pNewUIMng, setPosRight(640 - 227), setPosDown(480 - 51 - 87)) == false)
		return false;

	m_pNewNPCDialogue = new CNewUINPCDialogue;
	if (m_pNewNPCDialogue->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewQuestProgress = new CNewUIQuestProgress;
	if (m_pNewQuestProgress->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewQuestProgressByEtc = new CNewUIQuestProgressByEtc;
	if (m_pNewQuestProgressByEtc->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewEmpireGuardianNPC = new CNewUIEmpireGuardianNPC;
	if (m_pNewEmpireGuardianNPC->Create(m_pNewUIMng, m_pNewUI3DRenderMng, setPosRight(450), 0) == false)
		return false;

	m_pNewEmpireGuardianTimer = new CNewUIEmpireGuardianTimer;
	if (m_pNewEmpireGuardianTimer->Create(m_pNewUIMng, setPosRight(507), setPosDown(342)) == false)
		return false;

#ifdef PBG_MOD_STAMINA_UI
	m_pNewUIStamina = new CNewUIStamina;
	if (m_pNewUIStamina->Create(m_pNewUIMng, 640, 480) == false)
		return false;
#endif //PBG_MOD_STAMINA_UI

	m_pNewGensRanking = new CNewUIGensRanking;
	if (m_pNewGensRanking->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewUnitedMarketPlaceWindow = new CNewUIUnitedMarketPlaceWindow;
	if (m_pNewUnitedMarketPlaceWindow->Create(m_pNewUIMng, m_pNewUI3DRenderMng, setPosRight(640 - 190), 0) == false)
		return false;

#ifdef LEM_ADD_LUCKYITEM
	m_pNewUILuckyItemWnd = new CNewUILuckyItemWnd;
	if (m_pNewUILuckyItemWnd->Create(m_pNewUIMng, 260, 0) == false)
		return false;
#endif // LEM_ADD_LUCKYITEM

	m_pNewUIMuHelper = new CNewUIMuHelper;
	if (m_pNewUIMuHelper->Create(m_pNewUIMng, setPosRight(640 - 190), 0) == false)
		return false;

	m_pNewUIMuHelperConfig = new CNewUIMuHelperConfig;
	if (m_pNewUIMuHelperConfig->Create(m_pNewUIMng, setPosRight(640 - 380), 0) == false)
		return false;

	m_pNewUIMuHelperSkillList = new CNewUIMuHelperSkillList;
	if (m_pNewUIMuHelperSkillList->Create(m_pNewUIMng, m_pNewUI3DRenderMng) == false)
		return false;

	m_pNewUIMenuOption = new CNewUIMenuOption;
	if (m_pNewUIMenuOption->Create(m_pNewUIMng, (640 - 270) / 2, 0) == false)
		return false;

	m_pNewUIRanking = new CNewUIRanking;
	if (m_pNewUIRanking->Create(m_pNewUIMng, 0, 0) == false)
		return false;

	m_pNewUIJewelBank = new CNewUIJewelBank;
	if (m_pNewUIJewelBank->Create(m_pNewUIMng, 90, 55) == false)
		return false;
	
	m_pNewUIMainMap = new CNewUIMainMap;
	if (m_pNewUIMainMap->Create(m_pNewUIMng, 90, 55) == false)
		return false;

	m_pNewUIPKSystem = new CNewUIPKSystem;
	if (m_pNewUIPKSystem->Create(m_pNewUIMng, 90, 55) == false)
		return false;

	m_pNewUIEventTime = new CNewUIEventTime;
	if (m_pNewUIEventTime->Create(m_pNewUIMng, 0, 0) == false)
		return false;

	m_pNewUIRelife = new CNewUIRelife;
	if (m_pNewUIRelife->Create(m_pNewUIMng, (640 - 190) / 2, 55) == false)
		return false;

	m_pNewUIResetChange = new CNewUIResetChange;
	if (m_pNewUIResetChange->Create(m_pNewUIMng, (640 - 270) / 2, 55) == false)
		return false;

	m_pNewUIGiftCode = new CNewUIGiftCode;
	if (m_pNewUIGiftCode->Create(m_pNewUIMng, (640 - 270) / 2, 55) == false)
		return false;


	m_pNewUIDanhHieu = new CNewUIDanhHieu;
	if (m_pNewUIDanhHieu->Create(m_pNewUIMng, (640 - 270) / 2, 55) == false)
		return false;

	m_pNewUIQuanHam = new CNewUIQuanHam;
	if (m_pNewUIQuanHam->Create(m_pNewUIMng, (640 - 270) / 2, 55) == false)
		return false;	

	m_pNewUITuLuyen = new CNewUITuLuyen;
	if (m_pNewUITuLuyen->Create(m_pNewUIMng, (640 - 270) / 2, 55) == false)
		return false;

	m_pNewUIHonHoan = new CNewUIHonHoan;
	if (m_pNewUIHonHoan->Create(m_pNewUIMng, (640 - 270) / 2, 0) == false)
		return false;

	m_pNewUIRenderDameBoss = new CNewUIRenderDameBoss;
	if (m_pNewUIRenderDameBoss->Create(m_pNewUIMng, (640 - 270) / 2, 55) == false)
		return false;

	m_pNewUIRenderInvasion = new CNewUIRenderInvasion;
	if (m_pNewUIRenderInvasion->Create(m_pNewUIMng, (640 - 144) / 2, 0) == false)
		return false;

	m_pNewUIRenderCoin = new CNewUIRenderCoin;
	if (m_pNewUIRenderCoin->Create(m_pNewUIMng, (640 - 144) / 2, 0) == false)
		return false;

	m_pNewUISpinPush = new CNewUISpinPush;
	if (m_pNewUISpinPush->Create(m_pNewUIMng, (640 - 270) / 2, 0) == false)
		return false;

	m_pNewUIViewItem = new CNewUIViewItem;
	if (m_pNewUIViewItem->Create(m_pNewUIMng, m_pNewUI3DRenderMng,(640 - 270) / 2, 0) == false)
		return false;

	m_pNewUIAccountLevel = new CNewUIAccountLevel;
	if (m_pNewUIAccountLevel->Create(m_pNewUIMng, 0, 0) == false)
		return false;

	m_pNewUIMocNap = new CNewUIMocNap;
	if (m_pNewUIMocNap->Create(m_pNewUIMng, (640 - 460) / 2, 39) == false)
		return false;

	m_pNewUINewShop = new CNewUINewShop;
	if (m_pNewUINewShop->Create(m_pNewUIMng, 0, 0) == false)
		return false;

	m_pNewUIChangeClass = new CNewUIChangeClass;
	if (m_pNewUIChangeClass->Create(m_pNewUIMng, (640 - 270) / 2, 0) == false)
		return false;

	m_pNewUIChangePass = new CNewUIChangePass;
	if (m_pNewUIChangePass->Create(m_pNewUIMng, (640 - 190) / 2, 0) == false)
		return false;

	m_pNewUIQQ = new CNewUIQQ;
	if (m_pNewUIQQ->Create(m_pNewUIMng, (640 - 270) / 2, 0) == false)
		return false;

	m_pNewUICastleEvent = new CNewUICastleEvent;
	if (m_pNewUICastleEvent->Create(m_pNewUIMng, (640 - 270) / 2, 0) == false)
		return false;

	m_pNewUIRenderColorItem = new CNewUIRenderColorItem;
	if (m_pNewUIRenderColorItem->Create(m_pNewUIMng, (640 - 270) / 2, 0) == false)
		return false;

	m_pNewUIAutoReset = new CNewUIAutoReset;
	if (m_pNewUIAutoReset->Create(m_pNewUIMng, (640 - 270) / 2, 55) == false)
		return false;
	return true;
}

void CNewUISystem::UnloadMainSceneInterface()
{
	if (g_pNewUIMng)
	{
		g_pNewUIMng->ShowAllInterfaces(false);
	}

	SAFE_DELETE(m_pNewHelpWindow);
	SAFE_DELETE(m_pNewItemExplanationWindow);
	SAFE_DELETE(m_pNewSetItemExplanation);
	SAFE_DELETE(m_pNewQuickCommandWindow);
	SAFE_DELETE(m_pNewWindowMenu);
	SAFE_DELETE(m_pNewBattleSoccerScore);
	SAFE_DELETE(m_pNewCatapultWindow);
	SAFE_DELETE(m_pNewKanturu2ndEnterNpc);
	SAFE_DELETE(m_pNewKanturuInfoWindow);
	SAFE_DELETE(m_pNewTrade);
	SAFE_DELETE(m_pNewNPCQuest);
	SAFE_DELETE(m_pNewMyQuestInfoWindow);
	SAFE_DELETE(m_pNewCharacterInfoWindow);
	SAFE_DELETE(m_pNewPurchaseShopInventory);
	SAFE_DELETE(m_pNewMyShopInventory);
	SAFE_DELETE(m_pNewGuildMakeWindow);
	SAFE_DELETE(m_pNewGuildInfoWindow);
	SAFE_DELETE(m_pNewStorageInventory);
	SAFE_DELETE(m_pNewMixInventory);
	SAFE_DELETE(m_pNewCastleWindow);
	SAFE_DELETE(m_pNewGuardWindow);
	SAFE_DELETE(m_pNewGatemanWindow);
	SAFE_DELETE(m_pNewGateSwitchWindow);
	SAFE_DELETE(m_pNewNPCShop);
	SAFE_DELETE(m_pNewPetInfoWindow);
	SAFE_DELETE(m_pNewMyInventory);
	SAFE_DELETE(m_pNewFriendWindow);
	SAFE_DELETE(m_pNewChatInputBox);
	SAFE_DELETE(m_pNewNameWindow);
	SAFE_DELETE(m_pNewSkillList);
	SAFE_DELETE(m_pNewMainFrameWindow);
	SAFE_DELETE(m_pNewPartyInfoWindow);
	SAFE_DELETE(m_pNewPartyListWindow);
	SAFE_DELETE(m_pNewEnterBloodCastle);
	SAFE_DELETE(m_pNewEnterDevilSquare);
	SAFE_DELETE(m_pNewBloodCastle);
	SAFE_DELETE(m_pNewChaosCastleTime);
	SAFE_DELETE(m_pNewCommandWindow);
	SAFE_DELETE(m_pNewMoveCommandWindow);
	SAFE_DELETE(m_pNewHeroPositionInfo);
	SAFE_DELETE(m_pNewUIHotKey);
	SAFE_DELETE(m_pNewSiegeWarfare);
	SAFE_DELETE(m_pNewItemEnduranceInfo);
	SAFE_DELETE(m_pNewBuffWindow);
	SAFE_DELETE(m_pNewCursedTempleResultWindow);
	SAFE_DELETE(m_pNewCursedTempleWindow);
	SAFE_DELETE(m_pNewCursedTempleEnterWindow);
	SAFE_DELETE(m_pNewCryWolfInterface);
#if(NEW_MASTER_SKILL_TREE)
	SAFE_DELETE(m_pNewMaster_SkillTree_Interface);
#endif
	SAFE_DELETE(m_pNewGoldBowman);
	SAFE_DELETE(m_pNewGoldBowmanLena);
	SAFE_DELETE(m_pNewLuckyCoinRegistration);
	SAFE_DELETE(m_pNewExchangeLuckyCoinWindow);
	SAFE_DELETE(m_pNewDuelWatchWindow);
	SAFE_DELETE(m_pNewDuelWindow);
	SAFE_DELETE(m_pNewDuelWatchMainFrameWindow);
	SAFE_DELETE(m_pNewDuelWatchUserListWindow);
	SAFE_DELETE(m_pNewInGameShop);
	SAFE_DELETE(m_pNewDoppelGangerWindow);
	SAFE_DELETE(m_pNewDoppelGangerFrame);
	SAFE_DELETE(m_pNewNPCDialogue);
	SAFE_DELETE(m_pNewQuestProgress);
	SAFE_DELETE(m_pNewQuestProgressByEtc);
	SAFE_DELETE(m_pNewEmpireGuardianNPC);
	SAFE_DELETE(m_pNewEmpireGuardianTimer);
	SAFE_DELETE(m_pNewMiniMap);
	SAFE_DELETE(m_pNewItemMng);
#ifdef PBG_MOD_STAMINA_UI
	SAFE_DELETE(m_pNewUIStamina);
#endif //PBG_MOD_STAMINA_UI
	SAFE_DELETE(m_pNewGensRanking);
	SAFE_DELETE(m_pNewUnitedMarketPlaceWindow);
#ifdef LEM_FIX_LUCKYITEM_UICLASS_SAFEDELETE
	SAFE_DELETE(m_pNewUILuckyItemWnd);
#endif // LEM_FIX_LUCKYITEM_UICLASS_SAFEDELETE

	SAFE_DELETE(m_pNewUIMuHelper);
	SAFE_DELETE(m_pNewUIMuHelperConfig);
	SAFE_DELETE(m_pNewUIMuHelperSkillList);
	SAFE_DELETE(m_pNewUIMenuOption);
	SAFE_DELETE(m_pNewUIRanking);
	SAFE_DELETE(m_pNewUIJewelBank);
	SAFE_DELETE(m_pNewUIPKSystem);
	SAFE_DELETE(m_pNewUIEventTime);
	SAFE_DELETE(m_pNewUIRelife);
	SAFE_DELETE(m_pNewUIResetChange);
	SAFE_DELETE(m_pNewUIDanhHieu);
	SAFE_DELETE(m_pNewUIQuanHam);
	SAFE_DELETE(m_pNewUITuLuyen);
	SAFE_DELETE(m_pNewUIHonHoan);
	SAFE_DELETE(m_pNewUIRenderDameBoss);
	SAFE_DELETE(m_pNewUIRenderInvasion);
	SAFE_DELETE(m_pNewUIRenderCoin);
	SAFE_DELETE(m_pNewUIViewItem);
	SAFE_DELETE(m_pNewUIAccountLevel);
	SAFE_DELETE(m_pNewUIMocNap);
	SAFE_DELETE(m_pNewUIQQ);
	SAFE_DELETE(m_pNewUIChangeClass);
	SAFE_DELETE(m_pNewUIChangePass);
	SAFE_DELETE(m_pNewUICastleEvent);
	SAFE_DELETE(m_pNewUIRenderColorItem);
	SAFE_DELETE(m_pUIFormSystemRender);
	SAFE_DELETE(m_pNewUINewShop);
	SAFE_DELETE(m_pNewUIAutoReset);

	ReleasePersonalItemTable();
}

bool CNewUISystem::IsVisible(DWORD dwKey)
{
	if (m_pNewUIMng)
	{
		return m_pNewUIMng->IsInterfaceVisible(dwKey);
	}

	return false;
}

void CNewUISystem::Show(DWORD dwKey)
{
	if (g_pInGameShop->IsInGameShop() || g_pMasterSkillTreeInterface->IsInMasterSkill())
		return;
	
	if (!m_pNewUIMng)
	{
		return;
	}

	if (dwKey == INTERFACE_FRIEND)
	{
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_FRIEND, true);

		m_pNewFriendWindow->OpenMainWnd(setPosRight(640 - 250), setPosDown(432 - 173));
	}
	else if (dwKey == INTERFACE_EVENTITME)
	{
		if (IsVisible(INTERFACE_JEWELBANK))
		{
			Hide(INTERFACE_JEWELBANK);
		}
		HideGroupBeforeOpenInterface();
	}
	else if (dwKey == INTERFACE_MUHELPER)
	{
		Hide(INTERFACE_INVENTORY);
		HideGroupBeforeOpenInterface();
		MUHelper::g_MuHelper.Stop();
	}
	else if (dwKey == INTERFACE_INVENTORY)
	{
		HideGroupBeforeOpenInterface();

		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, true);

		if (IsVisible(INTERFACE_CHARACTER))
		{
			g_pMyInventory->SetPos(setPosRight(640 - 190 * 2), 0);
		}
		if (IsVisible(INTERFACE_MYQUEST))
		{
			Hide(INTERFACE_MYQUEST);
		}
		g_pMyInventory->OpenningProcess();
	}
	else if (dwKey == INTERFACE_INVENTORY_EXT)
	{
		if (IsVisible(INTERFACE_STORAGE_EXT))
		{
			Hide(INTERFACE_STORAGE_EXT);
		}

		if (IsVisible(INTERFACE_STORAGE))
		{
			g_pStorageInventory->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
		}

		if (IsVisible(INTERFACE_TRADE))
		{
			g_pTrade->SetPos(setPosRight(640 - 190 * 3), 0);
		}

		if (IsVisible(INTERFACE_MYSHOP_INVENTORY))
		{
			g_pMyShopInventory->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
			if (IsVisible(INTERFACE_MYQUEST))
			{
				Hide(INTERFACE_MYQUEST);
			}

			if (IsVisible(INTERFACE_CHARACTER))
			{
				Hide(INTERFACE_CHARACTER);
			}
		}
		if (IsVisible(INTERFACE_NPCSHOP))
		{
			g_pNPCShop->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
		}
		if (IsVisible(INTERFACE_MIXINVENTORY))
		{
			g_pMixInventory->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
			if (IsVisible(INTERFACE_MYQUEST))
			{
				Hide(INTERFACE_MYQUEST);
			}

			if (IsVisible(INTERFACE_CHARACTER))
			{
				Hide(INTERFACE_CHARACTER);
			}
		}
		else if (IsVisible(INTERFACE_MYQUEST))
		{
			Hide(INTERFACE_MYQUEST);
		}
		else if (IsVisible(INTERFACE_CHARACTER))
		{
			Hide(INTERFACE_CHARACTER);
		}

		else if (IsVisible(INTERFACE_TRADE))
		{
			g_pTrade->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
		}
	}
	else if (dwKey == INTERFACE_CHARACTER)
	{
		HideGroupBeforeOpenInterface();
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_CHAINFO, true);

		if (IsVisible(INTERFACE_INVENTORY))
		{
			if (IsVisible(INTERFACE_INVENTORY_EXT))
			{
				g_pMyInventory->SetPos(setPosRight(640 - 190 * 3), 0);
			}
			else
			{
				g_pMyInventory->SetPos(setPosRight(640 - 190 * 2), 0);
			}
		}

		else if (IsVisible(INTERFACE_MYQUEST))
		{
			g_pMyQuestInfoWindow->SetPos(setPosRight(640 - 190 * 2), 0);
		}
		g_pCharacterInfoWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_PET)
	{
		if (IsVisible(INTERFACE_INVENTORY))
		{
			Hide(INTERFACE_INVENTORY);
		}
		if (IsVisible(INTERFACE_MYQUEST))
		{
			Hide(INTERFACE_MYQUEST);
		}

		HideGroupBeforeOpenInterface();

		m_pNewUIMng->ShowInterface(INTERFACE_CHARACTER);
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_CHAINFO, true);
		m_pNewPetInfoWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_MYQUEST)
	{
		HideGroupBeforeOpenInterface();

		if (IsVisible(INTERFACE_CHARACTER))
		{
			g_pMyQuestInfoWindow->SetPos(setPosRight(640 - 190 * 2), 0);
		}
		if (IsVisible(INTERFACE_INVENTORY))
		{
			Hide(INTERFACE_INVENTORY);
		}
		if (IsVisible(INTERFACE_PET))
		{
			Hide(INTERFACE_PET);
		}
		g_pMyQuestInfoWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_MIXINVENTORY)
	{
		HideAllGroupA();
		g_pMixInventory->OpeningProcess();
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY);
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, true);
	}
	else if (dwKey == INTERFACE_NPCSHOP)
	{
		HideAllGroupA();
		g_pNPCShop->OpenningProcess();
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY);
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, true);

		const bool isExtendedInventoryOpen = IsVisible(INTERFACE_INVENTORY_EXT);
		if (isExtendedInventoryOpen)
		{
			Show(INTERFACE_INVENTORY_EXT);
			g_pNPCShop->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
		}
		else
		{
			g_pNPCShop->SetPos(setPosRight(640 - 190 * 2), 0);
			Show(INTERFACE_HERO_POSITION_INFO);
		}
	}
	else if (dwKey == INTERFACE_STORAGE)
	{
		const bool isExtendedInventoryOpen = IsVisible(INTERFACE_INVENTORY_EXT);
		HideAllGroupA();
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY);
		if (isExtendedInventoryOpen)
		{
			Show(INTERFACE_INVENTORY_EXT);
			g_pStorageInventory->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
		}
		else
		{
			g_pStorageInventory->SetPos(setPosRight(640 - 190 * 2), 0);
			Show(INTERFACE_HERO_POSITION_INFO);
		}

		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, true);
	}
	else if (dwKey == INTERFACE_STORAGE_EXT)
	{
		if (IsVisible(INTERFACE_INVENTORY_EXT))
		{
			m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY_EXT, false);
		}

		//Hide(INTERFACE_HERO_POSITION_INFO);
		g_pStorageInventory->SetPos(setPosRight(640 - 190 * 2), 0);
		g_pStorageInventoryExt->SetPos(setPosRight(640 - 190 * 3), 0);

		m_pNewUIMng->ShowInterface(INTERFACE_STORAGE_EXT);
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, true);
	}
	else if (dwKey == INTERFACE_MIXINVENTORY)
	{
		const bool isExtendedInventoryOpen = IsVisible(INTERFACE_INVENTORY_EXT);
		HideAllGroupA();
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY);
		if (isExtendedInventoryOpen)
		{
			Show(INTERFACE_INVENTORY_EXT);
			g_pMixInventory->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
		}
		else
		{
			g_pMixInventory->SetPos(setPosRight(640 - 190 * 2), 0);
			Show(INTERFACE_HERO_POSITION_INFO);
		}
	}
	else if (dwKey == INTERFACE_MYSHOP_INVENTORY)
	{
		const bool isExtendedInventoryOpen = IsVisible(INTERFACE_INVENTORY_EXT);
		HideAllGroupA();
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY);
		if (isExtendedInventoryOpen)
		{
			Show(INTERFACE_INVENTORY_EXT);
			g_pMyShopInventory->SetPos(setPosRight(640 - 190 * 3), 0);
			//Hide(INTERFACE_HERO_POSITION_INFO);
		}
		else
		{
			g_pMyShopInventory->SetPos(setPosRight(640 - 190 * 2), 0);
			Show(INTERFACE_HERO_POSITION_INFO);
		}

		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, true);
		if (gMapManager.IsCursedTemple() == true)
		{
			g_pMyShopInventory->OpenButtonLock();
		}
		else
		{
			g_pMyShopInventory->OpenButtonUnLock();
		}
	}
	else if (dwKey == INTERFACE_PURCHASESHOP_INVENTORY)
	{
		HideAllGroupA();
	}
	else if (dwKey == INTERFACE_PARTY)
	{
		HideAllGroupA();
		m_pNewPartyInfoWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_NPCQUEST)
	{
		HideAllGroupA();
		g_pNPCQuest->ProcessOpening();
	}
	else if (dwKey == INTERFACE_TRADE)
	{
		HideAllGroupA();
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY);
		const bool isExtendedInventoryOpen = IsVisible(INTERFACE_INVENTORY_EXT);
		if (isExtendedInventoryOpen)
		{
			Show(INTERFACE_INVENTORY_EXT);
			g_pTrade->SetPos(setPosRight(640 - 190 * 3), 0);
		}
		else
		{
			g_pTrade->SetPos(setPosRight(640 - 190 * 2), 0);
		}
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, true);
	}
	else if (dwKey == INTERFACE_BLOODCASTLE)
	{
		HideAllGroupA();
		g_pEnterBloodCastle->OpenningProcess();
	}
	else if (dwKey == INTERFACE_DEVILSQUARE)
	{
		HideAllGroupA();
		g_pEnterDevilSquare->OpenningProcess();
	}
	else if (dwKey == INTERFACE_CATAPULT)
	{
		HideAllGroupA();
		g_pCatapultWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_COMMAND)
	{
		HideAllGroupA();
		m_pNewCommandWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_GUILDINFO)
	{
		HideAllGroupA();
		g_pGuildInfoWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_WINDOW_MENU)
	{
		g_pWindowMenu->OpenningProcess();
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_WINDOW, true);
	}
	else if (dwKey == INTERFACE_SENATUS)
	{
		HideAllGroupA();
		g_pCastleWindow->OpeningProcess();
	}
	else if (dwKey == INTERFACE_GUARDSMAN)
	{
		HideAllGroupA();
		g_pGuardWindow->OpeningProcess();
	}
	else if (dwKey == INTERFACE_GATEKEEPER)
	{
		HideAllGroupA();
		g_pGatemanWindow->OpeningProcess();
	}
	else if (dwKey == INTERFACE_GATESWITCH)
	{
		HideAllGroupA();
		g_pGateSwitchWindow->OpeningProcess();
	}
	else if (dwKey == INTERFACE_NPCGUILDMASTER)
	{
		HideAllGroupA();
	}
	else if (dwKey == INTERFACE_MASTER_LEVEL)
	{
		HideAllGroupA();
		HideAll();
	}
	else if (dwKey == INTERFACE_KANTURU2ND_ENTERNPC)
	{
		HideAllGroupB();
	}
	else if (dwKey == INTERFACE_HERO_POSITION_INFO)
	{
		m_pNewHeroPositionInfo->OpenningProcess();
	}
	else if (dwKey == INTERFACE_CHAOSCASTLE_TIME)
	{
		if (IsVisible(INTERFACE_CHATINPUTBOX))
		{
			Hide(INTERFACE_CHATINPUTBOX);
		}
		m_pNewChaosCastleTime->OpenningProcess();
	}
	else if (dwKey == INTERFACE_BLOODCASTLE_TIME)
	{
		g_pBloodCastle->OpenningProcess();
	}
	else if (dwKey == INTERFACE_OPTION)
	{
		g_pOption->OpenningProcess();
	}
	else if (dwKey == INTERFACE_HELP)
	{
		Hide(INTERFACE_MOVEMAP);
		Hide(INTERFACE_ITEM_EXPLANATION);
		Hide(INTERFACE_SETITEM_EXPLANATION);
		g_pHelp->OpenningProcess();
	}
	else if (dwKey == INTERFACE_ITEM_EXPLANATION)
	{
		Hide(INTERFACE_MOVEMAP);
		Hide(INTERFACE_HELP);
		Hide(INTERFACE_SETITEM_EXPLANATION);
		g_pItemExplanation->OpenningProcess();
	}
	else if (dwKey == INTERFACE_SETITEM_EXPLANATION)
	{
		Hide(INTERFACE_MOVEMAP);
		Hide(INTERFACE_HELP);
		Hide(INTERFACE_ITEM_EXPLANATION);
		g_pSetItemExplanation->OpenningProcess();
	}
	else if (dwKey == INTERFACE_QUICK_COMMAND)
	{
		g_pQuickCommand->OpenningProcess();
	}
	else if (dwKey == INTERFACE_MOVEMAP)
	{
		Hide(INTERFACE_HELP);
		Hide(INTERFACE_ITEM_EXPLANATION);
		Hide(INTERFACE_SETITEM_EXPLANATION);
		m_pNewMoveCommandWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_CHATINPUTBOX)
	{
		m_pNewChatInputBox->OpenningProcess();
	}
	else if (dwKey == INTERFACE_SIEGEWARFARE)
	{
		m_pNewSiegeWarfare->OpenningProcess();
	}
	else if (dwKey == INTERFACE_ITEM_ENDURANCE_INFO)
	{
		m_pNewItemEnduranceInfo->OpenningProcess();
	}
	else if (dwKey == INTERFACE_BUFF_WINDOW)
	{
		m_pNewBuffWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_CRYWOLF)
	{
		m_pNewCryWolfInterface->OpenningProcess();
	}
	else if (dwKey == INTERFACE_GOLD_BOWMAN)
	{
		m_pNewGoldBowman->OpeningProcess();
	}
	else if (dwKey == INTERFACE_GOLD_BOWMAN_LENA)
	{
		m_pNewGoldBowmanLena->OpeningProcess();
	}
	else if (dwKey == INTERFACE_LUCKYCOIN_REGISTRATION)
	{
		HideAllGroupA();
		g_pLuckyCoinRegistration->OpeningProcess();
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY);
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, true);
	}
	else if (dwKey == INTERFACE_EXCHANGE_LUCKYCOIN)
	{
		HideAllGroupA();
		g_pExchangeLuckyCoinWindow->OpenningProcess();
	}
	else if (dwKey == INTERFACE_DUELWATCH)
	{
		m_pNewDuelWatchWindow->OpeningProcess();
	}
	else if (dwKey == INTERFACE_DUELWATCH_MAINFRAME)
	{
		m_pNewDuelWatchMainFrameWindow->OpeningProcess();
	}
	else if (dwKey == INTERFACE_DUELWATCH_USERLIST)
	{
		m_pNewDuelWatchUserListWindow->OpeningProcess();
	}
	else if (dwKey == INTERFACE_INGAMESHOP)
	{
		g_ConsoleDebug->Write(MCD_NORMAL, "InGameShopStatue.Txt CallStack - CNewUISystem.Show()\r\n");
		HideAll();
		g_pInGameShop->OpeningProcess();
	}
	else if (dwKey == INTERFACE_DOPPELGANGER_NPC)
	{
		m_pNewDoppelGangerWindow->OpeningProcess();
	}
	else if (dwKey == INTERFACE_DOPPELGANGER_FRAME)
	{
		m_pNewDoppelGangerFrame->OpenningProcess();
	}
	else if (dwKey == INTERFACE_NPC_DIALOGUE)
	{
		HideAllGroupA();
		g_pNPCDialogue->ProcessOpening();
	}
	else if (dwKey == INTERFACE_QUEST_PROGRESS)
	{
		HideAllGroupA();
		g_pQuestProgress->ProcessOpening();
	}
	else if (dwKey == INTERFACE_QUEST_PROGRESS_ETC)
	{
		if (IsVisible(INTERFACE_INVENTORY))
			Hide(INTERFACE_INVENTORY);
		if (IsVisible(INTERFACE_MYQUEST))
		{
			Hide(INTERFACE_MYQUEST);
			g_pQuestProgressByEtc->SetPos(640 - 190, 0);
		}
		if (IsVisible(INTERFACE_CHARACTER))
			g_pQuestProgressByEtc->SetPos(640 - 190 * 2, 0);
		g_pQuestProgressByEtc->ProcessOpening();
	}
	else if (dwKey == INTERFACE_EMPIREGUARDIAN_NPC)
	{
		m_pNewEmpireGuardianNPC->OpenningProcess();
	}
	else if (dwKey == INTERFACE_EMPIREGUARDIAN_TIMER)
	{
		m_pNewEmpireGuardianTimer->OpenningProcess();
	}
	else if (dwKey == INTERFACE_MINI_MAP)
	{
		m_pNewMiniMap->OpenningProcess();
	}
	else if (dwKey == INTERFACE_GENSRANKING)
	{
		HideAllGroupA();
		g_pNewUIGensRanking->OpenningProcess();
		g_pNewUIGensRanking->SetPos(640 - 190, 0);
	}
	else if (dwKey == INTERFACE_UNITEDMARKETPLACE_NPC_JULIA)
	{
		m_pNewUnitedMarketPlaceWindow->OpeningProcess();
	}
#ifdef LEM_ADD_LUCKYITEM
	else if (dwKey == SEASON3B::INTERFACE_LUCKYITEMWND)
	{
		HideAllGroupA();
		g_pLuckyItemWnd->OpeningProcess();
		m_pNewUIMng->ShowInterface(SEASON3B::INTERFACE_INVENTORY);
	}
#endif // LEM_ADD_LUCKYITEM

	m_pNewUIMng->ShowInterface(dwKey);
	int iScreenWidth = GetScreenWidth();
	m_pNewItemEnduranceInfo->SetPos(iScreenWidth);
	m_pNewBuffWindow->SetPos(iScreenWidth);
	m_pNewPartyListWindow->SetPos(iScreenWidth);
}

void CNewUISystem::Hide(DWORD dwKey)
{
	if (!m_pNewUIMng)
	{
		return;
	}

	if (dwKey == INTERFACE_FRIEND)
	{
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_FRIEND, false);
		m_pNewFriendWindow->HideAllWindow(TRUE, TRUE);
	}
	else if (dwKey == INTERFACE_CHARACTER)
	{
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_CHAINFO, false);
		if (IsVisible(INTERFACE_MYQUEST))
		{
			g_pMyQuestInfoWindow->SetPos(setPosRight(640 - 190), 0);
		}
		if (IsVisible((INTERFACE_INVENTORY)))
		{
			g_pMyInventory->SetPos(setPosRight(640 - 190), 0);
		}
		if (IsVisible((INTERFACE_PET)))
		{
			Hide(INTERFACE_PET);
		}
		if (IsVisible((INTERFACE_QUEST_PROGRESS_ETC)))
		{
			g_pQuestProgressByEtc->SetPos(setPosRight(640 - 190), 0);
		}
	}
	else if (dwKey == INTERFACE_INVENTORY_EXT)
	{
		if (IsVisible(INTERFACE_MYSHOP_INVENTORY))
		{
			g_pMyShopInventory->SetPos(setPosRight(640 - 190 * 2), 0);

		}
		if (IsVisible(INTERFACE_TRADE))
		{
			g_pTrade->SetPos(setPosRight(640 - 190 * 2), 0);
		}

		if (IsVisible(INTERFACE_MIXINVENTORY))
		{
			g_pMixInventory->SetPos(setPosRight(640 - 190 * 2), 0);
		}

		if (IsVisible(INTERFACE_TRADE))
		{
			g_pTrade->SetPos(setPosRight(640 - 190 * 2), 0);
		}

		if (IsVisible(INTERFACE_NPCSHOP))
		{
			g_pNPCShop->SetPos(setPosRight(640 - 190 * 2), 0);
		}

		if (IsVisible(INTERFACE_STORAGE))
		{
			g_pStorageInventory->SetPos(setPosRight(640 - 190 * 2), 0);
		}

		if (IsVisible(INTERFACE_CHARACTER) && IsVisible(INTERFACE_INVENTORY))
		{
			g_pMyInventory->SetPos(setPosRight(640 - 190 * 2), 0);
		}

		Show(INTERFACE_HERO_POSITION_INFO);
	}
	else if (dwKey == INTERFACE_INVENTORY)
	{
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, false);

		if (IsVisible(INTERFACE_INVENTORY_EXT))
		{
			m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY_EXT, false);
		}

		if (IsVisible(INTERFACE_MIXINVENTORY))
		{
			if (g_pMixInventory->ClosingProcess() == false)
				return;
			m_pNewUIMng->ShowInterface(INTERFACE_MIXINVENTORY, false);
			g_pMixInventory->SetPos(setPosRight(260), 0);  //ACuoi Khóa Hòm Đồ Chung Khi Mix NPC Gobin
		}

#ifdef LEM_ADD_LUCKYITEM
		if (IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND))
		{
			if (g_pLuckyItemWnd->ClosingProcess() == false)
				return;
			m_pNewUIMng->ShowInterface(SEASON3B::INTERFACE_LUCKYITEMWND, false);
		}
#endif
		if (IsVisible(INTERFACE_NPCSHOP))
		{
			g_pNPCShop->ClosingProcess();
			m_pNewUIMng->ShowInterface(INTERFACE_NPCSHOP, false);
		}
		if (IsVisible(INTERFACE_MYSHOP_INVENTORY))
		{
			m_pNewUIMng->ShowInterface(INTERFACE_MYSHOP_INVENTORY, false);
		}
		if (IsVisible(INTERFACE_PURCHASESHOP_INVENTORY))
		{
			g_pPurchaseShopInventory->ClosingProcess();
			m_pNewUIMng->ShowInterface(INTERFACE_PURCHASESHOP_INVENTORY, false);
		}
		if (IsVisible(INTERFACE_STORAGE))
		{
			g_pStorageInventoryExt->ProcessClosing();
			if (!g_pStorageInventory->ProcessClosing())
				return;

			m_pNewUIMng->ShowInterface(INTERFACE_STORAGE_EXT, false);
			m_pNewUIMng->ShowInterface(INTERFACE_STORAGE, false);
		}
		if (IsVisible(INTERFACE_TRADE))
		{
			g_pTrade->ProcessCloseBtn();
			m_pNewUIMng->ShowInterface(INTERFACE_TRADE, false);
		}

		if (IsVisible(INTERFACE_LUCKYCOIN_REGISTRATION))
		{
			m_pNewLuckyCoinRegistration->ClosingProcess();
			m_pNewUIMng->ShowInterface(INTERFACE_LUCKYCOIN_REGISTRATION, false);
		}
		if (IsVisible(INTERFACE_EXCHANGE_LUCKYCOIN))
		{
			m_pNewExchangeLuckyCoinWindow->ClosingProcess();
			m_pNewUIMng->ShowInterface(INTERFACE_EXCHANGE_LUCKYCOIN, false);
		}

#ifdef LEM_ADD_LUCKYITEM
		if (IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND))
		{
			m_pNewUILuckyItemWnd->ClosingProcess();
			m_pNewUIMng->ShowInterface(SEASON3B::INTERFACE_LUCKYITEMWND, false);
		}
#endif

		g_pMyInventory->SetPos(setPosRight(640 - 190), 0);
		g_pMyInventory->ClosingProcess();
	}
	else if (dwKey == INTERFACE_MIXINVENTORY)
	{
		if (g_pMixInventory->ClosingProcess() == false)
		{
			return;
		}
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, false);
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY, false);
		Show(INTERFACE_HERO_POSITION_INFO);
	}
	else if (dwKey == INTERFACE_NPCSHOP)
	{
		g_pNPCShop->ClosingProcess();
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, false);
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY, false);
	}
	else if (dwKey == INTERFACE_MYSHOP_INVENTORY || dwKey == INTERFACE_PURCHASESHOP_INVENTORY)
	{
		if (dwKey == INTERFACE_MYSHOP_INVENTORY)
		{
			g_pMyShopInventory->ClosingProcess();
		}
		else if (dwKey == INTERFACE_PURCHASESHOP_INVENTORY)
		{
			g_pPurchaseShopInventory->ClosingProcess();
		}
		g_pMyInventory->SetPos(setPosRight(640 - 190), 0);
		Show(INTERFACE_HERO_POSITION_INFO);
	}
	else if (dwKey == INTERFACE_STORAGE)
	{
		g_pStorageInventoryExt->ProcessClosing();
		if (!g_pStorageInventory->ProcessClosing())
			return;
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, false);
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY_EXT, false);
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY, false);
		Show(INTERFACE_HERO_POSITION_INFO);
	}
	else if (dwKey == INTERFACE_STORAGE_EXT)
	{
		Show(INTERFACE_HERO_POSITION_INFO);
	}
	else if (dwKey == INTERFACE_PET)
	{
		m_pNewPetInfoWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_PARTY)
	{
		m_pNewPartyInfoWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_MYQUEST)
	{
		m_pNewMyQuestInfoWindow->ClosingProcess();
		m_pNewMyQuestInfoWindow->SetPos(setPosRight(640 - 190), 0);
	}
	else if (dwKey == INTERFACE_SENATUS)
	{
		m_pNewCastleWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_GUARDSMAN)
	{
		m_pNewGuardWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_GATEKEEPER)
	{
		m_pNewGatemanWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_GATESWITCH)
	{
		m_pNewGateSwitchWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_NPCQUEST)
	{
		m_pNewNPCQuest->ProcessClosing();
	}
	else if (dwKey == INTERFACE_BLOODCASTLE)
	{
		g_pEnterBloodCastle->ClosingProcess();
	}
	else if (dwKey == INTERFACE_DEVILSQUARE)
	{
		g_pEnterDevilSquare->ClosingProcess();
	}
	else if (dwKey == INTERFACE_BLOODCASTLE_TIME)
	{
		g_pBloodCastle->ClosingProcess();
	}
	else if (dwKey == INTERFACE_TRADE)
	{
		g_pTrade->ProcessClosing();
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_MYINVEN, false);
		m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY, false);
		Show(INTERFACE_HERO_POSITION_INFO);
	}
	else if (dwKey == INTERFACE_CATAPULT)
	{
		g_pCatapultWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_CHAOSCASTLE_TIME)
	{
		m_pNewChaosCastleTime->ClosingProcess();
	}
	else if (dwKey == INTERFACE_COMMAND)
	{
		m_pNewCommandWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_WINDOW_MENU)
	{
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_WINDOW, false);
	}
	else if (dwKey == INTERFACE_OPTION)
	{

	}
	else if (dwKey == INTERFACE_HERO_POSITION_INFO)
	{
		m_pNewHeroPositionInfo->ClosingProcess();
	}
	else if (dwKey == INTERFACE_HELP)
	{
		g_pHelp->ClosingProcess();
	}
	else if (dwKey == INTERFACE_ITEM_EXPLANATION)
	{
		g_pItemExplanation->ClosingProcess();
	}
	else if (dwKey == INTERFACE_SETITEM_EXPLANATION)
	{
		g_pSetItemExplanation->ClosingProcess();
	}
	else if (dwKey == INTERFACE_QUICK_COMMAND)
	{
		g_pQuickCommand->ClosingProcess();
	}
	else if (dwKey == INTERFACE_MOVEMAP)
	{
		m_pNewCommandWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_CHATINPUTBOX)
	{
		m_pNewChatInputBox->ClosingProcess();
	}
	else if (dwKey == INTERFACE_GUILDINFO)
	{
		m_pNewGuildInfoWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_NPCGUILDMASTER)
	{
		m_pNewGuildMakeWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_SIEGEWARFARE)
	{
		m_pNewSiegeWarfare->ClosingProcess();
	}
	else if (dwKey == INTERFACE_ITEM_ENDURANCE_INFO)
	{
		m_pNewItemEnduranceInfo->ClosingProcess();
	}
	else if (dwKey == INTERFACE_BUFF_WINDOW)
	{
		m_pNewBuffWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_CURSEDTEMPLE_RESULT)
	{
		m_pNewCursedTempleResultWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_CRYWOLF)
	{
		m_pNewCryWolfInterface->ClosingProcess();
	}
	else if (dwKey == INTERFACE_GOLD_BOWMAN)
	{
		m_pNewGoldBowman->ClosingProcess();
	}
	else if (dwKey == INTERFACE_GOLD_BOWMAN_LENA)
	{
		m_pNewGoldBowmanLena->ClosingProcess();
	}
	else if (dwKey == INTERFACE_LUCKYCOIN_REGISTRATION)
	{
		m_pNewLuckyCoinRegistration->ClosingProcess();

		if (IsVisible(INTERFACE_INVENTORY))
		{
			m_pNewMyInventory->ClosingProcess();
			m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY, false);
		}
	}
	else if (dwKey == INTERFACE_EXCHANGE_LUCKYCOIN)
	{
		m_pNewExchangeLuckyCoinWindow->ClosingProcess();

		if (IsVisible(INTERFACE_INVENTORY))
		{
			m_pNewMyInventory->ClosingProcess();
			m_pNewUIMng->ShowInterface(INTERFACE_INVENTORY, false);
		}
	}
	else if (dwKey == INTERFACE_DUELWATCH)
	{
		m_pNewDuelWatchWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_DUELWATCH_MAINFRAME)
	{
		m_pNewDuelWatchMainFrameWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_DUELWATCH_USERLIST)
	{
		m_pNewDuelWatchUserListWindow->ClosingProcess();
	}
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	else if (dwKey == INTERFACE_INGAMESHOP)
	{
		g_pInGameShop->ClosingProcess();
		g_pMainFrame->SetBtnState(MAINFRAME_BTN_PARTCHARGE, false);
	}
#endif //PBG_ADD_INGAMESHOP_UI_MAINFRAME
	else if (dwKey == INTERFACE_DOPPELGANGER_NPC)
	{
		m_pNewDoppelGangerWindow->ClosingProcess();
	}
	else if (dwKey == INTERFACE_DOPPELGANGER_FRAME)
	{
		m_pNewDoppelGangerFrame->ClosingProcess();
	}
	else if (dwKey == INTERFACE_NPC_DIALOGUE)
	{
		m_pNewNPCDialogue->ProcessClosing();
	}
	else if (dwKey == INTERFACE_QUEST_PROGRESS)
	{
		m_pNewQuestProgress->ProcessClosing();
	}
	else if (dwKey == INTERFACE_QUEST_PROGRESS_ETC)
	{
		m_pNewQuestProgressByEtc->ProcessClosing();
	}
	else if (dwKey == INTERFACE_EMPIREGUARDIAN_NPC)
	{
		m_pNewEmpireGuardianNPC->ClosingProcess();
	}
	else if (dwKey == INTERFACE_EMPIREGUARDIAN_TIMER)
	{
		m_pNewEmpireGuardianTimer->ClosingProcess();
	}
	else if (dwKey == INTERFACE_MINI_MAP)
	{
		m_pNewMiniMap->ClosingProcess();
	}
	else if (dwKey == INTERFACE_GENSRANKING)
	{
		g_pNewUIGensRanking->ClosingProcess();
	}
	else if (dwKey == INTERFACE_UNITEDMARKETPLACE_NPC_JULIA)
	{
		m_pNewUnitedMarketPlaceWindow->ClosingProcess();
	}
#ifdef LEM_ADD_LUCKYITEM
	else if (dwKey == SEASON3B::INTERFACE_LUCKYITEMWND)
	{
		if (g_pLuckyItemWnd->ClosingProcess() == false)
			return;
		if (IsVisible(SEASON3B::INTERFACE_INVENTORY))
		{
			m_pNewMyInventory->ClosingProcess();
			m_pNewUIMng->ShowInterface(SEASON3B::INTERFACE_INVENTORY, false);
		}
	}
#endif
	else if (dwKey == INTERFACE_MUHELPER)
	{
		m_pNewUIMng->ShowInterface(SEASON3B::INTERFACE_MUHELPER_SKILL_LIST, false);
		m_pNewUIMng->ShowInterface(SEASON3B::INTERFACE_MUHELPER_EXT, false);
	}
	m_pNewUIMng->ShowInterface(dwKey, false);
	int iScreenWidth = GetScreenWidth();
	m_pNewItemEnduranceInfo->SetPos(iScreenWidth);
	m_pNewBuffWindow->SetPos(iScreenWidth);
	m_pNewPartyListWindow->SetPos(iScreenWidth);
}

void CNewUISystem::Toggle(DWORD dwKey)
{
	IsVisible(dwKey) ? Hide(dwKey) : Show(dwKey);
}

void CNewUISystem::HideAll()
{
	if (m_pNewUIMng)
	{
		for (int i = INTERFACE_BEGIN + 1; i < INTERFACE_END; i++)
		{
			if (IsImpossibleHideInterface(i) == false)
			{
				if (IsVisible(i) == true)
				{
					Hide(i);
				}
			}
		}
	}
}

void CNewUISystem::HideAllGroupA()
{
	Hide(INTERFACE_INVENTORY);
	Hide(INTERFACE_CHARACTER);

	DWORD dwGroupA[] =
	{
		//SEASON3B::INTERFACE_INVENTORY,
		//SEASON3B::INTERFACE_CHARACTER,			
		//SEASON3B::INTERFACE_WINDOW_MENU,
		INTERFACE_MUHELPER,
		INTERFACE_MUHELPER_EXT,
		INTERFACE_MUHELPER_SKILL_LIST,
		INTERFACE_MIXINVENTORY,
		INTERFACE_STORAGE,
		INTERFACE_NPCSHOP,
		INTERFACE_MYSHOP_INVENTORY,
		INTERFACE_PURCHASESHOP_INVENTORY,
		INTERFACE_PET,
		INTERFACE_MYQUEST,
		INTERFACE_NPCQUEST,
		INTERFACE_PARTY,
		INTERFACE_SENATUS,
		INTERFACE_GUARDSMAN,
		INTERFACE_COMMAND,
		INTERFACE_GUILDINFO,
		INTERFACE_KANTURU2ND_ENTERNPC,
		INTERFACE_DUELWATCH,
		INTERFACE_DOPPELGANGER_NPC,
		//SEASON3B::INTERFACE_HELP,
		//SEASON3B::INTERFACE_ITEM_EXPLANATION,
		//SEASON3B::INTERFACE_SETITEM_EXPLANATION,
		INTERFACE_GOLD_BOWMAN,
		INTERFACE_GOLD_BOWMAN_LENA,
		INTERFACE_NPC_DIALOGUE,
		INTERFACE_QUEST_PROGRESS,
		INTERFACE_QUEST_PROGRESS_ETC,
		INTERFACE_EMPIREGUARDIAN_NPC,
#ifdef PBG_MOD_STAMINA_UI
		SEASON3B::INTERFACE_STAMINA_GAUGE,
#endif //PBG_MOD_STAMINA_UI
#ifdef PBG_ADD_GENSRANKING
		INTERFACE_GENSRANKING,
#endif //PBG_ADD_GENSRANKING
		INTERFACE_UNITEDMARKETPLACE_NPC_JULIA,

#ifdef LEM_ADD_LUCKYITEM
		SEASON3B::INTERFACE_LUCKYITEMWND,
#endif // LEM_ADD_LUCKYITEM

		0,
	};

	if (m_pNewUIMng)
	{
		for (int i = 0; dwGroupA[i] != 0; i++)
		{
			m_pNewUIMng->ShowInterface(dwGroupA[i], false);
		}
	}
}

void CNewUISystem::HideAllGroupB()
{
	Hide(INTERFACE_FRIEND);
	Hide(INTERFACE_INVENTORY);
	Hide(INTERFACE_CHARACTER);

	DWORD dwGroupB[] =
	{
		//SEASON3B::INTERFACE_FRIEND,
		//SEASON3B::INTERFACE_INVENTORY,
		//SEASON3B::INTERFACE_CHARACTER,
		//SEASON3B::INTERFACE_WINDOW_MENU,

		INTERFACE_MIXINVENTORY,
		INTERFACE_STORAGE,
		INTERFACE_NPCSHOP,
		INTERFACE_MYSHOP_INVENTORY,
		INTERFACE_PURCHASESHOP_INVENTORY,
		INTERFACE_PET,
		INTERFACE_MYQUEST,
		INTERFACE_NPCQUEST,
		INTERFACE_PARTY,
		INTERFACE_SENATUS,
		INTERFACE_GUARDSMAN,
		INTERFACE_COMMAND,
		INTERFACE_GUILDINFO,
		INTERFACE_KANTURU2ND_ENTERNPC,
		INTERFACE_CURSEDTEMPLE_NPC,
		INTERFACE_DUELWATCH,
		INTERFACE_DOPPELGANGER_NPC,
		//SEASON3B::INTERFACE_HELP,
		//SEASON3B::INTERFACE_ITEM_EXPLANATION,
		//SEASON3B::INTERFACE_SETITEM_EXPLANATION,	
		INTERFACE_GOLD_BOWMAN,
		INTERFACE_GOLD_BOWMAN_LENA,
		INTERFACE_NPC_DIALOGUE,
		INTERFACE_QUEST_PROGRESS,
		INTERFACE_QUEST_PROGRESS_ETC,
		INTERFACE_EMPIREGUARDIAN_NPC,
#ifdef PBG_MOD_STAMINA_UI
		SEASON3B::INTERFACE_STAMINA_GAUGE,
#endif //PBG_MOD_STAMINA_UI
#ifdef PBG_ADD_GENSRANKING
		INTERFACE_GENSRANKING,
#endif //PBG_ADD_GENSRANKING
		INTERFACE_UNITEDMARKETPLACE_NPC_JULIA,
#ifdef LEM_ADD_LUCKYITEM
		SEASON3B::INTERFACE_LUCKYITEMWND,
#endif // LEM_ADD_LUCKYITEM

		0,
	};

	if (m_pNewUIMng)
	{
		for (int i = 0; dwGroupB[i] != 0; i++)
		{
			m_pNewUIMng->ShowInterface(dwGroupB[i], false);
		}
	}
}
void CNewUISystem::HideGroupBeforeOpenInterface()
{
	DWORD dwGroupC[] =
	{
		INTERFACE_PARTY,
		INTERFACE_COMMAND,
		INTERFACE_GUILDINFO,
		INTERFACE_GOLD_BOWMAN,
		INTERFACE_GOLD_BOWMAN_LENA,
		INTERFACE_GENSRANKING,
		INTERFACE_RANKING,
		INTERFACE_EVENTITME,
		INTERFACE_EVENRELIFE,
		INTERFACE_EVENRSCHANGE,
		INTERFACE_GIFTCODE,
		INTERFACE_EVENDANHHIEU,
		INTERFACE_EVENQUANHAM,
		INTERFACE_EVENTULUYEN,
		INTERFACE_EVENHONHOAN,
		INTERFACE_EVENDAMEBOSS,
		INTERFACE_IDLEVEL,
		INTERFACE_MAINMAP,
		INTERFACE_MUHELPER,
		INTERFACE_MUHELPER_EXT,
		INTERFACE_MUHELPER_SKILL_LIST,
		0,
	};

	if (m_pNewUIMng)
	{
		for (int i = 0; dwGroupC[i] != 0; i++)
		{
			m_pNewUIMng->ShowInterface(dwGroupC[i], false);
		}
	}
}

void CNewUISystem::Enable(DWORD dwKey)
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->EnableInterface(dwKey);
	}
}

void CNewUISystem::Disable(DWORD dwKey)
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->EnableInterface(dwKey, false);
	}
}

bool CNewUISystem::CheckMouseUse()
{
	if (m_pNewUIMng)
	{
		if (m_pNewUIMng->GetActiveMouseUIObj())
			return true;
	}
	return false;
}

bool CNewUISystem::CheckKeyUse()
{
	if (m_pNewUIMng)
	{
		if (m_pNewUIMng->GetActiveKeyUIObj())
			return true;
	}
	return false;
}

bool CNewUISystem::Update()
{
	if (m_pNewItemMng)
	{
		m_pNewItemMng->Update();
	}

	if (m_pNewUIMng)
	{
		m_pNewUIMng->UpdateMouseEvent();
		m_pNewUIMng->UpdateKeyEvent();
		return m_pNewUIMng->Update();
	}
	return false;
}

bool CNewUISystem::Render()
{
	bool bResult = false;

	BeginBitmap();
	if (m_pNewUIMng)
		bResult = m_pNewUIMng->Render();
	EndBitmap();

	return bResult;
}

void SEASON3B::CNewUISystem::RenderItem3DFree(float sx, float sy, float Width, float Height, int Type, int Level, int Option1, int ExtOption, bool PickUp, float Scale, bool FixY)
{
	float x = sx;
	float y = sy;
	float s = Scale;

	if (FixY == true)
	{
		if (Type >= GET_ITEM(0, 0) && Type < GET_ITEM(1, 0))
		{
			x = sx - 5;
			y = sy + 5;
		}
		else if (Type >= GET_ITEM(3, 0) && Type < GET_ITEM(4, 0))
		{
			y = sy + 30;
		}
		else if (Type >= GET_ITEM(7, 0) && Type < GET_ITEM(8, 0))
		{
			s = Scale - 0.3;
		}
		else if (Type >= GET_ITEM(8, 0) && Type < GET_ITEM(9, 0))
		{
			y = sy + 4;
		}
		else if (Type >= GET_ITEM(9, 0) && Type < GET_ITEM(12, 0))
		{
			y = sy + 0;
		}
		else if (Type >= GET_ITEM(12, 0) && Type < GET_ITEM(13, 0))
		{
			if (Type >= GET_ITEM(12, 50))
			{
				y = sy + 0;
			}
			else
			{
				y = sy + 8;
			}
		}
		else if (Type >= GET_ITEM(13, 0) && Type < GET_ITEM(14, 0))
		{
			y = sy + 0;
		}
		else if (Type >= 2048)
		{
			y = sy + 25;
		}
		else
		{
			y = sy + 20;
		}
	}

	EnableAlphaTest();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);

	float SizeItem = Scale;

	if (m_Resolution >= 1)
	{
		SizeItem += 0.70f;
	}

	gluPerspective2(SizeItem, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();

	glClear(GL_DEPTH_BUFFER_BIT);

	float BWidth = (float)ItemAttribute[Type].Width * INVENTORY_SCALE;
	float BHeight = (float)ItemAttribute[Type].Height * INVENTORY_SCALE;
	RenderItem3D(x, y, Width, Height, Type, Level, Option1, ExtOption, PickUp);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glColor3f(1, 1, 1);
	EnableAlphaTest(false);
}

void SEASON3B::CNewUISystem::RenderItem3DNew(float sx, float sy, float Width, float Height, int Type, int Level, int Option1, int ExtOption, bool PickUp, float Scale, bool FixY)
{
	float x = sx;
	float y = sy;
	float s = Scale;

	//155 75

	if (FixY == true)
	{
		switch (Type)
		{
			case GET_ITEM(0, 0):  x = sx - 5; y = sy + 7;  s = Scale - 1.0f; break;
			case GET_ITEM(0, 1):  x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(0, 2):
			case GET_ITEM(0, 3):
			case GET_ITEM(0, 4):
			case GET_ITEM(0, 5):
			case GET_ITEM(0, 6):
			case GET_ITEM(0, 7):
			case GET_ITEM(0, 8):
			case GET_ITEM(0, 9):
			case GET_ITEM(0, 10):
			case GET_ITEM(0, 11):
			case GET_ITEM(0, 12):
			case GET_ITEM(0, 13): x = sx - 6; y = sy + 10; s = Scale - 0.6f; break;
			case GET_ITEM(0, 14):
			case GET_ITEM(0, 15):
			case GET_ITEM(0, 16): x = sx - 6; y = sy + 10; s = Scale - 0.4f; break;
			case GET_ITEM(0, 17): x = sx - 6; y = sy + 10; s = Scale - 0.5f; break;
			case GET_ITEM(0, 18): x = sx - 6; y = sy + 10; s = Scale - 0.7f; break;
			case GET_ITEM(0, 19): x = sx - 6; y = sy + 10; s = Scale - 0.4f; break;
			case GET_ITEM(0, 20):
			case GET_ITEM(0, 21): x = sx - 6; y = sy + 15; s = Scale - 0.7f; break;
			case GET_ITEM(0, 22): x = sx - 6; y = sy + 15; s = Scale - 0.5f; break;
			case GET_ITEM(0, 23): x = sx - 6; y = sy + 12; s = Scale - 0.5f; break;
			case GET_ITEM(0, 24): x = sx - 6; y = sy + 15; s = Scale - 0.5f; break;
			case GET_ITEM(0, 25): x = sx - 6; y = sy + 12; s = Scale - 0.6f; break;
			case GET_ITEM(0, 26):
			case GET_ITEM(0, 27): x = sx - 6; y = sy + 15; s = Scale - 0.6f; break;
			case GET_ITEM(0, 28): x = sx - 6; y = sy + 15; s = Scale - 0.5f; break;
			case GET_ITEM(0, 31): x = sx - 6; y = sy + 10; s = Scale - 0.5f; break;
			case GET_ITEM(0, 32):
			case GET_ITEM(0, 33):
			case GET_ITEM(0, 34):
			case GET_ITEM(0, 35):
			case GET_ITEM(0, 46):
			case GET_ITEM(0, 52):
			case GET_ITEM(0, 60):
			case GET_ITEM(0, 61):
			case GET_ITEM(0, 81):
			case GET_ITEM(0, 102):
			case GET_ITEM(0, 105):
			case GET_ITEM(0, 108):
			case GET_ITEM(0, 114):
			case GET_ITEM(0, 117):
			case GET_ITEM(0, 120):
			case GET_ITEM(0, 123):
			case GET_ITEM(0, 126):
			case GET_ITEM(0, 129):
			case GET_ITEM(0, 132):
			case GET_ITEM(0, 135):
			case GET_ITEM(0, 138): x = sx - 6; y = sy + 2;  s = Scale - 0.9f; break;
			case GET_ITEM(0, 47):  x = sx - 6; y = sy + 12; s = Scale - 0.5f; break;
			case GET_ITEM(0, 51):  x = sx - 6; y = sy + 9;  s = Scale - 0.2f; break;
			case GET_ITEM(0, 83):  x = sx - 6; y = sy + 8;  s = Scale - 0.2f; break;
			case GET_ITEM(0, 100):
			case GET_ITEM(0, 101):
			case GET_ITEM(0, 103):
			case GET_ITEM(0, 104):
			case GET_ITEM(0, 106):
			case GET_ITEM(0, 107): x = sx - 6; y = sy + 12; s = Scale - 0.5f; break;
			case GET_ITEM(0, 109): x = sx - 6; y = sy + 10; s = Scale - 0.6f; break;
			case GET_ITEM(0, 110): x = sx - 6; y = sy + 5;  s = Scale - 0.4f; break;
			case GET_ITEM(0, 111): x = sx - 6; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(0, 112):
			case GET_ITEM(0, 113): x = sx - 6; y = sy + 13; s = Scale - 0.7f; break;
			case GET_ITEM(0, 115): x = sx - 6; y = sy + 13; s = Scale - 0.6f; break;
			case GET_ITEM(0, 116):
			case GET_ITEM(0, 118): x = sx - 6; y = sy + 13; s = Scale - 0.7f; break;
			case GET_ITEM(0, 119): x = sx - 6; y = sy + 13; s = Scale - 0.6f; break;
			case GET_ITEM(0, 121): x = sx - 6; y = sy + 8;  s = Scale - 0.6f; break;
			case GET_ITEM(0, 124): x = sx - 6; y = sy + 10; s = Scale - 0.8f; break;
			case GET_ITEM(0, 125): x = sx - 6; y = sy + 13; s = Scale - 0.6f; break;
			case GET_ITEM(0, 127): x = sx - 6; y = sy + 13; s = Scale - 0.75f; break;
			case GET_ITEM(0, 128): x = sx - 6; y = sy + 13; s = Scale - 0.6f; break;
			case GET_ITEM(0, 130): x = sx - 6; y = sy + 13; s = Scale - 0.7f; break;
			case GET_ITEM(0, 131): x = sx - 6; y = sy + 11; s = Scale - 0.7f; break;
			case GET_ITEM(0, 133):
			case GET_ITEM(0, 134):
			case GET_ITEM(0, 136):
			case GET_ITEM(0, 137): x = sx - 6; y = sy + 12; s = Scale - 0.3f; break;


			case GET_ITEM(1, 0):
			case GET_ITEM(1, 1):
			case GET_ITEM(1, 2):
			case GET_ITEM(1, 3):
			case GET_ITEM(1, 4):
			case GET_ITEM(1, 5):
			case GET_ITEM(1, 6): x = sx - 6; y = sy + 14; s = Scale - 1.0f; break;
			case GET_ITEM(1, 7): x = sx - 6; y = sy + 14; s = Scale - 0.9f; break;
			case GET_ITEM(1, 8): x = sx - 6; y = sy + 14; s = Scale - 0.8f; break;

			case GET_ITEM(2, 0): x = sx - 6; y = sy + 14; s = Scale - 1.0f; break;
			case GET_ITEM(2, 1): x = sx - 6; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(2, 2): x = sx - 6; y = sy + 0;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 3): x = sx - 6; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(2, 4): x = sx - 6; y = sy + 8;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 5): x = sx - 6; y = sy + 18; s = Scale - 0.5f; break;
			case GET_ITEM(2, 6): x = sx - 6; y = sy + 16; s = Scale - 0.6f; break;
			case GET_ITEM(2, 7): x = sx - 6; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(2, 8): x = sx - 6; y = sy + 11; s = Scale - 0.8f; break;
			case GET_ITEM(2, 9): x = sx - 6; y = sy + 13; s = Scale - 0.8f; break;

			case GET_ITEM(2, 10):
			case GET_ITEM(2, 11):
			case GET_ITEM(2, 12):
			case GET_ITEM(2, 13): x = sx - 6; y = sy + 5;  s = Scale - 0.5f; break;
			case GET_ITEM(2, 14):
			case GET_ITEM(2, 15): x = sx - 8; y = sy + 8;  s = Scale - 0.3f; break;
			case GET_ITEM(2, 16): x = sx - 8; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(2, 17): x = sx - 8; y = sy + 12; s = Scale - 0.45f; break;
			case GET_ITEM(2, 18): x = sx - 8; y = sy + 12; s = Scale - 0.7f; break;
			case GET_ITEM(2, 25): x = sx - 8; y = sy + 4;  s = Scale - 0.5f; break;
			case GET_ITEM(2, 34):
			case GET_ITEM(2, 35): x = sx - 8; y = sy + 4;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 100): x = sx - 8; y = sy + 10; s = Scale - 0.8f; break;
			case GET_ITEM(2, 101): x = sx - 8; y = sy + 5;  s = Scale - 0.7f; break;
			case GET_ITEM(2, 102): x = sx - 8; y = sy + 3;  s = Scale - 0.5f; break;
			case GET_ITEM(2, 103):
			case GET_ITEM(2, 104): x = sx - 8; y = sy + 5;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 105): x = sx - 8; y = sy + 7;  s = Scale - 1.1f; break;
			case GET_ITEM(2, 106):
			case GET_ITEM(2, 107): x = sx - 8; y = sy + 3;  s = Scale - 0.7f; break;
			case GET_ITEM(2, 108):
			case GET_ITEM(2, 109): x = sx - 8; y = sy - 2;  s = Scale - 0.7f; break;
			case GET_ITEM(2, 110): x = sx - 8; y = sy + 5;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 111):
			case GET_ITEM(2, 112): x = sx - 8; y = sy + 8;  s = Scale - 0.0f; break;
			case GET_ITEM(3, 0): x = sx - 8;  y = sy + 8;  s = Scale - 0.3f; break;
			case GET_ITEM(3, 1): x = sx - 0;  y = sy + 6;  s = Scale - 0.3f; break;
			case GET_ITEM(3, 2): x = sx - 0;  y = sy + 20; s = Scale - 0.7f; break;
			case GET_ITEM(3, 3):
			case GET_ITEM(3, 4): x = sx - 0;  y = sy + 8;  s = Scale - 0.5f; break;
			case GET_ITEM(3, 5): x = sx - 0;  y = sy + 4;  s = Scale - 0.8f; break;
			case GET_ITEM(3, 6): x = sx - 0;  y = sy + 7;  s = Scale - 0.8f; break;
			case GET_ITEM(3, 7): x = sx - 0;  y = sy + 2;  s = Scale - 0.6f; break;
			case GET_ITEM(3, 8): x = sx - 0;  y = sy + 7;  s = Scale - 0.6f; break;
			case GET_ITEM(3, 9): x = sx - 0;  y = sy + 3;  s = Scale - 0.4f; break;
			case GET_ITEM(3, 10): x = sx - 0; y = sy + 11; s = Scale - 0.5f; break;
			case GET_ITEM(3, 11): x = sx - 0; y = sy + 8;  s = Scale - 0.5f; break;

			case GET_ITEM(4, 0): x = sx - 12; y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(4, 1): x = sx - 12; y = sy + 6;  s = Scale - 1.0f; break;
			case GET_ITEM(4, 2):
			case GET_ITEM(4, 3): x = sx - 12; y = sy + 6;  s = Scale - 0.9f; break;
			case GET_ITEM(4, 4): x = sx - 12; y = sy + 4;  s = Scale - 0.9f; break;
			case GET_ITEM(4, 5): x = sx - 8;  y = sy + 4;  s = Scale - 0.7f; break;
			case GET_ITEM(4, 6): x = sx - 8;  y = sy + 1;  s = Scale - 0.4f; break;
			case GET_ITEM(4, 7): x = sx - 0;  y = sy + 2;  s = Scale - 1.3f; break;
			case GET_ITEM(4, 8): x = sx - 5;  y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(4, 9): x = sx - 5;  y = sy + 13; s = Scale - 1.0f; break;
			case GET_ITEM(4, 10): x = sx - 5; y = sy + 12; s = Scale - 1.0f; break;
			case GET_ITEM(4, 11): x = sx - 5; y = sy + 14; s = Scale - 1.0f; break;
			case GET_ITEM(4, 12): x = sx - 5; y = sy + 18; s = Scale - 1.0f; break;
			case GET_ITEM(4, 13): x = sx - 5; y = sy + 14; s = Scale - 1.0f; break;
			case GET_ITEM(4, 14): x = sx - 5; y = sy + 11; s = Scale - 0.9f; break;
			case GET_ITEM(4, 15): x = sx - 8; y = sy + 0;  s = Scale - 1.3f; break;
			case GET_ITEM(4, 16): x = sx - 0; y = sy + 16; s = Scale - 1.0f; break;
			case GET_ITEM(4, 17): x = sx - 0; y = sy + 1;  s = Scale - 0.58f; break;
			case GET_ITEM(4, 18): x = sx - 0; y = sy + 15; s = Scale - 0.8f; break;
			case GET_ITEM(4, 19): x = sx - 0; y = sy + 12; s = Scale - 1.0f; break;
			case GET_ITEM(4, 20): x = sx - 8; y = sy + 6;  s = Scale - 0.45f; break;
			case GET_ITEM(4, 21): x = sx - 8; y = sy + 12; s = Scale - 0.45f; break;
			case GET_ITEM(4, 22): x = sx - 8; y = sy + 12; s = Scale - 0.6f; break;
			case GET_ITEM(4, 23): x = sx - 4; y = sy + 8;  s = Scale - 0.37f; break;
			case GET_ITEM(4, 24): x = sx - 4; y = sy + 8;  s = Scale - 0.6f; break;
			case GET_ITEM(4, 30): x = sx - 4; y = sy + 12; s = Scale - 0.6f; break;
			case GET_ITEM(4, 100): x = sx - 4;  y = sy + 8;  s = Scale - 0.7f; break;
			case GET_ITEM(4, 101): x = sx - 0;  y = sy + 16;  s = Scale - 0.15f; break;
			case GET_ITEM(4, 102): x = sx - 4;  y = sy + 4;  s = Scale - 0.5f; break;
			case GET_ITEM(4, 103): x = sx - 8;  y = sy + 8;  s = Scale - 0.45f; break;
			case GET_ITEM(4, 104): x = sx - 8;  y = sy + 6;  s = Scale - 0.45f; break;
			case GET_ITEM(4, 105): x = sx - 0;  y = sy + 6;  s = Scale - 0.5f; break;
			case GET_ITEM(4, 106): x = sx - 8;  y = sy + 8;  s = Scale - 0.6f; break;
			case GET_ITEM(4, 107):
			case GET_ITEM(4, 108): x = sx - 8;  y = sy + 6;  s = Scale - 0.0f; break;
			case GET_ITEM(4, 109): x = sx - 10; y = sy + 8;  s = Scale - 0.35f; break;
			case GET_ITEM(4, 110): x = sx - 10; y = sy + 9;  s = Scale - 0.6f; break;
			case GET_ITEM(4, 111):
			case GET_ITEM(4, 112): x = sx - 10; y = sy + 6;  s = Scale - 0.2f; break;
			case GET_ITEM(5, 0):  x = sx - 2; y = sy + 6;  s = Scale - 1.0f; break;
			case GET_ITEM(5, 1):  x = sx - 2; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(5, 2):  x = sx - 2; y = sy + 4;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 3):  x = sx - 2; y = sy + 2;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 4):  x = sx - 2; y = sy + 10; s = Scale - 0.7f; break;
			case GET_ITEM(5, 5):  x = sx - 2; y = sy + 8;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 6):  x = sx - 2; y = sy + 8;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 7):  x = sx - 2; y = sy + 6;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 8):  x = sx - 6; y = sy + 2;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 9):  x = sx - 8; y = sy + 6;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 10): x = sx - 4; y = sy + 4;  s = Scale - 0.3f; break;
			case GET_ITEM(5, 11): x = sx - 4; y = sy + 6;  s = Scale - 0.6f; break;
			case GET_ITEM(5, 12):
			case GET_ITEM(5, 13): x = sx - 8; y = sy + 4;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 14): x = sx - 8; y = sy + 4;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 15): x = sx - 8; y = sy + 6;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 16): x = sx - 8; y = sy + 5;  s = Scale - 0.75f; break;
			case GET_ITEM(5, 17): x = sx - 8; y = sy + 5;  s = Scale - 0.6f; break;
			case GET_ITEM(5, 18):
			case GET_ITEM(5, 19): x = sx - 4; y = sy + 4;  s = Scale - 0.75f; break;
			case GET_ITEM(5, 20): x = sx - 4; y = sy + 4;  s = Scale - 0.45f; break;
			case GET_ITEM(5, 21):
			case GET_ITEM(5, 22):
			case GET_ITEM(5, 23): x = sx - 8; y = sy + 0;  s = Scale - 1.2f; break;
			case GET_ITEM(5, 30): x = sx - 6; y = sy + 8;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 31): x = sx - 6; y = sy + 8;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 32): x = sx - 6; y = sy + 8;  s = Scale - 0.48f; break;
			case GET_ITEM(5, 33): x = sx - 6; y = sy + 4;  s = Scale - 0.55f; break;
			case GET_ITEM(5, 34): x = sx - 6; y = sy + 8;  s = Scale - 0.65f; break;
			case GET_ITEM(5, 36): x = sx - 6; y = sy + 2;  s = Scale - 0.55f; break;
			case GET_ITEM(5, 49): x = sx - 6; y = sy + 8;  s = Scale - 0.45f; break;
			case GET_ITEM(5, 50): x = sx - 6; y = sy + 4;  s = Scale - 0.45f; break;
			case GET_ITEM(5, 100): x = sx - 4; y = sy + 8;  s = Scale - 0.6f; break;
			case GET_ITEM(5, 101): x = sx - 4; y = sy + 8;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 102): x = sx - 4; y = sy + 9;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 103): x = sx - 4; y = sy + 2;  s = Scale - 0.7f; break;
			case GET_ITEM(5, 104): x = sx - 4; y = sy + 10; s = Scale - 0.8f; break;
			case GET_ITEM(5, 105): x = sx - 4; y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(5, 106): x = sx - 4; y = sy + 6;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 107): x = sx - 4; y = sy + 23; s = Scale - 1.2f; break;
			case GET_ITEM(5, 108): x = sx - 4; y = sy + 4;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 109): x = sx - 4; y = sy + 10; s = Scale - 1.0f; break;
			case GET_ITEM(5, 110): x = sx - 2; y = sy + 5;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 111): x = sx - 2; y = sy + 5;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 112): x = sx - 2; y = sy + 3;  s = Scale - 0.7f; break;
			case GET_ITEM(5, 113): x = sx - 8; y = sy + 10; s = Scale - 1.0f; break;
			case GET_ITEM(5, 114): x = sx - 0; y = sy + 5;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 115): x = sx - 4; y = sy + 17; s = Scale - 0.7f; break;
			case GET_ITEM(5, 116): x = sx - 4; y = sy + 6;  s = Scale - 0.6f; break;
			case GET_ITEM(5, 117): x = sx - 4; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(5, 118): x = sx - 4; y = sy + 4;  s = Scale - 0.82f; break;
			case GET_ITEM(5, 119): x = sx - 4; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(5, 120): x = sx - 4; y = sy + 0;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 121): x = sx - 4; y = sy + 8;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 122):
			case GET_ITEM(5, 123): x = sx - 4; y = sy + 8;  s = Scale - 0.1f; break;
			case GET_ITEM(5, 124):
			case GET_ITEM(5, 125): x = sx - 4; y = sy + 9;  s = Scale - 0.0f; break;

			case GET_ITEM(6, 0):
			case GET_ITEM(6, 1):
			case GET_ITEM(6, 2):
			case GET_ITEM(6, 3):
			case GET_ITEM(6, 4):
			case GET_ITEM(6, 5):
			case GET_ITEM(6, 6):
			case GET_ITEM(6, 7):
			case GET_ITEM(6, 8):
			case GET_ITEM(6, 9):
			case GET_ITEM(6, 10):
			case GET_ITEM(6, 11):
			case GET_ITEM(6, 12): x = sx - 4; y = sy + 5;  s = Scale - 1.0f; break;
			case GET_ITEM(6, 13):
			case GET_ITEM(6, 14):
			case GET_ITEM(6, 15):
			case GET_ITEM(6, 16): x = sx - 4; y = sy + 8;  s = Scale - 0.8f; break;
			case GET_ITEM(6, 17):
			case GET_ITEM(6, 18):
			case GET_ITEM(6, 19):
			case GET_ITEM(6, 20):
			case GET_ITEM(6, 21): x = sx - 8; y = sy + 2;  s = Scale - 0.8f; break;
			case GET_ITEM(6, 100): x = sx - 12; y = sy + 8;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 101):
			case GET_ITEM(6, 102): x = sx - 12; y = sy + 6;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 103): x = sx - 12; y = sy + 10;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 104): x = sx - 12; y = sy + 12;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 105):
			case GET_ITEM(6, 106): x = sx - 12; y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(6, 107):
			case GET_ITEM(6, 108): x = sx - 12; y = sy + 14;  s = Scale - 1.0f; break;
			case GET_ITEM(6, 109): x = sx - 4; y = sy + 20;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 110): x = sx - 8; y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(6, 111): x = sx - 10; y = sy + 2;  s = Scale - 0.9f; break;
			case GET_ITEM(6, 112): x = sx - 10; y = sy + 4;  s = Scale - 0.7f; break;

			case GET_ITEM(8, 0):
			case GET_ITEM(8, 2):
			case GET_ITEM(8, 3):
			case GET_ITEM(8, 4): x = sx - 5; y = sy + 15;  s = Scale - 1.0f; break;
			case GET_ITEM(8, 1): x = sx - 5; y = sy + 10;  s = Scale - 0.7f; break;
			case GET_ITEM(8, 5):
			case GET_ITEM(8, 6):
			case GET_ITEM(8, 7):
			case GET_ITEM(8, 8):
			case GET_ITEM(8, 9): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(8, 10):
			case GET_ITEM(8, 11): x = sx - 5; y = sy + 5; s = Scale - 1.0f; break;
			case GET_ITEM(8, 12):
			case GET_ITEM(8, 13):
			case GET_ITEM(8, 14): x = sx - 5; y = sy + 15; s = Scale - 1.0f; break;
			case GET_ITEM(8, 15): x = sx - 5; y = sy + 15; s = Scale - 0.8f; break;
			case GET_ITEM(8, 16):
			case GET_ITEM(8, 17):
			case GET_ITEM(8, 18):
			case GET_ITEM(8, 19):
			case GET_ITEM(8, 20):
			case GET_ITEM(8, 21):
			case GET_ITEM(8, 22): x = sx - 5; y = sy + 8; s = Scale - 0.6f; break;
			case GET_ITEM(8, 23):
			case GET_ITEM(8, 24): x = sx - 5; y = sy + 15; s = Scale - 0.6f; break;
			case GET_ITEM(8, 25):
			case GET_ITEM(8, 26):
			case GET_ITEM(8, 27):
			case GET_ITEM(8, 28): x = sx - 5; y = sy + 6; s = Scale - 0.6f; break;
			case GET_ITEM(8, 29): x = sx - 5; y = sy + 11; s = Scale - 0.6f; break;
			case GET_ITEM(8, 30): x = sx - 5; y = sy + 0; s = Scale - 0.3f; break;
			case GET_ITEM(8, 31): x = sx - 5; y = sy + 15; s = Scale - 0.9f; break;
			case GET_ITEM(8, 32): x = sx - 5; y = sy + 10; s = Scale - 0.7f; break;
			case GET_ITEM(8, 33):
			case GET_ITEM(8, 34): x = sx - 5; y = sy + 8; s = Scale - 0.8f; break;
			case GET_ITEM(8, 35): x = sx - 5; y = sy + 2; s = Scale - 0.6f; break;
			case GET_ITEM(8, 36): x = sx - 5; y = sy + 15; s = Scale - 0.9f; break;
			case GET_ITEM(8, 37): x = sx - 5; y = sy + 18; s = Scale - 0.9f; break;
			case GET_ITEM(8, 38): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(8, 39):
			case GET_ITEM(8, 40):
			case GET_ITEM(8, 41):
			case GET_ITEM(8, 42):
			case GET_ITEM(8, 43):
			case GET_ITEM(8, 44): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(8, 45):
			case GET_ITEM(8, 46):
			case GET_ITEM(8, 47):
			case GET_ITEM(8, 48): x = sx - 5; y = sy + 22; s = Scale - 0.9f; break;
			case GET_ITEM(8, 49): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(8, 50): x = sx - 5; y = sy + 15; s = Scale - 0.9f; break;
			case GET_ITEM(8, 51):
			case GET_ITEM(8, 52):
			case GET_ITEM(8, 53): x = sx - 5; y = sy + 22; s = Scale - 0.9f; break;
			case GET_ITEM(8, 59): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(8, 60): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(8, 61): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(8, 73): x = sx - 5; y = sy + 16; s = Scale - 0.9f; break;
			case GET_ITEM(8, 77): x = sx - 5; y = sy + 22; s = Scale - 0.9f; break;
			case GET_ITEM(8, 100): x = sx - 5; y = sy + 24; s = Scale - 0.9f; break;

			case GET_ITEM(9, 0): 
			case GET_ITEM(9, 1): 
			case GET_ITEM(9, 2): 
			case GET_ITEM(9, 3): 
			case GET_ITEM(9, 4): 
			case GET_ITEM(9, 5): 
			case GET_ITEM(9, 6): 
			case GET_ITEM(9, 7): 
			case GET_ITEM(9, 8): 
			case GET_ITEM(9, 9): 
			case GET_ITEM(9, 10):
			case GET_ITEM(9, 11):
			case GET_ITEM(9, 12):
			case GET_ITEM(9, 13):
			case GET_ITEM(9, 14):
			case GET_ITEM(9, 15):
			case GET_ITEM(9, 16):
			case GET_ITEM(9, 17): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 18): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 19): 
			case GET_ITEM(9, 20): 
			case GET_ITEM(9, 21): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 22): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 23): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 24): x = sx - 5; y = sy + 15; s = Scale - 0.9f; break;
			case GET_ITEM(9, 25): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 26): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 27): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 28): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 29): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 30): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 31): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 32): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 33): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 34): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 35): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 36): x = sx - 5; y = sy + 16; s = Scale - 0.9f; break;
			case GET_ITEM(9, 37): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 38): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 39): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 40): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 41): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 42): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 43): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 44): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 45): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 46): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 47): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 48): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 49): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 50): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 51): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 52): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 53): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 59): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 60): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 61): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 73): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 77): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 100): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 101): x = sx - 5; y = sy + 4; s = Scale - 0.9f; break;
			case GET_ITEM(9, 102): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 103): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 104): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 105): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 106): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 107): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 108): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 109): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 110): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 111): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 112): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 113): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 114): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 115): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 116): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 117): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 118): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 119): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 120): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 121): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 122): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 123): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 124): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 125): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 126): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 127): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 128): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 129): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 130): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 131): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 132): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 133): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 134): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 135): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 136): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 137): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 138): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 139): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 140): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 141): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 142): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 143): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 144): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 145): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 146): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 147): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 148): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 149): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 150): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 151): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 152): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 153): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 154): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 155): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 156): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 157): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 158): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 159): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 160): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 161): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 162): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 163): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 164): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 165): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 166): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 167): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 168): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 169): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 170): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 171): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 172): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 173): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 174): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 175): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 176): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 177): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 178): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 179): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 180): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 181): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 182): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 183): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 184): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 185): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 186): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 187): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 188): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 189): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 190): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;

			case GET_ITEM(12, 0): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 1): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 2): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(12, 3): x = sx - 5; y = sy + 2; s = Scale - 0.7f; break;
			case GET_ITEM(12, 4): x = sx - 5; y = sy + 0; s = Scale - 0.7f; break;
			case GET_ITEM(12, 5): x = sx - 5; y = sy + 8; s = Scale - 0.8f; break;
			case GET_ITEM(12, 6): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 7): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 8): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 9): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 10): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 11): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 12): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 13): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 14): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 15): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 16): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 17): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 18): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 19): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 20): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(12, 21): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 22): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 23): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 24): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 30): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 31): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 32): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 33): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 34): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 35): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 36): x = sx - 5; y = sy + 12; s = Scale - 0.7f; break;
			case GET_ITEM(12, 37): x = sx - 5; y = sy + 4; s = Scale - 0.7f; break;
			case GET_ITEM(12, 38): x = sx - 5; y = sy + 4; s = Scale - 0.7f; break;
			case GET_ITEM(12, 39): x = sx - 5; y = sy + 12; s = Scale - 1.1f; break;
			case GET_ITEM(12, 40): x = sx - 5; y = sy + 4; s = Scale - 0.9f; break;
			case GET_ITEM(12, 41): x = sx - 5; y = sy + 8; s = Scale - 1.1f; break;
			case GET_ITEM(12, 42): x = sx - 5; y = sy + 2; s = Scale - 1.0f; break;
			case GET_ITEM(12, 43): x = sx - 5; y = sy + 2; s = Scale - 0.76f; break;
			case GET_ITEM(12, 44): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 45): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 46): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 47): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 48): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 49): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 50): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 60): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 61): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 62): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 63): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 64): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 65): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 70): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 71): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 72): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 73): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 74): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 100): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 101): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 102): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 103): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 104): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 105): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 106): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 107): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 108): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 109): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 110): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 111): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 112): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 113): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 114): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 115): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 116): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 117): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 118): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 119): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 120): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 121): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 122): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 123): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 124): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 125): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 126): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 127): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 128): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 129): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(12, 130): x = sx - 5; y = sy + 0; s = Scale - 1.2f; break;
			case GET_ITEM(12, 131): x = sx - 5; y = sy + 8; s = Scale - 1.2f; break;
			case GET_ITEM(12, 132): x = sx - 5; y = sy + 4; s = Scale - 1.2f; break;
			case GET_ITEM(12, 133): x = sx - 5; y = sy + 0; s = Scale - 1.2f; break;
			case GET_ITEM(12, 134): x = sx - 5; y = sy + 10; s = Scale - 1.2f; break;
			case GET_ITEM(12, 135): x = sx - 5; y = sy + 6; s = Scale - 1.2f; break;
			case GET_ITEM(12, 136): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 137): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 138): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 139): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 140): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 141): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 142): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 143): x = sx - 5; y = sy + 0; s = Scale - 0.9f; break;
			case GET_ITEM(12, 150): x = sx - 5; y = sy - 4; s = Scale - 0.9f; break;
			case GET_ITEM(12, 151): x = sx - 5; y = sy + 4; s = Scale - 0.9f; break;
			case GET_ITEM(12, 152): x = sx - 5; y = sy + 0; s = Scale - 0.8f; break;
			case GET_ITEM(12, 153): x = sx - 10; y = sy + 8; s = Scale - 0.8f; break;
			case GET_ITEM(12, 154): x = sx - 10; y = sy + 0; s = Scale - 0.8f; break;
			case GET_ITEM(12, 155): x = sx - 5; y = sy + 0; s = Scale - 0.8f; break;
			case GET_ITEM(12, 156): x = sx - 5; y = sy + 0; s = Scale - 0.7f; break;
			case GET_ITEM(12, 157): x = sx - 5; y = sy + 4; s = Scale - 0.5f; break;
			case GET_ITEM(12, 158): x = sx - 5; y = sy + 0; s = Scale - 0.6f; break;
			case GET_ITEM(12, 159): x = sx - 5; y = sy + 0; s = Scale - 0.8f; break;
			case GET_ITEM(12, 160): x = sx - 5; y = sy - 2; s = Scale - 0.8f; break;
			case GET_ITEM(12, 161): x = sx - 5; y = sy - 2; s = Scale - 0.8f; break;
			case GET_ITEM(12, 162): x = sx - 5; y = sy - 2; s = Scale - 0.8f; break;
			case GET_ITEM(12, 210): x = sx - 5; y = sy + 0; s = Scale - 0.6f; break;
			case GET_ITEM(12, 211): x = sx - 5; y = sy + 8; s = Scale - 0.6f; break;
			case GET_ITEM(12, 212): x = sx - 5; y = sy + 8; s = Scale - 0.6f; break;
			case GET_ITEM(12, 213): x = sx - 5; y = sy + 12; s = Scale - 0.6f; break;
			case GET_ITEM(12, 214): x = sx - 5; y = sy + 4; s = Scale - 0.4f; break;
			case GET_ITEM(12, 215): x = sx - 5; y = sy + 4; s = Scale - 0.4f; break;
			case GET_ITEM(12, 216): x = sx - 5; y = sy + 8; s = Scale - 0.4f; break;
			case GET_ITEM(12, 217): x = sx - 10; y = sy + 8; s = Scale - 0.7f; break;
			case GET_ITEM(12, 218): x = sx - 10; y = sy + 8; s = Scale - 0.7f; break;
			case GET_ITEM(12, 219): x = sx - 10; y = sy + 0; s = Scale - 0.4f; break;
			case GET_ITEM(12, 220): x = sx - 10; y = sy + 12; s = Scale - 0.4f; break;
			case GET_ITEM(12, 221): x = sx - 10; y = sy + 4; s = Scale - 0.4f; break;
			case GET_ITEM(12, 222): x = sx - 10; y = sy + 4; s = Scale - 0.7f; break;
			case GET_ITEM(12, 223): x = sx - 10; y = sy + 4; s = Scale - 0.7f; break;
			case GET_ITEM(12, 224): x = sx - 10; y = sy + 4; s = Scale - 0.5f; break;
			case GET_ITEM(12, 225): x = sx - 10; y = sy + 4; s = Scale - 0.5f; break;
			case GET_ITEM(12, 226): x = sx - 10; y = sy + 4; s = Scale - 0.9f; break;
			case GET_ITEM(12, 227): x = sx - 10; y = sy - 2; s = Scale - 0.4f; break;
			case GET_ITEM(12, 228): x = sx - 10; y = sy + 0; s = Scale - 0.4f; break;
			case GET_ITEM(12, 229): x = sx - 10; y = sy + 10; s = Scale - 0.7f; break;
			case GET_ITEM(12, 230): x = sx - 10; y = sy + 4; s = Scale - 0.9f; break;

			case GET_ITEM(13, 0): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 1): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 2): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 3): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 4): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 5): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 7): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 8): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 9): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 10): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 11): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 12): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 13): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 14): x = sx - 5; y = sy + 10; s = Scale - 1.0f; break;
			case GET_ITEM(13, 15): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 16): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 17): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 18): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 19): x = sx - 5; y = sy + 5; s = Scale - 1.0f; break;
			case GET_ITEM(13, 20): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 21): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 22): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 23): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 24): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 25): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 26): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 27): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 28): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 29): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 30): x = sx - 5; y = sy + 4; s = Scale - 0.9f; break;
			case GET_ITEM(13, 31): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(13, 32): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 33): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 34): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 35): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 36): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 37): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(13, 38): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 39): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 40): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 41): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 42): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 43): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 44): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 45): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 46): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 47): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 48): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 49): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 50): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 51): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 52): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 53): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 54): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(13, 55): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(13, 56): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(13, 57): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(13, 58): x = sx - 5; y = sy + 4; s = Scale - 1.0f; break;
			case GET_ITEM(13, 59): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 60): x = sx - 5; y = sy + 8; s = Scale - 1.0f; break;
			case GET_ITEM(13, 61): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 62): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 63): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 64): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 65): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 66): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 67): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 68): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 69): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 70): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			case GET_ITEM(13, 71): x = sx - 5; y = sy + 4; s = Scale - 0.8f; break;
			case GET_ITEM(13, 72): x = sx - 5; y = sy + 4; s = Scale - 0.8f; break;
			case GET_ITEM(13, 73): x = sx - 5; y = sy + 4; s = Scale - 0.8f; break;
			case GET_ITEM(13, 74): x = sx - 5; y = sy + 4; s = Scale - 0.8f; break;
			case GET_ITEM(13, 75): x = sx - 5; y = sy + 4; s = Scale - 0.8f; break;
			case GET_ITEM(13, 76): x = sx - 5; y = sy + 0; s = Scale - 0.8f; break;

			case GET_ITEM(14, 157):
			case GET_ITEM(14, 158):
			case GET_ITEM(14, 159): 
			case GET_ITEM(14, 160): 
			case GET_ITEM(14, 161): 
			case GET_ITEM(14, 162): x = sx - 5; y = sy + 8; s = Scale - 0.8f; break;
			case GET_ITEM(14, 163): x = sx - 5; y = sy + 5; s = Scale - 0.8f; break;
			case GET_ITEM(14, 173): x = sx - 5; y = sy + 5; s = Scale - 0.8f; break;
			case GET_ITEM(14, 174): 
			case GET_ITEM(14, 175): 
			case GET_ITEM(14, 176): 
			case GET_ITEM(14, 177): 
			case GET_ITEM(14, 178): 
			case GET_ITEM(14, 179): x = sx - 5; y = sy + 0; s = Scale - 1.0f; break;
			
			default:

			break;
		}
		
		if (Type >= GET_ITEM(7, 0) && Type <= GET_ITEM(7, 190))
		{
			x = sx - 5;	y = sy + 4;	s = Scale - 1.0;
		}
		else if (Type >= GET_ITEM(8, 100) && Type <= GET_ITEM(8, 190))
		{
			x = sx - 5;	y = sy + 24; s = Scale - 0.9;
		}
		else if (Type >= GET_ITEM(10, 0) && Type <= GET_ITEM(10, 190))
		{
			x = sx - 5;	y = sy + 10; s = Scale - 0.9;
		}
		else if (Type >= GET_ITEM(11, 0) && Type <= GET_ITEM(11, 190))
		{
			x = sx - 5;	y = sy + 10; s = Scale - 0.9;
		}
		else if (Type >= GET_ITEM(13, 76) && Type <= GET_ITEM(13, 145))
		{
			x = sx - 5; y = sy + 0; s = Scale - 0.8f;
		}
		else if (Type >= GET_ITEM(14, 0) && Type <= GET_ITEM(14, 150))
		{
			x = sx - 5; y = sy + 0; s = Scale - 0.8f;
		}
		else if (Type >= GET_ITEM(14, 190) && Type <= GET_ITEM(14, 215))
		{
			x = sx - 5; y = sy - 8; s = Scale - 1.0f;
		}
		else if (Type >= GET_ITEM(14, 220) && Type <= GET_ITEM(14, 252))
		{
			x = sx - 8; y = sy + 0; s = Scale - 1.0f;
		}
		else if (Type >= GET_ITEM(15, 0) && Type <= GET_ITEM(15, 36))
		{
			x = sx - 8; y = sy + 0; s = Scale - 1.0f;
		}
	}
	else
	{
		switch (Type)
		{
			case GET_ITEM(0, 0):  x = sx - 5; y = sy + 7;  s = Scale - 1.0f; break;
			case GET_ITEM(0, 1):  x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(0, 2):
			case GET_ITEM(0, 3):
			case GET_ITEM(0, 4):
			case GET_ITEM(0, 5):
			case GET_ITEM(0, 6):
			case GET_ITEM(0, 7):
			case GET_ITEM(0, 8):
			case GET_ITEM(0, 9):
			case GET_ITEM(0, 10):
			case GET_ITEM(0, 11):
			case GET_ITEM(0, 12):
			case GET_ITEM(0, 13): x = sx - 6; y = sy + 10; s = Scale - 0.6f; break;
			case GET_ITEM(0, 14):
			case GET_ITEM(0, 15):
			case GET_ITEM(0, 16): x = sx - 6; y = sy + 10; s = Scale - 0.4f; break;
			case GET_ITEM(0, 17): x = sx - 6; y = sy + 10; s = Scale - 0.5f; break;
			case GET_ITEM(0, 18): x = sx - 6; y = sy + 10; s = Scale - 0.7f; break;
			case GET_ITEM(0, 19): x = sx - 6; y = sy + 10; s = Scale - 0.4f; break;
			case GET_ITEM(0, 20):
			case GET_ITEM(0, 21): x = sx - 6; y = sy + 15; s = Scale - 0.7f; break;
			case GET_ITEM(0, 22): x = sx - 6; y = sy + 15; s = Scale - 0.5f; break;
			case GET_ITEM(0, 23): x = sx - 6; y = sy + 12; s = Scale - 0.5f; break;
			case GET_ITEM(0, 24): x = sx - 6; y = sy + 15; s = Scale - 0.5f; break;
			case GET_ITEM(0, 25): x = sx - 6; y = sy + 12; s = Scale - 0.6f; break;
			case GET_ITEM(0, 26):
			case GET_ITEM(0, 27): x = sx - 6; y = sy + 15; s = Scale - 0.6f; break;
			case GET_ITEM(0, 28): x = sx - 6; y = sy + 15; s = Scale - 0.5f; break;
			case GET_ITEM(0, 31): x = sx - 6; y = sy + 10; s = Scale - 0.5f; break;
			case GET_ITEM(0, 32):
			case GET_ITEM(0, 33):
			case GET_ITEM(0, 34):
			case GET_ITEM(0, 35):
			case GET_ITEM(0, 46):
			case GET_ITEM(0, 52):
			case GET_ITEM(0, 60):
			case GET_ITEM(0, 61):
			case GET_ITEM(0, 81):
			case GET_ITEM(0, 102):
			case GET_ITEM(0, 105):
			case GET_ITEM(0, 108):
			case GET_ITEM(0, 114):
			case GET_ITEM(0, 117):
			case GET_ITEM(0, 120):
			case GET_ITEM(0, 123):
			case GET_ITEM(0, 126):
			case GET_ITEM(0, 129):
			case GET_ITEM(0, 132):
			case GET_ITEM(0, 135):
			case GET_ITEM(0, 138): x = sx - 6; y = sy + 2;  s = Scale - 0.9f; break;
			case GET_ITEM(0, 47):  x = sx - 6; y = sy + 12; s = Scale - 0.5f; break;
			case GET_ITEM(0, 51):  x = sx - 6; y = sy + 9;  s = Scale - 0.2f; break;
			case GET_ITEM(0, 83):  x = sx - 6; y = sy + 8;  s = Scale - 0.2f; break;
			case GET_ITEM(0, 100):
			case GET_ITEM(0, 101):
			case GET_ITEM(0, 103):
			case GET_ITEM(0, 104):
			case GET_ITEM(0, 106):
			case GET_ITEM(0, 107): x = sx - 6; y = sy + 12; s = Scale - 0.5f; break;
			case GET_ITEM(0, 109): x = sx - 6; y = sy + 10; s = Scale - 0.6f; break;
			case GET_ITEM(0, 110): x = sx - 6; y = sy + 5;  s = Scale - 0.4f; break;
			case GET_ITEM(0, 111): x = sx - 6; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(0, 112):
			case GET_ITEM(0, 113): x = sx - 6; y = sy + 13; s = Scale - 0.7f; break;
			case GET_ITEM(0, 115): x = sx - 6; y = sy + 13; s = Scale - 0.6f; break;
			case GET_ITEM(0, 116):
			case GET_ITEM(0, 118): x = sx - 6; y = sy + 13; s = Scale - 0.7f; break;
			case GET_ITEM(0, 119): x = sx - 6; y = sy + 13; s = Scale - 0.6f; break;
			case GET_ITEM(0, 121): x = sx - 6; y = sy + 8;  s = Scale - 0.6f; break;
			case GET_ITEM(0, 124): x = sx - 6; y = sy + 10; s = Scale - 0.8f; break;
			case GET_ITEM(0, 125): x = sx - 6; y = sy + 13; s = Scale - 0.6f; break;
			case GET_ITEM(0, 127): x = sx - 6; y = sy + 13; s = Scale - 0.75f; break;
			case GET_ITEM(0, 128): x = sx - 6; y = sy + 13; s = Scale - 0.6f; break;
			case GET_ITEM(0, 130): x = sx - 6; y = sy + 13; s = Scale - 0.7f; break;
			case GET_ITEM(0, 131): x = sx - 6; y = sy + 11; s = Scale - 0.7f; break;
			case GET_ITEM(0, 133):
			case GET_ITEM(0, 134):
			case GET_ITEM(0, 136):
			case GET_ITEM(0, 137): x = sx - 6; y = sy + 12; s = Scale - 0.3f; break;


			case GET_ITEM(1, 0):
			case GET_ITEM(1, 1):
			case GET_ITEM(1, 2):
			case GET_ITEM(1, 3):
			case GET_ITEM(1, 4):
			case GET_ITEM(1, 5):
			case GET_ITEM(1, 6): x = sx - 6; y = sy + 14; s = Scale - 1.0f; break;
			case GET_ITEM(1, 7): x = sx - 6; y = sy + 14; s = Scale - 0.9f; break;
			case GET_ITEM(1, 8): x = sx - 6; y = sy + 14; s = Scale - 0.8f; break;

			case GET_ITEM(2, 0): x = sx - 6; y = sy + 14; s = Scale - 1.0f; break;
			case GET_ITEM(2, 1): x = sx - 6; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(2, 2): x = sx - 6; y = sy + 0;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 3): x = sx - 6; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(2, 4): x = sx - 6; y = sy + 8;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 5): x = sx - 6; y = sy + 18; s = Scale - 0.5f; break;
			case GET_ITEM(2, 6): x = sx - 6; y = sy + 16; s = Scale - 0.6f; break;
			case GET_ITEM(2, 7): x = sx - 6; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(2, 8): x = sx - 6; y = sy + 11; s = Scale - 0.8f; break;
			case GET_ITEM(2, 9): x = sx - 6; y = sy + 13; s = Scale - 0.8f; break;

			case GET_ITEM(2, 10):
			case GET_ITEM(2, 11):
			case GET_ITEM(2, 12):
			case GET_ITEM(2, 13): x = sx - 6; y = sy + 5;  s = Scale - 0.5f; break;
			case GET_ITEM(2, 14):
			case GET_ITEM(2, 15): x = sx - 8; y = sy + 8;  s = Scale - 0.3f; break;
			case GET_ITEM(2, 16): x = sx - 8; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(2, 17): x = sx - 8; y = sy + 12; s = Scale - 0.45f; break;
			case GET_ITEM(2, 18): x = sx - 8; y = sy + 12; s = Scale - 0.7f; break;
			case GET_ITEM(2, 25): x = sx - 8; y = sy + 4;  s = Scale - 0.5f; break;
			case GET_ITEM(2, 34):
			case GET_ITEM(2, 35): x = sx - 8; y = sy + 4;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 100): x = sx - 8; y = sy + 10; s = Scale - 0.8f; break;
			case GET_ITEM(2, 101): x = sx - 8; y = sy + 5;  s = Scale - 0.7f; break;
			case GET_ITEM(2, 102): x = sx - 8; y = sy + 3;  s = Scale - 0.5f; break;
			case GET_ITEM(2, 103):
			case GET_ITEM(2, 104): x = sx - 8; y = sy + 5;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 105): x = sx - 8; y = sy + 7;  s = Scale - 1.1f; break;
			case GET_ITEM(2, 106):
			case GET_ITEM(2, 107): x = sx - 8; y = sy + 3;  s = Scale - 0.7f; break;
			case GET_ITEM(2, 108):
			case GET_ITEM(2, 109): x = sx - 8; y = sy - 2;  s = Scale - 0.7f; break;
			case GET_ITEM(2, 110): x = sx - 8; y = sy + 5;  s = Scale - 0.9f; break;
			case GET_ITEM(2, 111):
			case GET_ITEM(2, 112): x = sx - 8; y = sy + 8;  s = Scale - 0.0f; break;
			case GET_ITEM(3, 0): x = sx - 8;  y = sy + 8;  s = Scale - 0.3f; break;
			case GET_ITEM(3, 1): x = sx - 0;  y = sy + 6;  s = Scale - 0.3f; break;
			case GET_ITEM(3, 2): x = sx - 0;  y = sy + 20; s = Scale - 0.7f; break;
			case GET_ITEM(3, 3):
			case GET_ITEM(3, 4): x = sx - 0;  y = sy + 8;  s = Scale - 0.5f; break;
			case GET_ITEM(3, 5): x = sx - 0;  y = sy + 4;  s = Scale - 0.8f; break;
			case GET_ITEM(3, 6): x = sx - 0;  y = sy + 7;  s = Scale - 0.8f; break;
			case GET_ITEM(3, 7): x = sx - 0;  y = sy + 2;  s = Scale - 0.6f; break;
			case GET_ITEM(3, 8): x = sx - 0;  y = sy + 7;  s = Scale - 0.6f; break;
			case GET_ITEM(3, 9): x = sx - 0;  y = sy + 3;  s = Scale - 0.4f; break;
			case GET_ITEM(3, 10): x = sx - 0; y = sy + 11; s = Scale - 0.5f; break;
			case GET_ITEM(3, 11): x = sx - 0; y = sy + 8;  s = Scale - 0.5f; break;

			case GET_ITEM(4, 0): x = sx - 12; y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(4, 1): x = sx - 12; y = sy + 6;  s = Scale - 1.0f; break;
			case GET_ITEM(4, 2):
			case GET_ITEM(4, 3): x = sx - 12; y = sy + 6;  s = Scale - 0.9f; break;
			case GET_ITEM(4, 4): x = sx - 12; y = sy + 4;  s = Scale - 0.9f; break;
			case GET_ITEM(4, 5): x = sx - 8;  y = sy + 4;  s = Scale - 0.7f; break;
			case GET_ITEM(4, 6): x = sx - 8;  y = sy + 1;  s = Scale - 0.4f; break;
			case GET_ITEM(4, 7): x = sx - 0;  y = sy + 2;  s = Scale - 1.3f; break;
			case GET_ITEM(4, 8): x = sx - 5;  y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(4, 9): x = sx - 5;  y = sy + 13; s = Scale - 1.0f; break;
			case GET_ITEM(4, 10): x = sx - 5; y = sy + 12; s = Scale - 1.0f; break;
			case GET_ITEM(4, 11): x = sx - 5; y = sy + 14; s = Scale - 1.0f; break;
			case GET_ITEM(4, 12): x = sx - 5; y = sy + 18; s = Scale - 1.0f; break;
			case GET_ITEM(4, 13): x = sx - 5; y = sy + 14; s = Scale - 1.0f; break;
			case GET_ITEM(4, 14): x = sx - 5; y = sy + 11; s = Scale - 0.9f; break;
			case GET_ITEM(4, 15): x = sx - 8; y = sy + 0;  s = Scale - 1.3f; break;
			case GET_ITEM(4, 16): x = sx - 0; y = sy + 16; s = Scale - 1.0f; break;
			case GET_ITEM(4, 17): x = sx - 0; y = sy + 1;  s = Scale - 0.58f; break;
			case GET_ITEM(4, 18): x = sx - 0; y = sy + 15; s = Scale - 0.8f; break;
			case GET_ITEM(4, 19): x = sx - 0; y = sy + 12; s = Scale - 1.0f; break;
			case GET_ITEM(4, 20): x = sx - 8; y = sy + 6;  s = Scale - 0.45f; break;
			case GET_ITEM(4, 21): x = sx - 8; y = sy + 12; s = Scale - 0.45f; break;
			case GET_ITEM(4, 22): x = sx - 8; y = sy + 12; s = Scale - 0.6f; break;
			case GET_ITEM(4, 23): x = sx - 4; y = sy + 8;  s = Scale - 0.37f; break;
			case GET_ITEM(4, 24): x = sx - 4; y = sy + 8;  s = Scale - 0.6f; break;
			case GET_ITEM(4, 30): x = sx - 4; y = sy + 12; s = Scale - 0.6f; break;
			case GET_ITEM(4, 100): x = sx - 4;  y = sy + 8;  s = Scale - 0.7f; break;
			case GET_ITEM(4, 101): x = sx - 4;  y = sy + 6;  s = Scale - 0.45f; break;
			case GET_ITEM(4, 102): x = sx - 4;  y = sy + 4;  s = Scale - 0.5f; break;
			case GET_ITEM(4, 103): x = sx - 8;  y = sy + 8;  s = Scale - 0.45f; break;
			case GET_ITEM(4, 104): x = sx - 8;  y = sy + 6;  s = Scale - 0.45f; break;
			case GET_ITEM(4, 105): x = sx - 0;  y = sy + 6;  s = Scale - 0.5f; break;
			case GET_ITEM(4, 106): x = sx - 8;  y = sy + 8;  s = Scale - 0.6f; break;
			case GET_ITEM(4, 107):
			case GET_ITEM(4, 108): x = sx - 8;  y = sy + 6;  s = Scale - 0.0f; break;
			case GET_ITEM(4, 109): x = sx - 10; y = sy + 8;  s = Scale - 0.35f; break;
			case GET_ITEM(4, 110): x = sx - 10; y = sy + 9;  s = Scale - 0.6f; break;
			case GET_ITEM(4, 111):
			case GET_ITEM(4, 112): x = sx - 10; y = sy + 6;  s = Scale - 0.2f; break;
			case GET_ITEM(5, 0):  x = sx - 2; y = sy + 6;  s = Scale - 1.0f; break;
			case GET_ITEM(5, 1):  x = sx - 2; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(5, 2):  x = sx - 2; y = sy + 4;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 3):  x = sx - 2; y = sy + 2;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 4):  x = sx - 2; y = sy + 10; s = Scale - 0.7f; break;
			case GET_ITEM(5, 5):  x = sx - 2; y = sy + 8;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 6):  x = sx - 2; y = sy + 8;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 7):  x = sx - 2; y = sy + 6;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 8):  x = sx - 6; y = sy + 2;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 9):  x = sx - 8; y = sy + 6;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 10): x = sx - 4; y = sy + 4;  s = Scale - 0.3f; break;
			case GET_ITEM(5, 11): x = sx - 4; y = sy + 6;  s = Scale - 0.6f; break;
			case GET_ITEM(5, 12):
			case GET_ITEM(5, 13): x = sx - 8; y = sy + 4;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 14): x = sx - 8; y = sy + 4;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 15): x = sx - 8; y = sy + 6;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 16): x = sx - 8; y = sy + 5;  s = Scale - 0.75f; break;
			case GET_ITEM(5, 17): x = sx - 8; y = sy + 5;  s = Scale - 0.6f; break;
			case GET_ITEM(5, 18):
			case GET_ITEM(5, 19): x = sx - 4; y = sy + 4;  s = Scale - 0.75f; break;
			case GET_ITEM(5, 20): x = sx - 4; y = sy + 4;  s = Scale - 0.45f; break;
			case GET_ITEM(5, 21):
			case GET_ITEM(5, 22):
			case GET_ITEM(5, 23): x = sx - 8; y = sy + 0;  s = Scale - 1.2f; break;
			case GET_ITEM(5, 30): x = sx - 6; y = sy + 8;  s = Scale - 0.5f; break;
			case GET_ITEM(5, 31): x = sx - 6; y = sy + 8;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 32): x = sx - 6; y = sy + 8;  s = Scale - 0.48f; break;
			case GET_ITEM(5, 33): x = sx - 6; y = sy + 4;  s = Scale - 0.55f; break;
			case GET_ITEM(5, 34): x = sx - 6; y = sy + 8;  s = Scale - 0.65f; break;
			case GET_ITEM(5, 36): x = sx - 6; y = sy + 2;  s = Scale - 0.55f; break;
			case GET_ITEM(5, 49): x = sx - 6; y = sy + 8;  s = Scale - 0.45f; break;
			case GET_ITEM(5, 50): x = sx - 6; y = sy + 4;  s = Scale - 0.45f; break;
			case GET_ITEM(5, 100): x = sx - 4; y = sy + 8;  s = Scale - 0.6f; break;
			case GET_ITEM(5, 101): x = sx - 4; y = sy + 8;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 102): x = sx - 4; y = sy + 9;  s = Scale - 0.4f; break;
			case GET_ITEM(5, 103): x = sx - 4; y = sy + 2;  s = Scale - 0.7f; break;
			case GET_ITEM(5, 104): x = sx - 4; y = sy + 10; s = Scale - 0.8f; break;
			case GET_ITEM(5, 105): x = sx - 4; y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(5, 106): x = sx - 4; y = sy + 6;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 107): x = sx - 4; y = sy + 23; s = Scale - 1.2f; break;
			case GET_ITEM(5, 108): x = sx - 4; y = sy + 4;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 109): x = sx - 4; y = sy + 10; s = Scale - 1.0f; break;
			case GET_ITEM(5, 110): x = sx - 2; y = sy + 5;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 111): x = sx - 2; y = sy + 5;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 112): x = sx - 2; y = sy + 3;  s = Scale - 0.7f; break;
			case GET_ITEM(5, 113): x = sx - 8; y = sy + 10; s = Scale - 1.0f; break;
			case GET_ITEM(5, 114): x = sx - 0; y = sy + 5;  s = Scale - 0.8f; break;
			case GET_ITEM(5, 115): x = sx - 4; y = sy + 17; s = Scale - 0.7f; break;
			case GET_ITEM(5, 116): x = sx - 4; y = sy + 6;  s = Scale - 0.6f; break;
			case GET_ITEM(5, 117): x = sx - 4; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(5, 118): x = sx - 4; y = sy + 4;  s = Scale - 0.82f; break;
			case GET_ITEM(5, 119): x = sx - 4; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(5, 120): x = sx - 4; y = sy + 0;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 121): x = sx - 4; y = sy + 8;  s = Scale - 0.9f; break;
			case GET_ITEM(5, 122):
			case GET_ITEM(5, 123): x = sx - 4; y = sy + 8;  s = Scale - 0.1f; break;
			case GET_ITEM(5, 124):
			case GET_ITEM(5, 125): x = sx - 4; y = sy + 9;  s = Scale - 0.0f; break;

			case GET_ITEM(6, 0):
			case GET_ITEM(6, 1):
			case GET_ITEM(6, 2):
			case GET_ITEM(6, 3):
			case GET_ITEM(6, 4):
			case GET_ITEM(6, 5):
			case GET_ITEM(6, 6):
			case GET_ITEM(6, 7):
			case GET_ITEM(6, 8):
			case GET_ITEM(6, 9):
			case GET_ITEM(6, 10):
			case GET_ITEM(6, 11):
			case GET_ITEM(6, 12): x = sx - 4; y = sy + 5;  s = Scale - 1.0f; break;
			case GET_ITEM(6, 13):
			case GET_ITEM(6, 14):
			case GET_ITEM(6, 15):
			case GET_ITEM(6, 16): x = sx - 4; y = sy + 8;  s = Scale - 0.8f; break;
			case GET_ITEM(6, 17):
			case GET_ITEM(6, 18):
			case GET_ITEM(6, 19):
			case GET_ITEM(6, 20):
			case GET_ITEM(6, 21): x = sx - 8; y = sy + 2;  s = Scale - 0.8f; break;
			case GET_ITEM(6, 100): x = sx - 12; y = sy + 8;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 101):
			case GET_ITEM(6, 102): x = sx - 12; y = sy + 6;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 103): x = sx - 12; y = sy + 10;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 104): x = sx - 12; y = sy + 12;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 105):
			case GET_ITEM(6, 106): x = sx - 12; y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(6, 107):
			case GET_ITEM(6, 108): x = sx - 12; y = sy + 14;  s = Scale - 1.0f; break;
			case GET_ITEM(6, 109): x = sx - 4; y = sy + 20;  s = Scale - 1.1f; break;
			case GET_ITEM(6, 110): x = sx - 8; y = sy + 8;  s = Scale - 1.0f; break;
			case GET_ITEM(6, 111): x = sx - 10; y = sy + 2;  s = Scale - 0.9f; break;
			case GET_ITEM(6, 112): x = sx - 10; y = sy + 4;  s = Scale - 0.7f; break;

			case GET_ITEM(8, 0):
			case GET_ITEM(8, 2):
			case GET_ITEM(8, 3):
			case GET_ITEM(8, 4): x = sx - 5; y = sy + 15;  s = Scale - 1.0f; break;
			case GET_ITEM(8, 1): x = sx - 5; y = sy + 10;  s = Scale - 0.7f; break;
			case GET_ITEM(8, 5):
			case GET_ITEM(8, 6):
			case GET_ITEM(8, 7):
			case GET_ITEM(8, 8):
			case GET_ITEM(8, 9): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(8, 10):
			case GET_ITEM(8, 11): x = sx - 5; y = sy + 5; s = Scale - 1.0f; break;
			case GET_ITEM(8, 12):
			case GET_ITEM(8, 13):
			case GET_ITEM(8, 14): x = sx - 5; y = sy + 15; s = Scale - 1.0f; break;
			case GET_ITEM(8, 15): x = sx - 5; y = sy + 15; s = Scale - 0.8f; break;
			case GET_ITEM(8, 16):
			case GET_ITEM(8, 17):
			case GET_ITEM(8, 18):
			case GET_ITEM(8, 19):
			case GET_ITEM(8, 20):
			case GET_ITEM(8, 21):
			case GET_ITEM(8, 22): x = sx - 5; y = sy + 8; s = Scale - 0.6f; break;
			case GET_ITEM(8, 23):
			case GET_ITEM(8, 24): x = sx - 5; y = sy + 15; s = Scale - 0.6f; break;
			case GET_ITEM(8, 25):
			case GET_ITEM(8, 26):
			case GET_ITEM(8, 27):
			case GET_ITEM(8, 28): x = sx - 5; y = sy + 6; s = Scale - 0.6f; break;
			case GET_ITEM(8, 29): x = sx - 5; y = sy + 11; s = Scale - 0.6f; break;
			case GET_ITEM(8, 30): x = sx - 5; y = sy + 0; s = Scale - 0.3f; break;
			case GET_ITEM(8, 31): x = sx - 5; y = sy + 15; s = Scale - 0.9f; break;
			case GET_ITEM(8, 32): x = sx - 5; y = sy + 10; s = Scale - 0.7f; break;
			case GET_ITEM(8, 33):
			case GET_ITEM(8, 34): x = sx - 5; y = sy + 8; s = Scale - 0.8f; break;
			case GET_ITEM(8, 35): x = sx - 5; y = sy + 2; s = Scale - 0.6f; break;
			case GET_ITEM(8, 36): x = sx - 5; y = sy + 15; s = Scale - 0.9f; break;
			case GET_ITEM(8, 37): x = sx - 5; y = sy + 18; s = Scale - 0.9f; break;
			case GET_ITEM(8, 38): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(8, 39):
			case GET_ITEM(8, 40):
			case GET_ITEM(8, 41):
			case GET_ITEM(8, 42):
			case GET_ITEM(8, 43):
			case GET_ITEM(8, 44): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(8, 45):
			case GET_ITEM(8, 46):
			case GET_ITEM(8, 47):
			case GET_ITEM(8, 48): x = sx - 5; y = sy + 22; s = Scale - 0.9f; break;
			case GET_ITEM(8, 49): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(8, 50): x = sx - 5; y = sy + 15; s = Scale - 0.9f; break;
			case GET_ITEM(8, 51):
			case GET_ITEM(8, 52):
			case GET_ITEM(8, 53): x = sx - 5; y = sy + 22; s = Scale - 0.9f; break;
			case GET_ITEM(8, 59): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(8, 60): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(8, 61): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(8, 73): x = sx - 5; y = sy + 16; s = Scale - 0.9f; break;
			case GET_ITEM(8, 77): x = sx - 5; y = sy + 22; s = Scale - 0.9f; break;
			case GET_ITEM(8, 100): x = sx - 5; y = sy + 24; s = Scale - 0.9f; break;

			case GET_ITEM(9, 0):
			case GET_ITEM(9, 1):
			case GET_ITEM(9, 2):
			case GET_ITEM(9, 3):
			case GET_ITEM(9, 4):
			case GET_ITEM(9, 5):
			case GET_ITEM(9, 6):
			case GET_ITEM(9, 7):
			case GET_ITEM(9, 8):
			case GET_ITEM(9, 9):
			case GET_ITEM(9, 10):
			case GET_ITEM(9, 11):
			case GET_ITEM(9, 12):
			case GET_ITEM(9, 13):
			case GET_ITEM(9, 14):
			case GET_ITEM(9, 15):
			case GET_ITEM(9, 16):
			case GET_ITEM(9, 17): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 18): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 19):
			case GET_ITEM(9, 20):
			case GET_ITEM(9, 21): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 22): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 23): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 24): x = sx - 5; y = sy + 15; s = Scale - 0.9f; break;
			case GET_ITEM(9, 25): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 26): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 27): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 28): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 29): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 30): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 31): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 32): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 33): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 34): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 35): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 36): x = sx - 5; y = sy + 16; s = Scale - 0.9f; break;
			case GET_ITEM(9, 37): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 38): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 39): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 40): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 41): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 42): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 43): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 44): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 45): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 46): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 47): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 48): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 49): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 50): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 51): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 52): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 53): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 59): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 60): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 61): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 73): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 77): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 100): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;
			case GET_ITEM(9, 101): x = sx - 5; y = sy + 4; s = Scale - 0.9f; break;
			case GET_ITEM(9, 102): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 103): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 104): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 105): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 106): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 107): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 108): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 109): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 110): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 111): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 112): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 113): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 114): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 115): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 116): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 117): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 118): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 119): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 120): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 121): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 122): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 123): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 124): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 125): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 126): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 127): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 128): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 129): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 130): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 131): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 132): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 133): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 134): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 135): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 136): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 137): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 138): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 139): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 140): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 141): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 142): x = sx - 5; y = sy + 8; s = Scale - 0.9f; break;
			case GET_ITEM(9, 143): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 144): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 145): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 146): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 147): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 148): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 149): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 150): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 151): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 152): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 153): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 154): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 155): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 156): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 157): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 158): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 159): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 160): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 161): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 162): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 163): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 164): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 165): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 166): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 167): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 168): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 169): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 170): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 171): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 172): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 173): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 174): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 175): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 176): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 177): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 178): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 179): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 180): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 181): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 182): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 183): x = sx - 5; y = sy + 6; s = Scale - 0.9f; break;
			case GET_ITEM(9, 184): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 185): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 186): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 187): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 188): x = sx - 5; y = sy + 12; s = Scale - 0.9f; break;
			case GET_ITEM(9, 189): x = sx - 5; y = sy + 2; s = Scale - 0.9f; break;
			case GET_ITEM(9, 190): x = sx - 5; y = sy + 10; s = Scale - 0.9f; break;

			case GET_ITEM(12, 0): x = sx + 1; y = sy + 4; s = Scale - 0.9; break;
			case GET_ITEM(12, 1): x = sx + 4; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(12, 2): x = sx + 2; y = sy + 8; s = Scale - 0.7; break;
			case GET_ITEM(12, 3): x = sx + 0; y = sy + 9; s = Scale - 0.5; break;
			case GET_ITEM(12, 4): x = sx + 0; y = sy + 9; s = Scale - 0.5; break;
			case GET_ITEM(12, 5): x = sx + 0; y = sy + 2; s = Scale - 0.5; break;
			case GET_ITEM(12, 6): x = sx + 0; y = sy + 2; s = Scale - 0.5; break;
			case GET_ITEM(12, 7):
			case GET_ITEM(12, 8):
			case GET_ITEM(12, 9):
			case GET_ITEM(12, 10):
			case GET_ITEM(12, 11): x = sx - 1; y = sy + 4; s = Scale - 0.9; break;
			case GET_ITEM(12, 12): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 13): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 14): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 15): x = sx + 2; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 16): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 17): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 18): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 19): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 20): x = sx + 2; y = sy + 7; s = Scale - 0.9; break;
			case GET_ITEM(12, 21): x = sx + 1; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 22): x = sx + 1; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 23): x = sx + 1; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 24): x = sx + 1; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 30): x = sx - 3; y = sy - 1; s = Scale - 0.9; break;
			case GET_ITEM(12, 31): x = sx - 3; y = sy - 1; s = Scale - 0.9; break;
			case GET_ITEM(12, 32): x = sx + 1; y = sy - 0; s = Scale - 0.9; break;
			case GET_ITEM(12, 33): x = sx + 1; y = sy - 0; s = Scale - 0.9; break;
			case GET_ITEM(12, 34): x = sx + 1; y = sy - 0; s = Scale - 0.9; break;
			case GET_ITEM(12, 35): x = sx + 1; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 36): x = sx + 2; y = sy + 7; s = Scale - 0.4; break;
			case GET_ITEM(12, 37): x = sx + 2; y = sy + 7; s = Scale - 0.4; break;
			case GET_ITEM(12, 38): x = sx + 2; y = sy + 8; s = Scale - 0.3; break;
			case GET_ITEM(12, 39): x = sx + 2; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(12, 40): x = sx + 2; y = sy + 8; s = Scale - 0.8; break;
			case GET_ITEM(12, 41): x = sx + 2; y = sy + 8; s = Scale - 0.8; break;
			case GET_ITEM(12, 42): x = sx + 2; y = sy + 6; s = Scale - 0.8; break;
			case GET_ITEM(12, 43): x = sx + 2; y = sy + 6; s = Scale - 0.5; break;
			case GET_ITEM(12, 44): x = sx + 1; y = sy + 4; s = Scale - 0.9; break;
			case GET_ITEM(12, 45): x = sx + 1; y = sy + 4; s = Scale - 0.9; break;
			case GET_ITEM(12, 46): x = sx + 1; y = sy + 4; s = Scale - 0.9; break;
			case GET_ITEM(12, 47): x = sx + 1; y = sy + 4; s = Scale - 0.9; break;
			case GET_ITEM(12, 48): x = sx + 1; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(12, 49): x = sx + 1; y = sy + 4; s = Scale - 0.9; break;
			case GET_ITEM(12, 50): x = sx + 1; y = sy + 4; s = Scale - 0.7; break;
			case GET_ITEM(12, 60): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 61): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 62): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 63): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 64): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 65): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 70): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 71): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 72): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 73): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 74): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 100): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 101): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 102): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 103): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 104): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 105): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 106): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 107): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 108): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 109): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 110): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 111): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 112): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 113): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 114): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 115): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 116): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 117): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 118): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 119): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 120): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 121): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 122): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 123): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 124): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 125): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 126): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 127): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 128): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 129): x = sx + 2; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(12, 130): x = sx + 0; y = sy + 2; s = Scale - 1.0; break;
			case GET_ITEM(12, 131): x = sx + 0; y = sy + 6; s = Scale - 1.0; break;
			case GET_ITEM(12, 132): x = sx + 1; y = sy + 6; s = Scale - 1.1; break;
			case GET_ITEM(12, 133): x = sx + 0; y = sy + 4; s = Scale - 1.1; break;
			case GET_ITEM(12, 134): x = sx + 0; y = sy + 10; s = Scale - 1.1; break;
			case GET_ITEM(12, 135): x = sx + 0; y = sy + 10; s = Scale - 1.1; break;
			case GET_ITEM(12, 136): x = sx - 3; y = sy - 6; s = Scale - 0.8; break;
			case GET_ITEM(12, 137): x = sx - 3; y = sy - 5; s = Scale - 0.8; break;
			case GET_ITEM(12, 138): x = sx - 3; y = sy - 4; s = Scale - 0.8; break;
			case GET_ITEM(12, 139): x = sx - 3; y = sy - 4; s = Scale - 0.8; break;
			case GET_ITEM(12, 140): x = sx - 3; y = sy - 4; s = Scale - 0.8; break;
			case GET_ITEM(12, 141): x = sx - 3; y = sy - 7; s = Scale - 1.0; break;
			case GET_ITEM(12, 142): x = sx - 3; y = sy - 3; s = Scale - 1.0; break;
			case GET_ITEM(12, 143): x = sx - 3; y = sy - 3; s = Scale - 0.8; break;
			case GET_ITEM(12, 150): x = sx - 0; y = sy - 0; s = Scale - 0.6; break;
			case GET_ITEM(12, 151): x = sx + 0; y = sy + 6; s = Scale - 0.6; break;
			case GET_ITEM(12, 152): x = sx + 0; y = sy + 2; s = Scale - 0.6; break;
			case GET_ITEM(12, 153): x = sx + 0; y = sy + 10; s = Scale - 0.6; break;
			case GET_ITEM(12, 154): x = sx + 0; y = sy - 2; s = Scale - 0.4; break;
			case GET_ITEM(12, 155): x = sx + 2; y = sy + 2; s = Scale - 0.8; break;
			case GET_ITEM(12, 156): x = sx + 0; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(12, 157): x = sx + 0; y = sy + 6; s = Scale - 0.4; break;
			case GET_ITEM(12, 158): x = sx + 0; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(12, 159): x = sx + 0; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(12, 160): x = sx + 0; y = sy + 0; s = Scale - 0.4; break;
			case GET_ITEM(12, 161): x = sx + 0; y = sy + 0; s = Scale - 0.4; break;
			case GET_ITEM(12, 162): x = sx + 0; y = sy + 0; s = Scale - 0.4; break;
			case GET_ITEM(12, 210): x = sx + 0; y = sy + 2; s = Scale - 0.2; break;
			case GET_ITEM(12, 211): x = sx + 0; y = sy + 10; s = Scale - 0.4; break;
			case GET_ITEM(12, 212): x = sx + 0; y = sy + 10; s = Scale - 0.2; break;
			case GET_ITEM(12, 213): x = sx + 0; y = sy + 12; s = Scale - 0.2; break;
			case GET_ITEM(12, 214): x = sx + 0; y = sy + 8; s = Scale - 0.1; break;
			case GET_ITEM(12, 215): x = sx + 0; y = sy + 8; s = Scale - 0.1; break;
			case GET_ITEM(12, 216): x = sx + 0; y = sy + 10; s = Scale - 0.1; break;
			case GET_ITEM(12, 217): x = sx + 0; y = sy + 6; s = Scale - 0.4; break;
			case GET_ITEM(12, 218): x = sx + 0; y = sy + 8; s = Scale - 0.4; break;
			case GET_ITEM(12, 219): x = sx + 0; y = sy + 8; s = Scale - 0.4; break;
			case GET_ITEM(12, 220): x = sx + 0; y = sy + 14; s = Scale - 0.1; break;
			case GET_ITEM(12, 221): x = sx + 0; y = sy + 8; s = Scale - 0.2; break;
			case GET_ITEM(12, 222): x = sx + 0; y = sy + 4; s = Scale - 0.4; break;
			case GET_ITEM(12, 223): x = sx + 0; y = sy + 6; s = Scale - 0.4; break;
			case GET_ITEM(12, 224): x = sx + 0; y = sy + 12; s = Scale - 0.1; break;
			case GET_ITEM(12, 225): x = sx + 0; y = sy + 6; s = Scale - 0.3; break;
			case GET_ITEM(12, 226): x = sx + 0; y = sy + 6; s = Scale - 0.6; break;
			case GET_ITEM(12, 227): x = sx + 0; y = sy + 0; s = Scale - 0.2; break;
			case GET_ITEM(12, 228): x = sx + 0; y = sy + 6; s = Scale - 0.2; break;
			case GET_ITEM(12, 229): x = sx + 0; y = sy + 12; s = Scale - 0.6; break;
			case GET_ITEM(12, 230): x = sx + 0; y = sy + 6; s = Scale - 0.6; break;
			case GET_ITEM(12, 239): x = sx + 0; y = sy + 6; s = Scale - 0.6; break;
			case GET_ITEM(12, 240): x = sx + 0; y = sy + 6; s = Scale - 0.6; break;
			case GET_ITEM(13, 1): x = sx + 2; y = sy + 0; s = Scale - 0.9; break;
			case GET_ITEM(13, 2): x = sx + 2; y = sy + 4; s = Scale - 0.5; break;
			case GET_ITEM(13, 3): x = sx + 2; y = sy + 2; s = Scale - 0.5; break;
			case GET_ITEM(13, 4): x = sx + 0; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 5): x = sx + 1; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 7): x = sx + 1; y = sy - 7; s = Scale - 0.9; break;
			case GET_ITEM(13, 8): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(13, 9): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(13, 10): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(13, 11): x = sx + 2; y = sy - 2; s = Scale - 0.9; break;
			case GET_ITEM(13, 12): x = sx + 2; y = sy - 4; s = Scale - 0.9; break;
			case GET_ITEM(13, 13): x = sx + 2; y = sy - 4; s = Scale - 0.9; break;
			case GET_ITEM(13, 14): x = sx + 2; y = sy - 4; s = Scale - 0.5; break;
			case GET_ITEM(13, 15): x = sx + 4; y = sy - 12; s = Scale - 0.5; break;
			case GET_ITEM(13, 16): x = sx + 2; y = sy - 5; s = Scale - 0.5; break;
			case GET_ITEM(13, 17): x = sx + 2; y = sy - 6; s = Scale - 0.5; break;
			case GET_ITEM(13, 18): x = sx + 1; y = sy - 0; s = Scale - 0.5; break;
			case GET_ITEM(13, 19): x = sx + 3; y = sy + 8; s = Scale - 0.5; break;
			case GET_ITEM(13, 20): x = sx + 2; y = sy + 0; s = Scale - 0.9; break;
			case GET_ITEM(13, 21): x = sx + 2; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 22): x = sx + 2; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 23): x = sx + 2; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 24): x = sx + 2; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 25): x = sx + 2; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 26): x = sx + 2; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 27): x = sx + 2; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 28): x = sx + 2; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 29): x = sx + 2; y = sy + 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 30): x = sx + 1; y = sy + 8; s = Scale - 0.7; break;
			case GET_ITEM(13, 31): x = sx + 2; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 32): x = sx + 0; y = sy + 5; s = Scale - 0.7; break;
			case GET_ITEM(13, 33): x = sx + 2; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 34): x = sx + 0; y = sy + 0; s = Scale - 0.9; break;
			case GET_ITEM(13, 35): x = sx + 0; y = sy + 0; s = Scale - 0.9; break;
			case GET_ITEM(13, 36): x = sx + 0; y = sy + 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 37): x = sx + 0; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 38): x = sx + 2; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 39): x = sx + 2; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 40): x = sx + 2; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 41): x = sx + 2; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 42): x = sx + 2; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 43): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 44): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 45): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 46): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 47): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 48): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 49): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 50): x = sx + 2; y = sy + 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 51): x = sx + 1; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 52): x = sx + 2; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 53): x = sx + 2; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 54): x = sx + 2; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 55): x = sx + 2; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 56): x = sx + 2; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 57): x = sx + 2; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 58): x = sx + 2; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 59): x = sx + 1; y = sy - 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 60): x = sx + 2; y = sy + 2; s = Scale - 0.4; break;
			case GET_ITEM(13, 61): x = sx + 2; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 62): x = sx + 0; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 63): x = sx + 0; y = sy - 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 64): x = sx + 2; y = sy - 1; s = Scale - 0.7; break;
			case GET_ITEM(13, 65): x = sx + 2; y = sy - 1; s = Scale - 0.7; break;
			case GET_ITEM(13, 66): x = sx + 1; y = sy - 1; s = Scale - 0.7; break;
			case GET_ITEM(13, 67): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 68): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 69): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 70): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 71): x = sx + 1; y = sy + 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 72): x = sx + 1; y = sy + 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 73): x = sx + 1; y = sy + 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 74): x = sx + 1; y = sy + 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 75): x = sx + 1; y = sy + 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 76): x = sx + 2; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 80): x = sx + 0; y = sy - 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 81): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 82): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 83): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 84): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 85): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 86): x = sx + 2; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 87): x = sx + 2; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 88): x = sx + 2; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 89): x = sx + 2; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 90): x = sx + 2; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 91): x = sx + 2; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 92): x = sx + 2; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 93): x = sx + 1; y = sy + 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 94): x = sx + 1; y = sy + 3; s = Scale - 0.7; break;
			case GET_ITEM(13, 97): x = sx + 1; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 98): x = sx + 1; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 103): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 104): x = sx + 0; y = sy + 1; s = Scale - 0.7; break;
			case GET_ITEM(13, 105): x = sx + 0; y = sy + 1; s = Scale - 0.7; break;
			case GET_ITEM(13, 106): x = sx + 1; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 107): x = sx + 1; y = sy - 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 109): x = sx + 1; y = sy - 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 110): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 111): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 112): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 113): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 114): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 115): x = sx + 1; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 116): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 121): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 122): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 123): x = sx + 1; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 124): x = sx + 1; y = sy + 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 125): x = sx + 1; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 126): x = sx + 1; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 127): x = sx + 1; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 200): x = sx + 0; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 201): x = sx + 0; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 202): x = sx + 0; y = sy + 0; s = Scale - 0.6; break;
			case GET_ITEM(13, 203): x = sx + 4; y = sy - 14; s = Scale - 0.6; break;
			case GET_ITEM(13, 204): x = sx + 2; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(13, 205): x = sx + 2; y = sy - 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 206): x = sx + 2; y = sy - 16; s = Scale - 0.7; break;
			case GET_ITEM(13, 207): x = sx + 4; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(13, 208): x = sx + 0; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(13, 209): x = sx + 0; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(13, 210): x = sx + 3; y = sy - 10; s = Scale - 0.7; break;
			case GET_ITEM(13, 211): x = sx + 3; y = sy - 10; s = Scale - 0.7; break;
			case GET_ITEM(13, 212): x = sx + 3; y = sy - 7; s = Scale - 0.3; break;
			case GET_ITEM(13, 213): x = sx + 3; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 214): x = sx + 6; y = sy - 6; s = Scale - 0.7; break;
			case GET_ITEM(13, 215): x = sx + 0; y = sy - 4; s = Scale - 0.7; break;
			case GET_ITEM(13, 216): x = sx + 4; y = sy + 6; s = Scale - 0.7; break;
			case GET_ITEM(13, 217): x = sx + 0; y = sy - 10; s = Scale - 0.7; break;
			case GET_ITEM(13, 218): x = sx + 0; y = sy - 10; s = Scale - 0.7; break;
			case GET_ITEM(13, 219): x = sx + 0; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(13, 220): x = sx + 0; y = sy - 18; s = Scale - 0.7; break;
			case GET_ITEM(13, 221): x = sx + 6; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 222): x = sx + 6; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 223): x = sx + 6; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 224): x = sx + 6; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(13, 226): x = sx + 6; y = sy - 6; s = Scale - 0.7; break;
			case GET_ITEM(13, 227): x = sx + 6; y = sy - 6; s = Scale - 0.7; break;
			case GET_ITEM(14, 0): x = sx + 2; y = sy + 1; s = Scale - 0.9; break;
			case GET_ITEM(14, 1): x = sx - 8; y = sy - 14; s = Scale - 0.9; break;
			case GET_ITEM(14, 2): x = sx - 8; y = sy - 14; s = Scale - 0.9; break;
			case GET_ITEM(14, 3): x = sx - 8; y = sy - 12; s = Scale - 0.9; break;
			case GET_ITEM(14, 4): x = sx - 8; y = sy - 14; s = Scale - 0.9; break;
			case GET_ITEM(14, 5): x = sx - 8; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 6): x = sx - 8; y = sy - 12; s = Scale - 0.9; break;
			case GET_ITEM(14, 7): x = sx + 2; y = sy + 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 8): x = sx - 8; y = sy - 14; s = Scale - 0.9; break;
			case GET_ITEM(14, 9): x = sx - 8; y = sy - 0; s = Scale - 0.9; break;
			case GET_ITEM(14, 10): x = sx - 8; y = sy - 5; s = Scale - 0.8; break;
			case GET_ITEM(14, 11): x = sx + 1; y = sy - 9; s = Scale - 0.8; break;
			case GET_ITEM(14, 12): x = sx + 2; y = sy + 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 13): x = sx + 1; y = sy - 10; s = Scale - 0.9; break;
			case GET_ITEM(14, 14): x = sx + 1; y = sy - 10; s = Scale - 0.9; break;
			case GET_ITEM(14, 15): x = sx - 8; y = sy - 16; s = Scale - 0.9; break;
			case GET_ITEM(14, 16): x = sx + 2; y = sy - 12; s = Scale - 0.9; break;
			case GET_ITEM(14, 17): x = sx + 2; y = sy + 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 18): x = sx + 2; y = sy + 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 19): x = sx + 2; y = sy + 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 20): x = sx - 8; y = sy - 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 21): x = sx + 1; y = sy + 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 22): x = sx + 1; y = sy - 11; s = Scale - 0.9; break;
			case GET_ITEM(14, 23): x = sx + 1; y = sy + 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 24): x = sx + 3; y = sy + 16; s = Scale - 0.9; break;
			case GET_ITEM(14, 25): x = sx + 3; y = sy - 4; s = Scale - 0.9; break;
			case GET_ITEM(14, 26): x = sx + 1; y = sy - 4; s = Scale - 0.9; break;
			case GET_ITEM(14, 27): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 28): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 29): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 31): x = sx + 2; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(14, 32): x = sx - 7; y = sy - 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 33): x = sx - 7; y = sy - 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 34): x = sx - 7; y = sy - 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 35): x = sx - 7; y = sy - 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 36): x = sx - 8; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 37): x = sx - 8; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 38): x = sx - 8; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 39): x = sx - 8; y = sy - 0; s = Scale - 0.9; break;
			case GET_ITEM(14, 40): x = sx - 8; y = sy - 0; s = Scale - 0.9; break;
			case GET_ITEM(14, 41): x = sx - 0; y = sy - 10; s = Scale - 0.9; break;
			case GET_ITEM(14, 42): x = sx + 2; y = sy - 10; s = Scale - 0.9; break;
			case GET_ITEM(14, 43): x = sx + 1; y = sy - 10; s = Scale - 0.9; break;
			case GET_ITEM(14, 44): x = sx + 1; y = sy - 10; s = Scale - 0.9; break;
			case GET_ITEM(14, 45): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 46): x = sx + 2; y = sy + 6; s = Scale - 0.5; break;
			case GET_ITEM(14, 47): x = sx + 2; y = sy + 6; s = Scale - 0.5; break;
			case GET_ITEM(14, 48): x = sx + 2; y = sy + 6; s = Scale - 0.5; break;
			case GET_ITEM(14, 49): x = sx - 9; y = sy - 12; s = Scale - 0.5; break;
			case GET_ITEM(14, 50): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 51): x = sx - 8; y = sy - 4; s = Scale - 0.9; break;
			case GET_ITEM(14, 52): x = sx + 2; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 53): x = sx - 8; y = sy - 9; s = Scale - 0.9; break;
			case GET_ITEM(14, 54): x = sx - 8; y = sy - 4; s = Scale - 0.9; break;
			case GET_ITEM(14, 55): x = sx - 8; y = sy - 9; s = Scale - 0.9; break;
			case GET_ITEM(14, 56): x = sx - 8; y = sy - 9; s = Scale - 0.9; break;
			case GET_ITEM(14, 57): x = sx - 8; y = sy - 9; s = Scale - 0.9; break;
			case GET_ITEM(14, 58): x = sx - 8; y = sy - 4; s = Scale - 0.9; break;
			case GET_ITEM(14, 59): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 60): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 61): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 62): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 63): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 64): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 65): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 66): x = sx - 11; y = sy + 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 67): x = sx - 7; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 68): x = sx - 8; y = sy + 1; s = Scale - 0.9; break;
			case GET_ITEM(14, 70): x = sx - 10; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 71): x = sx - 10; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 72): x = sx + 1; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 73): x = sx + 1; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 74): x = sx + 1; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 75): x = sx + 1; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 76): x = sx + 1; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 77): x = sx + 1; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 78): x = sx - 9; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(14, 79): x = sx - 9; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(14, 80): x = sx - 9; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(14, 81): x = sx - 9; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(14, 82): x = sx - 9; y = sy + 0; s = Scale - 0.7; break;
			case GET_ITEM(14, 83): x = sx - 8; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(14, 84): x = sx - 8; y = sy - 10; s = Scale - 0.7; break;
			case GET_ITEM(14, 85): x = sx - 8; y = sy - 2; s = Scale - 0.7; break;
			case GET_ITEM(14, 86): x = sx - 8; y = sy - 10; s = Scale - 0.7; break;
			case GET_ITEM(14, 87): x = sx - 9; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(14, 88): x = sx - 9; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(14, 89): x = sx - 9; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(14, 90): x = sx - 9; y = sy - 0; s = Scale - 0.7; break;
			case GET_ITEM(14, 91): x = sx - 9; y = sy - 10; s = Scale - 0.8; break;
			case GET_ITEM(14, 92): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 93): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 94): x = sx - 10; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 95): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 96): x = sx + 1; y = sy - 12; s = Scale - 0.9; break;
			case GET_ITEM(14, 97): x = sx + 1; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 98): x = sx + 1; y = sy - 13; s = Scale - 0.9; break;
			case GET_ITEM(14, 99): x = sx - 8; y = sy - 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 100): x = sx + 2; y = sy + 11; s = Scale - 0.9; break;
			case GET_ITEM(14, 101): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 102): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 103): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 104): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 105): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 106): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 107): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 108): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 109): x = sx - 9; y = sy - 7; s = Scale - 0.9; break;
			case GET_ITEM(14, 110): x = sx - 9; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 111): x = sx - 9; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 112): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 113): x = sx - 9; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 114): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 115): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 116): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 117): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 118): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 119): x = sx - 9; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 120): x = sx - 10; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 121): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 122): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 123): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 124): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 126): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 127): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 128): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 129): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 130): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 131): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 132): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 133): x = sx - 10; y = sy - 4; s = Scale - 0.9; break;
			case GET_ITEM(14, 134): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 135): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 136): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 137): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 138): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 139): x = sx - 8; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 140): x = sx - 8; y = sy - 5; s = Scale - 0.6; break;
			case GET_ITEM(14, 141): x = sx - 8; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(14, 142): x = sx - 8; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(14, 143): x = sx - 8; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(14, 144): x = sx - 8; y = sy - 8; s = Scale - 0.7; break;
			case GET_ITEM(14, 146): x = sx - 10; y = sy - 9; s = Scale - 0.7; break;
			case GET_ITEM(14, 147): x = sx - 10; y = sy - 9; s = Scale - 0.7; break;
			case GET_ITEM(14, 148): x = sx - 10; y = sy - 9; s = Scale - 0.7; break;
			case GET_ITEM(14, 149): x = sx - 10; y = sy - 9; s = Scale - 0.7; break;
			case GET_ITEM(14, 150): x = sx - 10; y = sy - 9; s = Scale - 0.7; break;
			case GET_ITEM(14, 157): x = sx - 8; y = sy - 6; s = Scale - 0.1; break;
			case GET_ITEM(14, 158): x = sx - 8; y = sy - 6; s = Scale - 0.1; break;
			case GET_ITEM(14, 159): x = sx - 8; y = sy - 6; s = Scale - 0.1; break;
			case GET_ITEM(14, 162): x = sx - 10; y = sy - 6; s = Scale - 0.9; break;
			case GET_ITEM(14, 163): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(14, 173): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(14, 174): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 175): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 176): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 177): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 178): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 179): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 190): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 191): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 192): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 193): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 194): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 195): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 196): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 197): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 198): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 199): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 200): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 201): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 202): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 203): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 204): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 205): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 206): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 207): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 208): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 209): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 210): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 211): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 212): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 213): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 214): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 215): x = sx - 8; y = sy - 17; s = Scale - 0.9; break;
			case GET_ITEM(14, 220): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 221): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 222): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 223): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 224): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 225): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 226): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 227): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 228): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 229): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 230): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 231): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 232): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 233): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 234): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 235): x = sx - 8; y = sy - 8; s = Scale - 0.9; break;
			case GET_ITEM(14, 244): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 245): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 246): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 247): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 248): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 249): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 250): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 251): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(14, 252): x = sx - 4; y = sy - 5; s = Scale - 0.9; break;
			case GET_ITEM(15, 0): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 1): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 2): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 3): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 4): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 5): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 6): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 7): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 8): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 9): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 10): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 11): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 12): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 13): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 14): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 15): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 16): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 17): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 18): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 19): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 20): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 21): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 22): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 23): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 24): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 25): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 26): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 27): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 28): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 29): x = sx - 8; y = sy + 2; s = Scale - 0.9; break;
			case GET_ITEM(15, 30): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 31): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 32): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 33): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 35): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;
			case GET_ITEM(15, 36): x = sx - 10; y = sy + 8; s = Scale - 0.9; break;

			default:
			break;
		}
		
		if (Type >= GET_ITEM(7, 0) && Type <= GET_ITEM(7, 190))
		{
			x = sx - 5;	y = sy + 4;	s = Scale - 1.0;
		}
		else if (Type >= GET_ITEM(8, 100) && Type <= GET_ITEM(8, 190))
		{
			x = sx - 5;	y = sy + 24; s = Scale - 0.9;
		}
		else if (Type >= GET_ITEM(10, 0) && Type <= GET_ITEM(10, 190))
		{
			x = sx - 5;	y = sy + 10; s = Scale - 0.9;
		}
	}

	EnableAlphaTest();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);

	float SizeItem = s;

	if (m_Resolution >= 1) 
	{ 
		SizeItem += 0.70f; 
	}

	gluPerspective2(SizeItem, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();

	glClear(GL_DEPTH_BUFFER_BIT);

	float BWidth = (float)ItemAttribute[Type].Width * INVENTORY_SCALE;
	float BHeight = (float)ItemAttribute[Type].Height * INVENTORY_SCALE;
	RenderItem3D(x, y, Width, Height, Type, Level, Option1, ExtOption, PickUp);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glColor3f(1, 1, 1);
	EnableAlphaTest(false);
}
CNewUIManager* CNewUISystem::GetNewUIManager() const
{
	return m_pNewUIMng;
}

CNewUI3DRenderMng* CNewUISystem::GetNewUI3DRenderMng() const
{
	return m_pNewUI3DRenderMng;
}


CNewUIHotKey* CNewUISystem::GetNewUIHotKey() const
{
	return m_pNewUIHotKey;
}

bool CNewUISystem::IsImpossibleSendMoveInterface()
{
	if (IsVisible(INTERFACE_MIXINVENTORY)
		|| IsVisible(INTERFACE_KANTURU2ND_ENTERNPC)
#ifdef LEM_ADD_LUCKYITEM
		|| IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND)
#endif // LEM_ADD_LUCKYITEM
		)
	{
		return true;
	}

	return false;
}

bool CNewUISystem::IsImpossibleTradeInterface()
{
	if (IsVisible(INTERFACE_MIXINVENTORY)
		|| IsVisible(INTERFACE_KANTURU2ND_ENTERNPC)
		|| IsVisible(INTERFACE_STORAGE)
		|| IsVisible(INTERFACE_INGAMESHOP)
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
		|| IsVisible(INTERFACE_INGAMESHOP)
#endif //PBG_ADD_INGAMESHOP_UI_MAINFRAME
#ifdef LEM_ADD_LUCKYITEM
		|| IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND)
#endif // LEM_ADD_LUCKYITEM
		)
	{
		return true;
	}

	return false;
}

bool CNewUISystem::IsImpossibleDuelInterface()
{
	if (IsVisible(INTERFACE_MIXINVENTORY)
		|| IsVisible(INTERFACE_KANTURU2ND_ENTERNPC)
		|| IsVisible(INTERFACE_STORAGE)
		|| IsVisible(INTERFACE_INGAMESHOP)
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
		|| IsVisible(INTERFACE_INGAMESHOP)
#endif //PBG_ADD_INGAMESHOP_UI_MAINFRAME
#ifdef LEM_ADD_LUCKYITEM
		|| IsVisible(SEASON3B::INTERFACE_LUCKYITEMWND)
#endif // LEM_ADD_LUCKYITEM
		)
	{
		return true;
	}

	return false;
}

bool CNewUISystem::IsImpossibleHideInterface(DWORD dwKey)
{
	if (dwKey == INTERFACE_MAINFRAME
		|| dwKey == INTERFACE_SKILL_LIST
		|| dwKey == INTERFACE_SLIDEWINDOW
		|| dwKey == INTERFACE_MESSAGEBOX
		|| dwKey == INTERFACE_CHATLOGWINDOW
		|| dwKey == INTERFACE_PARTY_INFO_WINDOW
		|| dwKey == INTERFACE_KANTURU_INFO
		|| dwKey == INTERFACE_BLOODCASTLE_TIME
		|| dwKey == INTERFACE_CHAOSCASTLE_TIME
		|| dwKey == INTERFACE_BATTLE_SOCCER_SCORE
		|| dwKey == INTERFACE_DUEL_WINDOW
		|| dwKey == INTERFACE_CRYWOLF
		|| dwKey == INTERFACE_HERO_POSITION_INFO
		|| dwKey == INTERFACE_NAME_WINDOW
		|| dwKey == INTERFACE_SIEGEWARFARE
		|| dwKey == INTERFACE_ITEM_TOOLTIP
		|| dwKey == INTERFACE_HOTKEY
		|| dwKey == INTERFACE_CURSEDTEMPLE_GAMESYSTEM
		|| dwKey == INTERFACE_ITEM_ENDURANCE_INFO
		|| dwKey == INTERFACE_BUFF_WINDOW
		|| (dwKey >= INTERFACE_3DRENDERING_CAMERA_BEGIN && dwKey <= INTERFACE_3DRENDERING_CAMERA_END)
		|| dwKey == INTERFACE_DUELWATCH_MAINFRAME
		|| dwKey == INTERFACE_DUELWATCH_USERLIST
		|| dwKey == INTERFACE_DOPPELGANGER_FRAME
		|| dwKey == INTERFACE_GOLD_BOWMAN
		|| dwKey == INTERFACE_GOLD_BOWMAN_LENA
		|| dwKey == INTERFACE_EMPIREGUARDIAN_TIMER
		)
	{
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------------------------

void CNewUISystem::UpdateSendMoveInterface()
{
	if (IsVisible(INTERFACE_TRADE))
	{
		SendRequestTradeExit();
		Hide(INTERFACE_TRADE);
	}
	if (IsVisible(INTERFACE_STORAGE_EXT))
	{
		Hide(INTERFACE_STORAGE_EXT);
	}
	if (IsVisible(INTERFACE_STORAGE))
	{
		Hide(INTERFACE_STORAGE);
	}
	if (IsVisible(INTERFACE_NPCGUILDMASTER))
	{
		Hide(INTERFACE_NPCGUILDMASTER);
	}
	if (IsVisible(INTERFACE_MYQUEST))
	{
		Hide(INTERFACE_MYQUEST);
	}
	if (IsVisible(INTERFACE_NPCQUEST))
	{
		Hide(INTERFACE_NPCQUEST);
	}
	if (IsVisible(INTERFACE_NPCSHOP))
	{
		Hide(INTERFACE_NPCSHOP);
	}
	if (IsVisible(INTERFACE_GUARDSMAN))
	{
		Hide(INTERFACE_GUARDSMAN);
	}
	if (IsVisible(INTERFACE_GUARDSMAN))
	{
		Hide(INTERFACE_GUARDSMAN);
	}
	if (IsVisible(INTERFACE_DEVILSQUARE))
	{
		Hide(INTERFACE_DEVILSQUARE);
	}
	if (IsVisible(INTERFACE_BLOODCASTLE))
	{
		Hide(INTERFACE_BLOODCASTLE);
	}
	if (IsVisible(INTERFACE_CURSEDTEMPLE_NPC))
	{
		Hide(INTERFACE_CURSEDTEMPLE_NPC);
	}
	if (IsVisible(INTERFACE_MYSHOP_INVENTORY))
	{
		Hide(INTERFACE_MYSHOP_INVENTORY);
	}
	if (IsVisible(INTERFACE_PURCHASESHOP_INVENTORY))
	{
		Hide(INTERFACE_PURCHASESHOP_INVENTORY);
	}
	if (IsVisible(INTERFACE_DUELWATCH))
	{
		Hide(INTERFACE_DUELWATCH);
	}
	if (IsVisible(INTERFACE_DOPPELGANGER_NPC))
	{
		Hide(INTERFACE_DOPPELGANGER_NPC);
	}
	if (IsVisible(INTERFACE_NPC_DIALOGUE))
	{
		Hide(INTERFACE_NPC_DIALOGUE);
	}
	if (IsVisible(INTERFACE_QUEST_PROGRESS))
	{
		Hide(INTERFACE_QUEST_PROGRESS);
	}
	if (IsVisible(INTERFACE_QUEST_PROGRESS_ETC))
	{
		Hide(INTERFACE_QUEST_PROGRESS_ETC);
	}
	if (IsVisible(INTERFACE_EMPIREGUARDIAN_NPC))
	{
		Hide(INTERFACE_EMPIREGUARDIAN_NPC);
	}
	if (IsVisible(INTERFACE_LUCKYCOIN_REGISTRATION))
	{
		Hide(INTERFACE_LUCKYCOIN_REGISTRATION);
	}
	if (IsVisible(INTERFACE_EXCHANGE_LUCKYCOIN))
	{
		Hide(INTERFACE_EXCHANGE_LUCKYCOIN);
	}
	if (IsVisible(INTERFACE_UNITEDMARKETPLACE_NPC_JULIA))
	{
		Hide(INTERFACE_UNITEDMARKETPLACE_NPC_JULIA);
	}
}

// GetInstance()
CNewUISystem* CNewUISystem::GetInstance()
{
	static CNewUISystem s_NewUISystem;
	return &s_NewUISystem;
}

CUIFormSystemRender* CNewUISystem::Get_pUIFormSystemRender() const
{
	return m_pUIFormSystemRender;
}

CNewUIChatLogWindow* CNewUISystem::GetUI_NewChatLogWindow() const
{
	return m_pNewChatLogWindow;
}

CNewUISlideWindow* CNewUISystem::GetUI_NewSlideWindow() const
{
	return m_pNewSlideWindow;
}

CNewUIFriendWindow* CNewUISystem::GetUI_NewFriendWindow() const
{
	return m_pNewFriendWindow;
}

CNewUIMainFrameWindow* CNewUISystem::GetUI_NewMainFrameWindow() const
{
	return m_pNewMainFrameWindow;
}

CNewUISkillList* CNewUISystem::GetUI_NewSkillList() const
{
	return m_pNewSkillList;
}

CNewUIChatInputBox* CNewUISystem::GetUI_NewChatInputBox() const
{
	return m_pNewChatInputBox;
}

CNewUIItemMng* CNewUISystem::GetUI_NewItemMng() const
{
	return m_pNewItemMng;
}

CNewUIMyInventory* CNewUISystem::GetUI_NewMyInventory() const
{
	return m_pNewMyInventory;
}

CNewUIInventoryExtension* CNewUISystem::GetUI_NewMyInventoryExt() const
{
	return m_pNewMyInventoryExt;
}

CNewUINPCShop* CNewUISystem::GetUI_NewNpcShop() const
{
	return m_pNewNPCShop;
}

CNewUIPetInfoWindow* CNewUISystem::GetUI_NewPetInfoWindow() const
{
	return m_pNewPetInfoWindow;
}

CNewUIMixInventory* CNewUISystem::GetUI_NewMixInventory() const
{
	return m_pNewMixInventory;
}

CNewUICastleWindow* CNewUISystem::GetUI_NewCastleWindow() const
{
	return m_pNewCastleWindow;
}

CNewUIGuardWindow* CNewUISystem::GetUI_NewGuardWindow() const
{
	return m_pNewGuardWindow;
}

CNewUIGatemanWindow* CNewUISystem::GetUI_NewGatemanWindow() const
{
	return m_pNewGatemanWindow;
}

CNewUIGateSwitchWindow* CNewUISystem::GetUI_NewGateSwitchWindow() const
{
	return m_pNewGateSwitchWindow;
}

CNewUIStorageInventory* CNewUISystem::GetUI_NewStorageInventory() const
{
	return m_pNewStorageInventory;
}

CNewUIStorageInventoryExt* CNewUISystem::GetUI_NewStorageInventoryExt() const
{
	return m_pNewStorageInventoryExt;
}

CNewUIGuildMakeWindow* CNewUISystem::GetUI_NewGuildMakeWindow() const
{
	return m_pNewGuildMakeWindow;
}

CNewUIGuildInfoWindow* CNewUISystem::GetUI_NewGuildInfoWindow() const
{
	return m_pNewGuildInfoWindow;
}

CNewUICryWolf* CNewUISystem::GetUI_NewCryWolfInterface() const
{
	return m_pNewCryWolfInterface;
}
#if(NEW_MASTER_SKILL_TREE)
CNewUIMasterSkillTree* SEASON3B::CNewUISystem::GetUI_NewMasterSkillTreeInterface() const
{
	return m_pNewMaster_SkillTree_Interface;
}
#endif
CNewUIMyShopInventory* CNewUISystem::GetUI_NewMyShopInventory() const
{
	return m_pNewMyShopInventory;
}

CNewUIPurchaseShopInventory* CNewUISystem::GetUI_NewPurchaseShopInventory() const
{
	return m_pNewPurchaseShopInventory;
}

CNewUICharacterInfoWindow* CNewUISystem::GetUI_NewCharacterInfoWindow() const
{
	return m_pNewCharacterInfoWindow;
}

CNewUIMyQuestInfoWindow* CNewUISystem::GetUI_NewMyQuestInfoWindow() const
{
	return m_pNewMyQuestInfoWindow;
}

CNewUIPartyInfoWindow* CNewUISystem::GetUI_NewPartyInfoWindow() const
{
	return m_pNewPartyInfoWindow;
}

CNewUIPartyListWindow* CNewUISystem::GetUI_NewPartyListWindow() const
{
	return m_pNewPartyListWindow;
}

CNewUINPCQuest* CNewUISystem::GetUI_NewNPCQuest() const
{
	return m_pNewNPCQuest;
}

CNewUIEnterBloodCastle* CNewUISystem::GetUI_NewEnterBloodCastle() const
{
	return m_pNewEnterBloodCastle;
}

CNewUIEnterDevilSquare* CNewUISystem::GetUI_NewEnterDevilSquare() const
{
	return m_pNewEnterDevilSquare;
}

CNewUIBloodCastle* CNewUISystem::GetUI_NewBloodCastle() const
{
	return m_pNewBloodCastle;
}

CNewUITrade* CNewUISystem::GetUI_NewTrade() const
{
	return m_pNewTrade;
}

CNewUIKanturu2ndEnterNpc* CNewUISystem::GetUI_NewKanturu2ndEnterNpc() const
{
	return m_pNewKanturu2ndEnterNpc;
}

CNewUIKanturuInfoWindow* CNewUISystem::GetUI_NewKanturuInfoWindow() const
{
	return m_pNewKanturuInfoWindow;
}

CNewUICatapultWindow* CNewUISystem::GetUI_NewCatapultWindow() const
{
	return m_pNewCatapultWindow;
}

CNewUIChaosCastleTime* CNewUISystem::GetUI_NewChaosCastleTime() const
{
	return m_pNewChaosCastleTime;
}

CNewUICommandWindow* CNewUISystem::GetUI_NewCommandWindow() const
{
	return m_pNewCommandWindow;
}

CNewUIWindowMenu* CNewUISystem::GetUI_NewWindowMenu() const
{
	return m_pNewWindowMenu;
}

CNewUIOptionWindow* CNewUISystem::GetUI_NewOptionWindow() const
{
	return m_pNewOptionWindow;
}

CNewUIMoveCommandWindow* CNewUISystem::GetUI_NewMoveCommandWindow() const
{
	return m_pNewMoveCommandWindow;
}

CNewUIHeroPositionInfo* CNewUISystem::GetUI_NewHeroPositionInfo() const
{
	return m_pNewHeroPositionInfo;
}

CNewUIHelpWindow* CNewUISystem::GetUI_NewHelpWindow() const
{
	return m_pNewHelpWindow;
}

CNewUIItemExplanationWindow* CNewUISystem::GetUI_NewItemExplanationWindow() const
{
	return m_pNewItemExplanationWindow;
}

CNewUISetItemExplanation* CNewUISystem::GetUI_NewSetItemExplanation() const
{
	return m_pNewSetItemExplanation;
}

CNewUIQuickCommandWindow* CNewUISystem::GetUI_NewQuickCommandWindow() const
{
	return m_pNewQuickCommandWindow;
}

CNewUIBattleSoccerScore* CNewUISystem::GetUI_NewBattleSoccerScore() const
{
	return m_pNewBattleSoccerScore;
}

CNewUIDuelWindow* CNewUISystem::GetUI_NewDuelWindow() const
{
	return m_pNewDuelWindow;
}

CNewUISiegeWarfare* CNewUISystem::GetUI_NewSiegeWarfare() const
{
	return m_pNewSiegeWarfare;
}

CNewUIItemEnduranceInfo* CNewUISystem::GetUI_NewItemEnduranceInfo() const
{
	return m_pNewItemEnduranceInfo;
}

CNewUIBuffWindow* CNewUISystem::GetUI_NewBuffWindow() const
{
	return m_pNewBuffWindow;
}

CNewUICursedTempleEnter* CNewUISystem::GetUI_NewCursedTempleEnterWindow() const
{
	return m_pNewCursedTempleEnterWindow;
}

CNewUICursedTempleSystem* CNewUISystem::GetUI_NewCursedTempleWindow() const
{
	return m_pNewCursedTempleWindow;
}

CNewUICursedTempleResult* CNewUISystem::GetUI_NewCursedTempleResultWindow() const
{
	return m_pNewCursedTempleResultWindow;
}

CNewUIGoldBowmanWindow* CNewUISystem::GetUI_pNewGoldBowman() const
{
	return m_pNewGoldBowman;
}

CNewUIGoldBowmanLena* CNewUISystem::GetUI_pNewGoldBowmanLena() const
{
	return m_pNewGoldBowmanLena;
}

CNewUIRegistrationLuckyCoin* CNewUISystem::GetUI_pNewLuckyCoinRegistration() const
{
	return m_pNewLuckyCoinRegistration;
}

CNewUIExchangeLuckyCoin* CNewUISystem::GetUI_pNewExchangeLuckyCoin() const
{
	return m_pNewExchangeLuckyCoinWindow;
}

CNewUIMiniMap* CNewUISystem::GetUI_pNewUIMiniMap() const
{
	return m_pNewMiniMap;
}

CNewUIDuelWatchWindow* CNewUISystem::GetUI_pNewDuelWatch() const
{
	return m_pNewDuelWatchWindow;
}

CNewUIDuelWatchMainFrameWindow* CNewUISystem::GetUI_pNewDuelWatchMainFrame() const
{
	return m_pNewDuelWatchMainFrameWindow;
}

CNewUIDuelWatchUserListWindow* CNewUISystem::GetUI_pNewDuelWatchUserList() const
{
	return m_pNewDuelWatchUserListWindow;
}

#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
CNewUIInGameShop* CNewUISystem::GetUI_pNewInGameShop() const
{
	return m_pNewInGameShop;
}
#endif //PBG_ADD_INGAMESHOP_UI_MAINFRAME

CNewUIDoppelGangerWindow* CNewUISystem::GetUI_pNewDoppelGangerWindow() const
{
	return m_pNewDoppelGangerWindow;
}

CNewUIDoppelGangerFrame* CNewUISystem::GetUI_pNewDoppelGangerFrame() const
{
	return m_pNewDoppelGangerFrame;
}

CNewUINPCDialogue* CNewUISystem::GetUI_NewNPCDialogue() const
{
	return m_pNewNPCDialogue;
}

CNewUIQuestProgress* CNewUISystem::GetUI_NewQuestProgress() const
{
	return m_pNewQuestProgress;
}

CNewUIQuestProgressByEtc* CNewUISystem::GetUI_NewQuestProgressByEtc() const
{
	return m_pNewQuestProgressByEtc;
}

CNewUIEmpireGuardianNPC* CNewUISystem::GetUI_pNewEmpireGuardianNPC() const
{
	return m_pNewEmpireGuardianNPC;
}

CNewUIEmpireGuardianTimer* CNewUISystem::GetUI_pNewEmpireGuardianTimer() const
{
	return m_pNewEmpireGuardianTimer;
}

#ifdef PBG_MOD_STAMINA_UI
CNewUIStamina* SEASON3B::CNewUISystem::GetUI_pNewUIStamina() const
{
	return m_pNewUIStamina;
}
#endif //PBG_MOD_STAMINA_UI

#ifdef PBG_ADD_GENSRANKING
CNewUIGensRanking* CNewUISystem::GetUI_NewGensRanking() const
{
	return m_pNewGensRanking;
}
#endif //PBG_ADD_GENSRANKING

CNewUIUnitedMarketPlaceWindow* CNewUISystem::GetUI_pNewUnitedMarketPlaceWindow() const
{
	return m_pNewUnitedMarketPlaceWindow;
}

#ifdef LEM_ADD_LUCKYITEM
CNewUILuckyItemWnd* SEASON3B::CNewUISystem::Get_pNewUILuckyItemWnd() const
{
	return m_pNewUILuckyItemWnd;
}
#endif // LEM_ADD_LUCKYITEM

CNewUIMuHelper* CNewUISystem::Get_pNewUIMuHelper() const
{
	return m_pNewUIMuHelper;
}

CNewUIMuHelperConfig* CNewUISystem::Get_pNewUIMuHelperConfig() const
{
	return m_pNewUIMuHelperConfig;
}

CNewUIMuHelperSkillList* CNewUISystem::Get_pNewUIMuHelperSkillList() const
{
	return m_pNewUIMuHelperSkillList;
}

CNewUIMenuOption* CNewUISystem::Get_pNewUIMenuOption() const
{
	return m_pNewUIMenuOption;
}

CNewUIRanking* CNewUISystem::Get_pNewUIRanking() const
{
	return m_pNewUIRanking;
}
CNewUIMainMap* CNewUISystem::Get_pNewUIMainMap() const
{
	return m_pNewUIMainMap;
}

CNewUIJewelBank* CNewUISystem::Get_pNewUIJewelBank() const
{
	return m_pNewUIJewelBank;
}

CNewUIPKSystem* CNewUISystem::Get_pNewUIPKSystem() const
{
	return m_pNewUIPKSystem;
}

CNewUIEventTime* CNewUISystem::Get_pNewUIEventTime() const
{
	return m_pNewUIEventTime;
}

CNewUIRelife* CNewUISystem::Get_pNewUIRelife() const
{
	return m_pNewUIRelife;
}

CNewUIResetChange* CNewUISystem::Get_pNewUIResetChange() const
{
	return m_pNewUIResetChange;
}

CNewUIGiftCode* CNewUISystem::Get_pNewUIGiftCode() const
{
	return m_pNewUIGiftCode;
}

CNewUIChangeClass* CNewUISystem::Get_pNewUIChangeClass() const
{
	return m_pNewUIChangeClass;
}
#if ChangePassTurn
CNewUIChangePass* CNewUISystem::Get_pNewUIChangePass() const
{
	return m_pNewUIChangePass;
}
#endif
CNewUIDanhHieu* CNewUISystem::Get_pNewUIDanhHieu() const
{
	return m_pNewUIDanhHieu;
}
CNewUIQuanHam* CNewUISystem::Get_pNewUIQuanHam() const
{
	return m_pNewUIQuanHam;
}
CNewUITuLuyen* CNewUISystem::Get_pNewUITuLuyen() const
{
	return m_pNewUITuLuyen;
}
CNewUIHonHoan* CNewUISystem::Get_pNewUIHonHoan() const
{
	return m_pNewUIHonHoan;
}

CNewUIRenderDameBoss* CNewUISystem::Get_pNewUIRenderDameBoss() const
{
	return m_pNewUIRenderDameBoss;
}

CNewUIRenderInvasion* CNewUISystem::Get_pNewUIRenderInvasion() const
{
	return m_pNewUIRenderInvasion;
}

CNewUIRenderCoin* CNewUISystem::Get_pNewUIRenderCoin() const
{
	return m_pNewUIRenderCoin;
}

CNewUISpinPush* CNewUISystem::Get_pNewUISpinPush() const
{
	return m_pNewUISpinPush;
}

CNewUIViewItem* CNewUISystem::Get_pNewUIViewItem() const
{
	return m_pNewUIViewItem;
}

CNewUIAccountLevel* CNewUISystem::Get_pNewUIAccountLevel() const
{
	return m_pNewUIAccountLevel;
}
CNewUINewShop* CNewUISystem::Get_pNewUINewShop() const
{
	return m_pNewUINewShop;
}

CNewUIMocNap* CNewUISystem::Get_pNewUIMocNap() const
{
	return m_pNewUIMocNap;
}

CNewUIQQ* CNewUISystem::Get_pNewUIQQ() const
{
	return m_pNewUIQQ;
}

CNewUICastleEvent* CNewUISystem::Get_pNewUICastleEvent() const
{
	return m_pNewUICastleEvent;
}

CNewUIRenderColorItem* CNewUISystem::Get_pNewUIRenderColorItem() const
{
	return m_pNewUIRenderColorItem;
}

CNewUIAutoReset* CNewUISystem::Get_pNewUIAutoReset() const
{
	return m_pNewUIAutoReset;
}