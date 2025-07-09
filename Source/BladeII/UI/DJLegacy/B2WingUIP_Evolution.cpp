// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2WingUIP_Evolution.h"
#include "B2Airport.h"
#include "B2WingInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2ItemInfo.h"
#include "B2Inventory.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2ClientDataStore.h"


#include "BladeIIGameImpl.h"

UB2WingUIP_Evolution::UB2WingUIP_Evolution(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EvolutionFailMessageShowTime = 1.5f;
}

void UB2WingUIP_Evolution::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_EvolutionTitle);

	GET_SLOT(UImage, IMG_EvolveIngred);
	GET_SLOT(UTextBlock, TB_CurrentIngredNum);
	GET_SLOT(UTextBlock, TB_RequiredIngredNum);

	GET_SLOT(UOverlay, OV_NormalEvolveMenuSet);
		GET_SLOT(UB2WingUIP_EvolveGradeStar, UIP_EvolveGradeStar);
		GET_SLOT(UTextBlock, TB_EvolveSuccessRateLabel);
		GET_SLOT(UTextBlock, TB_EvolveSuccessRate);
		GET_SLOT(UProgressBar, PB_EvolvePoint);
		GET_SLOT(UTextBlock, TB_EvolvePointLabel);
		GET_SLOT(UTextBlock, TB_EvolveFailMessage);
		GET_SLOT(UB2RichTextBlock, TB_EvolveNote);

		GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_EvolutionGoodInfo);

		if (TB_EvolveFailMessage.IsValid())
		{ // 이건 일단 숨기고 이벤트 상황에서 표시
			TB_EvolveFailMessage->SetVisibility(ESlateVisibility::Collapsed);
		}

	GET_SLOT(UOverlay, OV_MaxEvolvedSet);
		GET_SLOT(UB2WingUIP_EvolveGradeStar, UIP_EvolveGradeStarAtMax);
		GET_SLOT(UB2RichTextBlock, TB_MaxEvolveNote);
}

void UB2WingUIP_Evolution::InitFromWingMain()
{
	UpdateStaticWidgets();
}

void UB2WingUIP_Evolution::UpdateByWingData(const FB2Wing& InWingData)
{
	const bool bShouldShowMaxEvolveSet = IsUltimatelyEvolvedWing(InWingData);

	// 최대 진화레벨 도달 여부에 따라 크게 보여지는 세트가 달라짐.
	if (OV_NormalEvolveMenuSet.IsValid())
	{
		OV_NormalEvolveMenuSet->SetVisibility(bShouldShowMaxEvolveSet ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	if (OV_MaxEvolvedSet.IsValid())
	{
		OV_MaxEvolvedSet->SetVisibility(bShouldShowMaxEvolveSet ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	// 일반진화메뉴 / 최대진화메뉴 각각에 따른 세부 업데이트
	if (bShouldShowMaxEvolveSet)
	{
		if (UIP_EvolveGradeStarAtMax.IsValid())
		{
			UIP_EvolveGradeStarAtMax->SetCurrentGrade(InWingData.EvolutionGrade);
		}
	}
	else
	{
		int32 CurrentEvolveIngredNum = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingEvolveResourceRefID());
		int32 RequiredItemCount = 0;
		int32 RequiredGoldCost = 0;
		float SuccessRate = 0.0f;
		int32 MaxAllowedFailPoint = 0;
		BladeIIGameImpl::GetClientDataStore().GetWingEvolveRefData(InWingData, RequiredItemCount, RequiredGoldCost, SuccessRate, MaxAllowedFailPoint);

		if (TB_CurrentIngredNum.IsValid())
		{
			TB_CurrentIngredNum->SetText(FText::AsNumber(CurrentEvolveIngredNum));
			TB_CurrentIngredNum->SetColorAndOpacity( // 컬러값 걍 하드코딩.. 굳이..
				(CurrentEvolveIngredNum >= RequiredItemCount) ? FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)) : FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))
				);
		}
		if (TB_RequiredIngredNum.IsValid())
		{
			TB_RequiredIngredNum->SetText(FText::AsNumber(RequiredItemCount));
		}

		if (UIP_EvolveGradeStar.IsValid())
		{ // 현 진화등급과 함께 다음 가능한 진화등급을 같이 보여준다.
			UIP_EvolveGradeStar->SetCurrentGrade(InWingData.EvolutionGrade, true);
		}
		if (TB_EvolveSuccessRate.IsValid())
		{ // 만분율로 들어오는 만큼 일단 소수 둘째자리로 표시해 보고.. 아니라 하면 int 로 바꾸고..
			TB_EvolveSuccessRate->SetText(FText::FromString(FString::Printf(TEXT("%.2f %%"), SuccessRate * 100.0f)));
		}
		if (PB_EvolvePoint.IsValid())
		{
			PB_EvolvePoint->SetPercent((float)InWingData.EvolutionFailedCount / (float)MaxAllowedFailPoint);
		}
	}
}

void UB2WingUIP_Evolution::BeginShowEvolutionFailMessage()
{
	if (TB_EvolveFailMessage.IsValid())
	{
		TB_EvolveFailMessage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(EvolutionFailMessageShowTH, this, &UB2WingUIP_Evolution::StopShowEvolutionFailMessage, EvolutionFailMessageShowTime, false);
	}

	// 이 타이밍에 애니메이션 플레이를.
	BeginShowEvolutionFailMessage_BP();
}

void UB2WingUIP_Evolution::StopShowEvolutionFailMessage()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(EvolutionFailMessageShowTH);
	}

	if (TB_EvolveFailMessage.IsValid())
	{ // 이건 일단 숨기고 이벤트 상황에서 표시
		TB_EvolveFailMessage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2WingUIP_Evolution::ForceStopShowEvolutionFailMessage()
{
	StopShowEvolutionFailMessage();
}

void UB2WingUIP_Evolution::UpdateStaticWidgets()
{
	if (TB_EvolutionTitle.IsValid())
	{
		TB_EvolutionTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Evolution")));
	}

	// 최대 진화레벨 도달 여부에 따라 실제 보여지는 게 좀 다른데 여하간 일단 다 업데이트를..

	if (TB_EvolveSuccessRateLabel.IsValid())
	{
		TB_EvolveSuccessRateLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_EvolutionSuccessRate")));
	}
	if (TB_EvolvePointLabel.IsValid())
	{
		TB_EvolvePointLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_EvolutionPoint")));
	}
	if (TB_EvolveFailMessage.IsValid())
	{
		TB_EvolveFailMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_EvolutionFail")));
	}
	if (TB_EvolveNote.IsValid())
	{
		TB_EvolveNote->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Evolution_Note")));
	}
	if (TB_MaxEvolveNote.IsValid())
	{
		TB_MaxEvolveNote->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Evolution_FullNoti")));
	}

	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable && IMG_EvolveIngred.IsValid())
	{
		// 진화재료의 정해진 아이템 ID 가 있음.
		FSingleItemInfoData* EvolveIngredItemInfoData = ItemInfoTable->GetInfoData(FItemRefIDHelper::GetWingEvolveResourceRefID());
		if (EvolveIngredItemInfoData)
		{ // 고정된 이미지 하나긴 하지만 이미지 바뀌면서 누락될 수 있으므로 인포테이블서 가져와서 넣어준다.
			IMG_EvolveIngred->SetBrushFromMaterial(EvolveIngredItemInfoData->GetIconMaterial(ItemInfoTable));
		}
	}
	if (BTN_EvolutionGoodInfo.IsValid())
	{
		BTN_EvolutionGoodInfo->SetItemInfo(FItemRefIDHelper::GetWingEvolveResourceRefID());
	}
}
