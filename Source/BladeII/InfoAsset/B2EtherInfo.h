// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "Engine/DataTable.h"
#include "B2UIEnum.h"
#include "B2EtherInfo.generated.h"

/**
 *
 */
USTRUCT()
struct FEtherInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FEtherInfoData()
	{
		FGoodsType = GoodsTypeFName::None;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName FGoodsType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TypeID;												//俊抛福 鸥涝

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString EtherName;											//俊抛福 捞抚

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TypeName;											//鸥涝 捞抚

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> IconMaterial;			//俊抛福 Icon
};

UCLASS()
class BLADEII_API UB2EtherInfo : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	class UDataTable* TheData;

	TArray<FEtherInfoData*> EtherDatas;

public:
	UB2EtherInfo(const FObjectInitializer& ObjectInitializer);

	FEtherInfoData* GetInfoData(FName InGoodsType);

	FString GetEtherName(int32 InTypeID);

	FString GetEtherTypeName(int32 InTypeID);

	class UMaterialInterface* GetEtherIcon(int32 InTypeID);
	int32 GetTypeID(int32 TypeName);

	TArray<int32> GetTypes();

	virtual void PostLoad() override;

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
	void EditorLoadAll();
#endif

private:
	FEtherInfoData* GetInfoData(int32 InTypeID);
};
