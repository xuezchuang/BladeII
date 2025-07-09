// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "B2UIBattleVignette.generated.h"

/*
* When the HP of the character turned low, it serves to inform the risk.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBattleVignette : public UB2UIWidgetBase, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UIBattleVignette(const FObjectInitializer& ObjectInitializer);
	void BindDoc(class UB2UIDocBattle* DocBattle);
	virtual void UnbindDoc() override;

	void SetWarningHP(bool bWarning);


public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Vignette")
		void SetWarningHP_BP(bool bWarning);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	//=================================================== Btn

	//==================================================== Doc Delegate
	UFUNCTION()
		void OnChangedCurrPCInCriticalCondition(class UB2UIDocBase* Sender, bool CurrPCInCriticalCondition, bool PrevCurrPCInCriticalCondition);


protected:
	//TWeakObjectPtr<UImage>					IMG_Vignette;

private:
};
