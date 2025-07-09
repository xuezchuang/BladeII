// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIQuestSlot.h"
#include "B2UIQuestNoticeSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIQuestNoticeSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

	TWeakObjectPtr<UImage>				IMG_ProgressState;
	TWeakObjectPtr<UTextBlock>			TB_ProgressState;
	TWeakObjectPtr<class UB2UIQuestSlot> UIP_QuestSlot;

public:

	virtual void Init() override;
	virtual void CacheAssets() override;

	void UpdateProcessImage(int32 ProcessState);

	void InitQuestNoticeSlot(int32 SlotID, int32 QuestState);
	
	TWeakObjectPtr<UB2UIQuestSlot>	GetQuestSlot()
	{
		return UIP_QuestSlot.IsValid() ? UIP_QuestSlot : nullptr;
	}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> ProcessMaterials;
};
