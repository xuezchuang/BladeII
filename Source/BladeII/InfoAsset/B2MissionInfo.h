// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "B2UIEnum.h"
#include "B2MissionInfo.generated.h"

/**
 * 
 */


USTRUCT()
struct FMissionInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> MissionIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUIScene Scene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Optional1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Optional2;
};

/** Loaded TAsset reference holder, mainly for editor */
USTRUCT()
struct FMissionInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FMissionInfoLoadedRefHolder()
	{
		LoadedMissionIcon = NULL;
	}

	UPROPERTY(Transient)
	class UMaterialInterface* LoadedMissionIcon;

};


UCLASS()
class BLADEII_API UB2MissionInfo : public UObject
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TArray<FMissionInfoLoadedRefHolder> EditorAllLoadedTAssetHolder;

public:
	UB2MissionInfo(const FObjectInitializer& ObjectInitializer);

	FMissionInfo*	GetMissionInfo(int32 MissionID, int32 OptionalStep = 0);

private:
	UPROPERTY()
	UDataTable* MissionInfoTable;

public:
#if WITH_EDITOR
	void EditorLoadAll();
#endif
};

FText GetMissionTypeText(int32 MissionID);
FString GetMissionTitleKey(int32 MissionID, int32 Step = 0);
FString GetMissionDescKey(int32 MissionID, int32 Step = 0);