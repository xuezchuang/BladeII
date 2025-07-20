// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "../Common/CommonStruct.h"
#include "B2TotemRefineInfo.generated.h"


USTRUCT(BlueprintType)
struct FTotemRefineInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FTotemRefineInfoData()
		: RefineSuccessPrefixLevel(0), RefineLevel(-1)
	{ }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RefineSuccessPrefixLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RefineLevel;
};
/**
*
*/
UCLASS()
class BLADEII_API UB2TotemRefineInfo : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class UDataTable* DataTable;
	TMap<int32, int32> TotemRefineDataMap;

public:
	UB2TotemRefineInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;

	int32 GetTotemRefineValue(ETotemRefinePrefix PrefixLevel);
};
