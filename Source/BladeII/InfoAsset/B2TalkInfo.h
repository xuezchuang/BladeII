// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "B2TalkInfo.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPortraitPosition : uint8
{
	Position_1,
	Position_2,
	Position_3,
	Position_4
};

UENUM(BlueprintType)
enum class EPortraitAppearType : uint8
{
	Default,
	OnTime,
	OnlySound
};

struct FTalkLine
{
	FTalkLine(struct FTalkInfoData*);

	int32			GroupID;
	EPortraitAppearType AppearType;
	bool			SpeakerLeave;
	bool			ReverseImage;
	bool			ModifyDepth;
	float			ShowingTime;
	float			DelayToNextLine;
	EPortraitPosition PortraitPosition;
	FName			SpeakerCodeName;
	FName			TalkLineCodeName;
	FName			TalkSoundCodeName;
	int32			BackgroundCode;
};

struct FTalkInfoInstance
{
	FTalkInfoInstance(struct FTalkInfoData*);

	int32				GroupID;
	TArray<FTalkLine>	TalkSequence;
};

USTRUCT()
struct FTalkInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 GroupID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool SpeakerLeave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ReverseImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ModifyDepth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ShowingTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DelayToNextLine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPortraitAppearType AppearType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SpeakerCodeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName TalkLineCodeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName TalkSoundCodeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPortraitPosition PortraitPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BackgroundCode = -1;
};

UCLASS()
class BLADEII_API UB2TalkInfo : public UObject
{
	GENERATED_BODY()
	
public:
	void SetDataTable(UDataTable* InDataTable);

	const FTalkInfoInstance* GetTalkInfo(const int32 GroupID);
	
private:
	UPROPERTY()
	UDataTable *TalkInfoTable;

	TMap<int32, FTalkInfoInstance>	TalkInfoMap;
};