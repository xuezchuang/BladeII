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
	// PCClass ���� ��ε��Ϸ��� LoadedPCIntroFlipbookMtrls �� �ε��� ���� �־�� �ؼ� �ϴ� �ϰ� ��ε�. ���� �ʿ䵵 ���� �� ����.
	LoadedPCIntroFlipbookMtrls.Empty();
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		UMaterialInterface* DummyLoadedPtr = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, PCIntroFlipbookMtrls[PCI], &DummyLoadedPtr);
	}
}

// �߰��Ǵ� �͵� ����������..


// �Ʒ� LoadAndConsume~~ ��¼���� �� Ư���� �翬�̴�. �Ϲ����� ���� ����� ���� �ڵ尡 �ƴ�
void UB2SomeInfo::LoadAndConsumeUniquePreRenderPS(TArray<class UParticleSystem*>& OutLoaded)
{
	for (TSoftObjectPtr<UParticleSystem>& ThisToLoad : UniquePreRenderParticleSystems)
	{
		UParticleSystem* ThisLoaded = ThisToLoad.LoadSynchronous(); // �������� ��Ȳ���� ����ϹǷ� �̰� �ܼ� LoadSynchronous
		if (ThisLoaded)
		{
			OutLoaded.Add(ThisLoaded);
		}
	}

	// �ε��� �� ���ʿ� ���� ĳ���ϴ� �͵� �ƴϰ� Unload ó���� ��������� ���� �ʴ´�. ���� �ѹ� ���� �� ������ ��Ȯ�� �ϱ� ���� ���. 
	// �����Ͷ�� ���� ������� �ȵǰڴ�.
	if (!GIsEditor)
	{
		UniquePreRenderParticleSystems.Empty();
	}
}
void UB2SomeInfo::LoadAndConsumeUniquePreRenderNPCClass(TArray<TSubclassOf<class UB2NPCSingleClassInfo>>& OutLoaded)
{
	for (TSoftClassPtr<UB2NPCSingleClassInfo>& ThisToLoad : UniquePreRenderNPCClass)
	{
		TSubclassOf<class UB2NPCSingleClassInfo> ThisLoaded = ThisToLoad.LoadSynchronous(); // �������� ��Ȳ���� ����ϹǷ� �̰� �ܼ� LoadSynchronous
		if (ThisLoaded)
		{
			OutLoaded.Add(ThisLoaded);
		}
	}
	// �ε��� �� ���ʿ� ���� ĳ���ϴ� �͵� �ƴϰ� Unload ó���� ��������� ���� �ʴ´�. ���� �ѹ� ���� �� ������ ��Ȯ�� �ϱ� ���� ���. 
	// �����Ͷ�� ���� ������� �ȵǰڴ�.
	if (!GIsEditor)
	{
		UniquePreRenderNPCClass.Empty();
	}
}
void UB2SomeInfo::LoadAndConsumeUniquePreRenderSKMesh(TArray<class USkeletalMesh*>& OutLoaded)
{
	for (TSoftObjectPtr<USkeletalMesh>& ThisToLoad : UniquePreRenderSkMeshes)
	{
		USkeletalMesh* ThisLoaded = ThisToLoad.LoadSynchronous(); // �������� ��Ȳ���� ����ϹǷ� �̰� �ܼ� LoadSynchronous
		if (ThisLoaded)
		{
			OutLoaded.Add(ThisLoaded);
		}
	}

	// �ε��� �� ���ʿ� ���� ĳ���ϴ� �͵� �ƴϰ� Unload ó���� ��������� ���� �ʴ´�. ���� �ѹ� ���� �� ������ ��Ȯ�� �ϱ� ���� ���. 
	// �����Ͷ�� ���� ������� �ȵǰڴ�.
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

	// UniquePreRenderParticleSystems �� ���� ����� �ƴ�
}

#if WITH_EDITOR
void UB2SomeInfo::EditorLoadAll()
{
	if (GIsEditor == false){
		return;
	}

	// �ѹ� �̸� �ҷ��ָ� AssetPtr �� valid �� �� ��. ���� �� ���߿� �޸𸮴� ��â �԰���.
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

	// UniquePreRenderParticleSystems �� ���⼭�� ����..
}
#endif

// TAsset lazy-loading �� ����ϴ� Blueprint InfoAsset �� ������ ������ �������� �ذ��ϱ� �������� �ӽ� ����.
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

	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
	CleanupOnPreSave();
}
#endif