// Fill out your copyright notice in the Description page of Project Settings.
#include "B2WingUIP_Evolution.h"
//#include "BladeII.h"

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
		{ // 捞扒 老窜 见扁绊 捞亥飘 惑炔俊辑 钎矫
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

	// 弥措 柳拳饭骇 档崔 咯何俊 蝶扼 农霸 焊咯瘤绰 技飘啊 崔扼咙.
	if (OV_NormalEvolveMenuSet.IsValid())
	{
		OV_NormalEvolveMenuSet->SetVisibility(bShouldShowMaxEvolveSet ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	if (OV_MaxEvolvedSet.IsValid())
	{
		OV_MaxEvolvedSet->SetVisibility(bShouldShowMaxEvolveSet ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	// 老馆柳拳皋春 / 弥措柳拳皋春 阿阿俊 蝶弗 技何 诀单捞飘
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
			TB_CurrentIngredNum->SetColorAndOpacity( // 拿矾蔼 傲 窍靛内爹.. 被捞..
				(CurrentEvolveIngredNum >= RequiredItemCount) ? FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)) : FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))
				);
		}
		if (TB_RequiredIngredNum.IsValid())
		{
			TB_RequiredIngredNum->SetText(FText::AsNumber(RequiredItemCount));
		}

		if (UIP_EvolveGradeStar.IsValid())
		{ // 泅 柳拳殿鞭苞 窃膊 促澜 啊瓷茄 柳拳殿鞭阑 鞍捞 焊咯霖促.
			UIP_EvolveGradeStar->SetCurrentGrade(InWingData.EvolutionGrade, true);
		}
		if (TB_EvolveSuccessRate.IsValid())
		{ // 父盒啦肺 甸绢坷绰 父怒 老窜 家荐 笛掳磊府肺 钎矫秦 焊绊.. 酒聪扼 窍搁 int 肺 官操绊..
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

	// 捞 鸥捞怪俊 局聪皋捞记 敲饭捞甫.
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
	{ // 捞扒 老窜 见扁绊 捞亥飘 惑炔俊辑 钎矫
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

	// 弥措 柳拳饭骇 档崔 咯何俊 蝶扼 角力 焊咯瘤绰 霸 粱 促弗单 咯窍埃 老窜 促 诀单捞飘甫..

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
		// 柳拳犁丰狼 沥秦柳 酒捞袍 ID 啊 乐澜.
		FSingleItemInfoData* EvolveIngredItemInfoData = ItemInfoTable->GetInfoData(FItemRefIDHelper::GetWingEvolveResourceRefID());
		if (EvolveIngredItemInfoData)
		{ // 绊沥等 捞固瘤 窍唱变 窍瘤父 捞固瘤 官差搁辑 穿遏瞪 荐 乐栏骨肺 牢器抛捞喉辑 啊廉客辑 持绢霖促.
			IMG_EvolveIngred->SetBrushFromMaterial(EvolveIngredItemInfoData->GetIconMaterial(ItemInfoTable));
		}
	}
	if (BTN_EvolutionGoodInfo.IsValid())
	{
		BTN_EvolutionGoodInfo->SetItemInfo(FItemRefIDHelper::GetWingEvolveResourceRefID());
	}
}
