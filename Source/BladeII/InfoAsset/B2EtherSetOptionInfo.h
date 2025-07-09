// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "B2EtherSetOptionInfo.generated.h"

/**
 *
 */

USTRUCT()
struct FEtherSetOptionInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SetOptionID;												//���׸� ��Ʈ �ɼ� ID

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString EtherSetOptionName;										//���׸� ��Ʈ �ɼ� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString EtherSetOptionDesc;										//���׸� ��Ʈ �ɼ� ����
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
