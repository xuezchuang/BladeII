// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIRewardSlot.generated.h"


USTRUCT(BlueprintType)
struct FRewardDisplayInfo
{
	GENERATED_USTRUCT_BODY()

	FRewardDisplayInfo() : RewardIconMaterial(nullptr), RewardLocTextKey(TEXT("")) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	MailGiftType RewardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* RewardIconMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RewardLocTextKey;
};


UCLASS()
class BLADEII_API UB2UIRewardSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIRewardSlot(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	// CommonStruct MailGiftType 劝侩
	void InitRewardSlotTemplate(MailGiftType RewardType);
	void SetRewardSlotDefaultDisplay(const FRewardDisplayInfo& InRewardDisplayInfo);
	void SetRewardSlotItem(int32 ItemID);
	void SetRewardAmount(int32 Amount);
	
protected:
	virtual void CacheAssets() override;

private:

	TWeakObjectPtr<UImage> IMG_RewardIcon;
	TWeakObjectPtr<UTextBlock> TB_RewardName;

	TWeakObjectPtr<UTextBlock> TB_RewardAmount;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRewardDisplayInfo> RewardDisplayInfo;
};
