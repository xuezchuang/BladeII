// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIPSubQuest.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPSubQuest : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void CurrentTabUpdateScroll();

protected:
	virtual void CacheAssets() override;

private:
	void UpdateScroll(int32 InTabType);
	int32 GetCurrentDifficulty();
	TWeakObjectPtr<class UB2UIPQuestList> UIP_QuestList;
	TMap<int32, TWeakObjectPtr<class UB2UIPSubQuestTab>> Tabs;
	int32 TabType;
};
