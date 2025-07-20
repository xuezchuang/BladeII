// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2UIRepeatBattleInGameInfo.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIStageResult.generated.h"

/** Variables needed for managing exp progress bar progress value per player */
struct FExpProgressVars
{
	FExpProgressVars()
	{
		FMemory::Memset(this, 0, sizeof(*this));
	}

	int32 BasicLevelToIncrease;
	float BasicTargetExpPercent;
	float BasicCurrentExpPercent;
	uint32 bBasicExpIncreaseComplete : 1;

	int32 ExtraLevelToIncrease;
	int32 ExtraLevelToIncreaseFromBasic;
	float ExtraTargetExpPercent;
	float ExtraCurrentExpPercent;
	uint32 bExtraExpIncreaseComplete : 1;

	int32 CurrentIncreasedLevel;
};

/**
 * For stage clear event, to display player's glorious fucking moment.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStageResult : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:

	/** All widgets are to be displayed right at open, without time delay. It is meant to be used on level-up scene, successive to stage result page 
	 * -> Now the LevelUp scene is jus UI, so we don't need this much functionality now.. just a stat flag now.. */
	UPROPERTY(BlueprintReadOnly, Category = "BladeII")
	uint32 bIsSuccessiveLevelUpOpen : 1;

public:
	UB2UIStageResult(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void BindDocMainChar(class UB2UIDocHero* DocHero); // Main here means "active at stage clear moment" not the actual main character selection from lobby
	void BindDocSubChar(class UB2UIDocHero* DocHero); // Sub here means "inactive at stage clear moment" not the actual sub character selection from lobby
	
	void SetStageClearGrade(const TArray<bool>& Grade, FServerStageID InServerStageNum);
	void SetClearTime(int32 InValue);
	void SetDropItem(const TArray<FB2Item> DropItemData, int32 MultipleValue = 1);
	void SetAcquiredGold(int32 InValue);
	void SetSoldGold(int32 InValue);
	void SetButtonRevitalize();
	void SetHotTimeBuff(int32 GoldRate, int32 ExpRate);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII StageReward")
		void SetStageClearGrade_BP(int32 InValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII StageReward")
		class UB2UISlotItem* CreateRewardItem();

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	/** Overridden to modify event signal timing */
	virtual void OnOpenComplete() override;
	virtual void OnCloseComplete() override;

	void OnActualOpenComplete(); // For StageResult UI's custom open complete timing

	virtual void CloseWidgetDelegate() override;
private:
	/** Common functionalities to initialize FExpProgressVars for each player character. */
	void InitExpVarSet(FExpProgressVars& ExpVarSet, EPCClass PCClass);

	/** Variable set for exp bar progress increasing animation. */
	FExpProgressVars EBV_MainPlayer; // For the active one at stage clear moment
	FExpProgressVars EBV_SubPlayer; // For the inactive one at stage clear moment

	void UpdateExpPercent(float DeltaSeconds);
	bool UpdateExpPercentCommon(FExpProgressVars& ExpVarSet, float DeltaSeconds);

	void UpdateMainCharLevel();
	void UpdateSubCharLevel();

	/** Override because this StageClearWidget is expected to be used at paused state. */
	virtual void SetupWidgetCustomTimer() override;
	/** Override because this StageClearWidget is expected to be used at paused state. */
	virtual void SetupWidgetCustomHideTimer() override;

	float CachedWidgetStartTime;
	void UpdateWidgetCustomTimer();

	void StartRepeatBattleCountdown( bool CountdownVisibility = false );
	void StopRepeatBattleCountdown();
	void SetupOnRepeatBattleStateChange( bool bIsOn, bool CountdownVisibility = false );
	bool IsRepeatBattleOn();

	void SetLevelUpTextVisible(bool bForMain);

	void CheckAndOpenLevelUpPart(); // Do this on completion timing.
	void SetIsSuccessiveLevelUpOpen(bool bIsSuccessive); // It will skip all opening anim stuff and bring level up part visible.

	void OpenQuestRewardDialog();
	void OnQuestDialogClosed();

	bool IsShowMainQuestReward();

	void OnAllProcessEnd();
	void SetButtonClickable(bool bClickable);
	void CheckFinalStageDisableButton();

	void StageFxResult(bool bMain, bool bSub);

	//===================================================Click
	UFUNCTION()
		void OnClickBtnVillage();
	UFUNCTION()
		void OnClickBtnAreaSelection();
	UFUNCTION()
		void OnClickBtnRestart();
	UFUNCTION()
		void OnClickBtnNextArea();

	//====================================================Doc Delegate

	UFUNCTION()
		void OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn);

public:
	/** Timing that the Exp bar start to increase after the creation of this widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII StageReward")
		float ExpIncreaseStartTime;

	/** The speed of filling in exp progress to target, in progress per second */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII StageReward")
		float ExpIncreaseSpeed;

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnUpdateExpPercentPlaySoundStartBP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnUpdateExpPercentPlaySoundStopBP();

	UFUNCTION()
	void OnLevelUpFXFinish();

protected:
	TWeakObjectPtr<UButton>				BTN_Village;
	TWeakObjectPtr<UTextBlock>			TB_BtnVillage;
	TWeakObjectPtr<UButton>				BTN_AreaSelection;
	TWeakObjectPtr<UTextBlock>			TB_BtnAreaSelection;
	TWeakObjectPtr<UButton>				BTN_Restart;
	TWeakObjectPtr<UTextBlock>			TB_BtnRestart;
	TWeakObjectPtr<UButton>				BTN_NextArea;
	TWeakObjectPtr<UTextBlock>			TB_BtnNextArea;

	/* For the active character at stage clear moment. It is not the same to the lobby's main character selection. */
	TWeakObjectPtr<UTextBlock>			TB_Main_TX;
	TWeakObjectPtr<UImage>				IMG_Main_LevelUp;
	TWeakObjectPtr<UTextBlock>			TB_MainCharLevel;
	TWeakObjectPtr<UImage>				IMG_PortraitMain;
	TWeakObjectPtr<UProgressBar>		BasicExpBar_MainPlayer;
	TWeakObjectPtr<UProgressBar>		ExtraExpBar_MainPlayer;
	TWeakObjectPtr<UTextBlock>			TB_ExpPercentMainChar;

	/* For the inactive (waiting for tag) character at stage clear moment. It is not the same to the lobby's sub character selection. */
	TWeakObjectPtr<UTextBlock>			TB_Tag_TX;
	TWeakObjectPtr<UImage>				IMG_Tag_LevelUp;
	TWeakObjectPtr<UTextBlock>			TB_SubCharLevel;
	TWeakObjectPtr<UImage>				IMG_PortraitSub;
	TWeakObjectPtr<UProgressBar>		BasicExpBar_SubPlayer;
	TWeakObjectPtr<UProgressBar>		ExtraExpBar_SubPlayer;
	TWeakObjectPtr<UTextBlock>			TB_ExpPercentSubChar;

	TWeakObjectPtr<UTextBlock>			TB_CombatTimeText;
	TWeakObjectPtr<UTextBlock>			TB_CombatTime;

	TWeakObjectPtr<UTextBlock>			TB_AcquiredGoldText;
	TWeakObjectPtr<UB2RichTextBlock>	TB_AcquiredGold;

	TWeakObjectPtr<UTextBlock>			TB_CombatTimeLeft;
	TWeakObjectPtr<UTextBlock>			TB_CombatTimeRight;

	TWeakObjectPtr<UTextBlock>			TB_AcquiredGoldLeft;
	TWeakObjectPtr<UB2RichTextBlock>	TB_AcquiredGoldRight;
	TWeakObjectPtr<UTextBlock>			TB_Boost;

	TWeakObjectPtr<UHorizontalBox>		HB_SoldGold;
	TWeakObjectPtr<UTextBlock>			TB_SoldGoldLeft;
	TWeakObjectPtr<UB2RichTextBlock>	TB_SoldGoldRight;
	TWeakObjectPtr<UTextBlock>			TB_SoldBoost;

	TWeakObjectPtr<UTextBlock>			TB_Title;

	TWeakObjectPtr<UTextBlock>			STB_RepeatOpen;
	TWeakObjectPtr<UTextBlock>			STB_BoostOpen;
	TWeakObjectPtr<UTextBlock>			STB_Reward;

	TWeakObjectPtr<class UB2UIStageResultLevelUp>	UIP_LevelUp;

	TWeakObjectPtr<UB2UIRepeatBattleCountDown>		UIP_RepeatBattleCountDownP;
	TWeakObjectPtr<UB2UIRepeatBattleInGameInfo>		UIP_RepeatBattleInGameInfoP;

	TWeakObjectPtr<UPanelWidget>					CP_NewStageInfo;
	TWeakObjectPtr<UB2ScrollBox>					SB_RewardItems;
	TWeakObjectPtr<UHorizontalBox>					HB_AcqItemGroupPanel;

	TArray<TWeakObjectPtr<UWidgetSwitcher>>			WS_ClearResultStar;
	TArray<TWeakObjectPtr<UB2RichTextBlock>>		TB_ClearResult;

	TWeakObjectPtr<class UB2UIHotTimeIcon>		UIP_Hottime;
	TWeakObjectPtr<UTextBlock>					TB_ActiveHotTime;
	TWeakObjectPtr<UOverlay>					O_ActiveHotTime;

private:
	bool bExpIncreaseStarted;
	bool bExpIncreaseFinished;
	bool bButtonClicked;

	bool bShowMainQuestReward;

	TArray<FB2Item>									RewardItemData;
	TArray<class UB2UISlotItem*>					BattleRewardItems;

	uint32 ToolTipCloseTicket;
	uint32 HandleServerError4015;
	uint32 HandleServerError4030;
		
	int32 LoadClearChapterNum();
};
