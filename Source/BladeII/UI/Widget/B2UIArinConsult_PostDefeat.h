// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "CommonStruct.h"
#include "B2UIDocBindable.h"
#include "B2UIArinConsult_Common.h"
#include "B2UIArinConsult_PostDefeat.generated.h"

/*
 * Missy sexy Arin's insulting consulting
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIArinConsult_PostDefeat : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

protected:

	TWeakObjectPtr<UB2UIArinConsult_Common> UIP_ACCommon;

	TWeakObjectPtr<UTextBlock> TB_Arin;
	TWeakObjectPtr<UTextBlock> TB_ConsultMessage;
	
	TWeakObjectPtr<UB2Button> BTN_ItemEnhance;
	TWeakObjectPtr<UTextBlock> TB_ItemEnhance;

	TWeakObjectPtr<UB2Button> BTN_SkillEnhance;
	TWeakObjectPtr<UTextBlock> TB_SkillEnhance;

	TWeakObjectPtr<UB2Button> BTN_ItemPurchase;
	TWeakObjectPtr<UTextBlock> TB_ItemPurchase;

public:
	UB2UIArinConsult_PostDefeat(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
		
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBtnItemEnhance();

	UFUNCTION()
	void OnClickBtnSkillEnhance();

	UFUNCTION()
	void OnClickBtnItemPurchase();

	void CloseMe();
};
