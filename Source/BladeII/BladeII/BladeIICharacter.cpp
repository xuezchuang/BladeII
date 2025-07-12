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
//	//// ���� Info �¾� ����ؼ� PSC ���� �� attach �ϴ� �⺻ ����.
//	//if (InCompOwner && InParentComp)
//	//{
//	//	// InParentComp �� InCompOwner �� ���Ե� �ɷ� ������..
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
//{ // �̰� �������� ���ƾ� �� ������ �����ϸ� PostEdit ���� �����Ϸ��� ��.
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
	else if (bSpawningNoDamageGuard) // ��ü�� SpawnPool ���� Spawn ��Ű�鼭 ����ϱ� ��.
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
//	bNeedSpawnCheck = false; // �ʱ� spawn state �� ����ϴ� ��� true �� ���ְ�, AI �� ���� false �� ���µǸ� �����̱� ����.
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
////	{ // DLC Front ��� ���ҽ��ε� �ִ��� ����
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
////			BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("FloatingHPBarWidgetClass"))); // MinimalDLCFrontMode �� ���� ������� BaseBP �� ���� �������� �ʱ� ���� �̸� �߰�
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
////	TemporaryMeshCompUpdateFlagBackUp = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones; // �̰� �Ƹ��� ���� ������ ���� ��. ���� �̰� �״�� ����Ǵ� �� �ƴ�.
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
////	bSpawningNoDamageGuard = false; // ��� ��쿡 ó������ true ���ٰ� spawn ó�� �� false �� �δ� �� �ƴ϶� �Ϻ��� ��츸 ��ð� true �� ���.
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
	if (!bMovementPreventByStageEvent) // bMovementPreventByStageEvent �� �������� �뵵
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
	bAbleToMove = true; // �����δ� �̰� ������ �־�� ��.
}

void ABladeIICharacter::PreventMovementByStageEvent()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_PreventMovementByStageEvent);
	bMovementPreventByStageEvent = true;
	bAbleToMove = false; // �����δ� �̰� ������ �־�� ��.
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
	// ������ ���� % �� ǥ�õǴ� ������ ����. 0.01 �����ϸ��� ���⼭ ��.
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

	//	// ü��ȸ��UI(������ǥ�� �̺�Ʈ�� ���Ʈ -�����ָ� ȸ��ǥ���ε�)
	//	// �÷��̾ ����

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
	//		SetRegenerateHPRate(0.f); // ����.
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
	//	// AnimNotifyState_B2MobStateUndead���� m_bIsUndead = true�� ����(���߿� �״� ����)
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
	//else //�̰Ŵ� �̻� �����̱�� �ϳ� ���� ó�������� �߰�
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
//	// ���� �� �� ������ �༭ Ʈ��ŷ.
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
	//bIsDying = true; // Animation Blueprint �� ������ ��ȣ. �������� �����̳� ���� Ʈ���� ���������� ó���� ����Ƽ�꼭 ������ ������ ���� ����� �ߴ��ϰ� dead ����� ���ϴ� �� �̰� ���� ��.
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
	// �� �������� �ٷ� DestroyUIManager �� ���� �ʴ´�. �ֳ��ϸ� DamageNum ���� �ֵ��� ���� �Ⱓ�� �����־�� �ϱ� ����. 
	// ������ 1�� �Ŀ� �Ҹ����� Ÿ�̸Ӹ� ��ġ�ϴµ� �� ���� �ı��� �Ǹ� Destroyed ���� ó���� �� �Ű�, 1�ʰ� ª�ٸ�.. ��.. �װ� ��Ⱑ ������ ����.
	//bool ResurrectionGameModeType = B2GM && B2GM->GetUIManager_InGameCombat() && B2GM->GetUIManager_InGameCombat()->CanResurrectionGameModeType();

	//if (ResurrectionGameModeType == false)
	//{
	//	GetWorldTimerManager().SetTimer(DelayedDestroyUIManagerTH, this, &ABladeIICharacter::DelayedDestroyUIManagerCB, 1.0f, false);
	//	HideFloatingHPBar(true); // ��� HPBar �� �ٷ� �� ���̰�
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

	ConditionalReportMyDeathToBirthplace(); // ���� �������� �ϴ� ���̽��� �ִ�.. �Ѥ�

	//CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Signal(this);

	//for (auto Key : EventCancelCastAreaDamageSubscribeKeys)
	//	CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Unsubscribe(Key);

	// ������ �׾��ٰ� ���Ӹ�忡 �˸�(�ǴϽ� ���� ����뵵�� �ۼ�)
	// ReportMyDeathToBirthplace ���� ������ ������ �ߺ��Ǵ� ���� �ִµ� ����μ� ������ ���ݾ� �ٸ�.
	if (B2GM)
		//B2GM->NotifyCharacterDead(this, PawnInstigator, DamageCauser);

	TriggerKillMonster();

	//CharacterDeathClass<ABladeIICharacter*>::GetInstance().Signal(this);

	// ��������
	//BuffManager->ClearAllBuffs();
}

void ABladeIICharacter::DieForQTE()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DieForQTE);
	//// QTE Ÿ������ �ɸ� ���� ��� ó��. �Ϲ������� �̵��� OnDeath �� ������ �ʰ� �� �� �ʿ��� ó���� ���� ���⼭ �� �ش�.

	//AB2StageManager::GetCacheStageKeepEssentialData().AddQTECount(QTEStateNum);
	//DropItem();

	//ConditionalReportMyDeathToBirthplace(); // SpawnPool ���� ����Ű�
}

void ABladeIICharacter::ReportMyDeathToBirthplace()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_ReportMyDeathToBirthplace);
	// SpawnPool ���� ����� ���� ��� SpawnPool ������ ������ ������ ����Ʈ���� ���� ���� ó���� �ϵ��� ��. �Ϲ����� Dead ��Ʈ�� ������ �ʴ� ���� �̰� ó���� �ؾ� ���� ������ �ȴ�.
	// �Ϲ����� �������� ���࿡���� ���Ϳ� ���� ���� �������� �ǹ��� ��� �Ű�
	//if (CachedBirthplaceInfo.Birthplace)
	{
		CachedBirthplaceInfo.bOwnerDeathReported = true; // ���� ��� �Ű� ���θ� üũ�ϴµ� ���� �� �ִ�.

		if (CachedBirthplaceInfo.bIsSummoned)
		{
			// bIsSummoned �� ���� �̺�Ʈ�� �ٸ��� ���ļ� �ȵ� �͵� ���� ��.��
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

	//// �������� ���̴� �������� �Ǵܺ��� ����(�÷��̾� �������� ������)
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

	//// Animation Blueprint �� ���� Dead �ִϸ��̼��� �ϸ� ��ġ�� AnimNotify �� ���� ���� �ȴ�.
	//// �Ϲ������δ� ���� �󿡼��� �״� ó�� (�������� ���� ü���� 0 �Ʒ��� �������ų� ���� ��) ���� ���;� �Ѵ�. 
	//// SpawnPool �� ��ϵ� ���Ͷ�� ���ʿ����� �̹� ����Ű� �� ����
	//// ���� �𸮾� Object ���������� valid �ϳ� ���� �����鼭 �𸮾� Object ���� countdown �� �� ����.

	//if (IsPlayerControlled())
	//{
	//	return; // �ϴ� Mob ����
	//}

	//if (IsAttacking() && // DeadEnd �� �Դµ� Attack ���¶�� ���� Attack �ִϸ��̼ǿ� DeadEnd �� �ִٴ� ���� ��. ��, �ڻ����.. ���̽��� �ִ� �� ��������ε�..
	//	// ��, ���� �ǵ��� �ٸ��� �̷��� ������ ��찡 ���� �ʴ�.. �׸� ������� ������..
	//	!IsInQTEState() && !CachedBirthplaceInfo.bOwnerDeathReported)
	//{
	//	// �� ���� �״�� �����ϸ� �� ĳ���Ͱ� ���Ŵ� ������ ���� ���࿡ ������ �ִ� OnDeath ������ ó���� �����ǹǷ� Suicide �� ���� �����ش�.
	//	bCommittedSuicideAtDeadEnd = true; // AnimBP ���� ������� Dead Anim �ڵ鸵�� ���� ���� �߰� �÷��� ����. �ֳ��ϸ� Dead Anim �ڵ鸵�� �̹� �� ���̹Ƿ�.
	//	Suicide();
	//}

	//// ������ ��츦 ����ϴ� ������ Ȯ�λ�� �ڵ�. �� ���������� ��� �Ű� �ȵ� �˷����� ���� ������ �ִٸ� Ȯ���� �ʿ䰡 �ִ�.
	//if (CachedBirthplaceInfo.Birthplace && !CachedBirthplaceInfo.bOwnerDeathReported && !IsInQTEState()) // QTE ������ ������ ����Ű� �ϴµ�.. �̶� QTEState �� �����Ǵ� �� ���⵵ �ϰ�..
	//{
	//	ensureMsgf(false, TEXT("Detected dead report miss by unknown reason. Forcing dead report.. %s, Wave%d_%d"), *this->GetName(), CachedBirthplaceInfo.WaveNumber, CachedBirthplaceInfo.WaveObjIndex);
	//	ReportMyDeathToBirthplace();
	//}

	//// ���Ӹ�忡 �˸��ʿ䰡 ����
	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (pGM)
	//{
	//	//pGM->NotifyAnimMobDeadEnd(this);
	//}

	//// bInUseDeadRagDoll �� AnimBP ���� ���� ����.
	//if (bInUseDeadRagDoll)
	//{
	//	SetRagdollPhysics(InRemainLifeSpan, !bInUseDeadRagDoll); // �� �ȿ��� GC ī��Ʈ �ٿ��� �� ����.
	//	GetCharacterMovement()->StopMovementImmediately();
	//}
	//else
	//{
	//	WrappedSetLifeSpan(InRemainLifeSpan); // GC ī��Ʈ �ٿ�..
	//	PreventMovementByStageEvent(); // ������ ������� �����鼭 movment stop ���� �ϸ� ���� ��̾���.
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
		return NULL; // �ϴ� Mob ����
	}

	class USkeletalMeshComponent* SKMeshComp = GetMesh();
	UMaterialInstanceDynamic* CreatedDeadBodyMID = NULL;

	if (InDeadBodyMI) // ���� �� �ٲ�ġ�� ��Ƽ����. ��İ� Ÿ���� ȿ��..
	{
		// ���������� �ϳ��ۿ� ó���� �� �� �ۿ� ���µ� AnimBP ���� OutCreatedDeadBodyMID �ϳ��� �޾Ƽ� �Ķ���͸� ������ ���̱� ����.
		for (int32 MII = 0; MII < SKMeshComp->GetNumMaterials(); ++MII)
		{
			if (!CreatedDeadBodyMID)
			{ // MID �� 0�������� ����� �� ��.
				CreatedDeadBodyMID = SKMeshComp->CreateDynamicMaterialInstance(MII, InDeadBodyMI);
			}
			else
			{ // ������ ������ ���� �� �ϳ������� �� �Ȱ��� ����.
				SKMeshComp->SetMaterial(MII, CreatedDeadBodyMID);
			}
		}

		if (CreatedDeadBodyMID && InDeadBodyMIParamName != NAME_None)
		{
			// ��İ� Ÿ���� ȸ�Ѿ �λ��� �������ϴ� ȿ���� �����Ѵ�..
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
	// ���� �����󿡼��� ��� ó���� �����̰� �𸮾� ������Ʈ ���������� GC �������� �� ��Ȳ���� (��ʽĿ� 49����� ������) �� ����ϴ� ���� �ִٸ� �׸� �ص��� ������ ��ε�ĳ��Ʈ�� �Ѵ�.	
	// ABladeIICharacter::Destroyed �� ������ �� �а� Ŀ�� �����ѵ� �׷� ��� SpawnPoolWaveMobPhaseChangeClass �� ���� StageEventDirector ���� phase ��ȭ�� �ٽ� spawn �� ���� �ٲ�ġ�� �� �� ��. ���� ���� ������ �� �ٲٸ� ������ �ϴ�.
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
	//// �ǰ� ��ü�� ������ this == ROLE_Authority!!
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

	// ��¥ Local Player�̰ų�
	if (IsPlayer && IsLocal)
		return true;

	// LocalPlayer���� �������ϴ� �� (QTE) ���̸� 
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

	//if (bSpawningNoDamageGuard) // ��ü�� SpawnPool ���� Spawn ��Ű�鼭 ����ϱ� ��.
	//	return false;

	//if (!IsEnemy(DamageCauser))
	//	return false;

	//if (IsBuffActive(EBuffType::Buff_Hide) || IsBuffActive(EBuffType::Buff_Unbeatable))
	//	return false;

	// bApplyNPC
	//if (GetTeamNum() == INDEX_NONE && DamageType->bApplyToNPC)
	//	return true;

	// ���Ϳ��ִ� �÷��׵� üũ
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
//		// ��ü�� DamageType �� ���� ��Ȳ�� ������ ���� �÷��� �Ǵ� attack �ִϸ��̼ǿ��� damage notify �� ������ ��. InterpTrackAnimControl �� SkipDamageNotifiersOnly �� üũ.
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
//			// �÷��׿����� �޴µ����� ���밪���� ��ȯ
//			EDamageLogicType DamageLogicType = DamageInfo.DamageLogicType;
//			if (bForceReceivdAbsoluteDamage)
//				DamageLogicType = EDamageLogicType::ELT_Absolute;
//
//			// �÷��׿����� �޴µ����� ������ Ÿ���� ��ȯ
//			if (bForceReceivdAttackCountDamage)
//				DamageLogicType = EDamageLogicType::ELT_AttactCount;
//
//			ActualDamage = GetDamageByDamageLogic(ActualDamage, DamageLogicType, Attacker, DamageCauser, DamageInfo, bCriticalDamage);
//
//			// ������ ��Ƽ�� ��Ƽ���̷� ����
//			ActualDamage *= DamageMultiplier;
//
//			// Shield ���� - �������� ü�¿� ū ������ �ָ� �Ϲ� Ÿ�ݰ��� ������ ������ �����Ƿ� ������ �ƴ� TakeDamage �������� ó��
//			const float ShieldAmount = GetShield();
//			const float RemainShieldAmount = ShieldAmount - ActualDamage;
//			float AbsorbeDamage = 0.0f;
//
//			if (RemainShieldAmount <= 0.f)	// Shield �̻��� Damage�� ����
//			{
//				AbsorbeDamage = ShieldAmount;
//				ActualDamage = ActualDamage - ShieldAmount;
//			}
//			else // Shield �̸��� Damage �� ���� == Shield�� ����
//			{
//				AbsorbeDamage = ActualDamage;
//				ActualDamage = 0.f;
//			}
//
//			SetShield(ShieldAmount - AbsorbeDamage);
//			// Shield ���� - �������� ü�¿� ū ������ �ָ� �Ϲ� Ÿ�ݰ��� ������ ������ �����Ƿ� ������ �ƴ� TakeDamage �������� ó��
//		}
//
//		// 0 �ʰ� (�̻� �ƴ�) �������� �ּҵ����� 1�� ���� (== 0.02 �������� 1��)
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
//		// Role_Authority �� �ƴϸ� ���� ���� �ٷ� Damage�� Ȱ��
//		ActualDamage = Damage;
//	}
//
//	// Damage Log ����� - Network��嵵 RemoteTakeDamage ��Ʈ�� ����ͼ� ����Ǿ�� �ϹǷ� Roleüũ ����
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
//	if (Attacker && IsPlayerControlled() == false) // �÷��̾� �̿��� ������ ����.
//	{
//		if (Attacker->IsHeroMode())
//			SetHealth(0.f);
//	}
//
//	// CheatCommand - zombie
//	if (IsImmortalMode())
//		SetHealth(1.f);	// �ǰ� 0 ���ϸ� ���� �ǰݷ������� �귯���� ���� ( if(Health <= 0.f) ������ Ž )
//#endif
//
//	// DamageEffectInfo 3D DamageNum ��� 2D UI ��� DamageNum. 
//	if (ActualDamage > 0.f) // Shield ���� ����� ���� Ÿ�� �ٸ��� �ؼ� ���� ǥ���ϰ� �� ����..
//	{ // Critical damage ǥ�õ� ���⼭ ��.
//
//		// Remote Pawn vs Remote Pawn ���� Damage�� ���� ���� Local���� ǥ�� �� �ʿ䰡 �������� �ǹ�
//		// [171214_YJ] GameMode Rule�� �ٸ� ĳ���͵� ǥ���ϴ� ��� ����, 
//		// �ٸ�ĳ���͵� ǥ������ �ʴ´ٸ� �����ų� ��������� �� ���� üũ�ؼ� ���.
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
//		// bIgnoreNotifyToAttacker�����Ȱ� ��Ʈ������ �Ȱ�
//		// �̹��̸� ��Ʈ������ �н�
//		// ������µ� ��Ʈ������ �н�
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
//	// Immune ���¿��� �����̻� �Ұ�
//	if (IsImmune(&DamageInfo) == false)
//	{
//		TakeDamageAbnormalState(DamageInfo, DamageCauser);
//		ProcessTakeDamageAnimation(DamageInfo, DamageCauser);
//	}
//
//	// �̹��̴��� �����߿� ��������+(�ݰ�������)
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
	// 1. QTE ����Ʈ ���¿����� Damage Animatino ���� X
	if ((GetQTEEnabled() && QTEType == EQTEType::EQT_Mount))
		return false;

	const auto CurrentDamageType = DamageInfo.DamageType;

	// 2. �˹� or ��ٿ �������� ������� ������ Animation ���� X ( ������ ��츸 )
	if (IsMob())
	{
		if (CurrentDamageType == EAttackType::EAT_KnockBack && !IsAllowState(EStateAttackType::EDT_NockBack))
			return false;

		if (CurrentDamageType == EAttackType::EAT_BigBounce && !IsAllowState(EStateAttackType::EDT_BigBounce))
			return false;

		// ������ �ִϸ��̼��� ������� �ʴ� ���, ��� ����X
		if (!bAllowRecoilAnimation)
			return false;
	}

	// 3. CBT ���� -  Skill ������� ����
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
			1. Guard ����
			1-1. ArmorBreak : true - �ƸӺ극��ũ �ߵ�
			1-2. ArmorBreak : false - Armor���� ����

			2. Guard �� GuardBreak X
			2-1. ArmorBreak : true - �ƸӺ극��ũ ����
			2-2. ArmorBreak : false - Armor�� ���� ����

			3. Guard �� GuardBreak O
			3-1. ArmorBreak : true - �ƸӺ극��ũ �ߵ�
			3-2. ArmorBreak : false - Guard�� ����
		*/

		const bool IsArmorBreak = IsPreventByGuard == false && (IsArmorLower || IsForceArmorBreak);

		PreDamageAnimation(DamageInfo, IsArmorBreak);
		PostDamageAnimation(DamageInfo, DamageCauser);

		const bool bForceBounceOrKnockback = DamageInfo.bForceApplyStateDamage;

		// �� ���������� LastDamageType�� B2AnimInstance�� ���� �� Type
		// DamageInfo.DamageType�� �⺻���� ���� ĳ������ ���¿� ���� PreDamageAnimation���� LastDamageType���� Ȯ��
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
		// Player ���� Default�������� ArmorBreak��� TinyDamage ó��
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
		// Monster RightSwing �� AnimationFreeze �Ǿ������ �̻��� ���װ� ����... 
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
	//		// ��������Ʈ Yawȸ���̿ܿ� ����Ұ���. ��� �ȵǾ��ִ�.
	//		// ����ġ�� ��������ġ������� Z�� �����ؼ� �̿�
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

	//	// ���� �극��ũ�� ���� �������� �ļ� �߰����� ����
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
	"���� ������ = �� ���ݷ� (��ĳ���� ���ݷ� + ��ĳ���� ���� ���ݷ� + ��ĳ���� ���ݷ� + ��ĳ���� ���� ���ݷ�)
		x ��ĳ���� ��� �ɼ�	1. EnemyTotalAttack ( �Ѱ��ݷ� * ��ĳ���� ��� �ɼ� * 0.01f)
		x ���� ����			2. OriginalDamage
		x ������ ������		3. TotalDamageRate ( LevelWeight * DamageRate * CharacterDamageRate(�⺻100) * 0.01f)
		x ���� ������"		4. RandomDamage
		x ���� ������ ����		5. �ִ� ������ ���� ������ ����
		x ũ��Ƽ�� ������
	*/

	//BII_CHECK(Attacker);

	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//// 1. EnemyTotalAttack ( �Ѱ��ݷ� * ��ĳ���� ��� �ɼ� * 0.01f)
	//const float AttackCoefficient = (GameMode != nullptr) ? GameMode->GetAttackCoefficient() : CombatStatEval::GetDefaultCombatCoefficient();
	//const float EnemyTotalAttack = Attacker->TotalAttack * 0.01f * AttackCoefficient;

	//// 3. TotalDamageRate(IncreaseRate * DamageRate * ExtraDamageRate * CharacterDamageRate(�⺻100) * 0.01f)
	//const float IncreaseRate = GetBaseDamageIncreaseRate(Attacker, DamageInfo, DamageCauser);
	//const float DamageRate = GetDamageRate(Attacker, DamageInfo, DamageCauser);
	//const float ExtraDamageRate = GetExtraTakeDamageRate();	// ex) Poision ������ 10% ������ ��

	//const float TotalDamageRate = IncreaseRate * DamageRate * ExtraDamageRate * (CharacterDamageRate * 0.01f/*�⺻ 100*/);

	//// 4. RandomDamage
	//const float RandomDamage = (GameMode != nullptr) ? GameMode->GetDamageRandomRate() : 1.f;

	//// 5. ���� ���� Adjustment
	//// ���̵� ������ - ������ 99999 ���� ������ ���� hp / Damage ��� ���
	//const float AdjustmentRate = IsMob() ? GameMode->GetPVEProportionalConstant() : 1.f;

	//// ���� Damage ��� ����

	//// Step 1. Critical, �뷱�� ���� ��
	//float ActualDamage = EnemyTotalAttack
	//	* OriginalDamage
	//	* TotalDamageRate
	//	* RandomDamage
	//	* AdjustmentRate;

	//// Step 2. Critical ������ ���
	//float CriticalRate = DamageInfo.CriticalRateOverride == -1 ? Attacker->GetCriticalRate() : DamageInfo.CriticalRateOverride;
	//CriticalRate *= GetResistCriticalRate();
	//CriticalRate = FMath::Clamp(CriticalRate, 0.f, 1.f);
	//OutIsCritical = FMath::FRand() < CriticalRate;
	//const float CriticalDamageRate = OutIsCritical ? (DamageInfo.CriticalDamageRateOverride == -1 ? Attacker->GetCriticalDamageRate() : DamageInfo.CriticalDamageRateOverride) : 1.f;

	//ActualDamage *= CriticalDamageRate;

	//// Step3. PVP �� ��� ������ ���� - �ִ�ü�� 20% �̻��� ������(Ȥ�� ��庰 ���ǵ� �ִ뷮)�� �ѹ濡 ���� �� ����
	//if (GameMode && GameMode->IsPVPGameMode())
	//{
	//	const float MaxActualDamageRatio = GameMode->GetPVPMaxDamageRatioByGameModeType();
	//	ActualDamage = FMath::Min<float>(ActualDamage, GetMaxHealth() * MaxActualDamageRatio);

	//	if (IsApplyGuarding())
	//		ActualDamage *= 0.2f;
	//}

	////step4. Additional Damage ó�� �׳� Max(0, (������ IncAdditionalDamage - ����� DecAdditionalDamage))
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

	float IncreaseRate = Attacker->GetDamageIncreaseRate();	// �������̹Ƿ� 1.f + @ �� �Ǿ�� �� -> ( + ���� )

	// 10%������ 0.1�� �� �ԷµǾ��ٰ� �����Ѵ�.
	if (DamageInfo.DamageIncreaseRateOverride != -1)
		IncreaseRate += DamageInfo.DamageIncreaseRateOverride;

	float DecreaseRate = GetDamageDecreaseRate();			// ���Ұ��̹Ƿ� 1.f - @ �� �Ǿ�� �� -> ( * ���� )

	if (IsVictimPlayer && DamageCauser)
	{
		// �����ڿ� ���� ���� Actor�� ���� ������ RangeAttack���� �Ǻ� ( Projectile, DamageAreaActor ��� )
		const bool IsRangeAttack = Attacker != DamageCauser ? true : false;
		const float RangeDecreaseRate = IsRangeAttack ? GetRangeDamageDecreaseRate() : GetMeleeDamageDecreaseRate();
		DecreaseRate *= RangeDecreaseRate;
	}
	// Increase / Decrease ���� ����

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

	// 1. Player(Victim) vs Player(Attacker)	- 1 �� 1 
	//if (IsVictimPlayer && IsEnemyPlayer)
	//{
	//	ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//	const float PVPDamageScale = (GameMode != nullptr) ? GameMode->GetPVPDamageScaleByGameModeType() : CombatStatEval::GetDefaultPVPDamageScale();

	//	// PVP ������ �뷱�� ����
	//	// 1. 30% ~ 300% 
	//	DamageRate = FMath::Clamp(DamageRate, 0.3f, 3.f);

	//	// 2. PVPScale ������ ����
	//	DamageRate *= PVPDamageScale;
	//}

	// 2. Mob(Victim) vs Player(Attacker) - �÷��̾ ���� ����
	//else if (IsEnemyPlayer)
	//{
	//	// NPC ������ �Ѱ� ���� 4% ~ 500%
	//	DamageRate = FMath::Clamp(DamageRate, 0.04f, 5.f);
	//}
	// 3. ��������� - Mob(Victim) vs Mob(Attacker) / Player(Victim) vs Mob(Attacker)
	// �Ϲ����� ���� �������� ��忡�� ������ �¾�����
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
	return GetMesh(); // �ǹ̻� GetMesh �� ���� ���⺸�ٴ� GetBaseMesh �� ����� ���� ����.
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

	//	// �� ��ġ
	//	FVector vBoneLocation = bUseSocketName ? GetMesh()->GetSocketLocation(BoneName) : GetMesh()->GetBoneLocation(BoneName);

	//	// Ÿ���� ����̳� �ϴܿ� ���� ��츦 ����ؼ� pitch �� ��½ �����ش�.
	//	// ������ġ �������� �ϴ��� ����ġ ������� �ٲ�.
	//	FRotator DirectRotationToTarget = GetDirectionToTargetCharacter(vBoneLocation).Rotation();

	//	// ��ġ ������ �������̼� �����Ҷ���.. �ϰ������ ���� �����ͼ��ö����� �׳ɰ���.
	//	// ������ �ɸ��� ���� �����ϴ°���.
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
	//else // ���� ����� player character �� Ÿ������. (AIBlueprintSearchPlayer)
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

		//�ٴ� ã��

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
	//// ���� ����� player character �� Ÿ������. (AIBlueprintSearchPlayer)
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
	//// ���� ����� player character �� Ÿ������. (AIBlueprintSearchPlayer)
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
	//	if (TargetCharacter->IsPlayerControlled())	// [Mob -> Player Ÿ��] , [Player -> TargetActor & TargetObject Ÿ��]
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

	//// �ʴ� �̵���ų ���ͻ�����
	//float fAdjustAmountPerSec = 200.f;

	//// ��ġ�� �о����(this��).
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	ABladeIICharacter* pChar = Cast<ABladeIICharacter>(*Iterator);

	//	if (pChar && pChar != this)
	//	{
	//		// ���� ����Ҷ��� ó��
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

	//				// sweep���� ������ �������� �ݴ���������� �о���.
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
	//	// ���� state �� �޾Ƽ� AnimBP ���� ó���ϸ� ����ϴ� AnimMontage ���� ������ GetPCSESAnimTableObject �� ���ؼ�. 
	//	// -> ������ ���ŵǾ��� ���⵵ �ٸ� �߰� ó������ �ǹ̰� �������� ���ŵ� �� ����.
	//	CachedStageEventDirector = InDirectorActor;
	//	StageEventShowState = InNewState;
	//}
	//else
	//{
	//	if (InNewState == EStageEventShowState::ESES_None)
	//	{
	//		// InDirectorActor �� NULL �� ���� None ���� �����ϴ� ����� �޾Ƶ���. StageEventDirector �������� ���ٸ� Notify ���� ������ ���¸� �����ϴ� �뵵.
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
	//	CachedStageEventDirector->NotifyStageEventShowOver(this); // ��¦ �ٸ� ���̹� ����
	//	CachedStageEventDirector = NULL; // Notify �� ���۷��� ����.
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

// ĸ��ũ�� ���Խ� �÷��̾� ���β� �������.
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

		// Ÿ���� �ɸ��Ͱ��ƴϸ� ĸ����� ������
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
	//		// ���������� Radius�� ����
	//		const float DistSq = FVector::DistSquaredXY(Enemy->GetActorLocation(), GetActorLocation());
	//		if (DistSq <= RadiusSq)
	//		{
	//			if (!FMath::IsNearlyEqual(FanAngle, 180.0f)) // üũ ������ 360����� // ������ (-1)180�����
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
	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // �ٷ� ���� phase ��ȭ�� ����Ų data �ε���. ���� phase ��ȭ�� �����ٸ� -1
	if (CurrentPhaseDataIndex >= 0 && PhaseDataArray.Num() > CurrentPhaseDataIndex && PhaseDataArray[CurrentPhaseDataIndex].AttackDataArray.Num() > 0)
	{
		// Override AIAttackDataArray
		return PhaseDataArray[CurrentPhaseDataIndex].AttackDataArray;
	}

	return AIAttackDataArray; // �⺻ phase ������ �⺻ ��.
}

const TArray<int32>& ABladeIICharacter::GetInvalidAttackIndices()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_GetInvalidAttackIndices);
	int32 CurrentPhaseNum = GetCurrentPhaseNum();
	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // �ٷ� ���� phase ��ȭ�� ����Ų data �ε���. ���� phase ��ȭ�� �����ٸ� -1
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

		// ���������� �Ÿ�0������������ ����ɷ�
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
//	{ // TestDummyNPC ������ AIAttackIndex ���� �����ؼ� ���.
//		return;
//	}
//#endif
//	int32 CurrentPhaseNum = GetCurrentPhaseNum();
//	int32 CurrentPhaseDataIndex = CurrentPhaseNum - 1; // �ٷ� ���� phase ��ȭ�� ����Ų data �ε���. ���� phase ��ȭ�� �����ٸ� -1
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
	bInQTEState = bInState; // ���⼭�� ���ٸ� �� ���� BladeIIPlayer �ʿ���

	//if (bInQTEState)
	//{
	//	// UIDoc �ʿ� ���� ������ �־��ִµ� ABladeIIPlayer �ʿ����� �ϱ� ������ Player ���� bInQTEState ������ ���ϴ� �Ⱓ���� �� ª�� ���ؼ� ���⼭��.
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
	// bAbleToMove �� false �� ��� ���� �Է��� ���ϱ� ������ �ִϸ��̼� ���� �ڵ鸵 ���� �̵��� ������ ���� ��찡 �־ ���⼭ �˻� �� ��.
	if (bMovementPreventByStageEvent)
	{ // ��Ÿ ���� ���·� �����ؼ��� �ȵ� ���ǵ��� ���⼭ ���͸� �ϵ���..
		bSignalAttack = false;
	}
	else
	{
		bSignalAttack = bInSignal; // AnimBlueprint ���� �̰� �Ⱦ��ؼ� ���� ��� state �� �� ���� ������ ��.
	}
}

void ABladeIICharacter::BeginPlay()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_BeginPlay);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::BeginPlay"));

	Super::BeginPlay();
//
//	InitializeCombatStats(); // ���⼭ �� �� �ϴµ� NPCClassInfo �� ���� �ʿ��� ���� ���� �� �����Ƿ� ���ʼ� �ѹ� �� �ҷ���. -> �׷����� AB2MonsterSpawnPool::SpawnWave ���� bDeferConstruction �� �����ϹǷ� NPCClassInfo ���Ŀ� ����� ���� �� ��.
//
//	// It could be already created and try create again, if outer level is loaded twice by some mishandling.
//	DestroyUIManager();
//	CreateUIManager();
//
//	CharacterBeginPlayClass<ABladeIICharacter*>::GetInstance().Signal(this);
//
//	if (ShadowMaterial)
//	{
//		SetupDecalCompForCenterShadowCommon(CenterShadow, GetMesh());  // SetupControlledMatineePuppet ������ ����ϴ� ���� �¾� �κ�.. �̾��µ� �ᱹ ���⼭�� ���
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
//	// �޸� ���� ��. �ؽ��� ��Ʈ���� �Ǵ� ��� �ٸ� ��θ� ���� ó���� ��.
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
//	SetupLODInfoAsNPC(); // LOD �� �Ϻ� NPC ������..
//
//	// ���̳��� ���͸��� �̸� ������
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
	//// PC �� ��쿡�� Final**Scale ��� �����ۿ� ���� ������ ����� ��.

	//// Need to be change to use formula, item, sub PC, etc...
	//TotalAttack = CombatStatEval::GetMobBaseAttack(CharacterLevel);
	//TotalDefense = CombatStatEval::GetMobBaseDefense(CharacterLevel);

	//AttackLevel = CombatStatEval::GetADLevel(TotalAttack, CombatStatEval::GetDefaultCombatCoefficient());
	//DefenseLevel = CombatStatEval::GetADLevel(TotalDefense / 3.f, CombatStatEval::GetDefaultCombatCoefficient());

	//// Need to be change to use formula, skill/item %, etc...
	//const float BaseHealth = FinalHPScale * CombatStatEval::GetMobBaseHealth(CharacterLevel);
	//MaxHealth = BaseHealth;

	//// ���̵� ������ - ������ 99999 ���� ������ ���� hp / Damage ��� ���
	//auto* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GameMode && IsMob())
	//{
	//	MaxHealth *= B2GameMode->GetPVEProportionalConstant();
	//	if (IsBossMob())
	//		MaxHealth *= B2GameMode->GetModifiedBossmobMaxHealth();

	//	/// Ÿ���� �� ���ϰ�쿡 ������ hp ����.
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

	//DestroyUIManager(); // �÷��̾����� �¾� ���� ������ �� ��Ʈ�� ź��.

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

	//// ������ �ϱ��ϴµ�. ���װ� ���������� �𸣴�.
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
// 			CachedStateDamageStates[StateIndex].LastStartedTime = (float)FPlatformTime::Seconds(); // ���� ���ؼ� UI �� �Ѹ��� �켱���� ���ϱ� ��.
// 		}
// 		CachedStateDamageStates[StateIndex].RemainingTime = RemainingTime;
// 
// 		StateDamageBuffUpdateUIdoc();
// 	}
// }

// void ABladeIICharacter::SetCachedInvincibleBuffState(bool bIsOn, float RemainingTime)
// {
// 	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetCachedInvincibleBuffState);
// 	CachedInvincibleBuffState.bIsOn = bIsOn; // On Ȥ�� Off �������� ����ؾ� ��.
// 	if (bIsOn)
// 	{
// 		CachedInvincibleBuffState.LastStartedTime = (float)FPlatformTime::Seconds(); // ���� ���ؼ� UI �� �Ѹ��� �켱���� ���ϱ� ��.
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
	//{ // 0 �̳� ������ �̰� ���� �ɷ�.
	//	GetWorldTimerManager().ClearTimer(SetRegenerateHPHandle);
	//}
}

void ABladeIICharacter::SetCanBeDamagedForB2(bool bNewCanBeDamaged)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetCanBeDamagedForB2);
	//// false�� ī���� �ø��� true�� ī���� ����
	//!bNewCanBeDamaged ? nCanBeDamagedFalseCount++ : nCanBeDamagedFalseCount--;

	//UE_LOG(LogBladeII, Log, TEXT("nCanBeDamagedFalseCount = %d"), nCanBeDamagedFalseCount);

	//// ī���� 0���ϸ� ������ ����.
	//bCanBeDamaged = nCanBeDamagedFalseCount <= 0;
}

void ABladeIICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// ���� ������ ������Ʈ.. �� ƽ���� �ؾ߸��ϳ�
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
	{ // PC �� �̰Ϳ� ������� �ʵ��� NPCClassEnum üũ
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
//		// ��� ĳ���� spawn �� ���̴� ���̽� �������Ʈ���� �⺻�� ���ҽ����� ���� �ִµ� �̰� �������� ���� ������ Ŭ �� �ִ�. ��ŷ ���̺� �� ����.
//		if (IsSavingForCookingOnPreSave(TargetPlatform) && GetMesh())
//		{
//			GetMesh()->SetSkeletalMesh(nullptr);
//			GetMesh()->SetAnimInstanceClass(nullptr); // �̰� Ŭ �Ŵ�.
//
//			UE_LOG(LogBladeII, Log, TEXT("NULL out resouce properties of %s"), *this->GetPathName());
//		}
//	}
//#endif
}

void ABladeIICharacter::RegenerateHPByTimer()
{ // �ۼ�Ʈ Ȥ�� ���밪 ó��
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

	//// CPU data �� ������ �� �ִ� ������ Ÿ�̹��� ������ �ƹ��������� �����ϸ� �ȵ�.
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
	// SpawnPool ������ ���� ���� �� ĳ������ �������� ������ ������ ������.
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
	//	// ������ Mesh �� �غ��ؼ� �Ѱ��� ���� �ִ�.
	//	USkeletalMesh* MainSKMesh = OptionalSKMeshToSet ? OptionalSKMeshToSet : MyBaseMeshComp->SkeletalMesh;

	//	PuppetBaseMeshComp->SetSkeletalMesh(MainSKMesh); // ���⿡���� �ٸ� ������Ƽ �޽��� �ٲ�ĥ ���� ����. ��, Skeleton �� �ٸ� SkeletalMesh ��� �ٲ��ĺ��� �ִϸ��̼� �÷��̰� �� �� ��.

	//	PuppetBaseMeshComp->SetWorldScale3D(MyBaseMeshComp->GetComponentScale());

	//	UB2DamageEffectInfo* DmgFxInfo = GetDamageEffectInfo();

	//	// SkeletalMeshComponent �� ���� material override �� ����.
	//	if (!OptionalSKMeshToSet) // ���� Mesh �� �����ϴ� ���� ������Ʈ���� ��Ƽ������ �������� �ʴ´�.
	//	{
	//		// ���� �� ��Ƽ���� ����� �Ķ���͸� ����.
	//		ResetFlash();
	//		ResetEmissive();

	//		for (int32 MI = 0; MI < MyBaseMeshComp->GetNumMaterials(); ++MI)
	//		{
	//			// ���� ȿ���� ���� material override �� �� ��Ȳ�̶�� ���� material �� �����ִ� �� ������ ���̴�.
	//			// ī�޶��ũ�� ����ϴ� �����̸� ���� ��Ȳ�� �״�� ���� �� �����ϰ����� �̰� �ִϸ��̼Ǳ��� ����� ��鿡�� ����� ���̹Ƿ�.
	//			UMaterialInterface* MaterialToSet = GetOriginalMaterial(MI);
	//			PuppetBaseMeshComp->SetMaterial(MI, MaterialToSet);
	//		}
	//	}

	//	// ��Ÿ attach �� �͵��� �ִٸ� ����뿡�� ���������� ����.

	//	// StaticMesh �� ���� �͵�
	//	TArray<UActorComponent*> AttachedStaticMeshComps = this->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	//	for (int32 SMI = 0; SMI < AttachedStaticMeshComps.Num(); ++SMI)
	//	{
	//		UStaticMeshComponent* ThisComp = Cast<UStaticMeshComponent>(AttachedStaticMeshComps[SMI]);
	//		// �ϴ�, SkeletalMeshComponent �� ���� �͸� ����� ����.. AttachTo �� �� ThisComp->GetAttachParent() �ؼ� �Կ��� �� ���� �븩�̶�..
	//		if (ThisComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForStaticMesh(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach �� �ֵ鸶�� �ϳ��� �����ؼ� ���� StaticMesh ������ ���� socket �� �ٿ���.
	//			UStaticMeshComponent* PuppetAttachComp = NewObject<UStaticMeshComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetStaticMesh(ThisComp->GetStaticMesh());
	//				PuppetAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh �� SkeletalMeshActor �� ���̴� �翬�� Movable ��
	//				// AttachParent �� SkeletalMeshComponent �� �ƴ� ��찡 ����ٸ� ����..
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetMaterial(0, ThisComp->GetMaterial(0)); // Attach Component �� Material �� SpawnPool �� ���� overriding �� �� �����Ƿ� �̰͵� ����. �� 0����
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ���� �� ���Ŵ� ���ϹǷ� Ȥ�� �𸣴� collision �� ������
	//				PuppetAttachComp->RegisterComponent();
	//			}
	//		}
	//	}
	//	// SkeletalMesh �� ���� �͵�
	//	TArray<UActorComponent*> AttachedSKComps = this->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	//	for (int32 SKI = 0; SKI < AttachedSKComps.Num(); ++SKI)
	//	{
	//		USkeletalMeshComponent* ThisComp = Cast<USkeletalMeshComponent>(AttachedSKComps[SKI]);
	//		// �ϴ�, SkeletalMeshComponent �� ���� �͸� ����� ����.. AttachTo �� �� ThisComp->GetAttachParent() �ؼ� �Կ��� �� ���� �븩�̶�..
	//		// ���� ���� SkeletalMeshComponent �� �ٷ� ������ ó���ϹǷ� ���� ���~
	//		if (ThisComp && ThisComp != MyBaseMeshComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForSkeletalMesh(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach �� �ֵ鸶�� �ϳ��� �����ؼ� ���� SkeletalMesh ������ ���� socket �� �ٿ���.
	//			USkeletalMeshComponent* PuppetAttachComp = NewObject<USkeletalMeshComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetSkeletalMesh(ThisComp->SkeletalMesh);
	//				// AttachParent �� SkeletalMeshComponent �� �ƴ� ��찡 ����ٸ� ����..
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetMaterial(0, ThisComp->GetMaterial(0)); // Attach Component �� Material �� SpawnPool �� ���� overriding �� �� �����Ƿ� �̰͵� ����. �� 0����
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ���� �� ���Ŵ� ���ϹǷ� Ȥ�� �𸣴� collision �� ������
	//				PuppetAttachComp->RegisterComponent();
	//			}
	//		}
	//	}
	//	// ParticleSystem
	//	// ParticleSystemComponent ���� ������ ���� �ִµ� InMatineePuppet �� Hidden �̰ų� ���� �غ� �� �ȵ��� ���¿����� �̷��� ������ ���Ѻ��� Deactivate �� �Ǵ� �� ���� ���δٴ� ��.
	//	// ���⼭ �ƹ��� Activate �� �ص� �ҿ����. StageEventDirector �ʿ��� Visibility �����ϸ鼭 ���� ó���� ��.
	//	// �߰��� ���⼭�� BladeIIPlayer �� FloorRingPS �� �����Ǿ� �ٿ��� �ǵ� �װ͵� StageEventDirector �ʿ��� ó����..
	//	TArray<UActorComponent*> AttachedPSComps = this->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//	for (int32 PSI = 0; PSI < AttachedPSComps.Num(); ++PSI)
	//	{
	//		UParticleSystemComponent* ThisComp = Cast<UParticleSystemComponent>(AttachedPSComps[PSI]);
	//		// ����������, SkeletalMeshComponent �� ���� �͸� ���
	//		if (ThisComp
	//			// [���� 17.02.13] �����߰� ���εǴ°͵鸸 ���� : �Ͻ����� ���ݸ�ǿ��� AnimNotify �� �ٴ� �ֵ��� ��ŵ�ϰ��� �ϴ� �ǹ�. �⺻������ ���⼭ ���� �ʿ䰡 �ִ� ����Ʈ���� ���������� �÷��̵Ǵ� (Looping �Ǵ�) �ֵ��̴�.
	//			&& IsStaticallyVisiblePSC(ThisComp)
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForParticleSystem(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach �� �ֵ鸶�� �ϳ��� �����ؼ� ���� PS ������ ���� socket �� �ٿ���. 
	//			UParticleSystemComponent* PuppetAttachComp = NewObject<UParticleSystemComponent>(InMatineePuppet, NAME_None, RF_Transient);
	//			if (PuppetAttachComp)
	//			{
	//				PuppetAttachComp->SetRelativeTransform(ThisComp->GetRelativeTransform());
	//				PuppetAttachComp->SetTemplate(ThisComp->Template);
	//				PuppetAttachComp->AttachToComponent(PuppetBaseMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisComp->GetAttachSocketName());
	//				PuppetAttachComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ���� �� ���Ŵ� ���ϹǷ� Ȥ�� �𸣴� collision �� ������
	//				PuppetAttachComp->RegisterComponent();
	//				PuppetAttachComp->Activate(true);
	//			}
	//		}
	//	}

	//	// Decal �� �ʿ���.
	//	TArray<UActorComponent*> AttachedDecalComps = this->GetComponentsByClass(UDecalComponent::StaticClass());
	//	for (int32 DCI = 0; DCI < AttachedDecalComps.Num(); ++DCI)
	//	{
	//		UDecalComponent* ThisComp = Cast<UDecalComponent>(AttachedDecalComps[DCI]);
	//		// ����������, SkeletalMeshComponent �� ���� �͸� ���
	//		if (ThisComp
	//			&& ThisComp->IsVisible() && ThisComp->GetAttachParent() == MyBaseMeshComp && !IsCompInEventSceneBlacklist(ThisComp)
	//			&& DoesMatineePuppetSetupForDecal(InMatineePuppet, ThisComp) == false)
	//		{
	//			// Attach �� �ֵ鸶�� �ϳ��� �����ؼ� ���� material ������ ���� socket �� �ٿ���.
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

	//	// PointLight �� �ٿ����µ�.. �̰� ����� dynamic point light ���� ���� ������ �� �ΰ���. ���� �̰� �� �ϴ� ���� ����
	//	if (bSetupLightComponent)
	//	{
	//		TArray<UActorComponent*> AttachedLightComps = this->GetComponentsByClass(UPointLightComponent::StaticClass());
	//		for (int32 LCI = 0; LCI < AttachedLightComps.Num(); ++LCI)
	//		{
	//			UPointLightComponent* ThisComp = Cast<UPointLightComponent>(AttachedLightComps[LCI]);
	//			// �̰� Capsule �� ���� �͵�..
	//			if (ThisComp && ThisComp->IsVisible() // && ThisComp->bAffectsWorld // Light �� Visible �� �ƴ϶� AffectsWorld �� ���� �´µ�.. �̰� �ʿ��� ��Ȳ�ӿ��� AffectsWorld �� ���� ���� ���ɼ��� ���� �� ���Ƽ� ���⼭ üũ ���ϰ� bSetupLightComponent ���ڸ� ������ �Ѱ��ִ� �ɷ�.
	//				&& (ThisComp->GetAttachParent() == MyBaseMeshComp || ThisComp->GetAttachParent() == RootComponent)
	//				&& !IsCompInEventSceneBlacklist(ThisComp)
	//				&& DoesMatineePuppetSetupForLight(InMatineePuppet, ThisComp) == false)
	//			{
	//				// Attach �� �ֵ鸶�� �ϳ��� �����ؼ� ������ �ϰ� ���� socket �� �ٿ���.
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
	//	// ������ �¾��ߴµ� �����ϴ� ��찡 ���� �� ������ On/Off ó���� �ٽ�.
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
	//	// SetupControlledMatineePuppet ���� �־��� Material override ���� ��� �����Ѵ�. �׳� �θ� �ӽ� ���� ������Ʈ���� �ʿ� ����Ǿ� (�ٵ� PIE �� ���� �������� �ʳ�..?) ��ġ �ʴ� ���۷����� �߻��� �� ����.
	//	for (int32 MI = 0; MI < MyBaseMeshComp->GetNumMaterials(); ++MI)
	//	{
	//		PuppetBaseMeshComp->SetMaterial(MI, NULL);
	//	}
	//	// ��Ÿ SetupControlledMatineePuppet ���� �����ϴ� Component ���� ������ RF_Transient �� �ֹǷ� ���� �ȵ� ��. �̹� �µ��� ����ǰ� �־����� ���� �̻��� ���� �Ͼ����..
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
	checkSlow(InCheckComp && InCheckComp->GetOwner() == this); // ������ ���� Component �� ���� üũ �����.
	return EventPuppetSetupBlacklist.Contains(InCheckComp);
}

// ����� fForceStrength -���϶� �����. +���̸� �е�
void ABladeIICharacter::PullEnemyByTick(float fDeltaTime, float fForceStrength, float fRadius, FVector LocationOffset)
{
	//// bFixInAir�÷��׷� ���� �����Ʈ������ �н�
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

// �Ʒ� DoesMatineePuppetSetupFor** �� ���ø����ε� �� �ؼ� �ɱ��̳��� �������� �κ� �߷���.
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
	//// InCompToCheck �� �� BladeIICharacter �� �پ��ִ� ����.

	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedStaticComps = InMatineePuppet->GetComponentsByClass(UStaticMeshComponent::StaticClass());

	//for (int32 SCI = 0; SCI < MPAttachedStaticComps.Num(); ++SCI)
	//{
	//	UStaticMeshComponent* ThisMPComp = Cast<UStaticMeshComponent>(MPAttachedStaticComps[SCI]);

	//	// ThisMPComp �� InCompToCheck �� ������� �˻��Ѵ�. ���� ���ҽ��� ����ϰ� �پ��ִ� ���� �̸��� ��ġ�ϴ���.
	//	if (ThisMPComp && ThisMPComp->GetStaticMesh() == InCompToCheck->GetStaticMesh() && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		// �پ��ִ� �θ� ��ü�� ���� �ٸ����� ���� ������ �پ��ִ����� �˻�.
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
	// InCompToCheck �� �� BladeIICharacter �� �پ��ִ� ����.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedSKComps = InMatineePuppet->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	//for (int32 SCI = 0; SCI < MPAttachedSKComps.Num(); ++SCI)
	//{
	//	USkeletalMeshComponent* ThisMPComp = Cast<USkeletalMeshComponent>(MPAttachedSKComps[SCI]);

	//	// ThisMPComp �� InCompToCheck �� ������� �˻��Ѵ�. ���� ���ҽ��� ����ϰ� �پ��ִ� ���� �̸��� ��ġ�ϴ���.
	//	if (ThisMPComp && ThisMPComp->SkeletalMesh == InCompToCheck->SkeletalMesh && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // �پ��ִ� �θ� ��ü�� ���� �ٸ����� ���� ������ �پ��ִ����� �˻�.
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
	//// InCompToCheck �� �� BladeIICharacter �� �پ��ִ� ����.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedPSComps = InMatineePuppet->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//for (int32 PCI = 0; PCI < MPAttachedPSComps.Num(); ++PCI)
	//{
	//	UParticleSystemComponent* ThisMPComp = Cast<UParticleSystemComponent>(MPAttachedPSComps[PCI]);

	//	// ThisMPComp �� InCompToCheck �� ������� �˻��Ѵ�. ���� ���ҽ��� ����ϰ� �پ��ִ� ���� �̸��� ��ġ�ϴ���.
	//	if (ThisMPComp && ThisMPComp->Template == InCompToCheck->Template && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // �پ��ִ� �θ� ��ü�� ���� �ٸ����� ���� ������ �پ��ִ����� �˻�.
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
	// InCompToCheck �� �� BladeIICharacter �� �پ��ִ� ����.
	if (InMatineePuppet == NULL || InCompToCheck == NULL)
	{
		return false;
	}

	//TArray<UActorComponent*> MPAttachedDecalComps = InMatineePuppet->GetComponentsByClass(UDecalComponent::StaticClass());
	//for (int32 DCI = 0; DCI < MPAttachedDecalComps.Num(); ++DCI)
	//{
	//	UDecalComponent* ThisMPComp = Cast<UDecalComponent>(MPAttachedDecalComps[DCI]);

	//	// ThisMPComp �� InCompToCheck �� ������� �˻��Ѵ�. ���� ���ҽ��� ����ϰ� �پ��ִ� ���� �̸��� ��ġ�ϴ���.
	//	if (ThisMPComp && ThisMPComp->GetDecalMaterial() == InCompToCheck->GetDecalMaterial() && ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // �پ��ִ� �θ� ��ü�� ���� �ٸ����� ���� ������ �پ��ִ����� �˻�.
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
	// InCompToCheck �� �� BladeIICharacter �� �پ��ִ� ����.
	//if (InMatineePuppet == NULL || InCompToCheck == NULL)
	//{
	//	return false;
	//}

	//TArray<UActorComponent*> MPAttachedLightComps = InMatineePuppet->GetComponentsByClass(UPointLightComponent::StaticClass());
	//for (int32 LCI = 0; LCI < MPAttachedLightComps.Num(); ++LCI)
	//{
	//	UPointLightComponent* ThisMPComp = Cast<UPointLightComponent>(MPAttachedLightComps[LCI]);

	//	// ThisMPComp �� InCompToCheck �� ������� �˻��Ѵ�. �������̶� ���� �̸��� ��ġ�ϴ���.
	//	// Light �� ���� �˻� ���� ��� ���� �ʿ䰡 �� �ִ�.. ��� ���� �����ϴ� light �� ���̰� �ٴϴ� �ֵ��� ���� �־�� �ȵǰ�.
	//	if (ThisMPComp && //ThisMPComp->RelativeLocation == InCompToCheck->RelativeLocation && 
	//		ThisMPComp->Intensity == InCompToCheck->Intensity &&
	//		ThisMPComp->LightColor == InCompToCheck->LightColor &&
	//		ThisMPComp->AttenuationRadius == InCompToCheck->AttenuationRadius
	//		//ThisMPComp->GetAttachSocketName() == InCompToCheck->GetAttachSocketName() &&
	//		//AreComponentsAttachedToSameKind(ThisMPComp, InCompToCheck) // �پ��ִ� �θ� ��ü�� ���� �ٸ����� ���� ������ �پ��ִ����� �˻�.
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

	//// this ������ �Ѱ��־� ������ �ϰ� ���ķδ� �������� ���۷����� �ʿ����.
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM && B2GM->GetUIManager_InGameCombat())
	//{
	//	B2GM->GetUIManager_InGameCombat()->CreateHUDForNewChar(this);
	//}

	//if (CachedBirthplaceInfo.bSpawnedAsBossMob)
	//{
	//	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();

	//	//BeginPlay������ DestroyUIManager()�� ���� �ڽ��� �����ӿ��� �ұ��ϰ� UI�� �����Ѵ�.
	//	//UI �ٽ� ����.
	//	if (DocBattle && AmIInterestedBossMob())
	//	{
	//		DocBattle->SetbCurrentlyBossAppeared(true);
	//	}
	//}
}

void ABladeIICharacter::HideFloatingHPBar(bool bInHide)
{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_HideFloatingHPBar);
//	// DestroyUIManager �� Death �������� delay �Ǹ鼭 ���� ���� HPBar �� �ٷ� ���� �ʿ䰡 ����. ���� ���� ������ ������� �� �ƴ�.. �а� ���� �װ� �� �����̱� �ϳ�..
//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	if (B2GM && B2GM->GetUIManager_InGameCombat())
//	{
//		B2GM->GetUIManager_InGameCombat()->HideFloatingHPBarForChar(this, bInHide); // �̰� �÷��̾� �ƴ� NPC ���� ���� ��.
//	}
}

void ABladeIICharacter::DestroyUIManager()
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_DestroyUIManager);
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM && B2GM->GetUIManager_InGameCombat())
	//{
	//	B2GM->GetUIManager_InGameCombat()->DestroyHUDForDyingChar(this); // �̰� �÷��̾� �ƴ� NPC ���� ���� ��.
	//}

	//if (CachedBirthplaceInfo.bSpawnedAsBossMob)
	//{
	//	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//	// Ȥ�ö� �� ���� ä�� destroy �� ���� �����Ƿ� ������ �� �ֱ� �ϴµ�.. �� ������ ���޾� ������ ��쵵 �����Ƿ� ���� DocBattle �� ���õ� �� �ڽŰ� ������������ Ȯ���� �ؾ� �Ѵ�.
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
	// bSpawnedAsBossMob �� �� ������ ������ �ȵǾ� ���� �Ŷ� ���� �� �� �ִ� �� ����. �װ� SpawnPool ����..
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
	//	B2AICon->ToNextPhase(); // �ȿ��� �ٽ� PhaseDataArray �� ã�Ƽ� ����� ��.

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
	//	// �ߺ��� ESkillOption::ESO_DecReceiveDamage �� �̿��Ѱ��� ������ 4����ų�� �̰ɾ��µ�
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
	//	fBuffAmount = Amount + GetBonusBuffHealHPByHit() * 100.f;	// HealingHPByPercent ���� �ۼ�Ʈ ���� �����Ƿ� �̰� 100 ���س���.
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

// InAmount�� 0�̸� ��ų���� ������
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

	// ������ Immune NotifyState�� �ϴ� �� ���� - ForceStateAttack�� ���� ( ��ȹ ��û )
	if (bImmune && IsPlayerCharacter() == false)
		return true;

	// - �켱 Ȯ�� ���� : DamageInfo�� �����̻� ���������� �ִٸ� �ݰ� / ��ӽ�ų / �̹ù������� ���� ��� �����̻� ����
	if (IsForceStateAttack(DamageInfoPtr))
		return false;

	// - �Ϲ�����
	// 1. Skill ������� Immune
	// 2. �̹� ���� ������
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

	//// 1. Local�� ���� �������� ������ ������
	//if (IsPlayerControlled() && IsLocalCharacter())
	//	return true;

	//// 2. ���� �� �����̸� ������ ������
	//auto* LocalCharacter = GetLocalCharacter();
	//if (LocalCharacter && LocalCharacter == Attacker)
	//	return true;

	//// 3. ���� ź QTE Actor�� �����̸� ������ ������
	//if (Attacker && Attacker->IsQTEActor(LocalCharacter))
	//	return true;

	return false;
}

// BP���� ���°��� ����.
bool ABladeIICharacter::IsStunned() const
{
	return IsBuffActive(EBuffType::Debuff_Stun);
}

// BP���� ���°��� ����.
bool ABladeIICharacter::IsFrozen() const
{
	return IsBuffActive(EBuffType::Debuff_Freezing);
}

//void ABladeIICharacter::TemporarySetMeshComponentUpdateFlag(TEnumAsByte<EMeshComponentUpdateFlag::Type> InNewFlag)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_TemporarySetMeshComponentUpdateFlag);
//	//// ���޾Ƽ� �̰� ����ϴ� ���� ������. �ƴ� �ּ��� �׷� ���� �ִ��� ������ ��� �����Ϸ��� ���� �Ȱ��� �Ŷ� (AlwaysTickPoseAndRefreshBones ���� ��) �����ص� �ƹ� Ż ����.
//	//checkSlow(!bTemporarilySetMeshCompUpdateFlag || (GetBaseMesh() && GetBaseMesh()->MeshComponentUpdateFlag == InNewFlag));
//
//	//if (GetBaseMesh() && !bTemporarilySetMeshCompUpdateFlag) // ������ ����� �� Restore �ϱ� ���� �ߺ� ���� �� �ǵ���. �� ���õ�.
//	//{// ���簪�� ����� ���� ��� �ٸ� ���� ������̶�� ǥ���� ����.
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

	// ������ ĳ��
	if (OverridenMatTypeList.Num() == 0)
	{
		for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
		{
			// Cache
			CachedMaterials.Add(GetMesh()->GetMaterial(i));
		}
	}

	//���͸��� ����
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
	//	//����
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

	//// �������߿� ���������� ����
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
	//// NPC �Ϻΰ� LOD �۾��� �ʿ� ���� ���ӿ����� �ϰ����� LODInfo �� �ش�.
	//// ���ҽ��� ������ �¾��Ϸ��� ĸ�� ���󿡼� �� �� ����..
	//// ���� �÷��̾� ĳ����ó�� �ɼ��̳� ���Ӹ�� ���� ��Ȳ�� �� Ż �ʿ�� ���� ��. �ϴ� ������ �ΰ��� �÷��� ���� �信�� LOD1 �� ������.

	//// ���⼭ LODInfo �� �����ϴ� �޽��� ���� �����Ϳ� ��ϵ� ���ҽ��̴�. PC �� ���ó�� ��Ÿ�� merge �� �޽��� �ƴ�.
	//// �̷��� �����ϰ� ���� ���� �޽��� �����ϴ� �ٸ� ����(e.g. SkeletalMeshActor)�� �ִٸ� ������ �ް� �� ���̴�.
	//// BladeIIPlayer.cpp �ʿ� �̿� �ߺ��� �ڵ尡 ������ ��ƿ�� �� ���� ������ �ƴ�.

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
	//	// ��ǻ� LOD1 �� ScreenSize �� ������
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
	//// ����ġ�� ����Ʈ ����(����, �Ӹ�Ż�� ����)
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
	// ��� �� ���̰� �� ���ٰ� AnimBP ������ �Ŀ� visibility �� �ٽ� �Ҵ�. ��Ƽ������ anim ���� ����� ȯ�濡���� ������ �� �־ �̷� ���� ��.
	//-> ����Ƽ�� Ŭ���� ��� spawn ���� �̰� ����. ��ǻ� SetAnimInstanceClass

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
	//// ���� UI �� ǥ���� ���������� ����. �������� ���̾� ���� �� �տ����� destroy �Ǳ� ���� �ڿ� ���� spawn �ϴ� Ÿ�̹� ������ �־ �̷��� ������ �ʿ䰡 ����
	//UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocBattle)
	//{ // ���̾� ������ Class �� Variation �� Boss ���� �����ϸ� ���� �̰ɷε� �ҿ��� ���� ����.
	//	return CachedBirthplaceInfo.bSpawnedAsBossMob && DocBattle->GetBossMobClass() == NPCClassEnum && DocBattle->GetBossMobClassVariation() == NPCClassVariation;
	//}
	return CachedBirthplaceInfo.bSpawnedAsBossMob;
}

void ABladeIICharacter::SetupDecalCompForCenterShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIICharacter_SetupDecalCompForCenterShadowCommon);
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIICharacter::SetupDecalCompForCenterShadowCommon"));

	// ���� BladeIICharacter �ڽ��̶�� InDecalCompToUse �� CenterShadow �� ����. SetupControlledMatineePuppet ����� ���� �Ű������� �޴´�. 
	// ���⼭ ������Ʈ register ���� ������ �ʴ´�.
	//if (InDecalCompToUse && AttachParent)
	//{
	//	//InDecalCompToUse->SetHiddenInGame(false);
	//	InDecalCompToUse->SetRelativeScale3D(FVector(200.f + ShadowMaxDistance, ShadowSizeCenter, ShadowSizeCenter));

	//	InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	//	InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//}
}

UB2NPCSingleClassInfo* ABladeIICharacter::GetMyNPCClassInfo()
{ // �÷��̾� ĳ����, Ȥ�� NPCClassEnum �� ���õ��� ���� ��� �ҿ� ���� ��.
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