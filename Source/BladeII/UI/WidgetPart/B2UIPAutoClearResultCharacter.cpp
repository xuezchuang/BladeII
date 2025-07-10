// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPAutoClearResultCharacter.h"
#include "B2UIDocHelper.h"
#include "BladeIIGameImpl.h"


void UB2UIPAutoClearResultCharacter::SetInfo(b2network::B2CharacterServerInfoPtr InCharacterInfo, bool IsLevelUP)
{
	EPCClass CharPCClass = SvrToCliPCClassType(InCharacterInfo->character_type);
	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(CharPCClass));
	if (DocHero)
	{
		if (IMG_Portrait.IsValid())
		{
			IMG_Portrait->SetBrushFromMaterial(UB2UIDocHero::GetPlayerPortraitMaterial(this, DocHero->GetHeroClass()));
		}

		if (TB_Name.IsValid())
		{
			TB_Name->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(DocHero->GetHeroClass()));
		}
	}

	if (TB_CharLevel.IsValid())
	{
		TB_CharLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("StageClear_Level"))),
			FText::AsNumber(InCharacterInfo->level)));
	}

	if (TB_ExpPercentChar.IsValid())
	{
		float ExpPercent = (InCharacterInfo->exp * 100.0f ) / InCharacterInfo->exp_max;
		TB_ExpPercentChar->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), ExpPercent)));
	}

	if (ExpBar_Player.IsValid())
	{
		ExpBar_Player->SetPercent(static_cast<float>(InCharacterInfo->exp) / static_cast<float>(InCharacterInfo->exp_max));
	}

	if (IMG_LevelUp.IsValid())
	{
		IMG_LevelUp->SetVisibility(IsLevelUP ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2UIPAutoClearResultCharacter::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Name);
	GET_SLOT(UTextBlock, TB_CharLevel);
	GET_SLOT(UTextBlock, TB_ExpPercentChar);
	GET_SLOT(UImage, IMG_Portrait);
	GET_SLOT(UImage, IMG_LevelUp);
	GET_SLOT(UImage, IMG_LevelMax);
	GET_SLOT(UProgressBar, ExpBar_Player);
}