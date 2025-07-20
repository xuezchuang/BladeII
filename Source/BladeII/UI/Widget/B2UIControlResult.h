// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIControlResult.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlResult : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void NativeConstruct() override;
	virtual void UpdateStaticText() override;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_AnimWin();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_AnimLose();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_AnimDraw();

	void CheckAndOpenLevelUpPart();

	void SetAssaultBattleStatus(const FB2ResponseGetAssaultBattleStatusPtr& MsgPtr);
private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	void SubscribeEvent();
	void UnsubscribeEvent();

	UFUNCTION()
		void OnClickExit();
	UFUNCTION()
		void OnClickUserReport();

	void CheckRankChanged();
	void ShowRankingChanged(int32 DiffChangedRanking);
	void CloseRankingChangePopup();

protected:
	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_Grade;
	TWeakObjectPtr<class UB2UIControlRecordItem> UIP_Control_Record_EnemyKill;
	TWeakObjectPtr<class UB2UIControlRecordItem> UIP_ControlRecord_TotalDamage;
	TWeakObjectPtr<class UB2UIControlRecordItem> UIP_ControlRecord_ContributeTime;
	TWeakObjectPtr<class UB2UIControlRecordItem> UIP_ControlRecord_Dead;

	TWeakObjectPtr<class UB2UIModRewardBox>	UIP_ModRewardBox;

	TWeakObjectPtr<class UB2UIRankChanged>	UIP_ControlRankingChangedPopup;
	
	TWeakObjectPtr<UB2Button> BTN_Exit;
	TWeakObjectPtr<UTextBlock>	STB_GetMailReward;
	TWeakObjectPtr<UTextBlock>	TB_LoseMent;

	TWeakObjectPtr<UPanelWidget> p_MailText;
	TWeakObjectPtr<UPanelWidget> P_LoseText;
	TWeakObjectPtr<UHorizontalBox> List_Reward;

	TWeakObjectPtr<class UB2UIStageResultLevelUp> UIP_LevelUp;

	TWeakObjectPtr<UTextBlock>				TB_WinningBonus;
	TWeakObjectPtr<UTextBlock>				TB_PVPRankPrevent;

	TWeakObjectPtr<UB2Button>				BTN_UserReport;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ControlResult")
	TSubclassOf<class UB2UIControlResultRewardIcon> ControlRewardClass;
	
private:
	uint32 DeliveryGetAssaultBattleStatusTicket;
};

UCLASS()
class BLADEII_API UB2UIControlResultRewardIcon : public UB2UIWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void CacheAssets() override;

	void SetRewardBox();
	void SetRewardItem(b2network::B2RewardPtr Rewardptr);
	void SetRewardMvp(int32 RewardCount);

	UFUNCTION(BlueprintImplementableEvent)
	void SetRewardBoxGrade_IMP(int32 Grade);
protected:
	TWeakObjectPtr<UWidgetSwitcher> SW_RewardType;
	TWeakObjectPtr<UTextBlock> TB_RewardTitle;
	TWeakObjectPtr<class UB2UIRewardIcon> UIP_RewardIcon;
};
