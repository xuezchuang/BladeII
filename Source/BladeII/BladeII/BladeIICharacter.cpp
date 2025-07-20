// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "BladeIICharacter.h"
#include "BladeIIUtil.h"
#include "BladeIIGameMode.h"
#include "../InfoAsset/B2NPCClassInfoBox.h"
#include "BladeIIGameMode.h"
#include "B2StageGameMode.h"
#include "B2DimensionGameMode.h"
#include "B2StageInfo.h"
#include "B2StageManager.h"
#include "Components/ArrowComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "B2FloatingHPBar.h"
#include "B2FloatingDamageNum.h"
#include "B2FloatingAbnormalEffect.h"
#include "B2UIManager_InGameCombat.h"
#include "B2MonsterSpawnPool.h"
#include "B2StageEventDirector.h"
#include "Animation/SkeletalMeshActor.h"
#include "Engine/SkeletalMesh.h"
#include "Particles/ParticleSystem.h"
#include "B2DamageEffectInfo.h"
#include "B2BuffModeEffectInfo.h"
#include "BladeIIProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Event.h"
#include "NavigationSystem.h"
#include "B2AreaDamageActorBase.h"
#include "B2UIDocHelper.h"
#include "B2CombatStatEvaluator.h"
#include "BladeIIAIController.h"
#include "Trigger.h"
#include "B2CounterAttackGameMode.h"
#include "B2PCClassInfoBox.h"
#include "B2NoAnimSkillInfo.h"
#include "B2CharacterBuffManager.h"
#include "B2GeneralBuffs.h"
//#include "B2NPCClassInfo.h"
#include "BladeIIPlayerController.h"
//#include "B2AutoAIController.h"
#include "B2AnimInstance.h"
#include "B2GameInstance.h"
//#include "B2AIUtil.h"
//#include "Components/DecalComponent.h"
//
//#ifdef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//#include "BladeIITestDummyNPC.h"
//#endif
////#include "B2CharacterBuffManager.h"
#include "B2FloatingHPBar.h"

DEFINE_LOG_CATEGORY_STATIC(LogB2Character, Log, All);

UParticleSystem* FCHSKCompAttachParticleSystemInfo::GetParticleSystemAsset()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(FCHSKCompAttachParticleSystemInfo_GetParticleSystemAsset);
	if (!ParticleSystemAsset.IsValid())
	{
		ParticleSystemAsset.LoadSynchronous();
	}
	return ParticleSystemAsset.Get();
}

UParticleSystemComponent* FCHSKCompAttachParticleSystemInfo::CreatePSCHelper(AActor* InCompOwner, USkeletalMeshComponent* InParentComp)
{
	////BLADE2_SCOPE_CYCLE_COUNTER(FCHSKCompAttachParticleSystemInfo_CreatePSCHelper);
	//// 咯扁 Info 悸诀 荤侩秦辑 PSC 积己 棺 attach 窍绰 扁夯 例瞒.
	//if (InCompOwner && InParentComp)
	//{
	//	// InParentComp 绰 InCompOwner 俊 器窃等 吧肺 啊沥阑..
	//	checkSlow(InParentComp->IsAttachedTo(InCompOwner->GetRootComponent()) || InParentComp == InCompOwner->GetRootComponent());

	//	UParticleSystem* LoadedPS = GetParticleSystemAsset();
	//	if (LoadedPS)
	//	{
	//		UParticleSystemComponent* CreatedComp = NewObject<UParticleSystemComponent>(InCompOwner, FName(*GetCompCreateName(InCompOwner)), RF_Transient);
	//		if (CreatedComp)
	//		{
	//			CreatedComp->bReceivesDecals = false;
	//			//CreatedComp->bIgnoreBoneRotation = bIgnoreBoneRotation;
	//			CreatedComp->SetTemplate(LoadedPS);
	//			CreatedComp->SetRelativeScale3D(AttachScale3D);
	//			CreatedComp->SetRelativeLocation(LocationOffset);
	//			CreatedComp->SetRelativeRotation(RotationOffset);
	//			CreatedComp->AttachToComponent(InParentComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), AttachSocketName);
	//			CreatedComp->RegisterComponent();
	//			CreatedComp->Activate(true);
	//			return CreatedComp;
	//		}
	//	}
	//}
	return NULL;
}

#if WITH_EDITOR
void FCHSKCompAttachParticleSystemInfo::EditorClearPSAsset()
{ // 捞芭 技泼窍瘤 富酒具 且 镑俊辑 技泼窍搁 PostEdit 俊辑 力芭窍妨绰 芭.
	ParticleSystemAsset.Reset();
}
#endif

bool ABladeIICharacter::CanTakeEtherEffect(AActor* EffectCauser)
{
	bool Result = true;

	if (Health <= 0.f)
	{
		Result = false;
	}
	else if (IsControlledByQTEPlayer())
	{
		Result = false;
	}
	else if (bSpawningNoDamageGuard) // 措眉肺 SpawnPool 俊辑 Spawn 矫虐搁辑 殿废窍扁 傈.
	{
		Result = false;
	}
	else if (!IsEnemy(EffectCauser))
	{
		Result = false;
	}
	else if (IsBuffActive(EBuffType::Buff_Hide) || IsBuffActive(EBuffType::Buff_Unbeatable))
	{
		Result = false;
	}
	else if (bInvincible)
	{
		Result = false;
	}
	else if (!CanBeDamaged())
	{
		Result = false;
	}

	return Result;
}

float ABladeIICharacter::FreezeTimeDilation = 0.0f;
float ABladeIICharacter::MinTimeDilation = 0.05f;

TArray<FName> ABladeIICharacter::BaseBPCopyExcludeProperties;

ABladeIICharacter::ABladeIICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NPCClassEnum = ENPCClass::ENC_End;
	NPCClassVariation = ENPCClassVariation::ENCV_Normal;
	//TODO : Init Data for Character Type
	CharacterType = ECharacterType::ECT_MeleeMob;
	//Need to set other data
	AttackState = EAttackState::ECS_None;
	bSignalAttack = false;
	Health = 100.f;
	MaxHealth = 100.f;
	Armor = 0.f;
	MaxArmor = 0.f;
	DamagedNum = 0;
	LastDamageType = EAttackType::EAT_Default;
	LastStateDamageType = EStateAttackType::EDT_Normal;
	QTEPCClassEnum = EPCClass::EPC_End;
	CurrentOverridenMatType = ECharacterMaterialOverrideType::ECMO_None;
	bAbleToMove = true;
	bMovementPreventByStageEvent = false;
	CachedTimeDilation = 1.f;
	B2TimeDilationForAnimNS = 1.f;
	B2CachedDilationForAnimNS = 1.f;

	FinalADScale = 1.0f;
	FinalHPScale = 1.0f;

	StageEventShowState = EStageEventShowState::ESES_None;
	bPlayingDirectedStageEvent = false;

	DetectDistance = 800.f;

	CameraTargetBoomRotOffset = FRotator(0.0f, 0.0f, 0.0f);

	AIAttackIndex = 0;

	CharacterLevel = 1;
	CharacterDamageRate = 100;

	bNeedSpawnCheck = false; // 檬扁 spawn state 甫 荤侩窍绰 版快 true 肺 难林绊, AI 俊 狼秦 false 肺 府悸登搁 框流捞扁 矫累.

	bForceSpawAnim = false;

	DamageMultiplier = 1.f;
	BaseDamageRate = 1.f;
	ReflectDamageAmount = 1.f;
	HealHPByAttackRate = 0.f;
	StateAttackType = EStateAttackType::EDT_End;
	StateAttackDuration = 0.f;
	StateAttackAmount = 0.f;

	ShadowMaterial = NULL;
	ShadowSizeCenter = 120.f;
	ShadowMaxDistance = 50.f;

	DamageNumZOffset = 0.0f;

	CenterShadow = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("CenterShadowComponent"));
	CenterShadow->SetUsingAbsoluteLocation(true);
	CenterShadow->SetUsingAbsoluteRotation(true);
	CenterShadow->SetRelativeLocation_Direct(FVector::ZeroVector);
	CenterShadow->SetRelativeScale3D_Direct(FVector(200.f + ShadowMaxDistance, ShadowSizeCenter, ShadowSizeCenter));
	CenterShadow->SetRelativeRotation_Direct(FRotator(-90.f, 0.f, 0.f));
	CenterShadow->DecalSize = FVector(1.f, 1.f, 1.f); // for 4.11
	//CenterShadow->bHiddenInGame = true;
	CenterShadow->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	CenterShadow->Deactivate();

	//Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//Rotate character to camera direction for AIPlayerController
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	//GetCharacterMovement()->bForceMaxAccel = true;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
	//GetCharacterMovement()->bIgnoreForceDuringRootmotion = false;

	if (!GMinimalDLCFrontMode)
	{ // DLC Front 葛靛 府家胶肺爹 弥措茄 力芭

////////////////////////////////////////////////////////////////////////////////
//>>> Widget classes specification begin

		FString DefaultFloatingHPBarWidgetClassPath;
		GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultFloatingHPBarWidgetClass"), DefaultFloatingHPBarWidgetClassPath, GGameIni);
		static ConstructorHelpers::FClassFinder<UB2FloatingHPBar> DefaultFloatingHPBarWidgetClass(*DefaultFloatingHPBarWidgetClassPath);
		if (DefaultFloatingHPBarWidgetClass.Class != NULL)
		{
			FloatingHPBarWidgetClass = DefaultFloatingHPBarWidgetClass.Class;
			BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("FloatingHPBarWidgetClass"))); // MinimalDLCFrontMode 肺 牢秦 厚绢乐阑 BaseBP 率 蔼阑 啊廉坷瘤 臼扁 困秦 捞抚 眠啊
		}
	}

	DamageSocketName = FName(TEXT("S_hit01"));

	bNeedOscillation = false;
	DeferredImpulse = FVector::ZeroVector;

	// Disable receive decal
	GetMesh()->bReceivesDecals = false;
	GetMesh()->bPerBoneMotionBlur = false;
	GetMesh()->bEnablePhysicsOnDedicatedServer = false;
	GetMesh()->CanCharacterStepUpOn = ECB_No;

	// Per Mesh AnimNotify State Data
	B2CharEmissiveRemainTime = 0.f;
	ChainBitField = 0;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TemporaryMeshCompUpdateFlagBackUp = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones; // 捞霸 酒付档 啊厘 救傈茄 蔼老 淀. 拱沸 捞霸 弊措肺 利侩登绰 扒 酒丛.
	bTemporarilySetMeshCompUpdateFlag = false;

	RegenerateHPPercentagePerSecond = 0.f;
	RegenerateHPAmountPerSecond = 0.0f;
	bRegenerateHPByPercent = true;

	for (int32 i = static_cast<int32>(EDamageElement::EDE_None); i < static_cast<int32>(EDamageElement::EDE_End); ++i)
	{
		CachedStateMaterials.Add(NULL);
	}

	CachedBaseMaxWalkSpeed = 1000.f;

	CachedStateDamageStates.AddZeroed(GetStateAttackTypeArrayNum());

	RandDamageAnimType = EAttackType::EAT_Default;

	QTEType = EQTEType::EQT_End;
	QTEInvokeHPPercent = 0.0f;
	QTEMountTime = 20.f;
	QTEEnableTime = 5.f;
	QTELimitEnable = false;

	TeamNum = -1;

	QTEEnableTeam = ETeamType::ETT_End;

	bCommittedSuicideAtDeadEnd = false;
	bIsDeadSinking = false;
	DeadSinkZOffsetPerSecond = 0.f;
	DeadSinkLeftTime = 0.f;

	bSpawningNoDamageGuard = false; // 葛电 版快俊 贸澜何磐 true 看促啊 spawn 贸府 饶 false 肺 滴绰 霸 酒聪扼 老何狼 版快父 泪矫埃 true 肺 荤侩.

	bForceReceivdAbsoluteDamage = false;
	bForceReceivdAttackCountDamage = false;
	FullAttactCountSize = 0;

	InitialInfluence = EInfluenceType::EIT_End;
	InitialInfluenceAmount = 0.f;
	InitialInfluenceDuration = 0.f;
	InitialInfluenceStateDuration = 0.f;
	InitialInfluenceStateTriggerRate = 0.f;

	InjectedGameRule = nullptr;

	nCanBeDamagedFalseCount = 0;

	bInvincible = false;
	m_bIsUndead = false;

	DamageReservationInfos.Empty();

	BuffManager = nullptr;

	OverridenMatTypeList.Empty();

	HPBarSocket = nullptr;
	bHPBarAttachedToSocket = false;

	SummonOwner = nullptr;

	bHideFromEventDirector = false;

	bIgnoreRotateByDamage = false;
	bEmissiveActiveByBuff = false;

	bAllowRecoilAnimation = true;

	bBloodStoneAura = false;
	BloodStoneType = EBloodStoneType::EBST_UnBeatable;
	BloodStoneAmount = -1.f;
	BloodStoneBuffLogicType = EDamageLogicType::ELT_Normal;

	SpawnNotifyMsgString = TEXT("");
	SpawnNotifyMsgTime = -1.f;
}

float ABladeIICharacter::GetMaxHealth() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetMaxHealth);
	return MaxHealth;
}

void ABladeIICharacter::RecoverArmor()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_RecoverArmor);
	bHadRecoverArmor = true;
	Armor = GetMaxArmor();
}

void ABladeIICharacter::AllowMovement()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_AllowMovement);
	if (!bMovementPreventByStageEvent) // bMovementPreventByStageEvent 狼 角龙利牢 侩档
	{
		bAbleToMove = true;
	}
}

void ABladeIICharacter::PreventMovement()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_PreventMovement);
	bAbleToMove = false;
}

void ABladeIICharacter::AllowMovementByStageEvent()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_AllowMovementByStageEvent);
	bMovementPreventByStageEvent = false;
	bAbleToMove = true; // 角力肺绰 捞吧 技泼秦 林绢具 窃.
}

void ABladeIICharacter::PreventMovementByStageEvent()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_PreventMovementByStageEvent);
	bMovementPreventByStageEvent = true;
	bAbleToMove = false; // 角力肺绰 捞吧 技泼秦 林绢具 窃.
}

bool ABladeIICharacter::ConsumeForceCancelAttackMotion()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ConsumeForceCancelAttackMotion);
	bool RtnValue = bForceCancelAttackTriggered;
	bForceCancelAttackTriggered = false;

	return RtnValue;
}

void ABladeIICharacter::MakeSlow(float _Duration)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_MakeSlow);
	CustomTimeDilation = _Duration;
	//GetWorldTimerManager().SetTimer(SetSlowTimerHandle, _Duration, false);
}

void ABladeIICharacter::SetMinDrawDistance(float _distance)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetMinDrawDistance);
	TArray<USceneComponent*> Components;
	GetRootComponent()->GetChildrenComponents(true, Components);
	for (USceneComponent* pComponent : Components)
	{
		UPrimitiveComponent* primitiveChild(Cast<UPrimitiveComponent>(pComponent));
		if (primitiveChild)
		{
			primitiveChild->bAllowCullDistanceVolume = true;
			primitiveChild->MinDrawDistance = _distance;

			if (UWorld* World = GetWorld())
			{
				World->UpdateCullDistanceVolumes(nullptr, primitiveChild);
			}
		}
		pComponent->MarkRenderStateDirty();
	}
	/*
	GetMesh()->MinDrawDistance = _distance;
	GetMesh()->MarkRenderStateDirty();
	*/
	//GetRootPrimitiveComponent()->MarkRenderStateDirty();
}

void ABladeIICharacter::HealingHPByPercent(float Percentage, bool bUseEffect, bool bUseTextEffect)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_HealingHPByPercent);
	// 甸绢坷绰 蔼篮 % 肺 钎矫登绰 窜困肺 啊沥. 0.01 胶纳老傅篮 咯扁辑 窃.
	float HealAmount = FMath::Min<float>(GetMaxHealth() * Percentage * 0.01f, GetMaxHealth() - GetHealth());
	HealingHPByAmount(HealAmount, bUseEffect, bUseTextEffect);
}

void ABladeIICharacter::HealingHPByAmount(float InHealAmount, bool bUseEffect, bool bUseTextEffect)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_HealingHPByAmount);
	//if (InHealAmount > 0.f && Health < MaxHealth)
	//{
	//	float ActualHealAmount = FMath::Min(InHealAmount, MaxHealth - Health);

	//	SetHealth(FMath::Min(Health + ActualHealAmount, MaxHealth));

	//	// 眉仿雀汗UI(单固瘤钎扁 捞亥飘俊 绢付款飘 -蔼阑林搁 雀汗钎扁牢淀)
	//	// 敲饭捞绢父 利侩

	//	bool bIgnoreHealEffect = false;
	//	if (auto* LocalChar = GetLocalCharacter())
	//		bIgnoreHealEffect = IsEnemy(LocalChar->GetTeamNum());

	//	if (IsApplyDamageNotifyUI(nullptr) && IsPlayerCharacter() && bIgnoreHealEffect == false)
	//	{
	//		//CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Signal(this, -ActualHealAmount, false);
	//	}

	//	if (bUseEffect || bUseTextEffect)
	//	{
	///*		UB2BuffModeEffectInfo* BuffModeEffectInfo = GetBuffModeEffectInfo();
	//		if (BuffModeEffectInfo)
	//		{
	//			if (bUseEffect) BuffModeEffectInfo->CharacterBuffEffect(EBuffType::Buff_SkillHealHP, this);
	//			if (bUseTextEffect) BuffModeEffectInfo->SpawnCharacterBuffTextEffect(EBuffType::Buff_HealHPByAttack, this);
	//		}*/
	//	}
	//}
}

bool ABladeIICharacter::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_CanDie);
	//if (bIsDying										// already dying
	//	|| IsPendingKill())								// already destroyed
	//	//|| Role != ROLE_Authority						// not authority
	//	//|| GetWorld()->GetAuthGameMode() == NULL
	//	//|| GetWorld()->GetAuthGameMode()->GetMatchState() == MatchState::LeavingMap)				// level transition occurring
	//{
	//	return false;
	//}

	return true;
}

bool ABladeIICharacter::CanDieFromCounterDungeon()
{
	//if (this->CachedBirthplaceInfo.bSpawnedAsBossMob && GetB2GameModeType(GetWorld()) == EB2GameMode::CounterDungeon)
	//{
	//	auto* B2GameMode = Cast<AB2CounterAttackGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	if (B2GameMode && B2GameMode->SetFinishState(this))
	//	{
	//		AddBuff(EBuffType::Debuff_Stun, -1);
	//		SetRegenerateHPRate(0.f); // 馋促.
	//		return false;
	//	}
	//}

	return true;
}

bool ABladeIICharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_Die);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::Die"));

	//if (!CanDie(KillingDamage, DamageEvent, Killer, DamageCauser))
	//{
	//	return false;
	//}

	//if (!CanDieFromCounterDungeon())
	//{
	//	return false;
	//}

	//Health = FMath::Min(0.0f, Health);

	//// if this is an environmental death then refer to the previous killer so that they receive credit (knocked into lava pits, etc)
	//UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	//Killer = GetDamageInstigator(Killer, *DamageType);

	//NetUpdateFrequency = GetDefault<ABladeIICharacter>()->NetUpdateFrequency;
	//GetCharacterMovement()->ForceReplicationUpdate();

	//APawn* KillerPawn(Killer ? Killer->GetPawn() : nullptr);
	//auto* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (B2GM)
	//{
	//	// AnimNotifyState_B2MobStateUndead俊辑 m_bIsUndead = true肺 父惦(唱吝俊 磷绰 惑怕)
	//	if (m_bIsUndead == false)
	//		OnDeath(KillingDamage, DamageEvent, KillerPawn, DamageCauser);
	//	else
	//	{
	//		B2GM->OnReserveDeath().AddLambda([this, KillingDamage, DamageEvent, KillerPawn, DamageCauser]()
	//			{
	//				this->OnDeath(KillingDamage, DamageEvent, KillerPawn, DamageCauser);
	//				m_bIsUndead = false;
	//			});
	//	}
	//}
	//else //捞芭绰 捞惑 惑怕捞扁绰 窍唱 抗寇 贸府侩栏肺 眠啊
	//{
	//	OnDeath(KillingDamage, DamageEvent, KillerPawn, DamageCauser);
	//}


	return true;
}

void ABladeIICharacter::DropItem()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DropItem);
	//// Currently assumes drops are monster only, but consider if it is required someday..

	//if (GetB2GameModeType(GetWorld()) == EB2GameMode::Dimension)
	//{
	//	auto* B2StageGM = Cast<AB2DimensionGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	auto* StageManager = B2StageGM ? B2StageGM->GetStageManager() : NULL;

	//	if (StageManager)
	//		StageManager->RequestSpawnDropItem(DropItemInfo, this, GetActorLocation());
	//}
	//else
	//{
	//	auto* B2StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	auto* StageManager = B2StageGM ? B2StageGM->GetStageManager() : NULL;

	//	if (StageManager)
	//		StageManager->RequestSpawnDropItem(DropItemInfo, this, GetActorLocation());
	//}
}

void ABladeIICharacter::TriggerKillMonster()
{
	//KillMonsterTriggerClass<>::GetInstance().Signal();
	//KillSpecificMonsterTriggerClass<ABladeIICharacter*>::GetInstance().Signal(this);
}

void ABladeIICharacter::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_OnDeath);
#if !UE_BUILD_SHIPPING
	// 咯变 粱 歹 沥焊甫 拎辑 飘发欧.
	FString ScopedTrackString = FString::Printf(TEXT("ABladeIICharacter::OnDeath %s by %s"), *GetName(), PawnInstigator ? *PawnInstigator->GetName() : TEXT("Unknown"));
	B2_SCOPED_TRACK_LOG(ScopedTrackString);
#endif

	if (bIsDying)
	{
		return;
	}
	SetReplicateMovement(false);
	SetCanBeDamaged(false);

	TearOff();
	bIsDying = true; // Animation Blueprint 肺 焊郴绰 脚龋. 胶抛捞瘤 柳青捞唱 楷免 飘府芭 瞒盔俊辑狼 贸府绰 匙捞萍宏辑 窍瘤父 阁胶磐狼 傈捧 葛记阑 吝窜窍绊 dead 葛记阑 秒窍绰 扒 捞吧 烹秦 甸绢皑.
	SetQTEEnabled(false);
	bAbleToMove = false;

	if (QTEType != EQTEType::EQT_Mount)
		SetQTEStateFlag(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);

	if (DamageCauser)
	{
		FVector Diff = DamageCauser->GetActorLocation() - GetActorLocation();
		FRotator TargetRotation(0.f, FRotator::ClampAxis(FMath::Atan2(Diff.Y, Diff.X) * 180.f / PI), 0.f);
		SetActorRotation(TargetRotation);
	}

	DropItem();

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	// For PC, we might not want destroy InGameHUDWidget at this moment
	// 捞 矫痢俊辑 官肺 DestroyUIManager 甫 窍瘤 臼绰促. 恐衬窍搁 DamageNum 鞍篮 局甸捞 老沥 扁埃篮 巢酒乐绢具 窍扁 锭巩. 
	// 利寸洒 1檬 饶俊 阂府档废 鸥捞赣甫 汲摹窍绰单 弊 傈俊 颇鲍啊 登搁 Destroyed 俊辑 贸府啊 瞪 芭绊, 1檬啊 陋促搁.. 澜.. 弊扒 娟扁啊 甸绢坷搁 积阿.
	//bool ResurrectionGameModeType = B2GM && B2GM->GetUIManager_InGameCombat() && B2GM->GetUIManager_InGameCombat()->CanResurrectionGameModeType();

	//if (ResurrectionGameModeType == false)
	//{
	//	GetWorldTimerManager().SetTimer(DelayedDestroyUIManagerTH, this, &ABladeIICharacter::DelayedDestroyUIManagerCB, 1.0f, false);
	//	HideFloatingHPBar(true); // 措脚 HPBar 绰 官肺 救 焊捞霸
	//}


	ResetFlash();

	//if (Role == ROLE_Authority)
	//{
	//	ReplicateHit(KillingDamage, DamageEvent, PawnInstigator, DamageCauser, true);
	//}

	GetWorldTimerManager().ClearTimer(SetHitFlashTimerHandle);
	GetWorldTimerManager().ClearTimer(SetRegenerateHPHandle);
	GetWorldTimerManager().ClearTimer(SetDeadSinkHandle);
	GetWorldTimerManager().ClearTimer(OnQTEEnableEndTH);

	ConditionalReportMyDeathToBirthplace(); // 购啊 犁柳涝阑 窍绰 纳捞胶啊 乐促.. ぱぱ

	//CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Signal(this);

	//for (auto Key : EventCancelCastAreaDamageSubscribeKeys)
	//	CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Unsubscribe(Key);

	// 夯牢捞 磷菌促绊 霸烙葛靛俊 舅覆(乔聪浆 傍拜 楷免侩档肺 累己)
	// ReportMyDeathToBirthplace 俊辑 焊郴绰 烹瘤客 吝汗登绰 搁捞 乐绰单 泅犁肺急 开且捞 炼陛究 促抚.
	if (B2GM)
		//B2GM->NotifyCharacterDead(this, PawnInstigator, DamageCauser);

	TriggerKillMonster();

	//CharacterDeathClass<ABladeIICharacter*>::GetInstance().Signal(this);

	// 滚橇力芭
	//BuffManager->ClearAllBuffs();
}

void ABladeIICharacter::DieForQTE()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DieForQTE);
	//// QTE 鸥百栏肺 吧赴 各狼 荤噶 贸府. 老馆利栏肺 捞甸篮 OnDeath 甫 烹窍瘤 臼绊 弊 吝 鞘夸茄 贸府甫 蝶肺 咯扁辑 秦 霖促.

	//AB2StageManager::GetCacheStageKeepEssentialData().AddQTECount(QTEStateNum);
	//DropItem();

	//ConditionalReportMyDeathToBirthplace(); // SpawnPool 肺狼 荤噶脚绊
}

void ABladeIICharacter::ReportMyDeathToBirthplace()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ReportMyDeathToBirthplace);
	// SpawnPool 俊辑 免积茄 各狼 版快 SpawnPool 率栏肺 烹瘤甫 朝妨辑 府胶飘俊辑 厚快绰 殿狼 贸府甫 窍档废 窃. 老馆利牢 Dead 风飘甫 蝶福瘤 臼绰 版快扼档 捞扒 贸府甫 秦具 霸烙 柳青捞 等促.
	// 老馆利牢 胶抛捞瘤 柳青俊辑狼 阁胶磐俊 措秦 啊厘 角龙利牢 狼固狼 荤噶 脚绊
	//if (CachedBirthplaceInfo.Birthplace)
	{
		CachedBirthplaceInfo.bOwnerDeathReported = true; // 阁胶磐 荤噶 脚绊 咯何甫 眉农窍绰单 静老 荐 乐促.

		if (CachedBirthplaceInfo.bIsSummoned)
		{
			// bIsSummoned 俊 蝶扼 捞亥飘啊 促弗单 钦媚辑 救瞪 巴档 绝澜 ぱ.ぱ
			//SpawnPoolSummonedMobDeadClass<AB2MonsterSpawnPool*, ABladeIICharacter*>::GetInstance().Signal(CachedBirthplaceInfo.Birthplace, this);
		}
		else
		{
			//SpawnPoolWaveMobDeadClass<AB2MonsterSpawnPool*, ABladeIICharacter*, int32, int32>::GetInstance().Signal(CachedBirthplaceInfo.Birthplace, this, CachedBirthplaceInfo.WaveNumber, CachedBirthplaceInfo.WaveObjIndex);
		}
	}
}

bool ABladeIICharacter::HasRootMotion() const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_HasRootMotion);
	return ClientRootMotionParams.bHasRootMotion;
}

void ABladeIICharacter::ConditionalReportMyDeathToBirthplace()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ConditionalReportMyDeathToBirthplace);
	if (!CachedBirthplaceInfo.bOwnerDeathReported)
	{
		ReportMyDeathToBirthplace();
	}
}

void ABladeIICharacter::OnMountStartComplete()
{
	//ABladeIIAIController* pAC = Cast<ABladeIIAIController>(GetController());

	//if (pAC)
	//	pAC->StopAutoPlay();

	//ABladeIIPlayerController* pPC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (pPC->GetAutoAIController()->GetPawn() && pAC)
	//{
	//	pPC->GetAutoAIController()->StopAutoPlay();
	//	pAC->ApplyMountBehaviorTree();
	//}

	//// 公宏捧俊 静捞绰 档馒咯何 魄窜函荐 炼累(敲饭捞绢 扁霖栏肺 嘎苗淋)
	//ABladeIICharacter* pPlayer = Cast<ABladeIICharacter>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//AIAttackMoveAcceptableRadius = pPlayer->GetMoveAcceptableRadius();
}

float ABladeIICharacter::GetScaledCapsuleRadius()
{
	if (GetCapsuleComponent())
	{
		return GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	return 0.0f;
}

float ABladeIICharacter::GetScaledCapsuleHalfHeight()
{
	//if (GetCapsuleComponent())
	//{
	//	return GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//}

	return 0.0f;
}

void ABladeIICharacter::SetMoveCollision(bool bBlockPawn)
{
	ECollisionResponse CollisionResponse = bBlockPawn ? ECollisionResponse::ECR_Block : ECollisionResponse::ECR_Ignore;
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, CollisionResponse);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, CollisionResponse);
	}

	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, CollisionResponse);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, CollisionResponse);
	}
}

void ABladeIICharacter::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_PlayHit);
	//if (Role == ROLE_Authority)
	//{
	//	ReplicateHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, false);
	//}

	//if (DamageTaken > 0.f && DamageEvent.ImpulseScale > 0.f && DamageEvent.DamageTypeClass != NULL)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void ABladeIICharacter::ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ApplyDamageMomentum);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::ApplyDamageMomentum"));

	//UDamageType const* const DmgTypeCDO = DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>();
	//float ImpulseScale = DmgTypeCDO->DamageImpulse * DamageEvent.ImpulseScale;

	//if ((FMath::Abs(ImpulseScale) > 3.f) && (GetCharacterMovement() != NULL))
	//{
	//	FHitResult HitInfo;
	//	FVector ImpulseDir;
	//	DamageEvent.GetBestHitInfo(this, PawnInstigator, HitInfo, ImpulseDir);

	//	FVector Impulse = ImpulseDir * ImpulseScale;
	//	bool const bMassIndependentImpulse = !DmgTypeCDO->bScaleMomentumByMass;

	//	// limit Z momentum added if already going up faster than jump (to avoid blowing character way up into the sky)
	//	{
	//		FVector MassScaledImpulse = Impulse;
	//		if (!bMassIndependentImpulse && GetCharacterMovement()->Mass > SMALL_NUMBER)
	//		{
	//			MassScaledImpulse = MassScaledImpulse / GetCharacterMovement()->Mass;
	//		}

	//		if ((GetCharacterMovement()->Velocity.Z > GetDefault<UCharacterMovementComponent>(GetCharacterMovement()->GetClass())->JumpZVelocity) && (MassScaledImpulse.Z > 0.f))
	//		{
	//			Impulse.Z *= 0.5f;
	//		}
	//	}

	//	if (DamageEvent.FreezeDuration > 0.f)
	//	{
	//		DeferredImpulse = Impulse;
	//	}
	//	else
	//	{
	//		GetCharacterMovement()->AddImpulse(Impulse, bMassIndependentImpulse);
	//	}
	//}
	//else
	//{
	//	DeferredImpulse = FVector::ZeroVector;
	//}
}

void ABladeIICharacter::OnAnimMobDeadEnd(bool bInUseDeadRagDoll, float InRemainLifeSpan, bool bInUseDeadSink, float InDeadSinkDelayTime, float InDeadSinkZOffset, float InDeadSinkDuration)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_OnAnimMobDeadEnd);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::OnAnimMobDeadEnd"));

	//// Animation Blueprint 甫 烹秦 Dead 局聪皋捞记阑 窍搁 汲摹茄 AnimNotify 甫 烹秦 坷霸 等促.
	//// 老馆利栏肺绰 肺流 惑俊辑狼 磷绰 贸府 (单固瘤甫 烹秦 眉仿捞 0 酒贰肺 冻绢瘤芭唱 磊气 殿) 捞饶 甸绢客具 茄促. 
	//// SpawnPool 俊 殿废等 阁胶磐扼搁 弊率俊辑档 捞固 荤噶脚绊 等 捞饶
	//// 酒流 攫府倔 Object 瞒盔俊辑绰 valid 窍唱 咯辨 瘤唱搁辑 攫府倔 Object 力芭 countdown 捞 瞪 巴烙.

	//if (IsPlayerControlled())
	//{
	//	return; // 老窜 Mob 傈侩
	//}

	//if (IsAttacking() && // DeadEnd 肺 吭绰单 Attack 惑怕扼绰 巴篮 Attack 局聪皋捞记俊 DeadEnd 啊 乐促绰 巴捞 凳. 溜, 磊混傍拜.. 纳捞胶父 乐绰 霸 锐噶荤亲牢单..
	//	// 窜, 盔贰 狼档客 促福霸 捞犯霸 甸绢坷绰 版快啊 绝柳 臼促.. 弊府 彬阐窍柳 臼篮单..
	//	!IsInQTEState() && !CachedBirthplaceInfo.bOwnerDeathReported)
	//{
	//	// 捞 惑怕 弊措肺 柳青窍搁 捞 某腐磐啊 力芭绰 登瘤父 霸烙 柳青俊 康氢阑 林绰 OnDeath 俊辑狼 贸府啊 积帆登骨肺 Suicide 甫 流立 妮秦霖促.
	//	bCommittedSuicideAtDeadEnd = true; // AnimBP 俊辑 烹惑利牢 Dead Anim 勤甸傅阑 阜扁 困秦 眠啊 敲贰弊 技泼. 恐衬窍搁 Dead Anim 勤甸傅篮 捞固 等 巴捞骨肺.
	//	Suicide();
	//}

	//// 父老狼 版快甫 措厚窍绰 弥饶狼 犬牢荤混 内靛. 捞 矫痢俊辑档 荤噶 脚绊啊 救等 舅妨瘤瘤 臼篮 盔牢捞 乐促搁 犬牢且 鞘夸啊 乐促.
	//if (CachedBirthplaceInfo.Birthplace && !CachedBirthplaceInfo.bOwnerDeathReported && !IsInQTEState()) // QTE 俊辑绰 喊档肺 荤噶脚绊甫 窍绰单.. 捞订 QTEState 啊 秦力登绰 芭 鞍扁档 窍绊..
	//{
	//	ensureMsgf(false, TEXT("Detected dead report miss by unknown reason. Forcing dead report.. %s, Wave%d_%d"), *this->GetName(), CachedBirthplaceInfo.WaveNumber, CachedBirthplaceInfo.WaveObjIndex);
	//	ReportMyDeathToBirthplace();
	//}

	//// 霸烙葛靛俊 舅副鞘夸啊 乐澜
	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (pGM)
	//{
	//	//pGM->NotifyAnimMobDeadEnd(this);
	//}

	//// bInUseDeadRagDoll 篮 AnimBP 俊辑 坷绰 汲沥.
	//if (bInUseDeadRagDoll)
	//{
	//	SetRagdollPhysics(InRemainLifeSpan, !bInUseDeadRagDoll); // 捞 救俊辑 GC 墨款飘 促款阑 且 巴烙.
	//	GetCharacterMovement()->StopMovementImmediately();
	//}
	//else
	//{
	//	WrappedSetLifeSpan(InRemainLifeSpan); // GC 墨款飘 促款..
	//	PreventMovementByStageEvent(); // 拱府甫 荤侩窍瘤 臼栏搁辑 movment stop 鳖瘤 窍搁 沥富 犁固绝澜.
	//	if (bInUseDeadSink && InDeadSinkZOffset > 0.f && InDeadSinkDuration > 0.f && InDeadSinkDelayTime < InRemainLifeSpan && InDeadSinkZOffset > 0.f)
	//	{
	//		DeadSinkZOffsetPerSecond = (InDeadSinkZOffset / InDeadSinkDuration) * GetRootComponent()->GetComponentScale().Z;
	//		DeadSinkLeftTime = InDeadSinkDuration;
	//		GetWorldTimerManager().SetTimer(SetDeadSinkHandle, this, &ABladeIICharacter::StartDeadSink, InDeadSinkDelayTime, false);
	//	}
	//}

	//DetachFromControllerPendingDestroy();

	//check(GetCapsuleComponent());

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ABladeIICharacter::StartDeadSink()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_StartDeadSink);
	bIsDeadSinking = true;
}

int32 ABladeIICharacter::GetAIAttackIndex(const TBitArray<>& InBitIndexArray, const float InTotalWeight, const int32 InPreIDX, const TArray<FAIAttackData>& InDataArray)
{
	int32 Result = -1;
	float Weight = FMath::FRand() * InTotalWeight;
	float CurrentWeight = 0.f;
	for (int32 i = 0; i < InBitIndexArray.Num(); ++i)
	{
		if (InBitIndexArray[i] && i != InPreIDX)
		{
			CurrentWeight += InDataArray[i].PossibilityWeight;
			if (Weight <= CurrentWeight)
			{
				Result = i;
				break;
			}
		}
	}
	return Result;
}

bool ABladeIICharacter::IsAboardMob(ENPCClass eNpcClass)
{
	bool isAboardMob = false;

	if (eNpcClass == ENPCClass::ENC_Mob_Minotaur ||
		eNpcClass == ENPCClass::ENC_Mob_Zombie_Fat)
	{
		isAboardMob = true;
	}

	return isAboardMob;
}

UMaterialInstanceDynamic* ABladeIICharacter::OnAnimMobDeadEffectSpawn(UMaterialInterface* InDeadBodyMI, FName InDeadBodyMIParamName,
	UParticleSystem* InDeadParticleTemplate, FName InDeadParticleAttachBoneName, FVector InScale /*= FVector(1.f)*/)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_OnAnimMobDeadEffectSpawn);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::OnAnimMobDeadEffectSpawn"));

	if (IsPlayerControlled())
	{
		return NULL; // 老窜 Mob 傈侩
	}

	class USkeletalMeshComponent* SKMeshComp = GetMesh();
	UMaterialInstanceDynamic* CreatedDeadBodyMID = NULL;

	if (InDeadBodyMI) // 磷阑 锭 官层摹绰 赣萍府倔. 鳖改霸 鸥甸绢啊绰 瓤苞..
	{
		// 角龙利栏肺 窍唱观俊 贸府甫 且 荐 观俊 绝绰单 AnimBP 俊辑 OutCreatedDeadBodyMID 窍唱父 罐酒辑 颇扼固磐甫 技泼且 巴捞扁 锭巩.
		for (int32 MII = 0; MII < SKMeshComp->GetNumMaterials(); ++MII)
		{
			if (!CreatedDeadBodyMID)
			{ // MID 绰 0锅俊辑父 父甸霸 瞪 巴.
				CreatedDeadBodyMID = SKMeshComp->CreateDynamicMaterialInstance(MII, InDeadBodyMI);
			}
			else
			{ // 唱赣瘤 冀记篮 父电 芭 窍唱啊瘤绊 促 度鞍捞 技泼.
				SKMeshComp->SetMaterial(MII, CreatedDeadBodyMID);
			}
		}

		if (CreatedDeadBodyMID && InDeadBodyMIParamName != NAME_None)
		{
			// 鳖改霸 鸥甸绢啊哥 雀茄绢赴 牢积阑 付公府窍绰 瓤苞甫 矫累茄促..
			CreatedDeadBodyMID->SetScalarParameterValue(InDeadBodyMIParamName, 0.0f);
		}
	}

	if (InDeadParticleTemplate)
	{
		UGameplayStatics::SpawnEmitterAtLocation(SKMeshComp, InDeadParticleTemplate, SKMeshComp->GetBoneLocation(InDeadParticleAttachBoneName), GetActorRotation());
		//UGameplayStatics::SpawnEmitterAttached(InDeadParticleTemplate, SKMeshComp, InDeadParticleAttachBoneName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, false, true, (InScale == FVector::ZeroVector) ? FVector(1.f, 1.f, 1.f) : InScale);
	}

	return CreatedDeadBodyMID;
}

void ABladeIICharacter::LifeSpanExpired()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_LifeSpanExpired);
	// 霸烙 肺流惑俊辑狼 荤噶 贸府绰 拱沸捞绊 攫府倔 坷宏璃飘 瞒盔俊辑狼 GC 流傈鳖瘤 埃 惑炔栏肺 (厘肥侥俊 49犁鳖瘤 场车澜) 朝 荤侩窍绰 镑捞 乐促搁 弊父 镭档废 弥饶狼 宏肺靛某胶飘甫 茄促.	
	// ABladeIICharacter::Destroyed 俊 持栏搁 歹 承霸 目滚 啊瓷茄单 弊凡 版快 SpawnPoolWaveMobPhaseChangeClass 甫 烹秦 StageEventDirector 俊辑 phase 函拳肺 促矫 spawn 茄 各阑 官层摹绰 霸 救 凳. 拱沸 弊率 备泅阑 粱 官操搁 啊瓷篮 窍促.
	//PleaseForgetMeClass<ABladeIICharacter*>::GetInstance().Signal(this);

	Super::LifeSpanExpired();
}

bool ABladeIICharacter::IsBuffActive(EBuffType BuffType) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsBuffActive);

	if (auto* BuffObj = GetBuff(BuffType))
		return BuffObj->IsActive();

	return false;
}

class UB2Buff_Base* ABladeIICharacter::GetBuff(EBuffType BuffType) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetBuff);

	if (auto* BuffManagerObj = GetBuffManager())
		return BuffManagerObj->FindBuff(BuffType);

	return nullptr;
}

bool ABladeIICharacter::ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage)
{
	return false;
}

bool ABladeIICharacter::RequestDamage(const float Damage, const FDamageInfo* DamageInfo, ABladeIICharacter* DamageCauser, bool NetBroadcast)
{
	//// 乔拜 林眉绰 公炼扒 this == ROLE_Authority!!
	//if (Role == ROLE_Authority || NetBroadcast)
	//{
	//	FDamageEvent RequestDamageEvent;
	//	FDamageInfo RequestDamageInfo;

	//	RequestDamageInfo.SetDamageInfo(DamageInfo, DamageCauser);
	//	RequestDamageEvent.DamageInfo = &RequestDamageInfo;

	//	if (DamageCauser && TakeDamage(Damage, RequestDamageEvent, DamageCauser->GetController(), DamageCauser) > 0.f)
	//		return true;
	//}

	return false;
}

void ABladeIICharacter::AddDamageReservation(const FDamageReservationInfo& ReservDmgInfo)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_AddDamageReservation);
	DamageReservationInfos.Add(ReservDmgInfo);
}

bool ABladeIICharacter::IsInTinyDamage() const
{
	if (IsInHugeDamage() || LastDamageType == EAttackType::EAT_Default)
		return false;

	const bool IsInTinyDamage1 = (LastDamageType == EAttackType::EAT_LeftSwing);
	const bool IsInTinyDamage2 = (LastDamageType == EAttackType::EAT_RightSwing);

	return IsInTinyDamage1 || IsInTinyDamage2;
}

bool ABladeIICharacter::IsInHugeDamage() const
{
	const bool IsInKnockBack = (LastDamageType == EAttackType::EAT_KnockBack);
	const bool IsInBigBounce = (LastDamageType == EAttackType::EAT_BigBounce);

	return IsInBigBounce || IsInKnockBack;
}

void ABladeIICharacter::ApplyDamageAnimation(EAttackType DamagedType)
{
	LastDamageType = DamagedType;
	ApplyDefaultDamageAnimation();
}

void ABladeIICharacter::ApplyDefaultDamageAnimation()
{
	RandDamageAnimType = (RandDamageAnimType == EAttackType::EAT_Default ? EAttackType::EAT_LeftSwing : EAttackType::EAT_Default);
}

//////////////////////////////////////////////////////////////////////////
// Animations

void ABladeIICharacter::SetRagdollPhysics(float LifeSpan, bool bNoRagdoll)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetRagdollPhysics);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetRagdollPhysics"));

	DetachFromControllerPendingDestroy();

	if (GetNetMode() == NM_DedicatedServer)
	{
		TurnOff();
		SetActorHiddenInGame(true);
		WrappedSetLifeSpan(!bNoRagdoll);
	}
	else
	{
		if (GetMesh())
		{
			static FName CollisionProfileName(TEXT("Ragdoll"));
			GetMesh()->SetCollisionProfileName(CollisionProfileName);
		}
		SetActorEnableCollision(true);

		// Death anim
		//float DeathAnimDuration = PlayAnimMontage(DeathAnim);

		// Ragdoll
		//if (DeathAnimDuration > 0.f)
		//{
		//	GetWorldTimerManager().SetTimer(SetRagdollPhysicsTimerHandle, this, &ABladeIICharacter::SetRagdollPhysics, FMath::Min(0.1f, DeathAnimDuration), false);
		//}
		//else
		//{
		//	SetRagdollPhysics();
		//}
	}

	//BII_CHECK(GetMesh() && GetCharacterMovement());

	bool bInRagdoll = false;

	//if (IsPendingKill() || bNoRagdoll)
	//{
	//	bInRagdoll = false;
	//}
	//else if (!GetMesh() || !GetMesh()->GetPhysicsAsset())
	//{
	//	bInRagdoll = false;
	//}
	//else
	//{
	//	// initialize physics/etc
	//	GetMesh()->SetAllBodiesSimulatePhysics(true);
	//	GetMesh()->SetSimulatePhysics(true);
	//	GetMesh()->WakeAllRigidBodies();
	//	GetMesh()->bBlendPhysics = true;

	//	bInRagdoll = true;
	//}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	if (!bInRagdoll)
	{
		// hide and set short lifespan
		TurnOff();
		SetActorHiddenInGame(!bNoRagdoll);
		WrappedSetLifeSpan(LifeSpan);
	}
	else
	{
		WrappedSetLifeSpan(LifeSpan);
	}
}

void ABladeIICharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_FellOutOfWorld);
	//Die(Health, FDamageEvent(dmgType.GetClass()), NULL, NULL);
}

bool ABladeIICharacter::IsControlledByLocalPlayer()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsControlledByLocalPlayer);
	const bool IsPlayer = IsPlayerControlled();
	const bool IsLocal = IsLocalCharacter();

	// 柳楼 Local Player捞芭唱
	if (IsPlayer && IsLocal)
		return true;

	// LocalPlayer俊霸 炼辆寸窍绰 吝 (QTE) 吝捞搁 
	if (IsQTEActor(GetLocalCharacter()))
		return true;

	return false;
}

void ABladeIICharacter::Suicide()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_Suicide);
	KilledBy(this);
}

void ABladeIICharacter::KilledBy(APawn* EventInstigator)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_KilledBy);
	if (GetLocalRole() == ROLE_Authority && !bIsDying)
	{
		AController* Killer = NULL;
		if (EventInstigator != NULL)
		{
			Killer = EventInstigator->Controller;
			LastHitBy = NULL;
		}

		//Die(Health, FDamageEvent(UDamageType::StaticClass()), Killer, NULL);
	}
}

bool ABladeIICharacter::IsEnemy(class AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsEnemy);
	//if (DamageCauser != this)
	//{
	//	ABladeIICharacter* pRealDamageCauser = Cast<ABladeIICharacter>(DamageCauser);

	//	// Projectile check owner
	//	ABladeIIProjectile* pProjectile = Cast<ABladeIIProjectile>(DamageCauser);
	//	if (pProjectile)
	//	{
	//		pRealDamageCauser = Cast<ABladeIICharacter>(pProjectile->GetOwner());
	//	}

	//	// Area Damage Actor check owner
	//	AB2AreaDamageActorBase* pAreaDamageActor = Cast<AB2AreaDamageActorBase>(DamageCauser);
	//	if (pAreaDamageActor)
	//	{
	//		pRealDamageCauser = Cast<ABladeIICharacter>(pAreaDamageActor->GetOwner());
	//	}

	//	return pRealDamageCauser && GetTeamNum() != pRealDamageCauser->GetTeamNum();
	//}

	return false;
}

bool ABladeIICharacter::IsEnemy(int32 TargetTeamNum)
{
	return GetTeamNum() != TargetTeamNum;
}

void ABladeIICharacter::CheckReservationDamage()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_CheckReservationDamage);
	//if (DamageReservationInfos.Num() == 0)
	//	return;

	//int32 nApplyDamageReservationInfoIdx = -1;

	//for (int32 nCnt = 0; nCnt < DamageReservationInfos.Num(); ++nCnt)
	//{
	//	if (GWorld->GetTimeSeconds() >= DamageReservationInfos[nCnt].ApplyWorldTime)
	//	{
	//		nApplyDamageReservationInfoIdx = nCnt;
	//		break;
	//	}
	//}

	//if (nApplyDamageReservationInfoIdx != -1)
	//{
	//	FDamageReservationInfo* DamageReservationInfoToApply = &DamageReservationInfos[nApplyDamageReservationInfoIdx];

	//	UGameplayStatics::SpawnEmitterAtLocation(GWorld, DamageReservationInfoToApply->DamageFx, GetActorTransform());

	//	if (DamageReservationInfoToApply->DamageCauser && DamageReservationInfoToApply->DamageCauser->IsValidObj())
	//	{
	//		ABladeIICharacter* pDamageCauser = Cast<ABladeIICharacter>(DamageReservationInfoToApply->DamageCauser);

	//		FDamageEvent DmgEvent;
	//		DmgEvent.DamageInfo = DamageReservationInfoToApply->DamageInfo;

	//		BII_CHECK(pDamageCauser);

	//		if (pDamageCauser)
	//		{
	//			TakeDamage(DamageReservationInfoToApply->Damage,
	//				DmgEvent,
	//				DamageReservationInfoToApply->EventInstigator,
	//				DamageReservationInfoToApply->DamageCauser);
	//		}
	//	}
	//	DamageReservationInfos.RemoveAt(nApplyDamageReservationInfoIdx);
	//}
}

bool ABladeIICharacter::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const FDamageInfo* DamageType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_CanTakeDamage);
	if (Health <= 0.f)
		return false;

	//if (DamageType == nullptr)
	//	return false;

	//if (IsControlledByQTEPlayer())
	//	return false;

	//if (bSpawningNoDamageGuard) // 措眉肺 SpawnPool 俊辑 Spawn 矫虐搁辑 殿废窍扁 傈.
	//	return false;

	//if (!IsEnemy(DamageCauser))
	//	return false;

	//if (IsBuffActive(EBuffType::Buff_Hide) || IsBuffActive(EBuffType::Buff_Unbeatable))
	//	return false;

	// bApplyNPC
	//if (GetTeamNum() == INDEX_NONE && DamageType->bApplyToNPC)
	//	return true;

	// 咀磐俊乐绰 敲贰弊档 眉农
	//if (!bCanBeDamaged)
		//return false;

	return true;
}

void ABladeIICharacter::SetInvincible(bool bNewInvincible)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetInvincible);

	//bInvincible = bNewInvincible;
}

const void* ABladeIICharacter::GetCustomDamageInfo()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetCustomDamageInfo);
	return &CurrentDamageInfo;
}

float ABladeIICharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_TakeDamage);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::TakeDamage"));
//	check(DamageCauser);
//
//	// TODO: find better cast
//	const FDamageInfo* DamageType = static_cast<const FDamageInfo*>(DamageEvent.DamageInfo);
//
//	if (!DamageType || CanTakeDamage(DamageCauser, DamageEvent, DamageType) == false)
//	{
//#if !UE_BUILD_SHIPPING
//		// 措眉肺 DamageType 捞 绝绰 惑炔篮 楷免阑 烹秦 敲饭捞 登绰 attack 局聪皋捞记俊辑 damage notify 啊 唱啊绰 芭. InterpTrackAnimControl 狼 SkipDamageNotifiersOnly 甫 眉农.
//		if (!DamageType)
//		{
//			UE_LOG(LogBladeII, Warning, TEXT("NULL DamageType is detected of DamageCauser %s"), DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"));
//		}
//#endif
//		return 0.f;
//	}
//
//	const FDamageInfo& DamageInfo = *(DamageType);
//	ABladeIICharacter* Attacker = GetAttacker(DamageCauser);
//
	float ActualDamage = 0.f;
//	bool bCriticalDamage = false;
//
//	ABladeIIGameMode* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//	const bool IsPVPGame = B2GameMode ? B2GameMode->IsPVPGameMode() : false;
//
//	if (Role == ROLE_Authority)
//	{
//		ActualDamage = Damage;
//
//		const bool IgnoreActualDamage = bInvincible || Attacker == nullptr || (IsPVPGame == false && IsGuardSuccess(&DamageInfo));
//
//		if (IgnoreActualDamage)
//		{
//			ActualDamage = 0.f;
//		}
//
//		else
//		{
//			// 敲贰弊俊蝶扼 罐绰单固瘤 例措蔼栏肺 函券
//			EDamageLogicType DamageLogicType = DamageInfo.DamageLogicType;
//			if (bForceReceivdAbsoluteDamage)
//				DamageLogicType = EDamageLogicType::ELT_Absolute;
//
//			// 敲贰弊俊蝶扼 罐绰单固瘤 肺流阑 鸥荐肺 函券
//			if (bForceReceivdAttackCountDamage)
//				DamageLogicType = EDamageLogicType::ELT_AttactCount;
//
//			ActualDamage = GetDamageByDamageLogic(ActualDamage, DamageLogicType, Attacker, DamageCauser, DamageInfo, bCriticalDamage);
//
//			// 单固瘤 钢萍敲 畴萍颇捞樊 利侩
//			ActualDamage *= DamageMultiplier;
//
//			// Shield 肺流 - 单固瘤客 眉仿俊 奴 康氢阑 林哥 老馆 鸥拜苞档 剐立茄 楷包捞 乐栏骨肺 滚橇啊 酒囱 TakeDamage 肺流俊辑 贸府
//			const float ShieldAmount = GetShield();
//			const float RemainShieldAmount = ShieldAmount - ActualDamage;
//			float AbsorbeDamage = 0.0f;
//
//			if (RemainShieldAmount <= 0.f)	// Shield 捞惑狼 Damage啊 甸绢咳
//			{
//				AbsorbeDamage = ShieldAmount;
//				ActualDamage = ActualDamage - ShieldAmount;
//			}
//			else // Shield 固父狼 Damage 啊 甸绢咳 == Shield啊 巢澜
//			{
//				AbsorbeDamage = ActualDamage;
//				ActualDamage = 0.f;
//			}
//
//			SetShield(ShieldAmount - AbsorbeDamage);
//			// Shield 肺流 - 单固瘤客 眉仿俊 奴 康氢阑 林哥 老馆 鸥拜苞档 剐立茄 楷包捞 乐栏骨肺 滚橇啊 酒囱 TakeDamage 肺流俊辑 贸府
//		}
//
//		// 0 檬苞 (捞惑 酒丛) 单固瘤绰 弥家单固瘤 1肺 绊沥 (== 0.02 单固瘤绰 1肺)
//		if (ActualDamage > 0.f)
//			ActualDamage = FMath::Clamp<float>(ActualDamage, 1.f, ActualDamage);
//
//		BII_CHECK(ActualDamage >= 0.f); // No heal by damage
//
//		if (ActualDamage > 0.0f)
//			SetHealth(Health - ActualDamage);	// Need to Replicate
//
//		// Defense Ether -> this == Authority && HasDefense Ether
//		ProcessEtherSetEffect(EEtherSetType::Defense, Attacker, DamageInfo, ActualDamage);
//	}
//	else
//	{
//		// Role_Authority 啊 酒聪搁 犁瘤 富绊 官肺 Damage肺 劝侩
//		ActualDamage = Damage;
//	}
//
//	// Damage Log 巢扁扁 - Network葛靛档 RemoteTakeDamage 风飘甫 蝶扼客辑 角青登绢具 窍骨肺 Role眉农 救窃
//	if (B2GameMode)
//		B2GameMode->RecordDamageLog(ActualDamage, this, Attacker);
//
//	if (QTEType == EQTEType::EQT_Break)
//		CurrentQTEBreakPoint -= (ActualDamage / DamageMultiplier);
//
//	if (GetBuffManager())
//		GetBuffManager()->OwnerTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// CheatCommnad - hero
//	// Development cheat to kill all the monsters at once.
//	if (Attacker && IsPlayerControlled() == false) // 敲饭捞绢 捞寇狼 各俊父 康氢.
//	{
//		if (Attacker->IsHeroMode())
//			SetHealth(0.f);
//	}
//
//	// CheatCommand - zombie
//	if (IsImmortalMode())
//		SetHealth(1.f);	// 乔啊 0 捞窍搁 沥惑 乔拜肺流栏肺 汝矾啊瘤 臼澜 ( if(Health <= 0.f) 肺流阑 沤 )
//#endif
//
//	// DamageEffectInfo 3D DamageNum 措脚 2D UI 扁馆 DamageNum. 
//	if (ActualDamage > 0.f) // Shield 俊辑 软荐茄 巴篮 鸥涝 促福霸 秦辑 蝶肺 钎矫窍霸 瞪 荐档..
//	{ // Critical damage 钎矫档 咯扁辑 窃.
//
//		// Remote Pawn vs Remote Pawn 埃狼 Damage甫 被捞 唱狼 Local俊辑 钎矫 且 鞘夸啊 乐阑瘤绰 狼巩
//		// [171214_YJ] GameMode Rule俊 促弗 某腐磐档 钎矫窍绰 葛靛 牢瘤, 
//		// 促弗某腐磐档 钎矫窍瘤 臼绰促搁 锭府芭唱 嘎篮荤恩捞 唱 牢瘤 眉农秦辑 免仿.
//		if (IsApplyDamageNotifyUI(Attacker))
//		{
//			CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Signal(this, ActualDamage, bCriticalDamage);
//		}
//	}
//
//	// Notify my enemy and GameMode
//	if (Attacker && B2GameMode && !DamageInfo.bIgnoreNotifyToAttacker)
//	{
//		Attacker->AffectToAttacker(this, ActualDamage, DamageInfo);
//		Attacker->NotifyTargetDamaged(this, ActualDamage);
//		B2GameMode->NotifyTargetDamagedFromDamagedTarget(Attacker, this, ActualDamage);
//
//		if (Health <= 0.f)
//		{
//			Attacker->AffectToAttackerAfterVictimDie(this, ActualDamage, DamageInfo);
//		}
//	}
//
//	if (Health <= 0.f)
//	{
//		ProcessTakeDamageEffect(DamageInfo, DamageCauser);
//		if (Role == ROLE_Authority && Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser))
//		{
//			//AffectToAttackerAfterDie(ActualDamage, DamageInfo, Attacker);
//		}
//
//		return ActualDamage;
//	}
//	else
//	{
//		PlayHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
//
//		// bIgnoreNotifyToAttacker汲沥等扒 洒飘橇府令 救八
//		// 捞姑捞搁 洒飘橇府令 菩胶
//		// 啊靛惑怕档 洒飘橇府令 菩胶
//		if (ActualDamage >= 1.0f && !DamageInfo.bIgnoreNotifyToAttacker
//			&& IsImmune(&DamageInfo) == false
//			&& !IsGuardSuccess(&DamageInfo))
//		{
//			PauseForDuration(DamageEvent.FreezeDuration);
//
//			if (Attacker && DamageEvent.FreezeDuration > 0)
//			{
//				Attacker->PauseForDuration(DamageEvent.FreezeDuration);
//			}
//		}
//		SetOscillation(DamageInfo.Amplitude, DamageInfo.Frequency);
//	}
//
//	// QTE Check
//	const ETeamType AttackerTeamType = Attacker ? ETeamType(Attacker->GetTeamNum()) : ETeamType::ETT_End;
//	CheckQTEEnable(AttackerTeamType);
//
//	UpdateArmorByDamageInfo(DamageInfo);
//
//	// Immune 惑怕俊辑 惑怕捞惑 阂啊
//	if (IsImmune(&DamageInfo) == false)
//	{
//		TakeDamageAbnormalState(DamageInfo, DamageCauser);
//		ProcessTakeDamageAnimation(DamageInfo, DamageCauser);
//	}
//
//	// 捞姑捞歹扼档 啊靛吝浚 单固瘤逞+(馆拜阑困秦)
//	if (IsImmune(&DamageInfo) && IsGuarding())
//	{
//		UpdateDamageNum(DamageInfo);
//	}
//
//	ProcessTakeDamageEffect(DamageInfo, DamageCauser);
//
//	// Check Phase Shift by health trigger
//	const int32 CurrentPhaseNum = GetCurrentPhaseNum();
//	if (PhaseDataArray.IsValidIndex(CurrentPhaseNum))
//	{
//		const int32 TriggerHealthRate = PhaseDataArray[CurrentPhaseNum].HealthTrigger;
//		const float CurrHealthRate = static_cast<float>(Health / MaxHealth) * 100.f;
//
//		if (TriggerHealthRate > CurrHealthRate)
//		{
//			if (bUsePhaseChangeAnimation)
//				bInPhaseAnimation = true;
//			else
//				GoToNextPhase();
//		}
//	}

	return ActualDamage;
}

void ABladeIICharacter::TakeDamageAbnormalState(const FDamageInfo& DamageInfo, AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_TakeDamageAbnormalState);

	//bool bIgnoreAbnormalFromGuard = IsGuardSuccess(&DamageInfo);
	//if (bIgnoreAbnormalFromGuard)
	//	return;

	//const bool bTriggerActivate = FMath::RandRange(0.0f, 1.0) <= DamageInfo.StateDamageTriggerRate;
	//if (bTriggerActivate == false)
	//	return;

	//const bool bValidVariables = DamageInfo.StateDamageDuration > 0.f && IsAllowState(DamageInfo.StateType);
	//const EBuffType DebuffType = GetBuffTypeByStateAttack(DamageInfo.StateType);

	////if (DebuffType != EBuffType::Buff_None && bValidVariables)
	////{
	////	auto* DebuffObj = AddBuff(DebuffType, DamageInfo.StateDamageDuration, DamageInfo.StateDamageAmount, DamageCauser);
	////	if (auto* DotDebuff = Cast<UB2Buff_DotDamage>(DebuffObj))
	////		DotDebuff->DotDamageLogicType = DamageInfo.StateDamageLogicType;
	////}

	//if (DamageInfo.StateType == EStateAttackType::EDT_BigBounce || DamageInfo.StateType == EStateAttackType::EDT_NockBack)
	//{
	//	SetForceComboEnd();
	//	ClearAttachedParticles();

	//	auto DamageType = (DamageInfo.StateType == EStateAttackType::EDT_BigBounce) ? EAttackType::EAT_BigBounce : EAttackType::EAT_KnockBack;
	//	LastDamageType = DamageType;

	//	IncreaseDamageNum();
	//}
}

void ABladeIICharacter::NotifyTargetDamaged(class ABladeIICharacter* DamagedTarget, float DamageAmount)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_NotifyTargetDamaged);
}


bool ABladeIICharacter::IsDamageAnimationProcessable(const FDamageInfo& DamageInfo)
{
	// 1. QTE 付款飘 惑怕俊辑绰 Damage Animatino 角青 X
	if ((GetQTEEnabled() && QTEType == EQTEType::EQT_Mount))
		return false;

	const auto CurrentDamageType = DamageInfo.DamageType;

	// 2. 乘归 or 候官款胶 单固瘤甫 倾侩窍瘤 臼栏搁 Animation 角青 X ( 阁胶磐狼 版快父 )
	if (IsMob())
	{
		if (CurrentDamageType == EAttackType::EAT_KnockBack && !IsAllowState(EStateAttackType::EDT_NockBack))
			return false;

		if (CurrentDamageType == EAttackType::EAT_BigBounce && !IsAllowState(EStateAttackType::EDT_BigBounce))
			return false;

		// 单固瘤 局聪皋捞记阑 荤侩窍瘤 臼绰 版快, 葛滴 角青X
		if (!bAllowRecoilAnimation)
			return false;
	}

	// 3. CBT 俺急 -  Skill 荤侩吝篮 力寇
	if (IsSkillAttacking() && DamageInfo.bForceApplyStateDamage == false)
		return false;

	return true;
}


void ABladeIICharacter::ProcessTakeDamageAnimation(const FDamageInfo& DamageInfo, AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ProcessTakeDamageAnimation);

	if (IsDamageAnimationProcessable(DamageInfo))
	{
		const bool IsArmorLower = Armor <= 0.f;
		const bool IsForceArmorBreak = IsForceApplyHitAnimation() || DamageInfo.ArmorBreakLogicType == EArmorBreakLogicType::EALT_AlwaysBreak;
		const bool IsPreventByGuard = IsGuarding() && IsGuardSuccess(&DamageInfo);

		/*
			1. Guard 救窃
			1-1. ArmorBreak : true - 酒赣宏饭捞农 惯悼
			1-2. ArmorBreak : false - Armor樊父 别烙

			2. Guard 吝 GuardBreak X
			2-1. ArmorBreak : true - 酒赣宏饭捞农 公矫
			2-2. ArmorBreak : false - Armor樊 别瘤 臼澜

			3. Guard 吝 GuardBreak O
			3-1. ArmorBreak : true - 酒赣宏饭捞农 惯悼
			3-2. ArmorBreak : false - Guard父 秦力
		*/

		const bool IsArmorBreak = IsPreventByGuard == false && (IsArmorLower || IsForceArmorBreak);

		PreDamageAnimation(DamageInfo, IsArmorBreak);
		PostDamageAnimation(DamageInfo, DamageCauser);

		const bool bForceBounceOrKnockback = DamageInfo.bForceApplyStateDamage;

		// 捞 矫痢俊辑狼 LastDamageType捞 B2AnimInstance俊 傈崔 瞪 Type
		// DamageInfo.DamageType阑 扁夯栏肺 泅犁 某腐磐狼 惑怕俊 蝶扼 PreDamageAnimation俊辑 LastDamageType蔼阑 犬沥
		const auto FinalDamageType = LastDamageType;
		if (bForceBounceOrKnockback || IsHugeDamage(FinalDamageType))
		{
			SetForceComboEnd();
			ClearEffectInAbnormalState();
		}
	}
}

EAttackType ABladeIICharacter::GetSuitableDamageType(const FDamageInfo& DamageInfo, bool IsArmorBreak)
{
	const auto OrgArmorBreakType = DamageInfo.DamageType;
	auto DamageType = EAttackType::EAT_Default;
	if (IsArmorBreak)
	{
		// Player 开矫 Default单固瘤扼档 ArmorBreak扼搁 TinyDamage 贸府
		if (OrgArmorBreakType == EAttackType::EAT_Default)
			DamageType = FMath::RandBool() ? EAttackType::EAT_LeftSwing : EAttackType::EAT_RightSwing;

		else
			DamageType = OrgArmorBreakType;
	}

	return DamageType;
}

void ABladeIICharacter::PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak)
{
	auto ArmorBreakResponse = GetSuitableDamageType(DamageInfo, IsArmorBreak);
	if (ArmorBreakResponse != EAttackType::EAT_Default)
	{
		// Monster RightSwing 矫 AnimationFreeze 登绢滚府绰 捞惑茄 滚弊啊 乐澜... 
		if (ArmorBreakResponse == EAttackType::EAT_RightSwing)
			ArmorBreakResponse = EAttackType::EAT_LeftSwing;

		ApplyDamageAnimation(ArmorBreakResponse);
		UpdateDamageNum(DamageInfo);
	}
}

void ABladeIICharacter::PostDamageAnimation(const FDamageInfo& DamageInfo, AActor* DamageCauser)
{
	const auto DamageType = DamageInfo.DamageType;

	if (IsFrozen())
	{
		if (IsHugeDamage(DamageType))
			ClearBuff(EBuffType::Debuff_Freezing);
	}

	else
	{
		// Turn to Damage Actor
		const bool IsInMoveableState = DamageType != EAttackType::EAT_Default && GetAttackState() < EAttackState::ECS_Skill01;
		const bool IsGuardableAttack = IsApplyGuarding() && (DamageInfo.GuardBreakLogicType != EGuardBreakLogicType::EGLT_AbleToGuard);
		if (DamageCauser && !bIgnoreRotateByDamage && (IsInMoveableState || IsGuardableAttack))
		{
			FVector Diff = DamageCauser->GetActorLocation() - GetActorLocation();
			FRotator TargetRotation(0.f, FRotator::ClampAxis(FMath::Atan2(Diff.Y, Diff.X) * 180.f / PI), 0.f);
			SetActorRotation(TargetRotation);
		}
	}
}

void ABladeIICharacter::ProcessTakeDamageEffect(const FDamageInfo& DamageInfo, AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ProcessTakeDamageEffect);
	//UB2DamageEffectInfo* DamageEffectInfo = GetDamageEffectInfo();
	//// Spawn damage effect and hit flash
	//if (DamageEffectInfo)
	//{
	//	// Hit Flash is deferred after abnormal state update
	//	if (DamageEffectInfo)
	//		DamageEffectInfo->PlayHitFlash(this);

	//	bool IsProjectile = Cast<ABladeIIProjectile>(DamageCauser) != nullptr ? true : false;

	//	const bool bForceUseHitActor = (IsProjectile) || IsApplyGuarding();
	//	if (bForceUseHitActor)
	//	{
	//		// 啊靛捞棋飘 Yaw雀傈捞寇浚 利侩阂啊瓷. 绊妨 救登绢乐促.
	//		// 郴困摹尔 啊秦磊困摹扁馆栏肺 Z绵 力芭秦辑 捞侩
	//		FVector RotVector = GetActorLocation() - DamageCauser->GetActorLocation();
	//		RotVector.Z = 0.0f;
	//		RotVector = RotVector.GetSafeNormal();
	//		FRotator ActorRot = RotVector.Rotation();

	//		DamageEffectInfo->SpawnDamageEffect(DamageInfo.WeaponType, DamageInfo.ElementType, this,
	//			EHitDirection::EHD_HitActor, DamageInfo.HitRandomAngleOverride,
	//			ActorRot);
	//	}
	//	else
	//	{
	//		DamageEffectInfo->SpawnDamageEffect(DamageInfo.WeaponType, DamageInfo.ElementType, this,
	//			DamageInfo.HitDirectionOverride, DamageInfo.HitRandomAngleOverride,
	//			DamageCauser->GetActorRotation());
	//	}

	//	// 啊靛 宏饭捞农档 啊靛 己傍栏肺 媚辑 眠啊炼扒 吧澜
	//	if (IsGuardSuccess(&DamageInfo) && DamageInfo.GuardBreakLogicType != EGuardBreakLogicType::EGLT_GuardBreak)
	//	{
	//		DamageEffectInfo->PlayGuardHitSound(this, GetActorLocation());
	//	}
	//	else
	//	{
	//		ABladeIICharacter* Attacker = GetAttacker(DamageCauser);
	//		if (DamageInfo.bUseHitSound && Attacker)
	//		{
	//			DamageEffectInfo->PlayHitSound(this, Attacker->CharacterType, GetActorLocation(), IsAboardMob(Attacker->GetNPCClass()));
	//		}
	//	}
	//}
}

ABladeIICharacter* ABladeIICharacter::GetAttacker(AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetAttacker);
	ABladeIICharacter* Attacker = Cast<ABladeIICharacter>(DamageCauser);
	//if (Attacker == nullptr)
	//{
	//	if (ABladeIIProjectile* Projectile = Cast<ABladeIIProjectile>(DamageCauser))
	//	{
	//		Attacker = Cast<ABladeIICharacter>(Projectile->GetOwner());
	//	}

	//	else if (AB2AreaDamageActorBase* AreaActor = Cast<AB2AreaDamageActorBase>(DamageCauser))
	//	{
	//		Attacker = Cast<ABladeIICharacter>(AreaActor->GetOwner());
	//	}
	//}

	return Attacker;
}

void ABladeIICharacter::AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
{
	//ProcessEtherSetEffect(EEtherSetType::Offense, Victim, DamageInfo, ActualDamage);

	//if (GetBuffManager())
	//	GetBuffManager()->AffectToAttacker(Victim, ActualDamage, DamageInfo);
}

void ABladeIICharacter::AffectToAttackerAfterVictimDie(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
{

}

bool ABladeIICharacter::IsLocalCharacter() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsLocalCharacter);
	ABladeIICharacter* LocalCharacter = GetLocalCharacter();
	return (LocalCharacter && LocalCharacter == this);
}

bool ABladeIICharacter::IsQTEActor(const ABladeIICharacter* OwnerCharacter) const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsQTEActor);
	if (OwnerCharacter && OwnerCharacter->GetQTEActor() == this)
		return true;

	return false;
}

ABladeIICharacter* ABladeIICharacter::GetLocalCharacter() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetLocalCharacter);

	//APlayerController* LocalController = UGameplayStatics::GetLocalPlayerController(this);
	//return LocalController ? Cast<ABladeIICharacter>(LocalController->GetPawn()) : nullptr;
	return nullptr;
}


void ABladeIICharacter::AddMinion(ABladeIICharacter* SingleMinion)
{
	if (SingleMinion && SingleMinion->IsAlive())
	{
		Minions.AddUnique(SingleMinion);
		SingleMinion->SetSummonOwner(this);
	}
}

void ABladeIICharacter::RemoveMinion(ABladeIICharacter* SingleMinion)
{
	if (SingleMinion && Minions.Find(SingleMinion))
	{
		SingleMinion->SetSummonOwner(nullptr);
		Minions.Remove(SingleMinion);
	}
}

float ABladeIICharacter::GetDamageByDamageLogic(const float OriginalDamage, EDamageLogicType DamageLogic, ABladeIICharacter* Attacker, AActor* DamageCauser, const FDamageInfo& DamageInfo, bool& OutIsCritical)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetDamageByDamageLogic);
	float ActualDamage = 0.f;

	// Handling Damage logic
	switch (DamageLogic)
	{
	case EDamageLogicType::ELT_Normal:
	{
		ActualDamage = GetNormalDamage(OriginalDamage, Attacker, DamageCauser, DamageInfo, OutIsCritical);
	}
	break;
	case EDamageLogicType::ELT_PctByTargetTotalHP:
	{
		ActualDamage = GetMaxHealth() * OriginalDamage * 0.01f;
	}
	break;
	case EDamageLogicType::ELT_PctByTargetCurrentHP:
	{
		ActualDamage = GetHealth() * OriginalDamage * 0.01f;
	}
	break;
	case EDamageLogicType::ELT_PctByTargetLostHP:
	{
		ActualDamage = (GetMaxHealth() - GetHealth()) * OriginalDamage * 0.01f;
	}
	break;
	case EDamageLogicType::ELT_PctByDealerTotalHP:
	{
		ActualDamage = Attacker->GetMaxHealth() * OriginalDamage * 0.01f;
	}
	break;
	case EDamageLogicType::ELT_PctByDealerCurrentHP:
	{
		ActualDamage = Attacker->GetHealth() * OriginalDamage * 0.01f;
	}
	break;
	case EDamageLogicType::ELT_PctByDealerLostHP:
	{
		ActualDamage = (Attacker->GetMaxHealth() - Attacker->GetHealth()) * OriginalDamage * 0.01f;
	}
	break;
	case EDamageLogicType::ELT_AttactCount:
	{
		ActualDamage = (this->GetMaxHealth()) / static_cast<float>(FullAttactCountSize);
	}
	break;
	case EDamageLogicType::ELT_Absolute:
	default:
	{
		ActualDamage = OriginalDamage;
	}
	break;
	}

	return ActualDamage;
}

float ABladeIICharacter::GetNormalDamage(const float OriginalDamage, ABladeIICharacter* Attacker, AActor* DamageCauser, const FDamageInfo& DamageInfo, bool& OutIsCritical)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetNormalDamage);
	/*
	"弥辆 单固瘤 = 醚 傍拜仿 (林某腐磐 傍拜仿 + 林某腐磐 公扁 傍拜仿 + 何某腐磐 傍拜仿 + 何某腐磐 公扁 傍拜仿)
		x 林某腐磐 厘厚 可记	1. EnemyTotalAttack ( 醚傍拜仿 * 林某腐磐 厘厚 可记 * 0.01f)
		x 硅啦 炼沥			2. OriginalDamage
		x 单固瘤 利侩啦		3. TotalDamageRate ( LevelWeight * DamageRate * CharacterDamageRate(扁夯100) * 0.01f)
		x 罚待 单固瘤"		4. RandomDamage
		x 弥辆 单固瘤 硅啦		5. 弥措 单固瘤 档崔 规瘤甫 困窃
		x 农府萍拿 单固瘤
	*/

	//BII_CHECK(Attacker);

	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//// 1. EnemyTotalAttack ( 醚傍拜仿 * 林某腐磐 厘厚 可记 * 0.01f)
	//const float AttackCoefficient = (GameMode != nullptr) ? GameMode->GetAttackCoefficient() : CombatStatEval::GetDefaultCombatCoefficient();
	//const float EnemyTotalAttack = Attacker->TotalAttack * 0.01f * AttackCoefficient;

	//// 3. TotalDamageRate(IncreaseRate * DamageRate * ExtraDamageRate * CharacterDamageRate(扁夯100) * 0.01f)
	//const float IncreaseRate = GetBaseDamageIncreaseRate(Attacker, DamageInfo, DamageCauser);
	//const float DamageRate = GetDamageRate(Attacker, DamageInfo, DamageCauser);
	//const float ExtraDamageRate = GetExtraTakeDamageRate();	// ex) Poision 滚橇矫 10% 单固瘤 歹

	//const float TotalDamageRate = IncreaseRate * DamageRate * ExtraDamageRate * (CharacterDamageRate * 0.01f/*扁夯 100*/);

	//// 4. RandomDamage
	//const float RandomDamage = (GameMode != nullptr) ? GameMode->GetDamageRandomRate() : 1.f;

	//// 5. 弥辆 硅啦 Adjustment
	//// 抄捞档 炼例侩 - 单固瘤 99999 档崔 规瘤甫 困茄 hp / Damage 厚肥 惑荐
	//const float AdjustmentRate = IsMob() ? GameMode->GetPVEProportionalConstant() : 1.f;

	//// 弥辆 Damage 拌魂 柳青

	//// Step 1. Critical, 闺繁教 炼例 傈
	//float ActualDamage = EnemyTotalAttack
	//	* OriginalDamage
	//	* TotalDamageRate
	//	* RandomDamage
	//	* AdjustmentRate;

	//// Step 2. Critical 单固瘤 拌魂
	//float CriticalRate = DamageInfo.CriticalRateOverride == -1 ? Attacker->GetCriticalRate() : DamageInfo.CriticalRateOverride;
	//CriticalRate *= GetResistCriticalRate();
	//CriticalRate = FMath::Clamp(CriticalRate, 0.f, 1.f);
	//OutIsCritical = FMath::FRand() < CriticalRate;
	//const float CriticalDamageRate = OutIsCritical ? (DamageInfo.CriticalDamageRateOverride == -1 ? Attacker->GetCriticalDamageRate() : DamageInfo.CriticalDamageRateOverride) : 1.f;

	//ActualDamage *= CriticalDamageRate;

	//// Step3. PVP 老 版快 单固瘤 焊沥 - 弥措眉仿 20% 捞惑狼 单固瘤(趣篮 葛靛喊 沥狼等 弥措樊)绰 茄规俊 持阑 荐 绝促
	//if (GameMode && GameMode->IsPVPGameMode())
	//{
	//	const float MaxActualDamageRatio = GameMode->GetPVPMaxDamageRatioByGameModeType();
	//	ActualDamage = FMath::Min<float>(ActualDamage, GetMaxHealth() * MaxActualDamageRatio);

	//	if (IsApplyGuarding())
	//		ActualDamage *= 0.2f;
	//}

	////step4. Additional Damage 贸府 弊成 Max(0, (傍拜磊 IncAdditionalDamage - 规绢磊 DecAdditionalDamage))
	//ActualDamage += GetAdditionalDamage(Attacker);

	//return ActualDamage;
	return 0.0;
}

float ABladeIICharacter::GetScaledAttackLevel()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetScaledAttackLevel);
	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//BII_CHECK(GameMode);

	//const float DifficultyScale = GameMode->GetCurrentStageDifficultyScale();
	//return FinalADScale * DifficultyScale;
	return 0.0;
}

float ABladeIICharacter::GetScaledDefenceLevel()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetScaledDefenceLevel);
	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//BII_CHECK(GameMode);

	//const float DifficultyScale = GameMode->GetCurrentStageDifficultyScale();
	//return FinalADScale * DifficultyScale;
	return 0.0;
}

float ABladeIICharacter::GetBaseDamageIncreaseRate(ABladeIICharacter* Attacker, const FDamageInfo& DamageInfo, AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetBaseDamageIncreaseRate);
	//BII_CHECK(Attacker);

	const bool IsEnemyPlayer = Attacker->IsPlayerCharacter();
	const bool IsVictimPlayer = IsPlayerCharacter();

	float IncreaseRate = Attacker->GetDamageIncreaseRate();	// 刘啊蔼捞骨肺 1.f + @ 肺 登绢具 窃 -> ( + 楷魂 )

	// 10%刘啊搁 0.1肺 蔼 涝仿登菌促绊 积阿茄促.
	if (DamageInfo.DamageIncreaseRateOverride != -1)
		IncreaseRate += DamageInfo.DamageIncreaseRateOverride;

	float DecreaseRate = GetDamageDecreaseRate();			// 皑家蔼捞骨肺 1.f - @ 肺 登绢具 窃 -> ( * 楷魂 )

	if (IsVictimPlayer && DamageCauser)
	{
		// 傍拜磊客 角力 啊拜 Actor啊 鞍捞 臼栏搁 RangeAttack栏肺 魄喊 ( Projectile, DamageAreaActor 殿殿 )
		const bool IsRangeAttack = Attacker != DamageCauser ? true : false;
		const float RangeDecreaseRate = IsRangeAttack ? GetRangeDamageDecreaseRate() : GetMeleeDamageDecreaseRate();
		DecreaseRate *= RangeDecreaseRate;
	}
	// Increase / Decrease 硅啦 炼沥

	// 1. Player(Victim) vs Player(Attacker)
	if (IsVictimPlayer && IsEnemyPlayer)
	{
		IncreaseRate += Attacker->GetAttackDamageIncreaseRateByClass(GetCurrentPlayerClass());
	}

	// 2. Mob(Victim) vs Player(Attacker)
	else if (!IsVictimPlayer && IsEnemyPlayer)
	{
		if (IsBossMob())
			IncreaseRate += DamageInfo.BossDamageIncreaseRateOverride == -1 ? Attacker->GetBossDamageIncreaseRate() : DamageInfo.BossDamageIncreaseRateOverride;
	}

	else
	{
		// Player(Victim) vs Mob(Attacker)
		if (IsVictimPlayer && IsBossMob())
			DecreaseRate *= GetBossDamageDecreaseRate();
	}

	return IncreaseRate * DecreaseRate;
}

float ABladeIICharacter::GetDamageRate(ABladeIICharacter* Attacker, const FDamageInfo& DamageInfo, AActor* DamageCauser)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetDamageRate);
	//BII_CHECK(Attacker);

	const float ScaledAttackLevel = Attacker->GetScaledAttackLevel();
	const float ScaledDefenceLevel = GetScaledDefenceLevel();
	const float LevelWeight = 0.1f;

	float DamageRate = 1.f + ((ScaledAttackLevel - ScaledDefenceLevel) * LevelWeight);

	const bool IsEnemyPlayer = Attacker->IsPlayerCharacter();
	const bool IsVictimPlayer = IsPlayerCharacter();

	// 1. Player(Victim) vs Player(Attacker)	- 1 措 1 
	//if (IsVictimPlayer && IsEnemyPlayer)
	//{
	//	ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	const float PVPDamageScale = (GameMode != nullptr) ? GameMode->GetPVPDamageScaleByGameModeType() : CombatStatEval::GetDefaultPVPDamageScale();

	//	// PVP 单固瘤 闺繁胶 炼沥
	//	// 1. 30% ~ 300% 
	//	DamageRate = FMath::Clamp(DamageRate, 0.3f, 3.f);

	//	// 2. PVPScale 单固瘤 利侩
	//	DamageRate *= PVPDamageScale;
	//}

	// 2. Mob(Victim) vs Player(Attacker) - 敲饭捞绢啊 各阑 傍拜
	//else if (IsEnemyPlayer)
	//{
	//	// NPC 单固瘤 茄拌 炼例 4% ~ 500%
	//	DamageRate = FMath::Clamp(DamageRate, 0.04f, 5.f);
	//}
	// 3. 唱赣瘤版快 - Mob(Victim) vs Mob(Attacker) / Player(Victim) vs Mob(Attacker)
	// 老馆利牢 版快肺 胶抛捞瘤 葛靛俊辑 各茄抛 嘎疽阑锭
	//else
	//{
	//	DamageRate = FMath::Clamp(DamageRate, 0.01f, 10.f);
	//}

	return DamageRate;
}

float ABladeIICharacter::GetAdditionalDamage(ABladeIICharacter* Attacker)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetAdditionalDamage);
	BII_CHECK(Attacker);

	ABladeIIGameMode* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	float ModeDamageRate = 1.0f;

	const bool IsEnemyPlayer = Attacker->IsPlayerCharacter();
	const bool IsVictimPlayer = IsPlayerCharacter();


	//if (B2GameMode != nullptr)
	//{
	//	if (B2GameMode->IsPVPGameMode() && IsEnemyPlayer && IsVictimPlayer)
	//		ModeDamageRate = 1.0f;
	//	else if (IsMob())
	//		ModeDamageRate = B2GameMode->GetPVEProportionalConstant();
	//}

	const float AdditionalDamage = FMath::Max(0.0f, (Attacker->GetIncAdditionalDamage() - GetDecAdditionalDamage())) * ModeDamageRate;
	return AdditionalDamage;
}

void ABladeIICharacter::UpdateArmorByDamageInfo(const FDamageInfo& DamageInfo)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_UpdateArmorByDamageInfo);

	// Armor change
	if (Armor <= 0.f)
		RecoverArmor();  // Defered Recover armor

	const auto GuardBreakType = DamageInfo.GuardBreakLogicType;
	const bool bGuardBreakDamage = GuardBreakType != EGuardBreakLogicType::EGLT_AbleToGuard;
	const bool bAllowArmorUpdate = !IsFrozen() && (!IsGuarding() || bGuardBreakDamage);

	if (bAllowArmorUpdate)
	{
		if (DamageInfo.ArmorBreakLogicType == EArmorBreakLogicType::EALT_AlwaysBreak)
			Armor = 0.f;

		else if (!IsGuarding() && DamageInfo.ArmorBreakAmount > 0.f)
			Armor -= DamageInfo.ArmorBreakAmount;
	}
}

USkeletalMeshComponent* ABladeIICharacter::GetBaseMesh() const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetBaseMesh);
	return GetMesh(); // 狼固惑 GetMesh 甫 流立 静扁焊促绰 GetBaseMesh 甫 荤侩且 巴阑 鼻厘.
}

void ABladeIICharacter::UpdateDamageInfo(const FDamageInfo& InDamageInfo)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_UpdateDamageInfo);
	//CurrentDamageInfo.SetDamageInfo(&InDamageInfo, this);
}

void ABladeIICharacter::ShootProjectile(float Damage, FName BoneName, const FDamageInfo& InDamageInfo, bool bOverrideDamageInfo, AActor* TargetActor, bool bUseSocketName, int32 ProjIndex, FRotator OffsetRotation, float ShootSpeed, float MaxLifeTime, bool bPenetrate, bool bIgnoreBoneRotation, float FreezeDuration)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ShootProjectile);
	////B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::ShootProjectile"));

	//if (ProjectileClasses.Num() > 0)
	//{
	//	ProjIndex = FMath::Clamp(ProjIndex, 0, ProjectileClasses.Num() - 1);
	//	TSubclassOf<class ABladeIIProjectile>& ProjectileClass = ProjectileClasses[ProjIndex];
	//	// Launch projectile
	//	FRotator FinalDir = bIgnoreBoneRotation ? GetActorRotation() : (bUseSocketName ? GetMesh()->GetSocketQuaternion(BoneName).Rotator() : GetMesh()->GetBoneQuaternion(BoneName).Rotator());
	//	FinalDir += OffsetRotation;

	//	// 夯 困摹
	//	FVector vBoneLocation = bUseSocketName ? GetMesh()->GetSocketLocation(BoneName) : GetMesh()->GetBoneLocation(BoneName);

	//	// 鸥百捞 惑窜捞唱 窍窜俊 乐阑 版快甫 措厚秦辑 pitch 父 浇陆 嘎苗霖促.
	//	// 咀磐困摹 扁霖栏肺 窍带吧 夯困摹 扁馆栏肺 官厕.
	//	FRotator DirectRotationToTarget = GetDirectionToTargetCharacter(vBoneLocation).Rotation();

	//	// 乔摹 焊沥篮 夯肺抛捞记 公矫且锭父.. 窍绊酵瘤父 扁粮 单捞磐技泼锭巩俊 弊成埃促.
	//	// 公炼扒 纳腐磐 臭捞 眠利窍绰芭烙.
	//	FinalDir.Pitch = DirectRotationToTarget.Pitch;

	//	FTransform SpawnTM(FinalDir, bUseSocketName ? GetMesh()->GetSocketLocation(BoneName) : GetMesh()->GetBoneLocation(BoneName));
	//	ABladeIIProjectile* Projectile = Cast<ABladeIIProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTM));
	//	BII_CHECK(Projectile && Projectile->IsValidObj());

	//	if (Projectile)
	//	{
	//		Projectile->Instigator = Instigator;
	//		Projectile->SetOwner(this);
	//		Projectile->InitProjectile(FinalDir.Vector(), Damage, TargetActor, ShootSpeed, MaxLifeTime, bPenetrate, bOverrideDamageInfo ? &InDamageInfo : NULL, FreezeDuration);

	//		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);
	//	}
	//}
}

void ABladeIICharacter::CastAreaDamage_DamageZoneSkillIndex(float Damage, const FDamageInfo& InDamageInfo, bool bOverrideDamageInfo, const FVector& TargetLocation, int32 ClassIndex, float Radius, float CastingTime, float Interval, float LifeTime)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_CastAreaDamage_DamageZoneSkillIndex);
	//if (AreaDamageClasses.IsValidIndex(ClassIndex))
	//{
	//	TWeakObjectPtr<AB2AreaDamageActorBase> AreaDamage(AB2AreaDamageActorBase::SpawnDamageAreaActor(AreaDamageClasses[ClassIndex], this, InDamageInfo, bOverrideDamageInfo, TargetLocation, Damage, Radius, CastingTime, Interval, LifeTime));

	//	if (AreaDamage.IsValid())
	//	{
	//		auto SubscribeKey = CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Subscribe([AreaDamage](ABladeIICharacter* InOwner)
	//			{
	//				if (AreaDamage.IsValid() && InOwner == AreaDamage->GetOwner())
	//				{
	//					AreaDamage->CancelCastAreaDamage_CallBack();
	//				}
	//			});

	//		EventCancelCastAreaDamageSubscribeKeys.Add(SubscribeKey);

	//		AreaDamage->SetCancelSubscribeKey(SubscribeKey);
	//	}
	//}
}

void ABladeIICharacter::CastAreaDamage_SpecifyFx(float Damage, const FDamageInfo& InDamageInfo, bool bOverrideDamageInfo, const FVector& TargetLocation, const FLinearColor& WarningFxColor, float Radius, float CastingTime, float Interval, float LifeTime, float FirstImpactTime, class UParticleSystem* WarningFX, class UParticleSystem* ActivatingFX, FVector WarningFxScale)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_CastAreaDamage_SpecifyFx);
	//TWeakObjectPtr<AB2AreaDamageActorBase> AreaDamage(AB2AreaDamageActorBase::SpawnDamageAreaActor(AB2AreaDamageActor::StaticClass(), this, InDamageInfo, bOverrideDamageInfo, TargetLocation, Damage, Radius, CastingTime, Interval, LifeTime, FirstImpactTime, WarningFX, ActivatingFX, WarningFxColor, WarningFxScale));

	//if (AreaDamage.IsValid())
	//{
	//	auto SubscribeKey = CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Subscribe([AreaDamage](ABladeIICharacter* InOwner)
	//		{
	//			if (AreaDamage.IsValid() && InOwner == AreaDamage->GetOwner())
	//			{
	//				AreaDamage->CancelCastAreaDamage_CallBack();
	//			}
	//		});

	//	EventCancelCastAreaDamageSubscribeKeys.Add(SubscribeKey);

	//	AreaDamage->SetCancelSubscribeKey(SubscribeKey);
	//}
}

bool ABladeIICharacter::GetAreaDamageTargetLocation(float RandMaxDist, FVector& OutTargetLocation, const FVector& LocationOffset, bool bSelfTarget)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetAreaDamageTargetLocation);
	//OutTargetLocation = FVector::ZeroVector;

	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	//ABladeIICharacter* Target = nullptr;

	//if (bSelfTarget)
	//	Target = this;
	//else // 啊厘 啊鳖款 player character 甫 鸥百栏肺. (AIBlueprintSearchPlayer)
	//	Target = Cast<ABladeIICharacter>(pGM->GetNearestPlayerCharacter(this->GetActorLocation()));

	//if (Target)
	//{
	//	InnerGetAreaDamageTargetLocation(Target, RandMaxDist, OutTargetLocation, LocationOffset);
	//	return true;
	//}

	return false;
}

void ABladeIICharacter::InnerGetAreaDamageTargetLocation(ABladeIICharacter* TargetCharacter, float RandMaxDist, FVector& OutTargetLocation, const FVector& LocationOffset)
{
	if (TargetCharacter)
	{
		FVector TargetLocation = TargetCharacter->ActorToWorld().TransformPosition(LocationOffset);

		float RandDist = FMath::FRandRange(0.f, RandMaxDist);
		FVector RandOffset = RandDist * FRotator(0.f, FMath::FRandRange(-180.f, 180.f), 0.f).Vector();

		OutTargetLocation = TargetLocation + RandOffset;

		//官蹿 茫扁

		FHitResult Hit;
		FVector Start = OutTargetLocation;
		FVector End = Start + FVector(0.f, 0.f, -1200.f);

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, FCollisionQueryParams(NAME_None, false, TargetCharacter));

		if (Hit.bBlockingHit)
		{
			OutTargetLocation = Hit.Location + FVector(0.f, 0.f, 10.f);
		}
	}
}

AActor* ABladeIICharacter::GetTeleportDestination(FName ActorTagName)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetTeleportDestination);
	//auto* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	return /*B2GM ? B2GM->GetTaggedActor(ActorTagName) :*/ NULL;
}

FVector ABladeIICharacter::GetTopLocation()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetTopLocation);
	FVector vLocation = GetActorLocation();

	//UCapsuleComponent* pComp = Cast<UCapsuleComponent>(RootComponent);

	//if (pComp)
	//	vLocation.Z += pComp->GetScaledCapsuleHalfHeight();

	return vLocation;
}

FVector ABladeIICharacter::GetDirectionToTargetCharacter()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetDirectionToTargetCharacter);
	//// 啊厘 啊鳖款 player character 甫 鸥百栏肺. (AIBlueprintSearchPlayer)
	//ABladeIICharacter* NearestCharacter = Cast<ABladeIICharacter>(UGameplayStatics::GetNearestPlayerCharacter(this, this->GetActorLocation()));
	//if (IsTargetableCharacter(NearestCharacter))
	//{
	//	FVector ToTarget = NearestCharacter->GetActorLocation() - this->GetActorLocation();
	//	ToTarget.Normalize();
	//	return ToTarget;
	//}

	return FVector(1.0f, 0.0f, 0.0f);
}

FVector ABladeIICharacter::GetDirectionToTargetCharacter(FVector& BoneLocation)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetDirectionToTargetCharacter_BoneLocation);
	//// 啊厘 啊鳖款 player character 甫 鸥百栏肺. (AIBlueprintSearchPlayer)
	//ABladeIICharacter* NearestCharacter = Cast<ABladeIICharacter>(UGameplayStatics::GetNearestPlayerCharacter(this, this->GetActorLocation()));
	//if (IsTargetableCharacter(NearestCharacter))
	//{
	//	FVector ToTarget = NearestCharacter->GetActorLocation() - BoneLocation;
	//	ToTarget.Normalize();
	//	return ToTarget;
	//}

	return FVector(1.0f, 0.0f, 0.0f);
}

bool ABladeIICharacter::IsTargetableCharacter(ABladeIICharacter* TargetCharacter)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsTargetableCharacter);
	//BII_CHECK(TargetCharacter && TargetCharacter->IsValidObj());
	//if (TargetCharacter && TargetCharacter != nullptr)
	//{
	//	if (TargetCharacter->IsPlayerControlled())	// [Mob -> Player 鸥百] , [Player -> TargetActor & TargetObject 鸥百]
	//		return true;
	//}

	return false;
}

void ABladeIICharacter::CheckQTEEnable(ETeamType HitTeam)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_CheckQTEEnable);
	if (!bQTEEnabled && !bInQTEState && !IsPlayerControlled() && QTEInvokeHPPercent > 0.f &&
		((QTEType == EQTEType::EQT_Normal || QTEType == EQTEType::EQT_Mount) && (Health / MaxHealth) * 100.f < QTEInvokeHPPercent
			|| (QTEType == EQTEType::EQT_Break && CurrentQTEBreakPoint <= 0.f))
		)
	{
		ApplyQTEEnable(HitTeam);
	}
}

void ABladeIICharacter::LocationAdjustCharacterOverlap(float DeltaSeconds)
{
	//if (IsRemoteCharacter())
	//	return;

	//// 檬寸 捞悼矫懦 氦磐荤捞令
	//float fAdjustAmountPerSec = 200.f;

	//// 般摹搁 剐绢郴焊磊(this甫).
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	ABladeIICharacter* pChar = Cast<ABladeIICharacter>(*Iterator);

	//	if (pChar && pChar != this)
	//	{
	//		// 辑肺 喉废且锭父 贸府
	//		bool bNeedProcess = pChar->GetCapsuleComponent()->GetCollisionResponseToChannel(GetCapsuleComponent()->GetCollisionObjectType()) == ECollisionResponse::ECR_Block;
	//		bNeedProcess = bNeedProcess && GetCapsuleComponent()->GetCollisionResponseToChannel(pChar->GetCapsuleComponent()->GetCollisionObjectType()) == ECollisionResponse::ECR_Block;

	//		if (bNeedProcess)
	//		{
	//			FVector LengthVec = GetActorLocation() - pChar->GetActorLocation();

	//			if (LengthVec.Size() < GetCapsuleComponent()->GetScaledCapsuleRadius() + pChar->GetCapsuleComponent()->GetScaledCapsuleRadius())
	//			{
	//				FVector vAdjustDir = LengthVec;
	//				vAdjustDir.Z = 0;
	//				vAdjustDir = vAdjustDir.GetSafeNormal();

	//				FHitResult AdjustHitResult = FHitResult();

	//				SetActorLocation(GetActorLocation() + (vAdjustDir * DeltaSeconds * fAdjustAmountPerSec), true, &AdjustHitResult);

	//				// sweep栏肺 购啊俊 阜躯栏搁 馆措规氢栏肺粱 剐绢淋.
	//				if (AdjustHitResult.bBlockingHit)
	//				{
	//					vAdjustDir = GetActorLocation() - AdjustHitResult.Location;
	//					vAdjustDir.Z = 0;
	//					vAdjustDir = vAdjustDir.GetSafeNormal();

	//					SetActorLocation(GetActorLocation() + (vAdjustDir * DeltaSeconds * fAdjustAmountPerSec));
	//				}
	//			}
	//		}
	//	}
	//}
}

void ABladeIICharacter::SetStageEventShowState(EStageEventShowState InNewState, class AB2StageEventDirector* InDirectorActor)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetStageEventShowState);
	//if (InDirectorActor != NULL)
	//{
	//	// 捞饶 state 甫 罐酒辑 AnimBP 俊辑 贸府窍哥 荤侩窍绰 AnimMontage 俊悸 曼炼绰 GetPCSESAnimTableObject 甫 烹秦辑. 
	//	// -> 弊率篮 力芭登菌绊 咯扁档 促弗 眠啊 贸府甸捞 狼固啊 绝绢瘤搁 力芭瞪 荐 乐澜.
	//	CachedStageEventDirector = InDirectorActor;
	//	StageEventShowState = InNewState;
	//}
	//else
	//{
	//	if (InNewState == EStageEventShowState::ESES_None)
	//	{
	//		// InDirectorActor 啊 NULL 牢 版快扼档 None 栏肺 技泼窍绰 版快扼搁 罐酒甸烙. StageEventDirector 率栏肺狼 喊促弗 Notify 绝捞 炼侩洒 惑怕甫 辆丰窍绰 侩档.
	//		CachedStageEventDirector = NULL;
	//		StageEventShowState = EStageEventShowState::ESES_None;
	//	}
	//}
}

void ABladeIICharacter::NotifyStageEventShowEnd()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_NotifyStageEventShowEnd);
	//StageEventShowState = EStageEventShowState::ESES_None;

	//if (CachedStageEventDirector)
	//{
	//	CachedStageEventDirector->NotifyStageEventShowOver(this); // 混娄 促弗 匙捞怪 偿偿
	//	CachedStageEventDirector = NULL; // Notify 饶 饭欺繁胶 力芭.
	//}
}

void ABladeIICharacter::ApplyQTEEnable(ETeamType HitTeam /*= ETeamType::ETT_End*/)
{
	//SetQTEEnabled(true);
	//QTEStateNum = 0;
	//QTEEnableTeam = HitTeam;

	//if (QTEType == EQTEType::EQT_Break) // On QTE Break..
	//{
	//	UB2BuffModeEffectInfo* BuffModeEffectInfo = GetBuffModeEffectInfo();
	//	if (BuffModeEffectInfo)
	//	{
	//		BuffModeEffectInfo->SpawnBreakTextEffect(this);
	//	}
	//}
}

// 母蕉农扁 器窃矫 敲饭捞绢 夯牢搏 绊妨救窃.
FVector ABladeIICharacter::GetTargetLocationVectorFromPlayer(AActor* pTargetActor, bool bIncludeCapsule/* = true*/)
{
	FVector Diff = FVector::ZeroVector;

	if (pTargetActor)
	{
		FVector SelfLocation = GetActorLocation();
		FVector TargetLocation = pTargetActor->GetActorLocation();

		SelfLocation.Z = 0;
		TargetLocation.Z = 0;

		Diff = TargetLocation - SelfLocation;

		ABladeIICharacter* pTargetCharacter = Cast<ABladeIICharacter>(pTargetActor);

		// 鸥百捞 纳腐磐啊酒聪搁 母蕉拌魂 给秦淋
		if (bIncludeCapsule && pTargetCharacter)
		{
			float fDistance = Diff.Size2D() - pTargetCharacter->GetScaledCapsuleRadius();

			if (fDistance <= 0)
				fDistance = 0;

			Diff *= fDistance / Diff.Size2D();
		}
	}

	return Diff;
}

UB2DamageEffectInfo* ABladeIICharacter::GetDamageEffectInfo()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetDamageEffectInfo);
	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	//return B2GI ? B2GI->GetDamageEffectInfo() : nullptr;
	return nullptr;
}

class UB2Buff_Base* ABladeIICharacter::AddBuff(EBuffType BuffType, float fDuration, float Amount, AActor* BuffCauser, bool bUseEffect/* = true*/, bool bUseTextEffect/* = true*/, TArray<float>* PtrAdditionalParams/* = nullptr*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_AddBuff);
	//if (!IsAllowBuff(BuffType))
		return nullptr;

	//return BuffManager->AddBuff(BuffType, fDuration, Amount, BuffCauser, bUseEffect, bUseTextEffect, PtrAdditionalParams);
}

void ABladeIICharacter::AddBuffEnemy(EBuffType BuffType, float fDistance, float fDuration, float Amount, bool bUseEffect/* = true*/, bool bUseTextEffect/* = true*/)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_AddBuffEnemy);
	//TArray<ABladeIICharacter*> Enemies;
	//GetEnemiesInRadius(fDistance, Enemies);

	//for (ABladeIICharacter* Enemy : Enemies)
	//{
	//	Enemy->AddBuff(BuffType, fDuration, Amount, this, bUseEffect, bUseTextEffect);
	//}
}

void ABladeIICharacter::ClearBuff(EBuffType BuffType)
{
	//BuffManager->ClearBuff(BuffType);

	//UB2Buff_Base* pBuff = BuffManager->GetBuff<UB2Buff_Base>(BuffType);

	//if (pBuff && pBuff->IsUseCrowdControlCount())
	//	BuffManager->ResetCrowdControlCount();
}

void ABladeIICharacter::ClearAllBuffs()
{
//	BuffManager->ClearAllBuffs();
//
//	BuffManager->ResetCrowdControlCount();
}

void ABladeIICharacter::ClearDebuffs()
{
	//BuffManager->ClearDebuffs();

	//BuffManager->ResetCrowdControlCount();
}

void ABladeIICharacter::UpdateDamageNum(const FDamageInfo& DamageInfo)
{
	if (GetLocalRole() == ROLE_Authority && DamageInfo.bIgnoreIncreaseDamageNum == false)
		IncreaseDamageNum();
}

void ABladeIICharacter::ResetDamageNum()
{
	SetDamagedNum(0);
}

void ABladeIICharacter::OnDamageEnd()
{
	ResetDamageNum();
	LastDamageType = EAttackType::EAT_Default;
	RandDamageAnimType = EAttackType::EAT_Default;
	ResetFlash();
}

void ABladeIICharacter::SetDamagedNum(int32 NewDamagedNum)
{
	if (GetLocalRole() == ROLE_Authority)
		DamagedNum = NewDamagedNum;
}

void ABladeIICharacter::ClearAttachedParticles()
{
	/*
	if (auto* MeshComp = GetMesh())
	{
		const TArray<USceneComponent*>& ChildSceneComps = MeshComp->GetAttachChildren();
		for(int32 Child = 0; Child < ChildSceneComps.Num(); Child++)
		{
			auto* ScenceComp = ChildSceneComps[Child];
			if (auto* Particle = Cast<UParticleSystemComponent>(ScenceComp))
			{

			}
		}
	}
	*/
}

bool ABladeIICharacter::IsAllowState(EStateAttackType StateAttack)
{
	return AllowedDamageStateData.IsAllowedState(StateAttack);
}

bool ABladeIICharacter::IsAllowBuff(EBuffType InBuffType)
{
	return AllowedDamageStateData.IsAllowedBuff(InBuffType);
}

void ABladeIICharacter::GetEnemiesInRadius(const float Radius, TArray<ABladeIICharacter*>& OutEnemies, float FanAngle)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetEnemiesInRadius);
	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//BII_CHECK(GameMode);

	//const float RadiusSq = Radius * Radius;
	//OutEnemies.Empty();

	//TArray<ABladeIICharacter*> EnemyList;
	//GameMode->GetAllWorldEnemyList(EnemyList, GetTeamNum());

	//for (ABladeIICharacter* Enemy : EnemyList)
	//{
	//	if (Enemy && Enemy->IsPendingKill() == false)
	//	{
	//		// 盔烹屈栏肺 Radius甫 炼荤
	//		const float DistSq = FVector::DistSquaredXY(Enemy->GetActorLocation(), GetActorLocation());
	//		if (DistSq <= RadiusSq)
	//		{
	//			if (!FMath::IsNearlyEqual(FanAngle, 180.0f)) // 眉农 裹困啊 360档扼搁 // 郴利捞 (-1)180档扼搁
	//			{
	//				if (FMath::RadiansToDegrees(acosf(GetDotProductTo(Enemy))) <= FanAngle)
	//					OutEnemies.Add(Enemy);
	//			}
	//			else
	//			{
	//				OutEnemies.Add(Enemy);
	//			}
	//		}
	//	}
	//}
}

UB2BuffModeEffectInfo* ABladeIICharacter::GetBuffModeEffectInfo()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetBuffModeEffectInfo);
	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	//return B2GI ? B2GI->GetBuffModeEffectInfo() : nullptr;
	return nullptr;
}

void ABladeIICharacter::PauseForDuration(float Duration)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_PauseForDuration);

	//if (auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this)))
	//{
	//	if (GameMode->GetGameRule()->IsHitFreezeEnabled())
	//	{
	//		GameMode->OverridePauseForDuration(Duration, this);
	//		AddBuff(EBuffType::Buff_HitFreeze, Duration);
	//	}
	//}
}

bool ABladeIICharacter::ShouldIgnoreTimeDilation() const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ShouldIgnoreTimeDilation);
	// Ignore Time Dilation when it is frozen
	return IsBuffActive(EBuffType::Debuff_Freezing);
}

void ABladeIICharacter::FlashForDuration(FName FlashColorName, FLinearColor FlashColor, FName FlashIntensityName, float FlashIntensity, float Duration)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_FlashForDuration);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::FlashForDuration"));

	//if (GetMesh() && Duration > SMALL_NUMBER && FlashIntensity > SMALL_NUMBER && FlashIntensityName != NAME_None)
	//{
	//	HitFlashIntensityName = FlashIntensityName;
	//	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
	//	{
	//		UMaterialInterface* MaterialInterface = GetMesh()->GetMaterial(i);
	//		if (MaterialInterface)
	//		{
	//			UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
	//			if (!DynamicMaterial) //Is it already a UMaterialInstanceDynamic
	//			{
	//				DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
	//			}
	//			if (FlashColorName != NAME_None)
	//			{
	//				DynamicMaterial->SetVectorParameterValue(FlashColorName, FlashColor);
	//			}
	//			DynamicMaterial->SetScalarParameterValue(FlashIntensityName, FlashIntensity);
	//		}
	//	}

	//	GetWorldTimerManager().SetTimer(SetHitFlashTimerHandle, this, &ABladeIICharacter::ResetFlash, Duration, false);
	//}
}


void ABladeIICharacter::WrappedSetLifeSpan(float InLifespan)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_WrappedSetLifeSpan);
	SetRole(ENetRole::ROLE_Authority);

	// Legacy for QTEState check on this.
	SetLifeSpan(InLifespan);
}

void ABladeIICharacter::SetPvPTutorialHandicap(const ABladeIICharacter* InPlayer, float InStatsPercent)
{
	MaxHealth = InPlayer->MaxHealth * InStatsPercent;
	TotalAttack = InPlayer->TotalAttack * InStatsPercent;
	TotalDefense = InPlayer->TotalDefense * InStatsPercent;

	Health = MaxHealth;
}

void ABladeIICharacter::SetCustomTimeDilation(float NewDilation)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetCustomTimeDilation);
	CustomTimeDilation = NewDilation;
}

void ABladeIICharacter::ResetFlash()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ResetFlash);
	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
	{
		UMaterialInterface* MaterialInterface = GetMesh()->GetMaterial(i);
		if (MaterialInterface)
		{
			UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
			if (DynamicMaterial)
			{
				// Only Intensity change
				DynamicMaterial->SetScalarParameterValue(HitFlashIntensityName, 0.f);
			}
		}
	}

	GetWorldTimerManager().ClearTimer(SetHitFlashTimerHandle);
}
// 
// void ABladeIICharacter::ResetIncreaseArmor()
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ResetIncreaseArmor);
// 	CurrentIncreaseArmor = 0.f;
// }
// 
// void ABladeIICharacter::ResetInvincible()
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ResetInvincible);
// 	SetInvincible(false);
// 	::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);
// 	SetCachedInvincibleBuffState(false, 0.0f);
// }
// 
// void ABladeIICharacter::ResetIncreaseAttackDamage()
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ResetIncreaseAttackDamage);
// 	BaseDamageRate = 1.f;
// }
// 
// void ABladeIICharacter::ResetDecreaseReceiveDamage()
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ResetDecreaseReceiveDamage);
// 	DamageMultiplier = 1.f;
// }
// 
// void ABladeIICharacter::ResetIncreaseMoveSpeed()
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ResetIncreaseMoveSpeed);
// 	check(GetCharacterMovement() && CachedBaseMaxWalkSpeed > 0.f);
// 	GetCharacterMovement()->MaxWalkSpeed = GetStaticOptionAppliedWalkSpeed();
// }
// 
// void ABladeIICharacter::ResetHealHPByAttack()
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ResetHealHPByAttack);
// 	HealHPByAttackRate = 0.f;
// }
// 
// void ABladeIICharacter::ResetStateAttack()
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ResetStateAttack);
// 	StateAttackType = EStateAttackType::EDT_End;
// 	StateAttackDuration = 0.f;
// 	StateAttackAmount = 0.f;
// }

void ABladeIICharacter::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_FaceRotation);
	const FRotator CurrentRotation = GetActorRotation();

	if (!bUseControllerRotationPitch)
	{
		NewControlRotation.Pitch = CurrentRotation.Pitch;
	}

	if (!bUseControllerRotationYaw)
	{
		NewControlRotation.Yaw = CurrentRotation.Yaw;
	}

	if (!bUseControllerRotationRoll)
	{
		NewControlRotation.Roll = CurrentRotation.Roll;
	}

	// leejswo : Added Rotation Rate limit
	//if (GetCharacterMovement())
	{
		FRotator DeltaRot = GetCharacterMovement()->GetDeltaRotation(DeltaTime);
		//YAW
		if (NewControlRotation.Yaw != CurrentRotation.Yaw)
		{
			NewControlRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, NewControlRotation.Yaw, DeltaRot.Yaw);
		}

		// PITCH
		if (NewControlRotation.Pitch != CurrentRotation.Pitch)
		{
			NewControlRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, NewControlRotation.Pitch, DeltaRot.Pitch);
		}

		// ROLL
		if (NewControlRotation.Roll != CurrentRotation.Roll)
		{
			NewControlRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, NewControlRotation.Roll, DeltaRot.Roll);
		}
	}

	SetActorRotation(NewControlRotation);
}

float ABladeIICharacter::GetDamageIncreaseRate() const
{
	float IncreaseAttackBuff = 0.f;
	float IncreaseAttackBuff2 = 0.f;

	//auto* Buff = GetBuffManager()->GetBuff<UB2Buff_Base>(EBuffType::Buff_IncreaseAttackDamage);
	//if (Buff && Buff->IsActive())
	//	IncreaseAttackBuff = Buff->GetBuffAmount();

	//auto* Buff2 = GetBuffManager()->GetBuff<UB2Buff_Base>(EBuffType::Buff_IncreaseAttackDamage_Orb);
	//if (Buff2 && Buff2->IsActive())
	//	IncreaseAttackBuff2 = Buff2->GetBuffAmount();

	//float IncreaseAttackByFury = IsBuffActive(EBuffType::Buff_Fury) ? 0.1f : 0.f;

	return /*BaseDamageRate +*/ IncreaseAttackBuff + IncreaseAttackBuff2 /*+ IncreaseAttackByFury*/;
}

float ABladeIICharacter::GetCriticalRate() const
{
	float fRate = 0.0f;

	//UB2Buff_Base* pBuff = GetBuffManager()->GetBuff<UB2Buff_Base>(EBuffType::Buff_IncreaseCriticalRate);

	//if (pBuff && pBuff->IsActive())
	//{
	//	fRate += pBuff->GetBuffAmount();
	//}

	return fRate;
}

float ABladeIICharacter::GetCriticalDamageRate() const
{
	float fRate = 1.6f;

	//UB2Buff_Base* pBuff = GetBuffManager()->GetBuff<UB2Buff_Base>(EBuffType::Buff_IncreaseCriticalDamage);

	//if (pBuff && pBuff->IsActive())
	//{
	//	fRate += pBuff->GetBuffAmount();
	//}

	return fRate;
}

float ABladeIICharacter::GetDamageDecreaseRate() const
{
	float fRate = 1.0f;

	//if (IsBuffActive(EBuffType::Buff_Fury))
	//	fRate *= 0.9f;

	//auto* BuffObj = GetBuff(EBuffType::Buff_DecreaseReceiveDamage);
	//if (BuffObj && BuffObj->IsActive())
	//	fRate *= BuffObj->GetBuffAmount();

	//auto* BuffObj2 = GetBuff(EBuffType::Buff_DecreaseReceiveDamage_Orb);
	//if (BuffObj2 && BuffObj2->IsActive())
	//	fRate *= BuffObj2->GetBuffAmount();

	return fRate;
}

float ABladeIICharacter::GetExtraTakeDamageRate() const
{
	static const float PoisionAdditionalDmg = 0.1f;

	float fRate = 1.f;
	//const auto* BuffObj = GetBuff(EBuffType::Debuff_Poison);
	//if (BuffObj && BuffObj->IsActive())
	//	fRate += PoisionAdditionalDmg;

	return fRate;
}

const TArray<FAIAttackData>& ABladeIICharacter::GetAIAttackArray()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetAIAttackArray);
	int32 CurrentPhaseNum = GetCurrentPhaseNum();
	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // 官肺 流傈 phase 函拳甫 老栏挪 data 牢郸胶. 拱沸 phase 函拳啊 绝菌促搁 -1
	if (CurrentPhaseDataIndex >= 0 && PhaseDataArray.Num() > CurrentPhaseDataIndex && PhaseDataArray[CurrentPhaseDataIndex].AttackDataArray.Num() > 0)
	{
		// Override AIAttackDataArray
		return PhaseDataArray[CurrentPhaseDataIndex].AttackDataArray;
	}

	return AIAttackDataArray; // 扁夯 phase 俊辑狼 扁夯 蔼.
}

const TArray<int32>& ABladeIICharacter::GetInvalidAttackIndices()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetInvalidAttackIndices);
	int32 CurrentPhaseNum = GetCurrentPhaseNum();
	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // 官肺 流傈 phase 函拳甫 老栏挪 data 牢郸胶. 拱沸 phase 函拳啊 绝菌促搁 -1
	if (CurrentPhaseDataIndex >= 0 && PhaseDataArray.Num() > CurrentPhaseDataIndex && PhaseDataArray[CurrentPhaseDataIndex].AttackDataArray.Num() > 0)
	{
		// Override InvalidAttackIndices
		return PhaseDataArray[CurrentPhaseDataIndex].InvalidAttackIndices;
	}

	return InvalidAttackIndices;
}

float ABladeIICharacter::GetMoveAcceptableRadius()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetMoveAcceptableRadius);
	return AIAttackMoveAcceptableRadius;
}

float ABladeIICharacter::GetTargetDistanceInTarget(AActor* InTargetActor) const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetTargetDistanceInTarget);
	if (InTargetActor)
	{
		ABladeIICharacter* CastedTargetCharacter = Cast<ABladeIICharacter>(InTargetActor);
		float fTargetCapsuleRadius = CastedTargetCharacter ? CastedTargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() : 0.0f;
		return (InTargetActor->GetActorLocation() - GetActorLocation()).Size2D() - fTargetCapsuleRadius;
	}
	return -1.f;
}

bool ABladeIICharacter::CalcAIAttackIndex(float TargetDistance, int32& OutAttackIndex, float& OutRadius)
{
	int32 NewAIAttackIndex = INDEX_NONE;

	const TArray<FAIAttackData>& DataArray = GetAIAttackArray();
	const TArray<int32>& InvalidIndices = GetInvalidAttackIndices();

	// Use 0 index as default when no AIAttackArray available
	if (DataArray.Num() == 0)
	{
		NewAIAttackIndex = 0;
		// no return for logging case
	}
	else
	{
		TBitArray<> InvalidBitArray(false, DataArray.Num());
		for (int32 i = 0; i < InvalidIndices.Num(); ++i)
		{
			if (InvalidBitArray.IsValidIndex(InvalidIndices[i]))
				InvalidBitArray[InvalidIndices[i]] = true;
		}

		float TotalWeight = 0.f, TotalWeight2 = 0.f;
		TBitArray<> BitIndexArray;
		TBitArray<> BitIndexArray2;
		for (int32 i = 0; i < DataArray.Num(); ++i)
		{
			if (!InvalidBitArray[i] && TargetDistance >= DataArray[i].MinDistance && (!bHadRecoverArmor || DataArray[i].bUseForRecoverArmor))
			{
				BitIndexArray2.Add(true);
				TotalWeight2 += DataArray[i].PossibilityWeight;
				if (TargetDistance < DataArray[i].MaxDistance)
				{
					BitIndexArray.Add(true);
					TotalWeight += DataArray[i].PossibilityWeight;
				}
				else
				{
					BitIndexArray.Add(false);
				}
			}
			else
			{
				BitIndexArray.Add(false);
				BitIndexArray2.Add(false);
			}
		}

		float ConditionalTotalWeight = TotalWeight;
		float ConditionalTotalWeight2 = TotalWeight2;

		bool IsContain = BitIndexArray.IsValidIndex(AIAttackIndex) && BitIndexArray[AIAttackIndex] == true;
		if (IsContain)
		{
			ConditionalTotalWeight2 -= DataArray[AIAttackIndex].PossibilityWeight;
			if (TargetDistance < DataArray[AIAttackIndex].MaxDistance)
			{
				ConditionalTotalWeight -= DataArray[AIAttackIndex].PossibilityWeight;
			}
		}

		if (ConditionalTotalWeight > 0.f)
		{
			NewAIAttackIndex = GetAIAttackIndex(BitIndexArray, ConditionalTotalWeight, AIAttackIndex, DataArray);
		}
		else if (ConditionalTotalWeight2 > 0.f)
		{
			NewAIAttackIndex = GetAIAttackIndex(BitIndexArray2, ConditionalTotalWeight2, AIAttackIndex, DataArray);
		}
		else if (TotalWeight > 0.f)
		{
			float Weight = FMath::FRand() * TotalWeight;
			float CurrentWeight = 0.f;
			for (int32 i = 0; i < BitIndexArray.Num(); ++i)
			{
				if (BitIndexArray[i])
				{
					CurrentWeight += DataArray[i].PossibilityWeight;
					if (Weight <= CurrentWeight)
					{
						NewAIAttackIndex = i;
						break;
					}
				}
			}
		}
		else if (TotalWeight2 > 0.f)
		{
			float Weight = FMath::FRand() * TotalWeight2;
			float CurrentWeight = 0.f;
			for (int32 i = 0; i < BitIndexArray2.Num(); ++i)
			{
				if (BitIndexArray2[i])
				{
					CurrentWeight += DataArray[i].PossibilityWeight;
					if (Weight <= CurrentWeight)
					{
						NewAIAttackIndex = i;
						break;
					}
				}
			}
		}

		if (NewAIAttackIndex >= 0 && NewAIAttackIndex < DataArray.Num())
		{
			OutRadius = DataArray[NewAIAttackIndex].MaxDistance;
		}

		// 给沥沁栏搁 芭府0悸泼窍瘤富绊 俩变吧肺
		if (NewAIAttackIndex < 0 && OutRadius == 0.f)
		{
			float fMaxForOutRadius = 0.f;
			for (int32 i = 0; i < DataArray.Num(); ++i)
			{
				if (DataArray[i].MaxDistance > fMaxForOutRadius)
				{
					fMaxForOutRadius = DataArray[i].MaxDistance;
				}
			}

			OutRadius = fMaxForOutRadius;
		}
	}
	OutAttackIndex = NewAIAttackIndex;

	return NewAIAttackIndex != INDEX_NONE;
}


void ABladeIICharacter::UpdateAIAttackIndex(float TargetDistance, bool bOnlyUpdateMoveDist)
{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_UpdateAIAttackIndex);
//	int32 NewAIAttackIndex = INDEX_NONE;
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (Cast<ABladeIITestDummyNPC>(this))
//	{ // TestDummyNPC 俊辑绰 AIAttackIndex 流立 瘤沥秦辑 荤侩.
//		return;
//	}
//#endif
//	int32 CurrentPhaseNum = GetCurrentPhaseNum();
//	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // 官肺 流傈 phase 函拳甫 老栏挪 data 牢郸胶. 拱沸 phase 函拳啊 绝菌促搁 -1
//
//	float OutRadius = 0.0f;
//
//	if (bHadChangedPhase)
//	{
//		if (!bOnlyUpdateMoveDist && CurrentPhaseDataIndex >= 0 && CurrentPhaseDataIndex < PhaseDataArray.Num() && PhaseDataArray[CurrentPhaseDataIndex].PhaseAIAttackIndex >= 0)
//		{
//			NewAIAttackIndex = PhaseDataArray[CurrentPhaseDataIndex].PhaseAIAttackIndex;
//		}
//	}
//	else
//	{
//		CalcAIAttackIndex(TargetDistance, NewAIAttackIndex, OutRadius);
//	}
//
//	if (bOnlyUpdateMoveDist)
//	{
//		AIAttackMoveAcceptableRadius = OutRadius;
//	}
//	else
//	{
//		AIAttackIndex = NewAIAttackIndex;
//		AIAttackMoveAcceptableRadius = OutRadius;
//
//		// Fixed bHadRecoverArmor next time, if there is no available attack after RecoverArmor
//		if (AIAttackIndex == INDEX_NONE)
//		{
//			if (bHadRecoverArmor)
//				bHadRecoverArmor = false;
//			if (bHadChangedPhase)
//				bHadChangedPhase = false;
//		}
//	}
//
//	if (bUseAIIndexLog)
//	{
//#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
//		UE_LOG(LogB2Character, Warning, TEXT("AIAttackIndex %d, TargetDistance %f, bOnlyUpdateMoveDist %d"), AIAttackIndex, TargetDistance, bOnlyUpdateMoveDist);
//#endif
//	}
}

float ABladeIICharacter::GetAIAttackDelay()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetAIAttackDelay);
	//const TArray<FAIAttackData>& DataArray = GetAIAttackArray();
	//if (AIAttackIndex >= 0 && AIAttackIndex < DataArray.Num())
	//{
	//	return FMath::FRand() * (DataArray[AIAttackIndex].MaxDelay - DataArray[AIAttackIndex].MinDelay) + DataArray[AIAttackIndex].MinDelay;
	//}
	return 0.f;
}


float ABladeIICharacter::GetAIMaxAttackRange()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetAIMaxAttackRange);
	//const TArray<FAIAttackData>& DataArray = GetAIAttackArray();
	//float MaxDistance = 0.f;
	//for (int32 i = 0; i < DataArray.Num(); ++i)
	//{
	//	MaxDistance = FMath::Max(DataArray[i].MaxDistance, MaxDistance);
	//}
	//return MaxDistance > 0.f ? MaxDistance : DetectDistance;
	return 0.f;
}

FVector ABladeIICharacter::GetFloatingBarPosition()
{
	//if (bHPBarAttachedToSocket)
	//	return HPBarSocket->GetSocketLocation(GetBaseMesh());

	return GetActorLocation();
}

void ABladeIICharacter::OnQTENext()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_OnQTENext);
	//// QTE Next for Mob during bInQTEEnabled
	//if (bQTEEnabled && (QTEType == EQTEType::EQT_Mount || QTEType == EQTEType::EQT_Break))
	//{
	//	QTEStateNum++;
	//	if (QTEStateNum == 1 && !QTELimitEnable)
	//	{
	//		ReserveQTEEnableEndCB();
	//	}
	//}
}

void ABladeIICharacter::ReserveQTEEnableEndCB()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ReserveQTEEnableEndCB);
	GetWorldTimerManager().SetTimer(OnQTEEnableEndTH, this, &ABladeIICharacter::OnQTEEnableEndCB, QTEEnableTime, false);
}

void ABladeIICharacter::OnQTEEnableEndCB()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_OnQTEEnableEndCB);
	GetWorldTimerManager().ClearTimer(OnQTEEnableEndTH);
	OnQTENext();
}

void ABladeIICharacter::CancleQTEEnableEndCB()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_CancleQTEEnableEndCB);
	GetWorldTimerManager().ClearTimer(OnQTEEnableEndTH);
}

ABladeIICharacter* ABladeIICharacter::FindQteOwnerPlayer() const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_FindQteOwnerPlayer);
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	if (ABladeIICharacter* QTEOwner = Cast<ABladeIICharacter>(*Iterator))
	//	{
	//		if (QTEOwner->IsPlayerControlled() && QTEOwner->GetQTEActor() == this)
	//			return QTEOwner;
	//	}
	//}

	return nullptr;
}

void ABladeIICharacter::SetDieForce()
{
	SetHealth(0.f);
	//Die(0, FDamageEvent(), nullptr, nullptr);
}

void ABladeIICharacter::SetHideFromEventDirector(bool bHide)
{
	bHideFromEventDirector = bHide;

	SetActorHiddenInGame(bHide);
}

void ABladeIICharacter::ResetEmissive()
{
	//::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);

	//GetMesh()->bB2EmissiveEnabled = false;
}

void ABladeIICharacter::SetQTEEnabled(bool bInEnable)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetQTEEnabled);
	bQTEEnabled = bInEnable;
}

void ABladeIICharacter::SetQTELimitEnabled(bool bQTELimitEnable)
{
	QTELimitEnable = bQTELimitEnable;
}

bool ABladeIICharacter::GetQTELimitEnabled()
{
	return QTELimitEnable;
}

void ABladeIICharacter::RecoverQTEBreakPoint()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_RecoverQTEBreakPoint);
	if (IsQTEBreakable() && IsInQTEBreakState())
	{
		CurrentQTEBreakPoint = GetMaxQTEBreakPoint();

		OnRecoverQTEBreakPoint();
	}
}

void ABladeIICharacter::SetQTEState()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetQTEState);
	// Double check...
	if (!IsAlive() || !bQTEEnabled)
	{
		return;
	}

	QTEStateNum = 0;
	SetQTEStateFlag(true);
	bQTERun = false;
	bForceCancelAttackTriggered = true;

	// No Collision for QTE for Mob
	//bTearOff = true;

	if (QTEType != EQTEType::EQT_Mount)
	{
		SetActorEnableCollision(false);
	}
}

void ABladeIICharacter::SetQTEStateFlag(bool bInState)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetQTEStateFlag);
	bInQTEState = bInState; // 咯扁辑绰 喊促弗 扒 绝绊 BladeIIPlayer 率俊辑

	//if (bInQTEState)
	//{
	//	// UIDoc 率俊 惑怕 函荐甫 持绢林绰单 ABladeIIPlayer 率俊辑档 窍变 窍瘤父 Player 率狼 bInQTEState 函荐啊 盔窍绰 扁埃焊促 粱 陋篮 淀秦辑 咯扁辑档.
	//	ABladeIICharacter* LocalPlayerChar = Cast<ABladeIICharacter>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (LocalPlayerChar && LocalPlayerChar->IsPlayerControlled() && LocalPlayerChar->GetQTEActor() == this)
	//	{
	//		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//		if (DocBattle)
	//		{
	//			DocBattle->SetbIsPlayerInQTEState(true);
	//		}
	//	}
	//}
}

void ABladeIICharacter::SetAttackState(EAttackState InAttackState)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetAttackState);
	AttackState = InAttackState;
}

void ABladeIICharacter::SetForceAttackState(EAttackState InAttackState)
{
	//if (GetMesh() != nullptr)
	//{
	//	UB2AnimInstance* AnimInst = Cast<UB2AnimInstance>(GetMesh()->GetAnimInstance());
	//	if (AnimInst)
	//		AnimInst->SetAnimAttackState(InAttackState, true);
	//}
}

void ABladeIICharacter::OnAttackState()
{
}
void ABladeIICharacter::OnEndAttackState()
{
}

void ABladeIICharacter::SetSignalAttack(bool bInSignal)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetSignalAttack);
	// bAbleToMove 啊 false 牢 版快 傍拜 涝仿阑 给窍变 窍瘤父 局聪皋捞记 惑怕 勤甸傅 档吝 捞悼父 阜绊磊 静绰 版快啊 乐绢辑 咯扁辑 八荤 救 窃.
	if (bMovementPreventByStageEvent)
	{ // 扁鸥 傍拜 惑怕肺 柳涝秦辑绰 救瞪 炼扒甸阑 咯扁辑 鞘磐傅 窍档废..
		bSignalAttack = false;
	}
	else
	{
		bSignalAttack = bInSignal; // AnimBlueprint 俊辑 捞吧 侨诀秦辑 傍拜 葛记 state 肺 埃 促澜 府悸且 巴.
	}
}

void ABladeIICharacter::BeginPlay()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_BeginPlay);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::BeginPlay"));

	Super::BeginPlay();
//
//	InitializeCombatStats(); // 咯扁辑 茄 锅 窍绰单 NPCClassInfo 甫 烹秦 鞘夸茄 蔼捞 甸绢棵 荐 乐栏骨肺 弊率辑 茄锅 歹 阂矾淋. -> 弊犯瘤父 AB2MonsterSpawnPool::SpawnWave 俊辑 bDeferConstruction 阑 技泼窍骨肺 NPCClassInfo 捞饶俊 咯扁肺 坷霸 瞪 巴.
//
//	// It could be already created and try create again, if outer level is loaded twice by some mishandling.
//	DestroyUIManager();
//	CreateUIManager();
//
//	CharacterBeginPlayClass<ABladeIICharacter*>::GetInstance().Signal(this);
//
//	if (ShadowMaterial)
//	{
//		SetupDecalCompForCenterShadowCommon(CenterShadow, GetMesh());  // SetupControlledMatineePuppet 俊辑档 荤侩窍绰 傍烹 悸诀 何盒.. 捞菌绰单 搬惫 咯扁辑父 荤侩
//
//		CenterShadow->ReregisterComponent();
//		CenterShadow->SetDecalMaterial(ShadowMaterial);
//		CenterShadow->Activate();
//	}
//
//	InitUpdateUIDoc();
//
//	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	if (pGM)
//		pGM->NotifyCharacterBeginPlay(this);
//
//	BuffManager = NewObject<UB2CharacterBuffManager>(this, *FString::Printf(TEXT("CharacterBuffManager_%s"), *this->GetName()), RF_Transient);
//	BuffManager->InitBuffManager(this);
//
//	if (InitialInfluence != EInfluenceType::EIT_End)
//	{
//		PreApplyInfluenceParam(InitialInfluence, InitialInfluenceAmount, InitialInfluenceDuration);
//		ApplyInfluence(InitialInfluence, InitialInfluenceAmount, InitialInfluenceDuration, InitialInfluenceStateDuration, InitialInfluenceStateTriggerRate, true, true);
//	}
//
//	// 皋葛府 例距 侩. 咆胶媚 胶飘府怪 登绰 版快 促弗 版肺甫 烹秦 贸府瞪 巴.
//#if PLATFORM_SUPPORTS_TEXTURE_STREAMING
//	IConsoleVariable* CVarTextureStreaming = IConsoleManager::Get().FindConsoleVariable(TEXT("r.TextureStreaming"));
//	checkSlow(CVarTextureStreaming);
//	if (CVarTextureStreaming && CVarTextureStreaming->GetInt() == 0)
//	{
//		RemoveCPURenderDataOfAllSkComps();
//	}
//#else
//	RemoveCPURenderDataOfAllSkComps();
//#endif
//
//	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	BII_CHECK(GameMode);
//	InjectedGameRule = GameMode->GetGameRule();
//
//	SetupLODInfoAsNPC(); // LOD 甸绢埃 老何 NPC 侩栏肺..
//
//	// 促捞唱雇 赣磐府倔 固府 父甸绢仇
//	PreCreateAndSetMaterialInstanceDynamic();
//
//	if (!AttachHPBarSocketName.IsNone())
//	{
//		HPBarSocket = GetBaseMesh()->GetSocketByName(AttachHPBarSocketName);
//		bHPBarAttachedToSocket = HPBarSocket != nullptr;
//	}
//
//	if (!SpawnNotifyMsgString.IsEmpty())
//	{
//		SetSpawnNotifyMessageClass<const FString&, const float>::GetInstance().Signal(SpawnNotifyMsgString, SpawnNotifyMsgTime);
//	}
}

void ABladeIICharacter::InitializeCombatStats()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_InitializeCombatStats);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::InitializeCombatStats"));

	//// Character stat do not change during game play
	//// PC 狼 版快俊绰 Final**Scale 措脚 酒捞袍俊 蝶弗 焊沥捞 利侩瞪 巴.

	//// Need to be change to use formula, item, sub PC, etc...
	//TotalAttack = CombatStatEval::GetMobBaseAttack(CharacterLevel);
	//TotalDefense = CombatStatEval::GetMobBaseDefense(CharacterLevel);

	//AttackLevel = CombatStatEval::GetADLevel(TotalAttack, CombatStatEval::GetDefaultCombatCoefficient());
	//DefenseLevel = CombatStatEval::GetADLevel(TotalDefense / 3.f, CombatStatEval::GetDefaultCombatCoefficient());

	//// Need to be change to use formula, skill/item %, etc...
	//const float BaseHealth = FinalHPScale * CombatStatEval::GetMobBaseHealth(CharacterLevel);
	//MaxHealth = BaseHealth;

	//// 抄捞档 炼例侩 - 单固瘤 99999 档崔 规瘤甫 困茄 hp / Damage 厚肥 惑荐
	//auto* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GameMode && IsMob())
	//{
	//	MaxHealth *= B2GameMode->GetPVEProportionalConstant();
	//	if (IsBossMob())
	//		MaxHealth *= B2GameMode->GetModifiedBossmobMaxHealth();

	//	/// 鸥荐肺 各 磷老版快俊 碍力肺 hp 绊沥.
	//	if (bForceReceivdAttackCountDamage)
	//		MaxHealth = FullAttactCountSize;

	//}

	//Health = MaxHealth;

	//// No formula for armor for now
	//Armor = MaxArmor;

	//if (IsQTEBreakable())
	//{
	//	CurrentQTEBreakPoint = GetMaxQTEBreakPoint();
	//}
	//else
	//{
	//	CurrentQTEBreakPoint = 0.f;
	//}

	//RandDamageAnimType = (FMath::FRand() < 0.7f) ? EAttackType::EAT_Default : EAttackType::EAT_LeftSwing;
}

void ABladeIICharacter::BeginDestroy()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_BeginDestroy);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::BeginDestroy"));
	//UWorld* MyWorld = GetWorld();
	//if (MyWorld)
	//{
	//	MyWorld->GetTimerManager().ClearAllTimersForObject(this);
	//}

	//DestroyUIManager(); // 敲饭捞绢茄抛 嘎酒 磷瘤 臼栏搁 捞 风飘甫 藕促.

	Super::BeginDestroy();
}

void ABladeIICharacter::Destroyed()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_Destroyed);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::Destroyed"));

	//UWorld* MyWorld = GetWorld();
	//if (MyWorld)
	//{
	//	MyWorld->GetTimerManager().ClearAllTimersForObject(this);
	//}

	//DestroyUIManager();

	//// 磷阑嫐 窍变窍绰单. 救磷绊 力芭且瘤档 葛福聪.
	//if (BuffManager)
	//{
	//	BuffManager->ClearAllBuffs();
	//}

	Super::Destroyed();
}

// void ABladeIICharacter::SetCachedStateDamageState(EStateAttackType InType, bool bIsOn, float RemainingTime)
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetCachedStateDamageState);
// 	B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetCachedStateDamageState"));
// 
// 	const int32 StateIndex = GetStateAttackTypeArrayIndex(InType);
// 	if (CachedStateDamageStates.IsValidIndex(StateIndex))
// 	{
// 		CachedStateDamageStates[StateIndex].bIsOn = bIsOn;
// 		if (bIsOn){
// 			CachedStateDamageStates[StateIndex].LastStartedTime = (float)FPlatformTime::Seconds(); // 辑肺 厚背秦辑 UI 俊 谎府绰 快急鉴困 沥窍扁 侩.
// 		}
// 		CachedStateDamageStates[StateIndex].RemainingTime = RemainingTime;
// 
// 		StateDamageBuffUpdateUIdoc();
// 	}
// }

// void ABladeIICharacter::SetCachedInvincibleBuffState(bool bIsOn, float RemainingTime)
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetCachedInvincibleBuffState);
// 	CachedInvincibleBuffState.bIsOn = bIsOn; // On 趣篮 Off 矫痢俊父 荤侩秦具 窃.
// 	if (bIsOn)
// 	{
// 		CachedInvincibleBuffState.LastStartedTime = (float)FPlatformTime::Seconds(); // 辑肺 厚背秦辑 UI 俊 谎府绰 快急鉴困 沥窍扁 侩.
// 	}
// 	CachedInvincibleBuffState.RemainingTime = RemainingTime;
// 	InvincibleBuffUpdateUIDoc();
// }

void ABladeIICharacter::SetRegenerateHPRate(float PercentageOrAmount, bool bInRegenByPercent)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetRegenerateHPRate);
	//bRegenerateHPByPercent = bInRegenByPercent;
	//if (bRegenerateHPByPercent)
	//{
	//	RegenerateHPPercentagePerSecond = PercentageOrAmount;
	//}
	//else
	//{
	//	RegenerateHPAmountPerSecond = PercentageOrAmount;
	//}

	//if (PercentageOrAmount > 0.f)
	//{
	//	GetWorldTimerManager().SetTimer(SetRegenerateHPHandle, this, &ABladeIICharacter::RegenerateHPByTimer, GetRegenerateHPPeriod(), true);
	//}
	//else
	//{ // 0 捞唱 澜荐绰 捞吧 掺绰 吧肺.
	//	GetWorldTimerManager().ClearTimer(SetRegenerateHPHandle);
	//}
}

void ABladeIICharacter::SetCanBeDamagedForB2(bool bNewCanBeDamaged)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetCanBeDamagedForB2);
	//// false搁 墨款泼 棵府绊 true搁 墨款泼 郴覆
	//!bNewCanBeDamaged ? nCanBeDamagedFalseCount++ : nCanBeDamagedFalseCount--;

	//UE_LOG(LogBladeII, Log, TEXT("nCanBeDamagedFalseCount = %d"), nCanBeDamagedFalseCount);

	//// 墨款泼 0捞窍搁 单固瘤 罐澜.
	//bCanBeDamaged = nCanBeDamagedFalseCount <= 0;
}

void ABladeIICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 滚橇 酒捞能 诀单捞飘.. 怖 平俊辑 秦具父窍唱
	CharacterBuffUpdateUIdoc();

	UpdateOscillation(DeltaSeconds);

	CheckReservationDamage();

	// Update Sink
	if (bIsDying && bIsDeadSinking)
	{
		FVector Delta(0.f, 0.f, -DeltaSeconds * DeadSinkZOffsetPerSecond);
		GetRootComponent()->MoveComponent(Delta, GetActorQuat(), false, nullptr, MOVECOMP_NoFlags, ETeleportType::None);
	}

	LocationAdjustCharacterOverlap(DeltaSeconds);

	//if (BuffManager)
	//{
	//	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//	if (GameMode && GameMode->IsAbleToBuffTick())
	//		BuffManager->BuffTick(/*DeltaSeconds*/);
	//}
}

void ABladeIICharacter::SetActorHiddenInGame(bool bNewHidden)
{
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
	if (bHideAllNPCs && NPCClassEnum != ENPCClass::ENC_End)
	{ // PC 啊 捞巴俊 康氢罐瘤 臼档废 NPCClassEnum 眉农
		bNewHidden = true;
	}
#endif

	Super::SetActorHiddenInGame(bNewHidden);
}

void ABladeIICharacter::PreSave(FObjectPreSaveContext SaveContext)
{
	//Super::PreSave(SaveContext);
//
//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		// 葛电 某腐磐 spawn 俊 静捞绰 海捞胶 喉风橇赴飘俊绰 扁夯蔼 府家胶甸捞 冠囚 乐绰单 捞霸 公矫窍瘤 给且 沥档肺 努 荐 乐促. 捻欧 技捞宏 矫 力芭.
//		if (IsSavingForCookingOnPreSave(TargetPlatform) && GetMesh())
//		{
//			GetMesh()->SetSkeletalMesh(nullptr);
//			GetMesh()->SetAnimInstanceClass(nullptr); // 捞霸 努 芭促.
//
//			UE_LOG(LogBladeII, Log, TEXT("NULL out resouce properties of %s"), *this->GetPathName());
//		}
//	}
//#endif
}

void ABladeIICharacter::RegenerateHPByTimer()
{ // 欺季飘 趣篮 例措蔼 贸府
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_RegenerateHPByTimer);
	//if (bRegenerateHPByPercent && RegenerateHPPercentagePerSecond > 0.f)
	//{
	//	HealingHPByPercent(RegenerateHPPercentagePerSecond);
	//}
	//else if (!bRegenerateHPByPercent && RegenerateHPAmountPerSecond > 0.0f)
	//{
	//	HealingHPByAmount(RegenerateHPAmountPerSecond);
	//}
}

void ABladeIICharacter::RemoveCPURenderDataOfAllSkComps()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_RemoveCPURenderDataOfAllSkComps);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::RemoveCPURenderDataOfAllSkComps"));

	//// CPU data 甫 力芭且 荐 乐绰 利例茄 鸥捞怪捞 乐栏聪 酒公镑俊辑唱 巢惯窍搁 救凳.
	//TArray<UActorComponent*> AllSKComps = GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	//for (UActorComponent* ThisComp : AllSKComps)
	//{
	//	USkeletalMeshComponent* ThisSKComp = Cast<USkeletalMeshComponent>(ThisComp);
	//	if (ThisSKComp)
	//	{
	//		ThisSKComp->ConditionalRemoveCPURenderData();
	//	}
	//}
}

void ABladeIICharacter::SetHealth(float NewHealth, bool bReceivedFromHost /*= false*/)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetHealth);
	Health = NewHealth;
}

void ABladeIICharacter::SetBirthPlace(const FMobSpawnedBirthplaceInfo& InBirthplaceInfo)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetBirthPlace);
	// SpawnPool 技泼阑 烹秦 坷绰 捞 某腐磐狼 胶抛捞瘤 备己苞 包访茄 沥焊甸.
	//CachedBirthplaceInfo.Birthplace = InBirthplaceInfo.Birthplace;
	CachedBirthplaceInfo.WaveNumber = InBirthplaceInfo.WaveNumber;
	CachedBirthplaceInfo.WaveObjIndex = InBirthplaceInfo.WaveObjIndex;
	CachedBirthplaceInfo.bSpawnedAsBossMob = InBirthplaceInfo.bSpawnedAsBossMob;
	CachedBirthplaceInfo.bIsSummoned = InBirthplaceInfo.bIsSummoned;
	CachedBirthplaceInfo.bOwnerDeathReported = false;
}

void ABladeIICharacter::SetupControlledMatineePuppet(ASkeletalMeshActor* InMatineePuppet, bool bSetupLightComponent, USkeletalMesh* OptionalSKMeshToSet)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetupControlledMatineePuppet);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetupControlledMatineePuppet"));

	//USkeletalMeshComponent* MyBaseMeshComp = this->GetBaseMesh();
	//USkeletalMeshComponent* PuppetBaseMeshComp = InMatineePuppet->GetSkeletalMeshComponent();

	//if (InMatineePuppet && PuppetBaseMeshComp && MyBaseMeshComp)
	//{
	//	// 喊档肺 Mesh 甫 霖厚秦辑 逞败临 荐档 乐促.
	//	USkeletalMesh* MainSKMesh = OptionalSKMeshToSet ? OptionalSKMeshToSet : MyBaseMeshComp->SkeletalMesh;

	//	PuppetBaseMeshComp->SetSkeletalMesh(MainSKMesh); // 楷免俊辑绰 促弗 绊镊府萍 皋浆肺 官层磨 荐档 乐澜. 窜, Skeleton 捞 促弗 SkeletalMesh 扼搁 官层媚毫具 局聪皋捞记 敲饭捞啊 救 瞪 巴.

	//	PuppetBaseMeshComp->SetWorldScale3D(MyBaseMeshComp->GetComponentScale());

	//	UB2DamageEffectInfo* DmgFxInfo = GetDamageEffectInfo();

	//	// SkeletalMeshComponent 俊 涝腮 material override 档 利侩.
	//	if (!OptionalSKMeshToSet) // 喊档 Mesh 甫 力傍窍绰 版快绰 哪器惩飘俊辑 赣萍府倔阑 啊廉坷瘤 臼绰促.
	//	{
	//		// 傈捧 吝 赣萍府倔俊 利侩等 颇扼固磐甫 府悸.
	//		ResetFlash();
	//		ResetEmissive();

	//		for (int32 MI = 0; MI < MyBaseMeshComp->GetNumMaterials(); ++MI)
	//		{
	//			// 傈捧 瓤苞肺 牢秦 material override 啊 等 惑炔捞扼搁 盔贰 material 阑 涝囚林绰 霸 利例且 巴捞促.
	//			// 墨皋扼况农父 荤侩窍绰 楷免捞搁 傈捧 惑炔捞 弊措肺 啊绰 霸 利例窍摆瘤父 捞扒 局聪皋捞记鳖瘤 楷免等 厘搁俊辑 荤侩且 巴捞骨肺.
	//			UMaterialInterface* MaterialToSet = GetOriginalMaterial(MI);
	//			PuppetBaseMeshComp->SetMaterial(MI, MaterialToSet);
	//		}
	//	}

	//	// 扁鸥 attach 等 巴甸捞 乐促搁 楷免侩俊档 付蛮啊瘤肺 嘿烙.

	//	// StaticMesh 肺 嘿牢 巴甸
	//	TArray<UActorComponent*> AttachedStaticMeshComps = this->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	//	for (int32 SMI = 0; SMI < AttachedStaticMeshComps.Num(); ++SMI)
	//	{
	//		UStaticMeshComponent* ThisComp = Cast<UStaticMeshComponent>(AttachedStaticMeshComps[SMI]);
	//		// 老窜, SkeletalMeshComponent 俊 嘿篮 巴父 秒鞭秦 夯促.. AttachTo 且 锭 ThisComp->GetAttachParent() 秦辑 冈咯临 鉴 绝绰 畴俯捞扼..
	//		if (ThisComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForStaticMesh(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach 等 局甸付促 窍唱究 积己秦辑 鞍篮 StaticMesh 涝洒绊 鞍篮 socket 俊 嘿咯淋.
	//			UStaticMeshComponent* PuppetAttachComp = NewObject<UStaticMeshComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetStaticMesh(ThisComp->GetStaticMesh());
	//				PuppetAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh 甫 SkeletalMeshActor 俊 嘿捞聪 寸楷洒 Movable 肺
	//				// AttachParent 啊 SkeletalMeshComponent 啊 酒囱 版快啊 积变促搁 荐沥..
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetMaterial(0, ThisComp->GetMaterial(0)); // Attach Component 狼 Material 捞 SpawnPool 阑 烹秦 overriding 瞪 荐 乐栏骨肺 捞巴档 技泼. 窜 0锅父
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 辆丰 饶 力芭绰 救窍骨肺 趣矫 葛福聪 collision 篮 掺档废
	//				PuppetAttachComp->RegisterComponent();
	//			}
	//		}
	//	}
	//	// SkeletalMesh 肺 嘿牢 巴甸
	//	TArray<UActorComponent*> AttachedSKComps = this->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	//	for (int32 SKI = 0; SKI < AttachedSKComps.Num(); ++SKI)
	//	{
	//		USkeletalMeshComponent* ThisComp = Cast<USkeletalMeshComponent>(AttachedSKComps[SKI]);
	//		// 老窜, SkeletalMeshComponent 俊 嘿篮 巴父 秒鞭秦 夯促.. AttachTo 且 锭 ThisComp->GetAttachParent() 秦辑 冈咯临 鉴 绝绰 畴俯捞扼..
	//		// 肚茄 皋牢 SkeletalMeshComponent 绰 官肺 困俊辑 贸府窍骨肺 拱沸 烹苞~
	//		if (ThisComp && ThisComp != MyBaseMeshComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForSkeletalMesh(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach 等 局甸付促 窍唱究 积己秦辑 鞍篮 SkeletalMesh 涝洒绊 鞍篮 socket 俊 嘿咯淋.
	//			USkeletalMeshComponent* PuppetAttachComp = NewObject<USkeletalMeshComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetSkeletalMesh(ThisComp->SkeletalMesh);
	//				// AttachParent 啊 SkeletalMeshComponent 啊 酒囱 版快啊 积变促搁 荐沥..
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetMaterial(0, ThisComp->GetMaterial(0)); // Attach Component 狼 Material 捞 SpawnPool 阑 烹秦 overriding 瞪 荐 乐栏骨肺 捞巴档 技泼. 窜 0锅父
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 辆丰 饶 力芭绰 救窍骨肺 趣矫 葛福聪 collision 篮 掺档废
	//				PuppetAttachComp->RegisterComponent();
	//			}
	//		}
	//	}
	//	// ParticleSystem
	//	// ParticleSystemComponent 甸篮 林狼且 巴捞 乐绰单 InMatineePuppet 捞 Hidden 捞芭唱 购啊 霖厚啊 代 等淀茄 惑怕俊辑绰 捞犯霸 积己阑 矫难毫具 Deactivate 啊 登绰 巴 鞍酒 焊牢促绰 巴.
	//	// 咯扁辑 酒公府 Activate 甫 秦档 家侩绝促. StageEventDirector 率俊辑 Visibility 力绢窍搁辑 蝶肺 贸府秦 淋.
	//	// 眠啊肺 咯扁辑绰 BladeIIPlayer 狼 FloorRingPS 档 积己登绢 嘿咯龙 扒单 弊巴档 StageEventDirector 率俊辑 贸府甫..
	//	TArray<UActorComponent*> AttachedPSComps = this->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//	for (int32 PSI = 0; PSI < AttachedPSComps.Num(); ++PSI)
	//	{
	//		UParticleSystemComponent* ThisComp = Cast<UParticleSystemComponent>(AttachedPSComps[PSI]);
	//		// 付蛮啊瘤肺, SkeletalMeshComponent 俊 嘿篮 巴父 秒鞭
	//		if (ThisComp
	//			// [酋吝 17.02.13] 炼扒眠啊 风俏登绰巴甸父 嘿烙 : 老矫利牢 傍拜葛记俊辑 AnimNotify 肺 嘿绰 局甸篮 胶诺窍绊磊 窍绰 狼固. 扁夯利栏肺 咯扁辑 嘿老 鞘夸啊 乐绰 捞棋飘甸篮 瘤加利栏肺 敲饭捞登绰 (Looping 登绰) 局甸捞促.
	//			&& IsStaticallyVisiblePSC(ThisComp)
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForParticleSystem(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach 等 局甸付促 窍唱究 积己秦辑 鞍篮 PS 涝洒绊 鞍篮 socket 俊 嘿咯淋. 
	//			UParticleSystemComponent* PuppetAttachComp = NewObject<UParticleSystemComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetTemplate(ThisComp->Template);
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 辆丰 饶 力芭绰 救窍骨肺 趣矫 葛福聪 collision 篮 掺档废
	//				PuppetAttachComp->RegisterComponent();
	//				PuppetAttachComp->Activate(true);
	//			}
	//		}
	//	}

	//	// Decal 档 鞘夸窃.
	//	TArray<UActorComponent*> AttachedDecalComps = this->GetComponentsByClass(UDecalComponent::StaticClass());
	//	for (int32 DCI = 0; DCI < AttachedDecalComps.Num(); ++DCI)
	//	{
	//		UDecalComponent* ThisComp = Cast<UDecalComponent>(AttachedDecalComps[DCI]);
	//		// 付蛮啊瘤肺, SkeletalMeshComponent 俊 嘿篮 巴父 秒鞭
	//		if (ThisComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForDecal(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach 等 局甸付促 窍唱究 积己秦辑 鞍篮 material 涝洒绊 鞍篮 socket 俊 嘿咯淋.
	//			UDecalComponent* PuppetAttachComp = NewObject<UDecalComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->DecalSize = ThisComp->DecalSize;
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetDecalMaterial(ThisComp->GetDecalMaterial());
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->RegisterComponent();
	//			}
	//		}
	//	}

	//	// PointLight 档 嘿咯焊绰单.. 捞扒 葛官老 dynamic point light 俺荐 力茄 锭巩俊 粱 刮皑窃. 蝶扼辑 捞吧 救 窍绰 牢磊 力傍
	//	if (bSetupLightComponent)
	//	{
	//		TArray<UActorComponent*> AttachedLightComps = this->GetComponentsByClass(UPointLightComponent::StaticClass());
	//		for (int32 LCI = 0; LCI < AttachedLightComps.Num(); ++LCI)
	//		{
	//			UPointLightComponent* ThisComp = Cast<UPointLightComponent>(AttachedLightComps[LCI]);
	//			// 捞扒 Capsule 俊 嘿篮 巴档..
	//			if (ThisComp && ThisComp->IsVisible() // && ThisComp->bAffectsWorld // Light 搁 Visible 捞 酒聪扼 AffectsWorld 甫 毫具 嘎绰单.. 捞霸 鞘夸茄 惑炔烙俊档 AffectsWorld 啊 波廉 乐阑 啊瓷己捞 乐阑 巴 鞍酒辑 咯扁辑 眉农 救窍绊 bSetupLightComponent 牢磊甫 利例洒 逞败林绰 吧肺.
	//				&& (ThisComp->GetAttachParent() == MyBaseMeshComp || ThisComp->GetAttachParent() == RootComponent)
	//				&& !IsCompInEventSceneBlacklist(ThisComp)
	//				&& DoesMatineePuppetSetupForLight(InMatineePuppet, ThisComp) == false)
	//			{
	//				// Attach 等 局甸付促 窍唱究 积己秦辑 蔼汲沥 窍绊 鞍篮 socket 俊 嘿咯淋.
	//				UPointLightComponent* PuppetAttachComp = NewObject<UPointLightComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//				if (PuppetAttachComp)
	//				{
	//					PuppetAttachComp->RelativeLocation = ThisComp->RelativeLocation;
	//					PuppetAttachComp->Intensity = ThisComp->Intensity;
	//					PuppetAttachComp->LightColor = ThisComp->LightColor;
	//					PuppetAttachComp->AttenuationRadius = ThisComp->AttenuationRadius;
	//					PuppetAttachComp->CastShadows = ThisComp->CastShadows;
	//					PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//					PuppetAttachComp->RegisterComponent();
	//				}
	//			}
	//		}
	//	}
	//	// 捞傈俊 悸诀沁绰单 犁荤侩窍绰 版快啊 乐阑 荐 乐栏聪 On/Off 贸府档 促矫.
	//	SetVisibilityMatineePuppetPointLight(InMatineePuppet, bSetupLightComponent);
	//}
}

void ABladeIICharacter::ClearControlledMatineePuppet(class ASkeletalMeshActor* InMatineePuppet)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ClearControlledMatineePuppet);
	//USkeletalMeshComponent* MyBaseMeshComp = this->GetBaseMesh();
	//USkeletalMeshComponent* PuppetBaseMeshComp = InMatineePuppet->GetSkeletalMeshComponent();

	//if (InMatineePuppet && PuppetBaseMeshComp && MyBaseMeshComp)
	//{
	//	// SetupControlledMatineePuppet 俊辑 持菌带 Material override 甸篮 葛滴 力芭茄促. 弊成 滴搁 烙矫 积己 坷宏璃飘甸捞 甘俊 历厘登绢 (辟单 PIE 甘 蝶肺 积己登瘤 臼唱..?) 盔摹 臼绰 饭欺繁胶啊 惯积且 荐 乐澜.
	//	for (int32 MI = 0; MI < MyBaseMeshComp->GetNumMaterials(); ++MI)
	//	{
	//		PuppetBaseMeshComp->SetMaterial(MI, NULL);
	//	}
	//	// 扁鸥 SetupControlledMatineePuppet 俊辑 积己窍绰 Component 甸捞 乐瘤父 RF_Transient 甫 林骨肺 历厘 救瞪 巴. 捞固 奥甸捞 历厘登绊 乐菌栏搁 购啊 捞惑茄 老捞 老绢车摆瘤..
	//}
}

void ABladeIICharacter::SetVisibilityMatineePuppetPointLight(class ASkeletalMeshActor* InMatineePuppet, bool bOn)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetVisibilityMatineePuppetPointLight);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetVisibilityMatineePuppetPointLight"));

	//USkeletalMeshComponent* PuppetBaseMeshComp = InMatineePuppet->GetSkeletalMeshComponent();

	//if (!PuppetBaseMeshComp)
	//	return;

	//TArray<USceneComponent*> PuppetChildComponents;
	//PuppetBaseMeshComp->GetChildrenComponents(false, PuppetChildComponents);
	//for (int32 SCI = 0; SCI < PuppetChildComponents.Num(); ++SCI)
	//{
	//	UPointLightComponent* ThisComp = Cast<UPointLightComponent>(PuppetChildComponents[SCI]);

	//	if (ThisComp && ThisComp->bAffectsWorld != bOn)
	//	{
	//		ThisComp->bAffectsWorld = bOn;
	//		ThisComp->MarkRenderStateDirty();
	//	}
	//}
}

bool ABladeIICharacter::IsCompInEventSceneBlacklist(UActorComponent* InCheckComp) const
{
	checkSlow(InCheckComp && InCheckComp->GetOwner() == this); // 唱俊霸 加茄 Component 父 咯扁 眉农 措惑烙.
	return EventPuppetSetupBlacklist.Contains(InCheckComp);
}

// 动扁扁 fForceStrength -蔼老锭 动变促. +蔼捞搁 剐淀
void ABladeIICharacter::PullEnemyByTick(float fDeltaTime, float fForceStrength, float fRadius, FVector LocationOffset)
{
	//// bFixInAir敲贰弊肺 牢秦 公宏刚飘舶栏搁 菩胶
	//if (!GetCharacterMovement()->bAutoActivate)
	//	return;

	//TArray<ABladeIICharacter*> Enemies;
	//if (auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this)))
	//	GameMode->GetAllCharacterList(Enemies);

	//const FVector GoalPosition = GetActorLocation() + GetActorRotation().RotateVector(LocationOffset);
	//const float PullApplyForce = FMath::Clamp(fForceStrength, -fRadius * 2, fRadius * 2);
	//const float PullApplyForceDelta = PullApplyForce * fDeltaTime;

	//const float OwnerRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();

	//for (auto* Enemy : Enemies)
	//{
	//	const FVector& PullVector = Enemy->GetActorLocation() - GoalPosition;
	//	if (IsPullableActor(Enemy) && PullVector.SizeSquared() <= FMath::Square(fRadius))
	//	{
	//		const FVector& PullDirection = PullVector.GetSafeNormal();
	//		const FVector& NewPosition = Enemy->GetActorLocation() + (PullDirection * PullApplyForceDelta);

	//		const float EnemyRadius = Enemy->GetCapsuleComponent()->GetScaledCapsuleRadius();
	//		const float MinRequireDist = OwnerRadius + EnemyRadius;

	//		const FVector& OwnerToEnemy = NewPosition - GetActorLocation();
	//		if (OwnerToEnemy.SizeSquared() > FMath::Square(MinRequireDist))
	//		{
	//			Enemy->SetActorLocation(NewPosition, true);
	//		}
	//	}
	//}
}

bool ABladeIICharacter::IsPullableActor(AActor* TargetActor)
{
	if (IsEnemy(TargetActor))
		return true;

	return false;
}

// 酒贰 DoesMatineePuppetSetupFor** 甫 袍敲复栏肺档 给 秦辑 律陛捞唱付 傍烹利牢 何盒 眠妨航.
bool AreComponentsAttachedToSameKind(USceneComponent* InCompA, USceneComponent* InCompB)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_AreComponentsAttachedToSameKind);
	//return (InCompA && InCompB && (
	//	(InCompA->GetAttachParent() == NULL && InCompB->GetAttachParent() == NULL) ||
	//	(InCompA->GetAttachParent()->StaticClass() == InCompB->GetAttachParent()->StaticClass())
	//	));
	return false;
}

bool ABladeIICharacter::DoesMatineePuppetSetupForStaticMesh(class ASkeletalMeshActor* InMatineePuppet, class UStaticMeshComponent* InCompToCheck)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DoesMatineePuppetSetupForStaticMesh);
	//// InCompToCheck 绰 捞 BladeIICharacter 俊 嘿绢乐绰 巴烙.

	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedStaticComps = InMatineePuppet->GetComponentsByClass(UStaticMeshComponent::StaticClass());

	//for (int32 SCI = 0; SCI < MPAttachedStaticComps.Num(); ++SCI)
	//{
	//	UStaticMeshComponent* ThisMPComp = Cast<UStaticMeshComponent>(MPAttachedStaticComps[SCI]);

	//	// ThisMPComp 啊 InCompToCheck 客 厚搅茄瘤 八荤茄促. 鞍篮 府家胶甫 荤侩窍绊 嘿绢乐绰 家南 捞抚捞 老摹窍绰瘤.
	//	if (ThisMPComp && ThisMPComp->GetStaticMesh() == InCompToCheck->GetStaticMesh() && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		// 嘿绢乐绰 何葛 磊眉绰 拱沸 促福瘤父 鞍篮 辆幅俊 嘿绢乐绰瘤档 八荤.
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck)
	//		)
	//	{
	//		return true;
	//	}
	//}

	return false;
}

bool ABladeIICharacter::DoesMatineePuppetSetupForSkeletalMesh(class ASkeletalMeshActor* InMatineePuppet, class USkeletalMeshComponent* InCompToCheck)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DoesMatineePuppetSetupForSkeletalMesh);
	// InCompToCheck 绰 捞 BladeIICharacter 俊 嘿绢乐绰 巴烙.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedSKComps = InMatineePuppet->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	//for (int32 SCI = 0; SCI < MPAttachedSKComps.Num(); ++SCI)
	//{
	//	USkeletalMeshComponent* ThisMPComp = Cast<USkeletalMeshComponent>(MPAttachedSKComps[SCI]);

	//	// ThisMPComp 啊 InCompToCheck 客 厚搅茄瘤 八荤茄促. 鞍篮 府家胶甫 荤侩窍绊 嘿绢乐绰 家南 捞抚捞 老摹窍绰瘤.
	//	if (ThisMPComp && ThisMPComp->SkeletalMesh == InCompToCheck->SkeletalMesh && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // 嘿绢乐绰 何葛 磊眉绰 拱沸 促福瘤父 鞍篮 辆幅俊 嘿绢乐绰瘤档 八荤.
	//		)
	//	{
	//		return true;
	//	}
	//}

	return false;
}

bool ABladeIICharacter::DoesMatineePuppetSetupForParticleSystem(class ASkeletalMeshActor* InMatineePuppet, class UParticleSystemComponent* InCompToCheck)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DoesMatineePuppetSetupForParticleSystem);
	//// InCompToCheck 绰 捞 BladeIICharacter 俊 嘿绢乐绰 巴烙.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedPSComps = InMatineePuppet->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//for (int32 PCI = 0; PCI < MPAttachedPSComps.Num(); ++PCI)
	//{
	//	UParticleSystemComponent* ThisMPComp = Cast<UParticleSystemComponent>(MPAttachedPSComps[PCI]);

	//	// ThisMPComp 啊 InCompToCheck 客 厚搅茄瘤 八荤茄促. 鞍篮 府家胶甫 荤侩窍绊 嘿绢乐绰 家南 捞抚捞 老摹窍绰瘤.
	//	if (ThisMPComp && ThisMPComp->Template == InCompToCheck->Template && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // 嘿绢乐绰 何葛 磊眉绰 拱沸 促福瘤父 鞍篮 辆幅俊 嘿绢乐绰瘤档 八荤.
	//		)
	//	{
	//		return true;
	//	}
	//}

	return false;
}

bool ABladeIICharacter::DoesMatineePuppetSetupForDecal(class ASkeletalMeshActor* InMatineePuppet, class UDecalComponent* InCompToCheck)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DoesMatineePuppetSetupForDecal);
	// InCompToCheck 绰 捞 BladeIICharacter 俊 嘿绢乐绰 巴烙.
	if (InMatineePuppet == NULL || InCompToCheck == NULL)
	{
		return false;
	}

	//TArray<UActorComponent*> MPAttachedDecalComps = InMatineePuppet->GetComponentsByClass(UDecalComponent::StaticClass());
	//for (int32 DCI = 0; DCI < MPAttachedDecalComps.Num(); ++DCI)
	//{
	//	UDecalComponent* ThisMPComp = Cast<UDecalComponent>(MPAttachedDecalComps[DCI]);

	//	// ThisMPComp 啊 InCompToCheck 客 厚搅茄瘤 八荤茄促. 鞍篮 府家胶甫 荤侩窍绊 嘿绢乐绰 家南 捞抚捞 老摹窍绰瘤.
	//	if (ThisMPComp && ThisMPComp->GetDecalMaterial() == InCompToCheck->GetDecalMaterial() && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // 嘿绢乐绰 何葛 磊眉绰 拱沸 促福瘤父 鞍篮 辆幅俊 嘿绢乐绰瘤档 八荤.
	//		)
	//	{
	//		return true;
	//	}
	//}

	return false;
}

bool ABladeIICharacter::DoesMatineePuppetSetupForLight(class ASkeletalMeshActor* InMatineePuppet, class UPointLightComponent* InCompToCheck)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DoesMatineePuppetSetupForLight);
	// InCompToCheck 绰 捞 BladeIICharacter 俊 嘿绢乐绰 巴烙.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedLightComps = InMatineePuppet->GetComponentsByClass(UPointLightComponent::StaticClass());
	//for (int32 LCI = 0; LCI < MPAttachedLightComps.Num(); ++LCI)
	//{
	//	UPointLightComponent* ThisMPComp = Cast<UPointLightComponent>(MPAttachedLightComps[LCI]);

	//	// ThisMPComp 啊 InCompToCheck 客 厚搅茄瘤 八荤茄促. 汲沥蔼捞尔 家南 捞抚捞 老摹窍绰瘤.
	//	// Light 狼 版快绰 八荤 炼扒 割俺啊 狐龙 鞘夸啊 粱 乐促.. 荤角 角力 悼累窍绰 light 甫 嘿捞绊 促聪绰 局甸捞 咯返 乐绢辑档 救登绊.
	//	if (ThisMPComp && //ThisMPComp->RelativeLocation == InCompToCheck->RelativeLocation && 
	//		ThisMPComp->Intensity == InCompToCheck->Intensity &&
	//		ThisMPComp->LightColor == InCompToCheck->LightColor &&
	//		ThisMPComp->AttenuationRadius == InCompToCheck->AttenuationRadius
	//		//ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		//AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // 嘿绢乐绰 何葛 磊眉绰 拱沸 促福瘤父 鞍篮 辆幅俊 嘿绢乐绰瘤档 八荤.
	//		)
	//	{
	//		return true;
	//	}
	//}

	return false;
}

void ABladeIICharacter::CreateUIManager()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_CreateUIManager);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::CreateUIManager"));

	//// this 沥焊甫 逞败林绢 积己父 窍绊 捞饶肺绰 流立利牢 饭欺繁胶绰 鞘夸绝澜.
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM && B2GM->GetUIManager_InGameCombat())
	//{
	//	B2GM->GetUIManager_InGameCombat()->CreateHUDForNewChar(this);
	//}

	//if (CachedBirthplaceInfo.bSpawnedAsBossMob)
	//{
	//	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();

	//	//BeginPlay矫痢俊 DestroyUIManager()肺 牢秦 磊脚捞 焊胶烙俊档 阂备窍绊 UI甫 昏力茄促.
	//	//UI 促矫 技泼.
	//	if (DocBattle && AmIInterestedBossMob())
	//	{
	//		DocBattle->SetbCurrentlyBossAppeared(true);
	//	}
	//}
}

void ABladeIICharacter::HideFloatingHPBar(bool bInHide)
{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_HideFloatingHPBar);
//	// DestroyUIManager 啊 Death 矫痢俊辑 delay 登搁辑 磷篮 各狼 HPBar 甫 官肺 见辨 鞘夸啊 积辫. 楷免 殿狼 捞蜡肺 见扁妨带 霸 酒丛.. 承霸 焊搁 弊霸 促 楷免捞变 窍匙..
//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	if (B2GM && B2GM->GetUIManager_InGameCombat())
//	{
//		B2GM->GetUIManager_InGameCombat()->HideFloatingHPBarForChar(this, bInHide); // 捞扒 敲饭捞绢 酒囱 NPC 俊父 冈鳃 巴.
//	}
}

void ABladeIICharacter::DestroyUIManager()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DestroyUIManager);
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM && B2GM->GetUIManager_InGameCombat())
	//{
	//	B2GM->GetUIManager_InGameCombat()->DestroyHUDForDyingChar(this); // 捞扒 敲饭捞绢 酒囱 NPC 俊父 冈鳃 巴.
	//}

	//if (CachedBirthplaceInfo.bSpawnedAsBossMob)
	//{
	//	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//	// 趣矫扼档 救 磷篮 盲肺 destroy 瞪 荐啊 乐栏骨肺 技泼阑 秦 林变 窍绰单.. 滴 焊胶啊 楷崔酒 唱坷绰 版快档 乐栏骨肺 瘤陛 DocBattle 俊 技泼等 霸 磊脚苞 付蛮啊瘤牢瘤 犬牢篮 秦具 茄促.
	//	if (DocBattle && AmIInterestedBossMob())
	//	{
	//		DocBattle->SetbCurrentlyBossAppeared(false);
	//	}
	//}
}

void ABladeIICharacter::DelayedDestroyUIManagerCB()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DelayedDestroyUIManagerCB);
	//GetWorldTimerManager().ClearTimer(DelayedDestroyUIManagerTH);

	DestroyUIManager();
}

void ABladeIICharacter::InitUpdateUIDoc()
{
	// bSpawnedAsBossMob 捞 捞 矫痢俊 技泼捞 救登绢 乐阑 芭扼 喊肺 且 荐 乐绰 霸 绝焙. 弊扒 SpawnPool 俊辑..
}

void ABladeIICharacter::SetOscillation(const FVector& Amplitude, const FVector& Frequency)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetOscillation);
	if (Amplitude.Size() > 0.f)
	{
		bNeedOscillation = true;
		OscillAmplitude = Amplitude;
		OscillFrequency = Frequency;
		OscillOffset = FVector(FMath::FRand() * (2.f * PI), FMath::FRand() * (2.f * PI), FMath::FRand() * (2.f * PI));
	}
	else
	{
		bNeedOscillation = false;
	}

	// Save original location
	//BaseTranslationOffset = GetMesh()->RelativeLocation;
}

void ABladeIICharacter::UpdateOscillation(float DeltaSeconds)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_UpdateOscillation);
	//if (bNeedOscillation)
	//{
	//	// Play oscillation...
	//	OscillOffset += GetDeltaTimeAsOtherWorldObj(DeltaSeconds, this) * OscillFrequency;
	//	GetMesh()->RelativeLocation = BaseTranslationOffset + FVector(FMath::Sin(OscillOffset.X) * OscillAmplitude.X, FMath::Sin(OscillOffset.Y) * OscillAmplitude.Y, FMath::Sin(OscillOffset.Z) * OscillAmplitude.Z);
	//}
}

void ABladeIICharacter::SetBehaviorTree(class UBehaviorTree* InBT)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetBehaviorTree);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetBehaviorTree"));

	//if (InBT != NULL && InBT != BehaviorTree)
	//{
	//	BehaviorTree = InBT;
	//	AAIController* AIController = Cast<AAIController>(GetController());
	//	if (AIController)
	//	{
	//		if (AIController->BrainComponent)
	//		{
	//			// Stop behavior tree before start new one
	//			AIController->BrainComponent->Cleanup();
	//			//AIController->BrainComponent->DestroyComponent();
	//		}
	//		// Start new behavior tree
	//		AIController->RunBehaviorTree(InBT);
	//	}
	//}
}

ABladeIICharacter* ABladeIICharacter::GetNearestEnemyCharacter()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetNearestEnemyCharacter);
	//return Cast<ABladeIICharacter>(UGameplayStatics::GetNearestPlayerCharacter(this, GetActorLocation()));
	return nullptr;
}

void ABladeIICharacter::GetNearestFriendCharacters(TArray<class ABladeIICharacter*>& Friends, float Range, int32 MaxNum)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetNearestFriendCharacters);
	//if (MaxNum <= 0 || Range < GetCapsuleComponent()->GetScaledCapsuleRadius())
	//	return;


	//Friends.Empty();
	//const float SquaredRange = FMath::Square(Range);

	//// All Mobs are friends
	//auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (GameMode)
	//{
	//	TArray<ABladeIICharacter*> FriendsCharacters;

	//	GameMode->GetAllWorldMobList(FriendsCharacters, true);
	//	for (ABladeIICharacter* Friend : FriendsCharacters)
	//	{
	//		if (Friend && Friend->IsPlayerControlled())
	//		{
	//			const bool IsInRange = FVector::DistSquared(Friend->GetActorLocation(), GetActorLocation()) < SquaredRange;
	//			if (IsInRange)
	//			{
	//				MaxNum--;
	//				Friends.Add(Friend);
	//			}
	//		}

	//		if (MaxNum <= 0)
	//			return;
	//	}
	//}
}

void ABladeIICharacter::PreCreateAndSetMaterialInstanceDynamic()
{
	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
	{
		// Set Dynamic Material data
		UMaterialInterface* MaterialInterface = GetMesh()->GetMaterial(i);
		if (MaterialInterface)
		{
			UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
			if (!DynamicMaterial) //Is it already a UMaterialInstanceDynamic
			{
				DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
			}
		}
	}
}

void ABladeIICharacter::NotifyActorCustomEvent(FName EventName, UObject* OptionalNotifyingObject)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_NotifyActorCustomEvent);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::NotifyActorCustomEvent"));

	//// Phase Change by custom event.
	//int32 CurrentPhaseNum = GetCurrentPhaseNum();
	//if (PhaseDataArray.Num() > CurrentPhaseNum && PhaseDataArray[CurrentPhaseNum].TriggerEventName == EventName)
	//{
	//	GoToNextPhase();
	//}

	//ReceiveActorCustomEvent(EventName);
}

int32 ABladeIICharacter::GetCurrentPhaseNum()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetCurrentPhaseNum);
	//ABladeIIAIController* B2AICon = Cast<ABladeIIAIController>(GetController());
	//return B2AICon ? B2AICon->GetCurrentPhaseNum() : 0;
	return  0;
}

void ABladeIICharacter::GoToNextPhase()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GoToNextPhase);
	//// Phase change
	//bHadChangedPhase = true;
	//bInPhaseAnimation = false;

	//ABladeIIAIController* B2AICon = Cast<ABladeIIAIController>(GetController());
	//int32 CurrentPhaseNum = B2AICon ? B2AICon->GetCurrentPhaseNum() : 0;
	//if (B2AICon && PhaseDataArray.Num() > CurrentPhaseNum)
	//{
	//	B2AICon->ToNextPhase(); // 救俊辑 促矫 PhaseDataArray 甫 茫酒辑 荤侩且 巴.

	//	if (IsStunned())
	//		ClearBuff(EBuffType::Debuff_Stun);

	//	if (IsFrozen())
	//		ClearBuff(EBuffType::Debuff_Freezing);

	//	SetDamagedNum(0);
	//	OnDamageEnd();
	//}
}

void ABladeIICharacter::SetBuffMode(EUniqueBuffType BuffMode, bool bForced, float Amount, float Duration, bool bUseEffect, bool bUseTextEffect)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetBuffMode);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetBuffMode"));

	//if (Duration >= 0.f)
	//{
	//	Duration += GetBonusBuffDuration();
	//}

	//float fBuffAmount = 0.f;

	//switch (BuffMode)
	//{
	//case EUniqueBuffType::EBT_Hide:
	//{
	//	AddBuff(EBuffType::Buff_Hide, Duration, 0, this, bUseEffect, bUseTextEffect);
	//}
	//break;
	//case EUniqueBuffType::EBT_DecreaseDamage:
	//{
	//	// 肋焊搁 ESkillOption::ESO_DecReceiveDamage 蔼 捞侩茄芭烙 八捧荤 4锅胶懦篮 捞吧静绰淀
	//	fBuffAmount = (Amount * 0.01f) + GetBonusBuffDamageDecreaseAmount();
	//	fBuffAmount = FMath::Max(1.f - fBuffAmount, 0.f);
	//	AddBuff(EBuffType::Buff_DecreaseReceiveDamage, Duration, fBuffAmount, this, bUseEffect, bUseTextEffect);
	//}
	//break;
	//case EUniqueBuffType::EBT_ReflectDamage:
	//{
	//	AddBuff(EBuffType::Buff_ReflectDamage, Duration, Amount * GetBuffReflectionDamageRate(), this, bUseEffect, bUseTextEffect);
	//}
	//break;
	//case EUniqueBuffType::EBT_HealHPByAttack:
	//{
	//	fBuffAmount = Amount + GetBonusBuffHealHPByHit() * 100.f;	// HealingHPByPercent 俊辑 欺季飘 蔼阑 罐栏骨肺 捞扒 100 蚌秦初澜.
	//	AddBuff(EBuffType::Buff_HealHPByAttack, Duration, fBuffAmount, this, bUseEffect, bUseTextEffect);
	//}
	//break;
	//default:
	//	break;
	//}
}

void ABladeIICharacter::PreApplyInfluenceParam(EInfluenceType InfluenceType, float& ApplyAmount, float& ApplyDuration) const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_PreApplyInfluenceParam);
	//// Pre-process
	//switch (InfluenceType)
	//{
	//case EInfluenceType::EIT_SkillHealHP:
	//{
	//	ApplyAmount += GetBonusBuffHealHP() * 100.f;
	//}
	//break;
	//case EInfluenceType::EIT_Invincible:
	//case EInfluenceType::EIT_DecreaseReceiveDamage:
	//case EInfluenceType::EIT_IncreaseAttackDamage:
	//{
	//	ApplyAmount *= 0.01f;
	//	ApplyDuration += GetBonusBuffDuration();
	//}
	//break;
	//case EInfluenceType::EIT_IncreaseMoveSpeed:
	//{
	//	ApplyAmount *= 0.01f;
	//	ApplyAmount += GetBonusBuffIncMoveSpeed();
	//	ApplyDuration = 3.f;
	//}
	//break;
	//default:
	//	return;
	//}
}

// InAmount啊 0捞搁 胶懦俊辑 蔼掘绢咳
void ABladeIICharacter::ApplyInfluence(EInfluenceType InfluenceType, float InAmount, float InDuration, float InStateDuration, float InStateTriggerRate, bool bUseEffect, bool bUseTextEffect)
{
	/*BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ApplyInfluence);

	TArray<float> arAdditionalParams;

	switch (InfluenceType)
	{
	case EInfluenceType::EIT_IncreaseArmor:
		IncreaseArmor(InAmount, bUseEffect, bUseTextEffect);
		break;
	case EInfluenceType::EIT_SkillHealHP:
	{
		if (InAmount == 0.0f && GetBonusBuffHealHP() > 0)
			HealingHPByPercent(GetBonusBuffHealHP(), bUseEffect, bUseTextEffect);
		else
			HealingHPByPercent(InAmount, bUseEffect, bUseTextEffect);
	}
	break;
	case EInfluenceType::EIT_Invincible:
		AddBuff(EBuffType::Buff_Unbeatable, InDuration, InAmount, this, bUseEffect, bUseTextEffect);
		break;
	case EInfluenceType::EIT_DecreaseReceiveDamage:
	{
		if (InAmount == 0.0f && GetBonusBuffReceiveDamageDecreaseRate() > 0)
			AddBuff(EBuffType::Buff_DecreaseReceiveDamage, InDuration, GetBonusBuffReceiveDamageDecreaseRate(), this, bUseEffect, bUseTextEffect);
		else
			AddBuff(EBuffType::Buff_DecreaseReceiveDamage, InDuration, InAmount, this, bUseEffect, bUseTextEffect);
	}
	break;
	case EInfluenceType::EIT_IncreaseAttackDamage:
	{
		if (InAmount == 0.0f && GetBonusBuffAttackDamageIncreaseRate() > 0)
			AddBuff(EBuffType::Buff_IncreaseAttackDamage, InDuration, GetBonusBuffAttackDamageIncreaseRate(), this, bUseEffect, bUseTextEffect);
		else
			AddBuff(EBuffType::Buff_IncreaseAttackDamage, InDuration, InAmount, this, bUseEffect, bUseTextEffect);
	}
	break;
	case EInfluenceType::EIT_IncreaseMoveSpeed:
	{
		if (InAmount == 0.0f && GetBonusBuffIncMoveSpeed() > 0)
			AddBuff(EBuffType::Buff_IncreaseMoveSpeed, InDuration, 1.0f + GetBonusBuffIncMoveSpeed(), this, bUseEffect, bUseTextEffect);
		else
			AddBuff(EBuffType::Buff_IncreaseMoveSpeed, InDuration, 1.0f + InAmount, this, bUseEffect, bUseTextEffect);
	}
	break;
	case EInfluenceType::EIT_ReflectDamage:
		AddBuff(EBuffType::Buff_ReflectDamage, InDuration, InAmount, this, bUseEffect, bUseTextEffect);
		break;
	case EInfluenceType::EIT_HealHPByAttack:
		AddBuff(EBuffType::Buff_HealHPByAttack, InDuration, InAmount, this, bUseEffect, bUseTextEffect);
		break;
	case EInfluenceType::EIT_AllFreezeAttack:
	{
		arAdditionalParams.Add(InStateDuration);
		arAdditionalParams.Add(InStateTriggerRate);
		AddBuff(EBuffType::Buff_AllFreezeAttack, InDuration, InAmount, this, bUseEffect, bUseTextEffect, &arAdditionalParams);
	}
	break;
	case EInfluenceType::EIT_AllStunAttack:
		arAdditionalParams.Add(InStateDuration);
		arAdditionalParams.Add(InStateTriggerRate);
		AddBuff(EBuffType::Buff_AllStunAttack, InDuration, InAmount, this, bUseEffect, bUseTextEffect, &arAdditionalParams);
		break;
	case EInfluenceType::EIT_AllBurningAttack:
		arAdditionalParams.Add(InStateDuration);
		arAdditionalParams.Add(InStateTriggerRate);
		AddBuff(EBuffType::Buff_AllBurningAttack, InDuration, InAmount, this, bUseEffect, bUseTextEffect, &arAdditionalParams);
		break;
	case EInfluenceType::EIT_AllPoisonAttack:
		arAdditionalParams.Add(InStateDuration);
		arAdditionalParams.Add(InStateTriggerRate);
		AddBuff(EBuffType::Buff_AllPoisonAttack, InDuration, InAmount, this, bUseEffect, bUseTextEffect, &arAdditionalParams);
		break;
	default:
		break;
	}*/
}

void ABladeIICharacter::IncreaseArmor(float Amount, bool bUseEffect, bool bUseTextEffect)
{
	/*BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IncreaseArmor);
	if (Amount > 0.f)
	{
		Armor += Amount;

		UB2BuffModeEffectInfo* BuffModeEffectInfo = GetBuffModeEffectInfo();
		if (BuffModeEffectInfo)
		{
			if (bUseEffect) BuffModeEffectInfo->CharacterBuffEffect(EBuffType::Buff_IncreaseArmor, this);
			if (bUseTextEffect) BuffModeEffectInfo->SpawnCharacterBuffTextEffect(EBuffType::Buff_IncreaseArmor, this);
		}
	}*/
}

bool ABladeIICharacter::IsAttacking() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsAttacking);
	// Need to watch for new EAttackState entry..
	return (IsComboAttacking() || IsSkillAttacking() || IsCounterAttacking());
}

bool ABladeIICharacter::IsComboAttacking() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsComboAttacking);
	// Including dash 
	return (IsOneOfComboAttackState(AttackState) || AttackState == EAttackState::ECS_Dash);
}

bool ABladeIICharacter::IsSkillAttacking() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsSkillAttacking);
	return IsOneOfSkillAttackState(AttackState);
}

bool ABladeIICharacter::IsCounterAttacking() const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsCounterAttacking);
	return IsCounterAttackingState(AttackState);
}

bool ABladeIICharacter::IsApplyGuarding()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsApplyGuarding);
	//return !BuffManager->IsBuffActive(EBuffType::Debuff_Freezing) && IsGuarding();
	return false;
}

bool ABladeIICharacter::IsGuarding()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsGuarding);
	return IsGuardingState(AttackState);
}

bool ABladeIICharacter::HasSkillAttackSignal() const
{
	return IsOneOfSkillAttackState(AttackState);
}

bool ABladeIICharacter::HadHugeDamaged()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_HadHugeDamaged);
	return (DamagedNum > 0 && (LastDamageType == EAttackType::EAT_BigBounce || LastDamageType == EAttackType::EAT_KnockBack));
}

bool ABladeIICharacter::IsImmune(const FDamageInfo* DamageInfoPtr/* = nullptr*/)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsImmune);

	// 阁胶磐狼 Immune NotifyState篮 老窜 促 阜澜 - ForceStateAttack档 阜澜 ( 扁裙 夸没 )
	if (bImmune && IsPlayerCharacter() == false)
		return true;

	// - 快急 犬牢 炼扒 : DamageInfo俊 惑怕捞惑 碍力利侩捞 乐促搁 馆拜 / 搬加胶懦 / 捞姑滚橇惑怕 力寇 葛滴 惑怕捞惑 利侩
	if (IsForceStateAttack(DamageInfoPtr))
		return false;

	// - 老馆炼扒
	// 1. Skill 荤侩吝篮 Immune
	// 2. 捞姑 滚橇 乐阑锭
	bool IsIgnoreAbnormal = IsSkillAttacking() || bImmune || IsBuffActive(EBuffType::Buff_AbnormalStateImmune);
	if (IsIgnoreAbnormal)
		return true;

	return false;
}

bool ABladeIICharacter::IsGuardSuccess(const FDamageInfo* DamageInfoPtr /*= nullptr*/)
{
	if (!DamageInfoPtr)
		return IsApplyGuarding();

	return IsApplyGuarding() && DamageInfoPtr->GuardBreakLogicType != EGuardBreakLogicType::EGLT_UnableToGuard;
}

bool ABladeIICharacter::IsForceStateAttack(const FDamageInfo* DamageInfoPtr)
{
	if (IsCounterAttacking() || AttackState == EAttackState::ECS_WeaponSkill || IsBuffActive(EBuffType::Buff_AbnormalStateImmune))
		return false;

	return DamageInfoPtr ? DamageInfoPtr->bForceApplyStateDamage : false;
}

bool ABladeIICharacter::IsApplyDamageNotifyUI(const ABladeIICharacter* Attacker) const
{
	//auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//auto* GameRule = GameMode != nullptr ? GameMode->GetGameRule() : nullptr;
	//if (GameRule && GameRule->AllowOtherPlayerDamageNumNotify())
	//	return true;

	//// 1. Local捞 嘎篮 单固瘤绰 公炼扒 焊咯淋
	//if (IsPlayerControlled() && IsLocalCharacter())
	//	return true;

	//// 2. 郴啊 茄 傍拜捞搁 单固瘤 焊咯淋
	//auto* LocalCharacter = GetLocalCharacter();
	//if (LocalCharacter && LocalCharacter == Attacker)
	//	return true;

	//// 3. 郴啊 藕 QTE Actor狼 傍拜捞搁 单固瘤 焊咯淋
	//if (Attacker && Attacker->IsQTEActor(LocalCharacter))
	//	return true;

	return false;
}

// BP俊辑 静绰镑捞 乐澜.
bool ABladeIICharacter::IsStunned() const
{
	return IsBuffActive(EBuffType::Debuff_Stun);
}

// BP俊辑 静绰镑捞 乐澜.
bool ABladeIICharacter::IsFrozen() const
{
	return IsBuffActive(EBuffType::Debuff_Freezing);
}

//void ABladeIICharacter::TemporarySetMeshComponentUpdateFlag(TEnumAsByte<EMeshComponentUpdateFlag::Type> InNewFlag)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_TemporarySetMeshComponentUpdateFlag);
//	//// 楷崔酒辑 捞吧 荤侩窍绰 老捞 绝档废. 酒丛 弥家茄 弊繁 老捞 乐歹扼档 绢瞒乔 泪瘪 技泼窍妨绰 蔼篮 度鞍篮 芭扼 (AlwaysTickPoseAndRefreshBones 摆瘤 赣) 公矫秦档 酒公 呕 绝绰.
//	//checkSlow(!bTemporarilySetMeshCompUpdateFlag || (GetBaseMesh() && GetBaseMesh()->MeshComponentUpdateFlag == InNewFlag));
//
//	//if (GetBaseMesh() && !bTemporarilySetMeshCompUpdateFlag) // 捞傈俊 荤侩茄 芭 Restore 窍扁 傈俊 吝汗 柳涝 救 登档废. 傲 公矫凳.
//	//{// 泅犁蔼阑 归诀秦 初绊 泪矫 促弗 蔼阑 荤侩吝捞扼绊 钎矫秦 初澜.
//	//	TemporaryMeshCompUpdateFlagBackUp = GetBaseMesh()->MeshComponentUpdateFlag;
//	//	GetBaseMesh()->MeshComponentUpdateFlag = InNewFlag;
//	//	bTemporarilySetMeshCompUpdateFlag = true;
//	//}
//}

void ABladeIICharacter::RestoreTemporaryMeshComponentUpdateFlag()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_RestoreTemporaryMeshComponentUpdateFlag);
	//if (GetBaseMesh() && bTemporarilySetMeshCompUpdateFlag)
	//{
	//	GetBaseMesh()->MeshComponentUpdateFlag = TemporaryMeshCompUpdateFlagBackUp;
	//	bTemporarilySetMeshCompUpdateFlag = false;
	//}
}

void ABladeIICharacter::SetDropItemInfo(FDropItemInfo& InDropItemInfo)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetDropItemInfo);
	DropItemInfo = InDropItemInfo; // Simple copying.
}

FVector ABladeIICharacter::GetFleeLocation(int32 TestNumLocation, float Radius)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetFleeLocation);

	//ABladeIICharacter* NearestCharacter = Cast<ABladeIICharacter>(UGameplayStatics::GetNearestPlayerCharacter(GetWorld(), GetActorLocation()));
	//if (NearestCharacter == nullptr || NearestCharacter->IsPlayerControlled() == false)
	//	return GetActorLocation();

	//TArray<ABladeIICharacter*> FleeToCharacters;
	//FleeToCharacters.Add(NearestCharacter);

	//return B2AIUtil::GetFleeLocation(this, FleeToCharacters, TestNumLocation, Radius);
	return FVector();
}

void ABladeIICharacter::OverrideMaterial(class UMaterialInterface* NewMaterial, ECharacterMaterialOverrideType MatType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_OverrideMaterial);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::OverrideMaterial"));

	//BII_CHECK(MatType != ECharacterMaterialOverrideType::ECMO_None && MatType != ECharacterMaterialOverrideType::EDE_End);

	if (CurrentOverridenMatType == MatType)
	{
		return;
	}

	// 盔贰波 某教
	if (OverridenMatTypeList.Num() == 0)
	{
		for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
		{
			// Cache
			CachedMaterials.Add(GetMesh()->GetMaterial(i));
		}
	}

	//赣磐府倔 利侩
	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
	{
		// Cache
		GetMesh()->SetMaterial(i, NewMaterial);
	}

	OverridenMatTypeList.Add(MatType);
	CurrentOverridenMatType = MatType;
}

void ABladeIICharacter::RestoreOverrideMaterial(ECharacterMaterialOverrideType MatType)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_RestoreOverrideMaterial);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::RestoreOverrideMaterial"));

	//BII_CHECK(MatType != ECharacterMaterialOverrideType::ECMO_None && MatType != ECharacterMaterialOverrideType::EDE_End);

	//BII_CHECK(OverridenMatTypeList.Num() != 0);

	//OverridenMatTypeList.Remove(MatType);

	//if (OverridenMatTypeList.Num() == 0)
	//{
	//	CurrentOverridenMatType = ECharacterMaterialOverrideType::ECMO_None;
	//	//汗备
	//	if (CachedMaterials.Num() == GetMesh()->GetNumMaterials())
	//	{
	//		for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
	//		{
	//			GetMesh()->SetMaterial(i, CachedMaterials[i]);
	//		}
	//		CachedMaterials.Empty();
	//	}

	//	return;
	//}

	//// 巢篮巴吝俊 付瘤阜铂肺 利侩
	//ECharacterMaterialOverrideType LastMaterialOverrideType = OverridenMatTypeList[OverridenMatTypeList.Num() - 1];

	//if (UB2DamageEffectInfo* DamageEffectInfo = GetDamageEffectInfo())
	//	DamageEffectInfo->OverrideCharacterMaterial(LastMaterialOverrideType, this);
}

class UMaterialInterface* ABladeIICharacter::GetOriginalMaterial(int32 MaterialIndex)
{
	if (CurrentOverridenMatType != ECharacterMaterialOverrideType::ECMO_None && CurrentOverridenMatType != ECharacterMaterialOverrideType::EDE_End &&
		CachedMaterials.Num() > MaterialIndex)
	{
		return CachedMaterials[MaterialIndex];
	}

	return GetMesh()->GetMaterial(MaterialIndex);
}

void ABladeIICharacter::SetupLODInfoAsNPC()
{
	//// NPC 老何啊 LOD 累诀捞 凳俊 蝶扼 霸烙俊辑绰 老褒利牢 LODInfo 甫 霖促.
	//// 府家胶俊 利寸洒 悸诀窍妨聪 母媚 康惑俊辑 镜 芭 鞍绊..
	//// 咯变 敲饭捞绢 某腐磐贸烦 可记捞唱 霸烙葛靛 殿狼 惑炔阑 怖 呕 鞘夸绰 绝阑 淀. 老窜 利例洒 牢霸烙 敲饭捞 沥档 轰俊辑 LOD1 捞 唱坷霸.

	//// 咯扁辑 LODInfo 甫 技泼窍绰 皋浆绰 角力 俊叼磐俊 殿废等 府家胶捞促. PC 狼 版快贸烦 繁鸥烙 merge 等 皋浆啊 酒丛.
	//// 捞犯霸 技泼窍绊 唱搁 鞍篮 皋浆甫 傍蜡窍绰 促弗 咀磐(e.g. SkeletalMeshActor)啊 乐促搁 康氢阑 罐霸 瞪 巴捞促.
	//// BladeIIPlayer.cpp 率俊 捞客 吝汗等 内靛啊 乐瘤父 蜡瓶肺 哗 初阑 己拜篮 酒丛.

	//if (GIsEditor)
	//{
	//	return;
	//}

	//float WantedLOD1ScreenSize = 0.1f;
	//if (GConfig)
	//{
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("NPCMeshLOD1ScreenSize"), WantedLOD1ScreenSize, GGameIni);
	//}

	//USkeletalMeshComponent* SKMeshComp = GetMesh();
	//USkeletalMesh* MeshResource = SKMeshComp ? SKMeshComp->SkeletalMesh : NULL;
	////FSkeletalMeshObject* MeshResourceObj = InMeshComp ? InMeshComp->MeshObject : NULL;
	//if (!MeshResource /*|| !MeshResourceObj*/)
	//{
	//	return;
	//}

	//bool bScreenSizeChanged = false;
	//for (int32 LODIdx = 0; LODIdx < MeshResource->LODInfo.Num(); ++LODIdx)
	//{
	//	FSkeletalMeshLODInfo& MeshLODInfo = MeshResource->LODInfo[LODIdx];
	//	const float PrevScreenSize = MeshLODInfo.ScreenSize;
	//	// 荤角惑 LOD1 狼 ScreenSize 父 汲沥窃
	//	MeshLODInfo.ScreenSize = (LODIdx == 0 ? 0.0f : WantedLOD1ScreenSize);
	//	if (PrevScreenSize != MeshLODInfo.ScreenSize)
	//	{
	//		bScreenSizeChanged = true;
	//	}
	//}
	//if (bScreenSizeChanged)
	//{
	//	SKMeshComp->UpdateLODStatus();
	//	SKMeshComp->MarkRenderStateDirty();
	//}
}

void ABladeIICharacter::ClearEffectInAbnormalState()
{
	//// 绢怕摹等 捞棋飘 力芭(风俏, 烙葛呕篮 哗备)
	//if (auto* CharMesh = GetMesh())
	//{
	//	TArray<USceneComponent*> ArMeshChildren;
	//	ArMeshChildren.Empty();

	//	CharMesh->GetChildrenComponents(false, ArMeshChildren);

	//	for (USceneComponent* pComp : ArMeshChildren)
	//	{
	//		UParticleSystemComponent* pPSC = Cast<UParticleSystemComponent>(pComp);
	//		if (pPSC && pPSC->Template && !pPSC->Template->IsImmortal() && !pPSC->Template->IsLooping())
	//		{
	//			pPSC->Deactivate();
	//			pPSC->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	//			pPSC->UnregisterComponent();
	//			pPSC->DestroyComponent();
	//		}
	//	}
	//}
}

bool ABladeIICharacter::GetAbleToControl()
{
	return !IsBuffActive(EBuffType::Debuff_Freezing) && !IsBuffActive(EBuffType::Debuff_Stun) && !IsBuffActive(EBuffType::Buff_HitFreeze) && !HadHugeDamaged();
}

bool ABladeIICharacter::GetAbleToMove()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetAbleToMove);

	bool bBlockMoveByQTE = bInQTEState && QTEType != EQTEType::EQT_Mount;

	return bAbleToMove && !IsBuffActive(EBuffType::Debuff_Freezing) && !IsBuffActive(EBuffType::Debuff_Stun) && !HadHugeDamaged() && !bBlockMoveByQTE;
}

void ABladeIICharacter::SafeSetAnimBPClass(class UAnimBlueprintGeneratedClass* InAnimBPClass)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SafeSetAnimBPClass);
	// 泪瘪 救 焊捞霸 秦 躇促啊 AnimBP 技泼茄 饶俊 visibility 甫 促矫 囊促. 钢萍静饭靛 anim 包访 葛官老 券版俊辑狼 巩力啊 粱 乐绢辑 捞繁 窿阑 窃.
	//-> 匙捞萍宏 努贰胶 扁馆 spawn 捞饶 捞扒 绝沮. 荤角惑 SetAnimInstanceClass

	//const bool bWasHidden = bHidden;

	//if (!bWasHidden)
	//{
	//	//SetActorHiddenInGame(true);
	//}

	//if (GetMesh())
	//{
	//	GetMesh()->SetAnimInstanceClass(InAnimBPClass);
	//}

	//if (!bWasHidden)
	//{
	//	//SetActorHiddenInGame(false);
	//}
}

AB2MonsterSpawnPool* ABladeIICharacter::GetBirthPlace()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetBirthPlace);
	//return CachedBirthplaceInfo.Birthplace;
	return nullptr;
}

bool ABladeIICharacter::AmIInterestedBossMob() const
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_AmIInterestedBossMob);
	//// 泅犁 UI 俊 钎矫且 焊胶各牢瘤 咯何. 焊胶各捞 楷捞绢 唱棵 锭 菊俊巴捞 destroy 登扁 捞傈 第俊 巴捞 spawn 窍绰 鸥捞怪 巩力啊 乐绢辑 捞犯霸 备盒且 鞘夸啊 积辫
	//UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocBattle)
	//{ // 楷捞绢 悼老茄 Class 客 Variation 狼 Boss 各捞 殿厘窍搁 拱沸 捞吧肺档 家侩捞 绝阑 巴烙.
	//	return CachedBirthplaceInfo.bSpawnedAsBossMob && DocBattle->GetBossMobClass() == NPCClassEnum && DocBattle->GetBossMobClassVariation() == NPCClassVariation;
	//}
	return CachedBirthplaceInfo.bSpawnedAsBossMob;
}

void ABladeIICharacter::SetupDecalCompForCenterShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetupDecalCompForCenterShadowCommon);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetupDecalCompForCenterShadowCommon"));

	// 盔贰 BladeIICharacter 磊脚捞扼搁 InDecalCompToUse 绰 CenterShadow 老 巴烙. SetupControlledMatineePuppet 扁瓷阑 困秦 概俺函荐肺 罐绰促. 
	// 咯扁辑 哪器惩飘 register 鳖瘤 窍瘤绰 臼绰促.
	//if (InDecalCompToUse && AttachParent)
	//{
	//	//InDecalCompToUse->SetHiddenInGame(false);
	//	InDecalCompToUse->SetRelativeScale3D(FVector(200.f + ShadowMaxDistance, ShadowSizeCenter, ShadowSizeCenter));

	//	InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	//	InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//}
}

UB2NPCSingleClassInfo* ABladeIICharacter::GetMyNPCClassInfo()
{ // 敲饭捞绢 某腐磐, 趣篮 NPCClassEnum 捞 技泼登瘤 臼篮 版快 家侩 绝阑 巴.
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	UB2NPCClassInfoBox* NPCInfoBox = B2GI ? B2GI->GetMobClassInfoBox() : StaticFindMobClassInfoBox(this);
	if (NPCInfoBox)
	{
		return NPCInfoBox->GetNPCSingleClassInfo(NPCClassEnum, NPCClassVariation);
	}
	return nullptr;
}

#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test

bool ABladeIICharacter::bHideAllNPCs = false;
#endif