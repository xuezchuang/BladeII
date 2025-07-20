// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "B2EtherSetOptionInfo.generated.h"

/**
 *
 */

USTRUCT()
struct FEtherSetOptionInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SetOptionID;												//俊抛福 技飘 可记 ID

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString EtherSetOptionName;										//俊抛福 技飘 可记 捞抚

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString EtherSetOptionDesc;										//俊抛福 技飘 可记 汲疙
};


UCLASS()
class BLADEII_API UB2EtherSetOptionInfo : public UObject
{
	GENERATED_BODY()
protected:

	UPROPERTY()
	class UDataTable* TheData;

	TArray<FEtherSetOptionInfoData*> EtherSetOptionDatas;

public:
	UB2EtherSetOptionInfo(const FObjectInitializer& ObjectInitializer);

	FText GetEtherSetOptionName(int32 InSetOptionID);

	FText GetEtherSetOptionDesc(int32 InSetOptionID);

	virtual void PostLoad() override;

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
#endif

private:
	FEtherSetOptionInfoData* GetInfoData(int32 InSetOptionID);
};
