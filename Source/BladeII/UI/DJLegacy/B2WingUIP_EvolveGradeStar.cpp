// Fill out your copyright notice in the Description page of Project Settings.

#include "B2WingUIP_EvolveGradeStar.h"
#include "B2Airport.h"
#include "B2WingInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "Event.h"

UB2WingUIP_EvolveGradeStar::UB2WingUIP_EvolveGradeStar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FilledStarImageMtrl = NULL;
	EmptyStarImageMtrl = NULL;

	StarImagePrefix = TEXT("GradeStar");
}

void UB2WingUIP_EvolveGradeStar::CacheAssets()
{
	Super::CacheAssets();

	AllStarImages.Empty();
	for (int32 SI = MIN_WING_EVOLUTION_GRADE; SI <= MAX_WING_NUMBERIMAGE; ++SI)
	{
		const FString& ThisStarName = FString::Printf(TEXT("%s_%d"), *StarImagePrefix, SI);
		UImage* ThisFoundImage = GetSlot<UImage>(FName(*ThisStarName));
		if (ThisFoundImage)
		{
			ThisFoundImage->SetVisibility(ESlateVisibility::Collapsed); // 老窜 见败初绊 殿鞭父怒 焊捞霸.
			AllStarImages.Add(ThisFoundImage);
		}
	}

	GET_SLOT(UImage, IMG_GradeNum);

	AllStarScale.Empty();

	AllStarScale.SetNum(MAX_WING_EVOLUTION_GRADE);
	GET_SLOT_BYNAME(UScaleBox, AllStarScale[0], SB_GradeStar_1);
	GET_SLOT_BYNAME(UScaleBox, AllStarScale[1], SB_GradeStar_2);
	GET_SLOT_BYNAME(UScaleBox, AllStarScale[2], SB_GradeStar_3);
	GET_SLOT_BYNAME(UScaleBox, AllStarScale[3], SB_GradeStar_4);
	GET_SLOT_BYNAME(UScaleBox, AllStarScale[4], SB_GradeStar_5);
	GET_SLOT_BYNAME(UScaleBox, AllStarScale[5], SB_GradeStar_6);
	GET_SLOT_BYNAME(UScaleBox, AllStarScale[6], SB_GradeStarNum);

	for (int32 i = 0; i < MAX_WING_NUMBERIMAGE; i++)
	{
		if (AllStarScale.IsValidIndex(i))
		{
			if(AllStarScale[i].IsValid())
				AllStarScale[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	GET_SLOT(UPanelWidget, SB_GradeStarNum);
	GET_SLOT(UPanelWidget, HB_StarGradeBase);
}

void UB2WingUIP_EvolveGradeStar::SetCurrentGrade(int32 InGrade, bool bInShowNextGradeExpectation)
{
	bool GradeDivision = InGrade <= MAX_WING_NUMBERIMAGE ?
		(InGrade == MAX_WING_NUMBERIMAGE && bInShowNextGradeExpectation ? false : true)			//6己狼 固府焊绊捞搁 捞固瘤肺 甸绢啊具窍绰单 弊炼扒
		: false;


	if (GradeDivision)
	{
		if (HB_StarGradeBase.IsValid())
			HB_StarGradeBase->SetVisibility(ESlateVisibility::Visible);

		if (SB_GradeStarNum.IsValid())
			SB_GradeStarNum->SetVisibility(ESlateVisibility::Collapsed);

		for (int32 SI = 0; SI < AllStarImages.Num(); ++SI)
		{
			UImage* ThisStarImage = AllStarImages[SI];
			if (ThisStarImage)
			{
				const bool bShouldShowAsFilledImage = (SI < InGrade);
				const bool bShouldShowAsEmptyImage = (bInShowNextGradeExpectation && SI == InGrade);

				if (InGrade < MAX_WING_EVOLUTION_GRADE)
				{
					ThisStarImage->SetVisibility((bShouldShowAsFilledImage || bShouldShowAsEmptyImage) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

					if (AllStarScale[SI].IsValid())
						AllStarScale[SI]->SetVisibility((bShouldShowAsFilledImage || bShouldShowAsEmptyImage) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);


					if (bShouldShowAsFilledImage) {
						ThisStarImage->SetBrushFromMaterial(FilledStarImageMtrl);
					}
					else if (bShouldShowAsEmptyImage) {
						ThisStarImage->SetBrushFromMaterial(EmptyStarImageMtrl);
					}
				}
				else
				{
					ThisStarImage->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
	else
	{
		if (HB_StarGradeBase.IsValid())
			HB_StarGradeBase->SetVisibility(ESlateVisibility::Collapsed);

		if (SB_GradeStarNum.IsValid())
			SB_GradeStarNum->SetVisibility(ESlateVisibility::Visible);


		if (bInShowNextGradeExpectation)									//6己老版快 弊成 7肺 父甸绢霖促 抗寇贸府
			++InGrade;
		int32 GradeDiversion = InGrade % (MAX_WING_NUMBERIMAGE + 1);


		if (StarGrade.IsValidIndex(GradeDiversion))
		{
			if (IMG_GradeNum.IsValid())
			{
				if (bInShowNextGradeExpectation)
				{
					IMG_GradeNum->SetBrushFromMaterial(StarGrade[GradeDiversion].EmptyStarImageMtrl);
				}
				else
				{
					IMG_GradeNum->SetBrushFromMaterial(StarGrade[GradeDiversion].FilledStarImageMtrl);
				}

			}
		}
	}
}
