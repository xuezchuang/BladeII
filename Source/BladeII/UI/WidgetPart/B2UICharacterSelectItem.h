// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "BladeIIPlayer.h"
#include "B2UICharacterSelectItem.generated.h"

/* This is when you click on informing purpose. */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCharacterSelectItemClickedDelgate, UB2UICharacterSelectItem*, ClickedHero);

/*
* individual hero Parts used in the ChangeCharacterUI.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICharacterSelectItem : public UB2UIWidgetBase, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UICharacterSelectItem(const FObjectInitializer& ObjectInitializer);
	virtual void BindDoc(class UB2UIDocHero* DocHero);
	virtual void UnbindDoc() override;

	void SetHeroImage(EPCClass CharType);
	void SetHeroName(const FText& InText);
	void SetHeroLevel(int32 InValue);
	void SetHeroCombatPower(int32 InValue);
	EPCClass GetCharacterClass() { return CharacterClass; }

	//State mode for the select.
	void SetSelectMode(EUIHeroSelectModeType InSelectMode);
	UFUNCTION(BlueprintCallable, Category = "BladeII CharacterSelectItem")
	EUIHeroSelectModeType GetSelecteMode() const { return SelectModeType; }

	//The actual select state.
	void SetSelectedType(EUIHeroSelectType InSelectType);
	UFUNCTION(BlueprintCallable, Category = "BladeII CharacterSelectItem")
	EUIHeroSelectType GetSelectedType() const { return SelectType; }


public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelectItem")
		void SetHeroImage_BP(EPCClass CharType);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelectItem")
		void SetSelectMode_BP(EUIHeroSelectModeType InSelectMode);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelectItem")
		void SetSelectedType_BP(EUIHeroSelectType InSelectType);
protected:
	virtual void CacheAssets() override;
	
private:
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	//=================================================== Btn
	UFUNCTION()
		void OnClickBtnArea();

	//==================================================== Doc Delegate
	UFUNCTION()
		void OnChangedHeroLevel(class UB2UIDocBase* Sender, int32 CurLevel, int32 PrevLevel);

public:
	FOnCharacterSelectItemClickedDelgate OnCharacterSelectItemClickedDelgate;


protected:
	//TWeakObjectPtr<UButton>				BTN_SelectStartMain;
	//TWeakObjectPtr<UButton>				BTN_SelectStartSub;
	TWeakObjectPtr<UButton>				BTN_Area;
	TWeakObjectPtr<UImage>				IMG_Hero;
	TWeakObjectPtr<UTextBlock>			TB_HeroName;
	TWeakObjectPtr<UTextBlock>			TB_HeroLevel;
	TWeakObjectPtr<UTextBlock>			TB_AttackPower;
	TWeakObjectPtr<UTextBlock>			TB_DefensePower;
	TWeakObjectPtr<UTextBlock>			TB_HeroLevel2;
	TWeakObjectPtr<UTextBlock>			TB_CombatPower;

private:
	EUIHeroSelectModeType SelectModeType;
	EUIHeroSelectType SelectType;
	EPCClass			CharacterClass;
};
