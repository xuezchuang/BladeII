// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "BladeIICharacter.h"
#include "BladeIIUtil.h"
#include "BladeIIGameMode.h"
#include "../InfoAsset/B2NPCClassInfoBox.h"
////#include "BladeIIGameMode.h"
////#include "B2StageGameMode.h"
////#include "B2DimensionGameMode.h"
//////#include "B2StageInfo.h"
////#include "B2StageManager.h"
////#include "Components/ArrowComponent.h"
////#include "BehaviorTree/BehaviorTree.h"
////#include "Runtime/AIModule/Classes/AIController.h"
////#include "Runtime/UMG/Public/UMG.h"
////#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
////#include "B2FloatingHPBar.h"
////#include "B2FloatingDamageNum.h"
////#include "B2FloatingAbnormalEffect.h"
////#include "B2UIManager_InGameCombat.h"
////#include "B2MonsterSpawnPool.h"
////#include "B2StageEventDirector.h"
////#include "Animation/SkeletalMeshActor.h"
////#include "Engine/SkeletalMesh.h"
////#include "Particles/ParticleSystem.h"
////#include "B2DamageEffectInfo.h"
////#include "B2BuffModeEffectInfo.h"
////#include "BladeIIProjectile.h"
////#include "Components/StaticMeshComponent.h"
////#include "Components/DecalComponent.h"
////#include "Event.h"
////#include "NavigationSystem.h"
////#include "B2AreaDamageActorBase.h"
////#include "B2UIDocHelper.h"
////#include "B2CombatStatEvaluator.h"
////#include "BladeIIAIController.h"
////#include "Trigger.h"
////#include "B2CounterAttackGameMode.h"
////#include "B2PCClassInfoBox.h"
////#include "B2NoAnimSkillInfo.h"
////#include "B2CharacterBuffManager.h"
////#include "B2GeneralBuffs.h"
//////#include "B2NPCClassInfo.h"
////#include "BladeIIPlayerController.h"
//////#include "B2AutoAIController.h"
////#include "B2AnimInstance.h"
#include "B2GameInstance.h"
//#include "B2AIUtil.h"
//#include "Components/DecalComponent.h"
//
//#ifdef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//#include "BladeIITestDummyNPC.h"
//#endif
////#include "B2CharacterBuffManager.h"

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
//
//UParticleSystemComponent* FCHSKCompAttachParticleSystemInfo::CreatePSCHelper(AActor* InCompOwner, USkeletalMeshComponent* InParentComp)
//{
//	////BLADE2_SCOPE_CYCLE_COUNTER(FCHSKCompAttachParticleSystemInfo_CreatePSCHelper);
//	//// 여기 Info 셋업 사용해서 PSC 생성 및 attach 하는 기본 절차.
//	//if (InCompOwner && InParentComp)
//	//{
//	//	// InParentComp 는 InCompOwner 에 포함된 걸로 가정을..
//	//	checkSlow(InParentComp->IsAttachedTo(InCompOwner->GetRootComponent()) || InParentComp == InCompOwner->GetRootComponent());
//
//	//	UParticleSystem* LoadedPS = GetParticleSystemAsset();
//	//	if (LoadedPS)
//	//	{
//	//		UParticleSystemComponent* CreatedComp = NewObject<UParticleSystemComponent>(InCompOwner, FName(*GetCompCreateName(InCompOwner)), RF_Transient);
//	//		if (CreatedComp)
//	//		{
//	//			CreatedComp->bReceivesDecals = false;
//	//			//CreatedComp->bIgnoreBoneRotation = bIgnoreBoneRotation;
//	//			CreatedComp->SetTemplate(LoadedPS);
//	//			CreatedComp->SetRelativeScale3D(AttachScale3D);
//	//			CreatedComp->SetRelativeLocation(LocationOffset);
//	//			CreatedComp->SetRelativeRotation(RotationOffset);
//	//			CreatedComp->AttachToComponent(InParentComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), AttachSocketName);
//	//			CreatedComp->RegisterComponent();
//	//			CreatedComp->Activate(true);
//	//			return CreatedComp;
//	//		}
//	//	}
//	//}
//	return NULL;
//}
//
//#if WITH_EDITOR
//void FCHSKCompAttachParticleSystemInfo::EditorClearPSAsset()
//{ // 이거 세팅하지 말아야 할 곳에서 세팅하면 PostEdit 에서 제거하려는 거.
//	ParticleSystemAsset.Reset();
//}
//#endif

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
	else if (bSpawningNoDamageGuard) // 대체로 SpawnPool 에서 Spawn 시키면서 등록하기 전.
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
//
//TArray<FName> ABladeIICharacter::BaseBPCopyExcludeProperties;

ABladeIICharacter::ABladeIICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
//	NPCClassEnum = ENPCClass::ENC_End;
//	NPCClassVariation = ENPCClassVariation::ENCV_Normal;
//	// TODO : Init Data for Character Type
//	CharacterType = ECharacterType::ECT_MeleeMob;
//	// Need to set other data
//	AttackState = EAttackState::ECS_None;
//	bSignalAttack = false;
//	Health = 100.f;
//	MaxHealth = 100.f;
//	Armor = 0.f;
//	MaxArmor = 0.f;
//	DamagedNum = 0;
//	LastDamageType = EAttackType::EAT_Default;
//	LastStateDamageType = EStateAttackType::EDT_Normal;
//	QTEPCClassEnum = EPCClass::EPC_End;
//	CurrentOverridenMatType = ECharacterMaterialOverrideType::ECMO_None;
//	bAbleToMove = true;
//	bMovementPreventByStageEvent = false;
//	CachedTimeDilation = 1.f;
//	B2TimeDilationForAnimNS = 1.f;
//	B2CachedDilationForAnimNS = 1.f;
//
//	FinalADScale = 1.0f;
//	FinalHPScale = 1.0f;
//
//	StageEventShowState = EStageEventShowState::ESES_None;
//	bPlayingDirectedStageEvent = false;
//
//	DetectDistance = 800.f;
//
//	CameraTargetBoomRotOffset = FRotator(0.0f, 0.0f, 0.0f);
//
//	AIAttackIndex = 0;
//
//	CharacterLevel = 1;
//	CharacterDamageRate = 100;
//
//	bNeedSpawnCheck = false; // 초기 spawn state 를 사용하는 경우 true 로 켜주고, AI 에 의해 false 로 리셋되면 움직이기 시작.
//
//	bForceSpawAnim = false;
//
//	DamageMultiplier = 1.f;
//	BaseDamageRate = 1.f;
//	ReflectDamageAmount = 1.f;
//	HealHPByAttackRate = 0.f;
//	StateAttackType = EStateAttackType::EDT_End;
//	StateAttackDuration = 0.f;
//	StateAttackAmount = 0.f;
//
//	ShadowMaterial = NULL;
//	ShadowSizeCenter = 120.f;
//	ShadowMaxDistance = 50.f;
//
//	DamageNumZOffset = 0.0f;
//
//	CenterShadow = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("CenterShadowComponent"));
//	//CenterShadow->bAbsoluteLocation = true;
//	CenterShadow->SetUsingAbsoluteRotation(true);
//	CenterShadow->SetRelativeLocation_Direct(FVector::ZeroVector);
//	CenterShadow->SetRelativeScale3D_Direct(FVector(200.f + ShadowMaxDistance, ShadowSizeCenter, ShadowSizeCenter));
//	CenterShadow->SetRelativeRotation_Direct(FRotator(-90.f, 0.f, 0.f));
//	CenterShadow->DecalSize = FVector(1.f, 1.f, 1.f); // for 4.11
//	//CenterShadow->bHiddenInGame = true;
//	CenterShadow->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	CenterShadow->Deactivate();
//
//	// Set size for player capsule
//	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
//
//	// Rotate character to camera direction for AIPlayerController
//	bUseControllerRotationPitch = false;
//	bUseControllerRotationYaw = true;
//	bUseControllerRotationRoll = false;
//
//	// Configure character movement
//	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
//	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
//	GetCharacterMovement()->bConstrainToPlane = true;
//	GetCharacterMovement()->bSnapToPlaneAtStart = true;
////	//GetCharacterMovement()->bForceMaxAccel = true;
////	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
////	GetCharacterMovement()->bIgnoreForceDuringRootmotion = false;
////
////	if (!GMinimalDLCFrontMode)
////	{ // DLC Front 모드 리소스로딩 최대한 제거
////
////////////////////////////////////////////////////////////////////////////////////
//////>>> Widget classes specification begin
////
////		FString DefaultFloatingHPBarWidgetClassPath;
////		GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultFloatingHPBarWidgetClass"), DefaultFloatingHPBarWidgetClassPath, GGameIni);
////		static ConstructorHelpers::FClassFinder<UB2FloatingHPBar> DefaultFloatingHPBarWidgetClass(*DefaultFloatingHPBarWidgetClassPath);
////		if (DefaultFloatingHPBarWidgetClass.Class != NULL)
////		{
////			FloatingHPBarWidgetClass = DefaultFloatingHPBarWidgetClass.Class;
////			BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("FloatingHPBarWidgetClass"))); // MinimalDLCFrontMode 로 인해 비어있을 BaseBP 쪽 값을 가져오지 않기 위해 이름 추가
////		}
////	}
////
////	DamageSocketName = FName(TEXT("S_hit01"));
////
////	bNeedOscillation = false;
////	DeferredImpulse = FVector::ZeroVector;
////
////	// Disable receive decal
////	GetMesh()->bReceivesDecals = false;
////	GetMesh()->bPerBoneMotionBlur = false;
////	GetMesh()->bEnablePhysicsOnDedicatedServer = false;
////	GetMesh()->CanCharacterStepUpOn = ECB_No;
////
////	// Per Mesh AnimNotify State Data
////	B2CharEmissiveRemainTime = 0.f;
////	ChainBitField = 0;
////
////	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
////
////	TemporaryMeshCompUpdateFlagBackUp = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones; // 이게 아마도 가장 안전한 값일 듯. 물론 이게 그대로 적용되는 건 아님.
////	bTemporarilySetMeshCompUpdateFlag = false;
////
////	RegenerateHPPercentagePerSecond = 0.f;
////	RegenerateHPAmountPerSecond = 0.0f;
////	bRegenerateHPByPercent = true;
////
////	for (int32 i = static_cast<int32>(EDamageElement::EDE_None); i < static_cast<int32>(EDamageElement::EDE_End); ++i)
////	{
////		CachedStateMaterials.Add(NULL);
////	}
////
////	CachedBaseMaxWalkSpeed = 1000.f;
////
////	CachedStateDamageStates.AddZeroed(GetStateAttackTypeArrayNum());
////
////	RandDamageAnimType = EAttackType::EAT_Default;
////
////	QTEType = EQTEType::EQT_End;
////	QTEInvokeHPPercent = 0.0f;
////	QTEMountTime = 20.f;
////	QTEEnableTime = 5.f;
////	QTELimitEnable = false;
////
////	TeamNum = -1;
////
////	QTEEnableTeam = ETeamType::ETT_End;
////
////	bCommittedSuicideAtDeadEnd = false;
////	bIsDeadSinking = false;
////	DeadSinkZOffsetPerSecond = 0.f;
////	DeadSinkLeftTime = 0.f;
////
////	bSpawningNoDamageGuard = false; // 모든 경우에 처음부터 true 였다가 spawn 처리 후 false 로 두는 게 아니라 일부의 경우만 잠시간 true 로 사용.
////
////	bForceReceivdAbsoluteDamage = false;
////	bForceReceivdAttackCountDamage = false;
////	FullAttactCountSize = 0;
////
////	InitialInfluence = EInfluenceType::EIT_End;
////	InitialInfluenceAmount = 0.f;
////	InitialInfluenceDuration = 0.f;
////	InitialInfluenceStateDuration = 0.f;
////	InitialInfluenceStateTriggerRate = 0.f;
////
////	InjectedGameRule = nullptr;
////
////	nCanBeDamagedFalseCount = 0;
////
////	bInvincible = false;
////	m_bIsUndead = false;
////
////	DamageReservationInfos.Empty();
////
////	BuffManager = nullptr;
////
////	OverridenMatTypeList.Empty();
////
////	HPBarSocket = nullptr;
////	bHPBarAttachedToSocket = false;
////
////	SummonOwner = nullptr;
////
////	bHideFromEventDirector = false;
////
////	bIgnoreRotateByDamage = false;
////	bEmissiveActiveByBuff = false;
////
////	bAllowRecoilAnimation = true;
////
////	bBloodStoneAura = false;
////	BloodStoneType = EBloodStoneType::EBST_UnBeatable;
////	BloodStoneAmount = -1.f;
////	BloodStoneBuffLogicType = EDamageLogicType::ELT_Normal;
////
////	SpawnNotifyMsgString = TEXT("");
////	SpawnNotifyMsgTime = -1.f;
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
	if (!bMovementPreventByStageEvent) // bMovementPreventByStageEvent 의 실질적인 용도
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
	bAbleToMove = true; // 실제로는 이걸 세팅해 주어야 함.
}

void ABladeIICharacter::PreventMovementByStageEvent()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_PreventMovementByStageEvent);
	bMovementPreventByStageEvent = true;
	bAbleToMove = false; // 실제로는 이걸 세팅해 주어야 함.
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
	// 들어오는 값은 % 로 표시되는 단위로 가정. 0.01 스케일링은 여기서 함.
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

	//	// 체력회복UI(데미지표기 이벤트에 어마운트 -값을주면 회복표기인듯)
	//	// 플레이어만 적용

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
	//		SetRegenerateHPRate(0.f); // 끈다.
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
	//	// AnimNotifyState_B2MobStateUndead에서 m_bIsUndead = true로 만듬(나중에 죽는 상태)
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
	//else //이거는 이상 상태이기는 하나 예외 처리용으로 추가
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
//	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_OnDeath);
//#if !UE_BUILD_SHIPPING
//	// 여긴 좀 더 정보를 줘서 트랙킹.
//	FString ScopedTrackString = FString::Printf(TEXT("ABladeIICharacter::OnDeath %s by %s"), *GetName(), PawnInstigator ? *PawnInstigator->GetName() : TEXT("Unknown"));
//	B2_SCOPED_TRACK_LOG(ScopedTrackString);
//#endif
//
//	if (bIsDying)
//	{
//		return;
//	}

	//bReplicateMovement = false;
	//bTearOff = true;
	//bIsDying = true; // Animation Blueprint 로 보내는 신호. 스테이지 진행이나 연출 트리거 차원에서의 처리는 네이티브서 하지만 몬스터의 전투 모션을 중단하고 dead 모션을 취하는 건 이걸 통해 들어감.
	//bCanBeDamaged = false;
	SetQTEEnabled(false);
	//bAbleToMove = false;

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
	// 이 시점에서 바로 DestroyUIManager 를 하지 않는다. 왜냐하면 DamageNum 같은 애들이 일정 기간은 남아있어야 하기 때문. 
	// 적당히 1초 후에 불리도록 타이머를 설치하는데 그 전에 파괴가 되면 Destroyed 에서 처리가 될 거고, 1초가 짧다면.. 음.. 그건 얘기가 들어오면 생각.
	//bool ResurrectionGameModeType = B2GM && B2GM->GetUIManager_InGameCombat() && B2GM->GetUIManager_InGameCombat()->CanResurrectionGameModeType();

	//if (ResurrectionGameModeType == false)
	//{
	//	GetWorldTimerManager().SetTimer(DelayedDestroyUIManagerTH, this, &ABladeIICharacter::DelayedDestroyUIManagerCB, 1.0f, false);
	//	HideFloatingHPBar(true); // 대신 HPBar 는 바로 안 보이게
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

	ConditionalReportMyDeathToBirthplace(); // 뭔가 재진입을 하는 케이스가 있다.. ㅡㅡ

	//CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Signal(this);

	//for (auto Key : EventCancelCastAreaDamageSubscribeKeys)
	//	CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Unsubscribe(Key);

	// 본인이 죽었다고 게임모드에 알림(피니쉬 공격 연출용도로 작성)
	// ReportMyDeathToBirthplace 에서 보내는 통지와 중복되는 면이 있는데 현재로선 역할이 조금씩 다름.
	if (B2GM)
		//B2GM->NotifyCharacterDead(this, PawnInstigator, DamageCauser);

	TriggerKillMonster();

	//CharacterDeathClass<ABladeIICharacter*>::GetInstance().Signal(this);

	// 버프제거
	//BuffManager->ClearAllBuffs();
}

void ABladeIICharacter::DieForQTE()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DieForQTE);
	//// QTE 타겟으로 걸린 몹의 사망 처리. 일반적으로 이들은 OnDeath 를 통하지 않고 그 중 필요한 처리를 따로 여기서 해 준다.

	//AB2StageManager::GetCacheStageKeepEssentialData().AddQTECount(QTEStateNum);
	//DropItem();

	//ConditionalReportMyDeathToBirthplace(); // SpawnPool 로의 사망신고
}

void ABladeIICharacter::ReportMyDeathToBirthplace()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ReportMyDeathToBirthplace);
	// SpawnPool 에서 출생한 몹의 경우 SpawnPool 쪽으로 통지를 날려서 리스트에서 비우는 등의 처리를 하도록 함. 일반적인 Dead 루트를 따르지 않는 경우라도 이건 처리를 해야 게임 진행이 된다.
	// 일반적인 스테이지 진행에서의 몬스터에 대해 가장 실질적인 의미의 사망 신고
	//if (CachedBirthplaceInfo.Birthplace)
	{
		CachedBirthplaceInfo.bOwnerDeathReported = true; // 몬스터 사망 신고 여부를 체크하는데 쓰일 수 있다.

		if (CachedBirthplaceInfo.bIsSummoned)
		{
			// bIsSummoned 에 따라 이벤트가 다른데 합쳐서 안될 것도 없음 ㅡ.ㅡ
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

	//// 무브투에 쓰이는 도착여부 판단변수 조작(플레이어 기준으로 맞춰줌)
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

	//// Animation Blueprint 를 통해 Dead 애니메이션을 하면 설치한 AnimNotify 를 통해 오게 된다.
	//// 일반적으로는 로직 상에서의 죽는 처리 (데미지를 통해 체력이 0 아래로 떨어지거나 자폭 등) 이후 들어와야 한다. 
	//// SpawnPool 에 등록된 몬스터라면 그쪽에서도 이미 사망신고 된 이후
	//// 아직 언리얼 Object 차원에서는 valid 하나 여길 지나면서 언리얼 Object 제거 countdown 이 될 것임.

	//if (IsPlayerControlled())
	//{
	//	return; // 일단 Mob 전용
	//}

	//if (IsAttacking() && // DeadEnd 로 왔는데 Attack 상태라는 것은 Attack 애니메이션에 DeadEnd 가 있다는 것이 됨. 즉, 자살공격.. 케이스만 있는 게 희망사항인데..
	//	// 단, 원래 의도와 다르게 이렇게 들어오는 경우가 없진 않다.. 그리 깔끔하진 않은데..
	//	!IsInQTEState() && !CachedBirthplaceInfo.bOwnerDeathReported)
	//{
	//	// 이 상태 그대로 진행하면 이 캐릭터가 제거는 되지만 게임 진행에 영향을 주는 OnDeath 에서의 처리가 생략되므로 Suicide 를 직접 콜해준다.
	//	bCommittedSuicideAtDeadEnd = true; // AnimBP 에서 통상적인 Dead Anim 핸들링을 막기 위해 추가 플래그 세팅. 왜냐하면 Dead Anim 핸들링은 이미 된 것이므로.
	//	Suicide();
	//}

	//// 만일의 경우를 대비하는 최후의 확인사살 코드. 이 시점에서도 사망 신고가 안된 알려지지 않은 원인이 있다면 확인할 필요가 있다.
	//if (CachedBirthplaceInfo.Birthplace && !CachedBirthplaceInfo.bOwnerDeathReported && !IsInQTEState()) // QTE 에서는 별도로 사망신고를 하는데.. 이땐 QTEState 가 해제되는 거 같기도 하고..
	//{
	//	ensureMsgf(false, TEXT("Detected dead report miss by unknown reason. Forcing dead report.. %s, Wave%d_%d"), *this->GetName(), CachedBirthplaceInfo.WaveNumber, CachedBirthplaceInfo.WaveObjIndex);
	//	ReportMyDeathToBirthplace();
	//}

	//// 게임모드에 알릴필요가 있음
	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (pGM)
	//{
	//	//pGM->NotifyAnimMobDeadEnd(this);
	//}

	//// bInUseDeadRagDoll 은 AnimBP 에서 오는 설정.
	//if (bInUseDeadRagDoll)
	//{
	//	SetRagdollPhysics(InRemainLifeSpan, !bInUseDeadRagDoll); // 이 안에서 GC 카운트 다운을 할 것임.
	//	GetCharacterMovement()->StopMovementImmediately();
	//}
	//else
	//{
	//	WrappedSetLifeSpan(InRemainLifeSpan); // GC 카운트 다운..
	//	PreventMovementByStageEvent(); // 물리를 사용하지 않으면서 movment stop 까지 하면 정말 재미없음.
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
		return NULL; // 일단 Mob 전용
	}

	class USkeletalMeshComponent* SKMeshComp = GetMesh();
	UMaterialInstanceDynamic* CreatedDeadBodyMID = NULL;

	if (InDeadBodyMI) // 죽을 때 바꿔치는 머티리얼. 까맣게 타들어가는 효과..
	{
		// 실질적으로 하나밖에 처리를 할 수 밖에 없는데 AnimBP 에서 OutCreatedDeadBodyMID 하나만 받아서 파라미터를 세팅할 것이기 때문.
		for (int32 MII = 0; MII < SKMeshComp->GetNumMaterials(); ++MII)
		{
			if (!CreatedDeadBodyMID)
			{ // MID 는 0번에서만 만들게 될 것.
				CreatedDeadBodyMID = SKMeshComp->CreateDynamicMaterialInstance(MII, InDeadBodyMI);
			}
			else
			{ // 나머지 섹션은 만든 거 하나가지고 다 똑같이 세팅.
				SKMeshComp->SetMaterial(MII, CreatedDeadBodyMID);
			}
		}

		if (CreatedDeadBodyMID && InDeadBodyMIParamName != NAME_None)
		{
			// 까맣게 타들어가며 회한어린 인생을 마무리하는 효과를 시작한다..
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
	// 게임 로직상에서의 사망 처리는 물론이고 언리얼 오브젝트 차원에서의 GC 직전까지 간 상황으로 (장례식에 49재까지 끝났음) 날 사용하는 곳이 있다면 그만 잊도록 최후의 브로드캐스트를 한다.	
	// ABladeIICharacter::Destroyed 에 넣으면 더 넓게 커버 가능한데 그럴 경우 SpawnPoolWaveMobPhaseChangeClass 를 통해 StageEventDirector 에서 phase 변화로 다시 spawn 한 몹을 바꿔치는 게 안 됨. 물론 그쪽 구현을 좀 바꾸면 가능은 하다.
	//PleaseForgetMeClass<ABladeIICharacter*>::GetInstance().Signal(this);

	Super::LifeSpanExpired();
}

bool ABladeIICharacter::IsBuffActive(EBuffType BuffType) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_IsBuffActive);

	//if (auto* BuffObj = GetBuff(BuffType))
	//	return BuffObj->IsActive();

	return false;
}

//class UB2Buff_Base* ABladeIICharacter::GetBuff(EBuffType BuffType) const
//{
//	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetBuff);
//
//	//if (auto* BuffManagerObj = GetBuffManager())
//	//	return BuffManagerObj->FindBuff(BuffType);
//
//	return nullptr;
//}

bool ABladeIICharacter::ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage)
{
	return false;
}

bool ABladeIICharacter::RequestDamage(const float Damage, const FDamageInfo* DamageInfo, ABladeIICharacter* DamageCauser, bool NetBroadcast)
{
	//// 피격 주체는 무조건 this == ROLE_Authority!!
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

	// 진짜 Local Player이거나
	if (IsPlayer && IsLocal)
		return true;

	// LocalPlayer에게 조종당하는 중 (QTE) 중이면 
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

	//if (bSpawningNoDamageGuard) // 대체로 SpawnPool 에서 Spawn 시키면서 등록하기 전.
	//	return false;

	//if (!IsEnemy(DamageCauser))
	//	return false;

	//if (IsBuffActive(EBuffType::Buff_Hide) || IsBuffActive(EBuffType::Buff_Unbeatable))
	//	return false;

	// bApplyNPC
	//if (GetTeamNum() == INDEX_NONE && DamageType->bApplyToNPC)
	//	return true;

	// 액터에있는 플래그도 체크
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
//		// 대체로 DamageType 이 없는 상황은 연출을 통해 플레이 되는 attack 애니메이션에서 damage notify 가 나가는 거. InterpTrackAnimControl 의 SkipDamageNotifiersOnly 를 체크.
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
//			// 플래그에따라 받는데미지 절대값으로 변환
//			EDamageLogicType DamageLogicType = DamageInfo.DamageLogicType;
//			if (bForceReceivdAbsoluteDamage)
//				DamageLogicType = EDamageLogicType::ELT_Absolute;
//
//			// 플래그에따라 받는데미지 로직을 타수로 변환
//			if (bForceReceivdAttackCountDamage)
//				DamageLogicType = EDamageLogicType::ELT_AttactCount;
//
//			ActualDamage = GetDamageByDamageLogic(ActualDamage, DamageLogicType, Attacker, DamageCauser, DamageInfo, bCriticalDamage);
//
//			// 데미지 멀티플 노티파이량 적용
//			ActualDamage *= DamageMultiplier;
//
//			// Shield 로직 - 데미지와 체력에 큰 영향을 주며 일반 타격과도 밀접한 연관이 있으므로 버프가 아닌 TakeDamage 로직에서 처리
//			const float ShieldAmount = GetShield();
//			const float RemainShieldAmount = ShieldAmount - ActualDamage;
//			float AbsorbeDamage = 0.0f;
//
//			if (RemainShieldAmount <= 0.f)	// Shield 이상의 Damage가 들어옴
//			{
//				AbsorbeDamage = ShieldAmount;
//				ActualDamage = ActualDamage - ShieldAmount;
//			}
//			else // Shield 미만의 Damage 가 들어옴 == Shield가 남음
//			{
//				AbsorbeDamage = ActualDamage;
//				ActualDamage = 0.f;
//			}
//
//			SetShield(ShieldAmount - AbsorbeDamage);
//			// Shield 로직 - 데미지와 체력에 큰 영향을 주며 일반 타격과도 밀접한 연관이 있으므로 버프가 아닌 TakeDamage 로직에서 처리
//		}
//
//		// 0 초과 (이상 아님) 데미지는 최소데미지 1로 고정 (== 0.02 데미지는 1로)
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
//		// Role_Authority 가 아니면 재지 말고 바로 Damage로 활용
//		ActualDamage = Damage;
//	}
//
//	// Damage Log 남기기 - Network모드도 RemoteTakeDamage 루트를 따라와서 실행되어야 하므로 Role체크 안함
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
//	if (Attacker && IsPlayerControlled() == false) // 플레이어 이외의 몹에만 영향.
//	{
//		if (Attacker->IsHeroMode())
//			SetHealth(0.f);
//	}
//
//	// CheatCommand - zombie
//	if (IsImmortalMode())
//		SetHealth(1.f);	// 피가 0 이하면 정상 피격로직으로 흘러가지 않음 ( if(Health <= 0.f) 로직을 탐 )
//#endif
//
//	// DamageEffectInfo 3D DamageNum 대신 2D UI 기반 DamageNum. 
//	if (ActualDamage > 0.f) // Shield 에서 흡수한 것은 타입 다르게 해서 따로 표시하게 될 수도..
//	{ // Critical damage 표시도 여기서 함.
//
//		// Remote Pawn vs Remote Pawn 간의 Damage를 굳이 나의 Local에서 표시 할 필요가 있을지는 의문
//		// [171214_YJ] GameMode Rule에 다른 캐릭터도 표시하는 모드 인지, 
//		// 다른캐릭터도 표시하지 않는다면 때리거나 맞은사람이 나 인지 체크해서 출력.
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
//		// bIgnoreNotifyToAttacker설정된건 히트프리즈 안검
//		// 이뮨이면 히트프리즈 패스
//		// 가드상태도 히트프리즈 패스
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
//	// Immune 상태에서 상태이상 불가
//	if (IsImmune(&DamageInfo) == false)
//	{
//		TakeDamageAbnormalState(DamageInfo, DamageCauser);
//		ProcessTakeDamageAnimation(DamageInfo, DamageCauser);
//	}
//
//	// 이뮨이더라도 가드중엔 데미지넘+(반격을위해)
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
	// 1. QTE 마운트 상태에서는 Damage Animatino 실행 X
	if ((GetQTEEnabled() && QTEType == EQTEType::EQT_Mount))
		return false;

	const auto CurrentDamageType = DamageInfo.DamageType;

	// 2. 넉백 or 빅바운스 데미지를 허용하지 않으면 Animation 실행 X ( 몬스터의 경우만 )
	if (IsMob())
	{
		if (CurrentDamageType == EAttackType::EAT_KnockBack && !IsAllowState(EStateAttackType::EDT_NockBack))
			return false;

		if (CurrentDamageType == EAttackType::EAT_BigBounce && !IsAllowState(EStateAttackType::EDT_BigBounce))
			return false;

		// 데미지 애니메이션을 사용하지 않는 경우, 모두 실행X
		if (!bAllowRecoilAnimation)
			return false;
	}

	// 3. CBT 개선 -  Skill 사용중은 제외
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
			1. Guard 안함
			1-1. ArmorBreak : true - 아머브레이크 발동
			1-2. ArmorBreak : false - Armor량만 깎임

			2. Guard 중 GuardBreak X
			2-1. ArmorBreak : true - 아머브레이크 무시
			2-2. ArmorBreak : false - Armor량 깎지 않음

			3. Guard 중 GuardBreak O
			3-1. ArmorBreak : true - 아머브레이크 발동
			3-2. ArmorBreak : false - Guard만 해제
		*/

		const bool IsArmorBreak = IsPreventByGuard == false && (IsArmorLower || IsForceArmorBreak);

		PreDamageAnimation(DamageInfo, IsArmorBreak);
		PostDamageAnimation(DamageInfo, DamageCauser);

		const bool bForceBounceOrKnockback = DamageInfo.bForceApplyStateDamage;

		// 이 시점에서의 LastDamageType이 B2AnimInstance에 전달 될 Type
		// DamageInfo.DamageType을 기본으로 현재 캐릭터의 상태에 따라 PreDamageAnimation에서 LastDamageType값을 확정
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
		// Player 역시 Default데미지라도 ArmorBreak라면 TinyDamage 처리
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
		// Monster RightSwing 시 AnimationFreeze 되어버리는 이상한 버그가 있음... 
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
	//		// 가드이펙트 Yaw회전이외엔 적용불가능. 고려 안되어있다.
	//		// 내위치랑 가해자위치기반으로 Z축 제거해서 이용
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

	//	// 가드 브레이크도 가드 성공으로 쳐서 추가조건 걸음
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
	"최종 데미지 = 총 공격력 (주캐릭터 공격력 + 주캐릭터 무기 공격력 + 부캐릭터 공격력 + 부캐릭터 무기 공격력)
		x 주캐릭터 장비 옵션	1. EnemyTotalAttack ( 총공격력 * 주캐릭터 장비 옵션 * 0.01f)
		x 배율 조정			2. OriginalDamage
		x 데미지 적용율		3. TotalDamageRate ( LevelWeight * DamageRate * CharacterDamageRate(기본100) * 0.01f)
		x 랜덤 데미지"		4. RandomDamage
		x 최종 데미지 배율		5. 최대 데미지 도달 방지를 위함
		x 크리티컬 데미지
	*/

	//BII_CHECK(Attacker);

	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//// 1. EnemyTotalAttack ( 총공격력 * 주캐릭터 장비 옵션 * 0.01f)
	//const float AttackCoefficient = (GameMode != nullptr) ? GameMode->GetAttackCoefficient() : CombatStatEval::GetDefaultCombatCoefficient();
	//const float EnemyTotalAttack = Attacker->TotalAttack * 0.01f * AttackCoefficient;

	//// 3. TotalDamageRate(IncreaseRate * DamageRate * ExtraDamageRate * CharacterDamageRate(기본100) * 0.01f)
	//const float IncreaseRate = GetBaseDamageIncreaseRate(Attacker, DamageInfo, DamageCauser);
	//const float DamageRate = GetDamageRate(Attacker, DamageInfo, DamageCauser);
	//const float ExtraDamageRate = GetExtraTakeDamageRate();	// ex) Poision 버프시 10% 데미지 더

	//const float TotalDamageRate = IncreaseRate * DamageRate * ExtraDamageRate * (CharacterDamageRate * 0.01f/*기본 100*/);

	//// 4. RandomDamage
	//const float RandomDamage = (GameMode != nullptr) ? GameMode->GetDamageRandomRate() : 1.f;

	//// 5. 최종 배율 Adjustment
	//// 난이도 조절용 - 데미지 99999 도달 방지를 위한 hp / Damage 비례 상수
	//const float AdjustmentRate = IsMob() ? GameMode->GetPVEProportionalConstant() : 1.f;

	//// 최종 Damage 계산 진행

	//// Step 1. Critical, 밸런싱 조절 전
	//float ActualDamage = EnemyTotalAttack
	//	* OriginalDamage
	//	* TotalDamageRate
	//	* RandomDamage
	//	* AdjustmentRate;

	//// Step 2. Critical 데미지 계산
	//float CriticalRate = DamageInfo.CriticalRateOverride == -1 ? Attacker->GetCriticalRate() : DamageInfo.CriticalRateOverride;
	//CriticalRate *= GetResistCriticalRate();
	//CriticalRate = FMath::Clamp(CriticalRate, 0.f, 1.f);
	//OutIsCritical = FMath::FRand() < CriticalRate;
	//const float CriticalDamageRate = OutIsCritical ? (DamageInfo.CriticalDamageRateOverride == -1 ? Attacker->GetCriticalDamageRate() : DamageInfo.CriticalDamageRateOverride) : 1.f;

	//ActualDamage *= CriticalDamageRate;

	//// Step3. PVP 일 경우 데미지 보정 - 최대체력 20% 이상의 데미지(혹은 모드별 정의된 최대량)는 한방에 넣을 수 없다
	//if (GameMode && GameMode->IsPVPGameMode())
	//{
	//	const float MaxActualDamageRatio = GameMode->GetPVPMaxDamageRatioByGameModeType();
	//	ActualDamage = FMath::Min<float>(ActualDamage, GetMaxHealth() * MaxActualDamageRatio);

	//	if (IsApplyGuarding())
	//		ActualDamage *= 0.2f;
	//}

	////step4. Additional Damage 처리 그냥 Max(0, (공격자 IncAdditionalDamage - 방어자 DecAdditionalDamage))
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

	float IncreaseRate = Attacker->GetDamageIncreaseRate();	// 증가값이므로 1.f + @ 로 되어야 함 -> ( + 연산 )

	// 10%증가면 0.1로 값 입력되었다고 생각한다.
	if (DamageInfo.DamageIncreaseRateOverride != -1)
		IncreaseRate += DamageInfo.DamageIncreaseRateOverride;

	float DecreaseRate = GetDamageDecreaseRate();			// 감소값이므로 1.f - @ 로 되어야 함 -> ( * 연산 )

	if (IsVictimPlayer && DamageCauser)
	{
		// 공격자와 실제 가격 Actor가 같이 않으면 RangeAttack으로 판별 ( Projectile, DamageAreaActor 등등 )
		const bool IsRangeAttack = Attacker != DamageCauser ? true : false;
		const float RangeDecreaseRate = IsRangeAttack ? GetRangeDamageDecreaseRate() : GetMeleeDamageDecreaseRate();
		DecreaseRate *= RangeDecreaseRate;
	}
	// Increase / Decrease 배율 조정

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

	// 1. Player(Victim) vs Player(Attacker)	- 1 대 1 
	//if (IsVictimPlayer && IsEnemyPlayer)
	//{
	//	ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	const float PVPDamageScale = (GameMode != nullptr) ? GameMode->GetPVPDamageScaleByGameModeType() : CombatStatEval::GetDefaultPVPDamageScale();

	//	// PVP 데미지 밸런스 조정
	//	// 1. 30% ~ 300% 
	//	DamageRate = FMath::Clamp(DamageRate, 0.3f, 3.f);

	//	// 2. PVPScale 데미지 적용
	//	DamageRate *= PVPDamageScale;
	//}

	// 2. Mob(Victim) vs Player(Attacker) - 플레이어가 몹을 공격
	//else if (IsEnemyPlayer)
	//{
	//	// NPC 데미지 한계 조절 4% ~ 500%
	//	DamageRate = FMath::Clamp(DamageRate, 0.04f, 5.f);
	//}
	// 3. 나머지경우 - Mob(Victim) vs Mob(Attacker) / Player(Victim) vs Mob(Attacker)
	// 일반적인 경우로 스테이지 모드에서 몹한테 맞았을때
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
	return GetMesh(); // 의미상 GetMesh 를 직접 쓰기보다는 GetBaseMesh 를 사용할 것을 권장.
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

	//	// 본 위치
	//	FVector vBoneLocation = bUseSocketName ? GetMesh()->GetSocketLocation(BoneName) : GetMesh()->GetBoneLocation(BoneName);

	//	// 타겟이 상단이나 하단에 있을 경우를 대비해서 pitch 만 슬쩍 맞춰준다.
	//	// 액터위치 기준으로 하던걸 본위치 기반으로 바꿈.
	//	FRotator DirectRotationToTarget = GetDirectionToTargetCharacter(vBoneLocation).Rotation();

	//	// 피치 보정은 본로테이션 무시할때만.. 하고싶지만 기존 데이터세팅때문에 그냥간다.
	//	// 무조건 케릭터 높이 추적하는거임.
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
	//else // 가장 가까운 player character 를 타겟으로. (AIBlueprintSearchPlayer)
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

		//바닥 찾기

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
	//// 가장 가까운 player character 를 타겟으로. (AIBlueprintSearchPlayer)
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
	//// 가장 가까운 player character 를 타겟으로. (AIBlueprintSearchPlayer)
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
	//	if (TargetCharacter->IsPlayerControlled())	// [Mob -> Player 타겟] , [Player -> TargetActor & TargetObject 타겟]
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

	//// 초당 이동시킬 벡터사이즈
	//float fAdjustAmountPerSec = 200.f;

	//// 겹치면 밀어내보자(this를).
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	ABladeIICharacter* pChar = Cast<ABladeIICharacter>(*Iterator);

	//	if (pChar && pChar != this)
	//	{
	//		// 서로 블록할때만 처리
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

	//				// sweep으로 뭔가에 막혔으면 반대방향으로좀 밀어줌.
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
	//	// 이후 state 를 받아서 AnimBP 에서 처리하며 사용하는 AnimMontage 에셋 참조는 GetPCSESAnimTableObject 를 통해서. 
	//	// -> 그쪽은 제거되었고 여기도 다른 추가 처리들이 의미가 없어지면 제거될 수 있음.
	//	CachedStageEventDirector = InDirectorActor;
	//	StageEventShowState = InNewState;
	//}
	//else
	//{
	//	if (InNewState == EStageEventShowState::ESES_None)
	//	{
	//		// InDirectorActor 가 NULL 인 경우라도 None 으로 세팅하는 경우라면 받아들임. StageEventDirector 쪽으로의 별다른 Notify 없이 조용히 상태를 종료하는 용도.
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
	//	CachedStageEventDirector->NotifyStageEventShowOver(this); // 살짝 다른 네이밍 낄낄
	//	CachedStageEventDirector = NULL; // Notify 후 레퍼런스 제거.
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

// 캡슐크기 포함시 플레이어 본인껀 고려안함.
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

		// 타겟이 케릭터가아니면 캡슐계산 못해줌
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
	//		// 원통형으로 Radius를 조사
	//		const float DistSq = FVector::DistSquaredXY(Enemy->GetActorLocation(), GetActorLocation());
	//		if (DistSq <= RadiusSq)
	//		{
	//			if (!FMath::IsNearlyEqual(FanAngle, 180.0f)) // 체크 범위가 360도라면 // 내적이 (-1)180도라면
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
	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // 바로 직전 phase 변화를 일으킨 data 인덱스. 물론 phase 변화가 없었다면 -1
	if (CurrentPhaseDataIndex >= 0 && PhaseDataArray.Num() > CurrentPhaseDataIndex && PhaseDataArray[CurrentPhaseDataIndex].AttackDataArray.Num() > 0)
	{
		// Override AIAttackDataArray
		return PhaseDataArray[CurrentPhaseDataIndex].AttackDataArray;
	}

	return AIAttackDataArray; // 기본 phase 에서의 기본 값.
}

const TArray<int32>& ABladeIICharacter::GetInvalidAttackIndices()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetInvalidAttackIndices);
	int32 CurrentPhaseNum = GetCurrentPhaseNum();
	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // 바로 직전 phase 변화를 일으킨 data 인덱스. 물론 phase 변화가 없었다면 -1
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

		// 못정했으면 거리0셋팅하지말고 젤긴걸로
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
//	{ // TestDummyNPC 에서는 AIAttackIndex 직접 지정해서 사용.
//		return;
//	}
//#endif
//	int32 CurrentPhaseNum = GetCurrentPhaseNum();
//	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // 바로 직전 phase 변화를 일으킨 data 인덱스. 물론 phase 변화가 없었다면 -1
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
	bInQTEState = bInState; // 여기서는 별다른 건 없고 BladeIIPlayer 쪽에서

	//if (bInQTEState)
	//{
	//	// UIDoc 쪽에 상태 변수를 넣어주는데 ABladeIIPlayer 쪽에서도 하긴 하지만 Player 쪽의 bInQTEState 변수가 원하는 기간보다 좀 짧은 듯해서 여기서도.
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
	// bAbleToMove 가 false 인 경우 공격 입력을 못하긴 하지만 애니메이션 상태 핸들링 도중 이동만 막고자 쓰는 경우가 있어서 여기서 검사 안 함.
	if (bMovementPreventByStageEvent)
	{ // 기타 공격 상태로 진입해서는 안될 조건들을 여기서 필터링 하도록..
		bSignalAttack = false;
	}
	else
	{
		bSignalAttack = bInSignal; // AnimBlueprint 에서 이걸 픽업해서 공격 모션 state 로 간 다음 리셋할 것.
	}
}

void ABladeIICharacter::BeginPlay()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_BeginPlay);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::BeginPlay"));

	Super::BeginPlay();
//
//	InitializeCombatStats(); // 여기서 한 번 하는데 NPCClassInfo 를 통해 필요한 값이 들어올 수 있으므로 그쪽서 한번 더 불러줌. -> 그렇지만 AB2MonsterSpawnPool::SpawnWave 에서 bDeferConstruction 을 세팅하므로 NPCClassInfo 이후에 여기로 오게 될 것.
//
//	// It could be already created and try create again, if outer level is loaded twice by some mishandling.
//	DestroyUIManager();
//	CreateUIManager();
//
//	CharacterBeginPlayClass<ABladeIICharacter*>::GetInstance().Signal(this);
//
//	if (ShadowMaterial)
//	{
//		SetupDecalCompForCenterShadowCommon(CenterShadow, GetMesh());  // SetupControlledMatineePuppet 에서도 사용하는 공통 셋업 부분.. 이었는데 결국 여기서만 사용
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
//	// 메모리 절약 용. 텍스쳐 스트리밍 되는 경우 다른 경로를 통해 처리될 것.
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
//	SetupLODInfoAsNPC(); // LOD 들어간 일부 NPC 용으로..
//
//	// 다이나믹 머터리얼 미리 만들어놈
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
	//// PC 의 경우에는 Final**Scale 대신 아이템에 따른 보정이 적용될 것.

	//// Need to be change to use formula, item, sub PC, etc...
	//TotalAttack = CombatStatEval::GetMobBaseAttack(CharacterLevel);
	//TotalDefense = CombatStatEval::GetMobBaseDefense(CharacterLevel);

	//AttackLevel = CombatStatEval::GetADLevel(TotalAttack, CombatStatEval::GetDefaultCombatCoefficient());
	//DefenseLevel = CombatStatEval::GetADLevel(TotalDefense / 3.f, CombatStatEval::GetDefaultCombatCoefficient());

	//// Need to be change to use formula, skill/item %, etc...
	//const float BaseHealth = FinalHPScale * CombatStatEval::GetMobBaseHealth(CharacterLevel);
	//MaxHealth = BaseHealth;

	//// 난이도 조절용 - 데미지 99999 도달 방지를 위한 hp / Damage 비례 상수
	//auto* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GameMode && IsMob())
	//{
	//	MaxHealth *= B2GameMode->GetPVEProportionalConstant();
	//	if (IsBossMob())
	//		MaxHealth *= B2GameMode->GetModifiedBossmobMaxHealth();

	//	/// 타수로 몹 죽일경우에 강제로 hp 고정.
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

	//DestroyUIManager(); // 플레이어한테 맞아 죽지 않으면 이 루트를 탄다.

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

	//// 죽을떄 하긴하는데. 안죽고 제거할지도 모르니.
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
// 			CachedStateDamageStates[StateIndex].LastStartedTime = (float)FPlatformTime::Seconds(); // 서로 비교해서 UI 에 뿌리는 우선순위 정하기 용.
// 		}
// 		CachedStateDamageStates[StateIndex].RemainingTime = RemainingTime;
// 
// 		StateDamageBuffUpdateUIdoc();
// 	}
// }

// void ABladeIICharacter::SetCachedInvincibleBuffState(bool bIsOn, float RemainingTime)
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetCachedInvincibleBuffState);
// 	CachedInvincibleBuffState.bIsOn = bIsOn; // On 혹은 Off 시점에만 사용해야 함.
// 	if (bIsOn)
// 	{
// 		CachedInvincibleBuffState.LastStartedTime = (float)FPlatformTime::Seconds(); // 서로 비교해서 UI 에 뿌리는 우선순위 정하기 용.
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
	//{ // 0 이나 음수는 이걸 끄는 걸로.
	//	GetWorldTimerManager().ClearTimer(SetRegenerateHPHandle);
	//}
}

void ABladeIICharacter::SetCanBeDamagedForB2(bool bNewCanBeDamaged)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetCanBeDamagedForB2);
	//// false면 카운팅 올리고 true면 카운팅 내림
	//!bNewCanBeDamaged ? nCanBeDamagedFalseCount++ : nCanBeDamagedFalseCount--;

	//UE_LOG(LogBladeII, Log, TEXT("nCanBeDamagedFalseCount = %d"), nCanBeDamagedFalseCount);

	//// 카운팅 0이하면 데미지 받음.
	//bCanBeDamaged = nCanBeDamagedFalseCount <= 0;
}

void ABladeIICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 버프 아이콘 업데이트.. 꼭 틱에서 해야만하나
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
	{ // PC 가 이것에 영향받지 않도록 NPCClassEnum 체크
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
//		// 모든 캐릭터 spawn 에 쓰이는 베이스 블루프린트에는 기본값 리소스들이 박혀 있는데 이게 무시하지 못할 정도로 클 수 있다. 쿠킹 세이브 시 제거.
//		if (IsSavingForCookingOnPreSave(TargetPlatform) && GetMesh())
//		{
//			GetMesh()->SetSkeletalMesh(nullptr);
//			GetMesh()->SetAnimInstanceClass(nullptr); // 이게 클 거다.
//
//			UE_LOG(LogBladeII, Log, TEXT("NULL out resouce properties of %s"), *this->GetPathName());
//		}
//	}
//#endif
}

void ABladeIICharacter::RegenerateHPByTimer()
{ // 퍼센트 혹은 절대값 처리
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

	//// CPU data 를 제거할 수 있는 적절한 타이밍이 있으니 아무곳에서나 남발하면 안됨.
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
	// SpawnPool 세팅을 통해 오는 이 캐릭터의 스테이지 구성과 관련한 정보들.
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
	//	// 별도로 Mesh 를 준비해서 넘겨줄 수도 있다.
	//	USkeletalMesh* MainSKMesh = OptionalSKMeshToSet ? OptionalSKMeshToSet : MyBaseMeshComp->SkeletalMesh;

	//	PuppetBaseMeshComp->SetSkeletalMesh(MainSKMesh); // 연출에서는 다른 고퀄리티 메쉬로 바꿔칠 수도 있음. 단, Skeleton 이 다른 SkeletalMesh 라면 바꿔쳐봐야 애니메이션 플레이가 안 될 것.

	//	PuppetBaseMeshComp->SetWorldScale3D(MyBaseMeshComp->GetComponentScale());

	//	UB2DamageEffectInfo* DmgFxInfo = GetDamageEffectInfo();

	//	// SkeletalMeshComponent 에 입힌 material override 도 적용.
	//	if (!OptionalSKMeshToSet) // 별도 Mesh 를 제공하는 경우는 컴포넌트에서 머티리얼을 가져오지 않는다.
	//	{
	//		// 전투 중 머티리얼에 적용된 파라미터를 리셋.
	//		ResetFlash();
	//		ResetEmissive();

	//		for (int32 MI = 0; MI < MyBaseMeshComp->GetNumMaterials(); ++MI)
	//		{
	//			// 전투 효과로 인해 material override 가 된 상황이라면 원래 material 을 입혀주는 게 적절할 것이다.
	//			// 카메라워크만 사용하는 연출이면 전투 상황이 그대로 가는 게 적절하겠지만 이건 애니메이션까지 연출된 장면에서 사용할 것이므로.
	//			UMaterialInterface* MaterialToSet = GetOriginalMaterial(MI);
	//			PuppetBaseMeshComp->SetMaterial(MI, MaterialToSet);
	//		}
	//	}

	//	// 기타 attach 된 것들이 있다면 연출용에도 마찬가지로 붙임.

	//	// StaticMesh 로 붙인 것들
	//	TArray<UActorComponent*> AttachedStaticMeshComps = this->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	//	for (int32 SMI = 0; SMI < AttachedStaticMeshComps.Num(); ++SMI)
	//	{
	//		UStaticMeshComponent* ThisComp = Cast<UStaticMeshComponent>(AttachedStaticMeshComps[SMI]);
	//		// 일단, SkeletalMeshComponent 에 붙은 것만 취급해 본다.. AttachTo 할 때 ThisComp->GetAttachParent() 해서 먹여줄 순 없는 노릇이라..
	//		if (ThisComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForStaticMesh(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach 된 애들마다 하나씩 생성해서 같은 StaticMesh 입히고 같은 socket 에 붙여줌.
	//			UStaticMeshComponent* PuppetAttachComp = NewObject<UStaticMeshComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetStaticMesh(ThisComp->GetStaticMesh());
	//				PuppetAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh 를 SkeletalMeshActor 에 붙이니 당연히 Movable 로
	//				// AttachParent 가 SkeletalMeshComponent 가 아닌 경우가 생긴다면 수정..
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetMaterial(0, ThisComp->GetMaterial(0)); // Attach Component 의 Material 이 SpawnPool 을 통해 overriding 될 수 있으므로 이것도 세팅. 단 0번만
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 종료 후 제거는 안하므로 혹시 모르니 collision 은 끄도록
	//				PuppetAttachComp->RegisterComponent();
	//			}
	//		}
	//	}
	//	// SkeletalMesh 로 붙인 것들
	//	TArray<UActorComponent*> AttachedSKComps = this->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	//	for (int32 SKI = 0; SKI < AttachedSKComps.Num(); ++SKI)
	//	{
	//		USkeletalMeshComponent* ThisComp = Cast<USkeletalMeshComponent>(AttachedSKComps[SKI]);
	//		// 일단, SkeletalMeshComponent 에 붙은 것만 취급해 본다.. AttachTo 할 때 ThisComp->GetAttachParent() 해서 먹여줄 순 없는 노릇이라..
	//		// 또한 메인 SkeletalMeshComponent 는 바로 위에서 처리하므로 물론 통과~
	//		if (ThisComp && ThisComp != MyBaseMeshComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForSkeletalMesh(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach 된 애들마다 하나씩 생성해서 같은 SkeletalMesh 입히고 같은 socket 에 붙여줌.
	//			USkeletalMeshComponent* PuppetAttachComp = NewObject<USkeletalMeshComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetSkeletalMesh(ThisComp->SkeletalMesh);
	//				// AttachParent 가 SkeletalMeshComponent 가 아닌 경우가 생긴다면 수정..
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetMaterial(0, ThisComp->GetMaterial(0)); // Attach Component 의 Material 이 SpawnPool 을 통해 overriding 될 수 있으므로 이것도 세팅. 단 0번만
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 종료 후 제거는 안하므로 혹시 모르니 collision 은 끄도록
	//				PuppetAttachComp->RegisterComponent();
	//			}
	//		}
	//	}
	//	// ParticleSystem
	//	// ParticleSystemComponent 들은 주의할 것이 있는데 InMatineePuppet 이 Hidden 이거나 뭔가 준비가 덜 된듯한 상태에서는 이렇게 생성을 시켜봐야 Deactivate 가 되는 것 같아 보인다는 것.
	//	// 여기서 아무리 Activate 를 해도 소용없다. StageEventDirector 쪽에서 Visibility 제어하면서 따로 처리해 줌.
	//	// 추가로 여기서는 BladeIIPlayer 의 FloorRingPS 도 생성되어 붙여질 건데 그것도 StageEventDirector 쪽에서 처리를..
	//	TArray<UActorComponent*> AttachedPSComps = this->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//	for (int32 PSI = 0; PSI < AttachedPSComps.Num(); ++PSI)
	//	{
	//		UParticleSystemComponent* ThisComp = Cast<UParticleSystemComponent>(AttachedPSComps[PSI]);
	//		// 마찬가지로, SkeletalMeshComponent 에 붙은 것만 취급
	//		if (ThisComp
	//			// [혁중 17.02.13] 조건추가 루핑되는것들만 붙임 : 일시적인 공격모션에서 AnimNotify 로 붙는 애들은 스킵하고자 하는 의미. 기본적으로 여기서 붙일 필요가 있는 이펙트들은 지속적으로 플레이되는 (Looping 되는) 애들이다.
	//			&& IsStaticallyVisiblePSC(ThisComp)
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForParticleSystem(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach 된 애들마다 하나씩 생성해서 같은 PS 입히고 같은 socket 에 붙여줌. 
	//			UParticleSystemComponent* PuppetAttachComp = NewObject<UParticleSystemComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetTemplate(ThisComp->Template);
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 종료 후 제거는 안하므로 혹시 모르니 collision 은 끄도록
	//				PuppetAttachComp->RegisterComponent();
	//				PuppetAttachComp->Activate(true);
	//			}
	//		}
	//	}

	//	// Decal 도 필요함.
	//	TArray<UActorComponent*> AttachedDecalComps = this->GetComponentsByClass(UDecalComponent::StaticClass());
	//	for (int32 DCI = 0; DCI < AttachedDecalComps.Num(); ++DCI)
	//	{
	//		UDecalComponent* ThisComp = Cast<UDecalComponent>(AttachedDecalComps[DCI]);
	//		// 마찬가지로, SkeletalMeshComponent 에 붙은 것만 취급
	//		if (ThisComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForDecal(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach 된 애들마다 하나씩 생성해서 같은 material 입히고 같은 socket 에 붙여줌.
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

	//	// PointLight 도 붙여보는데.. 이건 모바일 dynamic point light 개수 제한 때문에 좀 민감함. 따라서 이걸 안 하는 인자 제공
	//	if (bSetupLightComponent)
	//	{
	//		TArray<UActorComponent*> AttachedLightComps = this->GetComponentsByClass(UPointLightComponent::StaticClass());
	//		for (int32 LCI = 0; LCI < AttachedLightComps.Num(); ++LCI)
	//		{
	//			UPointLightComponent* ThisComp = Cast<UPointLightComponent>(AttachedLightComps[LCI]);
	//			// 이건 Capsule 에 붙은 것도..
	//			if (ThisComp && ThisComp->IsVisible() // && ThisComp->bAffectsWorld // Light 면 Visible 이 아니라 AffectsWorld 를 봐야 맞는데.. 이게 필요한 상황임에도 AffectsWorld 가 꺼져 있을 가능성이 있을 것 같아서 여기서 체크 안하고 bSetupLightComponent 인자를 적절히 넘겨주는 걸로.
	//				&& (ThisComp->GetAttachParent() == MyBaseMeshComp || ThisComp->GetAttachParent() == RootComponent)
	//				&& !IsCompInEventSceneBlacklist(ThisComp)
	//				&& DoesMatineePuppetSetupForLight(InMatineePuppet, ThisComp) == false)
	//			{
	//				// Attach 된 애들마다 하나씩 생성해서 값설정 하고 같은 socket 에 붙여줌.
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
	//	// 이전에 셋업했는데 재사용하는 경우가 있을 수 있으니 On/Off 처리도 다시.
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
	//	// SetupControlledMatineePuppet 에서 넣었던 Material override 들은 모두 제거한다. 그냥 두면 임시 생성 오브젝트들이 맵에 저장되어 (근데 PIE 맵 따로 생성되지 않나..?) 원치 않는 레퍼런스가 발생할 수 있음.
	//	for (int32 MI = 0; MI < MyBaseMeshComp->GetNumMaterials(); ++MI)
	//	{
	//		PuppetBaseMeshComp->SetMaterial(MI, NULL);
	//	}
	//	// 기타 SetupControlledMatineePuppet 에서 생성하는 Component 들이 있지만 RF_Transient 를 주므로 저장 안될 것. 이미 걔들이 저장되고 있었으면 뭔가 이상한 일이 일어났겠지..
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
	checkSlow(InCheckComp && InCheckComp->GetOwner() == this); // 나에게 속한 Component 만 여기 체크 대상임.
	return EventPuppetSetupBlacklist.Contains(InCheckComp);
}

// 땡기기 fForceStrength -값일때 땡긴다. +값이면 밀듯
void ABladeIICharacter::PullEnemyByTick(float fDeltaTime, float fForceStrength, float fRadius, FVector LocationOffset)
{
	//// bFixInAir플래그로 인해 무브먼트껐으면 패스
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

// 아래 DoesMatineePuppetSetupFor** 를 템플릿으로도 못 해서 쪼금이나마 공통적인 부분 추려봄.
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
	//// InCompToCheck 는 이 BladeIICharacter 에 붙어있는 것임.

	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedStaticComps = InMatineePuppet->GetComponentsByClass(UStaticMeshComponent::StaticClass());

	//for (int32 SCI = 0; SCI < MPAttachedStaticComps.Num(); ++SCI)
	//{
	//	UStaticMeshComponent* ThisMPComp = Cast<UStaticMeshComponent>(MPAttachedStaticComps[SCI]);

	//	// ThisMPComp 가 InCompToCheck 와 비슷한지 검사한다. 같은 리소스를 사용하고 붙어있는 소켓 이름이 일치하는지.
	//	if (ThisMPComp && ThisMPComp->GetStaticMesh() == InCompToCheck->GetStaticMesh() && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		// 붙어있는 부모 자체는 물론 다르지만 같은 종류에 붙어있는지도 검사.
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
	// InCompToCheck 는 이 BladeIICharacter 에 붙어있는 것임.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedSKComps = InMatineePuppet->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	//for (int32 SCI = 0; SCI < MPAttachedSKComps.Num(); ++SCI)
	//{
	//	USkeletalMeshComponent* ThisMPComp = Cast<USkeletalMeshComponent>(MPAttachedSKComps[SCI]);

	//	// ThisMPComp 가 InCompToCheck 와 비슷한지 검사한다. 같은 리소스를 사용하고 붙어있는 소켓 이름이 일치하는지.
	//	if (ThisMPComp && ThisMPComp->SkeletalMesh == InCompToCheck->SkeletalMesh && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // 붙어있는 부모 자체는 물론 다르지만 같은 종류에 붙어있는지도 검사.
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
	//// InCompToCheck 는 이 BladeIICharacter 에 붙어있는 것임.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedPSComps = InMatineePuppet->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//for (int32 PCI = 0; PCI < MPAttachedPSComps.Num(); ++PCI)
	//{
	//	UParticleSystemComponent* ThisMPComp = Cast<UParticleSystemComponent>(MPAttachedPSComps[PCI]);

	//	// ThisMPComp 가 InCompToCheck 와 비슷한지 검사한다. 같은 리소스를 사용하고 붙어있는 소켓 이름이 일치하는지.
	//	if (ThisMPComp && ThisMPComp->Template == InCompToCheck->Template && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // 붙어있는 부모 자체는 물론 다르지만 같은 종류에 붙어있는지도 검사.
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
	// InCompToCheck 는 이 BladeIICharacter 에 붙어있는 것임.
	if (InMatineePuppet == NULL || InCompToCheck == NULL)
	{
		return false;
	}

	//TArray<UActorComponent*> MPAttachedDecalComps = InMatineePuppet->GetComponentsByClass(UDecalComponent::StaticClass());
	//for (int32 DCI = 0; DCI < MPAttachedDecalComps.Num(); ++DCI)
	//{
	//	UDecalComponent* ThisMPComp = Cast<UDecalComponent>(MPAttachedDecalComps[DCI]);

	//	// ThisMPComp 가 InCompToCheck 와 비슷한지 검사한다. 같은 리소스를 사용하고 붙어있는 소켓 이름이 일치하는지.
	//	if (ThisMPComp && ThisMPComp->GetDecalMaterial() == InCompToCheck->GetDecalMaterial() && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // 붙어있는 부모 자체는 물론 다르지만 같은 종류에 붙어있는지도 검사.
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
	// InCompToCheck 는 이 BladeIICharacter 에 붙어있는 것임.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedLightComps = InMatineePuppet->GetComponentsByClass(UPointLightComponent::StaticClass());
	//for (int32 LCI = 0; LCI < MPAttachedLightComps.Num(); ++LCI)
	//{
	//	UPointLightComponent* ThisMPComp = Cast<UPointLightComponent>(MPAttachedLightComps[LCI]);

	//	// ThisMPComp 가 InCompToCheck 와 비슷한지 검사한다. 설정값이랑 소켓 이름이 일치하는지.
	//	// Light 의 경우는 검사 조건 몇개가 빠질 필요가 좀 있다.. 사실 실제 동작하는 light 를 붙이고 다니는 애들이 여럿 있어서도 안되고.
	//	if (ThisMPComp && //ThisMPComp->RelativeLocation == InCompToCheck->RelativeLocation && 
	//		ThisMPComp->Intensity == InCompToCheck->Intensity &&
	//		ThisMPComp->LightColor == InCompToCheck->LightColor &&
	//		ThisMPComp->AttenuationRadius == InCompToCheck->AttenuationRadius
	//		//ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		//AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // 붙어있는 부모 자체는 물론 다르지만 같은 종류에 붙어있는지도 검사.
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

	//// this 정보를 넘겨주어 생성만 하고 이후로는 직접적인 레퍼런스는 필요없음.
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM && B2GM->GetUIManager_InGameCombat())
	//{
	//	B2GM->GetUIManager_InGameCombat()->CreateHUDForNewChar(this);
	//}

	//if (CachedBirthplaceInfo.bSpawnedAsBossMob)
	//{
	//	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();

	//	//BeginPlay시점에 DestroyUIManager()로 인해 자신이 보스임에도 불구하고 UI를 삭제한다.
	//	//UI 다시 세팅.
	//	if (DocBattle && AmIInterestedBossMob())
	//	{
	//		DocBattle->SetbCurrentlyBossAppeared(true);
	//	}
	//}
}

void ABladeIICharacter::HideFloatingHPBar(bool bInHide)
{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_HideFloatingHPBar);
//	// DestroyUIManager 가 Death 시점에서 delay 되면서 죽은 몹의 HPBar 를 바로 숨길 필요가 생김. 연출 등의 이유로 숨기려던 게 아님.. 넓게 보면 그게 다 연출이긴 하네..
//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	if (B2GM && B2GM->GetUIManager_InGameCombat())
//	{
//		B2GM->GetUIManager_InGameCombat()->HideFloatingHPBarForChar(this, bInHide); // 이건 플레이어 아닌 NPC 에만 먹힐 것.
//	}
}

void ABladeIICharacter::DestroyUIManager()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DestroyUIManager);
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM && B2GM->GetUIManager_InGameCombat())
	//{
	//	B2GM->GetUIManager_InGameCombat()->DestroyHUDForDyingChar(this); // 이건 플레이어 아닌 NPC 에만 먹힐 것.
	//}

	//if (CachedBirthplaceInfo.bSpawnedAsBossMob)
	//{
	//	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//	// 혹시라도 안 죽은 채로 destroy 될 수가 있으므로 세팅을 해 주긴 하는데.. 두 보스가 연달아 나오는 경우도 있으므로 지금 DocBattle 에 세팅된 게 자신과 마찬가지인지 확인은 해야 한다.
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
	// bSpawnedAsBossMob 이 이 시점에 세팅이 안되어 있을 거라 별로 할 수 있는 게 없군. 그건 SpawnPool 에서..
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
	//	B2AICon->ToNextPhase(); // 안에서 다시 PhaseDataArray 를 찾아서 사용할 것.

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
	//	// 잘보면 ESkillOption::ESO_DecReceiveDamage 값 이용한거임 검투사 4번스킬은 이걸쓰는듯
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
	//	fBuffAmount = Amount + GetBonusBuffHealHPByHit() * 100.f;	// HealingHPByPercent 에서 퍼센트 값을 받으므로 이건 100 곱해놓음.
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

// InAmount가 0이면 스킬에서 값얻어옴
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

	// 몬스터의 Immune NotifyState은 일단 다 막음 - ForceStateAttack도 막음 ( 기획 요청 )
	if (bImmune && IsPlayerCharacter() == false)
		return true;

	// - 우선 확인 조건 : DamageInfo에 상태이상 강제적용이 있다면 반격 / 결속스킬 / 이뮨버프상태 제외 모두 상태이상 적용
	if (IsForceStateAttack(DamageInfoPtr))
		return false;

	// - 일반조건
	// 1. Skill 사용중은 Immune
	// 2. 이뮨 버프 있을때
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

	//// 1. Local이 맞은 데미지는 무조건 보여줌
	//if (IsPlayerControlled() && IsLocalCharacter())
	//	return true;

	//// 2. 내가 한 공격이면 데미지 보여줌
	//auto* LocalCharacter = GetLocalCharacter();
	//if (LocalCharacter && LocalCharacter == Attacker)
	//	return true;

	//// 3. 내가 탄 QTE Actor의 공격이면 데미지 보여줌
	//if (Attacker && Attacker->IsQTEActor(LocalCharacter))
	//	return true;

	return false;
}

// BP에서 쓰는곳이 있음.
bool ABladeIICharacter::IsStunned() const
{
	return IsBuffActive(EBuffType::Debuff_Stun);
}

// BP에서 쓰는곳이 있음.
bool ABladeIICharacter::IsFrozen() const
{
	return IsBuffActive(EBuffType::Debuff_Freezing);
}

//void ABladeIICharacter::TemporarySetMeshComponentUpdateFlag(TEnumAsByte<EMeshComponentUpdateFlag::Type> InNewFlag)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_TemporarySetMeshComponentUpdateFlag);
//	//// 연달아서 이걸 사용하는 일이 없도록. 아님 최소한 그런 일이 있더라도 어차피 잠깐 세팅하려는 값은 똑같은 거라 (AlwaysTickPoseAndRefreshBones 겠지 머) 무시해도 아무 탈 없는.
//	//checkSlow(!bTemporarilySetMeshCompUpdateFlag || (GetBaseMesh() && GetBaseMesh()->MeshComponentUpdateFlag == InNewFlag));
//
//	//if (GetBaseMesh() && !bTemporarilySetMeshCompUpdateFlag) // 이전에 사용한 거 Restore 하기 전에 중복 진입 안 되도록. 걍 무시됨.
//	//{// 현재값을 백업해 놓고 잠시 다른 값을 사용중이라고 표시해 놓음.
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

	// 원래꺼 캐싱
	if (OverridenMatTypeList.Num() == 0)
	{
		for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
		{
			// Cache
			CachedMaterials.Add(GetMesh()->GetMaterial(i));
		}
	}

	//머터리얼 적용
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
	//	//복구
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

	//// 남은것중에 마지막껄로 적용
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
	//// NPC 일부가 LOD 작업이 됨에 따라 게임에서는 일괄적인 LODInfo 를 준다.
	//// 리소스에 적당히 셋업하려니 캡쳐 영상에서 쓸 거 같고..
	//// 여긴 플레이어 캐릭터처럼 옵션이나 게임모드 등의 상황을 꼭 탈 필요는 없을 듯. 일단 적절히 인게임 플레이 정도 뷰에서 LOD1 이 나오게.

	//// 여기서 LODInfo 를 세팅하는 메쉬는 실제 에디터에 등록된 리소스이다. PC 의 경우처럼 런타임 merge 된 메쉬가 아님.
	//// 이렇게 세팅하고 나면 같은 메쉬를 공유하는 다른 액터(e.g. SkeletalMeshActor)가 있다면 영향을 받게 될 것이다.
	//// BladeIIPlayer.cpp 쪽에 이와 중복된 코드가 있지만 유틸로 빼 놓을 성격은 아님.

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
	//	// 사실상 LOD1 의 ScreenSize 만 설정함
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
	//// 어태치된 이펙트 제거(루핑, 임모탈은 빼구)
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
	// 잠깐 안 보이게 해 놨다가 AnimBP 세팅한 후에 visibility 를 다시 켠다. 멀티쓰레드 anim 관련 모바일 환경에서의 문제가 좀 있어서 이런 짓을 함.
	//-> 네이티브 클래스 기반 spawn 이후 이건 없앰. 사실상 SetAnimInstanceClass

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
	//// 현재 UI 에 표시할 보스몹인지 여부. 보스몹이 연이어 나올 때 앞에것이 destroy 되기 이전 뒤에 것이 spawn 하는 타이밍 문제가 있어서 이렇게 구분할 필요가 생김
	//UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocBattle)
	//{ // 연이어 동일한 Class 와 Variation 의 Boss 몹이 등장하면 물론 이걸로도 소용이 없을 것임.
	//	return CachedBirthplaceInfo.bSpawnedAsBossMob && DocBattle->GetBossMobClass() == NPCClassEnum && DocBattle->GetBossMobClassVariation() == NPCClassVariation;
	//}
	return CachedBirthplaceInfo.bSpawnedAsBossMob;
}

void ABladeIICharacter::SetupDecalCompForCenterShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetupDecalCompForCenterShadowCommon);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetupDecalCompForCenterShadowCommon"));

	// 원래 BladeIICharacter 자신이라면 InDecalCompToUse 는 CenterShadow 일 것임. SetupControlledMatineePuppet 기능을 위해 매개변수로 받는다. 
	// 여기서 컴포넌트 register 까지 하지는 않는다.
	//if (InDecalCompToUse && AttachParent)
	//{
	//	//InDecalCompToUse->SetHiddenInGame(false);
	//	InDecalCompToUse->SetRelativeScale3D(FVector(200.f + ShadowMaxDistance, ShadowSizeCenter, ShadowSizeCenter));

	//	InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	//	InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//}
}

UB2NPCSingleClassInfo* ABladeIICharacter::GetMyNPCClassInfo()
{ // 플레이어 캐릭터, 혹은 NPCClassEnum 이 세팅되지 않은 경우 소용 없을 것.
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