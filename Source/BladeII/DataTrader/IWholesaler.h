#pragma once

#include <memory>
#include "B2MessageInfoCommonStruct.h"

namespace data_trader
{
	class OnlineWholesaler;
	class OfflineWholesaler;

	class IWholesaler
	{
	public:
		virtual ~IWholesaler()
		{
		}
	};

	using WholesalerPtr = std::shared_ptr<IWholesaler>;
}