#pragma once
//#include "BladeII.h"
#include "B2NetGameMode.h"
#include "../DataStore/B2ClientDataStore.h"
#include "B2RaidGameMode.generated.h"

class UB2NPCClassInfoBox;
class ABladeIINetCharacter;
class AMatineeActor;

enum class ERaidMatchSceneState : uint8
{
	None,
	Loading,		 // 로딩중
	WaitMatchPlayer, // 매칭 준비중
	PreparePlayers,	 // 플레이어 준비중
	Battle,			 // 게임 시작
	EndMatchByTime,	 // 게임 시간 종료
	BattleEnd // 게임종료
};

enum class ERaidBGMPlayContext : uint8
{
	BGMCTX_Standby,
	BGMCTX_Combat,
	BGMCTX_Victory,
	BGMCTX_Defeat,
	BGMCTX_SargonCombat,
	BGMCTX_SargonPhaseChange,
	BGMCTX_SargonDie,
	BGMCTX_End
};

enum class ERaidPlayerLiveState : uint8
{
	All,
	LiveOnly,
	DefeatOnly,
};

class FRaidMatchSceneManager
{
public:
	//180122_YJ 180초에서 300초로 재 수정.
	static const int32 BATTLE_TIME = 300;
	static const int32 FAIL_COUNTDOWN = 5;
	static const int32 CHECK_READY_TIME = 5;
	static const int32 MAX_CHECKREADY_TRY_COUNT = 10;
	static const int32 FORCE_HOST_START = 10;

public:
	FRaidMatchSceneManager();

	ERaidMatchSceneState GetState() { return CurrentState; }
	void SetGameMode(class AB2RaidGameMode* InGameMode);
	void SetState(ERaidMatchSceneState InState);
	void Tick(float DeltaSeconds);
	bool IsHost();
	void OnHostChanged(int32 RoomType, int32 NewHostId);
	void StartFailCountDown();
	void StopFailCountDown();

	void StartCheckAllPeerReady(bool IsClearTryCount = false);
	void SetBattleBeginTime(int64 time);
	void SendStartTimeByHost();
private:
	void SetLoading();
	void SetWaitMatchPlayer();
	void SetPreparePlayers();
	void SetBattle();
	void SetEndMatchByTime();
private:
	ERaidMatchSceneState CurrentState;
	class AB2RaidGameMode* GameMode;

	uint8	CheckAllLoadingPeerTryCount;
	double	CheckAllPeerReadyBeginPlayTime;
	double	CheckAllPeerReadyLimitTime;
	double	CheckAllPeerReadyLeftTime;

	int64	BattleBeginTime;
	int32	BattleLimitTime;
	int32	BattleLeftTime;

	double	FailCountBeginPlayTime;
	double	FailCountLimitTime;
	double	FailCountLeftTime;
};

USTRUCT()
struct FRaidPeerInfo
{
	GENERATED_USTRUCT_BODY()

	FRaidPeerInfo() 
		: IsConnectedMap(false)
		, IsFinishReady(false)
		, PlayerStartIdx(0)
		, AttackDamage(0)
		, IsExitMainGame(false)
		, IsDead(false)
		, HelpResurrectionTime(0)
		, ReservedPCClass(EPCClass::EPC_End)
		, PeerPlayer(nullptr)
	{}

	bool IsConnectedMap;
	bool IsFinishReady;
	uint8 PlayerStartIdx;
	int32 AttackDamage;
	bool IsExitMainGame;
	bool IsDead;
	float HelpResurrectionTime;
	EPCClass ReservedPCClass;	// 선택된 캐릭터를 동기화 (비동기로 처리되기때문에 필요)

	UPROPERTY(Transient)
	ABladeIINetPlayer* PeerPlayer;
};

class RaidGameRule : public DefaultGameRule
{
public:
	virtual bool AllowToTag() const override 
	{ 
		return false; 
	}

	virtual bool IsQTEEnabled() const override 
	{ 
		return true; 
	} 
	// QTE Break is for RAID.. God save us..

	virtual FString GetBehaviorTreeAsset() const override
	{ 
		return FString(TEXT("/Game/Blueprints/AI/PCAutoMeleeBT.PCAutoMeleeBT")); 
	};

	virtual bool UseSectionMergedPCModel() const override
	{
		return true; // Performance rule
	}

	virtual bool AllowOtherPlayerDamageNumNotify() const override
	{
		return false; 
	}

	virtual bool IsGlobalTimeDilationEnabled() const override 
	{ 
		return false; 
	}

	virtual bool IsHitFreezeEnabled() const override
	{
		return false;
	}

};

UCLASS()
class BLADEII_API AB2RaidGameMode : public AB2NetGameMode
{
	GENERATED_BODY()

public:
	static const uint8		MAX_PLAYERS = 4;
	static const uint8		MAX_RESURRECTION_COUNT = 3;

	static const int32		ZERO_REMOTE_PLAYER = 7404;
	static const int32		CHANNEL_DISCONNECTED = 8003; //7410;

	AB2RaidGameMode(const FObjectInitializer& ObjectInitializer);

	// Actor interfaces...
	virtual void BeginPlay() final;
	virtual void BeginDestroy() final;
	virtual void Destroyed() final;
	virtual void Tick(float) final;
	virtual void GoToVillage() final;

	// BladeIIGameMode interfaces...
	virtual void PreBeginPlay()			final;
	virtual void SubscribeEvents()		final;
	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;
	virtual void SetupUIScene() override;
	virtual void NotifyPlayerDeadEnd(ABladeIIPlayer*) final;
	virtual void NotifyCharacterDead(ABladeIICharacter*, APawn*,AActor*) final;
	virtual void NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool) override;
	virtual float GetCurrentStageDifficultyScale() override;

	//virtual ABladeIICharacter* SpawnCharacter(UB2NPCClassInfoBox*, ENPCClass, ENPCClassVariation, FTransform&, FActorSpawnParameters&) override;
	virtual ACharacter* GetNearestPlayerCharacter(FVector, bool = false) override;
	virtual void GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum = INDEX_NONE, bool bOnlyLiveMobs = true) final;
	virtual void GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /* = true */) final;
	virtual void GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLive = true) final;
	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::Raid; }
	virtual EB2SkillLODType GetSkillLODType() final { return EB2SkillLODType::ESLT_ONLY_FOR_SELF; }
	virtual float GetTotalAttack(ICharacterDataStore*) final;
	virtual float GetTotalDefense(ICharacterDataStore*) final;
	virtual bool CanTakeDamage() override;

	virtual float GetModifiedBossmobMaxHealth() override;

	virtual void GatherEquippedSkillAnimIDs(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutEquippedSkills) override;

	virtual void NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector);

	// NetGameMode interfaces...
	virtual bool HasMatchStarted() const final;
	virtual void SetMatchState(FName NewState) final;
	virtual void HandleRequestMatchMaking() final;
	virtual void HandleMatchHasStarted() final;
	virtual void HandleStartResult() override;
	virtual void HandleNotifyUpdateRoomProperty(const TArray<b2network::B2RoomPropertyPtr>& PropertyList) override;
	virtual void HandleNotifyResult(ENetMatchResult, uint8) override;
	virtual void HandleNotifyLeave(uint32) final;
	virtual void HandleHealth(uint32, float, uint32) override;	
	virtual void SetMyPlayerInfoData(uint32, const FB2FindAccountInfoPtr&) final;
	virtual void SetRemotePlayerInfoData(uint32, int64, const FB2FindAccountInfoPtr&) final;
	virtual void SetRoomParticipant(const TArray<FB2RoomParticipantPtr>&) final;
	virtual void StartEventScene_MatchResult(bool HasLocalPlayerWon) override;
	virtual void SetResurrectPlayerPosition(ABladeIIPlayer* pBladePlayer = NULL) final;
	virtual bool RequestResurrect(EResurrectBuffType InResurrectBuffType) final;
	virtual void RemotePlayerResurrect(uint32, FVector, FRotator, TMap<EResurrectBuffType, uint8>&) final;
	virtual void LeaveRoomIfJoined() final;
	virtual void HandleNotifyRoomLeave() final;
	virtual void HandleNotifyRoomClose(const FString& RoomToken, int32 RoomType) final;
	virtual bool CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const override;


	// RaidGameMode interfaces...
	virtual void OnHostChanged(int32, int32) final;
	virtual void ResponseChangeRoomAccess(uint8 AccessKey) final;
	virtual void HandleLobbyMatchToGameMode() final;
	virtual void HandleWaitToMeetCondition() final;
	virtual void HandlePreparePlayers() final;
	virtual void SetRemotePlayerClass(uint32, EPCClass) final;
	virtual void ResponsePrerequisiteToStart(int32 RoomType, const FString& RoomToken, TArray<FB2NetMatchPeerInfo> const& NetMatchPeerInfos) final;
	virtual void ResponseAckPrerequisiteToStart(uint32 NetId) final;
	virtual bool CanStartInGame() final;
	virtual EResurrectGameModeType GetResurrectGameModeType() final { return EResurrectGameModeType::EResurrectType_Raid; }
	virtual void SendResurrectHelper(const ABladeIINetPlayer* player) final;

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void SetMatchLobbyResources(class AMatineeActor* IsSuccessMatinee, class AMatineeActor* InFailMatinee, AActor* ResultActor01, AActor* ResultActor02, AActor* ResultActor03, AActor* ResultActor04);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnMatineeEnd_StartEvent();

	void NotifyTargetDamaged(class ABladeIICharacter* InNotifyOwner, int32 SuccessiveComboNum, float DamageAmount); // SuccessiveComboNum or SuccessiveHit
	void NotifyStageEventDirector_FinishShow(int32 EventType);

	void HandleResponseRaidResult(const FB2ResponseRaidResultPtr& RaidResultReward);
	void HandleResponseRaidResultInvenFull();
	void HandleNotifyRaidResult(bool IsSuccess, uint32 BonusPlayer, const TArray<TPair<int32, int32>> &Contributions);
	void HandleBornAgainChangedPlayer(int32 InNetId, ABladeIIPlayer* oldPlayer, ABladeIIPlayer* newPlayer);
	void HandleSyncReadyToStartGame(uint32 InNetId);	
	void HandleNotifyRaidTimeEnd(uint32 InNetId);

	void CheckAllPeerReady(int32 CurCheckCount);
	void SendSyncReadyToGame();	
	void ForceStartGame();	
	void PrepareHostPlayer();
	void PrepareRemotePlayer(uint32);
	void RestartRemotePlayer(uint32);
	void IncreasePeerPlayers(uint32);
	void SetEndMatchByTime();	
	void PlayRaidBGM(ERaidBGMPlayContext InContext, bool bForceStopCurrent = true);
	void BossHealthAndDamage(uint32 _PlayerIDX, float _Health, uint32 _DamageCauserID);
	void RevalueContribution();
	void SetAddFriendButtonForRaidResult();
	void ReturnToLobby();
	void ReturnToRaidMainMenu();
	bool SetupEditorRaidDefaultInfo();
	void SetupEditorRaidPreviewRemoteDead();

	void SendLeavePlayer(uint32 NetId);

	void NetReceiveQTEEnable(uint32 NetId, uint8 TeamNum);

	bool IsLoadingState();

	void SyncUserBattleStartTime();
	void BattleStartTimeCheck();
	void EndTimePlayerBossStop();

	void SendNotifyTimeEnd();

	bool IsStandaloneGame();
	bool IsForceStartMatch();
	void ClearPartyRetryInfo();

public:
	ABladeIIPlayer*		GetRaidPlayer(int32 BattlePlayerIndex, ERaidPlayerLiveState LiveState = ERaidPlayerLiveState::All, int32* outNetId = nullptr);
	int32				GetClientStageId();
	int32				GetRaidStep();
	void				OnFinishRaidFailCountDown();
	bool				IsAllPlayerDead();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartMatchResult();
protected:
	virtual void		NotifyUIWidgetOpenComplete(FName InOpenedUI) override;

	void				InitializeUpdateUIDoc();
	virtual void		RealtimeUpdateUIDoc() override;	

	void				OnChannelDisconnectedEvent();

	void				EmplacePeerPlayer(int32 InNetId);
	void				EmplacePeerPlayer(int32 InNetId, ABladeIINetPlayer* NetPlayer);
	void				EmplacePeerPlayer(int32 InNetId, EPCClass PCClass);
	void				RemovePeerPlayer(int32 InNetId);
	void				SetStartResultState(bool IsSuccess);
	int32				GetMatchPlayerNum();
	int32				GetMaxMatchPlayerNum();
	bool				IsMatchMaxPlayerNum();
	void				GetDelayConnectPeerList(TArray<uint32>& outPeerList);
	void				GetDelayReadyPeerList(TArray<uint32>& outPeerList);
	void				RemoveDelayPeerList(const TArray<uint32>& RemovePeerList);
	void				RemoveAllButRemainPeerList(const TArray<uint32>& RemainPeerList);
	void				SetPlayerDead(const uint32 NetId, bool IsDead);
	int32				GetMyJoinSlotIndex();
	int32				GetJoinSlotIndex(const int32 NetId);
	uint8				GetNextPlayStartIdx(bool IsMyStartIdx);
	int32				GetPeerNetId(ABladeIICharacter* B2Character);
	int32				GetTotalAttackDamage();
	int32				GetAttackDamage(int32 NetId);
	uint8				GetContribution(int32 NetId, int32 PeerTotalAttackDamage = 0);
	void				SetResultRaidContribution();
	int32				GetRankingNetID(int32 Rank);
	int32				GetMyRanking();
	int32				GetBonusPlayerNetID(uint32 BonusSeed);
	FVector				GetActorPositionFromMatineeActor(int32 PlayerIndex, bool IsWin);
	void				TryGetContributionList(TArray<TPair<int32, int32>> &ContributionArray);
	virtual FString		GetLocalSavedAutoStateKeyName() const override { return TEXT("AutoState_RaidGameMode"); }
	bool				CheckRemotePlayer();
	void				ErrorReturnToRaidMainMenu(int32 ErrorCode);
	
	void CheckPromoteNetStatus();
	void RestartAllPlayers();
	// Event Tickets
private:
	uint32	ChannelDisconnectedEventTicket;
	uint32	ReturnToRaidMainClassTicket;
	uint32	ReceivedNotifyUpdateRoomPropertyTicket;
	uint32	ReceivedRaidNotifyResultTicket;
	uint32	ReceivedRaidResultTicket;
	uint32	HandleServerError4015InRaidTicket;
	uint32	TargetDamagedTicket;
	uint32	StageEventDirectorFinishShowEventTicket;
	uint32	BornAgainChangedPlayerTicket;
	uint32	ReceivedSyncReadyToStartGameTicket;
	uint32	ReceivedSyncStartGameTicket;
	uint32	ReceivedRaidNotifyTimeEndTicket;

	uint32	ReceiveQteEnableTicket;
private:
	class AMatineeActor* SuccessResultMatinee;
	class AMatineeActor* FailResultMatinee;
	TArray<AActor*> ResultLocationArray;

private:
	FRaidMatchSceneManager	SceneManager;
	uint32					MobNetIdCounter;
	int32					MaxMatchPlayerNum;
	FTimerHandle			BattleTimeSyncTimer;

	UPROPERTY(Transient)
	ABladeIINetCharacter*	RaidBoss;

	TMap<uint32, FRaidPeerInfo>				AllPeerInfos;
	TMap<ABladeIICharacter*, uint32>		CacheOtherPeerNetIds;	
	bool									bWaitingForResult;
	bool									bRequestResult;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	TSubclassOf<class UB2FloatingHPBar> FloatingResurrectionBarWidgetClass;
};
