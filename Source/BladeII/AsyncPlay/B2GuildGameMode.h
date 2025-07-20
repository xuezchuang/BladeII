#pragma once

#include "B2AsyncGameMode.h"
#include "B2ModPlayerInfoData.h"
#include "B2GuildGameMode.generated.h"

namespace GuildMatchState
{
	extern BLADEII_API const FName ReadyToRound;
	extern BLADEII_API const FName ProgressRound;
}

UENUM(BlueprintType)
enum class ETeamIndex : uint8
{
	nNULL = 0,
	Player = 1,
	Enemy = 2,
	TeamIndex_Max = 3,
};

USTRUCT()
struct FRoundInfo
{
	GENERATED_USTRUCT_BODY()

	int8 RoundNumber;
	int64 BattleTimeUnix;

	TMap<ETeamIndex, int32> TeamInfoIndexInRound;
	TMap<ETeamIndex, int32> WinRoundCount;
	UPROPERTY()
	TArray<class AAIController*> MyTeamControllerCache;
	UPROPERTY()
	TArray<class AAIController*> EnemyTeamControllerCache;
	
	TArray<class AAIController*> & GetTeamControllerCache(ETeamIndex InTeamIndex)
	{
		if (InTeamIndex == ETeamIndex::Player)
			return MyTeamControllerCache;
		else
			return EnemyTeamControllerCache;
	}

	ETeamIndex WinnerTeam;

	int32 GetRoundTeamNum(ETeamIndex InTeamIndex)
	{
		return TeamInfoIndexInRound[InTeamIndex];
	}
};

struct FGuildPlayerInfo : public FCharInfo
{
	FGuildPlayerInfo()
		: FCharInfo()
	{}

	FGuildPlayerInfo(int8 InSpot, EPCClass InClass, int32 InDocCharIndex, int32 InAggroPriority, int8 FormationNum = 0)
		: FCharInfo(InSpot, InClass, InDocCharIndex, InAggroPriority, FormationNum)
	{}

};

struct FGuildMercenaryInfo : public FGuildPlayerInfo
{
	FGuildMercenaryInfo()
		: FGuildPlayerInfo()
	{}

	//Mercenary
	int32 MercenaryID;
	int32 MercenaryLevel;
	
	FGuildMercenaryInfo(int32 InMercenaryID, int32 InMercenaryLevel, int8 InSpot, int32 InDocCharIndex, int32 InAggroPriority, int8 FormationNum = 0)
		:FGuildPlayerInfo(InSpot, EPCClass::EPC_End, InDocCharIndex, InAggroPriority, FormationNum), 
		MercenaryID(InMercenaryID), MercenaryLevel(InMercenaryLevel)
	{}
};

struct FGuildTeamInfo
{
	FGuildTeamInfo() : TeamIndex(ETeamIndex::TeamIndex_Max) {}
	TArray<FGuildPlayerInfo> PlayersInfos;
	TArray<FGuildMercenaryInfo> MercenaryInfos;
	ETeamIndex TeamIndex;
};

struct FNPCUsingSkillState
{
	bool bUsingSkill;
};

struct FMoveToCompleteInfo
{
	TMap<ETeamIndex, int32>								MoveReqeustCount;
	TMap<ETeamIndex, TMap<uint32, AAIController*>>		MoveRequestMap;

	FMoveToCompleteInfo();

	void SetMoveReqeustCount(ETeamIndex InTeamIndex, int32 Count);
	void AddMoveRequestID(ETeamIndex InTeamIndex, uint32 RequestID, AAIController* AIController);
	//bool ReceiveRequestID(uint32 RequestID, EPathFollowingResult::Type Result, ETeamIndex & ComplateTeam, AAIController** OutController);
};

//config肺? Config 炼累捞 呈公 浆况辑 老窜 内靛俊
const float GUILD_FIGHT_TIME_LIMIT = 90.0f;

USTRUCT()
struct FPCCache
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Transient)
	TArray<ABladeIIPlayer*> MyTeamPC;

	UPROPERTY(Transient)
	TArray<ABladeIIPlayer*> EnemyTeamPC;

	void ClearCache();
	void AddCache(ETeamIndex team, ABladeIIPlayer* AddedCachePC);
};



class GuildGameRule : public DefaultGameRule
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
	virtual bool AllowPlayerPointLight() const override { return false; }
};

UINTERFACE()
class UGuildPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class IGuildPawnInterface
{
	GENERATED_BODY()
public:
	virtual const FGuildPlayerInfo& GetGuildPlayerInfo() = 0;
	virtual bool PawnIsAlive() = 0;
	virtual class ABladeIICharacter* GetPawnInstance() = 0;
	virtual AController* GetPawnController() = 0;
	virtual FVector GetLocation() const = 0;
	virtual FString GetPlayerStartName() = 0;
};


UCLASS()
class BLADEII_API AB2GuildGameMode : public AB2AsyncGameMode
{
	GENERATED_BODY()

public:
	const float GUILD_BATTLE_TIME = 90.0f;
	const uint32 ROUND_PER_MAX_NUMBER = 4;

	AB2GuildGameMode(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	bool IsTestMode;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void CreateDummyGuildTeamInfo();
	TArray<int32> SelectedMercenaryIDList;
#endif

	void CreateGuildTeamInfo();

	// AActor interfaces...
	virtual void BeginPlay() override;

	// AGameMode Interfaces
	virtual void StartPlay() override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleStartResult();
	virtual void HandleFinishBattle();
	virtual void HandleReadyToRound();
	virtual void HandleProgressRound();

	virtual void KillAllEnemy() override;
	virtual void KillAllAlly();

	void KillAllTeam(ETeamIndex InTeamIndex);

	void SetRemainBattleTime(float InTime);

	virtual void OnMatchStateSet() override;
	virtual void StartEventScene_MatchResult(bool HasLocalPlayerWon) override;
	virtual void Tick(float DeltaSeconds) override;

	// ABladeIIGameMode Interfaces...
	virtual void PreBeginPlay() override;
	virtual void SubscribeEvents() override; // Events of EventSubsystem
	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;

	virtual void SetupUIScene() override;

	virtual void GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs = true) override;
	virtual void GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum = INDEX_NONE, bool bOnlyLiveMobs = true) override;
	virtual void GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum, bool bOnlyLive = true) override;

	int32 NumberOfAliveCharacter(ETeamIndex InTeamIndex);
	float GetTotalRemainHPPercent(ETeamIndex InTeamIndex);

	virtual void NotifyCharacterDead(class ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser) final;

	ABladeIICharacter* GetHighAggroOpponentCharacter(ABladeIICharacter* OwnerPawn, int32 InMyTeamNumber, FVector CurrentPawnLocation);
	
	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::Guild; }
	virtual EB2SkillLODType GetSkillLODType() final { return EB2SkillLODType::ESLT_ALL_LOD; }

	virtual void SetBaseActiveCamera() override;
	void ChangePlayerActiveCamera(bool bShort, class ABladeIIPlayer* ViewTarget, float BlendTime);
	bool IsShortActiveCamera();

	virtual bool IsPVPGameMode() const { return true; }

	void ReturnToGuildMap();

	void TakeDamageUpdate();
	virtual ABladeIIPlayer* GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly = false, bool bSpawnPuppetForDead = false) override;
	virtual ABladeIICharacter* GetTeamCharacter(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly = false, bool bSpawnPuppetForDead = false);

	virtual ABladeIICharacter* SpawnTeamCharacterAsPuppet(bool bAllyTeam, int32 nPlayerIndex, bool bPossessByAIController = true);
	
	//class ABladeIIMercenaryPlayer* SpawnGuildNPCCharacter(UB2NPCClassInfoBox* InfoBox, ENPCClass InClass, ENPCClassVariation InVariation, FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);
	void SpawnTeam(FGuildTeamInfo& InGuildTeamInfo);
	void ResetPositionTeam(ETeamIndex InTeamIndex);

	int32 GetAttackDamageBonusPercentageByFormation(int32 InTeamNum);
	int32 GetReduceDamageBonusPercentageByFormation(int32 InTeamNum);

	void SetFormationPosition(class AController* NewPlayer, FString const& TagName);

	void AllControllerAutoTrigger(bool Start);

	void TeamPlayerMoveToBattlePosition(ETeamIndex InTeamIndex);

	void ClearAllDebuff(ETeamIndex InTeamIndex);
	void ResetSkillCoolTime(ETeamIndex InTeamIndex);

	//UFUNCTION(BlueprintCallable, Category = "ActiveCameraActor")
	//void SetGuildMatchActiveCameraActor(AB2ActiveCameraActor* pActiveCameraPlayerLong, AB2ActiveCameraActor* pActiveCameraPlayerShort, AB2ActiveCameraActor* pActiveCameraBaseFix);

	void ReturnToGuildMainMenu();

	virtual int8 GetMyTeamNum()			{ return (int8)ETeamIndex::Player; }
	virtual int8 GetOpponentTeamNum() 	{ return (int8)ETeamIndex::Enemy; }
	
	virtual void MatchGameStartEventSceneEnd();

	TSharedPtr<FNPCUsingSkillState> SharedUsingNPCSKillState();


	virtual void PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie) override;

	virtual TArray<EPCClass> GetPCClassesToPreLoad() override;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void CallSpawnAnim(ETeamIndex InTeamIndex);

	void FinishBattlePresentation(bool bTimeEnd, ABladeIICharacter* InInflictor = nullptr);

	void ShowRoundUI(bool bStart, float InTime);
	void InitDilation();

	void SetAutoChangeCamera(bool bSet);

protected:
	class UB2UIDocGuild* GetGuildDoc();

	virtual void InitGuildDoc();
	
	virtual int32 GetFormationSlotPosition(int32 SlotIdx);
	virtual int32 GetAggroPriority(int32 SlotIdx);

	virtual void NotifyStageEventDirector_FinishShow(int32 EventType);

	EFormationNUM GetForamtionNUM(int32 Index);
	void OnPlayerStartWeaponSkillByIndex(int32 nInputIndex);

	void SetDefaultTeamCombatStat(FGuildTeamInfo& TeamInfo, FCombatStatParam& CombatStat);
	FCombatStatParam GetCombatStat(FGuildTeamInfo& TeamInfo, FGuildPlayerInfo& CharacterInfo);

	bool HaveNextTeam(ETeamIndex InTeamIndex);
	void SweepingTeam(FRoundInfo & InRound ,ETeamIndex TeamIndex, bool bForceDie = false);

	void StartRound();
	bool SetNextRound();

	FGuildTeamInfo& GetGuildTeamInfo(ETeamIndex InTeamIndex);

	class AAIController* GetFindControllerInRound(ETeamIndex InTeamIndex, const FGuildPlayerInfo & InGuildPlayerInfo);

	void OnTeamPlayerMoveToBattlePosition(ETeamIndex InTeamIndex);

	//UFUNCTION()
	//void OnMoveToComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void OnReadyToRoundTime();

	UFUNCTION()
	void OnEndTimeFinishBattle();

	UFUNCTION()
	void OnChangeCamera();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	class UParticleSystem* GuildNPCFloorRing;

public:

	UPROPERTY(EditAnywhere, Category = B2Animation)
	TMap<EPCClass, UAnimSequenceBase *>			PCAnimMap;

	UPROPERTY(EditAnywhere, Category = "BladeIIGame")
		float WaitReadyToRoundTime;

	UPROPERTY(EditAnywhere, Category = "BladeIIGame")
		float WaitEndTimeFinishBattleTime;

	UPROPERTY(EditAnywhere, Category = "BladeIIGame")
		float ChangeCameraTime;

	UPROPERTY(EditAnywhere, Category = "BladeIIGame")
		float FirstWaitSpawnAnimTime;

private:

	//AB2ActiveCameraActor* PlayerLongActiveCameraActor;
	//AB2ActiveCameraActor* PlayerShortActiveCameraActor;
	//AB2ActiveCameraActor* BaseFixActiveCameraActor;

	FB2ModPlayerInfo OpponentTeamCharacterDataStore;
	FB2ModPlayerInfo MyTeamCharacterDataStore;
	TMap<ETeamIndex, TArray<FGuildTeamInfo>> MatchTeamInfos;
	
	FRoundInfo CurrentRound;

	TArray<class ASkeletalMeshActor*>	SpawnedActors;

	TArray<FTransform>					CharTransform;
	
	FPCCache							PCCache;
	TSharedPtr<FNPCUsingSkillState>		IsNPCSkillUsing;

	FMoveToCompleteInfo					MoveToCompleteInfo;

	class UB2UIDocGuild*				CacheGuildDoc;

	float RemainBattleTime;

	FTimerHandle						ReadyToRoundTimer;
	FTimerHandle						EndTimeFinishBattleTimer;

	FTimerHandle						ChangeCameraTimer;
	bool								bExcutedFinishBattle;
};