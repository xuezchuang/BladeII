// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2SkillInfo.h"
#include "B2PCClassInfoBox.h"
#include "BladeIIPlayer.h"
#include "Engine/SkeletalMesh.h"
#include "CharacterDataStore.h"
#include "BladeIIGameImpl.h"
#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2PCClassInfo* FPCClassInfoSearchMap::GetPCClassInfoObject(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap)
{
	// 일단 로딩된 에셋 캐싱 구조체부터 확인.
	FSinglePCClassInfoLoadedRefHolder* FoundLoadedPtrEntry = LoadedPtrMap.Find(MyArrayIndex);
	if (!FoundLoadedPtrEntry)
	{
		FSinglePCClassInfoLoadedRefHolder NewLoadedEntry;
		LoadedPtrMap.Add(MyArrayIndex, NewLoadedEntry);
		FoundLoadedPtrEntry = LoadedPtrMap.Find(MyArrayIndex);
	}
	checkSlow(FoundLoadedPtrEntry); // 없으면 바보

	if (FoundLoadedPtrEntry->LoadedInfoPtr)
		return Cast<UB2PCClassInfo>(FoundLoadedPtrEntry->LoadedInfoPtr->GetDefaultObject());

	GetSomeLazyLoadedClassAssetCommon<UB2PCClassInfo>(InLoadManager, AssetPtr, &(FoundLoadedPtrEntry->LoadedInfoPtr), TEXT("PCClassInfo"));

	if (FoundLoadedPtrEntry->LoadedInfoPtr)
	{
		UB2PCClassInfo* LoadedInfoDO = Cast<UB2PCClassInfo>(FoundLoadedPtrEntry->LoadedInfoPtr->GetDefaultObject());
		if (!UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition())
		{// 레벨 내릴 때 unload 안 할 꺼면 확실히 RootSet 에 박아둠.
			FoundLoadedPtrEntry->LoadedInfoPtr->AddToRoot(); 
		}
		return LoadedInfoDO;
	}
	return nullptr;
}

void FPCClassInfoSearchMap::Unload(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap)
{
	// UB2PCClassInfo 자체도 내부적으로 TAsset 구성을 한 게 있어서 언로딩을 해 준다.
	UB2PCClassInfo* TheInfoAsset = AssetPtr.IsValid() ? Cast<UB2PCClassInfo>(AssetPtr->GetDefaultObject()) : NULL;
	if (TheInfoAsset)
	{
		// ManagedPCClassUnload 에 따라 RootSet 일 수 있다.
		AssetPtr->RemoveFromRoot();
		
		TheInfoAsset->UnloadInGameOnlyAssets();
	}

	LoadedPtrMap.Remove(MyArrayIndex);

	TSubclassOf<UB2PCClassInfo> DummyLoadePtr = NULL; // 단지 UnloadSomeLazyLoadedClassAssetCommon 에 넘겨주기 위함. 여기선 위에서 비우고 시작.
	UnloadSomeLazyLoadedClassAssetCommon<UB2PCClassInfo>(InLoadManager, AssetPtr, &DummyLoadePtr);
}

bool FPCClassInfoSearchMap::IsClassDataAtRootSet(bool bCheckForInGameOnlyInfo)
{
	if (!AssetPtr.IsValid()) { // 이 경우도 혹여나 InGameOnlyInfo 는 로딩되어 있을지도 모르지만 그것까지 생각할 필요는 없고..
		return false;
	}

	UB2PCClassInfo* LoadedInfo = Cast<UB2PCClassInfo>(AssetPtr->GetDefaultObject());

	// bCheckForInGameOnlyInfo 에 따라 실제로 서로 다른 대상을 체크
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
 * 직접 참조하지 말고 ShouldUnloadAllPCClassInfoOnLevelTransition 를 사용할 것!
 */
bool UB2PCClassInfoBox::bManagedPCClassUnload = true;

UB2PCClassInfoBox::UB2PCClassInfoBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

#if WITH_EDITOR && !PLATFORM_MAC// Enum 네이밍 체크. 좀 맞춰야 할 사정이 있어서.. ABladeIIPlayer::GetTrimmedNameFromPCClassEnum 참고
	if (UB2PCClassInfo::CheckPCClassEnumNamingConvention() == false)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("[B2PCClassInfo] PCClass Enum 목록에 엄청난 오류가 나타났음. 컴퓨터가 곧 폭발한다.")));
	}
#endif

	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII.B2PCClassInfo"), TEXT("ManagedPCClassUnload"), bManagedPCClassUnload, GGameIni);
	}

	MeshSectionMergeInfo = nullptr;
}

// TAsset lazy-loading 을 사용하는 Blueprint InfoAsset 의 오동작 문제를 정식으로 해결하기 전까지의 임시 땜질.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2PCClassInfoBox::CleanupOnPreSave()
{
	LoadedPtrMap.Empty();
}
void UB2PCClassInfoBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 의도치 않게 저장된 레퍼런스를 날려준다.
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
				ThisInfo.GetPCClassInfoObject(InfoLoadManager, IMI, LoadedPtrMap); // AssetPtr 이 valid 하지 않다면 미리 준비해 놓게 될 것.
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
	LoadedPtrMap.Empty(); // 확인사살
}
void UB2PCClassInfoBox::UnloadExcept(const TArray<EPCClass>& InExceptClasses)
{
	// ManagedPCClassUnload 에서 사용을 위함. 몇개는 언로드 안하고 남겨두려는..
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		FPCClassInfoSearchMap& ThisInfo = InfoMapArray[CII];
		if (InExceptClasses.Contains(ThisInfo.PCClassEnum)) {
			continue;
		}
		ThisInfo.Unload(InfoLoadManager, CII, LoadedPtrMap);
	}
	// 적어도 남겨두려는 클래스보다 많이 로딩된 상태면 안되겠지.
	check(LoadedPtrMap.Num() <= InExceptClasses.Num());
}
void UB2PCClassInfoBox::UnloadAllOrExceptSelected()
{
	// 레벨 transition 혹은 StageClear 시점에 불림. ManagedPCClassUnload 에 따라 어떤 걸 선별적으로 남겨둘지 판별..
	if (ShouldUnloadAllPCClassInfoOnLevelTransition()) 
	{
		UnloadAll(); // 안전하게 죄다 내리지만 다음 로딩이 길어지겠지
	}
	else 
	{
		TArray<EPCClass> UnloadExceptClasses;
		// 현재 선택되어 있는 메인, 태그 두 종류를 남겨두는 걸로. 계속 똑같은 걸로 플레이 하면 로딩 시간이 줄어든 채로 가겠지..
		// 적어도 로비에서는 현재 선택이 다음 스테이지 캐릭터 그대로이기도 하고.
		UnloadExceptClasses.Add(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
		UnloadExceptClasses.Add(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());
		UnloadExcept(UnloadExceptClasses);

		// 그러나 아직 이게 의도대로 동작하지는 않아서.. 아래 UnloadAllOrExceptSelectedIfAllowed 을 따로 제공하게 됨.
	}
}
void UB2PCClassInfoBox::UnloadAllOrExceptSelectedIfAllowed()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING]
	// iOS 에서는 이걸 사용하더라도 필요한 것만 남겨놓음.
	UnloadAllOrExceptSelected();
#else
	if (ShouldUnloadAllPCClassInfoOnLevelTransition())
	{
		UnloadAll(); // 안전하게 죄다 내리지만 다음 로딩이 길어지겠지
	}
	else
	{
		// 여기선 ManagedPCClassUnload 인 경우 암것도 하지 않는다.
		// 한 번 로딩한 PCClass 정보는 끝까지 들고 있는 것.
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
	return !bManagedPCClassUnload; // 좋다 여긴 iOS 도 해 본다. 단, 약간은 다를 수 있다..
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

// 맵 로딩 progress 산정 참고에 쓰임
bool UB2PCClassInfoBox::IsClassDataAtRootSet(EPCClass InClass, bool bCheckForInGameOnlyInfo)
{
	// 꼬락 몇 개 안되지만 매번 iteration 도는 거 썩 보기 좋지는 않은 듯. ㅜㅜ
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
	UB2PCClassInfo* PCInfo = GetSingleClassInfo(InClass);
	if (PCInfo == NULL)
	{
		return NULL;
	}

	if (PCInfo->BaseMeshAsset)
	{
		return PCInfo->BaseMeshAsset;
	}
	// GetBaseBPClass 에 null 을 넘겨준다고 해서 당장 작동 안하는 건 아니고 asyn 로딩 flush 가 일어나는 걸 경계하는 건데 에디터에서 그러는 거야 문제될 거 없고.
	TSubclassOf<class ABladeIIPlayer> PCInfoBaseBPClass = PCInfo->GetBaseBPClass(nullptr);
	if (PCInfoBaseBPClass)
	{
		ABladeIIPlayer* DefaultPlayerObj = Cast<ABladeIIPlayer>(PCInfoBaseBPClass->GetDefaultObject());
		USkeletalMeshComponent* SkeletalMeshComponent = DefaultPlayerObj->GetMesh();

		// 닒莉숭櫓삿혤USkeletalMesh
		USkeletalMesh* SkeletalMesh = nullptr;
		if (SkeletalMeshComponent)
		{
			SkeletalMesh = SkeletalMeshComponent->GetSkeletalMeshAsset();// SkeletalMesh;
		}
	}

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
	const int32 TotalProgUpdateCount = InfoMapArray.Num(); // 이건 하나 로딩할 때 마다 progress 체크
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	// 한번 미리 불러주면 AssetPtr 이 valid 해 질 것. 물론 이 와중에 메모리는 왕창 먹겠지.
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		////////////////////////////// 실제 로딩
		UB2PCClassInfo* ThisLoadedinfo = InfoMapArray[CII].GetPCClassInfoObject(InfoLoadManager, CII, LoadedPtrMap);
		//////////////////////////////

		if (InSplashProgFnPtr)
		{
			CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount * 0.2f);
			InSplashProgFnPtr(CurrProg);
		}

		////////////////////////////// 실제 로딩
		if (ThisLoadedinfo)
		{ // 물론 인게임 전용 에셋들도 로딩. 여기에 인게임 AnimBP 가 들어있어서 훨 큼.
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
				FString::Printf(TEXT("[Warning] PCClassInfoBox 와 개별 ClassInfo 의 PCClassEnum 설정 간에 불일치가 있음. 컴퓨터가 곧 폭발한다."))
				));
		}
	}
#endif
}
#endif