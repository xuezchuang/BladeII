// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "CommonStruct.h"
#include "B2UIDocRaid.h"
#include "../B2RichTextBlock.h"
#include "B2UIRaidMain.generated.h"

enum class ERaidBlockCode : uint8
{
	ABLEJOIN = 0,
	NOT_ABLE_DAY,
	NOT_COMBATPOWER,
	NOT_OPEN_TIME,

};

USTRUCT(BlueprintType)
struct FBossImageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERaidBossIndex BossIndex;

	UPROPERTY(EditAnywhere)
	UTexture2D* BossBGImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* BossDescImage;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* BossTabImage;
};

UCLASS()
class BLADEII_API UB2UIRaidMain : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void ForcedStartGame(int32 RaidType, int32 RaidStep, ERaidJoinType RaidJoinType, EPCClass InPcClass, bool IsMatchMaking);
	void ButtonFree();
	void SetButtonEnable(bool bIsEnable);

protected:
	UFUNCTION()
	void OnClickBTN_GoToHeroMgmtMode();
	UFUNCTION() 
	void OnClickBTN_MakeRoom();
	UFUNCTION()
	void OnClickBTN_QuickStart();

	UFUNCTION()
	void OnClickTAB_Boss(int32 BossNum);

	UFUNCTION()
	void OnClickEntrySlot(EPCClass InEntry);

private:
	void InitEntrySlot();
	void UpdateEntryInfo(EPCClass InEntry);
	void SetBossInfo();
	void StartGame(ERaidJoinType JoinType);
	void CheckRaidAble();
	int32 CheckAbleRaidStep(int32 inStep);

	void SetMaxDifficulty();
	void ChangeDifficulty(int32 InDifficulty);
	void RefreshRecommendStats(int32 RecommandPower, int32 RecommendAttack, int32 RecommendDefense);
	void SetRaidRewardInfo(int32 RaidType, int32 RaidStep);

	void SelectRaidDefaultBoss();
	void SelectRaidDefaultStep();

	void UpdateRaidRewardInfo(int32 RaidType, int32 RaidStep);
	void SetItemRewardAssessory(int32 ItemMinGrade, int32 ItemMaxGrade);
	void SetItemReward(int32 RewardId);

	bool CheckRaidPenalty();
	bool CheckRaidAbleTime();
	void ShowRaidNotOpenMsgPopup();

	FText GetLockedDayText(TArray<int32> ModeOpenDays);
	FText GetLockedTimeText(TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes);
	FText GetBlockMessageText(ERaidBlockCode eCode, FText AddMsg = FText::GetEmpty());

	FString GetEquipItemGradeText(int32 ItemMinGrade, int32 ItemMaxGrade);
	FString GetEquipItemTypeText(EEquipCategoryType EquipCategory);
	FString GetEquipItemText(EEquipCategoryType EquipCategory, int32 ItemMinGrade, int32 ItemMaxGrade);
	
private:
	TWeakObjectPtr<UB2Button> BTN_GoToHeroMgmtMode;
	TWeakObjectPtr<UButton> BTN_MakeRoom;
	TWeakObjectPtr<UButton> BTN_QuickStart;

	TArray<TWeakObjectPtr<class UB2UICommonTab>>	TopTabs;

	TWeakObjectPtr<class UB2UIDungeonDifficulty> UIP_DungeonLevelofDifficulty;

	TWeakObjectPtr<UTextBlock>						STB_RecommandPower;
	TWeakObjectPtr<UTextBlock>						STB_CombatPower;
	TWeakObjectPtr<UTextBlock>						STB_MakeRoom;
	TWeakObjectPtr<UTextBlock>						STB_QuickStart;
	TWeakObjectPtr<UTextBlock>						STB_RewardInfo;
	TWeakObjectPtr<UTextBlock>						STB_Reward_Step;
	TWeakObjectPtr<UTextBlock>						STB_Reward_Exp;
	TWeakObjectPtr<UTextBlock>						STB_GoToHeroMgmtMode;
	TWeakObjectPtr<UTextBlock>						STB_EntenHero;
	TWeakObjectPtr<UTextBlock>						STB_RaidSelect;
	TWeakObjectPtr<UB2RichTextBlock>				STB_JoinExplanation;


	TWeakObjectPtr<UTextBlock>						TB_ExceptionItem;
	TWeakObjectPtr<UTextBlock>						TB_DailyEnter;
	TWeakObjectPtr<UTextBlock>						TB_Reward_Main;
	TWeakObjectPtr<UTextBlock>						TB_Reward_Contribution;
	TWeakObjectPtr<UTextBlock>						TB_CombatPower;
	TWeakObjectPtr<UTextBlock>						TB_RecommandPower;
	TWeakObjectPtr<UTextBlock>						TB_BossTitle;
	TWeakObjectPtr<UTextBlock>						TB_BossDesc;
	TWeakObjectPtr<UTextBlock>						TB_BlockDesc;

	TWeakObjectPtr<UImage>							IMG_RaidBossBG;
	TWeakObjectPtr<UImage>							IMG_BossImg;

	TWeakObjectPtr<UWidgetSwitcher>					SW_ButtonSet;

	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_Reward_Contribution;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_Reward_Main;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_Reward_Step;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_Reward_Exp;
	TWeakObjectPtr<class UB2UIRecommandPower>			UIP_RecommendPower;

	TWeakObjectPtr<class UB2UIFairyLinkButton>		UIP_FairyLinkButton;

	TArray<TWeakObjectPtr<class UB2UIPVPEntryHero>>	HeroEntryList;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<FBossImageInfo>							BossImageArray;

private:
	FTimerHandle									ButtonFreeHandle;

	int32 CurrentRaidStep;
	int32 CurrentBossType;
	EPCClass CurrentPCClass;
	int32 RecomandCombatPower;

};
