// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "B2ItemSortInfo.generated.h"

USTRUCT(BlueprintType)
struct FSingleItemSortInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleItemSortInfoData()
		: ItemRefID(0), ItemSortValue(-1)
	{ }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemRefID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemSortValue;
};
/**
*
*/
UCLASS()
class BLADEII_API UB2ItemSortInfo : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class UDataTable* DataTable;
	TMap<int32, int32> ItemSortDataMap;

public:
	UB2ItemSortInfo(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostLoad() override;

	int32 GetItemSortValue(int32 ItemRefID);
};
