#pragma once

#include <memory>
#include <string>
#include "SmartEnum.h"

//ENUM(CacheCategory, Key_Item = 1, Key_Raid, Key_Quest);
ENUM(CacheCategory, Key_MasterData = 1);

struct FMD5Hash;
class FSHAHash;

namespace b2network
{
	struct B2NETWORK_API CacheResult
	{
		std::string		Hash;
		TArray<uint8>	PBContent;
	};

	class B2NETWORK_API MasterDataCacher
	{
	public:
		static MasterDataCacher& GetInstance();

		MasterDataCacher(MasterDataCacher const&)				= delete;
		MasterDataCacher& operator = (MasterDataCacher const&)	= delete;

		CacheResult	Query(CacheCategory) const;
		void Update(CacheCategory, uint8*, uint32);

	private:
		MasterDataCacher();
		~MasterDataCacher();

		void		Add(CacheCategory, uint8*, uint32);
		void		Delete(CacheCategory);

		bool		Intialize();
		bool		UpdateCacheResult(CacheCategory);
		FSHAHash	MakeHashFromByteStream(TArray<uint8>&);

		const FString MasterDataDirectory;

		TMap<CacheCategory, CacheResult>	Caches;
		bool								Status;
	};
}