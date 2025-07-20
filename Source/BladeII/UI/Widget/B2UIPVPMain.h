#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UIPVPMain.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPVPMain : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	void RequestUpdate();

	void UpdateList();
	void SetModRewardBox();

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	void UpdateStaticText();

	UFUNCTION()
	void OnClickBtnAllRank();
	UFUNCTION()
	void OnClickBtnFriendRank();

	UFUNCTION()
	void OnClickBtnStartPVPMatch();

	UFUNCTION()
	void OnClickBtnBattleFriendly();

	//UFUNCTION()
	//void OnClickBtnDaillyReward();

	UFUNCTION()
	void OnClickBtnWeeklyReward();

	void OnChangedRank(class UB2UIDocBase* Sender, int32 CurRank, int32 PrevRank);
	void OnChangedGrade(class UB2UIDocBase* Sender, int32 CurGrade, int32 PrevGrade);
	void OnChangedRankingList(const TArray<FModRankerInfo>& RankerInfos);
	void OnChangedRewardBox(class UB2UIDocBase* Sender, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo2);

	void ShowRankerList(bool bShowAllRanker);
	void ShowWeekReward();
	//void SetDayRewardInfo();
	void SetFriendList(const FB2ResponseGetFriendRankingsPtr& FriendRank);
	void SetRewardList();

	void ShowInviteUI(bool bShowInvite);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RankerInfo)
	TSubclassOf<class UB2UIRankerInfo> RankerInfoClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RankerInfo)
	TSubclassOf<class UB2UIRewardIcon> RewardInfoClass;

private:
	TWeakObjectPtr<UVerticalBox> VB_RankerList;

	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_MyPVPGrade;
	TWeakObjectPtr<UTextBlock> TB_MyRanking;
	TWeakObjectPtr<UTextBlock> TB_RankPercentage;
	//TWeakObjectPtr<class UB2UIChapterStarRewardBox> UIP_PVPDayRewardBox;
	TWeakObjectPtr<class UB2UIChapterStarRewardBox> UIP_PVPWeekRewardBox;

	TWeakObjectPtr<UButton> BTN_StartPVPMatch;

	TWeakObjectPtr<class UB2UIStartButton>	UIP_StartBT;
	
	TWeakObjectPtr<UTextBlock>	TB_BattleFriendly;
	TWeakObjectPtr<UButton>		BTN_BattleFriendly;
	//Static Text
	TWeakObjectPtr<UTextBlock> TB_RankUnit;
	TWeakObjectPtr<UTextBlock> TB_Weekly;
	TWeakObjectPtr<UTextBlock> TB_EntenHero;
	
	TWeakObjectPtr<UHorizontalBox>		 List_Reward;

	TWeakObjectPtr<class UB2UICommonTab> TAB_AllRank;
	TWeakObjectPtr<class UB2UICommonTab> TAB_FriendRank;

	TWeakObjectPtr<class UB2UIModRewardBox>		UIP_ModReward;

	TArray<FModRankerInfo> FriendRankingList;

	TWeakObjectPtr<UTextBlock>			TB_WinMax;
	TWeakObjectPtr<UTextBlock>			TB_WinMaxNumber;
	TWeakObjectPtr<UTextBlock>			TB_CurrentWin;
	TWeakObjectPtr<UTextBlock>			TB_CurrentWinNumber;


	int32 DeliveryGetFriendRankingTicket;
	uint32 DeliveryOpenDuelRewardBoxTicket;
	
private:
	TArray<TWeakObjectPtr<UB2UIRankerInfo>> RankerList;

	bool bShowingAllRanker;
	const TArray<FModRankerInfo>* ModRankerList;
};