// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "MissionManager.h"
#include "../B2RichTextBlock.h"
#include "../B2UIEnum.h"
#include "B2UIPMissionSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPMissionSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void OnClickGetRewardButton();

	UB2UIPMissionSlot(const FObjectInitializer& ObjectInitializer);
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void InitMissionSlot(int32 ID);
	void UpdateMissionSlot(MissionPtr Mission);
	void UpdateMissionSlotDefault(MissionPtr Mission);
	void UpdateMissionSlotDynamic(MissionPtr Mission);
	void UpdateMissionStateDisplay(EMissionState InMissionState);
	void UpdateMissionRewardIcon(MissionPtr Mission);
	void UpdateBtn_Link(MissionPtr Mission);
	void UpdateStaticText_TitleDesc(MissionPtr Mission);
	void ClearMissionSlot();
	struct FMissionInfo* GetLocalMissionInfo(int32 InMissionID);
	void SetRewardButtonDisable(bool bDisable);
	void SetRewardButtonVisibility(bool InVisibility);

	FORCEINLINE int32 GetMissionID() const { return MissionID; }
	FORCEINLINE EMissionType GetMissionType() const { return MissionType; }
	FORCEINLINE EMissionState GetMissionState() const { return MissionState; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor CompleteTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UMaterialInterface*>	SlotBaseMaterials; // InProgress, Complete, Rewarded

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "RewardPointMaterial_DEPRECATED"))
		UMaterialInterface* RewardPointMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RewardPointItemIndex;

private:
	UFUNCTION()
		void OnClickLinkButton();

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	TWeakObjectPtr<UImage>				IMG_MissionIcon;
	TWeakObjectPtr<UImage>				IMG_BaseDefault;
	TWeakObjectPtr<UOverlay>			O_BaseCompleted;
	TWeakObjectPtr<UImage>				IMG_Complete;
	TWeakObjectPtr<UImage>				IMG_Dimmed;
	TWeakObjectPtr<UPanelWidget>		X_OverlayCompleted;
	TWeakObjectPtr<UTextBlock>			TB_MissionTitle;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_MissionDesc;
	TWeakObjectPtr<UButton>				BTN_Link;
	TWeakObjectPtr<UButton>				BTN_GetReward;
	TWeakObjectPtr<UTextBlock>			TB_Link;
	TWeakObjectPtr<UTextBlock>			TB_GetReward;
	TWeakObjectPtr<UProgressBar>		PB_MissionProgress;
	TWeakObjectPtr<UTextBlock>			TB_ProgressCount;
	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon1;
	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon2;
	int32			MissionID;
	EMissionType	MissionType;
	EMissionState	MissionState;
	EUIScene		LinkScene;
	int32			LinkOptional1;
	int32			LinkOptional2;
};
