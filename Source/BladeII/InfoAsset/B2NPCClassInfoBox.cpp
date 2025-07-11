// Fill out your copyright notice in the Description page of Project Settings.


#include "B2NPCClassInfoBox.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystem.h"
#include "Materials/Material.h"
#include "Components/DecalComponent.h"
#include "BladeIICharacter.h"
//#include "BladeIINetCharacter.h"
#include "B2DropItem.h"
#include "B2InGameHUDInfo_BossMob.h"
#include "BladeIIGameMode.h"
#include "B2StageGameMode.h"
#include "B2StageInfo.h"
#include "B2StageManager.h"
#include "B2NPCSensitiveInfo.h"
//#include "BladeIIControlNetCharacter.h"
//#include "BladeIIRaidNetCharacter.h"
//#include "BladeIIMercenaryPlayer.h"
//#include "BladeIIGameImpl.h"
//#include "B2AssetLoader.h"
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
#include "BladeIITestDummyNPC.h"
#endif
//#if WITH_EDITOR
//#include "AssetEditorManager.h"
//#endif

#include "FB2ErrorMessage.h"
#include "../BladeII/BladeIIUtil.h"

bool UB2NPCClassInfoBox::bManagedNPCClassUnload = true;
TMap<ENPCClass, bool> UB2NPCClassInfoBox::SpawnPoolAsyncBlacklists;

void UB2NPCClassInfoBox::SetupSpawnPoolAsyncBlacklist()
{
	// �� ������Ʈ�� SpawnPool ���� ���� ���� ���� async �ε����� Ư���� ���ܽ�ų Ŭ������.
	// ù ���� ������ ���� async �ε��� �Ϸ�� �ʿ䰡 �ִµ� ���.. BossMob Master AnimBP ���� �͵� �ϳ��� ���� ������ ũ�� �������Ƿ� �װ͵��� ���ܽ�Ű���� ��.
	//if (SpawnPoolAsyncBlacklists.Num() == 0)
	//{
	//	TArray<FString> ListStr;
	//	GConfig->GetArray(TEXT("/Script/BladeII.B2NPCClassInfo"), TEXT("SpawnPoolAsyncBlacklist"), ListStr, GGameIni);
	//	
	//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASS_ENUM_PACKAGE_NAME, true);
	//	if (EnumPtr)
	//	{
	//		for (const FString& ThisNPCName : ListStr)
	//		{
	//			ENPCClass CastedNPCClassEnum = (ENPCClass)(EnumPtr->GetValueByName(FName(*ThisNPCName)));
	//			if (CastedNPCClassEnum != ENPCClass::ENC_End)
	//			{
	//				SpawnPoolAsyncBlacklists.Add(CastedNPCClassEnum, true); // bool Value �� ���� �ǹ̴� ����.
	//			}
	//		}
	//	}
	//}
}

void UB2NPCClassInfoBox::GetAsyncRequestInfoFromNPCIds(const TArray<FCombinedNPCClassID>& NPCIDs, TArray<FB2AsyncRequestInfo>& OutRequestInfoList)
{ // FCombinedNPCClassID �� �ش��ϴ� AssetReference �� ������. �̰� NPCClassInfo ���� �ε��� �� �� �ִ�.
	//for(const auto& ClassID : NPCIDs)
	//{
	//	auto NPCSingleClassAsset = GetNPCSingleClassAsset(ClassID);
	//	if (NPCSingleClassAsset.IsNull() == false)
	//	{
	//		FSoftObjectPath	AssetString = NPCSingleClassAsset.GetSoftObjectPath();
	//		FB2AsyncRequestInfo		RequestInfo(ClassID, AssetString);

	//		OutRequestInfoList.Add(RequestInfo);
	//	}
	//}
}

TSoftClassPtr<UB2NPCSingleClassInfo> UB2NPCClassInfoBox::GetNPCSingleClassAsset(const FCombinedNPCClassID& ClassID)
{
	// ���⼱ �ε��� ���� �ʴ´�. �ε��� �Ǿ� ���� ���� �ְ� �ƴ� ���� �ִ�.

	if (const FNPCClassInfoSearchMap* SearchMapPtr = NewInfoMap.Find(ClassID.GetNPCClassEnum()))
	{
		if (const auto* AssetClassPtr = SearchMapPtr->VariationAssets.Find(ClassID.GetNPCVariationEnum()))
		{
			return *AssetClassPtr;
		}
	}

	return nullptr;
}

void UB2NPCClassInfoBox::Unload(const FCombinedNPCClassID& NPCClassID)
{
	//if (auto* InfoObj = FindSingleClassInfo(NPCClassID))
	//{
	//	const int32 NPCKey = GetNPCClassKey(NPCClassID);
	//	LoadedPtrMap.Remove(NPCKey);
	//}

	//if (const auto& InfoAsset = GetNPCSingleClassAsset(NPCClassID))
	//{
	//	if (InfoAsset.IsValid())
	//	{ // RootSet ó���� �ϰ� �� �� �����Ƿ� unload ���� ������´�.
	//		InfoAsset->RemoveFromRoot();
	//	}
	//	UnloadAsset(InfoAsset.GetSoftObjectPath());
	//}
}

void UB2NPCClassInfoBox::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList)
{
	TArray<FCombinedNPCClassID> SuccessivePhaseClasses; // �ε��� ���� ������ �� �� �ִ� Phase Ŭ������ �ִ�. ���� ���ϴ� ��Ȳ�� �ƴϰ�����..
	for (const auto& RequestInfo : CompleteRequestList)
	{ // �ٸ� ��η� ���� �̹� �ε��� �Ǿ� ���� �� ����.
		UObject*& FoundOrAddCache = LoadedPtrMap.FindOrAdd(RequestInfo.AssetKey);
		FoundOrAddCache = RequestInfo.StringAsset.ResolveObject();
		
		UClass* CastedClass = Cast<UClass>(FoundOrAddCache);
		if (CastedClass)
		{
			UB2NPCSingleClassInfo* CastedInfo = Cast<UB2NPCSingleClassInfo>(CastedClass->GetDefaultObject());
			if (CastedInfo)
			{
				CastedInfo->GetAllExpectedPhaseClasses(SuccessivePhaseClasses);
			}
		}
	}
	
	if(SuccessivePhaseClasses.Num() > 0)
	{
		// Phase Ŭ���� �߰� �ε��� �ʿ��� ���. �÷��� ���� ���� Async �ε��� ���޾� �Ǵ� ��Ȳ�̸� flush �� ����� ȯ�濡 ���̰�����.. ��¿ �� ����.. �� �Ѥ�;
		TArray<FB2AsyncRequestInfo> SuccessivePhaseClassAssets;
		GetAsyncRequestInfoFromNPCIds(SuccessivePhaseClasses, SuccessivePhaseClassAssets);
		TryAsyncLoad(FString::Printf(TEXT("Successive PhaseClass from %s"), *CompleteRequest), SuccessivePhaseClassAssets);
	}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (UB2AssetLoader::bEnableDevLog)
	{
		if (CompleteRequestList.Num() > 0)
		{
			FString LogMsg = FString::Printf(TEXT("[NPCClassInfo] Async request of %d assets are completed and cached."), CompleteRequestList.Num());
			if(bDevTestLastAsyncReqTimeMarked && DevTestLastAsyncReqTimeName == CompleteRequest)
			{ 
				LogMsg += FString::Printf(TEXT(" Time %.2f s."), FPlatformTime::Seconds() - DevTestLastAsyncReqTime);
				bDevTestLastAsyncReqTimeMarked = false;
				DevTestLastAsyncReqTimeName.Empty();
				DevTestLastAsyncReqTime = 0.0;
			}
			BII_SCREEN_LOG(LogMsg, FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 14, 8.0f);
		}

		if (SuccessivePhaseClasses.Num() > 0)
		{
			BII_SCREEN_LOG(FString::Printf(TEXT("[NPCClassInfo] %d phase class data is additionally request."), SuccessivePhaseClasses.Num()),
				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 14, 8.0f);
		}
	}
#endif
}

UB2NPCClassInfoBox::UB2NPCClassInfoBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bDevTestLastAsyncReqTimeMarked = false;
	DevTestLastAsyncReqTime = 0.0;
#endif
	EstimatedRootedAssetNum = 0;
	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII.B2NPCClassInfo"), TEXT("ManagedNPCClassUnload"), bManagedNPCClassUnload, GGameIni);
	}
}

void UB2NPCClassInfoBox::PostLoad()
{
	Super::PostLoad();

	SetupSpawnPoolAsyncBlacklist();

//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		FAssetEditorManager::Get().OnAssetEditorOpened().AddUObject(this, &UB2NPCClassInfoBox::OnAssetEditorOpen);
//	}
//#endif
}

// TAsset lazy-loading �� ����ϴ� Blueprint InfoAsset �� ������ ������ �������� �ذ��ϱ� �������� �ӽ� ����. -> �ӽð� �ƴ� ��
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2NPCClassInfoBox::CleanupOnPreSave()
{
	LoadedPtrMap.Empty();
}
void UB2NPCClassInfoBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
	CleanupOnPreSave();
}
#endif

/**
 * �Ϲ����� NPCSingleClassInfo ���� �������̽�. �ε��� �� �Ǿ� �ִٸ� ���⼭ Synchronous �ε��� �� ��. (async ��û�� �ߴٸ� �Ϸ���� ���)
 */
UB2NPCSingleClassInfo* UB2NPCClassInfoBox::GetNPCSingleClassInfo(ENPCClass InClass, ENPCClassVariation InVariation)
{
	//if (InClass == ENPCClass::ENC_End)
	//	return nullptr;

	//FCombinedNPCClassID NPCClassID(InClass, InVariation);
	//if (auto* LoadedObject = FindSingleClassInfo(NPCClassID))
	//{
	//	return LoadedObject;
	//}

	//else // need to load
	//{
	//	const FCombinedNPCClassID ClassID(InClass, InVariation);
	//	auto AssetClass = GetNPCSingleClassAsset(ClassID);
	//	if (AssetClass.IsNull() == false) // ���� IsNull �� �ε��� �ȵǾ� ������ ���ϴ� �� �ƴ϶� ���� �Ҵ��� �ȵǾ� ������ ����
	//	{
	//		const FSoftObjectPath& StringAsset = AssetClass.GetSoftObjectPath();
	//		UClass* SingleClassInfo = Cast<UClass>(LoadSynchronous(StringAsset, GENERAL_TASSET_ASYNC_LOAD_TIMEOUT));
	//		auto* CastedAsset = Cast<UB2NPCSingleClassInfo>(SingleClassInfo->GetDefaultObject());

	//		const int32 NPCKey = GetNPCClassKey(ClassID.GetNPCClassEnum(), ClassID.GetNPCVariationEnum());
	//		LoadedPtrMap.Add(NPCKey, CastedAsset);

	//		return CastedAsset;
	//	}
	//}

	return nullptr;
}

// �ε� �Ǿ� �ִ� �� ã�� ��.
UB2NPCSingleClassInfo* UB2NPCClassInfoBox::FindSingleClassInfo(const FCombinedNPCClassID& ClassID)
{
	const int32 NPCKey = GetNPCClassKey(ClassID.GetNPCClassEnum(), ClassID.GetNPCVariationEnum());

	if (UObject** NPCInfoAssetPtr = LoadedPtrMap.Find(NPCKey))
	{
		if(UClass* NPCInfoAsset = Cast<UClass>(*NPCInfoAssetPtr))
			return Cast<UB2NPCSingleClassInfo>(NPCInfoAsset->GetDefaultObject());
	}

	return nullptr;
}

template<typename T>
ABladeIICharacter* SpawnCharacter(UB2NPCClassInfoBox* InfoBox, UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation, const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters)
{
	//if (SpawnWorld && InClass != ENPCClass::ENC_End) // Even for the ENC_End, we can still spawn though..
	//{
	//	UB2NPCSingleClassInfo* ClassInfo = InfoBox->GetNPCSingleClassInfo(InClass, InVariation);
	//	BII_CHECK(ClassInfo);
	//	if (ClassInfo)
	//	{
	//		// Ư�� OverrideByBaseBP �� ���ؼ� bDeferPostSpawnInitialize �� �ϴ� ���� ����. 
	//		// PCClassInfo spawn �� ���� �̰� �ʼ��ε� NPCClassInfo �� ���� �ʿ���� ���̱⵵..
	//		FActorSpawnParameters FinalSpawnParameter = SpawnParameters;
	//		FinalSpawnParameter.bDeferPostSpawnInitialize = true;
	//		// �� NPCClassInfo �� ����� �������� SpawnParameters ���� bDeferConstruction �� ���õǾ� �־�� ��. �׷��� BeginPlay �� �� ���� �Ҹ��� ��. �̰� ����ϴ� �κ�(AB2MonsterSpawnPool::SpawnSingleMonsterCommon ��).
	//		BII_CHECK(FinalSpawnParameter.bDeferConstruction); // Ȥ�ö� bDeferConstruction �� ������� �ʴ� ��찡 ����� �׽�Ʈ �� �̰� �����ϵ���. �Ϲ����� �ΰ��� ĳ���ʹ� �ƴ� ���̴�.

	//		ABladeIICharacter* SpawnedActor = Cast<ABladeIICharacter>(SpawnWorld->SpawnActor(T::StaticClass(), &SpawnTransform, FinalSpawnParameter));
	//		BII_CHECK(SpawnedActor && SpawnedActor->IsValidObj());
	//		if (SpawnedActor)
	//		{
	//			SpawnedActor->OverrideByNPCClassInfo(InClass, ClassInfo); // �������� ClassInfo ���� ����.
	//			SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParameter); // bDeferPostSpawnInitialize �� ���� �߰� ó��. 
	//			// Added Initial Buff handling
	//			if (ClassInfo->InfluenceType != EInfluenceType::EIT_End)
	//			{
	//				SpawnedActor->InitialInfluence = ClassInfo->InfluenceType;
	//				SpawnedActor->InitialInfluenceAmount = ClassInfo->InfluenceAmount;
	//				SpawnedActor->InitialInfluenceDuration = ClassInfo->InfluenceDuration;
	//				SpawnedActor->InitialInfluenceStateDuration = ClassInfo->InfluenceStateDuration;
	//				SpawnedActor->InitialInfluenceStateTriggerRate = ClassInfo->InitialInfluenceStateTriggerRate;
	//			}
	//			return SpawnedActor;
	//		}
	//	}
	//}

	return NULL;
}

ABladeIICharacter* UB2NPCClassInfoBox::SpawnNPCClassInfoCharacter(
	UWorld* SpawnWorld, 
	ENPCClass InClass, 
	ENPCClassVariation InVariation,
	const FTransform& SpawnTransform, 
	FActorSpawnParameters& SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2NPCClassInfoBox::SpawnNPCClassInfoCharacter"));
	return SpawnCharacter<ABladeIICharacter>(this, SpawnWorld, InClass, InVariation, SpawnTransform, SpawnParameters);
}

ABladeIICharacter* UB2NPCClassInfoBox::SpawnNPCClassInfoNetCharacter(
	UWorld* SpawnWorld, 
	ENPCClass InClass, 
	ENPCClassVariation InVariation, 
	const FTransform& SpawnTransform,
	FActorSpawnParameters& SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2NPCClassInfoBox::SpawnNPCClassInfoNetCharacter"));
	//return SpawnCharacter<ABladeIINetCharacter>(this, SpawnWorld, InClass, InVariation, SpawnTransform, SpawnParameters);
	return NULL;
}

ABladeIICharacter* UB2NPCClassInfoBox::SpawnNPCClassInfoRaidNetCharacter(class UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation, const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2NPCClassInfoBox::SpawnNPCClassInfoControlNetCharacter"));
	//return SpawnCharacter<ABladeIIRaidNetCharacter>(this, SpawnWorld, InClass, InVariation, SpawnTransform, SpawnParameters);
	return NULL;
}

ABladeIICharacter* UB2NPCClassInfoBox::SpawnNPCClassInfoControlNetCharacter(
	UWorld* SpawnWorld,
	ENPCClass InClass,
	ENPCClassVariation InVariation,
	const FTransform& SpawnTransform,
	FActorSpawnParameters& SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2NPCClassInfoBox::SpawnNPCClassInfoControlNetCharacter"));
	//return SpawnCharacter<ABladeIIControlNetCharacter>(this, SpawnWorld, InClass, InVariation, SpawnTransform, SpawnParameters);
	return NULL;
}

ABladeIICharacter * UB2NPCClassInfoBox::SpawnNPCClassInfoGuildCharacter(UWorld * SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation, const FTransform & SpawnTransform, FActorSpawnParameters & SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2NPCClassInfoBox::SpawnNPCClassInfoControlNetCharacter"));
	//return SpawnCharacter<ABladeIIMercenaryPlayer>(this, SpawnWorld, InClass, InVariation, SpawnTransform, SpawnParameters);
	return NULL;
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
ABladeIITestDummyNPC* UB2NPCClassInfoBox::SpawnNPCClassInfoDummyCharacter(class UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation,
	const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2NPCClassInfoBox::SpawnNPCClassInfoDummyCharacter"));
	return Cast<ABladeIITestDummyNPC>(SpawnCharacter<ABladeIITestDummyNPC>(this, SpawnWorld, InClass, InVariation, SpawnTransform, SpawnParameters));
}
#endif

void UB2NPCClassInfoBox::SyncPreloadClassAssets(const TArray<FCombinedNPCClassID>& InClasses)
{
#if !UE_BUILD_SHIPPING
	int32 AllLoadedCount = 0;
#endif

	for (int32 CI = 0; CI < InClasses.Num(); ++CI)
	{
		const FCombinedNPCClassID& ThisLoadClass = InClasses[CI];
		GetNPCSingleClassInfo(ThisLoadClass.GetNPCClassEnum(), ThisLoadClass.GetNPCVariationEnum());
#if !UE_BUILD_SHIPPING
		AllLoadedCount++;
#endif
	}

#if !UE_BUILD_SHIPPING
	if (AllLoadedCount > 0){
		UE_LOG(LogBladeII, Log, TEXT("NPCClassInfo pre-load for %d assets"), AllLoadedCount);
	}
#endif	
}

void UB2NPCClassInfoBox::UnloadAll()
{
	for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
	{
		FNPCClassInfoSearchMap& ThisInfoMap = InfoMapIt.Value();
		for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(ThisInfoMap.VariationAssets); VariationIt; ++VariationIt)
		{
			FCombinedNPCClassID NPCClassID(InfoMapIt.Key(), VariationIt.Key());
			Unload(NPCClassID);
		}		
	}
	LoadedPtrMap.Empty(); // Ȯ�λ��
}
void UB2NPCClassInfoBox::UnloadExcept(const TMap<FCombinedNPCClassID, bool>& InExceptIDs)
{
	for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
	{
		FNPCClassInfoSearchMap& ThisInfoMap = InfoMapIt.Value();
		for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(ThisInfoMap.VariationAssets); VariationIt; ++VariationIt)
		{
			FCombinedNPCClassID NPCClassID(InfoMapIt.Key(), VariationIt.Key());
			if (!InExceptIDs.Find(NPCClassID))
			{
				Unload(NPCClassID);
			}
		}
	}
	// LoadedPtrMap �� ���⼭ ������� �ʴ´�.
}
void UB2NPCClassInfoBox::UnloadExceptCurrentExplicit(ABladeIIGameMode* CurrGameMode)
{ 
	//if (CurrGameMode && CurrGameMode->GetB2GameModeType() == EB2GameMode::Lobby)
	//{
	//	// ������ �κ񿡼� NPCClassInfo �� ����ϰ� �Ǹ� �� ������ Ʋ���� �Ƿ���.
	//	// LoadedPtrMap �� ��������� �ִ� �� ������ ���� ������ RootSet ó���� �� �� �ִٸ� �װ� �ǵ帮���� �ʴ´�.
	//	check(LoadedPtrMap.Num() == 0); 
	//}
	//else if (ShouldUnloadAllOnLevelTransition())
	//{
	//	UnloadAll();
	//}
	//else
	//{
	//	// ���� LoadedPtrMap �� ĳ�̵� �͵��� RootSet ó���� �ϰ�
	//	// �װ� �ƴ� �͵��� RemoveFromRoot ó���� �ؼ� Ȯ���� �������� �Ѵ�.

	//	// ���� ��������� �ε��� ���� ���� ���´�.. ������ NPC �� ���� ���Ӹ��ų� (���� ��� PVP), �ƴϸ� ���Ӹ�� ������ �� ���� �ι� �̻� ������ ��.
	//	// � ���� Unload �� �������� �ʴ´�. ���������� RootSet �� �־���� �͵��� �ִٸ� ������ ��.
	//	if (LoadedPtrMap.Num() > 0)
	//	{
	//		EstimatedRootedAssetNum = 0; // �ٽ� ����..

	//		TMap<FCombinedNPCClassID, bool> AllExceptIDs;
	//		// ���� LoadedPtrMap �� �ִٴ� �� �̹� �������� ��������� �ε��ߴٴ� �ɷ� �����ϰ� ��Ʈ�¿� ���ܵη��� ��.
	//		// ���� �������� ��������� �ε����� �ʴ��� �ε��� ���°����� LoadedPtrMap �� ���Ե����� ���� ���̴�. �׷��� �����ְ� �Ǹ� ���� ������ ������ �� unload �� ��.
	//		for (TMap<uint32, UObject*>::TIterator LoadedIt(LoadedPtrMap); LoadedIt; ++LoadedIt)
	//		{
	//			FCombinedNPCClassID ThisLoadedId;

	//			FromNPCClassKey(LoadedIt.Key(), ThisLoadedId);
	//			AllExceptIDs.Add(ThisLoadedId, true); // bool value �� �ǹ̴� ����.

	//			// ��Ʈ�� ó���� ���⼭ ��, �ε� �Ǿ��� �� �Ϸ��� �Ѱ��� �ƴϱ⵵ �ϰ� �ؼ�..
	//			const TSoftClassPtr<UB2NPCSingleClassInfo>& LoadedAssetClass = GetNPCSingleClassAsset(ThisLoadedId);
	//			check(LoadedAssetClass.IsValid());
	//			if (LoadedAssetClass.IsValid())
	//			{
	//				LoadedAssetClass->AddToRoot(); // ���� �������� ��������� �ε����� �ʴ´ٸ� �������� ���� ��ġ�鼭 RootSet ���� ���ŵǰ� ��ε� �� ���̴�.
	//				++EstimatedRootedAssetNum;
	//			}
	//		}
	//		UnloadExcept(AllExceptIDs);
	//	}
	//}
	LoadedPtrMap.Empty(); // ����Ǿ��� ������� RootSet �� �� �͵��� �ε��� ���¸� ������ ��.
}

bool UB2NPCClassInfoBox::ShouldUnloadAllOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �� �޸� ��ĥ ���� ��������..
	return true;
#else
	return !bManagedNPCClassUnload;
#endif
}

bool UB2NPCClassInfoBox::IsInSpawnPoolAsyncBlacklist(ENPCClass InCheckClass)
{
	return (SpawnPoolAsyncBlacklists.Find(InCheckClass) != nullptr);
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void UB2NPCClassInfoBox::DevTestMarkLastAsyncReqTime(const FString& InReqName)
{ // UB2AssetLoader::bEnableDevLog �� Async �α� �Ѹ� �� �Ҹ� �ð��� ���� �Ѹ��� ������ ��
	// ������ ��û�� ��ø�Ǿ��� �� ������ üũ������ ����. �˾Ƽ� �� ��� ��
	bDevTestLastAsyncReqTimeMarked = true;
	DevTestLastAsyncReqTimeName = InReqName;
	DevTestLastAsyncReqTime = FPlatformTime::Seconds();
}
#endif

#if WITH_EDITOR
UB2NPCClassInfoBox::NPCClassPreviewAssetInfo UB2NPCClassInfoBox::GetPreviewAssetInfoFromClass(ENPCClass InClass, ENPCClassVariation InVariation)
{
	NPCClassPreviewAssetInfo RetInfo;

	UB2NPCSingleClassInfo* NPCInfo = GetNPCSingleClassInfo(InClass, InVariation);
	BII_CHECK(NPCInfo);

	//if (NPCInfo)
	//{
	//	if (NPCInfo->MeshAssetOverride)
	//	{
	//		RetInfo.SKMesh = NPCInfo->MeshAssetOverride;
	//	}
	//	else
	//	{
	//		// If not overridden, then get it from the Blueprint class.
	//		// GetBaseBPClass �� null �� �Ѱ��شٰ� �ؼ� ���� �۵� ���ϴ� �� �ƴϰ� asyn �ε� flush �� �Ͼ�� �� ����ϴ� �ǵ� �����Ϳ��� �׷��� �ž� ������ �� ����.
	//		TSubclassOf<class ABladeIICharacter> NPCInfoBaseBPClass = NPCInfo->GetBaseBPClass(nullptr);
	//		if (NPCInfoBaseBPClass)
	//		{
	//			ABladeIICharacter* DefaultCharacterObj = Cast<ABladeIICharacter>(NPCInfoBaseBPClass->GetDefaultObject());
	//			if (DefaultCharacterObj && DefaultCharacterObj->GetMesh())
	//			{
	//				RetInfo.SKMesh = DefaultCharacterObj->GetMesh()->SkeletalMesh;
	//			}
	//		}
	//	}
	//	RetInfo.MaterialOverrides = NPCInfo->MaterialOverrides;
	//}

	return RetInfo;
}

void UB2NPCClassInfoBox::OnAssetEditorOpen(UObject* InAsset)
{
	UBlueprint* CastedBPObj = Cast<UBlueprint>(InAsset);

	// this �� ���߰� ������ ������ �ʴ�.. �Ф�
	if (CastedBPObj && CastedBPObj->ParentClass && CastedBPObj->ParentClass->GetName() == TEXT("B2NPCClassInfoBox"))
	{ // �⺻ EditorLoadAll �� ���� �ʰ� �Ǹ鼭 ���� ������ �� �� �ε��ϵ��� ��.

		// �̰� �� �ص� �̹� �� �� ���ư��� ��..
		//FScopedSlowTask SlowTask(100.0f, FText::FromString(FString::Printf(TEXT("It might fully load all NPCClassInfo. Please wait..."))));

		EditorLoadAll(nullptr, 0.0f, 0.0f);
		CheckInfoDataIntegrity();
	}
}

void UB2NPCClassInfoBox::EditorLoadAll(SetB2ContentLoadingProgressSplashFnPtr InSplashProgFnPtr, float InStartProgress, float InEndProgress)
{
	if (GIsEditor == false)
	{
		return;
	}

	const float TotalProgRange = FMath::Max(0.0f, InEndProgress - InStartProgress);
	// InfoAsset �� ��ϵǴ� �ֵ��� �󸶳� �Ǵ����� ���� ������ ���ϴ� ��. � element �ε����� �ѹ��� ������Ʈ �� ��.
	const int32 ProgUpdateInterval = 2;
	const int32 TotalProgUpdateCount = NewInfoMap.Num() / ProgUpdateInterval;
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	int32 InfoLoadCount = 0;
	// �ѹ� �̸� �ҷ��ָ� TAsset ���� valid �� �� ��. ���� �� ���߿� �޸𸮴� ��â �԰���.
	for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
	{
		FNPCClassInfoSearchMap& ThisInfoMap = InfoMapIt.Value();

		for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(ThisInfoMap.VariationAssets); VariationIt; ++VariationIt)
		{
			ENPCClassVariation ThisVariation = VariationIt.Key();
			// �ٽ��� ��Ű� ������ progress ����� ����.
			GetNPCSingleClassInfo(ThisInfoMap.NPCClassEnum, ThisVariation);
		}
				
		if (InSplashProgFnPtr && (InfoLoadCount % ProgUpdateInterval == ProgUpdateInterval - 1))
		{
			CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount);
			InSplashProgFnPtr(CurrProg);
		}

		++InfoLoadCount; // Variation ������ �ƴ϶� Class ������ ī��Ʈ �Ѵ�. Variation ���������� ó������ �ε��ؾ� �� ���� ���� ���ؼ�.
	}

	CheckInfoDataIntegrity();
}

void UB2NPCClassInfoBox::CheckInfoDataIntegrity()
{
//	// Check for matching NPCSensitiveInfo entry
//	UB2NPCSensitiveInfo* NPCSensitiveInfo = StaticFindNPCSensitiveInfoTable();
//	const UEnum* NPCClassEnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASS_ENUM_PACKAGE_NAME, true);
//	const UEnum* NPCVaryEnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASSVARI_ENUM_PACKAGE_NAME, true);
//	if (NPCSensitiveInfo && NPCClassEnumPtr && NPCVaryEnumPtr)
//	{
//#if !PLATFORM_MAC
//		FString WarnMessage = TEXT("���� NPCClassInfo �׸� ���� SensitiveInfo �׸��� �����Ǿ��ų� SensitiveDataRowKey �� ���� ����.\n\n");
//#endif
//
//		// ��� SensitiveInfo ���� ��ġ üũ�� ���� �׸� �߿��� �� �ƴ�. ���� ���߹��������� ���̴� �Ŷ�..
//
//		int32 FoundNullEntryNum = 0;
//		for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
//		{
//			FNPCClassInfoSearchMap& ThisInfoMap = InfoMapIt.Value();
//			for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(ThisInfoMap.VariationAssets); VariationIt; ++VariationIt)
//			{
//				const FCombinedNPCClassID NPCClassID(InfoMapIt.Key(), VariationIt.Key());
//				if (auto* NPCClassInfo = GetNPCSingleClassInfo(NPCClassID))
//				{
//					if(NPCClassInfo->ClassVariation == ENPCClassVariation::ENCV_GuildBattle)
//						continue;
//					if (NPCSensitiveInfo->GetInfoData(NPCClassID.GetNPCClassEnum(), NPCClassInfo) == nullptr)
//					{
//						++FoundNullEntryNum;
//#if !PLATFORM_MAC
//						WarnMessage += FString::Printf(TEXT("Class %s, %s\n"),
//							*NPCClassEnumPtr->GetNameStringByIndex((int32)NPCClassInfo->NPCClassEnum),
//							*NPCVaryEnumPtr->GetNameStringByIndex((int32)NPCClassInfo->ClassVariation));
//#endif
//					}
//				}
//			}
//		}
//
//#if !PLATFORM_MAC
//		if (FoundNullEntryNum > 0)
//		{
//			WarnMessage += TEXT("\n��ǻ�Ͱ� ���������� ���� ���̴� ������ �̿ܿ��� �Ƚ��ϰ� �۾��� �����ϼ���.");
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessage));
//		}
//#endif
//	}
//
//	CheckAllNPCClassEnumSettingValid();
}

void UB2NPCClassInfoBox::CheckAllNPCClassEnumSettingValid()
{

//	TArray<FCombinedNPCClassID> InvalidSettings;
//	if (GatherInvalidSetting(InvalidSettings))
//	{
//#if !PLATFORM_MAC
//		const UEnum* NPCClassEnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASS_ENUM_PACKAGE_NAME, true);
//		FString ErrorMsg(TEXT("[Warning] NPCClassInfoBox �� ���� ClassInfo �� NPCClassEnum ���� ���� ����ġ�� ����. ��ǻ�Ͱ� �� �����Ѵ�.\r\n\r\n"));
//		for (int32 EI = 0; EI < InvalidSettings.Num(); ++EI)
//		{
//			ErrorMsg += FString::Printf(TEXT("%d. %s\r\n"), EI + 1, NPCClassEnumPtr ? *(NPCClassEnumPtr->GetNameStringByValue((int32)InvalidSettings[EI].GetNPCClassEnum())) : TEXT("SomeClass"));
//		}
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(ErrorMsg));
//#endif
//	}
}

UB2NPCSingleClassInfo* UB2NPCClassInfoBox::TryGetMatchingClassInfo(FName InSensitiveDataKey)
{
	// ���� ��뿡���� �ݴ�� ClassInfo �ʿ��� SensitiveInfo ������ SensitiveDataRowKey �� ����ؼ� ����.
	// �̰� ������ ������μ� �ܼ� ���� search
	for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
	{
		FNPCClassInfoSearchMap& ThisInfoMap = InfoMapIt.Value();
		for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(ThisInfoMap.VariationAssets); VariationIt; ++VariationIt)
		{
			const FCombinedNPCClassID NPCClassID(InfoMapIt.Key(), VariationIt.Key());
			UB2NPCSingleClassInfo* ThisVariationInfo = GetNPCSingleClassInfo(NPCClassID);

			if (ThisVariationInfo && ThisVariationInfo->SensitiveDataRowKey == InSensitiveDataKey)
				return ThisVariationInfo;
		}
	}
	return nullptr;
}

void UB2NPCClassInfoBox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_NPCClassEnum = GET_MEMBER_NAME_CHECKED(FNPCClassInfoSearchMap, NPCClassEnum);
	
	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	// ��ϵ� ���µ��� �ε� �� �� ���¿����� ���� �ɸ��µ�.. �̸� �� �� �ε��� �ʿ�� ����..
	CheckInfoDataIntegrity();
}

void UB2NPCClassInfoBox::FirstTimeNPCSensitiveInfoConstruction()
{ 
	//// NPCSensitiveInfo ���� �������� NPCClassInfo �� �־� �ξ��� �����͵��� NPCSensitiveInfo �� �ڵ� �̰��ϱ� ���� �ӽ� ���

	//UB2NPCSensitiveInfo* NPCSensitiveInfo = StaticFindNPCSensitiveInfoTable();

	//for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
	//{
	//	FNPCClassInfoSearchMap& ThisInfoMap = InfoMapIt.Value();
	//	for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(ThisInfoMap.VariationAssets); VariationIt; ++VariationIt)
	//	{
	//		FCombinedNPCClassID NPCClassID(InfoMapIt.Key(), VariationIt.Key());

	//		if (UB2NPCSingleClassInfo* NPCClassInfo = GetNPCSingleClassInfo(NPCClassID))
	//		{
	//			FSingleNPCSensitiveData* SensitiveData = new FSingleNPCSensitiveData();

	//			SensitiveData->MatchingNPCClass = NPCClassInfo->NPCClassEnum;
	//			SensitiveData->MatchingNPCClassVari = NPCClassInfo->ClassVariation;

	//			FName SuggestedSensitiveDataKeyName;
	//			if (NPCSensitiveInfo->FirstTimeConstruction_EditorAddInfoData(NPCClassInfo->NPCClassEnum, NPCClassInfo->ClassVariation, SensitiveData, SuggestedSensitiveDataKeyName))
	//			{
	//				// SensitiveDataRowKey �� �׵��� �׳� ������ �Ǿ� �ִ� ���� ���� �����ϰ� �Ǿ� �ִٰų� ������ �ߺ��� �Ǿ� �ִ� �͵� ����.
	//				// ���̺��� ó�� �����ϱ� ���� ����̴� �⺻ ���� ������ ������ �ش�.
	//				if (NPCClassInfo->SensitiveDataRowKey != SuggestedSensitiveDataKeyName)
	//				{
	//					NPCClassInfo->SensitiveDataRowKey = SuggestedSensitiveDataKeyName;
	//					NPCClassInfo->MarkPackageDirty();
	//				}
	//			}
	//		}
	//	}
	//}
}

bool UB2NPCClassInfoBox::GatherInvalidSetting(TArray<FCombinedNPCClassID>& OutInvalidTypes)
{
	OutInvalidTypes.Empty();

	for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
	{
		FNPCClassInfoSearchMap& NPCSpecificMap = InfoMapIt.Value();
		for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(NPCSpecificMap.VariationAssets); VariationIt; ++VariationIt)
		{
			FCombinedNPCClassID NPCClassID(NPCSpecificMap.NPCClassEnum, VariationIt.Key());
			if (auto* NPCClassInfo = GetNPCSingleClassInfo(NPCClassID))
			{
				bool IsSameType		= NPCClassInfo->NPCClassEnum == NPCClassID.GetNPCClassEnum();
				bool IsSameVariation = NPCClassInfo->ClassVariation == NPCClassID.GetNPCVariationEnum();

				if ((IsSameType && IsSameVariation) == false)
					OutInvalidTypes.Add(NPCClassID);
				
			}
		}
	}

	return OutInvalidTypes.Num() > 0 ? true : false;
}

#endif	// WITH_EDITOR


int32 GetNPCClassKey(ENPCClass InClassEnum, ENPCClassVariation InClassVariation)
{ // �ε��� UB2NPCSingleClassInfo ���۷��� ĳ���� �� Ű. �����ϰ� �Ǹ� �Ʒ� FromNPCClassKey ��.
	return (static_cast<int32>(InClassEnum) * 100 + static_cast<int32>(InClassVariation));
}
void FromNPCClassKey(int32 InKeyValue, ENPCClass& OutClassEnum, ENPCClassVariation& OutClassVariation)
{ // GetNPCClassKey �� �ݴ�
	OutClassEnum = static_cast<ENPCClass>(InKeyValue / 100);
	OutClassVariation = static_cast<ENPCClassVariation>(InKeyValue % 100);
}
int32 GetNPCClassKey(const FCombinedNPCClassID& NPCClassID)
{ // �ε��� UB2NPCSingleClassInfo ���۷��� ĳ���� �� Ű.
	return GetNPCClassKey(NPCClassID.GetNPCClassEnum(), NPCClassID.GetNPCVariationEnum());
}
void FromNPCClassKey(int32 InKeyValue, FCombinedNPCClassID& OutNPCClassID)
{
	ENPCClass ClassEnum = ENPCClass::ENC_End;
	ENPCClassVariation ClassVar = ENPCClassVariation::ENCV_Normal;
	FromNPCClassKey(InKeyValue, ClassEnum, ClassVar);
	OutNPCClassID = FCombinedNPCClassID(ClassEnum, ClassVar);
}