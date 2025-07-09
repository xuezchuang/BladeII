// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "CommonStruct.h"
#include "BladeII.h"
#include "B2LoadingImageInfo.generated.h"

/**
 * Asset container for loading images. This is special in that they are used during level transition.
 * Loaded objects must be root set and manually unloaded.
 */

USTRUCT(BlueprintType)
struct BLADEII_API FB2LoadingSceneImageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<TSoftObjectPtr<class UTexture2D>> LoadingImages;
};

USTRUCT(BlueprintType)
struct BLADEII_API FB2LoadingChapterImageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TMap<int, FB2LoadingSceneImageInfo> ChapterLoadingImages;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2LoadingImageInfo : public UObject
{
	GENERATED_BODY()

	/** Just an holder. Expected only one element (except the editor).. at most..? */
	UPROPERTY(Transient)
	TArray<class UTexture2D*> LoadedLoadingImages;

public:
	UB2LoadingImageInfo(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UTexture2D>> LoadingImages_Default;

	UPROPERTY(EditAnywhere)
	TMap<EB2GameMode, FB2LoadingChapterImageInfo> AllLoadingImages;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UTexture2D> LobbyLoginLoadingImage_Fallback;
	UPROPERTY(EditAnywhere)
	TMap<EPCClass, TSoftObjectPtr<class UTexture2D>> LobbyLoginLoadingImage_PerPCClass;

protected:
	TArray<TSoftObjectPtr<class UTexture2D>>& GetTAssetArrayForGameMode(EB2GameMode InGM);

	/** Suppose to use the TAsset array get by GetTAssetArrayForGameMode.. */
	class UTexture2D* GetLoadingImageCommon(TArray<TSoftObjectPtr<class UTexture2D>>& InTAssetArray, int32 InIndex);

	class UTexture2D* LoadLoadingImage(TSoftObjectPtr<class UTexture2D>& Texture);

public:

	int32 GetChpaterIndex(EB2GameMode InGameMode) const;
	/** It will randomly select loading image from relevant list of current game-mode. */
	class UTexture2D* RandGetLoadingImage(EB2GameMode InGameMode, int32& OutRandomlySetIndex);

	/** It is provided to use the index returned from RandGetLoadingImage */
	class UTexture2D* GetLoadingImageOfIndex(EB2GameMode InGameMode, int32 InImageIndex);

	int32 GetLoadingImageNum(EB2GameMode InGameMode);

	FString GetLobbyLoginLoadingImageName() const;

	/** To be called at level start, when loading image is used. */
	void UnloadAll();
#if WITH_EDITOR
	/** For safe use of TSoftClassPtr in editor. */
	void EditorLoadAll();
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP // Clean-up loaded TAsset references. They are temporary code until formally fix wrong handling of lazy-loaded info asset.
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
	
};
