// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2DynItemIcon_SealBoxPreview.h"
#include "B2UISealBoxResultSimple.generated.h"

/**
 *
 */
UCLASS()
class BLADEII_API UB2UISealBoxResultSimple : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	void SetRewardDataManually(const FB2ResponseUnsealBoxPtr& UnsealBoxPtr, int32 pcClass);
	void ContinueResultShowing();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;

	UFUNCTION()
	void OnClickedBTNOneMoreTime();
	UFUNCTION()
	void OnClickedBTNClose();

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<UB2DynItemIcon_SealBoxPreview>	resultWidget;

private:
	FB2ResponseUnsealBoxPtr UnsealData;
	int32 savedClass;
	int32 currentArrayIndex;

private:
	TWeakObjectPtr<UTextBlock>		TB_ResultTitle;

	TWeakObjectPtr<UB2ScrollBox>	SB_ResultScroll;

	TWeakObjectPtr<UB2Button>		BTN_OneMoreTime;
	TWeakObjectPtr<UTextBlock>		TB_OneMoreTime;
	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UTextBlock>		TB_Close;
};
