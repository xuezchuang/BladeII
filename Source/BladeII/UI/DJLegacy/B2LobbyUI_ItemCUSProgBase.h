// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_ItemCusProgBase.generated.h"

USTRUCT(BlueprintType)
struct FStarEffectInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	float OverallPlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	float FxCreatingDepth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	float ResultGradeStarListingOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	FVector ResultGradeStarFxScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	FVector2D ResultGradeStarCenterPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UParticleSystem* ResultGradeStarPS;
};
/**
 * Common functionalities for use of item compose. upgrade and surpass scene.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemCusProgBase : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
protected:
	
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	/** Must be one among Upgrade/Composition/Surpass. */
	ELobbyInvenItemOpMode TargetOpMode;

	FB2Item NativeItemData_Target; // Cached one before updated.
	FB2Item NativeItemData_Result; // Supposed to have same RefID and UID to Target except compose

	//================================================================================
	// UMG item icon stuff

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_ItemOpScene> ItemIconWidgetClass;

	/** Canvas panels where item icons are placed. */
	TWeakObjectPtr<UCanvasPanel> X_CP_ItemIconSet_Target;
	TWeakObjectPtr<UCanvasPanel> X_CP_ItemIconSet_Result;

	/** Created item icons inside CP_ItemIconSet** 
	 * Even when they are not being used, they need to be created if their relevant Fx are to be created, 
	 * as Fx material uses UI icon's material parameter to set up the same item outlook. */
	//UPROPERTY(Transient)
	class UB2DynItemIcon_ItemOpScene* CreatedItemIcon_Target;
	UPROPERTY(Transient)
	class UB2DynItemIcon_ItemOpScene* CreatedItemIcon_Result;

	//================================================================================

	//================================================================================
	// Fx stuff

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TArray<FStarEffectInfo> StarsInfo; // This is array, element number is same to star grade.

	UPROPERTY(Transient)
	TArray<class UParticleSystemComponent*> CreatedResultGradeStarFx; // This is array, element number is same to star grade.

	//================================================================================
	
	/** If checked, the main camera FOV will be adjusted by screen aspect ratio.
	 * It can be desired or not, depend on the directed scene and FX setup.
	 * Directed scene can be clamped if Adaptive FOV is not used, but with this feature, unwanted background can be seen due to wider view. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	uint32 bUseCameraAdaptiveFOV : 1;
	/**
	 * It needs to be adjusted carefully at various screen resolution. 
	 * The directing scene based on it possibly has 3D-2D object alignment.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float AdaptiveFOVScale;

	/** Final result user widget page, as a part here. */
	TWeakObjectPtr<class UB2LobbyUI_ItemDetailPostOp> UIP_FinalResultPageP;


public:

	UB2LobbyUI_ItemCusProgBase(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	
	void DestroySelf() override;
	
protected:

	float GetOverallPlayRateForStar(const FB2Item& InItem);
	float GetCreatingDepthForStar(const FB2Item& InItem);
	float GetResultListingOffsetForStar(const FB2Item& InItem);

	FVector GetStarFxScaleForStar(const FB2Item& InItem);
	FVector2D GetResultStarCenterPosForStar(const FB2Item& InItem);

	UParticleSystem* GetResultGradeStarPS(const FB2Item& InItem);

	FStarEffectInfo* GetStarEffectInfo(const FB2Item& InItem);
	FStarEffectInfo* GetStarEffectInfo(const int32 InGrade);

	void UpdateStaticWidgets();

	// Event Calls from Blueprint

	/** Most final step as a part of UniteOrUpgrade scene. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartResultGradeStarFx();

	/** Make result item icon visible. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartShowResultItemIcon();
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void ShowFinalResultPage();

	virtual void InitFinalResultPage(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM);

	void ApplyCameraAdaptiveFOV();

	FVector GetDesiredScreenCenterWorldPos(); // Fx created at screen center will use this value for its locating.
	
	// Set data for each item, and create UMG icon representing it. Icons need to be set invisible from the first moment or at some point.
	void SetItemData_Target(FB2Item& InItem);
	void SetItemData_Result(FB2Item& InItem);

	// Common functionality for target, and result.
	class UB2DynItemIcon_ItemOpScene* CreateItemIconCommon(UCanvasPanel* ParentCP, const FB2Item& InItemData, ELobbyInvenItemIconUsage IconUsage);

	void DestroyItemIcon_Target();
	void DestroyItemIcon_Result();

	virtual void DestroyAllFx(); // Must be override to include others for extended child classes.
	void DestroyResultGradeStarFx();
	
	void FinishAndProceedToResult();

	// Must be overridden to return true for actually supported mode.
	virtual bool IsSupportedOpMode(ELobbyInvenItemOpMode InMode) const { return false; }

#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
