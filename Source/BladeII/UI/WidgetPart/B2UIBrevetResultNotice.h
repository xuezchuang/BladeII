// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBrevetResultNotice.generated.h"

DECLARE_DELEGATE(FUB2UIBrevetResultNotice_FinishNoticeAni);

UCLASS()
class BLADEII_API UB2UIBrevetResultNotice : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void InitResultNotice(EBrevetStatusType StatusType, int32 StatusValue);
	void StartNoticeAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void StartNoticeAnimation_BP();

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
	void OnFinishAnimationEvent_NoticeAnimation_BP();

public:
	FUB2UIBrevetResultNotice_FinishNoticeAni FFinishNoticeDelegate;

private:
	TWeakObjectPtr<UTextBlock> TB_SuccessKey;
	TWeakObjectPtr<UTextBlock> TB_SuccessValue;
};
