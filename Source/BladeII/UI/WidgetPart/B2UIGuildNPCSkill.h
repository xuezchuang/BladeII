// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "B2GuildNPCInfo.h"
#include "B2UIGuildNPCSkill.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildNPCSkill : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIGuildNPCSkill(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetNPCSkill(class ABladeIIMercenaryPlayer* InMercenaryPlayer);

	void SetSkillName(const FText& InText);
	void SetMercenaryName(const FText& InText);
	void SetButtonStyle(const FGuildMercenarySkill* InSkillData);
	void SetCooltime(float InValue, float MaxCoolTime);
	void SetCooltimeVisible(bool bVisible);
	void SetCooltimeTextColor(const FSlateColor& InColor);
	void SetEnabledBtn(bool bEnabled);
	void SetLock(bool bLock);

	bool IsActivatable();
	bool IsAbnormalState();
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII GuildNPCSkill")
		void SetEmpty(bool bEmptySkillSlot);
	
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void UpdateCooltimeMaterial(float RemainingCooltime, float MaxCoolTime);

	//=================================================== Btn
	UFUNCTION()
		void OnPressedBtnArea();

protected:
	TWeakObjectPtr<UTextBlock>			TB_SkillName;
	TWeakObjectPtr<UTextBlock>			TB_SkillCooltimeNum;
	TWeakObjectPtr<UImage>				IMG_SkillCooltime;
	TWeakObjectPtr<UB2Button>			BTN_Area;
	TWeakObjectPtr<UB2Button>			BTN_Area2;
	TWeakObjectPtr<UImage>				IMG_Lock;
	TWeakObjectPtr<UTextBlock>			TB_NPCName;
	
	UPROPERTY(Transient)
	class UMaterialInstanceDynamic*		MID_SkilCooltime;

	UWidgetAnimation*			Skill_On;

	UPROPERTY(Transient)
	class ABladeIIMercenaryPlayer* MercenaryPlayer;

	TSharedPtr<struct FMercenarySkillData> UseSkillData;

private:
	bool CacheEnableSkill;
	bool bDead;
};
