// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager_Lobby.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2LobbyUI_InventoryMain.h"
#include "B2LobbyUI_SetupSkillMain.h"
#include "B2LobbyUI_AutoEquip.h"
#include "B2LobbyUI_ItemLevelupMain.h"
#include "B2LobbyUI_ItemUpgradeMain.h"
#include "B2LobbyUI_ItemComposeMain.h"
#include "B2LobbyUI_ItemSurpassMain.h"
#include "B2LobbyUI_AutoItemLevelup.h"
#include "B2LobbyUI_ItemEnhance.h"
#include "B2LobbyUI_ItemDetail.h"
#include "B2LobbyUI_CharStatDetail.h"
#include "B2LobbyUI_BrevetMain.h"
#include "B2LobbyUI_WingMain.h"
#include "B2LobbyUI_WingPropOptionEnhance.h"
#include "B2LobbyUI_RelicMain.h"
#include "B2LobbyUI_RelicManagement.h"
#include "B2LobbyUI_ItemAfterOption.h"
#include "B2UILobbyMain.h"
#include "B2UIManager.h"
#include "B2UILogin.h"
#include "Event.h"
#include "B2FloatingLobbyOnHeadDisplay.h"
#include "Animation/SkeletalMeshActor.h"
#include "B2LobbyInventory.h"
#include "QuestManager.h"
#include "QuestDataTable.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIQuestDialog.h"
#include "B2UILinkManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "TutorialManager.h"
#include "B2UIMailRewardPopUp.h"
#include "B2UIAttendanceMain.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2DynItemIcon_LobbyEquip.h"
#include "B2AndroidBackManager.h"
#include "B2UIMission.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "B2CodeTable.h"
#include "B2LobbyUI_ItemEnhanceChangeMain.h"

const FString UB2UIManager_Lobby::AsyncOpenUIStr = TEXT("Open_");

void OpenDJLegacyHeroMgmtPageForUIScene(EUIScene InUIScene)
{
	// DJLegacy UI 와의 호환을 위해 EUIScene 에 넣어둔 더미 엔트리 맵핑
	switch (InUIScene)
	{
	case EUIScene::Inventory: 
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
		break;
	case EUIScene::Skill:
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Skill);
		break;
	case EUIScene::Brevet:
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Brevet);
		break;
	case EUIScene::Wing:
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Wing);
		break;
	case EUIScene::Relic:
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Relic);
		break;
	case EUIScene::RelicManagement:
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE);
		break;
	case EUIScene::Costume:
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Costume);
		break;
	case EUIScene::Totem:
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Totem);
		break;
	}
	// Don't forget for new entry
	checkSlow(static_cast<int32>(EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE) + 1 == static_cast<int32>(EHeroMgmtSubMode::EHMSM_End));
}

FDJLegacySubpopupAddPanelInfo::FDJLegacySubpopupAddPanelInfo(UB2UnitedWidgetBase* InOwner, UPanelWidget* InParent)
{
	OwnerWidget = InOwner;
	ParentPanel = InParent;
}
bool FDJLegacySubpopupAddPanelInfo::AddWidgetToThisInfo(UB2LobbyUserWidget* InCreatedWidget)
{
	// ParentPanel 을 특정 타입만 허용하도록 할 수도..
	if (InCreatedWidget && OwnerWidget && ParentPanel)
	{
		UPanelSlot* AddeddSlot = ParentPanel->AddChild(InCreatedWidget);
		UCanvasPanelSlot* CastedCPSlot = Cast<UCanvasPanelSlot>(AddeddSlot);
		// Slot 설정을 필요에 따라 다양하게 해야 할 수도..
		if (CastedCPSlot)
		{
			CastedCPSlot->SetAutoSize(true);
			CastedCPSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
			CastedCPSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
		}
		else
		{
			UOverlaySlot* CastedOVSlot = Cast<UOverlaySlot>(AddeddSlot);
			if(CastedOVSlot)
			{
				CastedOVSlot->SetHorizontalAlignment(HAlign_Fill);
				CastedOVSlot->SetVerticalAlignment(VAlign_Fill);
			}
		}
		return true;
	}

	return false;
}

/** 과거에 로비 캐릭터 머리 위에 둥둥 떠 있던 widget 인데 지금 기능 제거되어 있지만 혹시 몰라서 LobbySceneManager 쪽에 사용하는 코드를 남겨둔 듯 함
 * 단, 여기서 리소스 로딩하고 CreateWidget 하고 이런 것들은 넘나 낭비라서 요 define 으로 꺼 둠. */
#define RESTORE_BII_LOBBY_ONHEAD_DISPLAY 0

UB2UIManager_Lobby::UB2UIManager_Lobby(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerLobbyGM = NULL;
	CachedPlayerController = NULL;
	LobbyUIAssetInfo = NULL;
	CurrentActiveUIPage = NULL;
	CachedInvenStoredItemIconClass = NULL;
	CachedInvenEquippedItemIconClass = NULL;
	QuestNoticeState = NoticeState::NONE;
	AttendanceNoticeState = NoticeState::NONE;
	bEventsSubscribed = false;
	IsOpenDailyQeustDialog = false;

#if RESTORE_BII_LOBBY_ONHEAD_DISPLAY
	if (!GMinimalDLCFrontMode){ // DLC Front 모드 리소스로딩 최대한 제거
		FString OnHeadDisplayWidgetClassPath;
		GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("LobbyOnHeadDisplayWidgetClass"), OnHeadDisplayWidgetClassPath, GGameIni);
		static ConstructorHelpers::FClassFinder<UB2FloatingLobbyOnHeadDisplay> OnHeadDisplayWidgetClassFoundSet(*OnHeadDisplayWidgetClassPath);
		OnHeadDisplayWidgetClass = OnHeadDisplayWidgetClassFoundSet.Class;
	}
#else
	OnHeadDisplayWidgetClass = nullptr;
#endif
}

void UB2UIManager_Lobby::BeginDestroy()
{
	Super::BeginDestroy();

	RemoveCachedWidgetClasses();
	RemoveCachedSubWidgetClasses();

	UnsubscribeEvents();
}

void UB2UIManager_Lobby::OnPreLoadMap()
{ 
	// UObject 파괴는 BeginDestroy 가 아니라 여기가 적절하다.

	if (LobbyUIAssetInfo)
		LobbyUIAssetInfo->UnloadTAssets(true, this);

	DestroyAllLobbyCharOnHeadDisplay();
	DestroyDynItemIconWidgetPool();
}

bool UB2UIManager_Lobby::InitUIManager(AB2LobbyGameMode* InLobbyGM)
{
//	if (GMinimalDLCFrontMode){
//		return false; // DLC Front 모드 리소스로딩 최대한 제거
//	}
//
//	// Anything necessary..
//	// 만일 계속해서 메모리 상주하는 것이 있다면 여기서 생성하게 되겠다.
//
//	OwnerLobbyGM = InLobbyGM;
//	check(OwnerLobbyGM);
//
//	CachedPlayerController = UGameplayStatics::GetLocalPlayerController(OwnerLobbyGM);
//	check(CachedPlayerController);
//
//	// UISwitcher. 블루프린트 클래스 가져와서 Default Object 로다가
//	FString CfgClassPath;
//	GConfig->GetString(TEXT("/Script/BladeII.B2UIManager_Lobby"), TEXT("LobbyUISwitcherClass"), CfgClassPath, GGameIni);
//	ConstructorHelpers::StripObjectClass(CfgClassPath, true);
//	TSubclassOf<UB2LobbyUISwitcher> SwitcherClass = ConstructorHelpersInternal::FindOrLoadClass(CfgClassPath, UB2LobbyUISwitcher::StaticClass());
//	if (SwitcherClass != NULL)
//	{
//		LobbyUIAssetInfo = Cast<UB2LobbyUISwitcher>(SwitcherClass->GetDefaultObject());
//		LobbyUIAssetInfo->InitUISwitcher(OwnerLobbyGM);
//	}
//#if WITH_EDITOR && !PLATFORM_MAC
//	if (LobbyUIAssetInfo == NULL)
//	{
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//			FString::Printf(TEXT("[Warning] LobbyUISwitcher 를 찾지 못함. 컴퓨터가 곧 폭발한다."))
//			));
//		return false;
//	}
//#endif
//	
//	InitializeDynItemIconWidgetPool();
//
//	SubscribeEvents();
//
//	/* //UIManager쪽 체계와 붙히는 작업중...
//	// 일단 로긴창부터 시작. 이 이후는 GameMode 쪽의 DJLegacy_ChangeLobbyUIPage 를 사용하게 될 것.
//	DJLegacy_ChangeLobbyUIPage(ELobbyUIPages::ELUP_Login);
//	*/
//
//	//현재 인벤부분 때문에 B2UIManager_Lobby를 남겨두는 것이고 추후 제거하여야 함 -> DJLegacy 가 쉽게 사리지지는 않을 듯함.
//	UB2UIManager::GetInstance()->ClearUISceneHistory();	
//	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CharacterIntro); // 일단 CharacterIntro 로 시작했다가.. 전투에서 돌아오는 상황이라면 다른 걸로 바꾸게 될 것.
	return true;
}

void UB2UIManager_Lobby::UpdateManual(float DeltaSeconds)
{
	for (UB2FloatingLobbyOnHeadDisplay* ThisOnHead : CreatedOnHeadDisplays)
	{
		if (ThisOnHead)
		{
			ThisOnHead->UpdatePosition(); // 좀 바보같은 사정이 있어서 이렇게 직접 업데이트 해 준다. 처음 생성시에는 제대로 세팅이 안되는 문제에다가 NativeTick 이 안 불리는 등.
		}
	}
	/*
	if (IsOnDemandAsyncLoadUIPagePending())
	{
		TickCheckForOnDemandAsyncLoadTimeOut(DeltaSeconds);
	}
	*/
}

void UB2UIManager_Lobby::SubscribeEvents()
{
	if (bEventsSubscribed)
		return;

	CAPTURE_UOBJECT(UB2UIManager_Lobby);

	DJLegacy_ChangeLobbyUIPageTicket = DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DJLegacy_ChangeLobbyUIPage, ELobbyUIPages p)
			Capture->DJLegacy_ChangeLobbyUIPage(p); 
		END_CAPTURE_OBJECT()
	);
	DJLegacy_OpenLobbySubPopupTicket = DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DJLegacy_OpenLobbySubPopup, ELobbySubPopups p, FDJLegacySubpopupAddPanelInfo panelinfo)
			Capture->DJLegacy_OpenLobbySubPopup(p, panelinfo);
		END_CAPTURE_OBJECT());
	DJLegacy_CloseLobbySubPopupTicket = DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DJLegacy_CloseLobbySubPopup, ELobbySubPopups p)
			Capture->DJLegacy_CloseLobbySubPopup(p); 
		END_CAPTURE_OBJECT()
	);
	LobbyPCSelectionChangedTicket = LobbyPCSelectionChangedClass<EPCClass>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyPCSelectionChanged, EPCClass p)
			Capture->NotifyPCSelectionChanged(p); 
		END_CAPTURE_OBJECT()
	);
	LobbyUpdateAllItemIconsTicket = LobbyUpdateAllItemIconsClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyUpdateAllItemIcons)
			Capture->TryUpdateAllVisibleItemIcons();
		END_CAPTURE_OBJECT()
	);
	LobbyTryUpdateSingleItemIconTicket = LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyTryUpdateSingleItemIcon, int64 i)
			Capture->TryUpdateSingleVisibleItemIcon(i); 
		END_CAPTURE_OBJECT()
	);
	LobbyPopupCreateAccountTicket = LobbyPopupCreateAccountClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyPopupCreateAccount)
			Capture->TryPopupCreateAccountMenu(); 
		END_CAPTURE_OBJECT()
	);
	LobbyTryUpdateItemOPMainMenuTicket = LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyTryUpdateItemOPMainMenu)
			Capture->TryUpdateItemOPMainMenu(); 
		END_CAPTURE_OBJECT()
	);
	LobbyInvenSetItemOpModeGuideTextTicket = LobbyInvenSetItemOpModeGuideTextClass<const FText&>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyInvenSetItemOpModeGuideText, const FText& t)
			Capture->SetInventoryItemOpModeGuideText(t); 
		END_CAPTURE_OBJECT()
	);
	LobbyInvenHideItemOpModeGuideTextTicket = LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyInvenHideItemOpModeGuideText)
			Capture->HideInventoryItemOpModeGuideText();
		END_CAPTURE_OBJECT()
	);
	LobbyExternalSetInventoryTabTicket = LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyExternalSetInventoryTab, EItemInvenType it, bool bDisableOtherTabSelection)
			Capture->ExternalSetInventoryTab(it, bDisableOtherTabSelection);
		END_CAPTURE_OBJECT()
	);
	CreateLobbyCharOnHeadDisplayTicket = CreateLobbyCharOnHeadDisplayClass<EPCClass, ASkeletalMeshActor*, ELobbyOnHeadDisplayType>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(CreateLobbyCharOnHeadDisplay, EPCClass pc, ASkeletalMeshActor* sk, ELobbyOnHeadDisplayType mt)
			Capture->CreateLobbyCharOnHeadDisplay(pc, sk, mt);
		END_CAPTURE_OBJECT()
	);
	DestroyAllLobbyCharOnHeadDisplayTicket = DestroyAllLobbyCharOnHeadDisplayClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DestroyAllLobbyCharOnHeadDisplay)
			Capture->DestroyAllLobbyCharOnHeadDisplay();
		END_CAPTURE_OBJECT()
	);
	UpdateLobbyCharOnHeadDisplayTicket = UpdateLobbyCharOnHeadDisplayClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UpdateLobbyCharOnHeadDisplay)
			Capture->UpdateLobbyCharOnHeadDisplay();
		END_CAPTURE_OBJECT()
	);
	LobbyPlayUIEquipEffectTicket = LobbyPlayUIEquipEffectClass<EItemEquipPlace>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyPlayUIEquipEffect, EItemEquipPlace ep)
			Capture->PlayUIEquipEffect(ep);
		END_CAPTURE_OBJECT()
	);
	LobbyUpdateWingMainUITicket = LobbyUpdateWingMainUIClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyUpdateWingMainUI)
			Capture->UpdateWingMainUI();			
		END_CAPTURE_OBJECT()
	);

	DeliveryGetQuestUITicket = DeliveryGetQuestUIClass<FB2GetQuest>::GetInstance().Subscribe(//[this](const FB2GetQuest& InQuest){
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetQuestUI, const FB2GetQuest& InQuest)
			Capture->ResponseGetQuest(InQuest);
		END_CAPTURE_OBJECT()
	);

	DeliveryAcceptQuestTicket = DeliveryAcceptQuestClass<FB2AcceptQuest>::GetInstance().Subscribe(//[this](const FB2GetQuest& InQuest){
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryAcceptQuest, const FB2AcceptQuest& InQuest)
			Capture->ResponseAcceptQuest(InQuest);
		END_CAPTURE_OBJECT()
	);

	DeliveryQuestRewardTicket = DeliveryQuestRewardClass<Quest, TArray<b2network::B2RewardPtr>>::GetInstance().Subscribe(//[this](const FB2GetQuest& InQuest){
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryQuestReward, Quest CompleteQuest, const TArray<b2network::B2RewardPtr>& Reward)
			Capture->ResponseGetQuestReward(CompleteQuest, Reward);
		END_CAPTURE_OBJECT()
	);

	OnQuestCompleteNotifyTicket = OnQuestCompleteNotifyClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(OnQuestCompleteNotify, int32 CompleteQuestSlot)
			Capture->ResponseQuestComplete(CompleteQuestSlot);
		END_CAPTURE_OBJECT()
	);

	//
	DeliveryGetAccountRelicTicket = DeliveryGetAccountRelicClass<FB2ResponseGetAccountRelicPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetAccountRelic, const FB2ResponseGetAccountRelicPtr& msgPtr)
			Capture->ResponseGetAccountRelic(msgPtr);
		END_CAPTURE_OBJECT()
	);

	DeliveryEnhanceRelicTicket = DeliveryEnhanceRelicClass<FB2ResponseEnhanceRelicPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryEnhanceRelic, const FB2ResponseEnhanceRelicPtr& msgPtr)
			Capture->ResponseEnhanceRelic(msgPtr);
		END_CAPTURE_OBJECT()
	);

	DeliveryPromotionRelicTicket = DeliveryPromotionRelicClass<FB2ResponsePromotionRelicPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryPromotionRelic, const FB2ResponsePromotionRelicPtr& msgPtr)
			Capture->ResponsePromotionRelic(msgPtr);
		END_CAPTURE_OBJECT()
	);

	LobbyNoticePopUpTicket = LobbyNoticePopUpClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyNoticePopUp, int32 NoticeIndex)
			Capture->PopUpLobbyNotice(NoticeIndex);
		END_CAPTURE_OBJECT()
	);

	bEventsSubscribed = true;
}

void UB2UIManager_Lobby::UnsubscribeEvents()
{
	if (!bEventsSubscribed)
		return;

	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Unsubscribe(DJLegacy_ChangeLobbyUIPageTicket);
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Unsubscribe(DJLegacy_OpenLobbySubPopupTicket);
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Unsubscribe(DJLegacy_CloseLobbySubPopupTicket);
	LobbyPCSelectionChangedClass<EPCClass>::GetInstance().Unsubscribe(LobbyPCSelectionChangedTicket);
	LobbyUpdateAllItemIconsClass<>::GetInstance().Unsubscribe(LobbyUpdateAllItemIconsTicket);
	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Unsubscribe(LobbyTryUpdateSingleItemIconTicket);
	LobbyPopupCreateAccountClass<>::GetInstance().Unsubscribe(LobbyPopupCreateAccountTicket);
	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Unsubscribe(LobbyTryUpdateItemOPMainMenuTicket);
	LobbyInvenSetItemOpModeGuideTextClass<const FText&>::GetInstance().Unsubscribe(LobbyInvenSetItemOpModeGuideTextTicket);
	LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Unsubscribe(LobbyInvenHideItemOpModeGuideTextTicket);
	LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Unsubscribe(LobbyExternalSetInventoryTabTicket);
	CreateLobbyCharOnHeadDisplayClass<EPCClass, ASkeletalMeshActor*, ELobbyOnHeadDisplayType>::GetInstance().Unsubscribe(CreateLobbyCharOnHeadDisplayTicket);
	DestroyAllLobbyCharOnHeadDisplayClass<>::GetInstance().Unsubscribe(DestroyAllLobbyCharOnHeadDisplayTicket);
	UpdateLobbyCharOnHeadDisplayClass<>::GetInstance().Unsubscribe(UpdateLobbyCharOnHeadDisplayTicket);
	LobbyPlayUIEquipEffectClass<EItemEquipPlace>::GetInstance().Unsubscribe(LobbyPlayUIEquipEffectTicket);
	LobbyUpdateWingMainUIClass<>::GetInstance().Unsubscribe(LobbyUpdateWingMainUITicket);
	DeliveryGetQuestUIClass<FB2GetQuest>::GetInstance().Unsubscribe(DeliveryGetQuestUITicket);
	DeliveryAcceptQuestClass<FB2AcceptQuest>::GetInstance().Unsubscribe(DeliveryAcceptQuestTicket);
	DeliveryQuestRewardClass<Quest, TArray<b2network::B2RewardPtr>>::GetInstance().Unsubscribe(DeliveryQuestRewardTicket);
	DeliveryGetAccountRelicClass<FB2ResponseGetAccountRelicPtr>::GetInstance().Unsubscribe(DeliveryGetAccountRelicTicket);
	DeliveryEnhanceRelicClass<FB2ResponseEnhanceRelicPtr>::GetInstance().Unsubscribe(DeliveryEnhanceRelicTicket);
	DeliveryPromotionRelicClass<FB2ResponsePromotionRelicPtr>::GetInstance().Unsubscribe(DeliveryPromotionRelicTicket);
	LobbyNoticePopUpClass<int32>::GetInstance().Unsubscribe(LobbyNoticePopUpTicket);
	OnQuestCompleteNotifyClass<int32>::GetInstance().Unsubscribe(OnQuestCompleteNotifyTicket);

	bEventsSubscribed = false;
}


void UB2UIManager_Lobby::ClearLobbyUIPage()
{
	if (CurrentActiveUIPage != nullptr)
	{
		CurrentPageEnum = ELobbyUIPages::ELUP_End;

		CurrentActiveUIPage->DestroySelf();
		CurrentActiveUIPage = nullptr;

		for (auto& SubPopup : CreatedSubPopups)
		{
			if (auto* PopupWidget = SubPopup.Value)
			{
				PopupWidget->DestroySelf();

				if (Cast<IB2UIBackWidget>(PopupWidget))
				{
					B2AndroidBackManager::GetInstance()->RemoveBackWidget(PopupWidget);
				}
			}
		}

		CreatedSubPopups.Empty();
	}
}

void UB2UIManager_Lobby::DJLegacy_ChangeLobbyUIPage(ELobbyUIPages InPageEnum)
{

#if !UE_BUILD_SHIPPING
	FString PageEnumStr = GetUIPageEnumString(InPageEnum);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UIManager_Lobby::DJLegacy_ChangeLobbyUIPage %s"), *PageEnumStr));
#endif

	if (InPageEnum == ELobbyUIPages::ELUP_End)
	{
		ClearLobbyUIPage();
	}

	else
	{
		BII_CHECK(LobbyUIAssetInfo);

		TSubclassOf<UB2LobbyUserWidget>	WidgetClass = LobbyUIAssetInfo->GetWidgetClass(InPageEnum);
		OnLoadedLobbyWidget(InPageEnum, WidgetClass);	// Caching or Add to root

		auto* NewWidget = CreateWidget<UB2LobbyUserWidget>(CachedPlayerController, WidgetClass.Get());

		if (NewWidget != nullptr)
		{
			ClearLobbyUIPage();
			
			CurrentActiveUIPage = NewWidget;
			CurrentPageEnum = InPageEnum;

			NewWidget->AddToViewport(1);
			NewWidget->StartFromLobby(this, OwnerLobbyGM);

			if (GIsEditor == false)
				LobbyUIAssetInfo->UnloadTAssets(false, this);

			FName WidgetFName = GetDJLegacyUIName(NewWidget);
			UIWidgetOpenCompleteClass<FName>::GetInstance().Signal(WidgetFName);
		}
	}
}

void UB2UIManager_Lobby::DJLegacy_OpenLobbySubPopup(ELobbySubPopups InPopupEnum, FDJLegacySubpopupAddPanelInfo InOptionalAddPanelInfo)
{
	auto* ExistPoupWidget = DJLegacy_FindLobbySubPopup(InPopupEnum);
	if(ExistPoupWidget == nullptr)
	{
		BII_CHECK(LobbyUIAssetInfo);

		UClass* SubPopupClass = LobbyUIAssetInfo->GetSubPoupClass(InPopupEnum);
		auto* NewPopupWidget = CreateWidget<UB2LobbyUserWidget>(CachedPlayerController, SubPopupClass);

		if (NewPopupWidget)
		{
			CreatedSubPopups.Add(InPopupEnum, NewPopupWidget);

			if (!InOptionalAddPanelInfo.AddWidgetToThisInfo(NewPopupWidget))
			{ 
				// AddWidgetToThisInfo 가 성공하면 마치 별도로 전달된 Widget 및 Panel 아래의 UIP 처럼 되는 거.
				// 그게 아니면 그냥 Viewport 에 넣기. 이렇게 되면 완전 별도로 동작하는데 여타 다른 체계적으로 레이어링 된 UI 와 어긋나는 일이 발생할 수 있음.
				const int32 ZOrder = LobbyUIAssetInfo->GetSubPopupZOrder(InPopupEnum);
				NewPopupWidget->AddToViewport(ZOrder);
			}
			NewPopupWidget->StartFromLobby(this, OwnerLobbyGM); // 여기의 Init 격..

			if(Cast<IB2UIBackWidget>(NewPopupWidget))
			{
				B2AndroidBackManager::GetInstance()->AddBackWidget(NewPopupWidget);
			}
		}

		if (GIsEditor == false)
			LobbyUIAssetInfo->UnloadTAssets(false, this);
	}
}
void UB2UIManager_Lobby::DJLegacy_CloseLobbySubPopup(ELobbySubPopups InPopupEnum)
{
	CloseLobbySubPopup(InPopupEnum);
}

UB2LobbyUserWidget* UB2UIManager_Lobby::DJLegacy_FindLobbySubPopup(ELobbySubPopups InPopupEnum)
{
	UB2LobbyUserWidget** FoundOne = CreatedSubPopups.Find(InPopupEnum);
	if (FoundOne && *FoundOne) // LobbyUISwitcher 에서도 여기 목록을 참고하는데 굳이 거기로 갈 필요가 없지 낄낄
	{
		return (*FoundOne);
	}
	return NULL;
}

// 한번의 Request에 하나의 UIPage만 로드 가능 - 1Request : 1UI 만 지키면 여러개를 동시 요청해도 상관없음
void UB2UIManager_Lobby::ReqAsyncLoadUIPageAsset(ELobbyUIPages InLoadUIPage, bool bBlockingOpenUI/* = false*/)
{
	ReqAsyncLoadUIPageAssets(TArray<ELobbyUIPages> { InLoadUIPage }, bBlockingOpenUI);
}

bool UB2UIManager_Lobby::TryAsyncLoadOpenUI(ELobbyUIPages InLoadUIPage, bool bBlockingOpenUI)
{
	if (bBlockingOpenUI) // Async 요청 대기 후 Callback에서 OpenUI. 대기중임을 나타내는 UI 표시.
	{
		ReqAsyncLoadUIPageAsset(InLoadUIPage, bBlockingOpenUI);
	}

	else // Synchronous Open
	{
		const auto HeroMgmtMode = LobbyUIPageToHeroMgmtSubMode(InLoadUIPage);
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(HeroMgmtMode);
	}
	
	return true;
}

void UB2UIManager_Lobby::CloseLobbySubPopup(ELobbySubPopups InPopupEnum)
{
	if (auto* PopupWidget = DJLegacy_FindLobbySubPopup(InPopupEnum))
	{
		if (Cast<IB2UIBackWidget>(PopupWidget))
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(PopupWidget);
		}
		
		PopupWidget->DestroySelf();
		CreatedSubPopups.Remove(InPopupEnum);
	}
}

void UB2UIManager_Lobby::OpenItemDetailPopup(int32 InUID)
{
	auto* InvenPage = Cast<UB2LobbyUI_InventoryMain>(CurrentActiveUIPage);

	if (InvenPage)
	{
		auto* InItem = InvenPage->FindSingleItemIcon(InUID);

		if (InItem)
		{
			InItem->OnClickedBTNArea();
		}
	}
}

void UB2UIManager_Lobby::ReqAsyncLoadUIPageAssets(const TArray<ELobbyUIPages>& InLoadUIPages, bool bBlocking)
{
	if (InLoadUIPages.IsValidIndex(0) == false)
		return;

	if (auto* LobbySwitcher = GetLobbyUIAssetInfo())
	{
		ELobbyUIPages RequestUI = InLoadUIPages[0];
		auto ClassAsset = LobbySwitcher->GetWidgetClassAsset(RequestUI);

		const FString	UIPageName = GetAsyncRequestName(RequestUI, bBlocking);
		const uint32	UIHash = static_cast<uint32>(RequestUI);

		FB2AsyncRequestInfo RequestInfo(UIHash, ClassAsset.ToSoftObjectPath());
		TArray<FB2AsyncRequestInfo> RequestInfoVar{ RequestInfo };

		if (ClassAsset.IsValid())
			OnAsyncLoadComplete(UIPageName, RequestInfoVar);

		else
			TryAsyncLoad(UIPageName, RequestInfoVar, bBlocking);
	}
}

void UB2UIManager_Lobby::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList)
{
	if (CompleteRequestList.IsValidIndex(0))
	{
		const auto& RequestInfo = CompleteRequestList[0];

		const auto LoadedUIPage = static_cast<ELobbyUIPages>(RequestInfo.AssetKey);
		TSubclassOf<UB2LobbyUserWidget> LoadedClass = Cast<UClass>(RequestInfo.StringAsset.ResolveObject());

		if (LoadedUIPage != ELobbyUIPages::ELUP_End && LoadedClass.Get() != nullptr)
			OnLoadedLobbyWidget(LoadedUIPage, LoadedClass);

		if (IsOpenUIRequest(CompleteRequest))
		{
			const auto HeroMgmtMode = LobbyUIPageToHeroMgmtSubMode(LoadedUIPage);
			LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(HeroMgmtMode);
		}

		ReleaseAsyncLoad(CompleteRequest);
	}
}

void UB2UIManager_Lobby::OnLoadedLobbyWidget(ELobbyUIPages UIPage, TSubclassOf<UB2LobbyUserWidget>& LoadedClass)
{
	const bool IsValid = LobbyUIAssetInfo && LoadedClass.Get() != nullptr;
	if (IsValid)
	{
		if (const auto* PageInfo = LobbyUIAssetInfo->GetUIPageMapping(UIPage))
		{
			if (PageInfo->ShouldCacheOnLoad())
				CachedLobbyUserWidgetClasses.Add(UIPage, LoadedClass);

#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 는 메모리 우려로 인해 이런 거 하지 않는다..
			if (UB2LobbyUISwitcher::IsRootSetPage(UIPage))
				LoadedClass->AddToRoot();
#endif
		}
	}
}

bool UB2UIManager_Lobby::IsOpenUIRequest(const FString& RequestName)
{
	return RequestName.Contains(UB2UIManager_Lobby::AsyncOpenUIStr);
}

FString UB2UIManager_Lobby::GetAsyncRequestName(ELobbyUIPages UIPage, bool bOpenUI)
{
	FString	UIPageName = GetUIPageEnumString(UIPage);
	
	if (bOpenUI)
		UIPageName = UB2UIManager_Lobby::AsyncOpenUIStr + UIPageName;

	return UIPageName;
}

void UB2UIManager_Lobby::RemoveCachedWidgetClasse(ELobbyUIPages UIPageEnum)
{
	if (CachedLobbyUserWidgetClasses.Find(UIPageEnum))
		CachedLobbyUserWidgetClasses.Remove(UIPageEnum);
}

void UB2UIManager_Lobby::RemoveCachedSubWidgetClasses()
{
	CachedInvenStoredItemIconClass = nullptr;
	CachedInvenEquippedItemIconClass = nullptr;
}

void UB2UIManager_Lobby::RemoveCachedWidgetClasses()
{
	CachedLobbyUserWidgetClasses.Empty();
}

UClass* UB2UIManager_Lobby::GetInventoryItemIconClass(bool bIsForEquipped)
{
	if (LobbyUIAssetInfo)
		return LobbyUIAssetInfo->GetInventoryItemIconClass(bIsForEquipped);

	return nullptr;
}

void UB2UIManager_Lobby::InitializeDynItemIconWidgetPool()
{
	EquippedItemIconWidgetPool.PreInitSetType(EInventoryDynItemIconPoolClassType::EquippedItem);
	// FloatingWidgetPool 마냥 PreCreate 해 놓을 필요는 없을 듯
	EquippedItemIconWidgetPool.InitPool(this, CachedPlayerController, 0, 0); 

	StoredItemIconWidgetPool.PreInitSetType(EInventoryDynItemIconPoolClassType::StoredItem);
	StoredItemIconWidgetPool.InitPool(this, CachedPlayerController, 0, 0);
}
void UB2UIManager_Lobby::DestroyDynItemIconWidgetPool()
{
	EquippedItemIconWidgetPool.DestroyAll();
	StoredItemIconWidgetPool.DestroyAll();
}
FInventoryDynItemIconPool& UB2UIManager_Lobby::GetMatchingDynItemIconWidgetPool(bool bIsForEquipped)
{
	return bIsForEquipped ? EquippedItemIconWidgetPool : StoredItemIconWidgetPool;
}
UB2DynItemIcon* UB2UIManager_Lobby::RentInvenDynItemIconWidget(bool bIsForEqupped, APlayerController* InPC, UB2UnitedWidgetBase* InRentOwner, UPanelWidget* InIconPlacePanel)
{
	FInventoryDynItemIconPool& DesiredPool = GetMatchingDynItemIconWidgetPool(bIsForEqupped);
	return Cast<UB2DynItemIcon>(DesiredPool.Rent(InPC, InRentOwner, InIconPlacePanel));
}
void UB2UIManager_Lobby::TurnInDynItemIconWidget(bool bIsForEqupped, UB2DynItemIcon* InUsedOne)
{
	FInventoryDynItemIconPool& DesiredPool = GetMatchingDynItemIconWidgetPool(bIsForEqupped);
	DesiredPool.TurnIn(InUsedOne);
}

void UB2UIManager_Lobby::NotifyPCSelectionChanged(EPCClass InSelectedPCClass)
{
	
}

void UB2UIManager_Lobby::TryUpdateAllVisibleItemIcons()
{
	UB2LobbyUI_InventoryMain* InvenPage = Cast<UB2LobbyUI_InventoryMain>(CurrentActiveUIPage);
	if (InvenPage)
	{ // 걍 아이템 아이콘 전체 업데이트 하는 것. 아이템 상태가 바뀐 다양한 경우에 사용하기 위함. 물론 매 프레임 사용하는 식은 아니고.
		InvenPage->UpdateAllItemIconsForCurrentSelection();
	}
	UB2LobbyUI_AutoEquip* AutoEquipPopup = Cast<UB2LobbyUI_AutoEquip>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_AutoEquip));
	if (AutoEquipPopup) // 자동 장착 창이 있다면 거기도 업데이트
	{
		AutoEquipPopup->UpdateOnEquipChange();
	}
	// 이건 좀 특수 케이스긴 한데 스탯 창이 열려 있는 와중에 장착 상태 변경 시 스탯 갱신을 위함. 
	UB2LobbyUI_CharStatDetail* CharStatPopup = Cast<UB2LobbyUI_CharStatDetail>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_CharStatDetail));
	if (CharStatPopup)
	{
		CharStatPopup->UpdateStats();
	}
}

void UB2UIManager_Lobby::TryUpdateSingleVisibleItemIcon(int64 ItemInstanceUID)
{
	UB2LobbyUI_InventoryMain* InvenPage = Cast<UB2LobbyUI_InventoryMain>(CurrentActiveUIPage);
	if (InvenPage)
	{
		InvenPage->TryUpdateSingleItemIconOfID(ItemInstanceUID);
	}

	// 아이템 세부 정보 창도.. 예를 들어 잠금 상태 업데이트 같은 경우 응답이 온 후 여길 통해 UI 업데이트를 함.
	// 보관함이랑 장착장비 쪽 둘 다 이게 떠 있을 수 있으므로 둘 다 검사해서 InstanceID 맞는 걸 업데이트 한다.
	UB2LobbyUI_ItemDetail* ItemDetailPopup_Stored = Cast<UB2LobbyUI_ItemDetail>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_StoredItemDetail));
	UB2LobbyUI_ItemDetail* ItemDetailPopup_Equipped = Cast<UB2LobbyUI_ItemDetail>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_EquippedItemDetail));

	UB2LobbyUI_ItemDetail* FinalUpdateItemDetailPopup = NULL;
	if (ItemDetailPopup_Stored && ItemDetailPopup_Stored->GetDisplayItemUID() == ItemInstanceUID) {
		FinalUpdateItemDetailPopup = ItemDetailPopup_Stored;
	}
	else if (ItemDetailPopup_Equipped && ItemDetailPopup_Equipped->GetDisplayItemUID() == ItemInstanceUID) {
		FinalUpdateItemDetailPopup = ItemDetailPopup_Equipped;
	}

	if (FinalUpdateItemDetailPopup)
	{
		FB2Item UpdatedItemData;
		if (UB2LobbyInventory::FindFromAll(UpdatedItemData, ItemInstanceUID))
		{
			FinalUpdateItemDetailPopup->UpdateItemData(UpdatedItemData);
		}
	}
}

void UB2UIManager_Lobby::TryPopupCreateAccountMenu()
{ // 임시 구현임. 테스트가 필요한 부분이라.

	/* //UIManager쪽 체계와 붙히는 작업중...
	UB2LobbyUI_LoginMenu* LoginPage = Cast<UB2LobbyUI_LoginMenu>(CurrentActiveUIPage);
	if (LoginPage)
	{
		LoginPage->PopupCreateAccountMenu();
	}
	*/

	auto LoginUI = UB2UIManager::GetInstance()->GetUI<UB2UILogin>(UIFName::LogIn);
	if (LoginUI)
	{
		LoginUI->PopupCreateAccountMenu();
	}
}

void UB2UIManager_Lobby::TryUpdateItemOPMainMenu()
{
	// 모드별로 열려 있는 창이 다를 것.
	auto* LevelupPopup = Cast<UB2LobbyUI_ItemLevelupMain>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemLevelUpMain));
	if (LevelupPopup)
	{
		LevelupPopup->UpdateOnLevelupStateChange();
	}

	auto* UpgradePopup = Cast<UB2LobbyUI_ItemUpgradeMain>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemUpgradeMain));
	if (UpgradePopup)
	{
		UpgradePopup->UpdateOnUpgradeStateChange();
	}

	auto* ComposePopup = Cast<UB2LobbyUI_ItemComposeMain>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemComposeMain));
	if (ComposePopup)
	{
		ComposePopup->UpdateOnComposeStateChange();
	}

	auto* SurpassPopup = Cast<UB2LobbyUI_ItemSurpassMain>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemSurpassMain));
	if (SurpassPopup)
	{
		SurpassPopup->UpdateOnSurpassStateChange();
	}

	auto* EnhancePopup = Cast<UB2LobbyUI_ItemEnhance>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemEnhanceMain));
	if (EnhancePopup)
	{
		EnhancePopup->UpdateOnEnhanceStateChange();
	}

	auto* EnhanceChangePopup = Cast<UB2LobbyUI_ItemEnhanceChangeMain>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemEnhanceChangeMain));
	if (EnhanceChangePopup)
	{
		EnhanceChangePopup->UpdateStateChange();
	}

	UB2LobbyUI_ItemAfterOption* AfterOptionPopup = Cast<UB2LobbyUI_ItemAfterOption>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemAfterOpiton));
	if (AfterOptionPopup)
	{
		AfterOptionPopup->UpdateOnInfoStateChange();
	}
}

void UB2UIManager_Lobby::SetInventoryItemOpModeGuideText(const FText& InText)
{
	UB2LobbyUI_InventoryMain* InvenPage = Cast<UB2LobbyUI_InventoryMain>(CurrentActiveUIPage);
	if (InvenPage)
	{
		InvenPage->SetItemOpModeGuideText(InText);
	}
}

void UB2UIManager_Lobby::HideInventoryItemOpModeGuideText()
{
	UB2LobbyUI_InventoryMain* InvenPage = Cast<UB2LobbyUI_InventoryMain>(CurrentActiveUIPage);
	if (InvenPage)
	{
		InvenPage->HideItemOpModeGuideText();
	}
}

EItemInvenType UB2UIManager_Lobby::GetInventoryTabSelection()
{
	UB2LobbyUI_InventoryMain* InvenPage = Cast<UB2LobbyUI_InventoryMain>(CurrentActiveUIPage);

	if (InvenPage == nullptr)
		return EItemInvenType::EIIVT_End;

	//// Note : 아이템 강화같이 장비(무기,방어구)창에서 강제적으로 소모품창을 열어줬을경우 다시 장비(무기,방어구)창을 이동하기위한 용도
	//EItemInvenType CustomInvenTab = InvenPage->GetCustomOldInvenTab();
	//if (CustomInvenTab != EItemInvenType::EIIVT_End)
	//	return CustomInvenTab;

	return InvenPage->GetCurrentInvenTabSelection();
}

void UB2UIManager_Lobby::ExternalSetInventoryTab(EItemInvenType InNewTab, bool bDisableOtherTabSelection /*= false*/)
{
	UB2LobbyUI_InventoryMain* InvenPage = Cast<UB2LobbyUI_InventoryMain>(CurrentActiveUIPage);
	if (InvenPage)
	{
		InvenPage->NativeSetInvenTabSelection(InNewTab);
		
		if (bDisableOtherTabSelection){
			InvenPage->SetEnableOnlyOneTabType(InNewTab); // InNewTab 외의 다른 선택 못하게 함. 물론 따로 풀어주어야.
		}
		else{
			InvenPage->SetEnableAllTabButtons(true); // 걍 bDisableOtherTabSelection 을 false 로 넘겨주면 다 푸는 걸로..
		}
	}
}

UB2FloatingLobbyOnHeadDisplay* UB2UIManager_Lobby::CreateLobbyCharOnHeadDisplay(EPCClass InCharacerClass, class ASkeletalMeshActor* InLobbyChar, ELobbyOnHeadDisplayType InMasterType)
{
#if RESTORE_BII_LOBBY_ONHEAD_DISPLAY
	if (OnHeadDisplayWidgetClass && InLobbyChar)
	{
		UB2FloatingLobbyOnHeadDisplay* ThisCreated = CreateWidget<UB2FloatingLobbyOnHeadDisplay>(CachedPlayerController, OnHeadDisplayWidgetClass);
		if (ThisCreated)
		{
			ThisCreated->AddToViewport(BII_WIDGET_ZORDER_LOBBYONHEADDISPLAY);
			ThisCreated->SetMasterType(InMasterType);
			ThisCreated->SetOwnerInfo(InCharacerClass, InLobbyChar);
			CreatedOnHeadDisplays.Add(ThisCreated);
			return ThisCreated;
		}
	}
#endif
	return NULL;
}

void UB2UIManager_Lobby::DestroyAllLobbyCharOnHeadDisplay()
{
	//for (UB2FloatingLobbyOnHeadDisplay* ThisOnHead : CreatedOnHeadDisplays)
	//{
	//	if (ThisOnHead && !ThisOnHead->IsPendingKillOrUnreachable() && !ThisOnHead->HasAnyFlags(EObjectFlags::RF_BeginDestroyed))
	//	{
	//		ThisOnHead->DestroySelf();
	//	}
	//}
	//CreatedOnHeadDisplays.Empty();
}

void UB2UIManager_Lobby::UpdateLobbyCharOnHeadDisplay()
{
	//for (UB2FloatingLobbyOnHeadDisplay* ThisOnHead : CreatedOnHeadDisplays)
	//{
	//	if (ThisOnHead && !ThisOnHead->IsPendingKillOrUnreachable() && !ThisOnHead->HasAnyFlags(EObjectFlags::RF_BeginDestroyed))
	//	{
	//		ThisOnHead->UpdateOnMasterTypeChange();
	//	}
	//}
}

void UB2UIManager_Lobby::PlayUIEquipEffect(EItemEquipPlace InEquippedPlace)
{
	UB2LobbyUI_InventoryMain* InvenPage = Cast<UB2LobbyUI_InventoryMain>(CurrentActiveUIPage);
	if (InvenPage)
	{
		InvenPage->PlayEquipEffect(InEquippedPlace);
	}
}

void UB2UIManager_Lobby::UpdateWingMainUI()
{
	UB2LobbyUI_WingMain* WingMainPage = Cast<UB2LobbyUI_WingMain>(CurrentActiveUIPage);
	if (WingMainPage)
	{
		WingMainPage->UpdateByCurrentWingData();
	}
	// 이것도 열려 있을지 모르니.. 목적상 이것도 업데이트가 필요..
	UB2LobbyUI_WingPropOptionEnhance* WingPropOptionEnhancePopup = Cast<UB2LobbyUI_WingPropOptionEnhance>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_WingPropOptionEnhance));
	if (WingPropOptionEnhancePopup)
	{
		WingPropOptionEnhancePopup->UpdateByCurrentWingData();
	}

//	BladeIIGameImpl::GetRedDotManager().UpdatedUIData(EUICategoryMarkForNew::Wing);
}

void UB2UIManager_Lobby::BeginShowWingEvolutionFailMessage()
{
	UB2LobbyUI_WingMain* WingMainPage = Cast<UB2LobbyUI_WingMain>(CurrentActiveUIPage);
	if (WingMainPage)
	{
		WingMainPage->BeginShowEvolutionFailMessage();
	}
}

void UB2UIManager_Lobby::BeginShowWingEvolutionSuccessMessage()
{
	UB2LobbyUI_WingMain* WingMainPage = Cast<UB2LobbyUI_WingMain>(CurrentActiveUIPage);
	if (WingMainPage)
	{
		WingMainPage->BeginShowWingEvolutionSuccessMessage();
	}
}

void UB2UIManager_Lobby::PlayWingEnhancedAnim()
{
	UB2LobbyUI_WingMain* WingMainPage = Cast<UB2LobbyUI_WingMain>(CurrentActiveUIPage);
	if (WingMainPage)
	{
		WingMainPage->PlayOnEnhancedAnim();
	}
}

void UB2UIManager_Lobby::UpdateOnWingAutoEnhanceComplete()
{
	UB2LobbyUI_WingMain* WingMainPage = Cast<UB2LobbyUI_WingMain>(CurrentActiveUIPage);
	if (WingMainPage)
	{
		WingMainPage->UpdateOnAutoEnhanceComplete();
	}
}

void UB2UIManager_Lobby::UpdateOnWingAutoEvolveComplete()
{
	UB2LobbyUI_WingMain* WingMainPage = Cast<UB2LobbyUI_WingMain>(CurrentActiveUIPage);
	if (WingMainPage)
	{
		WingMainPage->UpdateOnAutoEvolveComplete();
	}
}

void UB2UIManager_Lobby::PopUpLobbyNotice(int32 NoticeIndex)
{
	if (NoticeIndex == 0)
	{
		// 만약 notice가 더 연결되어야 한다면 상태를 모두 받고 특정 시점에서 일괄 처리해야할듯.
		if (BladeIIGameImpl::GetClientDataStore().GetAttendanceInfo().bIsAttendance || BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_EventAttendance())
		{
			if (UB2UIManager* UIMgrInst = UB2UIManager::GetInstance())
			{
				if (auto* AttendanceMainWidget = UIMgrInst->GetUI<UB2UIAttendanceMain>(UIFName::AttendanceMain))
				{
					AttendanceMainWidget->Init();
				}
				UIMgrInst->OpenUI<UB2UIAttendanceMain>(UIFName::AttendanceMain);
				AttendanceNoticeState = NoticeState::DISABLE;
			}
		}
	}
	else if(NoticeIndex == 1)
	{
		if (QuestNoticeState == NoticeState::ENABLE)
		{
			if (UB2UILobbyMain* LobbyMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain))
			{
				LobbyMainWidget->ShowQuestNotifyDialog(QuestNoticeList);
				QuestNoticeList.Reset();
				QuestNoticeState = NoticeState::DISABLE;
			}
		}
	}
	else
	{
		LobbyCharPlayMatineeClass<float>::GetInstance().Signal(0.01f);
	}
}

void UB2UIManager_Lobby::ResponseQuestComplete(int32 QuestSlot)
{
	UB2UILobbyMain* LobbyMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	if (LobbyMainWidget != nullptr)
	{
		LobbyMainWidget->UpdateSingleQuestDisplay(QuestSlot);
	}
}

void UB2UIManager_Lobby::ResponseGetQuest(const FB2GetQuest& Quest)
{
//	QuestNoticeList = GET_TUPLE_DATA(FB2ResponseGetQuest::quest_infos_index, Quest);
//	bool bDailyInitialized = GET_TUPLE_DATA(FB2ResponseGetQuest::daily_quest_initialized_index, Quest);
//
//	// Sorting 해서 메인퀘가 무조건 앞으로 오도록
//	QuestNoticeList.Sort([](const b2network::B2QuestInfoPtr Left, const b2network::B2QuestInfoPtr Right) {
//		return Left->slot < Right->slot; });
//
//	UB2UILobbyMain* LobbyMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
//	if (LobbyMainWidget != nullptr)
//	{
//		LobbyMainWidget->UpdateQuestDisplay(QuestNoticeList);
//		if (!bDailyInitialized)
//			QuestNoticeList.Reset();
//		QuestNoticeState = bDailyInitialized ? NoticeState::ENABLE : NoticeState::DISABLE;
//	}
//
//	if (auto* QuestDialogUI = UB2UIManager::GetInstance()->GetUI<UB2UIQuestDialog>(UIFName::QuestDialog))
//	{
//		QuestDialogUI->UpdateDialog();
//		
//	}
//
//	if (IsOpenDailyQeustDialog == true)
//	{
//		IsOpenDailyQeustDialog = false;
//		QuestPtr QuestPtrInfo = QuestManager::GetInstance().GetQuest(b2network::B2QuestSlotType::SUB1);
//		if (QuestPtrInfo != nullptr)
//		{
//			if (QuestPtrInfo->GetState() != EQS_REWARDED)
//			{
//				if (auto* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog))
//				{
//					QuestDialogUI->OpenDialogBySlotIndex(b2network::B2QuestSlotType::SUB1);
//				}
//			}
//		}
//	 }
}

void UB2UIManager_Lobby::ResponseAcceptQuest(const FB2AcceptQuest& Quest)
{
	//const b2network::B2QuestInfoPtr	AcceptQuest = GET_TUPLE_DATA(FB2ResponseAcceptQuest::quest_info_index, Quest);
	//UE_LOG(LogBladeII, Log, TEXT("AcceptQuest %d"), AcceptQuest->slot);

	//UB2UILobbyMain* LobbyMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//if (LobbyMainWidget != nullptr && AcceptQuest != nullptr)
	//{
	//	LobbyMainWidget->UpdateSingleQuestDisplay(AcceptQuest->slot);

	//	switch (AcceptQuest->type)
	//	{
	//	case MAIN_QUEST:
	//		{
	//			UB2QuestInfoTable* QuestTable = StaticFindQuestTable();
	//			FQuestCommonInfo* CommonTable = ((AcceptQuest->type == MAIN_QUEST) ? QuestTable->GetQuestMainInfo(AcceptQuest->quest_id) : QuestTable->GetQuestDailyInfo(AcceptQuest->quest_id));

	//			if (CommonTable != nullptr)
	//			{
	//				FB2UILinkManager::LinkUIScene(CommonTable->LinkScene, CommonTable->Optional1, CommonTable->Optional2);
	//			}
	//			
	//		}
	//		break;
	//	case DAILY_QUEST:
	//		{
	//			// 일일퀘스트의 경우 로비 메인화면에서 퀘스트 버튼 클릭으로 요청 한다. (다이얼로그를 띄워주자)
	//			if (auto* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog))
	//				QuestDialogUI->OpenDialogBySlotIndex(AcceptQuest->slot);
	//		}
	//		break;
	//	}		
	//}
}

void UB2UIManager_Lobby::ResponseGetQuestReward(const Quest& CompleteQuest, const TArray<b2network::B2RewardPtr>& Reward)
{
	ToViewReward = Reward;
	UB2UILobbyMain* LobbyMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	UB2UIMission* MIssionWidget = UB2UIManager::GetInstance()->GetUI<UB2UIMission>(UIFName::Mission);

	if (LobbyMainWidget != nullptr || MIssionWidget != nullptr)
	{
		if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
		{
			if (auto* RewardMailPopUp = UB2UIManager::GetInstance()->OpenRewardMailPopUp(Reward))
			{
				if (CompleteQuest.GetType() == MAIN_QUEST)
				{
					RewardMailPopUp->SetConfirmCallback(FOnQuestDialogClosed::CreateLambda([this]() { this->OnClickMsgReward(b2network::B2QuestSlotType::MAIN2);}));
				}
				else
				{
					IsOpenDailyQeustDialog = true;
					RewardMailPopUp->SetConfirmCallback(FOnQuestDialogClosed::CreateLambda([this]() { data_trader::Retailer::GetInstance().RequestGetQuest();}));
				}
			}
			else if(auto* RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
			{

				RewardPopup->AddRewardItems(Reward);

				// 뽑기권은 수량 전달에 대한 논의가 끝난 후 적용
				if (CompleteQuest.GetType() == MAIN_QUEST)
				{
					RewardPopup->SetConfirmCallback(FOnQuestDialogClosed::CreateLambda([this](){ this->OnClickMsgReward(b2network::B2QuestSlotType::MAIN2); BladeIIGameImpl::GetClientDataStore().ResponseGetQuestReward(this->ToViewReward); }));
				}
				else
				{
					RewardPopup->SetConfirmCallback(FOnQuestDialogClosed::CreateLambda([this](){ data_trader::Retailer::GetInstance().RequestGetQuest(); BladeIIGameImpl::GetClientDataStore().ResponseGetQuestReward(this->ToViewReward); }));
				}
				
				RewardPopup->ShowRewardPopup();
			}
		}
	}
}

void UB2UIManager_Lobby::OnClickMsgReward(int32 QuestSlot)
{
	// 캐쉬된 퀘스트를 지워주고, 서버로부터 새로 갱신받는다.
	QuestManager &Manager = QuestManager::GetInstance();
	Manager.FlushCachedCompleteQuest(QuestSlot);
	
	QuestPtr QuestPtrInfo = Manager.GetQuest(QuestSlot);

	if (QuestPtrInfo != nullptr && QuestPtrInfo->GetState() != EQS_REWARDED)
	{
		if (auto* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog))
		{
			// 만약 대화형같은 미리 완료로 되있을 경우 Auto Clear 상태로 만들어 주어 대화후 보상받기로 넘어가게 한다.
			if (QuestPtrInfo->GetState() == EQS_COMPLETED)
			{
				QuestPtrInfo->UpdateState(EQS_PROGRESS);
				Manager.SetAutoClearQuest(QuestPtrInfo->GetQuestSlot(), QuestPtrInfo->GetQuestId());
			}

			QuestDialogUI->OpenDialogBySlotIndex(QuestSlot);
			// 팝업 종료 후 퀘스트 갱신
			QuestDialogUI->SetDialogCloseDelegate(FOnQuestDialogClosed::CreateLambda([this, QuestSlot]() {
				this->ResponseQuestComplete(QuestSlot);
			}));
		}
	}
	else
	{
		ResponseQuestComplete(QuestSlot);
	}
}

FName UB2UIManager_Lobby::GetDJLegacyUIName(UB2UnitedWidgetBase* WidgetBase)
{
	if (WidgetBase)
	{
		FName WidgetFName = WidgetBase->GetFName();
		FString WidgetString = WidgetBase->GetFName().ToString();
		const int32 SubfixIndex = WidgetString.Find(TEXT("_C"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		if (SubfixIndex != INDEX_NONE)
		{
			return FName(*WidgetString.Left(SubfixIndex));
		}
	}

	return FName(TEXT("None"));
}

UB2UnitedWidgetBase* UB2UIManager_Lobby::GetCurrActiveUI()
{
	return CurrentActiveUIPage;
}

void UB2UIManager_Lobby::ResponseGetAccountRelic(const FB2ResponseGetAccountRelicPtr& msgPtr)
{
	UB2LobbyUI_RelicMain* RelicPage = Cast<UB2LobbyUI_RelicMain>(CurrentActiveUIPage);
	if (RelicPage)
	{
		RelicPage->UpdateList();
	}
}

void UB2UIManager_Lobby::ResponseEnhanceRelic(const FB2ResponseEnhanceRelicPtr& msgPtr)
{
	// 남은 영웅의 조각 인벤토리 갱신
	const FMD_AncientRelicLevelElem* LevelUpInfo = GLOBALRELICMANAGER.getRelicLevelInfo(msgPtr->relic->relic_id, msgPtr->relic->enhance_level);
	if (!LevelUpInfo) return;

	BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE,
																			LevelUpInfo->nNeed_HeroPiece,
																			msgPtr->changed_hero_piece_items,
																			msgPtr->deleted_hero_piece_item_ids);
	// 남은 돈, 영웅의조각 갱신
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, msgPtr->current_money);
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::HeroPiece);

	UB2LobbyUI_RelicManagement* RelicManagement = Cast<UB2LobbyUI_RelicManagement>(CurrentActiveUIPage);
	if (RelicManagement)
		RelicManagement->OnResponseRelicLevelUp(msgPtr->relic->enhance_level);
	
	//////////////////////////////////////////////////////////////////
	//액션 지표 로그 (캐릭터 고대 유물 강화)
	const EPCClass PCClass(SvrToCliPCClassType(msgPtr->relic->character_type));
	FString itemName(TEXT("ITEM_ENHANCE_NONE"));
	UB2ItemInfo* IteminfoTable(StaticFindItemInfo());
	itemName = BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, "Relic_Type" + FString::FromInt(msgPtr->relic->relic_id)).ToString().ToUpper();

	B2ActionLogSender Sender;
	Sender.setCategory(FString("CHARACTER"))
		  .setAction(FString("RELIC_ENHANCE"))
		  .setLabel(B2ActionLogType<EPCClass>::GetName(PCClass))
		  .setActionAttr1(itemName)
		  .LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//////////////////////////////////////////////////////////////////
}

void UB2UIManager_Lobby::ResponsePromotionRelic(const FB2ResponsePromotionRelicPtr& msgPtr)
{
	// 남은 돈 갱신
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, msgPtr->current_money);

	// 유물조각 갱신
	const FMD_AncientRelicGradeElem* GradeUpInfo = GLOBALRELICMANAGER.getRelicGradeInfo(msgPtr->relic->relic_id, msgPtr->relic->grade);
	if (!GradeUpInfo) return;
	BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(FItemRefIDHelper::GetBossPieceRefIDFromRelicID(msgPtr->relic->relic_id), 
																			GradeUpInfo->nPromotion_BossPiece,
																			msgPtr->changed_boss_piece_items, 
																			msgPtr->deleted_boss_piece_item_ids);

	UB2LobbyUI_RelicManagement* RelicManagement = Cast<UB2LobbyUI_RelicManagement>(CurrentActiveUIPage);
	if (RelicManagement)
		RelicManagement->OnResponseRelicGradeUp(msgPtr->relic->grade);

}
