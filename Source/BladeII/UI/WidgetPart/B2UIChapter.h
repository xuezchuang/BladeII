// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIChapter.generated.h"

/** Defines which chapter and stage a uniquely define ChapterStage UIP is for. */
USTRUCT(BlueprintType)
struct FChapterStageUniqueSettingKey
{
	GENERATED_USTRUCT_BODY()
	FChapterStageUniqueSettingKey()
	{
		ChapterNum = 0;
		SubStageNum = 0;
	}

	UPROPERTY(EditAnywhere)
	int32 ChapterNum;
	/** Stagenum in the Chapter, not the whole stagenum. */
	UPROPERTY(EditAnywhere)
	int32 SubStageNum;

	FORCEINLINE bool operator==(const FChapterStageUniqueSettingKey& InOther) const
	{
		return (this->ChapterNum == InOther.ChapterNum && this->SubStageNum == InOther.SubStageNum);
	}

	FORCEINLINE uint32 GetMyTypeHash() const { return ChapterNum * 100 + SubStageNum; }
};
/** For use of TMap with FChapterStageBossSettingKey key */
FORCEINLINE uint32 GetTypeHash(const FChapterStageUniqueSettingKey& InSettingKey)
{
	return InSettingKey.GetMyTypeHash();
}
/** The actual setting data of uniquely defined ChapterSTage UIP.. */
USTRUCT(BlueprintType)
struct FChapterStageUniqueSettingValue
{
	GENERATED_USTRUCT_BODY()
	FChapterStageUniqueSettingValue()
	{
		DisplayPanelSize.X = DisplayPanelSize.Y = 0.0f;
	}

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UIChapterStage> WidgetClass;

	/** Zero or less will use AutoSize */
	UPROPERTY(EditAnywhere)
	FVector2D DisplayPanelSize;
};

/*
 * Defines chapter stage setup.. inside ChapterFrame.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChapter : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIChapter(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	FORCEINLINE void SetChapterNumOnly(int32 InValue) { ChapterNum = InValue; }
	void SetChapterNum(int32 InValue, EStageDifficulty InStageDifficulty); // It update ChapterStage parts too.
	void SetStageDatas(/*TArray<FStage绢录绊> StageDatas*/);

	void UpdateStageUIPosition(TArray<FVector2D>& Positions2D);

	FORCEINLINE int32 GetChapterNum() const { return ChapterNum; }
	FORCEINLINE EStageDifficulty GetStageDifficulty() const { return StageDifficulty; }

	class UB2UIChapterStage* GetStageWidget(const int32 StageNumber);

	void GenerateUniquelyDefinedPartsOfCurrentChapter(TArray<class UB2UIChapterStage*>& OutCreatedParts);

	void ResetStateForReuse();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayOpeningAnim();

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	//=================================================== Btn

	//==================================================== Doc Delegate

public:
	//Which holds the number of stages.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Chapter" )
	int32 StageNum;
	//The prefix name of the stage assets.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Chapter")
	FString PrefixStageName;

	/** Optional settings for a chapter UI shared among multiple chapters. 
	 * There must be an empty panel slot having the same naming convention at the place that the part by this setting is to be created.
	 * See UniquelyDefinedPartsCreatePanels. */
	UPROPERTY(EditAnywhere, Category = "BladeII Chapter")
	TMap<FChapterStageUniqueSettingKey, FChapterStageUniqueSettingValue> UniquelyDefinedStageParts;

	/** If true, each stage part location will be dynamically updated by 3d world marker */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Chapter")
	uint32 bExternalDynamicSetupReady : 1;

protected:
	//TWeakObjectPtr<UImage>					IMG_Sample;

	TWeakObjectPtr<UPanelWidget>			CP_TempNonMatineeBG; // It can be used as a panel cotaining temporary background image before matinee is ready.
	
	UPROPERTY(Transient)
	TArray<class UB2UIChapterStage*>		ChapterStages;
	/** Subset of ChapterStages.. created by UniquelyDefinedStageParts */
	UPROPERTY(Transient)
	TArray<class UB2UIChapterStage*>		DynCreatedStages;

	/** 
	 * Most elements in ChapterStages are statically placed widgets, but there can be dynamically created widgets (by UniquelyDefinedStageParts)
	 * These will be parent panels for them. Key is sub-stage number.
	 */
	UPROPERTY(Transient)
	TMap<int32, class UCanvasPanel*>		UniquelyDefinedPartsCreatePanels;

private:
	int32 ChapterNum;
	EStageDifficulty StageDifficulty;
};
