// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIItemOptionChange.h"
#include "B2UIManager.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2DynText_Multiple.h"
#include "B2ButtonGoodsShortcutToolTip.h"
#include "EventSubsystem.h"
#include "Retailer.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2UIManager_Lobby.h"

#include "B2UIShortagePopupManager.h"
#include "B2LobbyUI_ItemAfterOption.h"

#include "BladeIIGameImpl.h"

void UB2UIItemOptionChange::Init()
{
	Super::Init();

	ItemUniqueInstanceID = 0;
	ItemReferenceID = 0;

	bEnableApplyOption = false;
	bItemOptionApplied = false;

	SubscribeEvents();
}

void UB2UIItemOptionChange::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	if (UIP_OriginalOption.IsValid())
		UIP_OriginalOption->DestroySelf(InUIManager);
	if (UIP_ChangedOption.IsValid())
		UIP_ChangedOption->DestroySelf(InUIManager);
	if (UIP_ResultOptionDetail.IsValid())
		UIP_ResultOptionDetail->DestroySelf(InUIManager);

	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemAfterOpiton);

	Super::DestroySelf(InUIManager);
}

void UB2UIItemOptionChange::OnCloseComplete()
{
	Super::OnCloseComplete();

	//if (bItemOptionApplied)
	//{
	//	AB2LobbyGameMode* B2LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	//	UB2UIManager_Lobby* DJLegacyLobbyUIMgr = (B2LobbyGM && B2LobbyGM->IsValidObj()) ? B2LobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
	//	if (DJLegacyLobbyUIMgr)
	//	{
	//		DJLegacyLobbyUIMgr->TryUpdateSingleVisibleItemIcon(ItemUniqueInstanceID);
	//	}
	//}
}

void UB2UIItemOptionChange::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_OptionChangeTitle);
	GET_SLOT(UB2UIOptionChangeDetail, UIP_OriginalOption);
	if (UIP_OriginalOption.IsValid())
		UIP_OriginalOption->Init();
	GET_SLOT(UB2UIOptionChangeDetail, UIP_ChangedOption);
	if (UIP_ChangedOption.IsValid())
		UIP_ChangedOption->Init();

	GET_SLOT(UTextBlock, TB_OriginalOption);
	GET_SLOT(UTextBlock, TB_ChangedOption);
	GET_SLOT(UB2DynItemIcon_LobbyInven, UIP_ItemIcon);
	GET_SLOT(UB2RichTextBlock, RTB_ChangeDesc);
	GET_SLOT(UB2Button, BTN_ApplyOption);
	GET_SLOT(UTextBlock, TB_ApplyOption);
	GET_SLOT(UB2ButtonGoodsShortcutToolTip, BTN_ChangeOption);
	GET_SLOT(UTextBlock, TB_OptionChange);
	GET_SLOT(UTextBlock, TB_OptionChangeCost);
	GET_SLOT(UB2ButtonGoodsShortcutToolTip, BTN_ChangeOptionCash);
	GET_SLOT(UTextBlock, TB_OptionChangeCash);
	GET_SLOT(UTextBlock, TB_OptionChangeCostCash);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_OptionInfo);

	// Result Popup
	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UTextBlock, TB_Confirm);
	GET_SLOT(UTextBlock, TB_ChangeResultTitle);
	GET_SLOT(UB2UIOptionChangeDetail, UIP_ResultOptionDetail);
	if (UIP_ResultOptionDetail.IsValid())
		UIP_ResultOptionDetail->Init();

	GET_SLOT(UPanelWidget, X_MainPopup);
	GET_SLOT(UPanelWidget, X_AfterOptionPopup);
}

void UB2UIItemOptionChange::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_ApplyOption, &UB2UIItemOptionChange::OnClickBTN_ApplyOption);
	BIND_CLICK_FUNC_TO_BTN(BTN_ChangeOption, &UB2UIItemOptionChange::OnClickBTN_ChangeOption);
	BIND_CLICK_FUNC_TO_BTN(BTN_ChangeOptionCash, &UB2UIItemOptionChange::OnClickBTN_ChangeOptionCash);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIItemOptionChange::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIItemOptionChange::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_OptionInfo, &UB2UIItemOptionChange::OnClickBTN_OptionInfo);
}


void UB2UIItemOptionChange::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (RTB_OptionChangeTitle.IsValid())
	{
		RTB_OptionChangeTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_Title")));
	}

	if (TB_OriginalOption.IsValid())
	{
		TB_OriginalOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_OriginalOption")));
	}

	if (TB_ChangedOption.IsValid())
	{
		TB_ChangedOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_NewOption")));
	}

	if (RTB_ChangeDesc.IsValid())
	{
		RTB_ChangeDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_Description")));
	}

	if (TB_ApplyOption.IsValid())
	{
		TB_ApplyOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_ApplyOption")));
	}

	if (TB_OptionChange.IsValid())
	{
		TB_OptionChange->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_ChangeOption")));
	}

	if (TB_OptionChangeCash.IsValid())
	{
		TB_OptionChangeCash->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_ChangeOption")));
	}

	if (TB_OptionChangeCost.IsValid())
	{
		TB_OptionChangeCost->SetText(FText::FromString(FString::FromInt(BladeIIGameImpl::GetClientDataStore().GetItemOptionChangeDarkCrystalCost())));
	}

	if (TB_OptionChangeCostCash.IsValid())
	{
		TB_OptionChangeCostCash->SetText(FText::FromString(FString::FromInt(BladeIIGameImpl::GetClientDataStore().GetItemOptionChangeCashCost())));
	}

	// Result Popup
	if (TB_ChangeResultTitle.IsValid())
		TB_ChangeResultTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_NewOption")));
	if (TB_Confirm.IsValid())
		TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Confirm")));
}

void UB2UIItemOptionChange::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

bool UB2UIItemOptionChange::HasRemainUnAppliedOption()
{
	return bEnableApplyOption || BladeIIGameImpl::GetClientDataStore().HasUnappliedItemOption();
}

bool UB2UIItemOptionChange::IsEnableButtonOptionChange()
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

bool UB2UIItemOptionChange::IsEnableButtonOptionCashChange()
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

void UB2UIItemOptionChange::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryGenerateItemRandomOptionClass<FB2ResponseGenerateItemRandomOptionPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseGenerateItemRandomOptionPtr ServerGeneratedItemOptionPtr)
	{
		this->ResponseGenerateItemRandomOption(ServerGeneratedItemOptionPtr);
	}
	));

	Issues.Add(DeliveryApplyItemRandomOptionClass<FB2ResponseApplyItemRandomOptionPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseApplyItemRandomOptionPtr ServerApplyItemOptionPtr)
	{
		this->ResponseApplyItemRandomOption(ServerApplyItemOptionPtr);
	}
	));

	Issues.Add(DeliveryDiscardItemRandomOptionClass<FB2ResponseDiscardItemRandomOptionPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseDiscardItemRandomOptionPtr ServerDiscardItemOptionPtr)
	{
		this->ResponseDiscardItemRandomOptionn(ServerDiscardItemOptionPtr);
	}
	));
}

void UB2UIItemOptionChange::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIItemOptionChange::ResponseGenerateItemRandomOption(FB2ResponseGenerateItemRandomOptionPtr GenerateItemRandomOptionPtr)
{
	TArray<FItemOption> GeneratedOptions;
	if (GenerateItemRandomOptionPtr)
	{
		// DarkCrystal
		int32 CurrentDarkCrystal = GenerateItemRandomOptionPtr->current_dark_crystal;
		BladeIIGameImpl::GetClientDataStore().SetDarkCrystal(CurrentDarkCrystal);

		int32 CurrentCash = GenerateItemRandomOptionPtr->current_cash;
		BladeIIGameImpl::GetClientDataStore().SetGemAmount(CurrentCash);

		ChangeUserAccountInfoClass<>::GetInstance().Signal();

		EItemOption ItemOption1 = SvrToCliOptionType(GenerateItemRandomOptionPtr->random_option_id1);
		EItemOption ItemOption2 = SvrToCliOptionType(GenerateItemRandomOptionPtr->random_option_id2);
		EItemOption ItemOption3 = SvrToCliOptionType(GenerateItemRandomOptionPtr->random_option_id3);

		GeneratedOptions.Add(FItemOption(ItemOption1, GenerateItemRandomOptionPtr->random_option_value1));
		GeneratedOptions.Add(FItemOption(ItemOption2, GenerateItemRandomOptionPtr->random_option_value2));
		GeneratedOptions.Add(FItemOption(ItemOption3, GenerateItemRandomOptionPtr->random_option_value3));

		SetNewOptionDetail(GeneratedOptions);

		if (bEnableApplyOption)
		{
			OnGenerateOtherItemOption_BP();
		}
		else
		{
			bEnableApplyOption = true;
			OnGenerateNewItemOption_BP();
		}

		if (BTN_ApplyOption.IsValid())
		{
			BTN_ApplyOption->SetIsEnabled(bEnableApplyOption);
		}

		RefreshOptionChangeCostColor();

		if (UIP_ChangedOption.IsValid())
		{
			UIP_ChangedOption->OnItemOptionGenerated_BP(NewOptionCount);
		}
	}
}

void UB2UIItemOptionChange::ResponseApplyItemRandomOption(FB2ResponseApplyItemRandomOptionPtr ApplyItemRandomOptionPtr)
{
	if (ApplyItemRandomOptionPtr)
	{
		FB2Item AppliedItem;
		AppliedItem = ApplyItemRandomOptionPtr->item_data;
		BladeIIGameImpl::GetClientDataStore().ItemOptionChange(AppliedItem);

		SetApplyOptionDetail(AppliedItem);

		bEnableApplyOption = false;

		CachedApplyItemOptionForAnimation = AppliedItem;
		// UI갱신 - Original Item Option 갱신
		//SetTargetItem(AppliedItem);

		// UI갱신 - NewOption 삭제
		if (UIP_ChangedOption.IsValid())
			UIP_ChangedOption->ClearOptionTextList();

		bItemOptionApplied = true; // ItemOptionChange UI Close 후 InventoryUI 갱신

								   // Apply되면 Datastore에 저장된 값 삭제해버려
		BladeIIGameImpl::GetClientDataStore().ClearUnappliedItemOptionInfo();

		OnItemOptionApplied_BP();

		// 전투력 갱신
		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(IntToPCClass(AppliedItem.AllowedPCClass), false);
	}
}

void UB2UIItemOptionChange::ResponseDiscardItemRandomOptionn(FB2ResponseDiscardItemRandomOptionPtr DiscardItemRandomOptionPtr)
{
	BladeIIGameImpl::GetClientDataStore().ClearUnappliedItemOptionInfo();
	bEnableApplyOption = false;
}

void UB2UIItemOptionChange::SetTargetItem(const FB2Item& TargetItem)
{
	SetOriginalOptionDetail(TargetItem);
	SetTargetItemIcon(TargetItem);
	RefreshOptionChangeCostColor();

	if (BTN_ApplyOption.IsValid())
	{
		bool bEnableRequestApply = HasRemainUnAppliedOption();
		BTN_ApplyOption->SetIsEnabled(bEnableRequestApply);
	}

	// 재화 부족해도 활성화 처리함.
	if (BTN_ChangeOption.IsValid())
	{
		BTN_ChangeOption->ClearInfo();
	}

	// 재화 부족해도 활성화 처리함.
	if (BTN_ChangeOptionCash.IsValid())
	{
		BTN_ChangeOptionCash->ClearInfo();
	}

	ItemUniqueInstanceID = TargetItem.InstanceUID;
	ItemReferenceID = TargetItem.ItemRefID;
	CachedItem = TargetItem;
}

void UB2UIItemOptionChange::RefreshOptionChangeCostColor()
{
	if (TB_OptionChangeCost.IsValid())
	{
		TB_OptionChangeCost->SetColorAndOpacity(IsEnableButtonOptionChange() ? EnoughColor : NotEnoughColor);
	}

	if (TB_OptionChangeCostCash.IsValid())
	{
		TB_OptionChangeCostCash->SetColorAndOpacity(IsEnableButtonOptionCashChange() ? EnoughColor : NotEnoughColor);
	}
}

void UB2UIItemOptionChange::SetTargetItemIcon(const FB2Item& TargetItem)
{
	if (UIP_ItemIcon.IsValid())
	{
		UIP_ItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow);
		UIP_ItemIcon->UpdateItemData(TargetItem);
	}
}

void UB2UIItemOptionChange::SetOriginalOptionDetail(const FB2Item& TargetItem)
{
	if (UIP_OriginalOption.IsValid())
	{
		const int32 OptionCount = TargetItem.RandomOptions.Num();
		const EPCClass OwnerPCClass = IntToPCClass(TargetItem.AllowedPCClass);	// Server값 아님 -> 바로 Casting 해서 쓰고있음

		ItemOwnerPCClass = OwnerPCClass;
		OriginOption.Empty();

		for (int32 OptionIdx = 0; OptionIdx < OptionCount; OptionIdx++)
		{
			if (TargetItem.RandomOptions.IsValidIndex(OptionIdx))
			{
				const FItemOption& OptionInfo = TargetItem.RandomOptions[OptionIdx];

				OriginOption.Add(OptionInfo);
				// FItemOption 초기값 EItemOption::EIO_End 로 없는 option이 들어가면 옵션 Text를 자동 Collapsed 처리
				UIP_OriginalOption->SetOptionText(OptionIdx, OptionInfo.OptionType, OwnerPCClass, OptionInfo.RawOptionAmount);
			}
		}

		OriginalOptionCount = OptionCount;
	}
}

void UB2UIItemOptionChange::SetApplyOptionDetail(const FB2Item& TargetItem)
{
	if (UIP_ResultOptionDetail.IsValid())
	{
		const int32 OptionCount = TargetItem.RandomOptions.Num();
		const EPCClass OwnerPCClass = IntToPCClass(TargetItem.AllowedPCClass);	// Server값 아님 -> 바로 Casting 해서 쓰고있음

		ItemOwnerPCClass = OwnerPCClass;
		OriginOption.Empty();

		for (int32 OptionIdx = 0; OptionIdx < OptionCount; OptionIdx++)
		{
			if (TargetItem.RandomOptions.IsValidIndex(OptionIdx))
			{
				const FItemOption& OptionInfo = TargetItem.RandomOptions[OptionIdx];

				OriginOption.Add(OptionInfo);
				// FItemOption 초기값 EItemOption::EIO_End 로 없는 option이 들어가면 옵션 Text를 자동 Collapsed 처리
				UIP_ResultOptionDetail->SetOptionText(OptionIdx, OptionInfo.OptionType, OwnerPCClass, OptionInfo.RawOptionAmount);
			}
		}
	}
}

void UB2UIItemOptionChange::SetNewOptionDetail(const TArray<FItemOption>& NewItemOptions)
{
	if (UIP_ChangedOption.IsValid())
	{
		const int32 OptionCount = NewItemOptions.Num();
		NewOptionCount = 0;

		TArray<FItemOption> TempNewOption(NewItemOptions);
		SortOptionArray(TempNewOption);

		for (int32 OptionIdx = 0; OptionIdx < OptionCount; OptionIdx++)
		{
			if (TempNewOption.IsValidIndex(OptionIdx))
			{
				const FItemOption& OptionInfo = TempNewOption[OptionIdx];
				// FItemOption 초기값 EItemOption::EIO_End 로 없는 option이 들어가면 옵션 Text를 자동 Collapsed 처리
				UIP_ChangedOption->SetCompareOptionText(OptionIdx, OptionInfo.OptionType, ItemOwnerPCClass, OptionInfo.RawOptionAmount, OriginOption);
				if (OptionInfo.OptionType != EItemOption::EIO_End)
					NewOptionCount++;
			}
		}
	}
}

void UB2UIItemOptionChange::SortOptionArray(TArray<FItemOption>& OptionArray)
{
	int32 SortOriginOptionCount = OriginOption.Num();
	int32 SortNewOptionCount = OptionArray.Num();

	for (int32 i = 0; i < SortOriginOptionCount; ++i)
	{
		for (int32 k = 0; k < SortNewOptionCount; ++k)
		{
			if (OriginOption.IsValidIndex(i) && OptionArray.IsValidIndex(k))
			{
				if (OriginOption[i].OptionType == OptionArray[k].OptionType && i != k)
				{
					if (OptionArray.IsValidIndex(i) && OptionArray.IsValidIndex(k))
						OptionArray.Swap(i, k);
				}
			}
		}
	}
}

void UB2UIItemOptionChange::VisibleCloseButton()
{
	SetClose(true);
}

void UB2UIItemOptionChange::InvisibleCloseButton()
{
	SetClose(false);
}

void UB2UIItemOptionChange::RequestItemOptionApply(/*...*/)
{
	data_trader::Retailer::GetInstance().RequestApplyItemRandomOption(ItemUniqueInstanceID);
}

void UB2UIItemOptionChange::RequestNewItemOption(int32 InButType)
{
	data_trader::Retailer::GetInstance().RequestGenerateItemRandomOption(ItemUniqueInstanceID, InButType);
}

void UB2UIItemOptionChange::OnClickBTN_ApplyOption()
{
	RequestItemOptionApply();
	bEnableApplyOption = false;
	if (BTN_ApplyOption.IsValid())
		BTN_ApplyOption->SetIsEnabled(bEnableApplyOption);
}

void UB2UIItemOptionChange::OnClickBTN_ChangeOption()
{
	if (IsEnableButtonOptionChange() == false)
	{
		ShortageMGR->PopupGotoShopForDarkCrystalMessage();
	}
	else
	{
		RequestNewItemOption(b2network::B2ShopBuyPriceType::DARK_CRYSTAL);
	}
}

void UB2UIItemOptionChange::OnClickBTN_ChangeOptionCash()
{
	if (IsEnableButtonOptionCashChange() == false)
	{
		ShortageMGR->PopupGoToShopForGemMessage();
	}
	else
	{
		RequestNewItemOption(b2network::B2ShopBuyPriceType::GEM);
	}
}

void UB2UIItemOptionChange::OnClickBTN_Close()
{
	if (HasRemainUnAppliedOption())
	{
		// 변경 후 적용하지 않은 옵션이 있다면

		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_ApplyCancelNotice")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemOptionChange_ApplyCancelDescription")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([this]() {
			data_trader::Retailer::GetInstance().RequestDiscardItemRandomOption(this->ItemUniqueInstanceID);
			UB2UIManager::GetInstance()->CloseUI(UIFName::ItemOptionChange); })
		);

		return;
	}

	UB2UIManager::GetInstance()->CloseUI(UIFName::ItemOptionChange);
}

void UB2UIItemOptionChange::OnClickBTN_OptionInfo()
{
	if (X_MainPopup.IsValid())
		X_MainPopup->SetVisibility(ESlateVisibility::HitTestInvisible);

	CreateLobbyItemAfterOptionPopup(GetOwningPlayer(), CachedApplyItemOptionForAnimation);
}

void UB2UIItemOptionChange::OnItemOptionApplyAnimEnd()
{
	// 애니메이션 연출 후 변경된 옵션은 Reset하고 현재옵션창을 변경된 옵션으로 교체하는 작업
	if (UIP_OriginalOption.IsValid())
		UIP_OriginalOption->OnItemOptionGenerated_BP(OriginalOptionCount);

	SetTargetItem(CachedApplyItemOptionForAnimation);
	CachedApplyItemOptionForAnimation.Empty();
}

void UB2UIItemOptionChange::CreateLobbyItemAfterOptionPopup(UObject* WorldContextObject, FB2Item& InItemData)
{
	// 보관함 아이템 혹은 장착 아이템
	const ELobbySubPopups LobbyPopupEnum = ELobbySubPopups::ELSPU_ItemAfterOpiton;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 해당 user widget 생성 신호.
	// 생성된 Popup widget 을 UIP 처럼 배치해 넣을 패널 정보를 만들어준다.
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(LobbyPopupEnum, FDJLegacySubpopupAddPanelInfo(this, X_AfterOptionPopup.Get()));

	// 생성된 user widget 을 찾아서 ItemData 를 업데이트
	UB2LobbyUI_ItemAfterOption* CreatedDetailWindow = LobbyGM ? Cast<UB2LobbyUI_ItemAfterOption>(LobbyGM->DJLegacy_FindLobbySubPopup(LobbyPopupEnum)) : NULL;

	if (CreatedDetailWindow)
	{
		CreatedDetailWindow->UpdateItemData(CachedItem, true);
		CreatedDetailWindow->ClickCloseDelegate.BindUObject(this, &UB2UIItemOptionChange::CloseAfterOptionPopup);
	}
}

void UB2UIItemOptionChange::CloseAfterOptionPopup()
{
	if (X_MainPopup.IsValid())
		X_MainPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIOptionChangeDetail::CacheAssets()
{
	OptionTextList.AddDefaulted(MAX_ITEM_RANDOM_OPTION);

	GET_SLOT_BYNAME(UB2DynText_Multiple, OptionTextList[0], UIP_OptionText1);
	GET_SLOT_BYNAME(UB2DynText_Multiple, OptionTextList[1], UIP_OptionText2);
	GET_SLOT_BYNAME(UB2DynText_Multiple, OptionTextList[2], UIP_OptionText3);
	GET_SLOT_BYNAME(UB2DynText_Multiple, OptionTextList[3], UIP_OptionText4);

	GET_SLOT(UImage, IMG_Empty);
}

void UB2UIOptionChangeDetail::Init()
{
	Super::Init();

	ClearOptionTextList();
}

void UB2UIOptionChangeDetail::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIOptionChangeDetail::ClearOptionTextList()
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

void UB2UIOptionChangeDetail::SetOptionText(int32 OptionIndex, EItemOption OptionType, EPCClass OwnerClassType, float RawOptionAmount)
{
	if (OptionTextList.IsValidIndex(OptionIndex) && OptionTextList[OptionIndex].IsValid())
	{
		TWeakObjectPtr<UB2DynText_Multiple> OptionTextWidget = OptionTextList[OptionIndex];
		OptionTextWidget->SetVisibility(OptionType != EItemOption::EIO_End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		FText OptionText = GetLOCTextOfItemOption(OptionType, OwnerClassType);
		FText OptionValueText = GetItemOptionValueDisplayText(OptionType, RawOptionAmount, true);

		OptionTextWidget->SetDynText(0, OptionText, OptionValueText);
		OptionTextWidget->SetVisibility_Textbox(2, ESlateVisibility::Collapsed);

		if (IMG_Empty.IsValid())
			IMG_Empty->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIOptionChangeDetail::SetCompareOptionText(int32 OptionIndex, EItemOption OptionType, EPCClass OwnerClassType, float RawOptionAmount, const TMap<EItemOption, float>& CompareOption)
{
	if (OptionTextList.IsValidIndex(OptionIndex) && OptionTextList[OptionIndex].IsValid())
	{
		TWeakObjectPtr<UB2DynText_Multiple> OptionTextWidget = OptionTextList[OptionIndex];
		OptionTextWidget->SetVisibility(OptionType != EItemOption::EIO_End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		FText CompareValueText;

		if (CompareOption.Find(OptionType))
		{
			float CompareValue = CompareOption[OptionType] - RawOptionAmount;
			CompareValueText = GetItemOptionValueDisplayText(OptionType, CompareValue, true);
			// 색깔

		}
		else
		{
			CompareValueText = FText::FromString(FString("New"));
		}

		FText OptionText = GetLOCTextOfItemOption(OptionType, OwnerClassType);
		FText OptionValueText = GetItemOptionValueDisplayText(OptionType, RawOptionAmount, true);

		OptionTextWidget->SetDynText(0, OptionText, OptionValueText, CompareValueText);
		OptionTextWidget->SetVisibility_Textbox(2, ESlateVisibility::Visible);

		if (IMG_Empty.IsValid())
			IMG_Empty->SetVisibility(ESlateVisibility::Hidden);
	}
}

FSlateColor UB2UIOptionChangeDetail::GetCompareTextColor(const float CompareValue, const bool FindOption)
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

FText UB2UIOptionChangeDetail::MakeParenthesesText(const float CompareValue, const FText& CompareText, const bool FindOption)
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

void UB2UIOptionChangeDetail::SetCompareOptionText(int32 OptionIndex, EItemOption OptionType, EPCClass OwnerClassType, float RawOptionAmount, const TArray<FItemOption>& OriginOption)
{
	if (OptionTextList.IsValidIndex(OptionIndex) && OptionTextList[OptionIndex].IsValid())
	{
		TWeakObjectPtr<UB2DynText_Multiple> OptionTextWidget = OptionTextList[OptionIndex];
		OptionTextWidget->SetVisibility(OptionType != EItemOption::EIO_End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		bool FindOption = false;
		float CompareValue = 0.0f;
		FText CompareValueText;

		for (int32 i = 0; i < OriginOption.Num(); ++i)
		{
			if (OriginOption.IsValidIndex(i) && OriginOption[i].OptionType == OptionType)
			{
				CompareValue = RawOptionAmount - OriginOption[i].RawOptionAmount;
				CompareValueText = GetItemOptionValueDisplayText(OptionType, CompareValue, true);
				FindOption = true;
				break;
			}
		}

		if (!FindOption)
			CompareValueText = FText::FromString(FString("New"));

		FText OptionText = GetLOCTextOfItemOption(OptionType, OwnerClassType);
		FText OptionValueText = GetItemOptionValueDisplayText(OptionType, RawOptionAmount, true);
		FSlateColor CompareColor = GetCompareTextColor(CompareValue, FindOption);

		OptionTextWidget->SetDynText(0, OptionText, OptionValueText, MakeParenthesesText(CompareValue, CompareValueText, FindOption));
		OptionTextWidget->SetVisibility_Textbox(2, ESlateVisibility::Visible);
		OptionTextWidget->SetDynTextColor(2, CompareColor);

		if (IMG_Empty.IsValid())
			IMG_Empty->SetVisibility(ESlateVisibility::Hidden);
	}
}
