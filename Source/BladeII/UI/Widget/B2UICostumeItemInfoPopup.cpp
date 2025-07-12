// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UICostumeItemInfoPopup.h"

#include "B2UIManager.h"
#include "B2UICostumeItemSlot.h"
#include "B2Inventory.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2UIUtil.h"
#include "B2UICostumeItemOption.h"
#include "B2UICostumeDecompositionPopup.h"
#include "B2UICostumeEnhancePopup.h"
#include "Retailer.h"
#include "B2UICostumeItemDetailSetEffect.h"
#include "BladeIIGameImpl.h"
#include "B2UICostumeOpenBtn.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Slate/SceneViewport.h"
#include "BladeIIUtil.h"

void UB2UICostumeItemInfoPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvent();

	Super::DestroySelf(InUIManager);
}

void UB2UICostumeItemInfoPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_PopupMain);

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_EquipOrRelease);
	GET_SLOT(UB2Button, BTN_Enhance);
	GET_SLOT(UB2Button, BTN_Decomposition);
	GET_SLOT(UB2RichTextBlock, RTB_EquipOrReleaseBtn);

	GET_SLOT(UB2UICostumeItemSlot, UIP_CostumeItemSlot);
	if (UIP_CostumeItemSlot.IsValid())
	{
		UIP_CostumeItemSlot->Init();
	}

	GET_SLOT(UB2RichTextBlock, RTB_Combat);
	GET_SLOT(UB2RichTextBlock, RTB_CombatPower);
	GET_SLOT(UB2RichTextBlock, RTB_CombatPowerDelta);

	GET_SLOT(UB2RichTextBlock, RTB_PrimaryPointType);
	GET_SLOT(UB2RichTextBlock, RTB_PrimaryPoint);
	GET_SLOT(UB2RichTextBlock, RTB_PrimaryPointDelta);
	GET_SLOT(UB2RichTextBlock, RTB_ItemDescription);

	GET_SLOT(UVerticalBox, VB_OptionsDisplayPanel);

	GET_SLOT(UVerticalBox, VB_SetEffectList);

	GET_SLOT(UVerticalBox, VB_LeftButtonSet);

	GET_SLOT(UB2UICostumeOpenBtn, UIP_CostumeOpenLinkBtn);

	GET_SLOT(UPanelWidget, X_P_ItemLockButtonSet);
	GET_SLOT(UButton, BTN_ItemLock);
	GET_SLOT(UWidgetSwitcher, WS_Lock);

	GET_SLOT(UPanelWidget, X_P_ItemInfoDisplayPanel);
	GET_SLOT(UPanelWidget, X_P_ItemDescriptionPanel);

	GET_SLOT(UPanelWidget, P_CostumeLinkBtn);
	
	GET_SLOT(UTextBlock, STB_EnhanceBtn);
	if (STB_EnhanceBtn.IsValid())
		STB_EnhanceBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Enhance")));

	GET_SLOT(UTextBlock, STB_Decomposition);
	if (STB_Decomposition.IsValid())
		STB_Decomposition->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Decomposition")));
	
	GET_SLOT(UTextBlock, TB_ItemDesc);
	if (TB_ItemDesc.IsValid())
		TB_ItemDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemDesc")));

	SubscribeEvent();
}

void UB2UICostumeItemInfoPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UICostumeItemInfoPopup::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_EquipOrRelease, &UB2UICostumeItemInfoPopup::OnClickBTN_EquipOrRelease);
	BIND_CLICK_FUNC_TO_BTN(BTN_Enhance, &UB2UICostumeItemInfoPopup::OnClickBTN_Enhance);
	BIND_CLICK_FUNC_TO_BTN(BTN_Decomposition, &UB2UICostumeItemInfoPopup::OnClickBTN_Decomposition);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemLock, &UB2UICostumeItemInfoPopup::OnClickBTN_ItemLock);
}

void UB2UICostumeItemInfoPopup::SubscribeEvent()
{
	UnsubscribeEvent();

	//Issues.Add(DeliveryLockCostumesClass<FB2ResponseLockCostumesPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseLockCostumesPtr& msg)
	//{
	//	this->UpdateLockState(true, msg->locked_costumes);
	//}
	//));
	//Issues.Add(DeliveryUnlockCostumesClass<FB2ResponseUnlockCostumesPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseUnlockCostumesPtr& msg)
	//{
	//	this->UpdateLockState(false, msg->unlocked_costumes);
	//}
	//));
	//Issues.Add(DeliveryDismantleCostumesClass<FB2ResponseDismantleCostumesPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseDismantleCostumesPtr& msg)
	//{
	//	this->OnClickBTN_Close();
	//}
	//));
}

void UB2UICostumeItemInfoPopup::UnsubscribeEvent()
{
	Issues.Empty();
}

void UB2UICostumeItemInfoPopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UICostumeItemInfoPopup::OnClickBTN_Close()
{
	this->DestroySelf(UB2UIManager::GetInstance());
}

void UB2UICostumeItemInfoPopup::OnClickBTN_EquipOrRelease()
{
	if (CachedB2ItemInfo.bCurrentlyEquipped)
	{
		RequestUnEquipCostumeItem();
	}
	else
	{
		RequestEquipCostumeItem();
	}

	PlayEquipOrReleaseSound(!CachedB2ItemInfo.bCurrentlyEquipped);

	OnClickBTN_Close();
}

void UB2UICostumeItemInfoPopup::OnClickBTN_Enhance()
{
	/*float fPoupSizeX = 0.f;

	float fConvertScale = 1.f / UWidgetLayoutLibrary::GetViewportScale(this);

	FDisplayMetrics Metrics;
	FSlateApplication::Get().GetDisplayMetrics(Metrics);
	FVector2D vSafePaddingSize(Metrics.TitleSafePaddingSize);

	vSafePaddingSize.X *= fConvertScale;

	FVector2D vPopupPostion(vSafePaddingSize.X, 0.0f);

	if (UGameViewportClient* ViewportClient = this->GetWorld()->GetGameViewport())
	{
		if (FSceneViewport* SceneViewport = ViewportClient->GetGameViewport())
		{
			fPoupSizeX = SceneViewport->GetCachedGeometry().GetLocalSize().X * fConvertScale * 0.5;
			fPoupSizeX -= vSafePaddingSize.X;
		}

	}

	auto* UI = UB2UIManager::GetInstance()->OpenUI<UB2UICostumeEnhancePopup>(UIFName::CostumeEnhancePopup);
	if (UI)
	{
		UI->InitCostumeItemInfoPopup(vPopupPostion, fPoupSizeX);
		UI->UpdatePopupInfo(CachedB2ItemInfo);
	}
	
	ForcedClickCostumeMaterialClass<bool>::GetInstance().Signal(true);

	OnClickBTN_Close();*/
}

void UB2UICostumeItemInfoPopup::OnClickBTN_Decomposition()
{	if (CachedB2ItemInfo.bIsLocked)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_AskForUnlockForSomeItemAct")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_UnlockAskingItemAct_Decompose"))),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([this]() {
			this->OnClickBTN_ItemLock();
		})
		);
		return;
	}

	auto* UI = UB2UIManager::GetInstance()->OpenUI<UB2UICostumeDecompositionPopup>(UIFName::CostumeDecompositonPopup);
	if (UI)
	{
		UI->UpdatePopupInfo(CachedB2ItemInfo);
	}
}

void UB2UICostumeItemInfoPopup::OnClickBTN_ItemLock()
{
	TArray<int64> ReqIDs;
	ReqIDs.Add(CachedB2ItemInfo.InstanceUID);

	if(CachedB2ItemInfo.bIsLocked)
		data_trader::Retailer::GetInstance().RequestUnlockCostumes(ReqIDs);
	else
		data_trader::Retailer::GetInstance().RequestLockCostumes(ReqIDs);
}

void UB2UICostumeItemInfoPopup::InitCostumeItemInfoPopup(const FVector2D& v2PopupPosition, const float& fPopupSizeX, const FB2Item& InItem)
{
	CachedB2ItemInfo = InItem;

	if (CP_PopupMain.IsValid())
	{
		CP_PopupMain->SetRenderTranslation(v2PopupPosition);

		if (UCanvasPanelSlot* pCanvasPanelSlot = Cast<UCanvasPanelSlot>(CP_PopupMain->Slot))
		{
			pCanvasPanelSlot->SetOffsets(FMargin(0, fTopBezel_BP, fPopupSizeX, 0));
		}
	}

	UpdateCostumeItemInfoPopup(InItem);
	UpdateButtonsVisibleState();

	if (WS_Lock.IsValid())
		WS_Lock->SetActiveWidgetIndex(static_cast<int32>(CachedB2ItemInfo.bIsLocked));
}

void UB2UICostumeItemInfoPopup::UpdateCostumeItemInfoPopup(const FB2Item& InItem)
{
	bool bIsConsumable = InItem.InventoryType == EItemInvenType::EIIVT_Consumables;
	FB2Item* pEquippedItemAtSamePlace = nullptr;

	FB2Item EquippedItemAtPlace;
	if (GetEquippedItemAtPlace(EquippedItemAtPlace, InItem.EquipPlace))
	{
		pEquippedItemAtSamePlace = &EquippedItemAtPlace;
	}

	UpdateCostumeItemSlot(InItem);

	UpdateCombatPowerText(InItem, pEquippedItemAtSamePlace);

	UpdatePrimaryPointText(InItem, pEquippedItemAtSamePlace);

	UpdateCostumeItemSetEffect(InItem);

	UpdateEquipOrReleaseBtnText(InItem);

	if (bIsConsumable)
		UpdateCostumeConsumableItemOptions(InItem);
	else
		UpdateCostumeItemOptions(InItem, pEquippedItemAtSamePlace);

	if (X_P_ItemInfoDisplayPanel.IsValid())
		X_P_ItemInfoDisplayPanel->SetVisibility(bIsConsumable ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

	if (X_P_ItemDescriptionPanel.IsValid())
		X_P_ItemDescriptionPanel->SetVisibility(bIsConsumable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

}

void UB2UICostumeItemInfoPopup::UpdateCostumeItemSlot(const FB2Item& InItem)
{
	if (UIP_CostumeItemSlot.IsValid())
	{
		UIP_CostumeItemSlot->UpdateCostumeItemSlot_FromFB2Item(InItem);
	}
}

void UB2UICostumeItemInfoPopup::UpdateCostumeItemOptions(const FB2Item& InItem, const FB2Item* pEquippedItemAtSamePlace)
{
	if (VB_OptionsDisplayPanel.IsValid() == false)
	{
		return;
	}

	VB_OptionsDisplayPanel->ClearChildren();

	const EPCClass ItemAllowedPCClass = ItemAllowedPCClassEnum(InItem.AllowedPCClass);

	const TArray<FItemOption>* pIntrinsicOptionsAtSamePlace = pEquippedItemAtSamePlace ? &pEquippedItemAtSamePlace->IntrinsicOptions : nullptr;
	UpdateOptionList(ItemAllowedPCClass, InItem.IntrinsicOptions, pIntrinsicOptionsAtSamePlace);

	const TArray<FItemOption>* pRandomOptionsAtSamePlace = pEquippedItemAtSamePlace ? &pEquippedItemAtSamePlace->RandomOptions : nullptr;
	UpdateOptionList(ItemAllowedPCClass, InItem.RandomOptions, pRandomOptionsAtSamePlace);
}

void UB2UICostumeItemInfoPopup::UpdateCostumeConsumableItemOptions(const FB2Item& InItem)
{
	if (RTB_ItemDescription.IsValid())
		RTB_ItemDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMNAMESUB, FString::FromInt(InItem.ItemRefID)));
}

void UB2UICostumeItemInfoPopup::UpdateCostumeItemSetEffect(const FB2Item& InItem)
{
	if (VB_SetEffectList.IsValid() == false)
	{
		return;
	}

	VB_SetEffectList->ClearChildren();

	if (IsItemSetEffectAvailable(InItem))
	{
		UB2UICostumeItemDetailSetEffect* pSetItemInfo = Cast<UB2UICostumeItemDetailSetEffect>(DynCreateInVerticalBox(UICostumeItemDetailSetEffectClass, this, VB_SetEffectList.Get(), FMargin()));
		if (pSetItemInfo)
		{
			pSetItemInfo->InitCostumeItemDetailSetEffect(InItem);
		}
	}
}

void UB2UICostumeItemInfoPopup::UpdateEquipOrReleaseBtnText(const FB2Item& InItem)
{
	if (RTB_EquipOrReleaseBtn.IsValid() == false)
	{
		return;
	}

	if (CachedB2ItemInfo.bCurrentlyEquipped)
	{
		RTB_EquipOrReleaseBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_UnEquip")));
	}
	else
	{
		RTB_EquipOrReleaseBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Equiped_Preset")));
	}
}

void UB2UICostumeItemInfoPopup::UpdateButtonsVisibleState()
{
	const EUIScene eCurrentUIScene = UB2UIManager::GetInstance()->GetCurrUIScene();
	bool bIsEquipOrReleaseBtnVisible = false;
	if (eCurrentUIScene == EUIScene::Costume || eCurrentUIScene == EUIScene::Inventory)
	{
		const ECostumePage eCurrentCostumePage = BladeIIGameImpl::GetCostumeDataStore().GetCachedCostumePageType();
		if (eCurrentCostumePage != ECostumePage::Store)
		{
			bIsEquipOrReleaseBtnVisible = true;
		}
	}

	if (BTN_EquipOrRelease.IsValid())
	{
		bool bVisible = bIsEquipOrReleaseBtnVisible && (CachedB2ItemInfo.ItemRefID > 0 ? CachedB2ItemInfo.InventoryType != EItemInvenType::EIIVT_Consumables : false);
		BTN_EquipOrRelease->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	bool bIsCostumeOpenLinkBtnVisible = true;
	if (eCurrentUIScene == EUIScene::Costume)
	{
		bIsCostumeOpenLinkBtnVisible = false;
	}

	if (P_CostumeLinkBtn.IsValid())
	{
		P_CostumeLinkBtn->SetVisibility(bIsCostumeOpenLinkBtnVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	// 분해 강화 잠금 버튼은 코스튬 인벤토리일때만..
	if (BTN_Enhance.IsValid())
	{
		bool bVisible = bIsEquipOrReleaseBtnVisible && (CachedB2ItemInfo.ItemRefID > 0 ? MAX_ENHANCE_COSTUME_LEVEL > CachedB2ItemInfo.EnhanceLevel : false);
		BTN_Enhance->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	
	if (BTN_Decomposition.IsValid())
	{
		bool bVisible = bIsEquipOrReleaseBtnVisible && (CachedB2ItemInfo.ItemRefID > 0 ? !CachedB2ItemInfo.bCurrentlyEquipped : false) && (CachedB2ItemInfo.ItemRefID > 0 ? CachedB2ItemInfo.InventoryType != EItemInvenType::EIIVT_Consumables : false);
		BTN_Decomposition->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (X_P_ItemLockButtonSet.IsValid())
	{
		bool bVisible = bIsEquipOrReleaseBtnVisible && (CachedB2ItemInfo.ItemRefID > 0 ? CachedB2ItemInfo.InventoryType != EItemInvenType::EIIVT_Consumables : false);
		X_P_ItemLockButtonSet->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (X_P_ItemLockButtonSet.IsValid())
	{
		bool bVisible = bIsEquipOrReleaseBtnVisible && (CachedB2ItemInfo.ItemRefID > 0 ? CachedB2ItemInfo.InventoryType != EItemInvenType::EIIVT_Consumables : false);
		X_P_ItemLockButtonSet->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (VB_LeftButtonSet.IsValid())
	{
		bool bVisible = bIsEquipOrReleaseBtnVisible && (CachedB2ItemInfo.ItemRefID > 0 ? CachedB2ItemInfo.InventoryType != EItemInvenType::EIIVT_Consumables : false);
		VB_LeftButtonSet->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2UICostumeItemInfoPopup::UpdateCombatPowerText(const FB2Item& InItem, const FB2Item* pEquippedItemAtSamePlace)
{
	if (RTB_Combat.IsValid())
	{
		RTB_Combat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
	}
	if (RTB_CombatPower.IsValid())
	{
		const int32 iCombatPower = static_cast<int32>(InItem.Power);
		RTB_CombatPower->SetText(FText::AsNumber(iCombatPower));
	}

	if (RTB_CombatPowerDelta.IsValid())
	{
		int32 iDeltaValue = 0;
		bool IsVisible = false;
		if (pEquippedItemAtSamePlace)
		{
			iDeltaValue = static_cast<int32>(InItem.Power - pEquippedItemAtSamePlace->Power);
			IsVisible = true;
		}

		SetDeltaText(RTB_CombatPowerDelta, iDeltaValue, IsVisible);
	}
}

void UB2UICostumeItemInfoPopup::UpdatePrimaryPointText(const FB2Item& InItem, const FB2Item* pEquippedItemAtSamePlace)
{
	if (RTB_PrimaryPointType.IsValid())
	{
		RTB_PrimaryPointType->SetText(GetLOCTextOfPrimPointType(InItem.PrimaryPointType));
	}
	if (RTB_PrimaryPoint.IsValid())
	{
		const int32 iPrimaryPoint = static_cast<int32>(InItem.GetPrimaryPoint());
		RTB_PrimaryPoint->SetText(FText::AsNumber(iPrimaryPoint));
	}

	if (RTB_PrimaryPointDelta.IsValid())
	{
		int32 iDeltaValue = 0;
		bool IsVisible = false;
		if (pEquippedItemAtSamePlace)
		{
			iDeltaValue = B2UIUtil::DifferenceItemPrimaryPoint(InItem, *pEquippedItemAtSamePlace);
			IsVisible = true;
		}
		SetDeltaText(RTB_PrimaryPointDelta, iDeltaValue, IsVisible);
	}
}

void UB2UICostumeItemInfoPopup::UpdateOptionList(const EPCClass& ItemAllowedPCClass, const TArray<FItemOption>& ArrayItemOption, const TArray<FItemOption>* ArrayItemOptionAtSamePlace)
{
	if (VB_OptionsDisplayPanel.IsValid() == false)
	{
		return;
	}

	if (ArrayItemOption.Num() <= 0)
	{
		return;
	}

	UB2UICostumeItemOption* pCostumeItemOption = Cast<UB2UICostumeItemOption>(DynCreateInVerticalBox(UICostumeItemOptionClass, this, VB_OptionsDisplayPanel.Get(), FMargin()));
	if (pCostumeItemOption)
	{
		pCostumeItemOption->InitCostumeItemOption(ItemAllowedPCClass, ArrayItemOption, ArrayItemOptionAtSamePlace);
	}
}

void UB2UICostumeItemInfoPopup::RequestEquipCostumeItem()
{
	TArray<int64> ReqIDs;
	ReqIDs.Add(CachedB2ItemInfo.InstanceUID);
	data_trader::Retailer::GetInstance().RequestEquipCostume(ReqIDs);
}

void UB2UICostumeItemInfoPopup::RequestUnEquipCostumeItem()
{
	TArray<int64> ReqIDs;
	ReqIDs.Add(CachedB2ItemInfo.InstanceUID);
	data_trader::Retailer::GetInstance().RequestUnequipCostume(ReqIDs);
}

bool UB2UICostumeItemInfoPopup::GetEquippedItemAtPlace(FB2Item& OutFoundItem, const EItemEquipPlace& InEquipPlace)
{
	bool IsSamePlace = false;

	AB2LobbyGameMode* pLobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* pLobbyInven = pLobbyGameMode ? pLobbyGameMode->GetLobbyInventory() : NULL;
	if (pLobbyInven)
	{
		IsSamePlace = pLobbyInven->GetEquippedItemAtPlace(OutFoundItem, InEquipPlace);
	}

	return IsSamePlace;
}

void UB2UICostumeItemInfoPopup::SetDeltaText(TWeakObjectPtr<UB2RichTextBlock> pRTB, const int32& iDeltaValue, const bool& IsVisible)
{
	if (pRTB.IsValid() == false)
	{
		return;
	}

	FString		 DeltaString;
	FLinearColor DeltaColor;

	if (iDeltaValue == 0)
	{
		DeltaString = TEXT("( - )");
		DeltaColor = ItemStatSameColor;
	}
	else if (iDeltaValue > 0)
	{
		DeltaString = FString::Printf(TEXT("( %s%d )"), TEXT("+"), FMath::Abs(iDeltaValue));
		DeltaColor = ItemStatIncColor;
	}
	else
	{
		DeltaString = FString::Printf(TEXT("( %s%d )"), TEXT("-"), FMath::Abs(iDeltaValue));
		DeltaColor = ItemStatDecColor;
	}

	pRTB->SetText(FText::FromString(DeltaString));
	pRTB->SetColorAndOpacity(DeltaColor);
	pRTB->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UB2UICostumeItemInfoPopup::UpdateLockState(bool bIsLock, TArray<b2network::B2CostumeServerInfoPtr> ChangeItem)
{
	if (ChangeItem.IsValidIndex(0))
	{
		if (ChangeItem[0]->id == CachedB2ItemInfo.InstanceUID)
		{
			CachedB2ItemInfo.bIsLocked = bIsLock;

			BladeIIGameImpl::GetClientDataStore().ChangeCostumeItem(ChangeItem[0]);

			if (WS_Lock.IsValid())
				WS_Lock->SetActiveWidgetIndex(static_cast<int32>(CachedB2ItemInfo.bIsLocked));

			UpdateCostumeItemSlot(CachedB2ItemInfo);
		}
	}
}