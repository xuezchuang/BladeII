// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2UIRepeatBattleInGameInfo.h"
#include "B2UIStageClearRewardFx.h"
#include "B2UIBackWidget.h"
#include "../../BladeII.h"
#include "UObject/ObjectSaveContext.h"
#include "B2UIStageClear.generated.h"

const int32 MAX_REWARD_FX_PART_NUM = 5; // Max reward number itself might be different.

/**
 * For stage clear event, to display player's glorious fucking moment. It is succeeded by StageResult UI
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStageClear : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	FStageClearRewardPSSetupLoadedRefHolder LoadedRewardPSHolder;

	/** Particle system set-up for reward Fx, per item star-grade */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FStageClearRewardPSSetup RewardPSSetup;

	/** Main stage of reward item Fx play. Only one will be loaded and instanced by total reward item number. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<TSoftClassPtr<class UB2UIStageClearRewardFx>> RewardFxPartWidgetClasses;

	/** Created widget part instance, by one among RewardFxPartWidgetClasses */
	UPROPERTY(Transient)
	class UB2UIStageClearRewardFx* CreatedRewardFxPart;

	TArray<FB2Item> RewardItemData; // Gathered for displaying here.

public:
	UB2UIStageClear(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif

	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void CloseWidgetDelegate() override;
	UFUNCTION(BlueprintCallable, Category = "BladeII StageClear")
	void BeginRewardItemFx();

	/* 3d effects at this point are to be played. */
	UFUNCTION(BlueprintCallable, Category = "BladeII StageClear")
		void Show3DWingEffect();
	UFUNCTION(BlueprintCallable, Category = "BladeII StageClear")
		void Show3DGemEffect();

	/** It is setting up for RepeatBattle, just signal from Blueprint for proper timing. */
	UFUNCTION(BlueprintCallable, Category = "BladeII StageClear")
		void StartCountDownIfRepeatBattleIsOn();

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void GatherRewardItemData();
	bool IsMaterialFromAtlas(int32 ItemRefID);

	/** Override because this StageClearWidget is expected to be used at paused state. */
	virtual void SetupWidgetCustomTimer() override;
	/** Override because this StageClearWidget is expected to be used at paused state. */
	virtual void SetupWidgetCustomHideTimer() override;

	float CachedWidgetStartTime;
	void UpdateWidgetCustomTimer();

	void StartRepeatBattleCountdown();
	void StopRepeatBattleCountdown();
	void SetupOnRepeatBattleStateChange(bool bIsOn);
	bool IsRepeatBattleOn();

	UB2UIStageClearRewardFx* GetRewardPartForRewardNum(int32 InRewardNum);

	//===================================================Click
	UFUNCTION()
		void OnClickBtnGoStageResult();

	//====================================================Doc Delegate

	UFUNCTION()
		void OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn);
	
protected:
	TWeakObjectPtr<UButton>					BTN_GoStageResult;

	TWeakObjectPtr<UB2UIRepeatBattleCountDown> UIP_RepeatBattleCountDownP;
	TWeakObjectPtr<UB2UIRepeatBattleInGameInfo> UIP_RepeatBattleInGameInfoP;

};
