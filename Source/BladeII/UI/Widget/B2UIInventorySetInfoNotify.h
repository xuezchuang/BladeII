// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIInventorySetInfoNotify.generated.h"

enum class ESetItemEffectNotify
{
	SIEN_None,
	SIEN_Add,
	SIEN_Delete,
	SIEN_Change,
};

UCLASS()
class BLADEII_API UB2UIInventorySetInfoNotify : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateValueAmount();
	void ClosePanel(float InDeltaTime);

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetItemEffectType(ESetItemEffectNotify SetItemEffectType, const bool& InIsCostumeItem = false);

protected:
	float m_fValueChangeStartTime;
	float ValueActionTime = 1.0f;
	float m_endTime;


private:
	TWeakObjectPtr<UTextBlock> TB_CombatLable;
};
