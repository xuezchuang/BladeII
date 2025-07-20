// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDiceEventExplainPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDiceEventExplainPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

public:
	void OpenPopup();
	void ClosePopup();

	void InitDiceEventExplainPopup(int32 iEventID);
	
private:
	void InitDiceEventMasterData_Mission();
	void InitDiceEventMasterData_FinishReward();

public:
	void UpdateFinishRewardState(int32 iFinishCount);
	
private:
	UFUNCTION()
	void OnClickBTN_Close();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PopupZOrder_BP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32	MAX_HorizontalFinishReward_BP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotHB> UISlotHBClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIDiceEventFinishReward>	UIDiceEventFinishRewardClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIDiceEventMission>		UIDiceEventMissionClass;

private:
	TWeakObjectPtr<UB2Button>	BTN_Close;

	TWeakObjectPtr<UScrollBox>	SB_FinishRewardList;

	TWeakObjectPtr<UScrollBox>	SB_MissionList;

	TWeakObjectPtr<UTextBlock>	TB_FinishRewardTitle;
	TWeakObjectPtr<UTextBlock>	TB_MissionListTitle;
	
	TArray<TWeakObjectPtr<class UB2UIDiceEventFinishReward>>	ArrayFinishReward;

private:
	int32 CachedEventID;
};
