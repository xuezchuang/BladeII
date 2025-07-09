// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UISeasonEventCombineMenuBtn.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UISeasonEventCombineMenuBtn : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void RegisterUIMarkForRedDot() final;
	void SubscribeEvents();
	void UnsubscribeEvents();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

protected:
	UFUNCTION()
	void OnClickBTN_SeasonEvent();

	UFUNCTION()
	bool HasRedDot_SeasonEvent();

public:
	void SeasonEventVisbleCheck();
	bool IsEnableDateSeasonEvent();
	void InitEventIDSeasonEvent();

private:
	TWeakObjectPtr<UB2Button>	BTN_SeasonEvent;

	TWeakObjectPtr<UTextBlock>	TB_SeasonEvent;

	int32						SeasonEventID;

private:
	bool						bSubscribed;
	TArray<issue_ptr>			Issues;
};
