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
#include "BladeIIUtil.h"

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
		//咯扁辑 坷锹 局丛阑 付瘤阜俊辑 谗绢 犁积秦霖促
		OnOpenComplete();
	}
	else
	{
		SetOpenState(EUIOpenState::OnOpening);

		// GetAnimation 力措肺 累悼 救 窃.
		//UWidgetAnimation* OpenCloseAnim = GetAnimation(TEXT("ANIM_OpenClose"), this);
		//if (OpenCloseAnim)
		//{
		//	//咯扁辑 坷锹 局丛阑 犁积秦霖促.

		//	//泅犁 坷锹 局丛 包访 累诀 救沁栏聪 凯赴吧肺
		//	OnOpenComplete();
		//}
		//else
		//局丛捞 绝栏搁 凯赴吧肺
		OnOpenComplete();

		//UB2UIWidget::OnOpen(bool RightNow) 俊辑绰 捞 Scope观俊 乐绰单, 荤角 OnOpen_BP()啊 坷锹 楷免阑 淬寸窍绊 乐扁 锭巩俊 肺流惑 咯扁俊 乐绰 巴捞 官恩流窃.
		//倾唱 BaseClass狼 巴捞扼 荤捞靛捞棋飘啊 堪妨登绢 快急 捞镑父 官槽促. 
		//RightNow啊 true牢 版快啊 捞镑父 唱棵淀.
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
	//	if (!DocUICondition->GetHideAllLobbyMainUIs() && ElapsedTimeNoInput >= AutoHideInterval) // 老沥 矫埃 牢遣捞 绝阑 矫 皋牢 UI 甫 濧 糠栏肺 见变促.
	//	{
	//		if (CheckOtherPopupVisible())
	//			ElapsedTimeNoInput = 0.f;
	//		else
	//			DocUICondition->SetHideAllLobbyMainUIs(true);
	//	}
	//	CheckConnectRewardRedDot(LobbyGameMode);
	//}

	//if (bWaitingForPreFetchedActInfo)
	//{ // 茅磐 轰 柳涝窍妨绊 辑滚 单捞磐 固府 夸没秦 躇绰单 UI 局聪皋捞记 场朝 锭鳖瘤 酒流 览翠 救 柯 惑炔.
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
			QuestSlot->SetVisibility(ESlateVisibility::Collapsed); // 贸澜篮 救焊捞霸 汲沥
		}
	}

	BTN_SlideButtons.AddUninitialized(MaxSlideButtonCount);
	GET_SLOT_BYNAME(UButton, BTN_SlideButtons[0], BTN_SlideButton0);	 // 阿 滚瓢甸 扁瓷捞 搬沥登搁 捞抚 函版
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
	/*Super::OnOpenComplete();

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

	BladeIIGameImpl::GetChatStore().RequestBlockUserList();*/
}

void UB2UILobbyMain::OnCloseComplete()
{
	UpdateCombineMenuVisible(false);
	CloseGuestWarningPopup();

	UnSubscribeEvent();

	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		// 酒捞袍 牢亥配府俊辑 罐疽绰单 焊包窃俺荐啊 鞘夸秦辑 肺厚唱哎锭 罐嚼聪促
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

	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory); // GameMode 率 脚龋
}

void UB2UILobbyMain::OnReceiveCheckRedDot(FB2MessageInfoResponseCheckRedDotPtr LobbyRedDotInfo)
{
	DoMarkCombineRedDot();

	// bHasUnappliedItem = GET_TUPLE_DATA(FB2ResponseCheckRedDot::has_item_random_option_change_index, LobbyRedDotInfo);
	bool bHasUnappliedItem = false;
	if (bHasUnappliedItem)	// 利侩 救茄 酒捞袍捞 乐促 沥档父 犬牢登搁 惑技胶棋 夸没
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

			// 肺厚 皋牢 霉 柳涝 楷免 敲贰弊 技泼阑 捞芭 傈俊 (FLobbyTitleScene) 茄锅 窍变 窍绰单, 
			// 弊 捞饶 免籍 览翠 坷扁 傈鳖瘤 捞固 茄 锅 焊咯林霸 登绢辑 免籍 摧篮 饶 肚 敲饭捞窍扁 困秦 咯扁辑 府悸.
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
		// 扁埃力 巢篮 矫埃
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

		if (Index == 1)	// 舅覆 浇吩捞 2俺 捞惑老 版快 付瘤阜 捞棋飘绰 阐
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

	// 譬配府倔 柳青 吝 涅胶飘 夸没篮 郴何俊辑 Slot父 劝己拳窍绊 扑诀篮 劝己拳X
	RetailerInst.RequestGetQuest();
	RetailerInst.RequestGetFriendList();
	RetailerInst.RequestGetAllReceiveAskFriend();
	RetailerInst.RequestGetAllSendAskFriend();

	RetailerInst.RequestCheckRedDot();						// LobbyMain New 盎脚

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
	// 譬配府倔 泪陛 困连 肺厚柳涝矫 公炼扒 茄锅 府橇饭矫
	if (TutorialManager::GetInstance().IsTutorialAvailable())
		TutorialManager::GetInstance().RefreshContentLockUI();

#if !PLATFORM_MAC
	// 牢聚搬力 檬扁拳
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
	// 父乏捞搁 剁快淋.
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

// 菩哦 罐阑锭尔 滚瓢鸥捞赣 倒疽阑锭 笛促 龋免窃.
void UB2UILobbyMain::ExecutePendingDungeonSceneInner()
{
	// 鸥固绢档 倒备 菩哦档 罐酒具 角青
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

// 菩哦 罐阑锭尔 滚瓢鸥捞赣 倒疽阑锭 笛促 龋免窃.
void UB2UILobbyMain::ExecutePendingPVPSceneInner()
{
	// 鸥固绢档 倒备 菩哦档 罐酒具 角青
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
		// PreFetchAdventureSceneInfo 甫 茄 惑炔俊辑 酒流 览翠捞 坷瘤 臼篮 芭..
		bWaitingForPreFetchedActInfo = true;
	}
	else
	{
		// 弥辟 敲饭捞茄 胶抛捞瘤 沥焊
		FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();

		FServerStageID ServerStageID = StageStore.GetLastPlayedServerStageId();
		int32 ChapterNumber = StageStore.GetActIdByClientStageId(StageStore.GetClientStageId(ServerStageID));
		EStageDifficulty StageDifficulty = StageStore.GetStageDifficulty(ServerStageID);

		if (ServerStageID == 0)	// 辑滚俊 历厘等 弥辟 敲饭捞 沥焊啊 0 老锭. 碍力肺 1-1肺 技泼
		{
			ChapterNumber = 1;
			StageDifficulty = EStageDifficulty::ESD_Normal;
		}
		else if (ChapterNumber > 5)
		{
			ChapterNumber = 5;
			StageDifficulty = EStageDifficulty::ESD_Hell;
		}

		// PreFetchAdventureSceneInfo 甫 沁促搁 官肺 茅磐 轰肺 逞绢哎 波绊 酒聪搁 咯扁辑 辑滚 沥焊 夸没阑 窍霸 瞪 波.
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::AdventureEnterBattleMain);
		// BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(ChapterNumber, StageDifficulty);

		bWaitingForPreFetchedActInfo = false; // UI 厘搁 逞绢啊摆瘤父 弊贰档 付农.
	}
}
void UB2UILobbyMain::PreFetchAdventureSceneInfo()
{
	// 茅磐甘 轰 柳涝 捞傈俊 UI 局聪皋捞记阑 困秦 Pending 阑 矫虐绊 酒流 单捞磐甫 救 罐篮 惑炔捞搁 辑滚俊 单捞磐甫 夸没鳖瘤 窍绰单 
	// 捞霸 矫埃 扯厚扼 UI 局聪皋捞记 窍绰 客吝俊 辑滚俊 单捞磐 夸没阑 固府 秦 滴绰 狼固.

	// CheckAnGoToChapterView 俊辑 夸没且 沥焊客 付蛮啊瘤咯具. 弊犯瘤父 父老 酒聪扼绊 秦档 奴老唱绰 扒 酒丛.
	FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();

	FServerStageID ServerStageID = StageStore.GetLastPlayedServerStageId();
	int32 ChapterNumber = StageStore.GetActIdByClientStageId(StageStore.GetClientStageId(ServerStageID));
	EStageDifficulty StageDifficulty = StageStore.GetStageDifficulty(ServerStageID);

	if (ServerStageID == 0)// 辑滚俊 历厘等 弥辟 敲饭捞 沥焊啊 0 老锭. 碍力肺 1-1肺 技泼
	{
		ChapterNumber = 1;
		StageDifficulty = EStageDifficulty::ESD_Normal;
	}

	// RequestGetActInfo 父 窍绊 览翠矫 茅磐 轰肺 啊绰 扒 酒丛. 弊扒 RequestGetActInfoAndChangeChapter 俊辑..
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
	// Widget Anim 敲饭捞 饶俊 角力 Challenge 拳搁俊 柳涝窍档废 抗距父 秦 敌促.
	if (!bPendingEnterGameSelection)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			// 档傈 葛靛牢器 夸没
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
	// Widget Anim 敲饭捞 饶俊 角力 Challenge 拳搁俊 柳涝窍档废 抗距父 秦 敌促.
	if (!bPendingEnterGameSelection)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			// 搬捧 葛靛牢器 夸没
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
	// Widget Anim 敲饭捞 饶俊 角力 Adventure 拳搁俊 柳涝窍档废 抗距父 秦 敌促.
	if (!bPendingEnterGameSelection)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter();
			if (DocChapter && DocChapter->GetCurChapterNum() <= 0)
			{
#if WITH_EDITOR && !PLATFORM_MAC// 酒隔尔 ばば
				//UE_LOG(LogBladeII, Warning, TEXT("泅犁 甸绢啊绊磊 窍绰 Chapter 锅龋啊 檬扁拳 登瘤 臼篮 惑怕.. 攫力何畔啊 肺厚俊辑 葛氰甘 贸澜 柳涝窍搁 Chapter 锅龋啊 0 牢盲肺 吧府辨贰.. 公郊 老捞 积变 扒瘤.."));
#else
				//UE_LOG(LogBladeII, Warning, TEXT("Trying to open Chapter scene while chapter data is not initialized.. Update it on demand but it doesn't look so fine."));
#endif
				if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					LobbyGM->UpdateChapterData();
				}
			}

			PreFetchAdventureSceneInfo(); // ExecutePendingAdventureScene 鳖瘤 矫埃捞 乐栏骨肺 弊 货 ActInfo 单捞磐甫 固府 夸没秦 狄
			TheWorld->GetTimerManager().SetTimer(EnterGameSelectionTH, this, &UB2UILobbyMain::ExecutePendingAdventureScene, EnterGameSelectionDelayTime, false);
			bPendingEnterGameSelection = true;
		}
	}
}

void UB2UILobbyMain::ChangeChaperScene(int32 ChapterNum)
{
	// LobbyChangeSceneByUISceneClass 措脚 茅磐 锅龋甫 疙矫窍绰 茅磐 傈侩 捞亥飘甫 朝赴促.
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

	//譬配府倔 柳青 吝 牢亥配府 柳涝矫 MainPC函版阑 啊厘 付瘤阜俊 龋免窍绰 何盒 唱吝俊 函版 瞪 荐 档 乐促.
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

	//UB2UIItemForge* ItemForgeUI = UB2UIManager::GetInstance()->GetUI<UB2UIItemForge>(UIFName::ItemForge);
	//if (ItemForgeUI)
	//{
	//	ItemForgeUI->ForgeInit();
	//}
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
	//肺厚俊辑 10檬埃, 酒公 青悼档 窍瘤 臼栏搁 葛电 UI甫 见扁扼绊 茄促.
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
//void UB2UILobbyMain::CheckConnectRewardRedDot(const AB2LobbyGameMode* LobbyGameMode)
//{
//	static int IndexAfterCallRedDot = -1; // 公茄 盎脚 规瘤
//
//	const auto& PlayTimeStatus = BladeIIGameImpl::GetClientDataStore().GetPlayTimeStatus();
//	const auto& RewardInfo = BladeIIGameImpl::GetClientDataStore().GetPlayTimeRewardData();
//
//	if (IndexAfterCallRedDot != PlayTimeStatus.daily_play_time_reward_index // 弥辟俊 龋免茄 牢郸胶客 鞍栏搁 楷魂 x
//		&& PlayTimeStatus.daily_play_time_reward_index > 0 // 罐阑霸 乐绊
//		&& RewardInfo.Num() > PlayTimeStatus.daily_play_time_reward_index // 促 罐瘤 臼疽绊
//		&& RewardInfo.IsValidIndex(PlayTimeStatus.daily_play_time_reward_index))
//	{
//		bool IsNewConnectReward = RewardInfo[PlayTimeStatus.daily_play_time_reward_index].play_time_inmin * 60 < UB2GameInstance::GetPlayTime();
//
//		if (IsNewConnectReward)
//		{
//			DoMarkCombineRedDot();
//			IndexAfterCallRedDot = PlayTimeStatus.daily_play_time_reward_index;
//		}
//	}
//}

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
	// 促澜 譬配府倔 坷锹 炼扒捞扁俊 胶跑 0栏肺 眉农秦档 公规窍促.
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

		// Lobby俊辑 犬牢
		if (StageDataStore.IsStageClearedOnce(RequireServerStageID) || RequireServerStageID == 0)
		{
			CheckValue = true;
		}

		// StageClear UI (In Game) 俊辑 犬牢
		FServerStageID CurrClearStageID = AB2StageManager::GetCacheStageKeepEssentialData().GetServerStageID();
		CurrClearStageID = BladeIIGameImpl::GetStageDataStore().GetServerStageID(CurrClearStageID.Id % 100, EStageDifficulty(CurrClearStageID.Id / 1000));
		if ((RequireServerStageID < CurrClearStageID) || RequireServerStageID == CurrClearStageID)
		{
			CheckValue = true;
		}

		// CachedActData啊 绝阑荐档 乐栏聪 AccountInfo俊辑 犬牢
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
	//if (!Created_UIP_GuestWarningPopup && X_CP_GuestWarningPopup_Panel.IsValid())
	//{
	//	Created_UIP_GuestWarningPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIMsgPopupGuestWarning>(UIP_GuestWarningPopup_Class, this, X_CP_GuestWarningPopup_Panel.Get());
	//	if (Created_UIP_GuestWarningPopup)
	//	{
	//		Created_UIP_GuestWarningPopup->Init();

	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Cancel"))), &UB2UILobbyMain::CloseGuestWarningPopup);
	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Buy"))), &UB2UILobbyMain::BuyGuestWarningPopup);
	//	}
	//}
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
