#pragma once
//#include "BladeII.h"
#include "Serializer.h"

namespace packet
{
	void		DecodePacket(uint32, uint32, FString const&);

	FVector		UnpackingVector(ByteStream& decoded);
	FRotator	UnpackingRotator(ByteStream& decoded);

	class IPacketDecoder
	{
	public:
		virtual ~IPacketDecoder()
		{
		}

		virtual void Decode(uint32, ByteStream&) = 0;
		virtual FString GetName() = 0;
	};

	using IPacketDecoderPtr = std::shared_ptr<IPacketDecoder>;
	using PacketDecoderType = std::function<void(uint32, ByteStream&)>;

	class PacketDecoderBank
	{
	public:
		static PacketDecoderBank&	GetInstance();

		PacketDecoderBank(PacketDecoderBank const&)					= delete;
		PacketDecoderBank& operator = (PacketDecoderBank const&)	= delete;

		IPacketDecoderPtr	GetDecoder(uint32) const;
		void				DepositDecoder(uint32, IPacketDecoderPtr);

	private:
		PacketDecoderBank()
		{
		}

		~PacketDecoderBank();

		std::map<uint32, IPacketDecoderPtr>	Decoders_;
	};

#define DECLARE_PACKET_DECODER(decoder_name)																\
	class decoder_name##Decoder : public IPacketDecoder														\
	{																										\
	public:																									\
		decoder_name##Decoder()																				\
		{																									\
			codeName = #decoder_name;																		\
		}																									\
		explicit decoder_name##Decoder(uint32 decoder_id)													\
		{																									\
			auto ptr = std::shared_ptr<decoder_name##Decoder>(new decoder_name##Decoder);					\
			PacketDecoderBank::GetInstance().DepositDecoder(decoder_id, ptr);								\
		}																									\
																											\
		virtual void Decode(uint32, ByteStream&) override;													\
		virtual FString GetName() override { return codeName; }												\
																											\
	private:																								\
		FString codeName;																					\
	};

#define IMPLEMENT_PACKET_DECODER(decoder_name, decoder_id)													\
	static decoder_name##Decoder	decoder_name##Instance{decoder_id};

	DECLARE_PACKET_DECODER(BeginHandShaking)
	DECLARE_PACKET_DECODER(ExchangeInformations)
	DECLARE_PACKET_DECODER(AckExchangeInformations)
	DECLARE_PACKET_DECODER(NotifyResult)
	DECLARE_PACKET_DECODER(SendPrerequisiteToStart)
	DECLARE_PACKET_DECODER(AckSendPrerequisiteToStart)
	DECLARE_PACKET_DECODER(PrepareToStart)
	DECLARE_PACKET_DECODER(ChangeRoomAccess)
	DECLARE_PACKET_DECODER(Move)
	DECLARE_PACKET_DECODER(Rotation)
	DECLARE_PACKET_DECODER(Attack)
	DECLARE_PACKET_DECODER(StopAttack)
	DECLARE_PACKET_DECODER(StartSkill)
//	DECLARE_PACKET_DECODER(Guard)
//	DECLARE_PACKET_DECODER(StopGuard)
	DECLARE_PACKET_DECODER(TagPlayer)
	DECLARE_PACKET_DECODER(ComboEnd)
	DECLARE_PACKET_DECODER(TakeDamage)
	DECLARE_PACKET_DECODER(RequestDamage)
	DECLARE_PACKET_DECODER(Health)
	DECLARE_PACKET_DECODER(UpdateTarget)
//	DECLARE_PACKET_DECODER(CounterAttack)
	DECLARE_PACKET_DECODER(MobAttack)
	DECLARE_PACKET_DECODER(RaidNotifyResult)
	DECLARE_PACKET_DECODER(Resurrect)
	DECLARE_PACKET_DECODER(KickUser)
	DECLARE_PACKET_DECODER(NoResponsePrerequisiteToStart)
	DECLARE_PACKET_DECODER(SendRoomPlayerClass)
	DECLARE_PACKET_DECODER(AckSendRoomPlayerClass)
	DECLARE_PACKET_DECODER(ConquestAreaState)
	DECLARE_PACKET_DECODER(ControlState)
	DECLARE_PACKET_DECODER(ControlKillCount)
	DECLARE_PACKET_DECODER(ControlResult)
	DECLARE_PACKET_DECODER(ControlTimer)
	DECLARE_PACKET_DECODER(ControlEnemyLeaveAll)
	DECLARE_PACKET_DECODER(SyncReadyToStartGame)
	DECLARE_PACKET_DECODER(SendSyncStartGame)
	DECLARE_PACKET_DECODER(AskSpawnComplete)
	DECLARE_PACKET_DECODER(SpawnComplete)
	DECLARE_PACKET_DECODER(BattleGradeInfo)
	DECLARE_PACKET_DECODER(MatchToken)
	DECLARE_PACKET_DECODER(RequestQTE)
	DECLARE_PACKET_DECODER(ResponseQTE)
	DECLARE_PACKET_DECODER(NotifyEndQTE)
	DECLARE_PACKET_DECODER(ControlChat)
	DECLARE_PACKET_DECODER(QTEEnable)
	DECLARE_PACKET_DECODER(QTEEnableEnd)
	DECLARE_PACKET_DECODER(SpawnControlNpc)
	DECLARE_PACKET_DECODER(EtherActivated)
	DECLARE_PACKET_DECODER(Die)
	DECLARE_PACKET_DECODER(BuffActivated)
	DECLARE_PACKET_DECODER(ConsumeControlSanctuary)
	DECLARE_PACKET_DECODER(RespawnControlSanctuary)
	DECLARE_PACKET_DECODER(DrinkControlSanctuary)
	DECLARE_PACKET_DECODER(RoomPlayerTeamInfo)
	DECLARE_PACKET_DECODER(SetAnimState)
	DECLARE_PACKET_DECODER(ChangeDamageNum)
	DECLARE_PACKET_DECODER(RaidNotifyTimeEnd)
	DECLARE_PACKET_DECODER(SyncAnimStateIndex)
	DECLARE_PACKET_DECODER(QteNext)
	DECLARE_PACKET_DECODER(SpawnEnd)
}
