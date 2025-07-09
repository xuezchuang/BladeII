// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2CharacterTagDialogueInfo.h"


const FCharacterTagSoundInfoTransientSoundAssetInfo FCharacterTagSoundInfo::GetSoundAssetPtrOfIndex(int32 OuterRowNum, int32 InIndex) const
{
	check(OuterRowNum >= 0); // �ڽ��� ��ġ�� DataTable �󿡼��� �� ��ȣ�� �� ��.

	FCharacterTagSoundInfoTransientSoundAssetInfo NewInfo(
		GetCharTagSoundAssetCacheKey(OuterRowNum, InterActionPCClass, InIndex),
		(TagSound.IsValidIndex(InIndex) ? &TagSound[InIndex] : nullptr)
		);

	// AssetPtr �� �ε��ϰ� ĳ���� ��(LoadedPtrMap)�� Ű���� ��. 
	// UB2CharacterTagDialogueInfo �� ��ϵ� ��ü TagSound ��ҵ鿡 ���ļ� unique �� ���� �Ǿ��.
	check(NewInfo.MyCacheMapKey >= 0);
	
	return NewInfo;
}
const FCharacterTagSoundInfoTransientSoundAssetInfo FCharacterTagSoundInfo::GetRandSoundAssetPtr(int32 OuterRowNum) const
{
	const int32 RandIndex = FMath::RandRange(0, GetNumAllSoundAsset() - 1);
	return GetSoundAssetPtrOfIndex(OuterRowNum, RandIndex);
}
void FCharacterTagSoundInfo::GatherAllSoundAssetPtr(TArray<TSoftObjectPtr<USoundCue>>& OutSoundAssets)
{// Valid �� �͸� �Ѱ��ִ� �ɼ��� �ʿ��� ����..
	OutSoundAssets.Append(TagSound);
}

#if WITH_EDITOR
void FCharacterTagSoundInfo::EditorLoadAll()
{
	const int32 iArrayMax = TagSound.Num();
	for (int32 idx = 0; idx < iArrayMax; ++idx)
	{
		// ��Ģ�����δ� �ε��� ���� ĳ���� ���ƾ� �Ѵ�.
		// �ΰ��� �ڵ忡���� �̷��� �ε����� �ʴ´�.
		// �����Ϳ��� �ѹ��� �̷��� �� �־ Redirector ó���� ������ ���� ������ �ϰ� ���� ��.. ���� ū ���̺��� �ƴ� �״�.
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
	{ // Valid �� �͸� �Ѱ��ִ� �ɼ��� �ʿ��� ����..
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
	// ���̺� �ε��ϴ� ��. ���߿� �ؽ�Ʈ �����͵� ���� �� ���� �ε������� TAsset ���� ���۷��� �� ���µ��� �ε����� �ʴ´�.

	if (CachedCharacterTagDialogueInfoTable == nullptr)
	{
		FString fstrFilePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2CharacterTagDialogueInfo"), TEXT("CharacterTagDialogueInfoTable"), fstrFilePath, GGameIni);
		CachedCharacterTagDialogueInfoTable = LoadObject<UDataTable>(nullptr, *fstrFilePath);

		GConfig->GetBool(TEXT("/Script/BladeII.B2CharacterTagDialogueInfo"), TEXT("ManagedTagDialogueInfoUnload"), bManagedUnload, GGameIni);

		if (CachedCharacterTagDialogueInfoTable)
		{
			// ���̺� ó�� �ε����� �� ó���� ��..
			TArray<FName> RowKeys = CachedCharacterTagDialogueInfoTable->GetRowNames();

			const int32 iRowKeyMax = RowKeys.Num();
			for (int32 i = 0; i < iRowKeyMax; ++i)
			{
				FCharacterTagDialogueInfo* CharacterTagDialogueInfo = 
					CachedCharacterTagDialogueInfoTable->FindRow<FCharacterTagDialogueInfo>(RowKeys[i], TEXT(""));

				// �Ƹ��� �� �ε����� Ű���ϰ� ��ġ�� �� ������ �װŶ� ������� �ܼ��� �� �ٸ����� �Ϸù�ȣ�� ����� �д�.
				// ���� ��ü�� �ǹ��ִ� �� �ƴϰ� �� row �� �ĺ��� �� ������ ��.
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
			// ó�� �ε��ϴ� �Ÿ� �ε� �� ĳ���� ����, �̹� �ε��� �Ǿ� �ִ� �Ŷ�� �ʿ��� ã�� ����.
			// �̰� �̹� �ε� �� ���� LoadSynchronous ���� �ణ�̳��� ������ �ϰ����� 
			// �ε� �� ĳ���� ���� ���� �������� ������ �������� ���� ����� �ƴ϶� ������ �������� ���ؼ���.
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
	// �ϰ� ��ε��ε� �ϳ��� �� �� Unload �Ϸ��� ���ϴ� ��ε� ����� Ű���� ������ LoadedPtrMap �ʿ��� �� �ִ� �� �ؾ� �Ѵ�.
	// �Ƹ��� �ϳ��� �� �� Unload �ϴ� �� �ʿ��� �� ������ ������ �� ������ �� Unload �ϴ� �� �ʿ��� ���� �ִ�.

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
	{ // ��ε� �� �ϰ� ��� ���� transition ���� �������� Rootset �� �־����.
		MarkAllLoadedAssetsRootSet();
	}

	// �ٸ� �͵鵵 �̷� ���ε� ĳ�� ���� � ���� ���. ���� �����ؼ�. 
	// �̷��� �ص� ��Ʈ���̸� �������� �ε��� �� �ϰ� ��.
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
			if (ThisAsset.IsValid() && !ThisAsset->IsRooted()) // �ε��Ǿ� �ִ� �͸�.
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