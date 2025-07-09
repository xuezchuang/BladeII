// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "UObject/Object.h"
#include "BladeIIPlayer.h"
#include "B2MessageInfo.h"
#include "CommonStruct.h"
#include "B2ClientDataStore.h"
#include "B2Airport.generated.h"

const int32 MIN_WING_EVOLUTION_GRADE = 1;
const int32 MAX_WING_EVOLUTION_GRADE = 9; // -> This is to be increased.
const int32 MAX_WING_NUMBERIMAGE = 6;
const int32 MAX_WING_EVOLUTION_GRADE_UNOFFICIAL = 9; //-> MAX_WING_EVOLUTION_GRADE is to be this value in the future. It is defined separately only for resource generation.
const int32 MIN_WING_ENHANCE_LEVEL = 0; // Start from 0.
const int32 MAX_WING_ENHANCE_LEVEL = 10;

/** Wing has unique name for each character. */
FText GetLOCTextOfWingName(EPCClass InPCClass, int32 InEvolveGrade);
FText GetLOCTextOfWingPropOption(EItemOption InOption, EPCClass InPCClass);

// Condition to show up evolution menu
FORCEINLINE bool IsItTimeToEvolve(const FB2Wing& InWingData) { return (InWingData.EnhanceLevel >= MAX_WING_ENHANCE_LEVEL && InWingData.EvolutionGrade < MAX_WING_EVOLUTION_GRADE); }
// Condition to show up max evolution menu (almost notification)
FORCEINLINE bool IsUltimatelyEvolvedWing(const FB2Wing& InWingData) { return (InWingData.EnhanceLevel >= MAX_WING_ENHANCE_LEVEL && InWingData.EvolutionGrade >= MAX_WING_EVOLUTION_GRADE); }

#if !UE_BUILD_SHIPPING
// It is a temporary function to fill-up data by limited information to be used until full information is available
void TempSetupWingDataByLimitedData(FPCWingContainer& OutWingContainder, EPCClass InPCClass, int32 InGrade);
#endif

/**
 * B2Airport is a manager class of Wing system like inventory to item
 * Like B2LobbyInventory, non-static members are for LobbyGameMode
 */
UCLASS()
class BLADEII_API UB2Airport : public UObject
{
	GENERATED_BODY()

	/** Better be careful when need to handle it. This is static due to allow access from some static functions here.
	 * It must be strictly valid only in LobbyGameMode. */
	static class AB2LobbyGameMode* TransientLobbyGM;
	
	/** When it is true, It continue to request wing data until all characters check their wing data, on GetWing response. */
	uint32 bIsRequestingWingDataForInit : 1;

	/** True means currently in auto enhancement mode, continuously requesting enhancement signal while it is possible */
	uint32 bIsInAutoEnhancement : 1;
	EPCClass AutoEnhancementPCClass;
	FTimerHandle AutoEnhanceResponseWaitTH;

	/** True means currently in auto evolution mode, continuously requesting evolution signal until succeed (as well as while it is possible) */
	uint32 bIsInAutoEvolution : 1;
	EPCClass AutoEvolutionPCClass;
	FTimerHandle AutoEvolutionResponseWaitTH;

	/** To maintain the least time between each request of auto enhancement/evolution. */
	const float AutoEnhEvolMinInterval = 0.3f;
	const float AutoProEvolMinInterval = 1.2f;
	float CachedLastAutoEnhEvolExecTime;
	FTimerHandle DelayedAutoEnhEvolExecTH;
	
	/** Being cached at wing evolution success, to display compared stats between pre-post evolution */
	FB2Wing TemporaryPreEvolveData;
	/** It is supposed to be true from wing evolution success to wing evolution scene ends. */
	bool bPreEvolveDataValid;

	/** For execution of ConditionalReqForFullWingData */
	bool FullWingDataReqState[PCCLASS_ARRAY_SIZE];

	/** Set by SetWingVisibility to request to server later by FlushPendingWingVisibilityReq */
	static TMap<EPCClass, bool> PendingWingVisibilityReq;

	/** Supposed to be set to true only once for a single user. Locally saved data. */
	static bool bWingUIPageOpenedOnce;

	//////////////////////////////////////////////////
	// List Event subscription tickets..

	uint32 bEventSubscribed : 1;
	
	uint32 DeliveryReceiveGetWingTicket;
	uint32 DeliveryReceiveEnhanceWingTicket;
	uint32 DeliveryReceiveUpgradeWingTicket;
	uint32 DeliveryReceiveEnhanceWingOptionTicket;
	uint32 DeliveryReceiveSetWingVisibleTicket;
	uint32 BeginReqForLobbyInitWingDataTicket;
	uint32 SetLocalWingDataByAccountInfoTicket;
	uint32 UpdateWingCombatStatsOnMDTicket;

public:

	UB2Airport(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;

	static void StaticInit(); // From game module starting.
	static void StaticOnModuleShutdown();

	void InitAirportLobby(class AB2LobbyGameMode* InLobbyGM);
	
	/** To be used at wing main UI page, just at the first time. It will request for full wing data containing extra data for enhancement and evolution stuff. */
	void ConditionalReqForFullWingData(EPCClass InWingPCClass);

private:

	void SetWingDataByAccountInfo(const TArray<b2network::B2CharacterServerInfoPtr>& InAllCharInfoArray, const TArray<b2network::B2WingOptionInfoPtr>& InAllWingOptionInfoArray);
	void UpdateWingCombatStatsOnMD();

	/** Beginning procedure for requesting wing data of all characters, it was implemented when essential wing data was not included at account info. */
	void BeginInitReqForWingData();
	void OnReceiveGetWingCheckForContinueReq(const FB2Wing& ReceivedWingData);

	void SubscribeEvents();
	void UnsubscribeEvents();

	// Server response handling.
	void OnReceiveGetWing(const FB2Wing& GetWingData);
	void OnReceiveEnhanceWing(const FB2EnhanceWing& EnhanceWingData);
	void OnReceiveUpgradeWing(const FB2UpgradeWing& UpgradeWingData);
	void OnReceiveEnhanceWingOption(const FB2EnhanceWingOption& EnhanceWingOptionData);
	void OnReceiveSetWingVisible(const FB2SetWingVisible& WingVisibleData);
	
	void UpdateOnWingDataChange(EPCClass InChangedWingPCClass, bool bUpdateMeshToo);
	
	enum class EWingAutoOpEndReason : uint8{
		WAEER_Graceful, // Ended with totally expected (or desired) result
		WAEER_NormalButCannotAffordGold, // Could not continue due to lack of gold
		WAEER_NormalButCannotAffordIngredient, // Could not continue due to lack of ingredient
		WAEER_Custom, // Probably by user.
		WAEER_Abnormal // Something wrong systematically 
	};

	///// Auto Enhancement
public:
	void BeginAutoEnhancement(EPCClass EnhanceWingPCClass);
	void CustomEndAutoItemEnhancement(); // It is meant to be called from outside to stop auto enhancement at any point
private:
	void EndAutoItemEnhance(EWingAutoOpEndReason InEndReason);
	/** It checks if additional enhancement is possible and either continue request or stop auto enhancement. 
	 * An additional response handling of wing enhancement for auto enhancement. */
	void SingleAutoEnhancementTask();
	/** It is about calling SingleAutoEnhancementTask at least interval. */
	void ReserveOrExecSingleAutoEnhancementTask();
	void ForceEndAutoItemEnhanceTimerCB();

	///// Auto Evolution
public:
	void BeginAutoEvolution(EPCClass EvolveWingPCClass);
	void CustomEndAutoEvolution(); // It is meant to be called from outside to stop auto evolution at any point
private:
	void EndAutoEvolution(EWingAutoOpEndReason InEndReason);
	/** It checks if we need to and able to continue evolution request stop auto evolution.
	 * An additional response handling of wing evolution for auto evolution. */
	void SingleAutoEvolutionTask(bool bInLastEvolutionSucceeded); // If bInLastEvolutionSucceeded is true, auto evolution will just stop.
	/** It is about calling SingleAutoEvolutionTask at least interval. */
	void ReserveOrExecSingleAutoEvolutionTask(bool bInLastEvolutionSucceeded);
	void ForceEndAutoEvolutionTimerCB();
public:

	/** Get local player character's extra attack point by wing equip state, 0 if not has one. */
	static float GetWingAttackPointLocalPlayer(EPCClass InPCClass);
	/** Get local player character's extra defense point by wing equip state, 0 if not has one. */
	static float GetWingDefensePointLocalPlayer(EPCClass InPCClass);
	/** Get local player character's extra health point by wing equip state, 0 if not has one. */
	static float GetWingHealthPointLocalPlayer(EPCClass InPCClass);

	// Functions sets to check if user can afford various wing upgrade operation.
	static bool HasEnoughGoldForWingEnhanceLocalPlayer(EPCClass InPCClass);
	static bool HasEnoughIngredForWingEnhanceLocalPlayer(EPCClass InPCClass);
	static bool HasEnoughGoldForWingEvolveLocalPlayer(EPCClass InPCClass);
	static bool HasEnoughIngredForWingEvolveLocalPlayer(EPCClass InPCClass);
	static bool HasEnoughGoldForWingOptionEnhanceLocalPlayer(EPCClass InPCClass, int32 InOptionIndex);
	static bool HasEnoughIngredForWingOptionEnhanceLocalPlayer(EPCClass InPCClass, int32 InOptionIndex);


	static void LoadLocalData();
	static void SaveLocalData();
	/** Set wing visibility for local player character of InPCClass. 
	 * If bReqToServerImmediately is false, it just set visible flag on local struct and request to server later when FlushPendingWingVisibilityReq is called */
	static void SetWingVisibility(EPCClass InPCClass, bool bInShow, bool bReqToServerImmediately);
	/** Execute wing visibility request to server queued by SetWingVisibility with bReqToServerImmediately being false. */
	static void FlushPendingWingVisibilityReq();

	/** Set visible on "first" wing UI page open */
	static void ConditionalSetWingVisibleOnWingPageOpen();

	/** Check this to see if we need to display new mark on hero selection portrait. */
	static bool HasAnyNewStuff_Char(EPCClass InCheckChar);
	/** Check this to see if we need to display new mark on wing prop option icon at wing main UI. */
	static bool HasAnyNewStuff_WingPropOption(EPCClass InCheckChar, int32 InPropOptionIndex);

	FORCEINLINE bool IsInAutoEvolution() const { return bIsInAutoEvolution; }
	FORCEINLINE bool IsInAutoEnhancement() const { return bIsInAutoEnhancement; }
	FORCEINLINE bool IsInAutoEvolOrEnhTask() const { return (IsInAutoEnhancement() || IsInAutoEvolution()); }

	/** OutData is valid if it returns true. */
	bool GetPreEvolveData(FB2Wing& OutData) const;
	/** To be called on wing evolution scene end. */
	FORCEINLINE void MarkPreEvolveDataInvalid() { bPreEvolveDataValid = false; }

#if !UE_BUILD_SHIPPING
	// For all PCClass..
	void DevOnlyGenerateDummyWingData(int32 InEvolveGrade, bool bRequestToServer);
#endif
};
