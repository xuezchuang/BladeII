// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIGuildSupportCharacterSlot.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGuildSupportCharacterClickedDelgate, EPCClass, ClickedHero);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildSupportCharacterSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void SetHeroImage_BP(EPCClass CharType);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void SetSwitchState_BP(bool IsEnabled);

public:
	void SetMainHero(bool IsMain);
	void SetDefaultHero();
	void SetHeroImage(EPCClass CharType);
	void SetSwitchState(bool IsEnabled);

	EPCClass GetCharacterClass() { return CharacterClass; }
private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	UFUNCTION()
	void OnClickBtnArea();

public:
	FOnGuildSupportCharacterClickedDelgate OnClickedDelgate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	EPCClass CharacterClass;

	TWeakObjectPtr<UTextBlock> TB_CharNumber;
	TWeakObjectPtr<UB2Button> BTN_Area;
private:

};
