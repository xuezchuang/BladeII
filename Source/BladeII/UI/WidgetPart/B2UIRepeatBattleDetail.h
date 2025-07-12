// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIRepeatBattleDetail.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRepeatBattleDetail : public UB2UIWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION()
		void CheckStateChanged_1(bool bCheck);
	UFUNCTION()
		void CheckStateChanged_2(bool bCheck);
	UFUNCTION()
		void CheckStateChanged_3(bool bCheck);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	UFUNCTION()
		void OnClickBtnOk();
	UFUNCTION()
		void OnClickBtnClose();
	TWeakObjectPtr<UB2Button>						BTN_Close;
	TWeakObjectPtr<UB2Button>						BTN_Ok;
	TWeakObjectPtr<UCheckBox>						CB_Sub_Detail_1;
	TWeakObjectPtr<UCheckBox>						CB_Sub_Detail_2;
	TWeakObjectPtr<UCheckBox>						CB_Sub_Detail_3;
	TWeakObjectPtr<UTextBlock>						TB_RepeatBattleBoostCount;
	
	TWeakObjectPtr<UTextBlock>						TB_Title;
	TWeakObjectPtr<UTextBlock>						TB_Detail;
	TWeakObjectPtr<UB2RichTextBlock>				TB_Sub_Detail_1;
	TWeakObjectPtr<UB2RichTextBlock>				TB_Sub_Detail_2;
	TWeakObjectPtr<UB2RichTextBlock>				TB_Sub_Detail_3;
	TWeakObjectPtr<UTextBlock>						TB_Sub_Title_1;
	TWeakObjectPtr<UTextBlock>						TB_Sub_Title_2;
	TWeakObjectPtr<UTextBlock>						TB_Sub_Title_3;
};
