// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDonationRewardInfoPopup.generated.h"

USTRUCT(BlueprintType)
struct FDonationRewardInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RewardInfo")
		FText RewardName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RewardInfo")
		TSoftObjectPtr<class UMaterialInterface> RewardIcon;

#if WITH_EDITOR
	void EditorOnPostLoad();
#endif

};


/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationRewardInfoPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
private:
	struct FDonationRewardSlotInfo
	{
		FText ItemName;
		class UMaterialInterface* Icon;
	};

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnClickDonationReward();
	UFUNCTION()
		void OnClickRankingReward();
	UFUNCTION()
		void OnClickFixedReward();

	void UpdateDonationRewardScroll();
	void UpdateRankingRewardScroll();
	void UpdateFixedRewardScroll();
	void UpdateRewardScroll(TArray<FDonationRewardSlotInfo>& InDatas);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class UB2UIDonationRewardSlot> RewardSlotClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FDonationRewardInfo> DonationRewardInfos;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
		int32 MaxDonationRewardColumn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class UB2UIDonationRankingRewardSlot> RankingRewardSlotClass;

private:
	TArray<TWeakObjectPtr<class UB2UIDonationRewardSlot>> DonationRewardSlots;
	TArray<TWeakObjectPtr<class UB2UIDonationRankingRewardSlot>> DonationRankingRewardSlots;
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_DonationSelected;
	TWeakObjectPtr<UTextBlock> TB_DonationUnselected;
	TWeakObjectPtr<UTextBlock> TB_RankingSelected;
	TWeakObjectPtr<UTextBlock> TB_RankingUnselected;
	TWeakObjectPtr<UTextBlock> TB_Name;
	TWeakObjectPtr<UTextBlock> TB_Talk;
	TWeakObjectPtr<UUniformGridPanel> UGP_Reward;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_DonationReward;
	TWeakObjectPtr<UB2Button> BTN_RankingReward;
	TWeakObjectPtr<UB2ScrollBox> SB_Reward;
	TWeakObjectPtr<UB2ScrollBox> SB_RankingReward;
	TWeakObjectPtr<UOverlay> OV_DonationUnselected;
	TWeakObjectPtr<UOverlay> OV_DonationSelected;
	TWeakObjectPtr<UOverlay> OV_RankingUnselected;
	TWeakObjectPtr<UOverlay> OV_RankingSelected;
	TWeakObjectPtr<UB2Button> BTN_FixedReward;
	TWeakObjectPtr<UOverlay> OV_FixedUnselected;
	TWeakObjectPtr<UOverlay> OV_FixedSelected;
	TWeakObjectPtr<UTextBlock> TB_FixedUnselected;
	TWeakObjectPtr<UTextBlock> TB_FixedSelected;
	TWeakObjectPtr<UTextBlock> TB_FixedRewardDesc;
};
