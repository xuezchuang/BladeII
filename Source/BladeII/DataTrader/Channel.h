#pragma once
//#include "BladeII.h"
#include <memory>
#include "CoreMinimal.h"
//#include "Array.h"

namespace data_trader
{
	class Channel
	{
	public:
		explicit Channel(const FString& Id) : Identify(Id)
		{}

		~Channel()
		{
			Participants.Empty();
		}

		void SubscribeChannel(uint32);
		void UnsubscribeChannel(uint32);

		int32 HowMany() const
		{
			return Participants.Num();
		}

		FString GetIdentify() const
		{
			return Identify;
		}

		void SetHostId(uint32 id)
		{
			HostId = id;
		}

		uint32 GetHostId() const
		{
			return HostId;
		}

	private:
		FString			Identify;
		uint32			HostId;
		TArray<uint32>	Participants;
	};

	using ChannelPtr = std::shared_ptr<Channel>;
}