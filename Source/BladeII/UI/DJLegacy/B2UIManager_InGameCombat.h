// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "B2UIEnum.h"
#include "B2WidgetPool.h"
#include "BladeIICharacter.h"
#include "B2FloatingAbnormalEffect.h"
#include "B2UIManager_InGameCombat.generated.h"

/**
 * The "super" and unique UI manager working in-game. Contains UI managers for all characters in world.
 * In-game UI events are subscribed here.
 * NOTE : It is part of DJLegacy UI, but still being used, even for lobby.
 */
UCLASS()
class BLADEII_API AB2UIManager_InGameCombat : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	class UB2UIManager_InGameHUDPlayer* PlayerHUDManager;

	/** UI managers for all live NPCs in the world. */
	UPROPERTY(Transient)
	TArray<class UB2UIManager_InGameHUDChar*> AllNPCHUDManager;

	/** For Set/HideCenterMessage */
	UPROPERTY(Transient)
	class UB2UIMsgPopupBase* TempCurrentOpenCenterPopup;
	
	/** Widget object pool, for reducing dynamic widget create hitch, for some type of FloatingWidgets. 
	 * Each FFloatingWidgetPool element of index is a whole pool for a FloatingWidget type matching EFloatingWidgetPoolClassType */
	UPROPERTY(Transient)
	TArray<FFloatingWidgetPool> FloatingWidgetPools;
	
	////////////////////////
	// Now need some floating widget classes here, instead of BladeIICharacter or BladeIIPlayer
	/** PC and Mob have different DamageNum widget. */
	UPROPERTY()
	TSubclassOf<class UB2FloatingDamageNum> DamageNumWidgetClass_Mob;
	UPROPERTY()
	TSubclassOf<class UB2FloatingDamageNum> DamageNumWidgetClass_PC;
	UPROPERTY()
	TSubclassOf<class UB2FloatingAbnormalEffect> AbnormalEffectWidgetClass;
	UPROPERTY()
	TSubclassOf<class UB2FloatingGoldAcq> GoldAcqDisplayWidgetClass;

public:
	AB2UIManager_InGameCombat(const FObjectInitializer& ObjectInitializer);

	/** Overridden to manually update some widgets */
	virtual void Tick(float DeltaSeconds) override;

	/** Need to handle some stuff before other world objects are ready. */
	void PreBeginPlay();

	/** Do some clean up UObject stuff here instead of BeginDestroy */
	void OnPreLoadMap();

	virtual void BeginDestroy() override;
	virtual void Destroyed() override;

	class UB2UIManager_InGameHUDChar* CreateHUDForNewChar(class ABladeIICharacter* InNewCreated); // Returns newly created UIManager object.
	/** Need to separate for player and character (NPC) */
	void DestroyHUDForDyingChar(class ABladeIICharacter* InDying);
	void DestroyHUDForDyingPlayer(class ABladeIIPlayer* InDying);
		
	void HideFloatingHPBarForChar(class ABladeIICharacter* InChar, bool binHide);
	void HideFloatingHPBarForPlayer(class ABladeIIPlayer* InChar, bool binHide);
	void HideMobPointer(bool bInHide);
	void HideVirtualJoystick(bool bInHide);

	bool CanResurrectionGameModeType();

	TSubclassOf<class UB2FloatingDamageNum> GetDamageNumWidgetClassByOwnerChar(class ABladeIICharacter* InOwnerChar);
	EFloatingWidgetPoolClassType GetDamageNumWidgetPoolTypeByOwnerChar(class ABladeIICharacter* InOwnerChar);

	FORCEINLINE TSubclassOf<class UB2FloatingDamageNum> GetDamageNumWidgetClass_Mob() { return DamageNumWidgetClass_Mob; }
	FORCEINLINE TSubclassOf<class UB2FloatingDamageNum> GetDamageNumWidgetClass_PC() { return DamageNumWidgetClass_PC; }
	FORCEINLINE TSubclassOf<class UB2FloatingAbnormalEffect> GetAbnormalEffectWidgetClass() { return AbnormalEffectWidgetClass; }
	FORCEINLINE TSubclassOf<class UB2FloatingGoldAcq> GetFloatingGoldAcqWidgetClass() { return GoldAcqDisplayWidgetClass; }

protected:
	void InitializeFloatingWidgetPool();
	void DestroyFloatingWidgetPool();
	FFloatingWidgetPool* GetMatchingFloatingWidgetPool(EFloatingWidgetPoolClassType InFloatingWidgetType);
public:
	/** Interfaces for using floating widget pool, reducing dynamic creation hitch. */
	class UB2FloatingWidget* RentFloatingWidget(EFloatingWidgetPoolClassType InRentWidgetType, class APlayerController* InPC, class ABladeIICharacter* InOwnerChar);
	void TurnInFloatingWidget(EFloatingWidgetPoolClassType InRentWidgetType, class UB2FloatingWidget* InUsedOne);

private:
	/** Get the UIManager owned by InChar. */
	class UB2UIManager_InGameHUDChar* GetRelevantUIManager(class ABladeIICharacter* InChar);

	void SubscribeEvents();
	void UnsubscribeEvents();

	uint32 bEventsSubscribed:1;

	uint32 StageBeginTicket;
	uint32 CharacterBeginPlayTicket;
	uint32 TargetDamagedTicket;
	uint32 CharacterTakeDamageTicket;
	uint32 CharacterAbnormalEffectTicket;
	uint32 PlayerTaggedTicket;
	uint32 PlayerSuckedGoldTicket;
	uint32 SetHUDCenterMessageTicket;
	uint32 HideHUDCenterMessageTicket;
	uint32 PlayerHideMobPointerTicket;
	uint32 CharacterDeathTicket; 
	uint32 SetSpawnNotifyMessageTicket;
	uint32 RemoveSpawnNotifyMessageTicket;

public:
	//////////////////////////////////////////////////////////////////////
	// All in-game UI notifications need to be provided here.
	// Any notifications which could be sent by multiple characters must provide NotifyOwner input argument.

	void NotifyBeginPlay(class ABladeIICharacter* InNotifyOwner);
	void NotifyStageBegin();
	void NotifyTargetDamaged(class ABladeIICharacter* InNotifyOwner, int32 SuccessiveComboNum, float DamageAmount); // SuccessiveComboNum or SuccessiveHit
	void NotifyAutoStateChange(bool bInAutoNow);
	void NotifyPlayerTagged(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer);
	void NotifyPlayerSuckedGold(int32 AcquiredAmount);
	void NotifyTakeDamage(class ABladeIICharacter* InNotifyOwner, float Damage, bool bInCritical);
	void NotifyAbnormalEffect(class ABladeIICharacter* InNotifyOwner, const FLoadedFloatingAbnormalEffectSetupInfo& InEffectSetupInfo);
	void NotifyCharacterDeath(class ABladeIICharacter* InNotifyOwner);
	

	void SetCenterMessage(const FText& InMessageText, float DisplayTime, int32 AnimIndex);	
	void HideCenterMessage();
	void SetSpawnNotifyMessage(const FString& InMessageText, const float InMessageTime);
	void RemoveSpawnNotifyMessage();

	//////////////////////////////////////////////////////////////////////
};
