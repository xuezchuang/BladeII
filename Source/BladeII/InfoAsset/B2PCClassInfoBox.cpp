// Fill out your copyright notice in the Description page of Project Settings.

#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"

#include "BladeIIPlayer.h"
#include "Engine/SkeletalMesh.h"
//#include "CharacterDataStore.h"
//#include "BladeIIGameImpl.h"
#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2PCClassInfo* FPCClassInfoSearchMap::GetPCClassInfoObject(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap)
{
	// 老窜 肺爹等 俊悸 某教 备炼眉何磐 犬牢.
	FSinglePCClassInfoLoadedRefHolder* FoundLoadedPtrEntry = LoadedPtrMap.Find(MyArrayIndex);
	if (!FoundLoadedPtrEntry)
	{
		FSinglePCClassInfoLoadedRefHolder NewLoadedEntry;
		LoadedPtrMap.Add(MyArrayIndex, NewLoadedEntry);
		FoundLoadedPtrEntry = LoadedPtrMap.Find(MyArrayIndex);
	}
	checkSlow(FoundLoadedPtrEntry); // 绝栏搁 官焊

	if (FoundLoadedPtrEntry->LoadedInfoPtr)
		return Cast<UB2PCClassInfo>(FoundLoadedPtrEntry->LoadedInfoPtr->GetDefaultObject());

	GetSomeLazyLoadedClassAssetCommon<UB2PCClassInfo>(InLoadManager, AssetPtr, &(FoundLoadedPtrEntry->LoadedInfoPtr), TEXT("PCClassInfo"));

	if (FoundLoadedPtrEntry->LoadedInfoPtr)
	{
		UB2PCClassInfo* LoadedInfoDO = Cast<UB2PCClassInfo>(FoundLoadedPtrEntry->LoadedInfoPtr->GetDefaultObject());
		if (!UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition())
		{// 饭骇 郴副 锭 unload 救 且 波搁 犬角洒 RootSet 俊 冠酒狄.
			FoundLoadedPtrEntry->LoadedInfoPtr->AddToRoot(); 
		}
		return LoadedInfoDO;
	}
	return nullptr;
}

void FPCClassInfoSearchMap::Unload(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap)
{
	// UB2PCClassInfo 磊眉档 郴何利栏肺 TAsset 备己阑 茄 霸 乐绢辑 攫肺爹阑 秦 霖促.
	UB2PCClassInfo* TheInfoAsset = AssetPtr.IsValid() ? Cast<UB2PCClassInfo>(AssetPtr->GetDefaultObject()) : NULL;
	if (TheInfoAsset)
	{
		// ManagedPCClassUnload 俊 蝶扼 RootSet 老 荐 乐促.
		AssetPtr->RemoveFromRoot();
		
		TheInfoAsset->UnloadInGameOnlyAssets();
	}

	LoadedPtrMap.Remove(MyArrayIndex);

	TSubclassOf<UB2PCClassInfo> DummyLoadePtr = NULL; // 窜瘤 UnloadSomeLazyLoadedClassAssetCommon 俊 逞败林扁 困窃. 咯扁急 困俊辑 厚快绊 矫累.
	UnloadSomeLazyLoadedClassAssetCommon<UB2PCClassInfo>(InLoadManager, AssetPtr, &DummyLoadePtr);
}

bool FPCClassInfoSearchMap::IsClassDataAtRootSet(bool bCheckForInGameOnlyInfo)
{
	if (!AssetPtr.IsValid()) { // 捞 版快档 趣咯唱 InGameOnlyInfo 绰 肺爹登绢 乐阑瘤档 葛福瘤父 弊巴鳖瘤 积阿且 鞘夸绰 绝绊..
		return false;
	}

	UB2PCClassInfo* LoadedInfo = Cast<UB2PCClassInfo>(AssetPtr->GetDefaultObject());

	// bCheckForInGameOnlyInfo 俊 蝶扼 角力肺 辑肺 促弗 措惑阑 眉农
	return LoadedInfo && (
		(!bCheckForInGameOnlyInfo && LoadedInfo->IsRooted()) || (bCheckForInGameOnlyInfo && LoadedInfo->IsInGameOnlyInfoAtRootSet())
		);
}

#if WITH_EDITOR
bool FPCClassInfoSearchMap::CheckPCClassEnumSettingValid(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap)
{
	UB2PCClassInfo* ClassInfoObject = GetPCClassInfoObject(InLoadManager, MyArrayIndex, LoadedPtrMap);
	return (ClassInfoObject && ClassInfoObject->PCClassEnum == PCClassEnum);
}
#endif

/** 
 * 流立 曼炼窍瘤 富绊 ShouldUnloadAllPCClassInfoOnLevelTransition 甫 荤侩且 巴!
 */
bool UB2PCClassInfoBox::bManagedPCClassUnload = true;

UB2PCClassInfoBox::UB2PCClassInfoBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

#if WITH_EDITOR && !PLATFORM_MAC// Enum 匙捞怪 眉农. 粱 嘎苗具 且 荤沥捞 乐绢辑.. ABladeIIPlayer::GetTrimmedNameFromPCClassEnum 曼绊
	if (UB2PCClassInfo::CheckPCClassEnumNamingConvention() == false)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("[B2PCClassInfo] PCClass Enum 格废俊 决没抄 坷幅啊 唱鸥车澜. 哪腔磐啊 梆 气惯茄促.")));
	}
#endif

	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII.B2PCClassInfo"), TEXT("ManagedPCClassUnload"), bManagedPCClassUnload, GGameIni);
	}

	MeshSectionMergeInfo = nullptr;
}

// TAsset lazy-loading 阑 荤侩窍绰 Blueprint InfoAsset 狼 坷悼累 巩力甫 沥侥栏肺 秦搬窍扁 傈鳖瘤狼 烙矫 东龙.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2PCClassInfoBox::CleanupOnPreSave()
{
	LoadedPtrMap.Empty();
}
void UB2PCClassInfoBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 狼档摹 臼霸 历厘等 饭欺繁胶甫 朝妨霖促.
	CleanupOnPreSave();
}
#endif

void UB2PCClassInfoBox::PreloadClassAssets(const TArray<EPCClass>& InClasses)
{
#if !UE_BUILD_SHIPPING
	int32 AllLoadedCount = 0;
#endif
	for (int32 IMI = 0; IMI < InfoMapArray.Num(); ++IMI)
	{
		FPCClassInfoSearchMap& ThisInfo = InfoMapArray[IMI];

		for (int32 CI = 0; CI < InClasses.Num(); ++CI)
		{
			if (ThisInfo.PCClassEnum == InClasses[CI])
			{
				ThisInfo.GetPCClassInfoObject(InfoLoadManager, IMI, LoadedPtrMap); // AssetPtr 捞 valid 窍瘤 臼促搁 固府 霖厚秦 初霸 瞪 巴.
#if !UE_BUILD_SHIPPING
				AllLoadedCount++;
#endif
				break;
			}
		}
	}
#if !UE_BUILD_SHIPPING
	if (AllLoadedCount > 0){
		UE_LOG(LogBladeII, Log, TEXT("PCClassInfo pre-load for %d assets"), AllLoadedCount);
	}
#endif	
}

void UB2PCClassInfoBox::UnloadAll()
{
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		InfoMapArray[CII].Unload(InfoLoadManager, CII, LoadedPtrMap);
	}
	LoadedPtrMap.Empty(); // 犬牢荤混
}
void UB2PCClassInfoBox::UnloadExcept(const TArray<EPCClass>& InExceptClasses)
{
	// ManagedPCClassUnload 俊辑 荤侩阑 困窃. 割俺绰 攫肺靛 救窍绊 巢败滴妨绰..
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		FPCClassInfoSearchMap& ThisInfo = InfoMapArray[CII];
		if (InExceptClasses.Contains(ThisInfo.PCClassEnum)) {
			continue;
		}
		ThisInfo.Unload(InfoLoadManager, CII, LoadedPtrMap);
	}
	// 利绢档 巢败滴妨绰 努贰胶焊促 腹捞 肺爹等 惑怕搁 救登摆瘤.
	check(LoadedPtrMap.Num() <= InExceptClasses.Num());
}
void UB2PCClassInfoBox::UnloadAllOrExceptSelected()
{
	// 饭骇 transition 趣篮 StageClear 矫痢俊 阂覆. ManagedPCClassUnload 俊 蝶扼 绢恫 吧 急喊利栏肺 巢败笛瘤 魄喊..
	if (ShouldUnloadAllPCClassInfoOnLevelTransition()) 
	{
		UnloadAll(); // 救傈窍霸 了促 郴府瘤父 促澜 肺爹捞 辨绢瘤摆瘤
	}
	else 
	{
		TArray<EPCClass> UnloadExceptClasses;
		// 泅犁 急琶登绢 乐绰 皋牢, 怕弊 滴 辆幅甫 巢败滴绰 吧肺. 拌加 度鞍篮 吧肺 敲饭捞 窍搁 肺爹 矫埃捞 临绢电 盲肺 啊摆瘤..
		// 利绢档 肺厚俊辑绰 泅犁 急琶捞 促澜 胶抛捞瘤 某腐磐 弊措肺捞扁档 窍绊.
		//UnloadExceptClasses.Add(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
		//UnloadExceptClasses.Add(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());
		UnloadExcept(UnloadExceptClasses);

		// 弊矾唱 酒流 捞霸 狼档措肺 悼累窍瘤绰 臼酒辑.. 酒贰 UnloadAllOrExceptSelectedIfAllowed 阑 蝶肺 力傍窍霸 凳.
	}
}
void UB2PCClassInfoBox::UnloadAllOrExceptSelectedIfAllowed()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING]
	// iOS 俊辑绰 捞吧 荤侩窍歹扼档 鞘夸茄 巴父 巢败初澜.
	UnloadAllOrExceptSelected();
#else
	if (ShouldUnloadAllPCClassInfoOnLevelTransition())
	{
		UnloadAll(); // 救傈窍霸 了促 郴府瘤父 促澜 肺爹捞 辨绢瘤摆瘤
	}
	else
	{
		// 咯扁急 ManagedPCClassUnload 牢 版快 鞠巴档 窍瘤 臼绰促.
		// 茄 锅 肺爹茄 PCClass 沥焊绰 场鳖瘤 甸绊 乐绰 巴.
	}
#endif
}

UB2SkillInfo* UB2PCClassInfoBox::GetAllSkillInfo()
{
	return AllPCSkillInfos ? Cast<UB2SkillInfo>(AllPCSkillInfos->GetDefaultObject()) : nullptr;
}

UB2PCMeshSectionMergeInfo* UB2PCClassInfoBox::GetMeshSectionMergeInfo()
{
	return MeshSectionMergeInfo ? Cast<UB2PCMeshSectionMergeInfo>(MeshSectionMergeInfo->GetDefaultObject()) : nullptr;
}

bool UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING]
	return !bManagedPCClassUnload; // 亮促 咯变 iOS 档 秦 夯促. 窜, 距埃篮 促甫 荐 乐促..
#else
	return !bManagedPCClassUnload;
#endif
}

UB2PCClassInfo* UB2PCClassInfoBox::GetSingleClassInfo(EPCClass InClass)
{
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		if (InfoMapArray[CII].PCClassEnum != InClass){
			continue;
		}

		UB2PCClassInfo* ClassInfoObject = InfoMapArray[CII].GetPCClassInfoObject(InfoLoadManager, CII, LoadedPtrMap);

		if (ClassInfoObject && ClassInfoObject->PCClassEnum == InClass){
			return ClassInfoObject;
		}
	}
	return NULL;
}

// 甘 肺爹 progress 魂沥 曼绊俊 静烙
bool UB2PCClassInfoBox::IsClassDataAtRootSet(EPCClass InClass, bool bCheckForInGameOnlyInfo)
{
	// 部遏 割 俺 救登瘤父 概锅 iteration 档绰 芭 解 焊扁 亮瘤绰 臼篮 淀. ぬぬ
	for (FPCClassInfoSearchMap& ThisMap : InfoMapArray)
	{
		if (ThisMap.PCClassEnum == InClass)
		{
			return ThisMap.IsClassDataAtRootSet(bCheckForInGameOnlyInfo);
		}
	}
	return false;
}

#if WITH_EDITOR
USkeletalMesh* UB2PCClassInfoBox::GetBaseSkeletalMeshFromClass(EPCClass InClass)
{
	//UB2PCClassInfo* PCInfo = GetSingleClassInfo(InClass);
	//if (PCInfo == NULL)
	//{
	//	return NULL;
	//}

	//if (PCInfo->BaseMeshAsset)
	//{
	//	return PCInfo->BaseMeshAsset;
	//}
	//// GetBaseBPClass 俊 null 阑 逞败霖促绊 秦辑 寸厘 累悼 救窍绰 扒 酒聪绊 asyn 肺爹 flush 啊 老绢唱绰 吧 版拌窍绰 扒单 俊叼磐俊辑 弊矾绰 芭具 巩力瞪 芭 绝绊.
	//TSubclassOf<class ABladeIIPlayer> PCInfoBaseBPClass = PCInfo->GetBaseBPClass(nullptr);
	//if (PCInfoBaseBPClass)
	//{
	//	ABladeIIPlayer* DefaultPlayerObj = Cast<ABladeIIPlayer>(PCInfoBaseBPClass->GetDefaultObject());
	//	USkeletalMeshComponent* SkeletalMeshComponent = DefaultPlayerObj->GetMesh();

	//	// 从组件中获取USkeletalMesh
	//	USkeletalMesh* SkeletalMesh = nullptr;
	//	if (SkeletalMeshComponent)
	//	{
	//		SkeletalMesh = SkeletalMeshComponent->GetSkeletalMeshAsset();// SkeletalMesh;
	//	}
	//}

	return NULL;
}

void UB2PCClassInfoBox::GetSkeletalMeshesForAllClasses(TArray<USkeletalMesh*>& OutMeshes)
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		USkeletalMesh* ThisMesh = GetBaseSkeletalMeshFromClass(IntToPCClass(PCI));
		if (ThisMesh)
		{
			OutMeshes.Add(ThisMesh);
		}
	}
}
UB2PCClassInfo* UB2PCClassInfoBox::GetPCClassInfoForMatchingSKMesh(class USkeletalMesh* InTestSKMesh)
{
	if (InTestSKMesh == NULL)
	{
		return NULL;
	}

	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		USkeletalMesh* ThisMesh = GetBaseSkeletalMeshFromClass(IntToPCClass(PCI));

		if (ThisMesh == InTestSKMesh)
		{
			return GetSingleClassInfo(IntToPCClass(PCI));
		}
	}
	return NULL;
}

void UB2PCClassInfoBox::EditorLoadAll(SetB2ContentLoadingProgressSplashFnPtr InSplashProgFnPtr, float InStartProgress, float InEndProgress)
{
	if (GIsEditor == false)
	{
		return;
	}

	const float TotalProgRange = FMath::Max(0.0f, InEndProgress - InStartProgress);
	const int32 TotalProgUpdateCount = InfoMapArray.Num(); // 捞扒 窍唱 肺爹且 锭 付促 progress 眉农
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	// 茄锅 固府 阂矾林搁 AssetPtr 捞 valid 秦 龙 巴. 拱沸 捞 客吝俊 皋葛府绰 空芒 冈摆瘤.
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		////////////////////////////// 角力 肺爹
		UB2PCClassInfo* ThisLoadedinfo = InfoMapArray[CII].GetPCClassInfoObject(InfoLoadManager, CII, LoadedPtrMap);
		//////////////////////////////

		if (InSplashProgFnPtr)
		{
			CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount * 0.2f);
			InSplashProgFnPtr(CurrProg);
		}

		////////////////////////////// 角力 肺爹
		if (ThisLoadedinfo)
		{ // 拱沸 牢霸烙 傈侩 俊悸甸档 肺爹. 咯扁俊 牢霸烙 AnimBP 啊 甸绢乐绢辑 任 怒.
			ThisLoadedinfo->SyncLoadInGameOnlyAssets();
		}
		//////////////////////////////

		if (InSplashProgFnPtr)
		{
			CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount * 0.8f);
			InSplashProgFnPtr(CurrProg);
		}
	}
}

bool UB2PCClassInfoBox::CheckAllPCClassEnumSettingValid()
{
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		if (InfoMapArray[CII].CheckPCClassEnumSettingValid(InfoLoadManager, CII, LoadedPtrMap) == false)
		{
			return false;
		}
	}
	return true;
}

void UB2PCClassInfoBox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

#if !PLATFORM_MAC
	const FName Name_PCClassEnum = GET_MEMBER_NAME_CHECKED(FPCClassInfoSearchMap, PCClassEnum);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == Name_PCClassEnum)
	{
		if (CheckAllPCClassEnumSettingValid() == false)
		{
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[Warning] PCClassInfoBox 客 俺喊 ClassInfo 狼 PCClassEnum 汲沥 埃俊 阂老摹啊 乐澜. 哪腔磐啊 梆 气惯茄促."))
				));
		}
	}
#endif
}
#endif