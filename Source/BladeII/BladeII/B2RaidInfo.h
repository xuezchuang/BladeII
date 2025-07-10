// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "B2RaidInfo.generated.h"

/**
* The actual data composing UB2RaidInfoTable.
*/
USTRUCT()
struct FRaidInfoTableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FRaidInfoTableData()
	{
		ClientStageId = 0;
		RaidName = FText();
		RaidDesc = FText();
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ClientStageId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText RaidName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText RaidDesc;
};

UCLASS()
class BLADEII_API UB2RaidInfoTable : public UObject
{
	GENERATED_BODY()	

protected:

	UPROPERTY()
	class UDataTable* TheData;

public:
	UB2RaidInfoTable(const FObjectInitializer& ObjectInitializer);

	FRaidInfoTableData* GetInfoData(int32 Idx);

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
#endif

public:
	FText GetRaidNameByRaidMapId(int32 InRaidMapId, int32 InRaidStep);
	FText GetRaidDescByRaidMapId(int32 InRaidMapId, int32 InRaidStep);
	int32 GetClientStageIdByRaidMapId(int32 InRaidMapId, int32 InRaidStep);

	FORCEINLINE int32 GetRaidKey(int32 InRaidMapId, int32 InRaidStep) { return (InRaidMapId*10000+InRaidStep); }
};
