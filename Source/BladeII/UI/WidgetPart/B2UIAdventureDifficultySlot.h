// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "B2UIWidgetBase.h"
#include "B2UIAdventureDifficultySlot.generated.h"

DECLARE_DELEGATE_OneParam(UB2UIAdventureDifficultySlot_OnClickBTNSelect, EStageDifficulty)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIAdventureDifficultySlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
	void OnClickButton();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void SetDifficulty_BP(int32 idx);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void SetUnLock_BP(bool isPlayAnimation);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void SetSelect_BP(bool isSelect);

public:
	void InitDifficulty(EStageDifficulty CurStageDifficulty, EStageDifficulty MaxStageDifficulty);
	void SetUnlock(bool isUnLock, bool animation);	
	void SetSelectSlot(bool isSelect);
	void UpdateSlot(EStageDifficulty CurSelectDifficulty);
	void SetDelegate(UB2UIAdventureDifficultySlot_OnClickBTNSelect OnClick_BTNSelect);

protected:
	TWeakObjectPtr<UB2Button>				BTN_Select;
	TWeakObjectPtr<UOverlay>				O_LockOverlay;
	TWeakObjectPtr<UOverlay>				O_SelectOverlay;

	TWeakObjectPtr<UTextBlock>				TB_Unlock1;
	TWeakObjectPtr<UTextBlock>				TB_Unlock2;
	TWeakObjectPtr<UTextBlock>				TB_Unlock3;
	TWeakObjectPtr<UTextBlock>				TB_Reward1;
	TWeakObjectPtr<UTextBlock>				TB_Reward2;
	TWeakObjectPtr<UTextBlock>				TB_Reward3;

private:
	EStageDifficulty						SlotDifficulty;
	UB2UIAdventureDifficultySlot_OnClickBTNSelect	FOnClickBTNSelect;
};
	
	