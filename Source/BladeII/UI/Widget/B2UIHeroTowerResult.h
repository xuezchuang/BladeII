// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIHeroTowerResult.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIHeroTowerResult : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void NativeConstruct() override;
	virtual void UpdateStaticText() override;

	void OnShowResult();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_Anim_Open();

	UFUNCTION(BlueprintCallable, Category = "UI Herotower")
	void PlayAnimationEvent_Anim_End();
private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
		void OnClickBtnOK();

	UFUNCTION()
		void OnClickBtnNextFloor();

	void UpdateAutoMoveStageTimeSec();

	/* Events that occur when finished completely open. */
	virtual void OnOpenComplete() override;

	void CheckAndOpenLevelUpPart();
	void OnLevelUpFXFinish();
private:
	TWeakObjectPtr<UButton>				BTN_OK;
	TWeakObjectPtr<UButton>				BTN_NextFloor;
	TWeakObjectPtr<UButton>				BTN_FailConfirm;
	
	TWeakObjectPtr<UCanvasPanel>				CP_Clear;
	TWeakObjectPtr<UCanvasPanel>				CP_Fail;

	TWeakObjectPtr<UTextBlock> TB_NFloor;
	TWeakObjectPtr<UTextBlock> TB_RecordSec;
	TWeakObjectPtr<UHorizontalBox> HB_Rewards;
	TWeakObjectPtr<UB2RichTextBlock> TB_AutoSceneMoveTime;

	TWeakObjectPtr<UTextBlock> STB_PastTime;
	TWeakObjectPtr<UTextBlock> STB_RewardTitle;
	TWeakObjectPtr<UTextBlock> STB_NextFloor;
	TWeakObjectPtr<UTextBlock> STB_Quit;
	TWeakObjectPtr<UTextBlock> STB_Confirm;
	
	TWeakObjectPtr<UOverlay>				O_NextFloor;
	
	TWeakObjectPtr<class UB2UIStageResultLevelUp> UIP_LevelUp;

	int32 DeliveryGetHeroTowerTicket;

	const int32 AutoMoveStageTimeSec = 5;
	int32 m_nRemainAutoMoveStageTimeSec;

	FTimerHandle TimeToAutoMoveStage;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2UIRewardIcon> RewardItemClass;
	
};
