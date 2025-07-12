#pragma once

#include "B2AsyncGameMode.h"
#include "B2UIDocTeamMatch.h"
#include "../DataStore/B2ModPlayerInfoData.h"
#include "../BladeII/BladeIIGameMode.h"

#include "B2TMGameMode.generated.h"

enum class ETeamMatchState : uint8
{
	Finding,
	Ready,
	Battle,
	EndMatchByTime,
	BattleEnd,			//[@AKI, 170420] HP가 다 되어 끝난 경우(HandleNotifyLeave, NotifyPlayerDeadEnd) Casting해서 사용 할 수도 있으니 마지막에 넣어줌
};

enum class ETMBGMPlayContext : uint8
{
	TMBGMCTX_Standby,
	TMBGMCTX_Combat,
	TMBGMCTX_Victory,
	TMBGMCTX_Lose,
	TMBGMCTX_End
};

struct FTeamInfo
{
	FTeamInfo() : TeamNum(INDEX_NONE) {}
	TArray<FCharInfo> Characters;
	TArray<float>	CharacterAccumulateDamage; // order by EPCClass
	int8 TeamNum;

	void SpawnTeamCharacters(class AB2TMGameMode* GameMode);
	void SetAccumulateDamage(const class ABladeIICharacter* InPlayer, float InDamage);

	void GetCharacterList(TArray<class ABladeIICharacter*>& CharacterList, bool bOnlyLive);
	int32 GetNumAliveCharacters();
	float GetCharacterHPTotal();
	int32 GetCharacterCount();
};


class FTeamMatchSceneMagnager
{
public:
	FTeamMatchSceneMagnager() : CurrentState(ETeamMatchState::Finding), TimeLimitBattle(90.0f), LeftTimeBattle(90.0f)
	{
	}

public:
	void SetState(ETeamMatchState InState);
	void SetFinding();
	void SetReady();
	void SetBattle();
	void SetEndMatchByTime();
	void SetBattleEnd();
	void ShowResultUiImage();
	void SetGameMode(class AB2TMGameMode* InGameMode);

	void SetMatchLobbyResources(
		class ALevelSequenceActor* InMatinee,
		const FMatchingAnims& Gladiator,
		const FMatchingAnims& Assassin,
		const FMatchingAnims& Wizard,
		const FMatchingAnims& Fighter
	);
	void SetMatchLobbyCharacterLocationZ(float GladiatorZ, float AssassinZ, float WizardZ, float FighterZ);
	void SetLeftEntry();
	void SetRightEntry();

	void SetStartTimeCount(bool InStartCount);

	void Tick(float DeltaSeconds);
protected:
	ETeamMatchState CurrentState;
	class ALevelSequenceActor * LobbyMatinee;
	TArray<FMatchingAnims> AnimArray;
	TArray<float> CharacterLocationZ;
	TArray<class ASkeletalMeshActor*> SpawnedActors;

	float WaitingReadyToMatchTime;
	float ElapsedWaitingReadyToMatchTime;

	class AB2TMGameMode* GameMode;

	bool IsStartTimeCount;

	float TimeLimitBattle;
	float LeftTimeBattle;
};

class TMGameModeRule : public DefaultGameRule
{
public:
	virtual bool AllowToTag() const override
	{
		return false;
	}

	virtual bool IsAIAllowedToEvade() const override
	{
		return false;
	}

	virtual FString GetBehaviorTreeAsset() const override
	{
		return FString(TEXT("/Game/Blueprints/AI/TMPlayerBehaviorTree.TMPlayerBehaviorTree"));
	}

	virtual bool UseSectionMergedPCModel() const override
	{
		return true;  // Performance rule
	}
	virtual bool IsGlobalTimeDilationEnabled() const override { return false; } 
};

UCLASS()
class BLADEII_API AB2TMGameMode : public AB2AsyncGameMode
{
	GENERATED_BODY()

public:

	AB2TMGameMode(const FObjectInitializer& ObjectInitializer);

	// AActor interfaces...
	virtual void BeginPlay() override;
	virtual void BeginDestroy() final;
	
	// AGameMode Interfaces
	virtual void StartPlay() override;
	virtual bool HasMatchStarted() const override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void SetMatchState(FName NewState) override;
	
	// ABladeIIGameMode Interfaces...
	virtual void PreBeginPlay() override;
	virtual void SubscribeEvents() override; // Events of EventSubsystem
	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;

	virtual void GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs = true) override;
	virtual void GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum = INDEX_NONE, bool bOnlyLiveMobs = true) override;
	virtual void GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum, bool bOnlyLive = true) override;

	virtual void NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer) override;
	virtual void NotifyCharacterDead(class ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser) override;
	virtual void NotifyTargetDamagedFromDamagedTarget(class ABladeIICharacter* DamageCauser, class ABladeIICharacter* DamagedTarget, float fDamageAmount);
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartMatchResult();

	virtual void SetBaseActiveCamera() override;

	virtual class ICharacterDataStore* GetRemotePlayerDataStore(uint32 InTeamNum) override;

	virtual bool IsPVPGameMode() const { return true; }

	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::PVP_Team; }
	virtual EB2SkillLODType GetSkillLODType() final { return EB2SkillLODType::ESLT_ALL_LOD; }

	int32 GetAttackDamageBonusPercentageByFormation(int32 InTeamNum);
	int32 GetReduceDamageBonusPercentageByFormation(int32 InTeamNum);

	float GetOpponentHandicap(int32 InTeamNum);

	// AB2TMGameMode Interfaces
	virtual void HandleRequestMatchMaking();
	virtual void HandleMatchOtherPlayer();
	virtual void HandleStartResult();
	virtual void HandleFinishBattle();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetMatchLobbyResources(
		class ALevelSequenceActor* InMatinee,
		const FMatchingAnims& Gladiator,
		const FMatchingAnims& Assassin,
		const FMatchingAnims& Wizard,
		const FMatchingAnims& Fighter
	);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetMatchLobbyCharacterLocationZ(float GladiatorZ, float AssassinZ, float WizardZ, float FighterZ);

	virtual int8 GetMyTeamNum();
	virtual int8 GetOpponentTeamNum();

	void TakeDamageUpdate();
	virtual ABladeIIPlayer* GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly = false, bool bSpawnPuppetForDead = false) override;
	
	virtual ABladeIIPlayer* SpawnTeamPlayerAsPuppet(bool bAllyTeam, int32 nPlayerIndex, bool bPossessByAIController = true);
	ABladeIIPlayer* SpawnTeamPlayerAsPuppetbyClass(bool bAllyTeam, EPCClass InCharClass, bool bPossessByAIController = true);

	UFUNCTION(BlueprintCallable, Category = "ActiveCameraActor")
	void SetTeamMatchActiveCameraActor(AB2ActiveCameraActor* pActiveCameraFixLong, AB2ActiveCameraActor* pActiveCameraFixShort, AB2ActiveCameraActor* pActiveCameraPlayer);
	AB2ActiveCameraActor* GetTeamPlayerActiveCameraActor();

	void RequestRematch();
	void ReturnToFindAccount();
	void ReturnToTMMainMenu();
	void OpenPopupGotoTMMainMenu();

	void ChangeCharacterCamera(bool bMyTeam, int32 SlotIdx);
	bool ChangeFixedCamera();

	void ActiveTeamMatchActiveCameraActor(AB2ActiveCameraActor* ActiveCameraActor);
	void ActiveTeamMatchActiveCameraActorNextTarget();
	virtual void UpdateUICharSlotCamera();

	void SetOpponentInfo(FB2OpponentTeamMatchInfoDataPtr OpponentServerInfo);

	virtual void GatherEquippedSkillAnimIDs(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutEquippedSkills) override;
	
	ENetMatchResult CalcResult();
	void SetMatchResultInfo(const FB2TeamMatchResult& MatchResult);

	UFUNCTION(BlueprintCallable, Category = "TeamMatch")
	void TeamPlayerMoveToStartPosition(bool bMyTeam);

	virtual void StartEventScene_MatchResult(bool HasLocalPlayerWon) override;

	void SetStartTimeCount(bool InStart);

	void PlayTMBGM(ETMBGMPlayContext InContext, bool bForceStopCurrent = true);
	virtual void UpdateHighAggroCharacters();
	virtual ABladeIICharacter* UpdateHighAggroOpponentCharacter(int32 InMyTeamNumber);

	virtual ABladeIICharacter* GetHighAggroOpponentCharacter(int32 InMyTeamNumber);

	static int8 MyTeamNum;
	static int8 OpponentTeamNum;
	static int32 NumCharactersOfTeam;
	virtual int32 GetNumCharactersOfTeam() const
	{
		return 3;
	};

	TSubclassOf<APawn> GetDefaultPlayerClass() const
	{
		return DefaultPlayerClass;
	}

	void OnMyTeamDead_AddUnityPoint(int32 TeamNum);
	void OnOtherPlayerUnityPointClear(class ABladeIITMAIPlayer* player);

	void NotifyStageEventDirector_FinishShow(int32 EventType);

private:
	int32 GetNextCameraPlayer() const;
protected:

	virtual void OnMatchStateSet() override;
	virtual void NotifyUIWidgetOpenComplete(FName InOpenedUI) override;
	virtual void AddEntryCharacterInfo(int8 TeamNum, int8 Spot, EPCClass CharClass, int32 InDocCharIndex, int32 AggroPriority, int8 FormationNum = 0);
	virtual void SetCharacterDoc();

	virtual void AddMyTeamCharacterInfo();
	virtual void AddOpponentTeamCharacterInfo();

	virtual APawn* RestartEntryPlayer(class AController* NewPlayer, FString const& TagName);

	virtual APlayerController* GetSpectatorController();
	
	virtual int32 GetFormationSlotPosition(ETMFormation Formation, int32 SlotIdx);
	virtual int32 GetAggroPriority(ETMFormation Formation, int32 SlotIdx);

	void SetMyTeamMatchInfo(const FB2TeamBattleStatus& TeamBattleStatus);

	virtual void AllPlayerStopAI();
	virtual void AllPlayerBeginAI();

	virtual void MatchGameStartEventSceneEnd();

	void SetTeamMatchPointInfo(const FB2TeamMatchMaking& MatchInfo);
	
	virtual TArray<EPCClass> GetPCClassesToPreLoad() override;
	virtual TArray<FCombinedPCSkillAnimID> GetPCSkillAnimsToPreLoad() override;
	virtual void PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie) override;

	virtual void PreloadLocalSkillAnimation() {}

	virtual void EnterShop(int32 nWhereShop) override;
	
	void OnPlayerStartWeaponSkillByIndex(int32 nInputIndex);

	virtual bool NeedToRecordDamageLog(const float ActualDamage, class ABladeIICharacter* Attacker) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	float DeathUnityPointChargeAmount = 0.f; // When My Team member Death..

protected:
	FTeamInfo MyTeamInfo;
	FTeamInfo OpponentTeamInfo;

	FTeamInfo& GetTeamInfo(int32 InTeamNum);
	FTeamMatchSceneMagnager SceneManager;

	AB2ActiveCameraActor* TeamMatchFixLongActiveCameraActor;
	AB2ActiveCameraActor* TeamMatchFixShortActiveCameraActor;
	AB2ActiveCameraActor* TeamMatchPlayerActiveCameraActor;
	AB2ActiveCameraActor* TeamMatchTempActiveCameraActor;
	FB2ModPlayerInfo OpponentTeamCharacterDataStore;

	friend struct FTeamInfo;

	FString	PlayToken;

	FTimerHandle TimeToChangeCameraTarget;

	float	ReduceOpponentStatForTutorial;

	int32 CurrentActiveCameraPlayerIndex;

	UPROPERTY(Transient)
	TArray<ABladeIICharacter*> CachedHighAggroCharacter;

	UPROPERTY(Transient)
	TSubclassOf<class APawn>			DefaultPlayerClass;

};
