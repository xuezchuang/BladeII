// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UITMCharacterSelectItem.h"
#include "B2UIDocHero.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UITMCharacterSelectItem::BindDoc(class UB2UIDocHero* DocHero)
{
	if (DocHero)
	{
		Super::BindDoc(DocHero);

		EPCClass pcClass = DocHero->GetHeroClass();

		FText Name = BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(pcClass);

		if (TB_SelectHeroName.IsValid())
			TB_SelectHeroName->SetText(Name);

		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore °³¼±
		//int32 Level = DocHero->GetCurrentLevel();
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
		int32 Level = CharacterDataStore.GetCharacterLevel(pcClass);
		if (TB_SelectHeroLevel.IsValid())
			TB_SelectHeroLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Hero_Level"))), FText::AsNumber(Level)));

		int32 Power = DocHero->GetCombatPower();

		if (TB_SelectCombatPower.IsValid())
			TB_SelectCombatPower->SetText(FText::AsNumber(Power));
	}
	
}

void UB2UITMCharacterSelectItem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_SelectHeroName);
	GET_SLOT(UTextBlock, TB_SelectHeroLevel);
	GET_SLOT(UTextBlock, TB_SelectCombatPower);
	GET_SLOT(UTextBlock, TB_CharNumber);
}

void UB2UITMCharacterSelectItem::SetFormationNum(EFormationNUM Formation)
{
	if (TB_CharNumber.IsValid())
	{
		int8 FormationNum = static_cast<int8>(Formation);
		TB_CharNumber->SetText(FText::AsNumber(FormationNum));
	}
}
