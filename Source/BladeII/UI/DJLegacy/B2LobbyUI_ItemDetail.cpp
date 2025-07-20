// Fill out your copyright notice in the Description page of Project Settings.

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

		//可记函版 昏力
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
		bIsContainsOtherPreset = ItemContainer.IsInOtherItemPresets(CastedPCClass, NativeItemData.InstanceUID); // 促弗 preset 俊 器窃登绢 乐绰瘤..
	}
	UE_LOG(LogBladeII, Log, TEXT("Item detail window of [UID] %I64d and [RefID] %d has been invoked"), NativeItemData.InstanceUID, NativeItemData.ItemRefID);

	UpdateItemIcon();

	UpdateSubWidgets(); // 夯拜 硅摹茄 widget 甸 诀单捞飘

	UpdateItemOptions(); // 腹篮 何盒阑 瞒瘤窍绰 可记 府胶飘.

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
		
	// Anchor 汲沥俊 蝶扼 GetSize 绰 盔窍绰 蔼捞 救 唱棵 巴捞骨肺 林狼.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(X_P_ItemIconPanel->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	// ItemIconPanelNRef 困俊 弥辆利牢 酒捞袍 酒捞能 widget 积己
	CreatedItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, X_P_ItemIconPanel.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
		));

	if (CreatedItemIcon)
	{
		CreatedItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow); // 焊包窃俊辑 焊咯瘤带 割啊瘤 沥焊绰 焊咯林瘤 臼档废 眠啊 汲沥.
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
	// Native ref 甸阑 登绰措肺 诀单捞飘

	// 悼老 困摹俊 厘馒茄 酒捞袍 沥焊啊 鞘夸且 荐 乐栏骨肺 哗柯促.
	FB2Item EquippedItemAtSamePlace;
	bool bHasEquippedAtSamePlace = CachedInventory ? CachedInventory->GetEquippedItemAtPlace(EquippedItemAtSamePlace, NativeItemData.EquipPlace) : false;
	// bHasEquippedAtSamePlace 啊 true 扼搁 EquippedItemAtSamePlace 捞 valid 茄 巴.

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
		// EnhanceExp 绰 0 ~ 100 荤捞 欺季飘肺 啊沥. 辟单 ProgressBar 狼 SetPercent 绰 角力 弊 欺季飘啊 酒囱 0.0 ~ 1.0 荤捞 蔼捞扼绰 霸 窃沥 せ
		PB_ItemExpProgress->SetPercent( // 弥措 饭骇俊辑绰 傲 父顶 盲况霖促.
			(NativeItemData.Level == NativeItemData.MaxLevel) ? 1.0f : NativeItemData.Exp * 0.01f
			);
	}
	if (TB_ItemExpPercent.IsValid())
	{
		// 夸霸 printf 沁阑 矫 馆棵覆 登绰 淀 窃. 弊霸 窍鞘 99.99 啊 100.0 栏肺 钎矫登绰 霸 巩力啊 登绢辑.. 老何矾 磊抚. 酒付 谍单档 粱 鞘夸且 淀 茄单.. ぇぇ
		float TruncatedExp = FMath::FloorToFloat(NativeItemData.Exp * 10.0f) * 0.1f;

		TB_ItemExpPercent->SetText(
			(NativeItemData.Level == NativeItemData.MaxLevel) ?
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Maximum_ValueMark")) : // 弥措饭骇俊辑绰 箭磊 措脚 max 肺 钎矫.
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
	{ // 厘馒 滚瓢阑 厘馒 酒捞袍 寇俊 荤侩且 捞蜡啊 绝瘤.. 傲 见扁绊 酵瘤父 老窜篮 沥纺捞 救登辑..
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

	//可记函版 昏力
	/*if (SB_ItemOptionChange.IsValid())
		SB_ItemOptionChange->SetVisibility(GetItemOptionChangeButtonVisibility());*/

	/*if (TB_ItemOptionChange.IsValid())
		TB_ItemOptionChange->SetText(GetItemOptionChangeText());*/

	if (X_P_ItemLockButtonSet.IsValid())
	{ // 厘馒 酒捞袍 寇浚 泪陛 滚瓢阑 焊老 鞘夸啊 绝促.
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
	////豪牢窃俊 甸绢哎 酒捞袍俊 措秦 老何绰 流立 林绰 RefID档 乐栏唱 厘厚贸烦 蜡眠窍绰 单捞磐父 罐阑 荐档 乐促.
	////弊 蜡眠等 单捞磐客 泅犁 努贰胶(SealBoxClass)俊 蝶扼辑 RefID 啊 崔扼瘤骨肺 弊 蔼俊 嘎霸 眠啊且 荐 乐档废 茄促.

	//if (originalSealBox.sealboxRefID < 0)
	//	return;

	//TArray<FSealBoxRewardRateInfo> slot1, slot2, slot3, slot4, slot5;
	//CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot1, slot1);
	//CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot2, slot2);
	//CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot3, slot3);
	//CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot4, slot4);
	//CachedDataStore->GetSealBoxRewardRateInfo(originalSealBox.rewardSlot5, slot5);

	//TArray<FSealBoxItem> savedItemData;
	//TArray<FSealBoxEquipment> savedEquipmentData;

	//for (int32 i = 0; i < slot1.Num(); i++)
	//{
	//	if (slot1[i].itemType == b2network::B2RewardType::ITEM)
	//	{
	//		if (slot1[i].starGrade > 0)
	//		{
	//			if (savedEquipmentData.Num() == 0)
	//			{
	//				FSealBoxEquipment newData;
	//				newData.starGrade = slot1[i].starGrade;
	//				newData.groupID = slot1[i].groupID;
	//				newData.equipCategory = slot1[i].equipCategory;
	//				newData.equipPosition = slot1[i].equipPlace;
	//				newData.quality = slot1[i].quality;
	//				newData.enhancedLevel = slot1[i].enhanceLevel;
	//				newData.surpassCount = slot1[i].surpassCount;

	//				savedEquipmentData.Add(newData);
	//			}
	//			else
	//			{
	//				bool sameItem = false;
	//				for (int32 j = 0; j < savedEquipmentData.Num(); j++)
	//				{
	//					if (savedEquipmentData[j].starGrade == slot1[i].starGrade)
	//						if (savedEquipmentData[j].groupID == slot1[i].groupID)
	//							if (savedEquipmentData[j].equipCategory == slot1[i].equipCategory)
	//								if (savedEquipmentData[j].equipPosition == slot1[i].equipPlace)
	//									if (savedEquipmentData[j].quality == slot1[i].quality)
	//										if (savedEquipmentData[j].enhancedLevel == slot1[i].enhanceLevel)
	//											if (savedEquipmentData[j].surpassCount == slot1[i].surpassCount)
	//												sameItem = true;
	//				}

	//				if (sameItem == false)
	//				{
	//					FSealBoxEquipment newData;
	//					newData.starGrade = slot1[i].starGrade;
	//					newData.groupID = slot1[i].groupID;
	//					newData.equipCategory = slot1[i].equipCategory;
	//					newData.equipPosition = slot1[i].equipPlace;
	//					newData.quality = slot1[i].quality;
	//					newData.enhancedLevel = slot1[i].enhanceLevel;
	//					newData.surpassCount = slot1[i].surpassCount;

	//					savedEquipmentData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (savedItemData.Num() == 0)
	//		{
	//			if (slot1[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot1[i].itemType) > 0)
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = CachedDataStore->GetRewardItemId(slot1[i].itemType);
	//					newData.minQuantity = slot1[i].minQuantity;
	//					newData.maxQuantity = slot1[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				FSealBoxItem newData;
	//				newData.itemRefID = slot1[i].itemRefID;
	//				newData.minQuantity = slot1[i].minQuantity;
	//				newData.maxQuantity = slot1[i].maxQuantity;
	//				savedItemData.Add(newData);
	//			}
	//		}
	//		else
	//		{
	//			bool alreadyHave = false;
	//			if (slot1[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot1[i].itemType) > 0)
	//				{
	//					for (int32 j = 0; j < savedItemData.Num(); j++)
	//					{
	//						if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot1[i].itemType))
	//						{
	//							alreadyHave = true;
	//							if (savedItemData[j].minQuantity > slot1[i].minQuantity)
	//								savedItemData[j].minQuantity = slot1[i].minQuantity;
	//							if (savedItemData[j].maxQuantity < slot1[i].maxQuantity)
	//								savedItemData[j].maxQuantity = slot1[i].maxQuantity;
	//						}
	//					}
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				for (int32 j = 0; j < savedItemData.Num(); j++)
	//				{
	//					if (savedItemData[j].itemRefID == slot1[i].itemRefID)
	//					{
	//						alreadyHave = true;
	//						if (savedItemData[j].minQuantity > slot1[i].minQuantity)
	//							savedItemData[j].minQuantity = slot1[i].minQuantity;
	//						if (savedItemData[j].maxQuantity < slot1[i].maxQuantity)
	//							savedItemData[j].maxQuantity = slot1[i].maxQuantity;
	//					}
	//				}
	//			}

	//			if (alreadyHave == false)
	//			{
	//				if (slot1[i].itemRefID == 0)
	//				{
	//					if (CachedDataStore->GetRewardItemId(slot1[i].itemType) > 0)
	//					{
	//						FSealBoxItem newData;
	//						newData.itemRefID = CachedDataStore->GetRewardItemId(slot1[i].itemType);
	//						newData.minQuantity = slot1[i].minQuantity;
	//						newData.maxQuantity = slot1[i].maxQuantity;
	//						savedItemData.Add(newData);
	//					}
	//					else
	//					{
	//						continue;
	//					}
	//				}
	//				else
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = slot1[i].itemRefID;
	//					newData.minQuantity = slot1[i].minQuantity;
	//					newData.maxQuantity = slot1[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//}
	//for (int32 i = 0; i < slot2.Num(); i++)
	//{
	//	if (slot2[i].itemType == b2network::B2RewardType::ITEM)
	//	{
	//		if (slot2[i].starGrade > 0)
	//		{
	//			if (savedEquipmentData.Num() == 0)
	//			{
	//				FSealBoxEquipment newData;
	//				newData.starGrade = slot2[i].starGrade;
	//				newData.groupID = slot2[i].groupID;
	//				newData.equipCategory = slot2[i].equipCategory;
	//				newData.equipPosition = slot2[i].equipPlace;
	//				newData.quality = slot2[i].quality;
	//				newData.enhancedLevel = slot2[i].enhanceLevel;
	//				newData.surpassCount = slot2[i].surpassCount;

	//				savedEquipmentData.Add(newData);
	//			}
	//			else
	//			{
	//				bool sameItem = false;
	//				for (int32 j = 0; j < savedEquipmentData.Num(); j++)
	//				{
	//					if (savedEquipmentData[j].starGrade == slot2[i].starGrade)
	//						if (savedEquipmentData[j].groupID == slot2[i].groupID)
	//							if (savedEquipmentData[j].equipCategory == slot2[i].equipCategory)
	//								if (savedEquipmentData[j].equipPosition == slot2[i].equipPlace)
	//									if (savedEquipmentData[j].quality == slot2[i].quality)
	//										if (savedEquipmentData[j].enhancedLevel == slot2[i].enhanceLevel)
	//											if (savedEquipmentData[j].surpassCount == slot2[i].surpassCount)
	//												sameItem = true;
	//				}

	//				if (sameItem == false)
	//				{
	//					FSealBoxEquipment newData;
	//					newData.starGrade = slot2[i].starGrade;
	//					newData.groupID = slot2[i].groupID;
	//					newData.equipCategory = slot2[i].equipCategory;
	//					newData.equipPosition = slot2[i].equipPlace;
	//					newData.quality = slot2[i].quality;
	//					newData.enhancedLevel = slot2[i].enhanceLevel;
	//					newData.surpassCount = slot2[i].surpassCount;

	//					savedEquipmentData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (savedItemData.Num() == 0)
	//		{
	//			if (slot2[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot2[i].itemType) > 0)
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = CachedDataStore->GetRewardItemId(slot2[i].itemType);
	//					newData.minQuantity = slot2[i].minQuantity;
	//					newData.maxQuantity = slot2[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				FSealBoxItem newData;
	//				newData.itemRefID = slot2[i].itemRefID;
	//				newData.minQuantity = slot2[i].minQuantity;
	//				newData.maxQuantity = slot2[i].maxQuantity;
	//				savedItemData.Add(newData);
	//			}
	//		}
	//		else
	//		{
	//			bool alreadyHave = false;
	//			if (slot2[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot2[i].itemType) > 0)
	//				{
	//					for (int32 j = 0; j < savedItemData.Num(); j++)
	//					{
	//						if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot2[i].itemType))
	//						{
	//							alreadyHave = true;
	//							if (savedItemData[j].minQuantity > slot2[i].minQuantity)
	//								savedItemData[j].minQuantity = slot2[i].minQuantity;
	//							if (savedItemData[j].maxQuantity < slot2[i].maxQuantity)
	//								savedItemData[j].maxQuantity = slot2[i].maxQuantity;
	//						}
	//					}
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				for (int32 j = 0; j < savedItemData.Num(); j++)
	//				{
	//					if (savedItemData[j].itemRefID == slot2[i].itemRefID)
	//					{
	//						alreadyHave = true;
	//						if (savedItemData[j].minQuantity > slot2[i].minQuantity)
	//							savedItemData[j].minQuantity = slot2[i].minQuantity;
	//						if (savedItemData[j].maxQuantity < slot2[i].maxQuantity)
	//							savedItemData[j].maxQuantity = slot2[i].maxQuantity;
	//					}
	//				}
	//			}

	//			if (alreadyHave == false)
	//			{
	//				if (slot2[i].itemRefID == 0)
	//				{
	//					if (CachedDataStore->GetRewardItemId(slot2[i].itemType) > 0)
	//					{
	//						FSealBoxItem newData;
	//						newData.itemRefID = CachedDataStore->GetRewardItemId(slot2[i].itemType);
	//						newData.minQuantity = slot2[i].minQuantity;
	//						newData.maxQuantity = slot2[i].maxQuantity;
	//						savedItemData.Add(newData);
	//					}
	//					else
	//					{
	//						continue;
	//					}
	//				}
	//				else
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = slot2[i].itemRefID;
	//					newData.minQuantity = slot2[i].minQuantity;
	//					newData.maxQuantity = slot2[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//}
	//for (int32 i = 0; i < slot3.Num(); i++)
	//{
	//	if (slot3[i].itemType == b2network::B2RewardType::ITEM)
	//	{
	//		if (slot3[i].starGrade > 0)
	//		{
	//			if (savedEquipmentData.Num() == 0)
	//			{
	//				FSealBoxEquipment newData;
	//				newData.starGrade = slot3[i].starGrade;
	//				newData.groupID = slot3[i].groupID;
	//				newData.equipCategory = slot3[i].equipCategory;
	//				newData.equipPosition = slot3[i].equipPlace;
	//				newData.quality = slot3[i].quality;
	//				newData.enhancedLevel = slot3[i].enhanceLevel;
	//				newData.surpassCount = slot3[i].surpassCount;

	//				savedEquipmentData.Add(newData);
	//			}
	//			else
	//			{
	//				bool sameItem = false;
	//				for (int32 j = 0; j < savedEquipmentData.Num(); j++)
	//				{
	//					if (savedEquipmentData[j].starGrade == slot3[i].starGrade)
	//						if (savedEquipmentData[j].groupID == slot3[i].groupID)
	//							if (savedEquipmentData[j].equipCategory == slot3[i].equipCategory)
	//								if (savedEquipmentData[j].equipPosition == slot3[i].equipPlace)
	//									if (savedEquipmentData[j].quality == slot3[i].quality)
	//										if (savedEquipmentData[j].enhancedLevel == slot3[i].enhanceLevel)
	//											if (savedEquipmentData[j].surpassCount == slot3[i].surpassCount)
	//												sameItem = true;
	//				}

	//				if (sameItem == false)
	//				{
	//					FSealBoxEquipment newData;
	//					newData.starGrade = slot3[i].starGrade;
	//					newData.groupID = slot3[i].groupID;
	//					newData.equipCategory = slot3[i].equipCategory;
	//					newData.equipPosition = slot3[i].equipPlace;
	//					newData.quality = slot3[i].quality;
	//					newData.enhancedLevel = slot3[i].enhanceLevel;
	//					newData.surpassCount = slot3[i].surpassCount;

	//					savedEquipmentData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (savedItemData.Num() == 0)
	//		{
	//			if (slot3[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot3[i].itemType) > 0)
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = CachedDataStore->GetRewardItemId(slot3[i].itemType);
	//					newData.minQuantity = slot3[i].minQuantity;
	//					newData.maxQuantity = slot3[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				FSealBoxItem newData;
	//				newData.itemRefID = slot3[i].itemRefID;
	//				newData.minQuantity = slot3[i].minQuantity;
	//				newData.maxQuantity = slot3[i].maxQuantity;
	//				savedItemData.Add(newData);
	//			}
	//		}
	//		else
	//		{
	//			bool alreadyHave = false;
	//			if (slot3[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot3[i].itemType) > 0)
	//				{
	//					for (int32 j = 0; j < savedItemData.Num(); j++)
	//					{
	//						if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot3[i].itemType))
	//						{
	//							alreadyHave = true;
	//							if (savedItemData[j].minQuantity > slot3[i].minQuantity)
	//								savedItemData[j].minQuantity = slot3[i].minQuantity;
	//							if (savedItemData[j].maxQuantity < slot3[i].maxQuantity)
	//								savedItemData[j].maxQuantity = slot3[i].maxQuantity;
	//						}
	//					}
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				for (int32 j = 0; j < savedItemData.Num(); j++)
	//				{
	//					if (savedItemData[j].itemRefID == slot3[i].itemRefID)
	//					{
	//						alreadyHave = true;
	//						if (savedItemData[j].minQuantity > slot3[i].minQuantity)
	//							savedItemData[j].minQuantity = slot3[i].minQuantity;
	//						if (savedItemData[j].maxQuantity < slot3[i].maxQuantity)
	//							savedItemData[j].maxQuantity = slot3[i].maxQuantity;
	//					}
	//				}
	//			}

	//			if (alreadyHave == false)
	//			{
	//				if (slot3[i].itemRefID == 0)
	//				{
	//					if (CachedDataStore->GetRewardItemId(slot3[i].itemType) > 0)
	//					{
	//						FSealBoxItem newData;
	//						newData.itemRefID = CachedDataStore->GetRewardItemId(slot3[i].itemType);
	//						newData.minQuantity = slot3[i].minQuantity;
	//						newData.maxQuantity = slot3[i].maxQuantity;
	//						savedItemData.Add(newData);
	//					}
	//					else
	//					{
	//						continue;
	//					}
	//				}
	//				else
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = slot3[i].itemRefID;
	//					newData.minQuantity = slot3[i].minQuantity;
	//					newData.maxQuantity = slot3[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//}
	//for (int32 i = 0; i < slot4.Num(); i++)
	//{
	//	if (slot4[i].itemType == b2network::B2RewardType::ITEM)
	//	{
	//		if (slot4[i].starGrade > 0)
	//		{
	//			if (savedEquipmentData.Num() == 0)
	//			{
	//				FSealBoxEquipment newData;
	//				newData.starGrade = slot4[i].starGrade;
	//				newData.groupID = slot4[i].groupID;
	//				newData.equipCategory = slot4[i].equipCategory;
	//				newData.equipPosition = slot4[i].equipPlace;
	//				newData.quality = slot4[i].quality;
	//				newData.enhancedLevel = slot4[i].enhanceLevel;
	//				newData.surpassCount = slot4[i].surpassCount;

	//				savedEquipmentData.Add(newData);
	//			}
	//			else
	//			{
	//				bool sameItem = false;
	//				for (int32 j = 0; j < savedEquipmentData.Num(); j++)
	//				{
	//					if (savedEquipmentData[j].starGrade == slot4[i].starGrade)
	//						if (savedEquipmentData[j].groupID == slot4[i].groupID)
	//							if (savedEquipmentData[j].equipCategory == slot4[i].equipCategory)
	//								if (savedEquipmentData[j].equipPosition == slot4[i].equipPlace)
	//									if (savedEquipmentData[j].quality == slot4[i].quality)
	//										if (savedEquipmentData[j].enhancedLevel == slot4[i].enhanceLevel)
	//											if (savedEquipmentData[j].surpassCount == slot4[i].surpassCount)
	//												sameItem = true;
	//				}

	//				if (sameItem == false)
	//				{
	//					FSealBoxEquipment newData;
	//					newData.starGrade = slot4[i].starGrade;
	//					newData.groupID = slot4[i].groupID;
	//					newData.equipCategory = slot4[i].equipCategory;
	//					newData.equipPosition = slot4[i].equipPlace;
	//					newData.quality = slot4[i].quality;
	//					newData.enhancedLevel = slot4[i].enhanceLevel;
	//					newData.surpassCount = slot4[i].surpassCount;

	//					savedEquipmentData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (savedItemData.Num() == 0)
	//		{
	//			if (slot4[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot4[i].itemType) > 0)
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = CachedDataStore->GetRewardItemId(slot4[i].itemType);
	//					newData.minQuantity = slot4[i].minQuantity;
	//					newData.maxQuantity = slot4[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				FSealBoxItem newData;
	//				newData.itemRefID = slot4[i].itemRefID;
	//				newData.minQuantity = slot4[i].minQuantity;
	//				newData.maxQuantity = slot4[i].maxQuantity;
	//				savedItemData.Add(newData);
	//			}
	//		}
	//		else
	//		{
	//			bool alreadyHave = false;
	//			if (slot4[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot4[i].itemType) > 0)
	//				{
	//					for (int32 j = 0; j < savedItemData.Num(); j++)
	//					{
	//						if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot4[i].itemType))
	//						{
	//							alreadyHave = true;
	//							if (savedItemData[j].minQuantity > slot4[i].minQuantity)
	//								savedItemData[j].minQuantity = slot4[i].minQuantity;
	//							if (savedItemData[j].maxQuantity < slot4[i].maxQuantity)
	//								savedItemData[j].maxQuantity = slot4[i].maxQuantity;
	//						}
	//					}
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				for (int32 j = 0; j < savedItemData.Num(); j++)
	//				{
	//					if (savedItemData[j].itemRefID == slot4[i].itemRefID)
	//					{
	//						alreadyHave = true;
	//						if (savedItemData[j].minQuantity > slot4[i].minQuantity)
	//							savedItemData[j].minQuantity = slot4[i].minQuantity;
	//						if (savedItemData[j].maxQuantity < slot4[i].maxQuantity)
	//							savedItemData[j].maxQuantity = slot4[i].maxQuantity;
	//					}
	//				}
	//			}

	//			if (alreadyHave == false)
	//			{
	//				if (slot4[i].itemRefID == 0)
	//				{
	//					if (CachedDataStore->GetRewardItemId(slot4[i].itemType) > 0)
	//					{
	//						FSealBoxItem newData;
	//						newData.itemRefID = CachedDataStore->GetRewardItemId(slot4[i].itemType);
	//						newData.minQuantity = slot4[i].minQuantity;
	//						newData.maxQuantity = slot4[i].maxQuantity;
	//						savedItemData.Add(newData);
	//					}
	//					else
	//					{
	//						continue;
	//					}
	//				}
	//				else
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = slot4[i].itemRefID;
	//					newData.minQuantity = slot4[i].minQuantity;
	//					newData.maxQuantity = slot4[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//}
	//for (int32 i = 0; i < slot5.Num(); i++)
	//{
	//	if (slot5[i].itemType == b2network::B2RewardType::ITEM)
	//	{
	//		if (slot5[i].starGrade > 0)
	//		{
	//			if (savedEquipmentData.Num() == 0)
	//			{
	//				FSealBoxEquipment newData;
	//				newData.starGrade = slot5[i].starGrade;
	//				newData.groupID = slot5[i].groupID;
	//				newData.equipCategory = slot5[i].equipCategory;
	//				newData.equipPosition = slot5[i].equipPlace;
	//				newData.quality = slot5[i].quality;
	//				newData.enhancedLevel = slot5[i].enhanceLevel;
	//				newData.surpassCount = slot5[i].surpassCount;

	//				savedEquipmentData.Add(newData);
	//			}
	//			else
	//			{
	//				bool sameItem = false;
	//				for (int32 j = 0; j < savedEquipmentData.Num(); j++)
	//				{
	//					if (savedEquipmentData[j].starGrade == slot5[i].starGrade)
	//						if (savedEquipmentData[j].groupID == slot5[i].groupID)
	//							if (savedEquipmentData[j].equipCategory == slot5[i].equipCategory)
	//								if (savedEquipmentData[j].equipPosition == slot5[i].equipPlace)
	//									if (savedEquipmentData[j].quality == slot5[i].quality)
	//										if (savedEquipmentData[j].enhancedLevel == slot5[i].enhanceLevel)
	//											if (savedEquipmentData[j].surpassCount == slot5[i].surpassCount)
	//												sameItem = true;
	//				}

	//				if (sameItem == false)
	//				{
	//					FSealBoxEquipment newData;
	//					newData.starGrade = slot5[i].starGrade;
	//					newData.groupID = slot5[i].groupID;
	//					newData.equipCategory = slot5[i].equipCategory;
	//					newData.equipPosition = slot5[i].equipPlace;
	//					newData.quality = slot5[i].quality;
	//					newData.enhancedLevel = slot5[i].enhanceLevel;
	//					newData.surpassCount = slot5[i].surpassCount;

	//					savedEquipmentData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (savedItemData.Num() == 0)
	//		{
	//			if (slot5[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot5[i].itemType) > 0)
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = CachedDataStore->GetRewardItemId(slot5[i].itemType);
	//					newData.minQuantity = slot5[i].minQuantity;
	//					newData.maxQuantity = slot5[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				FSealBoxItem newData;
	//				newData.itemRefID = slot5[i].itemRefID;
	//				newData.minQuantity = slot5[i].minQuantity;
	//				newData.maxQuantity = slot5[i].maxQuantity;
	//				savedItemData.Add(newData);
	//			}
	//		}
	//		else
	//		{
	//			bool alreadyHave = false;
	//			if (slot5[i].itemRefID == 0)
	//			{
	//				if (CachedDataStore->GetRewardItemId(slot5[i].itemType) > 0)
	//				{
	//					for (int32 j = 0; j < savedItemData.Num(); j++)
	//					{
	//						if (savedItemData[j].itemRefID == CachedDataStore->GetRewardItemId(slot5[i].itemType))
	//						{
	//							alreadyHave = true;
	//							if (savedItemData[j].minQuantity > slot5[i].minQuantity)
	//								savedItemData[j].minQuantity = slot5[i].minQuantity;
	//							if (savedItemData[j].maxQuantity < slot5[i].maxQuantity)
	//								savedItemData[j].maxQuantity = slot5[i].maxQuantity;
	//						}
	//					}
	//				}
	//				else
	//				{
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				for (int32 j = 0; j < savedItemData.Num(); j++)
	//				{
	//					if (savedItemData[j].itemRefID == slot5[i].itemRefID)
	//					{
	//						alreadyHave = true;
	//						if (savedItemData[j].minQuantity > slot5[i].minQuantity)
	//							savedItemData[j].minQuantity = slot5[i].minQuantity;
	//						if (savedItemData[j].maxQuantity < slot5[i].maxQuantity)
	//							savedItemData[j].maxQuantity = slot5[i].maxQuantity;
	//					}
	//				}
	//			}

	//			if (alreadyHave == false)
	//			{
	//				if (slot5[i].itemRefID == 0)
	//				{
	//					if (CachedDataStore->GetRewardItemId(slot5[i].itemType) > 0)
	//					{
	//						FSealBoxItem newData;
	//						newData.itemRefID = CachedDataStore->GetRewardItemId(slot5[i].itemType);
	//						newData.minQuantity = slot5[i].minQuantity;
	//						newData.maxQuantity = slot5[i].maxQuantity;
	//						savedItemData.Add(newData);
	//					}
	//					else
	//					{
	//						continue;
	//					}
	//				}
	//				else
	//				{
	//					FSealBoxItem newData;
	//					newData.itemRefID = slot5[i].itemRefID;
	//					newData.minQuantity = slot5[i].minQuantity;
	//					newData.maxQuantity = slot5[i].maxQuantity;
	//					savedItemData.Add(newData);
	//				}
	//			}
	//		}
	//	}
	//}
	//
	//if (SB_SealBox_ContainList.IsValid())
	//{
	//	SB_SealBox_ContainList->ClearChildren();

	//	for (int32 i = 0; i < savedItemData.Num(); i++)
	//	{
	//		UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), SealBoxContainListBP);
	//		SealBoxIconWidget->CacheAssets();
	//		SealBoxIconWidget->SetItem(savedItemData[i].itemRefID, savedItemData[i].minQuantity, savedItemData[i].maxQuantity);
	//		SB_SealBox_ContainList->AddChild(SealBoxIconWidget);
	//	}

	//	for (int32 i = 0; i < savedEquipmentData.Num(); i++)
	//	{
	//		UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), SealBoxContainListBP);
	//		SealBoxIconWidget->CacheAssets();
	//		SealBoxIconWidget->SetItem(savedEquipmentData[i].groupID, SealBoxClass, savedEquipmentData[i].equipCategory, savedEquipmentData[i].equipPosition, savedEquipmentData[i].starGrade, savedEquipmentData[i].quality, savedEquipmentData[i].enhancedLevel, savedEquipmentData[i].surpassCount);
	//		SB_SealBox_ContainList->AddChild(SealBoxIconWidget);
	//	}
	//}
}

bool UB2LobbyUI_ItemDetail::GetConvertableItem()	// 窍靛内爹 //函券 矫胶袍捞 荤扼瘤绊 力累家肺 软荐登辑 荤侩窍瘤 臼澜
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
		if (!bIsForEquippedItem && EquippedOneAtSamePlace) // 荐摹 厚背 钎矫绰 焊包窃 侩捞绊 悼老 困摹俊 厘馒茄 酒捞袍捞 乐阑 版快.
		{
			TB_PrimaryPointDelta->SetVisibility(ESlateVisibility::Visible);
			TB_CombatPowerDelta->SetVisibility(ESlateVisibility::Visible);

			int32 PrimaryPointDelta = B2UIUtil::DifferenceItemPrimaryPoint(NativeItemData, *EquippedOneAtSamePlace);
			int32 CombatDelta = NativeItemData.Power - EquippedOneAtSamePlace->Power;

			// - 绰 弊成 箭磊父 持绢档 登瘤父 + 绰 弊成 救唱坷骨肺 傲 荐摹绰 例措蔼栏肺..
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
		FB2Item EquippedItemAtSamePlace; // 悼老 困摹俊 厘馒等 酒捞袍 沥焊.
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
		(!bRequiresSetEffectDisplay) || // 笛 吝 窍唱绰 措惑捞 登绢具.
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

	// 轰器飘 郴 困摹 嘎苗淋. 厘馒 酒捞袍 拳搁篮 谅螟俊, 焊包窃 酒捞袍 拳搁篮 快螟俊.

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
		if (bIsContainsPresetItem) // 窍唱父 器窃登绢档 老窜 葛滴 秦力 滚瓢篮 焊捞档废 窍唱? 绢路电 厘馒阑 窍变 茄 芭聪 迫 鉴 绝垒酒?
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
{ // 钦己苞 鞍篮 炼扒俊辑 悼矫俊 唱坷扁 锭巩俊 滚瓢阑 蝶肺 备己.
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Upgrade")));
}

FText UB2LobbyUI_ItemDetail::GetLevelupOrCompositionOrSurPassButtonText() const
{// 碍拳, 钦己, 檬岿篮 阿阿 唱坷绰 炼扒捞 促抚
	if (IsItemLevelupAllowed(NativeItemData)) { // 郴瘤绰 捞扒 扁夯.
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
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayUnsealCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnConvertButtonClicked()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayConvertCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnDecompositionButtonClicked()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
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
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelaySellCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnUpgradeButtonClicked()
{
	// 滚瓢捞 焊捞绰 巴 磊眉啊 Upgrade 啊瓷捞摆瘤父 歹喉 眉农
	if (IsItemUpgradeAllowed(NativeItemData) || IsItemAllowedCompositeAndSurpass(NativeItemData)){ // 郴瘤绰 捞扒 扁夯.
		// 铰鞭篮 捞固 铰鞭 葛靛牢 惑怕俊辑 焊包窃 酒捞袍 惑技芒阑 凯霸 瞪 荐 乐促. 犬角窍霸 瘤陛 惑怕 辆丰茄 饶 凯绢夯促.
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
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayLevelupCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnCompositionForSurpassButtonClicked()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayCompositionForSurpassCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnEnhanceButtonClicked()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayEnhanceCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnEnhanceChangeButtonClicked()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayEnhanceChangeCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnSealButtonClicked()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelaySealCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnEquipOrReleaseButtonClicked()
{
	bool bFailedOperation = false;

	// 厘馒 酒捞袍牢瘤, 焊包窃 酒捞袍牢瘤俊 蝶扼 馆措肺.
	if (bIsForEquippedItem)
	{	// 厘馒茄 版快绰 秦力. 牢亥配府 UI 狼 酒捞袍 酒捞能 诀单捞飘 脚龋甫 鞍捞 霖促.
		// 牢亥配府 钱牢 惑怕
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
	{ // 焊包窃狼 版快绰 厘馒. 牢亥配府 UI 狼 酒捞袍 酒捞能 诀单捞飘 脚龋甫 鞍捞 霖促.
		TArray<int64> ReqIDs;
		ReqIDs.Add(NativeItemData.InstanceUID);
		LobbyReqEquipItemsClass<const TArray<int64>&>::GetInstance().Signal(ReqIDs);

		PlayEquipOrReleaseSound(true);
		
		UB2LobbyUI_ItemDetail* EquippedDetailPopup = OwnerUIManager ? Cast<UB2LobbyUI_ItemDetail>(OwnerUIManager->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_EquippedItemDetail)) : NULL;
		if (EquippedDetailPopup) 
		{ 
			// 促弗 率俊 厘馒 酒捞袍 惑技 芒捞 凯妨 乐绰 版快. 厘馒 困摹啊 鞍篮 版快扼搁 角力 厘馒 沥焊啊 官差菌栏聪 绝局霖促. (诀单捞飘甫 秦 杭荐档 乐摆瘤父..)
			if (EquippedDetailPopup->NativeItemData.EquipPlace == this->NativeItemData.EquipPlace)
			{
				DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);
			}
		}
	}	

	if (!bFailedOperation)
	{
		// 咯窍埃 捞 芒篮 绝局具 窃.
		CloseMe();
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemDetail::OnItemLockButtonClicked()
{
	// 牢亥配府 率俊档 lock 脚龋甫 窍绊 咯扁 单捞磐档 诀单捞飘 秦具 窃.

	LobbyReqToggleItemLockClass<int64, bool>::GetInstance().Signal(NativeItemData.InstanceUID, !NativeItemData.bIsLocked);
	
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
	// NativeItemData 绰 辑滚俊辑 览翠捞 柯 饶俊 诀单捞飘 瞪 巴.
	//NativeItemData.bIsLocked = !NativeItemData.bIsLocked;
	//if (CreatedItemIcon)
	//{
	//	CreatedItemIcon->UpdateItemData(NativeItemData);
	//}
}

void UB2LobbyUI_ItemDetail::OnShowChanceBTNClicked()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_ItemDetail::OnDelayShowChanceCallback, 0.1f, false);
}

void UB2LobbyUI_ItemDetail::OnSealBoxClassBTNClicked()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
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
	QuitItemOpModeClass<bool>::GetInstance().Signal(false); // 铰鞭 葛靛俊辑 甸绢啊绰 版快啊 乐阑 荐 乐澜. 酒付 喊 老 绝摆瘤父 捞固 凯妨乐促搁 摧绰霸 亮阑 淀.
															// 惑炔俊 蝶扼 促弗 悼累阑 窍档废 官操绢具 茄促.
															// 碍拳, 钦己, 檬岿篮 阿阿 唱坷绰 炼扒捞 促抚

	if (IsItemLevelupAllowed(NativeItemData)) { // 郴瘤绰 捞扒 扁夯.
		EnterItemLevelupModeClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else if (IsItemCompositionAllowed(NativeItemData))
	{
		if (NativeItemData.bIsLocked)
		{ // 钦己篮 夯 酒捞袍狼 绊蜡茄 沥眉己阑 苞皑洒 朝妨滚府绰 苞沥捞骨肺 泪陛捞 登菌阑 矫 荤傈俊 汞绰 扑诀捞 鞘夸窍促.
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
		//茄锅俊 割俺 凯扼绰 郴侩篮 扁裙俊 绝绢辑 茄锅俊 1俺究 凯档废 窍靛内爹 凳
		//肚茄 辑滚俊辑 努贰胶甫 0俊辑 矫累窍绰霸 酒囱 1肺 矫累秦辑 喊档肺 1阑 歹秦辑 焊郴具 茄促.
		data_trader::Retailer::GetInstance().RequestUnsealBox(NativeItemData.InstanceUID, PCClassToInt(SealBoxClass)+1, 1);
		SealBoxSelectCharClass<int32>::GetInstance().Signal(PCClassToInt(SealBoxClass));

		this->CloseMe();
	}
}

void UB2LobbyUI_ItemDetail::OnDelayShowChanceCallback()
{
	if (GetUnsealableItem())
	{
		//犬伏傍矫 扑诀 累诀且 巴
	}
}

void UB2LobbyUI_ItemDetail::OnDelaySealBoxClassCallback()
{
	if (GetUnsealableItem())
	{
		//努贰胶 荤捞努窍绰 何盒
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

		//付瘤阜俊 葛电 郴侩阑 瘤快绊 货肺 盲况持档废 茄促.
		AddSealBoxContents();
	}
}

void UB2LobbyUI_ItemDetail::OnDelayConvertCallback()
{
	//UI 龋免 何盒
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
		LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 档 诀单捞飘
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
	{ // 钦己篮 夯 酒捞袍狼 绊蜡茄 沥眉己阑 苞皑洒 朝妨滚府绰 苞沥捞骨肺 泪陛捞 登菌阑 矫 荤傈俊 汞绰 扑诀捞 鞘夸窍促.
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
		AskForUnlockItemPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_UnlockAskingItemAct_Sell"))); // 泪陛 钱瘤 汞绰 扑诀阑 剁款促.
	}
	else
	{
		if (bIsContainsPresetItem) // 葛滴 秦力
		{
			// 牢亥配府 钱牢 惑怕
			if (CachedInventory->IsInventoryOverLimit(NativeItemData.InventoryType))
			{
				NoticeErrorPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EquipErrorInvenFull")));
			}
			else if (bIsContainsOtherPreset)
			{ // 瘤陛 焊捞绰 芭 富绊 促弗 橇府悸俊辑档 厘馒登绢 乐栏骨肺 傈眉 秦力且波衬 拱绢夯促.
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

					this->CloseMe(); // 捞 芒档 摧绰促.
				})
				);
			}
			else
			{ // 捞 版快 角力肺绰 泅犁 橇府悸俊辑父 秦力窍搁 凳. 傈眉 秦力且波唱 拱绢焊瘤 富绊 弊成 官肺.
				TArray<int64> ReqIDs;
				ReqIDs.Add(NativeItemData.InstanceUID);
				LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Signal(-1, ReqIDs);

				this->CloseMe(); // 捞 芒档 摧绰促.
			}
		}
		else
		{
			// 官肺 魄概绰 酒聪绊 魄概咯何甫 汞绰 (漂荐) 扑诀. 粱 备蹿促府 规侥栏肺 せ
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
							this->CloseMe(); // 迫酒摹奎栏聪 捞 芒档 摧绰促.
						}),
							FSellItemPopupOnClick::CreateLambda([this]() {})
							);
					}
				}
			}
		}
	}
}