#pragma once
//#include "BladeII.h"
#include "Serializer.h"

namespace packet
{
	ByteStream	FinalizePacket(uint16 Type, uint32 Sender, uint32 Receiver, uint8 option, ByteStream& payload);

	void		PackingHelper(packet::ByteStream& target, FVector const& v);
	void		PackingHelper(packet::ByteStream& target, FRotator const& r);
}