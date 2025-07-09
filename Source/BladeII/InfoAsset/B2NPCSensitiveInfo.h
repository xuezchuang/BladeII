// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "B2NPCClassInfo.h"
#include "B2NPCSensitiveInfo.generated.h"

/**
 * The actual data composing UB2NPCSensitiveInfo, most likely combat related, which can get benefit from excel editing.
 * Although individual NPC's combat setting is here, the most essential combat related variables are at server.
 * The row key that access data entry is what you get from UB2NPCClassInfo::GetSensitiveInfoDataRowKey, visible by SensitiveDataRowKey of UB2NPCSingleClassInfo
 */
USTRUCT()
struct FSingleNPCSensitiveData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleNPCSensitiveData()
	{
		MatchingNPCClass = ENPCClass::ENC_End;
		MatchingNPCClassVari = ENPCClassVariation::ENCV_Normal;

		HPScale = 1.0f;
		AttackDefenseLevelScale = 1.0f;
		MaxArmorOverride = 0.0f;
	}

	/** For data integrity check. The actual row search will be done by SensitiveDataRowKey of UB2NPCSingleClassInfo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ENPCClass MatchingNPCClass;
	/** For data integrity check. The actual row search will be done by SensitiveDataRowKey of UB2NPCSingleClassInfo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ENPCClassVariation MatchingNPCClassVari;

	/** Multiplication to the basic heath value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HPScale;

	/** Multiplication to the basic Attack/DefenseLevel value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDefenseLevelScale;

	/** Armor value is just overridden. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxArmorOverride;
};

/**
 * To contain some "sensitive" data like combat variables in DataTable format.
 */
UCLASS()
class BLADEII_API UB2NPCSensitiveInfo : public UObject
{
	GENERATED_BODY()
	
protected:

	/** Data table asset, containing the actual data, providing CSV import/export feature */
	UPROPERTY()
	class UDataTable* TheData;

public:
	UB2NPCSensitiveInfo(const FObjectInitializer& ObjectInitializer);

	/** Sets the real shit. */
	void SetDataTable(UDataTable* InDataTable);
	
	/** Method to access matching data to specific ClassInfo */
	FSingleNPCSensitiveData* GetInfoData(ENPCClass InClassEnum, UB2NPCSingleClassInfo* InClassInfo);
	
	/** To suggest NPCSensitiveInfo row key when it is not set. */
	static FName SuggestSensitiveInfoDataRowKey(ENPCClass InClassEnum, ENPCClassVariation InVariationEnum);

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();

	void OnAssetEditorOpen(UObject* InAsset);

	bool FirstTimeConstruction_EditorAddInfoData(ENPCClass InClassEnum, ENPCClassVariation InVariationEnum, FSingleNPCSensitiveData* InNewData, FName& OutSuggestedName);
#endif
};
