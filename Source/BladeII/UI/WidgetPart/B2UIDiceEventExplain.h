// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDiceEventExplain.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDiceEventExplain : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
	void OnClickBTN_Tip();

	void OpenDiceEventExplainPopup();

	void UpdateEventExplain(int32 iEventID, int32 iFinishCount);
	void UpdateRemaningTimeText();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIDiceEventExplainPopup> UIDiceEventExplainPopupClass;

protected:
	TWeakObjectPtr<UTextBlock>		TB_RemaningTimeExplain;
	TWeakObjectPtr<UTextBlock>		TB_RemaningTime;
	
	TWeakObjectPtr<UB2Button>		BTN_Tip;
	TWeakObjectPtr<UTextBlock>		TB_BtnTip;

	TWeakObjectPtr<class UB2UIDiceEventExplainPopup>	pDiceEventExplainPopup;

private:
	int32 CachedEventID;
	int32 CachedFinishCount;
};
