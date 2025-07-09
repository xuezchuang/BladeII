// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "B2BrevetInfo.generated.h"

/**
* The actual data composing UB2BrevetInfo.
*/
USTRUCT()
struct FSingleBrevetInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleBrevetInfoData() : RankNameForTextTable(TEXT(""))
	{
		BrevetRank = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BrevetRank;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString RankNameForTextTable;

	/** Icon material for the use of UMG. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> IconMaterialSmall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> IconMaterialBig;
};

UCLASS()
class BLADEII_API UB2BrevetInfo : public UObject
{
	GENERATED_BODY()	

protected:

	/** Data table asset, containing the actual data, providing CSV import/export feature */
	UPROPERTY()
	class UDataTable* TheData;

public:
	UB2BrevetInfo(const FObjectInitializer& ObjectInitializer);

	/** Get single info data of a BrevetRank, NULL if not found.
	* BrevetRank is supposed to be InfoArray index + 1 (start from 1, not 0) */
	FSingleBrevetInfoData* GetInfoData(int32 BrevetRank);

	FString GetBrevetRankNameKey(int32 BrevetRank);
	UMaterialInterface* GetBrevetRankIconMaterial(int32 BrevetRank, bool isBigSize = false);

	int32 GetDataCount() const;
	int32 GetMaxBrevetRank() const;
#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
#endif
};
