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
	// 요 블랙리스트는 SpawnPool 에서 게임 시작 이후 async 로딩에서 특별히 제외시킬 클래스들.
	// 첫 연출 끝나기 전에 async 로딩이 완료될 필요가 있는데 몇몇.. BossMob Master AnimBP 쓰는 것들 하나둘 끼어 있으면 크게 느려지므로 그것들은 제외시키려는 거.
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
	//				SpawnPoolAsyncBlacklists.Add(CastedNPCClassEnum, true); // bool Value 는 딱히 의미는 없음.
	//			}
	//		}
	//	}
	//}
}

void UB2NPCClassInfoBox::GetAsyncRequestInfoFromNPCIds(const TArray<FCombinedNPCClassID>& NPCIDs, TArray<FB2AsyncRequestInfo>& OutRequestInfoList)
{ // FCombinedNPCClassID 에 해당하는 AssetReference 를 가져옴. 이걸 NPCClassInfo 에셋 로딩에 쓸 수 있다.
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
	// 여기선 로딩을 하지 않는다. 로딩이 되어 있을 수도 있고 아닐 수도 있다.

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
	//	{ // RootSet 처리를 하게 될 수 있으므로 unload 전에 떼어놓는다.
	//		InfoAsset->RemoveFromRoot();
	//	}
	//	UnloadAsset(InfoAsset.GetSoftObjectPath());
	//}
}

void UB2NPCClassInfoBox::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList)
{
	TArray<FCombinedNPCClassID> SuccessivePhaseClasses; // 로딩해 놓고 나서야 알 수 있는 Phase 클래스가 있다. 자주 접하는 상황은 아니겠지만..
	for (const auto& RequestInfo : CompleteRequestList)
	{ // 다른 경로로 인해 이미 로딩이 되어 있을 수 있음.
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
		// Phase 클래스 추가 로딩이 필요한 경우. 플레이 시작 이후 Async 로딩이 연달아 되는 상황이면 flush 에 취약한 환경에 놓이겠지만.. 어쩔 수 없지.. 쩝 ㅡㅡ;
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

// TAsset lazy-loading 을 사용하는 Blueprint InfoAsset 의 오동작 문제를 정식으로 해결하기 전까지의 임시 땜질. -> 임시가 아닌 듯
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2NPCClassInfoBox::CleanupOnPreSave()
{
	LoadedPtrMap.Empty();
}
void UB2NPCClassInfoBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 의도치 않게 저장된 레퍼런스를 날려준다.
	CleanupOnPreSave();
}
#endif

/**
 * 일반적인 NPCSingleClassInfo 접근 인터페이스. 로딩이 안 되어 있다면 여기서 Synchronous 로딩이 될 것. (async 요청을 했다면 완료까지 대기)
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
	//	if (AssetClass.IsNull() == false) // 여기 IsNull 은 로딩이 안되어 있음을 뜻하는 게 아니라 에셋 할당이 안되어 있음을 뜻함
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

// 로딩 되어 있는 거 찾는 거.
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
	//		// 특히 OverrideByBaseBP 를 위해선 bDeferPostSpawnInitialize 를 하는 것이 좋다. 
	//		// PCClassInfo spawn 의 경우는 이게 필수인데 NPCClassInfo 의 경우는 필요없어 보이기도..
	//		FActorSpawnParameters FinalSpawnParameter = SpawnParameters;
	//		FinalSpawnParameter.bDeferPostSpawnInitialize = true;
	//		// 이 NPCClassInfo 가 제대로 먹히려면 SpawnParameters 에서 bDeferConstruction 이 세팅되어 있어야 함. 그래야 BeginPlay 가 이 이후 불리게 됨. 이걸 사용하는 부분(AB2MonsterSpawnPool::SpawnSingleMonsterCommon 등).
	//		BII_CHECK(FinalSpawnParameter.bDeferConstruction); // 혹시라도 bDeferConstruction 을 사용하지 않는 경우가 생기면 테스트 후 이걸 제거하도록. 일반적인 인게임 캐릭터는 아닐 것이다.

	//		ABladeIICharacter* SpawnedActor = Cast<ABladeIICharacter>(SpawnWorld->SpawnActor(T::StaticClass(), &SpawnTransform, FinalSpawnParameter));
	//		BII_CHECK(SpawnedActor && SpawnedActor->IsValidObj());
	//		if (SpawnedActor)
	//		{
	//			SpawnedActor->OverrideByNPCClassInfo(InClass, ClassInfo); // 실질적인 ClassInfo 적용 과정.
	//			SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParameter); // bDeferPostSpawnInitialize 에 따른 추가 처리. 
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
	LoadedPtrMap.Empty(); // 확인사살
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
	// LoadedPtrMap 을 여기서 비우지는 않는다.
}
void UB2NPCClassInfoBox::UnloadExceptCurrentExplicit(ABladeIIGameMode* CurrGameMode)
{ 
	//if (CurrGameMode && CurrGameMode->GetB2GameModeType() == EB2GameMode::Lobby)
	//{
	//	// 언젠가 로비에서 NPCClassInfo 를 사용하게 되면 이 가정이 틀리게 되려나.
	//	// LoadedPtrMap 에 명시적으로 있는 건 없더라도 이전 레벨서 RootSet 처리를 한 게 있다면 그걸 건드리지는 않는다.
	//	check(LoadedPtrMap.Num() == 0); 
	//}
	//else if (ShouldUnloadAllOnLevelTransition())
	//{
	//	UnloadAll();
	//}
	//else
	//{
	//	// 현재 LoadedPtrMap 에 캐싱된 것들은 RootSet 처리를 하고
	//	// 그게 아닌 것들은 RemoveFromRoot 처리를 해서 확실히 내려가게 한다.

	//	// 현재 명시적으로 로딩된 것이 없는 상태는.. 실제로 NPC 가 없는 게임모드거나 (예를 들어 PVP), 아니면 게임모드 떠나기 전 여길 두번 이상 진입할 때.
	//	// 어떤 경우든 Unload 를 실행하지 않는다. 마지막으로 RootSet 에 넣어놓은 것들이 있다면 유지될 것.
	//	if (LoadedPtrMap.Num() > 0)
	//	{
	//		EstimatedRootedAssetNum = 0; // 다시 집계..

	//		TMap<FCombinedNPCClassID, bool> AllExceptIDs;
	//		// 현재 LoadedPtrMap 에 있다는 건 이번 레벨에서 명시적으로 로딩했다는 걸로 간주하고 루트셋에 남겨두려는 거.
	//		// 다음 레벨에서 명시적으로 로딩되지 않더라도 로딩된 상태겠지만 LoadedPtrMap 에 포함되지는 않을 것이다. 그렇게 남아있게 되면 다음 레벨이 내려갈 때 unload 될 것.
	//		for (TMap<uint32, UObject*>::TIterator LoadedIt(LoadedPtrMap); LoadedIt; ++LoadedIt)
	//		{
	//			FCombinedNPCClassID ThisLoadedId;

	//			FromNPCClassKey(LoadedIt.Key(), ThisLoadedId);
	//			AllExceptIDs.Add(ThisLoadedId, true); // bool value 는 의미는 없다.

	//			// 루트셋 처리를 여기서 함, 로딩 되었을 때 하려니 한곳이 아니기도 하고 해서..
	//			const TSoftClassPtr<UB2NPCSingleClassInfo>& LoadedAssetClass = GetNPCSingleClassAsset(ThisLoadedId);
	//			check(LoadedAssetClass.IsValid());
	//			if (LoadedAssetClass.IsValid())
	//			{
	//				LoadedAssetClass->AddToRoot(); // 다음 레벨에서 명시적으로 로딩되지 않는다면 다음번에 여길 거치면서 RootSet 에서 제거되고 언로딩 될 것이다.
	//				++EstimatedRootedAssetNum;
	//			}
	//		}
	//		UnloadExcept(AllExceptIDs);
	//	}
	//}
	LoadedPtrMap.Empty(); // 어찌되었건 비우지만 RootSet 에 들어간 것들은 로딩된 상태를 유지할 것.
}

bool UB2NPCClassInfoBox::ShouldUnloadAllOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 는 메모리 넘칠 것이 무서워서..
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
{ // UB2AssetLoader::bEnableDevLog 로 Async 로그 뿌릴 때 소모 시간도 같이 뿌리고 싶으면 콜
	// 여러개 요청이 중첩되었을 때 일일이 체크하지는 못함. 알아서 잘 사용 ㅋ
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
	//		// GetBaseBPClass 에 null 을 넘겨준다고 해서 당장 작동 안하는 건 아니고 asyn 로딩 flush 가 일어나는 걸 경계하는 건데 에디터에서 그러는 거야 문제될 거 없고.
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

	// this 와 맞추고 싶은데 쉽지가 않다.. ㅠㅠ
	if (CastedBPObj && CastedBPObj->ParentClass && CastedBPObj->ParentClass->GetName() == TEXT("B2NPCClassInfoBox"))
	{ // 기본 EditorLoadAll 을 하지 않게 되면서 에셋 에디터 열 때 로딩하도록 함.

		// 이거 안 해도 이미 열 때 돌아가는 중..
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
	// InfoAsset 에 등록되는 애들이 얼마나 되는지에 따라 적절히 정하는 거. 몇개 element 로딩마다 한번씩 업데이트 할 지.
	const int32 ProgUpdateInterval = 2;
	const int32 TotalProgUpdateCount = NewInfoMap.Num() / ProgUpdateInterval;
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	int32 InfoLoadCount = 0;
	// 한번 미리 불러주면 TAsset 들이 valid 해 질 것. 물론 이 와중에 메모리는 왕창 먹겠지.
	for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
	{
		FNPCClassInfoSearchMap& ThisInfoMap = InfoMapIt.Value();

		for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(ThisInfoMap.VariationAssets); VariationIt; ++VariationIt)
		{
			ENPCClassVariation ThisVariation = VariationIt.Key();
			// 핵심은 요거고 나머지 progress 계산은 사족.
			GetNPCSingleClassInfo(ThisInfoMap.NPCClassEnum, ThisVariation);
		}
				
		if (InSplashProgFnPtr && (InfoLoadCount % ProgUpdateInterval == ProgUpdateInterval - 1))
		{
			CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount);
			InSplashProgFnPtr(CurrProg);
		}

		++InfoLoadCount; // Variation 단위가 아니라 Class 단위로 카운트 한다. Variation 단위까지는 처음부터 로딩해야 할 양을 알지 못해서.
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
//		FString WarnMessage = TEXT("다음 NPCClassInfo 항목에 대한 SensitiveInfo 항목이 누락되었거나 SensitiveDataRowKey 가 맞지 않음.\n\n");
//#endif
//
//		// 사실 SensitiveInfo 와의 일치 체크는 이제 그리 중요한 건 아님. 이젠 개발버전에서나 쓰이는 거라..
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
//			WarnMessage += TEXT("\n컴퓨터가 폭발하지는 않을 것이니 관련자 이외에는 안심하고 작업을 진행하세요.");
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
//		FString ErrorMsg(TEXT("[Warning] NPCClassInfoBox 와 개별 ClassInfo 의 NPCClassEnum 설정 간에 불일치가 있음. 컴퓨터가 곧 폭발한다.\r\n\r\n"));
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
	// 실제 사용에서는 반대로 ClassInfo 쪽에서 SensitiveInfo 쪽으로 SensitiveDataRowKey 를 사용해서 쿼리.
	// 이건 에디터 기능으로서 단순 무식 search
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

	// 등록된 에셋들이 로딩 안 된 상태에서는 오래 걸리는데.. 미리 한 번 로딩할 필요는 있음..
	CheckInfoDataIntegrity();
}

void UB2NPCClassInfoBox::FirstTimeNPCSensitiveInfoConstruction()
{ 
	//// NPCSensitiveInfo 도입 이전까지 NPCClassInfo 에 넣어 두었던 데이터들을 NPCSensitiveInfo 로 자동 이관하기 위한 임시 기능

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
	//				// SensitiveDataRowKey 가 그동안 그냥 노출이 되어 있다 보니 값이 엉뚱하게 되어 있다거나 심지어 중복이 되어 있는 것도 있음.
	//				// 테이블을 처음 구축하기 위한 기능이니 기본 권장 값으로 세팅해 준다.
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
{ // 로딩된 UB2NPCSingleClassInfo 레퍼런스 캐싱할 맵 키. 수정하게 되면 아래 FromNPCClassKey 도.
	return (static_cast<int32>(InClassEnum) * 100 + static_cast<int32>(InClassVariation));
}
void FromNPCClassKey(int32 InKeyValue, ENPCClass& OutClassEnum, ENPCClassVariation& OutClassVariation)
{ // GetNPCClassKey 의 반대
	OutClassEnum = static_cast<ENPCClass>(InKeyValue / 100);
	OutClassVariation = static_cast<ENPCClassVariation>(InKeyValue % 100);
}
int32 GetNPCClassKey(const FCombinedNPCClassID& NPCClassID)
{ // 로딩된 UB2NPCSingleClassInfo 레퍼런스 캐싱할 맵 키.
	return GetNPCClassKey(NPCClassID.GetNPCClassEnum(), NPCClassID.GetNPCVariationEnum());
}
void FromNPCClassKey(int32 InKeyValue, FCombinedNPCClassID& OutNPCClassID)
{
	ENPCClass ClassEnum = ENPCClass::ENC_End;
	ENPCClassVariation ClassVar = ENPCClassVariation::ENCV_Normal;
	FromNPCClassKey(InKeyValue, ClassEnum, ClassVar);
	OutNPCClassID = FCombinedNPCClassID(ClassEnum, ClassVar);
}