#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIPVPResult.generated.h"

enum class EStateAfterGradeUp : uint32
{
	GRADE_ANIM_START = 1,
	GRADE_CURRENT = 2,
	GRADE_UP_END = 3,
	GRADE_DOWN_END = 4,
	ADD_STAR = 5,
	ADD_STAR_AFTER_GRADE_UP = 6,
	STATE_END = 7,
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPVPResult : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

protected:
	EB2GameMode GameModeEnum;

	TWeakObjectPtr<UCanvasPanel> CP_Victory;
	TWeakObjectPtr<UCanvasPanel> CP_Lose;
	TWeakObjectPtr<UCanvasPanel> CP_Draw;
	TWeakObjectPtr<UCanvasPanel> CP_MainInfoSet;
	
	TWeakObjectPtr<UPanelWidget> P_RewardInfo;
	TWeakObjectPtr<UVerticalBox> VB_RankInfo;
	TWeakObjectPtr<UPanelWidget> P_BottomButtons;

	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_PVPGrade;

	TWeakObjectPtr<UHorizontalBox> List_Reward;

	TWeakObjectPtr<UOverlay> OV_WinResult;
	TWeakObjectPtr<UTextBlock> TB_ResultWin_Count;
	TWeakObjectPtr<UOverlay> OV_LoseResult;
	TWeakObjectPtr<UTextBlock> TB_ResultLose_Count;

	TWeakObjectPtr<UTextBlock> TB_SuccessiveWin;
	TWeakObjectPtr<UImage>  IMG_IncreaseWins;

	TWeakObjectPtr<UTextBlock> TB_PlayerRank;
	TWeakObjectPtr<UImage> IMG_IncreaseRank;
	TWeakObjectPtr<UImage> IMG_DecreaseRank;
	TWeakObjectPtr<UTextBlock> TB_DeltaRank;
	TWeakObjectPtr<UTextBlock> TB_SendMail;
	TWeakObjectPtr<UTextBlock> TB_VictoryReward;

	TWeakObjectPtr<UButton> BTN_OpponentInfo;
	TWeakObjectPtr<UButton> BTN_Restart;
	TWeakObjectPtr<UButton> BTN_Quit;

	TWeakObjectPtr<UB2Button> BTN_BlockTouch;

	TWeakObjectPtr<class UB2UIRankChanged> UIP_PVPRankingChangedPopup;

	TWeakObjectPtr<UWidgetAnimation> ANIM_Open;
	TWeakObjectPtr<UWidgetAnimation> ANIM_UP_01;
	TWeakObjectPtr<UWidgetAnimation> ANIM_UP_02;

	TWeakObjectPtr<class UB2UIModRewardBox>	UIP_ModRewardBox;
	TWeakObjectPtr<class UB2UIStageResultLevelUp> UIP_LevelUp;


	TWeakObjectPtr<UTextBlock>				TB_LoseMent;
	TWeakObjectPtr<UTextBlock>				TB_WinningBonus;
	TWeakObjectPtr<UTextBlock>				TB_PVPRankPrevent;

	TWeakObjectPtr<UB2Button>				BTN_UserReport;

	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_LoseReward;

private:
	TWeakObjectPtr<class UB2UIDocSome> DocSome;
	TWeakObjectPtr<class UB2UIDocBattle> PvPMatchDoc;
	TWeakObjectPtr<class UB2UIDocTeamMatch> TeamMatchDoc;


	bool IsFriendshipGame;

public:
#if WITH_EDITOR
	void SetCommandRankUp(const int32 _Rank, const int32 _StarCount, const int32 _WantedStarCount);
#endif // WITH_EDITOR
	uint32	StartRankUpAnimationTicket;
	uint32  UserReportTicket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PvPResult)
		TSubclassOf<class UB2UIRewardIcon> RewardIconClass;

	UB2UIPVPResult(const FObjectInitializer& ObjectInitializer);

	static int32 GetGrade(const EB2GameMode GameMode, const bool IsCurrent = true);
	static int32 GetStarCount(const EB2GameMode GameMode, const bool IsCurrent = true);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override {}
	virtual void UpdateStaticText() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnOpenAnimEnded();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnRankUpAnimEnded_One();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnRankUpAnimEnded_Two();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void SetDeltaRank(int32 DeltaRank);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void SetDeltaTopWins(int32 DeltaWins);

	UFUNCTION(BlueprintNativeEvent, Category = "BladeII")
		void SetRankUpImamge(const int32 Rank);

	void OnChangedRewardBox(class UB2UIDocBase* Sender, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo2);
	void UserReportComplete();

	virtual void PreventRematch();

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	void StartRankupAnimation(const EStateAfterGradeUp GradeUpState);
	void ShowRankingChanged(int32 DiffChangedRanking);
	void SetRewardBoxStatus();

	UFUNCTION()
		void OnClickBtnOpponentInfo();
	UFUNCTION()
		void OnClickBtnRestart();
	UFUNCTION()
		void OnClickBtnQuit();

	UFUNCTION()
		void OnClickReport();

	bool OnCheckAdditionalPoint_PvP();
	void OnCheckGemForBuyTicket_PvP();

	bool OnCheckAdditionalPoint_Team();
	void OnCheckGemForBuyTicket_Team();

	void CheckAndOpenLevelUpPart();
	void OnLevelUpFXFinish();
	void CloseRankingChangePopup();
	
};
