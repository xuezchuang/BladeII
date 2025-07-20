// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "CommonStruct.h"
#include "B2UIDocBindable.h"
#include "B2UIArinConsult_Common.h"
#include "B2UIArinConsult_PreCombat.generated.h"

/*
 * Missy sexy Arin's insulting consulting
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIArinConsult_PreCombat : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

	EArinConsultPreCombat_MainSubject MainSubject;

protected:

	/** One of those styles will be used depend on ConsultingMainPoint. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FButtonStyle ButtonStyle_WeaponEnhance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FButtonStyle ButtonStyle_ProtectionEnhance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FButtonStyle ButtonStyle_SkillEnhance;


	TWeakObjectPtr<UB2UIArinConsult_Common> UIP_ACCommon;

	TWeakObjectPtr<UTextBlock> TB_Arin;
	TWeakObjectPtr<UTextBlock> TB_ConsultMessage;
	
	TWeakObjectPtr<UB2Button> BTN_ItemOrSkillEnhance;
	TWeakObjectPtr<UTextBlock> TB_ItemOrSkillEnhance;

	TWeakObjectPtr<UB2Button> BTN_ProceedToCombat;
	TWeakObjectPtr<UTextBlock> TB_ProceedToCombat;
	
	bool bSetFromCounterDungeon;

public:
	UB2UIArinConsult_PreCombat(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	
	void SetConsultMessage(FText Message);

	FORCEINLINE void SetFromCounterDungeon() { bSetFromCounterDungeon = true; }
		
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBtnItemOrSkillEnhance();

	UFUNCTION()
	void OnClickBtnProceedToCombat();

	void CloseMe();
public:
	void CloseAndIgnore(); // If closed by this, Arin consulting will be skipped certain times afterward.
};
