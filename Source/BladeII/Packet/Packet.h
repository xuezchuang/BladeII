#pragma once

//#include "Platform.h"
#include "B2Reflection.h"

namespace packet
{
	// Receiver Types.
	static const uint8 ALL								= 1;
	static const uint8 ALLBUTME							= 2;	

	// Handshaking Packet Types
	static const uint16 BEGIN_HANDSHAKING				= 101;
	static const uint16 ACK_BEGIN_HANDSHAKING			= 102;
	static const uint16 EXCHANGE_INFORMATIONS			= 103;
	static const uint16 ACK_EXCHANGE_INFORMATIONS		= 104;
	static const uint16 SEND_ROOM_PLAYERCLASS			= 105;
	static const uint16 ACK_SEND_ROOM_PLAYERCLASS		= 106;
	static const uint16 SEND_PREREQUISITE_TO_START		= 107;
	static const uint16 ACK_SEND_PREREQUISITE_TO_START	= 108;
	static const uint16 PREPARE_TO_START				= 109;
	static const uint16 SYNC_READY_TO_STARTGAME			= 110;
	static const uint16 SEND_SYNC_STARTGAME				= 111;

	static const uint16 CHANGE_ROOM_ACCESS				= 112;
	static const uint16 KICK_USER						= 113;

	static const uint16 NO_RESPONSE_PREREQUISITE_TO_START = 114;

	// Game Packet Types.
	static const uint16 MOVE							= 1001;
	static const uint16 ATTACK							= 1002;
	static const uint16 STOPATTACK						= 1003;
	static const uint16 STARTSKILL						= 1004;
	static const uint16 GUARD							= 1005;
	static const uint16 STOPGUARD						= 1006;
	static const uint16 TAGPLAYER						= 1007;
	static const uint16 SPAWNMOTIONSTATECHANGED			= 1008;
	static const uint16 NOTIFYRESULT					= 1009;
	static const uint16 COMBOEND						= 1010;
	static const uint16 HEALTH							= 1011;
	static const uint16 RAID_NOTIFYRESULT				= 1012;
	static const uint16 RESURRECT						= 1013;
	static const uint16 UPDATETARGET					= 1014;
	static const uint16 COUNTERATTACK					= 1015;
	static const uint16 ROTATION						= 1016;
	static const uint16 TAKEDAMAGE						= 1017;
	static const uint16 REQUESTDAMAGE					= 1018;
	static const uint16 ACTIVATE_ETHER					= 1019;
	static const uint16 DIE								= 1020;
	static const uint16 ACTIVATE_BUFF					= 1021;
	static const uint16 ROOM_PLAYER_TEAM_INFO			= 1022;
	static const uint16 SET_ANIMSTATE					= 1023;
	static const uint16 CHANGE_DAMAGENUM				= 1024;
	static const uint16 RAID_NOTIFYTIMEEND				= 1025;

	static const uint16 CONQUEST_AREA_STATE				= 2001;
	static const uint16 CONTROL_STATE					= 2002;
	static const uint16 CONTROL_KILLCOUNT				= 2003;
	static const uint16 CONTROL_RESULT					= 2004;
	static const uint16 CONTROL_TIMER					= 2005;
	static const uint16 CONTROL_ENEMY_LEAVE_ALL			= 2006;
	static const uint16 ASK_SPAWN_COMPLETE				= 2007;
	static const uint16 SPAWN_COMPLETE					= 2008;
	static const uint16 BATTLE_GRADE_INFO				= 2009;
	static const uint16 MATCH_TOKEN						= 2010;
	static const uint16 REQUEST_QTE						= 2011;
	static const uint16 RESPONSE_QTE					= 2012;
	static const uint16 NOTIFY_END_QTE					= 2013;
	static const uint16 CONTROL_CHAT					= 2014;
	static const uint16 QTE_ENABLE						= 2015;
	static const uint16 QTE_ENABLE_END					= 2016;
	static const uint16 SPAWN_CONTROL_NPC				= 2017;
	static const uint16 CONSUME_CONTROL_SANCTUARY		= 2018;
	static const uint16 SPAWN_CONTROL_SANCTUARY			= 2019;
	static const uint16 DRINK_CONTROL_SANCTUARY			= 2020;

	// Game Packet Types Related To AI Mobs
	static const uint16 MOB_ATTACK						= 3001;

	static const uint16 SYNC_ANIMSTATEINDEX				= 4001;
	static const uint16 QTE_NEXT						= 4002;
	static const uint16 SPAWN_END						= 4003;

	// Protocol Version
	static const uint16 VERSION							= 0x0010;

	/*
		Header :
				 Version		- 2Byte
				 Type			- 2Byte
				 Sender			- 4Byte`
				 Receiver		- 4Byte
				 Option			- 1Byte
	*/
	//DECLARE_BEGIN(Header)
	//	DECLARE_FIELD(0, uint16, Version)
	//	DECLARE_FIELD(1, uint16, Type)
	//	DECLARE_FIELD(2, uint32, Sender)
	//	DECLARE_FIELD(3, uint32, Receiver)
	//	DECLARE_FIELD(4, uint8, Option)
	//DECLARE_END(5)
}