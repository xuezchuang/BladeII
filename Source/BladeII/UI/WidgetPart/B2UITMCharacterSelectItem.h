// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/WidgetPart/B2UICharacterSelectItem.h"
#include "B2UIDocTeamMatch.h"
#include "B2UITMCharacterSelectItem.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UITMCharacterSelectItem : public UB2UICharacterSelectItem
{
	GENERATED_BODY()

public :
	virtual void BindDoc(class UB2UIDocHero* DocHero) override;
	void SetFormationNum(EFormationNUM Formation);
private:
	virtual void CacheAssets() override;
protected:
	TWeakObjectPtr<UTextBlock> TB_SelectHeroName;
	TWeakObjectPtr<UTextBlock> TB_SelectHeroLevel;
	TWeakObjectPtr<UTextBlock> TB_SelectCombatPower;
	TWeakObjectPtr<UTextBlock> TB_CharNumber;
};
