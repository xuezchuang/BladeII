// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIControlMain.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlMain : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	virtual void UpdateStaticText() override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	void SetModRewardBox();
	void SetMVPRewardBox();
	void SetModePopup();
	void SetWinState();
	void OnChangedRewardBox(class UB2UIDocBase* Sender, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo2);
	void OnChangedMVPPoint(class UB2UIDocBase* Sender, int32 Point, int32 Point2);

	void OpenMVPRewardPopup(FB2AssaultMvpRewardPtr& MvpReward);


	void SubscribeEvents();
	void UnsubscribeEvents();

	UFUNCTION()
	void OnClickBtnStartGame();

	UFUNCTION()
	void OnClickBtnRanking();

	UFUNCTION()
	void OnClickBtnControlMode();

	UFUNCTION()
	void OnClickBtnControlModePopupClose();

	UFUNCTION()
	void OnClickBtnSeasonReward();

	void InitRemainTime();
	bool CheckControlAbleTime();

	void SetPreRewardList();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UB2UIRewardIcon>			RewardInfoClass;
private:
	TWeakObjectPtr<UButton> BTN_StartGame;
	TWeakObjectPtr<UButton> BTN_Ranking;
	TWeakObjectPtr<UButton> BTN_ControlMode;

	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_MyControlGrade;

	TWeakObjectPtr<class UB2UIChapterStarRewardBox> UIP_PVPSeason_Reward;

	TWeakObjectPtr<UTextBlock> TB_ControlMode;

	TWeakObjectPtr<UTextBlock> TB_MyRanking;
	TWeakObjectPtr<UTextBlock> TB_RankPercentage;

	TWeakObjectPtr<UTextBlock> TB_RemainTime;
	TWeakObjectPtr<UTextBlock> TB_RemainTimeValue;

	TWeakObjectPtr<UCanvasPanel>	CP_ModePopup;
	TWeakObjectPtr<UB2Button>		Mode_BTN_Close;
	TWeakObjectPtr<UTextBlock>		Mode_TB_Info;

	TWeakObjectPtr<UVerticalBox> List_Reward;
	TWeakObjectPtr<class UB2UIRewardIcon> UIP_RewardIcon_ScrollOnly;
	TWeakObjectPtr<class UB2UIModRewardBox>	UIP_RewardBox;
	
	TWeakObjectPtr<class UB2UIMvpRewardBox> UIP_MVPRewardBox;

	TWeakObjectPtr<UCanvasPanel> CP_TimeInfo;

	TWeakObjectPtr<UTextBlock>			TB_WinMax;
	TWeakObjectPtr<UTextBlock>			TB_WinMaxNumber;
	TWeakObjectPtr<UTextBlock>			TB_CurrentWin;
	TWeakObjectPtr<UTextBlock>			TB_CurrentWinNumber;
	
	uint32 DeliveryOpenDuelRewardBoxTicket;
	uint32 DeliveryAssaultMvpRewardTicket;
};
