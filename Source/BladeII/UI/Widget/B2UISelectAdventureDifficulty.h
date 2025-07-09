// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "CommonStruct.h"
#include "B2UIAdventureDifficultySlot.h"
#include "B2UISelectAdventureDifficulty.generated.h"

/**
 * UB2UISelectAdventureDifficulty
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISelectAdventureDifficulty : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void BindDelegates() override;
	
protected:	
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

public:
	UFUNCTION()
	void OnClickBtnConfirm();
	UFUNCTION()
	void OnClickBtnClose();

	UFUNCTION()
	void OnChangeDifficulty(class UB2UIDocBase* Sender, EStageDifficulty diff, EStageDifficulty PrevDiff);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "AdventureModSelect")
	void SetDifficulty_BP(int32 Idx);

public:
	void UpdateSlot(EStageDifficulty UpdateDifficulty);

private:
	void SetDifficulty(EStageDifficulty Difficulty);

	// widget caches
protected:
	TWeakObjectPtr<UButton>		BTN_Confirm;
	TWeakObjectPtr<UButton>		BTN_Close;
	TArray<TWeakObjectPtr<class UB2UIAdventureDifficultySlot>> UIP_SlotList;
};
