// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2SkillInfo.h"
#include "B2UIPlayerSkillInfo.h"
#include "B2UIBackWidget.h"
#include "B2UIStageResultLevelUp.generated.h"

#define TRACKING_LEVEL 11

enum class EStageLevelUpUIPhase : uint8
{ // Not all phases are ensured to be used. Some can be just skipped.
	SLVUUP_Hidden = 0,
	SLVUUP_Class1,
	SLVUUP_Class2,
	SLVUUP_Class3,
	SLVUUP_Class4,
	SLVUUP_End
};

DECLARE_DELEGATE(FOnLevelUpFXFinish);
/**
 * A part of stage result UI, to appear only when the character got level-up.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStageResultLevelUp : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:

	// Information to be set at beginning
	uint32 bIsMainCharLevelUp : 1;
	uint32 bIsSubCharLevelUp : 1;

	EStageLevelUpUIPhase ThisPhase;
	TArray<EPCClass> SelectedAllPCClass;

	// Auto phase advance interval when user does not touch. For basic level-up UI.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float AutoAdvanceInterval;
	UPROPERTY(EditAnywhere)
	TArray<FSkillBadgeImage> SkillBadgeImages;
public:
	
	UB2UIStageResultLevelUp(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void Init() override;

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

protected:
	/** Overridden to modify event signal timing */
	virtual void OnOpenComplete() override;
	virtual void OnCloseComplete() override;

private:
	void AdvancePhase(); // To be called for every area click or by timer.
	void SetLevelUpContent(EPCClass InClass, class UB2UIDocHero* InDocHero); // Sets level-up part visible.
	void SetNewSkillContent(EPCClass InClass, class UB2UIDocHero* InDocHero, int32 NewSkillID); // Sets new skill part visible.
	void SetSkillType(ESkillType InSkillType);
	void SetNewBrevetRankContnt(EPCClass InClass, class UB2UIDocHero* InDocHero, int32 BrevetRank);
	bool SetIfPlayerCharacterGotLevelUp(EPCClass CurrentClass);
	int32 GetNewAcquiredSkillID(EPCClass InClass, class UB2UIDocHero* InDocHero); // Zero or less return means no skill acquired.
public:
	void StartMe(TArray<EPCClass> AllPCClass); // It is about actual functional start of this page. To be called from outside at some proper timing.
	void SetAutoAdvanceInterval(float InValue);
private:
	void OnStart(); // Not like Init, the actual start of this widget.
	void CloseMe();
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnLevelUpSceneStart(EPCClass LevelUpClass);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnNewSkillSceneStart(EPCClass SkillAcquiredClass);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnNewBrevetRankSceneStart();
private:
	/** Override because this widget is expected to be used at paused state, and also to count when it is not hidden. */
	virtual void SetupWidgetCustomTimer() override;
	/** Override because this widget is expected to be used at paused state, and also to count when it is not hidden. */
	virtual void SetupWidgetCustomHideTimer() override;

	float CachedWidgetStartTime;
	void UpdateWidgetCustomTimer();

	float AutoAdvanceCounter; // Another timer stuff other than widget custom timer.
	float AACounterLastRealTimeSec;
	float AACounterThisRealTimeSec;
	void ResetAutoAdvanceCounter();

	UFUNCTION()
	void OnClickBtnWholeArea();

protected:
	TWeakObjectPtr<UB2Button> BTN_WholeArea;
	
	TWeakObjectPtr<UPanelWidget> CP_LevelUpTitleSet; // This must be statically on.

	TWeakObjectPtr<UTextBlock> TB_CharLevel_Prev;
	TWeakObjectPtr<UTextBlock> TB_CharLevel;
	TWeakObjectPtr<UImage> IMG_CharPortrait;
	TWeakObjectPtr<UTextBlock> TB_LevelUp;

	TWeakObjectPtr<UPanelWidget> CP_BasicStatInfoSet;

	TWeakObjectPtr<UTextBlock> TB_ClassName;
	TWeakObjectPtr<UTextBlock> TB_SkillPtAcquireInfo;
	
	TWeakObjectPtr<UTextBlock> TB_StatTitle_Health;
	TWeakObjectPtr<UTextBlock> TB_StatTitle_Attack;
	TWeakObjectPtr<UTextBlock> TB_StatTitle_Defense;

	TWeakObjectPtr<UTextBlock> TB_StatBefore_Health;
	TWeakObjectPtr<UTextBlock> TB_StatBefore_Attack;
	TWeakObjectPtr<UTextBlock> TB_StatBefore_Defense;

	TWeakObjectPtr<UTextBlock> TB_StatAfter_Health;
	TWeakObjectPtr<UTextBlock> TB_StatAfter_Attack;
	TWeakObjectPtr<UTextBlock> TB_StatAfter_Defense;

	TWeakObjectPtr<UPanelWidget> CP_AllNewSkillSet;
	
	TWeakObjectPtr<UImage> IMG_SkillIcon;
	TWeakObjectPtr<UTextBlock> TB_SkillName;
	TWeakObjectPtr<UTextBlock> TB_SkillAcquireMessage;

	TWeakObjectPtr<UPanelWidget> CP_AllNewBrevetRankSet;

	TWeakObjectPtr<UImage> IMG_RankIcon;
	TWeakObjectPtr<UImage> IMG_CharPortraitForBrevetRank;
	TWeakObjectPtr<UTextBlock> TB_ClassNameForBrevetRank;
	TWeakObjectPtr<UTextBlock> TB_RankUpNotice;

	TWeakObjectPtr<UPanelWidget> P_Bottom;
	TWeakObjectPtr<UTextBlock> TB_SkillType; 
	TWeakObjectPtr<UImage>	IMG_Skill_Badge;
	TWeakObjectPtr<UTextBlock> STB_Confirm;
public:

	FOnLevelUpFXFinish LevelupFinishDelegate;
};
