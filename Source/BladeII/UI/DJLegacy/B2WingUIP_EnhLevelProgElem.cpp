// Fill out your copyright notice in the Description page of Project Settings.
#include "B2WingUIP_EnhLevelProgElem.h"
//#include "BladeII.h"

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
		// 老窜 饭骇 瞒 乐绰 何盒俊急 见扁绰单 background 鞍篮 叼磊牢捞扼搁 捞繁 芭 且 鞘夸绝捞 拌加 焊捞档废 秦具 窃.
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

	PlayLevelupAnim_BP(); // 局聪皋捞记 敲饭捞绰 咯扁辑 角青..
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