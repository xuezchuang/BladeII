// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "UMG.h"
#include "B2UIManager_InGameHUDChar.generated.h"

/** Declared as a struct to manage the runtime dynamic creation/destruction of certain FloatingWidgets */
USTRUCT(BlueprintType)
struct FDynamicFloatingWidgetSet
{
	GENERATED_USTRUCT_BODY()

	FDynamicFloatingWidgetSet()
	{}

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	class UB2FloatingWidget* FloatingWidget;

	FTimerHandle WidgetDestroyTimerHandle;
};

/**
 * Managing UserWidgets for BladeIICharacter. Bridge between character class and user widgets
 */
UCLASS()
class BLADEII_API UB2UIManager_InGameHUDChar : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Transient)
	class ABladeIICharacter* OwnerCharacter;

	UPROPERTY(Transient)
	class AB2UIManager_InGameCombat* MasterUIManager;

	UPROPERTY(Transient)
	class AGameMode* OwnerGameMode;

	UPROPERTY(Transient)
	class ABladeIIPlayerController* CachedPlayerController;

	uint32 bCachedHUDHidingCinematicMode : 1; // Cached from PlayerController

	uint32 bCurrentlyHideAll : 1; // To hide dynamically spawned widgets.

	/** Generate by BladeIICharacter's FloatingHPBarWidgetClass. Only for mobs. */
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	class UB2FloatingHPBar* FloatingHPBarWidget;

	/** Generate by BladeIICharacter's DamageNumWidgetClass for each damage event. Both for mobs and player. */
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TArray<FDynamicFloatingWidgetSet> DamageNumWidgetSetArray;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TArray<FDynamicFloatingWidgetSet> AbnormalEffectWidgetSetArray;

public:
	UB2UIManager_InGameHUDChar(const FObjectInitializer& ObjectInitializer);
	
	virtual bool InitUIManager(class ABladeIICharacter* InOwner, class AB2UIManager_InGameCombat* InMasterManager);

	/** Update anything which cannot be done by UUserWidget::NativeTick */
	virtual void UpdateManual(float DeltaSeconds);

	virtual void NotifyCharacterDeath(class ABladeIICharacter* InNotifyOwner);	
	/** Destroy manually to not allow widgets remaining after the owner character is gone for any reasons. 
	 * We don't usually have to do this manually as Unreal has GC system, but there were some case that UserWidgets not GCd like other UObjects.
	 * In addition, it could be better to remove widgets right at the moment of owner's destruction.
	 * Destroy condition can be different for BladeIIPlayer. */
	virtual void DestroyUserWidgets();

	void HideFloatingHPBar(bool bInHide);

	/** Hide all widgets contained in this manager, and save their previous visibility state for later restoration. */
	virtual void HideAll();
	/** Restore the visibility of all widgets in this manager to the previous state before hidden by HideAll. */
	virtual void RestoreVisibilityAll();

	bool IsOwnerPlayer();

	FORCEINLINE class ABladeIICharacter* GetOwnerChar() { return OwnerCharacter; }

	void AdditionalUIForGameMode();

	//////////////////////////////////////////////////
	// Event notifications from BladeIICharacter.
	//

	virtual void NotifyBeginPlay();
	virtual void NotifyTakeDamage(float Damage, bool bInCritical);
	void NotifyAbnormalEffect(const FLoadedFloatingAbnormalEffectSetupInfo& InEffectSetupInfo);

	/** Created for each TakeDamage event */
	class UB2FloatingDamageNum* CreateSingleDamageNumWidget(float Damage, bool bInCritical);
	void DestroySingleDamageNumWidgetTimerCB(class UB2FloatingDamageNum* WidgetToDestroy);

	/* Without using the timer, it removes the animation time in UMG. */
	class UB2FloatingAbnormalEffect* CreateAbnormalEffectWidget(const FLoadedFloatingAbnormalEffectSetupInfo& InEffectSetupInfo);
	void DestroySingleAbnormalEffectWidgetTimerCB(class UB2FloatingAbnormalEffect* WidgetToDestroy);

};

