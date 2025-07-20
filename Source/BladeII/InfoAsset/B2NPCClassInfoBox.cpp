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
	// 夸 喉发府胶飘绰 SpawnPool 俊辑 霸烙 矫累 捞饶 async 肺爹俊辑 漂喊洒 力寇矫懦 努贰胶甸.
	// 霉 楷免 场唱扁 傈俊 async 肺爹捞 肯丰瞪 鞘夸啊 乐绰单 割割.. BossMob Master AnimBP 静绰 巴甸 窍唱笛 尝绢 乐栏搁 农霸 蠢妨瘤骨肺 弊巴甸篮 力寇矫虐妨绰 芭.
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
	//				SpawnPoolAsyncBlacklists.Add(CastedNPCClassEnum, true); // bool Value 绰 迭洒 狼固绰 绝澜.
	//			}
	//		}
	//	}
	//}
}

void UB2NPCClassInfoBox::GetAsyncRequestInfoFromNPCIds(const TArray<FCombinedNPCClassID>& NPCIDs, TArray<FB2AsyncRequestInfo>& OutRequestInfoList)
{ // FCombinedNPCClassID 俊 秦寸窍绰 AssetReference 甫 啊廉咳. 捞吧 NPCClassInfo 俊悸 肺爹俊 镜 荐 乐促.
	for (const auto& ClassID : NPCIDs)
	{
		auto NPCSingleClassAsset = GetNPCSingleClassAsset(ClassID);
		if (NPCSingleClassAsset.IsNull() == false)
		{
			FSoftObjectPath	AssetString = NPCSingleClassAsset.ToSoftObjectPath();
			FB2AsyncRequestInfo		RequestInfo(ClassID, AssetString);

			OutRequestInfoList.Add(RequestInfo);
		}
	}
}

TSoftClassPtr<UB2NPCSingleClassInfo> UB2NPCClassInfoBox::GetNPCSingleClassAsset(const FCombinedNPCClassID& ClassID)
{
	// 咯扁急 肺爹阑 窍瘤 臼绰促. 肺爹捞 登绢 乐阑 荐档 乐绊 酒匆 荐档 乐促.

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
	if (auto* InfoObj = FindSingleClassInfo(NPCClassID))
	{
		const int32 NPCKey = GetNPCClassKey(NPCClassID);
		LoadedPtrMap.Remove(NPCKey);
	}

	if (const auto& InfoAsset = GetNPCSingleClassAsset(NPCClassID))
	{
		if (InfoAsset.IsValid())
		{ // RootSet 贸府甫 窍霸 瞪 荐 乐栏骨肺 unload 傈俊 都绢初绰促.
			InfoAsset->RemoveFromRoot();
		}
		UnloadAsset(InfoAsset.ToSoftObjectPath());
	}
}

void UB2NPCClassInfoBox::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList)
{
	TArray<FCombinedNPCClassID> SuccessivePhaseClasses; // 肺爹秦 初绊 唱辑具 舅 荐 乐绰 Phase 努贰胶啊 乐促. 磊林 立窍绰 惑炔篮 酒聪摆瘤父..
	for (const auto& RequestInfo : CompleteRequestList)
	{ // 促弗 版肺肺 牢秦 捞固 肺爹捞 登绢 乐阑 荐 乐澜.
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
		// Phase 努贰胶 眠啊 肺爹捞 鞘夸茄 版快. 敲饭捞 矫累 捞饶 Async 肺爹捞 楷崔酒 登绰 惑炔捞搁 flush 俊 秒距茄 券版俊 初捞摆瘤父.. 绢驴 荐 绝瘤.. 铝 ぱぱ;
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

// TAsset lazy-loading 阑 荤侩窍绰 Blueprint InfoAsset 狼 坷悼累 巩力甫 沥侥栏肺 秦搬窍扁 傈鳖瘤狼 烙矫 东龙. -> 烙矫啊 酒囱 淀
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2NPCClassInfoBox::CleanupOnPreSave()
{
	LoadedPtrMap.Empty();
}
void UB2NPCClassInfoBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 狼档摹 臼霸 历厘等 饭欺繁胶甫 朝妨霖促.
	CleanupOnPreSave();
}
#endif

/**
 * 老馆利牢 NPCSingleClassInfo 立辟 牢磐其捞胶. 肺爹捞 救 登绢 乐促搁 咯扁辑 Synchronous 肺爹捞 瞪 巴. (async 夸没阑 沁促搁 肯丰鳖瘤 措扁)
 */
UB2NPCSingleClassInfo* UB2NPCClassInfoBox::GetNPCSingleClassInfo(ENPCClass InClass, ENPCClassVariation InVariation)
{
	if (InClass == ENPCClass::ENC_End)
		return nullptr;

	FCombinedNPCClassID NPCClassID(InClass, InVariation);
	if (auto* LoadedObject = FindSingleClassInfo(NPCClassID))
	{
		return LoadedObject;
	}

	else // need to load
	{
		const FCombinedNPCClassID ClassID(InClass, InVariation);
		auto AssetClass = GetNPCSingleClassAsset(ClassID);
		if (AssetClass.IsNull() == false) // 咯扁 IsNull 篮 肺爹捞 救登绢 乐澜阑 舵窍绰 霸 酒聪扼 俊悸 且寸捞 救登绢 乐澜阑 舵窃
		{
			const FSoftObjectPath& StringAsset = AssetClass.ToSoftObjectPath();
			UClass* SingleClassInfo = Cast<UClass>(LoadSynchronous(StringAsset, GENERAL_TASSET_ASYNC_LOAD_TIMEOUT));
			auto* CastedAsset = Cast<UB2NPCSingleClassInfo>(SingleClassInfo->GetDefaultObject());

			const int32 NPCKey = GetNPCClassKey(ClassID.GetNPCClassEnum(), ClassID.GetNPCVariationEnum());
			LoadedPtrMap.Add(NPCKey, CastedAsset);

			return CastedAsset;
		}
	}

	return nullptr;
}

// 肺爹 登绢 乐绰 芭 茫绰 芭.
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
	//		// 漂洒 OverrideByBaseBP 甫 困秦急 bDeferPostSpawnInitialize 甫 窍绰 巴捞 亮促. 
	//		// PCClassInfo spawn 狼 版快绰 捞霸 鞘荐牢单 NPCClassInfo 狼 版快绰 鞘夸绝绢 焊捞扁档..
	//		FActorSpawnParameters FinalSpawnParameter = SpawnParameters;
	//		FinalSpawnParameter.bDeferPostSpawnInitialize = true;
	//		// 捞 NPCClassInfo 啊 力措肺 冈洒妨搁 SpawnParameters 俊辑 bDeferConstruction 捞 技泼登绢 乐绢具 窃. 弊贰具 BeginPlay 啊 捞 捞饶 阂府霸 凳. 捞吧 荤侩窍绰 何盒(AB2MonsterSpawnPool::SpawnSingleMonsterCommon 殿).
	//		BII_CHECK(FinalSpawnParameter.bDeferConstruction); // 趣矫扼档 bDeferConstruction 阑 荤侩窍瘤 臼绰 版快啊 积扁搁 抛胶飘 饶 捞吧 力芭窍档废. 老馆利牢 牢霸烙 某腐磐绰 酒匆 巴捞促.

	//		ABladeIICharacter* SpawnedActor = Cast<ABladeIICharacter>(SpawnWorld->SpawnActor(T::StaticClass(), &SpawnTransform, FinalSpawnParameter));
	//		BII_CHECK(SpawnedActor && SpawnedActor->IsValidObj());
	//		if (SpawnedActor)
	//		{
	//			SpawnedActor->OverrideByNPCClassInfo(InClass, ClassInfo); // 角龙利牢 ClassInfo 利侩 苞沥.
	//			SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParameter); // bDeferPostSpawnInitialize 俊 蝶弗 眠啊 贸府. 
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
	LoadedPtrMap.Empty(); // 犬牢荤混
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
	// LoadedPtrMap 阑 咯扁辑 厚快瘤绰 臼绰促.
}
void UB2NPCClassInfoBox::UnloadExceptCurrentExplicit(ABladeIIGameMode* CurrGameMode)
{ 
	//if (CurrGameMode && CurrGameMode->GetB2GameModeType() == EB2GameMode::Lobby)
	//{
	//	// 攫哩啊 肺厚俊辑 NPCClassInfo 甫 荤侩窍霸 登搁 捞 啊沥捞 撇府霸 登妨唱.
	//	// LoadedPtrMap 俊 疙矫利栏肺 乐绰 扒 绝歹扼档 捞傈 饭骇辑 RootSet 贸府甫 茄 霸 乐促搁 弊吧 扒靛府瘤绰 臼绰促.
	//	check(LoadedPtrMap.Num() == 0); 
	//}
	//else if (ShouldUnloadAllOnLevelTransition())
	//{
	//	UnloadAll();
	//}
	//else
	//{
	//	// 泅犁 LoadedPtrMap 俊 某教等 巴甸篮 RootSet 贸府甫 窍绊
	//	// 弊霸 酒囱 巴甸篮 RemoveFromRoot 贸府甫 秦辑 犬角洒 郴妨啊霸 茄促.

	//	// 泅犁 疙矫利栏肺 肺爹等 巴捞 绝绰 惑怕绰.. 角力肺 NPC 啊 绝绰 霸烙葛靛芭唱 (抗甫 甸绢 PVP), 酒聪搁 霸烙葛靛 栋唱扁 傈 咯辨 滴锅 捞惑 柳涝且 锭.
	//	// 绢恫 版快电 Unload 甫 角青窍瘤 臼绰促. 付瘤阜栏肺 RootSet 俊 持绢初篮 巴甸捞 乐促搁 蜡瘤瞪 巴.
	//	if (LoadedPtrMap.Num() > 0)
	//	{
	//		EstimatedRootedAssetNum = 0; // 促矫 笼拌..

	//		TMap<FCombinedNPCClassID, bool> AllExceptIDs;
	//		// 泅犁 LoadedPtrMap 俊 乐促绰 扒 捞锅 饭骇俊辑 疙矫利栏肺 肺爹沁促绰 吧肺 埃林窍绊 风飘悸俊 巢败滴妨绰 芭.
	//		// 促澜 饭骇俊辑 疙矫利栏肺 肺爹登瘤 臼歹扼档 肺爹等 惑怕摆瘤父 LoadedPtrMap 俊 器窃登瘤绰 臼阑 巴捞促. 弊犯霸 巢酒乐霸 登搁 促澜 饭骇捞 郴妨哎 锭 unload 瞪 巴.
	//		for (TMap<uint32, UObject*>::TIterator LoadedIt(LoadedPtrMap); LoadedIt; ++LoadedIt)
	//		{
	//			FCombinedNPCClassID ThisLoadedId;

	//			FromNPCClassKey(LoadedIt.Key(), ThisLoadedId);
	//			AllExceptIDs.Add(ThisLoadedId, true); // bool value 绰 狼固绰 绝促.

	//			// 风飘悸 贸府甫 咯扁辑 窃, 肺爹 登菌阑 锭 窍妨聪 茄镑捞 酒聪扁档 窍绊 秦辑..
	//			const TSoftClassPtr<UB2NPCSingleClassInfo>& LoadedAssetClass = GetNPCSingleClassAsset(ThisLoadedId);
	//			check(LoadedAssetClass.IsValid());
	//			if (LoadedAssetClass.IsValid())
	//			{
	//				LoadedAssetClass->AddToRoot(); // 促澜 饭骇俊辑 疙矫利栏肺 肺爹登瘤 臼绰促搁 促澜锅俊 咯辨 芭摹搁辑 RootSet 俊辑 力芭登绊 攫肺爹 瞪 巴捞促.
	//				++EstimatedRootedAssetNum;
	//			}
	//		}
	//		UnloadExcept(AllExceptIDs);
	//	}
	//}
	LoadedPtrMap.Empty(); // 绢骂登菌扒 厚快瘤父 RootSet 俊 甸绢埃 巴甸篮 肺爹等 惑怕甫 蜡瘤且 巴.
}

bool UB2NPCClassInfoBox::ShouldUnloadAllOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 绰 皋葛府 逞磨 巴捞 公辑况辑..
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
{ // UB2AssetLoader::bEnableDevLog 肺 Async 肺弊 谎副 锭 家葛 矫埃档 鞍捞 谎府绊 酵栏搁 妮
	// 咯矾俺 夸没捞 吝酶登菌阑 锭 老老捞 眉农窍瘤绰 给窃. 舅酒辑 肋 荤侩 せ
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
	//		// GetBaseBPClass 俊 null 阑 逞败霖促绊 秦辑 寸厘 累悼 救窍绰 扒 酒聪绊 asyn 肺爹 flush 啊 老绢唱绰 吧 版拌窍绰 扒单 俊叼磐俊辑 弊矾绰 芭具 巩力瞪 芭 绝绊.
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

	// this 客 嘎眠绊 酵篮单 奖瘤啊 臼促.. ばば
	if (CastedBPObj && CastedBPObj->ParentClass && CastedBPObj->ParentClass->GetName() == TEXT("B2NPCClassInfoBox"))
	{ // 扁夯 EditorLoadAll 阑 窍瘤 臼霸 登搁辑 俊悸 俊叼磐 凯 锭 肺爹窍档废 窃.

		// 捞芭 救 秦档 捞固 凯 锭 倒酒啊绰 吝..
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
	// InfoAsset 俊 殿废登绰 局甸捞 倔付唱 登绰瘤俊 蝶扼 利例洒 沥窍绰 芭. 割俺 element 肺爹付促 茄锅究 诀单捞飘 且 瘤.
	const int32 ProgUpdateInterval = 2;
	const int32 TotalProgUpdateCount = NewInfoMap.Num() / ProgUpdateInterval;
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	int32 InfoLoadCount = 0;
	// 茄锅 固府 阂矾林搁 TAsset 甸捞 valid 秦 龙 巴. 拱沸 捞 客吝俊 皋葛府绰 空芒 冈摆瘤.
	for (TMap<ENPCClass, FNPCClassInfoSearchMap>::TIterator InfoMapIt(NewInfoMap); InfoMapIt; ++InfoMapIt)
	{
		FNPCClassInfoSearchMap& ThisInfoMap = InfoMapIt.Value();

		for (TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>>::TIterator VariationIt(ThisInfoMap.VariationAssets); VariationIt; ++VariationIt)
		{
			ENPCClassVariation ThisVariation = VariationIt.Key();
			// 琴缴篮 夸芭绊 唱赣瘤 progress 拌魂篮 荤练.
			GetNPCSingleClassInfo(ThisInfoMap.NPCClassEnum, ThisVariation);
		}
				
		if (InSplashProgFnPtr && (InfoLoadCount % ProgUpdateInterval == ProgUpdateInterval - 1))
		{
			CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount);
			InSplashProgFnPtr(CurrProg);
		}

		++InfoLoadCount; // Variation 窜困啊 酒聪扼 Class 窜困肺 墨款飘 茄促. Variation 窜困鳖瘤绰 贸澜何磐 肺爹秦具 且 剧阑 舅瘤 给秦辑.
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
//		FString WarnMessage = TEXT("促澜 NPCClassInfo 亲格俊 措茄 SensitiveInfo 亲格捞 穿遏登菌芭唱 SensitiveDataRowKey 啊 嘎瘤 臼澜.\n\n");
//#endif
//
//		// 荤角 SensitiveInfo 客狼 老摹 眉农绰 捞力 弊府 吝夸茄 扒 酒丛. 捞哩 俺惯滚傈俊辑唱 静捞绰 芭扼..
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
//			WarnMessage += TEXT("\n哪腔磐啊 气惯窍瘤绰 臼阑 巴捞聪 包访磊 捞寇俊绰 救缴窍绊 累诀阑 柳青窍技夸.");
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
//		FString ErrorMsg(TEXT("[Warning] NPCClassInfoBox 客 俺喊 ClassInfo 狼 NPCClassEnum 汲沥 埃俊 阂老摹啊 乐澜. 哪腔磐啊 梆 气惯茄促.\r\n\r\n"));
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
	// 角力 荤侩俊辑绰 馆措肺 ClassInfo 率俊辑 SensitiveInfo 率栏肺 SensitiveDataRowKey 甫 荤侩秦辑 孽府.
	// 捞扒 俊叼磐 扁瓷栏肺辑 窜鉴 公侥 search
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

	// 殿废等 俊悸甸捞 肺爹 救 等 惑怕俊辑绰 坷贰 吧府绰单.. 固府 茄 锅 肺爹且 鞘夸绰 乐澜..
	CheckInfoDataIntegrity();
}

void UB2NPCClassInfoBox::FirstTimeNPCSensitiveInfoConstruction()
{ 
	//// NPCSensitiveInfo 档涝 捞傈鳖瘤 NPCClassInfo 俊 持绢 滴菌带 单捞磐甸阑 NPCSensitiveInfo 肺 磊悼 捞包窍扁 困茄 烙矫 扁瓷

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
	//				// SensitiveDataRowKey 啊 弊悼救 弊成 畴免捞 登绢 乐促 焊聪 蔼捞 钧蹲窍霸 登绢 乐促芭唱 缴瘤绢 吝汗捞 登绢 乐绰 巴档 乐澜.
	//				// 抛捞喉阑 贸澜 备绵窍扁 困茄 扁瓷捞聪 扁夯 鼻厘 蔼栏肺 技泼秦 霖促.
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
{ // 肺爹等 UB2NPCSingleClassInfo 饭欺繁胶 某教且 甘 虐. 荐沥窍霸 登搁 酒贰 FromNPCClassKey 档.
	return (static_cast<int32>(InClassEnum) * 100 + static_cast<int32>(InClassVariation));
}
void FromNPCClassKey(int32 InKeyValue, ENPCClass& OutClassEnum, ENPCClassVariation& OutClassVariation)
{ // GetNPCClassKey 狼 馆措
	OutClassEnum = static_cast<ENPCClass>(InKeyValue / 100);
	OutClassVariation = static_cast<ENPCClassVariation>(InKeyValue % 100);
}
int32 GetNPCClassKey(const FCombinedNPCClassID& NPCClassID)
{ // 肺爹等 UB2NPCSingleClassInfo 饭欺繁胶 某教且 甘 虐.
	return GetNPCClassKey(NPCClassID.GetNPCClassEnum(), NPCClassID.GetNPCVariationEnum());
}
void FromNPCClassKey(int32 InKeyValue, FCombinedNPCClassID& OutNPCClassID)
{
	ENPCClass ClassEnum = ENPCClass::ENC_End;
	ENPCClassVariation ClassVar = ENPCClassVariation::ENCV_Normal;
	FromNPCClassKey(InKeyValue, ClassEnum, ClassVar);
	OutNPCClassID = FCombinedNPCClassID(ClassEnum, ClassVar);
}