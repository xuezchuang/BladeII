// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIPQuestList.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPQuestList : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
		TSubclassOf<class UB2UIPQuestSlot> QuestSlotClass;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void UpdateScroll(TArray<b2network::B2mdMainQuestInfoPtr>& QuestDatas, int32 CurrentQuestID);

protected:
	virtual void CacheAssets() override;

private:
	TArray<TWeakObjectPtr<class UB2UIPQuestSlot>>				QuestSlots;
	TWeakObjectPtr<UB2ScrollBox>								X_SB_QuestScrollBox;
};
