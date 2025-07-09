// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2WingUIP_PropOptionDetail.h"
#include "B2WingInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "Event.h"
#include "B2ClientDataStore.h"
#include "B2Inventory.h"
#include "B2LobbyInventory.h"
#include "Retailer.h"
#include "B2PCClassInfo.h"
#include "B2Airport.h"
#include "B2LobbyUI_WingMain.h"
#include "B2UIShortagePopupManager.h"
#include "B2ItemInfo.h"


#include "BladeIIGameImpl.h"

const int32 UB2WingUIP_PropOptionDetail::BGWSIndex_Locked = 0;
const int32 UB2WingUIP_PropOptionDetail::BGWSIndex_NormalOpen = 1;
const int32 UB2WingUIP_PropOptionDetail::BGWSIndex_AtMaxLevel = 2;

UB2WingUIP_PropOptionDetail::UB2WingUIP_PropOptionDetail(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedOptionIndex = 0;
}

void UB2WingUIP_PropOptionDetail::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2WingUIP_PropOptionIcon, UIP_PropOptionIcon);
	if (UIP_PropOptionIcon.IsValid())
	{ // 여기선 버튼 눌림이라든지 없도록.
		UIP_PropOptionIcon->SetUsageType(EWingPropOptionUIPUsage::WPOUU_PropOptionEnhance);
	}

	GET_SLOT(UTextBlock, TB_OptionName);
	GET_SLOT(UTextBlock, TB_OptionValue);
	GET_SLOT(UTextBlock, TB_OptionLevel);
	GET_SLOT(UTextBlock, TB_OptionMaxLevelInfo);

	GET_SLOT(UWidgetSwitcher, WS_PerOptionStateSet);

	GET_SLOT(UOverlay, OV_EnhanceBtnSet);
		GET_SLOT(UB2Button, BTN_Enhance);
		GET_SLOT(UImage, IMG_Feather);
		BIND_CLICK_FUNC_TO_BTN(BTN_Enhance, &UB2WingUIP_PropOptionDetail::OnClickedEnhanceBtn);
		GET_SLOT(UTextBlock, TB_DoEnhance);
		GET_SLOT(UTextBlock, TB_RequiredFeatherNum);
		GET_SLOT(UB2Button, BTN_DoEnhance);
		
	GET_SLOT(UOverlay, OV_AtMaxLevelSet);
		GET_SLOT(UTextBlock, TB_AtMaxLevel);

	GET_SLOT(UOverlay, OV_AtNextLevelSet);
		GET_SLOT(UTextBlock, TB_AtNextLevel);
	
	GET_SLOT(UProgressBar, PB_EnhanceProgress);
	GET_SLOT(UTextBlock, TB_EnhanceProgress);

	GET_SLOT(UB2RichTextBlock, TB_UnlockInfo);
}

void UB2WingUIP_PropOptionDetail::SetMyOptionInfo(int32 InMyOptionIndex, const FB2Wing& InWingData, const FWingPropOption& InOptionData)
{
	CachedOptionIndex = InMyOptionIndex;
	CachedWingData = InWingData;
	CachedOptionData = InOptionData;

	if (UIP_PropOptionIcon.IsValid())
	{
		UIP_PropOptionIcon->SetMyOptionInfo(InOptionData);
	}

	UpdateWidgets();
}

void UB2WingUIP_PropOptionDetail::UpdateWidgets()
{
	const bool bAtMaxLevel = (CachedOptionData.OptionLevel >= CachedOptionData.MaxOptionLevel);
	// bAtMaxLevel 이면 아마도 별다른 값을 얻어오지 못할 듯.
	int32 RequiredFeatherNum = 0;
	int32 CurrentFeatherNum = 0;
	const bool bCanAffordEnhance = GetEnhanceRequirementAndFeasibility(RequiredFeatherNum, CurrentFeatherNum);

	if (TB_OptionName.IsValid())
	{
		TB_OptionName->SetText(GetLOCTextOfWingPropOption(CachedOptionData.MyOptionType, IntToPCClass(CachedWingData.OwnerPCClass)));
	}
	if (TB_OptionValue.IsValid())
	{
		TB_OptionValue->SetVisibility(CachedOptionData.bIsOpen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		if (CachedOptionData.bIsOpen)
		{
			TB_OptionValue->SetText(GetItemOptionValueDisplayText(CachedOptionData.MyOptionType, CachedOptionData.RawOptionAmount, true));
		}
	}
	if (TB_OptionLevel.IsValid())
	{
		TB_OptionLevel->SetVisibility(CachedOptionData.bIsOpen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		if (CachedOptionData.bIsOpen)
		{
			TB_OptionLevel->SetText(
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(CachedOptionData.OptionLevel))
				);
		}
	}
	if (TB_OptionMaxLevelInfo.IsValid())
	{
		TB_OptionMaxLevelInfo->SetVisibility(CachedOptionData.bIsOpen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		if (CachedOptionData.bIsOpen)
		{
			TB_OptionMaxLevelInfo->SetText(
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_PropOption_MaxLevelInfo")), FText::AsNumber(CachedOptionData.MaxOptionLevel))
				);
		}
	}

	if (WS_PerOptionStateSet.IsValid())
	{
		if (!CachedOptionData.bIsOpen){
			WS_PerOptionStateSet->SetActiveWidgetIndex(BGWSIndex_Locked);
		}
		else if (bAtMaxLevel){
			WS_PerOptionStateSet->SetActiveWidgetIndex(BGWSIndex_AtMaxLevel);
		}
		else{
			WS_PerOptionStateSet->SetActiveWidgetIndex(BGWSIndex_NormalOpen);
		}
	}

	// OV_EnhanceBtnSet 과 OV_AtMaxLevelSet 은 WS_PerOptionStateSet 의 일부가 될 수도..

	if (OV_EnhanceBtnSet.IsValid())
	{
		OV_EnhanceBtnSet->SetVisibility((!bAtMaxLevel && CachedOptionData.bIsOpen) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (BTN_Enhance.IsValid())
	{// 아마 OV_EnhanceBtnSet 의 일부겠지만 확실히..
		BTN_Enhance->SetVisibility((!bAtMaxLevel && CachedOptionData.bIsOpen) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		if (!bAtMaxLevel && !bCanAffordEnhance)
		{ // 단지 재료 모자라서 강화 못하는 상황이면 disable.. 이 아니고 현질유도 팝업을 띄울 것.
			//BTN_Enhance->SetIsEnabled(false);
		}
	}
	if (IMG_Feather.IsValid())
	{
		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
		FSingleItemInfoData* AngelFeatherData = ItemInfoTable ? ItemInfoTable->GetInfoData(FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID()) : NULL;
		if (AngelFeatherData)
		{
			IMG_Feather->SetBrushFromMaterial(AngelFeatherData->GetIconMaterial(ItemInfoTable));
		}
	}

	if (bAtMaxLevel)
	{
		if (TB_AtMaxLevel.IsValid())
		{
			TB_AtMaxLevel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_PropOption_MaxLevel")));
		}
		if (TB_AtMaxLevel.IsValid())
		{
			TB_AtNextLevel->SetText(
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_attribute_OpenConditionDesc"))
				, FText::AsNumber(CachedWingData.EvolutionGrade + 1)
				, FText::AsNumber(CachedOptionData.MaxOptionLevel + 10)));
		}
		if (OV_AtNextLevelSet.IsValid())
		{
			if (CachedWingData.EvolutionGrade < MAX_WING_EVOLUTION_GRADE && CachedOptionData.bIsOpen)
				OV_AtNextLevelSet->SetVisibility(ESlateVisibility::Visible);
			else
				OV_AtNextLevelSet->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (OV_AtMaxLevelSet.IsValid())
		{
			if (CachedWingData.EvolutionGrade >= MAX_WING_EVOLUTION_GRADE && CachedOptionData.bIsOpen)
			//OV_AtMaxLevelSet->SetVisibility((bAtMaxLevel && CachedOptionData.bIsOpen) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
				OV_AtMaxLevelSet->SetVisibility(ESlateVisibility::Visible);
			else
				OV_AtMaxLevelSet->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		if (TB_DoEnhance.IsValid())
		{
			TB_DoEnhance->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_PropOption_DoEnhance")));
		}
		if (TB_RequiredFeatherNum.IsValid())
		{
			TB_RequiredFeatherNum->SetText(FText::AsNumber(RequiredFeatherNum));
			// 재료 모자란 경우 빨간색으로 하는데 일단 색상은 뻔해 보이니 하드코딩..
			TB_RequiredFeatherNum->SetColorAndOpacity(bCanAffordEnhance ? FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)) : FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)));
		}
		if (OV_AtMaxLevelSet.IsValid())
		{
			OV_AtMaxLevelSet->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (OV_AtNextLevelSet.IsValid())
		{
			OV_AtNextLevelSet->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (PB_EnhanceProgress.IsValid())
		{
			PB_EnhanceProgress->SetPercent((float)CachedOptionData.OptionLevel / (float)CachedOptionData.MaxOptionLevel);
		}
		if (TB_EnhanceProgress.IsValid())
		{
			TB_EnhanceProgress->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CachedOptionData.OptionLevel, CachedOptionData.MaxOptionLevel)));
		}
	}

	if (TB_UnlockInfo.IsValid())
	{
		TB_UnlockInfo->SetVisibility(CachedOptionData.bIsOpen ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		if (!CachedOptionData.bIsOpen)
		{
			TB_UnlockInfo->SetText(
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_PropOption_OpenInfo")), FText::AsNumber(CachedOptionData.RequiredWingEvolveGrade))
			);
		}
	}
}

bool UB2WingUIP_PropOptionDetail::GetEnhanceRequirementAndFeasibility(int32& OutRequiredFeatherNum, int32& OutCurrentFeatherNum)
{
	// CachedWingData 와 CachedOptionIndex 는 업데이트 된 상태여야
	const FWingMD_PropOptionSingleElem* PropOptionMD = BladeIIGameImpl::GetClientDataStore().GetWingMD_PropOption(CachedWingData, CachedOptionIndex);
	if (PropOptionMD){
		OutRequiredFeatherNum = PropOptionMD->RequiredItemCount;
	}
	OutCurrentFeatherNum = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID());
	return (OutCurrentFeatherNum >= OutRequiredFeatherNum); // 마지막으로 가능 여부 리턴
}

void UB2WingUIP_PropOptionDetail::OnClickedEnhanceBtn()
{
	if (!UB2Airport::HasEnoughGoldForWingOptionEnhanceLocalPlayer(IntToPCClass(CachedWingData.OwnerPCClass), CachedOptionIndex)){
		// 속성 옵션 강화에 돈이 필요없는 한 여기로 올 일은 없을 것..
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else if (!UB2Airport::HasEnoughIngredForWingOptionEnhanceLocalPlayer(IntToPCClass(CachedWingData.OwnerPCClass), CachedOptionIndex)){
		// 재료 획득 경로 팝업.
		OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_PropOptionIngred);
	}
	else
	{
		int32 SvrOwnerCharType = CliToSvrPCClassType(IntToPCClass(CachedWingData.OwnerPCClass));
		int32 SvrOptionIndex = CliToSvrWingPropOptionIndex(CachedOptionIndex);
		data_trader::Retailer::GetInstance().RequestEnhanceWingOption(SvrOwnerCharType, SvrOptionIndex);
		PlaySuccessAnim_BP();
	}
}