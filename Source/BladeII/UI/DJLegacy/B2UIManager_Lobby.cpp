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
	// DJLegacy UI ���� ȣȯ�� ���� EUIScene �� �־�� ���� ��Ʈ�� ����
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
	// ParentPanel �� Ư�� Ÿ�Ը� ����ϵ��� �� ����..
	if (InCreatedWidget && OwnerWidget && ParentPanel)
	{
		UPanelSlot* AddeddSlot = ParentPanel->AddChild(InCreatedWidget);
		UCanvasPanelSlot* CastedCPSlot = Cast<UCanvasPanelSlot>(AddeddSlot);
		// Slot ������ �ʿ信 ���� �پ��ϰ� �ؾ� �� ����..
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

/** ���ſ� �κ� ĳ���� �Ӹ� ���� �յ� �� �ִ� widget �ε� ���� ��� ���ŵǾ� ������ Ȥ�� ���� LobbySceneManager �ʿ� ����ϴ� �ڵ带 ���ܵ� �� ��
 * ��, ���⼭ ���ҽ� �ε��ϰ� CreateWidget �ϰ� �̷� �͵��� �ѳ� ����� �� define ���� �� ��. */
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
	if (!GMinimalDLCFrontMode){ // DLC Front ��� ���ҽ��ε� �ִ��� ����
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
	// UObject �ı��� BeginDestroy �� �ƴ϶� ���Ⱑ �����ϴ�.

	if (LobbyUIAssetInfo)
		LobbyUIAssetInfo->UnloadTAssets(true, this);

	DestroyAllLobbyCharOnHeadDisplay();
	DestroyDynItemIconWidgetPool();
}

bool UB2UIManager_Lobby::InitUIManager(AB2LobbyGameMode* InLobbyGM)
{
//	if (GMinimalDLCFrontMode){
//		return false; // DLC Front ��� ���ҽ��ε� �ִ��� ����
//	}
//
//	// Anything necessary..
//	// ���� ����ؼ� �޸� �����ϴ� ���� �ִٸ� ���⼭ �����ϰ� �ǰڴ�.
//
//	OwnerLobbyGM = InLobbyGM;
//	check(OwnerLobbyGM);
//
//	CachedPlayerController = UGameplayStatics::GetLocalPlayerController(OwnerLobbyGM);
//	check(CachedPlayerController);
//
//	// UISwitcher. �������Ʈ Ŭ���� �����ͼ� Default Object �δٰ�
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
//			FString::Printf(TEXT("[Warning] LobbyUISwitcher �� ã�� ����. ��ǻ�Ͱ� �� �����Ѵ�."))
//			));
//		return false;
//	}
//#endif
//	
//	InitializeDynItemIconWidgetPool();
//
//	SubscribeEvents();
//
//	/* //UIManager�� ü��� ������ �۾���...
//	// �ϴ� �α�â���� ����. �� ���Ĵ� GameMode ���� DJLegacy_ChangeLobbyUIPage �� ����ϰ� �� ��.
//	DJLegacy_ChangeLobbyUIPage(ELobbyUIPages::ELUP_Login);
//	*/
//
//	//���� �κ��κ� ������ B2UIManager_Lobby�� ���ܵδ� ���̰� ���� �����Ͽ��� �� -> DJLegacy �� ���� �縮������ ���� ����.
//	UB2UIManager::GetInstance()->ClearUISceneHistory();	
//	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CharacterIntro); // �ϴ� CharacterIntro �� �����ߴٰ�.. �������� ���ƿ��� ��Ȳ�̶�� �ٸ� �ɷ� �ٲٰ� �� ��.
	return true;
}

void UB2UIManager_Lobby::UpdateManual(float DeltaSeconds)
{
	for (UB2FloatingLobbyOnHeadDisplay* ThisOnHead : CreatedOnHeadDisplays)
	{
		if (ThisOnHead)
		{
			ThisOnHead->UpdatePosition(); // �� �ٺ����� ������ �־ �̷��� ���� ������Ʈ �� �ش�. ó�� �����ÿ��� ����� ������ �ȵǴ� �������ٰ� NativeTick �� �� �Ҹ��� ��.
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
				// AddWidgetToThisInfo �� �����ϸ� ��ġ ������ ���޵� Widget �� Panel �Ʒ��� UIP ó�� �Ǵ� ��.
				// �װ� �ƴϸ� �׳� Viewport �� �ֱ�. �̷��� �Ǹ� ���� ������ �����ϴµ� ��Ÿ �ٸ� ü�������� ���̾ �� UI �� ��߳��� ���� �߻��� �� ����.
				const int32 ZOrder = LobbyUIAssetInfo->GetSubPopupZOrder(InPopupEnum);
				NewPopupWidget->AddToViewport(ZOrder);
			}
			NewPopupWidget->StartFromLobby(this, OwnerLobbyGM); // ������ Init ��..

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
	if (FoundOne && *FoundOne) // LobbyUISwitcher ������ ���� ����� �����ϴµ� ���� �ű�� �� �ʿ䰡 ���� ����
	{
		return (*FoundOne);
	}
	return NULL;
}

// �ѹ��� Request�� �ϳ��� UIPage�� �ε� ���� - 1Request : 1UI �� ��Ű�� �������� ���� ��û�ص� �������
void UB2UIManager_Lobby::ReqAsyncLoadUIPageAsset(ELobbyUIPages InLoadUIPage, bool bBlockingOpenUI/* = false*/)
{
	ReqAsyncLoadUIPageAssets(TArray<ELobbyUIPages> { InLoadUIPage }, bBlockingOpenUI);
}

bool UB2UIManager_Lobby::TryAsyncLoadOpenUI(ELobbyUIPages InLoadUIPage, bool bBlockingOpenUI)
{
	if (bBlockingOpenUI) // Async ��û ��� �� Callback���� OpenUI. ��������� ��Ÿ���� UI ǥ��.
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

#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �� �޸� ����� ���� �̷� �� ���� �ʴ´�..
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
	// FloatingWidgetPool ���� PreCreate �� ���� �ʿ�� ���� ��
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
	{ // �� ������ ������ ��ü ������Ʈ �ϴ� ��. ������ ���°� �ٲ� �پ��� ��쿡 ����ϱ� ����. ���� �� ������ ����ϴ� ���� �ƴϰ�.
		InvenPage->UpdateAllItemIconsForCurrentSelection();
	}
	UB2LobbyUI_AutoEquip* AutoEquipPopup = Cast<UB2LobbyUI_AutoEquip>(DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_AutoEquip));
	if (AutoEquipPopup) // �ڵ� ���� â�� �ִٸ� �ű⵵ ������Ʈ
	{
		AutoEquipPopup->UpdateOnEquipChange();
	}
	// �̰� �� Ư�� ���̽��� �ѵ� ���� â�� ���� �ִ� ���߿� ���� ���� ���� �� ���� ������ ����. 
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

	// ������ ���� ���� â��.. ���� ��� ��� ���� ������Ʈ ���� ��� ������ �� �� ���� ���� UI ������Ʈ�� ��.
	// �������̶� ������� �� �� �� �̰� �� ���� �� �����Ƿ� �� �� �˻��ؼ� InstanceID �´� �� ������Ʈ �Ѵ�.
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
{ // �ӽ� ������. �׽�Ʈ�� �ʿ��� �κ��̶�.

	/* //UIManager�� ü��� ������ �۾���...
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
	// ��庰�� ���� �ִ� â�� �ٸ� ��.
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

	//// Note : ������ ��ȭ���� ���(����,��)â���� ���������� �Ҹ�ǰâ�� ����������� �ٽ� ���(����,��)â�� �̵��ϱ����� �뵵
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
			InvenPage->SetEnableOnlyOneTabType(InNewTab); // InNewTab ���� �ٸ� ���� ���ϰ� ��. ���� ���� Ǯ���־��.
		}
		else{
			InvenPage->SetEnableAllTabButtons(true); // �� bDisableOtherTabSelection �� false �� �Ѱ��ָ� �� Ǫ�� �ɷ�..
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
	// �̰͵� ���� ������ �𸣴�.. ������ �̰͵� ������Ʈ�� �ʿ�..
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
		// ���� notice�� �� ����Ǿ�� �Ѵٸ� ���¸� ��� �ް� Ư�� �������� �ϰ� ó���ؾ��ҵ�.
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
//	// Sorting �ؼ� �������� ������ ������ ������
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
	//			// ��������Ʈ�� ��� �κ� ����ȭ�鿡�� ����Ʈ ��ư Ŭ������ ��û �Ѵ�. (���̾�α׸� �������)
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

				// �̱���� ���� ���޿� ���� ���ǰ� ���� �� ����
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
	// ĳ���� ����Ʈ�� �����ְ�, �����κ��� ���� ���Ź޴´�.
	QuestManager &Manager = QuestManager::GetInstance();
	Manager.FlushCachedCompleteQuest(QuestSlot);
	
	QuestPtr QuestPtrInfo = Manager.GetQuest(QuestSlot);

	if (QuestPtrInfo != nullptr && QuestPtrInfo->GetState() != EQS_REWARDED)
	{
		if (auto* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog))
		{
			// ���� ��ȭ������ �̸� �Ϸ�� ������ ��� Auto Clear ���·� ����� �־� ��ȭ�� ����ޱ�� �Ѿ�� �Ѵ�.
			if (QuestPtrInfo->GetState() == EQS_COMPLETED)
			{
				QuestPtrInfo->UpdateState(EQS_PROGRESS);
				Manager.SetAutoClearQuest(QuestPtrInfo->GetQuestSlot(), QuestPtrInfo->GetQuestId());
			}

			QuestDialogUI->OpenDialogBySlotIndex(QuestSlot);
			// �˾� ���� �� ����Ʈ ����
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
	// ���� ������ ���� �κ��丮 ����
	const FMD_AncientRelicLevelElem* LevelUpInfo = GLOBALRELICMANAGER.getRelicLevelInfo(msgPtr->relic->relic_id, msgPtr->relic->enhance_level);
	if (!LevelUpInfo) return;

	BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE,
																			LevelUpInfo->nNeed_HeroPiece,
																			msgPtr->changed_hero_piece_items,
																			msgPtr->deleted_hero_piece_item_ids);
	// ���� ��, ���������� ����
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, msgPtr->current_money);
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::HeroPiece);

	UB2LobbyUI_RelicManagement* RelicManagement = Cast<UB2LobbyUI_RelicManagement>(CurrentActiveUIPage);
	if (RelicManagement)
		RelicManagement->OnResponseRelicLevelUp(msgPtr->relic->enhance_level);
	
	//////////////////////////////////////////////////////////////////
	//�׼� ��ǥ �α� (ĳ���� ��� ���� ��ȭ)
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
	// ���� �� ����
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, msgPtr->current_money);

	// �������� ����
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
