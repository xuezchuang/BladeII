

#include "PacketMaker.h"
#include "Packet.h"
//#include "B2LogManager.h"

//*
//B2GetLogger(PacketMaker)
/*/
#if DEBUG_LOG
static auto g_LoggerPacketMaker = B2Logger::B2LogManager::GetLogger("PacketMaker");
#endif
//*/


#define DEBUG_LOG 0

namespace packet
{
	ByteStream FinalizePacket(uint16 Type, uint32 Sender, uint32 Receiver, uint8 option, ByteStream& payload)
	{
#if DEBUG_LOG

#ifndef IntToName
#define IntToName(ValueName)		\
	case ValueName:					\
		TypeName = #ValueName;		\
		break;
#endif //IntToName

		std::string TypeName;
		switch(Type)
		{
			IntToName(ALL)
			IntToName(ALLBUTME)
			IntToName(BEGIN_HANDSHAKING)
			IntToName(ACK_BEGIN_HANDSHAKING)
			IntToName(EXCHANGE_INFORMATIONS)
			IntToName(ACK_EXCHANGE_INFORMATIONS)
			IntToName(SEND_ROOM_PLAYERCLASS)
			IntToName(ACK_SEND_ROOM_PLAYERCLASS)
			IntToName(SEND_PREREQUISITE_TO_START)
			IntToName(ACK_SEND_PREREQUISITE_TO_START)
			IntToName(PREPARE_TO_START)
			IntToName(SYNC_READY_TO_STARTGAME)
			IntToName(SEND_SYNC_STARTGAME)
			IntToName(CHANGE_ROOM_ACCESS)
			IntToName(KICK_USER)
			IntToName(MOVE)
			IntToName(ATTACK)
			IntToName(STOPATTACK)
			IntToName(STARTSKILL)
			IntToName(GUARD)
			IntToName(STOPGUARD)
			IntToName(TAGPLAYER)
			IntToName(SPAWNMOTIONSTATECHANGED)
			IntToName(NOTIFYRESULT)
			IntToName(COMBOEND)
			IntToName(HEALTH)
			IntToName(UPDATETARGET)
			IntToName(COUNTERATTACK)
			IntToName(RAID_NOTIFYRESULT)
			IntToName(RESURRECT)
			IntToName(CONQUEST_AREA_STATE)
			IntToName(CONTROL_STATE)
			IntToName(CONTROL_KILLCOUNT)
			IntToName(CONTROL_RESULT)
			IntToName(CONTROL_TIMER)
			IntToName(ASK_SPAWN_COMPLETE)
			IntToName(SPAWN_COMPLETE)
			IntToName(BATTLE_GRADE_INFO)
			IntToName(MOB_ATTACK)
			IntToName(VERSION)
		default:
			TypeName = "Unknown";
			break;
		}

		B2LOGMANAGER_BLANK(LoggerPacketMaker);
		B2LOGMANAGER_DEBUG(LoggerPacketMaker, "Type : [%s]\n Sender : [%d]\n payload : [%s]\n", TypeName.c_str(), Sender, TCHAR_TO_ANSI(*FString(payload)));
#endif

		//[@AKI, 170525] [FIXME] Crash肺 烙矫 阜澜
		//check(Sender != 0);

		ByteStream	tmp;

		tmp << packet::VERSION;
		tmp << Type;
		tmp << Sender;
		tmp << Receiver;
		tmp << option;
		tmp << payload;

		return tmp;
	}

	void PackingHelper(packet::ByteStream& target, FVector const& v)
	{
		//target << v.X;
		//target << v.Y;
		//target << v.Z;
	}

	void PackingHelper(packet::ByteStream& target, FRotator const& r)
	{
		//target << r.Pitch;
		//target << r.Yaw;
		//target << r.Roll;
	}
}
