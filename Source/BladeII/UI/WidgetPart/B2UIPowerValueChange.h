// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIPowerValueChange.generated.h"


UCLASS()
class BLADEII_API UB2UIPowerValueChange : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetValueAndPlay(int32 ChangeAmount);

	void SetValueOnlyImage(bool bIsUp);
protected:

private:
	TWeakObjectPtr<UCanvasPanel> C_StatUp;
	TWeakObjectPtr<UTextBlock> TB_CountUp;
	TWeakObjectPtr<UCanvasPanel> C_StatDown;
	TWeakObjectPtr<UTextBlock> TB_CountDown;

	TWeakObjectPtr<UWidgetAnimation> Anim_StatUp;
	TWeakObjectPtr<UWidgetAnimation> Anim_StatDown;

};
