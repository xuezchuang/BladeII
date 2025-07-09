// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2WingUIP_EnhLevelProgElem.h"
#include "B2Airport.h"
#include "Event.h"

UB2WingUIP_EnhLevelProgElem::UB2WingUIP_EnhLevelProgElem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyEnhanceLevel = 1;

	LevelUpAnimPlayTime = 0.6f;
}

void UB2WingUIP_EnhLevelProgElem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Filled);
	GET_SLOT(UImage, IMG_Unfilled);
	GET_SLOT(UImage, FX_Enhance);
	GET_SLOT(UTextBlock, TB_MyEnhanceLevelNum);

}

void UB2WingUIP_EnhLevelProgElem::SetMyEnhanceLevel(int32 InLevel)
{
	MyEnhanceLevel = FMath::Clamp(InLevel, MIN_WING_ENHANCE_LEVEL, MAX_WING_ENHANCE_LEVEL);
	if (TB_MyEnhanceLevelNum.IsValid())
	{
		TB_MyEnhanceLevelNum->SetText(FText::AsNumber(MyEnhanceLevel));
	}
}

void UB2WingUIP_EnhLevelProgElem::UpdateByWingEnhanceLevel(int32 InWingLevel)
{
	if (IMG_Filled.IsValid())
	{
		IMG_Filled->SetVisibility((MyEnhanceLevel <= InWingLevel) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (IMG_Unfilled.IsValid())
	{
		// 일단 레벨 차 있는 부분에선 숨기는데 background 같은 디자인이라면 이런 거 할 필요없이 계속 보이도록 해야 함.
		IMG_Unfilled->SetVisibility((MyEnhanceLevel > InWingLevel) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2WingUIP_EnhLevelProgElem::PlayLevelupAnim()
{
	if (FX_Enhance.IsValid())
	{
		FX_Enhance->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(LevelUpAnimStopTH, this, &UB2WingUIP_EnhLevelProgElem::StopLevelupAnim, LevelUpAnimPlayTime, false);
	}

	PlayLevelupAnim_BP(); // 애니메이션 플레이는 여기서 실행..
}

void UB2WingUIP_EnhLevelProgElem::StopLevelupAnim()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(LevelUpAnimStopTH);
	}
	if (FX_Enhance.IsValid())
	{
		FX_Enhance->SetVisibility(ESlateVisibility::Hidden);
	}
}