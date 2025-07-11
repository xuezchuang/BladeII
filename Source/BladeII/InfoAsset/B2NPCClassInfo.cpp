// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square


//#include "BehaviorTree/BehaviorTree.h"
//#include "Animation/AnimBlueprint.h"
//#include "Animation/AnimBlueprintGeneratedClass.h"
//#include "Materials/Material.h"
#include "B2NPCClassInfo.h"
#include "BladeIICharacter.h"


//#include "B2NPCClassInfoBox.h"
//#include "BladeIIGameMode.h"
//#include "B2StageGameMode.h"
//#include "B2HeroTowerGameMode.h"
//#include "B2StageManager.h"
//#include "B2DropItem.h"
//#include "B2Inventory.h"
//#include "B2ItemInfo.h"
//#include "B2AreaDamageActorBase.h"
//#include "B2NPCSensitiveInfo.h"
//#include "B2SomeInfo.h"
//#include "B2GameInstance.h"
#include "BladeIIUtil.h"

#include "FB2ErrorMessage.h"
//#include "BladeIIGameImpl.h"
#include "Components/CapsuleComponent.h"

void ABladeIICharacter::OverrideByNPCClassInfo(ENPCClass InClass, UB2NPCSingleClassInfo* InClassInfo)
{
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::OverrideByNPCClassInfo"));

	//check(InClassInfo);

	//// NPCClassInfo �� override �ϱ� �� BaseBP override ����. �� ������ ��ü�� ������ ������ PostSpawnInitialize �� ���� ���� ������ �� ��.
	//this->OverrideByBaseBP(InClassInfo->GetBaseBPClass(this));

	//this->NPCClassEnum = InClass;
	//this->NPCClassVariation = InClassInfo->ClassVariation;
	//
	//// CharacterType �� �״�� �������. 
	//// ���⼭ CharacterType �� �����ߴ� �ϴ��� MonsterSpawnPool ���� ���̴� MonsterClassTypeMap ������ �ٸ� type �� ������ �� �� ������ �ش� ������ ����ؼ� spawn �ǰ� ���� �׿� �°� CharacterType �� �ٽ� ���õ� ��.
	//this->CharacterType = InClassInfo->DefaultCharacterType;

	//UCapsuleComponent* CollisionCapsule = this->GetCapsuleComponent();

	//if (CollisionCapsule)
	//{
	//	if (InClassInfo->CapsuleHalfHeightOverride > 0.0f)
	//	{
	//		CollisionCapsule->SetCapsuleHalfHeight(InClassInfo->CapsuleHalfHeightOverride);
	//	}
	//	if (InClassInfo->CapsuleRadiusOverride > 0.0f)
	//	{
	//		CollisionCapsule->SetCapsuleRadius(InClassInfo->CapsuleRadiusOverride);
	//	}

	//	if (InClassInfo->CapsuleCollisionObjectTypeOverride != ECC_Pawn)
	//	{
	//		CollisionCapsule->SetCollisionObjectType(InClassInfo->CapsuleCollisionObjectTypeOverride);
	//	}
	//}

	//if (InClassInfo->ShadowSizeOverride > 0.0f)
	//{
	//	this->ShadowSizeCenter = InClassInfo->ShadowSizeOverride;
	//}
	//if (InClassInfo->DamageNumZOffsetOverride != 0.0f) // - ���� �� �� �ְ�..
	//{
	//	this->DamageNumZOffset = InClassInfo->DamageNumZOffsetOverride;
	//}

	//if (InClassInfo->bUsePreSpawnState)
	//{
	//	// �̰��� AI BT �ʿ��� DetectDistance ���� �÷��̾� ĳ���Ͱ� �����Ǹ� false �� �� ��. �̰��� ���õ��� ���� BT �� ���Ǹ� �ʱ� ���¿��� ����� ���ϰ� �ȴ�.
	//	// AnimBP ƽ���� ó������ �̰� üũ�ϹǷ� AnimBP ���� (SetAnimInstanceClass) ���� �ؾ� ��.
	//	this->bNeedSpawnCheck = true;
	//	//this->CustomTimeDilation = 0.0f; -> �� ����� �����ϰ� ������ �𸣰ڴ�. ������ note..
	//}

	//this->bUsePhaseChangeAnimation = InClassInfo->bUsePhaseChangeAnimation;

	//this->QTEType = InClassInfo->QTEType;
	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (GameMode && GameMode->GetGameRule()->IsQTEEnabled())
	//{
	//	this->QTEInvokeHPPercent = InClassInfo->QTEInvokeHPPercent;
	//}
	//else
	//{
	//	this->QTEInvokeHPPercent = -1.f; // Disable QTE Enable state
	//}

	//this->QTEMountTime = InClassInfo->QTEMountTime;
	//this->QTEEnableTime = InClassInfo->QTEEnableTime;

	//USkeletalMeshComponent* MeshComp = this->GetBaseMesh();  // NPC ���� Ư���� attachment �ܿ� �� �⺻�� multi-component ������ �ƴ� ���� BaseMesh �� ���.

	//if (MeshComp)
	//{
	//	if (InClassInfo->MeshAssetOverride)
	//	{
	//		USkeletalMesh* DefaultMesh = MeshComp->SkeletalMesh;

	//		if (DefaultMesh != NULL && DefaultMesh != InClassInfo->MeshAssetOverride)
	//		{
	//			// If the mesh is different, we better change the actor's name.. 
	//			// Replace if we spot name body containing the mesh name, or append to the back.
	//			FString DefaultName = this->GetName();
	//			FString DefaultMeshName = DefaultMesh->GetName();
	//			FString OverrideMeshName = InClassInfo->MeshAssetOverride->GetName();

	//			int32 MeshNameStartIndex = DefaultName.Find(DefaultMeshName);

	//			if (MeshNameStartIndex != INDEX_NONE)
	//			{
	//				// Replace the name body.
	//				FString NewName = DefaultName.Replace(*DefaultMeshName, *OverrideMeshName);
	//				this->Rename(*NewName);
	//			}
	//			else
	//			{
	//				// Or, just append the new mesh name to the back
	//				this->Rename(*(DefaultName + TEXT("_") + OverrideMeshName));
	//			}
	//		}

	//		// AnimBP �ٲ�ġ�� ���� SetSkeletalMesh ����
	//		MeshComp->SetSkeletalMesh(InClassInfo->MeshAssetOverride);
	//		if (InClassInfo->AnimBPOverride)
	//		{// �̰� �ᱹ MeshComp->SetAnimInstanceClass ��. ����� ����̽� ����� ������ ����
	//			this->SafeSetAnimBPClass(InClassInfo->AnimBPOverride);
	//		}
	//	}

	//	for (int32 MI = 0; MI < InClassInfo->MaterialOverrides.Num(); ++MI)
	//	{
	//		if (InClassInfo->MaterialOverrides[MI])
	//		{
	//			MeshComp->SetMaterial(MI, InClassInfo->MaterialOverrides[MI]);
	//		}
	//	}

	//	if (InClassInfo->MeshScaleOverride > 0.0f)
	//	{
	//		MeshComp->SetRelativeScale3D(FVector(InClassInfo->MeshScaleOverride, InClassInfo->MeshScaleOverride, InClassInfo->MeshScaleOverride));
	//	}

	//	if (CollisionCapsule)
	//	{
	//		// Mesh assets have their pivot root at their feet, so lower it by half capsule size to locate it at the center.
	//		// MeshRelativeOffset �� Z �� capsule �� half height �� �߰��� ����
	//		MeshComp->SetRelativeLocation(FVector(InClassInfo->MeshRelativeOffset.X, InClassInfo->MeshRelativeOffset.Y, -1.0f * CollisionCapsule->GetScaledCapsuleHalfHeight() + InClassInfo->MeshRelativeOffset.Z));
	//	}

	//	// SkeletalMeshComponent �� ���� �߰� ������Ʈ��
	//	for (int32 STI = 0; STI < InClassInfo->AttachStaticMeshes.Num(); ++STI)
	//	{
	//		FCHSKCompAttachStaticMeshInfo& ThisInfo = InClassInfo->AttachStaticMeshes[STI];
	//		if (ThisInfo.StaticMeshAsset != NULL)
	//		{
	//			UStaticMeshComponent* NewAttachComp = NewObject<UStaticMeshComponent>(this, NAME_None, RF_Transient);
	//			if (NewAttachComp)
	//			{
	//				NewAttachComp->bReceivesDecals = false;
	//				NewAttachComp->SetStaticMesh(ThisInfo.StaticMeshAsset);
	//				NewAttachComp->SetRelativeScale3D(FVector(ThisInfo.AttachScale, ThisInfo.AttachScale, ThisInfo.AttachScale));
	//				NewAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh �� Character �� ���̴� Movable ��
	//				NewAttachComp->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisInfo.AttachSocketName);
	//				NewAttachComp->RegisterComponent();
	//			}
	//		}
	//	}
	//	for (int32 STI = 0; STI < InClassInfo->AttachSkeletalMeshes.Num(); ++STI)
	//	{
	//		FCHSKCompAttachSkeletalMeshInfo& ThisInfo = InClassInfo->AttachSkeletalMeshes[STI];
	//		if (ThisInfo.SkeletalMeshAsset != NULL)
	//		{
	//			USkeletalMeshComponent* NewAttachComp = NewObject<USkeletalMeshComponent>(this, NAME_None, RF_Transient);
	//			if (NewAttachComp)
	//			{
	//				NewAttachComp->bReceivesDecals = false;
	//				NewAttachComp->SetSkeletalMesh(ThisInfo.SkeletalMeshAsset);
	//				NewAttachComp->SetRelativeScale3D(FVector(ThisInfo.AttachScale, ThisInfo.AttachScale, ThisInfo.AttachScale));
	//				NewAttachComp->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisInfo.AttachSocketName);
	//				NewAttachComp->RegisterComponent();
	//			}
	//		}
	//	}
	//	for (int32 STI = 0; STI < InClassInfo->AttachParticleSystems.Num(); ++STI)
	//	{
	//		FCHSKCompAttachParticleSystemInfo& ThisInfo = InClassInfo->AttachParticleSystems[STI];
	//		ThisInfo.CreatePSCHelper(this, MeshComp);
	//	}
	//}

	//UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(this->GetMovementComponent());
	//if (MovementComp)
	//{
	//	if (InClassInfo->CharMovementEssentialSettings.MaxWalkSpeedOverride > 0.0f)
	//	{
	//		MovementComp->MaxWalkSpeed = InClassInfo->CharMovementEssentialSettings.MaxWalkSpeedOverride;
	//	}
	//	if ( InClassInfo->CharMovementEssentialSettings.MaxAccelerationOverride > 0.0f)
	//	{
	//		MovementComp->MaxAcceleration = InClassInfo->CharMovementEssentialSettings.MaxAccelerationOverride;
	//	}

	//	// �����̼Ǽӵ� �����(�Է°��� 0�̸��̸� ����� ����)
	//	// ����� ���ϸ� UB2SomeInfo - DefaultNPCClassInfoBaseBPClass ������ �̿���.
	//	if (InClassInfo->CharMovementEssentialSettings.RotationRate.Yaw >= 0.0f)
	//		MovementComp->RotationRate.Yaw = InClassInfo->CharMovementEssentialSettings.RotationRate.Yaw;

	//	if (InClassInfo->CharMovementEssentialSettings.RotationRate.Pitch >= 0.0f)
	//		MovementComp->RotationRate.Pitch = InClassInfo->CharMovementEssentialSettings.RotationRate.Pitch;

	//	if (InClassInfo->CharMovementEssentialSettings.RotationRate.Roll >= 0.0f)
	//		MovementComp->RotationRate.Roll = InClassInfo->CharMovementEssentialSettings.RotationRate.Roll;

	//	this->CachedBaseMaxWalkSpeed = MovementComp->MaxWalkSpeed;
	//}
	//	
	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	//UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : StaticFindSomeInfo(this);

	//// I guess at this point, newly spawned actor with DeferConstruction flag is not yet possessed by controller, so just replacing the BehaviorTree reference is enough.
	//// Anyway, SetBehaviorTree do all stuff necessary. Not really tested though..
	//if (InClassInfo->BehaviorTreeOverride)
	//{
	//	this->SetBehaviorTree(InClassInfo->BehaviorTreeOverride);
	//}		
	//else if (SomeInfo)
	//{
	//	if (InClassInfo->bUsePreSpawnState)
	//	{ // UsePreSpawnState �� AI BT �ʿ��� �÷��̾� �����ؼ� �����ϴ� �� �ʿ���. ���� override �ȵǾ� ������ �⺻ SpawnBT �� ���.
	//		this->SetBehaviorTree(SomeInfo->GetDefaultNPCClassInfoSpawnBT());
	//	}
	//	else
	//	{ // ���� �⺻ BT
	//		this->SetBehaviorTree(SomeInfo->GetDefaultNPCClassInfoMeleeBT());
	//	}
	//}

	//// ����������������� ȸ�� ����
	//if (InClassInfo->bIgnoreRotateByDamage)
	//	this->SetIgnoreRotateByDamage();

	//float HPScale, AttackDefenseLevelScale, MaxArmorOverride;
	//BladeIIGameImpl::GetClientDataStore().GetMonsterGradeFactor(InClass, InClassInfo, HPScale, AttackDefenseLevelScale, MaxArmorOverride);
	//
	//this->SetFinalADScale(AttackDefenseLevelScale);
	//this->SetFinalHPScale(HPScale);

	//ABladeIIGameMode* B2GM = GetBladeIIGameMode(this);
	//// ���� �������� ���̵� ��ġ�� ĳ����(��) �����¿� �ݿ��ȴ�.
	//if (B2GM)
	//	this->CharacterLevel = B2GM->GetCurrentStageDifficultyScale();

	//// CharacterLevel �� ����Ͽ� ��Ÿ �ٸ� ���� �ɷ�ġ���� ����ϹǷ� ������ ��. �ٵ� ������ ABladeIICharacter::BeginPlay ���� ��.
	//this->InitializeCombatStats();

	//if (MaxArmorOverride > 0.0f)
	//{
	//	// Armor ���� ������. InitializeCombatStats ���� CharacterLevel �� ������� ���ȴٸ� ����� ���־� ��.
	//	this->MaxArmor = MaxArmorOverride;
	//	this->Armor = this->MaxArmor;
	//}			

	//if (InClassInfo->DetectDistanceOverride > 0.0f)
	//{
	//	this->DetectDistance = InClassInfo->DetectDistanceOverride;
	//}

	//if (InClassInfo->RegenerateHPPercentagePerSecondOverride > 0.f)
	//{
	//	this->SetRegenerateHPRate(InClassInfo->RegenerateHPPercentagePerSecondOverride, true);
	//}

	//for (int32 PCI = 0; PCI < InClassInfo->AdditionalProjectileClasses.Num(); ++PCI)
	//{
	//	if (InClassInfo->AdditionalProjectileClasses[PCI])
	//	{
	//		this->ProjectileClasses.AddUnique(InClassInfo->AdditionalProjectileClasses[PCI]);
	//	}
	//}
	//for (int32 AAI = 0; AAI < InClassInfo->AdditionalAIAttackDataArray.Num(); ++AAI)
	//{
	//	this->AIAttackDataArray.Add(InClassInfo->AdditionalAIAttackDataArray[AAI]);
	//}
	//for (int32 PCI = 0; PCI < InClassInfo->AdditionalAreaDamageClasses.Num(); ++PCI)
	//{
	//	if (InClassInfo->AdditionalAreaDamageClasses[PCI])
	//	{
	//		this->AreaDamageClasses.AddUnique(InClassInfo->AdditionalAreaDamageClasses[PCI]);
	//	}
	//}
	//// Prevent no AI attack
	//if (this->AIAttackDataArray.Num() == 0)
	//{
	//	this->AIAttackDataArray.Add(FAIAttackData());
	//}

	//for (int32 API = 0; API < InClassInfo->AdditionalPhaseDataArray.Num(); ++API)
	//{
	//	this->PhaseDataArray.Add(InClassInfo->AdditionalPhaseDataArray[API]);
	//}

	//// Currently no overriding condition flag. We might introduce one.
	//this->AllowedDamageStateData.bAllowStun = InClassInfo->AllowedDamageState.bAllowStun;
	//this->AllowedDamageStateData.bAllowFreezing = InClassInfo->AllowedDamageState.bAllowFreezing;
	//this->AllowedDamageStateData.bAllowBurning = InClassInfo->AllowedDamageState.bAllowBurning;
	//this->AllowedDamageStateData.bAllowPoison = InClassInfo->AllowedDamageState.bAllowPoison;
	//this->AllowedDamageStateData.bAllowBigBounce = InClassInfo->AllowedDamageState.bAllowBigBounce;
	//this->AllowedDamageStateData.bAllowKnockBack = InClassInfo->AllowedDamageState.bAllowKnockBack;

	//this->SetDropItemInfo(InClassInfo->DropItemInfo);

	//this->bForceReceivdAbsoluteDamage = InClassInfo->bForceReceivdAbsoluteDamage;
	//
	//this->bAllowRecoilAnimation = InClassInfo->bAllowRecoilAnimation;

	//this->bBloodStoneAura = InClassInfo->bBloodStoneAura;
	//this->BloodStoneType = InClassInfo->BloodStoneType;
	//this->BloodStoneAmount = InClassInfo->BloodStoneAmount;
	//this->BloodStoneBuffLogicType = InClassInfo->BloodStoneBuffLogicType;

	//this->bForceReceivdAttackCountDamage = InClassInfo->bForceReceivdAttackCountDamage;
	//this->FullAttactCountSize = InClassInfo->FullAttactCountSize;

	//this->AttachHPBarSocketName = InClassInfo->HPBarSocketName;

	//this->SpawnNotifyMsgString = InClassInfo->SpawnNotifyMsgString;
	//this->SpawnNotifyMsgTime = InClassInfo->SpawnNotifyMsgTime;

	//if (InClassInfo->bFixInAir)
	//{
	//	if (MovementComp)
	//	{
	//		MovementComp->SetAutoActivate(false);
	//	}
	//}	
}

void ABladeIICharacter::OverrideByBaseBP(TSubclassOf<class ABladeIICharacter> BPClassToOverride)
{
	//ABladeIICharacter* BPCDO = BPClassToOverride ? Cast<ABladeIICharacter>(BPClassToOverride->GetDefaultObject()) : NULL;
	//if (!BPCDO)
	//	return;

	//// �̰��� �Ҹ��� ������ NPCClassInfo �����̳� BeginPlay ������.
	//// ���ҽ� ���۷��� ������ ��� ���� �� ���� ��� ������ �������� Ȯ���� �ʿ䰡 ����.

	//// ���� ������Ʈ���� ���� ó���ؾ� �Ѵ�. this ����� CopyObjectProperties ���� ���� ������Ʈ���� �����ϰ� �ϸ� ������Ʈ ��ü�� �״�� ������ ����ϹǷ� �ȵ�.

	//CopyObjectProperties(this->GetMovementComponent(), BPCDO->GetMovementComponent(), true, true);
	//CopyObjectProperties(this->CenterShadow, BPCDO->CenterShadow, true, true);

	//// SkeletalMeshComponent �� �� ������ ������ �־ ���� �Ӽ��� �����ؼ� �������� ����. ���� CopyObjectProperties �� ����Ϸ��� ������ �Ӽ� �̸� ���͸� ������ ���� �ִ�.
	//{
	//	USkeletalMeshComponent* ThisSKMeshComp = this->GetMesh();
	//	USkeletalMeshComponent* BPCDOSKMeshComp = BPCDO->GetMesh();
	//	check(ThisSKMeshComp && BPCDOSKMeshComp);

	//	ThisSKMeshComp->ComponentTags = BPCDOSKMeshComp->ComponentTags;
	//	ThisSKMeshComp->bAutoActivate = BPCDOSKMeshComp->bAutoActivate;
	//	ThisSKMeshComp->SetCanEverAffectNavigation(BPCDOSKMeshComp->CanEverAffectNavigation());

	//	ThisSKMeshComp->SetLinearDamping(BPCDOSKMeshComp->GetLinearDamping());
	//	ThisSKMeshComp->SetAngularDamping(BPCDOSKMeshComp->GetAngularDamping());

	//	ThisSKMeshComp->SetCollisionProfileName(BPCDOSKMeshComp->GetCollisionProfileName());
	//	ThisSKMeshComp->SetCollisionEnabled(BPCDOSKMeshComp->GetCollisionEnabled());
	//	ThisSKMeshComp->SetCollisionObjectType(BPCDOSKMeshComp->GetCollisionObjectType());
	//	ThisSKMeshComp->SetCollisionResponseToChannels(BPCDOSKMeshComp->GetCollisionResponseToChannels());

	//	ThisSKMeshComp->SetRelativeScale3D(BPCDOSKMeshComp->GetRelativeTransform().GetScale3D());

	//	ThisSKMeshComp->MeshComponentUpdateFlag = BPCDOSKMeshComp->MeshComponentUpdateFlag;

	//	// [������] ������ ī�޶� ���������� ������� �׻� ������Ʈ�� �ϵ��� �Ѵ�
	//	if ( EB2GameMode::Scenario == GetB2GameModeType( this ) )
	//	{
	//		ThisSKMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//	}
	//}

	//CopyObjectProperties(this->GetCapsuleComponent(), BPCDO->GetCapsuleComponent(), true, true);

	//// �ڱ� �ڽ��� �� ���߿� ó���� ����. ���⿣ ���ܽ�Ű�� �Ӽ������� �� �ִµ� MinimalDLCFrontMode �� ���� �ʹݿ� BaseBP ���� �ε� �ȵ� ������ ����ϸ� �ȵǱ� ����.
	//CopyObjectProperties(this, BPCDO, true, true, BaseBPCopyExcludeProperties);
}

FDropItemInfo::FDropItemInfo()
{
	RewardGoldWeight = 0;
	bCanDropItem = false;
	GoldSpawnDelay = 0.0f;
	RewardSpawnDelay = 0.0f;
	TestNumSpawnGold = 1;
	TestNumSpawnItemRefID = 0;
}


/**
 * ========================================================================
 * NPCSingleClassInfo
 * NPCClass, Variation ������ Ŭ���̾�Ʈ ������ ����
 * ========================================================================
 */
UB2NPCSingleClassInfo::UB2NPCSingleClassInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NPCClassEnum = ENPCClass::ENC_End;
	ClassVariation = ENPCClassVariation::ENCV_Normal;

	//DefaultCharacterType = ECharacterType::ECT_MeleeMob;
	//SensitiveDataRowKey = NAME_None;
	//BaseBPClass = NULL; // Use default one at SomeInfo if NULL.

	//MeshAssetOverride = NULL;
	//MeshRelativeOffset = FVector(0.0f, 0.0f, 0.0f);
	//AnimBPOverride = NULL;
	//BehaviorTreeOverride = NULL;
	//bIgnoreRotateByDamage = false;
	//bFixInAir = false;

	//MeshScaleOverride = 0.0f;
	//CapsuleHalfHeightOverride = 0.0f;
	//CapsuleRadiusOverride = 0.0f;
	//CapsuleCollisionObjectTypeOverride = ECC_Pawn;
	//ShadowSizeOverride = 0.f;
	//DamageNumZOffsetOverride = 0.0f;

	//bUsePreSpawnState = false;
	//QTEType = EQTEType::EQT_End;
	//QTEInvokeHPPercent = 0.0f;
	//QTEMountTime = 20.f;
	//QTEEnableTime = 5.f;
	//DetectDistanceOverride = 0.0f;

	//DropItemInfo.GoldSpawnDelay = 0.0f;
	//DropItemInfo.RewardSpawnDelay = 0.0f;

	//bForceReceivdAbsoluteDamage = false;
	//bForceReceivdAttackCountDamage = false;
	//FullAttactCountSize = 0;

	//InfluenceType = EInfluenceType::EIT_End;
	//InfluenceAmount = 0.f;
	//InfluenceDuration = 0.f;
	//InfluenceStateDuration = 0.f;
	//InitialInfluenceStateTriggerRate = 1.0f;

	//bBloodStoneAura = false;
	//BloodStoneType = EBloodStoneType::EBST_UnBeatable;
	//BloodStoneAmount = -1.f;
	//BloodStoneBuffLogicType = EDamageLogicType::ELT_Normal;
	//
	//bAllowRecoilAnimation = true;

	//SpawnNotifyMsgString = TEXT("");
	//SpawnNotifyMsgTime = -1.f;
}

FText UB2NPCSingleClassInfo::GetLocalizedName()
{
	return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, LocalizedNameKey.ToString());;
	//return FText::GetEmpty();
}

FText UB2NPCSingleClassInfo::GetLocalizedNickName()
{
	return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, LocalizedNickNameKey.ToString());
}

TSubclassOf<class ABladeIICharacter> UB2NPCSingleClassInfo::GetBaseBPClass(UObject* WorldContextObject)
{
	//if (BaseBPClass)
	//{
	//	return BaseBPClass;
	//}
	//UB2SomeInfo* SomeInfo = StaticFindSomeInfo(WorldContextObject);
	//return SomeInfo ? SomeInfo->GetDefaultNPCClassInfoBaseBPClass() : NULL;
	return NULL;
}

void UB2NPCSingleClassInfo::PostLoad()
{
	//Super::PostLoad();
	//// ������ ���� ���ٸ� ��õ ������.
	//SensitiveDataRowKey = UB2NPCSensitiveInfo::SuggestSensitiveInfoDataRowKey(NPCClassEnum, ClassVariation);
}

void UB2NPCSingleClassInfo::GetAllExpectedPhaseClasses(TArray<FCombinedNPCClassID>& OutClassIDs)
{
	//// Phase Ŭ������ ������ �ĺ��ؼ� �ε��� ���� ������ �÷��� ���� �ε� ���� �����ϴ� ��Ұ� ��.
	//for (FAIPhaseData& ThisPhaseData : AdditionalPhaseDataArray)
	//{
	//	if (ThisPhaseData.BladeIICharacterClass != ENPCClass::ENC_End)
	//	{
	//		FCombinedNPCClassID ThisCombinedId(ThisPhaseData.BladeIICharacterClass, ThisPhaseData.CharacterClassVariation);
	//		OutClassIDs.AddUnique(ThisCombinedId);
	//	}
	//}
}

#if !UE_BUILD_SHIPPING
ENPCClass UB2NPCSingleClassInfo::FindNPCClassEnumFromTrimmedName(FString InTrimmedName)
{
	//FString ExpectedFullName = ENPCCLASSENUM_COMMON_PREFIX + InTrimmedName; // InTrimmedName �� prefix ���� �ɷ� ����

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASS_ENUM_PACKAGE_NAME, true);
	//if (EnumPtr)
	//{
	//	for (int32 EI = 0; EI < (int32)ENPCClass::ENC_End; ++EI)
	//	{
	//		if (EnumPtr->GetNameStringByIndex(EI) == ExpectedFullName)
	//		{
	//			return (ENPCClass)EI;
	//		}
	//	}
	//}
	return ENPCClass::ENC_End;
}

ENPCClassVariation UB2NPCSingleClassInfo::FindNPCClassVariEnumFromTrimmedName(FString InTrimmedName)
{
	FString ExpectedFullName = ENPCCLASSVARIENUM_COMMON_PREFIX + InTrimmedName; // InTrimmedName �� prefix ���� �ɷ� ����

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASSVARI_ENUM_PACKAGE_NAME, true);
	//if (EnumPtr)
	//{
	//	for (int32 EI = 0; EI <= (int32)ENPCClassVariation::ENCV_ControlBattle; ++EI) // �̰� End �� ����.. �Ǽ��� �����ȴٰ� �� ���� ������ �׽�Ʈ ����̴�.
	//	{
	//		if (EnumPtr->GetNameStringByIndex(EI) == ExpectedFullName)
	//		{
	//			return (ENPCClassVariation)EI;
	//		}
	//	}
	//}
	return ENPCClassVariation::ENCV_Normal;
}
#endif

#if WITH_EDITOR
void UB2NPCSingleClassInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	const FName Name_NPCClassEnum = GET_MEMBER_NAME_CHECKED(UB2NPCSingleClassInfo, NPCClassEnum);
//	const FName Name_ClassVariation = GET_MEMBER_NAME_CHECKED(UB2NPCSingleClassInfo, ClassVariation);
//	const FName Name_DefaultCharacterType = GET_MEMBER_NAME_CHECKED(UB2NPCSingleClassInfo, DefaultCharacterType);
//	const FName Name_LocalizedNameKey = GET_MEMBER_NAME_CHECKED(UB2NPCSingleClassInfo, LocalizedNameKey);
//	const FName Name_LocalizedNickNameKey = GET_MEMBER_NAME_CHECKED(UB2NPCSingleClassInfo, LocalizedNickNameKey);
//	const FName Name_UsePreSpawnState = GET_MEMBER_NAME_CHECKED(UB2NPCSingleClassInfo, bUsePreSpawnState);
//	const FName Name_BehaviorTreeOverride = GET_MEMBER_NAME_CHECKED(UB2NPCSingleClassInfo, BehaviorTreeOverride);
//
//	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
//	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;
//
//	if (PropertyName == Name_NPCClassEnum)
//	{
//		// ���⼱ MobClassInfoBox �� ã�Ƽ� �˻��ϴµ�, ���� �ٸ� NPCClassInfoBox �� �߰��Ǹ� �װ͵� �˻��ؾ� ��.
//		UB2NPCClassInfoBox* MobClassInfoBox = StaticFindMobClassInfoBox();
//		if (MobClassInfoBox)
//		{
//			if (NPCClassEnum == ENPCClass::ENC_Reserve01 || NPCClassEnum == ENPCClass::ENC_Reserve02 || NPCClassEnum == ENPCClass::ENC_Reserve03)
//			{
//#if !PLATFORM_MAC
//				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//					FString::Printf(TEXT("[Warning] ENC_Reserve# �� �� �ʿ��� ��Ȳ������ �ӽ������� ����մϴ�!"))
//				));
//#endif
//			}
//
//			MobClassInfoBox->CheckAllNPCClassEnumSettingValid();
//		}
//	}
//	if (PropertyName == Name_ClassVariation)
//	{
//		if (ClassVariation == ENPCClassVariation::ENCV_CounterDungeon)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[���] ENCV_CounterDungeon �� �� �̻� �������� �ʽ��ϴ�.\r\n�ٸ� �ɷ� �ٲ��ּ���."),
//				*GetName())
//			));
//#endif
//		}
//	}
//
//	if (PropertyName == Name_UsePreSpawnState || PropertyName == Name_BehaviorTreeOverride)
//	{
//		UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
//		UBehaviorTree* DefaultSpawnBT = SomeInfo ? SomeInfo->GetDefaultNPCClassInfoSpawnBT() : NULL;
//
//		// bUsePreSpawnState ���� �� BT �� bUsePreSpawnState �� �⺻���� �ƴ� �� ���� ������ �Ǿ� �ִٸ� ���. ���� �̰� �����ϴ� �ٸ� �� ���� ���� ������.
//		if (bUsePreSpawnState && BehaviorTreeOverride && BehaviorTreeOverride != DefaultSpawnBT)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[���] UsePreSpawnState �� �ش� ����� �����ϴ� AI BehaviorTree �� �ʿ��մϴ�.\n���� UsePreSpawnState �� ���õ� Class %d Variation %d �� �ش� ����� ���� �غ�� �⺻ BehaviorTree �� �ٸ� BehaviorTree �� �����Ǿ� �ֽ��ϴ�.\n\n���� ���� ������ Ȯ���� �� ���ٸ� UsePreSpawnState �� ���õ� Variation �� BehaviorTreeOverride �׸��� ��� �δ� ���� �����մϴ�."),
//				(int32)NPCClassEnum, (int32)ClassVariation)
//			));
//#endif
//		}
//	}
}
#endif