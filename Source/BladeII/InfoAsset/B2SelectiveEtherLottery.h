// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "Engine/DataTable.h"
#include "B2SelectiveEtherLottery.generated.h"


USTRUCT()
struct FSelectiveEtherLotteryData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 RefID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Grade;												//俊抛福 Star烙

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Tier;											//俊抛福 萍绢
};

UCLASS()
class BLADEII_API UB2SelectiveEtherLottery : public UObject
{
	GENERATED_BODY()

public:
	UB2SelectiveEtherLottery(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
#endif

	int32 GetSelectiveEtherLotteryGrade(int32 InRefID);
	int32 GetSelectiveEtherLotteryTier(int32 InRefID);

private:
	FSelectiveEtherLotteryData* GetInfoData(int32 InRefID);

protected:
	UPROPERTY()
		class UDataTable* TheData;

	TArray<FSelectiveEtherLotteryData*> SelectiveEtherLotteryDatas;

};
