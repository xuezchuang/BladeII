// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "Engine/DataTable.h"
#include "../Common/CommonStruct.h"
#include "B2ChapterInfo.generated.h"

/**
* The actual data composing UB2ChapterInfo, representing information per single chapter.
*/
USTRUCT()
struct FSingleChapterInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleChapterInfoData()
	{
	}
	
	/** Key string of localized text table which contains the name of this chapter. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LocalizedChapterNameKey;

};

/**
 * A chapter includes multiple stages. It requires its own (per-chapter) information.
 */
UCLASS()
class BLADEII_API UB2ChapterInfo : public UObject
{
	GENERATED_BODY()
	
protected:

	/** Data table asset, containing the actual data, providing CSV import/export feature */
	UPROPERTY()
	class UDataTable* TheData;
	
public:
	UB2ChapterInfo(const FObjectInitializer& ObjectInitializer);

	/** Get single info data of a ChapterNum, NULL if not found.
	 * ChapterNum is supposed to be InfoArray index + 1 (start from 1, not 0) */
	FSingleChapterInfoData* GetInfoData(int32 ChapterNum);
	
	/** Get single chapter info data associated with designated StageId. */
	FSingleChapterInfoData* GetChapterInfoOfStage(int32 InClientStageId);

	/** Returns how many chapter info exist here. */
	int32 GetAllChapterNum();

	/** Returns how many stages (sub-chapter) exist in designated number of chapter. */
	int32 GetStagesNumInChapter(int32 ChapterNum);

	/** Get the final chapter name text from designated chapter's LocalizedChapterNameKey */
	FText GetLocalizedChapterName(int32 ChapterNum);

	FText GetLocalizedDifficultyName(EStageDifficulty difficulty);

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
#endif
};
