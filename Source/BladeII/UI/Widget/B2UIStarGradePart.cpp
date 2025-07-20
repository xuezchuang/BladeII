// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIStarGradePart.h"
#include "B2UIManager.h"

#include "B2ItemInfo.h"

void UB2UIStarGradePart::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UPanelWidget, P_Stars);
	GET_SLOT(UPanelWidget, P_StarGradeOver6);

	IMG_StarUnder7.Empty();
	IMG_StarUnder7.SetNum(6);

	if (IMG_StarUnder7.Num() == 6)
	{
		GET_SLOT_BYNAME(UImage, IMG_StarUnder7[0], IMG_Star1);
		GET_SLOT_BYNAME(UImage, IMG_StarUnder7[1], IMG_Star2);
		GET_SLOT_BYNAME(UImage, IMG_StarUnder7[2], IMG_Star3);
		GET_SLOT_BYNAME(UImage, IMG_StarUnder7[3], IMG_Star4);
		GET_SLOT_BYNAME(UImage, IMG_StarUnder7[4], IMG_Star5);
		GET_SLOT_BYNAME(UImage, IMG_StarUnder7[5], IMG_Star6);
	}

	CONSTEXPR int32 OverCount = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE - FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE;
	CONSTEXPR int32 StartOverGrade = FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE + 1;

	StarOver6.Empty();
	StarOver6.SetNum(OverCount);

	if (StarOver6.Num() == OverCount)
	{
		for (int32 i = 0; i < OverCount; ++i)
		{
			StarOver6[i].StarGrade = i + StartOverGrade;
			StarOver6[i].IMG_NormalStar = GetCachedWidget<UImage>(FName(*FString::Printf(TEXT("IMG_StarGrade%d"), i + StartOverGrade)));
			StarOver6[i].IMG_SurpassStar = GetCachedWidget<UImage>(FName(*FString::Printf(TEXT("IMG_StarGrade%d_Surpass"), i + StartOverGrade)));
		}
	}
}

void UB2UIStarGradePart::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIStarGradePart::Init()
{
	Super::Init();

	EnbleSurpass = true;
}

void UB2UIStarGradePart::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIStarGradePart::SetVisibleOver6(const int32 InItemGrade, bool InIsSurpass)
{
	if (P_StarGradeOver6.IsValid())
	{
		P_StarGradeOver6->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (P_Stars.IsValid())
	{
		P_Stars->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 i = 0; i < StarOver6.Num(); ++i)
	{
		if (StarOver6[i].StarGrade == InItemGrade)
		{
			StarOver6[i].IMG_NormalStar->SetVisibility(InIsSurpass ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
			StarOver6[i].IMG_SurpassStar->SetVisibility(InIsSurpass ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
		else
		{
			StarOver6[i].IMG_NormalStar->SetVisibility(ESlateVisibility::Collapsed);
			StarOver6[i].IMG_SurpassStar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIStarGradePart::SetVisibleUnder7(const int32 InItemGrade, bool InIsSurpass)
{
	if (P_StarGradeOver6.IsValid())
	{
		P_StarGradeOver6->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (P_Stars.IsValid())
	{
		P_Stars->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	for (int32 i = 0; i < IMG_StarUnder7.Num(); ++i)
	{
		bool IsVisible = InItemGrade > i;
		
		SetVisibleUnder7Surpass(i, InIsSurpass);

		if (IMG_StarUnder7.IsValidIndex(i) && IMG_StarUnder7[i].IsValid())
		{
			IMG_StarUnder7[i]->SetVisibility(IsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIStarGradePart::SetVisibleUnder7Surpass(const int32 InIndex, bool InIsSurpass)
{
	if (IMG_StarUnder7.IsValidIndex(InIndex) && IMG_StarUnder7[InIndex].IsValid())
	{
		UImage* CurrSGImage = IMG_StarUnder7[InIndex].Get();

		if (CurrSGImage)
		{
			auto* ItemMiscInfo = StaticFindItemMiscInfo();
			auto* StarMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemStarGradeImage(InIsSurpass) : NULL;

			if (StarMtrl)
			{
				CurrSGImage->SetBrushFromMaterial(StarMtrl);
			}
		}
	}
}

void UB2UIStarGradePart::UpdateStar(const FB2Item& InItem)
{
	if (InItem.ItemClass == EItemClass::EIC_Ether)
	{
		UpdateStar(InItem.StarGrade, false);
	} 
	else
	{
		UpdateStar(InItem.StarGrade, InItem.SurpassCount > 0);
	}
}

void UB2UIStarGradePart::UpdateStar(const int32 InItemGrade, bool InIsSurpass)
{
	// 檬岿茄 酒捞袍捞绊, 檬岿(阑 救静绰 巴档 乐栏骨肺)阑 荤侩窍辨 盔窍搁
	bool IsSurpass = InIsSurpass && EnbleSurpass;
	bool IsOver6 = InItemGrade > FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE;

	if (IsOver6)
	{
		SetVisibleOver6(InItemGrade, IsSurpass);
	}
	else
	{
		SetVisibleUnder7(InItemGrade, IsSurpass);
	}
}


