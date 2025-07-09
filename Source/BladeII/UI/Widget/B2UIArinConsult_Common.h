// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIArinConsult_Common.generated.h"

UENUM()
enum class EArinConsultType : uint8
{
	ARCT_PreCombatConsulting,
	ARCT_PostDefeatGettingStrong,
	
	ARCT_End
};

/*
 * Missy sexy Arin's insulting consulting
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIArinConsult_Common : public UB2UIWidget
{
	GENERATED_BODY()

	EArinConsultType SubType; // Where this common part is being used?

public:
	UB2UIArinConsult_Common(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	void SetSubType(EArinConsultType InType);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;


	UFUNCTION()
	void OnClickBtnClose();

protected:

	TWeakObjectPtr<UTextBlock> TB_ArinConsultTitle;
	TWeakObjectPtr<UB2Button> BTN_Close;
	
};
