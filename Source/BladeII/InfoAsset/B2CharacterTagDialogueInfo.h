// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "B2AssetLoader.h"
#include "Sound/SoundCue.h"
#include "B2CharacterTagDialogueInfo.generated.h"

USTRUCT(BlueprintType)
struct FCharacterTagSuggestTextInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText>	Text_English;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText>	Text_Korean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText>	Text_Other;
};

/** This struct is for transfer data from FCharacterTagSoundInfo struct to where it is loaded. */
struct FCharacterTagSoundInfoTransientSoundAssetInfo
{
	FCharacterTagSoundInfoTransientSoundAssetInfo(int32 InCacheMapKey, const TSoftObjectPtr<USoundCue>* InAssetPtr)
		: MyCacheMapKey(InCacheMapKey)
		, TheAssetPtr(InAssetPtr)
	{
	}

	/** It can be got by GetCharTagSoundAssetCacheKey */
	const int32 MyCacheMapKey;
	const TSoftObjectPtr<USoundCue>* TheAssetPtr;
};
/** It can uniquely identify all FCharacterTagSoundInfo ::TagSound TAsset array elements */
FORCEINLINE int32 GetCharTagSoundAssetCacheKey(int32 TableRowNum, EPCClass InInterPCClass, int32 DataArrayIndex)
{
	return TableRowNum * 10000 + PCClassToInt(InInterPCClass) * 1000 + DataArrayIndex;
}

USTRUCT(BlueprintType)
struct FCharacterTagSoundInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPCClass InterActionPCClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<USoundCue>>	TagSound;
public:
	const FCharacterTagSoundInfoTransientSoundAssetInfo GetSoundAssetPtrOfIndex(int32 OuterRowNum, int32 InIndex) const;
	const FCharacterTagSoundInfoTransientSoundAssetInfo GetRandSoundAssetPtr(int32 OuterRowNum) const;
	FORCEINLINE int32 GetNumAllSoundAsset() const { return TagSound.Num(); }

	void GatherAllSoundAssetPtr(TArray<TSoftObjectPtr<USoundCue>>& OutSoundAssets);

#if WITH_EDITOR
	void EditorLoadAll();
#endif

};

USTRUCT()
struct FCharacterTagDialogueInfo : public FTableRowBase
{
	GENERATED_BODY()

	FCharacterTagDialogueInfo()
	{
		MyTableRowNum = INDEX_NONE;
	}

	/** Set by owner UObject, when the DataTable of this format is loaded. */
	int32 MyTableRowNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCharacterTagSuggestTextInfo TagSuggestText_FromSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCharacterTagSuggestTextInfo TagSuggestText_FromDyingHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCharacterTagSoundInfo> TagSound_InterAction;

	const FCharacterTagSoundInfo* GetTagSoundInfoPtr(EPCClass InterClass) const;
	const FCharacterTagSoundInfoTransientSoundAssetInfo GetRandSoundAssetPtr(EPCClass InterClass) const;

	void GatherAllSoundAssetPtr(TArray<TSoftObjectPtr<USoundCue>>& OutSoundAssets);

#if WITH_EDITOR
	void EditorLoadAll();
#endif

};

UCLASS()
class BLADEII_API UB2CharacterTagDialogueInfo : public UObject, public IB2LoadInterface
{
	GENERATED_BODY()

	/**
	 * If true, not all loaded asset data will be simply unloaded at level transition, instead some or all are get to added to RootSet.
	 * You should always use ShouldUnloadAllOnLevelTransition instead of direct access of this variable.
	 */
	static bool bManagedUnload;

public:
	UB2CharacterTagDialogueInfo(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY()
	UDataTable*	CachedCharacterTagDialogueInfoTable;

	FString		strNativeCulture;

	UPROPERTY(Transient)
	TMap<int32, USoundCue*> LoadedPtrMap;

public:
	const UDataTable* LoadCharacterTagDialogueInfoTable();

	FText GetRandomTagSuggestTextFromSkill(const int32 iPlayerClassType);
	FText GetRandomTagSuggestTextFromDyingHp(const int32 iPlayerClassType);

	USoundCue* GetRandomTagSound(EPCClass SoundPlayPCClass, EPCClass IntarActionPCClass);

	const FCharacterTagDialogueInfo* FindCharacterTagDialogueInfo(const int32 iPlayerClassType);

	static bool ShouldUnloadAllOnLevelTransition();

	void UnloadAll();
	void ConditionalUnloadAllOrPreserve();
private:
	void MarkAllLoadedAssetsRootSet();

public:
#if WITH_EDITOR
	void EditorLoadAll();
#endif
};
