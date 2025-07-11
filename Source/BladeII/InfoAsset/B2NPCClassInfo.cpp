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

	//// NPCClassInfo 로 override 하기 전 BaseBP override 부터. 이 시점은 대체로 생성은 했으나 PostSpawnInitialize 를 하지 않은 시점이 될 것.
	//this->OverrideByBaseBP(InClassInfo->GetBaseBPClass(this));

	//this->NPCClassEnum = InClass;
	//this->NPCClassVariation = InClassInfo->ClassVariation;
	//
	//// CharacterType 은 그대로 덮어씌워짐. 
	//// 여기서 CharacterType 을 설정했다 하더라도 MonsterSpawnPool 에서 쓰이는 MonsterClassTypeMap 에서는 다른 type 에 맵핑이 될 수 있으며 해당 정보를 사용해서 spawn 되고 나면 그에 맞게 CharacterType 이 다시 세팅될 것.
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
	//if (InClassInfo->DamageNumZOffsetOverride != 0.0f) // - 값도 들어갈 수 있게..
	//{
	//	this->DamageNumZOffset = InClassInfo->DamageNumZOffsetOverride;
	//}

	//if (InClassInfo->bUsePreSpawnState)
	//{
	//	// 이것은 AI BT 쪽에서 DetectDistance 내에 플레이어 캐릭터가 감지되면 false 가 될 것. 이것이 세팅되지 않은 BT 가 사용되면 초기 상태에서 벗어나지 못하게 된다.
	//	// AnimBP 틱에서 처음부터 이걸 체크하므로 AnimBP 세팅 (SetAnimInstanceClass) 전에 해야 함.
	//	this->bNeedSpawnCheck = true;
	//	//this->CustomTimeDilation = 0.0f; -> 이 방식을 지원하게 될지는 모르겠다. 아직은 note..
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

	//USkeletalMeshComponent* MeshComp = this->GetBaseMesh();  // NPC 들은 특수한 attachment 외에 그 기본이 multi-component 구성이 아닌 한은 BaseMesh 만 사용.

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

	//		// AnimBP 바꿔치기 전에 SetSkeletalMesh 부터
	//		MeshComp->SetSkeletalMesh(InClassInfo->MeshAssetOverride);
	//		if (InClassInfo->AnimBPOverride)
	//		{// 이건 결국 MeshComp->SetAnimInstanceClass 임. 모바일 디바이스 디버깅 과정의 유산
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
	//		// MeshRelativeOffset 의 Z 는 capsule 의 half height 에 추가로 적용
	//		MeshComp->SetRelativeLocation(FVector(InClassInfo->MeshRelativeOffset.X, InClassInfo->MeshRelativeOffset.Y, -1.0f * CollisionCapsule->GetScaledCapsuleHalfHeight() + InClassInfo->MeshRelativeOffset.Z));
	//	}

	//	// SkeletalMeshComponent 에 붙일 추가 컴포넌트들
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
	//				NewAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh 를 Character 에 붙이니 Movable 로
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

	//	// 로테이션속도 덮어쓰기(입력값이 0미만이면 덮어쓰기 안함)
	//	// 덮어쓰기 안하면 UB2SomeInfo - DefaultNPCClassInfoBaseBPClass 설정값 이용함.
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
	//	{ // UsePreSpawnState 는 AI BT 쪽에서 플레이어 감지해서 해제하는 게 필요함. 딱히 override 안되어 있으면 기본 SpawnBT 를 사용.
	//		this->SetBehaviorTree(SomeInfo->GetDefaultNPCClassInfoSpawnBT());
	//	}
	//	else
	//	{ // 완전 기본 BT
	//		this->SetBehaviorTree(SomeInfo->GetDefaultNPCClassInfoMeleeBT());
	//	}
	//}

	//// 데미지모션으로인한 회전 무시
	//if (InClassInfo->bIgnoreRotateByDamage)
	//	this->SetIgnoreRotateByDamage();

	//float HPScale, AttackDefenseLevelScale, MaxArmorOverride;
	//BladeIIGameImpl::GetClientDataStore().GetMonsterGradeFactor(InClass, InClassInfo, HPScale, AttackDefenseLevelScale, MaxArmorOverride);
	//
	//this->SetFinalADScale(AttackDefenseLevelScale);
	//this->SetFinalHPScale(HPScale);

	//ABladeIIGameMode* B2GM = GetBladeIIGameMode(this);
	//// 현재 스테이지 난이도 수치는 캐릭터(몹) 전투력에 반영된다.
	//if (B2GM)
	//	this->CharacterLevel = B2GM->GetCurrentStageDifficultyScale();

	//// CharacterLevel 에 기반하여 여타 다른 전투 능력치들을 계산하므로 갱신해 줌. 근데 어차피 ABladeIICharacter::BeginPlay 에서 함.
	//this->InitializeCombatStats();

	//if (MaxArmorOverride > 0.0f)
	//{
	//	// Armor 값은 따로임. InitializeCombatStats 에서 CharacterLevel 을 기반으로 계산된다면 여기는 없애야 함.
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

	//// 이것이 불리는 시점은 NPCClassInfo 적용이나 BeginPlay 이전임.
	//// 리소스 레퍼런스 세팅의 경우 세팅 후 실제 사용 시점이 언제인지 확인할 필요가 있음.

	//// 하위 컴포넌트들은 따로 처리해야 한다. this 대상의 CopyObjectProperties 에서 하위 컴포넌트들을 복사하게 하면 컴포넌트 자체를 그대로 가져와 사용하므로 안됨.

	//CopyObjectProperties(this->GetMovementComponent(), BPCDO->GetMovementComponent(), true, true);
	//CopyObjectProperties(this->CenterShadow, BPCDO->CenterShadow, true, true);

	//// SkeletalMeshComponent 는 좀 문제의 소지가 있어서 직접 속성들 지정해서 수동으로 복사. 굳이 CopyObjectProperties 를 사용하려면 제외할 속성 이름 필터를 지정할 수는 있다.
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

	//	// [모험모드] 에서는 카메라 프러스텀을 벗어나더라도 항상 업데이트를 하도록 한다
	//	if ( EB2GameMode::Scenario == GetB2GameModeType( this ) )
	//	{
	//		ThisSKMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//	}
	//}

	//CopyObjectProperties(this->GetCapsuleComponent(), BPCDO->GetCapsuleComponent(), true, true);

	//// 자기 자신은 맨 나중에 처리해 본다. 여기엔 제외시키는 속성값들이 좀 있는데 MinimalDLCFrontMode 로 인해 초반에 BaseBP 에서 로딩 안된 값들을 사용하면 안되기 때문.
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
 * NPCClass, Variation 단위의 클라이언트 데이터 정의
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
	//// 설정된 것이 없다면 추천 값으로.
	//SensitiveDataRowKey = UB2NPCSensitiveInfo::SuggestSensitiveInfoDataRowKey(NPCClassEnum, ClassVariation);
}

void UB2NPCSingleClassInfo::GetAllExpectedPhaseClasses(TArray<FCombinedNPCClassID>& OutClassIDs)
{
	//// Phase 클래스는 별도로 식별해서 로딩해 놓지 않으면 플레이 도중 로딩 랙을 유발하는 요소가 됨.
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
	//FString ExpectedFullName = ENPCCLASSENUM_COMMON_PREFIX + InTrimmedName; // InTrimmedName 은 prefix 빠진 걸로 간주

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
	FString ExpectedFullName = ENPCCLASSVARIENUM_COMMON_PREFIX + InTrimmedName; // InTrimmedName 은 prefix 빠진 걸로 간주

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASSVARI_ENUM_PACKAGE_NAME, true);
	//if (EnumPtr)
	//{
	//	for (int32 EI = 0; EI <= (int32)ENPCClassVariation::ENCV_ControlBattle; ++EI) // 이건 End 가 없군.. 실수로 누락된다고 해 봐야 에디터 테스트 기능이니.
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
//		// 여기선 MobClassInfoBox 만 찾아서 검색하는데, 만일 다른 NPCClassInfoBox 가 추가되면 그것도 검색해야 함.
//		UB2NPCClassInfoBox* MobClassInfoBox = StaticFindMobClassInfoBox();
//		if (MobClassInfoBox)
//		{
//			if (NPCClassEnum == ENPCClass::ENC_Reserve01 || NPCClassEnum == ENPCClass::ENC_Reserve02 || NPCClassEnum == ENPCClass::ENC_Reserve03)
//			{
//#if !PLATFORM_MAC
//				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//					FString::Printf(TEXT("[Warning] ENC_Reserve# 는 꼭 필요한 상황에서만 임시적으로 사용합니다!"))
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
//				FString::Printf(TEXT("[경고] ENCV_CounterDungeon 은 더 이상 지원하지 않습니다.\r\n다른 걸로 바꿔주세요."),
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
//		// bUsePreSpawnState 세팅 시 BT 가 bUsePreSpawnState 용 기본값이 아닌 게 직접 세팅이 되어 있다면 경고. 물론 이걸 지원하는 다른 게 있을 수도 있지만.
//		if (bUsePreSpawnState && BehaviorTreeOverride && BehaviorTreeOverride != DefaultSpawnBT)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[경고] UsePreSpawnState 는 해당 기능을 지원하는 AI BehaviorTree 가 필요합니다.\n현재 UsePreSpawnState 가 세팅된 Class %d Variation %d 에 해당 기능을 위해 준비된 기본 BehaviorTree 와 다른 BehaviorTree 가 설정되어 있습니다.\n\n만일 현재 설정을 확신할 수 없다면 UsePreSpawnState 가 세팅된 Variation 의 BehaviorTreeOverride 항목을 비워 두는 것을 권장합니다."),
//				(int32)NPCClassEnum, (int32)ClassVariation)
//			));
//#endif
//		}
//	}
}
#endif