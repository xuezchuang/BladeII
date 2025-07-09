// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "EventSubsystem.h"
#include "B2UICharacterSelectPage.generated.h"

DECLARE_DELEGATE_OneParam(FCharacterSlot, EPCClass)

UCLASS()
class BLADEII_API UB2UICharacterSelectPageSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetHeroInfo(EPCClass HeroClass, int32 Lv, int32 CombatPower);
	void SetHeroName(FText InText);
	void SetHeroLv(int32 Lv);
	void SetCombatPower(int32 CombatPower);

	void SlotSelect(int32 State);
	void SetSlotDelegate(FCharacterSlot Indelegate);
	void SetMainHero(bool IsMain);
	void SetSelectArrow(bool IsSelect);

	void SetSlotSelectable(bool bSelectable);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void SelectCharacterStateBP(int32 State);

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void SelectCharacterHero(EPCClass PCClass);


	UFUNCTION()
		void OnClickSelectButton();

protected:
	TWeakObjectPtr<UTextBlock> TB_SelectHeroLevel;
	TWeakObjectPtr<UTextBlock> TB_SelectHeroName;
	TWeakObjectPtr<UTextBlock> TB_SelectCombatPower;

	TWeakObjectPtr<UTextBlock> TB_UnSelectHeroLevel;
	TWeakObjectPtr<UTextBlock> TB_UnSelectHeroName;
	TWeakObjectPtr<UTextBlock> TB_UnSelectCombatPower;
	TWeakObjectPtr<UTextBlock> TB_CharNumber;

	TWeakObjectPtr<UB2Button> BTN_Select;

	TWeakObjectPtr<UImage> IMG_Arrow;

	EPCClass SlotClass;
	FCharacterSlot SlotDelegate;

};


UCLASS()
class BLADEII_API UB2UICharacterSelectPage : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UB2UICharacterSelectPage(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void UpdateStaticText() override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	virtual void SetChangeCombatPower(int32 InPower, int32 InAttack, int32 InDefense);
	void UpDateHero();

	void SlotDelegateFunction(EPCClass InValue);
	bool CanStartBattle();
	virtual void BattleStart();
	void SetupOnRepeatBattleStateChange(bool bIsOn);
	void StartNextRepeatBattleCountdown();
	void StopNextRepeatBattleCountdown();
	void StartNextRepeatBattle();
	void CheckStartButton();
	bool CheckAbleTime();
	void SetEnableStageGameStarted(bool InEnable);
	void PageBlock(bool bIsBlock);

	void CheckControlTutorialNextStep();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void PlayChangeChacterAnimBP();
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void ToolTipStartAni();

	UFUNCTION()
		void OnClickThreeStarToolTipOpen();
	UFUNCTION()
		void OnClickLoopAllToolTipOpen();
	UFUNCTION()
		void OnClickOneStarToolTipOpen();

	UFUNCTION()
		void OnClickMainButton();
	UFUNCTION()
		void OnClickSubButton();
	UFUNCTION()
		void OnClickCBTrebleAdventure();
	UFUNCTION()
		void CheckTriple(bool bCheck);
	UFUNCTION()
		void EnabledTriple(bool bCheck);
	UFUNCTION()
		void CheckAutoBattle(bool bCheck);
	void UnCheckAutoBattle();
	UFUNCTION()
		void EnabledAutoBattle(bool bCheck);
	UFUNCTION()
		void CheckStorySkip(bool bCheck);
	UFUNCTION()
		void EnabledStorySkip(bool bCheck);
	UFUNCTION()
		void CheckLoopAll(bool bCheck);
	void UnCheckLoopAll();
	UFUNCTION()
		void EnabledLoopAll(bool bCheck);
	UFUNCTION()
		void OnClickBtnChangeMainSub();
	UFUNCTION()
		void OnClickGoHero();
	UFUNCTION()
		void OnClickBtnNotifyNoWingIngame();
	UFUNCTION()
		void OnClickAutoSell();
	
	UFUNCTION()
		void OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn);

	UFUNCTION()
		void OnClickBTNAutoClear();

	UFUNCTION()
		void OnClickBTNToolTipDisableAutoClear();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladiII")
		bool IsSingleSelect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladiII")
		int32 AutoClearStar;

private:
	bool IsBoostOverCount();
	bool IsRepeatOverCount();

	void SetAutoSellBtnState();
	bool IsSetAutoSellBtnState();
	void SaveDocFilterInfo();
	void SetAutoClear(bool InValue);
	void SetAutoClearCount();

	void SetCharacterInfo();
	void ResponseSweepStage(const FB2SweepStageInfo& InSweepStageinfo);
	void ResponseSweepStageError(const FB2SweepStageInfo& InSweepStageinfo, int32 InErrorCode);

private:
	TArray<TWeakObjectPtr<UB2UICharacterSelectPageSlot>> HeroArr;

	TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_Goods;
	TWeakObjectPtr<UB2Button> BTN_MainHero;
	TWeakObjectPtr<UB2Button> BTN_SubHero;
	TWeakObjectPtr<UTextBlock> TB_TripleCount;

	TWeakObjectPtr<UTextBlock> STB_MainChar;
	TWeakObjectPtr<UTextBlock> STB_SubChar;
	TWeakObjectPtr<UTextBlock> STB_Title;
	TWeakObjectPtr<UTextBlock> STB_DirectionSkip;

	TWeakObjectPtr<UB2Image> IMG_EmptyMain;
	TWeakObjectPtr<UB2Image> IMG_EmptySub;

	TWeakObjectPtr<class UB2UIStartButton> UIP_StartBT;
	TWeakObjectPtr<class UB2UIFairyLinkButton> UIP_FairyLinkButton;

	TWeakObjectPtr<UCheckBox> CB_Triple;
	TWeakObjectPtr<UCheckBox> CB_AutoBattle;
	TWeakObjectPtr<UCheckBox> CB_StoryCheck;
	TWeakObjectPtr<UCheckBox> CB_LoopAll;
	TWeakObjectPtr<UB2Image> IMG_AutoBattleLock;
	TWeakObjectPtr<UB2Image> IMG_TrableAdventureLock;
	TWeakObjectPtr<UPanelWidget> HB_TripleNUM;
	TWeakObjectPtr<UPanelWidget> HB_AutoClearNUM;
	TWeakObjectPtr<UB2Image> IMG_StoryLock;
	TWeakObjectPtr<UB2Image> IMG_LoopAllLock;
	TWeakObjectPtr<class UB2UIRepeatBattleCountDown> UIP_RepeatBattleCountDownP;
	TWeakObjectPtr<UB2Button> BTN_CharacterChange;
	TWeakObjectPtr<UB2Button> BTN_ToolTipDisableTrableAdventure;
	TWeakObjectPtr<UB2Button> BTN_ToolTipDisableAutoBattle;
	TWeakObjectPtr<UB2Button> BTN_ToolTipDisableStorySkip;
	TWeakObjectPtr<UB2Button> BTN_ToolTipDisableLoopAll;
	TWeakObjectPtr<UB2Button> BTN_GoHero;

	TWeakObjectPtr<UVerticalBox> VB_MainCharacter;
	TWeakObjectPtr<UVerticalBox> VB_SubCharacter;

	TWeakObjectPtr<UTextBlock> TB_AutoText;
	TWeakObjectPtr<UTextBlock> TB_TripleText;

	TWeakObjectPtr<UTextBlock> X_TB_Recommend;
	TWeakObjectPtr<UTextBlock> X_TB_RecommendPower;
	TWeakObjectPtr<UTextBlock> X_TB_Hero;
	TWeakObjectPtr<UTextBlock> X_TB_HeroPower;

	TWeakObjectPtr<UTextBlock> TB_LoopAll;

	TWeakObjectPtr<UB2RichTextBlock> RTB_LockToolTip;
	TWeakObjectPtr<class UB2UIRecommandPower> UIP_StageRecommendPower;

	TWeakObjectPtr<class UB2UIHotTimeIcon> UIP_Hottime;
	TWeakObjectPtr<UTextBlock> TB_ActiveHotTime;
	TWeakObjectPtr<UOverlay> O_ActiveHotTime;

	TWeakObjectPtr<UOverlay> O_NoWingSet;
	TWeakObjectPtr<UB2Button> BTN_NotifyNoWingIngame;

	// Auto Sell
	TWeakObjectPtr<UWidgetSwitcher> WS_AutoSell;
	TWeakObjectPtr<UB2Button> BTN_AutoSell;
	TWeakObjectPtr<UTextBlock> TB_AutoSell;
	TWeakObjectPtr<UB2Button> BTN_ActiveAutoSell;
	TWeakObjectPtr<UTextBlock> TB_ActiveAutoSell;

	// Auto Clear
	TWeakObjectPtr<UB2Button> BTN_AutoClear;
	TWeakObjectPtr<UTextBlock> TB_AutoClear;
	TWeakObjectPtr<UTextBlock> TB_AutoClearCount;
	TWeakObjectPtr<UB2Button> BTN_ToolTipDisableAutoClear;
	TWeakObjectPtr<UB2Image> IMG_AutoClear;
	
	TWeakObjectPtr<UImage> FX_ToogleOn;

private:
	int32 ChangeMainClass;
	int32 ChangeSubClass;
	int32 NowCombatPower;
	int32 RecommendPower;
	bool StageGameStarted;

	int32 ClientStageId;
	FServerStageID ServerStageId;
	EStageDifficulty StageDifficulty;
	FTimerHandle StageGameStartedResetTH;

	int32 BoostCount;

	TArray<std::shared_ptr<issue>> Issues;

};
