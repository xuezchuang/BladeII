
#include "B2UILinkManager.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2ClientDataStore.h"
#include "Retailer.h"

//#include "B2UIStore.h"
//#include "B2UIMail.h"
//#include "B2UISummonItem.h"
//#include "B2UIEther.h"
//#include "B2UIHallOfFame.h"
//#include "B2UIMission.h"
//#include "B2UIFairyMain.h"

#include "BladeIIGameImpl.h"

FB2UILinkManager* FB2UILinkManager::UILinkManager;
EUIScene FB2UILinkManager::CurrentLinkerScene = EUIScene::Max;

FB2UILinkManager& FB2UILinkManager::GetInstance()
{
	if (UILinkManager == nullptr)
	{
		UILinkManager = new FB2UILinkManager();
		UILinkManager->GenerateUILinkBase();
	}

	return *UILinkManager;
}

bool FB2UILinkManager::LinkUIScene(EUIScene UIScene, int32 Optional1 /*= 0*/, int32 Optional2 /*= 0*/)
{
	if (FB2UILinkBase* LinkInfo = GetInstance().GetUILinkInfo(UIScene))
	{
		auto* DocCondition = UB2UIDocHelper::GetDocUICondition();
		if (DocCondition)
		{
			if (DocCondition->GetHideAllLobbyMainUIs() == true)
				DocCondition->SetHideAllLobbyMainUIs(false);
			
			CurrentLinkerScene = UIScene;
		}

		LinkInfo->LinkUIScene(Optional1, Optional2);

		return true;
	}

	return false;
}

void FB2UILinkManager::ExcuteUIScene()
{ 
	// 씬전환전에 정보값 갱신이 필요한 경우에 사용.
	// 정보값 갱신이 따로 필요없을 경우엔 각씬의 LinkUIScene()에서 CurrentLinkerScene을 MAX로 초기화해주어야함. 
	if (FB2UILinkBase* LinkInfo = GetInstance().GetUILinkInfo(CurrentLinkerScene))
	{
		LinkInfo->ExcuteUIScene();
	}

	GetInstance().SetCurrentLinkScene(EUIScene::Max);
}

void FB2UILinkManager::SetCurrentLinkScene(EUIScene InScene)
{
	CurrentLinkerScene = InScene;
}

void FB2UILinkManager::GenerateUILinkBase()
{
	REGISTER_UISCENE_LINK(Chapter, EUIScene::Chapter);
	REGISTER_UISCENE_LINK(BattleStageInfo, EUIScene::BattleStageInfo);
	REGISTER_UISCENE_LINK(CollectBookMain, EUIScene::CollectBookMain);
	REGISTER_UISCENE_LINK(HeroTowerReady, EUIScene::HeroTowerReady);
	REGISTER_UISCENE_LINK(Friend, EUIScene::Friend);
	REGISTER_UISCENE_LINK(CounterAttackDGMain, EUIScene::CounterAttackDGMain);
	REGISTER_UISCENE_LINK(Store, EUIScene::Store);
	REGISTER_UISCENE_LINK(TeamMatchUI, EUIScene::TeamMatchUI);
	REGISTER_UISCENE_LINK(PVPOneOnOne, EUIScene::PVPOneOnOne);
	REGISTER_UISCENE_LINK(Inventory, EUIScene::Inventory);
	REGISTER_UISCENE_LINK(Skill, EUIScene::Skill);
	REGISTER_UISCENE_LINK(Brevet, EUIScene::Brevet);
	REGISTER_UISCENE_LINK(Wing, EUIScene::Wing);
	REGISTER_UISCENE_LINK(Relic, EUIScene::Relic);
	REGISTER_UISCENE_LINK(GuildMain, EUIScene::GuildMain);
	REGISTER_UISCENE_LINK(RaidMain, EUIScene::RaidMain);
	REGISTER_UISCENE_LINK(Mail, EUIScene::Mail);
	REGISTER_UISCENE_LINK(Mission, EUIScene::Mission);
	REGISTER_UISCENE_LINK(EnterDungeonMain, EUIScene::EnterDungeonMain);
	REGISTER_UISCENE_LINK(EnterBattleMain, EUIScene::EnterBattleMain);
	REGISTER_UISCENE_LINK(LotteryStore, EUIScene::LotteryStore);
	REGISTER_UISCENE_LINK(ControlMain, EUIScene::ControlMain);
	REGISTER_UISCENE_LINK(EtherMain, EUIScene::EtherMain);
	REGISTER_UISCENE_LINK(HallOfFame, EUIScene::HallOfFame);
	REGISTER_UISCENE_LINK(FairyMain, EUIScene::FairyMain);
	REGISTER_UISCENE_LINK(PackageStore, EUIScene::PackageStore);
	REGISTER_UISCENE_LINK(DimensionMain, EUIScene::DimensionMain);
	REGISTER_UISCENE_LINK(Totem, EUIScene::Totem);
	REGISTER_UISCENE_LINK(Costume, EUIScene::Costume);
	REGISTER_UISCENE_LINK(ItemForge, EUIScene::ItemForge);
	REGISTER_UISCENE_LINK(SealBoxResult, EUIScene::SealBoxResult);
}

FB2UILinkBase* FB2UILinkManager::GetUILinkInfo(EUIScene InLinkUIScene)
{
	auto LinkInfo = UILinkMap.Find(InLinkUIScene);
	return LinkInfo != nullptr ? *LinkInfo : nullptr;
}

void FB2UILink_Chapter::LinkUIScene_Chapter(int32 Optional1, int32 Optional2)
{
	FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	FServerStageID ServerStageID = FServerStageID(Optional1);

	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	auto* ChapterUI = UB2UIDocHelper::GetDocChapter();

	int32 ChapterNum = StageDataStore.GetActIdByClientStageId(StageDataStore.GetClientStageId(ServerStageID));
	EStageDifficulty Difficulty = StageDataStore.GetStageDifficulty(ServerStageID);

	if (ServerStageID == INDEX_NONE)	// 최근 플레이 한 마지막 스테이지
	{
		//최근 플레이한 스테이지 정보
		ServerStageID = StageDataStore.GetLastPlayedServerStageId();
		int32 ChapterNumber = StageDataStore.GetActIdByClientStageId(StageDataStore.GetClientStageId(ServerStageID));
		EStageDifficulty StageDifficulty = StageDataStore.GetStageDifficulty(ServerStageID);

		if (ServerStageID == 0) // 서버에 저장된 최근 플레이 정보가 0 일때. 강제로 1-1로 세팅
		{
			ChapterNumber = 1;
			StageDifficulty = EStageDifficulty::ESD_Normal;
			ServerStageID = StageDataStore.GetServerStageID(1, StageDifficulty);
		}

		StageDataStore.RequestGetActInfoAndChangeChapter(ChapterNumber, Difficulty, true, ServerStageID);
	}

	else
	{
		if (ChapterNum > 0 && Difficulty != EStageDifficulty::ESD_None)
			StageDataStore.RequestGetActInfoAndChangeChapter(ChapterNum, Difficulty);
	}
}

void FB2UILink_Chapter::ExcuteUIScene_Chapter()
{

}

void FB2UILink_BattleStageInfo::LinkUIScene_BattleStageInfo(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//FServerStageID ServerStageID = FServerStageID(Optional1);

	//auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	//UB2StageInfo* StageInfoTable = StaticFindStageInfo();

	//const FServerStageID LastClearServerStageID = StageDataStore.GetLastClearServerStageIdByLastPlayedStageDifficulty();
	//const int32 LastClearClientStageID = StageDataStore.GetClientStageId(LastClearServerStageID);

	//FServerStageID RequestServerStageID = (ServerStageID.IsEmpty() == false) ? ServerStageID : LastClearServerStageID;
	//int32 RequestClientStageID = StageDataStore.GetClientStageId(RequestServerStageID);

	//int32 LastDifficulty = static_cast<int32>(StageDataStore.GetStageDifficulty(LastClearServerStageID));

	//int32 RequestDifficuly = static_cast<int32>(StageDataStore.GetStageDifficulty(RequestServerStageID));

	//int32 ActNumber = StageDataStore.GetActIdByClientStageId(RequestClientStageID);
	//EStageDifficulty Difficulty = StageDataStore.GetStageDifficulty(RequestServerStageID);

	//bool bSixChapterOrMore = false;

	//if (ActNumber > StageInfoTable->GetStartAdventureModeChapterNum())
	//{
	//	bSixChapterOrMore = !StageDataStore.GetNewChapterOpenState(ActNumber);
	//}



	//if ((RequestDifficuly >= LastDifficulty && LastClearClientStageID + 1 < RequestClientStageID)
	//	|| bSixChapterOrMore)
	//{
	//	// 앞으로 클리어 할 Stage보다 높은 단계 Stage를 Request할 경우
	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("NeedToClearStage")),
	//		0,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm
	//	);

	//	return;
	//}

	//

	//StageDataStore.RequestGetActInfoAndChangeChapter(ActNumber, Difficulty, true, RequestServerStageID);
}

void FB2UILink_BattleStageInfo::ExcuteUIScene_BattleStageInfo()
{

}

void FB2UILink_CollectBookMain::LinkUIScene_CollectBookMain(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//EPCClass LinkClass = IntToPCClass(Optional1);
	//EItemClass LinkItemClass = static_cast<EItemClass>(Optional2);
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CollectBookMain);

	//LinkUICollectBookClass<EPCClass, EItemClass>::GetInstance().Signal(LinkClass, LinkItemClass);
}

void FB2UILink_CollectBookMain::ExcuteUIScene_CollectBookMain()
{

}

void FB2UILink_HeroTowerReady::LinkUIScene_HeroTowerReady(int32 Optional1, int32 Optional2)
{
	data_trader::Retailer::GetInstance().RequestGetHeroTower();
	data_trader::Retailer::GetInstance().RequestGetChallengeModeInfo();
}

void FB2UILink_HeroTowerReady::ExcuteUIScene_HeroTowerReady()
{
	//LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_HeroTowerReady);
}

void FB2UILink_Friend::LinkUIScene_Friend(int32 Optional1, int32 Optional2)
{
	FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::Friend);
}

void FB2UILink_Friend::ExcuteUIScene_Friend()
{

}

void FB2UILink_CounterAttackDGMain::LinkUIScene_CounterAttackDGMain(int32 Optional1, int32 Optional2)
{
	data_trader::Retailer::GetInstance().RequestGetCounterDungeon();
	data_trader::Retailer::GetInstance().RequestGetChallengeModeInfo();
}

void FB2UILink_CounterAttackDGMain::ExcuteUIScene_CounterAttackDGMain()
{
	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::CounterAttackDGMain);
}

void FB2UILink_Store::LinkUIScene_Store(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//if (ShouldShowCBTVersionContent() && Optional1 == static_cast<int32>(EStoreCategory::Goods))
	//{
	//	DevShowNotReadyYetMessage(TEXT("CBT_CommonLimit"));
	//	return;
	//}

	//if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	//{
	//	DocStore->SetStoreTabToOpen(Optional1);
	//	DocStore->SetStoreGoodsTabToOpen(Optional2);
	//}
	//EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
}

void FB2UILink_Store::ExcuteUIScene_Store()
{

}

void FB2UILink_TeamMatchUI::LinkUIScene_TeamMatchUI(int32 Optional1, int32 Optional2)
{
	data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
	data_trader::Retailer::GetInstance().RequestGetDuelModeInfo();
}

void FB2UILink_TeamMatchUI::ExcuteUIScene_TeamMatchUI()
{
	//LobbyEnterTeamMatchMenuClass<>::GetInstance().Signal();
}

void FB2UILink_PVPOneOnOne::LinkUIScene_PVPOneOnOne(int32 Optional1, int32 Optional2)
{
	data_trader::Retailer::GetInstance().RequestGetMatchStatus();
	data_trader::Retailer::GetInstance().RequestGetDuelModeInfo();
}

void FB2UILink_PVPOneOnOne::ExcuteUIScene_PVPOneOnOne()
{
	//LobbyEnterPVPMainMenuClass<>::GetInstance().Signal();
}

void FB2UILink_Inventory::LinkUIScene_Inventory(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//// 지금은 그냥 UIScene 만 가지고 써먹어도 될 듯?
	//EHeroMgmtSubMode UISceneToDJLegacyMapping = UISceneToDJLegacyHeroMgmt(UIScene);

	//// Optional1 은 EUIScene::Inventory 이외의 것들이 추가되기 전에 필요했던 거.. Inventory 이외에는 UISceneToDJLegacyHeroMgmt 결과로 충분.
	//EHeroMgmtSubMode MgmtMode = (UISceneToDJLegacyMapping == EHeroMgmtSubMode::EHMSM_Inventory) ?
	//	static_cast<EHeroMgmtSubMode>(Optional1) : UISceneToDJLegacyMapping;

	//if (Optional2 > 0)
	//{
	//	EPCClass PCClass = IntToPCClass(Optional2 - 1); // 1:검투사 2:암살자 3:마법사 4:격투가
	//	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
	//}
	//LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(MgmtMode);
}

void FB2UILink_Inventory::ExcuteUIScene_Inventory()
{

}

void FB2UILink_Skill::LinkUIScene_Skill(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//// 지금은 그냥 UIScene 만 가지고 써먹어도 될 듯?
	//EHeroMgmtSubMode UISceneToDJLegacyMapping = UISceneToDJLegacyHeroMgmt(UIScene);

	//// Optional1 은 EUIScene::Inventory 이외의 것들이 추가되기 전에 필요했던 거.. Inventory 이외에는 UISceneToDJLegacyHeroMgmt 결과로 충분.
	//EHeroMgmtSubMode MgmtMode = (UISceneToDJLegacyMapping == EHeroMgmtSubMode::EHMSM_Inventory) ?
	//	static_cast<EHeroMgmtSubMode>(Optional1) : UISceneToDJLegacyMapping;

	//if (Optional2 > 0)
	//{
	//	EPCClass PCClass = IntToPCClass(Optional2 - 1); // 1:검투사 2:암살자 3:마법사 4:격투가
	//	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
	//}
	//LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(MgmtMode);
}

void FB2UILink_Skill::ExcuteUIScene_Skill()
{

}

void FB2UILink_Brevet::LinkUIScene_Brevet(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//// 지금은 그냥 UIScene 만 가지고 써먹어도 될 듯?
	//EHeroMgmtSubMode UISceneToDJLegacyMapping = UISceneToDJLegacyHeroMgmt(UIScene);

	//// Optional1 은 EUIScene::Inventory 이외의 것들이 추가되기 전에 필요했던 거.. Inventory 이외에는 UISceneToDJLegacyHeroMgmt 결과로 충분.
	//EHeroMgmtSubMode MgmtMode = (UISceneToDJLegacyMapping == EHeroMgmtSubMode::EHMSM_Inventory) ?
	//	static_cast<EHeroMgmtSubMode>(Optional1) : UISceneToDJLegacyMapping;

	//if (Optional2 > 0)
	//{
	//	EPCClass PCClass = IntToPCClass(Optional2 - 1); // 1:검투사 2:암살자 3:마법사 4:격투가
	//	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
	//}

	//LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(MgmtMode);
}

void FB2UILink_Brevet::ExcuteUIScene_Brevet()
{

}

void FB2UILink_Wing::LinkUIScene_Wing(int32 Optional1, int32 Optional2)
{
//	FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);
//
//	// 지금은 그냥 UIScene 만 가지고 써먹어도 될 듯?
//	EHeroMgmtSubMode UISceneToDJLegacyMapping = UISceneToDJLegacyHeroMgmt(UIScene);
//
//	// Optional1 은 EUIScene::Inventory 이외의 것들이 추가되기 전에 필요했던 거.. Inventory 이외에는 UISceneToDJLegacyHeroMgmt 결과로 충분.
//	EHeroMgmtSubMode MgmtMode = (UISceneToDJLegacyMapping == EHeroMgmtSubMode::EHMSM_Inventory) ?
//		static_cast<EHeroMgmtSubMode>(Optional1) : UISceneToDJLegacyMapping;
//
//	if (Optional2 > 0)
//	{
//		EPCClass PCClass = IntToPCClass(Optional2 - 1); // 1:검투사 2:암살자 3:마법사 4:격투가
//		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
//	}
//
//	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(MgmtMode);
}

void FB2UILink_Wing::ExcuteUIScene_Wing()
{

}

void FB2UILink_Relic::LinkUIScene_Relic(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//// 지금은 그냥 UIScene 만 가지고 써먹어도 될 듯?
	//EHeroMgmtSubMode UISceneToDJLegacyMapping = UISceneToDJLegacyHeroMgmt(UIScene);

	//// Optional1 은 EUIScene::Inventory 이외의 것들이 추가되기 전에 필요했던 거.. Inventory 이외에는 UISceneToDJLegacyHeroMgmt 결과로 충분.
	//EHeroMgmtSubMode MgmtMode = (UISceneToDJLegacyMapping == EHeroMgmtSubMode::EHMSM_Inventory) ?
	//	static_cast<EHeroMgmtSubMode>(Optional1) : UISceneToDJLegacyMapping;

	//if (Optional2 > 0)
	//{
	//	EPCClass PCClass = IntToPCClass(Optional2 - 1); // 1:검투사 2:암살자 3:마법사 4:격투가
	//	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
	//}

	//LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(MgmtMode);
}

void FB2UILink_Relic::ExcuteUIScene_Relic()
{

}

void FB2UILink_GuildMain::LinkUIScene_GuildMain(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//if (BladeIIGameImpl::GetClientDataStore().GetUserGuildID())
	//{
	//	data_trader::Retailer::GetInstance().RequestGuildDetailInfo(/*BladeIIGameImpl::GetClientDataStore().GetUserGuildID()*/0);
	//}
	//else
	//	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildJoinAndCreate);
}

void FB2UILink_GuildMain::ExcuteUIScene_GuildMain()
{

}

void FB2UILink_RaidMain::LinkUIScene_RaidMain(int32 Optional1, int32 Optional2)
{
	data_trader::Retailer::GetInstance().RequestGetChallengeModeInfo();
	data_trader::Retailer::GetInstance().RequestGetRaid(); // 패널티타임 받아오기용..
}

void FB2UILink_RaidMain::ExcuteUIScene_RaidMain()
{
	//LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_RaidMain);
}

//
void FB2UILink_Mail::LinkUIScene_Mail(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//MailTabIndex RequestTab = MailTabIndex::EMailTab_Gift;
	//if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::Mail)
	//{
	//	if (UB2UIMail* UIMail = UB2UIManager::GetInstance()->GetUI<UB2UIMail>(UIFName::Mail))
	//	{
	//		RequestTab = UIMail->GetCurrentMailTab();
	//	}
	//}
	//
	//data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(RequestTab));
}

void FB2UILink_Mail::ExcuteUIScene_Mail()
{

}

void FB2UILink_Mission::LinkUIScene_Mission(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::Mission);
	//auto MissionUI = UB2UIManager::GetInstance()->GetUI<UB2UIMission>(UIFName::Mission);
	//if (MissionUI)
	//{
	//	MissionUI->ChangeTapShortcut(static_cast<EMissionType>(Optional1));
	//}
}

void FB2UILink_Mission::ExcuteUIScene_Mission()
{

}

void FB2UILink_EnterDungeonMain::LinkUIScene_EnterDungeonMain(int32 Optional1, int32 Optional2)
{
	data_trader::Retailer::GetInstance().RequestGetChallengeModeInfo();
}

void FB2UILink_EnterDungeonMain::ExcuteUIScene_EnterDungeonMain()
{
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EnterDungeonMain);
}

void FB2UILink_EnterBattleMain::LinkUIScene_EnterBattleMain(int32 Optional1, int32 Optional2)
{
	data_trader::Retailer::GetInstance().RequestGetDuelModeInfo();
}

void FB2UILink_EnterBattleMain::ExcuteUIScene_EnterBattleMain()
{
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EnterBattleMain);
}

void FB2UILink_LotteryStore::LinkUIScene_LotteryStore(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//UB2UIManager* const UIMgr = UB2UIManager::GetInstance();
	//if (UIMgr)
	//{ // 자잘한 팝업이 뜬 상태에서 상점 진입하는 경우가 생길 수 있어서 그거 처리.
	//	UIMgr->CloseAllStandaloneUIs();
	//}

	//auto* SummonItemUI(UIMgr->GetUI<UB2UISummonItem>(UIFName::SummonItemStore));
	//if (SummonItemUI)
	//{
	//	ESummonItemTabMenu tab = static_cast<ESummonItemTabMenu>(Optional1);
	//	SummonItemUI->SetTab();
	//}
	//else
	//	data_trader::Retailer::GetInstance().RequestGetLotteryShop();
}

void FB2UILink_LotteryStore::ExcuteUIScene_LotteryStore()
{

}

void FB2UILink_ControlMain::LinkUIScene_ControlMain(int32 Optional1, int32 Optional2)
{
	data_trader::Retailer::GetInstance().RequestAssaultBattleStatus();
	data_trader::Retailer::GetInstance().RequestGetDuelModeInfo();
}

void FB2UILink_ControlMain::ExcuteUIScene_ControlMain()
{
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::ControlMain);
}

void FB2UILink_EtherMain::LinkUIScene_EtherMain(int32 Optional1, int32 Optional2)
{
	//FB2UILinkManager::GetInstance().SetCurrentLinkScene(EUIScene::Max);

	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EtherMain);

	//if (Optional2 > 0)
	//{
	//	EPCClass PCClass = IntToPCClass(Optional2 - 1); // 1:검투사 2:암살자 3:마법사 4:격투가
	//	UB2UIManager* const UIMgr = UB2UIManager::GetInstance();
	//	if (UIMgr)
	//	{
	//		auto* EtherUI = UIMgr->GetUI<UB2UIEther>(UIFName::EtherMain);
	//		if (EtherUI)
	//		{
	//			EtherUI->SetCharacterInfo(PCClass);
	//		}
	//	}
	//}
}

void FB2UILink_EtherMain::ExcuteUIScene_EtherMain()
{

}

void FB2UILink_HallOfFame::LinkUIScene_HallOfFame(int32 Optional1, int32 Optional2)
{
	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	{
		data_trader::Retailer::GetInstance().RequestGetDuelModeInfo();
		DocSome->SetOnClickHallOfFame(true);
		DocSome->SetHallOfFameMode(EHallOfFameMode::PVP);
	}
}

void FB2UILink_HallOfFame::ExcuteUIScene_HallOfFame()
{

}


void FB2UILink_FairyMain::LinkUIScene_FairyMain(int32 Optional1, int32 Optional2)
{
	//EFairyType FairyType = SvrToCliFairyType(Optional1);

	//if (FairyType == EFairyType::FAIRY_NONE)
	//	FairyType = EFairyType::FAIRY_NIECE;

	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::FairyMain);

	//UB2UIManager* const UIMgr = UB2UIManager::GetInstance();
	//if (UIMgr)
	//{
	//	auto* FairyUI = UIMgr->GetUI<UB2UIFairyMain>(UIFName::FairyMain);
	//	if (FairyUI)
	//	{
	//		FairyUI->OnChangedFairy(FairyType);
	//	}
	//}
}

void FB2UILink_FairyMain::ExcuteUIScene_FairyMain()
{

}

void FB2UILink_ItemForge::LinkUIScene_ItemForge(int32 Optional1, int32 Optional2)
{

}

void FB2UILink_ItemForge::ExcuteUIScene_ItemForge()
{

}

void FB2UILink_SealBoxResult::LinkUIScene_SealBoxResult(int32 Optional1, int32 Optional2)
{

}

void FB2UILink_SealBoxResult::ExcuteUIScene_SealBoxResult()
{

}

void FB2UILink_PackageStore::LinkUIScene_PackageStore(int32 Optional1, int32 Optional2)
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();

	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr)
	{ // 자잘한 팝업이 뜬 상태에서 상점 진입하는 경우가 생길 수 있어서 그거 처리.
		UIMgr->CloseAllStandaloneUIs();
		DocStore->SetPackageTabToOpen(Optional1);
	}

	data_trader::Retailer::GetInstance().RequestGetPackageShop();
}

void FB2UILink_PackageStore::ExcuteUIScene_PackageStore()
{

}

void FB2UILink_DimensionMain::LinkUIScene_DimensionMain(int32 Optional1, int32 Optional2)
{

}

void FB2UILink_DimensionMain::ExcuteUIScene_DimensionMain()
{

}

void FB2UILink_Totem::LinkUIScene_Totem(int32 Optional1, int32 Optional2)
{
	//// 지금은 그냥 UIScene 만 가지고 써먹어도 될 듯?
	//EHeroMgmtSubMode UISceneToDJLegacyMapping = UISceneToDJLegacyHeroMgmt(UIScene);

	//// Optional1 은 EUIScene::Inventory 이외의 것들이 추가되기 전에 필요했던 거.. Inventory 이외에는 UISceneToDJLegacyHeroMgmt 결과로 충분.
	//EHeroMgmtSubMode MgmtMode = (UISceneToDJLegacyMapping == EHeroMgmtSubMode::EHMSM_Inventory) ?
	//	static_cast<EHeroMgmtSubMode>(Optional1) : UISceneToDJLegacyMapping;

	//if (Optional2 > 0)
	//{
	//	EPCClass PCClass = IntToPCClass(Optional2 - 1); // 1:검투사 2:암살자 3:마법사 4:격투가
	//	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
	//}

	//LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(MgmtMode);
}

void FB2UILink_Totem::ExcuteUIScene_Totem()
{

}

void FB2UILink_Costume::LinkUIScene_Costume(int32 Optional1, int32 Optional2)
{
	//EnterCostumePageClass<ECostumePage>::GetInstance().Signal(ECostumePage::Inven);
}

void FB2UILink_Costume::ExcuteUIScene_Costume()
{

}