// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRecommandPower.h"
#include "BladeIIUtil.h"

#include "B2UIManager.h"

void UB2UIRecommandPower::SetInfo(const int32 InCurrentAttack, const int32 InCurrentDefense, const int32 InRecommendAttack, const int32 InRecommendDefense)
{
	const int32 AttackGap = InCurrentAttack - InRecommendAttack;
	const int32 DefenseGap = InCurrentDefense - InRecommendDefense;

	const bool OverAttack = AttackGap < 0;
	const bool OverDefense = DefenseGap < 0;

	const bool IsHard = OverAttack && OverDefense;
	const bool IsNormal = OverAttack || OverDefense;

	int32 RecommendState = 0;

	if (IsHard)
	{
		RecommendState = HARD;

		if (X_CP_One.IsValid())
		{
			X_CP_One->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (X_CP_Two.IsValid())
		{
			X_CP_Two->SetVisibility(ESlateVisibility::Visible);
		}

		if (X_TB_TwoText1.IsValid())
		{
			X_TB_TwoText1->SetText(FText::Format(FText::FromString(TEXT("{0} <span color=\"#ff0000ff\">{1}</>")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack")), FText::AsNumber(AttackGap)));
		}

		if (X_TB_TwoText2.IsValid())
		{
			X_TB_TwoText2->SetText(FText::Format(FText::FromString(TEXT("{0} <span color=\"#ff0000ff\">{1}</>")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense")), FText::AsNumber(DefenseGap)));
		}
	}
	else if (IsNormal)
	{
		if (X_CP_One.IsValid())
		{
			X_CP_One->SetVisibility(ESlateVisibility::Visible);
		}

		if (X_CP_Two.IsValid())
		{
			X_CP_Two->SetVisibility(ESlateVisibility::Collapsed);
		}

		RecommendState = NORMAL;

		if (X_TB_OneText.IsValid())
		{
			if (AttackGap < 0)
			{
				X_TB_OneText->SetText(FText::Format(FText::FromString(TEXT("{0} <span color=\"#ff0000ff\">{1}</>")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack")), FText::AsNumber(AttackGap)));
			}
			else
			{
				X_TB_OneText->SetText(FText::Format(FText::FromString(TEXT("{0} <span color=\"#ff0000ff\">{1}</>")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense")), FText::AsNumber(DefenseGap)));
			}
		}
	}
	else
	{
		if (X_CP_One.IsValid())
		{
			X_CP_One->SetVisibility(ESlateVisibility::Visible);
		}

		if (X_CP_Two.IsValid())
		{
			X_CP_Two->SetVisibility(ESlateVisibility::Collapsed);
		}

		RecommendState = ESAY;

		if (X_TB_OneText.IsValid())
		{
			X_TB_OneText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RecommendPower_Normal")));
		}
	}

	for (TWeakObjectPtr<UB2Image>& Elem : Slots)
	{
		if (Elem.IsValid())
		{
			Elem->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	for (int i = 0; i <= RecommendState; ++i)
	{
		if (Slots[i].IsValid())
		{
			Slots[i]->SetVisibility(ESlateVisibility::Visible);
			Slots[i]->SetBrushFromMaterial(Icons[RecommendState]);
		}
	}
}

void UB2UIRecommandPower::CacheAssets()
{
	Slots.SetNum(MAXSLOT);
	GET_SLOT_BYNAME(UB2Image, Slots[ESAY], X_IMG_Easy);
	GET_SLOT_BYNAME(UB2Image, Slots[NORMAL], X_IMG_Normal);
	GET_SLOT_BYNAME(UB2Image, Slots[HARD], X_IMG_Hard);

	GET_SLOT(UCanvasPanel, X_CP_One);
	GET_SLOT(UCanvasPanel, X_CP_Two);

	GET_SLOT(UB2RichTextBlock, X_TB_OneText);
	GET_SLOT(UB2RichTextBlock, X_TB_TwoText1);
	GET_SLOT(UB2RichTextBlock, X_TB_TwoText2);
}
