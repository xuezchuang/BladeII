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

	//// NPCClassInfo 肺 override 窍扁 傈 BaseBP override 何磐. 捞 矫痢篮 措眉肺 积己篮 沁栏唱 PostSpawnInitialize 甫 窍瘤 臼篮 矫痢捞 瞪 巴.
	//this->OverrideByBaseBP(InClassInfo->GetBaseBPClass(this));

	//this->NPCClassEnum = InClass;
	//this->NPCClassVariation = InClassInfo->ClassVariation;
	//
	//// CharacterType 篮 弊措肺 丹绢竞况咙. 
	//// 咯扁辑 CharacterType 阑 汲沥沁促 窍歹扼档 MonsterSpawnPool 俊辑 静捞绰 MonsterClassTypeMap 俊辑绰 促弗 type 俊 甘俏捞 瞪 荐 乐栏哥 秦寸 沥焊甫 荤侩秦辑 spawn 登绊 唱搁 弊俊 嘎霸 CharacterType 捞 促矫 技泼瞪 巴.
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
	//if (InClassInfo->DamageNumZOffsetOverride != 0.0f) // - 蔼档 甸绢哎 荐 乐霸..
	//{
	//	this->DamageNumZOffset = InClassInfo->DamageNumZOffsetOverride;
	//}

	//if (InClassInfo->bUsePreSpawnState)
	//{
	//	// 捞巴篮 AI BT 率俊辑 DetectDistance 郴俊 敲饭捞绢 某腐磐啊 皑瘤登搁 false 啊 瞪 巴. 捞巴捞 技泼登瘤 臼篮 BT 啊 荤侩登搁 檬扁 惑怕俊辑 哈绢唱瘤 给窍霸 等促.
	//	// AnimBP 平俊辑 贸澜何磐 捞吧 眉农窍骨肺 AnimBP 技泼 (SetAnimInstanceClass) 傈俊 秦具 窃.
	//	this->bNeedSpawnCheck = true;
	//	//this->CustomTimeDilation = 0.0f; -> 捞 规侥阑 瘤盔窍霸 瞪瘤绰 葛福摆促. 酒流篮 note..
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

	//USkeletalMeshComponent* MeshComp = this->GetBaseMesh();  // NPC 甸篮 漂荐茄 attachment 寇俊 弊 扁夯捞 multi-component 备己捞 酒囱 茄篮 BaseMesh 父 荤侩.

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

	//		// AnimBP 官层摹扁 傈俊 SetSkeletalMesh 何磐
	//		MeshComp->SetSkeletalMesh(InClassInfo->MeshAssetOverride);
	//		if (InClassInfo->AnimBPOverride)
	//		{// 捞扒 搬惫 MeshComp->SetAnimInstanceClass 烙. 葛官老 叼官捞胶 叼滚彪 苞沥狼 蜡魂
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
	//		// MeshRelativeOffset 狼 Z 绰 capsule 狼 half height 俊 眠啊肺 利侩
	//		MeshComp->SetRelativeLocation(FVector(InClassInfo->MeshRelativeOffset.X, InClassInfo->MeshRelativeOffset.Y, -1.0f * CollisionCapsule->GetScaledCapsuleHalfHeight() + InClassInfo->MeshRelativeOffset.Z));
	//	}

	//	// SkeletalMeshComponent 俊 嘿老 眠啊 哪器惩飘甸
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
	//				NewAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh 甫 Character 俊 嘿捞聪 Movable 肺
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

	//	// 肺抛捞记加档 丹绢静扁(涝仿蔼捞 0固父捞搁 丹绢静扁 救窃)
	//	// 丹绢静扁 救窍搁 UB2SomeInfo - DefaultNPCClassInfoBaseBPClass 汲沥蔼 捞侩窃.
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
	//	{ // UsePreSpawnState 绰 AI BT 率俊辑 敲饭捞绢 皑瘤秦辑 秦力窍绰 霸 鞘夸窃. 迭洒 override 救登绢 乐栏搁 扁夯 SpawnBT 甫 荤侩.
	//		this->SetBehaviorTree(SomeInfo->GetDefaultNPCClassInfoSpawnBT());
	//	}
	//	else
	//	{ // 肯傈 扁夯 BT
	//		this->SetBehaviorTree(SomeInfo->GetDefaultNPCClassInfoMeleeBT());
	//	}
	//}

	//// 单固瘤葛记栏肺牢茄 雀傈 公矫
	//if (InClassInfo->bIgnoreRotateByDamage)
	//	this->SetIgnoreRotateByDamage();

	//float HPScale, AttackDefenseLevelScale, MaxArmorOverride;
	//BladeIIGameImpl::GetClientDataStore().GetMonsterGradeFactor(InClass, InClassInfo, HPScale, AttackDefenseLevelScale, MaxArmorOverride);
	//
	//this->SetFinalADScale(AttackDefenseLevelScale);
	//this->SetFinalHPScale(HPScale);

	//ABladeIIGameMode* B2GM = GetBladeIIGameMode(this);
	//// 泅犁 胶抛捞瘤 抄捞档 荐摹绰 某腐磐(各) 傈捧仿俊 馆康等促.
	//if (B2GM)
	//	this->CharacterLevel = B2GM->GetCurrentStageDifficultyScale();

	//// CharacterLevel 俊 扁馆窍咯 咯鸥 促弗 傈捧 瓷仿摹甸阑 拌魂窍骨肺 盎脚秦 淋. 辟单 绢瞒乔 ABladeIICharacter::BeginPlay 俊辑 窃.
	//this->InitializeCombatStats();

	//if (MaxArmorOverride > 0.0f)
	//{
	//	// Armor 蔼篮 蝶肺烙. InitializeCombatStats 俊辑 CharacterLevel 阑 扁馆栏肺 拌魂等促搁 咯扁绰 绝局具 窃.
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

	//// 捞巴捞 阂府绰 矫痢篮 NPCClassInfo 利侩捞唱 BeginPlay 捞傈烙.
	//// 府家胶 饭欺繁胶 技泼狼 版快 技泼 饶 角力 荤侩 矫痢捞 攫力牢瘤 犬牢且 鞘夸啊 乐澜.

	//// 窍困 哪器惩飘甸篮 蝶肺 贸府秦具 茄促. this 措惑狼 CopyObjectProperties 俊辑 窍困 哪器惩飘甸阑 汗荤窍霸 窍搁 哪器惩飘 磊眉甫 弊措肺 啊廉客 荤侩窍骨肺 救凳.

	//CopyObjectProperties(this->GetMovementComponent(), BPCDO->GetMovementComponent(), true, true);
	//CopyObjectProperties(this->CenterShadow, BPCDO->CenterShadow, true, true);

	//// SkeletalMeshComponent 绰 粱 巩力狼 家瘤啊 乐绢辑 流立 加己甸 瘤沥秦辑 荐悼栏肺 汗荤. 被捞 CopyObjectProperties 甫 荤侩窍妨搁 力寇且 加己 捞抚 鞘磐甫 瘤沥且 荐绰 乐促.
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

	//	// [葛氰葛靛] 俊辑绰 墨皋扼 橇矾胶乓阑 哈绢唱歹扼档 亲惑 诀单捞飘甫 窍档废 茄促
	//	if ( EB2GameMode::Scenario == GetB2GameModeType( this ) )
	//	{
	//		ThisSKMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//	}
	//}

	//CopyObjectProperties(this->GetCapsuleComponent(), BPCDO->GetCapsuleComponent(), true, true);

	//// 磊扁 磊脚篮 盖 唱吝俊 贸府秦 夯促. 咯扁浚 力寇矫虐绰 加己蔼甸捞 粱 乐绰单 MinimalDLCFrontMode 肺 牢秦 檬馆俊 BaseBP 俊辑 肺爹 救等 蔼甸阑 荤侩窍搁 救登扁 锭巩.
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
 * NPCClass, Variation 窜困狼 努扼捞攫飘 单捞磐 沥狼
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
	//// 汲沥等 巴捞 绝促搁 眠玫 蔼栏肺.
	//SensitiveDataRowKey = UB2NPCSensitiveInfo::SuggestSensitiveInfoDataRowKey(NPCClassEnum, ClassVariation);
}

void UB2NPCSingleClassInfo::GetAllExpectedPhaseClasses(TArray<FCombinedNPCClassID>& OutClassIDs)
{
	//// Phase 努贰胶绰 喊档肺 侥喊秦辑 肺爹秦 初瘤 臼栏搁 敲饭捞 档吝 肺爹 发阑 蜡惯窍绰 夸家啊 凳.
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
	//FString ExpectedFullName = ENPCCLASSENUM_COMMON_PREFIX + InTrimmedName; // InTrimmedName 篮 prefix 狐柳 吧肺 埃林

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
	FString ExpectedFullName = ENPCCLASSVARIENUM_COMMON_PREFIX + InTrimmedName; // InTrimmedName 篮 prefix 狐柳 吧肺 埃林

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ENPCCLASSVARI_ENUM_PACKAGE_NAME, true);
	//if (EnumPtr)
	//{
	//	for (int32 EI = 0; EI <= (int32)ENPCClassVariation::ENCV_ControlBattle; ++EI) // 捞扒 End 啊 绝焙.. 角荐肺 穿遏等促绊 秦 毫具 俊叼磐 抛胶飘 扁瓷捞聪.
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
//		// 咯扁急 MobClassInfoBox 父 茫酒辑 八祸窍绰单, 父老 促弗 NPCClassInfoBox 啊 眠啊登搁 弊巴档 八祸秦具 窃.
//		UB2NPCClassInfoBox* MobClassInfoBox = StaticFindMobClassInfoBox();
//		if (MobClassInfoBox)
//		{
//			if (NPCClassEnum == ENPCClass::ENC_Reserve01 || NPCClassEnum == ENPCClass::ENC_Reserve02 || NPCClassEnum == ENPCClass::ENC_Reserve03)
//			{
//#if !PLATFORM_MAC
//				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//					FString::Printf(TEXT("[Warning] ENC_Reserve# 绰 怖 鞘夸茄 惑炔俊辑父 烙矫利栏肺 荤侩钦聪促!"))
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
//				FString::Printf(TEXT("[版绊] ENCV_CounterDungeon 篮 歹 捞惑 瘤盔窍瘤 臼嚼聪促.\r\n促弗 吧肺 官层林技夸."),
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
//		// bUsePreSpawnState 技泼 矫 BT 啊 bUsePreSpawnState 侩 扁夯蔼捞 酒囱 霸 流立 技泼捞 登绢 乐促搁 版绊. 拱沸 捞吧 瘤盔窍绰 促弗 霸 乐阑 荐档 乐瘤父.
//		if (bUsePreSpawnState && BehaviorTreeOverride && BehaviorTreeOverride != DefaultSpawnBT)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[版绊] UsePreSpawnState 绰 秦寸 扁瓷阑 瘤盔窍绰 AI BehaviorTree 啊 鞘夸钦聪促.\n泅犁 UsePreSpawnState 啊 技泼等 Class %d Variation %d 俊 秦寸 扁瓷阑 困秦 霖厚等 扁夯 BehaviorTree 客 促弗 BehaviorTree 啊 汲沥登绢 乐嚼聪促.\n\n父老 泅犁 汲沥阑 犬脚且 荐 绝促搁 UsePreSpawnState 啊 技泼等 Variation 狼 BehaviorTreeOverride 亲格阑 厚况 滴绰 巴阑 鼻厘钦聪促."),
//				(int32)NPCClassEnum, (int32)ClassVariation)
//			));
//#endif
//		}
//	}
}
#endif