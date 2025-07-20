// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UMG.h"
//#include "BladeII.h"
#include "BladeIIGameMode.h"
#include "Channel.h"
#include "B2ModPlayerInfoData.h"
#include "B2NetGameMode.generated.h"

class AActor;
class AController;
class ABladeIINetPlayer;

namespace NetMatchState
{
	// * MatchGameModeBase
	extern BLADEII_API const FName RequestMatchMaking;
	extern BLADEII_API const FName WaitMatchMaking;
	extern BLADEII_API const FName StartResult;
	extern BLADEII_API const FName FinishBattle;
	extern BLADEII_API const FName LobbyMatchToGameMode;
	extern BLADEII_API const FName FinishLobbyMatch;

	// * NetGameMode : Only 1 vs 1 Mode (ex PVPGameMode)
	extern BLADEII_API const FName MatchOtherPlayer;
	extern BLADEII_API const FName ExchangeInfos;
	extern BLADEII_API const FName DoneToExchange;
	extern BLADEII_API const FName LeaveEnemyInBattle;

	// * NetGameMode : Only n vs n Mode (ex RaidGameMode, ControlGameMode)
	extern BLADEII_API const FName WaitToMeetCondition;
	extern BLADEII_API const FName WaitHostReady;
	extern BLADEII_API const FName PreparePlayers;
}

static const uint32 PEER_SUPER			=	1;
static const uint32 PEER_RESTRICTED		=	2;
static const uint32 PEER_NETSTANDALONE	=	3; //沥惑利栏肺 措傈捞 捞风绢脸瘤父 惑措啊 栋唱辑 AI客 措搬窍绰 惑怕
static const uint32 PEER_STANDALONE		=	4; //矫累何磐 AI客 措傈
static const uint32 PEER_NONE			=	5;

USTRUCT()
struct FReservedDecodePacketInfo
{
	GENERATED_BODY()

	uint32 sender;
	uint32 receiver;
	FString context;
};


/*
	AB2NetGameModeBase : Match
*/
UCLASS(ABSTRACT)
class BLADEII_API AB2MatchGameModeBase : public ABladeIIGameMode
{
	GENERATED_BODY()

public:
	AB2MatchGameModeBase(const FObjectInitializer& ObjectInitializer);

	// AActor interfaces...
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
	virtual void Tick(float) override;

	// BladeIIGameMode interfaces...
	virtual void SubscribeEvents() override; // Events of EventSubsystem

	// GameMode interfaces...
	virtual bool HasMatchStarted() const override;
	virtual void SetMatchState(FName NewState) override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;

	// NetGameMode interfaces...
	virtual void HandleRequestMatchMaking();
	virtual void HandleMatchOtherPlayer();
	virtual void HandleExchangeInfos();
	virtual void HandleDoneToExchange();
	virtual void HandleLobbyMatchToGameMode();
	virtual void HandleWaitToMeetCondition();
	virtual void HandlePreparePlayers();
	virtual void HandleFinishLobbyMatch();
	virtual void HandleLeaveEnemy();
	virtual void HandleStartResult();
	virtual void HandleFinishBattle();
	virtual void HandleNotifyUpdateRoomProperty(const TArray<b2network::B2RoomPropertyPtr>& PropertyList);
	virtual void HandleNotifyResult(ENetMatchResult, uint8);
	virtual void HandleNotifyLeave(uint32);
	virtual void HandleNotifyRoomLeave();
	virtual void HandleNotifyRoomClose(const FString& RoomToken, int32 RoomType);
	virtual void HandleSuccessToMatchmake(data_trader::ChannelPtr ch, uint32 peer_role, const TArray<FB2RoomParticipantPtr>& participants);
	virtual void LeaveRoomIfJoined();
	virtual void ResponseChangeRoomAccess(uint8 AccessKey);
	virtual void PrepareNetGame(uint32);
	virtual void PrepareMyPlayer(uint32);
	virtual void PrepareSlaveNetGame(uint32);
	virtual APawn* RestartNetPlayer(class AController*, FString const&);
	virtual void SetRoomParticipant(const TArray<FB2RoomParticipantPtr>&) {}
	virtual void OnHostChanged(int32, int32);
	virtual void SetMyPlayerInfoData(uint32, const FB2FindAccountInfoPtr&);
	virtual void SetRemotePlayerInfoData(uint32, int64, const FB2FindAccountInfoPtr&);
	virtual void PostRemotePlayerInfoData();
	virtual void RemoveRemotePlayer(uint32 NetId);
	virtual FB2ModPlayerInfo& GetRemotePlayerInfoData(uint32);
	virtual class ICharacterDataStore* GetRemotePlayerDataStore(uint32 RemotePlayerIndex) override;

	virtual void ResponseSendRoomPlayerClass(TArray<FB2NetMatchPeerInfo> const& RoomPeerInfo);
	virtual void SetRemotePlayerClass(uint32, EPCClass);
	virtual void ResponsePrerequisiteToStart(int32 RoomType, const FString& RoomToken, TArray<FB2NetMatchPeerInfo> const&);
	virtual void ResponseAckPrerequisiteToStart(uint32 NetId);	

	virtual ERoomCloseReasonType GetStartRoomCloseReason();
	virtual int32 GetMatchModeMapId();
	virtual int32 GetMatchModeDifficulty();

	virtual bool CanRequestRoomClose();
	virtual bool CanRequestRoomCloseReason(ERoomCloseReasonType RoomCloseReason);
	virtual bool CanStartInGame();
	
	virtual void OnPlatFormPauseCase();
	virtual void	ClearMatchRoom();
	
	virtual void CheckFullPeerRoom();
	virtual void SendRequestRoomClose(ERoomCloseReasonType RoomCloseReason, int32 MapId, int32 Difficulty);
	void SetPreparePlayers(bool bInAlreadyStarted = false);
	
	TMap<uint32, FB2ModPlayerInfo>&	GetRemotePlayerInfoDatas();
	void	ShutdownChannelIfStillExist();
	void	SendGameMessage(FString const&);
	void	SendMessageToHost(FString const&);
	void	ChangeRoomPublicAccess();	// 规阑 傍俺规栏肺 函版秦霖促.

	void	ReserveDecodePacket(uint32 sender, uint32 receiver, FString const& context);
	void	ApplyReservedDecodePacket();
	
	virtual void ReceiveAnimStateIndex(uint32 RemoteNetId, FNetSyncAnimInfo& NetSyncAnimInfo) {};

// Invitation Interface
	virtual void RequestGameInvite(const FString& NaickName, EB2GameMode GameModeType);
	virtual void PrevAcceptGameInvite(const FString& Inviter, EB2GameMode GameModeType) {}

	void	SetGameInvitationInfo(int32 InvitationType, const FString& Inviter, const FString& InvitationRef, b2network::B2RoomLocationPtr RoomInfoPtr);
	void	OnNotifyAcceptInvitation(int32 InvitationType, const FString& Inviter, const FString& InvitationRef, b2network::B2RoomLocationPtr RoomInfoPtr);
	void	OnNotifyDeclineInviation(int32 InvitationType, const FString& Invitee, const FString& InvitationRef);

	virtual void RequestAcceptInvitation(EB2GameMode GameModeType);
	virtual void RequestDeclineInvitation(EB2GameMode GameModeType);

	virtual bool CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const;
	virtual bool HasGameInvitation(EB2GameMode GameModeType);

	virtual void OnDeliveryRoomJoin(FB2ResponseRoomJoinPtr Message);

	virtual void OnReceiveInvitation(const FString& Inviter, EB2GameMode GameModeType) {}
	virtual void OnInvitationSuccess(const FString& Invitee, EB2GameMode GameModeType, const FString& InvitationRef) {}
	virtual void OnInvitationDeclined(const FString& Inviter, EB2GameMode GameModeType) {}

	virtual void ClearInvitationGameInfo(EB2GameMode GameModeType);

// accessors...
	bool	IsSuccessRoomMatching();

	const FString&	GetRoomId() const
	{
		return RoomWhereIAm;
	}

	void SetRoomId(FString const& Id)
	{
		RoomWhereIAm = Id;
	}

	uint32 GetMyNetId() const
	{
		return MyNetId;
	}

	void SetMyNetId(uint32 Id)
	{
		MyNetId = Id;
	}

	void SetPeerType(uint32 type)
	{
		PeerType = type;
	}

	uint32 GetPeerType() const
	{
		return PeerType;
	}

	void SetHostId(uint32 id)
	{
		HostId = id;
	}

	uint32 GetHostId() const
	{
		return HostId;
	}

	bool IsHost() const
	{
		return GetPeerType() == PEER_SUPER;
	}

	void SetToken(const FString& Token);

	FString GetToken()
	{
		return TokenToPlay;
	}

	bool IsAlreadyStarted()
	{
		return bAlreadyStarted;
	}

protected:
	FString								TokenToPlay;
	FString								RoomWhereIAm;
	TMap<uint32, FB2ModPlayerInfo>		RemotePlayerInfoData;
	TArray<FB2NetMatchPeerInfo>			ReservedPlayerClasses;

private:
	uint32	MyNetId;
	uint32	HostId;
	uint32	PeerType;
	bool	bAlreadyStarted;

	TArray<FReservedDecodePacketInfo> ReservedDecodePacketInfos;
};

/**
 * 
 */
UCLASS(ABSTRACT)
class BLADEII_API AB2NetGameMode : public AB2MatchGameModeBase
{
	GENERATED_BODY()
	
public:
	AB2NetGameMode(const FObjectInitializer& ObjectInitializer);

	// AActor interfaces...
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
	virtual void Tick(float) override;

	// BladeIIGameMode interfaces...
	virtual void SubscribeEvents() override; // Events of EventSubsystem
	virtual void SetDesiredPCClassEnum() override;
	virtual void NotifyPlayerDeadEnd(class ABladeIIPlayer*) override;
	virtual void RealtimeUpdateUIDoc() override;
	virtual void PreloadLocalSkillAnimation() {}

	// GameMode interfaces...
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HookOnSpawnDefaultPawn(APawn*) override;

	virtual bool IsNetGameMode() const { return true; }

	// MatchGameMode interfaces...
	virtual void HandleMatchOtherPlayer() override;
	virtual void HandleLeaveEnemy() override;
	virtual void HandleStartResult() override;
	virtual void HandleFinishBattle() override;

	virtual void RemotePlayerResurrect(uint32, FVector, FRotator, TMap<EResurrectBuffType, uint8>&);
	virtual void PrepareNetGame(uint32) override;
	virtual void PrepareMyPlayer(uint32) override;
	virtual void SpawnRemotePlayer(uint32, EPCClass MainClass, EPCClass SubClass);
	virtual void PrepareSlaveNetGame(uint32) override;
	virtual APawn* RestartNetPlayer(class AController*, FString const&) override;
	//virtual UB2SkillRTManager* GetRemoteSkillRTManager(uint32);
	virtual void HandleCharacterDead(ABladeIICharacter* DeadGuy, uint32 NetId, float KillingDamage);

	virtual void PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie) override;

	static TArray<EPCClass> GetAllNetGameMatchedPCClasses();
protected:
	virtual TArray<EPCClass> GetPCClassesToPreLoad() override;
	//virtual TArray<FCombinedPCSkillAnimID> GetPCSkillAnimsToPreLoad() override;

	//virtual TArray<FCombinedPCSkillAnimID> GetLocalPCSkillAnims();
	//virtual TArray<FCombinedPCSkillAnimID> GetOpponentPCSkillAnims(int32 OpponentNetID);
	//virtual void GetEquippedPlayerSkillAnims(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutSkillCombIDs);

public:
	virtual void SendStartGamePacket();

	virtual void CheckNetAIPlayerResurrect(class ABladeIINetPlayer* pPlayer) {};

	// NetGameMode interfaces...
//	virtual void HandleTakeDamage(AActor* , class AController*, class AActor*);
//	virtual void HandleHealingHealth(AActor*); //老窜 TakeDamage俊 角绢 焊郴磊. 肋 救登搁 Packet阑 父甸绢辑 蝶肺 焊郴磊
	virtual void HandleHealth(uint32,float,uint32);
	virtual bool CanDie() const;
	virtual bool CanTakeDamage();
	virtual void SendResurrectHelper(const ABladeIINetPlayer* player);
public:
	virtual void ForceResurrectLocalPlayer();
	void	ForceResurrectAIPlayer(class ABladeIINetPlayer* pPlayer);

	ABladeIIPlayer* SpawnRemotePlayer(class UWorld* SpawnWorld, EPCClass InClass, const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);

	AActor* MyFindPlayerStart(const FString& IncomingName);
	uint32	FindNetIdByDamageCauser(AController*, AActor*, AActor*);
	APawn*	FindPawnByNetId(uint32);

	void SendChannelHeartbeat(float);

	void SetLocalController(APlayerController* InController);
	APlayerController* GetLocalController() const { return LocalController; }
	int32 GetLocalPlayerTeamNum() const;
	
	void SetOpponentAccountId(int64 InAccountId)
	{
		OpponentAccountId = InAccountId;
	}

	virtual void ReceiveAnimStateIndex(uint32 RemoteNetId, FNetSyncAnimInfo& NetSyncAnimInfo) override;

	TArray<FNetSyncAnimInfo> GetSyncedAnimStateIndexInfo(uint32 RemoteNetId);

protected:
	UPROPERTY(Transient)
	TSubclassOf<class APawn>			DefaultRemotePawnClass;

	//UPROPERTY(Transient)
	//TMap<uint32, UB2SkillRTManager*>	RemoteSkillRTManagers;

	int64								OpponentAccountId;

private:
	UPROPERTY(Transient)
	APlayerController*					LocalController;

	float								HeartbeatTick;

	// NetId喊肺 付瘤阜俊 罐篮 SyncedAnimStateIndex历厘
	// 固胶迄 纳腐俊 措茄 郴侩老版快 措厚. ABladeIINetPlayer::BeginPlay()俊辑 夯牢酒捞叼波 爱促靖
	TMap<uint32, TArray<FNetSyncAnimInfo>> SyncedAnimStateIndexByNetId;
};

