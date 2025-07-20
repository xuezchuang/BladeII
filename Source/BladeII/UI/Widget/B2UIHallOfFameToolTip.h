// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIHallOfFameToolTip.generated.h"


UCLASS()
class BLADEII_API UB2UIHallOfFameToolTip : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	UFUNCTION()
	void OnClickBTN_ClosePraiseToolTip();

private:
	TWeakObjectPtr<UB2RichTextBlock> TB_ToolTip1;
	TWeakObjectPtr<UB2RichTextBlock> TB_ToolTip2;
	TWeakObjectPtr<UB2Button> BTN_ClosePraiseToolTip;
};
