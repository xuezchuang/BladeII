// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "Quest.h"
#include "B2UIQuestSlot.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EQuestSlotState : uint8
{
	QuestOpen,
	QuestHideAll,
	QuestSimple,
	QuestDefault,
};

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	None,
	InProgress,
	New,
	Complete,
};

UENUM()
enum class EQuestDetailType : uint8
{
	DetailLink,
	LeftTimeView,

	DetailMax
};

UCLASS()
class BLADEII_API UB2UIQuestSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UImage> IMG_NPCICon;
	TWeakObjectPtr<UImage> IMG_QuestProgress;

	TWeakObjectPtr<class UB2RichTextBlock> RTB_QuestDistrict;

	TWeakObjectPtr<UB2RichTextBlock> RTB_QuestSummary;
	TWeakObjectPtr<UB2RichTextBlock> RTB_QuestProgress;
	TWeakObjectPtr<UB2RichTextBlock> RTB_LeftTime;

	TWeakObjectPtr<UB2Button> BTN_QuestDetailLink;	// QuestOpen상태에 활성화 & 누르면 Dialog
	TWeakObjectPtr<UB2Button> BTN_AreaQuest;		// QuestIcon 누르면 커지고 작아지고

	TWeakObjectPtr<UCanvasPanel> CP_QuestState;
	TWeakObjectPtr<UCanvasPanel> CP_QuestNew;
	TWeakObjectPtr<UCanvasPanel> CP_QuestComplete;
	TWeakObjectPtr<UCanvasPanel> Slide;
	TWeakObjectPtr<UCanvasPanel> CP_QuestSlot;

	TWeakObjectPtr<UPanelWidget> P_MainQuestEffect;
	TWeakObjectPtr<UWidgetAnimation> ANIM_MainQ;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestState CurrentQuestState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CompressTextLimitLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlotHideTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, TSoftObjectPtr<UMaterialInterface>>	NPCIconMaterials;

	EQuestSlotState CurrentSlotState;

	bool bInitQuest;

	int32 QuestSlot;
	int32 QuestGoalCount;
	int32 CurrentProgressCount;

	FTimerHandle SlotHideTimer;

	class UMaterialInstanceDynamic* QuestProgressMaterial;


public:
	UB2UIQuestSlot(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	bool HasQuestInfo() { return QuestSlot && bInitQuest; }
	int32 GetCurrentQuestSlot() const { return QuestSlot; }

	UFUNCTION()
	void OnHideQuestSlotByTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void OnQuestSlotStateChanged(EQuestSlotState SlotState);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMaterialDailyQuest();

	UFUNCTION()
	void OnClickBtn_AreaQuest();

	UFUNCTION()
	void OnClickBtn_QuestDetailLink();

	UFUNCTION()
	bool AlarmCondition_QuestState();

	void SetQuestSlotState(EQuestSlotState NewState);
	void SetVisibility_QuestSlot(bool bVisible);
	void SetSlotHideTimer();
	void SetMainQuestEffect(QuestPtr InTargetQuest);

	void InitQuestDefault(int32 QuestSlotIndex, bool bDisplayOnly);
	void UpdateQuestDetailLeftTime(int32 TimeValue);

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void RegisterUIMarkForRedDot() override;

	void GetVisualStateFromQuest(const int32 QuestState, EQuestSlotState& OutSlotState, EQuestState& OutQuestState);
	void UpdateQuestDisplayText(struct FQuestCommonInfo* QuestInfo, const FText& ProgressText);
	void UpdateQuestProgress(float CurrentProgress);
	void UpdateProgressMaterial(float CurrentProgress);
	void GenerateQuestInfo(QuestPtr TargetQuest);
	void UpdateQuestMaterial(EQuestState QuestState);
	void UpdateNPCMaterial(int32 NPCIndex);
	void MainQuestActFilter(int32 InQuestID);
};
