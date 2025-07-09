// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIMsgPopupUseResources.h"




void UB2UIMsgPopupUseResources::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UTextBlock, TB_Cost);
	GET_SLOT(UB2RichTextBlock, TB_ContentSub);
}


void UB2UIMsgPopupUseResources::SetCost(int32 nCost)
{
	if (TB_Cost.IsValid())
		TB_Cost->SetText(FText::AsNumber(nCost));
}

void UB2UIMsgPopupUseResources::SetSubContent(FText txtSubContent)
{
	if (TB_ContentSub.IsValid())
		TB_ContentSub->SetText(txtSubContent);
}
