#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIInGameBuffIcon.h"
#include "B2UIPVP1on1BattleMain.generated.h"

struct FB2UIPVP1on1BattleMainCachedAssetPerChar
{
	TWeakObjectPtr<UButton>					BTN_Tag;
	TWeakObjectPtr<UImage>					IMG_PortraitMain;
	TWeakObjectPtr<UImage>					IMG_MyBadge;
	TWeakObjectPtr<UTextBlock>				TB_MainCharName;
	TWeakObjectPtr<UTextBlock>				TB_MainCharLevel;
	TWeakObjectPtr<UProgressBar>			PB_MainCharHP;
	TWeakObjectPtr<UProgressBar>			PB_MainCharShield;

	TWeakObjectPtr<UTextBlock>				TB_CurrentHP;
	TWeakObjectPtr<UTextBlock>				TB_MaxHP;

	TWeakObjectPtr<UImage>					IMG_PortraitSub;
	TWeakObjectPtr<UImage>					IMG_PortraitSubDisable;
	TWeakObjectPtr<UProgressBar>			PB_SubCharHP;

	TWeakObjectPtr<UImage>					IMG_TagCooltime;
	TWeakObjectPtr<UImage>					IMG_TagCooltimeBottom;
	TWeakObjectPtr<UTextBlock>				TB_TagCooltimeNum;

	TWeakObjectPtr<UOverlay>				O_Death;

	TWeakObjectPtr<UImage>					IMG_FX_TagSuggest;

	TWeakObjectPtr<UHorizontalBox>			HB_BuffIconSet;
	TWeakObjectPtr<UHorizontalBox>			HB_Guild;

	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_PVPGrade;

	class UMaterialInstanceDynamic*			TagCooltimeDisplayMID;
	class UMaterialInstanceDynamic*			TagCooltimeDisplayMIDBottom;

	TWeakObjectPtr<UPanelWidget>			CP_TagBottomEnabledTrue;
	TWeakObjectPtr<UPanelWidget>			CP_TagBottomEnabledFalse;
	TWeakObjectPtr<UImage>			IMG_TagOblivionLockBG;

	TWeakObjectPtr<UB2UIDocBase>			CachedDoc;

	bool bIsCountingTagCoolTime;
	
	bool IsLocalPlayer;

	FB2UIPVP1on1BattleMainCachedAssetPerChar()
	{
		bIsCountingTagCoolTime = false;
		IsLocalPlayer = false;
	}

	void SetPortraitMainHero(class UMaterialInterface* InPortraitMtrl);
	void SetPortraitSubHero(class UMaterialInterface* InPortraitMtrl);
	void SetNameMainChar(const FText& InText);
	void SetLevelMainChar(int32 InValue);
	void SetHPMainChar(float CurHP, float MaxHP, float CurShield);
	void SetTextHPMainChar(bool bShow, float CurHP, float MaxHP);
	void SetHPSubChar(float CurHP, float MaxHP);
	void SetTagPossible(bool);

	void SetPortraitMainHero(UObject* WorldContextObject, UB2UIDocBase* InUIDocBase);
	void SetPortraitSubHero(UObject* WorldContextObject, UB2UIDocBase* InUIDocBase);
	void SetNameMainChar(UB2UIDocBase*);
	void SetLevelMainChar(UB2UIDocBase*);
	void SetHPMainChar(UB2UIDocBase*);
	void SetHPSubChar(UB2UIDocBase*);

	void SetTagCooltime(float RemainingTagCooltime);
	void SetIsCountingTagCooltime(bool bIsCounting);
	void SetTagCooltimeVisible(bool bShowTagCooltime);
	void UpdateTagCooltimeMaterial(float RemainingTagCooltime);

	void SetEnabledTagBtn(bool bEnabledTag);
	void SetEnabledSubPortrait(bool bEnabledTag);

	void SetGradeStar(int32 InGrade, int32 InStar);
	void SetVisibleGradeStar(bool bVisible);
	void SetMyBadgeImage(bool _isLocalPlayer);
};

#define DECLARE_PVP_ENTRY_DOC_DELEGATE(FUNCNAME, PARAMTYPE)			\
	UFUNCTION()														\
	void FUNCNAME##_Left(UB2UIDocBase*, PARAMTYPE, PARAMTYPE);		\
	UFUNCTION()														\
	void FUNCNAME##_Right(UB2UIDocBase*, PARAMTYPE, PARAMTYPE);				

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPVP1on1BattleMain : public UB2UIWidget, public IB2UIBackWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UIPVP1on1BattleMain(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetLeftTime(int32 LeftTimeSeconds);

	void InitializeBindDoc(class UB2UIDocBattle* InDocBattleForLocal, class UB2UIDocPVP1on1Rival* InDocBattleForRival);
	virtual void UnbindDoc() override;

	void BindDocMainChar(class UB2UIDocHero* InDocHero);
	void BindDocSubChar(class UB2UIDocHero* InDocHero);
	void BindDocSkill(int32 SkillInputIndex, class UB2UIDocSkill* SkillDoc);

	void SetLocalPlayerToLeft(bool bLeft);

	void SetLocalPlayerGradeStar(int32 InGrade, int32 InStar);
	void SetRivalPlayerGradeStar(int32 InGrade, int32 InStar);

	void SetLocalMyBadgeImage(bool _isLocalPlayer);
	void SetRivalMyBadgeImage(bool _isLocalPlayer);
	
	void SetDeathPortrait(const ENetMatchResult& result);
	void ShowResultImage(const ENetMatchResult& result);

	void SetLeaveEnemyText(ESlateVisibility InVisible);

	void SetOffenseEtherlCooltime(float InCoolTime);
	void SetDefenseEtherCooltime(float InCoolTime);
	void SetEnableOffenseEther(bool InEnable);
	void SetEnableDefenseEther(bool InEnable);

	void UpdateSkillBuffIcons();
	void CollectAndSortSkillBuffData(bool& bOutOnlyCountTimeChanged);
	void UpdateSkillBuffIconsByCollectedData(bool bUpdateCountTimeOnly);


	UB2UIInGameBuffIcon_Skill* CreateOrUpdateSkillBuffIcon(EUIUnitedSkillBuffType InType, float RemainingTime);
	UB2UIInGameBuffIcon_GuildBuff* CreateOrUpdateGuildBuffIcon(EItemOption InBuffType);

	void DestroyAllMyBuffIcons_Skill();
	void DestroyAllMyBuffIcons_Guild();


	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayTagSuggestAnim_Left_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void StopTagSuggestAnim_Left_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayTagSuggestAnim_Right_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void StopTagSuggestAnim_Right_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayTagSuggestByPlayerConditionAnim_Left_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void StopTagSuggestByPlayerConditionAnim_Left_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayTagSuggestByPlayerConditionAnim_Right_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void StopTagSuggestByPlayerConditionAnim_Right_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayFocusTimer_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void StopFocusTimer_BP();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void PlayResultImage_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void StopResultImage_BP();
	/* When the tag, for one must be processed in BP. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void Tag_BP(EPCClass MainClass, EPCClass SubClass, bool bIsTagAttack);

	/** To do some opening animation, which was invoked at OnOpen_BP, but was not visible at that time. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
	void OnPVPBeginEventSceneEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_AnimResultAction(ENetMatchResult InMatchResult);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleMain")
		void PlayAnim_Ether_OblivionAnim_BP();

	void OnChangedOffenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime);
	UFUNCTION()
	void OnChangedDefenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime);
	UFUNCTION()
	void OnChangedIsDefenseEther(class UB2UIDocBase* Sender, bool IsDefenseEther, bool PrevIsDefenseEther);
	UFUNCTION()
	void OnChangedIsOffenseEther(class UB2UIDocBase* Sender, bool IsOffenseEther, bool PrevIsOffenseEther);
	UFUNCTION()
	void OnAbnormalStateChanged(class UB2UIDocBase* Sender, bool NewValue, bool PrevValue);
	UFUNCTION()
	void OnChangedCharacterSkillBuffState(class UB2UIDocBase* Sender, TArray<FUISkillBuffDisplayInfo_Character> CurrentState, TArray<FUISkillBuffDisplayInfo_Character> Prevstate);

protected:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CloseWidgetDelegate() override;
private:
	UFUNCTION()
		void OnPressedSkillBtn(class UB2UIBattleSkill* ClickedSkill);	
	UFUNCTION()
		void OnPressedAttackBtn();
	UFUNCTION()
		void OnClickedPauseBtn();
	UFUNCTION()
		void OnReleasedAttackBtn();
	UFUNCTION()
		void OnPressedGuardBtn();
	UFUNCTION()
		void OnReleasedGuardBtn();
	UFUNCTION()
		void OnClickedTagBtn();

	
	//====================================================Doc Delegate
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedCurPCClass, int32);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedCurPCLevel, int32);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedCurrPCHealth, float);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedCurrPCMaxHealth, float);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedCurrPCShield, float);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedTagPCHealth, float);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedTagPCMaxHealth, float);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedIsCountingTagCooltime, bool);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedIsTagPossible, bool);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedIsTagDisabledByCooltime, bool);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedRemainingTagCooltime, float);
	DECLARE_PVP_ENTRY_DOC_DELEGATE(OnChangedIsTagSuggested, bool);	

	void OnChangedCurPCClassCommon(int32 CurPCClass, int32 PrevCurPCClass);

	void UpdateGuildBuff(ICharacterDataStore* CharacterDataStore);

	void OnChangedGuradCoolTime(class UB2UIDocBase* Sender, float GuradCoolTime, float PrevGuradCoolTime);
	void OnChangedHasNotTagDefence(class UB2UIDocBase* Sender, bool HasNotTagDefence, bool PrevHasNotTagDefence);
	void SetGuardUI(bool HasNotTagDefence);

private:
	bool bIsLeftControlledLocalPlayer;

	class UB2UIDocPVP1on1Rival*				BindedRivalDoc;

	/* The "currently active" HeroDocument. It is not the same to the lobby's main character selection. */
	class UB2UIDocHero*						MainHeroDoc;

	/* This document is a Hero "waiting for a tag". It is not the same to the lobby's sub character selection. */
	class UB2UIDocHero*						SubHeroDoc;

public:
	/* Name rules for obtaining BPSkillAssets in the code. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	FString PrefixSkillAsset;

protected:
	/** Tag Anim part is dynamically loaded to save memory. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSoftClassPtr<class UB2UIBattleTagAnim> TagAnimUIPClass;
	UPROPERTY(Transient)
	TSubclassOf<class UB2UIBattleTagAnim> LoadedTagAnimUIPClass;
	TSharedPtr<FStreamableHandle> TagAnimUIPAsyncHandle;
	UPROPERTY(Transient)
	class UB2UIBattleTagAnim* CreatedTagAnimUIP;

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<UB2UIInGameBuffIcon> BuffIconClass_Skill;
	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<UB2UIInGameBuffIcon> BuffIconClass_Guild;

	UPROPERTY(EditAnywhere, Category = "BladeII")
		FVector2D BuffIconSize;

public:
	// Declared as public to use LoadAndBeginTAssetBattleTagAnimUIP common util.
	UFUNCTION()
	void OnBattleTagAnimUIPExpired(class UB2UIBattleTagAnim* ExpiredUIP);

	void StartInGameFight();
	void BeginTagAnim(EPCClass NewTaggedClass, bool bIsTagAttack);
	void DestroyTagAnimUIP(bool bUnloadTAsset);

	void ProcessEvadeCoolTimeUI();

	void RequestAsyncLoadWidgets();
	void OnCompleteAsyncLoad(const FString& RequesetName);

	void UpdateAttackButtonStyleToPCClass(EPCClass InPCClass);

	FORCEINLINE void SetClickedPause(bool bClicked) { bClickedPause = bClicked; }

private:
	TWeakObjectPtr<UButton>					BTN_Pause;
	TWeakObjectPtr<UButton>					BTN_Attack;
	TWeakObjectPtr<UButton>					BTN_Guard;

	TWeakObjectPtr<UTextBlock>				TB_GuradCoolTime;
	TWeakObjectPtr<UPanelWidget>			CP_GuardEnable;
	TWeakObjectPtr<UPanelWidget>			CP_GuardDisable;

	TWeakObjectPtr<class UB2UIBattleCombo>		UIP_BattleCombo;
	TWeakObjectPtr<class UB2UIBattleVignette>	UIP_BattleVignette;

	FB2UIPVP1on1BattleMainCachedAssetPerChar LeftCharAssets;
	FB2UIPVP1on1BattleMainCachedAssetPerChar RightCharAssets;

	FB2UIPVP1on1BattleMainCachedAssetPerChar* LocalCharAssets;
	FB2UIPVP1on1BattleMainCachedAssetPerChar* RivalCharAssets;

	/* Skills list of the currently active hero. */
	TArray<class UB2UIBattleSkill*>			SkillAssets;

	TWeakObjectPtr<UTextBlock>				TB_LeftTime;
	TWeakObjectPtr<UTextBlock>				TB_Fight;
	
	TWeakObjectPtr<UTextBlock>				TB_TagAnim_Gladiator;
	TWeakObjectPtr<UTextBlock>				TB_TagAnim_Assassin;
	TWeakObjectPtr<UTextBlock>				TB_TagAnim_Wizard;
	TWeakObjectPtr<UTextBlock>				TB_TagAnim_Fighter;

	TWeakObjectPtr<UTextBlock>				TB_LeaveEnemy;

	TWeakObjectPtr<UImage>				IMG_FocusTimer;
	TWeakObjectPtr<UImage>				IMG_Victory;
	TWeakObjectPtr<UImage>				IMG_Lose;
	TWeakObjectPtr<UImage>				IMG_Draw;

	TWeakObjectPtr<UPanelWidget>		P_SkillSet;
	TWeakObjectPtr<class UB2UIPAutoBattleIcon> UIP_AutoBattleIcon;

	class UMaterialInstanceDynamic*			TagCooltimeDisplayMID;
	TWeakObjectPtr<class UB2UIEtherSetSkillIcon>		UIP_OffenseEtherIcon;
	TWeakObjectPtr<class UB2UIEtherSetSkillIcon>		UIP_DefenseEtherIcon;

	TWeakObjectPtr<class UB2UIBattleNotice>		UIP_BattleNotice;

	TWeakObjectPtr<UImage>					IMG_EvadeSkillCooltime;

	TWeakObjectPtr<UB2Button>				BTN_Tag;
	TWeakObjectPtr<UWidgetAnimation>		ANIM_TimeFocus;

	TArray<UB2UIInGameBuffIcon*> AllMyBuffIcons_Skill; 
	TArray<UB2UIInGameBuffIcon*> AllMyBuffIcons_Guild;
	TArray<FUISkillBuffDisplayInfo_United> AllCollectedSkillBuffData; // Collected and possibly sorted.

	uint32 StopPauseMenuEvent;
	bool bClickedPause;
	bool IsPlayingTimeFocusAnimation;
};