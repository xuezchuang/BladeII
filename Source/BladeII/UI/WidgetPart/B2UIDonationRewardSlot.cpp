// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationRewardSlot.h"




void UB2UIDonationRewardSlot::SetInfo(FText InName, UMaterialInterface* InIcon)
{
	//if (X_RTB_Name.IsValid())
	//{
	//	X_RTB_Name->SetText(InName);
	//}

	//if (IMG_Icon.IsValid())
	//{
	//	IMG_Icon->SetBrushFromMaterial(InIcon);
	//}
}

void UB2UIDonationRewardSlot::CacheAssets()
{
	Super::CacheAssets();
	//GET_SLOT(UImage, IMG_Icon);
	//GET_SLOT(UB2RichTextBlock, X_RTB_Name);
}
