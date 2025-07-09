// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupBase.h"
#include "B2UIPlayerSkillInfo.h"
#include "B2UIGuildMercenary.h"
#include "B2UIMsgPopupTooltipSkillOption.generated.h"

/*
* The base class of messaged popup. it has two buttons.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupTooltipSkillOption : public UB2UIMsgPopupBase
{
	GENERATED_BODY()

public:
	UB2UIMsgPopupTooltipSkillOption(const FObjectInitializer& ObjectInitializer);

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetSkillID(int32 SkillId);
	void SetUnitySkillID(int32 SkillId, EPCClass eMainClass);

	int32 GetSkillID();

	void SetSkillLevel(int32 InSkillLv, int32 InSkillMaxLevel);
	void SetSkillType(ESkillType InSkillType);


	//MercenarySkill
	void SetMercenarySkillID(int32 InSkillID);
	void SetMercenarySpecialSKillID(int32 InSkillID, int32 InLv, ENPCSpecialSKill InSkillType);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2UISkillOptionList> SkillOptionListClass;

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
	void OnClickBtnClose();

	bool HasNextSpecialDisplayOption(int32 InSkillLv, int32 InCurrentSkillLevel, int32& SpecialOptionLevel);

public:


protected:
	TWeakObjectPtr<UButton>		BTN_Close;
	TWeakObjectPtr<UImage>		IMG_SkillIcon;
	TWeakObjectPtr<UTextBlock>	TB_SkillName;
	TWeakObjectPtr<UTextBlock>	TB_SkillDesc_Text;
	TWeakObjectPtr<UVerticalBox> VB_OptionList;
	TWeakObjectPtr<UOverlay>	OverlayOptionList;

	TWeakObjectPtr<UImage>			IMG_Skill_Badge;
	TWeakObjectPtr<UTextBlock>		TB_SkillType;

	TWeakObjectPtr<UTextBlock>		TB_SkillLevel;
	TWeakObjectPtr<UTextBlock>		TB_SkillMaxLevel;

	TWeakObjectPtr<UPanelWidget>	P_Skill_Badge; 
	TWeakObjectPtr<UHorizontalBox>	HB_SkillLevel;

	UPROPERTY(EditAnywhere)
		TArray<FSkillBadgeImage> SkillBadgeImages;

	UPROPERTY(EditAnywhere)
		FSlateColor SkillLevelColor;
	
	int32 CurrentSkillID;

};