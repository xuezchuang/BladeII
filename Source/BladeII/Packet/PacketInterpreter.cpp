
#include "PacketInterpreter.h"
#include "Packet.h"
#include "PacketMaker.h"


#include "Event.h"
//#include "ChannelManager.h"

#include "B2LogManager.h"

//*
//B2GetLogger(PacketInterpreter)
/*/
#if DEBUG_LOG
static auto logger = B2Logger::B2LogManager::GetLogger("PacketInterpreter");
#endif //DEBUG_LOG
//*/

namespace packet
{
	PacketDecoderBank::~PacketDecoderBank()
	{
		Decoders_.clear();
	}

	PacketDecoderBank& PacketDecoderBank::GetInstance()
	{
		static	PacketDecoderBank	inst;
		return	inst;
	}

	IPacketDecoderPtr PacketDecoderBank::GetDecoder(uint32 decoder_id) const
	{
		auto decoder = Decoders_.find(decoder_id);
		return decoder != Decoders_.end() ? decoder->second : nullptr;
	}

	void PacketDecoderBank::DepositDecoder(uint32 decoder_id, IPacketDecoderPtr decoder)
	{
		if (Decoders_.find(decoder_id) != Decoders_.end())
			return;

		Decoders_.insert(std::make_pair(decoder_id, decoder));
	}

	FVector	UnpackingVector(ByteStream& decoded)
	{
		auto X = packet::SliceStream<float>(decoded);
		auto Y = packet::SliceStream<float>(decoded);
		auto Z = packet::SliceStream<float>(decoded);

		return FVector(X, Y, Z);
	}

	FRotator UnpackingRotator(ByteStream& decoded)
	{
		auto Pitch	= packet::SliceStream<float>(decoded);
		auto Yaw	= packet::SliceStream<float>(decoded);
		auto Roll	= packet::SliceStream<float>(decoded);

		return FRotator(Pitch, Yaw, Roll);
	}

	FGuid UnpackingToken(ByteStream& decoded)
	{
		uint32 A = packet::SliceStream<uint32>(decoded);
		uint32 B = packet::SliceStream<uint32>(decoded);
		uint32 C = packet::SliceStream<uint32>(decoded);
		uint32 D = packet::SliceStream<uint32>(decoded);

		return FGuid(A, B, C, D);
	}

	IMPLEMENT_PACKET_DECODER(BeginHandShaking, BEGIN_HANDSHAKING)
	IMPLEMENT_PACKET_DECODER(ExchangeInformations, EXCHANGE_INFORMATIONS)
	IMPLEMENT_PACKET_DECODER(AckExchangeInformations, ACK_EXCHANGE_INFORMATIONS)
	IMPLEMENT_PACKET_DECODER(NotifyResult, NOTIFYRESULT)
	IMPLEMENT_PACKET_DECODER(SendPrerequisiteToStart, SEND_PREREQUISITE_TO_START)
	IMPLEMENT_PACKET_DECODER(AckSendPrerequisiteToStart, ACK_SEND_PREREQUISITE_TO_START)
	IMPLEMENT_PACKET_DECODER(PrepareToStart, PREPARE_TO_START)
	IMPLEMENT_PACKET_DECODER(ChangeRoomAccess, CHANGE_ROOM_ACCESS)
	IMPLEMENT_PACKET_DECODER(Move, MOVE)
	IMPLEMENT_PACKET_DECODER(Rotation, ROTATION)
	IMPLEMENT_PACKET_DECODER(Attack, ATTACK)
	IMPLEMENT_PACKET_DECODER(StopAttack, STOPATTACK)
	IMPLEMENT_PACKET_DECODER(StartSkill, STARTSKILL)
// 	IMPLEMENT_PACKET_DECODER(Guard, GUARD)
// 	IMPLEMENT_PACKET_DECODER(StopGuard, STOPGUARD)
	IMPLEMENT_PACKET_DECODER(TagPlayer, TAGPLAYER)
	IMPLEMENT_PACKET_DECODER(ComboEnd, COMBOEND)
	IMPLEMENT_PACKET_DECODER(TakeDamage, TAKEDAMAGE)
	IMPLEMENT_PACKET_DECODER(RequestDamage, REQUESTDAMAGE)
	IMPLEMENT_PACKET_DECODER(Health, HEALTH)
	IMPLEMENT_PACKET_DECODER(UpdateTarget, UPDATETARGET)
//	IMPLEMENT_PACKET_DECODER(CounterAttack, COUNTERATTACK)
	IMPLEMENT_PACKET_DECODER(MobAttack, MOB_ATTACK)
	IMPLEMENT_PACKET_DECODER(RaidNotifyResult, RAID_NOTIFYRESULT)
	IMPLEMENT_PACKET_DECODER(Resurrect, RESURRECT)
	IMPLEMENT_PACKET_DECODER(KickUser, KICK_USER)
	IMPLEMENT_PACKET_DECODER(NoResponsePrerequisiteToStart, NO_RESPONSE_PREREQUISITE_TO_START)
	IMPLEMENT_PACKET_DECODER(SendRoomPlayerClass, SEND_ROOM_PLAYERCLASS)
	IMPLEMENT_PACKET_DECODER(AckSendRoomPlayerClass, ACK_SEND_ROOM_PLAYERCLASS)
	IMPLEMENT_PACKET_DECODER(ConquestAreaState, CONQUEST_AREA_STATE)
	IMPLEMENT_PACKET_DECODER(ControlState, CONTROL_STATE)
	IMPLEMENT_PACKET_DECODER(ControlKillCount, CONTROL_KILLCOUNT)
	IMPLEMENT_PACKET_DECODER(ControlResult, CONTROL_RESULT)
	IMPLEMENT_PACKET_DECODER(ControlTimer, CONTROL_TIMER)
	IMPLEMENT_PACKET_DECODER(ControlEnemyLeaveAll, CONTROL_ENEMY_LEAVE_ALL)
	IMPLEMENT_PACKET_DECODER(SyncReadyToStartGame, SYNC_READY_TO_STARTGAME)
	IMPLEMENT_PACKET_DECODER(SendSyncStartGame, SEND_SYNC_STARTGAME)
	IMPLEMENT_PACKET_DECODER(AskSpawnComplete, ASK_SPAWN_COMPLETE)
	IMPLEMENT_PACKET_DECODER(SpawnComplete, SPAWN_COMPLETE)
	IMPLEMENT_PACKET_DECODER(BattleGradeInfo, BATTLE_GRADE_INFO)
	IMPLEMENT_PACKET_DECODER(MatchToken, MATCH_TOKEN)
	IMPLEMENT_PACKET_DECODER(RequestQTE, REQUEST_QTE)
	IMPLEMENT_PACKET_DECODER(ResponseQTE, RESPONSE_QTE)
	IMPLEMENT_PACKET_DECODER(NotifyEndQTE, NOTIFY_END_QTE)
	IMPLEMENT_PACKET_DECODER(ControlChat, CONTROL_CHAT)
	IMPLEMENT_PACKET_DECODER(QTEEnable, QTE_ENABLE)
	IMPLEMENT_PACKET_DECODER(QTEEnableEnd, QTE_ENABLE_END)
	IMPLEMENT_PACKET_DECODER(SpawnControlNpc, SPAWN_CONTROL_NPC)
	IMPLEMENT_PACKET_DECODER(EtherActivated, ACTIVATE_ETHER)
	IMPLEMENT_PACKET_DECODER(Die, DIE)
	IMPLEMENT_PACKET_DECODER(BuffActivated, ACTIVATE_BUFF)
	IMPLEMENT_PACKET_DECODER(ConsumeControlSanctuary, CONSUME_CONTROL_SANCTUARY)
	IMPLEMENT_PACKET_DECODER(RespawnControlSanctuary, SPAWN_CONTROL_SANCTUARY)
	IMPLEMENT_PACKET_DECODER(DrinkControlSanctuary, DRINK_CONTROL_SANCTUARY)
	IMPLEMENT_PACKET_DECODER(RoomPlayerTeamInfo, ROOM_PLAYER_TEAM_INFO)
	IMPLEMENT_PACKET_DECODER(SetAnimState, SET_ANIMSTATE)
	IMPLEMENT_PACKET_DECODER(ChangeDamageNum, CHANGE_DAMAGENUM)
	IMPLEMENT_PACKET_DECODER(RaidNotifyTimeEnd, RAID_NOTIFYTIMEEND)
	IMPLEMENT_PACKET_DECODER(SyncAnimStateIndex, SYNC_ANIMSTATEINDEX)
	IMPLEMENT_PACKET_DECODER(QteNext, QTE_NEXT)
	IMPLEMENT_PACKET_DECODER(SpawnEnd, SPAWN_END)
		

	
	void BeginHandShakingDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto NetId = packet::SliceStream<uint32>(decoder);

		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "NetId : [%d]\n", NetId);
		ReceivedNetIdClass<uint32>::GetInstance().Signal(NetId);
	}

	void ExchangeInformationsDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto NetId = packet::SliceStream<uint32>(decoder);
		auto MainChar = packet::SliceStream<EPCClass>(decoder);
		auto SubChar = packet::SliceStream<EPCClass>(decoder);
		auto Grade = packet::SliceStream<uint32>(decoder);
		auto Star = packet::SliceStream<uint32>(decoder);
		auto ContinuousWin = packet::SliceStream<uint32>(decoder);
		FGuid Token = packet::UnpackingToken(decoder);
		auto AccountId = packet::SliceStream<int64>(decoder);

		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "NetId : [%d]\n MainChar : [%d]\n SubChar : [%d]\n Grade : [%d]\n Star : [%d]\n ContinuousWin : [%d]\n AccountId : [%d]\n", NetId, MainChar, SubChar, Grade, Star, ContinuousWin, AccountId);
		ReceivedExchangeInfomationsClass<uint32, EPCClass, EPCClass, uint32, uint32, uint32, const FGuid&, int64>::GetInstance().Signal(NetId, MainChar, SubChar, Grade, Star, ContinuousWin, Token, AccountId);
	}

	void AckExchangeInformationsDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		ReceivedAckExchangeInformationsClass<>::GetInstance().Signal();
	}

	void SendPrerequisiteToStartDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		int32 RoomType = packet::SliceStream<int32>(decoder);
		BII_CHECK(RoomType >= 0);
		FString RoomToken = packet::SliceStreamToString(decoder);
		auto	Count = packet::SliceStream<uint8>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "RoomType : [%d]\n RoomToken : [%s]\n", RoomType, TCHAR_TO_ANSI(*RoomToken));
		TArray<FB2NetMatchPeerInfo>	MatchPeerInfos;

		for (uint8 i = 0; i < Count; ++i)
		{
			FB2NetMatchPeerInfo	elem;
			elem.NetId = packet::SliceStream<uint32>(decoder);
			elem.Class = packet::SliceStream<uint8>(decoder);
			elem.PlayerStartIndex = packet::SliceStream<uint8>(decoder);
			elem.PlayerTeamType = packet::SliceStream<uint8>(decoder);

			B2LOGMANAGER_DEBUG
			(LoggerPacketInterpreter, 
				"MatchPeerInfos[%d].NetId : [%d]\n MatchPeerInfos[%d].Class : [%d]\n MatchPeerInfos[%d].PlayerStartIndex : [%d]\n MatchPeerInfos[%d].PlayerTeamType : [%d]\n", 
				i, elem.NetId, i, elem.Class, i, elem.PlayerStartIndex, i, elem.PlayerTeamType);

			MatchPeerInfos.Add(elem);
		}

		ReceivedPrerequisiteToStartClass<int32, const FString&, TArray<FB2NetMatchPeerInfo> const&>::GetInstance().Signal(RoomType, RoomToken, MatchPeerInfos);
	}

	void AckSendPrerequisiteToStartDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		auto NetId = packet::SliceStream<uint32>(decoder);
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "NetId : [%d]\n", NetId);
		ReceivedAckPrerequisiteToStartClass<uint32>::GetInstance().Signal(NetId);
	}

	void PrepareToStartDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		bool bAlreadyStarted = packet::SliceStream<bool>(decoder);
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "bAlreadyStarted : [%s]\n", bAlreadyStarted ? "True" : "False");
		ReceivedPrepareToStartClass<bool>::GetInstance().Signal(bAlreadyStarted);
	}

	void ChangeRoomAccessDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		uint8 AccessKey = packet::SliceStream<uint8>(decoder);
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "AccessKey : [%d]\n", AccessKey);
		ReceivedChangeRoomAccessClass<uint8>::GetInstance().Signal(AccessKey);
	}

	void NotifyResultDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto Result = packet::SliceStream<uint8>(decoder);
		auto State = packet::SliceStream<uint8>(decoder);
		ReceivedNotifyResultClass<ENetMatchResult, uint8>::GetInstance().Signal(static_cast<ENetMatchResult>(Result), State);
	}

	void MoveDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		FVector		Pos = packet::UnpackingVector(decoder);
		FVector		Vel = packet::UnpackingVector(decoder);
		FRotator	Rot = packet::UnpackingRotator(decoder);

		float fCurrentSpeed = packet::SliceStream<float>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : [%d]\n Pos : [X(%f), Y(%f), Z(%f)]\n Vel : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n", netid, Pos.X, Pos.Y, Pos.Z, Vel.X, Vel.Y, Vel.Z, Rot.Pitch, Rot.Roll, Rot.Yaw);
		ReceivedMovementClass<uint32, FVector,FVector, FRotator, float>::GetInstance().Signal(netid, Pos, Vel, Rot, fCurrentSpeed);
	}

	void RotationDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		FRotator	Rot = packet::UnpackingRotator(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : Rot : [P(%f), R(%f), Y(%f)]\n", netid, Rot.Pitch, Rot.Roll, Rot.Yaw);
		ReceivedRotationClass<uint32, FRotator>::GetInstance().Signal(netid, Rot);
	}

	void AttackDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		FVector		Pos = packet::UnpackingVector(decoder);
		FRotator	Rot = packet::UnpackingRotator(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : [%d]\n Pos : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n", netid, Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw);
		ReceivedAttackClass<uint32, FVector,FRotator>::GetInstance().Signal(netid, Pos, Rot);
	}

	void StopAttackDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		FVector		Pos = packet::UnpackingVector(decoder);
		FRotator	Rot = packet::UnpackingRotator(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : [%d]\n Pos : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n", netid, Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw);
		ReceivedStopAttackClass<uint32, FVector, FRotator>::GetInstance().Signal(netid, Pos, Rot);
	}

// 	void GuardDecoder::Decode(uint32 netid, ByteStream& payload)
// 	{
// 		packet::ByteStream decoder(payload);
// 
// 		FVector		Pos					= packet::UnpackingVector(decoder);
// 		FRotator	Rot					= packet::UnpackingRotator(decoder);
// 		
// 		auto		bSkillCancel		= packet::SliceStream<uint32>(decoder);
// 		auto		bNeedUpdateTarget	= packet::SliceStream<uint32>(decoder);
// 		auto		LastGuardBreakType	= packet::SliceStream<uint8>(decoder);
// 		auto		AttackState			= packet::SliceStream<uint8>(decoder);
// 
// 		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
// 		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : [%d]\n Pos : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n bSkillCancel : [%d]\n bNeedUpdateTarget : [%d]\n LastGuardBreakType : [%d]\n AttackState : [%d]\n", netid, Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw, bSkillCancel, bNeedUpdateTarget, LastGuardBreakType, AttackState);
// 		ReceivedGuardClass<uint32, FVector, FRotator, uint32, uint32, uint8, uint8>::GetInstance().Signal(netid, Pos, Rot, bSkillCancel, bNeedUpdateTarget, LastGuardBreakType, AttackState);
// 	}

// 	void StopGuardDecoder::Decode(uint32 netid, ByteStream& payload)
// 	{
// 		packet::ByteStream decoder(payload);
// 
// 		FVector		Pos = packet::UnpackingVector(decoder);
// 		FRotator	Rot = packet::UnpackingRotator(decoder);
// 
// 		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
// 		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : [%d]\n Pos : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n", netid, Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw);
// 		ReceivedStopGuardClass<uint32, FVector, FRotator>::GetInstance().Signal(netid, Pos, Rot);
// 	}

	void StartSkillDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto		i	= packet::SliceStream<uint8>(decoder);
		auto		ai	= packet::SliceStream<uint8>(decoder);
		FVector		Pos = packet::UnpackingVector(decoder);
		FRotator	Rot = packet::UnpackingRotator(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : [%d]\n Pos : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n i : [%d]\n ai : [%d]\n", netid, Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw, i, ai);
		ReceivedSkillAttackClass<uint32, uint8, uint8, FVector, FRotator>::GetInstance().Signal(netid, i, ai, Pos, Rot);
	}

	void TagPlayerDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto		TagState	= packet::SliceStream<uint8>(decoder);
		auto		TagClass	= packet::SliceStream<uint8>(decoder);
		FVector		Pos			= packet::UnpackingVector(decoder);
		FRotator	Rot			= packet::UnpackingRotator(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n TagState : [%d]\n TagClass : [%d]\n Pos : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n", netid, TagState, TagClass, Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw);
		ReceivedTagPlayerClass<uint32, uint8,uint8,FVector,FRotator>::GetInstance().Signal(netid, TagState, TagClass, Pos, Rot);
	}

	void ComboEndDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		uint8		InLastAttackState = packet::SliceStream<uint8>(decoder);
		bool IsAttackCanceled = packet::SliceStream<bool>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : [%d]\n InLastAttackState : %d\n", netid, InLastAttackState);
		ReceivedComboEndClass<uint32, uint8, bool>::GetInstance().Signal(netid, InLastAttackState, IsAttackCanceled);
	}

	void TakeDamageDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto    OwnerId = packet::SliceStream<uint32>(decoder);
		auto	DamageCauserId = packet::SliceStream<uint32>(decoder);
		auto	DamageAmount = packet::SliceStream<float>(decoder);
		auto	DamageInfoHash = packet::SliceStream<uint32>(decoder);
		auto	OwnerHealth = packet::SliceStream<int32>(decoder);
		int32	DummyDamagedNum = 0; // 泅犁 救靖

		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n DamageCauserNetId : [%d] DamageAmount : [%d]\n DamageInfoHash : [%d] OwnerHealth : [%d]\n DamageNum : [%d]\n payload : [%s]\n", netid, DamageCauserId, DamageAmount, DamageInfoHash, VictimHealth, DummyDamagedNum, TCHAR_TO_ANSI(*FString(payload)));
		ReceivedTakeDamageClass<uint32, uint32, float, uint32, int32, int32>::GetInstance().Signal(OwnerId, DamageCauserId, DamageAmount, DamageInfoHash, OwnerHealth, DummyDamagedNum);
	}

	void RequestDamageDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto    OwnerId = packet::SliceStream<uint32>(decoder);
		auto	DamageCauserId = packet::SliceStream<uint32>(decoder);
		auto	DamageAmount = packet::SliceStream<float>(decoder);
		auto	DamageInfoHash = packet::SliceStream<uint32>(decoder);
		
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n DamageCauserNetId : [%d] DamageAmount : [%d]\n DamageInfoHash : [%d] OwnerHealth : [%d]\n payload : [%s]\n", netid, DamageCauserId, DamageAmount, DamageInfoHash, TCHAR_TO_ANSI(*FString(payload)));
		ReceivedRequestDamageClass<uint32, uint32, float, uint32>::GetInstance().Signal(OwnerId, DamageCauserId, DamageAmount, DamageInfoHash);
	}

	void HealthDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto    OwnerId = packet::SliceStream<uint32>(decoder);
		auto	Health	= packet::SliceStream<float>(decoder);
		auto	NetId	= packet::SliceStream<uint32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n Health : [%f]\n DamageCauserNetId : [%d]\n payload : [%s]\n", netid, Health, NetId, TCHAR_TO_ANSI(*FString(payload)));
		ReceivedHealthClass<uint32, float, uint32>::GetInstance().Signal(OwnerId, Health, NetId);
	}

	void UpdateTargetDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto	NetId	= packet::SliceStream<uint32>(decoder);
		auto	Dist	= packet::SliceStream<float>(decoder);

		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d], Target NetId : [%d], Distance : [%f]", netid, NetId, Dist);
		ReceivedUpdateTargetClass<uint32, uint32, float>::GetInstance().Signal(netid, NetId, Dist);
	}

// 	void CounterAttackDecoder::Decode(uint32 netid, ByteStream& payload)
// 	{
// 		packet::ByteStream decoder(payload);
// 
// 		FVector		Pos = packet::UnpackingVector(decoder);
// 		FRotator	Rot = packet::UnpackingRotator(decoder);
// 
// 		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d], Pos : [X(%f), Y(%f), Z(%f)], Rot : [P(%f), R(%f), Y(%f)]", netid, Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw);
// 		ReceivedCounterAttackClass<uint32, FVector, FRotator>::GetInstance().Signal(netid, Pos, Rot);
// 	}

	void MobAttackDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		FVector		Pos				= packet::UnpackingVector(decoder);
		FRotator	Rot				= packet::UnpackingRotator(decoder);
		auto		AttackSignal	= packet::SliceStream<bool>(decoder);
		auto		AttackIndex		= packet::SliceStream<uint32>(decoder);

		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "Pos : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n AttackSignal : [%s]\n AttackIndex : [%d]\n", Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw, AttackSignal ? "True" : "False", AttackIndex);
		ReceivedMobAttackClass<uint32, FVector, FRotator, bool, uint32>::GetInstance().Signal(netid, Pos, Rot, AttackSignal, AttackIndex);
	}

	void RaidNotifyResultDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto		IsSuccess			= packet::SliceStream<bool>(decoder);
		auto		BonusPlayer			= packet::SliceStream<uint32>(decoder);
		auto		Count				= packet::SliceStream<uint8>(decoder);

		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "IsSuccess : [%s]\n BonusPlayer : [%d]\n", IsSuccess ? "True" : "False");
		TArray<TPair<int32, int32>>	Contributions;
		for (uint8 i = 0; i < Count; ++i)
		{
			TPair<int32, int32> Each;
			Each.Key = packet::SliceStream<uint32>(decoder);
			Each.Value = packet::SliceStream<uint32>(decoder);

			//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "Contributions.Key : [%d]\n Contributions.Value : [%d]\n", Each.Key, Each.Value);
			Contributions.Add(Each);
		}

		ReceivedRaidNotifyResultClass<bool, uint32, TArray<TPair<int32, int32>>const &>::GetInstance().Signal(IsSuccess, BonusPlayer, Contributions);
	}

	void ResurrectDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		FVector		Pos = packet::UnpackingVector(decoder);
		FRotator	Rot = packet::UnpackingRotator(decoder);

		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "Pos : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n", Pos.X, Pos.Y, Pos.Z, Rot.Pitch, Rot.Roll, Rot.Yaw);
		TMap<EResurrectBuffType, uint8> BuffMap;

		auto		Count = packet::SliceStream<uint8>(decoder);
		for (uint8 i = 0; i < Count; ++i)
		{
			uint8 BuffType = packet::SliceStream<uint8>(decoder);
			uint8 BuffCount = packet::SliceStream<uint8>(decoder);

			//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "BuffType [%d]\n BuffCount : [%d]\n", BuffType, BuffCount);
			BuffMap.Add((EResurrectBuffType)BuffType, BuffCount);
		}

		ReceivedResurrectClass<uint32, FVector, FRotator, TMap<EResurrectBuffType, uint8>>::GetInstance().Signal(netid, Pos, Rot, BuffMap);
	}

	void KickUserDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n", netid);
		ReceivedKickUserClass<>::GetInstance().Signal();
	}

	void NoResponsePrerequisiteToStartDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n", netid);
		ReceivedNoResponsePrerequisiteToStartClass<>::GetInstance().Signal();
	}
	
	void SendRoomPlayerClassDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		auto HostId = packet::SliceStream<uint32>(decoder);
		auto Count = packet::SliceStream<uint8>(decoder);

		//B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n HostId [%d]\n", netid, HostId);
		
		TArray<FB2NetMatchPeerInfo>	RaidPeerInfos;
		for (uint8 i = 0; i < Count; ++i)
		{
			FB2NetMatchPeerInfo	elem;
			elem.NetId = packet::SliceStream<uint32>(decoder);
			elem.Class = packet::SliceStream<uint8>(decoder);

			//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "RaidPeerInfos.NetID : [%d]\n RaidPeerInfos.Class : [%d]\n", elem.NetId, elem.Class);
			RaidPeerInfos.Add(elem);
		}

		ReceivedRoomPlayerClassClass<uint32, TArray<FB2NetMatchPeerInfo> const&>::GetInstance().Signal(HostId, RaidPeerInfos);
	}

	void AckSendRoomPlayerClassDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto	NetId = packet::SliceStream<uint32>(decoder);
		auto	Class = packet::SliceStream<EPCClass>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n NetID : [%d]\n Class : [%d]\n", netid, NetId, Class);
		ReceivedAckRoomPlayerClassClass<uint32, EPCClass>::GetInstance().Signal(NetId, Class);
	}
	
	void ConquestAreaStateDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		uint8 ConquestState = packet::SliceStream<uint8>(decoder);
		float RateRed = packet::SliceStream<float>(decoder);
		float RateBlue = packet::SliceStream<float>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n ConquestState : [%d]\n RateRed : [%f]\n RateBlue : [%f]\n", netid, ConquestState, RateRed, RateBlue);
		ReceivedConquestAreaStateClass<uint8, float, float>::GetInstance().Signal(ConquestState, RateRed, RateBlue);
	}

	void ControlStateDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		uint8 ControlState = packet::SliceStream<uint8>(decoder);
		float RateRed = packet::SliceStream<float>(decoder);
		float RateBlue = packet::SliceStream<float>(decoder);
		bool bResetConquestGage = packet::SliceStream<bool>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n ControlState : [%d]\n RateRed : [%f]\n RateBlue : [%f]\n bResetConquestGage : [%s]\n", netid, ControlState, RateRed, RateBlue, bResetConquestGage ? "True" : "False");
		ReceivedControlStateClass<uint8, float, float, bool>::GetInstance().Signal(ControlState, RateRed, RateBlue, bResetConquestGage);
	}

	void ControlKillCountDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		
		TMap<uint32, int32> ControlKillInfosMap;

		int32 RowCount = packet::SliceStream<int32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		for (int32 nCnt = 0; nCnt < RowCount; ++nCnt)
		{
			uint32 KillerNetId = packet::SliceStream<uint32>(decoder);
			int32 KillCount = packet::SliceStream<int32>(decoder);

			B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netID : [%d]\n ControlKillInfosMap[%d].KillerNetId : [%d]\n ControlKillInfosMap[%d].KillCount : [%d]\n", netid, nCnt, KillerNetId, nCnt, KillCount);
			ControlKillInfosMap.Add(KillerNetId, KillCount);
		}

		ReceivedControlKillCountClass<TMap<uint32, int32>*>::GetInstance().Signal(&ControlKillInfosMap);
	}

	void ControlResultDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		ETeamType WinTeam = ETeamType(packet::SliceStream<uint8>(decoder));

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n WinTeam : [%d]\n", netid, WinTeam);
		//ReceivedControlResultClass<ETeamType>::GetInstance().Signal(WinTeam);
	}

	void ControlTimerDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		uint8 SceneState = packet::SliceStream<uint8>(decoder);
		float ElapseTime = packet::SliceStream<float>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n SceneState : [%d]\n ElapseTime : [%f]\n", netid, SceneState, ElapseTime);
		ReceivedControlTimerClass<uint8, float>::GetInstance().Signal(SceneState, ElapseTime);
	}

	void ControlEnemyLeaveAllDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n SceneState : [%d]\n ElapseTime : [%f]\n", netid, SceneState, ElapseTime);
		ReceivedControlEnemyLeaveAllClass<>::GetInstance().Signal();
	}
	
	void SyncReadyToStartGameDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		auto SendId = packet::SliceStream<uint32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n SendId : [%d]\n", netid, SendId);
		ReceivedSyncReadyToStartGameClass<uint32>::GetInstance().Signal(SendId);
	}

	void SendSyncStartGameDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n", netid);
		ReceivedSyncStartGameClass<>::GetInstance().Signal();
	}

	void AskSpawnCompleteDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n", netid);
		ReceivedAskSpawnCompleteClass<>::GetInstance().Signal();
	}

	void SpawnCompleteDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		bool bForce = packet::SliceStream<bool>(decoder);
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n bForce : [%s]\n", netid, bForce ? "True" : "False");

		ReceivedSpawnCompleteClass<uint32, bool>::GetInstance().Signal(netid, bForce);
	}

	void BattleGradeInfoDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		int32 BattleGrade = packet::SliceStream<int32>(decoder);
		int32 BattleStarCount = packet::SliceStream<int32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n BattleGrade : [%d]\n BattleStarCount : [%d]\n", netid, BattleGrade, BattleStarCount);
		ReceivedBattleGradeInfoClass<uint32, int32, int32>::GetInstance().Signal(netid, BattleGrade, BattleStarCount);
	}

	void MatchTokenDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		FString MatchToken = SliceStreamToString(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n BattleGrade : [%d]\n BattleStarCount : [%d]\n", netid, BattleGrade, BattleStarCount);
		ReceivedOtherTokenClass<uint32, const FString&>::GetInstance().Signal(netid, MatchToken);
	}

	void RequestQTEDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n", netid);

		ReceiveRequestQTEClass<uint32>::GetInstance().Signal(netid);
	}

	void ResponseQTEDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		uint32 QTEPlayerNetId = packet::SliceStream<uint32>(decoder);
		bool bAllow = packet::SliceStream<bool>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n QTEPlayerNetId : [%d]\n bAllow : [%s]\n", netid, QTEPlayerNetId, bAllow ? "True" : "False");

		ReceiveResponseQTEClass<uint32, bool>::GetInstance().Signal(QTEPlayerNetId, bAllow);
	}

	void NotifyEndQTEDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n", netid);

		ReceiveNotifyEndQTEClass<uint32>::GetInstance().Signal(netid);
	}

	void ControlChatDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		uint8 ChatType = packet::SliceStream<uint8>(decoder);
		int32 ChatIndex = packet::SliceStream<uint32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]\n ChatType : [%d]\n ChatIndex : [%d]\n", netid, ChatType, ChatIndex);

		ReceiveControlChatClass<uint32, uint8, int32>::GetInstance().Signal(netid, ChatType, ChatIndex);
	}

	void QTEEnableDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);
		uint8 TeamNum = packet::SliceStream<uint8>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);
		
		ReceiveQteEnableClass<uint32, uint8>::GetInstance().Signal(netid, TeamNum);
	}

	void QTEEnableEndDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);

		ReceiveQteEnableEndClass<>::GetInstance().Signal();
	}	

	void SpawnControlNpcDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);

		ReceiveSpawnControlNpcClass<>::GetInstance().Signal();
	}	

	void EtherActivatedDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto    EtherOwnerId	= packet::SliceStream<uint32>(decoder);
		auto	EtherCauserId	= packet::SliceStream<uint32>(decoder);
		auto	EtherSetId		= packet::SliceStream<uint32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "EtherOwner : [%d]\n EtherCauser : [%d]\n EtherSetId : [%d]\n", EtherOwnerId, EtherCauserId, EtherSetId);

		ReceivedEtherActivatedClass<uint32, uint32, uint32>::GetInstance().Signal(EtherOwnerId, EtherCauserId, EtherSetId);
	}

	void DieDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		auto    OwnerId = packet::SliceStream<uint32>(decoder);
		auto	KillerId = packet::SliceStream<uint32>(decoder);
		auto	KilledClass = packet::SliceStream<int32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "Die : [%d]\n Victim : [%d]\n Killer : [%d]\n KilledClass : [%d]\n", OwnerId, KillerId, KilledClass);

		ReceivedDieClass<uint32, uint32, int32>::GetInstance().Signal(OwnerId, KillerId, KilledClass);
	}

	void BuffActivatedDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		uint32 BuffOwnerId = packet::SliceStream<uint32>(decoder);
		uint32 BuffCauserId = packet::SliceStream<uint32>(decoder);
		uint8 BuffType = packet::SliceStream<uint8>(decoder);
		float fDuration = packet::SliceStream<float>(decoder);
		float fAmount = packet::SliceStream<float>(decoder);
		bool bUseEffect = packet::SliceStream<bool>(decoder);
		bool bUseTextEffect = packet::SliceStream<bool>(decoder);

		TArray<float> arAdditionalParams;

		int32 nAdditionalParamCnt = packet::SliceStream<int32>(decoder);

		for (int32 nCnt = 0; nCnt < nAdditionalParamCnt; ++nCnt)
		{
			arAdditionalParams.Add(packet::SliceStream<float>(decoder));
		}

		ReceivedBuffActivatedClass<uint32, uint32, uint8, float, float, bool, bool, TArray<float>*>::GetInstance().Signal(BuffOwnerId, BuffCauserId, BuffType, fDuration, fAmount, bUseEffect, bUseTextEffect, arAdditionalParams.Num() == 0 ? nullptr : &arAdditionalParams);
	}

	void ConsumeControlSanctuaryDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);

		packet::ByteStream decoder(payload);

		int32 SancIndex = packet::SliceStream<int32>(decoder);

		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "SancIndex : [%d]", SancIndex);

		// 捞亥飘
		ReceivedConsumeControlSanctuaryClass<int32>::GetInstance().Signal(SancIndex);
	}

	void RespawnControlSanctuaryDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);

		packet::ByteStream decoder(payload);

		int32 SancIndex = packet::SliceStream<int32>(decoder);

		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "SancIndex : [%d]", SancIndex);

		// 捞亥飘
		ReceivedRespawnControlSanctuaryClass<int32>::GetInstance().Signal(SancIndex);
	}

	void DrinkControlSanctuaryDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);

		packet::ByteStream decoder(payload);

		int32 SancIndex = packet::SliceStream<int32>(decoder);

		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "SancIndex : [%d]", SancIndex);

		// 捞亥飘 netid
		ReceivedDrinkControlSanctuaryClass<uint32, int32>::GetInstance().Signal(netid, SancIndex);
	}

	void RoomPlayerTeamInfoDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		TMap<uint32, uint8> TeamInfos;

		int32 nCount = packet::SliceStream<int32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "nCount : [%d]", nCount);

		for (int32 i = 0; i < nCount; ++i)
		{
			uint32 TempNetId = packet::SliceStream<uint32>(decoder);
			uint8 TempTeamType = packet::SliceStream<uint8>(decoder);

			B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "TempNetId : [%d]", TempNetId);
			B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "TempTeamType : [%d]", TempTeamType);

			TeamInfos.Add(TempNetId, TempTeamType);
		}
		
		// 捞亥飘龋免. 甘持扁
		ReceiveRoomPlayerTeamInfoClass<TMap<uint32, uint8>&>::GetInstance().Signal(TeamInfos);
	}


	void SetAnimStateDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		FString AnimState = packet::SliceStreamToString(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "AnimState : [%s]", *AnimState);

		ReceivedSetAnimStateClass<uint32, const FString&>::GetInstance().Signal(netid, AnimState);
	}

	void ChangeDamageNumDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		int32 ReceivedDamageNum = packet::SliceStream<int32>(decoder);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "DamageNum : [%s]", *ReceivedDamageNum);

		ReceivedChangeDamageNumClass<uint32, int32>::GetInstance().Signal(netid, ReceivedDamageNum);
	}

	void RaidNotifyTimeEndDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);

		ReceivedRaidNotifyTimeEndClass<uint32>::GetInstance().Signal(netid);
	}

	void SyncAnimStateIndexDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		FNetSyncAnimInfo NetSyncAnimInfo = FNetSyncAnimInfo();

		NetSyncAnimInfo.TargetPCClass = packet::SliceStream<uint8>(decoder);
		NetSyncAnimInfo.nSyncedAnimStateIndex = packet::SliceStream<int32>(decoder);

		NetSyncAnimInfo.bSmash = packet::SliceStream<bool>(decoder);
		NetSyncAnimInfo.bDamaged = packet::SliceStream<bool>(decoder);
		NetSyncAnimInfo.bInQTEState = packet::SliceStream<bool>(decoder);

		NetSyncAnimInfo.PCSpawnMotionState = packet::SliceStream<uint8>(decoder);
		NetSyncAnimInfo.DamageAnimIndex = packet::SliceStream<uint8>(decoder);
		NetSyncAnimInfo.GuardBreakLogic = packet::SliceStream<uint8>(decoder);
		NetSyncAnimInfo.QTEType = packet::SliceStream<uint8>(decoder);
		NetSyncAnimInfo.QTENPCClass = packet::SliceStream<uint8>(decoder);
		NetSyncAnimInfo.AttackState = packet::SliceStream<uint8>(decoder);
		NetSyncAnimInfo.SkillAnimType = packet::SliceStream<uint8>(decoder);
		NetSyncAnimInfo.QTEStateNum = packet::SliceStream<uint8>(decoder);

		ReceivedSyncAnimStateIndexClass<uint32, FNetSyncAnimInfo&>::GetInstance().Signal(netid, NetSyncAnimInfo);

		FVector		Pos = packet::UnpackingVector(decoder);
		FVector		Vel = packet::UnpackingVector(decoder);
		FRotator	Rot = packet::UnpackingRotator(decoder);
		
		B2LOGMANAGER_BLANK(LoggerPacketInterpreter);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "netid : [%d]", netid);
		B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "StateIndex : [%d]", StateIndex);
		B2LOGMANAGER_TRACE(LoggerPacketInterpreter, "netid : [%d]\n Pos : [X(%f), Y(%f), Z(%f)]\n Vel : [X(%f), Y(%f), Z(%f)]\n Rot : [P(%f), R(%f), Y(%f)]\n", netid, Pos.X, Pos.Y, Pos.Z, Vel.X, Vel.Y, Vel.Z, Rot.Pitch, Rot.Roll, Rot.Yaw);
		
		float fCurrentSpeed = packet::SliceStream<float>(decoder);
				
		ReceivedMovementClass<uint32, FVector, FVector, FRotator, float>::GetInstance().Signal(netid, Pos, Vel, Rot, fCurrentSpeed);
	}

	void QteNextDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		ReceivedQteNextClass<uint32>::GetInstance().Signal(netid);
	}

	void SpawnEndDecoder::Decode(uint32 netid, ByteStream& payload)
	{
		packet::ByteStream decoder(payload);

		ReceivedSpawnEndClass<uint32>::GetInstance().Signal(netid);
	}
	
	/////////////////////////////////// 叼内歹 场 ///////////////////////////////////////////////////////////////

	void DecodePayload(uint32 type, uint32 netid, ByteStream& payload)
	{
		auto decoder = PacketDecoderBank::GetInstance().GetDecoder(type);
		if (!decoder)
			return;

		decoder->Decode(netid, payload);
	}

	bool IngorePacket(uint32 sender, uint32 receiver, uint8 option)
	{
        // @ReplayMode FIX
  //      if (data_trader::ChannelManager::GetInstance().IsReplayMode())
  //      {
  //          return false;
  //      }

		//if (option & packet::ALL)
		//	return false;

		//if ((option & packet::ALLBUTME) && sender == receiver)
		//	return true;

		return false;
	}

	void DecodePacket(uint32 sender, uint32 receiver, FString const& context)
	{
		packet::ByteStream stream(context);

		//packet::Header	header;

		//header.Version	= packet::SliceStream<uint16>(stream);
		//header.Type		= packet::SliceStream<uint16>(stream);
		//header.Sender	= packet::SliceStream<uint32>(stream);
		//header.Receiver	= packet::SliceStream<uint32>(stream);
		//header.Option	= packet::SliceStream<uint8>(stream);

		//auto payload	= packet::SliceStream(stream);

		//// by this line, Stream's size must be zero. but if not, 
		//// it means that there is different packet layout between encode and decode 
		//check(stream.size() == 0);

		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "Sender[%d] ===> Receiver[%d]", sender, receiver);
		//B2LOGMANAGER_DEBUG(LoggerPacketInterpreter, "Type[%d]", header.Type);

		//if (header.Version != VERSION)
		//{
		//	B2LOGMANAGER_WARNING(LoggerPacketInterpreter, "Protocol versions are different...");
		//	return;
		//}

		//// You're not interested in the packet you sent...
		//
		//if (IngorePacket(sender, receiver, header.Option))
		//{
		//	B2LOGMANAGER_DEBUG(LoggerPacketInterpreter,"Ignore this packet... because you send this packet...");
		//	return;
		//}

		//DecodePayload(header.Type, header.Sender, payload);
	}
}
