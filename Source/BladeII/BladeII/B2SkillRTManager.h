// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "BladeIICharacter.h"
#include "../DataStore/B2ClientDataStore.h"
#include "../BladeII.h"
#include "B2PCClassInfo.h"
#include "B2SkillInfo.h"
#include "B2Buff_Base.h"
#include "B2SkillRTManager.generated.h"

/**
 * Struct to contain runtime information for handling skill bound to specific input.
 * Not for serialization, but some information can be replicated from server side
 */
class FInputBoundSkillState_Base
{
public:
	FInputBoundSkillState_Base()
	{
		SkillID = SKILL_INVALID_ID;
		bIsSkillDisabledByCooltime = false; // False doesn't mean that this skill is possible by itself.
		RemainingCooltime = 0.0f;
		MaxCooltime = 0.0f;
		bCountingCooltime = false;		
		MappingAttackState = EAttackState::ECS_None;
		UsePCClass = EPCClass::EPC_End;
		RelevantDoc = nullptr;
		IncreaseMotion = 0;
	}

	virtual ~FInputBoundSkillState_Base()
	{
	}
		
	int32 SkillID;
	int32 IncreaseMotion;
	float EnableWeaponSkillPoint;

private:
	
	EAttackState MappingAttackState;
	EPCClass UsePCClass;
	class UB2UIDocBase* RelevantDoc;

protected:
	float MaxCooltime;
	/** Show if this skill is disabled by cooltime. Does not represent whether player acquired this skill and assigned for use. */
	bool bIsSkillDisabledByCooltime;
	
	/** Counted from cooltime to 0.0 during cooltime count.
	* This is not for enable the skill again, currently just for UI visualization. */
	float RemainingCooltime;
	
	/** True only when it is counting. A little bit different from bIsSkillDisabledByCooltime */
	bool bCountingCooltime;		

	bool bCurrentSuggest;
	bool bIsSkillDisabledByOtherSuggest;

public:	
	virtual void InitSkillState(class UB2SkillRTManager* SkillRTManager, int32 InSkillID, EPCClass InPCClass, EAttackState InAttackState, float InMaxCooltime, float InEnableWeaponSkillPoint, bool bWeaponSkill = false);
	virtual void ResetSkillState(UB2SkillRTManager* SkillRTManager);

	FORCEINLINE bool CheckPCClass(EPCClass CheckClass) const { return UsePCClass == CheckClass; }
	FORCEINLINE bool IsSkillDisabledByCooltime() const { return bIsSkillDisabledByCooltime; }
	FORCEINLINE float GetRemainingCooltime() const { return RemainingCooltime; }
	FORCEINLINE bool IsCountingCooltime() const { return bCountingCooltime; }
	FORCEINLINE void SetAttackState(EAttackState InState) { MappingAttackState = InState; }
	FORCEINLINE EAttackState GetAttackState() const { return MappingAttackState; }
	FORCEINLINE void SetDoc(class UB2UIDocBase* Doc){ RelevantDoc = Doc; }
	FORCEINLINE class UB2UIDocBase* GetDoc(){ return RelevantDoc; }
	FORCEINLINE class UB2UIDocBase const* GetDoc() const { return RelevantDoc; }
	FORCEINLINE bool IsCurrentSuggest() const { return bCurrentSuggest; }
	FORCEINLINE void SetIncreaseMotion(int32 IncMotion) { IncreaseMotion = IncMotion; }
	FORCEINLINE EAttackState GetMappingAttackState() { return MappingAttackState; }

	virtual float GetMaxCooltime(const class UB2SkillRTManager* SkillRTManager);

	virtual bool IsSkillDisabledByPoint() const { return false; }
	virtual void SetSkillDisabledByCooltime(bool bIn) = 0;
	virtual void SetSkillPoint(float Point) = 0;
	virtual void SetRemainingCooltime(float fIn) = 0;
	virtual void SetIsCountingCooltime(bool bIn) = 0;
	virtual void SyncToUIDoc() = 0; // Update all dynamic data here to RelevantSkillDoc		
	virtual void UpdateUIDoc(class UB2SkillRTManager* SkillRTManager) = 0;

	virtual void SetSkillSuggest(bool bSuggest) {}
	virtual void SetSkillDisableBySuggest(bool bDisable) {}
	
};

class FInputBoundSkillState_Skill : public FInputBoundSkillState_Base
{
public:
	virtual void InitSkillState(class UB2SkillRTManager* SkillRTManager, int32 InSkillID, EPCClass InPCClass, EAttackState InAttackState, float InMaxCooltime, float InEnableWeaponSkillPoint, bool bWeaponSkill) override;
	virtual void ResetSkillState(UB2SkillRTManager* SkillRTManager) override;

	virtual bool IsSkillDisabledByPoint() const override;
	virtual void SetSkillDisabledByCooltime(bool bIn) override;
	virtual void SetSkillPoint(float Point) override;
	virtual void SetRemainingCooltime(float fIn) override;
	virtual void SetIsCountingCooltime(bool bIn) override;
	virtual void SyncToUIDoc() override;
	virtual void UpdateUIDoc(class UB2SkillRTManager* SkillRTManager) override;
	virtual float GetMaxCooltime(const class UB2SkillRTManager* SkillRTManager) override;

	virtual void SetSkillSuggest(bool bSuggest) override;
	virtual void SetSkillDisableBySuggest(bool bDisable) override;

protected:
	bool bIsWeaponSkill;
	float CachedWeaponStackPoint;

private:
	class UB2UIDocSkill* GetSkillDoc();

};

class FInputBoundSkillState_Tag : public FInputBoundSkillState_Base
{
public:
	virtual void SetSkillDisabledByCooltime(bool bIn) override;
	virtual void SetSkillPoint(float Point) override {};
	virtual void SetRemainingCooltime(float fIn) override;
	virtual void SetIsCountingCooltime(bool bIn) override;
	virtual void SyncToUIDoc() override;
	virtual void UpdateUIDoc(class UB2SkillRTManager* SkillRTManager) override;
	virtual float GetMaxCooltime(const class UB2SkillRTManager* SkillRTManager) override;

private:
	class UB2UIDocBattle* GetBattleDoc();
};

class FInputBoundSkillState_IgnorePoint : public FInputBoundSkillState_Skill
{
public:
	virtual void SetSkillPoint(float Point) override;
	virtual bool IsSkillDisabledByPoint() const override { return false; };
};

/** To maintain and update (if necessary) player state, which not currently exist */
struct FInactivePlayerStateCache : public FTickableGameObject
{
	FInactivePlayerStateCache() 
		: bNotCachedYet(true), 
		LastHealth(0.0f), 
		CurrentHealth(0.0f), 
		MaxHealth(0.0f), 
		LastArmor(0.0f),
		InactiveCharHPRestoreLimit(0.25f), 
		InactiveCharHPRestoreRate(0.01f),
		WeaponSkillPoint(0),
		CounterAttackPoint(0),
		IsUnityTick(false),
		GuardCoolTime(0.0f),
		HasNotTagDefence(false),
		bTickable(false),
		Interval(1.0f),
		Timer(0.0f)
	{
	}

	~FInactivePlayerStateCache()
	{
		for (auto* skill : IBSkillStates)
		{
			delete skill;
		}

		IBSkillStates.Empty();
	}

	// FTickableGameObject interfaces...
	virtual void	Tick(float DeltaTime) override;
	virtual bool	IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual bool	IsTickableWhenPaused() const override;
	virtual bool	IsTickableInEditor() const override;

	void	StartTick()
	{
		bTickable = true;
	}

	void	StopTick()
	{
		bTickable = false;
		Timer	  = 0.0f;
	}

	/** True means it is still first player character, tag is not used yet. */
	uint32 bNotCachedYet : 1;

	float LastHealth; // The health at the moment of last retiring moment.
	float CurrentHealth; // Health can increase while inactive.. Start from LastHealth.
	float MaxHealth; // Don't need to track on it. Just for the UI HP bar.
	float LastArmor;

	/** Inactive character's HP can be restored up to this ratio (0.0 ~ 1.0 scale). Nothing will be done If inactive character's HP is higher than this. */
	float InactiveCharHPRestoreLimit;
	float InactiveCharHPRestoreRate; // per second unit.

	float WeaponSkillPoint;
	int32 CounterAttackPoint;
	bool IsUnityTick;

	float GuardCoolTime;
	bool HasNotTagDefence;

	// Main functionalities here.
	void SetVariablesByRetiringPlayer(class ABladeIIPlayer* RetiringPlayer);
	void SetToNewCommingPlayer(class ABladeIIPlayer* NewCommingPlayer);

	bool  bTickable;
	float Interval;
	float Timer;

	/** Cache the whole skill state to count the cooltime and restore it back. */
	TArray<FInputBoundSkillState_Base*> IBSkillStates;
};

/**
 * Runtime management of skill and its related features like tag and resurrection
 * It's like a total composite skill management system.
 */
UCLASS()
class BLADEII_API UB2SkillRTManager : public UObject
{
	GENERATED_BODY()

	/** The player character, this runtime manager class is relevant to.
	 * This is current active one. It will be changed along the game play by tag or resurrection. */
	UPROPERTY(Transient)
	class ABladeIIPlayer* CurrentOwnerPlayer;

	EPCClass MainPlayerClass;
	/** Must get this information from DB or some. */
	EPCClass SubPlayerClass;

	/** Cached from owner player (whenever the owner player is changed..). This is an asset information defining the Owner player's skill property. */
	UPROPERTY(Transient)
	class UB2SkillInfo* CachedSkillInfo; 

	/** Main driving stuff here. Each index represents skill input, bound by the user. 
	 * It is for current active player. It gets backed up to InactivePlayerStateCache at the moment of tag. */
	TArray<FInputBoundSkillState_Base*> IBSkillStates;

	//////////////////////////////////////////////////

	/** To maintain and/or update several inactive player state. */
	FInactivePlayerStateCache InactivePlayerStateCache;

	struct FCachedSkillOption
	{
		FCachedSkillOption()
		{
			SkillCooltimeDecbyPassive = 0.0f;
			TagCooltimeDecbyPassive = 0.0f;
		}

		// Ugly key for TMap...
		static FORCEINLINE int32 GetSkillKey(EAttackState AttackState, ESkillOption SkillOption)
		{
			return (static_cast<int32>(AttackState) << 8) + static_cast<int32>(SkillOption);
		}

		void CacheSkillOption(UB2SkillRTManager* SkillRTManager, EPCClass CharClass, const TArray<FInputBoundSkillState_Base*>& InIBSkillStates);


		// Most passive option values are not cached here, but options directly related to skills are here for convenience.
		
		float SkillCooltimeDecbyPassive;
		float TagCooltimeDecbyPassive;

		FORCEINLINE float GetActiveSkillOptionData(EAttackState Key1, ESkillOption Key2) const
		{
			const float* ValuePtr = CachedActiveSkillOptionData.Find(GetSkillKey(Key1, Key2));
			return ValuePtr ? *ValuePtr : 0.f;
		}
		TMap<int32, float> CachedActiveSkillOptionData;  // For options bound to equipped active skill
	};
	FCachedSkillOption CachedSkillOption[2];

	//////////////////////////////////////////////////

	/** Get as response of resurrection. Affect owner character's combat stat. 
	 * It contains all buff information that selected so far by multiple resurrection, but actual applied buff could be just one type. */
	TMap<EResurrectBuffType, int32> SelectedResurrectionBuffMap;

public:
	UB2SkillRTManager(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginDestroy() override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	/** Set owner player ref, and do some initialization, according to the player's property, etc. 
	 * It must be prior to other method calls below.
	 * It will consider InOwnerPlayer as the main class character. */
	void InitSkillRTManager(ABladeIIPlayer* InOwnerPlayer);
	void InitSkillRTManagerInner(ABladeIIPlayer* InOwnerPlayer);

	bool IsMyOwnerLocalPlayer();

private:
	void UpdateSkillDocDataOnPCSwap(); // Update some of static data, not only for PC swap, but also for init.

public:
	/** 
	 * Returns attack state relevant to one of skills bound to SkillInputIndex
	 * ECS_None if nothing available.
	 */
	EAttackState GetSkillOfInputIndex(int32 SkillInputIndex) const;
	ESkillAnimType GetSkillAnimIndex(int32 SkillInputIndex) const;
	static ESkillAnimType GetSkillAnimIndexInternal(int32 InSkillID, int32 InIncMotionValue, class UB2SkillInfo* InSkillInfo);
	int32 GetCounterMotionInc() const;
	int32 GetSkillID(EAttackState SkillAttackState);

#if !UE_BUILD_SHIPPING
	void ChangeSkillInfo(int32 SkillIndexToChange, int32 SkillId);
#endif

	/** To be called by skill attack user input. 
	 * Get the available skill attack state according to the user skill input binding, and do some necessary stuff for state management
	 * The return value is intended to be set to BladeIIPlayer's AttackState, if not ECS_None */
	EAttackState GetStartableSkill(int32 SkillInputIndex, bool IsPlayerInput);

	const float GetSkillShootingRangeForAI(class ABladeIIPlayer* _PlayerCharacter, int32 _SkillInputIndex);

	void ResetSkillCoolTime();

	/** Being called from ComboEnd when a skill attack is finished.
	* FinishedAttackState is supposed to be one of skill state. (Do nothing if not) */
	void SetSkillCoolTimeTimer(EAttackState FinishedAttackState);
	
	void OnEnterPlayerSkillState(EAttackState ReservedSkill);
	
	const TMap<EResurrectBuffType, int32>& GetSelectedResurrectionBuffMap() { return SelectedResurrectionBuffMap; }

public:
	void CountRemainingCoolTime(float DeltaSeconds);

	/** To update some of relevant UIDoc data on every tick basis. Be careful not to place too many variables here. */
	void RealtimeUpdateUIDoc();

	/** Returns current remaining time until usable state divided by cooltime. 0.0 if it is in usable state. */
	float GetRemainingCoolTimeRatio(int32 SkillInputIndex) const;
	/** Returns current remaining time. 0.0 if not available in any way. */
	float GetRemainingCoolTime(int32 SkillInputIndex) const;

	/** Get the player level condition to be able to use the skill bound to the input index. */
	int32 GetEnablePlayerLevel(int32 SkillInputIndex);

	/** See if any skill is bound and exist for SkillInputIndex, regardless of its usable state */
	bool DoesSkillExist(int32 SkillInputIndex) const;

	/** Returns true if any one skill is bound to the designated skill input index, and also usable by all runtime conditions (cooltime, level, etc.) */
	bool IsSkillCurrentlyUsable(int32 SkillInputIndex, bool IsPlayerInput) const;
	
	/** True only when actively counting, false while using skill */
	bool IsSkillCurrentlyCountingCooltime(int32 SkillInputIndex) const;

	/** A little bit different (longer) from IsSkillCurrentlyCountingCooltime. */
	bool IsSkillDisabledByCooltime(int32 SkillInputIndex) const;

	/** Returns all attack states representing skills, currently available by player state and cooltime stuff.
	 * Optionally include tag.
	 * Returns true if found at least one. */
	bool GetCurrentlyAvailableSkillAttackState(TArray<EAttackState>& OutSkillAttackState, bool bIncludeTag = false);

	////////////////////////////////////////////////////////////////////////////////
	// For Tag/Resurrection

	void ForceSetSkillCoolTimerWhenAIPlayerTagging(int32, EAttackState);
	/**
	 * Main (and final) tag operation. Main to sub player character, or sub to main player character.
	 * @param CurrentAttackState : Set this for TagAttack, to start count skill cooltime. In that case, CurrentAttackState is supposed to be one of skill.
	 */
	bool TagToOther(int32 TagAttackType = 0, EAttackState CurrentAttackState = EAttackState::ECS_None);

	/**
	 * Wrapped BornAgain, to call it when proper player got response of resurrection request.
	 */
	bool ResurrectOnResponse(const FB2Resurrection& ResurrectionResult);	
	bool RaidResurrectOnResponse(const FB2RaidResurrection& ResurrectionResult);
	bool ResurrectPlayer(EResurrectGameModeType ModeType, int32 UsedGem = -1, int32 AttackBuffCount = -1, int32 DefenseBuffCount = -1, int32 HealthBuffCount = -1);

	/**
	 * Simply destroy current one and spawn new player character as current,
	 * either maintaining current player class or forcing main class (by bForceSpawnAsMain)
	 * Mainly intended for resurrection, but can be used elsewhere.
	 */
	bool BornAgain(bool bForceSpawnAsMain);

	/** 
	 * Simply re-spawn as current player class without any motion or buff stuff
	 * It is meant to be used for resetting stage state
	 */
	bool SimpleRebirth(bool bKeepCurrentClass = false);

	/** Switching the local player with the selected player **/
	bool LocalPlayerSwitching(EPCClass NewClass);

	/** Get this data from DB or some. */
	int32 GetMaxResurrectionChance(EResurrectGameModeType ModeType);
	/** Currently remaining time to resurrect. Decreased from GetMaxResurrectionChance() */
	int32 GetRemainingResurrectionChance(EResurrectGameModeType ModeType);

	int32 GetResurrectionCost(EResurrectGameModeType ModeType);
	//////////////////////////////////////////////////

	/** Get resurrect buff value to apply owner player's combat stat. 
	 * It might return filtered type by game-mode buff policy even when the user actual selected more than one buff types by resurrections. */
	void GetAppliedResurrectionBuffValues(TMap<EResurrectBuffType, int32>& OutBuffValues);
	
	void OnPlayerChangedByTag(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer);
private:
	EResurrectGameModeType GetCurResurrectGameModeType();
	void UpdateResurrectionBuffOnResponse(int32 AttackBuffCount, int32 DefenseBuffCount, int32 HealthBuffCount);
	void SetResurrectionPlayerPosition();

	/** Common functionality for TagToOther and BornAgain.
	 * It replaces CurrentOwnerPlayer as the new spawned one if successful, as well as returning new spawned one. */
	class ABladeIIPlayer* DynSpawnNewPlayerCommon(EPCClass ClassToSpawn);

	/** Like DynSpawnNewPlayerCommon. But It use input and not use net player.*/
	class ABladeIIPlayer* DynSpawnLocalPlayerCommon(EPCClass ClassToSpawn);

	/** Self post-spawn new player character handling. Sad that we cannot put them in a single function. */
	void OnPlayerChanged_Common();
	void OnPlayerChanged_Tag(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer);
	void OnPlayerchanged_BornAgain();
	
	void SetPlayerCharacterSkill(class ABladeIIPlayer* PlayerCharacter, TArray<FInputBoundSkillState_Base*>& OutIBSkillStates, FCachedSkillOption& OutCachedSkillOption);
	void SetPlayerCharacterSkill_ExtraMode(class ABladeIIPlayer* PlayerCharacter, TArray<FInputBoundSkillState_Base*>& OutIBSkillStates);

	void BindSkillDoc(class ABladeIIPlayer* PlayerCharacter, TArray<FInputBoundSkillState_Base*>& SkillStates);

	bool AllocateSkillState(TArray<FInputBoundSkillState_Base*>& OutSkillStates);
	bool NeedsRebindSkillDoc(class ABladeIIPlayer* PlayerCharacter);

	
	class UB2SkillInfo* GetSkillInfoOfPlayer(class ABladeIIPlayer* InPlayerChar);

public:
		
	/** Override.. for development purpose. i.e. cheat. */
	void OverrideTagClass(EPCClass NewClass);
private:
	/** False means sub character. 
	 * -> It is not exposed as public, as it might not return correct result at some sensitive timing. Use IsMainPlayerCharacter instead. */
	bool IsCurrentlyMainCharacter();
	
public:
	/** False means sub character. It is about actual main/sub, regardless of current active/inactive state. */
	bool IsMainPlayerCharacter(class ABladeIIPlayer* InPC);

	/** Returns the player class that will be spawned at next tagging action. 
	 * It might not return correct result while the core tagging process is being executed here. */
	EPCClass GetTagPlayerClass();
	/** The opposite of GetTagPlayerClass 
	 * It might not return correct result while the core tagging process is being executed here. */
	EPCClass GetCurrentPlayerClass() const;

	/** Tag usable states.. */
	bool IsTagPossibleAtDeadMoment() const; // The most basic condition of tag possibility.
	bool IsTagPossible() const; // True might mean that tag is eventually possible but could be deferred.
	bool IsTagPossibleRightNow() const;
	bool IsTagPossibleButDeferred() const; // If trying tag when this is true, tag will be pending and executed after some sort of current state (probably attacking) is done.
	bool IsTagSuggestedPlayerCondition() const; // See if Tag is suggested because current player is in danger, not the TagAttack.
	bool IsTagAttackSuggested() const; // See if TagAttack is suggested by skill timing, not due to critical condition.
	
	bool IsCountingTagCooltime() const;
	float GetRemainingTagCoolTime() const;
	float GetRemainingTagCoolTimeRatio() const;

	/** Mainly for the In-game UI tag HP bar. */
	bool GetInactivePlayerNotCachedYet();
	float GetInactiveCharacterHealth();
	float GetInactiveCharacterMaxHealth();
	float GetInactiveCharacterArmor();
	float GetInactiveCharacterWeaponSkillPoint();

	void InitInactiveCharacterMaxHealth(float InMaxHealth);

	float GetActiveCharacterWeaponSkillPoint() const;

	float GetActiveCharacterMaxWeaponSkillPoint() const;
	void UpdateWeaponSkillPoint(float SkillPoint);
	void SetZeroWeaponSkillCoolTime();

	void UpdateCounterAttackPoint(int32 CAPoint);

	FORCEINLINE EPCClass GetMainPlayerClass() const { return MainPlayerClass; }
	FORCEINLINE EPCClass GetSubPlayerClass() const { return SubPlayerClass; }

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	/** Development cheat to set skill or tag cooltime to any wanted value */
	static void Cheat_ForceSetCooltime(bool bIsForTag, float NewCooltime);
#endif

public:
	void GetAllSkillOptionValues(int32 InSkillId, TArray<FSkillOptionData>& InOutSkillOption) const;
	void GetAllSkillOptionValuesForUnitySkill(TArray<FSkillOptionData>& InOutSkillOption) const;
	float GetSkillOptionValue(int32 InSkillId, ESkillOption InSkillOption) const;
	float GetPassiveSkillOptionValue(EPCClass CurrentClass, EPassiveType PassiveType, ESkillOption InSkillOption);
	float GetPassiveSkillOptionValue(EPCClass CurrentClass, ESkillOption InSkillOption) const;
	
	static bool IsPercentValue(ESkillOption InSkillOption);

	float GetSkillDamageIncreaseRateByActive(EAttackState InAttackState) const;
	float GetBossDamageIncreaseRateByActive(EAttackState InAttackState) const;

	float GetCriticalRateByActive(EAttackState InAttackState) const;
	float GetCriticalDamageRateByActive(EAttackState InAttackState) const;

	const FCachedSkillOption& GetCachedSkillOption() const;

	FORCEINLINE float GetSkillCooltimeDecbyPassive() const { return GetCachedSkillOption().SkillCooltimeDecbyPassive; }
	
	/** Simple getting TagCooltimeDecbyPassive from current player cached option means 
	 * this option will be applied when it tag to myself, not from myself to other. */
	FORCEINLINE float GetTagCooltimeDecbyPassive() const { return GetCachedSkillOption().TagCooltimeDecbyPassive; }

	void SetCurrentOwnerPlayer(ABladeIIPlayer* NewOwner);	
	FORCEINLINE class ICharacterDataStore* GetCurrentCharDataStore() const { return CurrentOwnerPlayer ? CurrentOwnerPlayer->GetCharacterDataStore() : nullptr; }

	void UpdatePlayerBuffChanged(const TArray<UB2Buff_Base*>& BuffList);

	//////////
	/// Gather code to check necessary SkillID and AnimTypes in static functions..
	/** Either by LocalCharacterData or specified CharacterDataStore */
	static void GetCharacterRegisteredSkillIDs(EPCClass InCharClass, EB2GameMode InGMType, TArray<int32>& OutSkillIDs, class ICharacterDataStore* InSpecifiedCharDataStore);
	/** It is provided to query all necessary SkillAnim IDs required for character of class and CharacterDataStore, mainly for pre-load asset purpose. */
	static void GetCharacterRegisteredSkillAnimTypes(UObject* WorldContextObject, EPCClass InCharClass, EB2GameMode InGMType, TArray<ESkillAnimType>& OutSkillAnimTypes, class ICharacterDataStore* InSpecifiedCharDataStore);
	/** Specially placed at here, to replace old code. You might want to look at CombatStatEval for more general usage.
	 * Either by LocalCharacterData or specified CharacterDataStore */
	static float GetCharacterSkillOptionValue(int32 InSkillId, ESkillOption InSkillOption, class ICharacterDataStore* InSpecifiedCharDataStore);
	static int32 GetCharacterSkillIncMotionValue(EPCClass InCharClass, EB2GameMode InGMType, int32 InSkillId, class ICharacterDataStore* InSpecifiedCharDataStore);
	//////////

	void ResetInitialized(ABladeIIPlayer* InOwnerPlayer);
	void SetAutoBattle(int32 InAutoBattle);

private:
	int32 AutoBattle;
	
	bool bInitialized;
	TArray<std::shared_ptr<class issue>> Issues;
};
