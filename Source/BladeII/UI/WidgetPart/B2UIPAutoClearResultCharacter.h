// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIPAutoClearResultCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPAutoClearResultCharacter : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetInfo(b2network::B2CharacterServerInfoPtr InCharacterInfo, bool IsLevelUP);

protected:
	virtual void CacheAssets() override;
	
private:
	TWeakObjectPtr<UTextBlock> TB_Name;
	TWeakObjectPtr<UTextBlock> TB_CharLevel;
	TWeakObjectPtr<UTextBlock> TB_ExpPercentChar;
	TWeakObjectPtr<UImage> IMG_Portrait;
	TWeakObjectPtr<UImage> IMG_LevelUp;
	TWeakObjectPtr<UImage> IMG_LevelMax;
	TWeakObjectPtr<UProgressBar> ExpBar_Player;
};
