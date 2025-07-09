// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIPQuestSlot.generated.h"


enum class EQuestSlotType : uint8
{
	Ongoing = 0,
	Complete,
	Lock,
	Reward
};

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPQuestSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	void SetInfo(b2network::B2mdMainQuestInfoPtr InQuestInfoPtr, EQuestSlotType InType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, TSoftObjectPtr<UTexture2D>>	NPCIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CompressTextLimitLength;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	UFUNCTION()
		void OnClickLinkButton();

	void SetTypeUI(EQuestSlotType InType);
	void SetQuestInfo(int32 QuestID, EQuestSlotType InType);

	TWeakObjectPtr<UImage> IMG_Portrait;
	TWeakObjectPtr<UPanelWidget> PW_RewardFrame;
	TWeakObjectPtr<UB2RichTextBlock> RTB_QuestName;
	TWeakObjectPtr<UB2RichTextBlock> RTB_QuestType;
	TWeakObjectPtr<UB2RichTextBlock> RTB_QuestInfo;
	struct QeustSlotRewardIcon
	{
		TWeakObjectPtr<UPanelWidget> Panel;
		TWeakObjectPtr<class UB2UIRewardIcon> Icon;
	};
	TArray<QeustSlotRewardIcon> Icons;
	TWeakObjectPtr<UPanelWidget> PW_Link;
	TWeakObjectPtr<UTextBlock> TB_Link;
	TWeakObjectPtr<UPanelWidget> PW_Reward;
	TWeakObjectPtr<UTextBlock> TB_Reward;
	TWeakObjectPtr<UPanelWidget> PW_Completed;
	TWeakObjectPtr<UB2Image> IMG_Lock;
	TWeakObjectPtr<UImage> BG_Disabled;
	TWeakObjectPtr<UImage> BG_Active;
	TWeakObjectPtr<UB2Button> BTN_Link;
	int32 CurrentQuestType;
};
