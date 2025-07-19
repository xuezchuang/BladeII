#include "MasterDataCacher.h"
#include "B2NetworkPlugin.h"


//#include "SecureHash.h"

namespace b2network
{
	FString ConvertKeyNameToFileName(FString const& KeyName)
	{
		FString FileName(KeyName);
		FileName.RemoveFromStart(TEXT("Key_"), ESearchCase::IgnoreCase);

		return FileName;
	}

	MasterDataCacher& MasterDataCacher::GetInstance()
	{
		static MasterDataCacher inst;
		return inst;
	}

	MasterDataCacher::MasterDataCacher()
#if PLATFORM_ANDROID || PLATFORM_IOS
		: MasterDataDirectory(FPaths::GamePersistentDownloadDir() / FString(TEXT("MasterData")))
#else
		// : MasterDataDirectory(FPaths::GameSavedDir() / FString(TEXT("MasterData")))
#endif
	{
		Status = Intialize();
	}

	MasterDataCacher::~MasterDataCacher()
	{
		Caches.Empty();
	}

	CacheResult	MasterDataCacher::Query(CacheCategory key) const
	{
		if (!Status)
			return CacheResult{};

		if (Caches.Contains(key))
			return Caches[key];

		return CacheResult{};
	}

	void MasterDataCacher::Add(CacheCategory key, uint8* content, uint32 size)
	{
		if (!content)
			return;

		auto	Result			= Query(key);

		if (!Result.Hash.empty() && Result.PBContent.Num() != 0)
			return;

		FString	FullFileName	= MasterDataDirectory / ConvertKeyNameToFileName(key.ToString());
		auto&	FileManager		= IFileManager::Get();

		auto*	ptr				= reinterpret_cast<void*>(content);

		TArray<uint8>	WriteData;
		WriteData.AddUninitialized(size);
		FMemory::Memcpy(WriteData.GetData(), ptr, size);
		if (!FFileHelper::SaveArrayToFile(WriteData, *FullFileName))
			return;

		UpdateCacheResult(key);
	}

	void MasterDataCacher::Delete(CacheCategory key)
	{
		FString	FullFileName = MasterDataDirectory / ConvertKeyNameToFileName(key.ToString());
		auto&	FileManager = IFileManager::Get();

		FileManager.Delete(*FullFileName);

		Caches.Remove(key);

		UpdateCacheResult(key);
	}

	void MasterDataCacher::Update(CacheCategory key, uint8* content, uint32 size)
	{
		if (!Status)
			return;

		Delete(key);
		Add(key, content, size);
	}

	bool MasterDataCacher::Intialize()
	{
		auto Ret = true;

		auto& FileManager = IFileManager::Get();
		if (!FileManager.DirectoryExists(*MasterDataDirectory))
			FileManager.MakeDirectory(*MasterDataDirectory);

		if (!FileManager.DirectoryExists(*MasterDataDirectory))
			return false;

		// iterate all category to make md5 for every single one only if category file is in directory.
		for (uint8 i = 0; i < CacheCategory::_count; ++i)
		{
			uint8 key = CacheCategory::_values()[i];
			Ret = Ret && UpdateCacheResult(CacheCategory(key));
		}

		return Ret;
	}

	bool MasterDataCacher::UpdateCacheResult(CacheCategory key)
	{
		CacheResult	Result;
		FString		FullFileName = MasterDataDirectory / ConvertKeyNameToFileName(key.ToString());

		auto& FileManager = IFileManager::Get();

		if (!FileManager.FileExists(*FullFileName))
		{
			Caches.Emplace(key, Result);
			return true;
		}

		if (!FFileHelper::LoadFileToArray(Result.PBContent, *FullFileName))
			return false;

		Result.Hash	= TCHAR_TO_ANSI(*MakeHashFromByteStream(Result.PBContent).ToString());

		Caches.Emplace(key, Result);

		return true;
	}

	FSHAHash MasterDataCacher::MakeHashFromByteStream(TArray<uint8>& Contents)
	{
		FSHAHash Hash;

		FSHA1::HashBuffer(Contents.GetData(), Contents.Num(), Hash.Hash);
		return Hash;
	}
}