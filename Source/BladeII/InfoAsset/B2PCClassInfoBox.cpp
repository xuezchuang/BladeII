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
	// �ϴ� �ε��� ���� ĳ�� ����ü���� Ȯ��.
	FSinglePCClassInfoLoadedRefHolder* FoundLoadedPtrEntry = LoadedPtrMap.Find(MyArrayIndex);
	if (!FoundLoadedPtrEntry)
	{
		FSinglePCClassInfoLoadedRefHolder NewLoadedEntry;
		LoadedPtrMap.Add(MyArrayIndex, NewLoadedEntry);
		FoundLoadedPtrEntry = LoadedPtrMap.Find(MyArrayIndex);
	}
	checkSlow(FoundLoadedPtrEntry); // ������ �ٺ�

	if (FoundLoadedPtrEntry->LoadedInfoPtr)
		return Cast<UB2PCClassInfo>(FoundLoadedPtrEntry->LoadedInfoPtr->GetDefaultObject());

	GetSomeLazyLoadedClassAssetCommon<UB2PCClassInfo>(InLoadManager, AssetPtr, &(FoundLoadedPtrEntry->LoadedInfoPtr), TEXT("PCClassInfo"));

	if (FoundLoadedPtrEntry->LoadedInfoPtr)
	{
		UB2PCClassInfo* LoadedInfoDO = Cast<UB2PCClassInfo>(FoundLoadedPtrEntry->LoadedInfoPtr->GetDefaultObject());
		if (!UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition())
		{// ���� ���� �� unload �� �� ���� Ȯ���� RootSet �� �ھƵ�.
			FoundLoadedPtrEntry->LoadedInfoPtr->AddToRoot(); 
		}
		return LoadedInfoDO;
	}
	return nullptr;
}

void FPCClassInfoSearchMap::Unload(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap)
{
	// UB2PCClassInfo ��ü�� ���������� TAsset ������ �� �� �־ ��ε��� �� �ش�.
	UB2PCClassInfo* TheInfoAsset = AssetPtr.IsValid() ? Cast<UB2PCClassInfo>(AssetPtr->GetDefaultObject()) : NULL;
	if (TheInfoAsset)
	{
		// ManagedPCClassUnload �� ���� RootSet �� �� �ִ�.
		AssetPtr->RemoveFromRoot();
		
		TheInfoAsset->UnloadInGameOnlyAssets();
	}

	LoadedPtrMap.Remove(MyArrayIndex);

	TSubclassOf<UB2PCClassInfo> DummyLoadePtr = NULL; // ���� UnloadSomeLazyLoadedClassAssetCommon �� �Ѱ��ֱ� ����. ���⼱ ������ ���� ����.
	UnloadSomeLazyLoadedClassAssetCommon<UB2PCClassInfo>(InLoadManager, AssetPtr, &DummyLoadePtr);
}

bool FPCClassInfoSearchMap::IsClassDataAtRootSet(bool bCheckForInGameOnlyInfo)
{
	if (!AssetPtr.IsValid()) { // �� ��쵵 Ȥ���� InGameOnlyInfo �� �ε��Ǿ� �������� ������ �װͱ��� ������ �ʿ�� ����..
		return false;
	}

	UB2PCClassInfo* LoadedInfo = Cast<UB2PCClassInfo>(AssetPtr->GetDefaultObject());

	// bCheckForInGameOnlyInfo �� ���� ������ ���� �ٸ� ����� üũ
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
 * ���� �������� ���� ShouldUnloadAllPCClassInfoOnLevelTransition �� ����� ��!
 */
bool UB2PCClassInfoBox::bManagedPCClassUnload = true;

UB2PCClassInfoBox::UB2PCClassInfoBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

#if WITH_EDITOR && !PLATFORM_MAC// Enum ���̹� üũ. �� ����� �� ������ �־.. ABladeIIPlayer::GetTrimmedNameFromPCClassEnum ����
	if (UB2PCClassInfo::CheckPCClassEnumNamingConvention() == false)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("[B2PCClassInfo] PCClass Enum ��Ͽ� ��û�� ������ ��Ÿ����. ��ǻ�Ͱ� �� �����Ѵ�.")));
	}
#endif

	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII.B2PCClassInfo"), TEXT("ManagedPCClassUnload"), bManagedPCClassUnload, GGameIni);
	}

	MeshSectionMergeInfo = nullptr;
}

// TAsset lazy-loading �� ����ϴ� Blueprint InfoAsset �� ������ ������ �������� �ذ��ϱ� �������� �ӽ� ����.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2PCClassInfoBox::CleanupOnPreSave()
{
	LoadedPtrMap.Empty();
}
void UB2PCClassInfoBox::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
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
				ThisInfo.GetPCClassInfoObject(InfoLoadManager, IMI, LoadedPtrMap); // AssetPtr �� valid ���� �ʴٸ� �̸� �غ��� ���� �� ��.
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
	LoadedPtrMap.Empty(); // Ȯ�λ��
}
void UB2PCClassInfoBox::UnloadExcept(const TArray<EPCClass>& InExceptClasses)
{
	// ManagedPCClassUnload ���� ����� ����. ��� ��ε� ���ϰ� ���ܵη���..
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		FPCClassInfoSearchMap& ThisInfo = InfoMapArray[CII];
		if (InExceptClasses.Contains(ThisInfo.PCClassEnum)) {
			continue;
		}
		ThisInfo.Unload(InfoLoadManager, CII, LoadedPtrMap);
	}
	// ��� ���ܵη��� Ŭ�������� ���� �ε��� ���¸� �ȵǰ���.
	check(LoadedPtrMap.Num() <= InExceptClasses.Num());
}
void UB2PCClassInfoBox::UnloadAllOrExceptSelected()
{
	// ���� transition Ȥ�� StageClear ������ �Ҹ�. ManagedPCClassUnload �� ���� � �� ���������� ���ܵ��� �Ǻ�..
	if (ShouldUnloadAllPCClassInfoOnLevelTransition()) 
	{
		UnloadAll(); // �����ϰ� �˴� �������� ���� �ε��� ���������
	}
	else 
	{
		TArray<EPCClass> UnloadExceptClasses;
		// ���� ���õǾ� �ִ� ����, �±� �� ������ ���ܵδ� �ɷ�. ��� �Ȱ��� �ɷ� �÷��� �ϸ� �ε� �ð��� �پ�� ä�� ������..
		// ��� �κ񿡼��� ���� ������ ���� �������� ĳ���� �״���̱⵵ �ϰ�.
		UnloadExceptClasses.Add(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
		UnloadExceptClasses.Add(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());
		UnloadExcept(UnloadExceptClasses);

		// �׷��� ���� �̰� �ǵ���� ���������� �ʾƼ�.. �Ʒ� UnloadAllOrExceptSelectedIfAllowed �� ���� �����ϰ� ��.
	}
}
void UB2PCClassInfoBox::UnloadAllOrExceptSelectedIfAllowed()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING]
	// iOS ������ �̰� ����ϴ��� �ʿ��� �͸� ���ܳ���.
	UnloadAllOrExceptSelected();
#else
	if (ShouldUnloadAllPCClassInfoOnLevelTransition())
	{
		UnloadAll(); // �����ϰ� �˴� �������� ���� �ε��� ���������
	}
	else
	{
		// ���⼱ ManagedPCClassUnload �� ��� �ϰ͵� ���� �ʴ´�.
		// �� �� �ε��� PCClass ������ ������ ��� �ִ� ��.
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
	return !bManagedPCClassUnload; // ���� ���� iOS �� �� ����. ��, �ణ�� �ٸ� �� �ִ�..
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

// �� �ε� progress ���� ���� ����
bool UB2PCClassInfoBox::IsClassDataAtRootSet(EPCClass InClass, bool bCheckForInGameOnlyInfo)
{
	// ���� �� �� �ȵ����� �Ź� iteration ���� �� �� ���� ������ ���� ��. �̤�
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
	// GetBaseBPClass �� null �� �Ѱ��شٰ� �ؼ� ���� �۵� ���ϴ� �� �ƴϰ� asyn �ε� flush �� �Ͼ�� �� ����ϴ� �ǵ� �����Ϳ��� �׷��� �ž� ������ �� ����.
	TSubclassOf<class ABladeIIPlayer> PCInfoBaseBPClass = PCInfo->GetBaseBPClass(nullptr);
	if (PCInfoBaseBPClass)
	{
		ABladeIIPlayer* DefaultPlayerObj = Cast<ABladeIIPlayer>(PCInfoBaseBPClass->GetDefaultObject());
		USkeletalMeshComponent* SkeletalMeshComponent = DefaultPlayerObj->GetMesh();

		// ������л�ȡUSkeletalMesh
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
	const int32 TotalProgUpdateCount = InfoMapArray.Num(); // �̰� �ϳ� �ε��� �� ���� progress üũ
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	// �ѹ� �̸� �ҷ��ָ� AssetPtr �� valid �� �� ��. ���� �� ���߿� �޸𸮴� ��â �԰���.
	for (int32 CII = 0; CII < InfoMapArray.Num(); ++CII)
	{
		////////////////////////////// ���� �ε�
		UB2PCClassInfo* ThisLoadedinfo = InfoMapArray[CII].GetPCClassInfoObject(InfoLoadManager, CII, LoadedPtrMap);
		//////////////////////////////

		if (InSplashProgFnPtr)
		{
			CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount * 0.2f);
			InSplashProgFnPtr(CurrProg);
		}

		////////////////////////////// ���� �ε�
		if (ThisLoadedinfo)
		{ // ���� �ΰ��� ���� ���µ鵵 �ε�. ���⿡ �ΰ��� AnimBP �� ����־ �� ŭ.
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
				FString::Printf(TEXT("[Warning] PCClassInfoBox �� ���� ClassInfo �� PCClassEnum ���� ���� ����ġ�� ����. ��ǻ�Ͱ� �� �����Ѵ�."))
				));
		}
	}
#endif
}
#endif