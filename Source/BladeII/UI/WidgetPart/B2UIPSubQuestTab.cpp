// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIPSubQuestTab.h"




void UB2UIPSubQuestTab::SetIcon(int32 InDifficulty)
{
	if (IMG_IconSelected.IsValid()&& SelectedImanges.Contains(InDifficulty))
	{
		IMG_IconSelected->SetBrushFromMaterial(SelectedImanges[InDifficulty]);
	}

	if (IMG_IconUnselected.IsValid() && UnselectedImanges.Contains(InDifficulty))
	{
		IMG_IconUnselected->SetBrushFromMaterial(UnselectedImanges[InDifficulty]);
	}
}

void UB2UIPSubQuestTab::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Image, IMG_IconSelected);
	GET_SLOT(UB2Image, IMG_IconUnselected);
}
