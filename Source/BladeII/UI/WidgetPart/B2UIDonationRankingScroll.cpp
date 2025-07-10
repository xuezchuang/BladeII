// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationRankingScroll.h"
#include "B2UIDonationRankingScrollSlot.h"
#include "B2UIDocHelper.h"
#include "B2UIDocDonation.h"


void UB2UIDonationRankingScroll::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (int i = 0; i < UIP_RankingScrollSlots.Num(); i++)
	{
		UIP_RankingScrollSlots[i]->DestroySelf(InUIManager);
	}
}

void UB2UIDonationRankingScroll::UpdateScroll()
{
	TArray<FRankerData> Rankers;
	Rankers.Empty();
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		DocDonation->GetDonationRanker(Rankers);
	}

	if (X_SB_RankingScrollBox.IsValid())
	{
		for (int i = 0; i < Rankers.Num(); i++)
		{
			UB2UIDonationRankingScrollSlot* NewItem;
			if (UIP_RankingScrollSlots.IsValidIndex(i) == false)
			{
				NewItem = CreateWidget<UB2UIDonationRankingScrollSlot>(GetWorld(), RankingSlotClass);
				UIP_RankingScrollSlots.Add(NewItem);
			}
			else
			{
				NewItem = UIP_RankingScrollSlots[i].Get();
			}
			NewItem->Init();
			NewItem->SetData(Rankers[i]);
			X_SB_RankingScrollBox->AddChild(NewItem);
		}
	}
}

void UB2UIDonationRankingScroll::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2ScrollBox, X_SB_RankingScrollBox);
	GET_SLOT(UB2RichTextBlock, X_RTB_ScrollTitle);
}

void UB2UIDonationRankingScroll::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (X_RTB_ScrollTitle.IsValid())
	{
		X_RTB_ScrollTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_RankTitle")));
	}
}
