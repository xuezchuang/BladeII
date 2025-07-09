// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UI_TotemInfoPopup.h"

#include "B2UIManager.h"
#include "B2Inventory.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2LobbyUI_TotemInventorySlot.h"
#include "B2LobbyUI_SellItemPopup.h"
#include "B2UI_TotemSmeltingBonus.h"
#include "B2UI_TotemSmeltingPopup.h"
#include "B2UIDecompositionPopup.h"
#include "B2DynText_Multiple.h"
#include "B2CombatStatEvaluator.h"
#include "B2AndroidBackManager.h"
#include "Retailer.h"

#include "BladeIIGameImpl.h"

void UB2UI_TotemInfoPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	if (UIP_TotemInventorySlot.IsValid())
	{
		UIP_TotemInventorySlot->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UI_TotemInfoPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel,					CP_PopupBackground);
	GET_SLOT(UCanvasPanel,					CP_PopupMain);

	GET_SLOT(UB2Button,						BTN_Close);

	GET_SLOT(UB2RichTextBlock,				RTB_Combat);
	GET_SLOT(UB2RichTextBlock,				RTB_CombatPower);
	GET_SLOT(UB2RichTextBlock,				RTB_CombatPowerDelta);

	GET_SLOT(UB2RichTextBlock,				RTB_PrimaryPointType);
	GET_SLOT(UB2RichTextBlock,				RTB_PrimaryPoint);
	GET_SLOT(UB2RichTextBlock,				RTB_PrimaryPointDelta);

	GET_SLOT(UB2RichTextBlock,				RTB_SmeltingSuccess);
	GET_SLOT(UB2RichTextBlock,				RTB_SmeltingLevel);

	GET_SLOT(UB2Button,						BTN_Smelting);
	GET_SLOT(UB2RichTextBlock,				RTB_Smelting);
 
	GET_SLOT(UB2Button,						BTN_Decomposition);
	GET_SLOT(UB2RichTextBlock,				RTB_Decomposition);

	GET_SLOT(UB2Button,						BTN_Sell);
	GET_SLOT(UB2RichTextBlock,				RTB_Sell);

	GET_SLOT(UB2Button,						BTN_EquipOrRelease);
	GET_SLOT(UB2RichTextBlock,				RTB_EquipOrRelease);

	GET_SLOT(UB2Button,						BTN_ItemLock);
	GET_SLOT(UImage,						IMG_LockButtonLocked);
	GET_SLOT(UImage,						IMG_LockButtonUnLocked);

	GET_SLOT(UB2LobbyUI_TotemInventorySlot,	UIP_TotemInventorySlot);
	if (UIP_TotemInventorySlot.IsValid())
	{
		UIP_TotemInventorySlot->Init();
	}

	GET_SLOT(UB2RichTextBlock,				RTB_OptionTitle);
	GET_SLOT(UVerticalBox,					VB_OptionList);

	GET_SLOT(UVerticalBox,					VB_OptionsDisplayPanel);
	GET_SLOT(UVerticalBox,					VB_SmeltingBonus);
}

void UB2UI_TotemInfoPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close,			&UB2UI_TotemInfoPopup::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_Smelting,		&UB2UI_TotemInfoPopup::OnClickBTN_Smelting);
	BIND_CLICK_FUNC_TO_BTN(BTN_Decomposition,	&UB2UI_TotemInfoPopup::OnClickBTN_Decomposition);
	BIND_CLICK_FUNC_TO_BTN(BTN_Sell,			&UB2UI_TotemInfoPopup::OnClickBTN_Sell);
	BIND_CLICK_FUNC_TO_BTN(BTN_EquipOrRelease,	&UB2UI_TotemInfoPopup::OnClickBTN_EquipOrRelease);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemLock,		&UB2UI_TotemInfoPopup::OnClickBTN_ItemLock);
}

void UB2UI_TotemInfoPopup::UpdateStaticText()
{
	if (RTB_Combat.IsValid())
	{
		RTB_Combat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
	}

	if (RTB_SmeltingSuccess.IsValid())
	{
		RTB_SmeltingSuccess->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_CompleteCount")));
	}

	if (RTB_Smelting.IsValid())
	{
		RTB_Smelting->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_smelt")));
	}

	if (RTB_Decomposition.IsValid())
	{
		RTB_Decomposition->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Decomposition")));
	}

	if (RTB_Sell.IsValid())
	{
		RTB_Sell->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Sell")));
	}

	if (RTB_OptionTitle.IsValid())
	{
		RTB_OptionTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyItemDetail_RandOptionPanelTitle")));
	}
}

void UB2UI_TotemInfoPopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UI_TotemInfoPopup::OnClickBTN_Close()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UI_TotemInfoPopup::OnClickBTN_Smelting()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UI_TotemInfoPopup::OnDelaySmeltingCallback, 0.1f, false);
}

void UB2UI_TotemInfoPopup::OnClickBTN_Decomposition()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UI_TotemInfoPopup::OnDelayDecompositionCallback, 0.1f, false);
}

void UB2UI_TotemInfoPopup::OnClickBTN_Sell()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UI_TotemInfoPopup::OnDelaySellCallback, 0.1f, false);
}

void UB2UI_TotemInfoPopup::OnClickBTN_EquipOrRelease()
{
	if (CachedB2TotemInfo.IsSmeltingComplete() == false)
	{
		PlayEquipOrReleaseSound(!CachedB2TotemInfo.IsEquipped());
		ABladeIIGameMode::SetHUDCenterMessage(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Totem_EquipNote")), 2.0f);
		return;
	}

	if (CachedB2TotemInfo.IsEquipped())
	{
		if (BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().IsOverTotemInventory())
		{
			NoticeErrorPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EquipErrorInvenFull")));
		}
		else
		{
			PlayEquipOrReleaseSound(!CachedB2TotemInfo.IsEquipped());
			RequestUnEquipTotem();
		}
	}
	else
	{
		PlayEquipOrReleaseSound(!CachedB2TotemInfo.IsEquipped());
		RequestEquipTotem();
	}
}

void UB2UI_TotemInfoPopup::OnClickBTN_ItemLock()
{
	PlayLockOrUnlockSound(!CachedB2TotemInfo.IsLock);

	LobbyReqToggleTotemLockClass<int64, bool>::GetInstance().Signal(CachedB2TotemInfo.ID, !CachedB2TotemInfo.IsLock);
}

void UB2UI_TotemInfoPopup::InitTotemInfoPopup(const FVector2D& v2PopupPosition, const float& fPopupSizeX, const FB2Totem& InTotem)
{
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	CachedPCClass = LobbyGM->GetHeroMgmtModePCClass();

	CachedB2TotemInfo = InTotem;

	if (CP_PopupMain.IsValid())
	{
		CP_PopupMain->SetRenderTranslation(v2PopupPosition);

		if (UCanvasPanelSlot* pCanvasPanelSlot = Cast<UCanvasPanelSlot>(CP_PopupMain->Slot))
		{
			pCanvasPanelSlot->SetOffsets(FMargin(0, fTopBezel_BP, fPopupSizeX, 0));
		}
	}

	SubscribeEvents();
	UpdateTotemInfo(InTotem);
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

UPanelWidget* UB2UI_TotemInfoPopup::GetPopupBackGroundPanel()
{
	if (CP_PopupBackground.IsValid())
	{
		return CP_PopupBackground.Get();
	}

	return nullptr;
}

void UB2UI_TotemInfoPopup::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(OpenTotemSmeltingInfoPopupClass<int64>::GetInstance().Subscribe2(
		[this](int64 InstanceID)
	{
		if (this->CachedB2TotemInfo.ID == InstanceID)
		{
			this->OpenSmeltingPopup();
		}
	}
	));

	Issues.Add(UpdateTotemInfoPopupControlClass<FB2Totem>::GetInstance().Subscribe2(
		[this](FB2Totem TotemInfo)
	{
		if (this->CachedB2TotemInfo.ID == TotemInfo.ID)
		{
			CachedB2TotemInfo = TotemInfo;
			this->UpdateTotemInfo(TotemInfo);
		}
	}
	));

	Issues.Add(DeliveryEquipTotemsClass<FB2EquipTotemsPtr>::GetInstance().Subscribe2(
		[this](const FB2EquipTotemsPtr& EquipTotemsPtr)
	{
		this->ResponseEquipTotems(EquipTotemsPtr);
	}
	));

	Issues.Add(DeliveryUnequipTotemsClass<FB2UnequipTotemsPtr>::GetInstance().Subscribe2(
		[this](const FB2UnequipTotemsPtr& UnequipTotemsPtr)
	{
		this->ResponseUnequipTotems(UnequipTotemsPtr);
	}
	));
}

void UB2UI_TotemInfoPopup::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UI_TotemInfoPopup::UpdateTotemInfo(const FB2Totem& InTotem)
{
	FB2Totem* pEquippedTotemAtSamePlace = nullptr;
	
	FB2Totem EquippedTotemAtPlace;

	if (BladeIIGameImpl::GetLocalCharacterData().GetEquuipedTotemByEquipSlot(CachedPCClass, 
		BladeIIGameImpl::GetTotemDataStore().GetTotemEquipSlot(InTotem.RefID), EquippedTotemAtPlace))
	{
		pEquippedTotemAtSamePlace = &EquippedTotemAtPlace;
	}

	UpdateCombatPower(InTotem, pEquippedTotemAtSamePlace);
	UpdatePrimaryPoint(InTotem, pEquippedTotemAtSamePlace);
	UpdateSmeltingLevel(InTotem);
	UpdateTotemInfoSlot(InTotem);
	UpdateTotemInfoOptionList(InTotem.SubOptionDetails);
	UpdateTotemSmeltingBonus(InTotem);
	UpdateEquipOrReleaseBtn(InTotem);
	UpdateWidgetVisibleState(InTotem);
}

void UB2UI_TotemInfoPopup::UpdateTotemInfoSlot(const FB2Totem& InTotem)
{
	if (UIP_TotemInventorySlot.IsValid())
	{
		UIP_TotemInventorySlot->UpdateTotemSlot_FromFB2Totem(InTotem);
	}
}

void UB2UI_TotemInfoPopup::UpdateTotemInfoOptionList(const TArray<FB2TotemSubOptionDetail>& ArrayItemOption)
{
	if (VB_OptionList.IsValid() == false)
	{
		return;
	}

	VB_OptionList->ClearChildren();

	const int32 iArrayOptionMax = ArrayItemOption.Num();

	if (iArrayOptionMax <= 0)
	{
		return;
	}

	for (int32 i = 0; i < iArrayOptionMax; ++i)
	{
		FText OptionText = (IsItemOptionPerPCClass(ArrayItemOption[i].SubOption.OptionType) ?
			BladeIIGameImpl::GetTotemDataStore().GetLOCTextOfSkillOption(ArrayItemOption[i].SubOption.OptionType) :
			GetLOCTextOfItemOption(ArrayItemOption[i].SubOption.OptionType, EPCClass::EPC_Fighter));

		if (CachedB2TotemInfo.IsSmeltingComplete())
		{
			FText OptionValueFormat = FText::FromString(TEXT("{0}{1}"));
			FText OptionValueText = FText::Format(OptionValueFormat, 
				SubOptionType.IsValidIndex(i) ? GetSubOptionTypeText(SubOptionType[i]) : FText::FromString(TEXT("")),
				GetItemOptionValueDisplayText(ArrayItemOption[i].SubOption.OptionType, CachedB2TotemInfo.SubOptionDetails[i].SubOption.RawOptionAmount, true));

			UB2DynText_Multiple* pTotemDetailOption = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(UITotemOptionClass, this, VB_OptionList.Get(), FMargin()));
			if (pTotemDetailOption)
			{
				pTotemDetailOption->SetIndexIcon(i);
				pTotemDetailOption->SetDynText(i, OptionText, FText::FromString(TEXT("")), OptionValueText);
				int32 OptionValueTextBlockIndex = 2;
				pTotemDetailOption->SetDynTextColor(OptionValueTextBlockIndex, OptionValueColor);
			}
		}
		else
		{
			FText OptionRangeFormat = FText::FromString(TEXT("[{0} ~ {1}{2}]"));
			float MinOptionValue, MaxOptionValue;
			BladeIIGameImpl::GetTotemDataStore().GetTotemSubOptionMinMax(CachedB2TotemInfo.Grade, CachedB2TotemInfo.SubOptionDetails[i].SubOption.OptionType, MinOptionValue, MaxOptionValue);
			FText OptionRangeText = FText::Format(OptionRangeFormat, 
				MinOptionValue, 
				SubOptionType.IsValidIndex(i) ? GetSubOptionTypeText(SubOptionType[i]) : FText::FromString(TEXT("{0}")),
				GetItemOptionValueDisplayText(ArrayItemOption[i].SubOption.OptionType, MaxOptionValue, true));

			FB2TotemInfo TotemInfo;
			int32 OptionChance = 0;
			if (BladeIIGameImpl::GetTotemDataStore().GetTotemInfo(CachedB2TotemInfo.RefID, TotemInfo))
			{
				OptionChance = TotemInfo.RefineLimitCounts[i];
			}
			FText OptionChanceFormat = FText::FromString(TEXT("({0})"));
			FText OptionChanceText = FText::Format(OptionChanceFormat, FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_ChanceCount")), OptionChance));

			UB2DynText_Multiple* pTotemDetailOption = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(UITotemOptionClass, this, VB_OptionList.Get(), FMargin()));
			if (pTotemDetailOption)
			{
				pTotemDetailOption->SetIndexIcon(i);
				pTotemDetailOption->SetDynText(i, OptionText, OptionRangeText, OptionChanceText);
				int32 OptionChanceTextBlockIndex = 2;
				pTotemDetailOption->SetDynTextColor(OptionChanceTextBlockIndex, OptionChanceColor);
			}
		}
	}
}

void UB2UI_TotemInfoPopup::UpdateTotemSmeltingBonus(const FB2Totem& InTotem)
{
	if (VB_SmeltingBonus.IsValid() == false)
	{
		return;
	}

	VB_SmeltingBonus->ClearChildren();

	UB2UI_TotemSmeltingBonus* pSmeltingBonus = Cast<UB2UI_TotemSmeltingBonus>(DynCreateInVerticalBox(UITotemSmeltingBonusClass, this, VB_SmeltingBonus.Get(), FMargin()));
	if (pSmeltingBonus)
	{
		pSmeltingBonus->InitTotemSmeltingBonus(InTotem);
	}
}

void UB2UI_TotemInfoPopup::UpdateEquipOrReleaseBtn(const FB2Totem& InTotem)
{
	if (RTB_EquipOrRelease.IsValid() == false || BTN_EquipOrRelease.IsValid() == false)
	{
		return;
	}

	if (InTotem.IsEquipped())
	{
		RTB_EquipOrRelease->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_UnEquip")));
	}
	else
	{
		RTB_EquipOrRelease->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Equiped_Preset")));
	}
}

void UB2UI_TotemInfoPopup::UpdateWidgetVisibleState(const FB2Totem& InTotem)
{
	if (BTN_Smelting.IsValid())
	{
		BTN_Smelting->SetVisibility(InTotem.IsEquipped() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	if (BTN_Decomposition.IsValid())
	{
		BTN_Decomposition->SetVisibility(InTotem.IsEquipped() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	if (BTN_Sell.IsValid())
	{
		BTN_Sell->SetVisibility(InTotem.IsEquipped() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	if (IMG_LockButtonLocked.IsValid())
	{
		IMG_LockButtonLocked->SetVisibility(InTotem.IsLock ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IMG_LockButtonUnLocked.IsValid())
	{
		IMG_LockButtonUnLocked->SetVisibility(InTotem.IsLock ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UI_TotemInfoPopup::UpdateCombatPower(const FB2Totem& InTotem, const FB2Totem* pEquippedTotemAtSamePlace)
{
	// 제련 완료되지 않은 토템의 경우 서버에서 준 토템 값이 아니라 메인Option 을 통해 계산된 전투력 만을 표기한다.
	const int32 CambatPower = InTotem.IsSmeltingComplete() ? InTotem.Power :
		(int32)(BladeIIGameImpl::GetClientDataStore().GetOptionWeight(InTotem.MainOptionDetail.OptionType) * InTotem.MainOptionDetail.RawOptionAmount);

	if (RTB_CombatPower.IsValid())
	{
		RTB_CombatPower->SetText(FText::AsNumber(CambatPower));
	}

	if (RTB_CombatPowerDelta.IsValid())
	{
		int32 iDeltaValue = 0;
		bool IsVisible = !CachedB2TotemInfo.IsEquipped();
		if (pEquippedTotemAtSamePlace)
		{
			iDeltaValue = static_cast<int32>(CambatPower - pEquippedTotemAtSamePlace->Power);
		}

		SetDeltaText(RTB_CombatPowerDelta, iDeltaValue, IsVisible);
	}
}

void UB2UI_TotemInfoPopup::UpdatePrimaryPoint(const FB2Totem& InTotem, const FB2Totem* pEquippedTotemAtSamePlace)
{
	EItemPrimaryPointType PrimaryPointType = CombatStatEval::ConvertItemOptionToItemPrimaryPointType(InTotem.MainOptionDetail.OptionType);

	if (PrimaryPointType == EItemPrimaryPointType::EIPP_End)
	{
		return;
	}

	if (RTB_PrimaryPointType.IsValid())
	{
		RTB_PrimaryPointType->SetText(GetLOCTextOfPrimPointType(PrimaryPointType));
	}

	if (RTB_PrimaryPoint.IsValid())
	{
		const int32 iPrimaryPoint = static_cast<int32>(FMath::FloorToFloat(InTotem.MainOptionDetail.RawOptionAmount));
		RTB_PrimaryPoint->SetText(FText::AsNumber(iPrimaryPoint));
	}

	if (RTB_PrimaryPointDelta.IsValid())
	{
		int32 iDeltaValue = 0;
		bool IsVisible = !CachedB2TotemInfo.IsEquipped();
		if (pEquippedTotemAtSamePlace)
		{
			iDeltaValue = static_cast<int32>(FMath::FloorToFloat(InTotem.MainOptionDetail.RawOptionAmount) - FMath::FloorToFloat(pEquippedTotemAtSamePlace->MainOptionDetail.RawOptionAmount));
		}
		SetDeltaText(RTB_PrimaryPointDelta, iDeltaValue, IsVisible);
	}
}

void UB2UI_TotemInfoPopup::UpdateSmeltingLevel(const FB2Totem& InTotem)
{
	if(RTB_SmeltingLevel.IsValid())
	{
		FText SmeltingLevelFormat = FText::FromString(TEXT("+{0}"));
		RTB_SmeltingLevel->SetText(FText::Format(SmeltingLevelFormat, InTotem.RefineLevel));
	}
}

void UB2UI_TotemInfoPopup::OpenSmeltingPopup()
{
	UB2UI_TotemSmeltingPopup* pTotemSmeltingPopup = nullptr;

	pTotemSmeltingPopup = UB2UIManager::GetInstance()->OpenUI<UB2UI_TotemSmeltingPopup>(UIFName::TotemSmeltingPopup);
	if (pTotemSmeltingPopup)
	{
		pTotemSmeltingPopup->InitTotemSmeltingPopup(CachedB2TotemInfo);
	}
}

void UB2UI_TotemInfoPopup::OnDelaySmeltingCallback()
{
	EnterTotemSmeltingModeClass<int64>::GetInstance().Signal(CachedB2TotemInfo.ID);
}

void UB2UI_TotemInfoPopup::OnDelayDecompositionCallback()
{
	if (CachedB2TotemInfo.IsLock)
	{
		AskForUnlockItemPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Decomposition")));
	}
	else
	{
		const int32 DementionPieceCount = BladeIIGameImpl::GetTotemDataStore().GetDismantlePieceCount(CachedB2TotemInfo);

		if (UB2UIDecompositionPopup* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIDecompositionPopup>(UIFName::DecompositionPopup))
		{
			Popup->SetTotemInfo(DementionPieceCount,
				FOnClickPositive::CreateLambda([this]() {
				this->Decomposition();
			}));
		}
	}
}

void UB2UI_TotemInfoPopup::OnDelaySellCallback()
{
	if (CachedB2TotemInfo.IsLock)
	{
		AskForUnlockItemPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_UnlockAskingItemAct_Sell")));
	}
	else
	{
		FDJLegacySubpopupAddPanelInfo SubPopupAddPanelInfo = GetDJLegacySubpopupInfo();

		// 바로 판매는 아니고 판매여부를 묻는 (특수) 팝업. 좀 구닥다리 방식으로 ㅋ
		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_SellItemPopup, SubPopupAddPanelInfo);

		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
		if (LobbyGM)
		{
			UB2LobbyUI_SellItemPopup* SellItemPopup = Cast<UB2LobbyUI_SellItemPopup>(LobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_SellItemPopup));
			if (SellItemPopup)
			{
				// PopupMain 에 아이템이 아닌 토템이 추가되었을 경우도 추가한다.
				// 토템 정보, 팔기 눌렀을때, 취소 눌렀을때
				SellItemPopup->PopupMain(CachedB2TotemInfo,
					FSellItemPopupOnClick::CreateLambda([this]() {
					Sell();
					this->OnClickBTN_Close(); // 팔아치웠으니 이 창도 닫는다.
				}),
					FSellItemPopupOnClick::CreateLambda([this]() {})
					);
			}
		}
	}
}

void UB2UI_TotemInfoPopup::SetDeltaText(TWeakObjectPtr<UB2RichTextBlock> pRTB, const int32& iDeltaValue, const bool& IsVisible)
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

FDJLegacySubpopupAddPanelInfo UB2UI_TotemInfoPopup::GetDJLegacySubpopupInfo()
{
	FDJLegacySubpopupAddPanelInfo ResultSubpopupAddPanelInfo;

	UB2UI_TotemInfoPopup* pEquippedTotemInfo = UB2UIManager::GetInstance()->GetUI<UB2UI_TotemInfoPopup>(UIFName::TotemInfoPopupLeft);
	if (pEquippedTotemInfo)
	{
		UPanelWidget* PanelWidget = pEquippedTotemInfo->GetPopupBackGroundPanel();
		if (PanelWidget)
		{
			ResultSubpopupAddPanelInfo = FDJLegacySubpopupAddPanelInfo(pEquippedTotemInfo, PanelWidget);
		}
	}

	return ResultSubpopupAddPanelInfo;
}

void UB2UI_TotemInfoPopup::RequestEquipTotem()
{
	TArray<int64> ReqIDs;
	ReqIDs.Add(CachedB2TotemInfo.ID);
	data_trader::Retailer::GetInstance().RequestEquipTotems(ReqIDs, CliToSvrPCClassType(CachedPCClass));
}

void UB2UI_TotemInfoPopup::RequestUnEquipTotem()
{
	TArray<int64> ReqIDs;
	ReqIDs.Add(CachedB2TotemInfo.ID);
	data_trader::Retailer::GetInstance().RequestUnequipTotems(ReqIDs);
}

void UB2UI_TotemInfoPopup::ResponseEquipTotems(const FB2EquipTotemsPtr& EquipTotemsPtr)
{
	if (EquipTotemsPtr == nullptr)
	{
		return;
	}

	ETotemEquipSlot TargetTotemEquipSlot = BladeIIGameImpl::GetTotemDataStore().GetTotemEquipSlot(CachedB2TotemInfo.RefID);

	BladeIIGameImpl::GetClientDataStore().EquipTotems(EquipTotemsPtr, CachedPCClass, TargetTotemEquipSlot);
	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(CachedPCClass, true);
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	OnClickBTN_Close();
}

void UB2UI_TotemInfoPopup::ResponseUnequipTotems(const FB2UnequipTotemsPtr& UnequipTotemsPtr)
{
	if (UnequipTotemsPtr == nullptr)
	{
		return;
	}

	BladeIIGameImpl::GetClientDataStore().UnequipTotems(UnequipTotemsPtr, CachedPCClass);
	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(CachedPCClass, true);
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	OnClickBTN_Close();
}

void UB2UI_TotemInfoPopup::Sell()
{
	LobbyReqSellItemClass<int64, bool>::GetInstance().Signal(CachedB2TotemInfo.ID, true);
}

void UB2UI_TotemInfoPopup::Decomposition()
{
	this->DecompositionEquipTotemEventID = DeliveryDismantleTotemsClass<FB2DismantleTotemsPtr>::GetInstance().Subscribe(
		[this](const FB2DismantleTotemsPtr TotemPtr)
	{
		UB2UIManager::GetInstance()->CloseUI(UIFName::DecompositionPopup);
		this->OnClickBTN_Close();
		this->UnsubscribeDecomposition();
	});

	TArray<int64> ReqIDs;
	ReqIDs.Add(CachedB2TotemInfo.ID);

	LobbyReqDecompositionItemsClass<const TArray<int64>&, bool>::GetInstance().Signal(ReqIDs, true);
}

void UB2UI_TotemInfoPopup::UnsubscribeDecomposition()
{
	if (DecompositionEquipTotemEventID != 0)
	{
		DeliveryDismantleTotemsClass<FB2DismantleTotemsPtr>::GetInstance().Unsubscribe(DecompositionEquipTotemEventID);
	}
}

FText UB2UI_TotemInfoPopup::GetSubOptionTypeText(ETotemSubOptionType InSubOptionType)
{
	if (InSubOptionType == ETotemSubOptionType::OPTION_PLUS)
	{
		return FText::FromString(TEXT("+"));
	}
	else if (InSubOptionType == ETotemSubOptionType::OPTION_MINUS)
	{
		return  FText::FromString(TEXT("-"));
	}
	else
	{
		return  FText::FromString(TEXT(""));
	}
}

void UB2UI_TotemInfoPopup::NoticeErrorPopupCommon(const FText& InAskingText)
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

void UB2UI_TotemInfoPopup::AskForUnlockItemPopupCommon(const FText& InAskingItemActText)
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_AskForUnlockForSomeItemAct")), InAskingItemActText),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
		LobbyReqToggleTotemLockClass<int64, bool>::GetInstance().Signal(CachedB2TotemInfo.ID, !CachedB2TotemInfo.IsLock);
	})
	);
}