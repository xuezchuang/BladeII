// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "BladeIICharacter.h"
#include "B2UIBossStatus.generated.h"

/*
* Boss Status UI InBattle
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBossStatus : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void SetName(const FText& InText);
	void SetHP(float CurHP, float MaxHP);
	void SetInfluence(const FText &InText);

	void HideInfluence();
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BossStatus")
		void SetDamaged(float Amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BossStatus")
	void PlayQTEBreakPointRefreshAnim();

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	void UpdateBossInfo(ENPCClass InClass, ENPCClassVariation InVariation);
private:

	//===================================================Click

	//====================================================Doc Delegate
	UFUNCTION()
		void OnChangedBossHPCurrent(class UB2UIDocBase* Sender, float BossHPCurrent, float PrevBossHPCurrent);
	UFUNCTION()
		void OnChangedBossHPMax(class UB2UIDocBase* Sender, float BossHPMax, float PrevBossHPMax);
	UFUNCTION()
		void OnChangedBossMobClass(class UB2UIDocBase* Sender, ENPCClass BossMobClass, ENPCClass PrevBossMobClass);
	UFUNCTION()
		void OnChangedBossMobClassVariation(class UB2UIDocBase* Sender, ENPCClassVariation BossMobClassVariation, ENPCClassVariation PrevBossMobClassVariation);

	UFUNCTION()
		void OnChangedBossQTEBreakPointCurrent(class UB2UIDocBase* Sender, float PointCurrent, float PrevPointCurrent);
	UFUNCTION()
		void OnChangedBossQTEBreakPointMax(class UB2UIDocBase* Sender, float PointMax, float PrevPointMax);
	UFUNCTION()
		void OnChangedBossIsQTEBreakable(class UB2UIDocBase* Sender, bool bBreakable, bool bPrevBreakable);

protected:
	TWeakObjectPtr<class UB2UIProgressBarDividedDual>	UIP_ProgressBar_Boss;
	TWeakObjectPtr<UTextBlock>						TB_BossName;
	TWeakObjectPtr<UTextBlock>						TB_BossInfluence;
};
