
#include "Channel.h"

namespace data_trader
{
	void Channel::SubscribeChannel(uint32 participant)
	{
		Participants.AddUnique(participant);
	}

	void Channel::UnsubscribeChannel(uint32 participant)
	{
		Participants.RemoveSingle(participant);
	}
}