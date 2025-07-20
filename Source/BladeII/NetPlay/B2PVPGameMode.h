// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "B2NetGameMode.h"
#include "../BladeII/BladeIIPlayer.h"
#include "B2PVPGameMode.generated.h"

enum class EPVPMatchState : uint8
{
	NONE,
	Finding,
	Ready,
	InGameReady,
	PreBattle,
	Battle,
	EndMatchByTime,
	BattleEnd,			//[@AKI, 170418] HP啊 促 登绢 场抄 版快(HandleNotifyLeave, NotifyPlayerDeadEnd) Casting秦辑 荤侩 且 荐档 乐栏聪 付瘤阜俊 持绢淋
};

enum class EPVPBGMPlayContext : uint8
{
	PVPBGMCTX_Standby,
	PVPBGMCTX_Combat,
	PVPBGMCTX_Victory,
	PVPBGMCTX_Lose,
	PVPBGMCTX_End
};

class ALevelSequenceActor ;
class AB2PVPGameMode;

class FPVPMatchSceneManager
{
public:
	FPVPMatchSceneManager() :
		CurrentState(EPVPMatchState::Finding), FindingMatchLevelPackageName(NAME_None), BattleMatchLevelPackageName(NAME_None), GameMode(nullptr) 
	{
	}

	void SetGameMode(class AB2PVPGameMode* InGameMode);
	void SetState(EPVPMatchState InState);
	EPVPMatchState GetState() const
	{ 
		return CurrentState; 
	};
	void Tick(float DeltaSeconds);

	FORCEINLINE void SetMatchLobbyLevelName(FName InName) { FindingMatchLevelPackageName = InName; }
	FORCEINLINE void SetMatchBattleLevelName(FName InName) { BattleMatchLevelPackageName = InName; }
	void SetMatchLobbyResources(
		class ALevelSequenceActor * InMatinee,
		const FMatchingAnims& Gladiator,
		const FMatchingAnims& Assassin,
		const FMatchingAnims& Wizard,
		const FMatchingAnims& Fighter
	);
	void SetMatchLobbyCharacterLocationZ(float GladiatorZ, float AssassinZ, float WizardZ, float FighterZ);
	void SetLeftEntry(class ICharacterDataStore* PlayerCharacterDataStore, EPCClass, EPCClass);
	void SetRightEntry(class ICharacterDataStore* PlayerCharacterDataStore, EPCClass, EPCClass);
	void SetStartTimeCount(bool InStartCount);
	void ChangeScene(bool bStateFromReadyToBattle);
	void ShowResult();
	void ShowResultUiImage();

private:
	void SetFinding();
	void SetReady();
	void SetInGameReady();
	void SetBattle();
	void SetPreBattle();
	void SetBattleEnd();
	void SetEndMatchByTime();
	void SetCameraMatineeCam(float KeyTime);

	EPVPMatchState						CurrentState;
	FName								FindingMatchLevelPackageName;
	FName								BattleMatchLevelPackageName;

	ALevelSequenceActor *				LobbyMatinee;
	TArray<FMatchingAnims>				AnimArray;
	TArray<float>						CharacterLocationZ;

	TArray<class ASkeletalMeshActor*>	SpawnedActors;

	AB2PVPGameMode*						GameMode;

	bool								IsStartTimeCount;

	float								WaitingReadyToMatchTime;
	float								ElapsedWaitingReadyToMatchTime;

	double								TimeBeginPlay;
	double								TimeLimitBattle;
	double								LeftTimeBattle;

	EPCClass							RemoteMainClass;
	EPCClass							RemoteSubClass;
};

class ABladeIINetPlayer;
class ABladeIIPlayerAIController;

static const int MAIN		=	0;
static const int TAG		=	1;

class PVPGameRule : public DefaultGameRule
{
public:
	virtual FString GetBehaviorTreeAsset() const override
	{ 
		return FString(TEXT("/Game/Blueprints/AI/PVPAIBT.PVPAIBT")); 
	}

	virtual bool IsGlobalTimeDilationEnabled() const override { return false; }
	virtual bool IsHitFreezeEnabled() const override { return false; }
	virtual bool IsEvadeCoolTimeCheck() const override { return true; }
	virtual bool ForceApplyHitAnimation() const { return true; }
};

UCLASS()
class BLADEII_API AB2PVPGameMode : public AB2NetGameMode
{
	GENERATED_BODY()
	
public:
	AB2PVPGameMode(const FObjectInitializer& ObjectInitializer);

	// AActor interfaces...
	virtual void BeginPlay() final;
	virtual void BeginDestroy() final;
	virtual void Destroyed() final;
	virtual void HandleMatchHasStarted() final;
	virtual void Tick(float DeltaSeconds) final;

	// BladeIIGameMode interfaces...
	/** Called before any other actor's BeginPlay is called. */
	virtual void PreBeginPlay() final;
	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;
	virtual void SetDesiredPCClassEnum() final;
	virtual void SubscribeEvents() final; // Events of EventSubsystem
	virtual bool TagPlayerCharacter(AActor*) final;
	virtual void GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum = INDEX_NONE, bool bOnlyLiveMobs = true) final;
	//virtual void GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /* = true */) final;
	virtual void NotifyPlayerDeadEnd(class ABladeIIPlayer*) final;
	virtual void NotifyCharacterDead(class ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser) final;
	virtual void SetupUIScene() override;

	virtual bool IsPVPGameMode() const { return true; }
	
	virtual bool IsGlobalTimeDilationEnabled(const AActor* RelativeActor /*= NULL*/) override;

	virtual void SetMatchState(FName NewState) override;
	/** General auto play on/off method. Other Start/StopAutoPlay methods in BladeIIPlayer(Controller) are not for direct use by user input. */
	virtual void SetPlayerAutoType(int32 InAuto) override;

	virtual TArray<EPCClass> GetPCClassesToPreLoad() override;
	virtual TArray<FCombinedPCSkillAnimID> GetPCSkillAnimsToPreLoad() override;
	virtual void SetupPreRenderObjects() override;
	void PreBuildTagCharMesh();

	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::PVP_Tag; }

	virtual EB2SkillLODType GetSkillLODType() final { return EB2SkillLODType::ESLT_ONLY_FOR_SELF; }

	// NetGameMode interfaces...
#if WITH_EDITOR
	void HandleEditorPVPPreviewSetup();
	void HandleEditorPVPPreviewRemoteDead();
#endif

	virtual void PrepareNetGame(uint32) override;
	virtual void HandleRequestMatchMaking() override;	
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchOtherPlayer() override;
	virtual void HandleExchangeInfos() override;
	virtual void HandleDoneToExchange() override;
	virtual void HandleStartResult() override;
	virtual void HandleLeaveEnemy() override; // to GameMode
	virtual void HandleFinishBattle() override;
	virtual void SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& AccountInfo) override;
	virtual void SetRemotePlayerInfoData(uint32 NetId, int64, const FB2FindAccountInfoPtr& AccountInfo) override;
	virtual void SpawnRemotePlayer(uint32, EPCClass MainClass, EPCClass SubClass) override;
	virtual void PrepareSlaveNetGame(uint32);
	virtual void HandleNotifyResult(ENetMatchResult, uint8) override;
	virtual void HandleNotifyLeave(uint32) override; // to server
	virtual void HandleCharacterDead(ABladeIICharacter*, uint32, float) override;
	virtual void SetRoomParticipant(const TArray<FB2RoomParticipantPtr>&) override;
	virtual ERoomCloseReasonType GetStartRoomCloseReason() final;
	virtual void OnHostChanged(int32 RoomType, int32 NewHostId) override;
	virtual bool CanTakeDamage() override;
	virtual void EnterShop(int32 nWhereShop) override;

	void PlayMatchWithAI(FB2TagMatchMakingPtr InMatchInfo);
	void LeaveOpponentPlayer();

	// PVPGameMode interfaces
	void ForceTagWhenPartnerDie(AActor*);
	bool RemotePlayerTagToOther(ABladeIINetPlayer*,int32);
	ABladeIIPlayer* SpawnTagPlayer(ABladeIINetPlayer*, EPCClass, const FTransform&, ESpawnActorCollisionHandlingMethod, bool);
	ABladeIIPlayer* DynamicSpawnNewPlayerForTag(ABladeIINetPlayer* Requester, EPCClass ClassToSpawn);

	/** Spawns remote player's tag character, without possess it by player controller or destroy current one.
	 * Most likely for cinematic purpose. */
	ABladeIIPlayer* SpawnRemoteTagAsPuppet(bool bPossessByAIController = true);

	void RestartRemotePlayer(FString const& TagName);
	void SwitchOpponent(ABladeIINetPlayer* opponent);

	void InitializeUpdateUIDoc();
	virtual void RealtimeUpdateUIDoc() override;

	void ReturnToFindAccount();
	void RequestRematch();
	void ReturnToPVPMainMenu();
	void OpenPopupGotoPVPMainMenu();
	
	void SendRequestMatchResult();

	void SetOpponent(ABladeIINetPlayer* opponent);
	// accessors
	ABladeIINetPlayer* GetOpponent() const
	{
		return Opponent;
	}

	ABladeIINetPlayer* GetLocalPlayer() const;

	bool IsPvPTutorial() const { return bIsTutorial; }
	float GetDebuffValue() const { return TutorialDebuffValue; }

	FORCEINLINE bool IsLeftLocalPlayer() { return bIsLeftLocalPlayer; }

	void EndMatchByDie();
	void EndMatchByBattleTime();
	void SetStartTimeCount(bool InStart);
	void SetMatchResult(int32 MyResult);

	void StartPVPGame();

	void HandleResponseMatchResult(FB2TagMatchResult Result);
	void HandleResponseCancelMatch();
	void HandleSyncReadyToStartGame(uint32 InNetId);
	void HandleSyncStartGame();
	void NotifyInGameReady(uint32 InNetId, const uint8 ReceiverType);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void SetMatchLobbyResources(
	//	class ALevelSequenceActor * InMatinee,
	//	const FMatchingAnims& Gladiator,
	//	const FMatchingAnims& Assassin,
	//	const FMatchingAnims& Wizard,
	//	const FMatchingAnims& Fighter
	//);
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void StartMatchResult();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetMatchLobbyCharacterLocationZ(float GladiatorZ, float AssassinZ, float WizardZ, float FighterZ);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetMatchLobbyLevel(ULevelStreaming* PackageToLoad);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetMatchBattleLevel(ULevelStreaming* PackageToLoad);

	void PlayPVPBGM(EPVPBGMPlayContext InContext, bool bForceStopCurrent = true);
	void SetOpponentToken(const FGuid& Token);

	virtual void StartEventScene_MatchResult(bool HasLocalPlayerWon) override;
	virtual void OnQuestCompleteNotify(int32 QuestSlot) {}
	
	void DelayedSetupPreRender();


	virtual bool IsApplyForceTinyDamage() { return false; }

	void NotifyStageEventDirector_FinishShow(int32 EventType);

	bool IsValidFriendshipGame();

	virtual bool CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const override;
	virtual void ClearInvitationGameInfo(EB2GameMode GameModeType) override;


	virtual void RequestGameInvite(const FString& NaickName, EB2GameMode GameModeType) override;
	virtual void PrevAcceptGameInvite(const FString& Inviter, EB2GameMode GameModeType) override;
	virtual void RequestDeclineInvitation(EB2GameMode GameModeType) override;

	virtual void OnReceiveInvitation(const FString& Inviter, EB2GameMode GameModeType);
	virtual void OnInvitationSuccess(const FString& Invitee, EB2GameMode GameModeType, const FString& InvitationRef);
	virtual void OnInvitationDeclined(const FString& Inviter, EB2GameMode GameModeType);



protected:
	virtual void NotifyUIWidgetOpenComplete(FName InOpenedUI) override;
	ENetMatchResult GetMatchResult() const;
	virtual void NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector) override;


private:
	ABladeIIPlayerAIController* SpawnAIController(FVector const& SpawnLocation, FRotator const& SpawnRotation);
	
	void LeaveEnemyInProgress();
	void CheckInGameReady(float DeltaSeconds);
	void MatchGameStartEventSceneEnd();
	void SetupNetwork(ABladeIIPlayer*, ABladeIIPlayer*);
	void SetNetStatus(ABladeIIPlayer*);
	void DeliveryMatchMaking(FB2TagMatchMakingPtr MatchInfo);
	void SettingResultInfo();
	void ResponseGetMatchStatus(const FB2TagMatchStatus& TagMatchStatus);
	void StopPlayers();
	int32 GetMatchType();

	virtual FString GetLocalSavedAutoStateKeyName() const override { return TEXT("AutoState_PVPGameMode"); }

	UPROPERTY(Transient)
	ABladeIINetPlayer*			Opponent;

	//UPROPERTY(Transient)
	//ABladeIIPlayerAIController*	PVPAIController;

	bool						LeaveEnemyBeforeInProgress;
	bool						bIsLeftLocalPlayer;
	bool						bRepeatRequestMatchByError;
	bool                        bIsTutorial;
	float						TutorialDebuffValue;
	float						ErrorHandlingTimeExchangeInfo;
	float						InGameReadyTime;
	int32						InGameReadySendPacketCount;
	FString						OpponentTokenToPlay;
	FTimerHandle				TimeToLeaveEnemy;
	FPVPMatchSceneManager		SceneManager;
	FB2TagMatchResult			ResultInfo;
	EPVPMatchState				HostMatchState;

	TMap<uint32, bool>			ReadyCheck;

	bool						bRequestResult;
};
