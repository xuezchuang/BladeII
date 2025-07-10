// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIItemQualityChange.h"

#include "B2UIManager.h"
#include "B2LobbyUI_InventoryMain.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUserWidget.h"
#include "B2LobbyUISwitcher.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2DynText_Multiple.h"
#include "B2ButtonGoodsShortcutToolTip.h"
#include "EventSubsystem.h"
#include "Retailer.h"
#include "Event.h"
#include "B2ItemInfo.h"
#include "B2LobbyGameMode.h"
#include "B2UIManager_Lobby.h"

#include "B2UIShortagePopupManager.h"
#include "B2LobbyUI_ItemAfterOption.h"
#include "UB2UIQualityHammerSelectPopup.h"
#include "CommonStruct.h"

#include "BladeIIGameImpl.h"

AB2LobbyGameMode* UB2UIItemQualityChange::CachedLobbyGM = NULL;

UB2UIItemQualityChange::UB2UIItemQualityChange(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SubscribeEvents();

	ItemUniqueInstanceID = 0;
	ItemReferenceID = 0;
	hammercycleCount = 0;

	bEnableApplyQuality = false;
	bItemQualityApplied = false;
}

void UB2UIItemQualityChange::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_OptionChangeTitle);
	GET_SLOT(UB2UIQualityChangeDetail, UIP_OriginalOption);
	if (UIP_OriginalOption.IsValid())
		UIP_OriginalOption->Init();
	GET_SLOT(UB2UIQualityChangeDetail, UIP_ChangedOption);
	if (UIP_ChangedOption.IsValid())
		UIP_ChangedOption->Init();

	GET_SLOT(UTextBlock, TB_OriginalOption);
	GET_SLOT(UTextBlock, TB_ChangedOption);
	GET_SLOT(UB2DynItemIcon_LobbyInven, UIP_BeforeItemIcon);
	GET_SLOT(UB2DynItemIcon_LobbyInven, UIP_AfterItemIcon);
	GET_SLOT(UTextBlock, TB_ExpectedQuality);
	GET_SLOT(UTextBlock, TB_MinQuality);
	GET_SLOT(UTextBlock, TB_MiddleQuality);
	GET_SLOT(UTextBlock, TB_MaxQuality);
	GET_SLOT(UB2RichTextBlock, RTB_ChangeDesc);
	GET_SLOT(UB2Button, BTN_ApplyQuality);
	GET_SLOT(UTextBlock, TB_ApplyQuality);
	GET_SLOT(UB2ButtonGoodsShortcutToolTip, BTN_ChangeHammer);
	GET_SLOT(UTextBlock, TB_ChangeHammer);
	GET_SLOT(UB2ButtonGoodsShortcutToolTip, BTN_ChangeQuality);
	GET_SLOT(UTextBlock, TB_ChangeQuality);
	GET_SLOT(UTextBlock, TB_ChangeQualityCost);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_OptionInfo);

	GET_SLOT(UImage, IMG_ChangeQuality);
	GET_SLOT(UTextBlock, TB_QualityChangeCost);

	// Result Popup
	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UTextBlock, TB_Confirm);
	GET_SLOT(UTextBlock, TB_ChangeResultTitle);
	GET_SLOT(UB2UIQualityChangeDetail, UIP_ResultOptionDetail);
	if (UIP_ResultOptionDetail.IsValid())
		UIP_ResultOptionDetail->Init();

	GET_SLOT(UPanelWidget, X_HammerChangePopup);

	BTN_OptionInfo->SetVisibility(ESlateVisibility::Collapsed);
	//TB_QualityChangeCost->SetVisibility(ESlateVisibility::Collapsed);

	UIP_AfterItemIcon->SetSelectIcon(false);

	BIND_CLICK_FUNC_TO_BTN(BTN_ApplyQuality, &UB2UIItemQualityChange::OnClickBTN_ApplyQuality);
	BIND_CLICK_FUNC_TO_BTN(BTN_ChangeQuality, &UB2UIItemQualityChange::OnClickBTN_ChangeQuality);
	BIND_CLICK_FUNC_TO_BTN(BTN_ChangeHammer, &UB2UIItemQualityChange::OnClickBTN_ChangeHammer);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIItemQualityChange::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIItemQualityChange::OnClickBTN_Confirm);
}

void UB2UIItemQualityChange::UpdateStaticText()
{
	if (RTB_OptionChangeTitle.IsValid())
	{
		RTB_OptionChangeTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_Name")));
	}
	if (TB_ExpectedQuality.IsValid())
	{
		TB_ExpectedQuality->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_QualityRange")));
	}

	if (TB_ApplyQuality.IsValid())
	{
		TB_ApplyQuality->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_QualityConfirm")));
	}
	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Confirm")));
	}
	if (TB_ChangeHammer.IsValid())
	{
		TB_ChangeHammer->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_ChangeHammer")));
	}
	if (TB_ChangeQuality.IsValid())
	{
		TB_ChangeQuality->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_Name")));
	}

	if (TB_OriginalOption.IsValid())
	{
		TB_OriginalOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_BeforeQuality")));
	}
	if (TB_ChangedOption.IsValid())
	{
		TB_ChangedOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_AfterQuality")));
	}

	if (RTB_ChangeDesc.IsValid())
	{
		RTB_ChangeDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_Guide_1")));
	}

	if (TB_ChangeResultTitle.IsValid())
	{
		TB_ChangeResultTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_AfterQuality")));
	}
}
void UB2UIItemQualityChange::UpdateDynamicText()
{
	if (BladeIIGameImpl::GetClientDataStore().GetMinQuality(SelectedHammer) == BladeIIGameImpl::GetClientDataStore().GetMaxQuality(SelectedHammer))
	{
		if (TB_MinQuality.IsValid())
		{
			TB_MinQuality->SetText(GetLOCTextOfItemQuality(BladeIIGameImpl::GetClientDataStore().GetMinQuality(SelectedHammer)));
		}
		if (TB_MiddleQuality.IsValid())
		{
			TB_MiddleQuality->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_Confirm")));
		}
		TB_MaxQuality->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (TB_MinQuality.IsValid())
		{
			TB_MinQuality->SetText(GetLOCTextOfItemQuality(BladeIIGameImpl::GetClientDataStore().GetMinQuality(SelectedHammer)));
		}
		if (TB_MiddleQuality.IsValid())
		{
			TB_MiddleQuality->SetText(FText::FromString("~"));
		}
		if (TB_MaxQuality.IsValid())
		{
			TB_MaxQuality->SetText(GetLOCTextOfItemQuality(BladeIIGameImpl::GetClientDataStore().GetMaxQuality(SelectedHammer)));
		}
		TB_MaxQuality->SetVisibility(ESlateVisibility::Visible);
	}

	if (TB_QualityChangeCost.IsValid())
	{
		TB_QualityChangeCost->SetText(FText::FromString(FString::FromInt(CachedInventory->HowMannyHammer(SelectedHammer))));
		TB_QualityChangeCost->SetColorAndOpacity(CachedInventory->HowMannyHammer(SelectedHammer)>0 ? EnoughColor : NotEnoughColor);
	}
}

void UB2UIItemQualityChange::ResponseGenerateItemRandomQuality(FB2ResponseGenerateItemRandomQualityPtr GenerateItemRandomQualityPtr)
{
	TArray<FItemOption> GeneratedQuality;
	if (GenerateItemRandomQualityPtr)
	{
		ChangeUserAccountInfoClass<>::GetInstance().Signal();

		EItemOption ItemOption1 = SvrToCliOptionType(GenerateItemRandomQualityPtr->quality);

		GeneratedQuality.Add(FItemOption(ItemOption1, GenerateItemRandomQualityPtr->quality));

		//SetNewQualityDetail(GeneratedQuality);
		CachedQuality = GenerateItemRandomQualityPtr->quality;
		SetAfterItemIcon(CachedItem, CachedQuality);
		
		
		if (bEnableApplyQuality)
		{
			OnGenerateOtherItemQuality_BP();
		}
		else
		{
			bEnableApplyQuality = true;
			OnGenerateNewItemQuality_BP();
		}
		
		if (BTN_ApplyQuality.IsValid())
		{
			BTN_ApplyQuality->SetIsEnabled(bEnableApplyQuality);
		}

		RefreshOptionChangeCostColor();
		
 		if (UIP_ChangedOption.IsValid())
 		{
			FB2Item previewItem = CachedItem;
			previewItem.Quality = GenerateItemRandomQualityPtr->quality;
			GetCalculateItem_PreviewItemEnhance(previewItem, previewItem);

 			UIP_ChangedOption->OnItemQualityGenerated_BP(1);
			AppliedOption.Empty();

			const EItemClass& ItemClass = CachedItem.ItemClass;

			AppliedOption.Add(ItemClass);
			// 장신구에서 강화 레벨에 따른 제곱을 제거하기 위한 용도로 강화 안된 아이템에만 -1을 함
			if(ItemClass == EItemClass::EIC_Belt || ItemClass == EItemClass::EIC_Necklace || ItemClass == EItemClass::EIC_Ring)
				if (previewItem.EnhanceLevel == 0) previewItem.SetPrimaryPoint(previewItem.GetPrimaryPoint() - 1);
			UIP_ChangedOption->SetCompareQualityText(ItemClass, CachedItem.GetPrimaryPoint(), previewItem.GetPrimaryPoint());
 		}


		if (CachedQuality == MAX_ITEM_QUALITY ||
			CachedQuality == BladeIIGameImpl::GetClientDataStore().GetMaxQuality(SelectedHammer))
		{
			DisableChangeButtons();
		}
	}
}

void UB2UIItemQualityChange::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryGetItemRandomQualityClass<FB2ResponseGetItemRandomQualityPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseGetItemRandomQualityPtr QualityItemDataPtr)
	{
		this->ResponseGetItemRandomQuality(QualityItemDataPtr);
	}
	));

	Issues.Add(DeliveryGenerateItemRandomQualityClass<FB2ResponseGenerateItemRandomQualityPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseGenerateItemRandomQualityPtr RandomQualityPtr)
	{
		this->ResponseGenerateItemRandomQuality(RandomQualityPtr);
	}
	));

	Issues.Add(DeliveryApplyItemRandomQualityClass<FB2ResponseApplyItemRandomQualityPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseApplyItemRandomQualityPtr ApplyQualityPtr)
	{
		this->ResponseApplyItemRandomQuality(ApplyQualityPtr);
	}
	));

	Issues.Add(DeliveryDiscardItemRandomQualityClass<FB2ResponseDiscardItemRandomQualityPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseDiscardItemRandomQualityPtr DiscardQualityPtr)
	{
		this->ResponseDiscardItemRandomQuality(DiscardQualityPtr);
	}
	));
}

void UB2UIItemQualityChange::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIItemQualityChange::ResponseGetItemRandomQuality(FB2ResponseGetItemRandomQualityPtr QualityItemDataPtr)
{
	if (QualityItemDataPtr)
	{
		/*
		if something is there
		{
			run same as generate stuff or discard it.

			//recover option


			//discard option
			BladeIIGameImpl::GetClientDataStore().ClearUnappliedItemQualityInfo();
			bEnableApplyQuality = false;
		}
		else
		{
			do nothing.
		}
		*/
	}
}

void UB2UIItemQualityChange::ResponseApplyItemRandomQuality(FB2ResponseApplyItemRandomQualityPtr ApplyItemRandomQualityPtr)
{
	if (ApplyItemRandomQualityPtr)
	{
		FB2Item AppliedItem;
		AppliedItem = ApplyItemRandomQualityPtr->main_item;
		BladeIIGameImpl::GetClientDataStore().ItemQualityChange(AppliedItem);

		FB2Item previewItem = CachedItem;
		previewItem.Quality = CachedQuality;
		GetCalculateItem_PreviewItemEnhance(previewItem, previewItem);

		SetApplyQualityDetail(previewItem);

		bEnableApplyQuality = false;

		//CachedApplyItemQualityForAnimation = AppliedItem;
		CachedApplyItemQualityForAnimation = previewItem;


		// UI갱신 - NewOption 삭제
		if (UIP_ChangedOption.IsValid())
			UIP_ChangedOption->ClearQualityTextList();

		bItemQualityApplied = true; // ItemOptionChange UI Close 후 InventoryUI 갱신

								   // Apply되면 Datastore에 저장된 값 삭제해버려
		BladeIIGameImpl::GetClientDataStore().ClearUnappliedItemQualityInfo();

		OnItemQualityApplied_BP();

		// 전투력 갱신
		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(IntToPCClass(AppliedItem.AllowedPCClass), false);
	}
}

void UB2UIItemQualityChange::ResponseDiscardItemRandomQuality(FB2ResponseDiscardItemRandomQualityPtr DiscardItemRandomQualityPtr)
{
	BladeIIGameImpl::GetClientDataStore().ClearUnappliedItemQualityInfo();
	bEnableApplyQuality = false;
	if(ment2Close==true)
		DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_QualityChange);
}

void UB2UIItemQualityChange::DisableChangeButtons()
{
	if (BTN_ChangeHammer.IsValid())
	{
		BTN_ChangeHammer->SetIsEnabled(false);
	}
	if (BTN_ChangeQuality.IsValid())
	{
		BTN_ChangeQuality->SetIsEnabled(false);
	}
}

void UB2UIItemQualityChange::SetApplyQualityDetail(const FB2Item& TargetItem)
{
	if (UIP_ResultOptionDetail.IsValid())
	{
		AppliedOption.Empty();

		FB2Item previewItem = CachedItem;
		previewItem.Quality = CachedQuality;
		GetCalculateItem_PreviewItemEnhance(previewItem, previewItem);

		const EItemClass& ItemClass = TargetItem.ItemClass;

		AppliedOption.Add(ItemClass);
		// 강화 레벨에 따른 제곱을 제거하기 위한 용도로 강화 안된 아이템에만 -1을 함
		if (ItemClass == EItemClass::EIC_Belt || ItemClass == EItemClass::EIC_Necklace || ItemClass == EItemClass::EIC_Ring)
			if (previewItem.EnhanceLevel == 0) previewItem.SetPrimaryPoint(previewItem.GetPrimaryPoint() - 1);
		UIP_ResultOptionDetail->SetOptionText(ItemClass, previewItem.GetPrimaryPoint());
	}
}

void UB2UIItemQualityChange::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

bool UB2UIItemQualityChange::HasRemainUnAppliedQuality()
{
	return bEnableApplyQuality || BladeIIGameImpl::GetClientDataStore().HasUnappliedItemQuality();
}

void UB2UIItemQualityChange::SetOriginalQualityDetail(const FB2Item& TargetItem)
{
	if (UIP_OriginalOption.IsValid())
	{
		//const int32 OptionCount = TargetItem.RandomOptions.Num();
		//const EPCClass OwnerPCClass = IntToPCClass(TargetItem.AllowedPCClass);	// Server값 아님 -> 바로 Casting 해서 쓰고있음

		//ItemOwnerPCClass = OwnerPCClass;
		OriginOption.Empty();

		const EItemClass& ItemClass = TargetItem.ItemClass;

		OriginOption.Add(ItemClass);
		// FItemOption 초기값 EItemOption::EIO_End 로 없는 option이 들어가면 옵션 Text를 자동 Collapsed 처리
		UIP_OriginalOption->SetOptionText(ItemClass, TargetItem.GetPrimaryPoint());



		//OriginalOptionCount = OptionCount;
	}
}

void UB2UIItemQualityChange::SetTargetItem(FB2Item& TargetItem, AB2LobbyGameMode* TransientLobbyGM)
{
	Construct();
	UpdateStaticText();

	SetOriginalQualityDetail(TargetItem);
	SetTargetItemIcon(TargetItem);
	RefreshOptionChangeCostColor();
	if(TransientLobbyGM!=NULL)
		CachedLobbyGM = TransientLobbyGM;
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();

	if (BTN_ChangeHammer.IsValid())
	{
		BTN_ChangeHammer->ClearInfo();

// 		if (hammerAmount > 0)
// 			BTN_ChangeHammer->SetIsEnabled(true);
// 		else
// 			BTN_ChangeHammer->SetIsEnabled(false);
	}

	AllIngredIDs.Empty();
	CachedInventory->GetAvailableHammers(TargetItem, AllIngredIDs);
	SelectedHammer = AllIngredIDs[0];

	int32 hammerAmount = 0;
	for (int32 i = 0; i < AllIngredIDs.Num(); i++)
	{
		hammerAmount += CachedInventory->HowMannyHammer(AllIngredIDs[i]);
	}

	for (int32 i = 0; i < AllIngredIDs.Num(); i++)
	{
		if (CachedInventory->HowMannyHammer(AllIngredIDs[i]) <= 0)
		{
			HammerCycle();
		}
		else
		{
			break;
		}
	}
	hammercycleCount = 0;
	if (hammerAmount == CachedInventory->HowMannyHammer(SelectedHammer))
	{
		BTN_ChangeHammer->SetIsEnabled(false);
	}
	else
	{
		BTN_ChangeHammer->SetIsEnabled(true);
	}

	UpdateDynamicText();

	if (HasRemainUnAppliedQuality())
	{
		DeliveryGenerateItemRandomQualityClass<FB2ResponseGenerateItemRandomQualityPtr>::GetInstance().Subscribe2(
			[this](FB2ResponseGenerateItemRandomQualityPtr RandomQualityPtr)
		{
			this->ResponseGenerateItemRandomQuality(RandomQualityPtr);
		}
		);
	}

	FSingleItemInfoData* itemInfo = ItemInfoTable ? ItemInfoTable->GetInfoData(SelectedHammer) : NULL;
	IMG_ChangeQuality->SetBrushFromMaterial(itemInfo->GetIconMaterial(ItemInfoTable));

	if (BTN_ApplyQuality.IsValid())
	{
		bool bEnableRequestApply = HasRemainUnAppliedQuality();
		BTN_ApplyQuality->SetIsEnabled(bEnableRequestApply);
	}

	if (BTN_ChangeQuality.IsValid())
	{
		BTN_ChangeQuality->ClearInfo();
	}

	ItemUniqueInstanceID = TargetItem.InstanceUID;
	ItemReferenceID = TargetItem.ItemRefID;
	CachedItem = TargetItem;
	ment2Close = false;
}

void UB2UIItemQualityChange::SwitchTargetItem(FB2Item& TargetItem)
{
	// doesn't use
	/*
	SetOriginalQualityDetail(TargetItem);
	SetTargetItemIcon(TargetItem);
	RefreshOptionChangeCostColor();
	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();

	const int32 hammerAmount = CachedInventory ? CachedInventory->HaveEnoughHammer(TargetItem) : 0;
	if (BTN_ChangeHammer.IsValid())
	{
		BTN_ChangeHammer->ClearInfo();

		if (hammerAmount > 0)
			BTN_ChangeHammer->SetIsEnabled(true);
		else
			BTN_ChangeHammer->SetIsEnabled(false);
	}

	CachedInventory = CachedLobbyGM->GetLobbyInventory();

	AllIngredIDs.Empty();
	CachedInventory->GetAvailableHammers(TargetItem, AllIngredIDs);
	SelectedHammer = AllIngredIDs[0];

	FSingleItemInfoData* itemInfo = ItemInfoTable ? ItemInfoTable->GetInfoData(SelectedHammer) : NULL;
	IMG_ChangeQuality->SetBrushFromMaterial(itemInfo->GetIconMaterial(ItemInfoTable));



	if (BTN_ApplyQuality.IsValid())
	{
		bool bEnableRequestApply = HasRemainUnAppliedQuality();
		BTN_ApplyQuality->SetIsEnabled(bEnableRequestApply);
	}

	if (BTN_ChangeQuality.IsValid())
	{
		BTN_ChangeQuality->ClearInfo();
	}

	ItemUniqueInstanceID = TargetItem.InstanceUID;
	ItemReferenceID = TargetItem.ItemRefID;
	CachedItem = TargetItem;
	*/
}

void UB2UIItemQualityChange::SetTargetItemIcon(const FB2Item& TargetItem)
{
	if (UIP_BeforeItemIcon.IsValid())
	{
		UIP_BeforeItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow);
		UIP_BeforeItemIcon->UpdateItemData(TargetItem);
	}
}
void UB2UIItemQualityChange::SetAfterItemIcon(const FB2Item& OriginalItem, int32 qualityValue)
{
	if (UIP_AfterItemIcon.IsValid())
	{
		FB2Item tempItemIcon = OriginalItem;
		tempItemIcon.Quality = qualityValue;
		UIP_AfterItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow);
		UIP_AfterItemIcon->UpdateItemData(tempItemIcon);
	}
}

void UB2UIItemQualityChange::RefreshOptionChangeCostColor()
{
	if (TB_ChangeQualityCost.IsValid())
	{
		TB_ChangeQualityCost->SetColorAndOpacity(IsEnableButtonQualityChange() ? EnoughColor : NotEnoughColor);
	}
}

bool UB2UIItemQualityChange::IsEnableButtonOptionCashChange()
{
	bool ResultValue = false;

	// If has Enough Cash
	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	// dummy
	const int32 CostCash = BladeIIGameImpl::GetClientDataStore().GetItemOptionChangeCashCost();
	const int32 UserCash = ClientDataStore.GetGemAmount();

	if (UserCash >= CostCash)
	{
		ResultValue = true;
	}

	return ResultValue;
}

bool UB2UIItemQualityChange::IsEnableButtonQualityChange()
{
	// If has Enough Dark Crystal
	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	// dummy
	const int32 CostDarkCrystal = BladeIIGameImpl::GetClientDataStore().GetItemOptionChangeDarkCrystalCost();
	const int32 UserDarkCrystal = ClientDataStore.GetDarkCrystal();

	if (UserDarkCrystal >= CostDarkCrystal)
		return true;

	return false;
}

void UB2UIItemQualityChange::VisibleCloseButton()
{
	SetClose(true);
}

void UB2UIItemQualityChange::InvisibleCloseButton()
{
	SetClose(false);
}

void UB2UIItemQualityChange::RequestNewItemQuality(int64 HammerID)
{
	data_trader::Retailer::GetInstance().RequestGenerateItemRandomQuality(ItemUniqueInstanceID, HammerID);
}

void UB2UIItemQualityChange::OnItemQualityApplyAnimEnd()
{
	// 애니메이션 연출 후 변경된 옵션은 Reset하고 현재옵션창을 변경된 옵션으로 교체하는 작업
	if (UIP_OriginalOption.IsValid()) {
		//UIP_OriginalOption->OnItemQualityGenerated_BP(OriginalOptionCount);
	}
	
	UIP_BeforeItemIcon = UIP_AfterItemIcon;
}

void UB2UIItemQualityChange::CreateLobbyHammerChangePopup(UObject* WorldContextObject, FB2Item& InItemData)
{
	//어떠한 망치를 쓸건지 선택하는 UI를 띄우는 함수
	//아직 그쪽 관련으로 어떠한 진행도 되지 않았다. 그냥 틀만 있으며 앞으로의 진행 방법에 대해 추가 연구 필요.

	// 보관함 아이템 혹은 장착 아이템
	const ELobbySubPopups LobbyPopupEnum = ELobbySubPopups::ELSPU_ItemAfterOpiton;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 해당 user widget 생성 신호.
	// 생성된 Popup widget 을 UIP 처럼 배치해 넣을 패널 정보를 만들어준다.
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(LobbyPopupEnum, FDJLegacySubpopupAddPanelInfo(this, X_HammerChangePopup.Get()));

	// 생성된 user widget 을 찾아서 ItemData 를 업데이트
	UB2LobbyUI_ItemAfterOption* CreatedDetailWindow = LobbyGM ? Cast<UB2LobbyUI_ItemAfterOption>(LobbyGM->DJLegacy_FindLobbySubPopup(LobbyPopupEnum)) : NULL;

	/*
	if (CreatedDetailWindow)
	{
		CreatedDetailWindow->UpdateItemData(CachedItem, true);
		CreatedDetailWindow->ClickCloseDelegate.BindUObject(this, &UB2UIItemQualityChange::CloseHammerChangePopup);
	}
	*/
}

void UB2UIItemQualityChange::CloseHammerChangePopup()
{
	if (X_HammerChangePopup.IsValid())
		X_HammerChangePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIItemQualityChange::OnClickBTN_ChangeQuality()
{
	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().DecSharedConsumableAmountOfType(SelectedHammer, 1);
	UpdateDynamicText();
	RequestNewItemQuality(SelectedHammer);
}

void UB2UIItemQualityChange::OnClickBTN_ChangeHammer()
{
	HammerCycle();

	hammercycleCount = 0;

	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	FSingleItemInfoData* itemInfo = ItemInfoTable ? ItemInfoTable->GetInfoData(SelectedHammer) : NULL;
	IMG_ChangeQuality->SetBrushFromMaterial(itemInfo->GetIconMaterial(ItemInfoTable));
	UpdateDynamicText();
}

void UB2UIItemQualityChange::HammerCycle()
{
	if (hammercycleCount >= AllIngredIDs.Num())
	{
		hammercycleCount = 0;
		return;
	}

	for (int32 i = 0; i < AllIngredIDs.Num(); i++)
	{
		if (SelectedHammer == AllIngredIDs[i])
		{
			if (i + 1 == AllIngredIDs.Num())
			{
				SelectedHammer = AllIngredIDs[0];
			}
			else
			{
				i++;
				SelectedHammer = AllIngredIDs[i];
			}

			if (CachedInventory->HowMannyHammer(AllIngredIDs[i]) <= 0)
			{
				hammercycleCount++;
				HammerCycle();
				return;
			}
		}
	}
}


void UB2UIItemQualityChange::RequestItemQualityApply()
{
	data_trader::Retailer::GetInstance().RequestApplyItemRandomQuality(ItemUniqueInstanceID);
}

void UB2UIItemQualityChange::OnClickBTN_ApplyQuality()
{
	RequestItemQualityApply();
	bEnableApplyQuality = false;
	if (BTN_ApplyQuality.IsValid())
		BTN_ApplyQuality->SetIsEnabled(bEnableApplyQuality);
}

void UB2UIItemQualityChange::OnClickBTN_Close()
{
	if (HasRemainUnAppliedQuality())
	{
		// 변경 후 적용하지 않은 옵션이 있다면
		ment2Close = true;
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QualityChange_Caution")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([this]() {
			data_trader::Retailer::GetInstance().RequestDiscardItemRandomQuality(this->ItemUniqueInstanceID);
			UB2UIManager::GetInstance()->CloseUI(UIFName::ItemQualityChange); })
		);

		return;
	}

	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_QualityChange);
}

void UB2UIItemQualityChange::OnClickBTN_Confirm()
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_QualityChange);
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

void UB2UIItemQualityChange::CreateHammerPopup(UObject* WorldContextObject, FB2Item& InItemData)
{
	/*
	// 보관함 아이템 혹은 장착 아이템
	const ELobbySubPopups LobbyPopupEnum = ELobbySubPopups::ELSPU_QualityChangeHammerSelect;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 해당 user widget 생성 신호.
	// 생성된 Popup widget 을 UIP 처럼 배치해 넣을 패널 정보를 만들어준다.
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(LobbyPopupEnum, FDJLegacySubpopupAddPanelInfo(this, X_HammerChangePopup.Get()));

	// 생성된 user widget 을 찾아서 ItemData 를 업데이트
	UB2UIQualityHammerSelectPopup* CreatedDetailWindow = LobbyGM ? Cast<UB2UIQualityHammerSelectPopup>(LobbyGM->DJLegacy_FindLobbySubPopup(LobbyPopupEnum)) : NULL;

	
	if (CreatedDetailWindow)
	{
	CreatedDetailWindow->UpdateItemData(CachedItem, true);
	CreatedDetailWindow->ClickCloseDelegate.BindUObject(this, &UB2UIItemOptionChange::CloseAfterOptionPopup);
	}
	*/
}

///////////////////////////////////////////////////////////////////
//					UB2UIQualityChangeDetail
///////////////////////////////////////////////////////////////////

void UB2UIQualityChangeDetail::CacheAssets()
{
	OptionTextList.AddDefaulted(MAX_ITEM_RANDOM_OPTION);

	GET_SLOT_BYNAME(UB2DynText_Multiple, OptionTextList[0], UIP_OptionText1);

	GET_SLOT(UImage, IMG_Empty);
}

void UB2UIQualityChangeDetail::Init()
{
	Super::Init();

	ClearQualityTextList();
}

void UB2UIQualityChangeDetail::ClearQualityTextList()
{
	for (auto& OptionTextWidget : OptionTextList)
	{
		if (OptionTextWidget.IsValid())
		{
			OptionTextWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (IMG_Empty.IsValid())
	{
		IMG_Empty->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UIQualityChangeDetail::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIQualityChangeDetail::SetOptionText(EItemClass ClassType, float value)
{

	TWeakObjectPtr<UB2DynText_Multiple> OptionTextWidget = OptionTextList[0];
	OptionTextWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FText OptionText;

	switch (ClassType)
	{
	case EItemClass::EIC_Weapon:
		OptionText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack"));
		break;
	case EItemClass::EIC_Helmet:
	case EItemClass::EIC_BodyArmor:
	case EItemClass::EIC_Glove:
	case EItemClass::EIC_Shoes:
		OptionText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense"));
		break;
	case EItemClass::EIC_Belt:
	case EItemClass::EIC_Necklace:
	case EItemClass::EIC_Ring:
		OptionText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health"));
		break;

	}
	FText OptionValueText = FText::FromString(FString::FromInt((int32)value));

	OptionTextWidget->SetDynText(0, OptionText, OptionValueText);

	if (IMG_Empty.IsValid())
		IMG_Empty->SetVisibility(ESlateVisibility::Hidden);

}

FText UB2UIQualityChangeDetail::MakeParenthesesText(const float CompareValue, const FText& CompareText, const bool FindOption)
{
	FString Left("(");
	FString Right(")");
	FString StringValue = CompareText.ToString();
	if (CompareValue > 0.0f)
	{
		StringValue.InsertAt(0, "+");
	}
	else if (FindOption && FMath::IsNearlyZero(CompareValue))
	{
		StringValue = "-";
	}
	FString CompareResult = Left + StringValue + Right;
	return FText::FromString(CompareResult);
}

void UB2UIQualityChangeDetail::SetCompareQualityText(EItemClass ClassType, float beforeValue, float afterValue)
{
	TWeakObjectPtr<UB2DynText_Multiple> OptionTextWidget = OptionTextList[0];
	OptionTextWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FText CompareValueText;

	float CompareValue = afterValue - beforeValue;
	CompareValueText = FText::FromString(FString::FromInt((int32)CompareValue));
	// 색깔


	FText OptionText;

	switch (ClassType)
	{
	case EItemClass::EIC_Weapon:
		OptionText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack"));
		break;
	case EItemClass::EIC_Helmet:
	case EItemClass::EIC_BodyArmor:
	case EItemClass::EIC_Glove:
	case EItemClass::EIC_Shoes:
		OptionText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense"));
		break;
	case EItemClass::EIC_Belt:
	case EItemClass::EIC_Necklace:
	case EItemClass::EIC_Ring:
		OptionText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health"));
		break;

	}
	FText OptionValueText = FText::FromString(FString::FromInt((int32)afterValue));
	FSlateColor CompareColor = GetCompareTextColor(CompareValue, true);


	//OptionTextWidget->SetDynText(0, OptionText, OptionValueText, CompareValueText);
	OptionTextWidget->SetDynText(0, OptionText, OptionValueText, MakeParenthesesText(CompareValue, CompareValueText, true));
	OptionTextWidget->SetVisibility_Textbox(2, ESlateVisibility::Visible);
	OptionTextWidget->SetDynTextColor(2, CompareColor);

	if (IMG_Empty.IsValid())
		IMG_Empty->SetVisibility(ESlateVisibility::Hidden);

}

FSlateColor UB2UIQualityChangeDetail::GetCompareTextColor(const float CompareValue, const bool FindOption)
{
	if (FindOption)
	{
		if (CompareValue > 0.0f)
		{
			return UB2UIManager::GetInstance()->TextColor_Increase;
		}
		else if (FMath::IsNearlyZero(CompareValue))
		{
			return UB2UIManager::GetInstance()->TextColor_Same;
		}
		else
		{
			return UB2UIManager::GetInstance()->TextColor_Decrease;
		}
	}
	else
	{
		return NewTextColor;
	}
}