#pragma once

#include "IWholesaler.h"
#include "OnlineWholesaler.h"
#include "OfflineWholesaler.h"

namespace data_trader
{
	class Association
	{
	public:
		static data_trader::WholesalerPtr BootStrap()
		{
			FString WarehouseLocation;
			GConfig->GetString(TEXT("Warehouse"), TEXT("Location"), WarehouseLocation, GGameIni);

			if (WarehouseLocation == TEXT("Remote"))
			{
				wholesaler = std::make_shared<data_trader::OnlineWholesaler>();
			}
			else if (WarehouseLocation == TEXT("Local"))
			{
				wholesaler = std::make_shared<data_trader::OfflineWholesaler>();
			}

			return wholesaler;
		}

		static void Shutdown()
		{
			wholesaler.reset();
		}

	private:
		static WholesalerPtr wholesaler;
	};
}