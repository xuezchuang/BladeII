// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2LobbyUI_ItemLevelupMain.h"
#include "B2LobbyUI_ItemLevelupProg.generated.h"

/** Set of dynamically created ingredients icon */
USTRUCT()
struct FItemLevelupProgIngredItemIconSet
{
	GENERATED_USTRUCT_BODY()

	FItemLevelupProgIngredItemIconSet()
	{
		DynIcon = NULL;
		RelativeCoord = FVector2D(0.0f, 0.0f);
	}

	UPROPERTY(Transient)
	class UB2DynItemIcon_ItemOpScene* DynIcon;

	/** Relative coordinate of created icon inside the panel. (Non-scale for current viewsize) 
	 * It is icon's center coordinates, not the final coordinate based on upper-left corner. */
	FVector2D RelativeCoord; 
};

/** Set of dynamically created ingredient particle FX.
 * For more sophisticated movement effect, might need projectile stuff. */
USTRUCT()
struct FItemLevelupProgIngredSuckingFxSet
{
	GENERATED_USTRUCT_BODY()

	FItemLevelupProgIngredSuckingFxSet()
	{
		FXComp = NULL;

		CurrentSpeed = 0.0f;
		TotalDistance = 0.0f;
		FlightTimeSoFar = 0.0f;
		FlightDistSoFar = 0.0f;
	}

	UPROPERTY(Transient)
	UParticleSystemComponent* FXComp;

	float CurrentSpeed; // Accelerated speed

	float TotalDistance; // TotalDistance to target.

	float FlightTimeSoFar; // For handling positive ItemSuckingTrailLifeTime
	float FlightDistSoFar; // If ItemSuckingTrailLifeTime is not specified, FXComp will be destroyed once it reaches TotalDistance
};

/**
 * Enhance target Item must be already enhanced at this point. Just directed showing stuff. 
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemLevelupProg : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	/** The item data to be the target of Enhancement. */
	FB2Item NativeTargetLevelupItem_Before; // Before Levelup Item. This item itself does not exist at this point. Just copied data.
	FB2Item NativeTargetLevelupItem_After; // After Levelup Item
	
protected:

	//================================================================================
	// For UMG item icon creation stuff

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_ItemOpScene> ItemIconWidgetClass;

	TWeakObjectPtr<UCanvasPanel> X_CP_TargetItemIconSet; // Where the target item icon is placed.
	
	/** Enhance target item icon created inside CP_ItemIconSet */
	UPROPERTY(Transient)
	class UB2DynItemIcon_ItemOpScene* CreatedTargetItemIcon;
	/** Center pos of created target item icon in screen space. */
	FVector2D TargetItemIconScreenPos;

	TWeakObjectPtr<UCanvasPanel> X_CP_EnhanceIngredIconPanel; // Where the ingredient item icons are placed and aligned

	/** Horizontal margin between ingredient icons. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	float IngredientIconPlacementMargin;

	/** Multiple rows are placed at same H and/or V coordinate with this offset  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	FVector2D IngredientIconNextRowOffset;

	/** The icon size is determined based on panel size, but cannot over this size for each dimension. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	FVector2D IngredientIconMaxSize;

	/** Information set of created ingredient icons. It is to be dynamically managed for some effect. */
	UPROPERTY(Transient)
	TArray<FItemLevelupProgIngredItemIconSet> CreatedIngredIconsSet;
	
	//================================================================================


	//================================================================================
	// For 3D effect stuff
	

	/** Global time dilation to be applied during this page. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	float OverallPlayRate;

	/** Used for computing final 3D coordinate of creating 3D ItemSuckingTrail FX, with each icon's screen space coordinate
	 * Better not use too big value for proper screen space alignment. You can still adjust scale. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	float ItemSuckingTrailDepth;

	/** Defines the time after widget creation that the first ingredient start to getting sucked to the target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	float ItemSuckingStartTime;

	/** How fast ingredient items are start getting sucked after former one just got sucked? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	float ItemSuckingInterval;

	FTimerHandle ItemSuckingIntervalTimer;

	/** Acceleration of ItemSuckingBaseAcc. 0 is constance acceleration */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	float ItemSuckingAccAcc;

	/** Base acceleration of ingredient item sucking flying at starting moment. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	float ItemSuckingBaseAcc;

	float EffectiveItemSuckingAcc; // Just the current value considering ItemSuckingBaseAcc and ItemSuckingAccIncPerIngred

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	float ItemSuckingMaxSpeed;
	
	/** For showing poor ingredients are getting sucked to the target icon. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	class UParticleSystem* ItemSuckingTrailPS;

	/** Created component's scale. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	FVector ItemSuckingTrailScale;

	/** If this is larger than 0, individual sucking trail Fx comp will last for this duration, otherwise it destroyed on arriving at target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX_ItemSucking")
	float ItemSuckingTrailLifeTime;
	
	UPROPERTY(Transient)
	TArray<FItemLevelupProgIngredSuckingFxSet> CreatedSuckingFxSet;
	
	uint32 bAllIngredSuckingFxCreated : 1; // Need this to check if all sucking are done.

	/** Setup for overall background. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FSingleUMGParticleSetup OverallBGFxSetup;
	UPROPERTY(Transient)
	UParticleSystemComponent* CreatedOverallBGFx;

	/** Setup for blowing effect filling up screen. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FSingleUMGParticleSetup EnhanceBlowupFxSetup;
	UPROPERTY(Transient)
	UParticleSystemComponent* CreatedEnhanceBlowupFx;

	/** Setup for background effect behind target item icon. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FSingleUMGParticleSetup TargetItemBGFxSetup;
	UPROPERTY(Transient)
	UParticleSystemComponent* CreatedTargetItemBGFx;

	/** Setup for final effect showing glorious enhancement success. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FSingleUMGParticleSetup FinalEnhanceSuccessFxSetup;
	UPROPERTY(Transient)
	UParticleSystemComponent* CreatedFinalEnhanceSuccessFx;

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
	UPROPERTY(Transient, BlueprintReadWrite)
	class UB2LobbyUI_ItemDetailPostOp* FinalResultPageNRef;

	TWeakObjectPtr<UWidgetAnimation> FinalResultAnimation;

public:

	UB2LobbyUI_ItemLevelupProg(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void DestroySelf() override;

private:

	// Names defined for NamedMaterialSlots of particle system template.
	const static FName Name_FxMID_ItemBGPanel;
	const static FName Name_FxMID_ItemIcon;
	// They are just GetFxNamedMID, for some defined names..
	class UMaterialInstanceDynamic* GetFxMID_ItemBGPanel(class UParticleSystemComponent* InPSComp);
	class UMaterialInstanceDynamic* GetFxMID_ItemIcon(class UParticleSystemComponent* InPSComp);
	
	void ApplyCameraAdaptiveFOV();

	void SetTargetItemIconData(FB2Item& InItem); // To update target item icon.
public:
	void SetIngredItemData(const TArray<FB2Item>& AllIngreds); // To update ingredient item icons. Might need to set them from outside for test purpose.
protected:
	
	int32 GetAllIngredNum();
	
	void UpdateStaticWidgets();

	void DestroyTargetItemIcon();
	void DestroyItemLevelupIngredientItemIcon();
	void DestroySuckingFXComps();
	void DestroyOverallBGFx();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void DestroyEnhanceBlowupFx();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void DestroyTargetItemBGFx();
	void DestroyFinalEnhanceSuccessFxComp();

	/** Main entry point of single ingredient item's sucking to target item action. */
	void BeginIngredItemSucking(int32 IngredIndex);

	void UpdateSingleSuckingFxComp(int32 IngredIndex, float InDeltaSec);
	void CheckForAllFxSucked();

	void OnFinalIngredJustSucked();

	void OnSingleIngredBeginSucking(int32 IngredIndex); // At the moment of start flying to get sucked
	void OnSingleIngredEndSucking(int32 IngredIndex); // At the moment just sucked.
protected:
	
	/** Event for Blueprint at the moment of first ingredient item start go get sucked. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnFirstIngredBeginSucking_BP();

	/** While each ingred's sucked event goes to item icon class, the final sucking event is handled here. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnFinalIngredJustSucked_BP();

	/** Blueprint need to send this signal when all necessary effects are done and it is time to show updated target item. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void UpdateTargetItemToNewData();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowOverallBGFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowEnhanceBlowupFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowTargetItemBGFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowFinalEnhanceSuccessFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetTargetItemIconVisible(bool bVisible);
	/** It does not affect visibility of icon itself, if it is shown and calling it with true, it will show only overlay info that PS cannot show */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetTargetItemIconOverlayOnly(bool bOverlayOnly);
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowFinalResultPage();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowBattleScorePopup();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowCloseButton();


	// Timing for Enhance sound fx. They are separated to support varying length of sound fx according to varying enhance ingredient's number.
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartSfx_Phase1_Beginning();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartSfx_Phase2_FirstIngredient();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartSfx_Phase3_FinalSucked(bool bIsCostume);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartSfx_Phase2N_EachIngredient();

private:
	/** It will return the allowed size by the main panel size, not the designed size of ItemIconWidgetClass.
	 * Return value is for slate widget, not scaled by screen size. */
	FVector2D GetAllowedSingleIngredientIconSize();
	FVector2D GetIngredIconPanelCenterPos();

	/** Get screen space coordinate of ingredient icons, of their center coordinate. */
	FVector2D GetIngredIconScreenPos(int32 IconIndex);
	FVector GetIngredIconDesiredWorldPos(int32 IconIndex); // World pos desired for FX.
	FVector GetTargetIconDesiredWorldPos(float Depth);
	
	void FinishAndProceedToResult();

	bool IsSupportedOpMode(ELobbyInvenItemOpMode InMode) const { return (InMode == ELobbyInvenItemOpMode::EIVIOP_Levelup || InMode == ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume); }
};
