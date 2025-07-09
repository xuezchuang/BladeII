// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIPMainQuest.generated.h"

/**
 * 
 */


UCLASS()
class BLADEII_API UB2UIPMainQuest : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tab")
		TSubclassOf<class UB2UIPMainQuestTab> QuestTabClass;

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void CurrentTabUpdateScroll();
	
protected:
	virtual void CacheAssets() override;

private:
	void UpdateScroll(int32 InTabType);
	int32 GetCurrentActID();
	int32 GetQuestID();

	TMap<int32, TWeakObjectPtr<class UB2UIPMainQuestTab>> QuestTabs;
	TWeakObjectPtr<class UB2UIPQuestList> UIP_QuestList;
	TWeakObjectPtr<class UB2ScrollBox> SB_TabList;
};
