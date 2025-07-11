//// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_ItemEnhanceChangeMain.h"
////#include "BladeII.h"
//
//#include "B2LobbyGameMode.h"
//#include "B2LobbyInventory.h"
//#include "B2UIManager_Lobby.h"
//#include "Event.h"
//#include "BladeIIGameImpl.h"
//#include "B2DynItemIcon_LobbyInven.h"
//#include "B2UIShortagePopupManager.h"
//#include "B2LobbyUI_ItemEnhUpgCommon.h"
//#include "Retailer.h"
//#include "B2MessageInfo.h"
//#include "CommonStruct.h"
//#include "B2ClientDataStore.h"
//#include "B2ItemInfo.h"
//
//void UB2LobbyUI_ItemEnhanceChangeMain::CacheAssets()
//{
//	GET_SLOT(UB2Button, BTN_Close)
//	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemEnhanceChangeMain::OnCloseButtonPressed)
//
//	GET_SLOT(UB2Button, BTN_IngredUnselect)
//	BIND_CLICK_FUNC_TO_BTN(BTN_IngredUnselect, &UB2LobbyUI_ItemEnhanceChangeMain::OnClickIngredUnselectBtn)
//
//	GET_SLOT(UB2Button, BTN_EnhanceChange)
//	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceChange, &UB2LobbyUI_ItemEnhanceChangeMain::OnClickEnhanceChangeBtn)
//
//	GET_SLOT(UB2Button, BTN_SkipResult)
//	BIND_CLICK_FUNC_TO_BTN(BTN_SkipResult, &UB2LobbyUI_ItemEnhanceChangeMain::OnClickSkipResultBtn)
//
//	GET_SLOT(UCanvasPanel, CP_TargetItemIconSet)
//	GET_SLOT(UCanvasPanel, CP_IngredItemIconSet)
//
//	GET_SLOT(UPanelWidget, P_SelectDescription)
//	GET_SLOT(UPanelWidget, P_UnselectDescription)
//	GET_SLOT(UPanelWidget, P_ResultDescription)
//
//	GET_SLOT(UTextBlock, TB_BeforeLeftEnhance)
//	GET_SLOT(UTextBlock, TB_BeforeEnhance)
//	GET_SLOT(UTextBlock, TB_BeforeRightEnhance)
//
//	GET_SLOT(UTextBlock, TB_AfterLeftEnhance)
//	GET_SLOT(UTextBlock, TB_AfterRightEnhance)
//	GET_SLOT(UTextBlock, TB_AfterEnhance)
//
//	GET_SLOT(UTextBlock, TB_LeftResultEnhance)
//	GET_SLOT(UTextBlock, TB_LeftResultEnhanceBefore)
//	GET_SLOT(UTextBlock, TB_CurrentEnhance)
//	GET_SLOT(UTextBlock, TB_RightResultEnhance)
//	GET_SLOT(UTextBlock, TB_RightResultEnhanceBefore)
//
//	GET_SLOT(UTextBlock, TB_LeftResultAttack)
//	GET_SLOT(UTextBlock, TB_LeftResultAttackBefore)
//	GET_SLOT(UTextBlock, TB_CurrentAttack)
//	GET_SLOT(UTextBlock, TB_RightResultAttack)
//	GET_SLOT(UTextBlock, TB_RightResultAttackBefore)
//
//	GET_SLOT(UTextBlock, TB_EnhanceChangeCost);
//	GET_SLOT(UTextBlock, TB_EnhanceChangeTitle);
//	GET_SLOT(UTextBlock, TB_EnhanceChangeBtn);
//	GET_SLOT(UTextBlock, TB_NoIngredMessage);
//	GET_SLOT(UTextBlock, TB_ComposeDescription);
//
//	GET_SLOT(UImage, IMG_EnhanceChangeIcon);
//	GET_SLOT(UImage, IMG_BackGround);
//
//	GET_SLOT(UWidgetAnimation, ANIM_EnhanceChange)
//
//	Issues.Add(DeliveryItemEnhanceLevelClass<FB2ItemEnhanceLevel>::GetInstance().Subscribe2(
//			[this](FB2ItemEnhanceLevel InEnhanceLevelInfo)
//	{
//		this->DeliveryItemEnhanceLevel(InEnhanceLevelInfo);
//	}
//	));
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
//{
//	Super::StartFromLobby(InUIManager, InGM);
//
//	CachedInventory = CachedLobbyGM->GetLobbyInventory();
//	check(CachedInventory);
//
//	// 이 시점에 Inventory 의 TargetItemID 는 세팅되어 있어야 함. (UB2LobbyInventory::EnterItemOpMode)
//	if (!CachedInventory->GetItemOPTargetItemData(TargetItem) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_EnhancementChange)
//	{
//		CloseAndQuitItemOpMode(); // 만에 하나 실패한다면.. 어쩔 수 없지.
//		return;
//	}
//
//	SetPositionInViewport(FVector2D(0.0f, 0.0f));
//
//	UpdateDescriptionState(DescriptionState::BEFRE);
//	UpdateItemIcons();
//	UpdateIngredItemIcon();
//	UpdateStaticText();
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::DestroySelf()
//{
//	Super::DestroySelf();
//
//	EnableInventoryTouchClass<bool>::GetInstance().Signal(false);
//	DestroyTargetItemIcon();
//	DestroyIngredItemIcon();
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::DestroyTargetItemIcon()
//{
//	if (CreatedTargetItemIcon)
//	{
//		CreatedTargetItemIcon->DestroySelf();
//		CreatedTargetItemIcon = NULL;
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::DestroyIngredItemIcon()
//{
//	if (CreatedIngredItemIcon)
//	{
//		CreatedIngredItemIcon->DestroySelf();
//		CreatedIngredItemIcon = NULL;
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::UpdateStaticText()
//{
//	if (TB_EnhanceChangeTitle.IsValid())
//	{
//		TB_EnhanceChangeTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhancementExchange_Title")));
//	}
//
//	if (TB_EnhanceChangeBtn.IsValid())
//	{
//		TB_EnhanceChangeBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhancementExchange_Title")));
//	}
//
//	if (TB_NoIngredMessage.IsValid())
//	{
//		TB_NoIngredMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhancementExchange_Guide_1")));
//	}
//
//	if (TB_ComposeDescription.IsValid())
//	{
//		TB_ComposeDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhancementExchange_Guide_2")));
//	}
//
//	if (TB_CurrentEnhance.IsValid())
//	{
//		TB_CurrentEnhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhancementExchange_Result_1")));
//	}
//
//	if (TB_CurrentAttack.IsValid())
//	{
//		TB_CurrentAttack->SetText(GetLOCTextOfPrimPointType(TargetItem.PrimaryPointType));
//	}
//
//	// 재화 아이콘, 현재는 다크크리스탈 고정
//	// RequestExchangeItemEnhanceLevel과 아이콘 수정하면 확장완료
//	if (IMG_EnhanceChangeIcon.IsValid())
//	{
//		auto* ItemInfoTable = StaticFindItemInfo();
//		IMG_EnhanceChangeIcon->SetBrushFromMaterial(ItemInfoTable->GetItemIcon(FItemRefIDHelper::GetGoodsID_DarkCrystal()));
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::UpdateStateChange()
//{
//	FindIngredItem();
//	UpdateIngredItemIcon();
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::OnCloseButtonPressed()
//{
//	CloseAndQuitItemOpMode();
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::OnClickIngredUnselectBtn()
//{
//	if (CachedInventory && CachedInventory->GetCurrentSelectIngredItemNum() > 0)
//	{
//		TArray<int64> IngredIDs;
//		CachedInventory->GetAllSelectedItemOpIngreds(IngredIDs);
//		// 물론 여기선 IngredIDs 는 하나여야
//		if (IngredIDs.Num() > 0)
//		{
//			UnSelectForEnhanceChangeIngredClass<int64>::GetInstance().Signal(IngredIDs[0]);
//		}
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::OnClickEnhanceChangeBtn()
//{
//		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
//			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
//			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhancementExchange_Guide_3")),
//			0.f,
//			true,
//			true,
//			EUIMsgPopupButtonGroup::YesOrNo,
//			FMsgPopupOnClick::CreateLambda([this]() {
//			//PlayEnhanceChangeAnimation();
//			RequestEnhanceChange();
//		})
//		);
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::OnClickSkipResultBtn()
//{
//	OnCloseButtonPressed();
//
//	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
//
//	if (LobbyGameMode)
//	{
//		UB2UIManager_Lobby* LobbyManager = LobbyGameMode->DJLegacy_GetLobbyUIManager();
//
//		if (LobbyManager)
//		{
//			LobbyManager->OpenItemDetailPopup(TargetItem.InstanceUID);
//		}
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::OnFinishEnhanceChange()
//{
//	UpdateItemIcons();
//	UpdateStateChange();
//	UpdateDescriptionState(DescriptionState::RESULT);
//
//	if (IsEquipmentItem(TargetItem))
//	{
//		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(static_cast<EPCClass>(TargetItem.AllowedPCClass), true);
//	}
//
//	if (IMG_BackGround.IsValid())
//	{
//		IMG_BackGround->SetVisibility(ESlateVisibility::Collapsed);
//	}
//
//	EnableInventoryTouchClass<bool>::GetInstance().Signal(false);
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::CloseAndQuitItemOpMode()
//{
//	QuitItemOpModeClass<bool>::GetInstance().Signal(false);
//	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::UpdateItemIcons()
//{
//	DestroyTargetItemIcon();
//
//	if (ItemIconWidgetClass == NULL || !CP_TargetItemIconSet.IsValid() || TargetItem.InstanceUID <= 0 || TargetItem.ItemRefID <= 0) {
//		return;
//	}
//
//	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
//	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_TargetItemIconSet->Slot);
//	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
//
//	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());
//
//	// ItemIconPanelNRef 위에 최종적인 아이템 아이콘ItemIconWidgetClass widget 생성
//	CreatedTargetItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
//		ItemIconWidgetClass, this, CP_TargetItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
//		FVector2D(0.0f, 0.0f), 0, false
//	));
//
//	if (CreatedTargetItemIcon)
//	{
//		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget);
//		CreatedTargetItemIcon->UpdateItemData(TargetItem);
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::UpdateDescriptionState(DescriptionState InState)
//{
//	if (P_UnselectDescription.IsValid())
//	{
//		P_UnselectDescription->SetVisibility(InState == DescriptionState::BEFRE ?  ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
//	}
//
//	if (P_SelectDescription.IsValid())
//	{
//		P_SelectDescription->SetVisibility(InState == DescriptionState::AFTER ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
//	}
//
//	if (P_ResultDescription.IsValid())
//	{
//		P_ResultDescription->SetVisibility(InState == DescriptionState::RESULT ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
//	}
//
//	if (BTN_EnhanceChange.IsValid())
//	{
//		BTN_EnhanceChange->SetVisibility(InState == DescriptionState::RESULT ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
//	}
//
//	if (BTN_SkipResult.IsValid())
//	{
//		BTN_SkipResult->SetVisibility(InState == DescriptionState::RESULT ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
//	}
//
//	switch (InState)
//	{
//	case UB2LobbyUI_ItemEnhanceChangeMain::BEFRE:
//		UpdateUnselectDescription();
//		break;
//	case UB2LobbyUI_ItemEnhanceChangeMain::AFTER:
//		UpdateSelectDescription();
//		break;
//	case UB2LobbyUI_ItemEnhanceChangeMain::RESULT:
//		UpdateResultDescription();
//		break;
//	default:
//		break;
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::UpdateUnselectDescription()
//{
//	if (TB_EnhanceChangeCost.IsValid())
//	{
//		TB_EnhanceChangeCost->SetText(FText::FromString(TEXT("-")));
//	}
//
//	if (BTN_EnhanceChange.IsValid())
//	{
//		BTN_EnhanceChange->SetIsEnabled(false);
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::UpdateSelectDescription()
//{
//	if (TB_BeforeLeftEnhance.IsValid())
//	{
//		TB_BeforeLeftEnhance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel"))
//			, FText::AsNumber(TargetItem.EnhanceLevel)));
//	}
//
//	if (TB_BeforeRightEnhance.IsValid())
//	{
//		TB_BeforeRightEnhance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel"))
//			, FText::AsNumber(SourceItem.EnhanceLevel)));
//	}
//
//	if (TB_AfterLeftEnhance.IsValid())
//	{
//		TB_AfterLeftEnhance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel"))
//			, FText::AsNumber(SourceItem.EnhanceLevel)));
//	}
//
//	if (TB_AfterRightEnhance.IsValid())
//	{
//		TB_AfterRightEnhance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel"))
//			, FText::AsNumber(TargetItem.EnhanceLevel)));
//	}
//
//	if (TB_BeforeEnhance.IsValid())
//	{
//
//	}
//	if (TB_AfterEnhance.IsValid())
//	{
//	}
//
//	if(TB_EnhanceChangeCost.IsValid())
//	{
//		int32 InLevel = SourceItem.EnhanceLevel > TargetItem.EnhanceLevel ? SourceItem.EnhanceLevel : TargetItem.EnhanceLevel;
//		FB2EnhanceItemCost CostInfo = BladeIIGameImpl::GetClientDataStore().GetEnhanceCostData(InLevel);
//
//		TB_EnhanceChangeCost->SetText(FText::AsNumber(CostInfo.CostDarkCrystalForExchange));
//	}
//
//	if (BTN_EnhanceChange.IsValid())
//	{
//		BTN_EnhanceChange->SetIsEnabled(true);
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::UpdateResultDescription()
//{
//	const bool IsTargetPlus = TargetItem.Power > TargetItemAttackPower;
//	const bool IsSourcePlus = SourceItem.Power > SourceItemAttackPower;
//
//	if (TB_LeftResultEnhance.IsValid())
//	{
//		TB_LeftResultEnhance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL,TEXT("General_Stat_EnhanceLevel"))
//			, FText::AsNumber(TargetItem.EnhanceLevel)));
//	}
//
//	if (TB_LeftResultEnhanceBefore.IsValid())
//	{
//		const int32 CompareEnhancValue = FMath::Abs(TargetItemEnhanceLevel - TargetItem.EnhanceLevel);
//
//		TB_LeftResultEnhanceBefore->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, IsTargetPlus ? TEXT("LobbyInvenText_IncreaseStat") : TEXT("EnhancementExchange_LevelDecrease"))
//			, FText::AsNumber(CompareEnhancValue)));
//
//		TB_LeftResultEnhanceBefore->SetColorAndOpacity(IsTargetPlus ? FLinearColor::Green : FLinearColor::Red);
//	}
//
//	if (TB_RightResultEnhance.IsValid())
//	{
//		TB_RightResultEnhance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel"))
//			, FText::AsNumber(SourceItem.EnhanceLevel)));
//	}
//
//	if (TB_RightResultEnhanceBefore.IsValid())
//	{
//		const int32 CompareEnhancValue = FMath::Abs(SourceItemEnhanceLevel - SourceItem.EnhanceLevel);
//
//		TB_RightResultEnhanceBefore->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, IsSourcePlus ? TEXT("LobbyInvenText_IncreaseStat") : TEXT("EnhancementExchange_LevelDecrease"))
//			, FText::AsNumber(CompareEnhancValue)));
//
//		TB_RightResultEnhanceBefore->SetColorAndOpacity(IsSourcePlus ? FLinearColor::Green : FLinearColor::Red);
//	}
//
//	if (TB_LeftResultAttack.IsValid())
//	{
//		TB_LeftResultAttack->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel"))
//			, FText::AsNumber(static_cast<int32>(TargetItem.Power))));
//	}
//
//	if (TB_LeftResultAttackBefore.IsValid())
//	{
//		const int32 CompareAttackPower = FMath::Abs(TargetItemAttackPower - TargetItem.Power);
//
//		TB_LeftResultAttackBefore->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, IsTargetPlus ? TEXT("LobbyInvenText_IncreaseStat") : TEXT("EnhancementExchange_LevelDecrease"))
//			, FText::AsNumber(CompareAttackPower)));
//
//		TB_LeftResultAttackBefore->SetColorAndOpacity(IsTargetPlus ? FLinearColor::Green : FLinearColor::Red);
//	}
//
//	if (TB_RightResultAttack.IsValid())
//	{
//		TB_RightResultAttack->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel"))
//			, FText::AsNumber(static_cast<int32>(SourceItem.Power))));
//	}
//
//	if (TB_RightResultAttackBefore.IsValid())
//	{
//		const int32 CompareAttackPower = FMath::Abs(SourceItemAttackPower - SourceItem.Power);
//
//		TB_RightResultAttackBefore->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, IsSourcePlus ? TEXT("LobbyInvenText_IncreaseStat") : TEXT("EnhancementExchange_LevelDecrease"))
//			, FText::AsNumber(CompareAttackPower)));
//
//		TB_RightResultAttackBefore->SetColorAndOpacity(IsSourcePlus ? FLinearColor::Green : FLinearColor::Red);
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::FindIngredItem()
//{
//	SourceItem.Empty();
//	CurrIngredIDs.Empty();
//
//	CachedInventory->GetAllSelectedItemOpIngreds(CurrIngredIDs);
//
//	if (CurrIngredIDs.Num() > 0)
//	{
//		UB2LobbyInventory::FindFromAll(SourceItem, CurrIngredIDs[0]);
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::RequestEnhanceChange()
//{
//	//// 1: Gem , 3: DarkCrystal
//	data_trader::Retailer::GetInstance().RequestExchangeItemEnhanceLevel(TargetItem.InstanceUID, SourceItem.InstanceUID, 3);
//
//	if (BTN_IngredUnselect.IsValid())
//	{
//		BTN_IngredUnselect->SetVisibility(ESlateVisibility::HitTestInvisible);
//	}
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::PlayEnhanceChangeAnimation()
//{
//	if (ANIM_EnhanceChange.IsValid())
//	{
//		PlayAnimation(ANIM_EnhanceChange.Get());
//		OnPlayAnimationSound();
//
//		if (IMG_BackGround.IsValid())
//		{
//			IMG_BackGround->SetVisibility(ESlateVisibility::Visible);
//		}
//	}
//
//	BladeIIGameImpl::GetClientDataStore().EnhanceChangeItem(TargetItem, SourceItem);
//
//	CachedInventory->UpdateSingleItemData(TargetItem.InstanceUID, TargetItem);
//	CachedInventory->UpdateSingleItemData(SourceItem.InstanceUID, SourceItem);
//
//	EnableInventoryTouchClass<bool>::GetInstance().Signal(true);
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::DeliveryItemEnhanceLevel(FB2ItemEnhanceLevel InEnhanceLevelInfo)
//{
//	TargetItemEnhanceLevel = TargetItem.EnhanceLevel;
//	SourceItemEnhanceLevel = SourceItem.EnhanceLevel;
//
//	TargetItemAttackPower = TargetItem.Power;
//	SourceItemAttackPower = SourceItem.Power;
//
//	//const int32 CurrentDarkCrystal = GET_TUPLE_DATA(FB2ResponseExchangeItemEnhanceLevel::current_dark_crystal_index, InEnhanceLevelInfo);
//	//const int32 CurrentGem = GET_TUPLE_DATA(FB2ResponseExchangeItemEnhanceLevel::current_gem_index, InEnhanceLevelInfo);
//	//
//	//BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::DarkGem, CurrentDarkCrystal);
//	//BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, CurrentGem);
//
//	//TargetItem = GET_TUPLE_DATA(FB2ResponseExchangeItemEnhanceLevel::item_data_1_index, InEnhanceLevelInfo);
//	//SourceItem = GET_TUPLE_DATA(FB2ResponseExchangeItemEnhanceLevel::item_data_2_index, InEnhanceLevelInfo);
//
//	PlayEnhanceChangeAnimation();
//}
//
//void UB2LobbyUI_ItemEnhanceChangeMain::UpdateIngredItemIcon()
//{
//	DestroyIngredItemIcon();
//
//	if (CurrIngredIDs.Num() > 0 && SourceItem.StarGrade != 0)
//	{
//		UpdateDescriptionState(DescriptionState::AFTER);
//
//		UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_IngredItemIconSet->Slot);
//
//		FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
//		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());
//
//		// MainPanelNativeRef 위에 최종적인 아이템 아이콘 widget 생성
//		CreatedIngredItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
//			ItemIconWidgetClass, this, CP_IngredItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
//			FVector2D(0.0f, 0.0f), 0, false
//		));
//		if (CreatedIngredItemIcon)
//		{
//			CreatedIngredItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ComposeOrSurpassIngredient);
//			CreatedIngredItemIcon->UpdateItemData(SourceItem); // 개별 아이템 정보를 넘겨준다.
//		}
//	}
//	else
//	{
//		UpdateDescriptionState(DescriptionState::BEFRE);
//	}
//
//	CurrIngredIDs.Empty();
//}