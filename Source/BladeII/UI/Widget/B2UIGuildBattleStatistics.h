#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIGuildBattleStatistics.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UB2UIGuildStatistics_BattleResult : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	void SetOpponentUserName(const FText& ToSet);
	void SetBattleNum(FText InBattleNumber);
	void SetNumAcquiredMedals(int32 ToSet);
	void SetAllyInfoAndPowerIndex(bool InAlly, int32 InPowerIndex);
	void SetAttackEmpty(bool InIsEmpty);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UTextBlock>	TB_BattleNum;
	TWeakObjectPtr<UTextBlock>	TB_OpponentUserName;
	TArray<TWeakObjectPtr<UImage>> IMG_AcquiredMedals;
	TArray<TWeakObjectPtr<class UB2UIGuildInfoMedal>> AcquiredMedals;

	TWeakObjectPtr<UTextBlock> TB_PowerIndex;
	TWeakObjectPtr<UPanelWidget> O_AllyMark;
	TWeakObjectPtr<UPanelWidget> O_EnemyIndex;

	TWeakObjectPtr<UPanelWidget>	O_UserInfo;
	TWeakObjectPtr<UPanelWidget>	O_Medal;
	TWeakObjectPtr<UPanelWidget>	O_AttackEmpty;
};

UCLASS(Blueprintable, BlueprintType)
class UB2UIGuildStatistics_UserRecords : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	void SetUserName(const FText& ToSet);
	void SetAllyInfoAndPowerIndex(bool InAlly, int32 InPowerIndex);

	void AddBattleResult(bool InbAlly , int32 InPowerIndex ,const FText& UserName, int32 AcquiredMedalsFromUser);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UTextBlock>	TB_UserName;
	TArray<TWeakObjectPtr<UB2UIGuildStatistics_BattleResult>>	UIP_BattleResults;

	TWeakObjectPtr<UTextBlock> TB_PowerIndex;
	TWeakObjectPtr<UPanelWidget> O_AllyMark;
	TWeakObjectPtr<UPanelWidget> O_EnemyMark;

	int32 AddedBattleResult;
};

UCLASS(Blueprintable, BlueprintType)
class UB2UIGuildStatistics_UserRecordsList : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	void SetGuildMark(bool bLeftGuild, int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor);
	UB2UIGuildStatistics_UserRecords* AddUserRecords(bool bLeftGuild);

	void SetData(bool bLeftGuild, const TArray<b2network::B2GuildBattleMemberPtr>& MemberData, const TArray<b2network::B2GuildBattleAttackPtr>& AttackData);

protected:
	virtual void CacheAssets() override;

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<UB2UIGuildStatistics_UserRecords> UserRecordsDefaultUI;

private:
	TWeakObjectPtr<UVerticalBox> VB_LeftGuild_UserList;
	TWeakObjectPtr<UVerticalBox> VB_RightGuild_UserList;

	struct GuildMarkData
	{
		int32 Pattern;
		int32 PatternColor;
		int32 BG;
		int32 BGColor;
	};

	GuildMarkData LeftGuildMark;
	GuildMarkData RightGuildMark;
};

UCLASS(Blueprintable, BlueprintType)
class UB2UIGuildStatistics_Statistics : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:	
	void SetData(const FB2ResponseGuildBattleTurnResultPtr& DataPtr);

protected:
	virtual void CacheAssets() override;

	void SetNumOfAttacks(bool bLeft, int32 ToSet);
	void SetNumOfVictories(bool bLeft, int32 ToSet);

	void SetMVPUser(const FText& UserName, int32 TotalAttack, int32 TotalDefense, const b2network::B2GuildBattleEntryPtr& EntryDataPtr);
	void AddMVPBattleResult(const FText& UserName, int32 AcquiredMedalsFromUser);

private:
	TWeakObjectPtr<UTextBlock> TB_LeftGuild_NumOfAttack;
	TWeakObjectPtr<UTextBlock> TB_LeftGuild_NumOfVictories;
	TWeakObjectPtr<UTextBlock> TB_RightGuild_NumOfAttack;
	TWeakObjectPtr<UTextBlock> TB_RightGuild_NumOfVictories;
	
	TWeakObjectPtr<UTextBlock>	TB_MVPUserName;
	TWeakObjectPtr<UTextBlock>	TB_TotalAttack_Value;
	TWeakObjectPtr<UTextBlock>	TB_TotalDefense_Value;	

	TArray<TWeakObjectPtr<class UB2UITeamEntryCharacter>> UIP_MVP_Entry_Charaters;

	TArray<TWeakObjectPtr<UB2UIGuildStatistics_BattleResult>>	UIP_BattleResults;
	int32 AddedBattleResult;
};

UCLASS(Blueprintable, BlueprintType)
class UB2UIGuildStatistics_GuildInfos : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	void SetGuildInfo(bool bLeftGuild
		, const FText& GuildName, int32 Point, int32 GuildExp
		, int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor);
	void SetGuildBattleResult(int32 InResult);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<class UB2UIGuildInfoPart5> UIP_LeftGuildInfo;
	TWeakObjectPtr<class UB2UIGuildInfoPart5> UIP_RightGuildInfo;


	TWeakObjectPtr<UPanelWidget>				O_Victory;
	TWeakObjectPtr<UPanelWidget>				O_Lose;
	TWeakObjectPtr<UPanelWidget>				O_Draw;
};

UCLASS(Blueprintable, BlueprintType)
class UB2UIGuildStatistics_Rewards : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	void SetRewardItems(const TArray<b2network::B2RewardPtr>& RewardList);

protected:
	virtual void CacheAssets() override;

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIRewardIcon> RewardIconDefaultUI;

private:
	TWeakObjectPtr<UHorizontalBox> HB_RewardList;
};

UCLASS(Blueprintable, BlueprintType)
class UB2UIGuildStatistics_Main : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	enum class EGuildStatisticsStates : uint8
	{
		StatisticsView,
		BattleRecordView,
		RewardView
	};
	
	void SetViewState(EGuildStatisticsStates ToSet);
	
	void SetData(const FB2ResponseGuildBattleTurnResultPtr& ResultPtr);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
		void OnClickedConfirmBTN();

	UFUNCTION()
		void OnClickedStatisticsBTN();

	UFUNCTION()
		void OnClickedRecordsBTN();

private:
	TWeakObjectPtr<UB2Button> BTN_Confirm;


	TWeakObjectPtr<UVerticalBox> VB_Statistics;

	TWeakObjectPtr<UB2UIGuildStatistics_GuildInfos> UIP_GuildInfos;
	TWeakObjectPtr<UB2UIGuildStatistics_UserRecordsList> UIP_UserRecordsList;

	EGuildStatisticsStates ViewStates;
};