// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationProductCost.h"
#include "B2UIDocHelper.h"
#include "B2UIDocDonation.h"


void UB2UIDonationProductCost::Init()
{
	Super::Init();

	if (TB_Cost.IsValid())
	{
		TB_Cost->SetColorAndOpacity(FLinearColor::White);
	}

	if (UIP_RedDot.IsValid())
	{
		UIP_RedDot->Init();
	}
}

void UB2UIDonationProductCost::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (UIP_RedDot.IsValid())
	{
		UIP_RedDot->DestroySelf(InUIManager);
	}
}

void UB2UIDonationProductCost::SetCost(int32 InCost, int32 FreeCount)
{
	if (FreeCount != 0)
	{
		if (IMG_Icon.IsValid())
		{
			IMG_Icon->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (TB_Cost.IsValid())
		{
			TB_Cost->SetColorAndOpacity(FLinearColor::White);
			TB_Cost->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_DonationButton_Free")), FreeCount));
		}

		if (UIP_RedDot.IsValid())
		{
			UIP_RedDot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		int32 UserCost = 0;

		if (IMG_Icon.IsValid())
		{
			IMG_Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
			UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
			if (DocDonation && DocUser)
			{
				int32 CostType = DocDonation->GetCostType();
				switch (CostType)
				{
				case b2network::B2DonationCostType::GEM:
					IMG_Icon->SetBrushFromMaterial(GemIcon.LoadSynchronous());
					UserCost = DocUser->GetGem();
					break;
				case b2network::B2DonationCostType::GOLD:
					IMG_Icon->SetBrushFromMaterial(GoldIcon.LoadSynchronous());
					UserCost = DocUser->GetGold();
					break;
				}
			}
		}

		if (TB_Cost.IsValid())
		{
			TB_Cost->SetText(FText::AsNumber(InCost));
			TB_Cost->SetColorAndOpacity(InCost <= UserCost ? FLinearColor::White : FLinearColor::Red);
		}

		if (UIP_RedDot.IsValid())
		{
			UIP_RedDot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIDonationProductCost::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UTextBlock, TB_Cost);
	GET_SLOT(UB2UIWidgetBase, UIP_RedDot);
}

#if WITH_EDITOR
void UB2UIDonationProductCost::EditorOnPostLoad()
{
	GemIcon.LoadSynchronous();
	GoldIcon.LoadSynchronous();
}
#endif
