// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIFindMatchPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFindMatchPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION()
		void OnCancleBtnClick();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_Anim_Completed();

	void SetPlayerCount(int32 InCount);
	void OnCancleBtnClickInner();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

private:
	TArray<TWeakObjectPtr<UWidgetSwitcher>> CharacterSlots;
	TWeakObjectPtr<UButton> BTN_Cancle;
	TWeakObjectPtr<UTextBlock> TB_PlayerCount;
};
