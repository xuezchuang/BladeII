// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "B2RewardInfo.generated.h"

/**
* The actual data composing UB2RewardInfo.
*/
USTRUCT()
struct FSingleRewardInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleRewardInfoData() : RewardNameForTextTable(TEXT(""))
	{
		RewardId = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RewardId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TextCategory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString RewardNameForTextTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemInfoIndex;

	/** Icon material for the use of UMG. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> IconMaterial;
};

/** Loaded TAsset reference holder, mainly for editor */
USTRUCT()
struct FRewardInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FRewardInfoLoadedRefHolder()
	{
		LoadedIconMaterial = NULL;
	}

	UPROPERTY(Transient)
	class UMaterialInterface* LoadedIconMaterial;

};

UCLASS()
class BLADEII_API UB2RewardInfo : public UObject
{
	GENERATED_BODY()	

protected:

	/** Data table asset, containing the actual data, providing CSV import/export feature */
	UPROPERTY()
	class UDataTable* TheData;

	UPROPERTY(Transient)
	TArray<FRewardInfoLoadedRefHolder> EditorAllLoadedTAssetHolder;
	TArray<FSingleRewardInfoData*>	RewardInfoData;

public:
	UB2RewardInfo(const FObjectInitializer& ObjectInitializer);

	FSingleRewardInfoData* GetInfoData(int32 RewardIndex);

	FString GetRewardNameKey(int32 RewardIndex);

	FText GetNameText(int32 RewardIndex);

	int32 GetItemInfoIndex(int32 RewardIndex);
	int32 GetItemRewardId(int32 ItemtemplateId);

	UMaterialInterface* GetRewardIconMaterial(int32 RewardIndex);

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
	void EditorLoadAll();
#endif
};
