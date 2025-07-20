// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "B2UIEnum.h"
#include "Engine/DataTable.h"  
#include "B2SeasonEventInfo.generated.h"

USTRUCT()
struct FSeasonEventInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUIScene UIScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Optional1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Optional2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text_English;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text_Korean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text_Other;
};

UCLASS()
class BLADEII_API UB2SeasonEventInfo : public UObject
{
	GENERATED_BODY()

public:
	UB2SeasonEventInfo(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY()
	UDataTable*	CachedSeasonEventInfoTable;

	FString		strNativeCulture;
	
public:
	UDataTable* LoadSeasonEventInfoTable();

	void GotoLinkUIScene(int32 iDay, int32 iIndex);

	FText GetSeasonEventSlotText(int32 iDay, int32 iIndex);

	FSeasonEventInfo* FindSeasonEventInfo(int32 iDay, int32 iIndex);

public:
#if WITH_EDITOR
	void EditorLoadAll();
#endif
};
