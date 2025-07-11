// Fill out your copyright notice in the Description page of Project Settings.


#include "B2SomeInfo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "B2DropItem.h"
#include "BladeIIPlayer.h"
#include "B2PCClassInfo.h"
#include "BladeIICharacter.h"
#include "B2NPCClassInfo.h"


#include "BladeIIUtil.h"

UB2SomeInfo::UB2SomeInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LoadedStageClearWingFxTemplate = NULL;
	LoadedStageClearGemFxTemplate = NULL;
	LoadedStageClearBGFxTemplate = NULL;
	//LoadedDefaultPCClassInfoBaseBPClass = NULL;
	//LoadedDefaultNPCClassInfoBaseBPClass = NULL;
	DefaultDropItemBaseBPClass = NULL;
	LoadedDefaultNPCClassInfoMeleeBT = NULL;
	LoadedDefaultNPCClassInfoSpawnBT = NULL;
	LoadedWeaponExtraEffect_Level20 = NULL;
	LoadedWeaponExtraEffect_Level30 = NULL;
	LoadedWeaponExtraEffect_Level40_Gl = NULL;
	LoadedWeaponExtraEffect_Level40_As = NULL;
	LoadedWeaponExtraEffect_Level40_Fi = NULL;
	LoadedWeaponExtraEffect_Level40_Wz = NULL;
	LoadedWeaponExtraEffect_Level50_Gl = NULL;
	LoadedWeaponExtraEffect_Level50_As = NULL;
	LoadedWeaponExtraEffect_Level50_Fi = NULL;
	LoadedWeaponExtraEffect_Level50_Wz = NULL;
	LoadedSurviveSkillRestoreEffect = NULL;
	UseWeaponExtraEffect_Level40 = 0;
	UseWeaponExtraEffect_Level50 = 0;
}

UParticleSystem* UB2SomeInfo::GetStageClearWingFxTemplate()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, StageClearWingFxTemplate, &LoadedStageClearWingFxTemplate, TEXT("SomeInfo"));
}
void UB2SomeInfo::UnloadStageClearWingFxTemplate()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, StageClearWingFxTemplate, &LoadedStageClearWingFxTemplate);
}

UParticleSystem* UB2SomeInfo::GetStageClearGemFxTemplate()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, StageClearGemFxTemplate, &LoadedStageClearGemFxTemplate, TEXT("SomeInfo"));
}
void UB2SomeInfo::UnloadStageClearGemFxTemplate()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, StageClearGemFxTemplate, &LoadedStageClearGemFxTemplate);
}

UParticleSystem* UB2SomeInfo::GetStageClearBGFxTemplate()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, StageClearBGFxTemplate, &LoadedStageClearBGFxTemplate, TEXT("SomeInfo"));
}
void UB2SomeInfo::UnloadStageClearBGFxTemplate()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, StageClearBGFxTemplate, &LoadedStageClearBGFxTemplate);
}

//TSubclassOf<ABladeIIPlayer> UB2SomeInfo::GetDefaultPCClassInfoBaseBPClass()
//{
//	return GetSomeLazyLoadedClassAssetCommon<ABladeIIPlayer>(InfoLoadManager, DefaultPCClassInfoBaseBPClass, &LoadedDefaultPCClassInfoBaseBPClass, TEXT("SomeInfo"));
//}
//void UB2SomeInfo::UnloadDefaultPCClassInfoBaseBPClass()
//{
//	UnloadSomeLazyLoadedClassAssetCommon<ABladeIIPlayer>(InfoLoadManager, DefaultPCClassInfoBaseBPClass, &LoadedDefaultPCClassInfoBaseBPClass);
//}

TSubclassOf<AB2DropItem> UB2SomeInfo::GetDefaultDropItemBaseBPClass()
{
	return GetSomeLazyLoadedClassAssetCommon<AB2DropItem>(InfoLoadManager, DefaultDropItemBaseBPClass, &LoadedDefaultDropItemBaseBPClass, TEXT("SomeInfo"));
}
void UB2SomeInfo::UnloadDefaultDropItemBaseBPClass()
{
	UnloadSomeLazyLoadedClassAssetCommon<AB2DropItem>(InfoLoadManager, DefaultDropItemBaseBPClass, &LoadedDefaultDropItemBaseBPClass);
}

//TSubclassOf<ABladeIICharacter> UB2SomeInfo::GetDefaultNPCClassInfoBaseBPClass()
//{
//	return GetSomeLazyLoadedClassAssetCommon<ABladeIICharacter>(InfoLoadManager, DefaultNPCClassInfoBaseBPClass, &LoadedDefaultNPCClassInfoBaseBPClass, TEXT("SomeInfo"));
//}
void UB2SomeInfo::UnloadDefaultNPCClassInfoBaseBPClass()
{
	//UnloadSomeLazyLoadedClassAssetCommon<ABladeIICharacter>(InfoLoadManager, DefaultNPCClassInfoBaseBPClass, &LoadedDefaultNPCClassInfoBaseBPClass);
}

UBehaviorTree* UB2SomeInfo::GetDefaultNPCClassInfoMeleeBT()
{
	return GetSomeLazyLoadedAssetCommon<UBehaviorTree>(InfoLoadManager, DefaultNPCClassInfoMeleeBT, &LoadedDefaultNPCClassInfoMeleeBT, TEXT("SomeInfo"));
}
void UB2SomeInfo::UnloadDefaultNPCClassInfoMeleeBT()
{
	UnloadSomeLazyLoadedAssetCommon<UBehaviorTree>(InfoLoadManager, DefaultNPCClassInfoMeleeBT, &LoadedDefaultNPCClassInfoMeleeBT);
}

class UBehaviorTree* UB2SomeInfo::GetMountAutoBT()
{
	return GetSomeLazyLoadedAssetCommon<UBehaviorTree>(InfoLoadManager, MountAutoBT, &LoadedMountAutoBT, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadMountAutoBT()
{
	UnloadSomeLazyLoadedAssetCommon<UBehaviorTree>(InfoLoadManager, MountAutoBT, &LoadedMountAutoBT);
}

UBehaviorTree* UB2SomeInfo::GetDefaultNPCClassInfoSpawnBT()
{
	return GetSomeLazyLoadedAssetCommon<UBehaviorTree>(InfoLoadManager, DefaultNPCClassInfoSpawnBT, &LoadedDefaultNPCClassInfoSpawnBT, TEXT("SomeInfo"));
}
void UB2SomeInfo::UnloadDefaultNPCClassInfoSpawnBT()
{
	UnloadSomeLazyLoadedAssetCommon<UBehaviorTree>(InfoLoadManager, DefaultNPCClassInfoSpawnBT, &LoadedDefaultNPCClassInfoSpawnBT);
}

UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level20()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level20, &LoadedWeaponExtraEffect_Level20, TEXT("SomeInfo"));
}
void UB2SomeInfo::UnloadWeaponExtraEffect_Level20()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level20, &LoadedWeaponExtraEffect_Level20);
}

UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level30()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level30, &LoadedWeaponExtraEffect_Level30, TEXT("SomeInfo"));
}
void UB2SomeInfo::UnloadWeaponExtraEffect_Level30()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level30, &LoadedWeaponExtraEffect_Level30);
}

class UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level40_Gl()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level40_Gl, &LoadedWeaponExtraEffect_Level40_Gl, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadWeaponExtraEffect_Level40_Gl()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level40_Gl, &LoadedWeaponExtraEffect_Level40_Gl);
}

class UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level40_As()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level40_As, &LoadedWeaponExtraEffect_Level40_As, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadWeaponExtraEffect_Level40_As()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level40_As, &LoadedWeaponExtraEffect_Level40_As);
}

class UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level40_Fi()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level40_Fi, &LoadedWeaponExtraEffect_Level40_Fi, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadWeaponExtraEffect_Level40_Fi()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level40_Fi, &LoadedWeaponExtraEffect_Level40_Fi);
}

class UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level40_Wz()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level40_Wz, &LoadedWeaponExtraEffect_Level40_Wz, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadWeaponExtraEffect_Level40_Wz()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level40_Wz, &LoadedWeaponExtraEffect_Level40_Wz);
}

int32 UB2SomeInfo::GetUseWeaponExtraEffect_Level40()
{
	return UseWeaponExtraEffect_Level40;
}

class UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level50_Gl()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level50_Gl, &LoadedWeaponExtraEffect_Level50_Gl, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadWeaponExtraEffect_Level50_Gl()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level50_Gl, &LoadedWeaponExtraEffect_Level50_Gl);
}

class UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level50_As()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level50_As, &LoadedWeaponExtraEffect_Level50_As, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadWeaponExtraEffect_Level50_As()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level50_As, &LoadedWeaponExtraEffect_Level50_As);
}

class UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level50_Fi()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level50_Fi, &LoadedWeaponExtraEffect_Level50_Fi, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadWeaponExtraEffect_Level50_Fi()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level50_Fi, &LoadedWeaponExtraEffect_Level50_Fi);
}

class UParticleSystem* UB2SomeInfo::GetWeaponExtraEffect_Level50_Wz()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level50_Wz, &LoadedWeaponExtraEffect_Level50_Wz, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadWeaponExtraEffect_Level50_Wz()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, WeaponExtraEffect_Level50_Wz, &LoadedWeaponExtraEffect_Level50_Wz);
}

int32 UB2SomeInfo::GetUseWeaponExtraEffect_Level50()
{
	return UseWeaponExtraEffect_Level50;
}

class UParticleSystem* UB2SomeInfo::GetSurviveSkillRestoreEffect()
{
	return GetSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, SurviveSkillRestoreEffect, &LoadedSurviveSkillRestoreEffect, TEXT("SomeInfo"));
}

void UB2SomeInfo::UnloadSurviveSkillRestoreEffect()
{
	UnloadSomeLazyLoadedAssetCommon<UParticleSystem>(InfoLoadManager, SurviveSkillRestoreEffect, &LoadedSurviveSkillRestoreEffect);
}

UMaterialInterface* UB2SomeInfo::GetPCIntroFlipbookMtrl(EPCClass InPCClass)
{
	int32 CastedPCI = PCClassToInt(InPCClass);
	if (PCIntroFlipbookMtrls.IsValidIndex(CastedPCI))
	{
		UMaterialInterface* LoadedMtrl = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, PCIntroFlipbookMtrls[CastedPCI], &LoadedMtrl, TEXT("SomeInfo"));
		if (LoadedMtrl)
		{
			LoadedPCIntroFlipbookMtrls.AddUnique(LoadedMtrl);
			return LoadedMtrl;
		}
	}

	return NULL;
}
void UB2SomeInfo::UnloadAllPCIntroFlipbookMtrl()
{
	// PCClass 별로 언로딩하려면 LoadedPCIntroFlipbookMtrls 에 인덱스 맞춰 넣어야 해서 일단 일괄 언로딩. 굳이 필요도 없을 거 같고.
	LoadedPCIntroFlipbookMtrls.Empty();
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		UMaterialInterface* DummyLoadedPtr = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, PCIntroFlipbookMtrls[PCI], &DummyLoadedPtr);
	}
}

// 추가되는 것들 마찬가지로..


// 아래 LoadAndConsume~~ 어쩌구는 좀 특별한 사연이다. 일반적인 게임 기능을 위한 코드가 아님
void UB2SomeInfo::LoadAndConsumeUniquePreRenderPS(TArray<class UParticleSystem*>& OutLoaded)
{
	for (TSoftObjectPtr<UParticleSystem>& ThisToLoad : UniquePreRenderParticleSystems)
	{
		UParticleSystem* ThisLoaded = ThisToLoad.LoadSynchronous(); // 제한적인 상황에서 사용하므로 이건 단순 LoadSynchronous
		if (ThisLoaded)
		{
			OutLoaded.Add(ThisLoaded);
		}
	}

	// 로딩한 걸 이쪽에 따로 캐싱하는 것도 아니고 Unload 처리를 명시적으로 하지 않는다. 단지 한번 쓰고 말 것임을 명확히 하기 위해 비움. 
	// 에디터라면 물론 비워서는 안되겠다.
	if (!GIsEditor)
	{
		UniquePreRenderParticleSystems.Empty();
	}
}
void UB2SomeInfo::LoadAndConsumeUniquePreRenderNPCClass(TArray<TSubclassOf<class UB2NPCSingleClassInfo>>& OutLoaded)
{
	for (TSoftClassPtr<UB2NPCSingleClassInfo>& ThisToLoad : UniquePreRenderNPCClass)
	{
		TSubclassOf<class UB2NPCSingleClassInfo> ThisLoaded = ThisToLoad.LoadSynchronous(); // 제한적인 상황에서 사용하므로 이건 단순 LoadSynchronous
		if (ThisLoaded)
		{
			OutLoaded.Add(ThisLoaded);
		}
	}
	// 로딩한 걸 이쪽에 따로 캐싱하는 것도 아니고 Unload 처리를 명시적으로 하지 않는다. 단지 한번 쓰고 말 것임을 명확히 하기 위해 비움. 
	// 에디터라면 물론 비워서는 안되겠다.
	if (!GIsEditor)
	{
		UniquePreRenderNPCClass.Empty();
	}
}
void UB2SomeInfo::LoadAndConsumeUniquePreRenderSKMesh(TArray<class USkeletalMesh*>& OutLoaded)
{
	for (TSoftObjectPtr<USkeletalMesh>& ThisToLoad : UniquePreRenderSkMeshes)
	{
		USkeletalMesh* ThisLoaded = ThisToLoad.LoadSynchronous(); // 제한적인 상황에서 사용하므로 이건 단순 LoadSynchronous
		if (ThisLoaded)
		{
			OutLoaded.Add(ThisLoaded);
		}
	}

	// 로딩한 걸 이쪽에 따로 캐싱하는 것도 아니고 Unload 처리를 명시적으로 하지 않는다. 단지 한번 쓰고 말 것임을 명확히 하기 위해 비움. 
	// 에디터라면 물론 비워서는 안되겠다.
	if (!GIsEditor)
	{
		UniquePreRenderSkMeshes.Empty();
	}
}

void UB2SomeInfo::UnloadAll()
{
	UnloadStageClearWingFxTemplate();
	UnloadStageClearGemFxTemplate();
	UnloadStageClearBGFxTemplate();
	//UnloadDefaultPCClassInfoBaseBPClass();
	UnloadDefaultNPCClassInfoBaseBPClass();
	UnloadDefaultDropItemBaseBPClass();
	UnloadDefaultNPCClassInfoMeleeBT();
	UnloadMountAutoBT();
	UnloadDefaultNPCClassInfoSpawnBT();
	UnloadWeaponExtraEffect_Level20();
	UnloadWeaponExtraEffect_Level30();
	UnloadWeaponExtraEffect_Level40_Gl();
	UnloadWeaponExtraEffect_Level40_As();
	UnloadWeaponExtraEffect_Level40_Wz();
	UnloadWeaponExtraEffect_Level40_Fi();
	UnloadWeaponExtraEffect_Level50_Gl();
	UnloadWeaponExtraEffect_Level50_As();
	UnloadWeaponExtraEffect_Level50_Wz();
	UnloadWeaponExtraEffect_Level50_Fi();
	UnloadSurviveSkillRestoreEffect();
	UnloadAllPCIntroFlipbookMtrl();

	// UniquePreRenderParticleSystems 는 포함 대상이 아님
}

#if WITH_EDITOR
void UB2SomeInfo::EditorLoadAll()
{
	if (GIsEditor == false){
		return;
	}

	// 한번 미리 불러주면 AssetPtr 이 valid 해 질 것. 물론 이 와중에 메모리는 왕창 먹겠지.
	GetStageClearWingFxTemplate();
	GetStageClearGemFxTemplate();
	GetStageClearBGFxTemplate();
	//GetDefaultPCClassInfoBaseBPClass();
	//GetDefaultNPCClassInfoBaseBPClass();
	GetDefaultDropItemBaseBPClass();
	GetDefaultNPCClassInfoMeleeBT();
	GetMountAutoBT();
	GetDefaultNPCClassInfoSpawnBT();
	GetWeaponExtraEffect_Level20();
	GetWeaponExtraEffect_Level30();
	GetWeaponExtraEffect_Level40_Gl();
	GetWeaponExtraEffect_Level40_As();
	GetWeaponExtraEffect_Level40_Wz();
	GetWeaponExtraEffect_Level40_Fi();
	GetWeaponExtraEffect_Level50_Gl();
	GetWeaponExtraEffect_Level50_As();
	GetWeaponExtraEffect_Level50_Wz();
	GetWeaponExtraEffect_Level50_Fi();
	GetSurviveSkillRestoreEffect();

	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		GetPCIntroFlipbookMtrl(IntToPCClass(PCI));
	}

	// UniquePreRenderParticleSystems 는 여기서도 빼자..
}
#endif

// TAsset lazy-loading 을 사용하는 Blueprint InfoAsset 의 오동작 문제를 정식으로 해결하기 전까지의 임시 땜질.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2SomeInfo::CleanupOnPreSave()
{
	//LoadedStageClearWingFxTemplate = NULL;
	//LoadedStageClearGemFxTemplate = NULL;
	//LoadedStageClearBGFxTemplate = NULL;
	//LoadedDefaultPCClassInfoBaseBPClass = NULL;
	//LoadedDefaultNPCClassInfoBaseBPClass = NULL;
	//LoadedDefaultDropItemBaseBPClass = NULL;
	//LoadedDefaultNPCClassInfoMeleeBT = NULL;
	//LoadedDefaultNPCClassInfoSpawnBT = NULL;
	//LoadedWeaponExtraEffect_Level20 = NULL;
	//LoadedWeaponExtraEffect_Level30 = NULL;
	//LoadedSurviveSkillRestoreEffect = NULL;
	//LoadedPCIntroFlipbookMtrls.Empty();
}
void UB2SomeInfo::PreSave(FObjectPreSaveContext SaveContext) 
{
	Super::PreSave(SaveContext);

	// 의도치 않게 저장된 레퍼런스를 날려준다.
	CleanupOnPreSave();
}
#endif