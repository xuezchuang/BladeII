// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "B2UIWidget.h"
#include "B2UITeamMatchHpBar.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamMatchHpBar : public UB2UIWidget
{
	GENERATED_BODY()

	UB2UITeamMatchHpBar(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UImage> IMG_CharacterImage;
	TWeakObjectPtr<UTextBlock> TB_Lv;
	TWeakObjectPtr<UTextBlock> TB_Job;
	TWeakObjectPtr<UProgressBar> PB_HP;
};
