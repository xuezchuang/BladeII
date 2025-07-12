// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "../../BladeII/BladeIIPlayer.h"
#include "B2UIBattleSkill.generated.h"

/* This is when you click on informing purpose. */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBattleSkillClickedDelgate, UB2UIBattleSkill*, ClickedSkill);

/*
* This represents each of the object skill. In battle.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBattleSkill : public UB2UIWidgetBase, public IB2UIDocBindable
{
	GENERATED_BODY()

public:

	UB2UIBattleSkill(const FObjectInitializer& ObjectInitializer);

	void BindDoc(class UB2UIDocSkill* SkillDoc);
	virtual void UnbindDoc() override;

	void SetLevel(int32 InValue);
	void SetName(const FText& InText);
	void SetButtonStyleByID(int32 InSkillID);
	void SetCooltime(float InValue);
	void SetCooltimeVisible(bool bVisible);
	void SetCooltimeTextColor(const FSlateColor& InColor);
	void SetEnabledBtn(bool bEnabled);
	virtual void SetSkillPointStack(float MaxPoint, float CurPoint) {}
	void SetLock(bool bLock);

	/* It is for storing the index for convenience. */
	void SetIndex(int32 InValue)	{ Index = InValue; }
	int32 GetIndex() const			{ return Index; }
	
	bool IsActivatable();

	bool IsSurviveSkill();
	bool IsWeaponSkill();
	
	FORCEINLINE bool IsFullStack() const { return bFullStackSkillPoint; }
	FORCEINLINE bool IsInCooltime() const { return bInCoolTime; }

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleSkill")
		void SetEmpty(bool bEmptySkillSlot);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleSkill")
		void PlayFullStackSkillPoint();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleSkill")
		void StopFullStackSkillPoint();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleSkill")
		void EtherActivated(int32 EhterSetID);

public:
	/* This is when you click on informing purpose. */
	FOnBattleSkillClickedDelgate  OnBattleSkillClicked;


private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	const FButtonStyle& GetSkillButtonStyle(int32 InSkillID);

	void UpdateCooltimeMaterial(float RemainingCooltime);

	//=================================================== Btn
	UFUNCTION()
		void OnPressedBtnArea();

	//==================================================== Doc Delegate
	UFUNCTION()
		void OnChangedIsCountingCooltime(class UB2UIDocBase* Sender, bool IsCountingCooltime, bool PrevIsCountingCooltime);
	UFUNCTION()
		void OnChangedIsDisabledByCooltime(class UB2UIDocBase* Sender, bool IsDisabledByCooltime, bool PrevIsDisabledByCooltime);
	UFUNCTION()
		void OnChangedIsDisabledByDebuff(class UB2UIDocBase* Sender, bool IsDisabledByCooltime, bool PrevIsDisabledByCooltime);
	UFUNCTION()
		void OnChangedRemainingCooltime(class UB2UIDocBase* Sender, float RemainingCooltime, float PrevRemainingCooltime);
	UFUNCTION()
		void OnChangedSkillPoint(class UB2UIDocBase* Sender, float CurrPoint, float PrevPoint);
	UFUNCTION()
		void OnChangedIsExistingSkill(class UB2UIDocBase* Sender, bool CurrExistingSkill, bool PrevExistingSkill);

	UFUNCTION()
	void OnChangedSuggetSkill(class UB2UIDocBase* Sender, bool CurrSuggest, bool PrevSuggest);

	UFUNCTION()
	void OnChangedDisabledByOtherSuggest(class UB2UIDocBase* Sender, bool CurrDisable, bool PrevDisable);

	void OnEtherActivated(int32 EtherSetID, ABladeIIPlayer* Owner, ABladeIICharacter* Causer);

public:
	UPROPERTY(EditAnywhere)
		FSlateColor SkillCoolTimeTextColor;
	/** The color of skill cool time text, when it is not counting yet (i.e. while skill is actively being invoked) */
	UPROPERTY(EditAnywhere)
		FSlateColor SkillCoolTimeTextPendingCountColor;

	void GetAreaBTNs(TArray<TWeakObjectPtr<UB2Button>>& OutAreaBtns);
	void GetButtonStyle(struct FButtonStyle& OutStyle);
	
protected:
	TWeakObjectPtr<UTextBlock>			TB_SkillName;
	TWeakObjectPtr<UTextBlock>			TB_SkillLevel;
	TWeakObjectPtr<UTextBlock>			TB_SkillCooltimeNum;
	TWeakObjectPtr<UImage>				IMG_SkillCooltime;
	TWeakObjectPtr<UB2Button>			BTN_Area;
	TWeakObjectPtr<UB2Button>			BTN_Area2;
	TWeakObjectPtr<UImage>				IMG_Lock;
	TWeakObjectPtr<UImage>				IMG_PointStack;

	UPROPERTY(Transient)
	class UMaterialInstanceDynamic*		MID_SkilCooltime;

	TWeakObjectPtr<UImage>				IMG_FX_SuggestSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface*			SurviveSuggestMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface*			DefaultSuggestMaterial;

	UWidgetAnimation*			Skill_On;

	bool bFullStackSkillPoint;

	bool bCurrentSuggest;

private:
	int32 Index = INDEX_NONE;

	bool bIsCountingCooltime;
	bool bInCoolTime;

	bool bOwnerHasDebuff;


	uint32 OnEtherActivatedTicket;
};
