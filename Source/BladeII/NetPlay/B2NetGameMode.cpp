// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2NetGameMode.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "BladeIINetCharacter.h"
#include "BladeIINetPlayer.h"
#include "Event.h"
#include "Retailer.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "PacketInterpreter.h"
#include "Serializer.h"
#include "ChannelManager.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2StageEventDirector.h"
#include "B2SkillRTManager.h"
#include "B2PCClassInfoBox.h"
#include "BladeIIPlayerAIController.h"
#include "B2LogManager.h"
#include "B2DamageEffectInfo.h"
#include "B2BuffModeEffectInfo.h"
#include "B2NetworkBridge.h"
#include "B2UIDocLobbyMatch.h"
#include "FairyManager.h"
#include "B2GameDefine.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

namespace NetMatchState
{
	// * MatchGameModeBase
	const FName RequestMatchMaking		= FName(TEXT("RequestMatchMaking"));
	const FName WaitMatchMaking			= FName(TEXT("WaitMatchMaking"));
	const FName StartResult				= FName(TEXT("StartResult"));
	const FName FinishBattle			= FName(TEXT("FinishBattle"));	//더이상 전투 할수 없는 상태(결과가 나온 상황)와 결과출력창 사이의 State가 없어서 추가
	const FName LobbyMatchToGameMode	= FName(TEXT("LobbyMatchToGameMode"));
	const FName FinishLobbyMatch = FName(TEXT("FinishLobbyMatch"));

	// * NetGameMode : Only 1 vs 1 Mode (ex PVPGameMode)
	const FName MatchOtherPlayer	= FName(TEXT("MatchOtherPlayer"));
	const FName ExchangeInfos		= FName(TEXT("ExchangeInfos"));
	const FName DoneToExchange		= FName(TEXT("DoneToExchange"));
	const FName LeaveEnemyInBattle = FName(TEXT("LeaveEnemyInBattle"));

	// * NetGameMode : Only n vs n Mode (ex RaidGameMode, ControlGameMode)
	const FName WaitToMeetCondition = FName(TEXT("WaitToMeetCondition"));
	const FName WaitHostReady		= FName(TEXT("WaitHostReady"));
	const FName PreparePlayers		= FName(TEXT("PreparePlayers"));
}

//B2GetLogger(B2NetGameMode)
DEFINE_LOG_CATEGORY_STATIC(LogNetGameMode, Log, All);

AB2MatchGameModeBase::AB2MatchGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// For NetGameMode
	PeerType = PEER_SUPER;

	bAlreadyStarted = false;
}

void AB2MatchGameModeBase::BeginDestroy()
{
	ShutdownChannelIfStillExist();
	RemotePlayerInfoData.Empty();
	ReservedPlayerClasses.Empty();
	Super::BeginDestroy();	
}

void AB2MatchGameModeBase::Destroyed()
{
	ShutdownChannelIfStillExist();
	RemotePlayerInfoData.Empty();
	ReservedPlayerClasses.Empty();
	Super::Destroyed();
}

void AB2MatchGameModeBase::Tick(float delta)
{
	Super::Tick(delta);

	ApplyReservedDecodePacket();
}

bool AB2MatchGameModeBase::HasMatchStarted() const
{
	auto result = GetMatchState() == MatchState::EnteringMap
		|| GetMatchState() == MatchState::WaitingToStart
		|| GetMatchState() == NetMatchState::RequestMatchMaking
		|| GetMatchState() == NetMatchState::WaitMatchMaking
		|| GetMatchState() == NetMatchState::MatchOtherPlayer
		|| GetMatchState() == NetMatchState::ExchangeInfos
		|| GetMatchState() == NetMatchState::DoneToExchange;

	return !result;
}

void AB2MatchGameModeBase::SetMatchState(FName NewState)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SetMatchState"));

	if (MatchState == NewState)
	{
		return;
	}

	//UE_LOG(LogBladeII, Log, TEXT("SetMatchState Prev : %s, Next : %s"), *MatchState.ToString(), *NewState.ToString());

	MatchState = NewState;

	if (MatchState == MatchState::WaitingToStart)
	{
		HandleMatchIsWaitingToStart();
	}
	else if (MatchState == NetMatchState::RequestMatchMaking)
	{
		HandleRequestMatchMaking();
	}
	else if (MatchState == NetMatchState::MatchOtherPlayer)
	{
		HandleMatchOtherPlayer();
	}
	else if (MatchState == NetMatchState::ExchangeInfos)
	{
		HandleExchangeInfos();
	}
	else if (MatchState == NetMatchState::DoneToExchange)
	{
		// after done to communicate each other's info...
		// and then we are ready to start match...
		HandleDoneToExchange();
	}
	else if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == NetMatchState::LeaveEnemyInBattle)
	{
		HandleLeaveEnemy();
	}
	else if (MatchState == NetMatchState::StartResult)
	{
		HandleStartResult();
	}
	else if (MatchState == NetMatchState::FinishBattle)
	{
		HandleFinishBattle();
	}
	else if (MatchState == MatchState::LeavingMap)
	{
		LeaveRoomIfJoined();
	}
	else if (MatchState == NetMatchState::LobbyMatchToGameMode)
	{
		HandleLobbyMatchToGameMode();
	}
	else if (MatchState == NetMatchState::WaitToMeetCondition)
	{
		HandleWaitToMeetCondition();
	}
	else if (MatchState == NetMatchState::PreparePlayers)
	{
		HandlePreparePlayers();
	}
	else if (MatchState == NetMatchState::FinishLobbyMatch)
	{
		HandleFinishLobbyMatch();
	}
}

void AB2MatchGameModeBase::HandleMatchIsWaitingToStart()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleMatchIsWaitingToStart"));

	Super::HandleMatchIsWaitingToStart();
}

void AB2MatchGameModeBase::HandleMatchHasStarted()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleMatchHasStarted"));

	Super::HandleMatchHasStarted();
}

void AB2MatchGameModeBase::HandleLeaveEnemy()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleLeaveEnemy"));
}

void AB2MatchGameModeBase::HandleNotifyUpdateRoomProperty(const TArray<b2network::B2RoomPropertyPtr>& PropertyList)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleNotifyUpdateRoomProperty"));
}

ICharacterDataStore* AB2MatchGameModeBase::GetRemotePlayerDataStore(uint32 RemotePlayerIndex)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::GetRemotePlayerDataStore"));

	//return &GetRemotePlayerInfoData(RemotePlayerIndex);
	return nullptr;
}

void AB2MatchGameModeBase::ResponseSendRoomPlayerClass(TArray<FB2NetMatchPeerInfo> const& PeerInfos)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::ResponseSendRoomPlayerClass"));	
	
	// 룸이 셋팅된 다음에 해당 패킷이 전달되어야 정상동작이지만..
	// 경우(디바이스 상황 등등)에 따라서 룸이 셋팅되기전에 호출되는 경우도 발생하기에 예외처리
	if (IsSuccessRoomMatching() == false)
	{
		ReservedPlayerClasses = PeerInfos;
		return;
	}

	// 기존 유저들 동기화
	for (auto& PeerInfo : PeerInfos)
	{
		if (PeerInfo.NetId != GetMyNetId())
			SetRemotePlayerClass(PeerInfo.NetId, IntToPCClass(PeerInfo.Class));
	}

	// 파티원들에게 내 PCClass 동기화
	//packet::ByteStream	payload;
	//payload << GetMyNetId() << BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
	//FString encoded_string = packet::FinalizePacket(packet::ACK_SEND_ROOM_PLAYERCLASS, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
}

void AB2MatchGameModeBase::SetRemotePlayerClass(uint32 InNetId, EPCClass InPCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::ResponseSendRoomPlayerClass"));

	if (RemotePlayerInfoData.Contains(InNetId))
		RemotePlayerInfoData[InNetId].SetMainPlayerClass(InPCClass);
}

void AB2MatchGameModeBase::ResponsePrerequisiteToStart(int32 RoomType, const FString& RoomToken, TArray<FB2NetMatchPeerInfo> const& NetMatchPeerInfos)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::ResponsePrerequisiteToStart"));
	BII_CHECK(IsSuccessRoomMatching());

	if (GetPeerType() == PEER_SUPER)
		return;

	// 룸 클로즈하면서 토큰 재설정은 레이드에서만.
	if(RoomType == b2network::B2RoomType::BRT_Raid)
		SetToken(RoomToken);

	for (auto& PeerInfo : NetMatchPeerInfos)
	{
		if (PeerInfo.NetId != GetMyNetId())
		{
			auto has = RemotePlayerInfoData.Contains(PeerInfo.NetId);
			
			ensure(RemotePlayerInfoData.Contains(PeerInfo.NetId));
			if (RemotePlayerInfoData.Contains(PeerInfo.NetId))
				RemotePlayerInfoData[PeerInfo.NetId].SetMainPlayerClass(IntToPCClass(PeerInfo.Class));
		}
	}

	ReservedPlayerClasses = NetMatchPeerInfos;


	//packet::ByteStream	payload;
	//payload << GetMyNetId();
	//FString encoded_string = packet::FinalizePacket(packet::ACK_SEND_PREREQUISITE_TO_START, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(GetHostId(), encoded_string);
}

void AB2MatchGameModeBase::ResponseAckPrerequisiteToStart(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::ResponseAckPrerequisiteToStart"));

	BII_CHECK(GetPeerType() == PEER_SUPER);

	if (CanStartInGame() == false)
		return;

	//UE_LOG(LogBladeII, Log, TEXT("AB2MatchGameModeBase::ResponseAckPrerequisiteToStart SendMessage PREPARE_TO_START"));

	//// PEER_SUPER일 경우 준비가 다되었으면 서버로 패킷 전송
	//packet::ByteStream	payload;
	//payload << bAlreadyStarted;
	//FString encoded_string = packet::FinalizePacket(packet::PREPARE_TO_START, GetMyNetId(), 0, packet::ALL, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);

	bAlreadyStarted = true;
}

void AB2MatchGameModeBase::SetPreparePlayers(bool bStarted)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SetPreparePlayers"));

	if(GetPeerType() != PEER_SUPER)
		bAlreadyStarted = bStarted;

	SetMatchState(NetMatchState::PreparePlayers);
}

bool AB2MatchGameModeBase::CanStartInGame()
{
	return true;
}

void AB2MatchGameModeBase::OnPlatFormPauseCase()
{
	// 플랫폼 Pause 상태일때 들어오는 함수
	// 이때 일반적인 경우 채널 종료를 시킴.
	ShutdownChannelIfStillExist();
}

void AB2MatchGameModeBase::CheckFullPeerRoom()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::CheckFullPeerRoom"));

	if (MatchState != NetMatchState::PreparePlayers)
	{
		int32 ModeMapId = GetMatchModeMapId();
		int32 ModeDifficulty = GetMatchModeDifficulty();
		ERoomCloseReasonType StartRoomCloseReason = GetStartRoomCloseReason();

		if (CanRequestRoomClose())
			SendRequestRoomClose(StartRoomCloseReason, ModeMapId, ModeDifficulty);
	}
}

bool AB2MatchGameModeBase::CanRequestRoomClose()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::CanRequestRoomClose"));

	return true;
}

ERoomCloseReasonType AB2MatchGameModeBase::GetStartRoomCloseReason()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::GetStartRoomCloseReason"));

	return ERoomCloseReasonType::NONE;
}

int32 AB2MatchGameModeBase::GetMatchModeMapId()
{
	return -1;
}

int32 AB2MatchGameModeBase::GetMatchModeDifficulty()
{
	return -1;
}

bool AB2MatchGameModeBase::CanRequestRoomCloseReason(ERoomCloseReasonType RoomCloseReason)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::CanRequestRoomCloseReason"));
	return (RoomCloseReason != ERoomCloseReasonType::NONE && RoomCloseReason == GetStartRoomCloseReason());
}

void AB2MatchGameModeBase::SubscribeEvents()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SubscribeEvents"));

	if (bEventsSubscribed)
		return;

	Super::SubscribeEvents();

	//Issues.Add(ReceivedNotifyUpdateRoomPropertyClass<const FString&, const TArray<b2network::B2RoomPropertyPtr>&>::GetInstance().Subscribe2(
	//	[this](const FString& RoomId, const TArray<b2network::B2RoomPropertyPtr>& PropertyList)
	//	{
	//		this->HandleNotifyUpdateRoomProperty(PropertyList);
	//	}
	//));
	//Issues.Add(ReceivedNotifyChangeHostClass<int32, int32>::GetInstance().Subscribe2(
	//	[this](int32 room_type, int32 InHostId)
	//	{
	//		this->OnHostChanged(room_type, InHostId);
	//	}
	//));
	//Issues.Add(ReceivedMyNetIdClass<uint32, int64, const FB2FindAccountInfoPtr&>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, int64 AccountId, const FB2FindAccountInfoPtr& AccountInfo)
	//	{
	//		this->SetMyPlayerInfoData(NetId, AccountInfo);
	//	}
	//));
	//Issues.Add(ReceivedOtherNetIdClass<uint32, int64, const FB2FindAccountInfoPtr&>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, int64 AccountId, const FB2FindAccountInfoPtr& AccountInfo)
	//	{
	//		this->SetRemotePlayerInfoData(NetId, AccountId, AccountInfo);
	//		this->PostRemotePlayerInfoData();
	//	}
	//));
	////Issues.Add(SuccessToMatchmakeClass<data_trader::ChannelPtr, uint32, const TArray<FB2RoomParticipantPtr>&>::GetInstance().Subscribe2(
	////	[this](data_trader::ChannelPtr ch, uint32 peer_role, const TArray<FB2RoomParticipantPtr>& participants)
	////	{
	////		this->HandleSuccessToMatchmake(ch, peer_role, participants);
	////	}
	////));
	////Issues.Add(BroadcastRoomMessageClass<uint32, const FString&, bool>::GetInstance().Subscribe2(
	////	[this](uint32 NetId, FString const& inPacket, bool bDelay)
	////	{
	////		if (IsActiveControlTutorial()) 
	////		{
	////			NetId = this->GetMyNetId();
	////		}
	////		
	////		if (bDelay)
	////		{
	////			this->ReserveDecodePacket(NetId, this->GetMyNetId(), inPacket);
	////		}
	////		else
	////		{
	////			packet::DecodePacket(NetId, this->GetMyNetId(), inPacket);
	////		}			
	////	}
	////));
	//Issues.Add(ReceivedNetIdClass<uint32>::GetInstance().Subscribe2(
	//	[this](uint32 NetId)
	//	{
 //           // @ReplayMode FIX
 //           if (data_trader::ChannelManager::GetInstance().IsReplayMode())
 //           {
 //               return;
 //           }

	//		check(NetId != this->GetMyNetId());
	//		this->SetMatchState(NetMatchState::ExchangeInfos);
	//	}
	//));
	//Issues.Add(ReceivedNotifyResultClass<ENetMatchResult, uint8>::GetInstance().Subscribe2(
	//	[this](ENetMatchResult ret, uint8 InState)
	//	{
	//		switch (ret)
	//		{
	//		case ENetMatchResult::LocalWin:
	//			ret = ENetMatchResult::LocalLose;
	//			break;
	//		case ENetMatchResult::LocalLose:
	//			ret = ENetMatchResult::LocalWin;
	//			break;
	//		}
	//		this->HandleNotifyResult(ret, InState);
	//	}
	//));
	//Issues.Add(ReceivedNotifyLeaveClass<uint32>::GetInstance().Subscribe2(
	//	[this](uint32 NetId)
	//	{
	//		this->HandleNotifyLeave(NetId);
	//	}
	//));
	//Issues.Add(DeliveryTokenToTagMatchClass<const FString&>::GetInstance().Subscribe2(
	//	[this](const FString& Token)
	//	{
	//		this->SetToken(Token);
	//	}
	//));	
	//Issues.Add(ReceivedRoomLeaveResponseClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->HandleNotifyRoomLeave();
	//	}
	//));
	//Issues.Add(ReceivedRoomCloseResponseClass<const FString&, int32>::GetInstance().Subscribe2(
	//	[this](const FString& RoomToken, int32 RoomType)
	//	{
	//		this->HandleNotifyRoomClose(RoomToken, RoomType);
	//	}
	//));
	//Issues.Add(ReceivedChangeRoomAccessClass<uint8>::GetInstance().Subscribe2(
	//	[this](uint8 AccessKey)
	//	{
	//		this->ResponseChangeRoomAccess(AccessKey);
	//	}
	//));
	//Issues.Add(HandleServerError901Class<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->HandleRequestMatchMaking();
	//	}
	//));
	//Issues.Add(ReceivedRoomPlayerClassClass<uint32, TArray<FB2NetMatchPeerInfo> const&>::GetInstance().Subscribe2(
	//	[this](uint32 InHostId, TArray<FB2NetMatchPeerInfo> const& PeerInfos)
	//	{
	//		this->SetHostId(InHostId);
	//		this->ResponseSendRoomPlayerClass(PeerInfos);
	//	}
	//));
	//Issues.Add(ReceivedAckRoomPlayerClassClass<uint32, EPCClass>::GetInstance().Subscribe2(
	//	[this](uint32 InNetId, EPCClass PCClass)
	//	{
	//	//if (InNetId != this->GetMyNetId())
	//		this->SetRemotePlayerClass(InNetId, PCClass);
	//	}
	//));
	//Issues.Add(ReceivedPrerequisiteToStartClass<int32, const FString&, TArray<FB2NetMatchPeerInfo> const&>::GetInstance().Subscribe2(
	//	[this](int32 RoomType, const FString& RoomToken, TArray<FB2NetMatchPeerInfo> const& PeerInfos)
	//	{
	//		this->ResponsePrerequisiteToStart(RoomType, RoomToken, PeerInfos);
	//	}
	//));
	//Issues.Add(ReceivedAckPrerequisiteToStartClass<uint32>::GetInstance().Subscribe2(
	//	[this](int32 InNetId)
	//	{
	//		this->ResponseAckPrerequisiteToStart(InNetId);
	//	}
	//));
	//Issues.Add(ReceivedPrepareToStartClass<bool>::GetInstance().Subscribe2(
	//	[this](bool bInAlreadyStarted)
	//	{
	//		this->SetPreparePlayers(bInAlreadyStarted);
	//	}
	//));
	//Issues.Add(data_trader::InvitationNotifyClass<int32, const FString&, const FString&, b2network::B2RoomLocationPtr>::GetInstance().Subscribe2(
	//	[this](int32 InvitationType, const FString& Inviter, const FString& InvitationRef, b2network::B2RoomLocationPtr RoomInfoPtr)
	//{
	//	this->SetGameInvitationInfo(InvitationType, Inviter, InvitationRef, RoomInfoPtr);
	//}
	//));

	//Issues.Add(data_trader::AcceptInvitationNotifyClass<int32, const FString&, const FString&, b2network::B2RoomLocationPtr>::GetInstance().Subscribe2(
	//	[this](int32 InvitationType, const FString& Inviter, const FString& InvitationRef, b2network::B2RoomLocationPtr RoomInfoPtr)
	//{
	//	this->OnNotifyAcceptInvitation(InvitationType, Inviter, InvitationRef, RoomInfoPtr);
	//}
	//));
	//Issues.Add(data_trader::DeclineInvitationNotifyClass<int32, const FString&, const FString&>::GetInstance().Subscribe2(
	//	[this](int32 InvitationType, const FString& Invitee, const FString& InvitationRef)
	//{
	//	this->OnNotifyDeclineInviation(InvitationType, Invitee, InvitationRef);
	//}
	//));
	//Issues.Add(DeliveryRoomJoinClass<FB2ResponseRoomJoinPtr>::GetInstance().Subscribe2(
	//	[this](FB2ResponseRoomJoinPtr Message)
	//{
	//	this->OnDeliveryRoomJoin(Message);
	//}
	//));
	//Issues.Add(ReceivedSyncAnimStateIndexClass<uint32, FNetSyncAnimInfo&>::GetInstance().Subscribe2(
	//	[this](uint32 netId, FNetSyncAnimInfo& NetSyncAnimInfo)
	//{
	//	this->ReceiveAnimStateIndex(netId, NetSyncAnimInfo);
	//}
	//));

	bEventsSubscribed = true;
}

void AB2MatchGameModeBase::HandleRequestMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleRequestMatchMaking"));

	ClearMatchRoom();

	SetMatchState(NetMatchState::WaitMatchMaking);
}

void AB2MatchGameModeBase::HandleMatchOtherPlayer()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleMatchOtherPlayer"));

	// Check Validations...
	BII_CHECK(!RoomWhereIAm.IsEmpty());
	BII_CHECK(GetMyNetId() != 0);

	if (GetPeerType() == PEER_NONE)
		SetPeerType(PEER_RESTRICTED);
}

void AB2MatchGameModeBase::HandleExchangeInfos()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleExchangeInfos"));
}

void AB2MatchGameModeBase::HandleDoneToExchange()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleDoneToExchange"));
}

void AB2MatchGameModeBase::HandleLobbyMatchToGameMode()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleLobbyMatchToGameMode"));
}

void AB2MatchGameModeBase::HandleWaitToMeetCondition()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleWaitToMeetCondition"));
}

void AB2MatchGameModeBase::HandlePreparePlayers()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandlePreparePlayers"));
}

void AB2MatchGameModeBase::HandleFinishLobbyMatch()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleFinishLobbyMatch"));
}

void AB2MatchGameModeBase::HandleStartResult()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleStartResult"));
}

void AB2MatchGameModeBase::HandleFinishBattle()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleFinishBattle"));
}

void AB2MatchGameModeBase::PrepareNetGame(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::PrepareNetGame"));

	// Check Validations...
	BII_CHECK(NetId != GetMyNetId());
	BII_CHECK(!RoomWhereIAm.IsEmpty());
}

APawn* AB2MatchGameModeBase::RestartNetPlayer(class AController* NewPlayer, FString const& TagName)
{
	return NULL;
}

#include "BehaviorTree/BehaviorTree.h"
void AB2MatchGameModeBase::PrepareMyPlayer(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::PrepareMyPlayer"));
}

void AB2MatchGameModeBase::PrepareSlaveNetGame(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::PrepareSlaveNetGame"));

	// check validations...
	BII_CHECK(GetMyNetId() != NetId);
	BII_CHECK(!RoomWhereIAm.IsEmpty());

	UE_LOG(LogNetGameMode, Log, TEXT("Preparing Networking Game Mode..."));
}

void AB2MatchGameModeBase::HandleNotifyResult(ENetMatchResult Ret, uint8 State)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleNotifyResult"));
}

void AB2MatchGameModeBase::HandleNotifyLeave(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleNotifyLeave"));
}

// when you play a kind of net game in editor and force to stop the game.
// game is over before sometimes you haven't got the response came from the server
// but ChannelManager still have the channel and that makes many kinds of problems.
// so in that case you must call this function.
void AB2MatchGameModeBase::ShutdownChannelIfStillExist()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::ShutdownChannelIfStillExist"));

	if (GetRoomId().IsEmpty() || GetPeerType() == PEER_STANDALONE)
		return;

	data_trader::Retailer::GetInstance().RequestLeaveMatch(GetRoomId());

	TArray<FString> Erase;
	Erase.Add(GetRoomId());
	
	//data_trader::ChannelManager::GetInstance().ShutdownChannel(Erase);

	SetRoomId(TEXT(""));
}

void AB2MatchGameModeBase::LeaveRoomIfJoined()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::LeaveRoomIfJoined"));

	if (GetRoomId().IsEmpty() || GetPeerType() == PEER_STANDALONE)
		return;

	data_trader::Retailer::GetInstance().RequestLeaveMatch(GetRoomId());

	// Already request to leave the room. 
	// So clear the room id in which I joined 
	SetRoomId(TEXT(""));
}

void AB2MatchGameModeBase::HandleNotifyRoomLeave()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleNotifyRoomLeave"));
}

void AB2MatchGameModeBase::HandleNotifyRoomClose(const FString& RoomToken, int32 RoomType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleNotifyRoomClose"));
}

void AB2MatchGameModeBase::HandleSuccessToMatchmake(data_trader::ChannelPtr ch, uint32 peer_role, const TArray<FB2RoomParticipantPtr>& participants)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::HandleSuccessToMatchmake"));

	SetPeerType(peer_role);
	SetRoomId(ch->GetIdentify());
	SetRoomParticipant(participants);
}

void AB2MatchGameModeBase::ResponseChangeRoomAccess(uint8 AccessKey)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::ResponseChangeRoomAccess"));
}

void AB2MatchGameModeBase::SendRequestRoomClose(ERoomCloseReasonType RoomCloseReason, int32 MapId, int32 Difficulty)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SendRequestRoomClose"));

	if (CanRequestRoomCloseReason(RoomCloseReason) == false)
		return;

	// 서버로 더 이상 들어오지 못하게하는 패킷을 전송
	data_trader::Retailer::GetInstance().RequestRoomClose(GetRoomId(), MapId, RoomCloseReason);
}

void AB2MatchGameModeBase::SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& AccountInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SetMyPlayerInfoData"));

	SetMyNetId(NetId);

	if (RemotePlayerInfoData.Num() > 0)
	{
		if (RemotePlayerInfoData.Contains(NetId))
			RemotePlayerInfoData.Remove(NetId);
	}
}

void AB2MatchGameModeBase::SetRemotePlayerInfoData(uint32 NetId, int64 AccountId, const FB2FindAccountInfoPtr& OtherPlayerInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SetMyPlayerInfoData"));

	//[@AKI 180102], 이건 Contain 안해도 됨
	RemotePlayerInfoData.Emplace(NetId).SetModPlayerInfo(OtherPlayerInfo);
	RemotePlayerInfoData[NetId].SetAccountId(AccountId);
}

void AB2MatchGameModeBase::PostRemotePlayerInfoData()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::PostRemotePlayerInfoData"));

	// 룸셋팅전에 호스트로부터 패킷을 미리 받았으면 유저들에게 내 정보도 전달
	if (GetPeerType() != PEER_SUPER && ReservedPlayerClasses.Num() > 0)
	{
		ResponseSendRoomPlayerClass(ReservedPlayerClasses);
		ReservedPlayerClasses.Empty();
	}	

	if (MyNetId != 0 && RemotePlayerInfoData.Num() > 0)
	{
		if (RemotePlayerInfoData.Contains(MyNetId))
			RemotePlayerInfoData.Remove(MyNetId);
	}
}

void AB2MatchGameModeBase::RemoveRemotePlayer(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::RemoveRemotePlayer"));

	RemotePlayerInfoData.Remove(NetId);
}

FB2ModPlayerInfo& AB2MatchGameModeBase::GetRemotePlayerInfoData(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::GetRemotePlayerInfoData"));

	bool has = RemotePlayerInfoData.Contains(NetId);
	ensure(has);

	return RemotePlayerInfoData[NetId];
}

TMap<uint32, FB2ModPlayerInfo>& AB2MatchGameModeBase::GetRemotePlayerInfoDatas()
{
	return RemotePlayerInfoData;
}

//void AB2MatchGameModeBase::SendMessage(FString const& message)
//{
//	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SendMessage"));
//
//	//if (GetPeerType() == PEER_NETSTANDALONE || GetPeerType() == PEER_STANDALONE)
//		return;
//
//	//data_trader::ChannelManager::GetInstance().SendMessage(0, message);
//}

void AB2MatchGameModeBase::SendMessage2(FString const& message)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SendMessage"));

	if (GetPeerType() == PEER_NETSTANDALONE || GetPeerType() == PEER_STANDALONE)
		return;

	//data_trader::ChannelManager::GetInstance().SendMessage(0, message);
}

void AB2MatchGameModeBase::SendMessageToHost(FString const& message)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::SendMessage"));

	if (GetPeerType() == PEER_NETSTANDALONE || GetPeerType() == PEER_STANDALONE)
		return;

	//data_trader::ChannelManager::GetInstance().SendMessage(GetHostId(), message);
}


void AB2MatchGameModeBase::ChangeRoomPublicAccess()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::ChangeRoomPublicAccess"));

	if (GetPeerType() != PEER_SUPER)
		return;

	//data_trader::Retailer::GetInstance().RequestRoomAccessControl(GetRoomId(),
	//	data_trader::ChannelManager::B2RoomAccessToPbRoomAccess(data_trader::ChannelManager::B2RoomAccess::Public));

	//packet::ByteStream	payload;
	//payload << (uint8)(data_trader::ChannelManager::B2RoomAccess::Public);
	//FString encoded_string = packet::FinalizePacket(packet::CHANGE_ROOM_ACCESS, GetMyNetId(), 0, packet::ALL, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
}

void AB2MatchGameModeBase::ClearMatchRoom()
{
	TokenToPlay.Empty();
	RoomWhereIAm.Empty();
	RemotePlayerInfoData.Empty();
	ReservedPlayerClasses.Empty();
	MyNetId = 0;
	HostId = 0;
	PeerType = PEER_NONE;
}

void AB2MatchGameModeBase::ReserveDecodePacket(uint32 sender, uint32 receiver, FString const& context)
{
	FReservedDecodePacketInfo ReservedDecodePacketInfo;

	ReservedDecodePacketInfo.sender = sender;
	ReservedDecodePacketInfo.receiver = receiver;
	ReservedDecodePacketInfo.context = context;

	ReservedDecodePacketInfos.Add(ReservedDecodePacketInfo);
}

void AB2MatchGameModeBase::ApplyReservedDecodePacket()
{
	if (ReservedDecodePacketInfos.Num() == 0)
		return;

	// 복사하구 비움. 저 아래 디코드안에서 메세지 보내는 케이스도 있는거같다.
	// 그냥 쓰면 반복문안에서 배열요소 추가될수있음.
	TArray<FReservedDecodePacketInfo> TempReservedDecodePacketInfos = ReservedDecodePacketInfos;
	ReservedDecodePacketInfos.Empty();

	for (FReservedDecodePacketInfo& ReservedDecodePacketInfo : TempReservedDecodePacketInfos)
	{
		//packet::DecodePacket(ReservedDecodePacketInfo.sender,
		//	ReservedDecodePacketInfo.receiver,
		//	ReservedDecodePacketInfo.context);
	}

	
}

void AB2MatchGameModeBase::RequestGameInvite(const FString& NaickName, EB2GameMode GameModeType)
{
	//data_trader::Retailer::GetInstance().RequestInvitation(NaickName, TEXT(""));
	//if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	//	DocLobbyMatch->SetInvitationState(EInvitationState::WaitingAccept);
}

void AB2MatchGameModeBase::SetGameInvitationInfo(int32 InvitationType, const FString& Inviter, const FString& InvitationRef, b2network::B2RoomLocationPtr RoomInfoPtr)
{
	if (CanReceiveGameInvite(Inviter, EB2GameMode::PVP_Tag) == false)
	{
		const FString MyNickName = BladeIIGameImpl::GetLocalCharacterData().GetUserNickName();

		data_trader::Retailer::GetInstance().RequestDeclineInvitation(
			InvitationType,
			Inviter,
			InvitationRef,
			MyNickName);
		
		return;
	}
		

	//if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	//{
	//	const FString MyNickName = BladeIIGameImpl::GetLocalCharacterData().GetUserNickName();
	//	
	//	// 현재 초대 수락대기 중이면 새로 온 초대는 거절처리
	//	const bool HasPrevInvitation = DocLobbyMatch->HasValidInvitation() && DocLobbyMatch->GetInvitationState() != EInvitationState::None;

	//	DocLobbyMatch->SetInvitationInfo(InvitationType, Inviter, MyNickName, InvitationRef, RoomInfoPtr, false);
	//	DocLobbyMatch->SetInvitationState(EInvitationState::WaitingDecision);
	//		
	//	EB2GameMode GameModeType = InvitationTypeToGameMode(InvitationType);
	//	OnReceiveInvitation(Inviter, GameModeType);
	//}
	
}

void AB2MatchGameModeBase::OnNotifyAcceptInvitation(int32 InvitationType, const FString& Invitee, const FString& InvitationRef, b2network::B2RoomLocationPtr RoomInfoPtr)
{
	/*if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{
		const FString MyNickName = BladeIIGameImpl::GetLocalCharacterData().GetUserNickName();
		const bool IsMyInvitation = Invitee != MyNickName;
		
		const FB2InvitationInfo& InvitationInfo = DocLobbyMatch->GetInvitationInfo();
		DocLobbyMatch->SetInvitationInfo(InvitationType, InvitationInfo.Inviter, Invitee, InvitationRef, RoomInfoPtr, IsMyInvitation);
		DocLobbyMatch->SetInvitationState(EInvitationState::Success);
		
		const EB2GameMode GameModeType = InvitationTypeToGameMode(InvitationType);
		OnInvitationSuccess(Invitee, GameModeType, InvitationRef);
	}*/
}

void AB2MatchGameModeBase::OnNotifyDeclineInviation(int32 InvitationType, const FString& Invitee, const FString& InvitationRef)
{
	//if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	//{
	//	const FB2InvitationInfo& InvitationInfo = DocLobbyMatch->GetInvitationInfo();
	//	const bool IsWaitingAcept = DocLobbyMatch->GetInvitationState() == EInvitationState::WaitingAccept;
	//	if(IsWaitingAcept || InvitationInfo.InvitationInfo == InvitationRef)
	//	{
	//		ClearInvitationGameInfo(EB2GameMode::PVP_Tag);
	//		OnInvitationDeclined(Invitee, EB2GameMode::PVP_Tag);
	//	}
	//}
}

void AB2MatchGameModeBase::RequestAcceptInvitation(EB2GameMode GameModeType)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		/*if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		{
			const FB2InvitationInfo& InvitationInfo = DocLobbyMatch->GetInvitationInfo();
			if (InvitationInfo.bMyInvitation == false)
			{
				data_trader::Retailer::GetInstance().RequestAcceptInvitation(
					InvitationInfo.InvitationType,
					InvitationInfo.Inviter,
					InvitationInfo.InvitationInfo,
					InvitationInfo.Invitee,
					InvitationInfo.RoomInfoPtr);
			}
		}*/
	}
	else if (GameModeType == EB2GameMode::Raid)
	{

	}
}

void AB2MatchGameModeBase::RequestDeclineInvitation(EB2GameMode GameModeType)
{
	//if (GameModeType == EB2GameMode::PVP_Tag)
	//{
	//	auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
	//	if (HasGameInvitation(GameModeType) && DocLobbyMatch)
	//	{
	//		const FB2InvitationInfo& InvitationInfo = DocLobbyMatch->GetInvitationInfo();
	//		data_trader::Retailer::GetInstance().RequestDeclineInvitation(
	//			InvitationInfo.InvitationType,
	//			InvitationInfo.Inviter,
	//			InvitationInfo.InvitationInfo,
	//			InvitationInfo.Invitee);

	//		// Request쪽은 Notify가 오지않기때문에 직접 호출해준다
	//		const FString MyNickName = BladeIIGameImpl::GetLocalCharacterData().GetUserNickName();
	//		OnNotifyDeclineInviation(InvitationInfo.InvitationType, MyNickName, InvitationInfo.InvitationInfo);
	//	}
	//}
	//else if (GameModeType == EB2GameMode::Raid)
	//{

	//}

	//ClearInvitationGameInfo(GameModeType);
}

bool AB2MatchGameModeBase::CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const
{
	//if (GameModeType == EB2GameMode::PVP_Tag || GameModeType == EB2GameMode::Raid)
	//{
	//	// 친선전 확인
	//	auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
	//	const bool CanReceivePVP = DocLobbyMatch && DocLobbyMatch->GetInvitationState() == EInvitationState::None;

	//	// 레이드 확인
	//	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	//	const bool CanReceiveRaid = DocRaid && DocRaid->HasRaidInvitationInfo() == false;

	//	// 둘다 초대받지 않은 상태에서만 초대를 받을 수 있음 ex) 레이드 초대중 PVP초대 불가
	//	return CanReceivePVP && CanReceiveRaid;
	//}

	return false;
}

bool AB2MatchGameModeBase::HasGameInvitation(EB2GameMode GameModeType)
{
	//if (GameModeType == EB2GameMode::PVP_Tag)
	//{
	//	auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
	//	return DocLobbyMatch && DocLobbyMatch->HasValidInvitation();
	//}

	//else if (GameModeType == EB2GameMode::Raid)
	//{
	//	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	//	return DocRaid && DocRaid->HasRaidInvitationInfo();
	//}

	return false;
}

void AB2MatchGameModeBase::SetToken(const FString& Token)
{
	TokenToPlay = Token;
}

void AB2MatchGameModeBase::OnDeliveryRoomJoin(FB2ResponseRoomJoinPtr Message)
{

}

void AB2MatchGameModeBase::ClearInvitationGameInfo(EB2GameMode GameModeType)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		//if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		//	DocLobbyMatch->ClearInvitationInfo();
	}

	//else if (GameModeType == EB2GameMode::Raid)
	//{
	//	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
	//		DocRaid->ClearRaidInvitationInfo();
	//}
}

bool AB2MatchGameModeBase::IsSuccessRoomMatching()
{
	// 점령전 튜토리얼에선 룸잘만들어졌다 치자.
	/*if (IsActiveControlTutorial())
	{
		return true;
	}*/

	return (GetRoomId().IsEmpty() == false && GetMyNetId() != 0 && GetPeerType() != PEER_NONE);
}

void AB2MatchGameModeBase::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2MatchGameModeBase::OnHostChanged"));

	if (MatchState == NetMatchState::StartResult || MatchState == MatchState::LeavingMap)
		return;

	SetHostId(NewHostId);

	if (GetMyNetId() == NewHostId)
		SetPeerType(PEER_SUPER);
}

// ============================================================================== AB2NetGameMode ==============================================================================
AB2NetGameMode::AB2NetGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//DefaultPawnClass = ABladeIINetPlayer::StaticClass();
	LocalController	= nullptr;
	HeartbeatTick = 0.0f;
}

void AB2NetGameMode::BeginDestroy()
{
	//RemoteSkillRTManagers.Empty();
	Super::BeginDestroy();
}

void AB2NetGameMode::Destroyed()
{
	//RemoteSkillRTManagers.Empty();
	Super::Destroyed();
}

void AB2NetGameMode::Tick(float Delta)
{
	Super::Tick(Delta);

	SendChannelHeartbeat(Delta);
}

void AB2NetGameMode::HandleMatchIsWaitingToStart()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::HandleMatchIsWaitingToStart"));

//	// Super Not Call
//	SetLocalController(UGameplayStatics::GetLocalPlayerController(this));
//	BII_CHECK(GetLocalController());
//
//	// B2NetGameMode게임의 경우 바로 매칭을 호출해준다.
//	SetMatchState(NetMatchState::RequestMatchMaking);
//
//	//World의 BeginPlay시점이 뒤에서 시작됨에 따라
//	//파티클 생성이 지연되는 문제가 있다.
//	//현 B2NetGameMode의 구조와 관련이 있어
//	//아래처럼 간단히 처리하나, 다른 액터에도 문제가 있다면 구조를 바꿔야할 듯.
//	for (TActorIterator<AEmitter> It(GetWorld()); It; ++It)
//	{
//		// Actors that have traveled seamlessly from other levels already had BeginPlay called in that level
//		if (!It->IsPendingKill() && !It->HasActorBegunPlay())
//		{
//#if !UE_BUILD_SHIPPING
//			UE_LOG(LogBladeII, Log, TEXT("Forcing call to 'BeginPlay' on newly spawned actor %s"), *It->GetName());
//#endif
//			It->DispatchBeginPlay();
//		}
//	}
}

void AB2NetGameMode::HandleMatchHasStarted()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::HandleMatchHasStarted"));
	// Super Not Call

	// Make sure level streaming is up to date before triggering NotifyMatchStarted
	GEngine->BlockTillLevelStreamingCompleted(GetWorld());

	BII_CHECK(GetWorldSettings());

	//auto* MyPawn = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());
	//if (MyPawn)
	//{
	//	MyPawn->SetActorHiddenInGame(false);
	//	MyPawn->SetMovementDisable(false);
	//}

	// First fire BeginPlay, if we haven't already in waiting to start match
	GetWorldSettings()->NotifyBeginPlay();

	// Then fire off match started
	GetWorldSettings()->NotifyMatchStarted();

#if BII_STATS // PerfStat 은 MatchStart 시점부터 모으기 시작.
	PerfStatCollector.OnBegin(this);
#endif
	
	//StartEventScene_GameBegin();
}

void AB2NetGameMode::HookOnSpawnDefaultPawn(APawn* InSpawned)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::HookOnSpawnDefaultPawn"));

	Super::HookOnSpawnDefaultPawn(InSpawned);

	//auto* NetPlayer = Cast<ABladeIINetPlayer>(InSpawned);
	//BII_CHECK(NetPlayer);

	//NetPlayer->SetNetStatus(ABladeIINetPlayer::NET_MASTER);
}

void AB2NetGameMode::SubscribeEvents()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::SubscribeEvents"));

	if (bEventsSubscribed)
		return;

	Super::SubscribeEvents();

	Issues.Add(ReceivedResurrectClass<uint32, FVector, FRotator, TMap<EResurrectBuffType, uint8>>::GetInstance().Subscribe2(
		[this](uint32 netid, FVector pos, FRotator rot, TMap<EResurrectBuffType, uint8> BuffInfo)
		{
			this->RemotePlayerResurrect(netid, pos, rot, BuffInfo);
		}
	));
	Issues.Add(ReceivedHealthClass<uint32, float, uint32>::GetInstance().Subscribe2(
		[this](uint32 netid, float health, uint32 DamageCauserNetId)
		{
			this->HandleHealth(netid, health, DamageCauserNetId);
		}
	));
}

void AB2NetGameMode::SetDesiredPCClassEnum()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::SetDesiredPCClassEnum"));

	auto& DataStore = BladeIIGameImpl::GetLocalCharacterData();
	CurrentPCClassEnum = DataStore.GetMainPlayerClass();
}

void AB2NetGameMode::NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadGuy)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::NotifyPlayerDeadEnd"));

	if (GetPeerType() != PEER_SUPER)
		return;
}

void AB2NetGameMode::RealtimeUpdateUIDoc()
{
	//Mob일 경우는 없을 수도 있으니까
	//BII_CHECK(GetLocalController());
	//auto const B2Player = GetLocalController() ? Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn()) : nullptr;
//	Super::RealtimeUpdateUIDocSkill(B2Player);
//
////	if (GetPeerType() == PEER_RESTRICTED)
////		return;
//
//	Super::RealtimeUpdateUIDocHealth(B2Player);
}

void AB2NetGameMode::HandleMatchOtherPlayer()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::HandleMatchOtherPlayer"));

	Super::HandleMatchOtherPlayer();

	PrepareMyPlayer(GetMyNetId());
}

void AB2NetGameMode::HandleLeaveEnemy()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::HandleLeaveEnemy"));

	Super::HandleLeaveEnemy();
}

void AB2NetGameMode::HandleStartResult()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::HandleStartResult"));

	//Super::HandleStartResult();

	//BII_CHECK(GetLocalController());
	//auto MyPawn = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());
	//BII_CHECK(MyPawn);

	//MyPawn->DisableInput(GetLocalController());
}

void AB2NetGameMode::HandleFinishBattle()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::HandleFinishBattle"));
	Super::HandleFinishBattle();
}

void AB2NetGameMode::PrepareNetGame(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::PrepareNetGame"));

	BII_CHECK(GetLocalController() != NULL);

	Super::PrepareNetGame(NetId);
}

void AB2NetGameMode::PrepareMyPlayer(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::PrepareMyPlayer"));

	Super::PrepareMyPlayer(NetId);

	//auto* PlayerController = GetLocalController();
	//if (PlayerController->GetPawn() != NULL)
	//	return;

	//FString TagName = GetPeerType() == PEER_RESTRICTED ? TEXT("P2") : TEXT("P1");

	//RestartNetPlayer(PlayerController, TagName);

	//auto MyPawn = Cast<ABladeIINetPlayer>(PlayerController->GetPawn());
	//BII_CHECK(MyPawn)

	//GetWorldSettings()->NotifyBeginPlay();

	//MyPawn->SetNetId(NetId);
	//MyPawn->DisableInput(PlayerController);
	//MyPawn->SetMovementDisable(true);
	//MyPawn->SetActorHiddenInGame(true);
	//SetLocalController(PlayerController);

	//auto  Asset = GetGameRule()->GetBehaviorTreeAsset();
	//if (!Asset.IsEmpty())
	//{
	//	MyPawn->BehaviorTree = LoadObject<UBehaviorTree>(NULL, *Asset);
	//}
}

APawn* AB2NetGameMode::RestartNetPlayer(class AController* NewPlayer, FString const& TagName)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::RestartNetPlayer"));

	UE_LOG(LogNetGameMode, Verbose, TEXT("Begin RestartNetPlayer... And Start Spot Name : %d"), *TagName);

	AActor* StartSpot = FindPlayerStart(NewPlayer, TagName);

	// if a start spot wasn't found,
	BII_CHECK(StartSpot != NULL);

	if (NewPlayer == NULL)
	{
		auto*	Pawn = SpawnDefaultPawnFor(NULL, StartSpot);
		BII_CHECK(Pawn);

		Pawn->FaceRotation(Pawn->GetActorRotation(), 0.f);
		Pawn->SetPlayerDefaults();
		SetDesiredPCClassEnum();

		return Pawn;
	}

	APawn*	created = NULL;
	//// try to create a pawn to use of the default class for this player
	//if (NewPlayer->GetPawn() == NULL && DefaultPawnClass != NULL)
	//{
	//	created = SpawnDefaultPawnFor(NewPlayer, StartSpot);
	//	BII_CHECK(created);

	//	NewPlayer->SetPawn(created);
	//}
	//else if (NewPlayer->GetPawn())
	//{
	//	auto* Pawn = NewPlayer->GetPawn();

	//	FRotator StartRotation(ForceInit);
	//	StartRotation.Yaw = StartSpot->GetActorRotation().Yaw;
	//	FVector StartLocation = StartSpot->GetActorLocation();
	//	Pawn->SetActorLocationAndRotation(StartLocation, StartRotation);
	//}

	//BII_CHECK(NewPlayer->GetPawn());
	//auto* MyNetPlayer = Cast<ABladeIINetPlayer>(NewPlayer->GetPawn());
	//BII_CHECK(MyNetPlayer);
	//MyNetPlayer->SetNetStatus(ABladeIINetPlayer::NET_MASTER);

	//// initialize and start it up
	//InitStartSpot(StartSpot, NewPlayer);
	//NewPlayer->Possess(NewPlayer->GetPawn());
 //   BII_CHECK(NewPlayer->GetPawn());

	//// set initial control rotation to player start's rotation
	//NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

	//FRotator NewControllerRot = StartSpot->GetActorRotation();
	//NewControllerRot.Roll = 0.f;
	//NewPlayer->SetControlRotation(NewControllerRot);

	//SetPlayerDefaults(NewPlayer->GetPawn());

	//K2_OnRestartPlayer(NewPlayer);

	//SetDesiredPCClassEnum();

	return created;
}

//UB2SkillRTManager* AB2NetGameMode::GetRemoteSkillRTManager(uint32 NetId)
//{
////	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::GetRemoteSkillRTManager"));
//
//	bool has = RemoteSkillRTManagers.Contains(NetId);
//
//	if (has)
//		return RemoteSkillRTManagers[NetId];
//
//	auto* SkillRTManager = NewObject<UB2SkillRTManager>(this, *FString::Printf(TEXT("SkillRTManager_%d"), NetId), RF_Transient);
//	RemoteSkillRTManagers.Emplace(NetId, SkillRTManager);
//
//	return SkillRTManager;
//}

void AB2NetGameMode::HandleCharacterDead(ABladeIICharacter* DeadGuy, uint32 NetId, float KillingDamage)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::HandleCharacterDead"));

	//auto* Causer = Cast<AActor>(AB2NetGameMode::FindPawnByNetId(NetId));
	// 
	//DeadGuy->Die(KillingDamage, FDamageEvent(), nullptr, Causer);
}

void AB2NetGameMode::PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie)
{
	////Synchronous 로딩의 Async 로딩 flush 여부에 따라.. 이걸 다른 async request 보다 먼저 혹은 이후에..
	//PreloadInGameInfoAsset_SynchronousOnly(bAboutToPlayLoadingMovie);

	//Super::PreloadAnyNecessaryInfo(bAboutToPlayLoadingMovie);
	//
	//PreloadInGameInfoAsset_AsyncAllowed(bAboutToPlayLoadingMovie);
}

TArray<EPCClass> AB2NetGameMode::GetAllNetGameMatchedPCClasses()
{
	TArray<EPCClass> RetArray;
	//// 태그가 없는 게임모드라면 미리 로딩하지 않아도 일단 생성 시 블로킹이 걸리지는 않으나 결국 Pre-render 를 통해 셰이더 캐싱을 하려면 필요함. 그리고 Skill Anim 도 따로 분리되고..
	//// 상대 정보가 Preload 시점 이후에 들어오거나, 태그가 있거나 하면 이걸 override 해야 함.
	//if (UB2UIDocLobbyMatch* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	//{
	//	for (int32 i = 0; i < DocLobbyMatch->GetRemotePlayerInfoDataCount(); ++i)
	//	{
	//		RetArray.AddUnique(DocLobbyMatch->GetReservedRemotePCClassByIndex(i));
	//	}
	//}
	//// 맞으려나..
	//RetArray.AddUnique(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());

	return RetArray;
}

TArray<EPCClass> AB2NetGameMode::GetPCClassesToPreLoad()
{
	return GetAllNetGameMatchedPCClasses();
}

//TArray<FCombinedPCSkillAnimID> AB2NetGameMode::GetPCSkillAnimsToPreLoad()
//{
//#if WITH_EDITOR
//	if (GIsEditor)
//	{ // 에디터에선 모든 skill 애니메이션을 로딩하도록 하는 상위 클래스 기능을 사용
//		return ABladeIIGameMode::GetPCSkillAnimsToPreLoad();
//	}
//#endif
//
//	TArray<FCombinedPCSkillAnimID> RetArray;
//
//	RetArray.Append(GetLocalPCSkillAnims());
//
//	return RetArray;
//}
//
//TArray<FCombinedPCSkillAnimID> AB2NetGameMode::GetLocalPCSkillAnims()
//{
//	TArray<FCombinedPCSkillAnimID> SkillCombineIDs;
//
//	auto& LocalCharData = BladeIIGameImpl::GetLocalCharacterData();
//	GetEquippedPlayerSkillAnims(&LocalCharData, SkillCombineIDs);
//
//	return SkillCombineIDs;
//}
//
//TArray<FCombinedPCSkillAnimID> AB2NetGameMode::GetOpponentPCSkillAnims(int32 OpponentNetID)
//{
//	TArray<FCombinedPCSkillAnimID> SkillCombineIDs;
//
//	//auto& OpponentInfo = GetRemotePlayerInfoData(OpponentNetID);
//	//GetEquippedPlayerSkillAnims(&OpponentInfo, SkillCombineIDs);
//
//	return SkillCombineIDs;
//}

//void AB2NetGameMode::GetEquippedPlayerSkillAnims(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutSkillCombIDs)
//{
//	if (DataStore)
//	{
//		TArray<ESkillAnimType> MainClassAnimTypes;
//		TArray<ESkillAnimType> SubClassAnimTypes;
//
//		const EPCClass MainClass = DataStore->GetMainPlayerClass();
//		const EPCClass SubClass = DataStore->GetSubPlayerClass();
//
//		UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, MainClass, GetB2GameModeType(), MainClassAnimTypes, DataStore);
//		UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, SubClass, GetB2GameModeType(), SubClassAnimTypes, DataStore);
//
//		for (auto AnimType : MainClassAnimTypes)
//		{
//			if (AnimType != ESkillAnimType::ESA_End)
//				OutSkillCombIDs.AddUnique(FCombinedPCSkillAnimID(MainClass, AnimType));
//		}
//
//		for (auto AnimType : SubClassAnimTypes)
//		{
//			if (AnimType != ESkillAnimType::ESA_End)
//				OutSkillCombIDs.AddUnique(FCombinedPCSkillAnimID(SubClass, AnimType));
//		}
//	}
//}

void AB2NetGameMode::SendResurrectHelper(const ABladeIINetPlayer* player)
{

}

void AB2NetGameMode::SendStartGamePacket()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SendStartGamePacket()"));

	BII_CHECK(GetPeerType() == PEER_SUPER);

	//packet::ByteStream	payload;
	//FString encoded_string = packet::FinalizePacket(packet::SEND_SYNC_STARTGAME, GetMyNetId(), 0, packet::ALL, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
}

void AB2NetGameMode::SpawnRemotePlayer(uint32 NetId, EPCClass MainClass, EPCClass SubClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::SpawnRemotePlayer"));
}

void AB2NetGameMode::PrepareSlaveNetGame(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::PrepareSlaveNetGame"));
	// check validations...
	BII_CHECK(LocalController != NULL);

	Super::PrepareSlaveNetGame(NetId);
}

void AB2NetGameMode::ForceResurrectLocalPlayer()
{
	// 서버체크없이 (재화를 사용하지않고) 강제 부활하는 경우
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::ForceResurrectLocalPlayer"));
	
	//if (ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn()))
	//{
	//	pPlayer->GetSkillRTManager()->ResurrectPlayer(GetResurrectGameModeType());

	//	// 부활쪽에서 교체되었다. 다시얻어와야함
	//	pPlayer = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());
	//	if(pPlayer)
	//		SetPlayerCollisionType(pPlayer);
	//}	
}

void AB2NetGameMode::ForceResurrectAIPlayer(class ABladeIINetPlayer* pPlayer)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::ForceResurrectLocalPlayer"));

	//if (pPlayer)
	//{
	//	// 부활시 살아날 위치를 재조정해준다.
	//	SetResurrectPlayerPosition(pPlayer);

	//	ABladeIIPlayerAIController* B2AC = Cast<ABladeIIPlayerAIController>(pPlayer->GetController());

	//	if (!B2AC)
	//		return;

	//	pPlayer->GetSkillRTManager()->BornAgain(false);

	//	// 부활쪽에서 교체되었다. 다시얻어와야함
	//	pPlayer = Cast<ABladeIINetPlayer>(B2AC->GetPawn());

	//	if (pPlayer)
	//	{
	//		SetPlayerCollisionType(pPlayer);

	//	}

	//	B2AC->BeginAutoPlay();
	//}
}

void AB2NetGameMode::RemotePlayerResurrect(uint32 NetId, FVector pos, FRotator rot, TMap<EResurrectBuffType, uint8>& BuffInfos)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::RemotePlayerResurrect"));

	//if (ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(FindPawnByNetId(NetId)))
	//{
	//	UE_LOG(LogNetGameMode, Log, TEXT("RemotePlayerResurrect NetId =  %d"), pPlayer->GetNetId());

	//	pPlayer->RemoteResurrect(pos, rot, BuffInfos);
	//}
}

AActor* AB2NetGameMode::MyFindPlayerStart(const FString& IncomingName)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::MyFindPlayerStart"));

	//UWorld* World = GetWorld();

	//// if incoming start is specified, then just use it
	//if (IncomingName.IsEmpty())
	//	return nullptr;

	//const FName IncomingPlayerStartTag = FName(*IncomingName);

	//for (TActorIterator<APlayerStart> It(World); It; ++It)
	//{
	//	APlayerStart* Start = *It;
	//	if (Start && Start->PlayerStartTag == IncomingPlayerStartTag)
	//	{
	//		return Start;
	//	}
	//}

	return nullptr;
}

uint32 AB2NetGameMode::FindNetIdByDamageCauser(AController* InInstigator, AActor* Causer, AActor* Victim)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::FindNetIdByDamageCauser"));

	uint32					ReturnNetId = 0;
	//ABladeIINetPlayer*		IsPlayer = nullptr;
	////ABladeIINetCharacter*	IsMob = nullptr;
	//auto*					IsProjectile = Cast<ABladeIIProjectile>(Causer);
	//AActor*					RealCauser = IsProjectile ? IsProjectile->GetOwner() : (Causer ? Causer : (InInstigator ? InInstigator->GetPawn() : nullptr));

	//if (!RealCauser)
	//	return 0;

	//IsPlayer = Cast<ABladeIINetPlayer>(RealCauser);
	//IsMob = Cast<ABladeIINetCharacter>(RealCauser);

	//if (IsPlayer)
	//	ReturnNetId = IsPlayer->GetNetId();
	//else if (IsMob)
	//	ReturnNetId = IsMob->GetNetId();

	//BII_CHECK(ReturnNetId);
	return ReturnNetId;
}

APawn* AB2NetGameMode::FindPawnByNetId(uint32 NetId)
{
	// 로그가 너무 많이뜸 주석
	//B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::FindPawnByNetId"));

	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	auto* Mob = Cast<ABladeIINetCharacter>(*Iterator);
	//	auto* Player = Cast<ABladeIINetPlayer>(*Iterator);

	//	if (!Player && !Mob)
	//		continue;

	//	if (Player && Player->IsPendingKill())
	//		continue;

	//	if (Mob && Mob->IsPendingKill())
	//		continue;

	//	if (Player && Player->GetNetId() == NetId)
	//		return Player;

	//	if (Mob && Mob->GetNetId() == NetId)
	//		return Mob;
	//}

	return nullptr;
}

void AB2NetGameMode::SendChannelHeartbeat(float Delta)
{
	HeartbeatTick += Delta;

	if (HeartbeatTick < 0.05f)
		return;

	HeartbeatTick = 0.0f;

	if (GetRoomId().IsEmpty())
		return;

	if (!HasMatchStarted())
		return;

	//FB2NetworkBridge::GetInstance()->ChannelHeartbeat(GetRoomId());

}

ABladeIIPlayer* AB2NetGameMode::SpawnRemotePlayer(class UWorld* SpawnWorld, EPCClass InClass, const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2NetGameMode::SpawnRemotePlayer"));

	//FActorSpawnParameters FinalSpawnParameter(SpawnParameters);
	//FinalSpawnParameter.bDeferPostSpawnInitialize = true; // bDeferConstruction 관련 처리는 밖에서 하지만 bDeferPostSpawnInitialize 는 여기선 무조건 사용해야. 

	//BII_CHECK(SpawnWorld);
	//BII_CHECK(InClass != EPCClass::EPC_End);
	//UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	//BII_CHECK(PCBox);

	//UB2PCClassInfo* ClassInfo = PCBox ? PCBox->GetSingleClassInfo(InClass) : nullptr;

	//BII_CHECK(ClassInfo);


	//// It will be picked up by player's BeginPlay.
	//CurrentPCClassEnum = InClass;

	//// Main spawn operation. Other spawn handling (e.g. FinishSpawning) needs to be done outside of here depend on spawn parameters.
	//auto* SpawnedActor = Cast<ABladeIINetPlayer>(SpawnWorld->SpawnActor(DefaultRemotePawnClass, &SpawnTransform, FinalSpawnParameter));
	//BII_CHECK(SpawnedActor);

	//SpawnedActor->SetNetStatus(ABladeIINetPlayer::NET_SLAVE);

	//SpawnedActor->FinalizePCClassInfoSpawn();

	//SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParameter); // bDeferPostSpawnInitialize 에 따른 처리.
	//return SpawnedActor;
	return NULL;
}

void AB2NetGameMode::SetLocalController(APlayerController* InController)
{
	BII_CHECK(InController);
	LocalController = InController;
}

int32 AB2NetGameMode::GetLocalPlayerTeamNum() const
{
	//if (LocalController && LocalController->HasClientLoadedCurrentWorld())
	//{
	//	if (LocalController->GetPawn() && LocalController->GetPawn()->IsValidObj())
	//	{
	//		ABladeIICharacter* pPawn = Cast<ABladeIICharacter>(LocalController->GetPawn());
	//		if (pPawn)
	//			return pPawn->GetTeamNum();
	//	}
	//}

	return 0;
}

void AB2NetGameMode::ReceiveAnimStateIndex(uint32 RemoteNetId, FNetSyncAnimInfo& NetSyncAnimInfo)
{
	if (!SyncedAnimStateIndexByNetId.Contains(RemoteNetId))
	{
		SyncedAnimStateIndexByNetId.Add(RemoteNetId);
	}
	//
	////모든 패킷을 다 저장 할 수 없으므로 정해진 갯수 이상이면 삭제
	//if (SyncedAnimStateIndexByNetId[RemoteNetId].Num() >= MAXSYNCANIM)
	//{
	//	SyncedAnimStateIndexByNetId[RemoteNetId].Empty();
	//	UE_LOG(LogTemp, Log, TEXT("AB2NetGameMode::ReceiveAnimStateIndex, SyncedAnimStateIndexByNetId[%d].Empty"), RemoteNetId);
	//}

	//SyncedAnimStateIndexByNetId[RemoteNetId].Add(NetSyncAnimInfo);
	//UE_LOG(LogTemp, Log, TEXT("AB2NetGameMode::ReceiveAnimStateIndex, RemoteNetId = %d, AttackState : %d ArrayNum = %d"), RemoteNetId, NetSyncAnimInfo.AttackState, SyncedAnimStateIndexByNetId[RemoteNetId].Num());
}

TArray<FNetSyncAnimInfo> AB2NetGameMode::GetSyncedAnimStateIndexInfo(uint32 RemoteNetId)
{
	TArray<FNetSyncAnimInfo> NetSyncAnimInfo;
	NetSyncAnimInfo.Empty();

	if (SyncedAnimStateIndexByNetId.Contains(RemoteNetId))
	{
		NetSyncAnimInfo = SyncedAnimStateIndexByNetId[RemoteNetId];
		SyncedAnimStateIndexByNetId[RemoteNetId].Empty();
		UE_LOG(LogTemp, Log, TEXT("AB2NetGameMode::GetSyncedAnimStateIndexInfo,RemoteNetId = %d, SyncedAnimStateIndexByNetId Num : %d"), RemoteNetId, SyncedAnimStateIndexByNetId[RemoteNetId].Num());
	}

	return NetSyncAnimInfo;
}

/*
void AB2NetGameMode::HandleTakeDamage(AActor* C, AController* EventInstigator, AActor* DamageCauser)
{
	return;

	if (GetPeerType() == PEER_RESTRICTED)
		return;

	ABladeIINetPlayer*		NetPlayer			= Cast<ABladeIINetPlayer>(C);
	ABladeIINetCharacter*	NetCharacter		= Cast<ABladeIINetCharacter>(C);
	const uint32			DamageCauserNetId	= FindNetIdByDamageCauser(EventInstigator, DamageCauser, C);

	uint32					NetId				= 0;
	float					Health				= 0.0f;
	
	if (NetPlayer)
	{
		NetId	= NetPlayer->GetNetId();
		Health	= NetPlayer->Health;
	}

	if (NetCharacter)
	{
		NetId	= NetCharacter->GetNetId();
		Health	= NetCharacter->Health;
	}

	packet::ByteStream	payload;
	payload << NetId;
	payload << Health;
	payload << DamageCauserNetId;

//	B2LOGMANAGER_DEBUG(LoggerB2NetGameMode, "netid : [%d]\n Health : [%f]\n DamageCauserNetId : [%d]\n payload:[%s]\n", NetId, Health, DamageCauserNetId, TCHAR_TO_ANSI(*FString(payload)));

	FString encoded_string = packet::FinalizePacket(packet::HEALTH, NetId, 0, packet::ALLBUTME, payload);
	SendMessage(encoded_string);
}

void AB2NetGameMode::HandleHealingHealth(AActor* C)
{
	if (GetPeerType() == PEER_RESTRICTED)
		return;

	ABladeIINetPlayer*		NetPlayer = Cast<ABladeIINetPlayer>(C);
	ABladeIINetCharacter*	NetCharacter = Cast<ABladeIINetCharacter>(C);

	uint32					NetId = 0;
	float					Health = 0.0f;
	uint32					DamageCauserNetId = 0;

	if (NetPlayer)
	{
		NetId = NetPlayer->GetNetId();
		Health = NetPlayer->Health;
		DamageCauserNetId = NetPlayer->GetNetId();
	}

	if (NetCharacter)
	{
		NetId = NetCharacter->GetNetId();
		Health = NetCharacter->Health;
		DamageCauserNetId = NetCharacter->GetNetId();
	}

	packet::ByteStream	payload;
	payload << NetId;
	payload << Health;
	payload << DamageCauserNetId;

//	B2LOGMANAGER_DEBUG(LoggerB2NetGameMode, "netid : [%d]\n Health : [%f]\n DamageCauserNetId : [%d]\n payload:[%s]\n", NetId, Health, DamageCauserNetId, TCHAR_TO_ANSI(*FString(payload)));

	FString encoded_string = packet::FinalizePacket(packet::HEALTH, NetId, 0, packet::ALLBUTME, payload);
	SendMessage(encoded_string);
}
*/

void AB2NetGameMode::HandleHealth(uint32 OwnerId, float Health, uint32 DamageCauserId)
{
/*
	if(GetPeerType() != PEER_RESTRICTED)
		return;

	auto* P = FindPawnByNetId(OwnerId);

	ABladeIINetPlayer*		NetPlayer		= Cast<ABladeIINetPlayer>(P);
	ABladeIINetCharacter*	NetCharacter	= Cast<ABladeIINetCharacter>(P);

	if (NetPlayer)
	{
		NetPlayer->RemoteAdjustHealth(Health, DamageCauserId);
	}
	else if (NetCharacter)
	{
		NetCharacter->RemoteAdjustHealth(Health, DamageCauserId);
	}
*/
}

bool AB2NetGameMode::CanDie() const
{
	return GetPeerType() != PEER_RESTRICTED;
}

bool AB2NetGameMode::CanTakeDamage()
{
	return true;
}
