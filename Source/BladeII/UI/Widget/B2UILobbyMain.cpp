#include "B2UILobbyMain.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUser.h"
#include "Retailer.h"
#include "B2UIQuestSlot.h"
#include "B2UIQuestNotice.h"
#include "B2UIQuestDialog.h"
#include "MissionManager.h"
#include "B2LobbyGameMode.h"
#include "B2RichTextBlock.h"
#include "B2UIItemOptionChange.h"
#include "TutorialManager.h"
#include "B2UIGuildCreate.h"
#include "B2UIDocHelper.h"
#include "B2UILobbyNoticeSlot.h"
#include "B2UICombineMenu.h"
#include "BladeIIPointCharger.h"
#include "B2StageManager.h"
#include "B2UIItemForge.h"
#include "FB2ErrorMessage.h"
#include "B2UIAttendance.h"
#include "B2UIQuestDialog.h"
#include "B2UIQuestNotice.h"
#include "BladeIIGameImpl.h"
#include "B2UIAttendanceMain.h"
#include "B2PlatformAPI.h"
#include "B2GameInstance.h"
#include "B2UILobbyBanner.h"
#include "B2UIStoreEventLabel.h"
#include "B2UIModeOpenNoticeSlot.h"
//#include "B2IAPGenericPlatform.h"
#include "BladeIIGameImpl.h"
#include "B2UIBackWidgetBase.h"
#include "B2UIHeader.h"
#include "RewardEventManager.h"
#include "B2UIDocDonation.h"
#include "B2CombatStatEvaluator.h"

#if PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#endif

UB2UILobbyMain::UB2UILobbyMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AutoHideInterval = 10.0f;

	EnterGameSelectionDelayTime = 0.4f;
	bPendingEnterGameSelection = false;
	bWaitingForPreFetchedActInfo = false;
	SlideOpen = false;

	bSubscribed = false;

	bReceiveGetChallengeMode = false;
	bExecutePendingDungeonScene = false;
	bReceiveGetDuelModeInfo = false;
	bExecutePendingPVPScene = false;

	Created_UIP_GuestWarningPopup = nullptr;
	m_nProductID = 0;
	m_sMarketProductID = TEXT("");
}

void UB2UILobbyMain::OnOpen(bool RightNow)
{
	SubscribeEvent();

	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	if (UIDocManager)
	{
		ABladeIIGameMode* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		UIDocManager->Init(B2GameMode);
	}

	if (RightNow)
	{
		//여기서 오픈 애님을 마지막에서 끊어 재생해준다
		OnOpenComplete();
	}
	else
	{
		SetOpenState(EUIOpenState::OnOpening);

		// GetAnimation 제대로 작동 안 함.
		//UWidgetAnimation* OpenCloseAnim = GetAnimation(TEXT("ANIM_OpenClose"), this);
		//if (OpenCloseAnim)
		//{
		//	//여기서 오픈 애님을 재생해준다.

		//	//현재 오픈 애님 관련 작업 안했으니 열린걸로
		//	OnOpenComplete();
		//}
		//else
		//애님이 없으면 열린걸로
		OnOpenComplete();

		//UB2UIWidget::OnOpen(bool RightNow) 에서는 이 Scope밖에 있는데, 사실 OnOpen_BP()가 오픈 연출을 담당하고 있기 때문에 로직상 여기에 있는 것이 바람직함.
		//허나 BaseClass의 것이라 사이드이펙트가 염려되어 우선 이곳만 바꾼다. 
		//RightNow가 true인 경우가 이곳만 나올듯.
		OnOpen_BP();
	}

	DoMarkCombineRedDot(true);

	CheckSurvey();

	ShowStoreEvent();

	InitDonationEvent();
}

void UB2UILobbyMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Super::NativeTick(MyGeometry, InDeltaTime);

	//ElapsedTimeNoInput += InDeltaTime;

	//AB2LobbyGameMode* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//if (LobbyGameMode)
	//{
	//	auto* PC = UGameplayStatics::GetLocalPlayerController(LobbyGameMode);
	//	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();

	//	if (PC && (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton) || PC->WasInputKeyJustPressed(EKeys::TouchKeys[0])))
	//	{
	//		if (DocUICondition->GetHideAllLobbyMainUIs())
	//			DocUICondition->SetHideAllLobbyMainUIs(false);

	//		ElapsedTimeNoInput = 0.f;
	//	}
	//	if (!DocUICondition->GetHideAllLobbyMainUIs() && ElapsedTimeNoInput >= AutoHideInterval) // 일정 시간 인풋이 없을 시 메인 UI 를 얖 옆으로 숨긴다.
	//	{
	//		if (CheckOtherPopupVisible())
	//			ElapsedTimeNoInput = 0.f;
	//		else
	//			DocUICondition->SetHideAllLobbyMainUIs(true);
	//	}
	//	CheckConnectRewardRedDot(LobbyGameMode);
	//}

	//if (bWaitingForPreFetchedActInfo)
	//{ // 챕터 뷰 진입하려고 서버 데이터 미리 요청해 놨는데 UI 애니메이션 끝날 때까지 아직 응답 안 온 상황.
	//	CheckAnGoToChapterView();
	//}
}

void UB2UILobbyMain::CacheAssets()
{
	GET_SLOT(UPanelWidget, P_TotalCanvas);
	GET_SLOT(UEditableTextBox, ETB_StageNum);

	GET_SLOT(UButton, BTN_Adventure);
	GET_SLOT(UButton, BTN_Dungeon);
	GET_SLOT(UButton, BTN_PVP);

	GET_SLOT(UButton, BTN_Inventory);
	GET_SLOT(UButton, BTN_Package);
	GET_SLOT(UButton, BTN_EquipShop);
	GET_SLOT(UButton, BTN_GoodsShop);
	GET_SLOT(UButton, BTN_MagicShop);
	GET_SLOT(UButton, BTN_ItemForge);

	GET_SLOT(UB2Button, BTN_DonationEvent);
	GET_SLOT(UOverlay, OV_DonationTimer);
	GET_SLOT(UTextBlock, TB_DonationTimer);
	GET_SLOT(UTextBlock, TB_DonationTitle);

	GET_SLOT(UTextBlock, TBS_Inventory);
	GET_SLOT(UTextBlock, TBS_Package);
	GET_SLOT(UTextBlock, TBS_EquipShop);
	GET_SLOT(UTextBlock, TBS_GoodsShop);
	GET_SLOT(UTextBlock, TBS_MagicShop);
	GET_SLOT(UTextBlock, TBS_ItemForge);

	GET_SLOT(UImage, FX_Adventure);
	GET_SLOT(UImage, FX_Dungeon);
	GET_SLOT(UImage, FX_PVP);
	GET_SLOT(UImage, FX_CombineMenu);

	QuestSlots.AddUninitialized(3);
	GET_SLOT_BYNAME(UB2UIQuestSlot, QuestSlots[0], UIP_QuestSlot1);
	GET_SLOT_BYNAME(UB2UIQuestSlot, QuestSlots[1], UIP_QuestSlot2);
	GET_SLOT_BYNAME(UB2UIQuestSlot, QuestSlots[2], UIP_QuestSlot3);
	for (auto QuestSlot : QuestSlots)
	{
		if (QuestSlot.IsValid())
		{
			QuestSlot->Init();
			QuestSlot->SetVisibility(ESlateVisibility::Collapsed); // 처음은 안보이게 설정
		}
	}

	BTN_SlideButtons.AddUninitialized(MaxSlideButtonCount);
	GET_SLOT_BYNAME(UButton, BTN_SlideButtons[0], BTN_SlideButton0);	 // 각 버튼들 기능이 결정되면 이름 변경
	GET_SLOT_BYNAME(UButton, BTN_SlideButtons[1], B2Button_2);
	GET_SLOT_BYNAME(UButton, BTN_SlideButtons[2], BTN_SlideButton2);
	GET_SLOT_BYNAME(UButton, BTN_SlideButtons[3], BTN_SlideButton3);
	GET_SLOT_BYNAME(UButton, BTN_SlideButtons[4], BTN_SlideButton4);
	GET_SLOT_BYNAME(UButton, BTN_SlideButtons[5], BTN_SlideButton5);

	/*GET_SLOT(UTextBlock, TB_ClassLevel);
	GET_SLOT(UTextBlock, TB_UserNickname);

	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, TB_PowerValue);*/

	//GET_SLOT(UButton, BTN_ChangeFocusCharacter);

	GET_SLOT(UButton, BTN_SlideMove);
	ANIM_SlideMove = GetAnimation(this, "ANIM_SlideMove");

	GET_SLOT(UB2Button, BTN_CombineMenu);
	GET_SLOT(UB2Button, BTN_UIOnOff);

	GET_SLOT(UB2UICombineMenu, UI_CombineMenu);
	GET_SLOT(UB2Image, IMG_CombineMenu_Up);
	GET_SLOT(UB2Image, IMG_CombineMenu_Down);

	GET_SLOT(UTextBlock, TB_Menu);

	GET_SLOT(UProgressBar, ExpProgressBar);
	GET_SLOT(UProgressBar, ExpProgressBar_Max);

	GET_SLOT(UB2UILobbyBanner, UIP_Lobby_Banner);

	if (UIP_Lobby_Banner.IsValid())
		UIP_Lobby_Banner->Init();

	if (UI_CombineMenu.IsValid())
		UI_CombineMenu->Init();

	GET_SLOT(UButton, X_BTN_Survey);
	if (X_BTN_Survey.IsValid())
		X_BTN_Survey->SetVisibility(ESlateVisibility::Collapsed);
	GET_SLOT(UTextBlock, X_TB_Survey);
	if (X_TB_Survey.IsValid())
		X_TB_Survey->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SurveyText")));

	GET_SLOT(UB2UIStoreEventLabel, UIP_Store_Event_Label);
	GET_SLOT(UB2UIStoreEventLabel, UIP_SummonItem_Event_Label);
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->Init();
	if (UIP_SummonItem_Event_Label.IsValid())
		UIP_SummonItem_Event_Label->Init();

	// MAX_VISIBLE_CONTENTOPEN_COUNT == 2
	ContentOpenSlots.AddDefaulted(MAX_VISIBLE_CONTENTOPEN_COUNT);

	GET_SLOT_BYNAME(UOverlay, ContentOpenSlots[0].O_ContentOpen, O_ContentOpen1);
	GET_SLOT_BYNAME(UImage, ContentOpenSlots[0].IMG_ContentOpen, IMG_ContentOpen1);
	GET_SLOT_BYNAME(UTextBlock, ContentOpenSlots[0].TB_ContentOpenTitle, TB_ContentOpenTitle1);
	GET_SLOT_BYNAME(UTextBlock, ContentOpenSlots[0].TB_ContentOpenDesc, TB_ContentOpenDesc1);
	GET_SLOT_BYNAME(UB2Button, ContentOpenSlots[0].BTN_ContentOpen, BTN_ContentOpen1);
	GET_SLOT_BYNAME(UImage, ContentOpenSlots[0].IMG_EffetContentOpen, IMG_EffetContentOpen1);

	GET_SLOT_BYNAME(UOverlay, ContentOpenSlots[1].O_ContentOpen, O_ContentOpen2);
	GET_SLOT_BYNAME(UImage, ContentOpenSlots[1].IMG_ContentOpen, IMG_ContentOpen2);
	GET_SLOT_BYNAME(UTextBlock, ContentOpenSlots[1].TB_ContentOpenTitle, TB_ContentOpenTitle2);
	GET_SLOT_BYNAME(UTextBlock, ContentOpenSlots[1].TB_ContentOpenDesc, TB_ContentOpenDesc2);
	GET_SLOT_BYNAME(UB2Button, ContentOpenSlots[1].BTN_ContentOpen, BTN_ContentOpen2);
	GET_SLOT_BYNAME(UImage, ContentOpenSlots[1].IMG_EffetContentOpen, IMG_EffetContentOpen2);

	GET_SLOT(UCanvasPanel, X_CP_GuestWarningPopup_Panel);

	GET_SLOT(UB2UIModeOpenNotice, UIP_ModeOpenNotice);
	if (UIP_ModeOpenNotice.IsValid())
		UIP_ModeOpenNotice->Init();
}

void UB2UILobbyMain::UpdateStaticText()
{
	/*if (STB_CombatPower.IsValid())
	STB_CombatPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));*/

	if (TB_Menu.IsValid())
		TB_Menu->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Menu")));

	if (TBS_Inventory.IsValid())
		TBS_Inventory->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_EquipMgmtButton")));
	if (TBS_Package.IsValid())
		TBS_Package->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Package")));
	if (TBS_EquipShop.IsValid())
		TBS_EquipShop->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem")));
	if (TBS_GoodsShop.IsValid())
		TBS_GoodsShop->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_GoodsStore")));
	if (TBS_MagicShop.IsValid())
		TBS_MagicShop->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_MagicStore")));
	if (TBS_ItemForge.IsValid())
		TBS_ItemForge->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Title")));

	if (TB_DonationTitle.IsValid())
	{
		TB_DonationTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_donation")));
	}
}

void UB2UILobbyMain::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Adventure, &UB2UILobbyMain::OnClickBtnAdventure);
	BIND_CLICK_FUNC_TO_BTN(BTN_Dungeon, &UB2UILobbyMain::OnClickBtnDungeon);
	BIND_CLICK_FUNC_TO_BTN(BTN_PVP, &UB2UILobbyMain::OnClickBtnPVP);

	BIND_CLICK_FUNC_TO_BTN(BTN_Inventory, &UB2UILobbyMain::OnClickBtnInventory);
	BIND_CLICK_FUNC_TO_BTN(BTN_Package, &UB2UILobbyMain::OnClickBtnPackage);
	BIND_CLICK_FUNC_TO_BTN(BTN_EquipShop, &UB2UILobbyMain::OnClickBtnEquipShop);
	BIND_CLICK_FUNC_TO_BTN(BTN_GoodsShop, &UB2UILobbyMain::OnClickBtnGoodsShop);
	BIND_CLICK_FUNC_TO_BTN(BTN_MagicShop, &UB2UILobbyMain::OnClickBtnMagicShop);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemForge, &UB2UILobbyMain::OnClickBtnItemForge);
	BIND_CLICK_FUNC_TO_BTN(BTN_DonationEvent, &UB2UILobbyMain::OnClickBtnDonationEvent);

	//BIND_CLICK_FUNC_TO_BTN(BTN_ChangeFocusCharacter, &UB2UILobbyMain::OnClickBtnChangeFocusCharacter);

	BIND_CLICK_FUNC_TO_BTN(BTN_SlideMove, &UB2UILobbyMain::OnClickBtnSlideMove);

	BIND_CLICK_FUNC_TO_BTN(BTN_SlideButtons[1], &UB2UILobbyMain::OnClickBtnAttendance);
	BIND_CLICK_FUNC_TO_BTN(BTN_CombineMenu, &UB2UILobbyMain::OnClickBtnCombineMenu);
	BIND_CLICK_FUNC_TO_BTN(BTN_UIOnOff, &UB2UILobbyMain::OnClickBtnUIOnOff);
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Survey, &UB2UILobbyMain::OnClickBtnSurvey);

	BladeIIGetDuelModLoseMentText();
}

void UB2UILobbyMain::DestroySelf(class UB2UIManager* InUIManager)
{
	DestroyGuestWarningPopup();

	if (GetCombineMenuIsVisible())
	{
		UI_CombineMenu->CloseMenu();
	}

	UI_CombineMenu->DestroySelf(InUIManager);
	UIP_ModeOpenNotice->DestroySelf(InUIManager);

	EndDonationEventTimer();

	Super::DestroySelf(InUIManager);
}

void UB2UILobbyMain::BindDocAuto()
{
	UnbindDoc();

	SetDoc(nullptr);

	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
	{
		DocUICondition->OnHideAllLobbyMainUIsChanged.AddUObject(this, &UB2UILobbyMain::OnChangedHideAllLobbyMainUIs);
	}
}

void UB2UILobbyMain::UnbindDoc()
{
	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
	{
		DocUICondition->OnHideAllLobbyMainUIsChanged.RemoveAll(this);
	}

	SetDoc(nullptr);
}

void UB2UILobbyMain::OnOpenComplete()
{
	Super::OnOpenComplete();

	ElapsedTimeNoInput = 0.f;

	HideLobbyContentOpenSlots();

	OnOpenComplete_RequestToServer();
	OnOpenComplete_Once();

	OnReceiveLobbyMissionAlarm();

	FBladeIIChargeBladePointCharger::GetBladeIIPointCharger().SetbInitialized(false);
	FBladeIIChargeBladePointCharger::GetPvPMatchPointCharger().SetbInitialized(false);
	FBladeIIChargeBladePointCharger::GetTeamMatchPointCharger().SetbInitialized(false);
	data_trader::Retailer::GetInstance().RequestCheckChargePoints(true, true, true);

	if (auto* AttMain = UB2UIManager::GetInstance()->GetUI<UB2UIAttendanceMain>(UIFName::AttendanceMain))
	{
		AttMain->Init();
	}

	UpdateCombineMenuVisible(GetCombineMenuIsVisible());

	BladeIIGameImpl::GetChatStore().RequestBlockUserList();
}

void UB2UILobbyMain::OnCloseComplete()
{
	UpdateCombineMenuVisible(false);
	CloseGuestWarningPopup();

	UnSubscribeEvent();

	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		// 아이템 인벤토리에서 받았는데 보관함개수가 필요해서 로비나갈때 받습니다
		BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(IntToPCClass(i));
	}

	Super::OnCloseComplete();
}

void UB2UILobbyMain::SubscribeEvent()
{
	if (bSubscribed == false)
	{
		UnSubscribeEvent();

		Issues.Add(LobbyCharObserveChangeScreenClass<bool>::GetInstance().Subscribe2(
			[this](bool bVertical)
		{
			this->SetVisibility(bVertical ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
		}
		));

		Issues.Add(DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Subscribe2(
			[this](FB2MessageInfoResponseCheckRedDotPtr LobbyRedDotInfo)
		{
			this->OnReceiveCheckRedDot(LobbyRedDotInfo);
		}
		));

		Issues.Add(DeliveryUpdateTutorialProgressClass<FB2ResponseUpdateTutorialProgressPtr>::GetInstance().Subscribe2(
			[this](FB2ResponseUpdateTutorialProgressPtr TutorialProgress)
		{
			this->OnReceiveTutorialProgress(TutorialProgress);
		}
		));

		Issues.Add(DeliveryDailyAttendanceClass<FB2ReceiveDailyAttendance>::GetInstance().Subscribe2(
			[this](FB2ReceiveDailyAttendance DailyAttendance)
		{
			this->ResponseDailyAttendance(DailyAttendance);
		}
		));

		Issues.Add(DoMarkCombineRedDotClass<>::GetInstance().Subscribe2(
			[this]()
		{
			this->DoMarkCombineRedDot(true);
		}
		));

		Issues.Add(DeliveryKakaoMessageProcessingClass<FB2ResponseKakaoMessageProcessingPtr>::GetInstance().Subscribe2(
			[this](FB2ResponseKakaoMessageProcessingPtr KakaoMessageProcessing)
		{
			this->OnReceiveKakaoMessageProcessing(KakaoMessageProcessing);
		}
		));

		Issues.Add(DeliveryGetFeatureMissionClass<FB2ReciveGetFeaturedMission>::GetInstance().Subscribe2(
			[this](FB2ReciveGetFeaturedMission GetFeaturedMission)
		{
			this->OnReceiveGetFeaturedMission(GetFeaturedMission);
		}
		));

		Issues.Add(LobbyMissionAlarmClass<>::GetInstance().Subscribe2(
			[this]()
		{
			this->OnReceiveLobbyMissionAlarm();
		}
		));

		Issues.Add(CloseCombineMenuClass<>::GetInstance().Subscribe2(
			[this]()
		{
			this->CloseCombineMenu();
		}
		));

		Issues.Add(DeliveryGetPeriodPackageStateClass<FB2ResponseGetPeriodPackageStatePtr>::GetInstance().Subscribe2(
			[this](FB2ResponseGetPeriodPackageStatePtr Message)
		{
			this->ResponseGetPeriodPackageState(Message);
		}
		));

		Issues.Add(RequestCombatPowerClass<EPCClass>::GetInstance().Subscribe2(
			[this](EPCClass SelectPCClass)
		{
			if (SelectPCClass == EPCClass::EPC_End)
			{
				for (int8 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
				{
					EPCClass PCClass = IntToPCClass(PCI);
					this->RequestCombatPower(PCClass);
				}
			}
			else
			{
				this->RequestCombatPower(SelectPCClass);
			}
		}
		));

		Issues.Add(DeliveryTrackingPowerDetailClass<FB2ResponseTrackingPowerDetailPtr>::GetInstance().Subscribe2(
			[this](FB2ResponseTrackingPowerDetailPtr Message)
		{
			this->ResponseTrackingPowerDetail(Message);
		}
		));

		Issues.Add(DeliveryJackpotFairyClass<FB2ServerCmdPtr>::GetInstance().Subscribe2(
			[this](FB2ServerCmdPtr Message)
		{
			this->ResponseJackpotFairy(Message);
		}
		));

#if PLATFORM_ANDROID 
		FJavaWrapper::OnKakaoPromotionDeepLinkDelegate.Remove(OnKakaoPromotionDeepLinkHandler);
		OnKakaoPromotionDeepLinkHandler = FJavaWrapper::OnKakaoPromotionDeepLinkDelegate.AddUObject(this, &UB2UILobbyMain::OnKakaoPromotionDeepLinkPlatformCallback);
#elif PLATFORM_IOS
		FKakaoTalkPlugin::OnDeepLinkURLDelegate.Remove(OnKakaoPromotionDeepLinkHandler);
		OnKakaoPromotionDeepLinkHandler = FKakaoTalkPlugin::OnDeepLinkURLDelegate.AddUObject(this, &UB2UILobbyMain::OnKakaoDeepLink);
#endif
		bSubscribed = true;
	}
}

void UB2UILobbyMain::UnSubscribeEvent()
{
	Issues.Empty();

	bSubscribed = false;
}

void UB2UILobbyMain::OpenUnappliedItemOptionChangeUI()
{
	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	FB2Item UnappliedItem;
	if (ClientDataStore.GetItemOptionUnappliedItem(UnappliedItem))
	{
		EPCClass TargetClass = IntToPCClass(UnappliedItem.AllowedPCClass);
		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(TargetClass);

		UB2UIItemOptionChange* ItemOptionChangeUI = UB2UIManager::GetInstance()->OpenUI<UB2UIItemOptionChange>(UIFName::ItemOptionChange);
		if (ItemOptionChangeUI)
		{
			const TArray<FItemOption>& UnappliedItemOptions = ClientDataStore.GetUnappliedItemOptions();
			ItemOptionChangeUI->SetTargetItem(UnappliedItem);
			ItemOptionChangeUI->SetNewOptionDetail(ClientDataStore.GetUnappliedItemOptions());
		}
	}

	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory); // GameMode 쪽 신호
}

void UB2UILobbyMain::OnReceiveCheckRedDot(FB2MessageInfoResponseCheckRedDotPtr LobbyRedDotInfo)
{
	DoMarkCombineRedDot();

	// bHasUnappliedItem = GET_TUPLE_DATA(FB2ResponseCheckRedDot::has_item_random_option_change_index, LobbyRedDotInfo);
	bool bHasUnappliedItem = false;
	if (bHasUnappliedItem)	// 적용 안한 아이템이 있다 정도만 확인되면 상세스펙 요청
	{
		data_trader::Retailer::GetInstance().RequestGetItemRandomOption();
	}
}

void UB2UILobbyMain::OnReceiveTutorialProgress(FB2ResponseUpdateTutorialProgressPtr TutorialProgress)
{
	if (TutorialProgress != nullptr)
	{
		UpdateLobbyContentOpenSlots();
	}
}

void UB2UILobbyMain::ResponseDailyAttendance(const FB2ReceiveDailyAttendance& AttendaceInfo)
{
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	ClientDataStore.SetDailyAttendanceInfo(AttendaceInfo);

	bool bAttendanceOpenCheck = false;
	float fStartMatineeDelay = 1.5f;

	if (ClientDataStore.GetAttendanceOpenDailyOnce() == false)
	{
		if (HasAttendanceNotice())
		{
			const int32 iAttendancePopupIndex = 0;
			LobbyNoticePopUpClass<int32>::GetInstance().Signal(iAttendancePopupIndex);
			ClientDataStore.SetAttendanceOpenDailyOnce(true);
			bAttendanceOpenCheck = true;

			// 로비 메인 첫 진입 연출 플래그 세팅을 이거 전에 (FLobbyTitleScene) 한번 하긴 하는데, 
			// 그 이후 출석 응답 오기 전까지 이미 한 번 보여주게 되어서 출석 닫은 후 또 플레이하기 위해 여기서 리셋.
			if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				LobbyGM->ResetPlayBeginActionState();
			}
		}

		fStartMatineeDelay = 0.01f;
	}

	if (bAttendanceOpenCheck == false)
	{
		CahcedCheckAndProgressTutorialClass<>::GetInstance().Signal();
		LobbyCharPlayMatineeClass<float>::GetInstance().Signal(fStartMatineeDelay);
	}
}

void UB2UILobbyMain::ResponseGetPeriodPackageState(FB2ResponseGetPeriodPackageStatePtr ReciveGetPeriodPackageState)
{
	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		// 기간제 남은 시간
		for (auto PeridicItem : ReciveGetPeriodPackageState->periodic_package_states)
		{
			DocStore->SetReceivedPeriodicPackageLeftTime(PeridicItem->package_id, PeridicItem->end_pay_time);
		}
	}

	if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		LobbyGM->SetFirstLobbyMain(false);
	}

	UpdateDailyNotice();
}

void UB2UILobbyMain::UpdateDailyNotice()
{
	UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	if (HeaderUI)
		HeaderUI->SetDailyNotice();
}

void UB2UILobbyMain::HideLobbyContentOpenSlots()
{
	UpdateLobbyContentOpenSlots();
}

void UB2UILobbyMain::UpdateLobbyContentOpenSlots()
{
	TutorialManager& TutorialMgr = TutorialManager::GetInstance();

	const int32 LastClearTutorialID = TutorialMgr.GetLastClearTutorialID();

	const int32 NextTutorialID1 = TutorialMgr.GetNextTutorialID(LastClearTutorialID);
	const int32 NextTutorialID2 = TutorialMgr.GetNextTutorialID(NextTutorialID1);

	TArray<int32> DisplayTutorialIDs;
	if (const FTutorialInfo* Tutorial1 = TutorialMgr.GetTutorialInfo(NextTutorialID1))
	{
		if (Tutorial1->TutorialType == ETutorialType::ContentOpen)
			DisplayTutorialIDs.Add(NextTutorialID1);
	}

	if (const FTutorialInfo* Tutorial2 = TutorialMgr.GetTutorialInfo(NextTutorialID2))
	{
		if (Tutorial2->TutorialType == ETutorialType::ContentOpen)
			DisplayTutorialIDs.Add(NextTutorialID2);
	}

	int32 Index = 0;
	for (auto& ContentOpenSlot : ContentOpenSlots)
	{
		int32 TutorialID = DisplayTutorialIDs.IsValidIndex(Index) ? DisplayTutorialIDs[Index] : INDEX_NONE;
		ContentOpenSlot.SetContentOpenSlot(TutorialID);

		if (Index == 1)	// 알림 슬롯이 2개 이상일 경우 마지막 이펙트는 끔
		{
			ContentOpenSlot.IMG_EffetContentOpen->SetVisibility(ESlateVisibility::Collapsed);
		}

		Index++;
	}
}

void UB2UILobbyMain::OnOpenComplete_RequestToServer()
{
	static int32 CurrentDay = -1;
	data_trader::Retailer& RetailerInst = data_trader::Retailer::GetInstance();

	// 튜토리얼 진행 중 퀘스트 요청은 내부에서 Slot만 활성화하고 팝업은 활성화X
	RetailerInst.RequestGetQuest();
	RetailerInst.RequestGetFriendList();
	RetailerInst.RequestGetAllReceiveAskFriend();
	RetailerInst.RequestGetAllSendAskFriend();

	RetailerInst.RequestCheckRedDot();						// LobbyMain New 갱신

	UB2GameInstance::RequestUpdateDailyPlayTime();

	if (CurrentDay != FDateTime::UtcNow().GetDay())
	{
		RetailerInst.RequestDailyAttendance();
		RewardEventManager::GetInstance().RequestSeasonEventStatus();
		CurrentDay = FDateTime::UtcNow().GetDay();
	}
	else
	{
		CahcedCheckAndProgressTutorialClass<>::GetInstance().Signal();
		LobbyCharPlayMatineeClass<float>::GetInstance().Signal(0.01f);
	}

#if PLATFORM_ANDROID || PLATFORM_IOS
	// kakao messagebox check	
	if (B2P_GetKakaoIDPCode() != EPlatformIDPCode::None)
		GetWorld()->GetGameInstance<UB2GameInstance>()->SendKakaoMessgaeInboxProcessing();

	if (B2P_IsGoogleLogin())
		RetailerInst.RequestGetFeaturedMission();
#endif
	// 튜토리얼 잠금 위젯 로비진입시 무조건 한번 리프레시
	if (TutorialManager::GetInstance().IsTutorialAvailable())
		TutorialManager::GetInstance().RefreshContentLockUI();

#if !PLATFORM_MAC
	// 인앱결제 초기화
	//B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->RequestGetMarketProduct_Once();
#endif
}

void UB2UILobbyMain::OnOpenComplete_Once()
{
	if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (LobbyGM->GetFirstLobbyMain())
		{
			data_trader::Retailer::GetInstance().RequestGetPeriodPackageState();
		}
	}
}

void UB2UILobbyMain::OnChangedLobbyFocusClass(class UB2UIDocBase* Sender, EPCClass CurrentFocusClass, EPCClass PrevFocusClass)
{
	/*
	if (CurrentFocusClass == EPCClass::EPC_End)
	return;

	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	const int32 FocusPCLevel = ClientDataStore.GetLocalCharacterData().GetCharacterLevel(CurrentFocusClass);
	const int32 CharacterMaxLevel = ClientDataStore.GetLocalCharacterData().GetCharacterMaxLevel();
	const float ExpPercent = ClientDataStore.GetLocalCharacterData().GetCharacterExpPercent(CurrentFocusClass) * 0.01f;

	if (TB_ClassLevel.IsValid())
	{
	TB_ClassLevel->SetText(FText::FromString(FString::Printf(TEXT("LV.%d"), FocusPCLevel)));
	}

	if (TB_UserNickname.IsValid())
	{
	TB_UserNickname->SetText(ClientDataStore.GetLocalCharClassOrNickName(CurrentFocusClass));
	}

	if (TB_PowerValue.IsValid())
	{
	auto* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(CurrentFocusClass));
	if(DocHero)
	TB_PowerValue->SetText(FText::AsNumber(DocHero->GetCombatPower()));
	}

	if (ExpProgressBar.IsValid() && ExpProgressBar_Max.IsValid())
	{
	// 만랩이면 띄우줌.
	bool IsMax = CharacterMaxLevel <= FocusPCLevel;
	ExpProgressBar->SetPercent(IsMax ? 0.0f : ExpPercent);
	ExpProgressBar->SetVisibility(IsMax ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	ExpProgressBar_Max->SetVisibility(IsMax ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	*/
}

bool UB2UILobbyMain::RedDotCondition_Adventure()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Adventure();
}

bool UB2UILobbyMain::RedDotCondition_Dungeon()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Dungeon();
}

bool UB2UILobbyMain::RedDotCondition_PVP()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Battle();
}

bool UB2UILobbyMain::RedDotCondition_Inventory()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Inventory();
}

bool UB2UILobbyMain::RedDotCondition_Package()
{
	return (
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Package_FlatRate() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Package_Levelup() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Package_ReachLevel()
		);
}

bool UB2UILobbyMain::RedDotCondition_EquipShop()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_FreeItem() || BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mileage_Item();
}

bool UB2UILobbyMain::RedDotCondition_GoodsShop()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mileage_Cash();
}

bool UB2UILobbyMain::RedDotCondition_MagicShop()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_MagicStore();
}

bool UB2UILobbyMain::RedDotCondition_SlideMove()
{
	return RedDotCondition_Slide_Event();
}

bool UB2UILobbyMain::RedDotCondition_Slide_Event()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_EventAttendance()
		|| BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_ConnectReward();
}

bool UB2UILobbyMain::RedDotCondition_CombineMenu()
{
	return (
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Inventory() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_CollectBook() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mission() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Friend() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt() ||
		BladeIIGameImpl::GetRedDotManager().HadLobbyRedDot_Attendance() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_RestPoint() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_SeasonEvent() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HallOfFame() ||
		BladeIIGameImpl::GetRedDotManager().HasNotApplyBlessFairy() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Costume() ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Totem()
		);
}

bool UB2UILobbyMain::RedDotCondition_DonationEvent()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Donation();
}

void UB2UILobbyMain::ExecutePendingDungeonScene()
{
	bExecutePendingDungeonScene = true;

	ExecutePendingDungeonSceneInner();
}

// 패킷 받을때랑 버튼타이머 돌았을때 둘다 호출함.
void UB2UILobbyMain::ExecutePendingDungeonSceneInner()
{
	// 타미어도 돌구 패킷도 받아야 실행
	if (!bReceiveGetChallengeMode || !bExecutePendingDungeonScene)
		return;

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(EnterGameSelectionTH);
	}
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EnterDungeonMain);
}

void UB2UILobbyMain::ExecutePendingPVPScene()
{
	bExecutePendingPVPScene = true;

	ExecutePendingPVPSceneInner();
}

// 패킷 받을때랑 버튼타이머 돌았을때 둘다 호출함.
void UB2UILobbyMain::ExecutePendingPVPSceneInner()
{
	// 타미어도 돌구 패킷도 받아야 실행
	if (!bReceiveGetDuelModeInfo || !bExecutePendingPVPScene)
		return;

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(EnterGameSelectionTH);
	}
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EnterBattleMain);
}

void UB2UILobbyMain::ExecutePendingAdventureScene()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(EnterGameSelectionTH);
	}

	CheckAnGoToChapterView();
}

void UB2UILobbyMain::CheckAnGoToChapterView()
{
	if (BladeIIGameImpl::GetStageDataStore().IsActInfoRequestPending())
	{
		// PreFetchAdventureSceneInfo 를 한 상황에서 아직 응답이 오지 않은 거..
		bWaitingForPreFetchedActInfo = true;
	}
	else
	{
		// 최근 플레이한 스테이지 정보
		FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();

		FServerStageID ServerStageID = StageStore.GetLastPlayedServerStageId();
		int32 ChapterNumber = StageStore.GetActIdByClientStageId(StageStore.GetClientStageId(ServerStageID));
		EStageDifficulty StageDifficulty = StageStore.GetStageDifficulty(ServerStageID);

		if (ServerStageID == 0)	// 서버에 저장된 최근 플레이 정보가 0 일때. 강제로 1-1로 세팅
		{
			ChapterNumber = 1;
			StageDifficulty = EStageDifficulty::ESD_Normal;
		}
		else if (ChapterNumber > 5)
		{
			ChapterNumber = 5;
			StageDifficulty = EStageDifficulty::ESD_Hell;
		}

		// PreFetchAdventureSceneInfo 를 했다면 바로 챕터 뷰로 넘어갈 꺼고 아니면 여기서 서버 정보 요청을 하게 될 꺼.
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::AdventureEnterBattleMain);
		// BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(ChapterNumber, StageDifficulty);

		bWaitingForPreFetchedActInfo = false; // UI 장면 넘어가겠지만 그래도 마크.
	}
}
void UB2UILobbyMain::PreFetchAdventureSceneInfo()
{
	// 챕터맵 뷰 진입 이전에 UI 애니메이션을 위해 Pending 을 시키고 아직 데이터를 안 받은 상황이면 서버에 데이터를 요청까지 하는데 
	// 이게 시간 낭비라 UI 애니메이션 하는 와중에 서버에 데이터 요청을 미리 해 두는 의미.

	// CheckAnGoToChapterView 에서 요청할 정보와 마찬가지여야. 그렇지만 만일 아니라고 해도 큰일나는 건 아님.
	FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();

	FServerStageID ServerStageID = StageStore.GetLastPlayedServerStageId();
	int32 ChapterNumber = StageStore.GetActIdByClientStageId(StageStore.GetClientStageId(ServerStageID));
	EStageDifficulty StageDifficulty = StageStore.GetStageDifficulty(ServerStageID);

	if (ServerStageID == 0)// 서버에 저장된 최근 플레이 정보가 0 일때. 강제로 1-1로 세팅
	{
		ChapterNumber = 1;
		StageDifficulty = EStageDifficulty::ESD_Normal;
	}

	// RequestGetActInfo 만 하고 응답시 챕터 뷰로 가는 건 아님. 그건 RequestGetActInfoAndChangeChapter 에서..
	BladeIIGameImpl::GetStageDataStore().ConditionalRequestGetActInfo(ChapterNumber, StageDifficulty);
}

void UB2UILobbyMain::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_Adventure.Get()), &UB2UILobbyMain::RedDotCondition_Adventure, [this](bool bShowRedDot)
	{
		if (FX_Adventure.IsValid())
			FX_Adventure->SetVisibility(bShowRedDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		OnChangeRedDot_Adventure(bShowRedDot);
	});
	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_Dungeon.Get()), &UB2UILobbyMain::RedDotCondition_Dungeon, [this](bool bShowRedDot)
	{
		if (FX_Dungeon.IsValid())
			FX_Dungeon->SetVisibility(bShowRedDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		OnChangeRedDot_Dungeon(bShowRedDot);
	});
	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_PVP.Get()), &UB2UILobbyMain::RedDotCondition_PVP, [this](bool bShowRedDot)
	{
		if (FX_PVP.IsValid())
			FX_PVP->SetVisibility(bShowRedDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		OnChangeRedDot_PVP(bShowRedDot);
	});

	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_Inventory.Get()), &UB2UILobbyMain::RedDotCondition_Inventory, [this](bool bShowRedDot)
	{
		OnChangeRedDot_Inventory(bShowRedDot);
	});

	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_Package.Get()), &UB2UILobbyMain::RedDotCondition_Package, [this](bool bShowRedDot)
	{
		OnChangeRedDot_Package(bShowRedDot);
	});

	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_EquipShop.Get()), &UB2UILobbyMain::RedDotCondition_EquipShop, [this](bool bShowRedDot)
	{
		OnChangeRedDot_EquipShop(bShowRedDot);
	});

	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_GoodsShop.Get()), &UB2UILobbyMain::RedDotCondition_GoodsShop, [this](bool bShowRedDot)
	{
		OnChangeRedDot_GoodsShop(bShowRedDot);
	});

	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_MagicShop.Get()), &UB2UILobbyMain::RedDotCondition_MagicShop, [this](bool bShowRedDot)
	{
		OnChangeRedDot_MagicShop(bShowRedDot);
	});

	REGISTER_REDDOT_CONDITION_WITHDELEGATE(GetRedDotWidget(BTN_CombineMenu.Get()), &UB2UILobbyMain::RedDotCondition_CombineMenu, [this](bool bShowRedDot)
	{
		if (FX_CombineMenu.IsValid())
			FX_CombineMenu->SetVisibility(bShowRedDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		OnChangeRedDot_CombineMenu(bShowRedDot);
	});

	// Slide
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_SlideMove.Get()), &UB2UILobbyMain::RedDotCondition_SlideMove);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_SlideButtons[1].Get()), &UB2UILobbyMain::RedDotCondition_Slide_Event);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_DonationEvent.Get()), &UB2UILobbyMain::RedDotCondition_DonationEvent);
}

bool UB2UILobbyMain::HasAttendanceNotice()
{
	if (BladeIIGameImpl::GetClientDataStore().GetAttendanceInfo().bIsAttendance ||
		BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_EventAttendance())
	{
		return true;
	}

	return false;
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UILobbyMain::OnClickBtnDungeon()
{
	// Widget Anim 플레이 후에 실제 Challenge 화면에 진입하도록 예약만 해 둔다.
	if (!bPendingEnterGameSelection)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			// 도전 모드인포 요청
			data_trader::Retailer::GetInstance().RequestGetChallengeModeInfo();

			TheWorld->GetTimerManager().SetTimer(EnterGameSelectionTH, this, &UB2UILobbyMain::ExecutePendingDungeonScene, EnterGameSelectionDelayTime, false);
			bPendingEnterGameSelection = true;

			bReceiveGetChallengeMode = false;
			bExecutePendingDungeonScene = false;
		}
	}

	static bool bOpen = false;

	if (bOpen == false)
	{
		ActiveHighlight(FName(TEXT("Highlight")));
		bOpen = true;
	}

	else
	{
		ClearHighlight();
		bOpen = false;
	}
}

void UB2UILobbyMain::OnClickBtnPVP()
{
	// Widget Anim 플레이 후에 실제 Challenge 화면에 진입하도록 예약만 해 둔다.
	if (!bPendingEnterGameSelection)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			// 결투 모드인포 요청
			data_trader::Retailer::GetInstance().RequestGetDuelModeInfo();

			TheWorld->GetTimerManager().SetTimer(EnterGameSelectionTH, this, &UB2UILobbyMain::ExecutePendingPVPScene, EnterGameSelectionDelayTime, false);
			bPendingEnterGameSelection = true;

			bReceiveGetDuelModeInfo = false;
			bExecutePendingPVPScene = false;
		}
	}
}

void UB2UILobbyMain::OnClickBtnAdventure()
{
	// LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::AdventureEnterBattleMain);
	// Widget Anim 플레이 후에 실제 Adventure 화면에 진입하도록 예약만 해 둔다.
	if (!bPendingEnterGameSelection)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter();
			if (DocChapter && DocChapter->GetCurChapterNum() <= 0)
			{
#if WITH_EDITOR && !PLATFORM_MAC// 아몰랑 ㅠㅠ
				//UE_LOG(LogBladeII, Warning, TEXT("현재 들어가고자 하는 Chapter 번호가 초기화 되지 않은 상태.. 언제부턴가 로비에서 모험맵 처음 진입하면 Chapter 번호가 0 인채로 걸리길래.. 무슨 일이 생긴 건지.."));
#else
				//UE_LOG(LogBladeII, Warning, TEXT("Trying to open Chapter scene while chapter data is not initialized.. Update it on demand but it doesn't look so fine."));
#endif
				if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					LobbyGM->UpdateChapterData();
				}
			}

			PreFetchAdventureSceneInfo(); // ExecutePendingAdventureScene 까지 시간이 있으므로 그 새 ActInfo 데이터를 미리 요청해 둠
			TheWorld->GetTimerManager().SetTimer(EnterGameSelectionTH, this, &UB2UILobbyMain::ExecutePendingAdventureScene, EnterGameSelectionDelayTime, false);
			bPendingEnterGameSelection = true;
		}
	}
}

void UB2UILobbyMain::ChangeChaperScene(int32 ChapterNum)
{
	// LobbyChangeSceneByUISceneClass 대신 챕터 번호를 명시하는 챕터 전용 이벤트를 날린다.
	LobbyChangeToChapterSceneByUISceneClass<int32>::GetInstance().Signal(ChapterNum);
}

void UB2UILobbyMain::OnClickBtnInventory()
{
	bool bGDC = ShouldShowGDCVersionContent(); // [GDC] Feb 2017

	if (bGDC)
	{
		DevShowNotReadyYetMessage();
		return;
	}

	//튜토리얼 진행 중 인벤토리 진입시 MainPC변경을 가장 마지막에 호출하는 부분 나중에 변경 될 수 도 있다.
	if (TutorialManager::GetInstance().HasAnyTutorial())
	{
		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(
			TutorialManager::GetInstance().GetTutorialRequestPrepareClass());
	}
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UILobbyMain::OnClickBtnPackage()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetPackageTabToOpen(1);
		//EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::PackageStore));
	}
}

void UB2UILobbyMain::OnClickBtnEquipShop()
{
	bool bGDC = ShouldShowGDCVersionContent(); // [GDC] Feb 2017

	if (bGDC)
	{
		DevShowNotReadyYetMessage();
		return;
	}

	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::EquipStore));
}

void UB2UILobbyMain::OnClickBtnGoodsShop()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::BLADE_POINT);
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
	}
}

void UB2UILobbyMain::OnClickBtnMagicShop()
{
	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::MagicStore));
}

void UB2UILobbyMain::OnClickBtnItemForge()
{
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::ItemForge);

	UB2UIItemForge* ItemForgeUI = UB2UIManager::GetInstance()->GetUI<UB2UIItemForge>(UIFName::ItemForge);
	if (ItemForgeUI)
	{
		ItemForgeUI->ForgeInit();
	}
}

void UB2UILobbyMain::OnClickBtnHiddenableMenu()
{

}

void UB2UILobbyMain::OnClickBtnAttendance()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->OpenUI(UIFName::AttendanceMain);
	}
}

/*
void UB2UILobbyMain::OnClickBtnChangeFocusCharacter()
{
UWorld* TheWorld = GetWorld();
if (TheWorld)
{
TheWorld->GetTimerManager().ClearTimer(EnterCharacterSelectionTH);
TheWorld->GetTimerManager().SetTimer(EnterCharacterSelectionTH, this, &UB2UILobbyMain::ExecutePendingSelectCharacterScene, EnterCharacterSelectionDelayTime, false);
}

HideAllMenus(true);
}
*/

void UB2UILobbyMain::ExecutePendingSelectCharacterScene()
{
	if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		{
			DocBattle->SetSavePCClass(DocBattle->GetCurPCClass());
			DocBattle->SetCurPCClass(PCClassToInt(EPCClass::EPC_End));
			LobbyGM->ChangeLobbyScene(ELobbyScene::ELobbyScene_LobbyMain);
		}
	}
}

void UB2UILobbyMain::OnClickBtnSlideMove()
{
	EUMGSequencePlayMode::Type PlayMode = SlideOpen ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward;
	PlayAnimation(ANIM_SlideMove.Get(), 0.0f, 1, PlayMode);
	SlideOpen = !SlideOpen;
}

void UB2UILobbyMain::OnClickBtnCombineMenu()
{
	UpdateCombineMenuVisible(true);
}

void UB2UILobbyMain::OnClickBtnUIOnOff()
{
	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
		DocUICondition->SetHideAllLobbyMainUIs(true);

	ElapsedTimeNoInput = 0.f;
}


//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UILobbyMain::OnChangedHideAllLobbyMainUIs(class UB2UIDocBase* Sender, bool HideAllLobbyMainUIs, bool PrevHideAllLobbyMainUIs)
{
	//로비에서 10초간, 아무 행동도 하지 않으면 모든 UI를 숨기라고 한다.
	HideAllMenus(HideAllLobbyMainUIs);

	if (HideAllLobbyMainUIs)
		ElapsedTimeNoInput = 0.f;
}

TWeakObjectPtr<UB2UIQuestSlot> UB2UILobbyMain::GetSuitableQuestSlot(int32 QuestSlotIndex)
{
	for (auto SuitableSlot : QuestSlots)
	{
		if (SuitableSlot->GetCurrentQuestSlot() == QuestSlotIndex ||
			(SuitableSlot.IsValid() && SuitableSlot->HasQuestInfo() == false))
			return SuitableSlot;
	}

	return nullptr;
}

void UB2UILobbyMain::InitQuestSlot(TWeakObjectPtr<class UB2UIQuestSlot>& QuestSlot, b2network::B2QuestInfoPtr QuestData)
{
	if (QuestSlot.IsValid() && QuestData != nullptr)
	{
		QuestSlot->InitQuestDefault(QuestData->slot, false);
		QuestSlot->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UILobbyMain::UpdateQuestDisplay(const TArray<b2network::B2QuestInfoPtr>& QuestList)
{
	for (auto QuestObj : QuestList)
	{
		if (QuestObj)
			UpdateSingleQuestDisplay(QuestObj->slot);
	}
}

void UB2UILobbyMain::UpdateSingleQuestDisplay(int64 QuestUniqueID)
{
	TWeakObjectPtr<UB2UIQuestSlot> SuitableSlot = GetSuitableQuestSlot(QuestUniqueID);
	if (SuitableSlot != nullptr)
	{
		SuitableSlot->InitQuestDefault(QuestUniqueID, false);
	}
}

void UB2UILobbyMain::ShowQuestNotifyDialog(const TArray<b2network::B2QuestInfoPtr>& QuestList)
{
	/*
	UB2UIQuestNotice* NoticeWidget = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestNotice>(UIFName::QuestNotice);
	if (NoticeWidget != nullptr)
	{
	NoticeWidget->InitQuestSlotNotice(QuestList);
	}
	*/
}

bool UB2UILobbyMain::GetCombineMenuIsVisible()
{
	if (auto* DocChapter = UB2UIDocHelper::GetDocUICondition())
		return DocChapter->GetIsCombineMenu();

	return false;
}

bool UB2UILobbyMain::CheckOtherPopupVisible()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();

	if (GetCombineMenuIsVisible())	return true;
	if (TutorialManager::GetInstance().HasAnyTutorial()) return true;

	if (UIMgrInst)
	{
		TArray<FName> IgnoreName;
		IgnoreName.Add(UIFName::LobbyMain);

		return UIMgrInst->GetIsOtherWidgetVisible(IgnoreName);
	}

	return false;
}

void UB2UILobbyMain::DoMarkCombineRedDot(bool CheckVisible)
{
	if (UI_CombineMenu.IsValid())
	{
		if (!CheckVisible || GetCombineMenuIsVisible())
			UI_CombineMenu->DoMarkRedDot();
	}

	DoMarkRedDot();
}
void UB2UILobbyMain::CheckConnectRewardRedDot(const AB2LobbyGameMode* LobbyGameMode)
{
	static int IndexAfterCallRedDot = -1; // 무한 갱신 방지

	const auto& PlayTimeStatus = BladeIIGameImpl::GetClientDataStore().GetPlayTimeStatus();
	const auto& RewardInfo = BladeIIGameImpl::GetClientDataStore().GetPlayTimeRewardData();

	if (IndexAfterCallRedDot != PlayTimeStatus.daily_play_time_reward_index // 최근에 호출한 인덱스와 같으면 연산 x
		&& PlayTimeStatus.daily_play_time_reward_index > 0 // 받을게 있고
		&& RewardInfo.Num() > PlayTimeStatus.daily_play_time_reward_index // 다 받지 않았고
		&& RewardInfo.IsValidIndex(PlayTimeStatus.daily_play_time_reward_index))
	{
		bool IsNewConnectReward = RewardInfo[PlayTimeStatus.daily_play_time_reward_index].play_time_inmin * 60 < UB2GameInstance::GetPlayTime();

		if (IsNewConnectReward)
		{
			DoMarkCombineRedDot();
			IndexAfterCallRedDot = PlayTimeStatus.daily_play_time_reward_index;
		}
	}
}

void UB2UILobbyMain::UpdateCombineMenuVisible(bool bVisible)
{
	if (UI_CombineMenu.IsValid())
	{
		if (bVisible)
		{
			UI_CombineMenu->OpenMenu();
			DoMarkCombineRedDot();
		}

		UI_CombineMenu->OnPlayOpenAni(bVisible);
	}

	if (bVisible)
	{
		BeginGlobalSceneBlurClass<float>::GetInstance().Signal(1.0);
		UIWidgetOpenCompleteClass<FName>::GetInstance().Signal(TEXT("CombineMenu"));
	}
	else
	{
		EndGlobalSceneBlurClass<>::GetInstance().Signal();
	}

	auto ParentVisibility = bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible;
	if (P_TotalCanvas.IsValid())
		P_TotalCanvas->SetVisibility(ParentVisibility);

	auto ButtonVisibility = bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
	if (BTN_CombineMenu.IsValid())
		BTN_CombineMenu->SetVisibility(ButtonVisibility);
}

class UB2UICombineMenu* UB2UILobbyMain::GetCombineMenuUI()
{
	if (UI_CombineMenu.IsValid())
		return UI_CombineMenu.Get();

	return nullptr;
}

void UB2UILobbyMain::CloseCombineMenu()
{
	if (GetCombineMenuIsVisible())
	{
		UpdateCombineMenuVisible(false);

		if (auto* DocChapter = UB2UIDocHelper::GetDocUICondition())
			DocChapter->SetIsCombineMenu(false);

		if (UI_CombineMenu.IsValid())
			UI_CombineMenu->CloseMenu();
	}
}

void UB2UILobbyMain::ForcedChangeFocusCharacter()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(EnterCharacterSelectionTH);
		TheWorld->GetTimerManager().SetTimer(EnterCharacterSelectionTH, this, &UB2UILobbyMain::ExecutePendingSelectCharacterScene, EnterCharacterSelectionDelayTime, false);
	}

	HideAllMenus(true);
}

void FContentOpenSlot::SetContentOpenSlot(int32 TutorialID)
{
	if (O_ContentOpen.IsValid())
	{
		bool bValidTutorial = TutorialManager::GetInstance().GetTutorialInfo(TutorialID) ? true : false;
		SetVisibile(bValidTutorial);

		if (bValidTutorial)
		{
			if (TB_ContentOpenTitle.IsValid())
				TB_ContentOpenTitle->SetText(GetTutorialTitle(TutorialID));

			if (TB_ContentOpenDesc.IsValid())
				TB_ContentOpenDesc->SetText(GetTutorialDesc(TutorialID));

			if (IMG_ContentOpen.IsValid())
				IMG_ContentOpen->SetBrushFromTexture(GetTutorialIconTexture(TutorialID));

			m_iTutorialID = TutorialID;
		}
	}
}

void FContentOpenSlot::SetVisibile(bool bVisible)
{
	if (O_ContentOpen.IsValid())
		O_ContentOpen->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UILobbyMain::OnClickBtnContentOpenSlots(int32 iArrayIndex)
{
	// 다음 튜토리얼 오픈 조건이기에 스텝 0으로 체크해도 무방하다.
	TutorialManager::OpenLockContentPopup(ContentOpenSlots[iArrayIndex].m_iTutorialID, 0);
}

void UB2UILobbyMain::OnReceiveKakaoMessageProcessing(FB2ResponseKakaoMessageProcessingPtr KakaoMessageProcessing)
{
	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE SUCESS."));
}

void UB2UILobbyMain::OnReceiveGetFeaturedMission(FB2ReciveGetFeaturedMission GetFeaturedMission)
{
	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE SUCESS."));
	//TArray<b2network::B2MissionInfoPtr> MissionInfoPtr = GET_TUPLE_DATA(FB2ResponseGetFeaturedMission::mission_infos_index, GetFeaturedMission);

	//for (auto MissionInfoSet : MissionInfoPtr)
	//{
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE mission_type.: %d"), MissionInfoSet->mission_type);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE mission_state_type.: %d"), MissionInfoSet->mission_state_type);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE mission_id.: %d"), MissionInfoSet->mission_id);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE progress_count.: %d"), MissionInfoSet->progress_count);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE completed_step.: %d"), MissionInfoSet->completed_step);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE rewarded_step.: %d"), MissionInfoSet->rewarded_step);
	//	// PROGRESS = 1;
	//	// COMPLETED = 2;
	//	// REWARDED = 3;

	//	if (MissionInfoSet->mission_state_type == 2)
	//	{
	//		switch (MissionInfoSet->mission_id)
	//		{
	//		case 1000001:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQAQ");
	//			break;
	//		case 1000002:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQAg");
	//			break;
	//		case 1000003:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQAw");
	//			break;
	//		case 1000004:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQBA");
	//			break;
	//		case 1000005:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQBQ");
	//			break;
	//		case 1000006:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQBg");
	//			break;
	//		case 1000007:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQBw");
	//			break;
	//		case 1000008:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQCA");
	//			break;
	//		case 1000009:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQCQ");
	//			break;
	//		case 1000010:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQCg");
	//			break;
	//		case 1000011:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQCw");
	//			break;
	//		case 1000012:
	//			B2P_AchievementUnlock("CgkI9auWtowCEAIQDA");
	//			break;
	//		}
	//	}
	//}
}

void UB2UILobbyMain::OnReceiveLobbyMissionAlarm()
{
	const bool bHasCompleteMission = MissionManager::GetInstance().HasCachedCompleteMission(EMissionType::Max);
	if (bHasCompleteMission)
	{
		data_trader::Retailer::GetInstance().RequestGetMission(0);
		ShowMissionDialogClass<>::GetInstance().Signal();
	}
}

void UB2UILobbyMain::CheckSurvey()
{
	if (ShouldShowCBTVersionContent())
	{
		bool CheckValue = false;
		FStageDataStore& StageDataStore = BladeIIGameImpl::GetStageDataStore();
		const FServerStageID RequireServerStageID = FServerStageID(RequireSurveyStageID);

		// Lobby에서 확인
		if (StageDataStore.IsStageClearedOnce(RequireServerStageID) || RequireServerStageID == 0)
		{
			CheckValue = true;
		}

		// StageClear UI (In Game) 에서 확인
		FServerStageID CurrClearStageID = AB2StageManager::GetCacheStageKeepEssentialData().GetServerStageID();
		CurrClearStageID = BladeIIGameImpl::GetStageDataStore().GetServerStageID(CurrClearStageID.Id % 100, EStageDifficulty(CurrClearStageID.Id / 1000));
		if ((RequireServerStageID < CurrClearStageID) || RequireServerStageID == CurrClearStageID)
		{
			CheckValue = true;
		}

		// CachedActData가 없을수도 있으니 AccountInfo에서 확인
		FServerStageID LastClearStageID = StageDataStore.GetLastClearServerStageIdByLastPlayedStageDifficulty();
		LastClearStageID = BladeIIGameImpl::GetStageDataStore().GetServerStageID(LastClearStageID.Id % 100, EStageDifficulty(LastClearStageID.Id / 1000));
		if ((RequireServerStageID < LastClearStageID) || RequireServerStageID == LastClearStageID)
		{
			CheckValue = true;
		}

		if (CheckValue)
		{
			X_BTN_Survey->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			X_BTN_Survey->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		X_BTN_Survey->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UILobbyMain::OnClickBtnSurvey()
{
	if (ShouldShowCBTVersionContent())
	{
		B2P_ShowOpenURL(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SurveyURL")).ToString());
	}
}

void UB2UILobbyMain::OnClickBtnDonationEvent()
{
	data_trader::Retailer::GetInstance().RequestGetDonationEvent();
}

void UB2UILobbyMain::ShowStoreEvent()
{
	bool bFundsSale = BladeIIGameImpl::GetClientDataStore().GetStoreFundsSaleCount();
	bool bLotterySale = BladeIIGameImpl::GetClientDataStore().GetSummonItemLotterySaleCount();

	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->SetLobbyEventLabel(bFundsSale);

	if (UIP_SummonItem_Event_Label.IsValid())
		UIP_SummonItem_Event_Label->SetLobbyEventLabel(bLotterySale);
}

#if PLATFORM_ANDROID
void UB2UILobbyMain::OnKakaoPromotionDeepLinkPlatformCallback(JNIEnv *env, jobject thiz, jint nProductCode)
{
	/*B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(nProductCode);

	m_nProductID = nProductCode;

	if (ProductInfo)
	{
		m_sMarketProductID = ProductInfo->MarketProductId;
	}

	if (B2P_GetKakaoIDPCode() == EPlatformIDPCode::Guest)
	{
		OpenGuestWarningPopup();
	}
	else
	{
		if (ProductInfo)
		{
			B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->RequestPreparePurchase(nProductCode, ProductInfo->MarketProductId);
		}
	}*/
}

#elif PLATFORM_IOS
void UB2UILobbyMain::OnKakaoDeepLink(FString DeepLink)
{
	/*FString Left;
	FString Right;
	DeepLink.Split(TEXT("/"), &Left, &Right, ESearchCase::CaseSensitive, ESearchDir::FromEnd);

	if (Left.Equals(TEXT("kakaogamelink://shop"), ESearchCase::IgnoreCase))
	{
		B2InAppPurchase::FB2CashProductInfo TempInfo;
		int32 TargetProductID = FCString::Atoi(*Right);

		B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(TargetProductID);
		if (ProductInfo != nullptr)
		{
			m_nProductID = TargetProductID;
			m_sMarketProductID = ProductInfo->MarketProductId;

			if (B2P_GetKakaoIDPCode() == EPlatformIDPCode::Guest)
			{
				OpenGuestWarningPopup();
			}
			else
			{
				B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->RequestPreparePurchase(m_nProductID, m_sMarketProductID);
			}
		}
	}*/
}
#endif

void UB2UILobbyMain::ConditionalCreateGuestWarningPopup()
{
	if (!Created_UIP_GuestWarningPopup && X_CP_GuestWarningPopup_Panel.IsValid())
	{
		Created_UIP_GuestWarningPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIMsgPopupGuestWarning>(UIP_GuestWarningPopup_Class, this, X_CP_GuestWarningPopup_Panel.Get());
		if (Created_UIP_GuestWarningPopup)
		{
			Created_UIP_GuestWarningPopup->Init();

			BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Cancel"))), &UB2UILobbyMain::CloseGuestWarningPopup);
			BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Buy"))), &UB2UILobbyMain::BuyGuestWarningPopup);
		}
	}
}

void UB2UILobbyMain::DestroyGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_GuestWarningPopup = nullptr;
	}
}

void UB2UILobbyMain::OpenGuestWarningPopup()
{
	ConditionalCreateGuestWarningPopup();
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("@@@  UB2UILobbyMain::OpenGuestWarningPopup Guest"));
	if (Created_UIP_GuestWarningPopup)
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("@@@  UB2UILobbyMain::OpenGuestWarningPopup 444444"));
		Created_UIP_GuestWarningPopup->OpenPopup();
	}
}

void UB2UILobbyMain::CloseGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->ClosePopup();
	}
}

void UB2UILobbyMain::BuyGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->ClosePopup();
	}

	//B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->RequestPreparePurchase(m_nProductID, m_sMarketProductID);
}

void UB2UILobbyMain::RequestCombatPower(EPCClass SelectPCClass)
{
	CharacterPowerSpecificArray.Empty();

	// Preset
	int32 CurrentItemPresetID = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(SelectPCClass);
	// Skill Slot
	EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(SelectPCClass);
	int32 SkillSlotID = static_cast<int32>(SlotType);

	CombatStatEval::GetReuqestPCCombatPower(SelectPCClass, CharacterPowerSpecificArray);
	/*
	CharacterPowerSpecificArray.Sort([](b2network::B2CharacterPowerSpecificPtr Elem1, b2network::B2CharacterPowerSpecificPtr Elem2)
	{
		return (Elem1->category < Elem2->category);
	});
	*/

	data_trader::Retailer::GetInstance().RequestTrackingPowerDetail(CliToSvrPCClassType(SelectPCClass), CurrentItemPresetID, SkillSlotID, CharacterPowerSpecificArray);
}

void UB2UILobbyMain::ResponseTrackingPowerDetail(FB2ResponseTrackingPowerDetailPtr Message)
{
	if (Message && Message->different_count > 0)
	{
		for (int32 i = 0; i < Message->specific_power_values.Num(); ++i)
		{
			for (int32 j = 0; j < CharacterPowerSpecificArray.Num(); ++j)
			{
				if (Message->specific_power_values[i]->category == CharacterPowerSpecificArray[j]->category 
					&& Message->specific_power_values[i]->power != CharacterPowerSpecificArray[j]->power)
				{
					BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower Error : Category(%d), Client Power(%d), Server Power(%d)")
						, CharacterPowerSpecificArray[j]->category, CharacterPowerSpecificArray[j]->power, Message->specific_power_values[i]->power)
						, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), 15, 10.0f);
				}
			}
		}
	}
}

void UB2UILobbyMain::ResponseJackpotFairy(FB2ServerCmdPtr Message)
{
	if (Message)
	{
		BII_SCREEN_LOG(FString::Printf(TEXT("ServerCmd Result[%s] Message[%s]")
			, Message->IsSuccess() ? TEXT("Success") : TEXT("Fail")
			, *FString(Message->result_message))
			, Message->IsSuccess() ? FLinearColor(0.0f, 1.0f, 0.0f, 1.0f) : FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)
			, 15, 10.0f);
	}
}

void UB2UILobbyMain::InitDonationEvent()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		EDonationEventState DonationEventState = DocDonation->GetDonationEventState();
		UpdateDonationEvent(static_cast<uint8>(DonationEventState));
	}
}

void UB2UILobbyMain::UpdateDonationEvent(uint8 InState)
{
	switch (static_cast<EDonationEventState>(InState))
	{
	case EDonationEventState::Ongoing:
		if (BTN_DonationEvent.IsValid())
		{
			BTN_DonationEvent->SetVisibility(ESlateVisibility::Visible);
		}

		if (OV_DonationTimer.IsValid())
		{
			OV_DonationTimer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		InitDonationRemainingTimer();
		break;
	case EDonationEventState::NotOngoing:
		if (BTN_DonationEvent.IsValid())
		{
			BTN_DonationEvent->SetVisibility(ESlateVisibility::Visible);
		}

		if (OV_DonationTimer.IsValid())
		{
			OV_DonationTimer->SetVisibility(ESlateVisibility::Collapsed);
		}
		InitDonationExpireTimer();
		break;
	case EDonationEventState::Expiration:
		if (BTN_DonationEvent.IsValid())
		{
			BTN_DonationEvent->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (OV_DonationTimer.IsValid())
		{
			OV_DonationTimer->SetVisibility(ESlateVisibility::Collapsed);
		}
		InitDonationStartTimer();
		break;
	case EDonationEventState::None:
		if (BTN_DonationEvent.IsValid())
		{
			BTN_DonationEvent->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (OV_DonationTimer.IsValid())
		{
			OV_DonationTimer->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	}
}

void UB2UILobbyMain::InitDonationRemainingTimer()
{
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld()))
	{
		GM->GetWorldTimerManager().ClearTimer(DonationEventTimer);
		GM->GetWorldTimerManager().SetTimer(DonationEventTimer, this, &UB2UILobbyMain::SetDonationRemainingTime, 1.0f, true);
	}

	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		DonationEventEndTime = DocDonation->GetEndTime();
	}

	SetDonationRemainingTime();
}

void UB2UILobbyMain::SetDonationRemainingTime()
{
	FDateTime DateNow = UB2GameInstance::GetLocalNow();
	FTimespan RemainingTime = DonationEventEndTime - DateNow;

	if (RemainingTime > FTimespan(0))
	{
		if (TB_DonationTimer.IsValid())
		{
			int32 RemainDay = RemainingTime.GetDays();
			int32 RemainHour = RemainingTime.GetHours() + (RemainDay * 24);
			int32 RemainMinutes = RemainingTime.GetMinutes();
			int32 RemainScond = RemainingTime.GetSeconds();

			TB_DonationTimer->SetText(FText::FromString(FString::Printf(TEXT("%d:%d:%d"), RemainHour, RemainMinutes, RemainScond)));
		}
	}
	else
	{
		EndDonationEventTimer();
		UpdateDonationEvent(static_cast<uint8>(EDonationEventState::NotOngoing));
	}
}

void UB2UILobbyMain::InitDonationStartTimer()
{
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld()))
	{
		GM->GetWorldTimerManager().ClearTimer(DonationEventTimer);
		GM->GetWorldTimerManager().SetTimer(DonationEventTimer, this, &UB2UILobbyMain::SetDonationStartTime, 1.0f, true);
	}

	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		DonationEventStartTime = DocDonation->GetStartTime();
	}
	SetDonationStartTime();
}

void UB2UILobbyMain::SetDonationStartTime()
{
	FDateTime DateNow = UB2GameInstance::GetLocalNow();
	FTimespan RemainingTime = DonationEventStartTime - DateNow;

	if (RemainingTime <= FTimespan(0))
	{
		EndDonationEventTimer();
		UpdateDonationEvent(static_cast<uint8>(EDonationEventState::Ongoing));
	}
}

void UB2UILobbyMain::InitDonationExpireTimer()
{
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld()))
	{
		GM->GetWorldTimerManager().ClearTimer(DonationEventTimer);
		GM->GetWorldTimerManager().SetTimer(DonationEventTimer, this, &UB2UILobbyMain::SetDonationExpireTime, 1.0f, true);
	}

	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		DonationEventExpireTime = DocDonation->GetExpireTime();
	}
	SetDonationExpireTime();
}

void UB2UILobbyMain::SetDonationExpireTime()
{
	FDateTime DateNow = UB2GameInstance::GetLocalNow();
	FTimespan RemainingTime = DonationEventExpireTime - DateNow;

	if (RemainingTime <= FTimespan(0))
	{
		EndDonationEventTimer();
		UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
		if (DocDonation)
		{
			UpdateDonationEvent(static_cast<uint8>(DocDonation->GetDonationEventState()));
		}
	}
}

void UB2UILobbyMain::EndDonationEventTimer()
{
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld()))
	{
		GM->GetWorldTimerManager().ClearTimer(DonationEventTimer);
	}
}
