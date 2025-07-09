// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIMobAppearBossStage.generated.h"

/*
* Widget for (BossStage) Mob appearing event
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMobAppearBossStage : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void SetMobName(const FText& InText);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

protected:
	TWeakObjectPtr<UTextBlock>			TB_MobName;
};
