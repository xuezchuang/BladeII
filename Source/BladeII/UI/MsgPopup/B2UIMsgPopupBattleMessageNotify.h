// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIMsgPopupBattleMessageNotify.generated.h"


UCLASS()
class BLADEII_API UB2UIMsgPopupBattleMessageNotify : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	void SetMessage(const FString& msg, const float duration);
	void TimeEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimation_IMP(bool bIsOpen);

private:
	TWeakObjectPtr<UTextBlock> TB_Title;
	FTimerHandle	TimerHandle;
};
