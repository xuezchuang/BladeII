#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "../../BladeII.h"
#include "B2UIMainCounterAttackDG.generated.h"



UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMainCounterAttackDG : public UB2UIWidget
{
	GENERATED_BODY()
public:
	virtual void OnOpen(bool RightNow = false) override;

public:
	UFUNCTION()
		void OnClickBtnStarRewardBox(class UB2UIChapterStarRewardBox* ClickedReward);

protected:
	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;


protected:
	TWeakObjectPtr<class UB2UIMainCounterAttackDG_Slot>		UIP_DungeonSlot1;
	TWeakObjectPtr<class UB2UIMainCounterAttackDG_Slot>		UIP_DungeonSlot2;
	TWeakObjectPtr<class UB2UIMainCounterAttackDG_Slot>		UIP_DungeonSlot3;
	TWeakObjectPtr<class UB2UIMainCounterAttackDG_Slot>		UIP_DungeonSlot4;

	TWeakObjectPtr<class UB2UIChapterStarRewardBox>			UIP_ChapterStarRewardBox0;
	TWeakObjectPtr<class UB2UIChapterStarRewardBox>			UIP_ChapterStarRewardBox1;
	TWeakObjectPtr<class UB2UIChapterStarRewardBox>			UIP_ChapterStarRewardBox2;

	TArray<TWeakObjectPtr<class UB2UIChapterStarRewardBox>>	UIP_ChapterStarReward;

	TWeakObjectPtr<class UB2UIChapterStarRewardPreview>		UIP_ChapterStarRewardPreview;

	TArray<TWeakObjectPtr<class UB2UIMainCounterAttackDG_Slot>> DungeonSlots;

private:
	virtual void OnCloseComplete() final;
	uint32 DeliveryCounterDungeonWeeklyRewardTicket;
	uint32 DeliveryGetCounterTicket;
public:
	void SetRewardInfo();
	void ResponseWeeklyReward(const FB2CounterDungeonWeeklyReward& WeeklyReward);
	void ResponseGetCounter(const FB2GetCounterDungeon& msg);
	void UpdateOpenedSlot();

	TWeakObjectPtr<class UB2UIMainCounterAttackDG_Slot> GetActiveDungeonSlot();
	TUTORIAL class UB2UnitedWidgetBase* GetActiveDungeonWidgetPart();
};