// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIMsgPopupInventoryFull.h"
#include "B2UIManager.h"
#include "B2PCClassInfo.h"
#include "B2PCClassInfoBox.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyInventory.h"

UB2UIMsgPopupInventoryFull::UB2UIMsgPopupInventoryFull(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentPCClass = EPCClass::EPC_End;
}

void UB2UIMsgPopupInventoryFull::SetInventoryItemCount(int32 CurrentItemCount, int32 MaxItemCount)
{
	if (TB_StoredItemCount.IsValid())
		TB_StoredItemCount->SetText(FText::AsNumber(CurrentItemCount));

	if (TB_ContainerItemCount.IsValid())
		TB_ContainerItemCount->SetText(FText::AsNumber(MaxItemCount));
}

void UB2UIMsgPopupInventoryFull::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UButton, BTN_ExtensionInven)
	GET_SLOT(UButton, BTN_MoveInven)
	GET_SLOT(UButton, BTN_Close)
	GET_SLOT(UTextBlock, TB_ExtensionInven)
	GET_SLOT(UTextBlock, TB_MoveInven)

	GET_SLOT(UTextBlock, TB_StoredItemCount)
	GET_SLOT(UTextBlock, TB_ContainerItemCount)
}

void UB2UIMsgPopupInventoryFull::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	
	if (OverrideClickMoveInven.IsBound())
		OverrideClickMoveInven.Unbind();
}

void UB2UIMsgPopupInventoryFull::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_ExtensionInven.IsValid())
		TB_ExtensionInven->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenExtend"))));
	if (TB_MoveInven.IsValid())
		TB_MoveInven->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenMove"))));
}

void UB2UIMsgPopupInventoryFull::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_ExtensionInven, &UB2UIMsgPopupInventoryFull::OnClickBtnExtensionInven)
	BIND_CLICK_FUNC_TO_BTN(BTN_MoveInven, &UB2UIMsgPopupInventoryFull::OnClickBtnMoveInven)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupInventoryFull::OnClickBtnClose)
}

void UB2UIMsgPopupInventoryFull::OnClickBtnExtensionInven()
{
	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(CurrentPCClass);
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ExtendInventory, FDJLegacySubpopupAddPanelInfo());

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupInventoryFull::OnClickBtnMoveInven()
{
	if (OverrideClickMoveInven.IsBound())
	{
		OverrideClickMoveInven.Execute();
		DestroySelf(UB2UIManager::GetInstance());
		return;
	}

	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(CurrentPCClass);
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
	LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupInventoryFull::OnClickBtnClose()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupInventoryFull::SetPCClass(const EPCClass PCClass)
{
	if (CurrentPCClass != PCClass && PCClass != EPCClass::EPC_End)
	{
		CurrentPCClass = PCClass;
		UpdateWidget();
	}
}

void UB2UIMsgPopupInventoryFull::UpdateWidget()
{
	UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
	UB2PCClassInfo* PCClassInfo = PCClassInfoBox ? PCClassInfoBox->GetSingleClassInfo(CurrentPCClass) : NULL;
	const FText PCClassText = PCClassInfo ? PCClassInfo->GetLocalizedName() : FText::FromString(TEXT("Unknown Class"));
	const FText InventoryFullText = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("StageStartErrorInvenFull"))), PCClassText);
	
	SetContent(InventoryFullText);
	
	int32 StoredItemCount = UB2LobbyInventory::GetStoredItemNumInUnitedSlot(EItemInvenType::EIIVT_United, CurrentPCClass);
	int32 ContainerItemCount = UB2LobbyInventory::GetItemContainerSize(EItemInvenType::EIIVT_United, CurrentPCClass); 
	
	if (TB_StoredItemCount.IsValid())
		TB_StoredItemCount->SetText(FText::AsNumber(StoredItemCount));

	if (TB_ContainerItemCount.IsValid())
		TB_ContainerItemCount->SetText(FText::AsNumber(ContainerItemCount));

	int32 Cache, ExtendSlotCount, MaxSlotCount, RetSlotCount;
	
	RetSlotCount = BladeIIGameImpl::GetClientDataStore().GetCharacterInvenSlotCount(CurrentPCClass, EItemInvenType::EIIVT_United);
	BladeIIGameImpl::GetClientDataStore().GetInventoryExtendReferenceData(Cache, ExtendSlotCount, MaxSlotCount);

	if (BTN_ExtensionInven.IsValid())
		BTN_ExtensionInven->SetIsEnabled(RetSlotCount < MaxSlotCount);

}

void UB2UIMsgPopupInventoryFull::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIMsgPopupInventoryFull::OverrideMoveInventoryLogic(const FMsgPopupOnClick & OnBtnPosClick)
{
	OverrideClickMoveInven = OnBtnPosClick;
}

void UB2UIMsgPopupInventoryFull::ShowExtensionBtn(bool Show)
{
	if (BTN_ExtensionInven.IsValid())
		BTN_ExtensionInven->SetVisibility(Show ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

