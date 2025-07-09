// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "CommonStruct.h"
#include "B2UIStageClearRewardFx.generated.h"


/** Struct to contain loaded TAsset references of a single NPCClassInfo entry to prevent it get GCd. */
USTRUCT()
struct FStageClearRewardPSSetupLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FStageClearRewardPSSetupLoadedRefHolder()
	{
	}

	UPROPERTY(Transient)
	TArray<class UParticleSystem*> LoadedPSPtr;

	void CacheMeIfYouCan(class UParticleSystem* InLoadedPS);
};


/**
 * For user setting-up properties of particle system asset per item star grade.
 */
USTRUCT(BlueprintType)
struct FStageClearRewardPSSetup
{
	GENERATED_USTRUCT_BODY()

	FStageClearRewardPSSetup()
	{
		FxCreateNormalizedScreenPos = FVector2D(0.5f, 0.5f);
		FxCreateRot = FRotator(0.0f, 180.0f, 0.0f);
		FxCreateDepth = 2.5f;
		MinOverlayHiddenTime = 1.0f;
	}

protected:
	/** 
	 * Each element's index is supposed to match Item's StarGrade - 1.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TArray<TSoftObjectPtr<class UParticleSystem>> PSPerItemGrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TArray<TSoftObjectPtr<class UParticleSystem>> PSPerEtherTier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TArray<TSoftObjectPtr<class UParticleSystem>> PSPerEtherTierExtend;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TSoftObjectPtr<class UParticleSystem> PSForGold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TSoftObjectPtr<class UParticleSystem> PSForAtlasMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TSoftObjectPtr<class UParticleSystem> PSForNonAtlasMaterial;

public:

	/** 
	 * Normalized screen position (0.0 ~ 1.0 scale) where the particle system component initially created.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	FVector2D FxCreateNormalizedScreenPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	FRotator FxCreateRot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	float FxCreateDepth;
	
	/**
	 * Item overlay info can be visible after at least this time has been passed. It can be almost the playing time of particle system.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	float MinOverlayHiddenTime;

	void PreloadForItems(TArray<FB2Item>& InItems, FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder);
	class UParticleSystem* GetPSForItem(const FB2Item& InItem, FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder) const;
	void UnloadTAssets(FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder);
#if WITH_EDITOR
	void EditorLoadAll();
#endif
};

/** 
 * Set of dynamically created reward item FX and its movement state.
 */
USTRUCT()
struct FStageClearRewardFxSet
{
	GENERATED_USTRUCT_BODY()

	FStageClearRewardFxSet()
	{
		FXComp = NULL;
		DestMarkerWidget = NULL;
		DestPos = FVector(0.0f, 0.0f, 0.0f);
		CurrentSpeed = 0.0f;
		TotalDistance = 0.0f;
		FlightDistSoFar = 0.0f;
		FlightTimeSoFar = 0.0f;
		MinOverlayHiddenTime = 0.0f;
		bArrivedAtDest = false;
	}

	UPROPERTY(Transient)
	UParticleSystemComponent* FXComp;

	UPROPERTY()
	class UB2UISlotItem* DestMarkerWidget;

	FVector DestPos; // Destination position placed by marker. (Screen > World converted position of DestMarkerWidget)
	float CurrentSpeed; // Accelerated speed
	float TotalDistance; // TotalDistance to target. Calculated and cached at the first moment. FxComp should not over this distance, must be back if so.
	float FlightDistSoFar; // FXComp will stop once it reaches TotalDistance
	float FlightTimeSoFar; // To count and compare time to see if it arrived before MinOverlayHiddenTime
	float MinOverlayHiddenTime; // Just cached variable from FStageClearRewardPSSetup

	FTimerHandle ItemOverlayInfoShowTH; // It will be used only when FXComp arrived at dest before MinOverlayHiddenTime

	bool bArrivedAtDest;
};

/*
 * A part of UIStageClear. For reward item Fx settings including PS template, scale, and destination placement, etc.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStageClearRewardFx : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	
	
	/** 
	 * Whole number of reward items that to be placed and created by ParticleSystem template.
	 * The same number of placement marker should be present. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	int32 ItemNumber;

	/** 
	 * Common prefix for Fx destination marker widget.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	FString FxDestMarkerPrefix;

	/**
	 * The size scale of created reward item Fx component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	float FxCreateScale;

	/** 
	 * Those three below decide how fast the created reward item Fx will move.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	float FxMoveBaseAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	float FxMoveMinSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	float FxMoveMaxSpeed;

	/** Somewhat like spring const */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	float FxMoveAccAdjustConst;

	float FxMoveFinalAcc; // Finally calculated considering FxMoveBaseAcceleration and FxMoveAccAdjustConst, like spring but not exactly.

	int FxStarSize;		// Fx Star Size;

	/** 
	 * Array of marker widgets found by FxDestMarkerPrefix and numbering.
	 * Each create Fx comp element will fly to one of its element position.
	 * They also display some of item information which cannot be done by 3D particle effect, as a slot item widget part.
	 */
	UPROPERTY()
	TArray<class UB2UISlotItem*> AllFxDestMarkers;

	UPROPERTY(Transient)
	TArray<FStageClearRewardFxSet> CreatedRewardFxSet;

private:
	// Names defined for NamedMaterialSlots of particle system template.
	const static FName Name_FxMID_ItemBGPanel;
	const static FName Name_FxMID_EtherPanel;
	const static FName Name_FxMID_ItemIcon;
	const static FName Name_FxMID_StaIcon;
	const static FName Name_FxMID_Param_StarTile;
	const static FName Name_FxMID_Param_StarSize;
	const static FName Name_FxMID_StarNumer;


public:
	
	UB2UIStageClearRewardFx(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

private:
	virtual void CacheAssets() override;

public:
	void BeginScene(const TArray<FB2Item>& InRewardItems, const FStageClearRewardPSSetup& InPSSetup, FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder);

	void CleanupScene(); // Destroy dynamically created objects here.
	
	FORCEINLINE int32 GetDisplayingItemNumber() { return ItemNumber; }

private:
	UParticleSystemComponent* CreateSingleRewardFx(const FB2Item& InRewardItem, class UB2UISlotItem* InMarkerWidget, const FStageClearRewardPSSetup& InPSSetup, FStreamableManager& InTAssetLoadManager, FStageClearRewardPSSetupLoadedRefHolder& InLoadedRefHolder);

	void UpdateFxMove(float DeltaSecond);

	void OnSingleRewardArriveAtDest(int32 RewardIndex);

	void ShowRewardOverlayInfo(int32 RewardIndex); // Show overlay info represented by UMG widget. (in fact, marker)

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeiI")
	void OnBeginScene_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeiI")
	void OnSingleRewardArriveAtDest_BP(int32 RewardIndex);
};
