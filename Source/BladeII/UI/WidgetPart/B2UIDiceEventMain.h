// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDiceEventMain.generated.h"

UCLASS()
class BLADEII_API UB2UIDiceEventMain : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
	void OnClickBTN_DiceRoll();

	void UpdateDiceEventMain(int32 iEventID, int32 iFreeDiceCount, int32 iGetDicePoint, int32 iRolledDiceCount, int32 iFinishCount);
	void UpdateDynamicText(int32 iGetDicePoint, int32 iRolledDiceCount, int32 iFinishCount);
	void UpdateProgressBarGetDicePoint(int32 iGetDicePoint);
	void UpdateDiceRollBtn(int32 iFreeDiceCount, int32 iGetDicePoint);

private:
	int32 GetProgressGetDicePoint();
	int32 GetPossessionDiceCount();
	bool IsFreeDice() { return CachedIsFreeDice > 0 ? true : false; }
	int32 GetDicePointMax();
	int32 GetDiceFreeCountMax();

	void OnDelayDiceRollCallback();

protected:
	TWeakObjectPtr<UB2Button>		BTN_DiceRoll;
	TWeakObjectPtr<UTextBlock>		TB_BtnDiceRoll;

	TWeakObjectPtr<UTextBlock>		TB_EventExplain;

	TWeakObjectPtr<UTextBlock>		TB_RolledDiceExplain;
	TWeakObjectPtr<UTextBlock>		TB_RolledDiceCount;
	
	TWeakObjectPtr<UProgressBar>	PB_ProgressGetDicePoint;
	TWeakObjectPtr<UTextBlock>		TB_ProgressGetDicePoint;
	TWeakObjectPtr<UTextBlock>		TB_PossessionDiceCount;

	TWeakObjectPtr<UTextBlock>		TB_FinishCount;

private:
	int32	CachedEventID;
	int32	CachedIsFreeDice;
	int32	CachedGetDicePoint;

	FTimerHandle DelayCallbackTH;
};
