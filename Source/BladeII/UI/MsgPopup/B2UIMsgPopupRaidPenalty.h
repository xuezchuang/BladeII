// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupRaidPenalty.generated.h"


UCLASS()
class BLADEII_API UB2UIMsgPopupRaidPenalty : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	UB2UIMsgPopupRaidPenalty(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void StartTimer(int32 nSecond);
protected:
	void SetRemainTimeText(int32 nSecond);
	void CheckLimittime();

	UFUNCTION()
	void OnPlatformResume();
private:
	TWeakObjectPtr<UB2RichTextBlock> TB_AddComment;
	FTimerHandle LeftTimeHandler;
	int32 nCurrentLeftTime;
};
