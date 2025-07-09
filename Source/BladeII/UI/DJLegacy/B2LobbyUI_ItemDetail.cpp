// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_ItemDetail.h"
#include "B2UIManager_Lobby.h"
#include "B2Inventory.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2LobbyUISwitcher.h"
#include "Event.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2DynText_ItemDetailOption.h"
#include "B2DynText_Multiple.h"
#include "B2LobbyUI_ItemDetailSetEffect.h"
#include "B2LobbyUI_SellItemPopup.h"
#include "B2UIItemOptionChange.h"
#include "B2UIDecompositionPopup.h"
#include "TutorialManager.h"
#include "B2ItemInfo.h"
#include "B2UILinkManager.h"
#include "B2UICreateNickname.h"
#include "B2UIManager.h"
#include "B2GoodsTypeInfo.h"
#include "B2UIUtil.h"
#include "Retailer.h"
#include "B2UIItemDetailOption.h"
#include "BladeIIGameImpl.h"
#include "B2UISealEquipmentMain.h"
#include "B2LobbyUI_ConvertItemPopup.h"
#include "B2UIDocHelper.h"
#include "B2UIBackWidgetBase.h"
#include "B2UIHeader.h"

UB2LobbyUI_ItemDetail::UB2LobbyUI_ItemDetail(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsContainsPresetItem = false;
	bIsContainsOtherPreset = false;
	bIsForEquippedItem = true;
	CachedInventory = NULL;

	ItemIconWidgetClass = NULL;
	CreatedItemIcon = NULL;
	
	ItemStatIncColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));
	ItemStatSameColor = FSlateColor(FLinearColor(10.0f, 10.0f, 10.0f));
	ItemStatDecColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f));

	SetEffectInfoDisplayClass = NULL;
	CreatedSpecialInfoDisplay = NULL;

	DecompositionEquipItemEventID = 0;

	CachedDataStore = &BladeIIGameImpl::GetClientDataStore();
}

void UB2LobbyUI_ItemDetail::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);
}

void UB2LobbyUI_ItemDetail::NativeConstruct()
{
	Super::NativeConstruct();
}

void UB2LobbyUI_ItemDetail::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_P_ItemIconPanel)

	GET_SLOT(UTextBlock, TB_PrimaryPointType)

	GET_SLOT(UTextBlock, TB_PrimaryPoint)
	GET_SLOT(UTextBlock, TB_PrimaryPointDelta)

	GET_SLOT(UTextBlock, TB_Combat)
	GET_SLOT(UTextBlock, TB_CombatPower)
	GET_SLOT(UTextBlock, TB_CombatPowerDelta)

	GET_SLOT(UTextBlock, TB_ItemLevel)
	GET_SLOT(UProgressBar, PB_ItemExpProgress)
	GET_SLOT(UTextBlock, TB_ItemExpPercent)

	GET_SLOT(UTextBlock, TB_Enhance)
	GET_SLOT(UTextBlock, TB_EnhanceLevel)
	GET_SLOT(UB2RichTextBlock, RTB_ItemDescription)

	GET_SLOT(UTextBlock, TB_ItemDesc)

	GET_SLOT(UTextBlock, TB_ShowChance)
	GET_SLOT(UTextBlock, TB_SealBoxDesc)
	GET_SLOT(UTextBlock, TB_SealBox_Contain)
	GET_SLOT(UTextBlock, TB_SealBox_ButtonSet)
	GET_SLOT(UB2RichTextBlock, RTB_SealBoxDescription)
	GET_SLOT(UB2ScrollBox, SB_SealBox_ContainList)

	GET_SLOT(UButton, BTN_ShowChance)
	BIND_CLICK_FUNC_TO_BTN(BTN_ShowChance, &UB2LobbyUI_ItemDetail::OnShowChanceBTNClicked)
	GET_SLOT(UButton, BTN_SealBoxClass)
	BIND_CLICK_FUNC_TO_BTN(BTN_SealBoxClass, &UB2LobbyUI_ItemDetail::OnSealBoxClassBTNClicked)

	// Bottom Button Set
	GET_SLOT(UButton, BTN_Unseal)
	BIND_CLICK_FUNC_TO_BTN(BTN_Unseal, &UB2LobbyUI_ItemDetail::OnUnsealButtonClicked)
	GET_SLOT(UTextBlock, TB_Unseal)

	GET_SLOT(UButton, BTN_Convert)
	BIND_CLICK_FUNC_TO_BTN(BTN_Convert, &UB2LobbyUI_ItemDetail::OnConvertButtonClicked)
	GET_SLOT(UTextBlock, TB_Convert)

	GET_SLOT(UButton, BTN_Decomposition)
	BIND_CLICK_FUNC_TO_BTN(BTN_Decomposition, &UB2LobbyUI_ItemDetail::OnDecompositionButtonClicked)
	GET_SLOT(UTextBlock, TB_Decomposition)

	GET_SLOT(UButton, BTN_Use)
	BIND_CLICK_FUNC_TO_BTN(BTN_Use, &UB2LobbyUI_ItemDetail::OnUseButtonClicked)
	GET_SLOT(UTextBlock, TB_UseBtn)

	GET_SLOT(UButton, BTN_Sell)
	BIND_CLICK_FUNC_TO_BTN(BTN_Sell, &UB2LobbyUI_ItemDetail::OnSellButtonClicked)
	GET_SLOT(UTextBlock, TB_SellBtn)

	GET_SLOT(UButton, BTN_EquipOrRelease)
	BIND_CLICK_FUNC_TO_BTN(BTN_EquipOrRelease, &UB2LobbyUI_ItemDetail::OnEquipOrReleaseButtonClicked)
	GET_SLOT(UTextBlock, TB_EquipOrReleaseBtn)

	// Left Button Set
	GET_SLOT(USizeBox, SB_Upgrade)
	GET_SLOT(UButton, BTN_Upgrade)
	BIND_CLICK_FUNC_TO_BTN(BTN_Upgrade, &UB2LobbyUI_ItemDetail::OnUpgradeButtonClicked)
	GET_SLOT(UTextBlock, TB_UpgradeBtn)

	GET_SLOT(UOverlay, OV_LevelupOrCompositionOrSurPass)
	GET_SLOT(UButton, BTN_LevelupOrCompositionOrSurPass)
	BIND_CLICK_FUNC_TO_BTN(BTN_LevelupOrCompositionOrSurPass, &UB2LobbyUI_ItemDetail::OnLevelupOrCompositionOrSurPassButtonClicked)
	GET_SLOT(UTextBlock, TB_LevelupOrCompositionOrSurPassBtn)

	GET_SLOT(USizeBox, SB_CompositionForSurpass)
	GET_SLOT(UButton, BTN_CompositionForSurpass)
	BIND_CLICK_FUNC_TO_BTN(BTN_CompositionForSurpass, &UB2LobbyUI_ItemDetail::OnCompositionForSurpassButtonClicked)
	GET_SLOT(UTextBlock, TB_CompositionForSurpass)

	GET_SLOT(USizeBox, SB_Enhance)
	GET_SLOT(UButton, BTN_Enhance)
	BIND_CLICK_FUNC_TO_BTN(BTN_Enhance, &UB2LobbyUI_ItemDetail::OnEnhanceButtonClicked)
	GET_SLOT(UTextBlock, TB_EnhanceBtn)

	GET_SLOT(USizeBox, SB_EnhanceChange)
	GET_SLOT(UButton, BTN_EnhanceChange)
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceChange, &UB2LobbyUI_ItemDetail::OnEnhanceChangeButtonClicked)
	GET_SLOT(UTextBlock, TB_EnhanceChange)


	GET_SLOT(USizeBox,SB_Seal)
	GET_SLOT(UButton,BTN_Seal)
	BIND_CLICK_FUNC_TO_BTN(BTN_Seal, &UB2LobbyUI_ItemDetail::OnSealButtonClicked)
	GET_SLOT(UTextBlock,TB_Seal)

		//옵션변경 삭제
	//GET_SLOT(USizeBox, SB_ItemOptionChange)
	//GET_SLOT(UButton, BTN_ItemOptionChange)
	//BIND_CLICK_FUNC_TO_BTN(BTN_ItemOptionChange, &UB2LobbyUI_ItemDetail::OnItemOptionChangeButtonClicked)
	//GET_SLOT(UTextBlock, TB_ItemOptionChange)

	GET_SLOT(UVerticalBox, VB_SpecialInfoDisplayPanel)

	GET_SLOT(UHorizontalBox, HB_ClassCycle)
	GET_SLOT(UPanelWidget, X_P_LeftButtonSet)
	GET_SLOT(UPanelWidget, X_P_BottomButtonSet)
	GET_SLOT(UPanelWidget, X_P_ItemLockButtonSet)
	GET_SLOT(UPanelWidget, X_P_ItemInfoDisplayPanel)
	GET_SLOT(UPanelWidget, X_P_ItemDescriptionPanel)
	GET_SLOT(UPanelWidget, X_P_ItemSealBoxPanel)

	GET_SLOT(UTextBlock, TB_SealBox_Class)

	GET_SLOT(UB2UIItemDetailOption, UIP_ItemDetailOption)

	GET_SLOT(UButton, BTN_ItemLock)
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemLock, &UB2LobbyUI_ItemDetail::OnItemLockButtonClicked)
	GET_SLOT(UButton, BTN_Close)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemDetail::OnCloseButtonClicked)
}

void UB2LobbyUI_ItemDetail::DestroySelf()
{
	Super::DestroySelf();

	UnsubscribeDecomposition();

	DestroyItemIcon();
	DestroySpecialInfoDisplay();

	if (UIP_ItemDetailOption.IsValid())
	{
		UIP_ItemDetailOption->DestroySelf();
	}
}

void UB2LobbyUI_ItemDetail::UpdateItemData(const FB2Item& InItem)
{
	NativeItemData = InItem;

	if (CachedDataStore)
	{
		FPCItemContainer &ItemContainer = CachedDataStore->GetLocalCharacterData().GetUserAllItems();
		EPCClass CastedPCClass = IntToPCClass(NativeItemData.AllowedPCClass);
		bIsContainsPresetItem = ItemContainer.IsInItemPresets(CastedPCClass, NativeItemData.InstanceUID);
		bIsContainsOtherPreset = ItemContainer.IsInOtherItemPresets(CastedPCClass, NativeItemData.InstanceUID); // 다른 preset 에 포함되어 있는지..
	}
	UE_LOG(LogBladeII, Log, TEXT("Item detail window of [UID] %I64d and [RefID] %d has been invoked"), NativeItemData.InstanceUID, NativeItemData.ItemRefID);

	UpdateItemIcon();

	UpdateSubWidgets(); // 본격 배치한 widget 들 업데이트

	UpdateItemOptions(); // 많은 부분을 차지하는 옵션 리스트.

	UpdateSpecialInfoDisplay();

	//TUTORIAL Hard Cording
	if (TutorialManager::GetInstance().HasAnyTutorial())
	{
		TutorialManager::GetInstance().OnOpenItemDetailPopup();
	}
}

void UB2LobbyUI_ItemDetail::UpdateItemIcon()
{
	DestroyItemIcon();

	if (ItemIconWidgetClass == NULL || !X_P_ItemIconPanel.IsValid())
	{
		return;
	}
		
	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(X_P_ItemIconPanel->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	// ItemIconPanelNRef 위에 최종적인 아이템 아이콘 widget 생성
	CreatedItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, X_P_ItemIconPanel.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
		));

	if (CreatedItemIcon)
	{
		CreatedItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow); // 보관함에서 보여지던 몇가지 정보는 보여주지 않도록 추가 설정.
		CreatedItemIcon->UpdateItemData(NativeItemData);
	}
}

void UB2LobbyUI_ItemDetail::DestroyItemIcon()
{
	if (CreatedItemIcon)
	{
		CreatedItemIcon->DestroySelf();
		CreatedItemIcon = NULL;
	}
}

void UB2LobbyUI_ItemDetail::UpdateSubWidgets()
{
	// Native ref 들을 되는대로 업데이트

	// 동일 위치에 장착한 아이템 정보가 필요할 수 있으므로 빼온다.
	FB2Item EquippedItemAtSamePlace;
	bool bHasEquippedAtSamePlace = CachedInventory ? CachedInventory->GetEquippedItemAtPlace(EquippedItemAtSamePlace, NativeItemData.EquipPlace) : false;
	// bHasEquippedAtSamePlace 가 true 라면 EquippedItemAtSamePlace 이 valid 한 것.

	UpdatePrimaryPointTextSet(bHasEquippedAtSamePlace ? &EquippedItemAtSamePlace : NULL);

	if (TB_ItemLevel.IsValid())
	{
		TB_ItemLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ProgressInfo")),
			FText::AsNumber(NativeItemData.Level),
			FText::AsNumber(NativeItemData.MaxLevel)));
	}
		
	if (TB_EnhanceLevel.IsValid())
	{
		TB_EnhanceLevel->SetText(FText::FromString(FString::Printf(TEXT("+%d"), 
			NativeItemData.EnhanceLevel)));
	}
		
	if (TB_ItemDesc.IsValid())
		TB_ItemDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemDesc")));

	if (PB_ItemExpProgress.IsValid())
	{
		// EnhanceExp 는 0 ~ 100 사이 퍼센트로 가정. 근데 ProgressBar 의 SetPercent 는 실제 그 퍼센트가 아닌 0.0 ~ 1.0 사이 값이라는 게 함정 ㅋ
		PB_ItemExpProgress->SetPercent( // 최대 레벨에서는 걍 만땅 채워준다.
			(NativeItemData.Level == NativeItemData.MaxLevel) ? 1.0f : NativeItemData.Exp * 0.01f
			);
	}
	if (TB_ItemExpPercent.IsValid())
	{
		// 요게 printf 했을 시 반올림 되는 듯 함. 그게 하필 99.99 가 100.0 으로 표시되는 게 문제가 되어서.. 일부러 자름. 아마 딴데도 좀 필요할 듯 한데.. ㄷㄷ
		float TruncatedExp = FMath::FloorToFloat(NativeItemData.Exp * 10.0f) * 0.1f;

		TB_ItemExpPercent->SetText(
			(NativeItemData.Level == NativeItemData.MaxLevel) ?
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Maximum_ValueMark")) : // 최대레벨에서는 숫자 대신 max 로 표시.
			FText::FromString(FString::Printf(TEXT("(%.1f %%)"), TruncatedExp)));
	}

	if (TB_SealBoxDesc.IsValid())
		TB_SealBoxDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemDesc")));
	if (TB_SealBox_Contain.IsValid())
		TB_SealBox_Contain->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealBoxText_RandomListText")));
	if (TB_SealBox_ButtonSet.IsValid())
		TB_SealBox_ButtonSet->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealBoxText_ListSetClass")));

	// Bottom Button Set
	if (TB_Unseal.IsValid())
		TB_Unseal->SetText(GetUnsealButtonText());
	if (BTN_Unseal.IsValid())
		BTN_Unseal->SetVisibility(GetUnsealButtonVisibility());

// 	if (BTN_Convert.IsValid())
// 		BTN_Convert->SetVisibility(GetConvertButtonVisibility());

	if (BTN_Decomposition.IsValid())
		BTN_Decomposition->SetVisibility(GetDecompositionButtonVisibility());

	if (TB_Decomposition.IsValid())
		TB_Decomposition->SetText(GetDecompositionButtonText());
	
	if (BTN_Use.IsValid())
		BTN_Use->SetVisibility(GetUseButtonVisibility());

	if (TB_UseBtn.IsValid())
		TB_UseBtn->SetText(GetUseButtonText());

	if (BTN_Sell.IsValid())
		BTN_Sell->SetVisibility(GetSellButtonVisibility());

	if (TB_SellBtn.IsValid())
		TB_SellBtn->SetText(GetSellButtonText());

	if (BTN_EquipOrRelease.IsValid())
	{ // 장착 버튼을 장착 아이템 외에 사용할 이유가 없지.. 걍 숨기고 싶지만 일단은 정렬이 안되서..
		BTN_EquipOrRelease->SetVisibility(IsEquipmentItem(NativeItemData) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (TB_EquipOrReleaseBtn.IsValid())
		TB_EquipOrReleaseBtn->SetText(GetEquipOrReleaseButtonText());

	// Left Button Set
	if (SB_Upgrade.IsValid())
		SB_Upgrade->SetVisibility(GetUpgradeButtonVisibility());

	if (TB_UpgradeBtn.IsValid())
		TB_UpgradeBtn->SetText(GetUpgradeButtonText());

	if (OV_LevelupOrCompositionOrSurPass.IsValid())
		OV_LevelupOrCompositionOrSurPass->SetVisibility(GetLevelupOrCompositionOrSurPassButtonVisibility());

	if (TB_LevelupOrCompositionOrSurPassBtn.IsValid())
		TB_LevelupOrCompositionOrSurPassBtn->SetText(GetLevelupOrCompositionOrSurPassButtonText());

	if (SB_CompositionForSurpass.IsValid())
		SB_CompositionForSurpass->SetVisibility(GetCompositionForSurpassVisibility());

	if (TB_CompositionForSurpass.IsValid())
		TB_CompositionForSurpass->SetText(GetCompositionForSurpassButtonText());

	if (SB_Enhance.IsValid())
		SB_Enhance->SetVisibility(GetEnhanceButtonVisibility());

	if (TB_EnhanceBtn.IsValid())
		TB_EnhanceBtn->SetText(GetEnhanceButtonText());

	if (SB_EnhanceChange.IsValid())
		SB_EnhanceChange->SetVisibility(GetEnhanceChangeButtonVisibility());

	if (RTB_ItemDescription.IsValid())
		RTB_ItemDescription->SetText(GetItemDescriptionText());

	if (TB_EnhanceChange.IsValid())
		TB_EnhanceChange->SetText(GetEnhanceChangeButtonText());

	if (SB_Seal.IsValid())
		SB_Seal->SetVisibility(GetSealButtonVisibility());

	if (TB_Seal.IsValid())
		TB_Seal->SetText(GetSealButtonText());

	//옵션변경 삭제
	/*if (SB_ItemOptionChange.IsValid())
		SB_ItemOptionChange->SetVisibility(GetItemOptionChangeButtonVisibility());*/

	/*if (TB_ItemOptionChange.IsValid())
		TB_ItemOptionChange->SetText(GetItemOptionChangeText());*/

	if (X_P_ItemLockButtonSet.IsValid())
	{ // 장착 아이템 외엔 잠금 버튼을 보일 필요가 없다.
		X_P_ItemLockButtonSet->SetVisibility(IsEquipmentItem(NativeItemData) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);		
	}

	bool IsConsumableItem = GetConsumableItem();
	IsConsumableItem |= GetEssenceItem();

	if (X_P_ItemInfoDisplayPanel.IsValid())
		X_P_ItemInfoDisplayPanel->SetVisibility(IsConsumableItem ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

	if (X_P_ItemDescriptionPanel.IsValid())
		X_P_ItemDescriptionPanel->SetVisibility(IsConsumableItem ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	bool IsUnsealable = GetUnsealableItem();
	if (GetUnsealableItem())
		UpdateWidgetForSealBox();
}

void UB2LobbyUI_ItemDetail::UpdateWidgetForSealBox()
{
	if (X_P_ItemInfoDisplayPanel.IsValid())
		X_P_ItemInfoDisplayPanel->SetVisibility(ESlateVisibility::Hidden);
	if (X_P_ItemDescriptionPanel.IsValid())
		X_P_ItemDescriptionPanel->SetVisibility(ESlateVisibility::Hidden);
	if (X_P_LeftButtonSet.IsValid())
		X_P_LeftButtonSet->SetVisibility(ESlateVisibility::Hidden);
	if (X_P_ItemSealBoxPanel.IsValid())
		X_P_ItemSealBoxPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	/*
	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		SealBoxClass = IntToPCClass(DocBattle->GetCurPCClass());
	}
	*/

	SealBoxClass = UB2LobbyInventory::GetInventorySelectedPCClass();
	if (TB_SealBox_Class.IsValid())
	{
		switch (SealBoxClass)
		{
		case EPCClass::EPC_Gladiator: TB_SealBox_Class->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Gladiator"))); break;
		case EPCClass::EPC_Assassin: TB_SealBox_Class->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Assassin"))); break;
		case EPCClass::EPC_Wizard: TB_SealBox_Class->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Wizard"))); break;
		case EPCClass::EPC_Fighter: TB_SealBox_Class->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Fighter"))); break;
		}
		
	}

	originalSealBox = CachedDataStore->GetSealBoxInfo(NativeItemData.ItemRefID);
	HB_ClassCycle->SetVisibility(GetSealboxClassCycleable() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (RTB_SealBoxDescription.IsValid())
	{
		RTB_SealBoxDescription->SetText(GetItemDescriptionText());
	}

	AddSealBoxContents();
}

void UB2LobbyUI_ItemDetail::AddSealBoxContents()
{
	//봉인함에 들어갈 아이템에 대해 일부는 직접 주는 RefID도 있으나 장비처럼 유추하는 데이터만 받을 수도 있다.
	//그 유추된 데이터와 현재 클래스(SealBoxClass)에 따라서 RefID 가 달라지므로 그 값에 맞게 추가할 수 있도록 한다.

	if (originalSealBox.sealboxRefID < 0)
		return;

	TArray<FSealBoxRewardRateInfo> slot1, slot2, slot3, slot4, slot5;
	CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot1, slot1);
	CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot2, slot2);
	CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot3, slot3);
	CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot4, slot4);
	CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot5, slot5);

	TArray<FSealBoxItem> savedItemData;
	TArray<FSealBoxEquipment> savedEquipmentData;

	for (int32 i = 0; i < slot1.Num(); i++)
	{
		if (slot1[i].itemType == b2network::B2RewardType::ITEM)
		{
			if (slot1[i].starGrade > 0)
			{
				if (savedEquipmentData.Num() == 0)
				{
					FSealBoxEquipment newData;
					newData.starGrade = slot1[i].starGrade;
					newData.groupID = slot1[i].groupID;
					newData.equipCategory = slot1[i].equipCategory;
					newData.equipPosition = slot1[i].equipPlace;
					newData.quality = slot1[i].quality;
					newData.enhancedLevel = slot1[i].enhanceLevel;
					newData.surpassCount = slot1[i].surpassCount;

					savedEquipmentData.Add(newData);
				}
				else
				{
					bool sameItem = false;
					for (int32 j = 0; j < savedEquipmentData.Num(); j++)
					{
						if (savedEquipmentData[j].starGrade == slot1[i].starGrade)
							if (savedEquipmentData[j].groupID == slot1[i].groupID)
								if (savedEquipmentData[j].equipCategory == slot1[i].equipCategory)
									if (savedEquipmentData[j].equipPosition == slot1[i].equipPlace)
										if (savedEquipmentData[j].quality == slot1[i].quality)
											if (savedEquipmentData[j].enhancedLevel == slot1[i].enhanceLevel)
												if (savedEquipmentData[j].surpassCount == slot1[i].surpassCount)
													sameItem = true;
					}

					if (sameItem == false)
					{
						FSealBoxEquipment newData;
						newData.starGrade = slot1[i].starGrade;
						newData.groupID = slot1[i].groupID;
						newData.equipCategory = slot1[i].equipCategory;
						newData.equipPosition = slot1[i].equipPlace;
						newData.quality = slot1[i].quality;
						newData.enhancedLevel = slot1[i].enhanceLevel;
						newData.surpassCount = slot1[i].surpassCount;

						savedEquipmentData.Add(newData);
					}
				}
			}
		}
		else
		{
			if (savedItemData.Num() == 0)
			{
				if (slot1[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot1[i].itemType) > 0)
					{
						FSealBoxItem newData;
						newData.itemRefID = CachedDataStore->GetRewardItemId(slot1[i].itemType);
						newData.minQuantity = slot1[i].minQuantity;
						newData.maxQuantity = slot1[i].maxQuantity;
						savedItemData.Add(newData);
					}
					else
					{
						continue;
					}
				}
				else
				{
					FSealBoxItem newData;
					newData.itemRefID = slot1[i].itemRefID;
					newData.minQuantity = slot1[i].minQuantity;
					newData.maxQuantity = slot1[i].maxQuantity;
					savedItemData.Add(newData);
				}
			}
			else
			{
				bool alreadyHave = false;
				if (slot1[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot1[i].itemType) > 0)
					{
						for (int32 j = 0; j < savedItemData.Num(); j++)
						{
							if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot1[i].itemType))
							{
								alreadyHave = true;
								if (savedItemData[j].minQuantity > slot1[i].minQuantity)
									savedItemData[j].minQuantity = slot1[i].minQuantity;
								if (savedItemData[j].maxQuantity < slot1[i].maxQuantity)
									savedItemData[j].maxQuantity = slot1[i].maxQuantity;
							}
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					for (int32 j = 0; j < savedItemData.Num(); j++)
					{
						if (savedItemData[j].itemRefID == slot1[i].itemRefID)
						{
							alreadyHave = true;
							if (savedItemData[j].minQuantity > slot1[i].minQuantity)
								savedItemData[j].minQuantity = slot1[i].minQuantity;
							if (savedItemData[j].maxQuantity < slot1[i].maxQuantity)
								savedItemData[j].maxQuantity = slot1[i].maxQuantity;
						}
					}
				}

				if (alreadyHave == false)
				{
					if (slot1[i].itemRefID == 0)
					{
						if (CachedDataStore->GetRewardItemId(slot1[i].itemType) > 0)
						{
							FSealBoxItem newData;
							newData.itemRefID = CachedDataStore->GetRewardItemId(slot1[i].itemType);
							newData.minQuantity = slot1[i].minQuantity;
							newData.maxQuantity = slot1[i].maxQuantity;
							savedItemData.Add(newData);
						}
						else
						{
							continue;
						}
					}
					else
					{
						FSealBoxItem newData;
						newData.itemRefID = slot1[i].itemRefID;
						newData.minQuantity = slot1[i].minQuantity;
						newData.maxQuantity = slot1[i].maxQuantity;
						savedItemData.Add(newData);
					}
				}
			}
		}
	}
	for (int32 i = 0; i < slot2.Num(); i++)
	{
		if (slot2[i].itemType == b2network::B2RewardType::ITEM)
		{
			if (slot2[i].starGrade > 0)
			{
				if (savedEquipmentData.Num() == 0)
				{
					FSealBoxEquipment newData;
					newData.starGrade = slot2[i].starGrade;
					newData.groupID = slot2[i].groupID;
					newData.equipCategory = slot2[i].equipCategory;
					newData.equipPosition = slot2[i].equipPlace;
					newData.quality = slot2[i].quality;
					newData.enhancedLevel = slot2[i].enhanceLevel;
					newData.surpassCount = slot2[i].surpassCount;

					savedEquipmentData.Add(newData);
				}
				else
				{
					bool sameItem = false;
					for (int32 j = 0; j < savedEquipmentData.Num(); j++)
					{
						if (savedEquipmentData[j].starGrade == slot2[i].starGrade)
							if (savedEquipmentData[j].groupID == slot2[i].groupID)
								if (savedEquipmentData[j].equipCategory == slot2[i].equipCategory)
									if (savedEquipmentData[j].equipPosition == slot2[i].equipPlace)
										if (savedEquipmentData[j].quality == slot2[i].quality)
											if (savedEquipmentData[j].enhancedLevel == slot2[i].enhanceLevel)
												if (savedEquipmentData[j].surpassCount == slot2[i].surpassCount)
													sameItem = true;
					}

					if (sameItem == false)
					{
						FSealBoxEquipment newData;
						newData.starGrade = slot2[i].starGrade;
						newData.groupID = slot2[i].groupID;
						newData.equipCategory = slot2[i].equipCategory;
						newData.equipPosition = slot2[i].equipPlace;
						newData.quality = slot2[i].quality;
						newData.enhancedLevel = slot2[i].enhanceLevel;
						newData.surpassCount = slot2[i].surpassCount;

						savedEquipmentData.Add(newData);
					}
				}
			}
		}
		else
		{
			if (savedItemData.Num() == 0)
			{
				if (slot2[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot2[i].itemType) > 0)
					{
						FSealBoxItem newData;
						newData.itemRefID = CachedDataStore->GetRewardItemId(slot2[i].itemType);
						newData.minQuantity = slot2[i].minQuantity;
						newData.maxQuantity = slot2[i].maxQuantity;
						savedItemData.Add(newData);
					}
					else
					{
						continue;
					}
				}
				else
				{
					FSealBoxItem newData;
					newData.itemRefID = slot2[i].itemRefID;
					newData.minQuantity = slot2[i].minQuantity;
					newData.maxQuantity = slot2[i].maxQuantity;
					savedItemData.Add(newData);
				}
			}
			else
			{
				bool alreadyHave = false;
				if (slot2[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot2[i].itemType) > 0)
					{
						for (int32 j = 0; j < savedItemData.Num(); j++)
						{
							if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot2[i].itemType))
							{
								alreadyHave = true;
								if (savedItemData[j].minQuantity > slot2[i].minQuantity)
									savedItemData[j].minQuantity = slot2[i].minQuantity;
								if (savedItemData[j].maxQuantity < slot2[i].maxQuantity)
									savedItemData[j].maxQuantity = slot2[i].maxQuantity;
							}
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					for (int32 j = 0; j < savedItemData.Num(); j++)
					{
						if (savedItemData[j].itemRefID == slot2[i].itemRefID)
						{
							alreadyHave = true;
							if (savedItemData[j].minQuantity > slot2[i].minQuantity)
								savedItemData[j].minQuantity = slot2[i].minQuantity;
							if (savedItemData[j].maxQuantity < slot2[i].maxQuantity)
								savedItemData[j].maxQuantity = slot2[i].maxQuantity;
						}
					}
				}

				if (alreadyHave == false)
				{
					if (slot2[i].itemRefID == 0)
					{
						if (CachedDataStore->GetRewardItemId(slot2[i].itemType) > 0)
						{
							FSealBoxItem newData;
							newData.itemRefID = CachedDataStore->GetRewardItemId(slot2[i].itemType);
							newData.minQuantity = slot2[i].minQuantity;
							newData.maxQuantity = slot2[i].maxQuantity;
							savedItemData.Add(newData);
						}
						else
						{
							continue;
						}
					}
					else
					{
						FSealBoxItem newData;
						newData.itemRefID = slot2[i].itemRefID;
						newData.minQuantity = slot2[i].minQuantity;
						newData.maxQuantity = slot2[i].maxQuantity;
						savedItemData.Add(newData);
					}
				}
			}
		}
	}
	for (int32 i = 0; i < slot3.Num(); i++)
	{
		if (slot3[i].itemType == b2network::B2RewardType::ITEM)
		{
			if (slot3[i].starGrade > 0)
			{
				if (savedEquipmentData.Num() == 0)
				{
					FSealBoxEquipment newData;
					newData.starGrade = slot3[i].starGrade;
					newData.groupID = slot3[i].groupID;
					newData.equipCategory = slot3[i].equipCategory;
					newData.equipPosition = slot3[i].equipPlace;
					newData.quality = slot3[i].quality;
					newData.enhancedLevel = slot3[i].enhanceLevel;
					newData.surpassCount = slot3[i].surpassCount;

					savedEquipmentData.Add(newData);
				}
				else
				{
					bool sameItem = false;
					for (int32 j = 0; j < savedEquipmentData.Num(); j++)
					{
						if (savedEquipmentData[j].starGrade == slot3[i].starGrade)
							if (savedEquipmentData[j].groupID == slot3[i].groupID)
								if (savedEquipmentData[j].equipCategory == slot3[i].equipCategory)
									if (savedEquipmentData[j].equipPosition == slot3[i].equipPlace)
										if (savedEquipmentData[j].quality == slot3[i].quality)
											if (savedEquipmentData[j].enhancedLevel == slot3[i].enhanceLevel)
												if (savedEquipmentData[j].surpassCount == slot3[i].surpassCount)
													sameItem = true;
					}

					if (sameItem == false)
					{
						FSealBoxEquipment newData;
						newData.starGrade = slot3[i].starGrade;
						newData.groupID = slot3[i].groupID;
						newData.equipCategory = slot3[i].equipCategory;
						newData.equipPosition = slot3[i].equipPlace;
						newData.quality = slot3[i].quality;
						newData.enhancedLevel = slot3[i].enhanceLevel;
						newData.surpassCount = slot3[i].surpassCount;

						savedEquipmentData.Add(newData);
					}
				}
			}
		}
		else
		{
			if (savedItemData.Num() == 0)
			{
				if (slot3[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot3[i].itemType) > 0)
					{
						FSealBoxItem newData;
						newData.itemRefID = CachedDataStore->GetRewardItemId(slot3[i].itemType);
						newData.minQuantity = slot3[i].minQuantity;
						newData.maxQuantity = slot3[i].maxQuantity;
						savedItemData.Add(newData);
					}
					else
					{
						continue;
					}
				}
				else
				{
					FSealBoxItem newData;
					newData.itemRefID = slot3[i].itemRefID;
					newData.minQuantity = slot3[i].minQuantity;
					newData.maxQuantity = slot3[i].maxQuantity;
					savedItemData.Add(newData);
				}
			}
			else
			{
				bool alreadyHave = false;
				if (slot3[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot3[i].itemType) > 0)
					{
						for (int32 j = 0; j < savedItemData.Num(); j++)
						{
							if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot3[i].itemType))
							{
								alreadyHave = true;
								if (savedItemData[j].minQuantity > slot3[i].minQuantity)
									savedItemData[j].minQuantity = slot3[i].minQuantity;
								if (savedItemData[j].maxQuantity < slot3[i].maxQuantity)
									savedItemData[j].maxQuantity = slot3[i].maxQuantity;
							}
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					for (int32 j = 0; j < savedItemData.Num(); j++)
					{
						if (savedItemData[j].itemRefID == slot3[i].itemRefID)
						{
							alreadyHave = true;
							if (savedItemData[j].minQuantity > slot3[i].minQuantity)
								savedItemData[j].minQuantity = slot3[i].minQuantity;
							if (savedItemData[j].maxQuantity < slot3[i].maxQuantity)
								savedItemData[j].maxQuantity = slot3[i].maxQuantity;
						}
					}
				}

				if (alreadyHave == false)
				{
					if (slot3[i].itemRefID == 0)
					{
						if (CachedDataStore->GetRewardItemId(slot3[i].itemType) > 0)
						{
							FSealBoxItem newData;
							newData.itemRefID = CachedDataStore->GetRewardItemId(slot3[i].itemType);
							newData.minQuantity = slot3[i].minQuantity;
							newData.maxQuantity = slot3[i].maxQuantity;
							savedItemData.Add(newData);
						}
						else
						{
							continue;
						}
					}
					else
					{
						FSealBoxItem newData;
						newData.itemRefID = slot3[i].itemRefID;
						newData.minQuantity = slot3[i].minQuantity;
						newData.maxQuantity = slot3[i].maxQuantity;
						savedItemData.Add(newData);
					}
				}
			}
		}
	}
	for (int32 i = 0; i < slot4.Num(); i++)
	{
		if (slot4[i].itemType == b2network::B2RewardType::ITEM)
		{
			if (slot4[i].starGrade > 0)
			{
				if (savedEquipmentData.Num() == 0)
				{
					FSealBoxEquipment newData;
					newData.starGrade = slot4[i].starGrade;
					newData.groupID = slot4[i].groupID;
					newData.equipCategory = slot4[i].equipCategory;
					newData.equipPosition = slot4[i].equipPlace;
					newData.quality = slot4[i].quality;
					newData.enhancedLevel = slot4[i].enhanceLevel;
					newData.surpassCount = slot4[i].surpassCount;

					savedEquipmentData.Add(newData);
				}
				else
				{
					bool sameItem = false;
					for (int32 j = 0; j < savedEquipmentData.Num(); j++)
					{
						if (savedEquipmentData[j].starGrade == slot4[i].starGrade)
							if (savedEquipmentData[j].groupID == slot4[i].groupID)
								if (savedEquipmentData[j].equipCategory == slot4[i].equipCategory)
									if (savedEquipmentData[j].equipPosition == slot4[i].equipPlace)
										if (savedEquipmentData[j].quality == slot4[i].quality)
											if (savedEquipmentData[j].enhancedLevel == slot4[i].enhanceLevel)
												if (savedEquipmentData[j].surpassCount == slot4[i].surpassCount)
													sameItem = true;
					}

					if (sameItem == false)
					{
						FSealBoxEquipment newData;
						newData.starGrade = slot4[i].starGrade;
						newData.groupID = slot4[i].groupID;
						newData.equipCategory = slot4[i].equipCategory;
						newData.equipPosition = slot4[i].equipPlace;
						newData.quality = slot4[i].quality;
						newData.enhancedLevel = slot4[i].enhanceLevel;
						newData.surpassCount = slot4[i].surpassCount;

						savedEquipmentData.Add(newData);
					}
				}
			}
		}
		else
		{
			if (savedItemData.Num() == 0)
			{
				if (slot4[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot4[i].itemType) > 0)
					{
						FSealBoxItem newData;
						newData.itemRefID = CachedDataStore->GetRewardItemId(slot4[i].itemType);
						newData.minQuantity = slot4[i].minQuantity;
						newData.maxQuantity = slot4[i].maxQuantity;
						savedItemData.Add(newData);
					}
					else
					{
						continue;
					}
				}
				else
				{
					FSealBoxItem newData;
					newData.itemRefID = slot4[i].itemRefID;
					newData.minQuantity = slot4[i].minQuantity;
					newData.maxQuantity = slot4[i].maxQuantity;
					savedItemData.Add(newData);
				}
			}
			else
			{
				bool alreadyHave = false;
				if (slot4[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot4[i].itemType) > 0)
					{
						for (int32 j = 0; j < savedItemData.Num(); j++)
						{
							if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot4[i].itemType))
							{
								alreadyHave = true;
								if (savedItemData[j].minQuantity > slot4[i].minQuantity)
									savedItemData[j].minQuantity = slot4[i].minQuantity;
								if (savedItemData[j].maxQuantity < slot4[i].maxQuantity)
									savedItemData[j].maxQuantity = slot4[i].maxQuantity;
							}
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					for (int32 j = 0; j < savedItemData.Num(); j++)
					{
						if (savedItemData[j].itemRefID == slot4[i].itemRefID)
						{
							alreadyHave = true;
							if (savedItemData[j].minQuantity > slot4[i].minQuantity)
								savedItemData[j].minQuantity = slot4[i].minQuantity;
							if (savedItemData[j].maxQuantity < slot4[i].maxQuantity)
								savedItemData[j].maxQuantity = slot4[i].maxQuantity;
						}
					}
				}

				if (alreadyHave == false)
				{
					if (slot4[i].itemRefID == 0)
					{
						if (CachedDataStore->GetRewardItemId(slot4[i].itemType) > 0)
						{
							FSealBoxItem newData;
							newData.itemRefID = CachedDataStore->GetRewardItemId(slot4[i].itemType);
							newData.minQuantity = slot4[i].minQuantity;
							newData.maxQuantity = slot4[i].maxQuantity;
							savedItemData.Add(newData);
						}
						else
						{
							continue;
						}
					}
					else
					{
						FSealBoxItem newData;
						newData.itemRefID = slot4[i].itemRefID;
						newData.minQuantity = slot4[i].minQuantity;
						newData.maxQuantity = slot4[i].maxQuantity;
						savedItemData.Add(newData);
					}
				}
			}
		}
	}
	for (int32 i = 0; i < slot5.Num(); i++)
	{
		if (slot5[i].itemType == b2network::B2RewardType::ITEM)
		{
			if (slot5[i].starGrade > 0)
			{
				if (savedEquipmentData.Num() == 0)
				{
					FSealBoxEquipment newData;
					newData.starGrade = slot5[i].starGrade;
					newData.groupID = slot5[i].groupID;
					newData.equipCategory = slot5[i].equipCategory;
					newData.equipPosition = slot5[i].equipPlace;
					newData.quality = slot5[i].quality;
					newData.enhancedLevel = slot5[i].enhanceLevel;
					newData.surpassCount = slot5[i].surpassCount;

					savedEquipmentData.Add(newData);
				}
				else
				{
					bool sameItem = false;
					for (int32 j = 0; j < savedEquipmentData.Num(); j++)
					{
						if (savedEquipmentData[j].starGrade == slot5[i].starGrade)
							if (savedEquipmentData[j].groupID == slot5[i].groupID)
								if (savedEquipmentData[j].equipCategory == slot5[i].equipCategory)
									if (savedEquipmentData[j].equipPosition == slot5[i].equipPlace)
										if (savedEquipmentData[j].quality == slot5[i].quality)
											if (savedEquipmentData[j].enhancedLevel == slot5[i].enhanceLevel)
												if (savedEquipmentData[j].surpassCount == slot5[i].surpassCount)
													sameItem = true;
					}

					if (sameItem == false)
					{
						FSealBoxEquipment newData;
						newData.starGrade = slot5[i].starGrade;
						newData.groupID = slot5[i].groupID;
						newData.equipCategory = slot5[i].equipCategory;
						newData.equipPosition = slot5[i].equipPlace;
						newData.quality = slot5[i].quality;
						newData.enhancedLevel = slot5[i].enhanceLevel;
						newData.surpassCount = slot5[i].surpassCount;

						savedEquipmentData.Add(newData);
					}
				}
			}
		}
		else
		{
			if (savedItemData.Num() == 0)
			{
				if (slot5[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot5[i].itemType) > 0)
					{
						FSealBoxItem newData;
						newData.itemRefID = CachedDataStore->GetRewardItemId(slot5[i].itemType);
						newData.minQuantity = slot5[i].minQuantity;
						newData.maxQuantity = slot5[i].maxQuantity;
						savedItemData.Add(newData);
					}
					else
					{
						continue;
					}
				}
				else
				{
					FSealBoxItem newData;
					newData.itemRefID = slot5[i].itemRefID;
					newData.minQuantity = slot5[i].minQuantity;
					newData.maxQuantity = slot5[i].maxQuantity;
					savedItemData.Add(newData);
				}
			}
			else
			{
				bool alreadyHave = false;
				if (slot5[i].itemRefID == 0)
				{
					if (CachedDataStore->GetRewardItemId(slot5[i].itemType) > 0)
					{
						for (int32 j = 0; j < savedItemData.Num(); j++)
						{
							if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot5[i].itemType))
							{
								alreadyHave = true;
								if (savedItemData[j].minQuantity > slot5[i].minQuantity)
									savedItemData[j].minQuantity = slot5[i].minQuantity;
								if (savedItemData[j].maxQuantity < slot5[i].maxQuantity)
									savedItemData[j].maxQuantity = slot5[i].maxQuantity;
							}
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					for (int32 j = 0; j < savedItemData.Num(); j++)
					{
						if (savedItemData[j].itemRefID == slot5[i].itemRefID)
						{
							alreadyHave = true;
							if (savedItemData[j].minQuantity > slot5[i].minQuantity)
								savedItemData[j].minQuantity = slot5[i].minQuantity;
							if (savedItemData[j].maxQuantity < slot5[i].maxQuantity)
								savedItemData[j].maxQuantity = slot5[i].maxQuantity;
						}
					}
				}

				if (alreadyHave == false)
				{
					if (slot5[i].itemRefID == 0)
					{
						if (CachedDataStore->GetRewardItemId(slot5[i].itemType) > 0)
						{
							FSealBoxItem newData;
							newData.itemRefID = CachedDataStore->GetRewardItemId(slot5[i].itemType);
							newData.minQuantity = slot5[i].minQuantity;
							newData.maxQuantity = slot5[i].maxQuantity;
							savedItemData.Add(newData);
						}
						else
						{
							continue;
						}
					}
					else
					{
						FSealBoxItem newData;
						newData.itemRefID = slot5[i].itemRefID;
						newData.minQuantity = slot5[i].minQuantity;
						newData.maxQuantity = slot5[i].maxQuantity;
						savedItemData.Add(newData);
					}
				}
			}
		}
	}
	
	if (SB_SealBox_ContainList.IsValid())
	{
		SB_SealBox_ContainList->ClearChildren();

		for (int32 i = 0; i < savedItemData.Num(); i++)
		{
			UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), SealBoxContainListBP);
			SealBoxIconWidget->CacheAssets();
			SealBoxIconWidget->SetItem(savedItemData[i].itemRefID, savedItemData[i].minQuantity, savedItemData[i].maxQuantity);
			SB_SealBox_ContainList->AddChild(SealBoxIconWidget);
		}

		for (int32 i = 0; i < savedEquipmentData.Num(); i++)
		{
			UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), SealBoxContainListBP);
			SealBoxIconWidget->CacheAssets();
			SealBoxIconWidget->SetItem(savedEquipmentData[i].groupID, SealBoxClass, savedEquipmentData[i].equipCategory, savedEquipmentData[i].equipPosition, savedEquipmentData[i].starGrade, savedEquipmentData[i].quality, savedEquipmentData[i].enhancedLevel, savedEquipmentData[i].surpassCount);
			SB_SealBox_ContainList->AddChild(SealBoxIconWidget);
		}
	}
}

bool UB2LobbyUI_ItemDetail::GetConvertableItem()	// 하드코딩 //변환 시스템이 사라지고 제작소로 흡수되서 사용하지 않음
{
	//if (NativeItemData.ItemClass == EItemClass::EIC_UpgradeStone)
	
	switch(NativeItemData.ItemRefID)
	{
	case 8000000:
	case 8000001:
	case 8000002:
		return true;
		break;
	}
	return false;

}

bool UB2LobbyUI_ItemDetail::GetUnsealableItem()
{

	FSealBoxInfo tempSealboxInfo = CachedDataStore->GetSealBoxInfo(NativeItemData.ItemRefID);

	if (tempSealboxInfo.sealboxRefID < 0)
		return false;
	else
		return true;
}

bool UB2LobbyUI_ItemDetail::GetSealboxClassCycleable()
{
	if (GetUnsealableItem())
	{
		if (CachedDataStore->DoesSealboxHaveEquipment(originalSealBox.rewardSlot1)) return true;
		if (CachedDataStore->DoesSealboxHaveEquipment(originalSealBox.rewardSlot2)) return true;
		if (CachedDataStore->DoesSealboxHaveEquipment(originalSealBox.rewardSlot3)) return true;
		if (CachedDataStore->DoesSealboxHaveEquipment(originalSealBox.rewardSlot4)) return true;
		if (CachedDataStore->DoesSealboxHaveEquipment(originalSealBox.rewardSlot5)) return true;
	}
	return false;
}

bool UB2LobbyUI_ItemDetail::GetConsumableItem()
{
	return NativeItemData.InventoryType == EItemInvenType::EIIVT_Consumables;
}
bool UB2LobbyUI_ItemDetail::GetEssenceItem()
{
	if (NativeItemData.ItemClass == EItemClass::EIC_WeaponEssence ||
		NativeItemData.ItemClass == EItemClass::EIC_ProtectionEssence ||
		NativeItemData.ItemClass == EItemClass::EIC_AccessoryEssence ||
		NativeItemData.ItemClass == EItemClass::EIC_WeaponSurpassAnvil ||
		NativeItemData.ItemClass == EItemClass::EIC_ArmorSurpassAnvil ||
		NativeItemData.ItemClass == EItemClass::EIC_AccessorySurpassAnvil
		)
	{
		return true;
	}

	return false;
}

const FSingleGoodsTypeInfoData * UB2LobbyUI_ItemDetail::GetItemGoodsTypeInfoData()
{
	if (NativeItemData.ItemRefID <= 0)
		return nullptr;

	FSingleItemInfoData* const singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(NativeItemData.ItemRefID) : nullptr;
	if (singleData == nullptr)
		return nullptr;

	return StaticFindGoodsTypeInfoTable() != NULL ? StaticFindGoodsTypeInfoTable()->GetInfoData(singleData->FGoodsType) : nullptr;
}

void UB2LobbyUI_ItemDetail::UpdatePrimaryPointTextSet(FB2Item* EquippedOneAtSamePlace)
{
	if (TB_PrimaryPointType.IsValid())
		TB_PrimaryPointType->SetText(GetLOCTextOfPrimPointType(NativeItemData.PrimaryPointType));

	if (TB_PrimaryPoint.IsValid())
		TB_PrimaryPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)NativeItemData.GetPrimaryPoint())));

	if (TB_Combat.IsValid())
		TB_Combat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));

	if (TB_Enhance.IsValid())
		TB_Enhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_EnhanceLevelDesc")));

	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(static_cast<int32>(NativeItemData.Power)));

	if (TB_PrimaryPointDelta.IsValid())
	{
		if (!bIsForEquippedItem && EquippedOneAtSamePlace) // 수치 비교 표시는 보관함 용이고 동일 위치에 장착한 아이템이 있을 경우.
		{
			TB_PrimaryPointDelta->SetVisibility(ESlateVisibility::Visible);
			TB_CombatPowerDelta->SetVisibility(ESlateVisibility::Visible);

			int32 PrimaryPointDelta = B2UIUtil::DifferenceItemPrimaryPoint(NativeItemData, *EquippedOneAtSamePlace);
			int32 CombatDelta = NativeItemData.Power - EquippedOneAtSamePlace->Power;

			// - 는 그냥 숫자만 넣어도 되지만 + 는 그냥 안나오므로 걍 수치는 절대값으로..
			FString DeltaString = (PrimaryPointDelta == 0) ? TEXT("( - )") : FString::Printf(TEXT("( %s%d )"), (PrimaryPointDelta > 0) ? TEXT("+") : TEXT("-"), FMath::Abs(PrimaryPointDelta));
			TB_PrimaryPointDelta->SetText(FText::FromString(DeltaString));

			TB_PrimaryPointDelta->SetColorAndOpacity((PrimaryPointDelta > 0) ? ItemStatIncColor : (PrimaryPointDelta == 0 ? ItemStatSameColor : ItemStatDecColor));

			 DeltaString = (CombatDelta == 0) ? TEXT("( - )") : FString::Printf(TEXT("( %s%d )"), (CombatDelta > 0) ? TEXT("+") : TEXT("-"), FMath::Abs(CombatDelta));

			 TB_CombatPowerDelta->SetText(FText::FromString(DeltaString));
			 TB_CombatPowerDelta->SetColorAndOpacity((CombatDelta > 0) ? ItemStatIncColor : (CombatDelta == 0 ? ItemStatSameColor : ItemStatDecColor));

		}
		else
		{
			TB_PrimaryPointDelta->SetVisibility(ESlateVisibility::Hidden);
			TB_CombatPowerDelta->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2LobbyUI_ItemDetail::UpdateItemOptions()
{
	if (UIP_ItemDetailOption.IsValid())
	{
		FB2Item EquippedItemAtSamePlace; // 동일 위치에 장착된 아이템 정보.
		if (CachedInventory)
		{
			CachedInventory->GetEquippedItemAtPlace(EquippedItemAtSamePlace, NativeItemData.EquipPlace);
		}
		UIP_ItemDetailOption->SetItemData(NativeItemData, EquippedItemAtSamePlace, bIsForEquippedItem, ItemStatIncColor, ItemStatSameColor, ItemStatDecColor);
	}
}

void UB2LobbyUI_ItemDetail::UpdateSpecialInfoDisplay()
{
	DestroySpecialInfoDisplay();

	const bool bRequiresSetEffectDisplay = IsItemSetEffectAvailable(NativeItemData);
	
	if (
		!VB_SpecialInfoDisplayPanel.IsValid() ||
		(!bRequiresSetEffectDisplay) || // 둘 중 하나는 대상이 되어야.
		(bRequiresSetEffectDisplay && !SetEffectInfoDisplayClass)		
		)
	{
		return;
	}

	if (bRequiresSetEffectDisplay)
	{
		UB2LobbyUI_ItemDetailSetEffect* SetItemInfo = CreateWidget<UB2LobbyUI_ItemDetailSetEffect>(GetWorld(), SetEffectInfoDisplayClass);
		if(SetItemInfo)
		{
			VB_SpecialInfoDisplayPanel->AddChild(SetItemInfo);
			SetItemInfo->UpdateItemData(NativeItemData);
		}
	}
}

void UB2LobbyUI_ItemDetail::DestroySpecialInfoDisplay()
{
	if (VB_SpecialInfoDisplayPanel.IsValid())
	{
		VB_SpecialInfoDisplayPanel->ClearChildren();
	}
}

void UB2LobbyUI_ItemDetail::AskForUnlockItemPopupCommon(const FText& InAskingItemActText)
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_AskForUnlockForSomeItemAct")), InAskingItemActText),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
			LobbyReqToggleItemLockClass<int64, bool>::GetInstance().Signal(NativeItemData.InstanceUID, false);
		})
	);
}

void UB2LobbyUI_ItemDetail::NoticeErrorPopupCommon(const FText& InAskingText)
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		InAskingText,
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
	);
}

void UB2LobbyUI_ItemDetail::AskForUnEquipItemPopupCommon(const FText& InAskingItemActText)
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_AskUnEquipForItemEnahnce")), InAskingItemActText),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
			TArray<int64> ReqIDs;
			ReqIDs.Add(NativeItemData.InstanceUID);
			LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Signal(-1, ReqIDs);
	})
	);
}

void UB2LobbyUI_ItemDetail::SetIsForEquippedItem(bool bInForEquipped)
{
	bIsForEquippedItem = bInForEquipped;

	// 뷰포트 내 위치 맞춰줌. 장착 아이템 화면은 좌측에, 보관함 아이템 화면은 우측에.

	FVector2D PositionInViewport(0.0f, 0.0f);

	APlayerController* OwnerPC = GetOwningPlayer();
	if (!bIsForEquippedItem && OwnerPC)
	{
		int32 ScreenSizeX = 0;
		int32 ScreenSizeY = 0;
		OwnerPC->GetViewportSize(ScreenSizeX, ScreenSizeY);

		PositionInViewport.X = (float)(ScreenSizeX / 2);
	}

	SetPositionInViewport(PositionInViewport);
}

void UB2LobbyUI_ItemDetail::SetFindUserInfoButtonHidden()
{
	if (X_P_LeftButtonSet.IsValid())
		X_P_LeftButtonSet->SetVisibility(ESlateVisibility::Hidden);

	if (X_P_BottomButtonSet.IsValid())
		X_P_BottomButtonSet->SetVisibility(ESlateVisibility::Hidden);

	if (X_P_ItemLockButtonSet.IsValid())
		X_P_ItemLockButtonSet->SetVisibility(ESlateVisibility::Hidden);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
ESlateVisibility UB2LobbyUI_ItemDetail::GetUnsealButtonVisibility()
{
	if (GetUnsealableItem())
	{
		return ESlateVisibility::Visible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetConvertButtonVisibility()
{
	if (GetConvertableItem())
	{
		return ESlateVisibility::Visible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetDecompositionButtonVisibility() const
{
	if (bIsContainsPresetItem) 
		return ESlateVisibility::Hidden;
	else 
		return IsItemDecompostionAllowed(NativeItemData) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetUseButtonVisibility()
{
	bool VisibleUseButton = false;
	if (GetConsumableItem())
	{
		const FSingleGoodsTypeInfoData * GoodsTypeInfoData = GetItemGoodsTypeInfoData();
		if (GoodsTypeInfoData != nullptr)
		{
			VisibleUseButton = GoodsTypeInfoData->UseShortcutInfo.ShortcutSceneName != EUIScene::None;

			if (GoodsTypeInfoData->FGoodsType.IsEqual(TEXT("NAMECHANGE_TICKET")))
			{
				return ESlateVisibility::Visible;
			}
		}
	}

	return VisibleUseButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetAllReleaseVisibility() const
{
	if (bIsContainsPresetItem && !bIsContainsOtherPreset) 
		return ESlateVisibility::Hidden;

	else 
		return ESlateVisibility::Visible;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetSellButtonVisibility() const
{
	if (bIsContainsPresetItem && !bIsContainsOtherPreset)
	{
		return ESlateVisibility::Hidden;
	}
	else
	{
		const FMD_ItemSingleElem* ItemMasterInfo = BladeIIGameImpl::GetClientDataStore().GetItemMasterDetailInfo(NativeItemData.ItemRefID);
	
		if(ItemMasterInfo && ItemMasterInfo->SellAble)
			return ESlateVisibility::Visible;
		else
			return ESlateVisibility::Collapsed;
	}
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetUpgradeButtonVisibility() const
{
	if (!IsEquipmentItem(NativeItemData) ||
		(NativeItemData.InventoryType != EItemInvenType::EIIVT_Weapon &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Protection &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Accessory)
		)
		return ESlateVisibility::Collapsed;

	return IsItemUpgradeAllowed(NativeItemData) || IsItemAllowedCompositeAndSurpass(NativeItemData) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetLevelupOrCompositionOrSurPassButtonVisibility() const
{
	if (!IsEquipmentItem(NativeItemData) ||
		(NativeItemData.InventoryType != EItemInvenType::EIIVT_Weapon &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Protection &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Accessory)
		)
		return ESlateVisibility::Collapsed;

	return (IsItemLevelupAllowed(NativeItemData) || IsItemCompositionAllowed(NativeItemData) || IsItemSurpassAllowed(NativeItemData))
		? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetCompositionForSurpassVisibility() const
{
	if (!IsEquipmentItem(NativeItemData) ||
		(NativeItemData.InventoryType != EItemInvenType::EIIVT_Weapon &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Protection &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Accessory)
		)
		return ESlateVisibility::Collapsed;

	return (IsItemAllowedCompositeAndSurpass(NativeItemData))
		? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetEnhanceButtonVisibility() const
{
	if (!IsEquipmentItem(NativeItemData) ||
		(NativeItemData.InventoryType != EItemInvenType::EIIVT_Weapon &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Protection &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Accessory)
		)
		return ESlateVisibility::Collapsed;

	return IsItemEnhanceAllowed(NativeItemData) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetEnhanceChangeButtonVisibility() const
{
	if (!IsEquipmentItem(NativeItemData) ||
		(NativeItemData.InventoryType != EItemInvenType::EIIVT_Weapon &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Protection &&
			NativeItemData.InventoryType != EItemInvenType::EIIVT_Accessory)
		)
		return ESlateVisibility::Collapsed;

	return ESlateVisibility::SelfHitTestInvisible;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetItemOptionChangeButtonVisibility() const
{
	return NativeItemData.RandomOptions.Num() > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetSealButtonVisibility() const
{

	if (!IsEquipmentItem(NativeItemData) || NativeItemData.StarGrade < 6 ||
		NativeItemData.InventoryType != EItemInvenType::EIIVT_Weapon &&
		NativeItemData.InventoryType != EItemInvenType::EIIVT_Protection &&
		NativeItemData.InventoryType != EItemInvenType::EIIVT_Accessory )
	{
		return ESlateVisibility::Collapsed;
	}

		

	return ESlateVisibility::SelfHitTestInvisible;
}

FText UB2LobbyUI_ItemDetail::GetUnsealButtonText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealBoxText_Unseal"));
}
FText UB2LobbyUI_ItemDetail::GetConvertButtonText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Conversion"));
}

FText UB2LobbyUI_ItemDetail::GetDecompositionButtonText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Decomposition"));
}

FText UB2LobbyUI_ItemDetail::GetUseButtonText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_UseItem"));
}

FText UB2LobbyUI_ItemDetail::GetSellButtonText() const
{
	if (CachedDataStore)
	{
		if (bIsContainsPresetItem) // 하나만 포함되어도 일단 모두 해제 버튼은 보이도록 하나? 어쨌든 장착을 하긴 한 거니 팔 순 없잖아?
			return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_UnequipAll")));
	}
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Sell")));
}

FText UB2LobbyUI_ItemDetail::GetEquipOrReleaseButtonText() const
{
	return bIsForEquippedItem ?
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_ReleaseEquip"))) :
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Equip")));
}

FText UB2LobbyUI_ItemDetail::GetUpgradeButtonText() const
{ // 합성과 같은 조건에서 동시에 나오기 때문에 버튼을 따로 구성.
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Upgrade")));
}

FText UB2LobbyUI_ItemDetail::GetLevelupOrCompositionOrSurPassButtonText() const
{// 강화, 합성, 초월은 각각 나오는 조건이 다름
	if (IsItemLevelupAllowed(NativeItemData)) { // 내지는 이건 기본.
		return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Levelup")));
	}
	else if (IsItemCompositionAllowed(NativeItemData)) {
		return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Composition")));
	}
	else if (IsItemSurpassAllowed(NativeItemData))
	{
		return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Surpass")));
	}

	return FText::FromString(TEXT(""));
}

FText UB2LobbyUI_ItemDetail::GetCompositionForSurpassButtonText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Composition"));
}

FText UB2LobbyUI_ItemDetail::GetEnhanceButtonText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Enhancement"));
}

FText UB2LobbyUI_ItemDetail::GetEnhanceChangeButtonText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Exchange_Title"));
}

FText UB2LobbyUI_ItemDetail::GetItemOptionChangeText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_ChangeOption"));
}

FText UB2LobbyUI_ItemDetail::GetSealButtonText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Seal"));
}

FText UB2LobbyUI_ItemDetail::GetItemDescriptionText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_ITEMNAMESUB, FString::FromInt(NativeItemData.ItemRefID));
}

FText UB2LobbyUI_ItemDetail::GetSealBoxDescText(int32 sealBoxRefID) const
{
	if (false)
	{

	}
	else
	{
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealBoxText_RandomGuide"));
	}
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetItemLockedImageVisibility() const
{
	return NativeItemData.bIsLocked ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}

ESlateVisibility UB2LobbyUI_ItemDetail::GetItemUnLockedImageVisibility() const
{
	return NativeItemData.bIsLocked ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible;
}

bool UB2LobbyUI_ItemDetail::GetItemIsLocked() const
{
	return NativeItemData.bIsLocked;
}

void UB2LobbyUI_ItemDetail::OnCloseButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);

	const ELobbySubPopups LobbyPopupEnum = bIsForEquippedItem ? ELobbySubPopups::ELSPU_EquippedItemDetail : ELobbySubPopups::ELSPU_StoredItemDetail;
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(LobbyPopupEnum);

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	OnCloseButtonClickedDelegate.Broadcast();
}

void UB2LobbyUI_ItemDetail::OnUnsealButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayUnsealCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnConvertButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayConvertCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnDecompositionButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayDecompositionCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnUseButtonClicked()
{
	const FSingleGoodsTypeInfoData * GoodsTypeInfoData = GetItemGoodsTypeInfoData();
	if (GoodsTypeInfoData != nullptr)
	{
		if (GoodsTypeInfoData->LockTutorialID > 0 && TutorialManager::GetInstance().IsLockTutorial(GoodsTypeInfoData->LockTutorialID, 0))
		{
			TutorialManager::OpenLockContentPopup(GoodsTypeInfoData->LockTutorialID, 0);
			return;
		}

		if (GoodsTypeInfoData->FGoodsType.IsEqual(TEXT("NAMECHANGE_TICKET")))
		{

			FText ContentText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("NameChange_Confirm")), FText::FromString(BladeIIGameImpl::GetClientDataStore().GetUserNickName()));
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
				ContentText,
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this]() {
				
				this->OnCloseButtonClicked();

				UB2UIManager::GetInstance()->OpenUI(UIFName::CreateNickName);

				UB2UICreateNickname* NickNameWidget = UB2UIManager::GetInstance()->GetUI<UB2UICreateNickname>(UIFName::CreateNickName);
				if (NickNameWidget)
				{
					NickNameWidget->SetModeChangeNickName(NativeItemData.ItemRefID);
				}
			})
			);

			return;
		}

		const FUB2GoodsShortcutButtonInfo& UseShortCutInfo = GoodsTypeInfoData->UseShortcutInfo;

		FB2UILinkManager::LinkUIScene(UseShortCutInfo.ShortcutSceneName, UseShortCutInfo.ShortcutScene_Option1, PCClassToInt(UB2LobbyInventory::GetInventorySelectedPCClass()) + 1);
	}
}

void UB2LobbyUI_ItemDetail::OnSellButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelaySellCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnUpgradeButtonClicked()
{
	// 버튼이 보이는 것 자체가 Upgrade 가능이겠지만 더블 체크
	if (IsItemUpgradeAllowed(NativeItemData) || IsItemAllowedCompositeAndSurpass(NativeItemData)){ // 내지는 이건 기본.
		// 승급은 이미 승급 모드인 상태에서 보관함 아이템 상세창을 열게 될 수 있다. 확실하게 지금 상태 종료한 후 열어본다.
		QuitItemOpModeClass<bool>::GetInstance().Signal(false); 
		data_trader::Retailer::GetInstance().RequestGetItemUpgradeCost(NativeItemData.InstanceUID);

		if (CachedInventory)
		{
			CachedInventory->SetBeforeUpgradeItem(NativeItemData);
		}
	}
}

void UB2LobbyUI_ItemDetail::OnLevelupOrCompositionOrSurPassButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayLevelupCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnCompositionForSurpassButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayCompositionForSurpassCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnEnhanceButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayEnhanceCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnEnhanceChangeButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayEnhanceChangeCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnSealButtonClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelaySealCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnEquipOrReleaseButtonClicked()
{
	bool bFailedOperation = false;

	// 장착 아이템인지, 보관함 아이템인지에 따라 반대로.
	if (bIsForEquippedItem)
	{	// 장착한 경우는 해제. 인벤토리 UI 의 아이템 아이콘 업데이트 신호를 같이 준다.
		// 인벤토리 풀인 상태
		if (CachedInventory->IsInventoryOverLimit(NativeItemData.InventoryType))
		{	
			bFailedOperation = true;

			NoticeErrorPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EquipErrorInvenFull")));
		}
		else
		{
			TArray<int64> ReqIDs;
			ReqIDs.Add(NativeItemData.InstanceUID);
			LobbyReqUnEquipItemsClass<const TArray<int64>&>::GetInstance().Signal(ReqIDs);

			PlayEquipOrReleaseSound(false);
		}
	}
	else
	{ // 보관함의 경우는 장착. 인벤토리 UI 의 아이템 아이콘 업데이트 신호를 같이 준다.
		TArray<int64> ReqIDs;
		ReqIDs.Add(NativeItemData.InstanceUID);
		LobbyReqEquipItemsClass<const TArray<int64>&>::GetInstance().Signal(ReqIDs);

		PlayEquipOrReleaseSound(true);
		
		UB2LobbyUI_ItemDetail* EquippedDetailPopup = OwnerUIManager ? Cast<UB2LobbyUI_ItemDetail>(OwnerUIManager->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_EquippedItemDetail)) : NULL;
		if (EquippedDetailPopup) 
		{ 
			// 다른 쪽에 장착 아이템 상세 창이 열려 있는 경우. 장착 위치가 같은 경우라면 실제 장착 정보가 바뀌었으니 없애준다. (업데이트를 해 볼수도 있겠지만..)
			if (EquippedDetailPopup->NativeItemData.EquipPlace == this->NativeItemData.EquipPlace)
			{
				DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);
			}
		}
	}	

	if (!bFailedOperation)
	{
		// 여하간 이 창은 없애야 함.
		CloseMe();
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemDetail::OnItemLockButtonClicked()
{
	// 인벤토리 쪽에도 lock 신호를 하고 여기 데이터도 업데이트 해야 함.

	LobbyReqToggleItemLockClass<int64, bool>::GetInstance().Signal(NativeItemData.InstanceUID, !NativeItemData.bIsLocked);
	
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
	// NativeItemData 는 서버에서 응답이 온 후에 업데이트 될 것.
	//NativeItemData.bIsLocked = !NativeItemData.bIsLocked;
	//if (CreatedItemIcon)
	//{
	//	CreatedItemIcon->UpdateItemData(NativeItemData);
	//}
}

void UB2LobbyUI_ItemDetail::OnShowChanceBTNClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayShowChanceCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnSealBoxClassBTNClicked()
{
	// 버튼 이미지 갱신 이슈가 있어 딜레이 후 로직 처리..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelaySealBoxClassCallback, 0.1f, false);
}

/*
void UB2LobbyUI_ItemDetail::OnItemOptionChangeButtonClicked()
{
	UB2UIItemOptionChange* ItemOptionChangeWidget = UB2UIManager::GetInstance()->OpenUI<UB2UIItemOptionChange>(UIFName::ItemOptionChange);
	if (ItemOptionChangeWidget)
	{
		ItemOptionChangeWidget->SetTargetItem(NativeItemData);
	}
}*/

void UB2LobbyUI_ItemDetail::CloseWidgetDelegate()
{
	OnCloseButtonClicked();
}

void UB2LobbyUI_ItemDetail::CloseMe()
{
	const ELobbySubPopups SelfPopupEnum = bIsForEquippedItem ? ELobbySubPopups::ELSPU_EquippedItemDetail : ELobbySubPopups::ELSPU_StoredItemDetail;
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(SelfPopupEnum);

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemDetail::Decomposition()
{
	this->DecompositionEquipItemEventID = DeliveryDismantleItemsClass<FB2DismantleItems>::GetInstance().Subscribe(
		[this](const FB2DismantleItems EtherPtr)
	{
		this->OnCloseButtonClicked();
		this->UnsubscribeDecomposition();
	});

	TArray<int64> ReqIDs;
	ReqIDs.Empty();
	ReqIDs.Add(NativeItemData.InstanceUID);

	LobbyReqDecompositionItemsClass<const TArray<int64>&, bool>::GetInstance().Signal(ReqIDs, false);
}

void UB2LobbyUI_ItemDetail::UnsubscribeDecomposition()
{
	if (DecompositionEquipItemEventID != 0)
	{
		DeliveryDismantleItemsClass<FB2DismantleItems>::GetInstance().Unsubscribe(DecompositionEquipItemEventID);
	}
}

void UB2LobbyUI_ItemDetail::OnDelayLevelupCallback()
{
	QuitItemOpModeClass<bool>::GetInstance().Signal(false); // 승급 모드에서 들어가는 경우가 있을 수 있음. 아마 별 일 없겠지만 이미 열려있다면 닫는게 좋을 듯.
															// 상황에 따라 다른 동작을 하도록 바꾸어야 한다.
															// 강화, 합성, 초월은 각각 나오는 조건이 다름

	if (IsItemLevelupAllowed(NativeItemData)) { // 내지는 이건 기본.
		EnterItemLevelupModeClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else if (IsItemCompositionAllowed(NativeItemData))
	{
		if (NativeItemData.bIsLocked)
		{ // 합성은 본 아이템의 고유한 정체성을 과감히 날려버리는 과정이므로 잠금이 되었을 시 사전에 묻는 팝업이 필요하다.
			AskForUnlockItemPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_UnlockAskingItemAct_Compose")));
		}
		else if (CachedInventory->IsInOtherItemPresets(NativeItemData))
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_ContainsItemOtherPreset"))),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this]() {
				if (this->CachedDataStore == nullptr)
					return;

				TArray<int64> ReqIDs;
				ReqIDs.Add(NativeItemData.InstanceUID);

				FPCItemContainer &ItemContainer = CachedDataStore->GetLocalCharacterData().GetUserAllItems();

				int32 PresetID = ItemContainer.GetContainsItemPresetID(IntToPCClass(NativeItemData.AllowedPCClass), NativeItemData.InstanceUID);

				LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Signal(PresetID, ReqIDs);
			})
			);
		}
		else
		{
			EnterItemCompositionModeClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
		}
	}
	else if (IsItemSurpassAllowed(NativeItemData))
	{
		FItemSurpassLevel InSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(NativeItemData.StarGrade);
		const int32 MaxSurpassLevel = FB2Item::ItemNormalMaxLevel + InSurpassInfo.MaxSurpassLevel;

		if (NativeItemData.Level >= MaxSurpassLevel)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, TEXT("2011")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
			);
		}
		else
		{
			EnterItemSurpassModeClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
		}
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemDetail::OnDelayEnhanceCallback()
{
	EnterItemEnhanceModeClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemDetail::OnDelayUnsealCallback()
{
	if (GetUnsealableItem())
	{
		//한번에 몇개 열라는 내용은 기획에 없어서 한번에 1개씩 열도록 하드코딩 됨
		//또한 서버에서 클래스를 0에서 시작하는게 아닌 1로 시작해서 별도로 1을 더해서 보내야 한다.
		data_trader::Retailer::GetInstance().RequestUnsealBox(NativeItemData.InstanceUID, PCClassToInt(SealBoxClass)+1, 1);
		SealBoxSelectCharClass<int32>::GetInstance().Signal(PCClassToInt(SealBoxClass));

		this->CloseMe();
	}
}

void UB2LobbyUI_ItemDetail::OnDelayShowChanceCallback()
{
	if (GetUnsealableItem())
	{
		//확률공시 팝업 작업할 것
	}
}

void UB2LobbyUI_ItemDetail::OnDelaySealBoxClassCallback()
{
	if (GetUnsealableItem())
	{
		//클래스 사이클하는 부분
		int32 tempclass = PCClassToInt(SealBoxClass);
		tempclass++;
		if (tempclass == PCClassToInt(EPCClass::EPC_End))
			tempclass = PCClassToInt(EPCClass::EPC_Gladiator);

		SealBoxClass = IntToPCClass(tempclass);

		if (TB_SealBox_Class.IsValid())
		{
			switch (SealBoxClass)
			{
			case EPCClass::EPC_Gladiator: TB_SealBox_Class->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Gladiator"))); break;
			case EPCClass::EPC_Assassin: TB_SealBox_Class->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Assassin"))); break;
			case EPCClass::EPC_Wizard: TB_SealBox_Class->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Wizard"))); break;
			case EPCClass::EPC_Fighter: TB_SealBox_Class->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Fighter"))); break;
			}

		}

		//마지막에 모든 내용을 지우고 새로 채워넣도록 한다.
		AddSealBoxContents();
	}
}

void UB2LobbyUI_ItemDetail::OnDelayConvertCallback()
{
	//UI 호출 부분
// 	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ConvertItemPopup, FDJLegacySubpopupAddPanelInfo());
// 	if (CachedLobbyGM)
// 	{
// 		UB2LobbyUI_ConvertItemPopup* ConvertItemPopup = Cast<UB2LobbyUI_ConvertItemPopup>(CachedLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ConvertItemPopup));
// 	}
	if (GetConvertableItem())
	{
		LobbyReqConvertDataClass<int32>::GetInstance().Signal((int32)(NativeItemData.InstanceUID));
	}
}

void UB2LobbyUI_ItemDetail::OnDelayDecompositionCallback()
{
	int32 DisCount = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_DISMANTLE_ITEM);

	if (NativeItemData.bIsLocked)
	{
		AskForUnlockItemPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Decomposition")));
	}
	else
	{
		if (CachedDataStore)
		{
			if (UB2UIDecompositionPopup* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIDecompositionPopup>(UIFName::DecompositionPopup))
			{
				FItemDismantleInfo DismantleInfo;
				bool bEnableDismantle = CachedDataStore->GetItemDismantleInfoData(NativeItemData.InventoryType, NativeItemData.StarGrade, DismantleInfo);
				if(bEnableDismantle)
				{
					Popup->SetEquipItemInfo(DismantleInfo.Matrial, DismantleInfo.MatrialAmount,
						FOnClickPositive::CreateLambda([this]() {
						this->Decomposition();
					}));
				}
			}
		}
	}
}

void UB2LobbyUI_ItemDetail::OnDelayEnhanceChangeCallback()
{

	QuitItemOpModeClass<bool>::GetInstance().Signal(false);

	if (CachedInventory)
	{
		UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
		LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 도 업데이트
		CachedInventory->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_EnhancementChangeMode, NativeItemData.InstanceUID);
	}
}


void UB2LobbyUI_ItemDetail::OnDelaySealCallback()
{
	UB2UISealEquipmentMain* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UISealEquipmentMain>(UIFName::SealMain);

	if (Popup)
	{
		Popup->Init();
		Popup->OpenSealItemMain(NativeItemData);
	}
}

void UB2LobbyUI_ItemDetail::OnDelayCompositionForSurpassCallback()
{
	if (NativeItemData.bIsLocked)
	{ // 합성은 본 아이템의 고유한 정체성을 과감히 날려버리는 과정이므로 잠금이 되었을 시 사전에 묻는 팝업이 필요하다.
		AskForUnlockItemPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_UnlockAskingItemAct_Compose")));
	}
	else if (CachedInventory->IsInOtherItemPresets(NativeItemData))
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_ContainsItemOtherPreset"))),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([this]() {
			if (this->CachedDataStore == nullptr)
				return;

			TArray<int64> ReqIDs;
			ReqIDs.Add(NativeItemData.InstanceUID);

			FPCItemContainer &ItemContainer = CachedDataStore->GetLocalCharacterData().GetUserAllItems();

			int32 PresetID = ItemContainer.GetContainsItemPresetID(IntToPCClass(NativeItemData.AllowedPCClass), NativeItemData.InstanceUID);

			LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Signal(PresetID, ReqIDs);
		})
		);
	}
	else
	{
		EnterItemCompositionModeClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemDetail::OnDelaySellCallback()
{
	if (NativeItemData.bIsLocked)
	{
		AskForUnlockItemPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_UnlockAskingItemAct_Sell"))); // 잠금 풀지 묻는 팝업을 띄운다.
	}
	else
	{
		if (bIsContainsPresetItem) // 모두 해제
		{
			// 인벤토리 풀인 상태
			if (CachedInventory->IsInventoryOverLimit(NativeItemData.InventoryType))
			{
				NoticeErrorPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EquipErrorInvenFull")));
			}
			else if (bIsContainsOtherPreset)
			{ // 지금 보이는 거 말고 다른 프리셋에서도 장착되어 있으므로 전체 해제할꺼냐 물어본다.
				UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
					BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
					BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_UnequipItemAllPreset"))),
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::YesOrNo,
					FMsgPopupOnClick::CreateLambda([this]() {
					TArray<int64> ReqIDs;
					ReqIDs.Add(NativeItemData.InstanceUID);
					LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Signal(-1, ReqIDs);

					this->CloseMe(); // 이 창도 닫는다.
				})
				);
			}
			else
			{ // 이 경우 실제로는 현재 프리셋에서만 해제하면 됨. 전체 해제할꺼나 물어보지 말고 그냥 바로.
				TArray<int64> ReqIDs;
				ReqIDs.Add(NativeItemData.InstanceUID);
				LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Signal(-1, ReqIDs);

				this->CloseMe(); // 이 창도 닫는다.
			}
		}
		else
		{
			// 바로 판매는 아니고 판매여부를 묻는 (특수) 팝업. 좀 구닥다리 방식으로 ㅋ
			DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_SellItemPopup, FDJLegacySubpopupAddPanelInfo());
			if (CachedLobbyGM)
			{
				UB2LobbyUI_SellItemPopup* SellItemPopup = Cast<UB2LobbyUI_SellItemPopup>(CachedLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_SellItemPopup));
				if (SellItemPopup)
				{
					if (GetConsumableItem())
					{
						SellItemPopup->PopupMain(NativeItemData,
							FSellItemPopupOnClick::CreateLambda([this, SellItemPopup]() {
							LobbyReqSellConsumableItemClass<int64, int32>::GetInstance().Signal(this->NativeItemData.InstanceUID, SellItemPopup->GetSellItemCount());
							this->CloseMe();
						}),
							FSellItemPopupOnClick::CreateLambda([this]() {})
							);
					}
					else
					{
						SellItemPopup->PopupMain(NativeItemData,
							FSellItemPopupOnClick::CreateLambda([this]() {
							LobbyReqSellItemClass<int64, bool>::GetInstance().Signal(this->NativeItemData.InstanceUID, false);
							this->CloseMe(); // 팔아치웠으니 이 창도 닫는다.
						}),
							FSellItemPopupOnClick::CreateLambda([this]() {})
							);
					}
				}
			}
		}
	}
}