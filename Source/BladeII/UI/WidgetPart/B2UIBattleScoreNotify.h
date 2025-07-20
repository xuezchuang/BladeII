// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIUtil.h"
#include "B2UIBattleScoreNotify.generated.h"

UCLASS()
class BLADEII_API UB2UIBattleScoreNotify : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void UpdateStaticText() override;

	void SetAmount(int32 NewValueAmount, int32 beforeValueAmount, EPCClass ClassType = EPCClass::EPC_End);

protected:
	void SetMarginScore(int32 nMarginScore);
	void SetBattleScore(int32 NewValueAmount);

	void UpdateValueAmount();
	void ClosePanel(float InDeltaTime);

protected:
	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> PortraitInfo;

private:
	TWeakObjectPtr<UTextBlock> TB_CombatPower;
	TWeakObjectPtr<UTextBlock> TB_CombatLable;
	TWeakObjectPtr<class UB2UIPowerValueChange> UIP_PowerValueChange;
	TWeakObjectPtr<UImage> IMG_CharacterPortrait;

	int32 m_nOrgValueAmount;
	int32 m_nCurValueAmount;
	int32 m_nGoalValueAmount;
	float m_fValueChangeStartTime;
	float ValueActionTime;
	float m_endTime;

	bool bIsActive = false;
};
