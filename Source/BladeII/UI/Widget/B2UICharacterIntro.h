// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "BladeIIPlayer.h"
#include "B2UICharacterIntro.generated.h"

/*
* This is the characterIntro that appears after the login
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICharacterIntro : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UB2UICharacterIntro(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	/* The processing that is required when the character is selected.
	@param CharType : Selected Class 
	@parma bCancelEffectOpeningSkillPV : Whether to cancel the animation SkillPV.*/
	void SelectCharacter(EPCClass CharType, bool bCancelEffectOpeningSkillPV = false);
	void SetTitle(const FText& InText);
	void SetSelectedCharDesc(const FText& InText);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII CharacterIntro")
		void SelectCharacter_BP(EPCClass CharType, bool bCancelEffectOpeningSkillPV = false);

private:
	/** Like a time offset to feed to the CharIntroFlipbook MID. The flipbook anim will start from the first frame if reset this to current time. */
	float CharIntroFlipbookMIDTimeSync;
	FTimerHandle CharIntroFlipbookResetDelayTH;


private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void WrappedResetCharIntroFlipbookAnim();	// To handle CharIntroFlipbookResetDelay
	void ResetCharIntroFlipbookAnim();			// Reset to start from the first frame.
	void UpdateCharIntroFlipbookMID();

	UFUNCTION()
		void OnClickBtnConfirm();
	UFUNCTION()
		void OnClickBtnGladiator();
	UFUNCTION()
		void OnClickBtnAssassin();
	UFUNCTION()
		void OnClickBtnWizard();
	UFUNCTION()
		void OnClickBtnFighter();


protected:
	TWeakObjectPtr<UButton>			BTN_Confirm;
	TWeakObjectPtr<UButton>			BTN_Gladiator;
	TWeakObjectPtr<UButton>			BTN_Assassin;
	TWeakObjectPtr<UButton>			BTN_Wizard;
	TWeakObjectPtr<UButton>			BTN_Fighter;
	TWeakObjectPtr<UTextBlock>		TB_Title;
	TWeakObjectPtr<UTextBlock>		TB_SelectedCharDesc;
	TWeakObjectPtr<UImage>			IMG_IntroFlipbook;

	//static text
	TWeakObjectPtr<UTextBlock>		TB_Gladiator;
	TWeakObjectPtr<UTextBlock>		TB_Assassin;
	TWeakObjectPtr<UTextBlock>		TB_Wizard;
	TWeakObjectPtr<UTextBlock>		TB_Fighter;
	TWeakObjectPtr<UTextBlock>		TB_Stat_Physical;
	TWeakObjectPtr<UTextBlock>		TB_Stat_Attack;
	TWeakObjectPtr<UTextBlock>		TB_Stat_Defense;
	TWeakObjectPtr<UTextBlock>		TB_BTNConfirm;

	/** Time to delay the flipbook anim reset from the moment of character button clicking.
	* This is needed due to animation on character button click. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		float CharIntroFlipbookResetDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		FSlateColor NameTextColor_Selected;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		FSlateColor NameTextColor_UnSelected;

	/** How much bigger the name text will become when it is selected? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		float NameTextSelectScale;



private:
	class UB2UIDocHero*				SelectedHeroDoc;

	/** The flipbook animation material instance, introducing each character like a video.
	* UMG script is responsible for setting this at runtime, whenever character selection is changed. */
	class UMaterialInstanceDynamic* CurrCharIntroFlipbookMID;
};

void SetFirstCharacterViewState(int32 InState);
