// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "B2UIManager_InGameHUDChar.h"
#include "B2UIManager_InGameHUDPlayer.generated.h"

/**
 * Managing UserWidgets for BladeIIPlayer
 */
UCLASS()
class BLADEII_API UB2UIManager_InGameHUDPlayer : public UB2UIManager_InGameHUDChar
{
	GENERATED_BODY()
	
protected:
	/** Just casted OwnerCharacter */
	UPROPERTY(Transient)
	class ABladeIIPlayer* OwnerPlayer;

	/** For on-screen gold acquisition effect. Created for each gold acquisition event. */
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TArray<FDynamicFloatingWidgetSet> GoldAcqWidgetSetArray;
	
	/** It was once a UserWidget too. Still here considering its role. */
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	class UParticleSystemComponent* HiddenMobPointerPSInst;

	uint32 HiddenMobPointerOnlyVisiblityFlag : 1;
	uint32 HiddenVirtualJoystick : 1;

private:

	/** The radius around the player where the hidden mob pointer is displayed, in worldspace. */
	float HMP_RadiusFromPlayer;

	/** It won't be needed to be cached here if the HMPTarget is to be updated in every frame. */
	UPROPERTY(Transient)
	class ABladeIICharacter* CurrentHMPTarget;

	FTimerHandle CenterMessageHideTimerHandle;

public:
	UB2UIManager_InGameHUDPlayer(const FObjectInitializer& ObjectInitializer);

	virtual bool InitUIManager(class ABladeIICharacter* InOwner, class AB2UIManager_InGameCombat* InMasterManager) override;
	
	/** Update anything which cannot be done by UUserWidget::NativeTick */
	virtual void UpdateManual(float DeltaSeconds) override;
	
	virtual void DestroyUserWidgets() override {};
	/** For manual destruction in a little bit different condition */
	void DestroyPlayerWidgets();	

	/** Hide all widgets contained in this manager, and save their previous visibility state for later restoration. */
	virtual void HideAll() override;
	/** Restore the visibility of all widgets in this manager to the previous state before hidden by HideAll. */
	virtual void RestoreVisibilityAll() override;

	void HideMobPointer(bool bInHide); // Even when bInHide is false, it might not show the mob pointer when all UIs are supposed to be invisible.

	//////////////////////////////////////////////////
	// Event notifications from BladeIIPlayer.
	//

	virtual void NotifyBeginPlay() override;
	void NotifyStageBegin();
	void NotifyTargetDamaged(int32 SuccessiveComboNum, float DamageAmount); // SuccessiveComboNum or SuccessiveHit
	void NotifyAutoStateChange(bool bInAutoNow);
	void NotifyPlayerTagged(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer);
	void NotifyPlayerSuckedGold(int32 AcquiredAmount);

	/** Created for each PlayerSuckedGold event */
	class UB2FloatingGoldAcq* CreateSingleGoldAcqWidget(int32 AcquiredAmount);
	void DestroySingleGoldAcqWidgetTimerCB(class UB2FloatingGoldAcq* WidgetToDestroy);
	
	/** Set simple message string displayed at the screen center, 
	 * You can specify display time or if it is same or less than 0, need to manually turn it off later.
	 * AnimIndex : Index of UMG animations prepared for the center message at player HUD widget, negative for no animation. */
	void SetCenterMessage(FText& InMessageText, float DisplayTime, int32 AnimIndex = -1);
	/** Turn off center message manually. Need this if you call SetCenterMessage without specifying positive DisplayTime */
	void HideCenterMessage();
	void HideCenterMessageTimerCB();

	void HideVirtualJoystick(bool bInHide);

private:

	void UpdateHiddenMobPointer();
	void UpdateConquestAreaPointerForControl();
	void UpdateConquestAreaPointerForPvP();
	void VisiblePointer();
};
