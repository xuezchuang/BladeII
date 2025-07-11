// Fill out your copyright notice in the Description page of Project Settings.


#include "B2PCClassInfo.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
//#include "Particles/ParticleSystem.h"
//#include "Components/DecalComponent.h"
//#include "Components/ChainComponent.h"
//#include "PhysicsEngine/RadialForceComponent.h"

//#include "B2SkillInfo.h"
//#include "B2InGameHUDInfo_Player.h"
//#include "BladeIIProjectile.h"
//#include "B2AreaDamageActorBase.h"
#include "BladeIIGameMode.h"
//#include "B2LobbyGameMode.h"
//#include "B2LobbyInventory.h"
//#include "B2ItemInfo.h"
#include "B2PCClassInfoBox.h"
//#include "SkeletalMeshMerge.h"
//#include "Animation/SkeletalMeshActor.h"
#include "B2WingInfo.h"
#include "B2Airport.h"
#include "B2SomeInfo.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
//#include "B2CompositeMeshCache.h"
#include "FB2ErrorMessage.h"
//#include "B2AssetLoader.h"
//#include "B2UIDocHelper.h"
//#include "B2GameInstance.h"

#define EQUIPMENTS_EFFECT

UB2InGameOnlyPCClassInfo::UB2InGameOnlyPCClassInfo(const FObjectInitializer& ObjectInitializer)
{
	BaseBPClass = NULL;
	AnimBPOverride = NULL;
	BehaviorTreeOverride = NULL;
	FloorRingPSOverride = NULL;
	ReadyWeaponSkillFXOverride = NULL;

	FloorRingColorMultiplier = 1.f;
}

TSubclassOf<ABladeIIPlayer> UB2InGameOnlyPCClassInfo::GetBaseBPClass(UObject* WorldContextObject)
{
	if (BaseBPClass)
	{
		return BaseBPClass;
	}

	//UB2GameInstance* B2GI = WorldContextObject ? Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)) : nullptr;
	//UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : StaticFindSomeInfo();
	return/* SomeInfo ? SomeInfo->GetDefaultPCClassInfoBaseBPClass() :*/ NULL;
}

void ABladeIIPlayer::OverrideByPCClassInfo(class ABladeIIGameMode* InGameMode, UB2PCClassInfo* CustomPCClassInfo, bool bNoOverrideByInGameInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OverrideByPCClassInfo"));

	check(InGameMode);
//	// �Ϲ����� Player spawn �ÿ��� GameMode ���� ������. CustomPCClassInfo ������ Ư���� ���
//	UB2PCClassInfo* FinalUseClassInfo = CustomPCClassInfo ? CustomPCClassInfo : InGameMode->GetCurrentPCClassInfo();
//
//#if !WITH_EDITOR
//	check(FinalUseClassInfo); // �������Ʈ Ŭ������ �׽�Ʈ�ϴ� ��Ȳ������ ���� �� ����..
//#endif
//	if (!FinalUseClassInfo) {
//		return;
//	}
//
//	// Skill LOD Level Setting
//	switch (InGameMode->GetSkillLODType())
//	{
//	case EB2SkillLODType::ESLT_NONE:
//		SkillLOD = 0;
//		break;
//	case EB2SkillLODType::ESLT_ALL_LOD:
//		SkillLOD = 1;
//		break;
//	default:
//	case EB2SkillLODType::ESLT_ONLY_FOR_SELF:
//		SkillLOD = GetSkillLOD();
//		break;
//	}
//
//	// �κ񿡼� �ε� �ȵ� �ֵ�. ��� �ΰ��ӿ����� �ε��ϸ� �Ǵ� �ֵ��� ���⼭ �����͸� ���´�.
//	// �ΰ����̶� ���⿡�� ���� �뵵�� dummy puppet �� bNoOverrideByInGameInfo ���ڸ� �ָ� InGameOnlyInfo �� ������� ���� ���̴�.
//	// Ư�� ���� Ŭ���� ������ ������ ������ ������ �� �޸𸮸� �ִ��� clear �ϴ� ���� �ʿ��� �� �ִ�.
//	UB2InGameOnlyPCClassInfo* InGameOnlyInfo = bNoOverrideByInGameInfo ? nullptr : FinalUseClassInfo->GetInGameOnlyInfo(this);
//
//	// PCClassInfo �� override �ϱ� �� BaseBP override ����
//	if (InGameOnlyInfo)
//	{
//		this->OverrideByBaseBP(InGameOnlyInfo->GetBaseBPClass(this));
//	}
//
//	this->CharacterType = UB2PCClassInfo::FromPCClassToCharacterType(FinalUseClassInfo->PCClassEnum);
//
//	// Rename the actor using EPCClass Enum name. -> It could allow duplicated name if we have remote player.. We don't really have to do renaming.
//	/*const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EPCCLASS_ENUM_PACKAGE_NAME, true);
//	if (EnumPtr)
//	{
//		FString ClassEnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue(FinalUseClassInfo->PCClassEnum));
//		this->Rename(*(TEXT("BladeIIPlayer_") + ClassEnumName));
//	}*/
//
//	UCapsuleComponent* CollisionCapsule = this->GetCapsuleComponent();
//
//	if (CollisionCapsule)
//	{
//		if (FinalUseClassInfo->CapsuleHalfHeightOverride > 0.0f)
//		{
//			CollisionCapsule->SetCapsuleHalfHeight(FinalUseClassInfo->CapsuleHalfHeightOverride);
//		}
//		if (FinalUseClassInfo->CapsuleRadiusOverride > 0.0f)
//		{
//			CollisionCapsule->SetCapsuleRadius(FinalUseClassInfo->CapsuleRadiusOverride);
//		}
//	}
//
//	if (FinalUseClassInfo->DamageNumZOffsetOverride != 0.0f) // - ���� �� �� �ְ�..
//	{
//		this->DamageNumZOffset = FinalUseClassInfo->DamageNumZOffsetOverride;
//	}
//
//	USkeletalMeshComponent* MeshComp = GetMesh();
//
//	if (MeshComp)
//	{
//		if (FinalUseClassInfo->BaseMeshAsset)
//		{
//			// AnimBP �ٲ�ġ�� ���� SetSkeletalMesh ����
//			MeshComp->SetSkeletalMesh(FinalUseClassInfo->BaseMeshAsset); // DefaultPart ���� �����Ƿ� BaseMeshAsset �� �״�� �����Ǵ� ���� ��Ʈ���� merge �ϴ� ��� component �� �����ϴ� ��찡 �� ��.
//			if (InGameOnlyInfo && InGameOnlyInfo->AnimBPOverride)
//			{// �̰� �ᱹ MeshComp->SetAnimInstanceClass ��. ����� ����� ������ ����
//				this->SafeSetAnimBPClass(InGameOnlyInfo->AnimBPOverride);
//			}
//		}
//
//		if (FinalUseClassInfo->MeshScaleOverride > 0.0f)
//		{
//			MeshComp->SetRelativeScale3D(FVector(FinalUseClassInfo->MeshScaleOverride, FinalUseClassInfo->MeshScaleOverride, FinalUseClassInfo->MeshScaleOverride));
//		}
//
//		if (CollisionCapsule)
//		{
//			// Mesh assets have their pivot root at their feet, so lower it by half capsule size to locate it at the center.
//			MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -1.0f * CollisionCapsule->GetScaledCapsuleHalfHeight()));
//		}
//
//		// SkeletalMeshComponent �� ���� �߰� ������Ʈ��. ���⼭ ���̴� �ֵ��� �⺻���� �͵��ε� PC �� ��� �̷� �� ������ �𸣰ڴ�. ���� ������ ���� ���� �� �� (SetupComponentsForParts).
//		for (int32 STI = 0; STI < FinalUseClassInfo->AttachStaticMeshes.Num(); ++STI)
//		{
//			FCHSKCompAttachStaticMeshInfo& ThisInfo = FinalUseClassInfo->AttachStaticMeshes[STI];
//			if (ThisInfo.StaticMeshAsset != NULL)
//			{
//				UStaticMeshComponent* NewAttachComp = NewObject<UStaticMeshComponent>(this, NAME_None, RF_Transient);
//				if (NewAttachComp)
//				{
//					NewAttachComp->bReceivesDecals = false;
//					NewAttachComp->SetStaticMesh(ThisInfo.StaticMeshAsset);
//					NewAttachComp->SetRelativeScale3D(FVector(ThisInfo.AttachScale, ThisInfo.AttachScale, ThisInfo.AttachScale));
//					NewAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh �� Character �� ���̴� Movable ��
//					NewAttachComp->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisInfo.AttachSocketName);
//					NewAttachComp->RegisterComponent();
//				}
//			}
//		}
//		for (int32 STI = 0; STI < FinalUseClassInfo->AttachSkeletalMeshes.Num(); ++STI)
//		{
//			FCHSKCompAttachSkeletalMeshInfo& ThisInfo = FinalUseClassInfo->AttachSkeletalMeshes[STI];
//			if (ThisInfo.SkeletalMeshAsset != NULL)
//			{
//				USkeletalMeshComponent* NewAttachComp = NewObject<USkeletalMeshComponent>(this, NAME_None, RF_Transient);
//				if (NewAttachComp)
//				{
//					NewAttachComp->bReceivesDecals = false;
//					NewAttachComp->SetSkeletalMesh(ThisInfo.SkeletalMeshAsset);
//					NewAttachComp->SetRelativeScale3D(FVector(ThisInfo.AttachScale, ThisInfo.AttachScale, ThisInfo.AttachScale));
//					NewAttachComp->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisInfo.AttachSocketName);
//					NewAttachComp->RegisterComponent();
//				}
//			}
//		}
//		for (int32 STI = 0; STI < FinalUseClassInfo->AttachParticleSystems.Num(); ++STI)
//		{
//			FCHSKCompAttachParticleSystemInfo& ThisInfo = FinalUseClassInfo->AttachParticleSystems[STI];
//			ThisInfo.CreatePSCHelper(this, MeshComp);
//		}
//	}
//
//	if (FinalUseClassInfo->HUDInfoClassOverride)
//	{
//		this->HUDInfoClass = FinalUseClassInfo->HUDInfoClassOverride;
//	}
//
//	UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(this->GetMovementComponent());
//	if (MovementComp)
//	{
//		if (FinalUseClassInfo->CharMovementEssentialSettings.MaxWalkSpeedOverride > 0.0f)
//		{
//			MovementComp->MaxWalkSpeed = FinalUseClassInfo->CharMovementEssentialSettings.MaxWalkSpeedOverride;
//		}
//		if (FinalUseClassInfo->CharMovementEssentialSettings.MaxAccelerationOverride > 0.0f)
//		{
//			MovementComp->MaxAcceleration = FinalUseClassInfo->CharMovementEssentialSettings.MaxAccelerationOverride;
//		}
//		// InitializeCombatStats ��� ���������� �ɼ� ���� ���̵� WalkSpeed �� �ٽ� ������ ����. ���� �ʱⰪ ����� ��.
//		this->CachedBaseMaxWalkSpeed = MovementComp->MaxWalkSpeed;
//	}
//
//	// ���� �ɷ� ���� ������ �� InfoAsset ���ÿ� ���� �޴� �͵�
//	if (FinalUseClassInfo->CombatAbilityEssentialSettings.MaxArmorOverride > 0.0f)
//	{
//		this->MaxArmor = FinalUseClassInfo->CombatAbilityEssentialSettings.MaxArmorOverride;
//		this->Armor = this->MaxArmor;
//	}
//
//	if (FinalUseClassInfo->TargetingDistanceOverride > 0.0f)
//	{
//		this->TargetingDistance = FinalUseClassInfo->TargetingDistanceOverride;
//	}
//	if (FinalUseClassInfo->MaxDistanceToAllowAutoAttackOverride > 0.0f)
//	{
//		this->MaxDistanceToAllowAutoAttack = FinalUseClassInfo->MaxDistanceToAllowAutoAttackOverride;
//	}
//	if (FinalUseClassInfo->DetectDistanceOverride > 0.0f)
//	{
//		this->DetectDistance = FinalUseClassInfo->DetectDistanceOverride;
//	}
//	if (FinalUseClassInfo->MoveToMobAcceptableRadiusOverride > 0.f)
//	{
//		this->MoveToMobAcceptableRadius = FinalUseClassInfo->MoveToMobAcceptableRadiusOverride;
//	}
//
//	// ETM_End �� override condition ���� �� ���� ������ editcondition �� Ȱ���� ����..
//	if (FinalUseClassInfo->bOverride_TargetingMode && FinalUseClassInfo->TargetingModeOverride != ETargetingMode::ETM_End)
//	{
//		this->TargetingMode = FinalUseClassInfo->TargetingModeOverride;
//	}
//
//	if (InGameOnlyInfo)
//	{
//		for (int32 PCI = 0; PCI < InGameOnlyInfo->AdditionalProjectileClasses.Num(); ++PCI)
//		{
//			if (InGameOnlyInfo->AdditionalProjectileClasses[PCI])
//			{
//				this->ProjectileClasses.AddUnique(InGameOnlyInfo->AdditionalProjectileClasses[PCI]);
//			}
//		}
//
//		for (int32 PCI = 0; PCI < InGameOnlyInfo->AdditionalAreaDamageClasses.Num(); ++PCI)
//		{
//			if (InGameOnlyInfo->AdditionalAreaDamageClasses[PCI])
//			{
//				this->AreaDamageClasses.AddUnique(InGameOnlyInfo->AdditionalAreaDamageClasses[PCI]);
//			}
//		}
//
//
//		if (InGameOnlyInfo->DefaultAttackRangeOverride > 0)
//		{
//			DefaultAttackRange = InGameOnlyInfo->DefaultAttackRangeOverride;
//		}
//		if (InGameOnlyInfo->AutoMoveRangeOverride > 0)
//		{
//			AutoMoveRange = InGameOnlyInfo->AutoMoveRangeOverride;
//		}
//	}
//
//	if (InGameOnlyInfo)
//	{
//		FloorRingColor = InGameOnlyInfo->FloorRingColor;
//	}
//
//	if (InGameOnlyInfo)
//	{
//		FloorRingColorMultiplier = FMath::Max(1.f, InGameOnlyInfo->FloorRingColorMultiplier);
//	}
//
//	if (InGameOnlyInfo && InGameOnlyInfo->FloorRingPSOverride)
//	{
//		FloorRingPS = InGameOnlyInfo->FloorRingPSOverride;
//	}
//
//	if (InGameOnlyInfo && InGameOnlyInfo->ReadyWeaponSkillFXOverride)
//	{
//		ReadyWeaponSkillFX = InGameOnlyInfo->ReadyWeaponSkillFXOverride;
//	}
//
//	if (InGameOnlyInfo && InGameOnlyInfo->BehaviorTreeOverride)
//	{
//		SetBehaviorTree(InGameOnlyInfo->BehaviorTreeOverride);
//	}
}

void ABladeIIPlayer::SetUseSectionMergedMesh(bool bUseSectionMerged)
{
	// AnimNotify �� ���� �̰� �ߵ��� �Ǵ� ��Ʈ�� ������ check �� �ɸ� ��. �׷� �� DeferredSetUseSectionMergedMesh �� ���.

	// ������ ������ ���� �� �ִٸ� �װ� �����ͼ� �� ���̰� �ƴϸ� ���� ����
	SetupComponentsForParts(bUseSectionMerged); 

	// �߰� ���� ������ ���� ���µ� ���� ���� �ʿ䵵 ������ Ȯ���� ���� Ʈ��ŷ�� �� �� ������ �𸣰ڱ⵵ �ؼ�

	GetWorldTimerManager().ClearTimer(DeferredSetUseSectionMergedMeshTH); // Deferred �� ȣ������ �� �ִ�.
}
void ABladeIIPlayer::DeferredSetUseSectionMergedMesh(bool bUseSectionMerged)
{
	FTimerManager& TheTM = GetWorldTimerManager();
	TheTM.ClearTimer(DeferredSetUseSectionMergedMeshTH);
	// �ð��� ���⼱ ������ �ش�. ���� �ﰢ ����Ǿ�� �ȴٰų� �ּ��� �ٷ� ���� ƽ�� ����Ǿ�� �ϴ� ����� �ٸ� ����� ã�ƺ�����.
	TheTM.SetTimer(DeferredSetUseSectionMergedMeshTH, FTimerDelegate::CreateUObject(this, &ABladeIIPlayer::SetUseSectionMergedMesh, bUseSectionMerged), 0.1f, false);
}

USkeletalMesh* ABladeIIPlayer::GetMergedMesh(bool bUseSectionMerged)
{
	USkeletalMesh* ResultMesh = nullptr;
	SetupComponentsForParts(bUseSectionMerged, &ResultMesh); // Component ���� ���� ������� �� ���´�.
	return ResultMesh;
}

void ABladeIIPlayer::SetupMeshesMain()
{
//#if WITH_EDITOR
//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	// ��Ʈ ���� �� �ٽ� PCClassInfo �� �ʿ�� �ϹǷ� PCClassEnum ���� ���� �������Ʈ Ŭ������ �׽�Ʈ�ϴ� ��� SetupComponentsForParts �� �����ؾ� ��.
//	const bool bDoSetupComponentsForParts = !(B2GM && B2GM->IsDefaultPawnTestBP());
//#else
//	const bool bDoSetupComponentsForParts = true;
//#endif
//	if (bDoSetupComponentsForParts)
//	{
//#if !UE_BUILD_SHIPPING
//		int32 SetupComponentsCount = 0;
//#endif
//		// ���� ������ �Ϲ� ���� ����ϴ� ���̽��ε� ��κ��� ���� �̰� �ʿ��� ���̴�.
//		// ���� ���� ���� �� ���� ����ϴ� ��忡�� ������� �װ� ����ϴ� �� ����� �Ǹ� �� �⺻ ���� �ʿ������ �� ����.
//		if (!UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this))
//		{
//#if !UE_BUILD_SHIPPING
//			++SetupComponentsCount;
//#endif
//			this->SetupComponentsForParts(false);
//		}
//		// ���� ���� �� ���� ����ϴ� ���̽�. ĳ���� ���� �����ϴ� ��� ��.
//		// �̰� �ڿ� �ҷ��� ���� ���� �� ���� ����Ϸ��� �̰� �⺻ ���õǵ��� ��.
//		if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
//		{
//#if !UE_BUILD_SHIPPING
//			++SetupComponentsCount;
//#endif
//			this->SetupComponentsForParts(true);
//		}
//#if !UE_BUILD_SHIPPING
//		check(SetupComponentsCount > 0); /* �翬�� �ϳ��� �־��. */
//#endif
//	}
}

void ABladeIIPlayer::SetupComponentsForParts(bool bMergeSections, USkeletalMesh** OutGetBuiltResultOnly)
{
	//// ABladeIIPlayer::OverrideByPCClassInfo ����..
	//if (GetCharacterDataStore())
	//{
	//	EPCClass PCClassEnum = GetCurrentPlayerClass();
	//	CharacterDataStore->GetEquippedItems(PCClassEnum, CachedEquippedItem);
	//	CharacterDataStore->GetEquippedCostumeItems(PCClassEnum, CachedEquippedItem);
	//	bHasWing = CharacterDataStore->GetCharacterWing(PCClassEnum, CachedWingData);
	//	ICharacterDataStore::GetRenderItem(CharacterDataStore, PCClassEnum, CachedEquippedItem);

	//	if (GetGameMode()->GetB2GameModeType() == EB2GameMode::Scenario)
	//	{
	//		UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();

	//		// �Ϲݴ����� ���丮 ��ŵ �ƴҶ�! ���� ������ ����.
	//		// CachedWingData ���� ����ü �����ؼ� ���Ƿ� �� �����ص� �ƿ����ӿ� ���� �ȹ�ħ.
	//		if(!DocCon->GetbUserSelectedSkipStory())
	//			CachedWingData.bShouldBeVisible = false;
	//	}

	//	// �ݰݴ����� ���� �Ⱥ���
	//	if (GetGameMode()->GetB2GameModeType() == EB2GameMode::CounterDungeon)
	//		CachedWingData.bShouldBeVisible = false;
	//}

	//// OutGetBuiltResultOnly �� valid �� �����ͷ� �ִ� ��� ������ Component ���� ���õ��� �ʰ� �׳� ������� �� �ش�.
	//SetupComponentsForPartsCommon(CachedEquippedItem, GetWingDataPtrIfValid(), bMergeSections, false, OutGetBuiltResultOnly);
}

void ABladeIIPlayer::SetupComponentsForPartsCustomDataStore(ICharacterDataStore* InCustomDataStore, bool bMergeSections)
{ // �Ϲ������δ� GetCharacterDataStore �� ���� ����� DataStore �� ���.. �̰� Ư���� ��츦 ����.
	//if (InCustomDataStore)
	//{
	//	EPCClass PCClassEnum = GetCurrentPlayerClass(); // �⺻ Ŭ�������� ��ü���� ���� �ʰ�..
	//	TArray<FB2Item> FoundEquipment;
	//	InCustomDataStore->GetEquippedItems(PCClassEnum, FoundEquipment);
	//	InCustomDataStore->GetEquippedCostumeItems(PCClassEnum, FoundEquipment);
	//	FB2Wing FoundWingData;
	//	bool bLocalHasWing = InCustomDataStore->GetCharacterWing(PCClassEnum, FoundWingData);
	//	ICharacterDataStore::GetRenderItem(InCustomDataStore, PCClassEnum, FoundEquipment);

	//	// �̰� ����ϴ� �������� GameEntryID �� �ǵ��ʹ� �ٸ� �� �����Ƿ� ĳ���� ���� �� ������� ���� ForceUpdate �ؼ� ���.
	//	// �� composite mesh ĳ���� ��� ���ձ��� üũ�� �ϹǷ� ���� ������� �������� ���� ���� ���ɿ��� �� ���� ������ ��ġ�� ��.
	//	// ���� ����� ĳ���Ϸ��� �������� ���� ���� ����� GameEntryID �� �Ѱ��� �� �־�� ��.
	//	SetupComponentsForPartsCommon(FoundEquipment, bLocalHasWing ? &FoundWingData : NULL, bMergeSections, true);
	//}
}

void ABladeIIPlayer::SetupComponentsForPartsCommon(const TArray<FB2Item>& EquippedItems, const FB2Wing* InWingData, bool bMergeSections, bool bForceUpdate, class USkeletalMesh** OutGetBuiltResultOnly)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupComponentsForPartsCommon"));

	//// ABladeIIPlayer::OverrideByPCClassInfo ����..

	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	//BII_CHECK(B2GI);

	//UB2PCClassInfoBox* PCInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox(this);
	//UB2PCClassInfo* MyInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(GetCurrentPlayerClass()) : NULL;
	//BII_CHECK(MyInfo);

	//USkeletalMeshComponent* BaseMeshComp = this->GetBaseMesh();
	//BII_CHECK(BaseMeshComp);

	//if (MyInfo && BaseMeshComp)
	//{ // BaseMeshComp �� �⺻ ������ �� ���̶�� MyInfo->BaseMeshAsset �� BaseMeshComp->SkeletalMesh �� �Ƹ��� ���ƾ�..
	//	
	//	USkeletalMesh* PrebuiltMeshOrResult = NULL;
	//	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
	//	if (!bForceUpdate)
	//	{ // ������ �̹� ������ ���� �޽��� �ִٸ� �װ� ���� ����.
	//		PrebuiltMeshOrResult = MeshCacheManager ?
	//			MeshCacheManager->GetCachedCompositeMesh(GetCurrentPlayerClass(), this, EquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections)
	//			: NULL;
	//	}

	//	// Dummy �뵵�� spawn �ϴ� ���.. InGameOnlyInfo �� �ε����� �ʾƵ� �Ǵ� ��찡 ����. �̰� ��ġ�� ũ�Ƿ� ����ȭ ��������.
	//	UB2InGameOnlyPCClassInfo* InGameOnlyInfo = ShouldLoadInGameOnlyInfoForPartSetup() ? MyInfo->GetInGameOnlyInfo(this) : nullptr;

	//	if (UB2PCClassInfo::SetupSKCompForParts(GetCurrentPlayerClass(), this, BaseMeshComp, PrebuiltMeshOrResult, MyInfo->BaseMeshAsset, MyInfo->DefaultPartsInfo,
	//		EquippedItems, InWingData, (InGameOnlyInfo ? InGameOnlyInfo->AnimBPOverride : NULL), false,
	//		bMergeSections,
	//		(OutGetBuiltResultOnly != nullptr) // �� ��� ���� ��� �޽��� ������ ���ֱ⸸ �ϰ�, ���� ������Ʈ�� ������ �� �ϴ� ���� �ν�.
	//		))
	//	{
	//		if (MeshCacheManager && !bForceUpdate) // bForceUpdate �� ��� ��� ���� �� ���� �� ĳ���Ͱ� ���� EntryID �δ� ĳ���ؼ��� �ȵǴ� ���� �� �ִ�. ĳ�̵� ����.
	//		{ // MeshMerging �� ����Ǵ� ��忡���� ����� �������̶�� CompositeMeshCache �ʿ� ĳ���� ����. ���������δ� ���� ������Ʈ�� �Ǿ��� ������ ĳ���� �� ����.
	//			MeshCacheManager->SetCachedCompositeMesh(GetCurrentPlayerClass(), this, PrebuiltMeshOrResult, EquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections);
	//		}

	//		if (OutGetBuiltResultOnly)
	//		{ // Ư���� �ʿ信 ���� ����� �޽��� �ܺη� �� �ִ� ���.
	//			*OutGetBuiltResultOnly = PrebuiltMeshOrResult;
	//		}
	//	}
	//	SetupLODInfoForInGame(); // ���� ����̸� LODInfo �� �ٽ� ���õ� ��.
	//}
}

void ABladeIIPlayer::OverrideByBaseBP(TSubclassOf<class ABladeIICharacter> BPClassToOverride)
{
	//ABladeIIPlayer* BPCDO = BPClassToOverride ? Cast<ABladeIIPlayer>(BPClassToOverride->GetDefaultObject()) : NULL;
	//if (BPCDO)
	//{
	//	// BladeIICharacter �ʿ� ���� ���� ������Ʈ��.
	//	
	//	//CopyObjectProperties(this->RightFootShadow, BPCDO->RightFootShadow, true, true);
	//	//CopyObjectProperties(this->LeftFootShadow, BPCDO->LeftFootShadow, true, true);
	//	CopyObjectProperties(this->RightWeapon, BPCDO->RightWeapon, true, true);
	//	CopyObjectProperties(this->LeftWeapon, BPCDO->LeftWeapon, true, true);
	//	CopyObjectProperties(this->LightComponent, BPCDO->LightComponent, true, true);
	//	CopyObjectProperties(this->GetTopDownCameraComponent(), BPCDO->GetTopDownCameraComponent(), true, true);
	//	CopyObjectProperties(this->GetCameraBoom(), BPCDO->GetCameraBoom(), true, true);
	//	CopyObjectProperties(this->FloorRingPSC, BPCDO->FloorRingPSC, true, true);
	//}

	//Super::OverrideByBaseBP(BPClassToOverride);
}

FString ABladeIIPlayer::GetTrimmedNameFromPCClassEnum()
{
	// �׳� CharacterType ���� ����ص� �ᱹ ���� ��������� PCClassInfo �� �� ���� �� ����.
	//EPCClass PCClassEnum = UB2PCClassInfo::FromCharacterTypeToPCClass(this->CharacterType);
	//
	//return UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(PCClassEnum);
	return FString("");
}

void ABladeIIPlayer::GetAllTrimmedNameFromPCClassEnum(TArray<FString>& OutResult)
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EPCCLASS_ENUM_PACKAGE_NAME, true);
	//BII_CHECK(EnumPtr);

	//if (EnumPtr)
	//{
	//	for (int32 EI = 0; EI < GetMaxPCClassNum(); ++EI)
	//	{
	//		FString ClassEnumName = EnumPtr->GetNameStringByIndex(EI);

	//		// ���̹� ���� �¾ƾ� �Ǵµ� �̰� Ʋ������ �ʵ��� Ȯ���ϴ� �� CheckPCClassEnumNamingConvention
	//		if (ClassEnumName.RemoveFromStart(EPCCLASSENUM_COMMON_PREFIX) == true)
	//		{
	//			OutResult.Add(ClassEnumName);
	//		}
	//	}
	//}
}

FString UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(EPCClass InPCClassEnum)
{
	//if (InPCClassEnum != EPCClass::EPC_End)
	//{
	//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EPCCLASS_ENUM_PACKAGE_NAME, true);
	//	BII_CHECK(EnumPtr);

	//	if (EnumPtr)
	//	{
	//		FString ClassEnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPCClassEnum));

	//		// ���̹� ���� �¾ƾ� �Ǵµ� �̰� Ʋ������ �ʵ��� Ȯ���ϴ� �� CheckPCClassEnumNamingConvention
	//		if (ClassEnumName.RemoveFromStart(EPCCLASSENUM_COMMON_PREFIX) == true)
	//		{
	//			return ClassEnumName;
	//		}
	//	}
	//}

	return TEXT("UnknownPC");  // ECharacterType �ʰ� ���� ���� �ʿ䰡 �ִ� �� �ƴ�.
}

FText UB2PCClassInfo::GetLocalizedName()
{
	return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, LocalizedNameKey.ToString());
}

#if WITH_EDITOR
/** PC Part �� mesh merge �� �⺻ skeleton �� �ҽ����� skeleton ���� ref pose �� �󸶳� �ٸ��� üũ�ؼ� ��� ���. */
void CheckAndWarnForDifferentRefPose(USkeletalMesh* InBaseSKMesh, const TArray<USkeletalMesh*>& CheckMeshes)
{
	if (!InBaseSKMesh){
		return;
	}

	const FReferenceSkeleton& BaseRefSkeleton = InBaseSKMesh->GetRefSkeleton();
	const TArray<FTransform>& BaseRefBonePose = BaseRefSkeleton.GetRefBonePose();
		
	int32 FoundQuestionableCount = 0;
	const float WarnDifferenceStandard = 0.1f; // �̰ͺ��� ����ġ�� ũ�� ��� ���

#if !PLATFORM_MAC
	//FString WarnMessageString = FString::Printf(TEXT("SkeletalMesh ���ս� ������ ���� ���� ��ġ %.2f (��)�� �Ѿ�� ���̽��� ��Ʈ �� ���۷��� ���� ����ġ �߰�"), WarnDifferenceStandard);
	FString WarnMessageString = FString::Printf(TEXT("SkeletalMesh  %.2f"), WarnDifferenceStandard);
#endif

	for (int32 CMI = 0; CMI < CheckMeshes.Num(); ++CMI)
	{
		USkeletalMesh* CurrCheckMesh = CheckMeshes[CMI];
		if (!CurrCheckMesh){
			continue;
		}

		const FReferenceSkeleton& CheckRefSkeleton = CurrCheckMesh->GetRefSkeleton();
		const TArray<FTransform>& CheckRefBonePose = CheckRefSkeleton.GetRefBonePose();

		float TotalDifferenceABS = 0.0f; // �̹� Check mesh �� ���� ref pose �� �ٸ� ���밪 ���� ��
		int TotalCheckCount = 0;

		for (int32 CBPI = 0; CBPI < CheckRefBonePose.Num(); ++CBPI)
		{
			// �� CheckMesh �� �� bone ���� BaseMesh ���� �ش��ϴ� bone �� ref pose �� ã�� �󸶳� ��ġ�ϴ��� üũ

			const FTransform& CurrCheckPose = CheckRefBonePose[CBPI];
			FName CurrCheckBoneName = CheckRefSkeleton.GetBoneName(CBPI);

			int32 BaseBoneIndex = BaseRefSkeleton.FindBoneIndex(CurrCheckBoneName);
			if (BaseBoneIndex >= 0 && BaseBoneIndex < BaseRefBonePose.Num())
			{
				const FTransform& CurrRefBonePose = BaseRefBonePose[BaseBoneIndex];
				TotalDifferenceABS += (CurrCheckPose.GetTranslation() - CurrRefBonePose.GetTranslation()).Size(); // �ϴ� translation ���̸� ����.
				++TotalCheckCount;
			}
		}

		// �̹� CheckMesh �� ���� ����
		if (TotalCheckCount > 0)
		{
			float AvgDifferenceABS = TotalDifferenceABS / (float)TotalCheckCount;
			// �̰� �׳� ���� �α�.
			UE_LOG(LogBladeII, Log, TEXT("Reference pose difference of %s : %f"), *(CurrCheckMesh->GetName()), AvgDifferenceABS);

			if (AvgDifferenceABS > 0.1f) // Ư�� ���� �̻��̸� �޽��� �ڽ� ���
			{
				++FoundQuestionableCount;

#if !PLATFORM_MAC
				WarnMessageString += FString::Printf(TEXT("%d : %s, ����ġ ���� : %.4f\n"), FoundQuestionableCount, *CurrCheckMesh->GetName(), AvgDifferenceABS);
#endif

			}
		}
	}

#if !PLATFORM_MAC
	if (GIsEditor)
	{
		if (FoundQuestionableCount > 0)
		{
			WarnMessageString += TEXT("\n�� ��Ͽ� �ִ��� �ǵ��� �ִϸ��̼��� ���´ٸ� �����ص� �ʤ�");

			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessageString));
			// ���� ���� ��Ŀ� ������ �ִ� ��.. �ϴ� �Ž����� �α׷� ��ü.
			UE_LOG(LogBladeII, Log, TEXT("%s"), *WarnMessageString);
		}
	}
#endif
}
#endif

int32 GetPCCompositeMtrlID(EPCPart InPartID)
{
	return (InPartID == EPCPart::EPCPT_End) ? INDEX_NONE : (int32)(InPartID); // UB2PCClassInfo::SetupSKCompForParts ���� merge �� �� ������ PartEnum �� ���� �����ϹǷ� �״�� int ĳ����
}

EItemEquipPlace GetItemEquipPlaceForPCPart(EPCPart InPCPart)
{
	check((int32)EPCPart::EPCPT_Wing + 1 == (int32)EPCPart::EPCPT_End); // �߰� ��Ʈ�� ��ġ�� �ʰ�

	switch (InPCPart)
	{
	case EPCPart::EPCPT_Face: return EItemEquipPlace::EIEP_End; // Face �� PCPart �θ� �����ϴ� ����. �ش��ϴ� EquipPlace �� ����.
	case EPCPart::EPCPT_Head: return EItemEquipPlace::EIEP_Helmet;
	case EPCPart::EPCPT_Body: return EItemEquipPlace::EIEP_BodyArmor;
	case EPCPart::EPCPT_Leg: return EItemEquipPlace::EIEP_Shoes;
	case EPCPart::EPCPT_Arm: return EItemEquipPlace::EIEP_Glove;
	case EPCPart::EPCPT_Weapon: return EItemEquipPlace::EIEP_Weapon;
	case EPCPart::EPCPT_Robe: return EItemEquipPlace::EIEP_End; // �̰͵� PCPart �θ�. ������ ���� �پ��� �� �̰� ������ ���� ����.
	case EPCPart::EPCPT_Wing: return EItemEquipPlace::EIEP_End;
	}
	return EItemEquipPlace::EIEP_End;
}

EPCPart GetPCPartForItemEquipPlace(EItemEquipPlace InEquipPlace)
{
	check((int32)EPCPart::EPCPT_Wing + 1 == (int32)EPCPart::EPCPT_End); // �߰� ��Ʈ�� ��ġ�� �ʰ�
	check((int32)EItemEquipPlace::EIEP_Ring + 1 == (int32)EItemEquipPlace::EIEP_End);

	switch (InEquipPlace)
	{
	case EItemEquipPlace::EIEP_Weapon: return EPCPart::EPCPT_Weapon;
	case EItemEquipPlace::EIEP_Helmet: return EPCPart::EPCPT_Head;
	case EItemEquipPlace::EIEP_BodyArmor: return EPCPart::EPCPT_Body;
	case EItemEquipPlace::EIEP_Glove: return EPCPart::EPCPT_Arm;
	case EItemEquipPlace::EIEP_Shoes: return EPCPart::EPCPT_Leg;
	}
	return EPCPart::EPCPT_End; // �������� �ΰ��ӿ��� pc �� ���յ� 3D �𵨷� ǥ�������� �ʴ� �͵�.
}

bool IsCostumeEquipPlace(const ECostumeEquipPlace& InCostumeEquipPlace)
{
	bool IsEquipCostume = false;

	switch (InCostumeEquipPlace)
	{
	case ECostumeEquipPlace::ECEP_Weapon:
	case ECostumeEquipPlace::ECEP_Helmet:
	case ECostumeEquipPlace::ECEP_Body:
		IsEquipCostume = true;
		break;

	default:
		break;
	}

	return IsEquipCostume;
}

EPCPart GetExtraAttachPartEnum(EPCPart InMainPart)
{
	if (InMainPart == EPCPart::EPCPT_Body){
		return EPCPart::EPCPT_Robe; // Ư�� ���̽���. ���� ���� ������ �ִ� �� �ƴ����� ���� �ٿ��� �ôٵ� �����ؾ� �ϴ�.
	}
	return EPCPart::EPCPT_End;
}

bool ShouldRemoveOtherPart(EPCClass InPCClass, EPCPart PartToAdd, EPCPart OtherPartToTest)
{ // PartToAdd �� ���� �� OtherPartToTest �� �����ؾ� �ϴ� ��� true ����
	// ��Ÿ �ٸ� ��� ����� �߰��Ѵ�.
	// Fighter �� Robe �� ��ǻ� �ٸ� �뵵�� Wing �� ���δٰ� �ؼ� ������ �ʴ´�.
	//return (InPCClass != EPCClass::EPC_Fighter && PartToAdd == EPCPart::EPCPT_Wing && OtherPartToTest == EPCPart::EPCPT_Robe);

	return PartToAdd == EPCPart::EPCPT_Wing && OtherPartToTest == EPCPart::EPCPT_Robe;	// ���� ���ÿ� ���� �Ұ���
}

FName GetWingEmissiveParamName()
{
	return FName(TEXT("WingEmissive"));
}

bool HasWingAuraEmissiveTexture(UMaterialInterface* OrgMaterial)
{ // Ư�� ��� �̻� ���� material �� �ִ� Emissive �ؽ��� �Ķ����.. ���ֿ̹� ���� üũ�� �ʿ���.
	UTexture* Dummy = nullptr;
	if (OrgMaterial && OrgMaterial->GetTextureParameterValue(FName(TEXT("1_auraMask")), Dummy))
		return true;

	return false;
}


struct FLocalPartMeshMtrlInfo
{
	FLocalPartMeshMtrlInfo(){
		PartEnum = EPCPart::EPCPT_End;
		CostumeEquipPlace = ECostumeEquipPlace::ECEP_None;
		MeshAsset = NULL;
		MaterialAsset = NULL;
	}
	EPCPart PartEnum;
	ECostumeEquipPlace CostumeEquipPlace;
	USkeletalMesh* MeshAsset;
	UMaterialInterface* MaterialAsset; // Valid only when overridden by iteminfo
};

void CombineCostumePart(UB2PCMeshSectionMergeInfo* SectionMergeInfo, TArray<FLocalPartMeshMtrlInfo>& InParts)
{
	if (SectionMergeInfo == nullptr)
		return;

	TArray<EPCPart> CombineParts;
	TArray<EPCPart> CurrCombineParts;

	for (const FLocalPartMeshMtrlInfo& Part : InParts)
	{
		SectionMergeInfo->GetReplaceCostumeEquipPlace(Part.CostumeEquipPlace, CurrCombineParts);
		//�ڱ� �ڽ��� ������ ������ ��ĥ��Ʈ PCPart�� 1:1 �����̸� �� �ʿ䰡 ����
		if (CurrCombineParts.Num() > 0)
			CurrCombineParts.RemoveAt(0);

		if (CurrCombineParts.Num() > 0)
			CombineParts.Append(CurrCombineParts);
	}

	InParts.RemoveAll([&CombineParts](const FLocalPartMeshMtrlInfo& LocalPartInfo)
	{
		return CombineParts.Contains(LocalPartInfo.PartEnum);
	});
}

// �̸� ������ �̰ɷ� ������ ������Ʈ���� �Ǻ��� �Ŷ� ������ ����ũ�ϰ�
#define PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX TEXT("B2ActPCPartXATCHFx")

/** ���� ��� merge �� ���� �߰��� PSC �����ؼ� attach �ϴµ��� ����ϱ� ���� �����͵��� ��Ƴ��� struct. 
 * FCHSKCompAttachParticleSystemInfo �⺻ ��ɿ� �� Ư���� �� ���ٿ��� InfoAsset �� �ƴ� ���� ó�� �뵵�� ����Ѵ�. */
USTRUCT()
struct FPCPartExtraAttachFxSet : public FCHSKCompAttachParticleSystemInfo
{
	FPCPartExtraAttachFxSet()
		: FCHSKCompAttachParticleSystemInfo()
	{
		LoadedAttachPS = NULL;
		ChangeColor = FLinearColor::Transparent;
	}

	FPCPartExtraAttachFxSet& operator = (FCHSKCompAttachParticleSystemInfo& OtherSuperStruct)
	{
		// FCHSKCompAttachParticleSystemInfo �ʿ� ����� �͸� ������ ���Ⲩ ����
		this->AttachSocketName = OtherSuperStruct.AttachSocketName;
		this->AttachScale3D = OtherSuperStruct.AttachScale3D;
		this->LocationOffset = OtherSuperStruct.LocationOffset;
		this->RotationOffset = OtherSuperStruct.RotationOffset;
		this->bIgnoreBoneRotation = OtherSuperStruct.bIgnoreBoneRotation;
		this->LoadedAttachPS = OtherSuperStruct.GetParticleSystemAsset(); // �̰� ������ ����ü�� ������ TAsset �� �ε��ؼ� ���ʿ� ����.
		this->ChangeColor = FLinearColor::Transparent;

		return *this;
	}

	virtual ~FPCPartExtraAttachFxSet() {} // This struct has virtual function so need virtual destructor.

	UPROPERTY(Transient)
	FLinearColor ChangeColor;

	UPROPERTY(Transient)
	UParticleSystem* LoadedAttachPS; // Here we just set already loaded ParticleSystem resource, not the TAsset reference of Super class.

	// GetCompCreateName �� ���� Ŭ�������ٴ� �ַ� ���⼭�� �ʿ信 ���� ���� ��. ���� ���Ͽ� ���� �ٴ´ٰų� �ϸ� �߰� �ĺ��ڸ� �־��..
	virtual FString GetCompCreateName(AActor* InOwner) const override { return LoadedAttachPS ? FString::Printf(TEXT("%s_%s_%s"), PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX, (InOwner ? *InOwner->GetName() : TEXT("_")), *AttachSocketName.ToString()) : TEXT(""); }
	FORCEINLINE static bool IsPossibleCreatedCompName(const FString& InNameToCheck) { return InNameToCheck.StartsWith(PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX); }

	// ���� Ŭ������ TAsset �ε� ��� ���� �� ���.
	virtual UParticleSystem* GetParticleSystemAsset() override { return LoadedAttachPS; }
};
// Just a simple code block to reduce duplicated code. 
void PCPartExtraAttachFxSetInfoExtractHelper(TArray<FPCPartExtraAttachFxSet>& OutAddedArray, UParticleSystem* InPS, const FString& InAttachSocketName, FLinearColor InChangeColor = FLinearColor::Transparent)
{ // �̰� ���� �⺻������ �����ϴ� ��. FCHSKCompAttachParticleSystemInfo ���ÿ��� ������ �� �̷� ������ ���� �ʴ´�.
	if (InPS)
	{
		FPCPartExtraAttachFxSet NewSet;
		NewSet.LoadedAttachPS = InPS;
		NewSet.AttachSocketName = FName(*InAttachSocketName);
		NewSet.ChangeColor = InChangeColor;
		OutAddedArray.Add(NewSet);
	}
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
/** Ư�� �� �� ���� ��� �ϰ� ������ �� �ֵ��� �ϴ� ġƮ. �ܰ���.. */
int32 gForcedPlayerDummyPartGrade = -1; // 1 ~ 6 �� �ָ� ���� ��.

TArray<FB2Item> gCheatCostumeItem;
#endif

// �÷��̾� ĳ���� ��Ʈ ���п� ���� skeletal mesh �� �����ϴ� ���� �Լ���. BladeIIPlayer Ȥ�� �κ� �ھƳ��� SkeletalMeshActor �� �� ����ϴ� ���� �ǵ�.
bool UB2PCClassInfo::SetupSKCompForParts(EPCClass InPCClass, AActor* SKCompOwner, USkeletalMeshComponent* SKCompToSetup,
	USkeletalMesh*& InOutPrebuiltCompositeMesh,
	USkeletalMesh* InBaseMesh,
	const TArray<FPCPartsAssetInfo>& InDefaultParts,
	const TArray<FB2Item>& InEquippedItems,
	const FB2Wing* InWingData,
	UAnimBlueprintGeneratedClass* AnimBPForInGameChar,
	bool bForceUnloadPartResourceAfterMerge,
	bool bInMergeSections,
	bool bBuildAndGetResultMeshOnly // ������Ʈ�� ���� ���ϰ� ��� �޽��� �� �ִ� ���.
)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PCClassInfo::SetupSKCompForParts"));
	B2_SCOPED_TIME_LOG(FString::Printf(TEXT("UB2PCClassInfo::SetupSKCompForParts Owner %s, PCClass %d"), SKCompOwner ? *SKCompOwner->GetName() : TEXT("Unknown"), PCClassToInt(InPCClass)));
//	
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ���� ���� �׽�Ʈ �÷���
//	if (UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge()) {
//		bInMergeSections = true;
//	}
//	else if (UB2PCMeshSectionMergeInfo::ShouldForceUnuseSectionMerge()) {
//		bInMergeSections = false;
//	}
//#endif
//
//	check(SKCompOwner && SKCompToSetup && InBaseMesh);
//	ABladeIIPlayer* CastedB2Player = Cast<ABladeIIPlayer>(SKCompOwner);
//	ASkeletalMeshActor* CastedSKActor = Cast<ASkeletalMeshActor>(SKCompOwner);
//	check(CastedB2Player || CastedSKActor); // �ΰ��� ĳ���Ͱų� �κ� �ھƳ��� ���Ͱų�.
//	// SKCompToSetup �� SKCompOwner ���� ����
//	check((CastedB2Player && CastedB2Player->GetBaseMesh() == SKCompToSetup) || (CastedSKActor && CastedSKActor->GetSkeletalMeshComponent() == SKCompToSetup));
//
//	UB2ItemInfo* IteminfoTable = StaticFindItemInfo();
//	BII_CHECK(IteminfoTable);
//
//	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(SKCompOwner));
//	UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : NULL;
//	BII_CHECK(SomeInfo);
//
//	UB2WingInfo* WingInfoTable = B2GI ? B2GI->GetWingInfo() : StaticFindWingInfo();
//	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
//
//	// �̰͵��� ���� �Ŀ� �����Ϸ���.
//	TArray<FSingleItemInfoData*> AllLoadedItemInfo;
//	FSingleWingInfoData* LoadedWingInfo = NULL; // ������ NULL �� �� �ִ�.
//
//	UB2PCClassInfoBox* PCInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox(SKCompOwner);
//	// Ư�� ��忡�� ���� Ȯ���� ���� section merge �� �����ϴ� ���� �ʿ��� ������ ��� �ִ� info asset.
//	UB2PCMeshSectionMergeInfo* SectionMergeInfo = PCInfoBox ? PCInfoBox->GetMeshSectionMergeInfo() : nullptr;
//	if (bInMergeSections)
//	{
//		check(SectionMergeInfo);
//		if (!SectionMergeInfo) {
//			return false;
//		}
//	}
//	// SectionMerge �ÿ� �� ���� �ȿ��� UV ������ �ٸ��� ����ϱ� ���� ����. Section merge �� �� �� �Ÿ� �ʿ������.
//	FSkelMeshMergeUVTransforms SectionMergeUVTransform;
//	
//	// ������ merge �� �޽��� �־����ٸ� ���⼭ �ϴ� ���� ���� �ʾƾ� �Ѵ�. ���� ������ ������� ���� ������ ���� �� ������ ��ŵ������ ���ϰ�.
//	const bool bPrebuiltMeshProvided = (InOutPrebuiltCompositeMesh != nullptr);
//
//#if !UE_BUILD_SHIPPING
//	extern bool GTest_UseNonCompositedRawPCMesh;
//	if (GTest_UseNonCompositedRawPCMesh)
//	{ // �⺻ �޽� ����ϴ� �׽�Ʈ ���
//		SKCompToSetup->SetSkeletalMesh(InBaseMesh);
//		return false;
//	}
//#endif
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	/** Ư�� �� �� ���� ��� �ϰ� ������ �� �ֵ��� �ϴ� ġƮ..  
//	 ġƮ�� �׳� �� �ȿ� �ִ� ���ε�, ���� ��ɿ����� �� �ȿ��� EquippedItems �� ���ٸ� ���� ���� ������ override �ؼ��� �ȵȴ�. 
//	 �ϴ� FB2GMPCCompositeMeshCacher ���� ���� ��� ���� ������ mesh �� ĳ���ؼ� ���� ������ �װͺ��� ��߳� ����. */
//	if (gForcedPlayerDummyPartGrade >= 1 && gForcedPlayerDummyPartGrade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE)
//	{
//		TArray<FB2Item> NewDummyPart;
//		GetStandardDummyPCPartItemData(InPCClass, gForcedPlayerDummyPartGrade, NewDummyPart);
//		TArray<FB2Item>* EquipitemPtrToModify = const_cast<TArray<FB2Item>*>(&InEquippedItems); // ���� ����� ġƮ�̹Ƿ� �ټҰ� ������ �������..
//		if (EquipitemPtrToModify)
//		{
//			EquipitemPtrToModify->Empty();
//			*EquipitemPtrToModify = NewDummyPart;
//		}
//	}
//
//	if (gCheatCostumeItem.Num() > 0)
//	{
//		TArray<FB2Item>* EquipitemPtrToModify = const_cast<TArray<FB2Item>*>(&InEquippedItems); // ���� ����� ġƮ�̹Ƿ� �ټҰ� ������ �������..
//		if (EquipitemPtrToModify)
//			EquipitemPtrToModify->Append(gCheatCostumeItem);
//	}
//#endif
//
//	TArray<FPCPartsAssetInfo> DefaultPartsCopy = InDefaultParts; // �Ϻ� ��Ұ� ���߿� ���ŵ� ���� �־ �����ؼ� ����.
//	TArray<FLocalPartMeshMtrlInfo> AllPartAssets; // �⺻��Ʈ�� �������� ������ ����ؼ� ���� ���ҽ� ���� ���� ��. ���⿡ ���� �͵��� ���� �޽� ���� ���� ���� ��.
//	TArray<FPCPartExtraAttachFxSet> AllExtraAttachFx; // �߰��� �̰͵� ������ �� ��.
//
//	TMap< EPCPart, const FB2Item* > ReplaceCostumeItems;
//	bool HaveCombineCostume = false;
//	
//	for (const FB2Item& CurrentItem : InEquippedItems)
//	{
//		if (CurrentItem.CostumeEquipPlace != ECostumeEquipPlace::ECEP_None)
//		{
//			TArray<EPCPart> ReplaceParts;
//			SectionMergeInfo->GetReplaceCostumeEquipPlace(CurrentItem.CostumeEquipPlace, ReplaceParts);
//			//�ڽ�Ƭ �Ѱ��� ���� ���� ��ġ�� Combine �ڽ�Ƭ �� �ִٰ� ����
//			if (ReplaceParts.Num() > 1)
//				HaveCombineCostume = true;
//
//			for (EPCPart PCPart : ReplaceParts)
//				ReplaceCostumeItems.Add(PCPart, &CurrentItem);
//		}
//	}
//
//	check(((int32)EPCPart::EPCPT_Face == 0) && ((int32)EPCPart::EPCPT_Wing + 1 == (int32)(EPCPart::EPCPT_End))); // Enum ��ȣ�� �𸣰� �ٲ�ų� �߰� �׸� ���� ��� ��ġ�� �ʵ���.
//	{
//		B2_SCOPED_TRACK_LOG_L2(TEXT("UB2PCClassInfo::SetupSKCompForParts -- Part Resource Loading"));
//		for (int32 PartIdx = 0; PartIdx < (int32)EPCPart::EPCPT_End; ++PartIdx)
//		{
//			const EPCPart CurrPartEnum = (EPCPart)PartIdx;
//			const FPCPartsAssetInfo* CurrPartDefaultInfo = NULL;
//			for (int32 DPI = 0; DPI < DefaultPartsCopy.Num(); ++DPI)
//			{
//				if (DefaultPartsCopy[DPI].PartEnum == CurrPartEnum)
//				{
//					CurrPartDefaultInfo = &DefaultPartsCopy[DPI];
//					break;
//				}
//			}
//
//			// CurrPartDefaultInfo �� NULL �̰ų� MeshAsset �� NULL �̾ �����Ѵ�.
//			check(!CurrPartDefaultInfo || CurrPartDefaultInfo->PartEnum != EPCPart::EPCPT_End);
//
//			// �Ʒ� ���� Equip �� �� ��츸 ���� �� ����. �ƴ϶�� DefaultPartsCopy ���� ���.
//			USkeletalMesh* EquippedPartMesh = NULL;
//			UMaterialInterface* EquippedPartMaterial = NULL; // �̰� equip �� ���� ���� �� ����. ���� �޽��� ����ϴ� ��� Material �� vary ġ�� ����.
//
//			FLocalPartMeshMtrlInfo ExtraAttachInfo; // �̹� EquippedPart ���� ���� ���� �߰� ��Ʈ�� ���� ��� ���� ��.
//			bool IsCostumeOverride = false;
//
//			// CurrPartEnum �� �ش��ϴ� ���� ��ġ ItemInfo �� ��ϵ� ���� �ִٸ� �װ� �����´�.
//			EItemEquipPlace RelevantEquipPlace = GetItemEquipPlaceForPCPart(CurrPartEnum);
//			ECostumeEquipPlace CurrCostumeEquipPlace = ECostumeEquipPlace::ECEP_None;
//			
//			if (RelevantEquipPlace != EItemEquipPlace::EIEP_End && IteminfoTable)
//			{
//				const FB2Item* CurrEquipInfo = InEquippedItems.FindByPredicate([RelevantEquipPlace](const FB2Item& item)
//				{
//					return RelevantEquipPlace == item.EquipPlace;
//				});
//
//				const FB2Item** Costume = ReplaceCostumeItems.Find(CurrPartEnum);
//				const FB2Item* CurrCostumeInfo = Costume != nullptr ? *Costume : nullptr;
//				IsCostumeOverride = CurrCostumeInfo != nullptr;
//				if(CurrCostumeInfo != nullptr)
//					CurrCostumeEquipPlace = CurrCostumeInfo->CostumeEquipPlace;
//
//				//Mesh, Material 
//				const FB2Item* ViewItem = CurrCostumeInfo != nullptr ? CurrCostumeInfo : CurrEquipInfo;
//
//				if (ViewItem != nullptr)
//				{
//					FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(ViewItem->ItemRefID);
//					AllLoadedItemInfo.Add(RelevantInfoData); // ���� �� �㿡 ��ε� �Ϸ��� ���� �迭�� �־����.
//
//					// ��Ʈ ���ҽ� �ε��� ���� mesh merge �� �ؾ� �ϴ� ��쿡�� ����
//					EquippedPartMesh = (RelevantInfoData && !bPrebuiltMeshProvided) ? RelevantInfoData->GetPCPartMesh(IteminfoTable) : NULL;
//					EquippedPartMaterial = (RelevantInfoData && !bPrebuiltMeshProvided) ? RelevantInfoData->GetPCPartMaterial(IteminfoTable) : NULL;
//
//					//�ڽ�Ƭ�϶� Ȯ���� �Ⱥ��δ�...
//					if (IsCostumeOverride == false)
//					{
//						// ������ ���� ������ ���� �ٸ� ��Ʈ�� ���� �ٰԵǴ� ��Ʈ.
//						EPCPart ExtraAttachPartEnum = GetExtraAttachPartEnum(CurrPartEnum);
//						if (ExtraAttachPartEnum != EPCPart::EPCPT_End && RelevantInfoData)
//						{
//							ExtraAttachInfo.PartEnum = ExtraAttachPartEnum;
//							if (!bPrebuiltMeshProvided) // ��Ʈ ���ҽ� �ε��� ���� mesh merge �� �ؾ� �ϴ� ��쿡�� ����
//							{
//								ExtraAttachInfo.MeshAsset = RelevantInfoData->GetPCPartExtraMesh(IteminfoTable);
//								ExtraAttachInfo.MaterialAsset = RelevantInfoData->GetPCPartExtraMaterial(IteminfoTable);
//							}
//						}
//					}
//
//					// �ڽ�Ƭ ���� ����Ʈ
//					if (IsCostumeOverride && CurrCostumeInfo != nullptr)
//					{
//						// Ư�� ������ ������ ���⿡ �ٴ� �߰� ��ƼŬ ȿ��. �� ������.. ����� ���� merge �ϴ� ��찡 �ƴϴ��� �ε��� �Ѵ�.
//						if (CurrCostumeInfo->ItemClass == EItemClass::EIC_CostumeWeapon ||
//							// Ȳ���ϰԵ� ������ ������ �����͸� ���� ���� ��Ȳ������ ItemClass �� ������ �ȵǾ� �ִ�. ���� ó���� 6������ ����Ʈ�� �ȳ����� ���� ����.
//							// InventoryType �� RefID üũ�� ��ü �����ϵ���.
//							(CurrCostumeInfo->InventoryType == EItemInvenType::EIIVT_CostumeWeapon && FItemRefIDHelper::ExtractItemClassFromRefID(CurrCostumeInfo->ItemRefID) == EItemClass::EIC_CostumeWeapon)
//							)
//						{
//
//							AllLoadedItemInfo.Add(RelevantInfoData); // ���� �� �㿡 ��ε� �Ϸ��� ���� �迭�� �־����.
//
//							UParticleSystem* SixStarWeaponEffectPS_L = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//							UParticleSystem* SixStarWeaponEffectPS_R = RelevantInfoData->GetSixStarWeaponEffect_R(IteminfoTable);
//							if (!SixStarWeaponEffectPS_R) {
//								// _R ������ �������� ���� �� �⺻ ������ �ɷ� ����, ���� ��� ����. �¿� ������ ���¸� �̷��� ��.
//								SixStarWeaponEffectPS_R = SixStarWeaponEffectPS_L;
//							}
//							if (SixStarWeaponEffectPS_L)
//							{
//								// �� �¾������� �⺻���� ������ ������ �����ϴµ�.. Ȥ �߰������� �𸣰ڴ�.
//								PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffectPS_L, SIXSTAR_WEAPON_EFFECT_SOCKET_L_01);
//							}
//							if (SixStarWeaponEffectPS_R)
//							{
//								PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffectPS_R, SIXSTAR_WEAPON_EFFECT_SOCKET_R_01);
//							}
//
//							UParticleSystem* SixStarWeaponEffect2PS_L = RelevantInfoData->GetSixStarWeaponEffect2(IteminfoTable);
//							UParticleSystem* SixStarWeaponEffect2PS_R = RelevantInfoData->GetSixStarWeaponEffect2_R(IteminfoTable);
//							if (!SixStarWeaponEffect2PS_R) {
//								// _R ������ �������� ���� �� �⺻ ������ �ɷ� ����, ���� ��� ����. �¿� ������ ���¸� �̷��� ��.
//								SixStarWeaponEffect2PS_R = SixStarWeaponEffect2PS_L;
//							}
//
//							if (SixStarWeaponEffect2PS_L)
//							{
//								PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffect2PS_L, SIXSTAR_WEAPON_EFFECT_SOCKET_L_02);
//							}
//							if (SixStarWeaponEffect2PS_R)
//							{
//								PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffect2PS_R, SIXSTAR_WEAPON_EFFECT_SOCKET_R_02);
//							}
//						}
//					}
//				}
//
//				//���� ������ ������ �������� ����..
//				if (CurrEquipInfo != nullptr)
//				{
//					// Ư�� ������ ������ ���⿡ �ٴ� �߰� ��ƼŬ ȿ��. �� ������.. ����� ���� merge �ϴ� ��찡 �ƴϴ��� �ε��� �Ѵ�.
//					if (CurrEquipInfo->ItemClass == EItemClass::EIC_Weapon ||
//						// Ȳ���ϰԵ� ������ ������ �����͸� ���� ���� ��Ȳ������ ItemClass �� ������ �ȵǾ� �ִ�. ���� ó���� 6������ ����Ʈ�� �ȳ����� ���� ����.
//						// InventoryType �� RefID üũ�� ��ü �����ϵ���.
//						(CurrEquipInfo->InventoryType == EItemInvenType::EIIVT_Weapon && FItemRefIDHelper::ExtractItemClassFromRefID(CurrEquipInfo->ItemRefID) == EItemClass::EIC_Weapon)
//						)
//					{
//						if (CurrEquipInfo->StarGrade >= FB2Item::MinimumSurpassStarGrade)
//						{ // 6�� ����
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // ���� �� �㿡 ��ε� �Ϸ��� ���� �迭�� �־����.
//
//							if (RelevantInfoData && !IsCostumeOverride)
//							{
//								{
//									UParticleSystem* SixStarWeaponEffectPS_L = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//									UParticleSystem* SixStarWeaponEffectPS_R = RelevantInfoData->GetSixStarWeaponEffect_R(IteminfoTable);
//									if (!SixStarWeaponEffectPS_R) {
//										// _R ������ �������� ���� �� �⺻ ������ �ɷ� ����, ���� ��� ����. �¿� ������ ���¸� �̷��� ��.
//										SixStarWeaponEffectPS_R = SixStarWeaponEffectPS_L;
//									}
//									if (SixStarWeaponEffectPS_L)
//									{
//										// �� �¾������� �⺻���� ������ ������ �����ϴµ�.. Ȥ �߰������� �𸣰ڴ�.
//										PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffectPS_L, SIXSTAR_WEAPON_EFFECT_SOCKET_L_01);
//									}
//									if (SixStarWeaponEffectPS_R)
//									{
//										PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffectPS_R, SIXSTAR_WEAPON_EFFECT_SOCKET_R_01);
//									}
//								}
//								{
//									UParticleSystem* SixStarWeaponEffect2PS_L = RelevantInfoData->GetSixStarWeaponEffect2(IteminfoTable);
//									UParticleSystem* SixStarWeaponEffect2PS_R = RelevantInfoData->GetSixStarWeaponEffect2_R(IteminfoTable);
//									if (!SixStarWeaponEffect2PS_R) {
//										// _R ������ �������� ���� �� �⺻ ������ �ɷ� ����, ���� ��� ����. �¿� ������ ���¸� �̷��� ��.
//										SixStarWeaponEffect2PS_R = SixStarWeaponEffect2PS_L;
//									}
//
//									if (SixStarWeaponEffect2PS_L)
//									{
//										PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffect2PS_L, SIXSTAR_WEAPON_EFFECT_SOCKET_L_02);
//									}
//									if (SixStarWeaponEffect2PS_R)
//									{
//										PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffect2PS_R, SIXSTAR_WEAPON_EFFECT_SOCKET_R_02);
//									}
//								}
//							}
//						}
//
//						//�ڽ�Ƭ�� ������� N�� ����Ʈ�� ������ ���⿡ ���� �߰����ش�.
//						// ��ȭ�� ���� ���� ��ƼŬ ����Ʈ ǥ��
//						if (ClientDataStore.HasEnhanceEffectAuraColor(CurrEquipInfo->EnhanceLevel))
//						{
//							if (SomeInfo)
//							{
//								UParticleSystem* AttachPS = nullptr;
//
//								if (CurrEquipInfo->EnhanceLevel <= SomeInfo->GetUseWeaponExtraEffect_Level40())
//									AttachPS = SomeInfo->GetWeaponExtraEffect_Level30();
//								else if(CurrEquipInfo->EnhanceLevel > SomeInfo->GetUseWeaponExtraEffect_Level40() && CurrEquipInfo->EnhanceLevel <= SomeInfo->GetUseWeaponExtraEffect_Level50())
//								{
//									//ĳ���͸��� ����Ʈ �ٸ�
//									if(InPCClass == EPCClass::EPC_Gladiator)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level40_Gl();
//									else if (InPCClass == EPCClass::EPC_Assassin)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level40_As();
//									else if (InPCClass == EPCClass::EPC_Fighter)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level40_Fi();
//									else if (InPCClass == EPCClass::EPC_Wizard)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level40_Wz();
//								}
//								else
//								{
//									//ĳ���͸��� ����Ʈ �ٸ�
//									if (InPCClass == EPCClass::EPC_Gladiator)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level50_Gl();
//									else if (InPCClass == EPCClass::EPC_Assassin)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level50_As();
//									else if (InPCClass == EPCClass::EPC_Fighter)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level50_Fi();
//									else if (InPCClass == EPCClass::EPC_Wizard)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level50_Wz();
//								}
//							
//
//								if (AttachPS)
//								{
//									FLinearColor EffectColor = ClientDataStore.GetEnhanceEffectAuraColor(CurrEquipInfo->EnhanceLevel);
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, AttachPS, ENHANCED_WEAPON_EFFECT_SOCKET_L, EffectColor);
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, AttachPS, ENHANCED_WEAPON_EFFECT_SOCKET_R, EffectColor);
//								}
//							}
//						}
//					}
//#ifdef EQUIPMENTS_EFFECT 
//					else if (CurrEquipInfo->ItemClass == EItemClass::EIC_BodyArmor
//						|| CurrEquipInfo->ItemClass == EItemClass::EIC_Helmet)
//					{
//						if (CurrEquipInfo->ItemRefID % 10 == 6 ||	// õ��
//							CurrEquipInfo->ItemRefID % 10 == 7)		// ����
//						{
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // ���� �� �㿡 ��ε� �Ϸ��� ���� �迭�� �־����.
//
//							if (RelevantInfoData && !IsCostumeOverride)
//							{
//								UParticleSystem* BodyOrHelmetEffectPS = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//
//								if (BodyOrHelmetEffectPS)
//								{
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, BodyOrHelmetEffectPS,
//										CurrEquipInfo->ItemClass == EItemClass::EIC_BodyArmor ?
//										BODYAMMOR_EFFECT_SOCKET : HELMET_EFFECT_SOCKET);
//								}
//							}
//						}
//					}
//					else if (CurrEquipInfo->ItemClass == EItemClass::EIC_Glove
//						|| CurrEquipInfo->ItemClass == EItemClass::EIC_Shoes)
//					{
//						if (CurrEquipInfo->ItemRefID % 10 == 6 || // õ��
//							CurrEquipInfo->ItemRefID % 10 == 7)	  // ����
//						{
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // ���� �� �㿡 ��ε� �Ϸ��� ���� �迭�� �־����.
//
//							if (RelevantInfoData && !IsCostumeOverride)
//							{
//
//								UParticleSystem* GloveOrShoesEffectPS_L = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//								UParticleSystem* GloveOrShoesEffectPS_R = RelevantInfoData->GetSixStarWeaponEffect_R(IteminfoTable);
//								if (!GloveOrShoesEffectPS_R) {
//									GloveOrShoesEffectPS_R = GloveOrShoesEffectPS_L;
//								}
//								if (GloveOrShoesEffectPS_L)
//								{
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, GloveOrShoesEffectPS_L,
//
//										CurrEquipInfo->ItemClass == EItemClass::EIC_Glove ?
//										GLOVE_EFFECT_SOCKET_L_01 : SHOSE_EFFECT_SOCKET_L_01);
//								}
//								if (GloveOrShoesEffectPS_R)
//								{
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, GloveOrShoesEffectPS_R,
//
//										CurrEquipInfo->ItemClass == EItemClass::EIC_Glove ?
//										GLOVE_EFFECT_SOCKET_R_01 : SHOSE_EFFECT_SOCKET_R_01);
//								}
//							}
//						}
//					}
//#endif // EQUIPMENTS_EFFECT 
//				}
//			}
//			else if (CurrPartEnum == EPCPart::EPCPT_Wing && InWingData && InWingData->bShouldBeVisible)
//			{// ������ �ٸ� ���ʹ� ������
//				LoadedWingInfo = WingInfoTable ? WingInfoTable->GetInfoData(*InWingData) : NULL;
//				if (LoadedWingInfo)
//				{
//					if (!bPrebuiltMeshProvided) // ��Ʈ ���ҽ� �ε��� ���� mesh merge �� �ؾ� �ϴ� ��쿡�� ����
//					{
//						EquippedPartMesh = LoadedWingInfo->GetWingMesh();
//						EquippedPartMaterial = LoadedWingInfo->GetMtrlOverride();
//
//#if !UE_BUILD_SHIPPING
//						// ���� ��Ƽ���� �� Ư�� �ؽ��� �Ķ���� ���̹� ���� üũ�ϴµ� �̰� 6������ �ִ�..
//						if (InWingData->EvolutionGrade >= 6)
//						{
//							UMaterialInterface* WingMtrlCheck = EquippedPartMaterial;
//							if (!WingMtrlCheck && EquippedPartMesh && EquippedPartMesh->Materials.Num() > 0)
//							{// ��� �� 6�� �̻� ���� ���� Ư�� ��Ƽ������ �⺻ ������ �ƴ� Override �� ����� ���� �ʰ���..
//								WingMtrlCheck = EquippedPartMesh->Materials[0].MaterialInterface;
//							}
//							if (WingMtrlCheck)
//							{
//								checkSlow(HasWingAuraEmissiveTexture(EquippedPartMaterial));
//							}
//						}
//#endif
//#if !UE_BUILD_SHIPPING // �����ϰ� ������ �� ��������� ��찡 ��Ȥ �ִ� �� ���� �α׸� �ѷ�����.
//						UE_LOG(LogBladeII, Log, TEXT("WingMesh %s has been added as one of equip part for PCClass %d"), EquippedPartMesh ? *EquippedPartMesh->GetName() : TEXT("InvalidMesh"), PCClassToInt(InPCClass));
//#endif
//					}
//					// ��� �Ƹ� 7�� �̻󿡼��� ������ �� ��. ������ �Ǿ��ִ� ��츸
//					for (int32 AEI = 0; AEI < LoadedWingInfo->AttachEffectInfoArray.Num(); ++AEI)
//					{
//						FCHSKCompAttachParticleSystemInfo& ThisAttachEffectInfo = LoadedWingInfo->AttachEffectInfoArray[AEI];
//						UParticleSystem* WingPS = ThisAttachEffectInfo.GetParticleSystemAsset();
//						if (WingPS)
//						{
//							FPCPartExtraAttachFxSet WingAttachPSInfo;
//							WingAttachPSInfo = ThisAttachEffectInfo; // ���� ��ġ�鼭 WingAttachPSInfo.LoadedAttachPS �� WingPS �� ���õ� ��.
//							AllExtraAttachFx.Add(WingAttachPSInfo);
//						}
//					}
//				}
//			}
//
//			// �̹� ��Ʈ������ ���������� ����� ���ҽ� ����. bPrebuiltMeshProvided �� ��� �����δ� ��� null �� ������ ��.
//			FLocalPartMeshMtrlInfo ThisPartAssetInfo;
//			ThisPartAssetInfo.PartEnum = CurrPartEnum;
//			ThisPartAssetInfo.MeshAsset = EquippedPartMesh ? EquippedPartMesh : (CurrPartDefaultInfo ? CurrPartDefaultInfo->MeshAsset : NULL);
//			ThisPartAssetInfo.MaterialAsset = EquippedPartMaterial; // Material �� ����Ʈ�� ���ǵ� ���� ����.
//			ThisPartAssetInfo.CostumeEquipPlace = CurrCostumeEquipPlace;
//
//			if (ThisPartAssetInfo.MeshAsset) { // �⺻ ����Ʈ���� ���� ���� ����Ʈ���� ������ NULL �� �� �ִ�. �׷� ��� ���� ���� ����Ʈ�� ���Խ�Ű�� �ʴ´�.
//				AllPartAssets.Add(ThisPartAssetInfo);
//			}
//			// ���� ���⿡ extra attach �� �߰��� ���� �ִ�.
//			if (ExtraAttachInfo.MeshAsset)
//			{
//				// AllPartAssets �� �ֱ� ����, ���� PartEnum �� �⺻ ����Ʈ�� �ִٸ� ���������� �װ� ������.
//				// DefaultParts ������ ����, �̹� AllPartAssets �� �� ���� ���� �����Ƿ� �ű⼭�� ����.
//				for (int32 DPI = 0; DPI < DefaultPartsCopy.Num(); ++DPI)
//				{
//					if (DefaultPartsCopy[DPI].PartEnum == ExtraAttachInfo.PartEnum)
//					{
//						DefaultPartsCopy.RemoveAt(DPI);
//						break;
//					}
//				}
//				for (int32 API = 0; API < AllPartAssets.Num(); ++API)
//				{
//					if (AllPartAssets[API].PartEnum == ExtraAttachInfo.PartEnum)
//					{
//						AllPartAssets.RemoveAt(API);
//						break;
//					}
//				}
//				AllPartAssets.Add(ExtraAttachInfo);
//			}
//		}
//	}
//
//	// ���� �����ϸ� �ȵǴ� ��Ʈ���� �ɷ���
//	TArray<int32> ToRemovePartIndices;
//	for (int32 PIA = 0; PIA < AllPartAssets.Num(); ++PIA)
//	{
//		const EPCPart PartToAdd = AllPartAssets[PIA].PartEnum;
//
//		for (int32 PIB = 0; PIB < AllPartAssets.Num(); ++PIB)
//		{ // �� ������ �־ PIB �� PIA+1 ���� ���������� ���ϰ� ���� ��츸 �ɷ�����.
//			if (PIA == PIB){
//				continue;
//			}
//			const EPCPart PartToRemoveTest = AllPartAssets[PIB].PartEnum;
//			if (ShouldRemoveOtherPart(InPCClass, PartToAdd, PartToRemoveTest)) // ���� �ϳ�.. ShouldRemove ���谡 ���̻罽ó�� �������� ���� �Ǿ �ȵǰڴ�..
//			{
//				ToRemovePartIndices.AddUnique(PIB);
//			}
//		}
//	}
//	if (ToRemovePartIndices.Num() > 0)
//	{
//		ToRemovePartIndices.Sort([](const int32& A, const int32& B) { return A > B; });
//		for (int32 ThisRemoveIdx : ToRemovePartIndices) // ���� �ε������� �ؼ� ����
//		{
//			AllPartAssets.RemoveAt(ThisRemoveIdx);
//		}
//	}
//
//	//�ڽ�Ƭ���� ���� ���� ���� �ɷ���
//	CombineCostumePart(SectionMergeInfo, AllPartAssets);
//
//	if (AllPartAssets.Num() == 0){ // �⺻��Ʈ�� �����������̵� �ƹ� ���� ����. �״�� �����ϸ� MeshMerge ���� ���� �α�.
//		return false;
//	}
//
//	// ���� MeshMerge �� �Ѿ�� �� material ���� ID �� Ȯ���� �ϱ� ���� ����Ʈ�� PartEnum ���� ����. (GetPCCompositeMtrlID)
//	struct FLocalPartMeshMtrlInfoSorter
//	{
//		bool operator()(const FLocalPartMeshMtrlInfo& A, const FLocalPartMeshMtrlInfo& B) const
//		{
//			return (int32)A.PartEnum < (int32)B.PartEnum;
//		}
//	};
//	AllPartAssets.Sort(FLocalPartMeshMtrlInfoSorter());
//
//#if WITH_EDITOR && !PLATFORM_MAC
//	// ��Ʈ �� ���� material ���ǿ� ���� ���. MeshMerge ���� �������� ��������� ���������� ���������� �ΰ��ӿ��� Material ID �� �����ϱ� ���ؼ��� �����.
//	int32 MultiSectionFoundNum = 0;
//	FString MultiSectionPartWarnMsg = TEXT("���� ��Ʈ�� ���� ������ �� �ý��ۿ��� ������ �ʽ��ϴ�. ���� ��Ʈ ����(��)�� Ȯ���� �ּ���.\n\n");
//	for (const FLocalPartMeshMtrlInfo& ThisAssetInfo : AllPartAssets)
//	{
//		if (ThisAssetInfo.MeshAsset)
//		{
//			FSkeletalMeshResource* ThisMeshResource = ThisAssetInfo.MeshAsset->GetImportedResource();
//			if (ThisMeshResource && ThisMeshResource->LODModels.Num() > 0 && ThisMeshResource->LODModels[0].Sections.Num() > 1)
//			{
//				++MultiSectionFoundNum;
//				MultiSectionPartWarnMsg += FString::Printf(TEXT("%d. %s\n"), MultiSectionFoundNum, *ThisAssetInfo.MeshAsset->GetName());
//			}
//		}
//	}
//	if (MultiSectionFoundNum > 0)
//	{
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(MultiSectionPartWarnMsg));
//	}
//#endif
//
//	bool bResult = false;
//
//	USkeletalMesh* CompositeMesh = InOutPrebuiltCompositeMesh ? InOutPrebuiltCompositeMesh : NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Transient);
//			
//	// �̰� Section ������ �ϴ� ��� ���� ��Ƽ������ �Ķ���͸� �����ϱ� ���� ��Ƶδ� ��
//	TMap<EPCPart, UMaterialInterface*> NonMergedMaterialsBackup;
//
//	if (!bPrebuiltMeshProvided)
//	{
//		// ���� FSkeletalMeshMerge ����� ����ϱ� ���� ����. ���� mesh merge �� ���� �ʴ� ��Ȳ���� �ʿ����. (bPrebuiltMeshProvided)
//
//		TArray<USkeletalMesh*> SourcePartMeshes;
//		TArray<FSkelMeshMergeSectionMapping> ForceSectionMapping;
//		TArray<FSkelMeshMergeMtrlOverrideMapping> SrcMtrlOverrides;
//		
//		// FSkeletalMeshMerge ���� ����ϱ� ���� AllPartAssets �� ��Ƴ��� �������� �ѹ� �� �籸���Ѵ�.
//		int32 ForceSectionID = 0;
//		for (int32 PAI = 0; PAI < AllPartAssets.Num(); ++PAI)
//		{
//			const FLocalPartMeshMtrlInfo& ThisAssetInfo = AllPartAssets[PAI];
//			check(ThisAssetInfo.MeshAsset); // �ٷ� ����� ���� �̰� NULL �� ���� ����� ��. ���� �Ʒ��� material overriding ���� ������ ����.
//			SourcePartMeshes.Add(ThisAssetInfo.MeshAsset);
//
//			if (bInMergeSections)
//			{ // ���� ���� �ÿ��� ForceSectionID �� ������ material �̶� �Կ��ָ� MeshMerge �ʿ��� �˾Ƽ� ������ ����.
//				// �ƴ� ���� ForceSectionID �� ���������� ����.
//				ForceSectionID = SectionMergeInfo->GetMergedSectionID(HaveCombineCostume, ThisAssetInfo.PartEnum);
//
//				// �׸��� ���� ������ �ϳ��� material �ȿ� �����鼭 �ʿ������� UV ��ǥ offset ������ ����.
//				TArray<FTransform> ThisPartUVTranform;
//				SectionMergeUVTransform.UVTransformsPerMesh.Add(ThisPartUVTranform);
//				check(SectionMergeUVTransform.UVTransformsPerMesh.Num() == PAI + 1);
//
//				// Merge �� ���� material parameter ���ÿ� �����ϱ� ���� ���� material ĳ��. ���� null �� ���� ������ ���� �߸��� �Ű���.
//				NonMergedMaterialsBackup.Add(ThisAssetInfo.PartEnum,
//					ThisAssetInfo.MaterialAsset ? ThisAssetInfo.MaterialAsset :
//					(ThisAssetInfo.MeshAsset->Materials.Num() > 0 ? ThisAssetInfo.MeshAsset->Materials[0].MaterialInterface : nullptr));
//			}
//
//			FSkeletalMeshResource* ThisMeshResource = ThisAssetInfo.MeshAsset->GetImportedResource();
//			if (ThisMeshResource && ThisMeshResource->LODModels.Num() > 0)
//			{
//				FSkelMeshMergeSectionMapping NewSectionMap;
//				FSkelMeshMergeMtrlOverrideMapping NewMtrlMap; // ���� ���߿� section ID ������ �ʿ� ���� �� �������� �ٷ� material override ��..
//				for (int32 SI = 0; SI < ThisMeshResource->LODModels[0].Sections.Num(); ++SI) // ForceSectionMapping �� LODModel 0 �� �������� �ϸ� ��.
//				{
//					NewSectionMap.SectionIDs.Add(ForceSectionID);
//
//					// ������ ��ġ�� �Ǹ� ��Ʈ�� ������ ���ļ� ����ϱ� ���� �غ�� ������ material �� ����Ѵ�. ���� �����̸� ������ material �� ����ؾ� ��.
//					// MergeSections ������ ������ null �� �� �ִ�. �׷� ���� ���� material �� ���� �ǵ� ������ ��������� ���� ������ �����ϴ� ��Ʈ�� ������ �׷��� �� ��
//					UMaterialInterface* MergedSectionMaterial = bInMergeSections ? SectionMergeInfo->GetMergedSectionMaterial(HaveCombineCostume, ThisAssetInfo.PartEnum) : nullptr;
//
//					if (bInMergeSections && SectionMergeUVTransform.UVTransformsPerMesh.IsValidIndex(PAI))
//					{
//						TArray<FTransform>& ThisPartUVTransformInfo = SectionMergeUVTransform.UVTransformsPerMesh[PAI];
//
//						FVector2D PartUVOffset = SectionMergeInfo->GetPartUVOffset(HaveCombineCostume, ThisAssetInfo.PartEnum);
//						FTransform PartUVTransform(
//							FRotator(0.0f, 0.0f, 0.0f),
//							FVector(PartUVOffset.X, PartUVOffset.Y, 0.0f),
//							FVector(0.5f, 0.5f, 1.0f) // �������� ���� ���� �� �ϰ� �⺻ �������� ���⼭ �ִ´�.
//						);
//						// �� �迭�� �� ��Ұ� Texture ��ǥ �ε����� �ش��ϴ� ���ε� ���⿡ �־��� ���� �̻��� �ؽ��� ��ǥ�� ���� �Ǹ� ���� �̻��������� ���� merge �Ǵ� ��带 �� �Ŀ��� �˰� ���ٸ�.. ��..
//						ThisPartUVTransformInfo.Add(PartUVTransform);
//						ThisPartUVTransformInfo.Add(PartUVTransform); // �׷��� Ȥ �𸣴� �ϳ� �� �־�����. �� �̻� �ɸ� ���� ������..
//					}
//
//					NewMtrlMap.SectionMatOverrides.Add(
//						(bInMergeSections && MergedSectionMaterial) ? MergedSectionMaterial :
//						ThisAssetInfo.MaterialAsset // ��Ʈ�� �⺻ material override. �̰� NULL �� ���� �ִ�.
//					);
//
//					if (!bInMergeSections) // ���� ��ġ�� ���� �ƴ� ����� �������� Section ID.
//					{
//						++ForceSectionID;
//					}
//				}
//
//				ForceSectionMapping.Add(NewSectionMap);
//				SrcMtrlOverrides.Add(NewMtrlMap);
//			}
//		}
//		check(!bInMergeSections || (NonMergedMaterialsBackup.Num() == AllPartAssets.Num()));
//		
//
//#if WITH_EDITOR
//		if (GIsEditor)
//		{/** PC Part �� mesh merge �� �⺻ skeleton �� �ҽ����� skeleton ���� ref pose �� �󸶳� �ٸ��� üũ�ؼ� ��� ���. */
//			CheckAndWarnForDifferentRefPose(InBaseMesh, SourcePartMeshes);
//		}
//#endif
//		{ 
//			B2_SCOPED_TRACK_LOG_L2(TEXT("UB2PCClassInfo::SetupSKCompForParts -- Actual Merge"));
//
//#if !UE_BUILD_SHIPPING
//			extern bool GCheat_PCPartMergeRemoveExtraBoneInfluence;
//#endif
//
//			//create an instance of the FSkeletalMeshMerge utility
//			// EPCPTCM_SeparateMergedComponent �� ��� InBaseMesh �� ���̷��� �� ������� �ʴµ� �� ��� ���������� BaseMesh ������Ʈ�� �ִϸ��̼��� ����� ���̱� ����.
//			FSkeletalMeshMerge MeshMergeUtil(CompositeMesh, SourcePartMeshes, ForceSectionMapping, 0, EMeshBufferAccess::Default
//				, bInMergeSections ? &SectionMergeUVTransform : nullptr
//				, InBaseMesh
//				, &SrcMtrlOverrides
//#if !UE_BUILD_SHIPPING
//				, GCheat_PCPartMergeRemoveExtraBoneInfluence
//#else
//				, false
//#endif
//				);
//
//			if (!MeshMergeUtil.DoMerge()) // ���� mesh merge ����. ���ݱ����� ���� �¾��̾��� �� ��
//			{
//				if (CompositeMesh){
//					CompositeMesh->ConditionalBeginDestroy();
//					CompositeMesh = NULL;
//				}
//			}
//		}
//	}
//
//	if (CompositeMesh) // ĳ�̵� ���� �־�����, Merge �� �����ߵ�
//	{
//		B2_SCOPED_TRACK_LOG_L2(TEXT("UB2PCClassInfo::SetupSKCompForParts -- Post Merge Setup"));
//
//		CompositeMesh->PhysicsAsset = InBaseMesh->PhysicsAsset;
//		InOutPrebuiltCompositeMesh = CompositeMesh; // ���� ������ �Ǿ��� ����� InOutPrebuiltCompositeMesh �� ���� ����� �����ε� ���޵� ��.
//				
//		// djsong UE4 MERGE 4.14. 4.14 ���ĺ��� �߻��� ���ε� merge �ϱ� ���� ���� �޽� ���¿��� MID �� ������� ������Ʈ�� override �� ���·� ���⿡ ���� ��찡 ����.
//		// �̷��� Ŭ��� ���� �� �ִ���, �ƴϸ� CompositeMesh->Materials ������ �Ȱ��� override �� �ִ���.
//		if (!bBuildAndGetResultMeshOnly)
//		{
//			for (int32 MI = 0; MI < SKCompToSetup->GetNumMaterials(); ++MI)
//			{
//				SKCompToSetup->SetMaterial(MI, nullptr);
//			}
//			SKCompToSetup->B2ScalarMatParameter.Empty(); // ������ Material Ŭ���� �ϴ� �Ŷ� ����� ������ �̰͵� Ŭ���� ��.
//		}
//
//		USkeletalMesh* OldSKMeshBackup = SKCompToSetup->SkeletalMesh;
//		if (!bBuildAndGetResultMeshOnly) // ������Ʈ�� �޽� ������ ���� �ʰ� ����� ������ ����.
//		{
//			SKCompToSetup->SetSkeletalMesh(CompositeMesh);
//		}
//
//		// �׸��� ������ ������ ���� ��Ƽ������ �ٸ��� ����ϹǷ� �Ķ���� �¾��� �ؾ� �Ѵ�.
//		if (bInMergeSections && SectionMergeInfo && !bPrebuiltMeshProvided) // ���� mesh merge �� �� ��츸
//		{
//			SectionMergeInfo->SetupMergedSectionMaterialParams(HaveCombineCostume, CompositeMesh, NonMergedMaterialsBackup);
//		}
//
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		// Ȥ�� �𸣰ھ ������ �����ߴ� �� Ȯ���� ����. (�ٸ� �Ŷ��)
//		if (OldSKMeshBackup && OldSKMeshBackup != InBaseMesh && OldSKMeshBackup != CompositeMesh 
//			&& OldSKMeshBackup->HasAnyFlags(RF_Transient) // ���� �� RF_Transient �� �ش�.
//			&& !(MeshCacheManager && MeshCacheManager->IsOneOfCachedCompositeMesh(OldSKMeshBackup))) // �������� �����ؼ� ĳ�̵Ǿ� �ִ� �Ŷ�� ĳ�ð� ���� ������ �ȵǾ��� �״� ���� ������� ���� ���� �ִµ� �׷��� �κ񿡼� �׷��� ���� ��쵵 �� �ȴ�.
//		{ 
//			// ���⼭ ������ �޽� ���ҽ��� ���� ���Ͱ� ���ÿ� �������� ����ϸ� �̷��� ��������� Destroy �ϴ� �� ������ ����ų �� �ִ�.
//			// �׷� ��찡 �߻��Ѵٸ� �޽� ���ҽ� ���� ���� �ÿ� �װ� ����� ���͵鿡 ��� ������Ʈ�� �� �ִ� �� �߰����� ó�� ���� �ʿ��ϴ�.
//			
//			// ���� ������ �ȴٸ� ���⼭ ConditionalBeginDestroy ������ �ڵ�� �����ϴ� �� ���� ����. ������ ������ �ʴ� mesh ������Ʈ��� �𸮾� GC �ý��ۿ� ���� �˾Ƽ� ���ŵ� ����.
//			//OldSKMeshBackup->ConditionalBeginDestroy(); �ƹ����� �״� ���� ������ ����.. ���� �̷� �ʿ� ���� �� ����..
//		}
//		
//		// �κ� SkeletalMeshActor �� ���� ���̶�� �̰� NULL �̾��.
//		if (AnimBPForInGameChar && !bBuildAndGetResultMeshOnly)
//		{
//			SKCompToSetup->SetAnimInstanceClass(AnimBPForInGameChar);
//		}
//
//		bResult = true;
//	}
//
//	if (!bBuildAndGetResultMeshOnly)
//	{
//		// ���� �߰� ������Ʈ���� ���� �ǵ� ���� ���̱� ���� ������ ���� �� �ִٸ� ����. �̰͵� SkeletalMesh �� �Ź� ���� �����ϴ� �κ񿡼��� ����� ��..
//		DestroyExtraAttachPartComponents(SKCompOwner, SKCompToSetup);
//
//		for (FPCPartExtraAttachFxSet ExtraAttachFx : AllExtraAttachFx)
//		{
//			UParticleSystemComponent* AttachFx = ExtraAttachFx.CreatePSCHelper(SKCompOwner, SKCompToSetup);
//
//			if (AttachFx)
//			{
//				// Note : �߰� ������Ʈ�� ������ ����������Ѵٸ� MtrlParamName_Color(TEXT("ColorVariControl")) ����Ʈ�� �±׷� ó���Ұ�
//				//		: SetColorParameter�� ��������ʰ� SetVectorParameter�� ��������Ѵ�.  SetColorParameter�� ��� RGB�� (255,255,255,0)�����ۿ� �������� ���ϹǷ� �� ū���� ���� ��� FLinearColor�� FVector�� ��ȯ�ؼ� �־��־����.
//				if (ExtraAttachFx.ChangeColor != FLinearColor::Transparent)
//					AttachFx->SetVectorParameter(MtrlParamName_Color, FVector(ExtraAttachFx.ChangeColor));
//			}
//		}
//	}
//
//	// MeshMerge �� �ϰ� �Ǹ� ���� ��Ʈ�ʹ� ������ ���ҽ��̹Ƿ� ���� ��Ʈ�� ���� �� �ִ�. 
//	// ��, ���� ĳ���� ���� ��Ʈ�� ��� ���� ��Ȳ���� �� �ʿ��� �� �־ ������ �������� �ʰ�.. ����� �Ѿ���� ���ڿ� ����. ��ü�� �Ͻ��� merge �� ForceUnload �� �Ѿ�� ��.
//	if (bForceUnloadPartResourceAfterMerge)
//	{
//		for (FSingleItemInfoData* ThisLoadedItemInfo : AllLoadedItemInfo)
//		{
//			if (ThisLoadedItemInfo) {
//				// ��, RootSet ���� ������ �� ���⼭�� �Ѿ���� �Ѵ�. (bRemoveFromRoot false)
//				// ���� ���� ĳ���Ͱ� ������ ��Ʈ�� �� �����Ƿ�.
//				ThisLoadedItemInfo->UnloadOnlyEquipPart(IteminfoTable, false);
//			}
//		}
//	}
//
//	/*if (LoadedWingInfo) { �굵 ������ Unload �� �ҷ��ִ� ������ ����...
//	LoadedWingInfo->UnloadTAssets(WingInfoTable);
//	}*/

	//return bResult;
return true;
}

void UB2PCClassInfo::DestroyExtraAttachPartComponents(AActor* SKCompOwner, USkeletalMeshComponent* SKCompToSetup)
{	
	//if (SKCompOwner && SKCompToSetup)
	//{
	//	// �ϴ� ����� FPCPartExtraAttachFxSet �� ���� ParticleSystemComponent �� ��. �߰��Ǹ� ����ϰ� Ȯ��
	//	TArray<UActorComponent*> AllPSC = SKCompOwner->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//	for (int32 PSCI = 0; PSCI < AllPSC.Num(); ++PSCI)
	//	{
	//		UParticleSystemComponent* ThisPSC = Cast<UParticleSystemComponent>(AllPSC[PSCI]);
	//		// ���ҽ� ���ø��̶� Attach �� ���ϵ� üũ�� �� �ִµ� ���� �׷� �ʿ���� �̸��� �ſ� ����ũ�ϰ� �ִ� �ɷ�..
	//		if (ThisPSC	&& ThisPSC->GetAttachParent() == SKCompToSetup && FPCPartExtraAttachFxSet::IsPossibleCreatedCompName(ThisPSC->GetName()))
	//		{
	//			ThisPSC->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	//			ThisPSC->ConditionalBeginDestroy();
	//		}
	//	}
	//}
}

void UB2PCClassInfo::SyncLoadInGameOnlyAssets()
{
	// ���⼭ �ε����� �ʴ� ��Ÿ �Ϲ� hard ptr ���� PCClassInfoBox ���� �̰� �ε��� �� ���� �⺻���� �ε��ȴ�.
	// �ΰ���(����)������ ����ϴ� �ֵ�(�� ������ �ֵ�)�� �̷� ������ ���� �����ؼ� �κ񿡼��� �޸𸮸� ����.

	//GetSomeLazyLoadedClassAssetCommon<UB2InGameOnlyPCClassInfo>(InfoLoadManager, InGameOnlyAssetPtr, &LoadedInGameOnlyAssetPtr, TEXT("PCClassInfo"));

	ConditionalSetLoadedInGameOnlyInfoRootSet();
}

void UB2PCClassInfo::UnloadInGameOnlyAssets()
{
	//// ManagedPCClassUnload �� ���� RootSet �� �� �ִ�.
	//if (InGameOnlyAssetPtr.IsValid()){
	//	InGameOnlyAssetPtr->RemoveFromRoot();
	//}
	//if (LoadedInGameOnlyAssetPtr) {
	//	LoadedInGameOnlyAssetPtr->RemoveFromRoot();
	//}

	//UnloadSomeLazyLoadedClassAssetCommon<UB2InGameOnlyPCClassInfo>(InfoLoadManager, InGameOnlyAssetPtr, &LoadedInGameOnlyAssetPtr);

	//if (InGameOnlyAssetAsyncLoadHandle.IsValid()) {
	//	InGameOnlyAssetAsyncLoadHandle->ReleaseHandle();
	//}
}

UB2PCClassInfo::UB2PCClassInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bInGameOnlyAssetAsyncLoadRequestPending = false;

	PCClassEnum = EPCClass::EPC_End;
	
	BaseMeshAsset = NULL;

	LoadedInGameOnlyAssetPtr = NULL;
	//HUDInfoClassOverride = NULL;

	MeshScaleOverride = 0.0f;
	CapsuleHalfHeightOverride = 0.0f;
	CapsuleRadiusOverride = 0.0f;
	DamageNumZOffsetOverride = 0.0f;

	TargetingDistanceOverride = 0.0f;
	MaxDistanceToAllowAutoAttackOverride = 0.0f;
	DetectDistanceOverride = 0.0f;

	bOverride_TargetingMode = false;
	TargetingModeOverride = ETargetingMode::ETM_Normal;
}

UB2InGameOnlyPCClassInfo* UB2PCClassInfo::GetInGameOnlyInfo(UObject* WorldContextObject)
{
//#if !UE_BUILD_SHIPPING
//	// ���⼭ WorldContextObject �� ���Ӹ�� üũ���̴�. NULL �� �����൵ �۵��� �ϰ����� ������ �׷��� ����.
//	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
//	// �κ񿡼� ���ٰ� �ؼ� �۵� ���ϴ� �� �ƴϴ�. ���� ������� ����� ���� �޸𸮿� �ε� �ð� ���� �����ؾ� �Ѵ�.
//	// ���� �κ񿡼� �̰� ����ؾ� �ϴ� �䱸������ ���� ���� �����ϰ� ����� ���� �Ѵ�. �ſ� ���� �͵��� �����ؾ� �� �� �ִ�.
//	// ������ ���� �������� �� ���鼭 �ѹ��� Ŭ���� �ϳ��� �񵿱� �ε��ذ��� ����� �� �ִٸ� �� �� ����.
//	checkSlow(!LobbyGM);
//#endif
//
//	// ������ AsyncLoad ��û�� ���ȴµ� ���� �Ϸ� �� �� ��Ȳ.
//	if (InGameOnlyAssetAsyncLoadHandle.IsValid() && !InGameOnlyAssetPtr.IsValid())
//	{
//		InGameOnlyAssetAsyncLoadHandle->WaitUntilComplete(GENERAL_TASSET_ASYNC_LOAD_TIMEOUT); // Timeout �� ������.. �Ѿ�� ��Ȳ�� �����̷���
//	}	
//	if(!InGameOnlyAssetPtr.IsValid())
//	{
//		SyncLoadInGameOnlyAssets();
//	}
//	BII_CHECK(InGameOnlyAssetPtr.IsValid()); /* ������� �ͼ��� invalid �� ���� */
//	if (InGameOnlyAssetPtr.IsValid())
//	{
//		ConditionalSetLoadedInGameOnlyInfoRootSet();
//
//		LoadedInGameOnlyAssetPtr = InGameOnlyAssetPtr.Get(); // SyncLoadInGameOnlyAssets ���� �⺻ ĳ�����ִµ� Async �ε��� �־.. ��ġ�� �ȵ�.
//		return Cast<UB2InGameOnlyPCClassInfo>(LoadedInGameOnlyAssetPtr->GetDefaultObject());
//	}
	return NULL;
}

bool UB2PCClassInfo::IsInGameOnlyInfoAtRootSet()
{
	//return (InGameOnlyAssetPtr.IsValid() && InGameOnlyAssetPtr->IsRooted());
	return  true;
}

void UB2PCClassInfo::ReqAsyncLoad(UObject* WorldContextObject, TAsyncLoadPriority InPriority)
{
	//// AsyncLoad �� ���� ���� �ε� �ð� ���� üũ, ������ �̻� ���� �� �׽�Ʈ. ���κ��弭�� �׽�Ʈ�� �ʿ��� �� �־ !UE_BUILD_SHIPPING ���� �� ��.
	//if (!BladeIIGameImpl::bAllowInfoAssetAsyncLoading) { 
	//	SyncLoadInGameOnlyAssets();
	//	return;
	//}

	//if (!InGameOnlyAssetPtr.IsValid()) // ManagedPCClassUnload �� ���� RootSet �� �� �ִµ� ��ư �ε��� �Ǿ� �ִٸ� ��û ����
	//{
	//	TArray<FSoftObjectPath> ThisReqList;
	//	ThisReqList.Add(InGameOnlyAssetPtr.GetSoftObjectPath());
	//	// ���⼭ this �� WorldContextObject �� �־��ְ� �ͱ⵵ �ѵ� �ܺο��� ������ Async Callback �� ���̱Ⱑ ��½ ��ٷο� ���̱⵵..
	//	InGameOnlyAssetAsyncLoadHandle = InfoLoadManager.RequestAsyncLoad(ThisReqList, FStreamableDelegate::CreateUObject(this, &UB2PCClassInfo::InGameOnlyAssetAsyncLoadCB), InPriority);
	//	bInGameOnlyAssetAsyncLoadRequestPending = true; // �� �÷��״� ���� ���� ���� �� �ִ�.. ���� �� ����
	//}
}

void UB2PCClassInfo::InGameOnlyAssetAsyncLoadCB() // ���ξ����忡�� �Ҹ�. ���� Ȯ�� �������θ�. ���� �ε��� ���� waiting ���� �� FStreamableHandle ��.
{
	// �� �⺻ ������ Ʋ���� �𸮾� StreamableManager ������ �߸� �ľ��� ���̰ų� �𸮾� ���� ���װų�
	/*checkf(InGameOnlyAssetPtr.IsValid() && InGameOnlyAssetAsyncLoadHandle.IsValid() && InGameOnlyAssetAsyncLoadHandle->HasLoadCompleted(),
		TEXT("Wrong assumption or abnormal behavior of PCClassInfo async loading. Call DJSong."));
	bInGameOnlyAssetAsyncLoadRequestPending = false;

	LoadedInGameOnlyAssetPtr = InGameOnlyAssetPtr.Get();

	ConditionalSetLoadedInGameOnlyInfoRootSet();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (UB2AssetLoader::bEnableDevLog)
	{
		BII_SCREEN_LOG(FString::Printf(TEXT("[PCClassInfo] AsyncLoad request of %s completed."), *InGameOnlyAssetPtr.ToString()),
			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 8.0f);
	}
#endif
#if !UE_BUILD_SHIPPING
	UE_LOG(LogBladeII, Log, TEXT("AsyncLoad request of %s completed"), *InGameOnlyAssetPtr.ToString());
#endif*/
}

void UB2PCClassInfo::ConditionalSetLoadedInGameOnlyInfoRootSet()
{
	//// ���� ���� �� unload �� �� ���� Ȯ���� RootSet �� �ھƵ�.
	//if (LoadedInGameOnlyAssetPtr && !UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition())
	//{
	//	LoadedInGameOnlyAssetPtr->AddToRoot();
	//}
}

TSubclassOf<ABladeIIPlayer> UB2PCClassInfo::GetBaseBPClass(UObject* WorldContextObject)
{
	//// ���⼱ InGameOnlyAsset �� �ε����� �ʴ´�. �ε����� ���� ��Ȳ���� �̰� ����� ��찡 ����� ���..
	//if (InGameOnlyAssetPtr.IsValid())
	//{
	//	UB2InGameOnlyPCClassInfo* InGameOnlyAssetObj = Cast<UB2InGameOnlyPCClassInfo>(InGameOnlyAssetPtr.Get()->GetDefaultObject());
	//	return InGameOnlyAssetObj ? InGameOnlyAssetObj->GetBaseBPClass(WorldContextObject) : NULL;
	//}

	return NULL;
}

void UB2PCClassInfo::PostLoad()
{
	Super::PostLoad();
}

ECharacterType UB2PCClassInfo::FromPCClassToCharacterType(EPCClass InPCClassEnum)
{
	// �̷��� �����ؼ� CharacterType �� �����ϴ� ��� BladeIIPlayer �ʿ� EPCClass ����� �־���� ����..
	switch (InPCClassEnum)
	{
	case EPCClass::EPC_Assassin: return ECharacterType::ECT_Assassin;
	case EPCClass::EPC_Gladiator : return ECharacterType::ECT_Gladiator;
	case EPCClass::EPC_Wizard: return ECharacterType::ECT_Wizard;
	case EPCClass::EPC_Fighter: return ECharacterType::ECT_Fighter;
	}

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	return ECharacterType::ECT_UnknownPC;
}

EPCClass UB2PCClassInfo::FromCharacterTypeToPCClass(ECharacterType InChTypeEnum)
{
	switch (InChTypeEnum)
	{
	case ECharacterType::ECT_Assassin: return EPCClass::EPC_Assassin;
	case ECharacterType::ECT_Gladiator: return EPCClass::EPC_Gladiator;
	case ECharacterType::ECT_Wizard: return EPCClass::EPC_Wizard;
	case ECharacterType::ECT_Fighter: return EPCClass::EPC_Fighter;
	}

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	return EPCClass::EPC_End;
}

#if WITH_EDITOR
void UB2PCClassInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

}

bool UB2PCClassInfo::CheckPCClassEnumNamingConvention()
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EPCCLASS_ENUM_PACKAGE_NAME, true);
	//BII_CHECK(EnumPtr);

	//if(EnumPtr == NULL)
	//{
	//	return false;
	//}

	//for(int32 EI = 0; EI < GetMaxPCClassNum(); ++EI)
	//{
	//	FString ClassEnumName = EnumPtr->GetNameStringByIndex(EI);

	//	// Prefix Ȯ��. ���� ABladeIIPlayer::GetTrimmedNameFromPCClassEnum ������ ����..
	//	if(ClassEnumName.StartsWith(EPCCLASSENUM_COMMON_PREFIX) == false)
	//	{
	//		return false;
	//	}
	//}

	return true;
}

#endif

// TAsset lazy-loading �� ����ϴ� Blueprint InfoAsset �� ������ ������ �������� �ذ��ϱ� �������� �ӽ� ����.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2PCClassInfo::CleanupOnPreSave()
{
	LoadedInGameOnlyAssetPtr = NULL;
}
void UB2PCClassInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
	CleanupOnPreSave();
}
#endif


/**
 * ======================================================================================================
 * For dynamically merge player character mesh sections
 * Ư�� ��� ���� Ȯ���� ���� �÷��̾� ĳ���� mesh section ���� �� InfoAsset
 * ======================================================================================================
 */

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
int32 UB2PCMeshSectionMergeInfo::SectionMergeTestFlag = 0; // 1�̸� ������ ��� 2�� ������ ����, �������� ���� ��ɿ� ����
bool UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge()
{
	return SectionMergeTestFlag == 1;
}
bool UB2PCMeshSectionMergeInfo::ShouldForceUnuseSectionMerge()
{
	return SectionMergeTestFlag == 2;
}
#endif

/** ���� ���� �޽� �� ��� ������ scalability ����. �Ϻ� ���Ӹ�忡���� ���ǰ�, �̰Ͱ� ������� ������ ���� ���� ���� ����ϴ� ��嵵 �ִ�.
 * ������ 1 �� ���� ��� ���� �ǵ��� SectionMergedMeshUse �� �ݴ� �ǹ̷�. */
static TAutoConsoleVariable<int32> CVarB2GameSectionPreservedMeshUse(
	TEXT("r.B2GameSectionPreservedMeshUse"),
	1,
	TEXT("If 0, Some game-mode will use section-merged mesh model to save render thread time.\n"),
	ECVF_Scalability 
);

bool UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(UObject* WorldContextObject)
{
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	//IGameRule* GameRuleObj = B2GM ? B2GM->GetGameRule() : nullptr;
	//return GameRuleObj ? 
	//	// ���Ӹ�� �������� ���� ���� ���� �����ϴ� ��쵵 �ְ�, �ɼǿ� ���� ����ϴ� ��쵵 ����.
	//	(GameRuleObj->UseSectionMergedPCModel() || (GameRuleObj->ConditionalUseSectionMergedPCModel() && (CVarB2GameSectionPreservedMeshUse.GetValueOnGameThread() == 0)))
	//	: false;
	return false;
}
bool UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(UObject* WorldContextObject)
{
	// ���⿡�� ���� ���յ� �� ������� �����ε� ���� ��������� �ʴ��� Ȥ�ö� ���߿� ����� �ְ� �� �� ���ľ� �ϴ� �κе� ã�� ������
//	return (
//		false // ������ �ϰ� �Ǹ� �Ƹ��� ����ڰ� ������ �ɼ� ������ �� ��..
//#if 0
//		&& ShouldUseSectionMergeByGameContext(WorldContextObject) // ���� ���� �� �� ������ �⺻���� üũ �ʿ�.
//#endif
//		);
	return false;
}

UB2PCMeshSectionMergeInfo::UB2PCMeshSectionMergeInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2PCMeshSectionMergeInfo::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	if (GIsEditor)
	{
		CheckDataIntegrity();
	}
#endif
}

const FPCMeshSectionMergeGroupData* UB2PCMeshSectionMergeInfo::GetMergeSectionData(bool InHaveCombineCostume, EPCPart InPartEnum) const
{
	const TArray<FPCMeshSectionMergeGroupData>* SelectMergeSctionData = InHaveCombineCostume ? &CombineCostumeMergedSectionData : &MergedSectionData;

	for (const FPCMeshSectionMergeGroupData& MergeGroup : *SelectMergeSctionData)
	{
		for (const FMergeSectionParameter& ParamInfo : MergeGroup.MergeMtrlParameters)
		{
			if (ParamInfo.TargetPart == InPartEnum)
				return &MergeGroup;
		}
	}

	return nullptr;
}

UMaterialInterface* UB2PCMeshSectionMergeInfo::GetMergedSectionMaterial(bool InHaveCombineCostume, EPCPart InPartEnum) const
{
	if(const FPCMeshSectionMergeGroupData* SectionData = GetMergeSectionData(InHaveCombineCostume, InPartEnum))
	{
		return SectionData->SectionMaterial;
	}
	
	return nullptr;
}

const FMergeSectionParameter* UB2PCMeshSectionMergeInfo::GetParamterReplaceInfo(bool InHaveCombineCostume, EPCPart InPartEnum) const
{
	if (const FPCMeshSectionMergeGroupData* SectionData = GetMergeSectionData(InHaveCombineCostume, InPartEnum))
	{
		for (const FMergeSectionParameter& ParamInfo : SectionData->MergeMtrlParameters)
		{
			if (ParamInfo.TargetPart == InPartEnum)
				return &ParamInfo;
		}
	}

	return nullptr;
}

int32 UB2PCMeshSectionMergeInfo::GetMergedSectionID(bool InHaveCombineCostume, EPCPart InPartEnum) const
{
	const FPCMeshSectionMergeGroupData* PartSectionData = GetMergeSectionData(InHaveCombineCostume, InPartEnum);
	return PartSectionData ? PartSectionData->SectionID : 0;
}

FVector2D UB2PCMeshSectionMergeInfo::GetPartUVOffset(bool InHaveCombineCostume, EPCPart InPartEnum) const
{ // ���� ���յ� ���� �ȿ����� �ڱ� ���� ã��
	if(const FMergeSectionParameter* Parameter = GetParamterReplaceInfo(InHaveCombineCostume, InPartEnum))
	{
		return Parameter->UVOffset;
	}

	return FVector2D(0.0f, 0.0f);
}

void UB2PCMeshSectionMergeInfo::SetupMergedSectionMaterialParams(bool InHaveCombineCostume, USkeletalMesh* InSectionMergedMesh, TMap<EPCPart, UMaterialInterface*>& OriginalNonMergedMaterials)
{
	//if (!InSectionMergedMesh) {
	//	return;
	//}
	//
	//// SkeletalMesh �� Material �� �������� ������ MIC �� �ٲ�ģ��.
	//// ���� ���� SkeletalMesh �� �������� ������ ������Ʈ���� �ؼ� �ٲ��ĵ� ��.
	//check(InSectionMergedMesh->HasAnyFlags(RF_Transient));

	//// �⺻������ merge �� ������ �ڵ鸵�ϱ� ���� Ư���� ���۵� material �� ���õǾ� �־�� �ϴµ�
	//// ������ ������ material �� ������ �� �ν��Ͻ����� parameter �� �ٸ��� �� ���̹Ƿ� MIC �� ���� �����Ѵ�.
	//for (int32 MI = 0; MI < InSectionMergedMesh->Materials.Num(); ++MI)
	//{
	//	UMaterialInterface* ParentMaterial = InSectionMergedMesh->Materials[MI].MaterialInterface;
	//	UMaterialInstanceConstant* ParentMIC = Cast<UMaterialInstanceConstant>(ParentMaterial);
	//	if (ParentMIC && ParentMIC->HasAnyFlags(RF_Transient)) 
	//	{
	//		// �� ��� �̹� ������ ���⼭ transient �� �����ؼ� ������ MIC �� ����. ���� ���ǿ��� merge �� �޽��� GameMode �� ĳ���� ���� ���Ƿ� ����� ���� �� �� �ִ�.
	//		// ���� ����ȭ�� ��ġ�� �Ǹ�.. �̹� �ѹ� ������ ���� �� ������ �̰����� ���� ���ų� �� �ʿ� ���� �״�� ����� �� �ֵ��� �ϴ� �� ���� ��.
	//	}
	//	else
	//	{
	//		// MIC �� dynamic create �� ���.. RF_Transient �� �־��ش�. MaterialInstanceConstant ���� Set~~TransientObjOnly �� ����.
	//		// Ư���� ����� ���ε� MIC �� �⺻������ Editor ���� �����ؼ� ����ϴ� ���̴�. �׷��� SkeletalMeshMerge ���Ͱ� �̹� �׷� ���� ���� �ǵ� ���� �� �� ����?
	//		UMaterialInstanceConstant* NewMIC = NewObject<UMaterialInstanceConstant>(InSectionMergedMesh, NAME_None, RF_Transient);
	//		if (NewMIC)
	//		{
	//			// ������ ���õǾ� �ִ� material �� parent �� �� MIC �� �����ؼ� ���.
	//			NewMIC->SetParentTransientObjOnly(ParentMaterial, false); // MIC �� dynamic �ϰ� ���鼭 ���� MIC ���� ���� �������̽� �վ����
	//			FSkeletalMaterial& OldMaterialSetup = InSectionMergedMesh->Materials[MI];
	//			FSkeletalMaterial NewMaterialSetup(NewMIC, true);
	//			NewMaterialSetup.UVChannelData = OldMaterialSetup.UVChannelData;
	//			InSectionMergedMesh->Materials[MI] = NewMaterialSetup;
	//		}
	//	}
	//}

	//// Merge �ȵ� ���� material ������ ���鼭 �ڽſ� �ش��ϴ� merge �� ��Ʈ�� section �� ���õ� MIC �� ã�� material parameter �� ����
	//for(TMap<EPCPart, UMaterialInterface*>::TIterator OriginalMtrlIt(OriginalNonMergedMaterials); OriginalMtrlIt; ++OriginalMtrlIt)
	//{
	//	const EPCPart ThisPartEnum = OriginalMtrlIt.Key();
	//	UMaterialInstance* ThisPartOriginalMI = Cast<UMaterialInstance>(OriginalMtrlIt.Value());

	//	// ���� MeshMerge ����� ���ϴ� SectionID ��� ������ ����� �ȵǰ� �־ Mesh �� Materials �� �ε��� �־ �������� ���ϴ� material �� �� �� ������ �� ����
	//	// �װ� ��ġ�� �������� �θ� ��Ƽ������ üũ�ϴ� ������ �ؼ� ���ϴ� ���� ��Ʈ�� MIC �� �����´�.		
	//	UMaterialInterface* SupposedParentMtrl = GetMergedSectionMaterial(InHaveCombineCostume, ThisPartEnum);
	//	UMaterialInstanceConstant* MergedMIC = nullptr;
	//	// �̰� ����μ��� �ȸ����ٴ� ��. MeshMerge ����� �� ������.
	//	//const int32 MergedSectionID = GetMergedSectionID(ThisPartEnum);
	//	//UMaterialInstanceConstant* MergedMIC = Cast<UMaterialInstanceConstant>(InSectionMergedMesh->Materials[MergedSectionID].MaterialInterface);
	//	for (int32 MI = 0; MI < InSectionMergedMesh->Materials.Num(); ++MI)
	//	{
	//		UMaterialInstanceConstant* ThisMIC = Cast<UMaterialInstanceConstant>(InSectionMergedMesh->Materials[MI].MaterialInterface);
	//		if (ThisMIC && ThisMIC->Parent == SupposedParentMtrl) {
	//			MergedMIC = ThisMIC;
	//			break;
	//		}
	//	}

	//	const FMergeSectionParameter* MergeParameter = GetParamterReplaceInfo(InHaveCombineCostume, ThisPartEnum);

	//	if (ThisPartOriginalMI && MergedMIC && MergeParameter)
	//	{
	//		// Set Scalar Parameters
	//		for (const FMaterialParamterReplace& ScalarInfo : MergeParameter->ScalarParams)
	//		{
	//			float ParamValue = 1.f;
	//			const FName OrgParamName = ScalarInfo.OriginalMtrlParam;

	//			if (ThisPartOriginalMI->GetScalarParameterValue(OrgParamName, ParamValue))
	//			{
	//				const FName& ReplaceParamName = ScalarInfo.ReplaceMtrlParam;
	//				MergedMIC->SetScalarParameterValueTransientObjOnly(ReplaceParamName, ParamValue);
	//			}
	//		}

	//		// Set Vector Parameters
	//		for (const FMaterialParamterReplace& VectorInfo : MergeParameter->VectorParams)
	//		{
	//			FLinearColor ParamValue(1.f, 1.f, 1.f, 1.f);
	//			const FName OrgParamName = VectorInfo.OriginalMtrlParam;

	//			if (ThisPartOriginalMI->GetVectorParameterValue(OrgParamName, ParamValue))
	//			{
	//				const FName& ReplaceParamName = VectorInfo.ReplaceMtrlParam;
	//				MergedMIC->SetVectorParameterValueTransientObjOnly(ReplaceParamName, ParamValue);
	//			}
	//		}

	//		// Set Texture Parameters
	//		for (const FMaterialParamterReplace& TextureInfo : MergeParameter->TextureParams)
	//		{
	//			UTexture* ParamValue = nullptr;
	//			const FName OrgParamName = TextureInfo.OriginalMtrlParam;
	//			const FName& ReplaceParamName = TextureInfo.ReplaceMtrlParam;

	//			if (ThisPartOriginalMI->GetTextureParameterValue(OrgParamName, ParamValue))
	//			{
	//				MergedMIC->SetTextureParameterValueTransientObjOnly(ReplaceParamName, ParamValue);
	//			}
	//			else
	//			{
	//				MergedMIC->SetTextureParameterValueTransientObjOnly(ReplaceParamName, nullptr);
	//			}
	//		}

	//		if (ThisPartEnum == EPCPart::EPCPT_Wing && HasWingAuraEmissiveTexture(ThisPartOriginalMI))
	//		{
	//			const FName EmissiveFlag = GetWingEmissiveParamName();
	//			MergedMIC->SetScalarParameterValueTransientObjOnly(EmissiveFlag, 1.f);
	//		}
	//	}
	//}
}

const FCostumeReplaceData * UB2PCMeshSectionMergeInfo::GetCostumeReplaceData(ECostumeEquipPlace InCostumeEquipPlace) const
{
	for (const FCostumeReplaceData& data : CostumeReplaceData)
	{
		if (data.CostumeEquipPlace == InCostumeEquipPlace)
			return &data;
	}
	return nullptr;
}

void UB2PCMeshSectionMergeInfo::GetReplaceCostumeEquipPlace(ECostumeEquipPlace InCostumeEquipPlace, TArray<EPCPart>& OutPCPartReplace)
{
	OutPCPartReplace.SetNum(0);
		
	const FCostumeReplaceData* ReplaceData = GetCostumeReplaceData(InCostumeEquipPlace);
	if (ReplaceData == nullptr)
		return;
	
	OutPCPartReplace.Append(ReplaceData->CombineCostumePCPart);
}

#if WITH_EDITOR
void UB2PCMeshSectionMergeInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CheckDataIntegrity();
}

void UB2PCMeshSectionMergeInfo::CheckDataIntegrity()
{
	// MergedSectionData ���� �� �߸��� �� �ִ� �κе� üũ.
	int32 ContainingPartConflictFound = 0;
	int32 ExistParts = 0;

	TArray<EPCPart> ValidParts;

	for (const FPCMeshSectionMergeGroupData& MergeGroup : MergedSectionData)
	{
		for (const FMergeSectionParameter& SectionParam : MergeGroup.MergeMtrlParameters)
		{
			ValidParts.AddUnique(SectionParam.TargetPart);
		}
	}

	const int32 PartsMax = static_cast<int32>(EPCPart::EPCPT_End);
	for (int32 PartsIndex = 0; PartsIndex < PartsMax; PartsIndex++)
	{
		EPCPart TargetPart = static_cast<EPCPart>(PartsIndex);
		if (ValidParts.Contains(TargetPart))
		{
			ExistParts++;
		}
		else
		{
			ContainingPartConflictFound++;
		}
	}


	int32 NonExistParts = PartsMax - ExistParts;
	if (ContainingPartConflictFound > 0 || NonExistParts > 0)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("[PCMeshSectionMergeInfo] �߸��� ������ �߰ߵǾ����ϴ�.\r\n���� �ٸ� �׷� ���� ContainingParts �ߺ� : %d ��\r\n�׷� ������ ContainingParts �� �������� ���� Part �� ���� ���� : %d ��"),
				ContainingPartConflictFound, NonExistParts)
		));
#endif

	}
}
#endif