#pragma once

#include "UObject/Object.h"
#include "Engine/DataTable.h"

#include "Quest.h"
#include "Sound/SoundCue.h"
#include "QuestDataTable.generated.h"


USTRUCT()
struct FQuestCommonInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartNPC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CompleteNPC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartDialog;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CompleteDialog;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 OutLine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUIScene LinkScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Optional1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Optional2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StartNpcSoundKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CompleteNpcSoundKey;
};

UCLASS()
class BLADEII_API  UB2QuestInfoTable : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UDataTable*	MainQuestTable;

	UPROPERTY()
	UDataTable* DailyQuestTable;

public:
	UB2QuestInfoTable(const FObjectInitializer& ObjectInitializer);
	
	FQuestCommonInfo* GetQuestMainInfo(int32 QuestID);
	FQuestCommonInfo* GetQuestDailyInfo(int32 QuestID);

	FQuestCommonInfo* GetQuestInfo(QuestPtr TargetQuest);
	
	void SetCommonTable(UDataTable* DataTable);

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity(UDataTable* DataTable);
#endif

};



FText GetQuestInfoText(int32 QuestID, int32 QuestType);
FText GetProgressText(int32 QuestID, int32 QuestType, int32 CurrentCount);
FText GetProgressText(QuestPtr TargetQuest);
FText GetProgressCompletedText(int32 QuestID, int32 QuestType);
FText GetCompressedText(const FText& OrgText, int32 LimitTextCount);
int32 ParseTextFromSpecialChar(const FText& OriginalText, const FString& ParseString, TArray<FText>& OutParsedResult);



USTRUCT(BlueprintType)
struct FQuestNPCSoundInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NPCCode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USoundCue> AssetPtr;
public:
	USoundCue* GetSoundCue();
};

USTRUCT(BlueprintType)
struct FQuestNPCSoundRow
{
	GENERATED_USTRUCT_BODY()

	TArray<FName> SoundCodes;
};

UCLASS()
class BLADEII_API UB2QuestNPCSoundInfo : public UObject
{
	GENERATED_BODY()
public:
	UB2QuestNPCSoundInfo(const FObjectInitializer& ObjectInitializer);

	void SetDataTable(UDataTable* InDataTable);

	USoundCue* GetNPCSound(const FName InRawSoundCode, const int32 NpcCode);

#if WITH_EDITOR
	void CheckInfoDataIntegrity();
#endif

private:
	TMap<int32, FQuestNPCSoundRow>	QuestNPCSoundInfoMap;

	UPROPERTY()
	class UDataTable*				NPCSoundInfoTable;
};

