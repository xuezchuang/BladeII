// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICombineMenu.h"
#include "B2UIManager.h"
#include "Retailer.h"

#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "TutorialManager.h"
#include "B2UIAttendanceMain.h"
#include "B2LobbyGameMode.h"
#include "MissionManager.h"
#include "B2PlatformAPI.h"
#include "B2AndroidBackManager.h"
#include "B2UILobbyMain.h"
#include "B2UIEventMain.h"
#include "B2UIDocHelper.h"
#include "B2UISeasonEventCombineMenuBtn.h"
#include "B2UIItemForge.h"
#include "InternalEvents.h"
#include "B2UIHallOfFame.h"
#include "B2UILinkManager.h"
#include "B2UICostumeOpenBtn.h"

#if PLATFORM_ANDROID
#include "AndroidRuntimeSettings.h"
#include "Android/AndroidJNI.h"
#endif
#if PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#include "IOSRuntimeSettings.h"
#endif

void UB2UICombineMenu::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Inven);
	GET_SLOT(UB2Button, BTN_Collect);
	GET_SLOT(UB2Button, BTN_Mission);
	GET_SLOT(UB2Button, BTN_Friend);
	GET_SLOT(UB2Button, BTN_Guild);
	GET_SLOT(UB2Button, BTN_Skill);
	GET_SLOT(UB2Button, BTN_Brevet);
	GET_SLOT(UB2Button, BTN_Wing);
	GET_SLOT(UB2Button, BTN_Relic);
	GET_SLOT(UB2Button, BTN_Ether);
	GET_SLOT(UB2Button, BTN_Attendance);
	GET_SLOT(UB2Button, BTN_RestPoint);
	GET_SLOT(UB2Button, BTN_Notice);
	GET_SLOT(UB2Button, BTN_Community);
	GET_SLOT(UB2Button, BTN_Event);
	GET_SLOT(UB2Button, BTN_GoogleMission);
	GET_SLOT(UB2Button, BTN_QNA);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_HallOfFame);
	GET_SLOT(UB2Button, BTN_Fairy);
	GET_SLOT(UB2Button, BTN_Totem);
	GET_SLOT(UB2Button, BTN_ItemForge);

	GET_SLOT(UPanelWidget, P_HeroMgt);
	GET_SLOT(UImage, IMG_HeroMgtSelect);

	GET_SLOT(UTextBlock, TB_Mgmt);
	GET_SLOT(UTextBlock, TB_HeroMgt);
	GET_SLOT(UTextBlock, TB_Collect);
	GET_SLOT(UTextBlock, TB_Mission);
	GET_SLOT(UTextBlock, TB_Friend);
	GET_SLOT(UTextBlock, TB_Guild);
	GET_SLOT(UTextBlock, TB_Skill);
	GET_SLOT(UTextBlock, TB_Promote);
	GET_SLOT(UTextBlock, TB_Wing);
	GET_SLOT(UTextBlock, TB_AncientRelic);
	GET_SLOT(UTextBlock, TB_Ether);
	GET_SLOT(UTextBlock, TB_Attendance);
	GET_SLOT(UTextBlock, TB_BreakTime);
	GET_SLOT(UTextBlock, TB_NoticeMenu);
	GET_SLOT(UTextBlock, TB_Community);
	GET_SLOT(UTextBlock, TB_Event);
	GET_SLOT(UTextBlock, TB_GoogleMission);
	GET_SLOT(UTextBlock, TB_HelpMenu);
	GET_SLOT(UTextBlock, TB_HallOfFame);
	GET_SLOT(UTextBlock, TB_Fairy);
	GET_SLOT(UTextBlock, TB_Totem);
	GET_SLOT(UTextBlock, TB_Forge);

	GET_SLOT(UB2UISeasonEventCombineMenuBtn, UIP_SeasonEventCombineMenuBtn);

	if (UIP_SeasonEventCombineMenuBtn.IsValid())
		UIP_SeasonEventCombineMenuBtn->Init();

	GET_SLOT(UB2UISettingMsgPopup, UI_MsgPopupSetting_01)
		if (UI_MsgPopupSetting_01.IsValid()) {
			UI_MsgPopupSetting_01->Init();
			UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Hidden);
		}

	GET_SLOT(UB2UICostumeOpenBtn, UIP_CostumeOpenBtn)
	if (UIP_CostumeOpenBtn.IsValid())
		UIP_CostumeOpenBtn->Init();
}

void UB2UICombineMenu::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Inven, &UB2UICombineMenu::OnClickBTN_Inven);
	BIND_CLICK_FUNC_TO_BTN(BTN_Collect, &UB2UICombineMenu::OnClickBTN_Collect);
	BIND_CLICK_FUNC_TO_BTN(BTN_Mission, &UB2UICombineMenu::OnClickBTN_Mission);
	BIND_CLICK_FUNC_TO_BTN(BTN_Friend, &UB2UICombineMenu::OnClickBTN_Friend);
	BIND_CLICK_FUNC_TO_BTN(BTN_Guild, &UB2UICombineMenu::OnClickBTN_Guild);
	BIND_CLICK_FUNC_TO_BTN(BTN_Skill, &UB2UICombineMenu::OnClickBTN_Skill);
	BIND_CLICK_FUNC_TO_BTN(BTN_Brevet, &UB2UICombineMenu::OnClickBTN_Brevet);
	BIND_CLICK_FUNC_TO_BTN(BTN_Wing, &UB2UICombineMenu::OnClickBTN_Wing);
	BIND_CLICK_FUNC_TO_BTN(BTN_Relic, &UB2UICombineMenu::OnClickBTN_Relic);
	BIND_CLICK_FUNC_TO_BTN(BTN_Ether, &UB2UICombineMenu::OnClickBTN_Ether);
	BIND_CLICK_FUNC_TO_BTN(BTN_Attendance, &UB2UICombineMenu::OnClickBTN_Attendance);
	BIND_CLICK_FUNC_TO_BTN(BTN_RestPoint, &UB2UICombineMenu::OnClickBTN_RestPoint);
	BIND_CLICK_FUNC_TO_BTN(BTN_Notice, &UB2UICombineMenu::OnClickBTN_Notice);
	BIND_CLICK_FUNC_TO_BTN(BTN_Community, &UB2UICombineMenu::OnClickBTN_Community);
	BIND_CLICK_FUNC_TO_BTN(BTN_Event, &UB2UICombineMenu::OnClickBTN_Event);
	BIND_CLICK_FUNC_TO_BTN(BTN_GoogleMission, &UB2UICombineMenu::OnClickBTN_GoogleMission);
	BIND_CLICK_FUNC_TO_BTN(BTN_QNA, &UB2UICombineMenu::OnClickBTN_QNA);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UICombineMenu::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_HallOfFame, &UB2UICombineMenu::OnClickBTN_HallOfFame);
	BIND_CLICK_FUNC_TO_BTN(BTN_Fairy, &UB2UICombineMenu::OnClickBTN_Fairy);
	BIND_CLICK_FUNC_TO_BTN(BTN_Totem, &UB2UICombineMenu::OnClickBTN_Totem);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemForge, &UB2UICombineMenu::OnClickBTN_ItemForge);
}

void UB2UICombineMenu::UpdateStaticText()
{
	if (TB_Mgmt.IsValid())
		TB_Mgmt->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_EquipMgmtButton")));
	if (TB_HeroMgt.IsValid())
		TB_HeroMgt->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_EquipManagement")));
	if (TB_Collect.IsValid())
		TB_Collect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook")));
	if (TB_Mission.IsValid())
		TB_Mission->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mission")));
	if (TB_Friend.IsValid())
		TB_Friend->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_FriendMenu")));
	if (TB_Guild.IsValid())
		TB_Guild->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Guild")));

	if (TB_Skill.IsValid())
		TB_Skill->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Skill")));
	if (TB_Promote.IsValid())
		TB_Promote->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Brevet")));
	if (TB_Wing.IsValid())
		TB_Wing->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Wing")));
	if (TB_AncientRelic.IsValid())
		TB_AncientRelic->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_AncientRelic")));
	if (TB_Ether.IsValid())
		TB_Ether->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Ether")));
	;
	if (TB_Attendance.IsValid())
		TB_Attendance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Attendance")));
	if (TB_BreakTime.IsValid())
		TB_BreakTime->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BreakTime_Reward")));
	if (TB_NoticeMenu.IsValid())
		TB_NoticeMenu->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_NoticeMenu")));
	if (TB_Community.IsValid())
		TB_Community->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_CommunityMenu")));
	if (TB_Event.IsValid())
		TB_Event->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_EventMenu")));
	if (TB_GoogleMission.IsValid())
		TB_GoogleMission->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_GoogleMissionMenu")));
	if (TB_HelpMenu.IsValid())
		TB_HelpMenu->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_HelpMenu")));

	if (TB_HallOfFame.IsValid())
		TB_HallOfFame->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mod_WeekReward_HallOfFame")));
	if (TB_Fairy.IsValid())
		TB_Fairy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Fairy")));
	if (TB_Totem.IsValid())
		TB_Totem->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Totem")));
	if (TB_Forge.IsValid())
		TB_Forge->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Title")));
}

void UB2UICombineMenu::Init()
{
	Super::Init();
	m_KakaoLinkDelegateCalled.AtomicSet(false);
	SubscribeEvent();
	// IOS에서는 구글 업적 아이콘이 안보이도록.
#if PLATFORM_ANDROID // kakao 로그인 delegate 등록
	FJavaWrapper::OnKakaoLinkCompletedDelegate.Remove(OnKakaoLinkHandler);
	OnKakaoLinkHandler = FJavaWrapper::OnKakaoLinkCompletedDelegate.AddUObject(this, &UB2UICombineMenu::KakaoLinkDelegate);
#elif PLATFORM_IOS
	FKakaoTalkPlugin::OnConnectDelegate.Remove(OnKakaoLinkHandler);
	OnKakaoLinkHandler = FKakaoTalkPlugin::OnConnectDelegate.AddUObject(this, &UB2UICombineMenu::KakaoLinkDelegate);
#endif

#if PLATFORM_IOS
	if (BTN_GoogleMission.IsValid())
		BTN_GoogleMission->SetVisibility(ESlateVisibility::Collapsed);
#endif
	if(B2P_IsInReview())
	{
		if (BTN_HallOfFame.IsValid())
		{
			BTN_HallOfFame->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	HallOfFameButtonDelay = false;
}

void UB2UICombineMenu::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	UnSubscribeEvent();

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(HallOfFameButtonDelayTH);
	}
}

void UB2UICombineMenu::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UICombineMenu::OpenMenu()
{
	B2AndroidBackManager::GetInstance()->AddBackWidget(this, FName(TEXT("CombineMenu")));
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (UI_MsgPopupSetting_01.IsValid()) {
		UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Hidden);
	}

	if (auto* DocChapter = UB2UIDocHelper::GetDocUICondition())
		DocChapter->SetIsCombineMenu(true);

	OnUpdateCombineMenuClass<bool>::GetInstance().Signal(true);
}

void UB2UICombineMenu::CloseMenu()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(FName(TEXT("CombineMenu")));
	SetVisibility(ESlateVisibility::Collapsed);

	OnUpdateCombineMenuClass<bool>::GetInstance().Signal(false);
}

void UB2UICombineMenu::OnPlayOpenAni(bool bIsOpen)
{
	OnOpenCombineMenu_IMP(bIsOpen);
}
void UB2UICombineMenu::OnClickBTN_Inven()
{
	//튜토리얼 진행 중 인벤토리 진입시 MainPC변경을 가장 마지막에 호출하는 부분 나중에 변경 될 수 도 있다.
	if (TutorialManager::GetInstance().HasAnyTutorial())
	{
		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(
			TutorialManager::GetInstance().GetTutorialRequestPrepareClass());
	}
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UICombineMenu::OnClickBTN_Collect()
{
	int32 SvrPCype = 0; // 모든 클래스
	int32 SvrEquipCategory = 0;	// 전체 종류 아이템
	data_trader::Retailer::GetInstance().RequestGetCollectionItem(SvrPCype, SvrEquipCategory);

	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->ChangeUIScene(EUIScene::CollectBookSelectClass);
	}
}

void UB2UICombineMenu::OnClickBTN_Mission()
{
	MissionManager::GetInstance().MarkIsChangeScene();
	data_trader::Retailer::GetInstance().RequestGetMission(0);
}

void UB2UICombineMenu::OnClickBTN_Friend()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::Friend);
}

void UB2UICombineMenu::OnClickBTN_Guild()
{
	bool bGDC = ShouldShowGDCVersionContent(); // [GDC] Feb 2017

	if (bGDC)
	{
		DevShowNotReadyYetMessage();
		return;
	}

	if (BladeIIGameImpl::GetClientDataStore().GetUserGuildID())
	{
		//자신의 길드가 있을때는 무조건 0으로 넣어줘야한다 
		data_trader::Retailer::GetInstance().RequestGuildDetailInfo(0/*BladeIIGameImpl::GetClientDataStore().GetUserGuildID()*/);

	}
	else
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildJoinAndCreate);
}

void UB2UICombineMenu::OnClickBTN_Skill()
{
	OpenHeroMgmtPagesWithAsyncLoadCommon(EHeroMgmtSubMode::EHMSM_Skill);
}

void UB2UICombineMenu::OnClickBTN_Brevet()
{
	OpenHeroMgmtPagesWithAsyncLoadCommon(EHeroMgmtSubMode::EHMSM_Brevet);
}

void UB2UICombineMenu::OnClickBTN_Wing()
{
	OpenHeroMgmtPagesWithAsyncLoadCommon(EHeroMgmtSubMode::EHMSM_Wing);
}

void UB2UICombineMenu::OnClickBTN_Relic()
{
	OpenHeroMgmtPagesWithAsyncLoadCommon(EHeroMgmtSubMode::EHMSM_Relic);
}

void UB2UICombineMenu::OnClickBTN_Ether()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EtherMain);
}

void UB2UICombineMenu::OnClickBTN_Attendance()
{
	if (auto* UIMgrInst = UB2UIManager::GetInstance())
	{
		auto* AttendanceMainWidget = UIMgrInst->OpenUI<UB2UIAttendanceMain>(UIFName::AttendanceMain);
		AttendanceMainWidget->Init();
	}
}

void UB2UICombineMenu::OnClickBTN_RestPoint()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::RestPointMain);
}

void UB2UICombineMenu::OnClickBTN_Notice()
{
	B2P_ShowKakaoNoticeView(false, "");
}

void UB2UICombineMenu::OnClickBTN_Community()
{
	/*
	if (B2P_GetKakaoIDPCode() == EPlatformIDPCode::Guest)
	{
		// 서버 쪽으로 우선 검증을 위하여 보냄
		FString sIDPCode = TEXT("kakaocapri");
		data_trader::Retailer::GetInstance().RequestKakaoAccountConversionPrepare(sIDPCode);
	}
	else
	{
		B2P_KakaoCommunityView();
	}*/
	// 네이버 카페로 변경되어 수정
	B2P_ShowOpenURL(TEXT("https://cafe.naver.com/b2forkakao"));

}

void UB2UICombineMenu::OnClickBTN_Event()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EventMain);
}

void UB2UICombineMenu::OnClickBTN_GoogleMission()
{
	if (B2P_IsGoogleLogin())
	{
		B2P_ShowAchivementView();	// 구글 업적 화면으로~
	}
	else
	{
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			UIMgrInst->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Google_MissionFeatured")), // 대략 이런 내용의 경고 문자가 들어가겠지..
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
				);
		}
	}
}

void UB2UICombineMenu::OnClickBTN_QNA()
{
	B2P_ShowOpenURL(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("GuideURL_Menu")).ToString());
}

void UB2UICombineMenu::OnClickBTN_Close()
{
	if (auto* LobbyUI = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain))
		LobbyUI->UpdateCombineMenuVisible(false);

	if (auto* DocChapter = UB2UIDocHelper::GetDocUICondition())
		DocChapter->SetIsCombineMenu(false);

	CloseMenu();
}

void UB2UICombineMenu::OnClickBTN_HallOfFame()
{
	if (HallOfFameButtonDelay)
		return;

	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	{
		UWorld* TheWorld = GetWorld();

		if (TheWorld)
		{
			TheWorld->GetTimerManager().ClearTimer(HallOfFameButtonDelayTH);
			TheWorld->GetTimerManager().SetTimer(HallOfFameButtonDelayTH, this, &UB2UICombineMenu::EnableHallOfFameButton, 3.f, false);

		}
		
		HallOfFameButtonDelay = true;

		data_trader::Retailer::GetInstance().RequestGetDuelModeInfo();
		DocSome->SetOnClickHallOfFame(true);
		DocSome->SetHallOfFameMode(EHallOfFameMode::PVP);
	}
}

void UB2UICombineMenu::OnClickBTN_Fairy()
{
	FB2UILinkManager::LinkUIScene(EUIScene::FairyMain);
}

void UB2UICombineMenu::OnClickBTN_Totem()
{
	OpenHeroMgmtPagesWithAsyncLoadCommon(EHeroMgmtSubMode::EHMSM_Totem);
}

void UB2UICombineMenu::OnClickBTN_ItemForge()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::ItemForge);

	UB2UIItemForge* ItemForgeUI = UB2UIManager::GetInstance()->GetUI<UB2UIItemForge>(UIFName::ItemForge);
	if (ItemForgeUI)
	{
		ItemForgeUI->ForgeInit();
	}
}

bool UB2UICombineMenu::HasRedDot_Inventory()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Inventory();
}

bool UB2UICombineMenu::HasRedDot_ManageMent()
{
	return (HasRedDot_Skill() || HasRedDot_Brevet() || HasRedDot_Wing() || HasRedDot_Relic() || HasRedDot_Ether());
}

bool UB2UICombineMenu::HasRedDot_Collection()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_CollectBook();
}

bool UB2UICombineMenu::HasRedDot_Mission()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mission();
}

bool UB2UICombineMenu::HasRedDot_Friend()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Friend();
}

bool UB2UICombineMenu::HasRedDot_Guild()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Guild();
}

bool UB2UICombineMenu::HasRedDot_Skill()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Skill();
}

bool UB2UICombineMenu::HasRedDot_Brevet()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Brevet();
}

bool UB2UICombineMenu::HasRedDot_Wing()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Wing();
}

bool UB2UICombineMenu::HasRedDot_Relic()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Relic();
}

bool UB2UICombineMenu::HasRedDot_Ether()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Ether();
}

bool UB2UICombineMenu::HasRedDot_Attandance()
{
	return BladeIIGameImpl::GetRedDotManager().HadLobbyRedDot_Attendance();
}

bool UB2UICombineMenu::HasRedDot_RestPoint()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_RestPoint();
}

bool UB2UICombineMenu::HasRedDot_Notice()
{
	return false;
}

bool UB2UICombineMenu::HasRedDot_GoogleMission()
{
	return false;
}

bool UB2UICombineMenu::HasRedDot_Event()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event();
}

bool UB2UICombineMenu::HasRedDot_HallOfFame()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HallOfFame();
}

bool UB2UICombineMenu::HasRedDot_Fairy()
{
	return BladeIIGameImpl::GetRedDotManager().HasNotApplyBlessFairy();
}

bool UB2UICombineMenu::HasRedDot_Totem()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Totem();
}

void UB2UICombineMenu::OpenHeroMgmtPagesWithAsyncLoadCommon(EHeroMgmtSubMode InHeroMgmtMode)
{
	if (auto* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		ELobbyUIPages MatchingDjLegacyUIPage = HeroMgmtSubModeToLobbyUIPage(InHeroMgmtMode);
		bool bRequetAsyncLoading = BladeIIGameImpl::bAllowUIAsyncLoading && BladeIIGameImpl::bAllowUIOnDemandAsyncLoading;

		LobbyGM->DJLegacy_AsyncLoadUIPage(MatchingDjLegacyUIPage, bRequetAsyncLoading);
	}
}

//====================================================================================
//									Mark For New
//====================================================================================

void UB2UICombineMenu::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Inven.Get()), &UB2UICombineMenu::HasRedDot_Inventory);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Collect.Get()), &UB2UICombineMenu::HasRedDot_Collection);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Mission.Get()), &UB2UICombineMenu::HasRedDot_Mission);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Friend.Get()), &UB2UICombineMenu::HasRedDot_Friend);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Guild.Get()), &UB2UICombineMenu::HasRedDot_Guild);


	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Skill.Get()), &UB2UICombineMenu::HasRedDot_Skill);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Brevet.Get()), &UB2UICombineMenu::HasRedDot_Brevet);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Wing.Get()), &UB2UICombineMenu::HasRedDot_Wing);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Relic.Get()), &UB2UICombineMenu::HasRedDot_Relic);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Ether.Get()), &UB2UICombineMenu::HasRedDot_Ether);

	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Attendance.Get()), &UB2UICombineMenu::HasRedDot_Attandance);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_RestPoint.Get()), &UB2UICombineMenu::HasRedDot_RestPoint);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Notice.Get()), &UB2UICombineMenu::HasRedDot_Notice);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_GoogleMission.Get()), &UB2UICombineMenu::HasRedDot_GoogleMission);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Event.Get()), &UB2UICombineMenu::HasRedDot_Event);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_HallOfFame.Get()), &UB2UICombineMenu::HasRedDot_HallOfFame);

	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Fairy.Get()), &UB2UICombineMenu::HasRedDot_Fairy);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Totem.Get()), &UB2UICombineMenu::HasRedDot_Totem);
}

void UB2UICombineMenu::DoMarkRedDot()
{
	Super::DoMarkRedDot();

	if (UIP_SeasonEventCombineMenuBtn.IsValid())
		UIP_SeasonEventCombineMenuBtn->DoMarkRedDot();

	if (UIP_CostumeOpenBtn.IsValid())
		UIP_CostumeOpenBtn->DoMarkRedDot();
}

void UB2UICombineMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_KakaoLinkDelegateCalled)
	{
		FString sIDPCode = TEXT("kakaocapri");

		UE_LOG(LogClass, Log, TEXT("####RequestKakaoAccountConversionComplete sIDPCode!!!!!!!!!!!!!! = [[[%s]]]"), *sIDPCode);
		data_trader::Retailer::GetInstance().RequestKakaoAccountConversionComplete(sIDPCode);
		m_KakaoLinkDelegateCalled.AtomicSet(false);
	}
}

void UB2UICombineMenu::SubscribeEvent()
{
	if (bSubscribed == false)
	{
		CAPTURE_UOBJECT(UB2UICombineMenu);

		DeliveryKakaoAccountConversionPrepareTicket = DeliveryKakaoAccountConversionPrepareClass<FB2ReciveKakaoAccountConversionPrepare>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(DeliveryKakaoAccountConversionPrepare, FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing)
			Capture->OnReceiveKakaoAccountConversionPrepare(KakaoMessageProcessing);
		END_CAPTURE_OBJECT()
			);

		DeliveryKakaoAccountConversionCompleteTicket = DeliveryKakaoAccountConversionCompleteClass<FB2ReciveKakaoAccountConversionComplete>::GetInstance().Subscribe(
			USE_CAPTURE_OBJECT_AND_TICKET(DeliveryKakaoAccountConversionComplete, FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing)
			Capture->OnReceiveKakaoAccountConversionComplete(KakaoMessageProcessing);
		END_CAPTURE_OBJECT()
			);

		bSubscribed = true;
	}
}

void UB2UICombineMenu::UnSubscribeEvent()
{
	DeliveryKakaoAccountConversionPrepareClass<FB2ReciveKakaoAccountConversionPrepare>::GetInstance().Unsubscribe(DeliveryKakaoAccountConversionPrepareTicket);
	DeliveryKakaoAccountConversionCompleteClass<FB2ReciveKakaoAccountConversionComplete>::GetInstance().Unsubscribe(DeliveryKakaoAccountConversionCompleteTicket);

	DeliveryKakaoAccountConversionPrepareTicket = 0;
	DeliveryKakaoAccountConversionCompleteTicket = 0;
}

void UB2UICombineMenu::OnReceiveKakaoAccountConversionPrepare(FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing)
{
	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoAccountConversionPrepare RESPONSE SUCESS."));

	//TArray<b2network::B2RawRewardPtr> RawRewardPtr = GET_TUPLE_DATA(FB2ResponseKakaoAccountConversionPrepare::rewards_index, KakaoMessageProcessing);

	//for (auto RawRewardSet : RawRewardPtr)
	//{

	//	UI_MsgPopupSetting_01->SetRewardItem(RawRewardSet->id, RawRewardSet->count);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet id: %d"), RawRewardSet->id);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet count.: %d"), RawRewardSet->count);
	//}

	//UI_MsgPopupSetting_01->SetOwnerCombineUI(this);
	//UI_MsgPopupSetting_01->SetPopupType(EPopupTypeSetting::EPopupType_CombineKakaoLink);
	//UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Visible);
}

void UB2UICombineMenu::OnReceiveKakaoAccountConversionComplete(FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing)
{
	//UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoAccountConversionComplete RESPONSE SUCESS."));

	//TArray<b2network::B2RawRewardPtr> RawRewardPtr = GET_TUPLE_DATA(FB2ResponseKakaoAccountConversionComplete::rewards_index, KakaoMessageProcessing);

	//for (auto RawRewardSet : RawRewardPtr)
	//{
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet id: %d"), RawRewardSet->id);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet count.: %d"), RawRewardSet->count);
	//}
}

#if PLATFORM_ANDROID
void UB2UICombineMenu::KakaoLinkDelegate(JNIEnv *env, jobject thiz)
{
	UE_LOG(LogClass, Log, TEXT("####KakaoLinkDelegate !!!!!!!!!!!!!! = [[[]]]"));
	MarkKakaoLink();

}
#elif PLATFORM_IOS
void UB2UICombineMenu::KakaoLinkDelegate(bool bIsSuccess)
{
	UE_LOG(LogClass, Log, TEXT("####KakaoLinkDelegate !!!!!!!!!!!!!! = [[[]]]"));
	if (bIsSuccess)
	{
		MarkKakaoLink();
	}
}
#endif

void UB2UICombineMenu::KakaoPopupHidden()
{
	UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UICombineMenu::MarkKakaoLink()
{
	m_KakaoLinkDelegateCalled.AtomicSet(true);
}

void UB2UICombineMenu::KakaoConnect()
{
	UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Hidden);
	B2P_KakaoAccountLink();
}

void UB2UICombineMenu::EnableHallOfFameButton()
{
	HallOfFameButtonDelay = false;
}
