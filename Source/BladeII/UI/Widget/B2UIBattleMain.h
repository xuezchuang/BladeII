// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIDocBattle.h"
#include "B2UIRepeatBattleInGameInfo.h"
#include "B2UIInGameBuffIcon.h"
#include "B2UIBattleMain.generated.h"

/*
* This is a UI that appears when you entered a combat situation. (In the past, it was B2InGameHUDWidget_Player.)
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBattleMain : public UB2UIWidget, public IB2UIBackWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UIBattleMain(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	/* The function that operates based documents. */
	void BindDocMainChar(class UB2UIDocHero* DocHero); // Main here means "currently active" not the actual main character selection from lobby
	void BindDocSubChar(class UB2UIDocHero* DocHero); // Sub here means "currently inactive" not the actual sub character selection from lobby
	void BindDocSkill(int32 SkillInputIndex, class UB2UIDocSkill* SkillDoc);

	void Tag();
	void Tag(int32 MainClass, int32 SubClass);

	virtual void TagCommon(EPCClass MainClass, EPCClass SubClass);

public:

	void SetCounterDungeonLv();

	void SetPortraitMainHero(class UMaterialInterface* InPortraitMtrl);
	void SetPortraitSubHero(class UMaterialInterface* InPortraitMtrl);

	void SetNameMainChar(const FText& InText);
	virtual void SetLevelMainChar(int32 InValue);

	void SetHPMainChar(float CurHP, float MaxHP, float CurShield);
	void SetHPSubChar(float CurHP, float MaxHP);

	void SetOffenseEtherlCooltime(float InCoolTime);
	void SetDefenseEtherCooltime(float InCoolTime);
	void SetEnableOffenseEther(bool InEnable);
	void SetEnableDefenseEther(bool InEnable);
	void SetTagCooltime(float RemainingTagCooltime);
	void SetTagCooltimeVisible(bool bShowTagCooltime);
	void SetEnabledTagBtn(bool bEnabledTag);
	void SetEnabledSubPortrait(bool bEnabledTag);
	
	void SetVisibilitySB_TbMainCharName(bool bVisibility);
	void SetVisibilityPlayerInfoWidget(bool bVisibility);
	void SetVisibilityTagWidget(bool bVisibility);
	void SetVisibilityGoldWidget(bool bVisibility);
	void SetVisibilityPlayTimeWidget(bool bVisibility);
	void SetVisibilityStageNumberWidget(bool bVisibility);
	void SetVisibilityCRTInfoBGImage_01(bool bVisibility);
	void SetVisibilityPauseButton(bool bVisibility);
	void SetVisibilityHotTime(bool bVisibility);

	void CheckWidgetVisibilityForMode();

	void SetGold(int32 InValue);

	/* To really pause the game one tick after pause button click. */
	//void HandlePendingPause(); 

	void UpdateStagePlayTimeInfoDisplay(int32 InCurrentTime);
	void SetVisibilityClearGradeCondition(bool InShow);

	void SetWidgetForCounterAttackBattle();
	void SetWidgetForNormalBattle();

	void SetAttackEnable(bool bToSet);
	void SetGuardEnable(bool bEnabled);

	void SetCounterFinishMode();
	void SetTagPCDead();
	
	void SetQTEEnable(EQTEUIAttackEnableTimingState InEnableState);
	virtual void HideHUDForQTE(bool bHide);

private:
	/** Overall UI part and this BattleMain UI visibility setting by QTE state. */
	void CheckForQTEStateUISetup();
	
	void UpdateGuildBuff();

	void ConditionalCreateQTEUIP();
	void DestroyQTEUIP(bool bUnloadTAsset);
	
public:
	void SetSkillEnable(int32 SkillIndex, bool bEnable);
	void SetSkillLock(int32 SkillIndex, bool bLock);
	
	void SetForQTEMountUI(bool ToSet);

	void SetComboUIPosition(FVector2D vPos);

	void InitCounterSuggestTimer();
	void AddCounterSuggestTimer(float time);
	void RemoveAllCounterSuggestRing();
	void OnPlayCounterSuggetAnim();
	void OnPauseCounterSuggestRing(float time);
	void OnUnPauseCounterSuggestRing();
	void OnStageBegin();

	/** To do some opening animation, which was invoked at OnOpen_BP, but was not visible at that time. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void OnStageBeginEventSceneEnd();

	/* When the tag, for one must be processed in BP. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void Tag_BP(EPCClass MainClass, EPCClass SubClass, bool bIsTagAttack);

	/** To do some animation at gold sucking moment. Other most gold sucking widget stuff are in B2FloatingGoldAcq */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void SetGold_BP(int32 AcquiredAmount);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void PlayTagSuggestAnim_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void StopTagSuggestAnim_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void PlayTagSuggestByPlayerConditionAnim_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void StopTagSuggestByPlayerConditionAnim_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayPauseBtnSound_BP();

	void UpdateTagSuggestDialogue(const bool bIsForTagAttack);
	void PlayTagDialogueSound(EPCClass SoundPlayPCClass, EPCClass IntarActionPCClass);

	/** PreDropIndex is 1 ~ 3. Some seconds (StageClearGradePreDropAnimTime) before actual stage clear grade drop */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void PlayStageClearGradePreDropAnim(int32 PreExpectedGrade);

	UFUNCTION(BlueprintImplementableEvent)
		void ShowAccuracy(int32 Accuracy);

	/** To do some opening animation, which was invoked at OnOpen_BP, but was not visible at that time. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void OnStageEventDirectorEndScene_BP();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void OnPlayAnimCounterSuggestRing_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void OnPlayResurrection_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleMain")
	void OnInitAnimationBegin();

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleMain")
	void OnInitAnimationFinish();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void PlayAnim_Ether_OblivionAnim_BP();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
	virtual void CloseBattleMainUIEvent(bool Valuse);
private:
	void UpdateTagCooltimeMaterial(float RemainingTagCooltime);
protected:
	void UpdateAttackButtonStyleToPCClass(EPCClass InPCClass);
private:
	//===================================================Click
	UFUNCTION()
		void OnPressedSkillBtn(class UB2UIBattleSkill* ClickedSkill);
	UFUNCTION()
		void OnClickTagBtn();
	UFUNCTION()
		void OnPressedPauseBtn();

	UFUNCTION()
		void OnPressedAttackBtn();
	UFUNCTION()
		void OnReleasedAttackBtn();

	UFUNCTION()
		void OnPressedGuardBtn();
	UFUNCTION()
		void OnReleasedGuardBtn();

	UFUNCTION()
		void OnClickCounterFinishBtn();
	
	UFUNCTION()
		void OnPressedShowClearGradeConditionBtn();
	UFUNCTION()
		void OnReleasedShowClearGradeConditionBtn();

	UFUNCTION()
		void OnClickBtnChat();

	//====================================================Doc Delegate
	UFUNCTION()
		void OnChangedCurPCClass(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass);
	UFUNCTION()
		void OnChangedCurrPCHealth(class UB2UIDocBase* Sender, float CurrPCHealth, float PrevCurrPCHealth);
	UFUNCTION()
		void OnChangedCurrPCMaxHealth(class UB2UIDocBase* Sender, float CurrPCMaxHealth, float PrevCurrPCMaxHealth);
	UFUNCTION()
		void OnChangedCurrPCShield(class UB2UIDocBase* Sender, float CurrPCShield, float PrevCurrPCShield);
	UFUNCTION()
		void OnChangedTagPCHealth(class UB2UIDocBase* Sender, float TagPCHealth, float PrevTagPCHealth);
	UFUNCTION()
		void OnChangedTagPCMaxHealth(class UB2UIDocBase* Sender, float TagPCMaxHealth, float PrevTagPCMaxHealth);
	UFUNCTION()
		void OnChangedIsCountingTagCooltime(class UB2UIDocBase* Sender, bool IsCountingTagCooltime, bool PrevIsCountingTagCooltime);
	UFUNCTION()
		void OnChangedIsTagPossible(class UB2UIDocBase* Sender, bool IsTagPossible, bool PrevIsTagPossible);
	UFUNCTION()
		void OnChangedIsTagDisabledByCooltime(class UB2UIDocBase* Sender, bool IsTagDisabledByCooltime, bool PrevIsTagDisabledByCooltime);
	UFUNCTION()
		void OnChangedOffenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime);
	UFUNCTION()
		void OnChangedDefenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime);
	UFUNCTION()
		void OnChangedIsDefenseEther(class UB2UIDocBase* Sender, bool IsDefenseEther, bool PrevIsDefenseEther);
	UFUNCTION()
		void OnChangedIsOffenseEther(class UB2UIDocBase* Sender, bool IsOffenseEther, bool PrevIsOffenseEther);
	UFUNCTION()
		void OnChangedRemainingTagCooltime(class UB2UIDocBase* Sender, float RemainingTagCooltime, float PrevRemainingTagCooltime);
	UFUNCTION()
		void OnChangedGold(class UB2UIDocBase* Sender, int32 Gold, int32 PrevGold);
	UFUNCTION()
		void OnChangedbCurrentlyBossAppeared(class UB2UIDocBase* Sender, bool bCurrentlyBossAppeared, bool PrevbCurrentlyBossAppeared); //It detects whether the time to display the Boss status.
	UFUNCTION()
		void OnChangedIsTagSuggested(class UB2UIDocBase* Sender, bool bTagSuggested, bool bPrevTagSuggested); 
	UFUNCTION()
		void OnChangedStagePlayTime(class UB2UIDocBase* Sender, int32 CurrentTime, int32 PrevTime);
	UFUNCTION()
		void OnChangedCharacterSkillBuffState(class UB2UIDocBase* Sender, TArray<FUISkillBuffDisplayInfo_Character> CurrentState, TArray<FUISkillBuffDisplayInfo_Character> Prevstate);
	UFUNCTION()
		void OnChangedCounterAttackPoint(class UB2UIDocBase* Sender, int32 CurrPoint, int32 PrevPoint);
	UFUNCTION()
		void OnChangedQTEEnableState(class UB2UIDocBase* Sender, EQTEUIAttackEnableTimingState CurrQTEState, EQTEUIAttackEnableTimingState PrevQTEState);
	UFUNCTION()
		void OnChangedQTEMounted(class UB2UIDocBase* Sender, bool bCurrentMounted, bool bPrevMounted);
	UFUNCTION()
		void OnChangedQTEMountedAndReadyForAttack(class UB2UIDocBase* Sender, bool bCurrentMountedAndReady, bool bPrevMountedAndReady);
	UFUNCTION()
		void OnChangedPlayerInQTEState(class UB2UIDocBase* Sender, bool bCurrentInQTEState, bool bPrevInQTEState);

	UFUNCTION()
		void OnChangedResurrectBuffValueAttack(class UB2UIDocBase* Sender, int32 NewValue, int32 PrevValue);
	UFUNCTION()
		void OnChangedResurrectBuffValueDefense(class UB2UIDocBase* Sender, int32 NewValue, int32 PrevValue);
	UFUNCTION()
		void OnChangedResurrectBuffValueHealth(class UB2UIDocBase* Sender, int32 NewValue, int32 PrevValue);

	UFUNCTION()
		void OnTagBTNEnableFunction(bool IsEnable);

	UFUNCTION()
	void OnAbnormalStateChanged(class UB2UIDocBase* Sender, bool NewValue, bool PrevValue);

	UFUNCTION()
	bool RedDotCondition_Chat();

	virtual void RegisterUIMarkForRedDot() final;

	void OnChangedGuradCoolTime(class UB2UIDocBase* Sender, float GuradCoolTime, float PrevGuradCoolTime);
	void OnChangedHasNotTagDefence(class UB2UIDocBase* Sender, bool HasNotTagDefence, bool PrevHasNotTagDefence);
	void SetGuardUI(bool HasNotTagDefence);

	uint32 SetCloseBattleMainUIEventID;
protected:
	/* Name rules for obtaining BPSkillAssets in the code. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
		FString PrefixSkillAsset = TEXT("");

	/** Before this time of the grade getting actually down, 
	 * clear grade pre-drop anim will be played, and time text color is changed. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
		float StageClearGradePreDropAnimTime;

	uint32 bLastPreExpectedGradeDifferent : 1; // Cached for playing anim purpose..

	UPROPERTY(EditAnywhere, Category = "BladeII")
		FSlateColor StagePlayTimeTextColor_Normal;
	/** Color applied when clear grade is about to drop (StageClearGradePreDropAnimTime sec ago) */
	UPROPERTY(EditAnywhere, Category = "BladeII")
		FSlateColor StagePlayTimeTextColor_PreDrop;

	// Buff icon classes for each buff type..
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIInGameBuffIcon> BuffIconClass_Stage;
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIInGameBuffIcon> BuffIconClass_Resurrect;
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIInGameBuffIcon> BuffIconClass_Skill;
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIInGameBuffIcon> BuffIconClass_Guild;

	// Put the whole panel size here.
	UPROPERTY(EditAnywhere, Category = "BladeII")
	FVector2D BuffIconSize;

	/////////////////////////////////////////////////////////////
	// Dynamically created UIPs
	/** Tag Anim part is dynamically loaded to save memory. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSoftClassPtr<class UB2UIBattleTagAnim> TagAnimUIPClass;
	UPROPERTY(Transient)
	TSubclassOf<class UB2UIBattleTagAnim> LoadedTagAnimUIPClass;
	UPROPERTY(Transient)
	class UB2UIBattleTagAnim* CreatedTagAnimUIP;

	/** LIke TagAnimUIP, QTE UIP is dynamically created, for special purpose other than saving memory,
	 * showing this part only while other battle UIs are hidden. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSoftClassPtr<class UB2UIBattleQTE> QTEUIPClass;
	UPROPERTY(Transient)
	TSubclassOf<class UB2UIBattleQTE> LoadedQTEUIPClass;
	UPROPERTY(Transient)
	class UB2UIBattleQTE* CreatedQTEUIP;
	/////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadWrite, Category = "BladeII")
	float CounterSuggestRingTime; // Set BPContruct SuggetAnimation full time.
public:
	// Declared as public to use LoadAndBeginTAssetBattleTagAnimUIP common util.
	UFUNCTION()
	void OnBattleTagAnimUIPExpired(class UB2UIBattleTagAnim* ExpiredUIP);

	class UB2UIBattleQTE* GetCreatedQTEUIP() { return CreatedQTEUIP; }

protected:
	TArray<TWeakObjectPtr<UObject>>								CommonWidgets;
	TArray<TWeakObjectPtr<UObject>>								NormalBattleWidgets;
	TArray<TWeakObjectPtr<UObject>>								CounterAttackWidgets;

	TArray<TPair<TWeakObjectPtr<UObject>, ESlateVisibility>>	WidgetsToHideInMountAttackState;

	
	TWeakObjectPtr<UPanelWidget>			CP_MainCharacterStatus;
	TWeakObjectPtr<UPanelWidget>			CP_Gold;
	TWeakObjectPtr<UPanelWidget>			CP_PlayTimeSet;
	TWeakObjectPtr<UPanelWidget>			P_StageNumberSet;
	TWeakObjectPtr<UPanelWidget>			P_Tag;
	
	TWeakObjectPtr<UTextBlock>				TB_Gold;
	TWeakObjectPtr<UTextBlock>				TB_StageNumber;
	TWeakObjectPtr<UTextBlock>				TB_StageDifficulty;

	TWeakObjectPtr<UImage>					IMG_SkillSetBG;
	TWeakObjectPtr<UPanelWidget>			CP_Attack;
	TWeakObjectPtr<UPanelWidget>			CP_PortraitSub;

	TWeakObjectPtr<UPanelWidget>			CP_MountAttack;

	TWeakObjectPtr<UTextBlock>				TB_StagePlayingTime;
	TWeakObjectPtr<UImage>					IMG_StageClearGradeStarTwinkle_01;
	TWeakObjectPtr<UImage>					IMG_StageClearGradeStarTwinkle_02;
	TWeakObjectPtr<UImage>					IMG_StageClearGradeStarTwinkle_03;

	TWeakObjectPtr<UB2Button>				BTN_ShowClearGradeCondition;
	TWeakObjectPtr<UTextBlock>				TB_ClearGradeCondition;

	TWeakObjectPtr<UButton>					BTN_Tag;
	TWeakObjectPtr<UButton>					BTN_Pause;
	TWeakObjectPtr<UButton>					BTN_Pause2;
	TWeakObjectPtr<UButton>					BTN_Attack;
	TWeakObjectPtr<UButton>					BTN_Guard;
	TWeakObjectPtr<UButton>					BTN_MountAttack;

	TWeakObjectPtr<UTextBlock>				TB_GuradCoolTime;
	TWeakObjectPtr<UPanelWidget>			CP_GuardEnable;
	TWeakObjectPtr<UPanelWidget>			CP_GuardDisable;

	TWeakObjectPtr<UB2Button>				BTN_Chat;

	TWeakObjectPtr<UPanelWidget>			CP_Guard;
	TWeakObjectPtr<UImage>					IMG_GuardBG;

	TWeakObjectPtr<UImage>					IMG_QTEBG;

	TWeakObjectPtr<class UB2UIBattleSkill>	UIP_CounterAttack_Skill;
	TWeakObjectPtr<class UB2UIWidgetBase>	UIP_CounterFinishAttack;
	TWeakObjectPtr<UImage>					FX_IMG_Enable;
	TWeakObjectPtr<UImage>					IMG_CounterSuggestRing;

	TWeakObjectPtr<UImage>					IMG_PortraitMain;
	TWeakObjectPtr<UTextBlock>				TB_MainCharName;
	TWeakObjectPtr<UTextBlock>				TB_MainCharLevel;
	TWeakObjectPtr<UProgressBar>			PB_MainCharHP;
	TWeakObjectPtr<UProgressBar>			PB_MainCharShield;
	TWeakObjectPtr<UImage>					CRTInfoBGImage_01;
	TWeakObjectPtr<UScaleBox>				SB_TbMainCharName;

	TWeakObjectPtr<UImage>					IMG_PortraitSub;
	TWeakObjectPtr<UImage>					IMG_PortraitSubDisable;
	TWeakObjectPtr<UProgressBar>			PB_SubCharHP;

	TWeakObjectPtr<UImage>					IMG_TagCooltime;
	TWeakObjectPtr<UImage>					IMG_TagCooltimeBottom;
	TWeakObjectPtr<UTextBlock>				TB_TagCooltimeNumBottom;

	TWeakObjectPtr<UPanelWidget>			CP_TagBottomEnabledTrue;
	TWeakObjectPtr<UPanelWidget>			CP_TagBottomEnabledFalse;

	TWeakObjectPtr<UImage>					IMG_FX_TagSuggest;
	TWeakObjectPtr<UPanelWidget>			CP_TagSuggest_TagCharChallenge;
	TWeakObjectPtr<UTextBlock>				TB_TagSuggest_TagCharChallenge;
	TWeakObjectPtr<UPanelWidget>			CP_TagAttackSuggestSet;
	TWeakObjectPtr<UTextBlock>				TB_TagAttack;

	TWeakObjectPtr<UTextBlock>				TB_TagAnim_Gladiator;
	TWeakObjectPtr<UTextBlock>				TB_TagAnim_Assassin;
	TWeakObjectPtr<UTextBlock>				TB_TagAnim_Wizard;
	TWeakObjectPtr<UTextBlock>				TB_TagAnim_Fighter;

	TWeakObjectPtr<UTextBlock>				TB_DungeonLv;

	TWeakObjectPtr<UOverlay>				O_Death;

	TWeakObjectPtr<class UB2UIBattleCombo>		UIP_BattleCombo;
	TWeakObjectPtr<class UB2UIBattleVignette>	UIP_BattleVignette;

	TWeakObjectPtr<class UB2UIHotTimeIcon>		UIP_Hottime;

	/* Skills list of the currently active hero. */
	TArray<class UB2UIBattleSkill*>			SkillAssets;

	class UMaterialInstanceDynamic*			TagCooltimeDisplayMID;
	class UMaterialInstanceDynamic*			TagCooltimeDisplayMIDBottom;
	
	TWeakObjectPtr<UB2UIRepeatBattleInGameInfo> UIP_RepeatBattleInGameInfoP;

	TWeakObjectPtr<UHorizontalBox> HB_StageResurrectBuffIconSet;
	
	TWeakObjectPtr<UHorizontalBox>	HB_Resurrect;
	TWeakObjectPtr<UHorizontalBox>	HB_Guild;

	TWeakObjectPtr<class UB2UIBattleNotice>		UIP_BattleNotice;

	TWeakObjectPtr<class UB2UIEtherSetSkillIcon>		UIP_OffenseEtherIcon;
	TWeakObjectPtr<class UB2UIEtherSetSkillIcon>		UIP_DefenseEtherIcon;
	TWeakObjectPtr<class UB2UIPAutoBattleIcon>			UIP_AutoBattleIcon;
	TWeakObjectPtr<UImage>					IMG_EvadeSkillCooltime;

	TWeakObjectPtr<UPanelWidget>			CP_MainButton;
	
	TArray<UB2UIInGameBuffIcon*> AllMyBuffIcons_Resurrection; // All elements created in HB_StageResurrectBuffIconSet
	TArray<UB2UIInGameBuffIcon*> AllMyBuffIcons_Skill; // All elements created in HB_SkillBuffIconSet
	TArray<UB2UIInGameBuffIcon*> AllMyBuffIcons_Guild; 
	TArray<FUISkillBuffDisplayInfo_United> AllCollectedSkillBuffData; // Collected and possibly sorted.

	TWeakObjectPtr<UTextBlock>				TB_CounterDungeonLv;

	UB2UIInGameBuffIcon* CreateBuffIconCommon(TSubclassOf<UB2UIInGameBuffIcon> InCreateClass, UHorizontalBox* InToCreatePanel = nullptr);
	UB2UIInGameBuffIcon_Stage* CreateOrFindStageBuffIcon(EStageBuffType InBuffType);
	UB2UIInGameBuffIcon_Resurrect* CreateOrUpdateResurrectBuffIcon(EResurrectBuffType InBuffType);
	UB2UIInGameBuffIcon_Skill* CreateOrUpdateSkillBuffIcon(EUIUnitedSkillBuffType InType, float RemainingTime);
	UB2UIInGameBuffIcon_GuildBuff* CreateOrUpdateGuildBuffIcon(EItemOption InBuffType);

	void DestroyAllMyBuffIcons_Resurrection();
	void DestroyAllMyBuffIcons_Skill();
	void DestroyAllMyBuffIcons_Guild();
	
	void UpdateSkillBuffIcons();
	void CollectAndSortSkillBuffData(bool& bOutOnlyCountTimeChanged);
	void UpdateSkillBuffIconsByCollectedData(bool bUpdateCountTimeOnly);

	void BeginTagAnim(EPCClass NewTaggedClass, bool bIsTagAttack);
	void DestroyTagAnimUIP(bool bUnloadTAsset);

	virtual void RequestAsyncLoadWidgets();
	void OnCompleteAsyncLoad(const FString& RequesetName);
	// AsyncLoadName : Not the asset name, just async request identification.
	const FString GetQTEAsyncLoadName() { return TEXT("QTEWidget");	}
	const FString GetTagAnimAsyncLoadName() { return TEXT("TagAnimWidget"); }

	void ProcessEvadeCoolTimeUI();
	bool CheckInDebuffAction();
protected:
	/* The "currently active" HeroDocument. It is not the same to the lobby's main character selection. */
	class UB2UIDocHero*						MainHeroDoc;

	/* This document is a Hero "waiting for a tag". It is not the same to the lobby's sub character selection. */
	class UB2UIDocHero*						SubHeroDoc;

	/** True for one tick after player pressed pause button and the pause menu is invoked but game is not paused yet. */
	uint32 bPendingPause : 1;

	EB2GameMode CurrentGameModeType;

	bool bIsTagDeath;
	bool bIsCountingTagCooltime;
	TArray<FTimerHandle> CounterAttackTimers;

	uint32 StopVisibleChatPopupTicket;

	UFUNCTION()
	void OnClickBtnDevFxLODPC();
	UFUNCTION()
	void OnClickBtnDevFxLODMob();
	UFUNCTION()
	void OnClickBtnDevFxLODBoss();
#if !UE_BUILD_SHIPPING // For effect resource optimization..
	TWeakObjectPtr<UPanelWidget> P_DevLODTest;
	TWeakObjectPtr<UB2Button> BTN_Dev_FxLOD_PC;
	TWeakObjectPtr<UB2Button> BTN_Dev_FxLOD_Mob;
	TWeakObjectPtr<UB2Button> BTN_Dev_FxLOD_Boss;
	TWeakObjectPtr<UTextBlock> TB_Dev_FxLOD_PC;
	TWeakObjectPtr<UTextBlock> TB_Dev_FxLOD_Mob;
	TWeakObjectPtr<UTextBlock> TB_Dev_FxLOD_Boss;
public:
	void UpdateDevFxLODWidgets();
#endif
};

/** Utli to set both HP and shield progress bar properly according to those values. It is required at multiple places.*/
void SetHPShieldPBCommon(UProgressBar* InHPPB, UProgressBar* InShieldPB, float HPValue, float MaxHPValue, float ShieldValue);