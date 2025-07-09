// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_BrevetNodeIconDummy.h"
#include "BladeIILocText.h"

void UB2LobbyUI_BrevetNodeIconDummy::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UTextBlock, TB_LeftUnlockCondition);
	GET_SLOT(UTextBlock, TB_RightTip);
	GET_SLOT(UTextBlock, TB_NextUpgrade);
	GET_SLOT(UTextBlock, TB_EndContent);

	if (IMG_Icon.IsValid())
		IMG_Icon->SetVisibility(ESlateVisibility::Hidden);

	if (TB_LeftUnlockCondition.IsValid())
		TB_LeftUnlockCondition->SetVisibility(ESlateVisibility::Hidden);

	if (TB_RightTip.IsValid())
		TB_RightTip->SetVisibility(ESlateVisibility::Hidden);

	UpdateStaticText();
}

void UB2LobbyUI_BrevetNodeIconDummy::UpdateStaticText()
{
	if (TB_NextUpgrade.IsValid())
		TB_NextUpgrade->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_NextUpgrade")));
}

void UB2LobbyUI_BrevetNodeIconDummy::SetNextRankInfo(UMaterialInterface* pMI, FText& txtRankName)
{
	if (IMG_Icon.IsValid())
	{
		IMG_Icon->SetBrushFromMaterial(pMI);
		IMG_Icon->SetVisibility(pMI ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (TB_RightTip.IsValid())
	{
		TB_RightTip->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_NextRankAchevementNotice")), txtRankName));
		TB_RightTip->SetVisibility(pMI ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2LobbyUI_BrevetNodeIconDummy::SetUnLockConditionText(int32 nOpenLevel)
{
	if (TB_LeftUnlockCondition.IsValid())
	{
		TB_LeftUnlockCondition->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("UnrockConditionNeedNLevel")), FText::AsNumber(nOpenLevel)));
		TB_LeftUnlockCondition->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2LobbyUI_BrevetNodeIconDummy::SetEndRanking()
{
	if (IMG_Icon.IsValid())
	{
		IMG_Icon->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (TB_EndContent.IsValid())
	{
		TB_EndContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_EndRankNotice")));
		TB_EndContent->SetVisibility(ESlateVisibility::Visible);
	}
}
