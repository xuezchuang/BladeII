// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "B2StageGameMode.h"
#include "BladeIIPlayer.h"
//#include "B2UITutorialBattleMain.h"
//#include "B2UIDocBattle.h"
#include "Engine/BlockingVolume.h"
#include "B2TutorialGameMode.generated.h"

class TutorialGameRule : public DefaultGameRule
{
public:
	virtual bool AllowToTag() const override {
		return false;
	}
	virtual bool IsQTEEnabled() const override {
		return bIsQTEEnabled;
	}
	void SetQTEEnabled(bool bIsEnabled) {
		bIsQTEEnabled = bIsEnabled;
	}

private:
	bool bIsQTEEnabled = false;
};

/**
*
*/

UENUM(BlueprintType)
enum class ETutorialNumber : uint8
{
	Tutorial_1,
	Tutorial_2,
	Tutorial_3,
	Tutorial_4,
	Tutorial_5,
	Tutorial_6,
	Tutorial_7,
	Tutorial_8,
	Tutorial_9,
	Max,
};

UENUM(BlueprintType)
enum class ETutorialEventType : uint8
{
	None,
	WidgetShow,
	WidgetHide,

	OpenSkill,
	OpenUnitySkill,
	OpenDialog,

	OpenTag,
	closeTag,

	OpenGuard,
	CloseGuard,

	WaitingForCondition,

	VirtualJoystickShow,
	VirtualJoystickHide,

	delay,

	Max,
};

UENUM(BlueprintType)
enum class ETutorialConditionType : uint8
{
	None,
	OpenDialog,
	CloseDialog,
	InputQTE,
	InputAttack,
	InputGuard,
	InputTag,
	InputSKill,
	InputUnitySkill,
	delay,
	WaitingForQTESuggest,
	WaitingForQTEPreparing,
	Max,
};

USTRUCT(BlueprintType)
struct FTutorialPhaseData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETutorialEventType TutorialEventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETutorialConditionType CompleteConditionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ContentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLockInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPauseScenarioUntilClose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScenarioPauseDelay;
};

USTRUCT(BlueprintType)
struct FTutorialPhase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTutorialPhaseData> Phases;
};


USTRUCT(BlueprintType)
struct FTutorialArrow
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AEmitter* TargetArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNumber;

	bool IsShow;
};

DECLARE_DELEGATE(FOnTutorialPhaseClear);

UCLASS()
class BLADEII_API AB2TutorialGameMode : public AB2StageGameMode
{
	GENERATED_BODY()

public:

	AB2TutorialGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void PreBeginPlay() override;
	virtual void BeginPlay() override;

	void SetTutorialGameRule(bool bIsQTE = false);

	virtual void SetDesiredPCClassEnum() override;

	virtual void SubscribeEvents() override;

	virtual void SetupUIScene() override;

	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSuccessQTE_Mino();

public:
	static void GetFullStackSkillId(EPCClass InCharacterClass, TArray<int32>& OutSkillArray);

	virtual EB2GameMode GetB2GameModeType() override {
		return EB2GameMode::Tutorial;
	}
	virtual bool IsQTEEnabled() {
		return true;
	}

	FORCEINLINE bool IsCurrentTutorial(ETutorialNumber Number) {
		return GetCurrentTutorial() == Number;
	}

	void SetOpenSkillButton(EPCClass TargetClass, int32 SkillIndex, bool bOpen);
	//void SetGuardLock(ETutorialWidgetState Lock);

	void StartForceFeedback();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetVirtualJoystickVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void InitTutorialUI();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetStartTutorial(ETutorialNumber Number);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetFinishTutorial();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowTutorialFinishScene();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ActionNextTutorial();

	void UpdateConditionTutorial1(FTutorialPhaseData* TutorialPhaseData);
	void UpdateConditionTutorial2(FTutorialPhaseData* TutorialPhaseData);
	void UpdateConditionTutorial3(FTutorialPhaseData* TutorialPhaseData);
	void UpdateConditionTutorial4(FTutorialPhaseData* TutorialPhaseData);
	void UpdateConditionTutorial5(FTutorialPhaseData* TutorialPhaseData);
	void UpdateConditionTutorial6(FTutorialPhaseData* TutorialPhaseData);
	void UpdateConditionTutorial7(FTutorialPhaseData* TutorialPhaseData);
	void UpdateConditionTutorial8(FTutorialPhaseData* TutorialPhaseData);
	void UpdateConditionTutorial9(FTutorialPhaseData* TutorialPhaseData);

	void InitSettingTutorialPhase(ETutorialNumber Number);
	void StopRepeatingInput();

	void OnInputMove();						// Tutorial 1
	void OnInputAttack();					// Tutorial 2
	void OnInputGuard();					// Tutorial 4
	void OnInputSkill(int32 SkillIndex);	// Tutorial 6
	void OnInputQte();
	void OnInputUnitySkill();
	void OnInputTag();

	void OnDelayTime();

	void OnCloseScenarioDialog(FName CloseDialogName);
	void OnOpenScenarioDialog(FName OpenDialogName);

	void StartGuideArrow();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnMobAttackNotify();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnMobStopAttackNotify();

	void OnCharacterDamaged(ABladeIICharacter* VictimCharacter, float Damage);
	void SetMinoQTEState(ABladeIICharacter* VictimCharacter);
	void StartAutoMinoRidding();
	void SetAutoMinoRidding();
	void StartAutoUnitySkill();
	void SetAutoUnitySkill();

	void ShowScenarioDialog(FName DialogCodeName);

	bool CheckTutorialCondition(ETutorialConditionType EventType);

	bool IsInvalidTutorialIndex();

	FORCEINLINE bool IsVisibleVirtualJoystick() const {
		return bVisibleVirtualJoystick;
	}

	UFUNCTION()
	void OnChangedQTEEnableState(class UB2UIDocBase* Sender, EQTEUIAttackEnableTimingState CurrQTEState, EQTEUIAttackEnableTimingState PrevQTEState);

	void OnCounterAttackSuccessed();

	void OnPlayerAttackStateChange(class ABladeIITutorialPlayer* TutorialPlayer, EAttackState InAttackState);
	void ActiveQTESuggest();

	UFUNCTION(BlueprintCallable, Category = "TutorialGameMode")
	void SetTutorialArrows(TArray<FTutorialArrow> GuideArray)
	{
		TutorialArrows = GuideArray;
	}

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetDirectPlayerTag();

	int32 GetClientStageId();
	int32 GetStageDifficulty();

	void SendFinishActionLog();

	void SetAllSkillButtonEnable(bool IsEnabled);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowTutorialArrow(int32 WaveNumber);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowTutorialArrowDirect(AEmitter* TargetArrow);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetForceKillEnemies();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	TArray<FTutorialPhase> TutorialPhases;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	//TSubclassOf<class UB2PreLoadingScreen> TutorialFinishScreenClass;

	//UPROPERTY()
	//class UB2PreLoadingScreen* FinishScreenPtr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	TArray<FTutorialArrow> TutorialArrows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	int32 MinoSpawnWaveNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	ABlockingVolume* CounterPhaseBlockingVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	float ScaledAttackLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	float DifficultyScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	float PCTotalAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	float PCTotalDefense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	float PCHealth;

protected:

	virtual float GetCurrentStageDifficultyScale() override;

	virtual void NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType) override;
	virtual void NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo) override;
	virtual void NotifySpawnPoolSingleWaveComplete(class AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum) override;

	FTutorialPhaseData* GetCurrentTutorialPhase();
	FORCEINLINE ETutorialNumber GetCurrentTutorial() {
		return static_cast<ETutorialNumber>(TutorialMainIndex);
	}

	void SetTutorialPhaseInputControl(bool bLockInput);

	void PauseTutorialGame();
	void ResumeTutorialGame();

	void StartSlowTime(float SlowSpeed, float DuringTime);
	void StopSlowTime();

	void SetUnitySkillUI();

	void SpawnWaveEnemies();
	void SetMinoRidingMatinee();
	void DisableBlockingComponent();

	void SetNextPlayerClass(EPCClass PCClass);

	virtual const FString GetClosingMovieName() override;

public:
	// Fixed pc equipment list for tutorial play
	static void GetPCPartsForTutorialPlay(EPCClass InClass, TArray<FB2Item>& OutItemData, FB2Wing& OutWingData);
	class ABladeIIPlayer* GetFallbackPuppetPlayer(EPCClass InCharClass); // Might be necessary for stage event scene setup
protected:
	void PreBuildCharacterMeshes();
	virtual void PreBuildTagCharMesh() override;

	virtual TArray<EPCClass> GetPCClassesToPreLoad() override;
	virtual TArray<EPCClass> GetPCClassesToMatineePreLoad() override;
public:
	void MarkBeginAsyncLoadNextPCClass();
protected:
	void BeginAsyncLoadNextPCClass();
	void BeginAsyncLoadPCClass(EPCClass InClassToReqLoad);

protected:

	UPROPERTY()
	class UB2UITutorialBattleMain* BattleMainUI;

	TutorialGameRule* GameRule;

	int32 TutorialSubIndex;
	int32 TutorialMainIndex;

	FTimerHandle TutorialTH;
	FTimerHandle DealyKillTH;
	FTimerHandle ScenarioDialogTH;
	FTimerHandle QTEPerfectTH;
	FTimerHandle SlowTimeTH;
	FTimerHandle AutoMinoRiddingTH;
	FTimerHandle AutoUnitySkillTH;

	bool bTutorialPhaseDone;

	bool bVisibleVirtualJoystick;

	bool bReadyForQTEAction;
	bool bSuggestQTEAction;
	bool bTutorialQTEComplete;

	bool bReadyForCounterAction;
	bool bTutorial4CounterAction;
	bool bCountAttackSuccess;
	bool bSetMinoZombie;
	bool bMinoAttackVibration;
	bool bTutorialUnitySkillAction;

	TArray<FOnTutorialPhaseClear> OnPhaseClearCallbackList;

	/** A new PCClass data load request will be begin at next tick if it is other than EPC_End. */
	EPCClass PendingAsyncLoadRequestPCClass;
	TArray<EPCClass> PCClassAsyncLoadOrder;

	/**
	* Specially for TutorialGM
	* Used for set-up controlled matinee puppet of StageEventDirector without actual player character.
	*/
	UPROPERTY(Transient)
	TMap<EPCClass, class ABladeIITutorialFallbackPuppetPlayer*> FallbackPuppets;
};


UCLASS()
class BLADEII_API ABladeIITutorialPlayer : public ABladeIIPlayer
{
	GENERATED_BODY()

public:

	ABladeIITutorialPlayer(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeCombatStats() override;

	virtual class ICharacterDataStore* GetCharacterDataStore();

	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const override;

	virtual float GetScaledAttackLevel() override;

	virtual void SetAttackState(EAttackState InAttackState) override;

	virtual void StartAttack()override;

	virtual void StopAttack()override;

	virtual void StartGuard(bool bFromInput = false) override;

	virtual void StopGuard(bool bForce = false) override;

	virtual void StartWeaponSkill() override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	virtual float GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds) override;
	virtual void AddWeaponSkillPoint(float ToAddPoint) override;

	virtual void StopQTEMount() override;

	void ActiveTutorialCounterGuard();
	void OnTutorialCounterActivated();

	FORCEINLINE void StartEanbleAttack(bool bEanble) {
		bEnableAttack = bEanble;
	}
	FORCEINLINE void StartEanbleGuard(bool bEanble) {
		bEnableGuard = bEanble;
	}

protected:

	virtual void SetupComponentsForParts(bool bMergeSections, class USkeletalMesh** OutGetBuiltResultOnly = nullptr) override;

protected:

	bool bEnableAttack;
	bool bEnableGuard;
	bool bWaitingCounterGuard;
	bool bWaitingMoveInput;
};

/** Just dummy, but It has its own reasone to be a separated class. */
UCLASS()
class BLADEII_API ABladeIITutorialFallbackPuppetPlayer : public ABladeIIPlayer
{
	GENERATED_BODY()

public:

	ABladeIITutorialFallbackPuppetPlayer(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void SetupComponentsForParts(bool bMergeSections, class USkeletalMesh** OutGetBuiltResultOnly = nullptr) override;

	virtual	void SubscribeEvents() override;
	virtual void UnsubscribeEvents() override;

	virtual bool ShouldLoadInGameOnlyInfoForPartSetup() const override {
		return false;
	}
	virtual bool IsHeadPointLightAllowed() const override {
		return false;
	}

	virtual bool CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType) override;
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const override;
};
