
// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UICostumePageInven.h"

#include "BladeIIUtil.h"
#include "B2UICostumeItemSlot.h"
#include "B2LobbyGameMode.h"
#include "BladeIIGameImpl.h"
#include "B2RedDotManager.h"
#include "B2ClientDataStore.h"
#include "B2LobbyInventory.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "B2ScrollBoxWithGridPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Slate/SceneViewport.h"
#include "B2UICostumeEnhancePopup.h"

const int32 iCOSTUME_TAB_WEAPON = 0;
const int32 iCOSTUME_TAB_ARMORS = 1;
const int32 iCOSTUME_TAB_MATERIAL = 2;

void UB2UICostumePageInven::Init()
{
	Super::Init();

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	CachedInventory = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;

	LastSelectedPCClass = EPCClass::EPC_End;

	InitCostumePageInven();
}

void UB2UICostumePageInven::OnOpen(bool RightNow)
{
	SubscribeEvents();

	BladeIIGameImpl::GetClientDataStore().RequestGetAccountCostumeAll();

	Super::OnOpen(RightNow);
}

void UB2UICostumePageInven::OnClose(bool RightNow)
{
	UnsubscribeEvents();

	RequestModifyCostumeNewFlag();

	Super::OnClose(RightNow);
}

void UB2UICostumePageInven::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(CP_TabWeapon.Get()), &UB2UICostumePageInven::HasRedDot_TabWeapon);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(CP_TabArmors.Get()), &UB2UICostumePageInven::HasRedDot_TabArmors);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(CP_TabMaterial.Get()), &UB2UICostumePageInven::HasRedDot_TabMaterial);
}

void UB2UICostumePageInven::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	RequestModifyCostumeNewFlag();

	Super::DestroySelf(InUIManager);
}

void UB2UICostumePageInven::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_TabWeapon);
	GET_SLOT(UB2Button, BTN_TabWeapon);
	GET_SLOT(UWidgetSwitcher, WS_TabWeapon);
	GET_SLOT(UTextBlock, TB_UnSelectedTabWeapon);
	GET_SLOT(UTextBlock, TB_SelectedTabWeapon);

	GET_SLOT(UCanvasPanel, CP_TabArmors);
	GET_SLOT(UB2Button, BTN_TabArmors);
	GET_SLOT(UWidgetSwitcher, WS_TabArmors);
	GET_SLOT(UTextBlock, TB_UnSelectedTabArmors);
	GET_SLOT(UTextBlock, TB_SelectedTabArmors);

	GET_SLOT(UCanvasPanel, CP_TabMaterial);
	GET_SLOT(UB2Button, BTN_TabMaterial);
	GET_SLOT(UWidgetSwitcher, WS_TabMaterial);
	GET_SLOT(UTextBlock, TB_UnSelectedTabMaterial);
	GET_SLOT(UTextBlock, TB_SelectedTabMaterial);

	GET_SLOT(UTextBlock, StoredItemNumText);
	GET_SLOT(UTextBlock, ContainerSizeText);
	GET_SLOT(UTextBlock, SlashText);
	
	GET_SLOT(UB2ScrollBoxWithGridPanel, UIP_ScrollBoxWithGridPanelBP);
	if (UIP_ScrollBoxWithGridPanelBP.IsValid())
	{
		UIP_ScrollBoxWithGridPanelBP->Init();
	}
}

void UB2UICostumePageInven::UpdateStaticText()
{
	if (TB_UnSelectedTabWeapon.IsValid())
	{
		TB_UnSelectedTabWeapon->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Weapon")));
	}
	if (TB_SelectedTabWeapon.IsValid())
	{
		TB_SelectedTabWeapon->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Weapon")));
	}

	if (TB_UnSelectedTabArmors.IsValid())
	{
		TB_UnSelectedTabArmors->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Protection")));
	}
	if (TB_SelectedTabArmors.IsValid())
	{
		TB_SelectedTabArmors->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Protection")));
	}

	if (TB_UnSelectedTabMaterial.IsValid())
	{
		TB_UnSelectedTabMaterial->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Tab_Material")));
	}
	if (TB_SelectedTabMaterial.IsValid())
	{
		TB_SelectedTabMaterial->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Tab_Material")));
	}

}

void UB2UICostumePageInven::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_TabWeapon, &UB2UICostumePageInven::OnClickBTN_TabWeapon);

	BIND_CLICK_FUNC_TO_BTN(BTN_TabArmors, &UB2UICostumePageInven::OnClickBTN_TabArmors);

	BIND_CLICK_FUNC_TO_BTN(BTN_TabMaterial, &UB2UICostumePageInven::OnClickBTN_TabMaterial);
}

void UB2UICostumePageInven::OnClickBTN_TabWeapon()
{
	if(CurSelectedItemInvenTab != EItemInvenType::EIIVT_Consumables)
		RequestModifyCostumeNewFlag(LastSelectedPCClass, CurSelectedItemInvenTab);

	UpdateTabBtnState(iCOSTUME_TAB_WEAPON);

	UpdateCostumeInvenFromCurrentPCClass();
}

void UB2UICostumePageInven::OnClickBTN_TabArmors()
{
	if (CurSelectedItemInvenTab != EItemInvenType::EIIVT_Consumables)
		RequestModifyCostumeNewFlag(LastSelectedPCClass, CurSelectedItemInvenTab);

	UpdateTabBtnState(iCOSTUME_TAB_ARMORS);

	UpdateCostumeInvenFromCurrentPCClass();
}

void UB2UICostumePageInven::OnClickBTN_TabMaterial()
{
	//RequestModifyCostumeNewFlag(LastSelectedPCClass, CurSelectedItemInvenTab);

	UpdateTabBtnState(iCOSTUME_TAB_MATERIAL);

	UpdateCostumeInvenFromCurrentPCClass();
}

bool UB2UICostumePageInven::HasRedDot_TabWeapon()
{
	if (AB2LobbyGameMode* pLobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer())))
	{
		const EPCClass CurrentPCClass = pLobbyGameMode->GetHeroMgmtModePCClass();
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_CostumeInventory(CurrentPCClass, EItemInvenType::EIIVT_CostumeWeapon);
	}
	return false;
}

bool UB2UICostumePageInven::HasRedDot_TabArmors()
{
	if (AB2LobbyGameMode* pLobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer())))
	{
		const EPCClass CurrentPCClass = pLobbyGameMode->GetHeroMgmtModePCClass();
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_CostumeInventory(CurrentPCClass, EItemInvenType::EIIVT_CostumeArmor);
	}
	return false;
}

bool UB2UICostumePageInven::HasRedDot_TabMaterial()
{
	return false;
}

void UB2UICostumePageInven::DeliveryModifyCostumeNewFlag()
{
	UB2LobbyInventory::SetChangeNewItemTofalseTap(BeforeCurSelectedPCClass, BeforeCurSelectedItemInvenTab);

	UpdateCostumeInvenFromCurrentPCClass();

	RedDotCondition_TapState();
	//hero mgmt
}

void UB2UICostumePageInven::SubscribeEvents()
{
	UnsubscribeEvents();

	CAPTURE_UOBJECT(UB2UICostumePageInven);

	Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
		[Capture](EPCClass SelectionPCClass)
	{
		if (Capture.IsValid())
		{
			Capture->RequestModifyCostumeNewFlag(Capture->LastSelectedPCClass, Capture->CurSelectedItemInvenTab);
			Capture->UpdateCostumeInvenFromSelectionPCClass(SelectionPCClass);
			Capture->CloseAllStandaloneUIFromUIManager();
		}
	}
	));

	Issues.Add(UpdateLobbyInventoryControlClass<>::GetInstance().Subscribe2(
	[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->UpdateCostumeInvenFromCurrentPCClass();
		}
	}
	));

	Issues.Add(SuccesGetAccountCostumeAllClass<>::GetInstance().Subscribe2(
		[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->UpdateCostumeInvenFromCurrentPCClass();
		}
	}
	));

	Issues.Add(DeliveryModifyCostumeNewFlagClass<>::GetInstance().Subscribe2(
		[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->DeliveryModifyCostumeNewFlag();
		}
	}
	));

	Issues.Add(ForcedClickCostumeMaterialClass<bool>::GetInstance().Subscribe2(
		[Capture](bool bFreezeTab)
	{
		if (Capture.IsValid())
		{
			Capture->ForcedTabClickMaterial(bFreezeTab);
		}
	}
	));
	Issues.Add(OnCallBackEnhanceCostumeInvenPageClass<const FB2Item&>::GetInstance().Subscribe2(
		[Capture](const FB2Item& InItem)
	{
		if (Capture.IsValid())
		{
			Capture->OnCallBackEnhance(InItem);
		}
	}
	));
}

void UB2UICostumePageInven::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UICostumePageInven::InitCostumePageInven()
{
	UpdateTabBtnState(iCOSTUME_TAB_WEAPON);

	InitCostumeItemList();
}

void UB2UICostumePageInven::InitCostumeItemList()
{
	if (UIP_ScrollBoxWithGridPanelBP.IsValid() == false)
	{
		return;
	}

	UIP_ScrollBoxWithGridPanelBP->SetUniformGridPanelClearChildren();
}

void UB2UICostumePageInven::InitScrollOffset()
{
	if (UIP_ScrollBoxWithGridPanelBP.IsValid())
	{
		const float fInitScrollOffset = 0.f;
		UIP_ScrollBoxWithGridPanelBP->SetB2ScrollOffset(fInitScrollOffset);
	}
}

void UB2UICostumePageInven::UpdateTabBtnState(const int32& iTabIndex)
{
	if (WS_TabWeapon.IsValid() == false || WS_TabArmors.IsValid() == false)
	{
		return;
	}

	int32 iIsSelected = static_cast<int32>(false);

	WS_TabWeapon->SetActiveWidgetIndex(iIsSelected);
	WS_TabArmors->SetActiveWidgetIndex(iIsSelected);
	WS_TabMaterial->SetActiveWidgetIndex(iIsSelected);

	iIsSelected = static_cast<int32>(true);

	switch (iTabIndex)
	{
	case iCOSTUME_TAB_WEAPON:
	{
		WS_TabWeapon->SetActiveWidgetIndex(iIsSelected);
		CurSelectedItemInvenTab = EItemInvenType::EIIVT_CostumeWeapon;
	}break;
	case iCOSTUME_TAB_ARMORS:
	{
		WS_TabArmors->SetActiveWidgetIndex(iIsSelected);
		CurSelectedItemInvenTab = EItemInvenType::EIIVT_CostumeArmor;
	}break;
	case iCOSTUME_TAB_MATERIAL:
	{
		WS_TabMaterial->SetActiveWidgetIndex(iIsSelected);
		CurSelectedItemInvenTab = EItemInvenType::EIIVT_Consumables;
	}break;
	}
}

void UB2UICostumePageInven::UpdateCostumeInvenFromCurrentPCClass()
{
	if (AB2LobbyGameMode* pLobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer())))
	{
		const EPCClass CurrentPCClass = pLobbyGameMode->GetHeroMgmtModePCClass();

		if (CurSelectedItemInvenTab == EItemInvenType::EIIVT_Consumables)
			UpdateCostumeInvenFromMaterials();
		else
			UpdateCostumeInvenFromSelectionPCClass(CurrentPCClass);
	}
}

void UB2UICostumePageInven::UpdateCostumeInvenFromSelectionPCClass(EPCClass SelectionPCClass)
{
	LastSelectedPCClass = SelectionPCClass;

	if (CurSelectedItemInvenTab == EItemInvenType::EIIVT_Consumables)
	{
		return;
	}

	if (UIP_ScrollBoxWithGridPanelBP.IsValid() == false)
	{
		return;
	}

	InitScrollOffset();

	TArray<FB2Item> CurrPCItemList;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInven)
	{
		LobbyInven->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CurSelectedItemInvenTab, SelectionPCClass, true);
	}
	
	const int32 iItemListArrayMax = CurrPCItemList.Num();
	for (int32 i = 0; i < iItemListArrayMax; ++i)
	{
		if (CurrPCItemList.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayCostumeItemSlot.IsValidIndex(i) == false)
		{
			auto* pCostumeItemSlot = CreateWidget<UB2UICostumeItemSlot>(GetOwningPlayer(), CostumeItemSlotClass);
			pCostumeItemSlot->Init();

			if (auto pGridSlot = Cast<UUniformGridSlot>(UIP_ScrollBoxWithGridPanelBP->UniformGridPanelAddChild(pCostumeItemSlot)))
			{
				pGridSlot->SetRow(i / iHorizontalMax);
				pGridSlot->SetColumn(i % iHorizontalMax);
			}
			
			ArrayCostumeItemSlot.Add(pCostumeItemSlot);
		}

		ArrayCostumeItemSlot[i]->SetupManualScrollBoxSender_byReceiver(this, UIP_ScrollBoxWithGridPanelBP->GetB2ScrollBox());
		ArrayCostumeItemSlot[i]->SetVisibility(ESlateVisibility::Visible);
		ArrayCostumeItemSlot[i]->UpdateCostumeItemSlot_FromFB2Item(CurrPCItemList[i]);
	}

	const int32 iItemSlotArrayMax = ArrayCostumeItemSlot.Num();
	for (int32 i = iItemListArrayMax; i < iItemSlotArrayMax; ++i)
	{
		if (ArrayCostumeItemSlot.IsValidIndex(i))
		{
			ArrayCostumeItemSlot[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	RedDotCondition_TapState();
	UpdateCostumeInvenItemCounter(true);
}

void UB2UICostumePageInven::UpdateCostumeInvenFromMaterials()
{
	if (UIP_ScrollBoxWithGridPanelBP.IsValid() == false)
	{
		return;
	}

	InitScrollOffset();

	TArray<FB2Item> CurrPCItemList;
	TArray<FB2Item> TempPCItemList;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInven)
	{
		LobbyInven->GetStoredItemList_ItemOpModeFilter(TempPCItemList, CurSelectedItemInvenTab, EPCClass::EPC_End, true);
		
		for (auto Item : TempPCItemList)
		{
			if (Item.ItemRefID == FItemRefIDHelper::ITEM_REF_ID_ENHANCE_COSTUMEWEAPON_ITEM ||
				Item.ItemRefID == FItemRefIDHelper::ITEM_REF_ID_ENHANCE_COSTUMEARMOR_ITEM)
			{
				CurrPCItemList.Add(Item);
			}
		}
	}

	const int32 iItemListArrayMax = CurrPCItemList.Num();
	for (int32 i = 0; i < iItemListArrayMax; ++i)
	{
		if (CurrPCItemList.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayCostumeItemSlot.IsValidIndex(i) == false)
		{
			auto* pCostumeItemSlot = CreateWidget<UB2UICostumeItemSlot>(GetOwningPlayer(), CostumeItemSlotClass);
			pCostumeItemSlot->Init();

			if (auto pGridSlot = Cast<UUniformGridSlot>(UIP_ScrollBoxWithGridPanelBP->UniformGridPanelAddChild(pCostumeItemSlot)))
			{
				pGridSlot->SetRow(i / iHorizontalMax);
				pGridSlot->SetColumn(i % iHorizontalMax);
			}

			ArrayCostumeItemSlot.Add(pCostumeItemSlot);
		}

		ArrayCostumeItemSlot[i]->SetupManualScrollBoxSender_byReceiver(this, UIP_ScrollBoxWithGridPanelBP->GetB2ScrollBox());
		ArrayCostumeItemSlot[i]->SetVisibility(ESlateVisibility::Visible);
		ArrayCostumeItemSlot[i]->UpdateCostumeItemSlot_FromFB2Item(CurrPCItemList[i]);
	}

	const int32 iItemSlotArrayMax = ArrayCostumeItemSlot.Num();
	for (int32 i = iItemListArrayMax; i < iItemSlotArrayMax; ++i)
	{
		if (ArrayCostumeItemSlot.IsValidIndex(i))
		{
			ArrayCostumeItemSlot[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	RedDotCondition_TapState();
	UpdateCostumeInvenItemCounter(false);
}

void UB2UICostumePageInven::CloseAllStandaloneUIFromUIManager()
{
	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		pUIManager->CloseAllStandaloneUIs();
	}
}

void UB2UICostumePageInven::RedDotCondition_TapState()
{
	DoMarkRedDot();
	RefreshCostumeHeroMgmtRedDotClass<>::GetInstance().Signal();
}

void UB2UICostumePageInven::RequestModifyCostumeNewFlag()
{// 서버로 NEW 마크 갱신
	
	if(CurSelectedItemInvenTab == EItemInvenType::EIIVT_Consumables)
		return;

	if (AB2LobbyGameMode* pLobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer())))
	{
		const EPCClass CurrentPCClass = pLobbyGameMode->GetHeroMgmtModePCClass();
	
		RequestModifyCostumeNewFlag(CurrentPCClass, CurSelectedItemInvenTab);

		UB2LobbyInventory::SetChangeNewItemTofalseTap(CurrentPCClass, CurSelectedItemInvenTab);
	}
}

void UB2UICostumePageInven::RequestModifyCostumeNewFlag(EPCClass CurrentClass, EItemInvenType InvenType)
{
	if (CurrentClass == EPCClass::EPC_End)
		return;

	if (InvenType == EItemInvenType::EIIVT_Consumables)
		return;

	if (BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_CostumeInventory(CurrentClass, InvenType))
	{
		data_trader::Retailer::GetInstance().RequestModifyCostumeNewFlag(CliToSvrPCClassType(CurrentClass), CliToSvrItemInvenType(InvenType));
		BeforeCurSelectedPCClass = CurrentClass;
		BeforeCurSelectedItemInvenTab = InvenType;
	}
}

FText UB2UICostumePageInven::GetContainerSizeText() const
{
	return FText::AsNumber(BladeIIGameImpl::GetClientDataStore().GetMaxCostumeInventorySlotCount());
}

FText UB2UICostumePageInven::GetStoredItemNumText() const
{
	return FText::FromString(FString::Printf(TEXT("%d"), CachedInventory->GetStoredCostomeItemNumInUnitedSlot(LastSelectedPCClass)));
}

FSlateColor UB2UICostumePageInven::GetStoredItemNumTextColor() const
{
	bool bIsFull = CachedInventory->GetStoredCostomeItemNumInUnitedSlot(LastSelectedPCClass) > BladeIIGameImpl::GetClientDataStore().GetMaxCostumeInventorySlotCount();
	return bIsFull ? StoredItemNumTextColor_Over : StoredItemNumTextColor_Normal;
}

void UB2UICostumePageInven::UpdateCostumeInvenItemCounter(bool bVisible)
{
	if (StoredItemNumText.IsValid())
	{
		StoredItemNumText->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		StoredItemNumText->SetText(GetStoredItemNumText());
		StoredItemNumText->SetColorAndOpacity(GetStoredItemNumTextColor());
	}

	if (ContainerSizeText.IsValid())
	{
		ContainerSizeText->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		ContainerSizeText->SetText(GetContainerSizeText());
	}

	if(SlashText.IsValid())
		SlashText->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UICostumePageInven::ForcedTabClickMaterial(bool bFreezeTab)
{
	if(bFreezeTab)
		OnClickBTN_TabMaterial();

	if (BTN_TabArmors.IsValid())
		BTN_TabArmors->SetIsEnabled(!bFreezeTab);
	if (BTN_TabWeapon.IsValid())
		BTN_TabWeapon->SetIsEnabled(!bFreezeTab);
}

void UB2UICostumePageInven::OnCallBackEnhance(const FB2Item& InItem)
{
	bool bIsEnhanceAble = InItem.EnhanceLevel < MAX_ENHANCE_COSTUME_LEVEL;
	if (bIsEnhanceAble)
	{
		float fPoupSizeX = 0.f;

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
			UI->UpdatePopupInfo(InItem);
		}

		ForcedClickCostumeMaterialClass<bool>::GetInstance().Signal(true);
	}
	else
	{
		bool bIsWeapon = InItem.InventoryType == EItemInvenType::EIIVT_CostumeWeapon;
		if (bIsWeapon)
		{
			OnClickBTN_TabWeapon();
		}
		else
		{
			OnClickBTN_TabArmors();
		}
	}
}