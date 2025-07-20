// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "B2UIBattleCombo.generated.h"

/*
* This represents each of the object skill. In battle.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBattleCombo : public UB2UIWidgetBase, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UIBattleCombo(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	void BindDoc(class UB2UIDocBattle* BattleDoc);
	void BindDoc(class UB2UIDocPVP1on1Rival* BattleDoc);
	virtual void UnbindDoc() override;

	/** To do some animation right at the moment of combonum change, and for any other possible effect. */
	void SetCombo(int32 SuccessiveComboNum);
	void SetComboExpireTime(float ComboExpireProgress);


public:
	/** Stems from TargetDamaged event, especially when the ComboPraiseText is just switched.
	* Mainly to handle ComboPraiseText animation. */
	UFUNCTION(BlueprintImplementableEvent, Category="BladeII BattleCombo")
		void ComboPraiseTextSwitched(int32 SuccessiveComboNum, int32 SwitchedIndex);

	/** Widget BP notification of Combo event */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleCombo")
		void SetCombo_BP(int32 SuccessiveComboNum, bool bPlayCounterGameMode);

public:
	//////////////////////////////////////////////////////////////////////
	// For combo praise text image including Widget BP editable properties

	/** At which interval combo praise text will be switched to next phrase. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComboPraiseText")
		int32 ComboPraiseTextSwitchInterval;

	/** The combo number that the combo praise text will show the least praise. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComboPraiseText")
		int32 ComboPraiseTextMinLevel;

	/** The combo number that the combo praise text will show the best praise. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComboPraiseText")
		int32 ComboPraiseTextMaxLevel;

	/** How long the combo num text will be shown after the recent switch? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComboPraiseText")
		float ComboPraiseTextShowDuration;

	uint32 bComboPraiseTextVisibility : 1;

	FTimerHandle ComboPraiseTextShowTimerHandle;

	//////////////////////////////////////////////////////////////////////


private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	/** Update material parameters to display combo number to current state. */
	void UpdateComboNumMaterial(float ExpireProgress);

	void ComboPraiseTextShowTimerCB();

	//=================================================== Btn

	//==================================================== Doc Delegate
	UFUNCTION()
		void OnChangedComboNum(class UB2UIDocBase* Sender, int32 ComboNum, int32 PrevComboNum);
	UFUNCTION()
		void OnChangedComboExpireProgress(class UB2UIDocBase* Sender, float ComboExpireProgress, float PrevComboExpireProgress);
	UFUNCTION()
		void OnChangedCriticalRateByComboNum(class UB2UIDocBase* Sender, float NewAdditionalRate, float PrevAdditionalRate);
	UFUNCTION()
		void OnChangedIncCounterDamageCombo(class UB2UIDocBase* Sender, float NewIncCounterDamageCombo, float PrevIncCounterDamageCombo);

protected:
	TWeakObjectPtr<UImage>					IMG_ComboNum;
	/** The widget switcher containing all available combo praise text image. UMG script is responsible for setting this at runtime. */
	TWeakObjectPtr<UWidgetSwitcher>			WS_ComboPraiseTextImage;

	TWeakObjectPtr<UTextBlock>				TB_AddCriticalRateByCombo;

	/** UMG script is responsible for setting this at runtime. */
	class UMaterialInstanceDynamic*			ComboNumDisplayMID;

private:
	/** Limit that material can express */
	const int32 COMBONUM_DISPLAY_LIMIT = 999;

	int32 CurComboNum = 0;

	bool bIsCounterGameMode = false;
};
