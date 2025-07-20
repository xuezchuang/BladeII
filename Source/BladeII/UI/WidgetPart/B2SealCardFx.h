// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "CommonStruct.h"
#include "B2SealCardFx.generated.h"

/**
 *
 */

USTRUCT()
struct FCardFXSetupLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FCardFXSetupLoadedRefHolder()
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
struct FCardFXSetup
{
	GENERATED_USTRUCT_BODY()

	FCardFXSetup()
	{
		FxCreateNormalizedScreenPos = FVector2D(0.5f, 0.5f);
		FxCreateRot = FRotator(0.0f, 180.0f, 90.0f);
		FxCreateDepth = 2.5f;
		MinOverlayHiddenTime = 1.0f;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TArray<TSoftObjectPtr<class UParticleSystem>> ChainedFX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TArray<TSoftObjectPtr<class UParticleSystem>> UnchainFX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeiI")
	TArray<TSoftObjectPtr<class UParticleSystem>> FlipCardFX;


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

	void PreloadForItems(TArray<FB2Item>& InItems, FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder);
	class UParticleSystem* GetPSForItem(const FB2Item& InItem, FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder) const;
	void UnloadTAssets(FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder);
#if WITH_EDITOR
	void EditorLoadAll();
#endif
};

USTRUCT()
struct FCardFXSet
{
	GENERATED_USTRUCT_BODY()

	FCardFXSet()
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
	float MinOverlayHiddenTime; // Just cached variable from FCardFXSetup

	FTimerHandle ItemOverlayInfoShowTH; // It will be used only when FXComp arrived at dest before MinOverlayHiddenTime

	bool bArrivedAtDest;
};

UCLASS()
class BLADEII_API UB2SealCardFx : public UB2UIWidgetBase
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
	TArray<FCardFXSet> CreatedRewardFxSet;

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

	UB2SealCardFx(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

private:
	virtual void CacheAssets() override;

public:
	void BeginScene(const TArray<FB2Item>& InRewardItems, const FCardFXSetup& InPSSetup, FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder);
	void CleanupScene(); // Destroy dynamically created objects here.

	FORCEINLINE int32 GetDisplayingItemNumber() { return ItemNumber; }

	void ManualTick();

private:
	UParticleSystemComponent* CreateSingleRewardFx(const FB2Item& InRewardItem, class UB2UISlotItem* InMarkerWidget, const FCardFXSetup& InPSSetup, FStreamableManager& InTAssetLoadManager, FCardFXSetupLoadedRefHolder& InLoadedRefHolder);
	void UpdateFxMove(float DeltaSecond);
	void OnSingleRewardArriveAtDest(int32 RewardIndex);
	void ShowRewardOverlayInfo(int32 RewardIndex); // Show overlay info represented by UMG widget. (in fact, marker)

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeiI")
	void OnBeginScene_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeiI")
	void OnSingleRewardArriveAtDest_BP(int32 RewardIndex);
};