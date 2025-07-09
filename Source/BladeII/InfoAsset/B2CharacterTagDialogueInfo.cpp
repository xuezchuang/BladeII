// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2CharacterTagDialogueInfo.h"


const FCharacterTagSoundInfoTransientSoundAssetInfo FCharacterTagSoundInfo::GetSoundAssetPtrOfIndex(int32 OuterRowNum, int32 InIndex) const
{
	check(OuterRowNum >= 0); // 자신이 위치한 DataTable 상에서의 줄 번호가 될 것.

	FCharacterTagSoundInfoTransientSoundAssetInfo NewInfo(
		GetCharTagSoundAssetCacheKey(OuterRowNum, InterActionPCClass, InIndex),
		(TagSound.IsValidIndex(InIndex) ? &TagSound[InIndex] : nullptr)
		);

	// AssetPtr 을 로딩하고서 캐싱할 맵(LoadedPtrMap)의 키값이 됨. 
	// UB2CharacterTagDialogueInfo 에 등록된 전체 TagSound 요소들에 걸쳐서 unique 한 값이 되어야.
	check(NewInfo.MyCacheMapKey >= 0);
	
	return NewInfo;
}
const FCharacterTagSoundInfoTransientSoundAssetInfo FCharacterTagSoundInfo::GetRandSoundAssetPtr(int32 OuterRowNum) const
{
	const int32 RandIndex = FMath::RandRange(0, GetNumAllSoundAsset() - 1);
	return GetSoundAssetPtrOfIndex(OuterRowNum, RandIndex);
}
void FCharacterTagSoundInfo::GatherAllSoundAssetPtr(TArray<TSoftObjectPtr<USoundCue>>& OutSoundAssets)
{// Valid 한 것만 넘겨주는 옵션이 필요할 수도..
	OutSoundAssets.Append(TagSound);
}

#if WITH_EDITOR
void FCharacterTagSoundInfo::EditorLoadAll()
{
	const int32 iArrayMax = TagSound.Num();
	for (int32 idx = 0; idx < iArrayMax; ++idx)
	{
		// 원칙적으로는 로딩한 것을 캐싱해 놓아야 한다.
		// 인게임 코드에서는 이렇게 로딩하지 않는다.
		// 에디터에선 한번만 이렇게 해 주어도 Redirector 처리에 문제가 없을 것으로 믿고 가는 거.. 아주 큰 테이블은 아닐 테니.
		if (TagSound.IsValidIndex(idx))
			TagSound[idx].LoadSynchronous();
	}
}
#endif

//========================================================================

const FCharacterTagSoundInfo* FCharacterTagDialogueInfo::GetTagSoundInfoPtr(EPCClass InterClass) const
{
	const int32 iTagSound_InterActionMax = TagSound_InterAction.Num();
	for (int32 idx = 0; idx < iTagSound_InterActionMax; ++idx)
	{
		const FCharacterTagSoundInfo& ThisInfo = TagSound_InterAction[idx];
		if (ThisInfo.InterActionPCClass == InterClass)
		{
			return &ThisInfo;
		}
	}
	return nullptr;
}
const FCharacterTagSoundInfoTransientSoundAssetInfo FCharacterTagDialogueInfo::GetRandSoundAssetPtr(EPCClass InterClass) const
{
	const FCharacterTagSoundInfo* InterClassSoundInfo = GetTagSoundInfoPtr(InterClass);
	if (InterClassSoundInfo)
	{
		check(MyTableRowNum >= 0);
		return InterClassSoundInfo->GetRandSoundAssetPtr(MyTableRowNum);
	}
	return FCharacterTagSoundInfoTransientSoundAssetInfo(INDEX_NONE, nullptr);
}
void FCharacterTagDialogueInfo::GatherAllSoundAssetPtr(TArray<TSoftObjectPtr<USoundCue>>& OutSoundAssets)
{
	for (FCharacterTagSoundInfo& ThisInfo : TagSound_InterAction)
	{ // Valid 한 것만 넘겨주는 옵션이 필요할 수도..
		ThisInfo.GatherAllSoundAssetPtr(OutSoundAssets);
	}
}
#if WITH_EDITOR
void FCharacterTagDialogueInfo::EditorLoadAll()
{
	const int32 iArrayMax = TagSound_InterAction.Num();
	for (int32 idx = 0; idx < iArrayMax; ++idx)
	{
		if (TagSound_InterAction.IsValidIndex(idx))
			TagSound_InterAction[idx].EditorLoadAll();
	}
}
#endif

//========================================================================

bool UB2CharacterTagDialogueInfo::bManagedUnload = true;
UB2CharacterTagDialogueInfo::UB2CharacterTagDialogueInfo(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	LoadCharacterTagDialogueInfoTable();

	FCultureRef fCurretCulture = FInternationalization::Get().GetCurrentCulture();

	strNativeCulture = fCurretCulture->GetName();
}

const UDataTable* UB2CharacterTagDialogueInfo::LoadCharacterTagDialogueInfoTable()
{
	// 테이블만 로딩하는 것. 와중에 텍스트 데이터들 같은 건 같이 로딩되지만 TAsset 으로 레퍼런스 된 에셋들은 로딩되지 않는다.

	if (CachedCharacterTagDialogueInfoTable == nullptr)
	{
		FString fstrFilePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2CharacterTagDialogueInfo"), TEXT("CharacterTagDialogueInfoTable"), fstrFilePath, GGameIni);
		CachedCharacterTagDialogueInfoTable = LoadObject<UDataTable>(nullptr, *fstrFilePath);

		GConfig->GetBool(TEXT("/Script/BladeII.B2CharacterTagDialogueInfo"), TEXT("ManagedTagDialogueInfoUnload"), bManagedUnload, GGameIni);

		if (CachedCharacterTagDialogueInfoTable)
		{
			// 테이블 처음 로딩했을 때 처리할 거..
			TArray<FName> RowKeys = CachedCharacterTagDialogueInfoTable->GetRowNames();

			const int32 iRowKeyMax = RowKeys.Num();
			for (int32 i = 0; i < iRowKeyMax; ++i)
			{
				FCharacterTagDialogueInfo* CharacterTagDialogueInfo = 
					CachedCharacterTagDialogueInfoTable->FindRow<FCharacterTagDialogueInfo>(RowKeys[i], TEXT(""));

				// 아마도 이 인덱스가 키값하고 일치할 거 같은데 그거랑 상관없이 단순히 각 줄마다의 일련번호를 기록해 둔다.
				// 숫자 자체가 의미있는 건 아니고 각 row 를 식별할 수 있으면 됨.
				CharacterTagDialogueInfo->MyTableRowNum = i;
			}
		}
	}

	return CachedCharacterTagDialogueInfoTable;
}

FText UB2CharacterTagDialogueInfo::GetRandomTagSuggestTextFromSkill(const int32 iPlayerClassType)
{
	FText fTagText;

	if (const FCharacterTagDialogueInfo* CharacterTagDialogueInfo = FindCharacterTagDialogueInfo(iPlayerClassType))
	{
		if (const FCharacterTagSuggestTextInfo* pTagText = &CharacterTagDialogueInfo->TagSuggestText_FromSkill)
		{
			int32 iMaxIndex = 0;
			int32 iTextIndex = 0;

			if (strNativeCulture == TEXT("en"))
			{
				iMaxIndex = pTagText->Text_English.Num() - 1;
				iTextIndex = FMath::RandRange(0, iMaxIndex);

				if (pTagText->Text_English.IsValidIndex(iTextIndex))
					fTagText = pTagText->Text_English[iTextIndex];
			}
			else if (strNativeCulture == TEXT("ko"))
			{
				iMaxIndex = pTagText->Text_Korean.Num() - 1;
				iTextIndex = FMath::RandRange(0, iMaxIndex);

				if (pTagText->Text_Korean.IsValidIndex(iTextIndex))
					fTagText = pTagText->Text_Korean[iTextIndex];
			}
			else
			{
				iMaxIndex = pTagText->Text_Other.Num() - 1;
				iTextIndex = FMath::RandRange(0, iMaxIndex);

				if (pTagText->Text_Other.IsValidIndex(iTextIndex))
					fTagText = pTagText->Text_Other[iTextIndex];
			}

		}
	}

	return fTagText;
}

FText UB2CharacterTagDialogueInfo::GetRandomTagSuggestTextFromDyingHp(const int32 iPlayerClassType)
{
	FText fTagText;

	if (const FCharacterTagDialogueInfo* CharacterTagDialogueInfo = FindCharacterTagDialogueInfo(iPlayerClassType))
	{
		if (const FCharacterTagSuggestTextInfo* pTagText = &CharacterTagDialogueInfo->TagSuggestText_FromDyingHp)
		{
			int32 iMaxIndex = 0;
			int32 iTextIndex = 0;

			if (strNativeCulture == TEXT("en"))
			{
				iMaxIndex = pTagText->Text_English.Num() - 1;
				iTextIndex = FMath::RandRange(0, iMaxIndex);

				if (pTagText->Text_English.IsValidIndex(iTextIndex))
					fTagText = pTagText->Text_English[iTextIndex];
			}
			else if (strNativeCulture == TEXT("ko"))
			{
				iMaxIndex = pTagText->Text_Korean.Num() - 1;
				iTextIndex = FMath::RandRange(0, iMaxIndex);

				if (pTagText->Text_Korean.IsValidIndex(iTextIndex))
					fTagText = pTagText->Text_Korean[iTextIndex];
			}
			else
			{
				iMaxIndex = pTagText->Text_Other.Num() - 1;
				iTextIndex = FMath::RandRange(0, iMaxIndex);

				if (pTagText->Text_Other.IsValidIndex(iTextIndex))
					fTagText = pTagText->Text_Other[iTextIndex];
			}

		}
	}

	return fTagText;
}

USoundCue* UB2CharacterTagDialogueInfo::GetRandomTagSound(EPCClass SoundPlayPCClass, EPCClass IntarActionPCClass)
{
	USoundCue* pTagSoundCue = nullptr;

	const int32 iPlayerClassType = PCClassToInt(SoundPlayPCClass);

	if (const FCharacterTagDialogueInfo* CharacterTagDialogueInfo = FindCharacterTagDialogueInfo(iPlayerClassType))
	{
		const FCharacterTagSoundInfoTransientSoundAssetInfo& RandAssetInfo = CharacterTagDialogueInfo->GetRandSoundAssetPtr(IntarActionPCClass);
		if (RandAssetInfo.TheAssetPtr)
		{ 
			check(RandAssetInfo.MyCacheMapKey >= 0);
			// 처음 로딩하는 거면 로딩 후 캐싱해 놓고, 이미 로딩이 되어 있는 거라면 맵에서 찾아 쓴다.
			// 이게 이미 로딩 된 것의 LoadSynchronous 보다 약간이나마 빠르긴 하겠지만 
			// 로딩 후 캐싱해 놓는 것의 일차적인 목적은 그정도의 작은 향상이 아니라 동작의 안정성을 위해서임.
			USoundCue*& FoundOrAddCache = LoadedPtrMap.FindOrAdd(RandAssetInfo.MyCacheMapKey);
			if (!FoundOrAddCache)
			{
				FoundOrAddCache = LoadSynchronous(*(RandAssetInfo.TheAssetPtr));
			}

			pTagSoundCue = FoundOrAddCache;
		}
	}

	return pTagSoundCue;
}

const FCharacterTagDialogueInfo* UB2CharacterTagDialogueInfo::FindCharacterTagDialogueInfo(const int32 iPlayerClassType)
{
	FCharacterTagDialogueInfo* CharacterTagDialogueInfo = nullptr;

	if (const UDataTable* CharacterTagDialogueInfoTable = LoadCharacterTagDialogueInfoTable())
	{
		FString Key = FString::Printf(TEXT("%d"), iPlayerClassType);
		CharacterTagDialogueInfo = CharacterTagDialogueInfoTable->FindRow<FCharacterTagDialogueInfo>(FName(*Key), TEXT(""));
		//CharacterTagDialogueInfo = CharacterTagDialogueInfoTable->FindRow<FCharacterTagDialogueInfo>(FName(*FString::Printf(*KeyFormat, iPlayerClassType)), TEXT(""));
	}

	return CharacterTagDialogueInfo;
}

bool UB2CharacterTagDialogueInfo::ShouldUnloadAllOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING]
	return true;
#else
	return !bManagedUnload;
#endif
}

void UB2CharacterTagDialogueInfo::UnloadAll()
{
	// 일괄 언로딩인데 하나만 콕 찝어서 Unload 하려면 원하는 언로딩 대상의 키값을 가지고 LoadedPtrMap 맵에서 빼 주는 걸 해야 한다.
	// 아마도 하나만 콕 찝어서 Unload 하는 게 필요할 것 같지는 않지만 줄 단위로 찝어서 Unload 하는 건 필요할 수도 있다.

	const UDataTable* TheDataTable = LoadCharacterTagDialogueInfoTable();
	if(TheDataTable)
	{
		LoadedPtrMap.Empty();

		TArray<TSoftObjectPtr<USoundCue>> AllSoundAssetPtr;
		TArray<FCharacterTagDialogueInfo*> AllRows;
		TheDataTable->GetAllRows<FCharacterTagDialogueInfo>(TEXT(""), AllRows);
		for (FCharacterTagDialogueInfo* ThisRow : AllRows)
		{
			if (ThisRow)
			{
				ThisRow->GatherAllSoundAssetPtr(AllSoundAssetPtr);
			}
		}

		for (TSoftObjectPtr<USoundCue>& ThisToUnload : AllSoundAssetPtr)
		{
			UnloadAsset(ThisToUnload.ToSoftObjectPath());
		}
	}
}
void UB2CharacterTagDialogueInfo::ConditionalUnloadAllOrPreserve()
{
	if (ShouldUnloadAllOnLevelTransition())
	{
		UnloadAll();
	}
	else
	{ // 언로드 안 하고 대신 레벨 transition 에도 끄떡없게 Rootset 에 넣어놓음.
		MarkAllLoadedAssetsRootSet();
	}

	// 다른 것들도 이런 식인데 캐시 맵은 어떤 경우라도 비움. 웬지 찝찝해서. 
	// 이렇게 해도 루트셋이면 실질적인 로딩은 안 하게 됨.
	LoadedPtrMap.Empty();
}
void UB2CharacterTagDialogueInfo::MarkAllLoadedAssetsRootSet()
{
	const UDataTable* TheDataTable = LoadCharacterTagDialogueInfoTable();
	if (TheDataTable)
	{
		TArray<TSoftObjectPtr<USoundCue>> AllSoundAssetPtr;
		TArray<FCharacterTagDialogueInfo*> AllRows;
		TheDataTable->GetAllRows<FCharacterTagDialogueInfo>(TEXT(""), AllRows);
		for (FCharacterTagDialogueInfo* ThisRow : AllRows)
		{
			if (ThisRow)
			{
				ThisRow->GatherAllSoundAssetPtr(AllSoundAssetPtr);
			}
		}
		for (TSoftObjectPtr<USoundCue>& ThisAsset : AllSoundAssetPtr)
		{
			if (ThisAsset.IsValid() && !ThisAsset->IsRooted()) // 로딩되어 있는 것만.
			{
				ThisAsset->AddToRoot();
			}
		}
	}
}

#if WITH_EDITOR
void UB2CharacterTagDialogueInfo::EditorLoadAll()
{
	const UDataTable* CharacterTagDialogueInfoTable = LoadCharacterTagDialogueInfoTable();

	if (!CharacterTagDialogueInfoTable)
		return;

	TArray<FName> RowKeys = CharacterTagDialogueInfoTable->GetRowNames();

	const int32 iRowKeyMax = RowKeys.Num();
	for (int32 i = 0; i < iRowKeyMax; ++i)
	{
		FCharacterTagDialogueInfo* CharacterTagDialogueInfo
			= CharacterTagDialogueInfoTable->FindRow<FCharacterTagDialogueInfo>(RowKeys[i], TEXT(""));

		if(!CharacterTagDialogueInfo)
			continue;

		CharacterTagDialogueInfo->EditorLoadAll();
	}
}
#endif