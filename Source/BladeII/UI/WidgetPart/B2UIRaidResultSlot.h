// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIRaidResultSlot.generated.h"

/**
 * 
 */

enum class ERaidResultSlotState : uint8
{
	Win,
	Lose,
	Giveup,
	Max,
};

struct FRaidResultSlotInfo
{
	int32 Number;
	int32 Level;
	EPCClass PCIndex;
	int32 RewardID;
	int32 RewardGrade;
	int32 RewardCount;
	int32 StepRewardID;
	int32 StepRewardGrade;
	int32 StepRewardCount;
	ERaidResultSlotState SlotState;
	int32 ContributionPercent;
	FText Name;	
	int32 GetExp;
	int64 AccountID;
	bool IsMy;
	bool IsMyGuildMan;
	bool IsGuildParty;
	bool IsMainUsedRewardID;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRaidResultSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	void InitRankSlot(int32 Rank);
	
	void UpdateSlot(const FRaidResultSlotInfo &SlotInfo);

	void SetRankNumber(int32 Number, ERaidResultSlotState State);
	void SetRankerName(FText Name);
	void SetRankerLevel(int32 Level);
	void SetRankerPercent(float Percent);
	void SetRankerCharacter(UMaterialInstance* pIconMI);
	void SetResultReward(const int32 Index, const FText& Text, const int32 nGrade = 0, const bool bUsedRewardID = false);

	void SetResultExpReward(const FText& Text);
	void SetResultStepReward(const int32 Index, const FText& Text, const int32 nGrade, const bool bMyGuildMan = false);
	void SetResultGiveup();
	void SetResultFail();
	void SetResultNotEnoughContribution();

	void UserReportComplete();

	UFUNCTION(BlueprintCallable, Category = BladeII)
	void ShowBonusItem();

	FORCEINLINE bool GetIsMySlot() { return bIsMy; }

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION()
		void OnClickUserRepot();

	UFUNCTION()
	void ShowBonusAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayBonusItemAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInterface*>	CharterTexture;

	enum class ERewardVisibleState : uint8
	{
		RewardItem,
		MultiLine,
		SingleLine,
		Max,
	};

	TWeakObjectPtr<UImage> IMG_Character;
	TWeakObjectPtr<UImage> IMG_MyBG;
	TWeakObjectPtr<UImage> IMG_BG;
	TWeakObjectPtr<UImage> IMG_Lose;

	TWeakObjectPtr<UTextBlock> RTB_RankerName;
	TWeakObjectPtr<UTextBlock> RTB_RankerLevel;
	TWeakObjectPtr<UTextBlock> RTB_RankerPercent;

	TWeakObjectPtr<UWidgetSwitcher> WS_Switcher;
	TWeakObjectPtr<UTextBlock> RTB_RewardNoticeMultiline;
	TWeakObjectPtr<UB2RichTextBlock> RTB_RewardNoticeSingleline;

	TWeakObjectPtr<UScaleBox> p_RewardExp;
	TWeakObjectPtr<UScaleBox> p_RewardItem;
	TWeakObjectPtr<UScaleBox> p_RewardStep;
	TWeakObjectPtr<class UB2UIRewardIcon> RewardItem0;
	TWeakObjectPtr<class UB2UIRewardIcon> RewardExp;
	TWeakObjectPtr<class UB2UIRewardIcon> RewardStep;
	TWeakObjectPtr<UTextBlock> TB_ContributionGuide;

	TWeakObjectPtr<UTextBlock> STB_GuildMan;
	TWeakObjectPtr<UPanelWidget> P_GuildMan;

	TArray<TWeakObjectPtr<class UImage>> Numbers;

	TWeakObjectPtr<UB2Button>		BTN_UserReport;
private:
	int32 RankIndex;
	bool bIsMy = false;

	int64 AccountID;
	uint32 UserReportTicket;
};
