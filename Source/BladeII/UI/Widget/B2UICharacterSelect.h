// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UICharacterSelect.generated.h"

/*
* The UI for changing the hero to entry before the battle.
*/
DECLARE_DELEGATE(FStartBattleCallBack)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICharacterSelect : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UICharacterSelect(const FObjectInitializer& ObjectInitializer);
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void Init() override;


	void SetHerosTotalCombat(int32 InValue);


	void NewSetHerosTotalCombat(int32 InValue);

	/* Select the type of setting allows the current progress. */
	void SetCurrentSelectType(EUIHeroSelectType InCurSelectType);

	/* Change the selection status of a hero. */
	void SetHeroSelected(int32 HeroIndex, EUIHeroSelectType SelectType);

	void SetMainSelectCharacter(EPCClass type, EUIHeroSelectType SelectType);
	void SetMainSelectType(EUIHeroSelectType SelectType, EUIHeroSelectModeType ModType);
	void SetGuideText(ESlateVisibility GuideText);

public:
	/* attackpower is increase or decrease? */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelect")
		void SetTotalAttackIncreaseAmount(int32 IncreaseAmount);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterSelect")
		void PlayCharacterChangeSoundBP();

	void SetStartBattleCallback(const FStartBattleCallBack& Handler);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	//The newly updated with the data of the selected hero.
	void UpdateNewHerosData();

	UFUNCTION()
		void OnClickBtnStartBattle();
	UFUNCTION()
		void OnClickBtnConfirm();
	UFUNCTION()
		void OnClickBtnClose();
	
	void OnFinalSelectionConfirmCommon();

	UFUNCTION()
		void OnClickBtnHero(class UB2UICharacterSelectItem* ClickedHero);

	UFUNCTION()
		void OnClickBtnMainHero();
	UFUNCTION()
		void OnClickBtnSubnHero();

public:
	/* Name rules for obtaining BP in the code. */
	UPROPERTY(EditAnywhere, Category = "BladeII CharacterSelect")
		FString PrefixHeroItem;

protected:
	TWeakObjectPtr<UButton>				BTN_StartBattle;
	TWeakObjectPtr<UButton>				BTN_Confirm;
	TWeakObjectPtr<UButton>				BTN_Close;
	TWeakObjectPtr<UButton>				BTN_Modal;
	TWeakObjectPtr<UTextBlock>			TB_HerosTotalAttack;
	TWeakObjectPtr<UTextBlock>			TB_HerosTotalDefense;
	TWeakObjectPtr<UTextBlock>			TB_NewHerosTotalAttack;
	TWeakObjectPtr<UTextBlock>			TB_NewHerosTotalDefense;
	TWeakObjectPtr<UTextBlock>			TB_HerosTotalCombat;
	TWeakObjectPtr<UTextBlock>			TB_NewHerosTotalCombat;
	TWeakObjectPtr<UOverlay>			O_TouchHero1;
	TWeakObjectPtr<UOverlay>			O_TouchHero2;

	TWeakObjectPtr<UImage>				IMG_MainCharacter;
	TWeakObjectPtr<UImage>				IMG_SubCharacter;
	TWeakObjectPtr<UTextBlock>			TBS_MainCharacterText;
	TWeakObjectPtr<UTextBlock>			TBS_SubCharacterText;
	TWeakObjectPtr<UTextBlock>			TBS_MainText;
	TWeakObjectPtr<UTextBlock>			TBS_SubText;
	TWeakObjectPtr<UTextBlock>			TBS_SubTitle;
	TWeakObjectPtr<UTextBlock>			TB_Guide;
	TWeakObjectPtr<UButton>				BTN_SubCharacter;
	TWeakObjectPtr<UButton>				BTN_MainCharacter;

	TWeakObjectPtr<UWidgetSwitcher>		WS_SelectMainTarget;
	TWeakObjectPtr<UWidgetSwitcher>		WS_SelectSubTarget;


	TArray<class UB2UICharacterSelectItem*> HeroItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		FLinearColor	DarkColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		FLinearColor	WhiteColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<UMaterialInterface*>	CharterTexture;

	FStartBattleCallBack StartBattleDelegate;


private:
	/* The currently active HeroDocument. */
	class UB2UIDocHero*						MainHeroDoc;
	class UB2UIDocHero*						SubHeroDoc;

	/* Hero you want to change. */
	class UB2UIDocHero*						NewMainHeroDoc;
	class UB2UIDocHero*						NewSubHeroDoc;

	/* Ongoing character selection state. */
	EUIHeroSelectType CurSelectType;

	/* Cache textblock's color of attack or defense. (Used to restore the color has been changed.)*/
	FSlateColor TextColor_OriginValue;
};
