#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UITeamRankerInfo.h"
#include "B2UITeamMatchMain.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamMatchMain : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void UnbindDoc() override;
	virtual void UpdateStaticText() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RankerInfo)
	TSubclassOf<class UB2UITeamRankerInfo> RankerInfoClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RankerInfo)
	TSubclassOf<class UB2UIRewardIcon> RewardInfoClass;

	void SetEntryInfo();

	void UpdateList();
	void ShowWeekReward();
	void SetFriendList(const FB2ResponseGetFriendRankingsPtr& FriendRank);

	UFUNCTION()
	void OnClickBtnWeeklyReward();

	void OnChangedRewardBox(class UB2UIDocBase* Sender, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo2);
	void OnChangedRankingList(const TArray<FModRankerInfo>& RankerInfos);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;	

	void SubscribeEvents();
	void UnsubscribeEvents();

	UFUNCTION()
		void OnClickStartMatch();
	UFUNCTION()
		void OnClickAllRanker();
	UFUNCTION()
		void OnClickFriendRanker();

	void ShowRankerList(bool bShowAllRanker);
	void SetModRewardBox();
	void SetRewardList();
public:
	void OnChangedFormationLevel(UB2UIDocBase* Doc, int32 CurrLv, int32 PreLv);

	UFUNCTION()
	void OnChangedGrade(UB2UIDocBase* Doc, int32 CurrGrade, int32 PrevGrade);

	UFUNCTION()
	void OnChangedRank(UB2UIDocBase* Doc, int32 CurrRank, int32 PrevRank);

private:
	//Button
	TWeakObjectPtr<class UB2UIStartButton> UIP_StartBT;

	TWeakObjectPtr<UVerticalBox> VB_RankerList;

	TWeakObjectPtr<UTextBlock>		TB_CombatPower;

	TWeakObjectPtr<UTextBlock>		TB_MyRanking;
	TWeakObjectPtr<UTextBlock>		TB_RankPercentage;

	//TWeakObjectPtr<class UB2UITMFormation>		UIP_HeroTM;
	TWeakObjectPtr<UB2UIGradeRankAndStar>		UIP_MyPVPGrade;

	TWeakObjectPtr<class UB2UIChapterStarRewardBox> UIP_TeamWeekRewardBox;

	TWeakObjectPtr<UTextBlock>						TB_Weekly;
	TWeakObjectPtr<UTextBlock>						TB_EntenHero;

	TWeakObjectPtr<UHorizontalBox>					List_Reward;

	TWeakObjectPtr<class UB2UIModRewardBox>			UIP_ModReward;

	TWeakObjectPtr<UTextBlock>			TB_WinMax;
	TWeakObjectPtr<UTextBlock>			TB_WinMaxNumber;
	TWeakObjectPtr<UTextBlock>			TB_CurrentWin;
	TWeakObjectPtr<UTextBlock>			TB_CurrentWinNumber;

	TArray<FModRankerInfo> FriendRankingList;
	int32 DeliveryGetFriendRankingTicket;

	TWeakObjectPtr<class UB2UICommonTab> TAB_AllRank;
	TWeakObjectPtr<class UB2UICommonTab> TAB_FriendRank;

private: 
	TArray<TWeakObjectPtr<UB2UITeamRankerInfo>> RankerList;
	bool bShowingAllRanker;

	uint32 DeliveryOpenDuelRewardBoxTicket;
};