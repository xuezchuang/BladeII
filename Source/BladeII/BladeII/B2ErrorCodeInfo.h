// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "B2UIEnum.h"
#include "B2ErrorCodeInfo.generated.h"

/**
* The actual data composing UB2ErrorCodeInfo.
*/
USTRUCT()
struct FSingleErrorCodeInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleErrorCodeInfoData()
	{
		PopupType = EUIMsgPopup::Simple;
		ViewErrorNumber = true;
		SpecialButtonTextKey = TEXT("");
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUIMsgPopup PopupType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ViewErrorNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SpecialButtonTextKey;
};

UCLASS()
class BLADEII_API UB2ErrorCodeInfo : public UObject
{
	GENERATED_BODY()	

protected:

	UPROPERTY()
	class UDataTable* TheData;

public:
	UB2ErrorCodeInfo(const FObjectInitializer& ObjectInitializer);

	FSingleErrorCodeInfoData* GetInfoData(int32 nErrorCode);

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
#endif
};
