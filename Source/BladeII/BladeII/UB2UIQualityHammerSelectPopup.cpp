// Fill out your copyright notice in the Description page of Project Settings.
#include "UB2UIQualityHammerSelectPopup.h"

#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"

#include "BladeIIGameImpl.h"

AB2LobbyGameMode* UB2UIQualityHammerSelectPopup::HammerCachedLobbyGM = NULL;

UB2UIQualityHammerSelectPopup::UB2UIQualityHammerSelectPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedDataStore = &BladeIIGameImpl::GetClientDataStore();
	ItemIconWidgetClass = NULL;
	CreatedItemIcon = NULL;
}

void UB2UIQualityHammerSelectPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_List)
	GET_SLOT(UB2ScrollBox, SB_HammerIcons)

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_ItemDesc);
	GET_SLOT(UTextBlock, TB_QualityRange);
	GET_SLOT(UTextBlock, TB_ExpectedQuality);

	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UTextBlock, TB_Select);

	GET_SLOT(UB2Button, BTN_Close);

	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIQualityHammerSelectPopup::OnClickBTN_Select);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIQualityHammerSelectPopup::OnClickBTN_Close);
}

void UB2UIQualityHammerSelectPopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UIQualityHammerSelectPopup::UpdateStaticWidgets()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChangeHammerPopupText_Title")));
	}
	if (TB_ItemDesc.IsValid())
	{
		TB_ItemDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChangeHammerPopupText_Description")));
	}
	if (TB_QualityRange.IsValid())
	{
		TB_QualityRange->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChangeHammerPopupText_QualityRange")));
	}
	if (TB_Select.IsValid())
	{
		TB_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChangeHammerPopupText_Select")));
	}
}

void UB2UIQualityHammerSelectPopup::UpdateDynamicWidgets()
{
	int32 justQuality;
	if (lowQuality == highQuality)
	{
		justQuality = highQuality;
	}

	if (TB_ExpectedQuality.IsValid())
	{
		TB_ExpectedQuality->SetText(GetLOCTextOfItemQuality(lowQuality));
	}
}

void UB2UIQualityHammerSelectPopup::OnClickBTN_Select()
{

}
void UB2UIQualityHammerSelectPopup::OnClickBTN_Close()
{
	//UB2UIManager::GetInstance()->CloseUI(UIFName::HammerChangePopup);
	ClickCloseDelegate.ExecuteIfBound();
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_QualityChangeHammerSelect);
}

void UB2UIQualityHammerSelectPopup::UpdateItemInfo(FB2Item& TargetItem)
{
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	UB2UIManager_Lobby* DJLegacyUIMgr = LobbyGM ? LobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;
	ItemIconWidgetClass = DJLegacyUIMgr ? DJLegacyUIMgr->GetInventoryItemIconClass(false) : nullptr;
	//if (ItemIconWidgetClass == NULL || !CachedInventory) {
		//return;
	//}

	CreatedItemIcons.Empty();

	TArray<int64> AllIngredIDs;
	CachedInventory->GetAvailableHammers(TargetItem, AllIngredIDs);

	//if (TB_SelectEnhanceItemText.IsValid())
	//	TB_SelectEnhanceItemText->SetVisibility(AllIngredIDs.Num() > 0 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	for (int32 II = 0; II < AllIngredIDs.Num(); ++II)
	{
		FB2Item ThisIngred;
		/*
		if (!UB2LobbyInventory::FindFromAll(ThisIngred, AllIngredIDs[II]))
			continue;
			*/

		UB2DynItemIcon_Hammer* NewItemIcon = CreateItemIcon();
		if (NewItemIcon)
		{
			NewItemIcon->UpdateItemData(ThisIngred); // 俺喊 酒捞袍 沥焊甫 逞败霖促.
			NewItemIcon->SetupManualScrollBoxSender_byReceiver(this, SB_HammerIcons.Get());
			CreatedItemIcons.Add(NewItemIcon);
		}
	}
}

UB2DynItemIcon_Hammer* UB2UIQualityHammerSelectPopup::CreateItemIcon()
{
 	if (ItemIconWidgetClass == nullptr) {
 		return nullptr;
 	}
 
 	UScaleBox* OuterSB = NewObject<UScaleBox>(this, NAME_None, RF_Transient);
 	if (!OuterSB) {
 		return nullptr;
 	}
 	UB2UnitedWidgetBase* CreatedWidget = CreateWidget<UB2UnitedWidgetBase>(GetOwningPlayer(), ItemIconWidgetClass);
 	if (CreatedWidget == NULL) {
 		return nullptr;
 	}

	const float DefaultSacle = 1.0f;
	OuterSB->SetStretch(EStretch::UserSpecified);
	OuterSB->SetUserSpecifiedScale(DefaultSacle);

	UScaleBoxSlot* AddedSBSlot = Cast<UScaleBoxSlot>(OuterSB->AddChild(CreatedWidget));
	if (AddedSBSlot)
	{
		CreatedWidget->SetDynCreatedDummyParent(OuterSB); // CreatedWidget 阑 力芭且 锭 OuterSB 档 鞍捞 力芭登档废 殿废.
	}

	UScrollBoxSlot* AddedCPSlot = Cast<UScrollBoxSlot>(SB_HammerIcons->AddChild(OuterSB));
	if (AddedCPSlot)
	{
		FMargin Margin = FMargin(5.0f, 0.0f, 0.0f, 0.0f);
		AddedCPSlot->SetPadding(Margin);
	}

	return Cast<UB2DynItemIcon_Hammer>(CreatedWidget);
}