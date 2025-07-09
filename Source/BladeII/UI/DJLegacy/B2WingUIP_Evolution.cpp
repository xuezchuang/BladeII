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
		{ // �̰� �ϴ� ����� �̺�Ʈ ��Ȳ���� ǥ��
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

	// �ִ� ��ȭ���� ���� ���ο� ���� ũ�� �������� ��Ʈ�� �޶���.
	if (OV_NormalEvolveMenuSet.IsValid())
	{
		OV_NormalEvolveMenuSet->SetVisibility(bShouldShowMaxEvolveSet ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	if (OV_MaxEvolvedSet.IsValid())
	{
		OV_MaxEvolvedSet->SetVisibility(bShouldShowMaxEvolveSet ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	// �Ϲ���ȭ�޴� / �ִ���ȭ�޴� ������ ���� ���� ������Ʈ
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
			TB_CurrentIngredNum->SetColorAndOpacity( // �÷��� �� �ϵ��ڵ�.. ����..
				(CurrentEvolveIngredNum >= RequiredItemCount) ? FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)) : FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))
				);
		}
		if (TB_RequiredIngredNum.IsValid())
		{
			TB_RequiredIngredNum->SetText(FText::AsNumber(RequiredItemCount));
		}

		if (UIP_EvolveGradeStar.IsValid())
		{ // �� ��ȭ��ް� �Բ� ���� ������ ��ȭ����� ���� �����ش�.
			UIP_EvolveGradeStar->SetCurrentGrade(InWingData.EvolutionGrade, true);
		}
		if (TB_EvolveSuccessRate.IsValid())
		{ // �������� ������ ��ŭ �ϴ� �Ҽ� ��°�ڸ��� ǥ���� ����.. �ƴ϶� �ϸ� int �� �ٲٰ�..
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

	// �� Ÿ�ֿ̹� �ִϸ��̼� �÷��̸�.
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
	{ // �̰� �ϴ� ����� �̺�Ʈ ��Ȳ���� ǥ��
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

	// �ִ� ��ȭ���� ���� ���ο� ���� ���� �������� �� �� �ٸ��� ���ϰ� �ϴ� �� ������Ʈ��..

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
		// ��ȭ����� ������ ������ ID �� ����.
		FSingleItemInfoData* EvolveIngredItemInfoData = ItemInfoTable->GetInfoData(FItemRefIDHelper::GetWingEvolveResourceRefID());
		if (EvolveIngredItemInfoData)
		{ // ������ �̹��� �ϳ��� ������ �̹��� �ٲ�鼭 ������ �� �����Ƿ� �������̺� �����ͼ� �־��ش�.
			IMG_EvolveIngred->SetBrushFromMaterial(EvolveIngredItemInfoData->GetIconMaterial(ItemInfoTable));
		}
	}
	if (BTN_EvolutionGoodInfo.IsValid())
	{
		BTN_EvolutionGoodInfo->SetItemInfo(FItemRefIDHelper::GetWingEvolveResourceRefID());
	}
}
