// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "BladeIIPlayer.h"
#include "BladeIIPlayerController.h"
//#include "BladeIIProjectile.h"
//#include "B2AutoAIController.h"
#include "BladeIIGameMode.h"
#include "B2TutorialGameMode.h"
//#include "B2SkillInfo.h"
#include "B2SkillRTManager.h"
//#include "B2InGameHUDInfo.h"
//#include "B2FloatingGoldAcq.h"
//#include "B2FloatingDamageNum.h"
//#include "B2UIManager_InGameCombat.h"
//#include "GameFramework/WorldSettings.h"
//#include "Components/ChainComponent.h"
//#include "Runtime/Engine/Public/EngineUtils.h"
//#include "Animation/SkeletalMeshActor.h"
//#include "SkeletalRenderPublic.h"
#include "Components/DecalComponent.h"
//#include "Navigation/PathFollowingComponent.h"
//#include "B2AreaDamageActorBase.h"
#include "Event.h"
//#include "B2UIDocHelper.h"
#include "B2CombatStatEvaluator.h"
//#include "B2LobbyInventory.h"
//#include "B2StageManager.h"
//#include "B2StageGameMode.h"
//#include "Camera/CameraAnimInst.h"
//#include "B2PCClassInfoBox.h"
//#include "B2BuffModeEffectInfo.h"
//#include "B2ActiveCameraActor.h"
//#include "BladeIIWorldSettings.h"
//#include "B2DestructibleLevelObjBase.h"
#include "B2SomeInfo.h"
//#include "BrainComponent.h"
//#include "DailyGameMode/B2CounterAttackGameMode.h"
//#include "B2EtherManager.h"
//#include "B2CharacterBuffManager.h"
//#include "B2Buff_Base.h"
//#include "B2GeneralBuffs.h"
//#include "BladeIIScalabilityControl.h"
//#include "B2EtherSetEffect.h"
//#include "B2EtherContainer.h"
//#include "B2UIManager.h"
//#include "B2UIBattleQTE.h"
//#include "B2CodeTable.h"
//#include "B2NetGameMode.h"
//#include "B2AnimInstance.h"
//#include "FB2ErrorMessage.h"
//#include "B2TargetSelector.h"
//#include "B2CharacterMovementComponent.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"
//#include "B2ControlGameMode.h"
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//#include "BladeIITestDummyNPC.h"
//#endif
#include "BladeIIUtil.h"
#include "DF.h"

#define FORCE_LOGGING_IN_ANDROID 0

FName ABladeIIPlayer::LeftFootBoneName(TEXT("Bip001-L-Toe0"));
FName ABladeIIPlayer::RightFootBoneName(TEXT("Bip001-R-Toe0"));
FName ABladeIIPlayer::TargetTraceName(TEXT("B2TargetTrace"));
FName ABladeIIPlayer::QTESocketName(TEXT("QTESocket"));
FName ABladeIIPlayer::FloorColorParamName(TEXT("ColorControl"));
FName ABladeIIPlayer::FloorColorMultiplyParamName(TEXT("ColorMultiplier"));

float ABladeIIPlayer::AutoMoveAttackState::GetCurrentTargetDistance()
{
	AActor* CurrentTarget = GetCurrentTargeActor();

	if (CurrentTarget)
		return Player->GetTargetLocationVectorFromPlayer(CurrentTarget).Size();

	return -1.0f;
}

AActor * ABladeIIPlayer::AutoMoveAttackState::GetCurrentTargeActor()
{
	bool bTargetActorValid = (Player->TargetActor && Player->TargetActor->IsAlive() && Player->GetCapsuleComponent());
	bool bTargetObjectValid = Player->TargetObject && Player->GetCapsuleComponent();

	if (bTargetActorValid)
		return Player->TargetActor;
	else if (bTargetObjectValid)
		return Player->TargetObject;
	else
		return nullptr;
}

void ABladeIIPlayer::AutoMoveAttackState::StopAutoMove()
{
	Player->AutoMoveTargetActor = nullptr;
	ABladeIIPlayerController* B2Controller = Cast<ABladeIIPlayerController>(Player->GetController());
	if (B2Controller)
		B2Controller->StopMovement();
}

void ABladeIIPlayer::AutoMoveAttackState::StartAutoMove(AActor * CurrentTarget)
{
	//if (Player->m_bAttackKeyPressed)
	//{
	//	if (Player->AutoMoveTargetActor != CurrentTarget)
	//	{
	//		ABladeIIPlayerController* B2Controller = Cast<ABladeIIPlayerController>(Player->GetController());
	//		if (B2Controller && B2Controller->MoveToActor(CurrentTarget, Player->DefaultAttackRange))
	//		{
	//			Player->StopAttack();
	//			Player->AutoMoveTargetActor = CurrentTarget;
	//		}
	//	}
	//}
}

bool ABladeIIPlayer::AutoMoveAttackState::CheckAutoMoveRadius(float InDistance)
{
	return InDistance <= (Player->DefaultAttackRange + Player->AutoMoveRange) && InDistance > Player->DefaultAttackRange;
}

bool ABladeIIPlayer::AutoMoveAttackState::CheckAttackTargetRadius(float InDistance)
{
	return InDistance <= Player->DefaultAttackRange;
}

void ABladeIIPlayer::AutoMoveAttackState::Tick(float delta)
{
	//bool bInputValid =  Player->GetLastMovementInputVector() != FVector::ZeroVector && 
	//				   !Player->GetCharacterMovement()->bApplyOptionalVelocityDuringRootmotion && 
	//				   !Player->bDisableRotateToInput;

	//if (Player->IsQTEMounting() || bInputValid)
	//{
	//	Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::NoSearchTarget);
	//	return;
	//}

	//float Distance = GetCurrentTargetDistance();
	//if (Distance < 0.0f)
	//{
	//	Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::NoSearchTarget);
	//	return;
	//}

	//bool bInAutoMoveRadius = CheckAutoMoveRadius(Distance);
	//if (bInAutoMoveRadius)
	//{
	//	if (Player->IsInAutoPlay())
	//	{
	//		Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::AttackTarget);
	//		return;
	//	}
	//	Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::AutoMoveToTarget);
	//	return;
	//}

	//bool bInAttackRadius = CheckAttackTargetRadius(Distance);
	//if (bInAttackRadius)
	//{
	//	Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::AttackTarget);
	//	return;
	//}
}


void ABladeIIPlayer::NoSearchTargetState::Enter()
{

}
void ABladeIIPlayer::NoSearchTargetState::Leave()
{

}
void ABladeIIPlayer::NoSearchTargetState::Tick(float delta)
{
	AutoMoveAttackState::Tick(delta);
}

void ABladeIIPlayer::AutoMoveToTargetState::Enter()
{
	AActor* CurrentTargetActor = GetCurrentTargeActor();
	
	StartAutoMove(CurrentTargetActor);
}
void ABladeIIPlayer::AutoMoveToTargetState::Leave()
{
	StopAutoMove();
}
void ABladeIIPlayer::AutoMoveToTargetState::Tick(float delta)
{
	AutoMoveAttackState::Tick(delta);

	if (Player->CurrentMoveAttackState != EAutoMoveAttackState::AutoMoveToTarget)
		return;

	AActor* CurrentTargetActor = GetCurrentTargeActor();
	if (Player->AutoMoveTargetActor != CurrentTargetActor)
		StartAutoMove(CurrentTargetActor);
}

void ABladeIIPlayer::AttackTargetState::Enter()
{
	if (Player->m_bAttackKeyPressed)
		Player->StartAttackFromInput();
}
void ABladeIIPlayer::AttackTargetState::Leave()
{

}
void ABladeIIPlayer::AttackTargetState::Tick(float delta)
{
	AutoMoveAttackState::Tick(delta);
}

ABladeIIPlayer::ABladeIIPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// TODO : Init Data for Character Type
	CharacterType = ECharacterType::ECT_UnknownPC;
	Health = 500.f;
	MaxHealth = 500.f;
	//MaxArmor = 100.f;

	TargetingMode = ETargetingMode::ETM_Normal;
	TagAttackMode = ETagAttackMode::EAM_DownAttack;

	CameraPitch = -50.f;
	CameraDistance = 700.f;
	CameraYaw = -130.f;

	MaxDistanceToAllowAutoAttack = 450.0f;

	TotalDashDuration = -1.f;
	CurrentDashDuration = -1.f;
	MaxDashDistance = 1000.f;
	MaxDashSpeed = 10000.f;

	BP_ConfigMinDashDistance = -1.f;
	BP_ConfigMaxDashDistance = -1.f;

	TargetActor = NULL;
	TargetObject = NULL;
	TargetingDistance = 2000.f;
	bDisableRotateToTarget = false;
	bDisableRotateToInput = false;
	RotateToTargetSpeed = 0.f;
	CounterCheckTime = 0.25f;
	EvadeCheckTime = 0.4f;

	bIsInCriticalCondition = false;
	CriticalConditionHealthRatio = 0.2f;
	bInvincibleByStageEvent = false;
	bReachedAtDeadEnd = false;

	bStopAttackSignal = false;

	bSkillCancel = false;

	CameraCullingRadius = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom...
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	UpdateCameraDistance(CameraDistance);
	//CameraBoom->RelativeRotation = FRotator(CameraPitch, CameraYaw, 0.f);
	//CameraBoom->bDoCollisionTest = true;

	// Create a camera...
	ActionCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("ActionCamera"));
	ActionCameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	ActionCameraComponent->bUsePawnControlRotation = false;
	ActionCameraComponent->bAutoActivate = false;

	TopDownCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachToComponent(CameraBoom, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TopDownCameraDefaultRotator = TopDownCameraComponent->GetRelativeTransform().GetRotation().Rotator();
	
	// LightComponent 绰 attachment 甫 促矫 技泼窍霸 瞪 荐 乐促.
	LightComponent = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("PointLightComponent"));
	LightComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	LightComponentAttachDistZ = 425.0f;
	//LightComponent->RelativeLocation = FVector(0.f, 0.f, 425.f);
	LightComponent->Intensity = 45000.f;
	LightComponent->LightColor = FColor(255, 222, 164);
	LightComponent->AttenuationRadius = 800.f;

	bHeadPointLightIsFormallyOn = false; // 老馆利牢 版肺肺 夸 LightComponent 甫 难绊 缠 锭俊绰 bHeadPointLightIsFormallyOn 甫 鞍捞 技泼秦具 茄促.

	//GetCharacterMovement()->bIgnoreForceDuringRootmotion = true;

	//// Create chain components
	//LeftWeapon = ObjectInitializer.CreateDefaultSubobject<UChainComponent>(this, TEXT("LeftWeaponComponent"));
	//LeftWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//LeftWeapon->Deactivate();

	//RightWeapon = ObjectInitializer.CreateDefaultSubobject<UChainComponent>(this, TEXT("RightWeaponComponent"));
	//RightWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//RightWeapon->Deactivate();

	ShadowSizeFoot = 75.f;

	DefaultAttackRange = 350.0f;
	AutoMoveRange = 200.0f;

	AutoMoveTargetActor = nullptr;

#if 0
	// Create foot shadow components
	LeftFootShadow = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("LeftFootShadowComponent"));
	//LeftFootShadow->bAbsoluteLocation = true;
	LeftFootShadow->bAbsoluteRotation = true;
	LeftFootShadow->RelativeLocation = FVector::ZeroVector;
	LeftFootShadow->RelativeScale3D = FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot);
	LeftFootShadow->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	LeftFootShadow->DecalSize = FVector(1.f, 1.f, 1.f); // for 4.11
	//LeftFootShadow->bHiddenInGame = true;
	LeftFootShadow->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	LeftFootShadow->Deactivate();

	RightFootShadow = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("RightFootShadowComponent"));
	//RightFootShadow->bAbsoluteLocation = true;
	RightFootShadow->bAbsoluteRotation = true;
	RightFootShadow->RelativeLocation = FVector::ZeroVector;
	RightFootShadow->RelativeScale3D = FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot);
	RightFootShadow->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	RightFootShadow->DecalSize = FVector(1.f, 1.f, 1.f); // for 4.11
	//RightFootShadow->bHiddenInGame = true;
	RightFootShadow->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	RightFootShadow->Deactivate();
#endif

	FloorRingPSC = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("FloorRingPSC"));
	FloorRingPSC->SecondsBeforeInactive = 0.f;
	FloorRingPSC->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
 	FloorRingPSC->Deactivate();	

	// 浇肺快, 葫搬锭档 力加档肺 倒霸
	//FloorRingPSC->bUseOwnerCustomTimeDilation = false;

	// 捞 FloorRing 篮 楷免厘搁 器窃矫虐瘤 臼档废 喉发府胶飘俊 眠啊窃.
	EventPuppetSetupBlacklist.Add(FloorRingPSC);

	FloorRingColorMultiplier = 1.f;

	ComboMaintainDuration = 3.0f;
	
	bLastInvokedAnySkillAttack = false;
	
	////////////////////////////////////////////////////////////////////////////////
	//>>> Widget classes specification begin

	// They will be managed not to be created, but let's null out anyway.
	//FloatingHPBarWidgetClass = NULL;
	HiddenMobPointerPSTemplate = NULL;


	//if (!GMinimalDLCFrontMode)
	//{ // DLC Front 葛靛 府家胶肺爹 弥措茄 力芭
	//	FString DefaultHiddenMobPointerPSTemplatePath;
	//	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultHiddenMobPointerPSTemplate"), DefaultHiddenMobPointerPSTemplatePath, GGameIni);
	//	HiddenMobPointerPSTemplate = LoadObject<UParticleSystem>(NULL, *DefaultHiddenMobPointerPSTemplatePath);
	//	BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("HiddenMobPointerPSTemplate"))); // MinimalDLCFrontMode 肺 牢秦 厚绢乐阑 BaseBP 率 蔼阑 啊廉坷瘤 臼扁 困秦 捞抚 眠啊

	//	FString DefaultForceFeedbackPath;
	//	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultForceFeedbackPath"), DefaultForceFeedbackPath, GGameIni);
	//	ForceFeedbackSetting = LoadObject<UForceFeedbackEffect>(NULL, *DefaultForceFeedbackPath);
	//	BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("ForceFeedbackSetting"))); // MinimalDLCFrontMode 肺 牢秦 厚绢乐阑 BaseBP 率 蔼阑 啊廉坷瘤 臼扁 困秦 捞抚 眠啊
	//}

	//<<< Widget classes specification end
	////////////////////////////////////////////////////////////////////////////////

	//CachedSkillRTManager = NULL;
	//HUDInfoClass = NULL;
	//
	//bFinalizedByPCClassInfo = false;
	//
	//PlayerTopDownCamInitialFOV = 90.0f;

	//CharacterLevel = 10;

	//bCheatImmortalMode = false;
	//bCheatHeroMode = false;

	//InAutoPlay = AutoBattleType::NONE;

	bEventsSubscribed = false;

	bPendingTag = false;
	bPendingQTENext = false;

	SwipeTouchIndex = ETouchIndex::Touch1;
	LastTouchLocation = FVector::ZeroVector;
	bIsSwiping = false;
	bIsTagPossible = true;

	// Anim Notify Data
	CachedFOV = 80.f;
	CachedOriginalTargetArmLength = 0.f;
	CachedOriginalCamBoomRelativeRotation = FRotator::ZeroRotator;
	CachedOriginalCamBoomRelativeLocation = FVector::ZeroVector;
	CameraDistZoomEstimatedPlayTime = 0.f;
	//bCachedForCameraDistZoomState = false;
	//CameraAnimPlayInstance = NULL;
	//RemainCameraAnims = 0;
	//CurrentCameraAnimTime = 0.f;
	//CameraShakePlayInstance = NULL;

	CachedLocation = FVector::ZeroVector;
	CurrentLocation = FVector::ZeroVector;
	DeltaPerTick = FVector::ZeroVector;
	B2RandomStartLocation = FVector::ZeroVector;

	OriginalCameraLagSpeed = 0.f;
	OriginalCameraLagMaxDist = 0.f;
	OriginalLightColor = FColor(255, 222, 164);
	OriginalLightIntensity = 40000.f;
	LightPrevColorBlend = 0.f;

	bKeepAttacking = false;
	bCachedPressedInCinematic = false;
	LastGuardTime = 0.f;
	//CachedBaseMaxWalkSpeed = 0.f; // PCClassInfo 俊 技泼等 蔼捞 甸绢啊 乐绢具 窃. Elevated to BladeIICharacter
	
	WeaponSkillPoint = 0;
	CurrentSkillAnimIdx = ESkillAnimType::ESA_End;
	SkillLOD = 0;
	
	RandDamageAnimType = EAttackType::EAT_Default;

	bEnableNormalAttack = true;

	QTENPCClass = ENPCClass::ENC_End;
	QTEMountRemainingTime = 0.0f;

	// 敲饭捞绢 扁夯汲沥 饭靛评
	SetTeamNum(int32(ETeamType::ETT_Red));

	//CharacterDataStore = NULL;

	m_fDeadTimeSec = -1;

	BaseDamageDecreaseRate = 1.0f;
	MeleeDamageDecreaseRate = 1.0f;
	RangeDamageDecreaseRate = 1.0f;
	BossDamageDecreaseRate = 1.0f;
	ResistCriticalRate = 1.0f;
	StateDurationDecreaseRate = 1.f;
	IncDamageValueByPassive = 0.0f;
	IncNormalDamageValueByPassive = 0.0f;
	IncSkillDamageValueByPassive = 0.0f;
	IncDamageToBossValueByPassive = 0.0f;
	IncCriticalRateValueByPassive = 0.0f;
	IncCriticalDamageValueByPassive = 0.0f;
	HealHPAmountRateOnCounter = 0.0f;
	HealHPAmountRateOnKill = 0.0f;
	AbsorbHPAmountRateOnAttack = 0.0f;
	AdditionalAttackSpeedRatebyPassive = 0.0f;
	FPlatformMemory::Memzero(IncAttackDamageByPassiveToClass, sizeof(IncAttackDamageByPassiveToClass));
	FPlatformMemory::Memzero(DecReceiveDamageByPassiveFromClass, sizeof(DecReceiveDamageByPassiveFromClass)); // 扁夯蔼 1 捞 嘎变 茄单 檬扁蔼 持绢林扁 瞒盔捞聪..

	InjectedGameMode = nullptr;

	LastRandomTargetLocation = FVector::ZeroVector;
	LastRandomTargetDirection = FVector::ZeroVector;

	m_bAttackKeyPressed = false;
	m_bGuardKeyPressed = false;

	bInWeaponSkillLOD0 = false;
	WeaponSkillLOD0SceneLight = nullptr;

	IsUnityTick = false;
	bOverUnityPoint = true;

	//AutoSkillPriority = { WEAPON_6STAR_SKILL_INDEX/*3*/, 0, 1, 2 };

	NoSearchTargetPtr = std::make_shared<NoSearchTargetState>(this);
	AutoMoveToTargetPtr = std::make_shared<AutoMoveToTargetState>(this);
	AttackTargetPtr = std::make_shared<AttackTargetState>(this);

	CurrentAutoMoveAttackStatePtr = NoSearchTargetPtr;
	ChangeAutoMoveAttackState(EAutoMoveAttackState::NoSearchTarget);

	PrevPosition = FVector::ZeroVector;
	StayTime = 0.0f;
	bRestartLevel = false;
	
	GuardCoolTime = 0.0f;
	HasNotTagDefence = false;
}

bool ABladeIIPlayer::CheckPlayerStopTime(float DeltaTime)
{
	if (bRestartLevel)
		return false;

	//AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this));
	//if (!StageGM)
	//	return false;

	//if (StageGM->GetStageManager()->IsStageCleared())
	//	return false;

	//if (IsInAutoPlay() == 0)
	//	return false;

	//if (!IsAlive())
	//	return false;

	//if (UGameplayStatics::IsGamePaused(GetWorld()) == true)
	//	return false;

	//if (GetHadInput())
	//{
	//	StayTime = 0.0f;
	//	return false;
	//}

	//FVector CurPosision = GetActorLocation();
	//if (CurPosision.Equals(PrevPosition))
	//{
	//	StayTime += DeltaTime;
	//}
	//else
	//{
	//	StayTime = 0.0f;
	//	PrevPosition = CurPosision;
	//}

	return StayTime > RESTART_TIME;
}

void ABladeIIPlayer::AddGuardCoolTime(float InAddCoolTime)
{
	if (InAddCoolTime > 0)
	{
		SetGuardCoolTime(GetGuardCoolTime() + InAddCoolTime);
		SetHasNotTagDefence(true);
	}
}

void ABladeIIPlayer::SetGuardCoolTime(float InCoolTime)
{
	GuardCoolTime = InCoolTime;
	UB2UIDocBattle* DocBattle = GetDocBattle();
	if (DocBattle)
	{
		//DocBattle->SetGuardCoolTime(GuardCoolTime);
	}
}

float ABladeIIPlayer::GetGuardCoolTime()
{
	return GuardCoolTime;
}

bool ABladeIIPlayer::GetHasNotTagDefence()
{
	return HasNotTagDefence;
}

void ABladeIIPlayer::SetHasNotTagDefence(bool InHasNotTagDefence)
{
	UB2UIDocBattle* DocBattle = GetDocBattle();
	if (DocBattle)
	{
		HasNotTagDefence = InHasNotTagDefence;
		//DocBattle->SetHasNotTagDefence(InHasNotTagDefence);
	}
}

void ABladeIIPlayer::UpdateGuardCoolTime(float InDeltaSeconds)
{
	if (GetGuardCoolTime() > 0.0f)
	{
		SetGuardCoolTime(GetGuardCoolTime() - InDeltaSeconds);

		if (GetGuardCoolTime() <= 0.0f)
		{
			SetGuardCoolTime(0.0f);
			SetHasNotTagDefence(false);
		}
	}
}

void ABladeIIPlayer::SetReservedSkill(EAttackState InSkillState)
{
	if (GetReservedSkill() == EAttackState::ECS_None && IsOneOfSkillAttackState(InSkillState))
	{
		ReservedSkill = InSkillState;
		CachedSkill = InSkillState;
		if (auto* World = GetWorld())
			LastSkillInputTime = World->GetTimeSeconds();
#ifdef FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLog ReservedSkillState : %d / time : %f threadId : %d\n"), static_cast<int32>(InSkillState), LastSkillInputTime, FPlatformTLS::GetCurrentThreadId());
#endif
	}
}

void ABladeIIPlayer::ConsumeReservedSkill()
{
	ReservedSkill = EAttackState::ECS_None;
	LastSkillInputTime = 0.f;
}

ESkillAnimType ABladeIIPlayer::GetCurrentSKillAnimIdx()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCurrentSKillAnimIdx);

	return GetCurrentSKillAnimIdx_Native();	
}

ESkillAnimType ABladeIIPlayer::GetCurrentSKillAnimIdx_Native()
{
	//[@AKI, 170815] 傈捧 盒籍 吝 SkillLOD肺 啊廉 坷绰 何盒 牢 淀 窃.
	if (SkillLOD != 0 && CurrentSkillAnimIdx >= ESkillAnimType::ESA_Weapon_01)
	{
		return static_cast<ESkillAnimType>(static_cast<int32>(ESkillAnimType::ESA_Weapon_Normal_End) + static_cast<int32>(CurrentSkillAnimIdx) - static_cast<int32>(ESkillAnimType::ESA_Weapon_01) + 1);
	}
	return CurrentSkillAnimIdx;
}

void ABladeIIPlayer::BeginPlay()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_BeginPlay);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::BeginPlay"));

	// 捞 矫痢俊 PlayerController 狼 possess 啊 登绢 乐绢具 窃. UB2SkillRTManager::DynSpawnNewPlayerCommon 捞唱 ABladeIIGameMode::StaticSpawnPCClassInfoPlayer 曼绊
	//GetSkillRTManager();
	SetupMeshesMain(); // 厘厚 炼钦俊 蝶弗 皋浆 悸诀

	SyncPCCharacterLevelFromExpManager(); // InitializeCombatStats 捞傈俊..

	// 母蕉 哪器惩飘 胶纳老捞 [敲饭捞绢 胶鸥飘] 胶纳老蔼俊 康氢阑 罐澜
	// 救傈窍霸 檬扁拳
	GetCapsuleComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	//if (EtherContainer == nullptr)
	//	EtherContainer = NewObject<UB2EtherContainer>(this, UB2EtherContainer::StaticClass()->GetFName());

	//Super::BeginPlay();

	//USkeletalMeshComponent* SKMeshComp = GetMesh();
	//if (LeftWeapon)
	//{
	//	LeftWeapon->TargetBoneNames.Add(FName(TEXT("L_Weapon_Cp_01")));
	//	LeftWeapon->TargetBoneNames.Add(FName(TEXT("L_Weapon_Cp_02")));
	//	LeftWeapon->TargetBoneNames.Add(FName(TEXT("L_Weapon_Cp_03")));
	//	LeftWeapon->TargetBoneNames.Add(FName(TEXT("L_Weapon_Cp_04")));

	//	LeftWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	//	LeftWeapon->AttachToComponent(SKMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//	LeftWeapon->ReregisterComponent();
	//	if (LeftWeapon->SkelComp)
	//	{
	//		LeftWeapon->Activate();
	//	}
	//}

	//if (RightWeapon)
	//{
	//	RightWeapon->TargetBoneNames.Add(FName(TEXT("R_Weapon_Cp_01")));
	//	RightWeapon->TargetBoneNames.Add(FName(TEXT("R_Weapon_Cp_02")));
	//	RightWeapon->TargetBoneNames.Add(FName(TEXT("R_Weapon_Cp_03")));
	//	RightWeapon->TargetBoneNames.Add(FName(TEXT("R_Weapon_Cp_04")));

	//	RightWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	//	RightWeapon->AttachToComponent(SKMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	//	RightWeapon->ReregisterComponent();
	//	if (RightWeapon->SkelComp)
	//	{
	//		RightWeapon->Activate();
	//	}
	//}

	//if (ActionCameraComponent)
	//{
	//	ActionCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	//}

#if 0
	if (ShadowMaterial)
	{
		if (SKMeshComp->GetSocketByName(B2PLAYER_SHADOW_SOCKET_NAME_LF))
		{
			SetupDecalCompForLeftShadowCommon(LeftFootShadow, SKMeshComp); // SetupControlledMatineePuppet 俊辑档 荤侩窍绰 傍烹 悸诀 何盒.. 捞菌绰单 搬惫 咯扁辑父 荤侩

			LeftFootShadow->ReregisterComponent();
			LeftFootShadow->SetDecalMaterial(ShadowMaterial);
			LeftFootShadow->Activate();
		}
		else
		{
			LeftFootShadow->Deactivate(); // 家南捞 穿遏登菌促搁 瞒扼府 见败具 捞惑茄 霸 救 唱咳.
			LeftFootShadow->SetVisibility(false);
		}

		if (SKMeshComp->GetSocketByName(B2PLAYER_SHADOW_SOCKET_NAME_RF))
		{
			SetupDecalCompForRightShadowCommon(RightFootShadow, SKMeshComp);  // SetupControlledMatineePuppet 俊辑档 荤侩窍绰 傍烹 悸诀 何盒.. 捞菌绰单 搬惫 咯扁辑父 荤侩

			RightFootShadow->ReregisterComponent();
			RightFootShadow->SetDecalMaterial(ShadowMaterial);
			RightFootShadow->Activate();
		}
		else
		{
			RightFootShadow->Deactivate();
			RightFootShadow->SetVisibility(false);
		}
	}
#endif

	auto* InGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	check(InGameMode);

	if (InGameMode->bOverrideFloorRingColor && InGameMode->FloorRingColors.Num() > GetTeamNum())
	{
		FloorRingColor = InGameMode->FloorRingColors[GetTeamNum()];
	}

	if (InGameMode->bOverrideFloorRingColor && InGameMode->FloorRingColorMultipliers.Num() > GetTeamNum())
	{
		FloorRingColorMultiplier = FMath::Max(1.f, InGameMode->FloorRingColorMultipliers[GetTeamNum()]);
	}

	if (FloorRingPS)
	{
		ChangeFloorRingPS(FloorRingPS);	
	}

	SetupLightComponentAttachment();
	// GameRule 绊妨秦辑 脑..
	FormalSetPointLightOn(true);
	BII_CHECK(GetInjectedGameRule());

	SubscribeEvents();

	// UpdateCamera** 阑 烹秦 技泼且 锭俊绰 Camera** 蔼甸捞 鞍捞 某教凳.
	CameraDistance = CameraBoom->TargetArmLength;
	//CameraPitch = CameraBoom->RelativeRotation.Pitch;
	//CameraYaw = CameraBoom->RelativeRotation.Yaw;
	PlayerTopDownCamInitialFOV = TopDownCameraComponent->FieldOfView;

	CameraLengthValueForBlending.TargetArmLength = CameraDistance;
	CameraFovValueForBlending.CameraFov = PlayerTopDownCamInitialFOV;

	LastAttackSuccessTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	CurrentComboNum = 0;

	CanEvade = true;
	EvadeTimer = 0.0f;

	InitUpdateUIDoc(); // 惑困 努贰胶俊辑 何福变 且扒单 咯扁辑 肚 秦辑 唱卉 芭 绝瘤. 弊货 诀单捞飘 登绰 霸 唱棵荐档


#if !UE_BUILD_SHIPPING
	extern bool gUseCharacterDefaultMaterial; // 某腐磐 default material 抛胶飘
	if (gUseCharacterDefaultMaterial)
	{
		USkeletalMeshComponent* MainMeshComp = GetMesh();
		for (int32 MI = 0; MI < MainMeshComp->GetNumMaterials(); ++MI)
		{
			//MainMeshComp->SetMaterial(MI, UMaterial::GetDefaultMaterial(MD_Surface));
		}
	}
#endif
}

void ABladeIIPlayer::BeginDestroy()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_BeginDestroy);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::BeginDestroy"));

	// Subscribe 甫 茄 Player 父.. Unsubscribe 矫 了促 秦力秦滚覆.
	//ABladeIIPlayer* CurrentLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if ((!CurrentLocalB2Player || CurrentLocalB2Player == this)) // 咯扁辑档 LocalPlayerCharacter 八荤甫 且 鞘夸啊 乐促. 怕弊 鞍篮 版快 积己/颇鲍 矫痢捞 均哎妨辑 部老 荐 乐澜.
	//{
	//	UnsubscribeEvents();
	//}

	Super::BeginDestroy();
}

void ABladeIIPlayer::Destroyed()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::Destroyed"));

	// This route is what we can expect from tagging.
	DestroyPlayerUIManager();
	UnsubscribeEvents();

	Super::Destroyed();
}

void ABladeIIPlayer::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

#if WITH_EDITOR
	if (GIsEditor)
	{
		// 葛电 某腐磐 spawn 俊 静捞绰 海捞胶 喉风橇赴飘俊绰 扁夯蔼 府家胶甸捞 冠囚 乐绰单 捞霸 公矫窍瘤 给且 沥档肺 努 荐 乐促. 捻欧 技捞宏 矫 力芭.
		//if (IsSavingForCookingOnPreSave(TargetPlatform) && GetMesh())
		//{
		//	// 咯扁辑 null 矫挪 扒 PCClassInfo 甫 烹秦 技泼捞 登绢具.
		//	FloorRingPS = nullptr;
		//	ReadyWeaponSkillFX = nullptr;
		//	HUDInfoClass = nullptr;

		//	UE_LOG(LogBladeII, Log, TEXT("NULL out resouce properties of %s"), *this->GetPathName());
		//}
	}
#endif
}

void ABladeIIPlayer::EnableInput(APlayerController* controller)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_EnableInput);
	SubscribeEvents();
	Super::EnableInput(controller);
}

void ABladeIIPlayer::DisableInput(APlayerController* controller)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_DisableInput);
	UnsubscribeEvents();
	Super::DisableInput(controller);
}

void ABladeIIPlayer::SubscribeEvents()
{
	if (bEventsSubscribed)
		return;

	//// 父老 咯矾 Player 啊 阿磊 捞亥飘甫 罐酒具 茄促搁 Subscribe 府畔蔼阑 历厘沁促啊 Unsubscribe 矫 荤侩.
	//CAPTURE_UOBJECT(ABladeIIPlayer);

	//PlayerStartAttackTicket = PlayerStartAttackClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartAttack)
	//		Capture->StartAttackFromInput();
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerStopAttackTicket = PlayerStopAttackClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStopAttack)
	//		Capture->StopAttackFromInput();
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerStartSkill01Ticket = PlayerStartSkill01Class<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartSkill01)
	//		Capture->StartSkill01();
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerStartSkill02Ticket = PlayerStartSkill02Class<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartSkill02)
	//		Capture->StartSkill02();
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerStartSkill03Ticket = PlayerStartSkill03Class<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartSkill03)
	//		Capture->StartSkill03();
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerStartWeaponSkillTicket = PlayerStartWeaponSkillClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartWeaponSkill)
	//		Capture->StartWeaponSkill();
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerStartGuardTicket = PlayerStartGuardClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartGuard)
	//		Capture->StartGuardFromInput();
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerStopGuardTicket = PlayerStopGuardClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStopGuard)
	//		Capture->StopGuardFromInput();
	//	END_CAPTURE_OBJECT()
	//);
	//PlayerStartTagTicket = PlayerStartTagClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartTag)
	//		Capture->StartTag();
	//	END_CAPTURE_OBJECT()
	//);
	//QTEAttackTicket = QTEAttackClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(QTEAttack)
	//		Capture->StartQTE();
	//	END_CAPTURE_OBJECT()
	//);
	//PleaseForgetMeTicket = PleaseForgetMeClass<ABladeIICharacter*>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(PleaseForgetMe, ABladeIICharacter* CharAboutTobeDestroyed)
	//		Capture->OnSomeMobTermination(CharAboutTobeDestroyed);
	//	END_CAPTURE_OBJECT()
	//);

	bEventsSubscribed = true;
}

void ABladeIIPlayer::UnsubscribeEvents()
{
	if (!bEventsSubscribed)
		return;

	//PlayerStartAttackClass<>::GetInstance().Unsubscribe(PlayerStartAttackTicket);
	//PlayerStopAttackClass<>::GetInstance().Unsubscribe(PlayerStopAttackTicket);
	//PlayerStartSkill01Class<>::GetInstance().Unsubscribe(PlayerStartSkill01Ticket);
	//PlayerStartSkill02Class<>::GetInstance().Unsubscribe(PlayerStartSkill02Ticket);
	//PlayerStartSkill03Class<>::GetInstance().Unsubscribe(PlayerStartSkill03Ticket);
	//PlayerStartWeaponSkillClass<>::GetInstance().Unsubscribe(PlayerStartWeaponSkillTicket);
	//PlayerStartGuardClass<>::GetInstance().Unsubscribe(PlayerStartGuardTicket);
	//PlayerStopGuardClass<>::GetInstance().Unsubscribe(PlayerStopGuardTicket);
	//PlayerStartTagClass<>::GetInstance().Unsubscribe(PlayerStartTagTicket);
	//QTEAttackClass<>::GetInstance().Unsubscribe(QTEAttackTicket);
	//PleaseForgetMeClass<ABladeIICharacter*>::GetInstance().Unsubscribe(PleaseForgetMeTicket);

	bEventsSubscribed = false;
}

void ABladeIIPlayer::FinalizePCClassInfoSpawn(bool bFinalizeWithoutInGameInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_FinalizePCClassInfoSpawn);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::FinalizePCClassInfoSpawn"));

	if (bFinalizedByPCClassInfo == false)
	{
		ABladeIIGameMode* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
		//if (B2GameMode)
		//{
		//	// Override by B2PCClassInfo. 
		//	// BladeIIGameMode may set PCClassEnum during its own spawning operation, and it will be picked up here. 
		//	this->OverrideByPCClassInfo(B2GameMode, nullptr, bFinalizeWithoutInGameInfo);
		//				
		//	B2GameMode->FinishSpawnByPCClassInfo(); // 茄锅 荤侩登绊 唱搁 官肺 府悸

		//	bFinalizedByPCClassInfo = true;

		//	InjectGameMode(B2GameMode);
		//}
	}
}


#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
/** 肺拿 某腐磐 傈捧仿 焊沥 摹飘 */
float gLocalPCCombatStatScale = -1.0f;
#endif

void ABladeIIPlayer::InitializeCombatStats()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_InitializeCombatStats);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::InitializeCombatStats"));

	// Super::InitializeCombatStats 甫 荤侩窍瘤 臼绰促.
	
	// 咯扁急 酒捞袍捞 利侩等 巴栏肺
	// GetCurrentPlayerClass 啊 力措肺 等 蔼阑 府畔窍妨搁 OverrideByPCClassInfo 啊 贸府等 捞饶咯具 窃.
//	GetSkillRTManager();
//	GetCharacterDataStore();
//
//	if (CachedSkillRTManager == nullptr || CharacterDataStore == nullptr) //老馆利牢 敲饭捞绢啊 酒囱 焊咯林扁侩(铰府纠俊辑狼 怕弊某腐磐..)
//		return; 
//
//	LocalizedUserNickName = FText::FromString(CharacterDataStore->GetUserNickName());
//	
//	auto* BladeGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//
//	TotalAttack = BladeGameMode->GetTotalAttack(GetCharacterDataStore());
//	TotalDefense = BladeGameMode->GetTotalDefense(GetCharacterDataStore());
//
//	//辨靛 滚橇 利侩 Doc俊辑 阂矾柯促
//	auto GuildDoc = UB2UIDocHelper::GetDocGuild();
//
//	check(GuildDoc)
//
//	float GuildAttack = GuildDoc->GetAttackGuildBuff();
//	float GuildDefense = GuildDoc->GetDefenseGuildBuff();
//	float GuildHealth = GuildDoc->GetHealthGuildBuff();
//
//	GuildAttack = TotalAttack * (GuildAttack * 0.01f);
//	GuildDefense = TotalDefense * (GuildDefense * 0.01f);
//
//	// 何劝 滚橇 利侩
//	//TMap<EResurrectBuffType, int32> AppliedResurrectBuff;
//	//CachedSkillRTManager->GetAppliedResurrectionBuffValues(AppliedResurrectBuff);
//	int32* AttackBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Attack);
//	if (AttackBuffIfAny && *AttackBuffIfAny > 0)
//	{
//		TotalAttack = TotalAttack + TotalAttack * ((float)(*AttackBuffIfAny) * 0.01f);
//	}
//	int32* DefenseBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Defense);
//	if (DefenseBuffIfAny && *DefenseBuffIfAny > 0)
//	{
//		TotalDefense = TotalDefense + TotalDefense * ((float)(*DefenseBuffIfAny) * 0.01f);
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 摹飘俊 蝶弗 胶湃 焊沥
//	if (gLocalPCCombatStatScale > 0.0f && IsLocalPlayer())
//	{
//		TotalAttack *= gLocalPCCombatStatScale;
//		TotalDefense *= gLocalPCCombatStatScale;
//	}
//#endif
//
//	float AttackCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
//	float DefenseCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
//	if (GetGameMode())
//	{
//		AttackCoefficientValue = GetGameMode()->GetAttackCoefficient();
//		DefenseCoefficientValue = GetGameMode()->GetDefenseCoefficient();
//	}
//
//	AttackLevel = CombatStatEval::GetADLevel(TotalAttack, AttackCoefficientValue);
//	DefenseLevel = CombatStatEval::GetADLevel(TotalDefense / 3.f, DefenseCoefficientValue);
//
//	// Need to be change to use formula, skill/item %, etc...
//	MaxHealth = CombatStatEval::GetPCHealth(GetCurrentPlayerClass(), CharacterDataStore);
//	MaxHealth += CombatStatEval::GetPCHealthByMod(GetCurrentPlayerClass(), BladeGameMode->GetB2GameModeType(), CharacterDataStore);
//
//	// 葛靛喊 眉仿鳖瘤 钦魂 饶 扁夯 眉仿焊促 撤篮 眉仿捞 等促搁 扁夯 眉仿鳖瘤绰 焊沥秦霖促.
//	MaxHealth = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseHealth(CharacterDataStore->GetCharacterLevel(GetCurrentPlayerClass()))), MaxHealth);
//
//	GuildHealth = MaxHealth * (GuildHealth * 0.01f);			//辨靛 滚橇 啪铰
//
//	int32* HealthBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Health);
//	if (HealthBuffIfAny && *HealthBuffIfAny > 0)
//	{
//		MaxHealth = MaxHealth + MaxHealth * ((float)(*HealthBuffIfAny) * 0.01f); // 眉仿 滚橇绰 MaxHealth 俊? 酒丛 MaxHealth 焊促 Health 啊 目瘤霸?
//	}
//
//	TotalAttack += GuildAttack;
//	TotalDefense += GuildDefense;
//	MaxHealth += GuildHealth;
//	Health = MaxHealth;
//
//	// No formula for armor for now
//	Armor = MaxArmor;
//
//	// 何劝 滚橇啊 粮犁茄促搁 何劝 惑炔捞扼绰 舵捞扁 锭巩俊 何劝矫俊绰 竣胶飘扼 单捞磐甫 啊廉坷瘤 臼绰促.
//	if (AB2StageManager::GetCacheStageKeepEssentialData().IsPlayInExtraLevel()
//		&& AppliedResurrectBuff.Num() == 0)
//	{ // ExtraMap 傈券 矫 贸府 老何 咯扁档..
//		// Health 绰 酒付 愧父秦急 SkillRTManager 率俊辑档
//		AB2StageManager::GetCacheStageKeepEssentialData().SetPostExtraMapLoadB2Player(this);
//	}
//		
//	// 扁夯 傍/规/眉 捞寇狼 傈捧 可记 荐摹甸 某教.
//	InitializeCombatOptionCacheValues();
//
//	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
//	if (MovementComp)
//	{ // PCClassInfo 俊辑绰 扁夯蔼父阑 持绊 酒捞袍捞尔 胶懦 可记 殿捞 馆康等 walk speed 利侩
//		MovementComp->MaxWalkSpeed = GetStaticOptionAppliedWalkSpeed();
//	}
//
//	TotalApplyAnimRateScale(); // 捞饶俊档 AttackState 俊 蝶扼 利侩凳.
//
//	//GameMode俊 蝶弗 胶湃焊沥
//	if (BladeGameMode)
//	{
//		CharacterDamageRate *= BladeGameMode->GetDamageRateByGameModeType();
//		Health *= BladeGameMode->GetHealthRateByGameModeType();
//		MaxHealth *= BladeGameMode->GetHealthRateByGameModeType();
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 摹飘俊 蝶弗 胶湃 焊沥
//	if (gLocalPCCombatStatScale > 0.0f && IsLocalPlayer())
//	{
//		Health *= gLocalPCCombatStatScale;
//		MaxHealth *= gLocalPCCombatStatScale;
//	}
//#endif
//
//	if (CachedSkillRTManager)
//	{//Update InactivePlayerState
//		CachedSkillRTManager->InitInactiveCharacterMaxHealth(MaxHealth);
//	}
//
//	if (EtherContainer)
//		EtherContainer->InitializeContainer(this);
}


void ABladeIIPlayer::InitializeCombatOptionCacheValues()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_InitializeCombatOptionCacheValues);
	const EPCClass CurrPCClassEnum = GetCurrentPlayerClass();
	checkSlow(CharacterDataStore); // 泅犁 龋免 鸥捞怪 惑 捞吧 捞固 檬扁拳 茄 惑怕俊 静绰 吧肺.

	//// 傈捧 可记 荐摹甸 某教. 角力 利侩 矫俊 Inc** 绰 歹窍绊 Dec** 绰 蚌窃. GetEffectiveValueForOptionApplyType 曼绊.

	//BaseDamageDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecDamage, CharacterDataStore);
	//MeleeDamageDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecMeleeDamage, CharacterDataStore);
	//RangeDamageDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecRangedDamage, CharacterDataStore);
	//BossDamageDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecBossDamage, CharacterDataStore);
	//ResistCriticalRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_ResistCritical, CharacterDataStore);
	//StateDurationDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecAbnormalStateTime, CharacterDataStore);
	//DecAdditionalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecAdditionalDamage, CharacterDataStore);

	//IncDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncDamage, CharacterDataStore);
	//IncNormalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncNormalDamage, CharacterDataStore);
	//IncSkillDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Skill_IncSkillDamage, CharacterDataStore);
	//// 酒贰绰 悼老茄 ID 肺 漂沥 ActiveSkill 俊父 利侩登绰 可记甸档 乐绢辑 弊巴甸篮 漂沥 AttackState 俊 蝶扼 蔼阑 蝶肺 啊廉客辑 利侩.
	//IncCriticalRateValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncCriticalRate, CharacterDataStore);
	//IncCriticalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncCriticalDamage, CharacterDataStore);
	//IncDamageToBossValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncBossDamage, CharacterDataStore);
	//IncAdditionalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncAdditionalDamage, CharacterDataStore);

	//// 酒贰 Heal/Absorb HP 荐摹甸篮 弥措 health 蔼 措厚 雀汗 厚啦.
	//HealHPAmountRateOnCounter = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_HealHPByCountAttack, CharacterDataStore);;
	//HealHPAmountRateOnKill = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Health_ProbableRecoverHP, CharacterDataStore);
	//AbsorbHPAmountRateOnAttack = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Health_ProbableHPAbsorption, CharacterDataStore);

	//AdditionalAttackSpeedRatebyPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncAttackSpeed, CharacterDataStore);

	//// PC 努贰胶 喊肺 侥喊磊啊 备盒登绰 可记甸.
	//int32 AttackDamageByClassBaseIdx = static_cast<int32>(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToGladiator);
	//int32 ReceiveDamageByClassBaseIdx = static_cast<int32>(EUnitedCombatOptions::UCO_Defense_DecDamageByGladiator);
	//for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	//{
	//	IncAttackDamageByPassiveToClass[i] = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, static_cast<EUnitedCombatOptions>(AttackDamageByClassBaseIdx + i), CharacterDataStore);
	//	DecReceiveDamageByPassiveFromClass[i] = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, static_cast<EUnitedCombatOptions>(ReceiveDamageByClassBaseIdx + i), CharacterDataStore);
	//}

	////SetRegenerateHPRate(
	////	// 捞扒 欺季飘 酒囱 例措蔼 秒鞭烙.
	////	CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Health_PerSecRecoverHP, CharacterDataStore),
	////	false
	////);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABladeIIPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupPlayerInputComponent);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupPlayerInputComponent"));

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	InputComponent->BindAxis("MoveForward", this, &ABladeIIPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABladeIIPlayer::MoveRight);

	FInputActionBinding& FirePressed = PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABladeIIPlayer::StartAttackFromInput);
	//FirePressed.bConsumeInput = false;
	FInputActionBinding& FireReleased = PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABladeIIPlayer::StopAttackFromInput);
	//FireReleased.bConsumeInput = false;

	FInputActionBinding& SkillPressed_01 = PlayerInputComponent->BindAction("Skill_01", IE_Pressed, this, &ABladeIIPlayer::StartSkill01);
	FInputActionBinding& SkillPressed_02 = PlayerInputComponent->BindAction("Skill_02", IE_Pressed, this, &ABladeIIPlayer::StartSkill02);
	FInputActionBinding& SkillPressed_03 = PlayerInputComponent->BindAction("Skill_03", IE_Pressed, this, &ABladeIIPlayer::StartSkill03);
	FInputActionBinding& SkillPressed_Weapon = PlayerInputComponent->BindAction("Skill_Weapon", IE_Pressed, this, &ABladeIIPlayer::StartWeaponSkill);
	//FInputActionBinding& SkillReleased = InputComponent->BindAction("Skill", IE_Released, this, &ABladeIIPlayer::StopSkill);

	FInputActionBinding& TagPressed = PlayerInputComponent->BindAction("Tag", IE_Pressed, this, &ABladeIIPlayer::StartTag);

	FInputActionBinding& GuardPressed = PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &ABladeIIPlayer::StartGuardFromInput);
	FInputActionBinding& GuardReleased = PlayerInputComponent->BindAction("Guard", IE_Released, this, &ABladeIIPlayer::StopGuardFromInput);

	FInputActionBinding& QTEPressed = PlayerInputComponent->BindAction("QTE", IE_Pressed, this, &ABladeIIPlayer::StartQTEByKeyPress);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("TurnRate", this, &ABladeIICharacter::TurnAtRate);
	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//InputComponent->BindAxis("LookUpRate", this, &ABladeIICharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ABladeIIPlayer::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ABladeIIPlayer::TouchStopped);
}

bool ABladeIIPlayer::IsFineToStartAnyNewInputHandling()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsFineToStartAnyNewInputHandling);
	//ABladeIIPlayerController* B2Controller = Cast<ABladeIIPlayerController>(GetController());
	//BII_CHECK(B2Controller);
	//return (IsAlive() && B2Controller != nullptr && B2Controller->IsFineToStartAnyNewInputHandling());
	return false;
}

bool ABladeIIPlayer::IsFineToStartAnyNewMoveInputHandling()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsFineToStartAnyNewMoveInputHandling);
	if (!GetAbleToMove())
	{
		return false;
	}
	// MoveInput 父 阜绊 酵篮 版快啊 乐绢辑 眠啊 八荤
	//ABladeIIPlayerController* B2Controller = Cast<ABladeIIPlayerController>(GetController());
	//BII_CHECK(B2Controller);
	//return (IsAlive() && B2Controller != nullptr && B2Controller->IsFineToStartAnyNewMoveInputHandling());
	return false;
}

void ABladeIIPlayer::OnStartSkill(EAttackState SkillType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnStartSkill);

	// 盔贰 霓焊浚靛俊辑 秦林绰扒单 胶懦矫累且锭档 窃 秦霖促.
	// 胶懦吝 - 葫搬 - 积粮胶懦
	// 捞霸 救唱啊辑..
	//GetAnimInstance()->SetComboSkill(false);

	//ClearBuff(EBuffType::Buff_Hide);

	//if (SkillType != EAttackState::ECS_WeaponSkill && Role == ROLE_Authority)
	//	AddWeaponSkillPoint(1.0f);

	//if (SkillType == EAttackState::ECS_WeaponSkill)
	//{
	//	ClearDebuffs();

	//	// 滚橇 努府绢窜拌(葫搬, 胶畔)俊辑 碍力 霓焊浚靛肺 汲沥窍绰霸 乐促. 咯扁急 秒家.
	//	// 碍力 霓焊浚靛 汲沥秦畴搁 局丛BP俊辑 绢琶胶抛捞飘 稠栏肺 官层滚赴促. ECS_Skill04 胶懦捞 救唱皑.
	//	ConsumeComboEnd();
	//	ResetDamageNum();
	//}

	//if (SkillType == EAttackState::ECS_Skill04)
	//{
	//	if (BuffManager && BuffManager->CrowdControlCount > 0)
	//		SpawnSurviveSkillEffect();

	//	ClearDebuffs();

	//	// 滚橇 努府绢窜拌(葫搬, 胶畔)俊辑 碍力 霓焊浚靛肺 汲沥窍绰霸 乐促. 咯扁急 秒家.
	//	// 碍力 霓焊浚靛 汲沥秦畴搁 局丛BP俊辑 绢琶胶抛捞飘 稠栏肺 官层滚赴促. ECS_Skill04 胶懦捞 救唱皑.
	//	ConsumeComboEnd();
	//	ResetDamageNum();
	//
	//	// Apply Cooltime decrease.. 漂荐茄 酿鸥烙 皑家捞哥 扁鸥 酿鸥烙 皑家 可记甸篮 SkillRTManager 率俊 备泅登绢 乐促.
	//	// 泅犁 ECombatOptionApplyType 捞 Subtract_Value 牢 备泅栏肺 OneMinusMultiply 肺 官拆 荐 乐促.
	//	if (CachedSkillRTManager && Role == ROLE_Authority)
	//	{
	//		const auto& SkillOption = CachedSkillRTManager->GetCachedSkillOption();

	//		const float CooltimeDecrease = SkillOption.GetActiveSkillOptionData(SkillType, ESkillOption::ESO_DecCoolTimeOtherSkills);
	//		if (CooltimeDecrease > 0.f)
	//			CachedSkillRTManager->CountRemainingCoolTime(CooltimeDecrease);
	//	}
	//}
}

bool ABladeIIPlayer::IsQTEMounting() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsQTEMounting);
	return IsInQTEState() && QTEActor && QTEActor->IsControlledByQTEPlayer() && QTEType == EQTEType::EQT_Mount;
}

bool ABladeIIPlayer::IsQTEMountingAndReadyForAttack() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsQTEMountingAndReadyForAttack);
	// 付款飘俊辑 角力 傍拜鳖瘤 官肺 啊瓷茄 惑怕..
	return (IsQTEMounting() && this->QTEStateNum >= 1 && QTEActor->QTEStateNum >= 1);
}

void ABladeIIPlayer::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TouchStarted);
	if (IsFineToStartAnyNewInputHandling())
	{
		if (!bIsSwiping)
		{
			bIsSwiping = true;
			LastTouchLocation = Location;
			SwipeTouchIndex = FingerIndex;
		}
	}
	else
	{
		/* Cinematic 捞傈俊 喘赴 Input篮 公矫 */
		//bCachedPressedInCinematic = true;
	}
}

void ABladeIIPlayer::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TouchStopped);
	//if (FingerIndex == SwipeTouchIndex && bIsSwiping && FVector(Location - LastTouchLocation).Size() > 100.f)
	//{
	//	bIsSwiping = false;
	//	BII_CHECK(CachedSkillRTManager);
	//	if (CachedSkillRTManager && CachedSkillRTManager->IsTagAttackSuggested())
	//	{
	//		StartTag();
	//	}
	//}

	bCachedPressedInCinematic = false;
}

void ABladeIIPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABladeIIPlayer::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABladeIIPlayer::MoveForward(float Value)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_MoveForward);

	//ProcessInputMovement(Value, CameraBoom->RelativeRotation.Yaw, EAxis::X);
}

void ABladeIIPlayer::MoveRight(float Value)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_MoveRight);
	
	//ProcessInputMovement(Value, CameraBoom->RelativeRotation.Yaw, EAxis::Y);
}

bool ABladeIIPlayer::CanHandleInputMovement(float InputValue)
{
	if (InputValue == 0.0f || (!IsFineToStartAnyNewMoveInputHandling() && !IsQTEMounting()))
		return false;

	return true;
}

void ABladeIIPlayer::ProcessInputMovement(float Value, float BoomYaw, EAxis::Type StandardAxis)
{
	if (CanHandleInputMovement(Value))
	{
		// find out which way is right
		const FRotator YawRotation(0, BoomYaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(StandardAxis);

		// add movement in that direction
		if (IsQTEMounting())
		{
			QTEActor->AddMovementInput(Direction, Value, false);
		}
		else
		{
			AddMovementInput(Direction, Value);
		}

		OnInputMovement(Direction);
	}
}

void ABladeIIPlayer::OnInputMovement(const FVector& MoveDirection)
{
	//AB2AutoAIController* AutoAIController = GetAutoAIController();
	//if (AutoAIController && AutoAIController->IsRunningAI() && IsLocalPlayer())
	//{
	//	const bool InAttackMotion = IsOneOfComboAttackState(GetAnimAttackState());
	//	if (InAttackMotion && m_bAttackKeyPressed == false)
	//		StopAttack();

	//	AutoAIController->SetPCHadMovementInput(); // Let the AutoBT knows PC had input.
	//}
}

void ABladeIIPlayer::UpdateCameraPitch(float InPitch)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraPitch);
	// CameraZoom 殿狼 烙矫 瓤苞 饶 惑怕 汗盔阑 困茄 default 蔼档 某教
	CameraPitch = InPitch;
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CameraRoll));
}

void ABladeIIPlayer::UpdateCameraYaw(float InYaw)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraYaw);
	// CameraZoom 殿狼 烙矫 瓤苞 饶 惑怕 汗盔阑 困茄 default 蔼档 某教
	CameraYaw = InYaw;
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CameraRoll));
}

void ABladeIIPlayer::UpdateCameraRoll(float InRoll)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraRoll);
	// CameraZoom 殿狼 烙矫 瓤苞 饶 惑怕 汗盔阑 困茄 default 蔼档 某教
	CameraRoll = InRoll;
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CameraRoll));
}

float ABladeIIPlayer::GetCameraPitch() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraPitch);
	return CameraPitch;
}

float ABladeIIPlayer::GetCameraYaw() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraYaw);
	return CameraYaw;
}

float ABladeIIPlayer::GetCameraRoll() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraRoll);
	return CameraRoll;
}

void ABladeIIPlayer::UpdateCameraDistance(float InDistance)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraDistance);
	// CameraZoom 殿狼 烙矫 瓤苞 饶 惑怕 汗盔阑 困茄 default 蔼档 某教
	CameraLengthValueForBlending.TargetArmLength = CameraBoom->TargetArmLength = CameraDistance = InDistance;
}

void ABladeIIPlayer::UpdateCameraTransform(const FTransform& InTransform)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraTransform);
	TopDownCameraComponent->SetRelativeTransform(InTransform);
}

void ABladeIIPlayer::UpdateCameraTargetFocusOffset(const FVector& Offset)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraTargetFocusOffset);
	CameraBoom->SetRelativeLocation(Offset);
}

float ABladeIIPlayer::GetCameraDistance() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraDistance);
	return CameraDistance;
}

FVector ABladeIIPlayer::GetCameraTargetFocusOffset()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraTargetFocusOffset);
	return CameraBoom->GetRelativeTransform().GetTranslation();
}

void ABladeIIPlayer::UpdateCameraBoomLockedViewTarget(bool bLockedViewTarget)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraBoomLockedViewTarget);
	CameraBoomLockedViewTarget = bLockedViewTarget;

	//CameraBoom->bAbsoluteRotation = !bLockedViewTarget;

	CameraBoom->UpdateComponentToWorld();
}

bool ABladeIIPlayer::GetCameraBoomLockedViewTarget()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraBoomLockedViewTarget);
	return CameraBoomLockedViewTarget;
}

void ABladeIIPlayer::NotifyActorCustomEvent(FName EventName, UObject* OptionalNotifyingObject)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyActorCustomEvent);
	//if (EventName == CAMERAANIM_STOP_ACTOR_CUSTOM_EVENT)
	{
		if (--RemainCameraAnims < 0)
		{
			// Reset to original camera
			ActionCameraComponent->Deactivate();
			TopDownCameraComponent->Activate();
			CurrentCameraAnimTime = 0.f;
			RemainCameraAnims = 0;
		}
	}

	Super::NotifyActorCustomEvent(EventName, OptionalNotifyingObject);
}

void ABladeIIPlayer::StartAttackFromInput()
{
	m_bAttackKeyPressed = true;
	if(CurrentMoveAttackState != EAutoMoveAttackState::AutoMoveToTarget)
		StartAttack();
}

void ABladeIIPlayer::StopAttackFromInput()
{
	m_bAttackKeyPressed = false;
	StopAttack();
}

void ABladeIIPlayer::StartGuardFromInput()
{
	if (m_bGuardKeyPressed)
		return;

	m_bGuardKeyPressed = true;
	StartGuard(true);
}

void ABladeIIPlayer::StopGuardFromInput()
{
	m_bGuardKeyPressed = false;
	StopGuard();
}

void ABladeIIPlayer::StartAttack()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAttack);
    //	B2_SCOPED_TRACK_LOG( FString::Printf(TEXT("ABladeIIPlayer::StartAttack, Current AttackState %d"), static_cast<int32>(GetAttackState())) );

	SetKeepAttacking(true);

	if (!IsFineToStartAnyNewInputHandling() || !GetAbleToControl())
		return;

	// GuardBreak 葛记吝俊绰 救凳
	if (InGuardBreak())
		return;

	StartAttackExecute();
}

void ABladeIIPlayer::StartAttackExecute()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAttackExecute);
	if (!bEnableNormalAttack)
		return;

	if (IsInQTEState())
	{
		if (IsQTEMounting())
		{
			bPendingQTENext = true; // Handled by OnQTENext
			bNeedUpdateTarget = true;
			if (QTEStateNum == 1)
			{
				OnQTENext();
			}
		}
	}
	else
	{
		if (!GetSignalAttack())
		{
			bNeedUpdateTarget = true;
		}

		SetSignalAttack(true);
	}

	RotateToTarget(0.f, true);

	ClearBuff(EBuffType::Buff_Hide);
}

void ABladeIIPlayer::StopAttack()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopAttack);

	if (m_bAttackKeyPressed && CurrentMoveAttackState != EAutoMoveAttackState::AutoMoveToTarget)
		return;

	SetKeepAttacking(false);
	SetSignalAttack(false);
}

void ABladeIIPlayer::StopRepeatingInput(bool bFromInput/* = false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopRepeatingInput);
	if (bFromInput)
	{
		StopAttackFromInput();
		StopGuardFromInput();
	}
	else
	{
		StopAttack();
		StopGuard();
	}	
}

void ABladeIIPlayer::ResetSkillCooltime()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAnySkill);
	BII_CHECK(CachedSkillRTManager);
	//if (CachedSkillRTManager)
	//{
	//	CachedSkillRTManager->ResetSkillCoolTime();
	//}
}

void ABladeIIPlayer::StartAnySkill()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAnySkill);
	BII_CHECK(CachedSkillRTManager);

	//const bool CanStartSkill = CachedSkillRTManager && TargetActor != nullptr;
	//if (CanStartSkill)
	//{
	//	const int32 SuitableSkillIdx = GetHighPrioritySkillIndex();

	//	if (SuitableSkillIdx != INDEX_NONE)
	//	{
	//		const float SkillShootingRange = CachedSkillRTManager->GetSkillShootingRangeForAI(this, SuitableSkillIdx);
	//		const float DistToTarget = GetTargetLocationVectorFromPlayer(TargetActor).Size();

	//		if (DistToTarget > SkillShootingRange)
	//			StartAttack();

	//		else if (SuitableSkillIdx != INDEX_NONE)
	//			StartSkillInner(SuitableSkillIdx, false);
	//	}
	//}
}

void ABladeIIPlayer::StartSkill01()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartSkill01);
	StartSkillInner(0, true);
}

void ABladeIIPlayer::StartSkill02()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartSkill02);
	StartSkillInner(1, true);
}

void ABladeIIPlayer::StartSkill03()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartSkill03);
	StartSkillInner(2, true);
}

void ABladeIIPlayer::StartWeaponSkill()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartWeaponSkill);
	//StartSkillInner(WEAPON_6STAR_SKILL_INDEX, true);
	//bOverUnityPoint = true;
}

void ABladeIIPlayer::AddWeaponSkillPoint(float ToAddPoint)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_AddWeaponSkillPoint);
	SetWeaponSkillPoint(WeaponSkillPoint + ToAddPoint);	
}

void ABladeIIPlayer::SetWeaponSkillPoint(float ToSetPoint)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetWeaponSkillPoint);

	if (!IsAlive())
		return;

	float MaxWeaponSkillPoint = 0.0f;
	//GetSkillRTManager();
	//if (CachedSkillRTManager)
	//{
	//	MaxWeaponSkillPoint = CachedSkillRTManager->GetActiveCharacterMaxWeaponSkillPoint();
	//	WeaponSkillPoint = FMath::Clamp(ToSetPoint, 0.0f, MaxWeaponSkillPoint);
	//	
	//	if (WeaponSkillPoint == MaxWeaponSkillPoint)
	//	{
	//		bOverUnityPoint = false;
	//	}

	//	CachedSkillRTManager->UpdateWeaponSkillPoint(WeaponSkillPoint);
	//}

	AttachReadyWeaponSkillFX(MaxWeaponSkillPoint == WeaponSkillPoint);
}

void ABladeIIPlayer::SetWeaponSkillPointMax()
{
	//if (CachedSkillRTManager)
		//SetWeaponSkillPoint(CachedSkillRTManager->GetActiveCharacterMaxWeaponSkillPoint());
}

bool ABladeIIPlayer::IsWeaponSkillPointMax()
{
	//if (CachedSkillRTManager)
	//	return GetWeaponSkillPoint() == CachedSkillRTManager->GetActiveCharacterMaxWeaponSkillPoint();

	return false;
}

void ABladeIIPlayer::AttachReadyWeaponSkillFX(bool bAttach)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_AttachReadyWeaponSkillFX);
	//if (CachedSkillRTManager)
	//{
	//	EAttackState CurrentInputSkill = CachedSkillRTManager->GetSkillOfInputIndex(WEAPON_6STAR_SKILL_INDEX);
	//	if (CurrentInputSkill != EAttackState::ECS_WeaponSkill)
	//		bAttach = false;
	//}


	//if (bAttach)
	//{
	//	if (ReadyWeaponSkillPSC == nullptr)
	//	{
	//		ReadyWeaponSkillPSC = NewObject<UParticleSystemComponent>(this, TEXT("ReadyWeaponSkillPSC"), RF_Transient);
	//		ReadyWeaponSkillPSC->SecondsBeforeInactive = 0.f;
	//		ReadyWeaponSkillPSC->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//		ReadyWeaponSkillPSC->SetTemplate(ReadyWeaponSkillFX);
	//		ReadyWeaponSkillPSC->RegisterComponent();
	//	}
	//	
	//	ReadyWeaponSkillPSC->Activate();
	//}
	//else if (ReadyWeaponSkillPSC && !bAttach)
	//{
	//	ReadyWeaponSkillPSC->Deactivate();
	//}
}

EAttackState ABladeIIPlayer::StartSkillInner(int32 SkillInputIndex, bool IsPlayerInput)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartSkillInner);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartSkillInner, Current AttackState %d"), static_cast<int32>(GetAttackState())));

	BII_CHECK(CachedSkillRTManager);
//
//	if (!IsFineToStartAnyNewInputHandling() || !CachedSkillRTManager)
//	{
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Skill - IsFineToStartAnyNewInputHandling false"));
//#endif
//		return EAttackState::ECS_None;
//	}
//
//	EAttackState CurrentInputSkill = CachedSkillRTManager->GetSkillOfInputIndex(SkillInputIndex);
//	if (IsAvailableStartSkill(CurrentInputSkill) == false)
//		return EAttackState::ECS_None;
//
//	EAttackState FinalAttackState = CachedSkillRTManager->GetStartableSkill(SkillInputIndex, IsPlayerInput);
//	if (FinalAttackState == EAttackState::ECS_None)
//	{
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Skill Can't Start- StartSkillByInputIndex Failed"));
//#endif
//		return EAttackState::ECS_None;
//	}
//	
//	SetAttackState(FinalAttackState); // Does not actually start by StartSkillByInputIndex call, must set the return value here.
//	CurrentSkillAnimIdx = CachedSkillRTManager->GetSkillAnimIndex(SkillInputIndex); // AnimBP 俊辑 荤侩窍电瘤, GetAnimStateOverrideSequence 俊辑 荤侩窍电瘤.
//	bLastInvokedAnySkillAttack = true;
//
//	OnStartSkill(FinalAttackState);
//	SetReservedSkill(FinalAttackState);
//
//	if (SkillInputIndex == WEAPON_6STAR_SKILL_INDEX)
//	{
//		if (SkillLOD == 0)
//		{ // 敲饭捞绢甫 啊鳖捞辑 努肺令诀 窍骨肺 LOD 殿 楷免侩 悸诀甸
//			OnBeginWeaponSkillLOD0();
//		}
//
//		SetWeaponSkillPoint(0);
//	}
//
//	ForceFeedback();
//
//	// 霸烙葛靛俊 舅覆(府葛飘 敲饭捞绢绰 咯扁 救沤)
//	GetGameMode()->NotifyPlayerStartSkill(this, FinalAttackState);

	//return FinalAttackState;
	return EAttackState::ECS_None;
}
#if !UE_BUILD_SHIPPING
// StartSkillInner 俊辑 鞘夸茄 巴甸 惯瞄. 摹飘 扁瓷捞聪 赣府甫 零绢垛绢啊搁辑 度鞍捞 嘎免 鞘夸鳖柳 绝绊.. 窍搁 亮绊..
void ABladeIIPlayer::DevTestStartWeaponSkill(ESkillAnimType WantedAnimType)
{
	if (WantedAnimType >= ESkillAnimType::ESA_Weapon_01 && WantedAnimType <= ESkillAnimType::ESA_Weapon_05_LOD)
	{
		SetAttackState(EAttackState::ECS_WeaponSkill);

		CurrentSkillAnimIdx = WantedAnimType;
		bLastInvokedAnySkillAttack = true;

		OnStartSkill(EAttackState::ECS_WeaponSkill);
		SetReservedSkill(EAttackState::ECS_WeaponSkill);

		if (SkillLOD == 0)
		{
			OnBeginWeaponSkillLOD0();
		}

		ForceFeedback();

		// 霸烙葛靛俊 舅覆(府葛飘 敲饭捞绢绰 咯扁 救沤)
		//GetGameMode()->NotifyPlayerStartSkill(this, EAttackState::ECS_WeaponSkill);
	}
}
#endif

bool ABladeIIPlayer::IsAvailableStartSkill(EAttackState InAttackState)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsAvailableStartSkill);
	if (IsInQTEState())
	{
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- IsInQTEState"));
#endif
		return false; // For now
	}

	// 葫搬 抗寇贸府
	if (IsBuffActive(EBuffType::Debuff_Freezing) && InAttackState == EAttackState::ECS_Skill04)
	{
		return true;
	}

	// 扁夯利栏肺 傍拜吝捞唱 馆拜吝, 惑怕捞惑吝俊绰 胶懦 荤侩捞 阂啊瓷
	if (IsSkillAttacking())
	{
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- IsSkillAttacking"));
#endif
		return false;
	} 
	if (IsCounterAttacking())
	{
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- IsCounterAttacking"));
#endif
		return false;

	}
	if (GetReservedSkill() != EAttackState::ECS_None)
	{
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- HasReservedSkill"));
#endif
		return false;

	} 

	// 乘促款 or 剐覆吝俊档 阂啊瓷
	if (HadHugeDamaged())
	{
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- HadHugeDamaged"));
#endif
		return false;
	}


	// 胶畔, 葫搬篮 扁夯利栏肺 阂啊瓷窍唱
	bool bMovableDebuff = IsStunned() || IsFrozen();
	if (bMovableDebuff)
	{
		if (InAttackState == EAttackState::ECS_Skill04 || InAttackState == EAttackState::ECS_WeaponSkill) // 积粮胶懦篮 荤侩 啊瓷
			return true;

#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- InMoveableDebuff"));
#endif
		return false;
	}

	if (GetSpawnMotionState() != EPCSpawnMotionState::EPST_Normal)
		return false;

	return true;
}

int32 ABladeIIPlayer::GetHighPrioritySkillIndex()
{
	//for (int32 PriorityIdx = 0; PriorityIdx < MAX_SKILL_INPUT; PriorityIdx++)
	//{
	//	if (AutoSkillPriority.IsValidIndex(PriorityIdx))
	//	{
	//		const int32 SkillIndex = AutoSkillPriority[PriorityIdx];
	//		//if (CachedSkillRTManager->GetStartableSkill(SkillIndex, false) != EAttackState::ECS_None)
	//		//	return SkillIndex;
	//	}
	//}

	return INDEX_NONE;
}

void ABladeIIPlayer::StartTag()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartTag);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartTag, Current AttackState %d"), static_cast<int32>(GetAttackState())));

	//check(CachedSkillRTManager);

	//if (CanTag() == false)
	//	return;

	//// 绢瞒乔 IsTagPossible 捞 酒聪扼搁 滚瓢捞 劝己拳登瘤 富酒具 窃.
	//if (CachedSkillRTManager == NULL || CachedSkillRTManager->IsTagPossible() == false || !IsFineToStartAnyNewInputHandling())
	//	return;

	//if (CachedSkillRTManager->IsTagPossibleButDeferred())
	//{
	//	bPendingTag = true; // 寸厘 啊瓷窍瘤 臼促搁 (傈捧 悼累 吝) 抗距父 茄促.
	//}
	//else
	//{
	//	StartTagCommon();
	//}
}

void ABladeIIPlayer::StartTagCommon()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartTagCommon);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartTagCommon, Current AttackState %d"), static_cast<int32>(GetAttackState())));

	if (CanTag() == false)
		return;

	if (IsLocalPlayer())
	{
		auto* DocBattle = GetDocBattle();
		if (DocBattle)
		{// 肺拿 敲饭捞绢 某腐磐狼 楷拌 怕弊 咯何甫 UIDoc 俊 技泼. 怕弊 寸矫俊 UI 率俊辑 曼绊甫 窍绊 怕弊 饶 府悸瞪 巴.
			//DocBattle->SetbTagAttackPending(CachedSkillRTManager && CachedSkillRTManager->IsTagAttackSuggested());
		}

		// 怕弊秦档 俊抛福 酿鸥烙 蜡瘤
		/*if (UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass()))
		{
			UB2EtherSetEffect* OffenseEtherObject = GetEtherSetEffect(EEtherSetType::Offense);
			if (OffenseEtherObject)
			{
				float OffenseCoolTime = OffenseEtherObject->GetCurrentCoolTime();
				DocHero->SetCurOffenseEtherCoolTime(OffenseCoolTime);
			}
			UB2EtherSetEffect* DefenseEtherObject = GetEtherSetEffect(EEtherSetType::Defense);
			if (DefenseEtherObject)
			{
				float DefenseCoolTime = DefenseEtherObject->GetCurrentCoolTime();
				DocHero->SetCurDefenseEtherCoolTime(DefenseCoolTime);
			}
		}*/
	}

	if (TagAttackMode > ETagAttackMode::EAM_Disable)
	{
		bTagAttackTriggered = true;
		AddWeaponSkillPoint(1.0f);
	}

	// 咯扁辑 技泼窍搁 AnimBlueprint, Animation, B2TagNotify 芭媚辑 UB2SkillRTManager::TagToOther 肺 哎 巴.
	SetAttackState(EAttackState::ECS_TagOtherChar);
	bLastInvokedAnySkillAttack = true;

	SetReservedSkill(EAttackState::ECS_TagOtherChar);
}

void ABladeIIPlayer::ExecutePendingTag()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ExecutePendingTag);
	//check(CachedSkillRTManager);
	//if (bPendingTag && CachedSkillRTManager->IsTagPossibleRightNow())
	//{ // 捞 版快 傍拜 档吝俊 tag 夸没阑 沁阑 巴.
	//	StartTagCommon();
	//	bPendingTag = false;
	//}
}

bool ABladeIIPlayer::InGuardBreak()
{
	return LastGuardBreakType != EGuardBreakLogicType::EGLT_AbleToGuard;
}

bool ABladeIIPlayer::IsGuardStartable()
{
	if (!CanEvade)
		return false;

	if (!IsFineToStartAnyNewInputHandling())
		return false;

	if (!GetAbleToControl())
		return false;

	if (IsInQTEState() || GetAttackState() == EAttackState::ECS_Evade) // || DamagedNum != 0)
		return false; // For now

	if (InGuardBreak())
		return false;

	if (GetReservedSkill() != EAttackState::ECS_None)
		return false;

	return true;
}

// bFromInput 平烹秦辑 甸绢柯芭绰 备福扁唱 馆拜鸥捞赣 眉农 救且妨备 眠啊
void ABladeIIPlayer::StartGuard(bool bFromInput/* = false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartGuard);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartGuard, Current AttackState %d"), static_cast<int32>(GetAttackState())));

	BII_CHECK(GetWorld());


	//const float PrevGuardTime = LastGuardTime;
	//if (bFromInput)
	//	LastGuardTime = GetWorld()->GetTimeSeconds();

	//if (IsGuardStartable() == false)
	//	return;

	//const float GuardDelta = LastGuardTime - PrevGuardTime;
	//const bool IsEvadeInput = bFromInput && PrevGuardTime != -1.f;
	//const bool IsEvadeTime = GuardDelta > 0.f && GuardDelta < EvadeCheckTime;

	//if (IsEvadeInput && IsEvadeTime)
	//{
	//	const bool IsCancelableState = IsSkillAttacking() || IsCounterAttacking();
	//	if (IsCancelableState)
	//	{
	//		bSkillCancel = true;
	//	}
	//	else
	//	{
	//		bNeedUpdateTarget = true;

	//		const auto CanEvadeState = GetAttackState() == EAttackState::ECS_None || GetAttackState() == EAttackState::ECS_GuardEnd;
	//		if (CanEvadeState)
	//		{
	//			SetAttackState(EAttackState::ECS_Evade);
	//			LastGuardTime = -1.f;
	//		}
	//	}
	//}

	//else
	//{
	//	if (auto* AnimInst = GetAnimInstance())
	//	{
	//		const auto AnimAttackState = AnimInst->GetAnimAttackState();
	//		const bool IsGuardableState = (AnimAttackState >= EAttackState::ECS_None && AnimAttackState <= EAttackState::ECS_Combo06);
	//		const bool IsInDamageAnim = AnimInst->IsDamaged() && AnimInst->GetDamageAnimIndex() > EAttackType::EAT_Default;

	//		if (IsInDamageAnim == false && (IsGuardableState || GetAttackState() == EAttackState::ECS_GuardEnd))
	//		{
	//			SetAttackState(EAttackState::ECS_GuardStart);
	//		}
	//	}
	//}
}

void ABladeIIPlayer::StopGuard(bool bForce/* = false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopGuard);

	if (m_bGuardKeyPressed && !bForce)
		return;

	// Don't try guard during skill attack, to get wanted comboend notify 
	if ((IsGuarding() || GetAttackState() == EAttackState::ECS_GuardStart) && GetReservedSkill() == EAttackState::ECS_None)
	{
		SetAttackState(EAttackState::ECS_GuardEnd);
		//::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);
	}
}

void ABladeIIPlayer::StartQTEByKeyPress()
{
	// 滚瓢UI乐栏搁 弊芭烹秦辑 角青
	//UB2UIBattleMain* pBattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain);
	//UB2UIBattleQTE* CreatedQTEUIP = pBattleMainUI ? pBattleMainUI->GetCreatedQTEUIP() : nullptr;
	//	
	//if (CreatedQTEUIP)
	//	CreatedQTEUIP->OnPressedBtnArea();
	//else
	//	StartQTE();
}

void ABladeIIPlayer::StartQTE(bool bForce/* = false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartQTE);
	if (!bForce && !IsQTEStartAble())
	{
		return;
	}

	if (IsInQTEState())
	{
		// QTE Next (QTE 惑怕俊辑 甸绢啊绰 2瞒 QTE 狼 版快)
		if (GetQTEEnabled() && QTEActor->IsAlive())
		{
			SetQTEEnabled(false);
			bPendingQTENext = true;
		}
		return;
	}

	// Find QTE Enabled Target
	bNeedUpdateTarget = false;
	UpdateTarget(GetLastMovementInputVector() != FVector::ZeroVector && bDisableRotateToInput, true);

	// Only in the control mode And Starting QTE situation, apply a safe QTE distance.
	bool bUseSafeEnableDistance = GetUseQTESafeEnebleDistance();
	ABladeIICharacter* pQTETargetActor = FindQTEEnableTarget(bUseSafeEnableDistance);

	if (pQTETargetActor && pQTETargetActor->IsAlive() && pQTETargetActor->GetDamagedNum() == 0 && pQTETargetActor->QTEType != EQTEType::EQT_Break)
	{
		SetAttackState(EAttackState::ECS_None);
		// Start QTE Dash
		StopRepeatingInput();
		// QTE Dash 辆丰 矫 QTEAttack 捞 True 肺 官差绢具 窍绰单 弊 傈俊 固府 True 肺 登绢乐促搁 False 肺 官槽促.
		//if (pQTETargetActor->QTEType == EQTEType::EQT_Mount)
		//{
		//	if (auto* AnimInst = GetAnimInstance())
		//	{
		//		AnimInst->SetQTEAttack(false);
		//	}
		//}
		SetQTEStateFlag(true);
		bQTERun = false;
		QTEStateNum = 0;
		pQTETargetActor->QTEPCClassEnum = GetCurrentPlayerClass();
		FVector Diff = GetTargetLocationVectorFromPlayer(pQTETargetActor, false);
		float ToTargetYaw = FRotator::ClampAxis(FMath::Atan2(Diff.Y, Diff.X) * 180.f / PI);
		SetActorRotation(FRotator(0.f, ToTargetYaw, 0.f));
		if (pQTETargetActor->QTEType == EQTEType::EQT_Mount)
		{
			float fYawForQTEMount = ToTargetYaw - 180.0f;
			fYawForQTEMount = fYawForQTEMount < 0.0f ? fYawForQTEMount + 360.0f : fYawForQTEMount;
			pQTETargetActor->SetActorRotation(FRotator(0.f, fYawForQTEMount, 0.f));
		}
		else
		{
			pQTETargetActor->SetActorRotation(FRotator(0.f, ToTargetYaw, 0.f));
		}

		// Start mount timer count
		if (pQTETargetActor->QTEType == EQTEType::EQT_Mount)
		{
			OnQTEMountBegin();
		}

		QTEActor = pQTETargetActor;
		QTENPCClass = pQTETargetActor->GetNPCClass();
		pQTETargetActor = NULL;
	}
}

void ABladeIIPlayer::OnQTENext()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTENext);
	//if ((bPendingQTENext || (QTEType == EQTEType::EQT_Mount && QTEStateNum == 0)) && IsInQTEState() && QTEActor && QTEActor->IsAlive())
	//{
	//	// EQT_Mount 鸥捞赣 倒府扁矫累窍绰 矫痢. 霸烙葛靛俊 舅覆
	//	if (QTEType == EQTEType::EQT_Mount && QTEStateNum == 0)
	//	{
	//		// UI绰 QTE角青磊啊 肺拿敲饭捞绢老锭父 盎脚
	//		if (IsLocalPlayer())
	//		{
	//			auto* DocBattle = GetDocBattle();
	//			if (DocBattle)
	//			{

	//				const float FinalMountTime = FMath::Max(5.f, QTEActor->QTEMountTime);
	//				const float InvalidMountTIme = -1.0f;
	//				QTEMountRemainingTime = QTEActor->GetQTELimitEnabled() ? InvalidMountTIme : FinalMountTime;

	//				DocBattle->SetMaxQTEMountTime(FinalMountTime); // MaxQTEMountTIme 篮 函版 妮归阑 罐瘤 臼阑 巴捞骨肺 捞吧 刚历 技泼.
	//				DocBattle->SetRemainingQTEMountTime(QTEMountRemainingTime);
	//				DocBattle->SetbIsQTEMounted(true);
	//			}
	//		}

	//		GetWorldTimerManager().SetTimer(ReserveQTEActorReportMyDeathToBirthplaceTimerHandle, this, &ABladeIIPlayer::ReserveQTEActorReportMyDeathToBirthplace, 0.2f, false);
	//		QTEActor->OnMountStartComplete();
	//		
	//		ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	//		// 付款飘俊辑 酒捞甸肺 官布促.
	//		// CountQTEMountTime() 焊搁 酒捞甸捞惑俊辑 鸥捞赣 皑家矫糯
	//		if (pGM)
	//			pGM->NotifyQTEMountBeginToIdle(this);
	//	}

	//	bPendingQTENext = false;
	//	QTEStateNum++;
	//	QTEActor->QTEStateNum++;
	//	
	//	// QTE mount 俊 角力 傍拜鳖瘤 啊瓷 咯何啊 函版瞪 荐 乐绰 鸥捞怪栏肺 埃林.
	//	auto* DocBattle = GetDocBattle();
	//	if (DocBattle)
	//	{
	//		DocBattle->SetbIsQTEMountedAndReadyForAttack(this->IsQTEMountingAndReadyForAttack());
	//	}
	//}
}

void ABladeIIPlayer::OnQTEAttackEnd()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTEAttackEnd);
	if (QTEType == EQTEType::EQT_Mount)
	{
		// QTE Attack mode during mount, need to return idle
		QTEStateNum = 1;
		QTEActor->QTEStateNum = 1;
	}
}

bool ABladeIIPlayer::GetQTEInputEnableReady() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetQTEInputEnableReady);
	return (IsQTEMounting() && QTEStateNum > 1);
}

void ABladeIIPlayer::SetQTEState()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetQTEState);
	SetQTEEnabled(false);
	if (IsInQTEState() && QTEActor && QTEActor->IsAlive())
	{
		QTEActor->SetQTEState();
		QTEType = QTEActor->QTEType;
	}
}

void ABladeIIPlayer::SetQTEStateFlag(bool bInState)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetQTEStateFlag);
	Super::SetQTEStateFlag(bInState); // 窜鉴 蔼 技泼.

	auto* DocBattle = GetDocBattle();

	if (IsLocalPlayer() && DocBattle)
	{ // UIDoc 俊 惑怕蔼 傈崔秦 林妨绊 蝶肺 捞犯霸 父电 芭.
		//DocBattle->SetbIsPlayerInQTEState(IsInQTEState());
	}
}

void ABladeIIPlayer::ForcedChangeQTEStepMount()
{
	StartQTE();
	//if (QTEActor && QTEActor->QTEType == EQTEType::EQT_Mount)
	//{
	//	OnQTEBegin();
	//	// 秦寸 肺流阑 鸥瘤 臼绰 扁粮 BP ( AnimNotify_QTEDashEnd ) 客 悼老窍霸 利侩
	//	if (auto* AnimInst = GetAnimInstance())
	//	{
	//		AnimInst->SetQTEAttack(true);
	//	}
	//	SetQTEState();
	//}
}

bool ABladeIIPlayer::IsTimingForSuccessiveQTE() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsTimingForSuccessiveQTE);
	// 捞固 QTE 牢 惑怕俊辑..
	return (QTEActor && QTEActor->IsInQTEState() && this->IsInQTEState() && this->GetQTEEnabled());
}

void ABladeIIPlayer::OnEvadeEnd()
{
	//// 酿倒赴促. 鞘夸窍搁. 霸烙葛靛俊蝶扼 促抚.
	//if (GetGameMode()->GetGameRule()->IsEvadeCoolTimeCheck() && EvadeTimer == 0.0f)
	//	SetEvadeTimer();
}

void ABladeIIPlayer::OnQTEBegin()
{
	//if (QTEActor)
	//{
	//	AAIController* TargetController = Cast<AAIController>(QTEActor->GetController());
	//	if (TargetController && TargetController->BrainComponent)
	//		TargetController->BrainComponent->StopLogic(FString(TEXT("QTE")));
	//	
	//	// Remove AI during QTE
	//	QTEActor->SetSignalAttack(false);
	//	QTEActor->HideFloatingHPBar(true); // 磷篮 格见捞唱 促抚绝栏聪 HP 官绰 绝矩促.

	//	// Start mount timer count
	//	if (QTEActor->QTEType == EQTEType::EQT_Mount)
	//	{
	//		// 评沥焊 牢拌
	//		QTEActor->SetTeamNum(GetTeamNum());

	//		ECollisionChannel PlayerCollisionChannel = GetCapsuleComponent()->GetCollisionObjectType();
	//		QTEActor->GetCapsuleComponent()->SetCollisionObjectType(PlayerCollisionChannel);
	//	}
	//}
}

void ABladeIIPlayer::StartDash(float InDashDuration, float InMaxDistance)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartDash);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartDash, Current AttackState %d"), static_cast<int32>(GetAttackState())));

	if (!IsFineToStartAnyNewInputHandling())
		return;
	
	//TotalDashDuration = InDashDuration;
	//CurrentDashDuration = InDashDuration;
	//MaxDashDistance = InMaxDistance;
	//GetCharacterMovement()->bApplyOptionalVelocityDuringRootmotion = true;
	//GetCharacterMovement()->OptionalVelocity = FVector::ZeroVector;
	//GetCharacterMovement()->OptionalVelocityBlend = 0.f;
}

void ABladeIIPlayer::StopDash()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopDash);
	//TotalDashDuration = -1.f;
	//CurrentDashDuration = -1.f;
	//GetCharacterMovement()->bApplyOptionalVelocityDuringRootmotion = false;
	//GetCharacterMovement()->OptionalVelocity = FVector::ZeroVector;
	//GetCharacterMovement()->OptionalVelocityBlend = 0.f;
}

void ABladeIIPlayer::SetAttackState(EAttackState InAttackState)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetAttackState);
	bool bGuardStart = InAttackState == EAttackState::ECS_GuardStart && InAttackState != GetAttackState();

	Super::SetAttackState(InAttackState);

	TotalApplyAnimRateScale(); // 林肺 AttackState 俊 蝶扼 利侩 咯何啊 崔扼瘤骨肺 SetAttackState 俊辑 窍搁 利例且 淀.

	//if (bGuardStart)
	//	PlayerGuardStartedClass<>::GetInstance().Signal();
}

FVector ABladeIIPlayer::GetDirectionToTargetCharacter()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDirectionToTargetCharacter);
	if (TargetActor)
	{
		FVector ToTarget = TargetActor->GetActorLocation() - this->GetActorLocation();
		ToTarget.Normalize();
		return ToTarget;
	}
	else if (TargetObject)
	{
		FVector ToTarget = TargetObject->GetActorLocation() - this->GetActorLocation();
		ToTarget.Normalize();
		return ToTarget;
	}
	return FVector(1.0f, 0.0f, 0.0f);
}

FVector ABladeIIPlayer::GetDirectionToTargetCharacter(FVector& BoneLocation)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDirectionToTargetCharacter_BoneLocation);
	if (TargetActor)
	{
		FVector ToTarget = TargetActor->GetActorLocation() - BoneLocation;
		ToTarget.Normalize();
		return ToTarget;
	}
	else if (TargetObject)
	{
		FVector ToTarget = TargetObject->GetActorLocation() - BoneLocation;
		ToTarget.Normalize();
		return ToTarget;
	}
	return FVector(1.0f, 0.0f, 0.0f);
}

UAnimSequenceBase* ABladeIIPlayer::GetAnimStateOverrideSequence()
{
	//
	// Animation Blueprint 俊辑 AllowSequenceDynamicChange 甫 眉农茄 局丛 state 肺 捞傈 矫 阂府霸 凳.
	// 咯扁辑 霸烙 惑怕俊 蝶扼 漂沥 AnimSequence 甫 府畔窍搁 AnimBP 狼 泅犁 state 绰 盔贰 硅摹等 局聪皋捞记捞 酒囱 咯扁辑 府畔茄 局聪皋捞记阑 敲饭捞窍霸 瞪 巴.
	// NULL 府畔 矫 扁夯蔼 敲饭捞
	//

	//ESkillAnimType FinalCurrentSkillAnimType = GetCurrentSKillAnimIdx(); // CurrentSkillAnimIdx 富绊 GetCurrentSKillAnimIdx 甫 结具
	//if (IsSkillAttacking() && FinalCurrentSkillAnimType != ESkillAnimType::ESA_End)
	//{ // AnimBP 啊 酒囱 喊档 抛捞喉俊 殿废等 skill 局聪皋捞记阑 阂矾甸咯辑 AnimBP 俊 冈咯淋.
	//	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	//	UB2SkillAnimInfo* SkillAnimInfo = B2GI ? B2GI->GetSkillAnimInfo() : nullptr;
	//	UAnimSequenceBase* WantedSkillAnim = SkillAnimInfo ? SkillAnimInfo->GetSingleSkillAnim(GetCurrentPlayerClass(), FinalCurrentSkillAnimType) : nullptr;
	//	return WantedSkillAnim;
	//}

	return nullptr;
}

float ABladeIIPlayer::GetTargetDistance() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetTargetDistance);
	if (TargetActor)
		return GetTargetDistanceInTarget(TargetActor);

	return -1.f;
}

int32 ABladeIIPlayer::GetCounterMotionIncrease() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCounterMotionIncrease);
	return CachedSkillRTManager ? CachedSkillRTManager->GetCounterMotionInc() : 0;
}

void ABladeIIPlayer::ForceFeedback() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ForceFeedback);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::ForceFeedback"));

	ABladeIIPlayerController* PC = Cast<ABladeIIPlayerController>(GetController());
	if (PC && IsPlayerControlled())
	{
		if (IsVibration())
		{
			PC->ClientPlayForceFeedback(ForceFeedbackSetting);
		}
	}
}

void ABladeIIPlayer::StopForceFeedback() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopForceFeedback);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::StopForceFeedback"));

	ABladeIIPlayerController* PC = Cast<ABladeIIPlayerController>(GetController());
	if (PC && IsPlayerControlled())
	{
		if (IsVibration())
		{
			PC->ClientStopForceFeedback(ForceFeedbackSetting, FName());
		}
	}
}

bool ABladeIIPlayer::GetHadInput() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetHadInput);

	bool bMoveInput = false;

	if (IsQTEMounting())
		bMoveInput = QTEActor->GetLastMovementInputVector() != FVector::ZeroVector;
	else
		bMoveInput = GetLastMovementInputVector() != FVector::ZeroVector;

	return bMoveInput || m_bGuardKeyPressed || m_bAttackKeyPressed;
}

bool ABladeIIPlayer::IsTagAttackEnabled() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsTagAttackEnabled);
	if (IsSkillAttacking())
	{
		return true;
	}

	//if (DamagedNum > 0 && CachedSkillRTManager)
	//{
	//	if (LastDamageType == EAttackType::EAT_KnockBack && CachedSkillRTManager->GetPassiveSkillOptionValue(GetCurrentPlayerClass(), EPassiveType::EPassiveType_Tag, ESkillOption::ESO_EnableTagOnKnockBack) > 0.f)
	//	{
	//		return true;
	//	}
	//	if (LastDamageType == EAttackType::EAT_BigBounce && CachedSkillRTManager->GetPassiveSkillOptionValue(GetCurrentPlayerClass(), EPassiveType::EPassiveType_Tag, ESkillOption::ESO_EnableTagOnBigBounce) > 0.f)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

void ABladeIIPlayer::SetKeepAttacking(bool bKeep)
{
	bKeepAttacking = bKeep;
}

bool ABladeIIPlayer::HadTagDisabledDamage() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_HadTagDisabledDamage);
	// Prevent tag during KnockBack and BigBounce
	
	const bool IsDamaged = DamagedNum > 0;
	const bool HasHugeDamage = IsDamaged && IsInHugeDamage();

	const bool IsAbnormal = IsFrozen() || IsStunned();

	if (IsAbnormal || HasHugeDamage)
		return true;

	return false;
}

void ABladeIIPlayer::SetTagEnable(bool IsEnable)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetTagEnable);
	bIsTagPossible = IsEnable;
}

bool ABladeIIPlayer::CanTag()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CanTag);	

	//if (Role == ROLE_SimulatedProxy)
	//	return true;

	//if (GetGameMode()->IsAllowTag() == false || bIsTagPossible == false)
	//	return false;

	if (IsInQTEState())
		return false;

	if (HadHugeDamaged())
		return false;

	//if (IsSkillAttacking())
	//{
	//	if (auto* DocBattle = Cast<UB2UIDocBattle>(UB2UIDocHelper::GetDocBattle()))
	//		if (DocBattle->GetIsTagSuggested() == false)
	//			return false;
	//	
	//}

	if (bIsDying)
		return false;

	return true;
}

bool ABladeIIPlayer::CanPlayerHaveMoveMotion() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CanPlayerHaveMoveMotion);
	//ABladeIIPlayerController*	B2PC				= Cast<ABladeIIPlayerController>(GetController());
	//AB2AutoAIController*		AutoAIController	= GetAutoAIController();

	//return (GetHadInput() || 
	//	// B2StageEventDirector 俊辑 付萍聪 矫累 困摹肺狼 教农 惑炔, ActiveCameraActor 版拌俊辑狼 贸府 惑炔.
	//	(B2PC && B2PC->CanPlayerHaveMoveMotion()) || 
	//	(AutoAIController && AutoAIController->CanPlayerHaveMoveMotion()));
	return true;
}

void ABladeIIPlayer::NotifyPlayerDeadEnd()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyPlayerDeadEnd);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::NotifyPlayerDeadEnd"));

	if (GetMesh())
	{
		// Dead 局聪皋捞记捞 场抄 惑怕肺 蜡瘤矫挪促. 何劝阑 窍扒 吝窜阑 窍扒, 绢瞒乔 货肺 spawn 窍霸 瞪 巴. 酒丛 促矫 积阿 せ
		bReachedAtDeadEnd = true;
		TotalApplyAnimRateScale();

		// 弊府绊.. 捞码俊辑 何劝 皋春甫 矫累窍档废 GameMode 率俊 脚龋
		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
		BII_CHECK(B2GM);

		//if (B2GM)
		//{
		//	B2GM->NotifyPlayerDeadEnd(this);
		//}

		// 磷篮 矫埃(楷免 肯丰) 历厘.
		m_fDeadTimeSec = GetWorld()->GetTimeSeconds();
	}
}

void ABladeIIPlayer::OnCounterStart()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnCounterStart);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OnCounterStart"));

	AddWeaponSkillPoint(2.0f);
	ForceFeedback();

	// 馆拜矫 HP 雀汗 可记篮 馆拜 己傍 矫 鸥拜 殿狼 蜡公客 惑包绝捞 公炼扒 利侩.
	if (HealHPAmountRateOnCounter > 0.0f) 
	{
		// GetUnitedOptionStatusValue 俊辑 欺季飘甫 0.01 胶纳老茄 蔼阑 临 巴牢单 HealingHPByPercent 绰 欺季飘蔼阑 罐栏骨肺 促矫 100 蚌窃.
		HealingHPByPercent(HealHPAmountRateOnCounter * 100.0f);
	}

	bForceCounterAttack = false;
	LastGuardBreakType = EGuardBreakLogicType::EGLT_AbleToGuard;
	//PlayerCounterAttackSucceedClass<>::GetInstance().Signal();
}

bool ABladeIIPlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const FDamageInfo* DamageType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CanTakeDamage);
	if (IsCounterAttacking())
		return false;

	if (IsInQTEState())
		return false;

	if (bInvincibleByStageEvent)
		return false;

	if (bbInvincibleByTutorial)
		return false;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (Cast<ABladeIITestDummyNPC>(GetAttacker(DamageCauser)))
	{ // TestDummyNPC 磊眉俊辑 Damage 甫 力芭窍档废 贸府窍绰 巴档 乐栏唱 弊巴父栏肺绰 茄拌啊 乐澜
		return false;
	}
#endif

	return Super::CanTakeDamage(DamageCauser, DamageEvent, DamageType);
}

void ABladeIIPlayer::ProcessTakeDamageAnimation(const FDamageInfo& DamageInfo, AActor* DamageCauser)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ProcessTakeDamageAnimation);
	Super::ProcessTakeDamageAnimation(DamageInfo, DamageCauser);
}

void ABladeIIPlayer::ProcessGuard(const FDamageInfo& DamageInfo, AActor* DamageCauser)
{
	if (IsGuarding() == false)
		return;

	if (auto* Attacker = GetAttacker(DamageCauser))
		RotateToTarget(Attacker);

	if (DamageInfo.GuardBreakLogicType != EGuardBreakLogicType::EGLT_AbleToGuard)
	{
		// Guard Break Handling
		LastGuardBreakType = DamageInfo.GuardBreakLogicType;
		StopGuard(true);
	}
}

void ABladeIIPlayer::PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak)
{
	auto ArmorBreakResponse = GetSuitableDamageType(DamageInfo, IsArmorBreak);

	// Dash吝俊绰 单固瘤 局聪皋捞记 角青 X
	if (IsTinyDamage(ArmorBreakResponse) && IsDashAttackState(GetAnimAttackState()))
		return;

	ApplyDamageAnimation(ArmorBreakResponse);
	UpdateDamageNum(DamageInfo);
}

void ABladeIIPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Tick_InputAndMovement(DeltaSeconds);
	//Tick_QTE(DeltaSeconds);
	//Tick_UnitySkill(DeltaSeconds);
	//
	//if (EtherContainer)
	//	EtherContainer->UpdateCoolTime(GetWorld()->GetDeltaSeconds());

	//if (CurrentAutoMoveAttackStatePtr)
	//	CurrentAutoMoveAttackStatePtr->Tick(DeltaSeconds);

	//if (CachedSkillRTManager)
	//	CachedSkillRTManager->CountRemainingCoolTime(GetWorld()->GetDeltaSeconds());

	//UpdateCameraBoom(DeltaSeconds);
	//UpdateLightComponentAttachment();


	//if (GetReservedSkill() != EAttackState::ECS_None && GetWorld())
	//{
	//	const float ReserveSkillRefreshTime = (Role == ROLE_Authority) ? 0.2f : 0.5f;
	//	if (GetWorld()->GetTimeSeconds() - LastSkillInputTime >= ReserveSkillRefreshTime)
	//		ConsumeReservedSkill();
	//}

	//UpdatePlayerBuffChanged();

	//if (CheckPlayerStopTime(DeltaSeconds))
	//{
	//	bRestartLevel = true;

	//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
	//	if (B2PC)
	//	{
	//		B2PC->RestartLevel();
	//	}
	//}

	//UpdateGuardCoolTime(DeltaSeconds);
}

void ABladeIIPlayer::Tick_InputAndMovement(float DeltaSeconds)
{
	// Dash Target Homing
	if (TotalDashDuration > 0.f)
	{
		CurrentDashDuration -= DeltaSeconds;
		if (CurrentDashDuration <= 0.f)
			StopDash();
	}

	if(CanRotateToTarget())
		RotateToTarget(DeltaSeconds);

	// To simulate repeat attack button
	if (IsKeepAttacking() && IsFineToStartAnyNewInputHandling() && GetAbleToControl() && !InGuardBreak())
		SetSignalAttack(true);

	else if (m_bGuardKeyPressed && !IsGuarding() && IsGuardStartable())
		StartGuard();

	/*if (GetGameMode()->GetGameRule()->IsEvadeCoolTimeCheck() && !CanEvade)
	{
		EvadeTimer -= DeltaSeconds;
		EvadeTimer = FMath::Max(EvadeTimer, 0.0f);

		CanEvade = (EvadeTimer == 0.0f);
	}*/

	if (bPendingTag) // 傈捧 悼累 吝 怕弊 抗距茄 版快
		ExecutePendingTag();

	if (bInWeaponSkillLOD0 && WeaponSkillLOD0SceneLight)
	{
		UpdateWeaponSkillLOD0LightComponentAttachment();
	}
	else
	{
		UpdateLightComponentAttachment();
	}
}

void ABladeIIPlayer::Tick_QTE(float DeltaSeconds)
{
	// QTE target handling
	if (IsInQTEState() && QTEActor && QTEActor->IsInQTEState())
	{
		// Sync to QTE host
		if (QTEActor->IsControlledByQTEPlayer())
		{
			if (QTEType == EQTEType::EQT_Mount)
			{
				const bool IsCurrentQTERun = QTEActor->GetVelocity().SizeSquared() > 100.f;
				bQTERun = QTEActor->bQTERun = IsCurrentQTERun;

				const FTransform& QTETransform = QTEActor->GetMesh()->GetSocketTransform(QTESocketName);
				SetActorLocationAndRotation(QTEActor->GetActorLocation(), QTEActor->GetActorRotation(), true);
				GetMesh()->SetWorldLocationAndRotation(QTETransform.GetLocation(), QTETransform.GetRotation());

				if (QTEStateNum >= 1) // QTE Idle/Attack
					CountQTEMountTime(DeltaSeconds);

				if (IsQTEMounting())
				{
					const int32 nQTEStateNum = GetQTEStateNum();
					if (nQTEStateNum > 0 && nQTEStateNum != QTEActor->QTEStateNum)
					{
						QTEActor->QTEStateNum = nQTEStateNum;
					}
				}
			}
		}
		else if (QTEActor->IsAlive())
		{
			FTransform QTETransform = GetMesh()->GetSocketTransform(QTESocketName);
			//QTETransform.RemoveScaling();
			QTETransform.AddToTranslation(FVector(0.f, 0.f, QTEActor->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
			QTEActor->SetActorTransform(QTETransform);
		}
	}
}

void ABladeIIPlayer::SetActorHiddenInGame(bool bNewHidden)
{
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
	if (bHideAllPCs){
		bNewHidden = true;
	}
#endif

	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetActorHiddenInGame);
	//const bool bWasHidden = bHidden;

	//Super::SetActorHiddenInGame(bNewHidden);

	//if (bWasHidden == bNewHidden) // 鞍篮 炼扒栏肺 咯矾锅 捞霸 阂副 锭啊 乐绢辑 酒贰 眠啊 内靛甸篮 犬角洒 函版捞 登菌阑 锭父 角青登档废
	//	return;

	//FormalSetPointLightOn(!bNewHidden); // 敲饭捞绢 某腐磐父 见扁搁 咯扁俊 嘿牢 扼捞飘啊 鞍捞 救 波廉辑 捞法菌唱?

	//// 漂洒 Hidden 捞饶俊 促矫 焊捞档废 沁阑 锭 Particle 嘿牢 巴甸捞 救焊捞绰 版快啊 乐绢辑.. 捞犯霸 秦 林聪 肋 唱坷绰淀 茄单..
	//if (!bNewHidden)
	//{
	//	EnsureActivateStaticallyAttachedPSCs(this);
	//}	
}

void ABladeIIPlayer::FormalSetPointLightOn(bool bIsOn)
{
	// 老馆利牢 霸烙 扁瓷 惑狼 格利栏肺 敲饭捞绢 赣府 困 PointLight On/Off 矫俊绰 捞吧 静档废.
	// ABladeIIPlayer 郴何俊辑档.
	if (LightComponent) 
	{
		// 老何 葛靛俊辑 赣府 困 PointLight 甫 倾侩窍瘤 臼绰 版快啊 乐澜.
		const bool bAllowedByGameRule = (GetInjectedGameRule() && GetInjectedGameRule()->AllowPlayerPointLight());

		// 逢俊 狼秦 倾啊登瘤 臼绰 版快..
		if (!bAllowedByGameRule || !IsHeadPointLightAllowed())
		{
			bHeadPointLightIsFormallyOn = false;
			LightComponent->bAffectsWorld = false;
			LightComponent->UnregisterComponent();
			LightComponent->Deactivate();
		}
		else
		{
			if (bIsOn)
			{
				bHeadPointLightIsFormallyOn = true;
				LightComponent->bAffectsWorld = true;
				LightComponent->SetVisibility(true);
				LightComponent->RegisterComponent();
				LightComponent->Activate();
			}
			else
			{
				bHeadPointLightIsFormallyOn = false;
				LightComponent->bAffectsWorld = false;
				LightComponent->SetVisibility(false); // 怖怖 见绢扼 赣府墨遏 焊老扼?
				LightComponent->UnregisterComponent();
			}

		}

#if !UE_BUILD_SHIPPING
		extern bool gbTestTurnOffPlayerLight;
		if (gbTestTurnOffPlayerLight) // Off 狼 版快父 蝶肺 贸府
		{
			bHeadPointLightIsFormallyOn = false;
			LightComponent->bAffectsWorld = false;
			LightComponent->UnregisterComponent();
			LightComponent->Deactivate();
		}
#endif

		LightComponent->MarkRenderStateDirty();
	}
}

void ABladeIIPlayer::SetupLightComponentAttachment()
{
	USkeletalMeshComponent* SKMeshComp = GetMesh();
	if (LightComponent && SKMeshComp)
	{
#if !UE_BUILD_SHIPPING
		extern int32 gTestAttachLightCompMode; // 捞吧 绢叼俊 绢痘霸 嘿咯具 亮阑瘤 酒流 隔扼辑 抛胶飘 力傍
		if (gTestAttachLightCompMode == 2) // 0, 1 锅篮 积己磊俊辑 技泼茄 措肺 母蕉 attach 蜡瘤
		{
			LightComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
			LightComponent->AttachToComponent(SKMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_ROOTBONE_NAME);
			// 风飘夯篮 某腐磐 窍窜 吝缴捞绊 母蕉篮 吝居捞扼 眠啊 offset 阑 啊窃
			//LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, LightComponentAttachDistZ + ZOffsetFromMatchingSKActorToBIICharacter(this)));
		}
		else
#endif
		{
			LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, LightComponentAttachDistZ));
		}
	}
}

void ABladeIIPlayer::UpdateLightComponentAttachment()
{
#if !UE_BUILD_SHIPPING
	extern int32 gTestAttachLightCompMode; // 酒流篮 抛胶飘 吝..
	if (gTestAttachLightCompMode == 0)
#endif
	{
		USkeletalMeshComponent* SKMeshComp = GetMesh();
		if (LightComponent && SKMeshComp)
		{
			// 嘿捞绰 扒 母蕉 哪器惩飘俊 嘿捞瘤父 吝缴(Bip001)夯 俊辑 惑措利牢 困摹俊 乐档废
			// 捞犯霸 秦辑 某腐磐 局聪皋捞记俊 蝶弗 皋浆 困摹 offset 阑 蝶扼啊档废 茄促. 
			// 弊成 夯俊 嘿咯 滚府搁 雀傈鳖瘤 蝶扼啊霸 登绢辑 拜纺茄 葛记俊辑 困摹啊 农霸 夸悼窍霸 登聪 夯俊 嘿捞瘤绰 臼绊 困摹父 蝶扼啊霸 秦 淋.
			FVector UpdatedLocation = SKMeshComp->GetBoneLocation(B2PLAYER_CENTERBONE_NAME, EBoneSpaces::WorldSpace);

			if (QTEActor && (QTEActor->IsInQTEState() || QTEActor->QTEType == EQTEType::EQT_Mount))
			{
				UpdatedLocation = QTEActor->GetMesh()->GetBoneLocation(B2PLAYER_MOUNT_CENTERBONE_NAME, EBoneSpaces::WorldSpace);
			}

			UpdatedLocation.Z += LightComponentAttachDistZ;
			LightComponent->SetWorldLocation(UpdatedLocation);
		}
	}
}

int32 ABladeIIPlayer::IsInAutoPlay() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsInAutoPlay);
	// 瘤陛 寸厘狼 Auto 惑怕 咯何肺辑 GameMode 率狼 Maintained 惑怕客 崔府 老矫利栏肺 波龙 荐 乐澜.
	return InAutoPlay;
}

AB2AutoAIController* ABladeIIPlayer::GetAutoAIController() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetAutoAIController);
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
	if (B2PC == nullptr)
		return nullptr;

	return B2PC->GetAutoAIController();
}

// 捞率狼 Start/StopAutoPlay 甫 流立 荤侩窍绰 巴篮 老矫利栏肺 Auto 甫 On/Off 窍芭唱 AI 敲饭捞绢狼 版快捞绊
// 老馆利牢 霸烙葛靛俊辑 Auto 甫 On/Off 且 锭俊绰 GameMode 率 牢磐其捞胶甫 荤侩秦具 惑怕啊 蜡瘤等促.
void ABladeIIPlayer::StartAutoPlay()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAutoPlay);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::StartAutoPlay"));

	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
	//if (B2PC != nullptr)
	//{
	//	if (B2PC->StartAutoPlay(this))
	//	{
	//		if (this->IsLocalCharacter())
	//		{
	//			auto* InGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//			check(InGameMode);
	//			SetInAutoPlay(InGameMode->GetPlayerAutoBattleType());
	//		}
	//		else
	//		{
	//			SetInAutoPlay(AutoBattleType::Skill);
	//		}
	//	}
	//	else
	//	{
	//		SetInAutoPlay(AutoBattleType::NONE);
	//	}
	//}
}

void ABladeIIPlayer::StopAutoPlay()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopAutoPlay);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::StopAutoPlay"));

	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
	//if (B2PC == nullptr)
	//	return;

	//AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this));
	//if (StageGM && StageGM->IsRepeatBattleOn()){
	//	return; // 馆汗傈捧俊辑绰 磊悼 惑怕甫 难初篮 盲 蜡瘤.
	//}

	//InAutoPlay = AutoBattleType::NONE;

	//B2PC->StopAutoPlay();
	//StopRepeatingInput();
}

void ABladeIIPlayer::FinalKillQTETarget()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_FinalKillQTETarget);
	//// Kill target after final QTE action or when take off mount 
	//if (QTEActor && (QTEActor->IsInQTEState() || QTEActor->QTEType != EQTEType::EQT_Mount))
	//{
	//	if (QTEActor->GetLifeSpan() < 0.f) // for safety, if there is no deadend in QTE animation
	//	{
	//		QTEActor->Die(QTEActor->Health, FDamageEvent(UDamageType::StaticClass()), GetController(), NULL);
	//		QTEActor->Destroy();
	//	}
	//	else
	//	{ // 老馆利牢 版快. QTE 俊 吧赴 各篮 内靛惑俊辑 Die 甫 烹窍瘤 臼绊 局聪皋捞记俊辑 DeadEnd 甫 烹茄 贸府父 等促. (OnAnimMobDeadEnd)
	//		QTEActor->DieForQTE(); // 蝶扼辑 霸烙 肺流 惑 鞘夸茄 荤噶脚绊 殿狼 贸府甫 蝶肺 秦 霖促.
	//	}
	//	QTEActor = NULL;
	//	QTENPCClass = ENPCClass::ENC_End;
	//	QTEType = EQTEType::EQT_End;
	//}
}

void ABladeIIPlayer::ResetAllQTEUIDocState()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ResetAllQTEUIDocState);
	// 傈捧 UI visibility 甫 刮皑窍霸 力绢窍绰 何盒捞 粱 乐绰单 捞繁历繁 鸥捞怪捞尔 揩老 锭啊 乐绰 芭 鞍酒 割割 焙单俊辑 犬牢荤混 瞒 龋免且 侩档.
	//auto* DocBattle = GetDocBattle();
	//if (DocBattle)
	//{
	//	DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::None);
	//	DocBattle->SetbIsPlayerInQTEState(false);
	//	DocBattle->SetbIsQTEMounted(false);
	//	DocBattle->SetbIsQTEMountedAndReadyForAttack(false);
	//}
}

void ABladeIIPlayer::ReceiveComboEnd(EAttackState InLastAttackState, bool IsAttackCanceled)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ReceiveComboEnd);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::ReceiveComboEnd, LastAttackState %d, CurrentAttackState"), static_cast<int32>(InLastAttackState), static_cast<int32>(GetAttackState())));

	//// QTE Data Handling
	//HandleComboEndForQTE();	

	//if (InLastAttackState >= EAttackState::ECS_GuardStart && InLastAttackState <= EAttackState::ECS_Evade)
	//{
	//	::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);
	//}

	//if (InLastAttackState == EAttackState::ECS_Evade)
	//	OnEvadeEnd();

	//if (!((InLastAttackState >= EAttackState::ECS_Skill01 && InLastAttackState < EAttackState::ECS_GuardStart) // Tag 档 咯扁 器窃矫虐绰单 Tag 肺绰 救棵 荐 乐促. 咯扁 贸府甫 促弗 风飘肺档 秦 林档废.
	//	// Intended to see if this is comboend notification for skill end, but it is not that easy to ensure, so check additional condition.
	//	// At worst, it could just set the timer in wrong situation, better than not setting it.
	//	// See StartGuard/StopGuard, we do some to get relevant ComboEnd for skill attack anyway.
	//	|| bLastInvokedAnySkillAttack))
	//	return;

	//BII_CHECK(CachedSkillRTManager);

	//// Do the cooltime timer count for skill end.
	//if (CachedSkillRTManager)
	//{
	//	CachedSkillRTManager->SetSkillCoolTimeTimer(InLastAttackState);
	//}
	//bLastInvokedAnySkillAttack = false;
}

void ABladeIIPlayer::OnProcessComboEnd()
{
	bSkillCancel = false;
	bComboEnabled = false;
	SetAttackState(EAttackState::ECS_None);
	TagAttackMode = ETagAttackMode::EAM_DownAttack;
	SetReceiveNetMotionCancleSignal(false);
	LastGuardBreakType = EGuardBreakLogicType::EGLT_AbleToGuard;

	//if (GetStageEventShowState() == EStageEventShowState::ESES_None)
	//{
	//	if (auto* PlayerCameraManager = UGameplayStatics::GetLocalPlayerCameraManager(this))
	//		PlayerCameraManager->StopAllCameraAnims(true);
	//}
}

bool ABladeIIPlayer::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (bCheatImmortalMode) // Damage 绰 涝瘤父 磷瘤绰 臼澜.
	{
		return false;
	}
#endif
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
	//if (B2PC && B2PC->IsStageEndNonCombatState()){
	//	return false; // 胶抛捞瘤 辆丰 矫痢俊辑狼 公利 贸府 眠啊肺.
	//}

	return Super::CanDie(KillingDamage, DamageEvent, Killer, DamageCauser);
}

void ABladeIIPlayer::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnDeath);

	// 何葛窃荐唱 历酒贰 怕弊芭摹搁辑 肺拿敲饭捞绢啊 酒聪霸凳 固府眉农
	CheckSendKakaoActionLog_Death();

	ABladeIIPlayer* KillerPlayer = Cast<ABladeIIPlayer>(DamageCauser);
	if (KillerPlayer)
		KillerPlayer->CheckSendKakaoActionLog_Kill();

	Super::OnDeath(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser);

	TargetActor = nullptr;

	//// 怕弊啊 倾侩等 霸烙葛靛捞绊 怕弊啊 啊瓷窍促搁 磊悼怕弊
	//// 齿霸烙葛靛绰 力寇. pvp霸烙葛靛 焊搁 NotifyCharacterDead 烹秦辑 怕弊窍绰 肺流捞 乐澜
	//AB2NetGameMode* pCastedNetGM = Cast<AB2NetGameMode>(GetGameMode());

	//if (!pCastedNetGM && GetInjectedGameRule()->AllowToTag() && CachedSkillRTManager && CachedSkillRTManager->IsTagPossibleAtDeadMoment())
	//{
	//	CachedSkillRTManager->TagToOther((int32)ETagAttackMode::EAM_DownAttack, EAttackState::ECS_None);
	//}

	//SetActorEnableCollision(false);	
}

int64 ABladeIIPlayer::GetAccountId()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetAccountId);
	//if (IsLocalPlayer())
	//	return BladeIIGameImpl::GetClientDataStore().GetAccountId();

	return 0;
}

bool ABladeIIPlayer::IsQTEStartAble()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsQTEStartAble);
	return GetInjectedGameRule()->IsQTEEnabled() && GetDamagedNum() == 0 && !IsStunned() && !IsFrozen() && GetAttackState() < EAttackState::ECS_Skill01 && !IsInAutoPlay();
}

float ABladeIIPlayer::GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds)
{
	// 搬加胶懦 荤侩吝浚 救曼
	if (UnitySkillCoolTime != 0 && GetAttackState() != EAttackState::ECS_WeaponSkill)
	{
		return (DeltaSeconds / UnitySkillCoolTime) * UnitySkillAddPoint;
	}

	return 0.0f;
}

void ABladeIIPlayer::Tick_UnitySkill(float DeltaSeconds)
{
	if (IsUnityTick == true && bOverUnityPoint == true)
	{
		float fWorldDeltaSeconds = GetWorld()->GetDeltaSeconds();
		float fAddPoint = GetUnitySkillPointDeltaSecondsRate(fWorldDeltaSeconds);
		if (fAddPoint != 0.0f)
		{
			AddWeaponSkillPoint(fAddPoint);
		}
	}
}

float ABladeIIPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TakeDamage);

//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
//	if (B2PC && B2PC->IsStageEndNonCombatState()){
//		return 0.0f; // 胶抛捞瘤 辆丰 矫痢俊辑狼 公利 贸府. bCanBeDamaged 甫 结档 扁瓷搁俊辑绰 瞪 芭绰 鞍篮单 趣 促弗 芭尔 揩老瘤 葛福摆焙.
//	}
//
//	const FDamageInfo* DamageType = static_cast<const FDamageInfo*>(DamageEvent.DamageInfo);
//	const FDamageInfo& DamageInfo = *(DamageType);
//	if (!DamageType) {
//#if !UE_BUILD_SHIPPING
//		// 措眉肺 DamageType 捞 绝绰 惑炔篮 楷免阑 烹秦 敲饭捞 登绰 attack 局聪皋捞记俊辑 damage notify 啊 唱啊绰 芭. InterpTrackAnimControl 狼 SkipDamageNotifiersOnly 甫 眉农.
//		//UE_LOG(LogBladeII, Warning, TEXT("NULL DamageType is detected of DamageCauser %s"), DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"));
//#endif
//		return 0.0f;
//	}

	//ProcessGuard(DamageInfo, DamageCauser);
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

//	CounterCheckTime = DamageType->bCountable ? FMath::Abs(CounterCheckTime) : -FMath::Abs(CounterCheckTime);
//
//	// 馆拜 带傈侩
//	bCounterCauseStun = DamageType->bStunCountable;
//
//	if (ActualDamage > 0.f || GetB2GameModeType(GetWorld()) == EB2GameMode::CounterDungeon)
//	{
//		PlayerTakeAttackClass<float>::GetInstance().Signal(0.1f);
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (bCheatImmortalMode)
//	{// CanDie 俊辑 return false 窍看瘤父 Health 蔼捞 0牢瘤 八荤秦辑 磷篮 巴 贸烦 贸府且 荐 乐栏骨肺 1篮 巢败狄.
//		Health = FMath::Max(Health, 1.0f); 
//	}
//#endif
//
//	AB2AutoAIController*	AutoAIController	= GetAutoAIController();
//	ABladeIIProjectile*		ProjectileCauser	= Cast<ABladeIIProjectile>(DamageCauser);
//	ABladeIICharacter*		ProjectileOwner		= ProjectileCauser ? Cast<ABladeIICharacter>(ProjectileCauser->GetOwner()) : nullptr;
//
//	if (AutoAIController && ProjectileCauser && ProjectileOwner && ProjectileOwner->IsAlive())
//	{
//		// To track the character who fired the projectile out of player's target range in autoplay mode..
//		AutoAIController->CachedRangeAttackedMob = ProjectileOwner;
//	}
//
//	// 后荤 惑怕
//	bIsInCriticalCondition = (Health / MaxHealth <= CriticalConditionHealthRatio) ? true : false;

	return ActualDamage;
}

void ABladeIIPlayer::NotifyTargetDamaged(class ABladeIICharacter* DamagedTarget, float DamageAmount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyTargetDamaged);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::NotifyTargetDamaged"));

	Super::NotifyTargetDamaged(DamagedTarget, DamageAmount);

	// Someday might need check Target for friendly fire..

	++CurrentComboNum; // Unconditionally increase ComboNum.

	if (!GetWorld())
		return;

	LastAttackSuccessTime = GetWorld()->GetTimeSeconds();
	if (ResetComboNumTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ResetComboNumTimerHandle);
	}
	GetWorldTimerManager().SetTimer(ResetComboNumTimerHandle, this, &ABladeIIPlayer::ResetComboNumTimerCallback, ComboMaintainDuration, false);

	//TargetDamagedClass<ABladeIICharacter*, int32, float>::GetInstance().Signal(this, CurrentComboNum, DamageAmount); // UI 肺 脚龋
	UpdateUIDocOnTargetDamaged();
}

void ABladeIIPlayer::ResetComboNumTimerCallback()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ResetComboNumTimerCallback);
	CurrentComboNum = 0;
	auto* DocBattle = GetDocBattle();
	if (DocBattle)
	{
		//DocBattle->SetComboNum(CurrentComboNum);
	}
}

void ABladeIIPlayer::InitUpdateUIDoc()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_InitUpdateUIDoc);
	// 割啊瘤 贸澜俊 技泼登绢具 且 巴甸..
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::InitUpdateUIDoc"));

	Super::InitUpdateUIDoc();
		
	// ABladeIIPlayer::SpawnOtherAsPuppet 鞍篮 吧肺 积己等 版快 update 俊 尝绢 甸绢啊急 救凳.
	//if (IsLocalPlayer())
	//{
	//	// UIDoc 捞 static 捞骨肺 概 平 诀单捞飘 登绰 霸 酒囱 腹篮 函荐甸俊 措秦 惑怕甫 檬扁拳秦 临 鞘夸啊 乐促.
	//	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//	InjectDocBattle(DocBattle);

	//	if (DocBattle)
	//	{
	//		DocBattle->SetCurrPCMaxHealth(MaxHealth);
	//		DocBattle->SetComboNum(CurrentComboNum);

	//		// 怕弊 矫痢俊档 搬惫 咯扁肺 棵 巴捞骨肺 怕弊 努贰胶 殿阑 咯扁辑 技泼
	//		// UB2SkillRTManager::DynSpawnNewPlayerCommon 曼绊. 捞 矫痢俊 TagPlayerClass 甫 掘绢棵 荐 乐阑 巴.
	//		// 牢霸烙俊辑绰 泅犁 劝己/厚劝己 某腐磐客 角力 main/sub 狼 备盒阑 秦具 窃. Main 捞 犬角洒 泅犁 劝己牢 版快绰 怕弊甫 酒流 窍瘤 臼篮 霸烙 矫累 檬馆挥烙.
	//		DocBattle->SetIsCurPCMain(AmIMainCharacter());
	//		DocBattle->SetCurPCClass((int32)GetCurrentPlayerClass());
	//		DocBattle->SetTagPCClass((int32)GetTagPlayerClass());
	//		DocBattle->SetInAutoPlay(IsInAutoPlay()); // AutoPlay 咯何绰 贸澜俊父 咯扁辑 技泼窍绊 捞饶俊绰 GameMode 率俊辑

	//		DocBattle->SetbTagAttackPending(false); // 秦寸 敲贰弊绰 捞固 荤侩等 饶老 巴捞促.
	//	}

	//	ResetAllQTEUIDocState();

	//	SetGuardCoolTime(0.0f);
	//	SetHasNotTagDefence(false);
	//}
}

void ABladeIIPlayer::UpdateUIDocOnTargetDamaged()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateUIDocOnTargetDamaged);
	//auto* DocBattle = GetDocBattle();
	//if (DocBattle)
	//{
	//	DocBattle->SetComboNum(CurrentComboNum);
	//	DocBattle->SetCriticalRateByComboNum(GetAdditionalCriticalRateByComboNum());
	//}
}

void ABladeIIPlayer::CharacterBuffUpdateUIdoc()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CharacterBuffUpdateUIdoc);
	//auto* DocBattle = GetDocBattle();
	//if (DocBattle && IsLocalPlayer())
	//{
	//	TArray<FUISkillBuffDisplayInfo_Character> AllCharacterBuffInfo; // 泅犁 惯悼茄 巴甸父 持扁

	//	for (UB2Buff_Base* pBuff : *(GetBuffManager()->GetBuffList()))
	//	{
	//		if (pBuff->IsActive())
	//		{
	//			FUISkillBuffDisplayInfo_Character ThisInfo;
	//			ThisInfo.MyType = pBuff->GetBuffType();
	//			ThisInfo.LastStartedTime = pBuff->GetBuffBeginTime();
	//			ThisInfo.RemainingTime = pBuff->GetRemainTime();
	//			AllCharacterBuffInfo.Add(ThisInfo);
	//		}
	//	}
	//	DocBattle->SetCharacterBuffState(AllCharacterBuffInfo);
	//}
}

void ABladeIIPlayer::SpawnSurviveSkillEffect()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnSurviveSkillEffect);
	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	//UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : nullptr;

	//UParticleSystem* RestoreParticle = SomeInfo ? SomeInfo->GetSurviveSkillRestoreEffect() : nullptr;
	//if (RestoreParticle)
	//{
	//	UGameplayStatics::SpawnEmitterAttached(RestoreParticle, GetBaseMesh(), FName(TEXT("Bip001-Pelvis")));
	//}
}

float ABladeIIPlayer::GetShield() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetShield);

	/*if (BuffManager != nullptr)
	{
		if (UB2Buff_Shield* ShieldBuff = Cast<UB2Buff_Shield>(BuffManager->FindBuff(EBuffType::Buff_Shield)))
		{
			return ShieldBuff->GetShieldAmount();
		}
	}*/

	return 0.0f;
}

void ABladeIIPlayer::SetShield(const float NewShieldAmount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetShield);
	/*if (UB2Buff_Shield* ShieldBuff = Cast<UB2Buff_Shield>(BuffManager->FindBuff(EBuffType::Buff_Shield)))
	{
		ShieldBuff->SetShieldAmount(NewShieldAmount);		

		if (IsLocalPlayer())
		{
			auto* DocBattle = GetDocBattle();
			DocBattle->SetCurrPCShield(NewShieldAmount);
		}
	}*/
}

bool ABladeIIPlayer::IsComboAttacking() const
{
	const auto CurrAttackState = GetAnimAttackState();
	return IsOneOfComboAttackState(CurrAttackState) || CurrAttackState == EAttackState::ECS_Dash;
}

bool ABladeIIPlayer::IsSkillAttacking() const
{
	return IsOneOfSkillAttackState(GetAnimAttackState());
}

bool ABladeIIPlayer::IsCounterAttacking() const
{
	return IsCounterAttackingState(GetAnimAttackState());
}

bool ABladeIIPlayer::HadHugeDamaged()
{
	//if (auto* AnimInst = GetAnimInstance())
	//{
	//	const EAttackType CurrDamageAnim = AnimInst->GetDamageAnimIndex();
	//	if (CurrDamageAnim == EAttackType::EAT_BigBounce || CurrDamageAnim == EAttackType::EAT_KnockBack)
	//		return true;
	//}
	return false;
}

bool ABladeIIPlayer::IsGuarding()
{
	return IsGuardingState(GetAnimAttackState());
}

void ABladeIIPlayer::SpawnResurrectBuffTextEffect()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnResurrectBuffTextEffect);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SpawnResurrectBuffTextEffect"));
	/*
		UB2BuffModeEffectInfo* BuffModeEffectInfo = GetBuffModeEffectInfo();
		if (BuffModeEffectInfo)
		{
			TMap<EResurrectBuffType, int32> AppliedResurrectBuff;
			CachedSkillRTManager->GetAppliedResurrectionBuffValues(AppliedResurrectBuff);
			int32* AttackBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Attack);
			if (AttackBuffIfAny && *AttackBuffIfAny > 0)
			{
				BuffModeEffectInfo->SpawnResurrectTextEffect(EResurrectBuffType::RBT_Attack, this);
			}
			int32* DefenseBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Defense);
			if (DefenseBuffIfAny && *DefenseBuffIfAny > 0)
			{
				BuffModeEffectInfo->SpawnResurrectTextEffect(EResurrectBuffType::RBT_Defense, this);
			}
			int32* HealthBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Health);
			if (HealthBuffIfAny && *HealthBuffIfAny > 0)
			{
				BuffModeEffectInfo->SpawnResurrectTextEffect(EResurrectBuffType::RBT_Health, this);
			}
		}*/
}

void ABladeIIPlayer::DestroyPlayerUIManager()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_DestroyPlayerUIManager);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::DestroyPlayerUIManager"));

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM && B2GM->GetUIManager_InGameCombat())
	//{
	//	B2GM->GetUIManager_InGameCombat()->DestroyHUDForDyingPlayer(this); // 敲饭捞绢 傈侩 力芭.
	//}
}

UB2SkillRTManager* ABladeIIPlayer::GetSkillRTManager()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetSkillRTManager);
	//if (CachedSkillRTManager == NULL)
	//{
	//	// Controller 俊 狼秦 Possess 登绢具 啊瓷
	//	auto* B2PC = Cast<ABladeIIPlayerController>(GetController());
	//	CachedSkillRTManager = B2PC ? B2PC->GetPlayerSkillRTManager() : NULL;

	//	if (CachedSkillRTManager)
	//		CachedSkillRTManager->InitSkillRTManager(this);
	//}
	//BII_CHECK(!Cast<ABladeIIPlayerController>(Controller) || CachedSkillRTManager); // Controller 啊 绝绰 惑炔捞 ABladeIIPlayer::SpawnOtherAsPuppet 俊辑 唱棵 荐 乐澜.
	return CachedSkillRTManager;
}

ICharacterDataStore* ABladeIIPlayer::GetCharacterDataStore()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCharacterDataStore);
	//咯扁肺 坷绰 巴篮 Local侩 Player character肺 啊沥茄促.
	if (CharacterDataStore == nullptr)
		CharacterDataStore = &BladeIIGameImpl::GetLocalCharacterData();

	return CharacterDataStore;
}

//UB2InGameHUDInfo* ABladeIIPlayer::GetHUDInfoObject()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetHUDInfoObject);
//	if (HUDInfoClass)
//	{
//		return Cast<UB2InGameHUDInfo>(HUDInfoClass->GetDefaultObject());
//	}
//	return NULL;
//}

void ABladeIIPlayer::SetupControlledMatineePuppet(ASkeletalMeshActor* InMatineePuppet, bool bSetupLightComponent, USkeletalMesh* OptionalSKMeshToSet)
{
	check(!OptionalSKMeshToSet); // 利绢档 咯扁 寇何俊辑 捞吧 流立 持绢林绰 版快绰 绝绰 吧肺.. 趣 乐阑 芭 鞍澜 捞 check 甫 力芭窍绰单 酒贰 内靛客 揩捞瘤 臼绰瘤 曼绊..

	//// 冀记 捍钦 葛胆阑 荤侩窍绰 霸烙葛靛牢单 楷免俊辑档 弊吧 楷加利栏肺 荤侩窍绰 霸 酒聪搁 货肺 冀记 盒府等 葛胆 技泼
	//USkeletalMesh* ExplicitNonMergedMesh = nullptr;
	///*if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this) && !UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this))
	//{
	//	ExplicitNonMergedMesh = GetMergedMesh(false);
	//}*/

	//USkeletalMeshComponent* MyBaseMeshComp = this->GetBaseMesh();
	//USkeletalMeshComponent* PuppetBaseMeshComp = InMatineePuppet ? InMatineePuppet->GetSkeletalMeshComponent() : nullptr;

	//////////////////////////////////////////

	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupControlledMatineePuppet);
	//Super::SetupControlledMatineePuppet(InMatineePuppet, bSetupLightComponent, 
	//	ExplicitNonMergedMesh ? ExplicitNonMergedMesh : OptionalSKMeshToSet // OptionalSKMeshToSet 牢磊甫 咯扁辑 父甸绢辑 逞败林霸 凳..
	//);

	//// 厘脚备 鞍篮 芭 嘿栏搁 眠啊肺 技泼秦 林妨绊 沁绰单 愧父茄 巴甸篮 惑困窜 窃荐俊辑 扁夯 贸府.
	//
	//if (MyBaseMeshComp && PuppetBaseMeshComp)
	//{
	//	// Scalability 牧飘费狼 老何肺辑 ForcedLOD 冈牢 巴档 傈崔秦 霖促. 荤角 楷免侩篮 0 老芭扼 鞘夸绰 绝摆瘤父 哗冈绢辑 亮阑 波 绝瘤.
	//	PuppetBaseMeshComp->SetForcedLOD(MyBaseMeshComp->ForcedLodModel);
	//}
}

/** 措眉肺 楷免 侩栏肺 歹固 怕弊 某腐磐 spawn. 歹固扼绊 秦辑 抛胶飘 扁瓷捞 酒聪促.
 * PossessByAIController 绰 AI 甫 困茄 霸 酒聪扼 焊促 扁夯利牢 吝仿 利侩 鞍篮 芭 贸府 瞒盔. */
ABladeIIPlayer* ABladeIIPlayer::SpawnOtherAsPuppet(bool bPossessByAIController /*= true*/, bool bTurnOnPointLight /*= false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnOtherAsPuppet);
	BII_CHECK(GetTagPlayerClass() != EPCClass::EPC_End);
	ABladeIIPlayer* Spawned = SpawnPuppetInternal(GetTagPlayerClass(), bPossessByAIController);
	if (Spawned)
	{ // 葛官老 pointlight 力茄档 乐绊.. 怕弊率篮 啊鞭利 掺档废.. 捞霸 鞘夸茄 惑炔捞 乐栏妨唱..
		Spawned->FormalSetPointLightOn(bTurnOnPointLight);
	}
	return Spawned;
}

ABladeIIPlayer* ABladeIIPlayer::SpawnMyselfAsPuppet(bool bPossessByAIController /*= true*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnMyselfAsPuppet);
	return SpawnPuppetInternal(GetCurrentPlayerClass(), bPossessByAIController);
}

ABladeIIPlayer* ABladeIIPlayer::SpawnPuppetInternal(EPCClass InSpawnClass, bool bPossessByAIController /*= true*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnPuppetInternal);
	//BII_CHECK(this->IsValidObj());

	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
	//if (InSpawnClass != EPCClass::EPC_End && B2PC && B2PC->IsLocalController())
	//{
	//	const FTransform& CurrTransform = GetTransform();
	//	const float AsideDistance = GetCapsuleComponent() ? GetCapsuleComponent()->GetScaledCapsuleRadius() : 50.0f;
	//	// 混娄 糠俊辑 spawn 矫挪促. 捞吧 静绰 惑炔俊辑 角力肺 焊咯瘤绰 困摹绰 楷免 飘发 殿栏肺 喊档肺 炼例秦具 窃.
	//	FTransform NewTransform(
	//		CurrTransform.GetRotation(),
	//		CurrTransform.GetTranslation() + FVector(AsideDistance, AsideDistance, 50.0f),
	//		CurrTransform.GetScale3D()
	//	);

	//	// 迭洒 捞傈 吧 绝矩促电瘤 且 鞘夸 绝捞 弊措肺 府畔. 罐篮 率俊辑 肋 贸府. 
	//	// PlayerController 俊 possess 窍瘤绰 臼栏聪 GetLocalPlayerCharacter 殿捞 狼档客 崔府 咯扁辑 积己茄 巴阑 府畔茄促电瘤 窍绰 老篮 绝阑 巴.
	//	return ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, InSpawnClass, NewTransform, bPossessByAIController);
	//}
	return nullptr;
}

void ABladeIIPlayer::SetEvadeTimer()
{
	CanEvade = false;
	EvadeTimer = EVADE_COOL_TIME;
}

// 酒贰 SetupLODInfo~~ 揪府令 侩 傍烹 扁瓷
static void SetB2PCMergedMeshCompLODInfoInternal(USkeletalMeshComponent* InMeshComp, float InScreenSize_0, float InScreenSize_1)
{
	BLADE2_SCOPE_CYCLE_COUNTER(Global_SetB2PCMergedMeshCompLODInfoInternal);
	//// 角力 俊叼磐俊辑 焊绰 府家胶啊 酒囱 繁鸥烙俊 merge 等 皋浆甫 惑沥窃. 咯扁辑 Mesh 府家胶狼 加己蔼阑 官曹 巴捞扁 嫐巩俊 老馆利栏肺 俊叼磐俊辑 焊绰 弊 府家胶咯辑绰 帮鄂窍促.
	//USkeletalMesh* MeshResource = InMeshComp ? InMeshComp->SkeletalMesh : NULL;
	////FSkeletalMeshObject* MeshResourceObj = InMeshComp ? InMeshComp->MeshObject : NULL;

	//if (!MeshResource /*|| !MeshResourceObj*/) {
	//	return;
	//}

	//bool bScreenSizeChanged = false;
	//for (int32 LODIdx = 0; LODIdx < MeshResource->LODInfo.Num(); ++LODIdx)
	//{
	//	FSkeletalMeshLODInfo& MeshLODInfo = MeshResource->LODInfo[LODIdx];
	//	const float PrevScreenSize = MeshLODInfo.ScreenSize;

	//	// 老窜篮 0 苞 1 捞惑狼 LOD 父 备盒. 弊 捞惑篮 鞘夸俊 蝶扼
	//	MeshLODInfo.ScreenSize = (LODIdx == 0 ? InScreenSize_0 : InScreenSize_1);
	//	
	//	if (PrevScreenSize != MeshLODInfo.ScreenSize)
	//	{
	//		bScreenSizeChanged = true;
	//	}
	//}
	//if (bScreenSizeChanged)
	//{
	//	InMeshComp->UpdateLODStatus(); // 捞犯霸 茄促绊 咯扁辑 官肺 函版捞 瞪 芭 鞍瘤绰 臼篮单..
	//	InMeshComp->MarkRenderStateDirty();
	//}
}

/** 
 * Scalability 矫胶袍狼 GameplayOnlyLODQuality 狼 技何 亲格栏肺 荤侩窍扁 困窃. 
 * 傈捧纠, 楷免纠 (肺厚器窃) 滴 何幅肺 唱床辑
 * ForceLODLevel 捞尔 ForcedLODLevel 捞 0 老 锭 荤侩且 ScreenSize 啊 乐促.
 * 扁瓷 磊眉父 杭 锭俊绰 ForceLODLevel 肺 愧父怒 目滚 啊瓷茄单 ForceLODLevel 俊 狼粮窍搁 r.SkeletalMeshLODBias 甫 烹茄 抛胶飘档 绢妨况廉辑 代 静霸 凳. 
 * 弊府绊 埃趣 抗惑摹 给茄 鞭拜茄 墨皋扼焢俊 措厚秦具 且 巴 鞍篮 锭档 乐绊.
 */
int32 GB2PlayerCharForcedLODLevelCombat = 0;
FAutoConsoleVariableRef CVarB2PlayerCharForcedLODLevelCombat(
	TEXT("r.B2PlayerCharForcedLODLevelCombat"),
	GB2PlayerCharForcedLODLevelCombat,
	TEXT("Forced LOD level for BladeII player character meshes in combat scene, intended to be controlled by Scalability system."),
	ECVF_Scalability
);
int32 GB2PlayerCharForcedLODLevelCinematic = 0;
FAutoConsoleVariableRef CVarB2PlayerCharForcedLODLevelCinematic(
	TEXT("r.B2PlayerCharForcedLODLevelCinematic"),
	GB2PlayerCharForcedLODLevelCinematic,
	TEXT("Forced LOD level for BladeII player character meshes in cinematic scene, intended to be controlled by Scalability system."),
	ECVF_Scalability
);
// 傈捧侩 LOD 1 ScreenSize 绰 厚背利 奴 蔼栏肺 霖促. LOD1 狼 倾侩 ScreenSize 啊 目龙荐废 辟芭府俊辑 LOD1 捞 利侩.
float GB2PlayerCharLOD1ScreenSizeCombat = 0.5f;
FAutoConsoleVariableRef CVarB2PlayerCharLOD1ScreenSizeCombat(
	TEXT("r.B2PlayerCharLOD1ScreenSizeCombat"),
	GB2PlayerCharLOD1ScreenSizeCombat,
	TEXT("LOD 1 ScreenSize for BladeII player character meshes in combat scene, intended to be controlled by Scalability system."),
	ECVF_Scalability
);
// 楷免侩 LOD 1 ScreenSize 绰 厚背利 累篮 蔼栏肺 霖促. LOD1 狼 倾侩 ScreenSize 啊 累酒龙荐废 盔芭府俊辑 LOD1 捞 利侩.
float GB2PlayerCharLOD1ScreenSizeCinematic = 0.1f;
FAutoConsoleVariableRef CVarB2PlayerCharLOD1ScreenSizeCinematic(
	TEXT("r.B2PlayerCharLOD1ScreenSizeCinematic"),
	GB2PlayerCharLOD1ScreenSizeCinematic,
	TEXT("LOD 1 ScreenSize for BladeII player character meshes in cinematic scene, intended to be controlled by Scalability system."),
	ECVF_Scalability
);

void ABladeIIPlayer::SetupLODInfoForInGame()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupLODInfoForInGame);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupLODInfoForInGame"));
	
	// Scalability 矫胶袍俊 蝶弗 LOD 汲沥 炼例. ForceLOD 趣篮 LODScreenSize
	SetB2PCMergedMeshCompLODInfoInternal(GetBaseMesh(), 0.0f, GB2PlayerCharLOD1ScreenSizeCombat);
	GetBaseMesh()->SetForcedLOD(GB2PlayerCharForcedLODLevelCombat);
}
void ABladeIIPlayer::SetupLODInfoForCinematic()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupLODInfoForCinematic);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupLODInfoForCinematic"));

#if !UE_BUILD_SHIPPING
	// 捞 内靛 磊眉绰 荤角惑 BladeIIScalabilityOverride 绰 酒聪促.
	// 窜, 己拜捞 绢蠢 沥档 厚搅窍绊 楷免 柳涝 矫 spike 盒籍 殿俊档 鞘夸秦辑.. 咯扁档 bUseBladeIIScalabilityOverride 俊 蝶扼 霸烙敲饭捞 侩 皋浆 LOD 汲沥苞 付蛮啊瘤肺 哎 荐 乐档废 窃.
	/*if (!B2Scalability::bUseBladeIIScalabilityOverride) {
		SetupLODInfoForInGame();
		return;
	}*/
#endif

	// SetUseSectionMergedMesh 技泼档 咯扁俊 尝况持扁 利例秦 焊老 荐 乐瘤父 弊矾瘤 富 巴.
	// 弊扒 惑炔俊 蝶扼 粱 歹 促福霸 甸绢埃促.

	// Scalability 矫胶袍俊 蝶弗 LOD 汲沥 炼例. ForceLOD 趣篮 LODScreenSize
	// 荤角 楷免俊辑 焊咯瘤绰 扒 皋浆 府家胶绰 咯扁 铂 傈崔秦 林摆瘤父 哪器惩飘绰 促福聪 ForceLOD 绰 弊率栏肺 傈崔秦 林绢具 茄促.
	SetB2PCMergedMeshCompLODInfoInternal(GetBaseMesh(), 0.0f, GB2PlayerCharLOD1ScreenSizeCinematic);
	GetBaseMesh()->SetForcedLOD(GB2PlayerCharForcedLODLevelCinematic);
}
void ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(class ASkeletalMeshActor* InLobbySKActor)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupLODInfoForLobbyRepresentative);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupLODInfoForLobbyRepresentative"));

	check(InLobbySKActor);
	//
	//// 肺厚俊辑绰 檬扁俊 Merge 焊促绰 Component 备己阑 积阿窍绊 乐菌扁 锭巩俊 葛滴 啊廉坷霸 沁绰单.. 搬惫 弊犯霸 救瞪 淀.
	//// 荤角 Component attach 备己阑 窍霸 登搁 酒贰 SetB2PCMergedMeshCompLODInfoInternal 阑 荤侩窍扁啊 帮鄂秦柳促. 繁鸥烙 积己等 皋浆啊 酒聪扼 烙器飘 等 府家胶甫 扒靛府霸 瞪 巴捞骨肺.
	//TArray<UActorComponent*> AllSKComps = InLobbySKActor->GetComponentsByClass(USkeletalMeshComponent::StaticClass());

	//for (UActorComponent* CurrComp : AllSKComps)
	//{
	//	USkeletalMeshComponent* CurrSKComp = Cast<USkeletalMeshComponent>(CurrComp);
	//	// 肺厚档 Cinematic 汲沥阑 啊廉埃促.		
	//	SetB2PCMergedMeshCompLODInfoInternal(CurrSKComp, 0.0f, GB2PlayerCharLOD1ScreenSizeCinematic); // ForcedLOD 0 捞搁 利侩.
	//	if (CurrSKComp)
	//	{
	//		CurrSKComp->SetForcedLOD(GB2PlayerCharForcedLODLevelCinematic); // 肺厚档 CinematicLODLevel 阑 利侩
	//	}
	//}
}
void ABladeIIPlayer::OnBeginWeaponSkillLOD0()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OnBeginWeaponSkillLOD0"));

	checkSlow(IsLocalPlayer()); // 酒囱 版快 乐绰瘤..?
	//
	//// 牢霸烙 傈捧俊辑绰 冀记 捍钦 葛胆阑 静瘤父 楷免俊辑绰 酒囱 版快.. 老矫利栏肺 冀记 盒府 葛胆 荤侩 技泼.
	//if (/*UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this) &&*/ // 夸 炼扒篮 救 焊扁肺.. 如摹 臼篮单 惑怕啊 部捞绰瘤 搬加胶懦俊辑 捍钦等 葛胆捞 唱坷绰 版快啊 乐促.
	//	!UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this))
	//{
	//	SetUseSectionMergedMesh(false);
	//}

	//SetupLODInfoForCinematic(); // 楷免侩 LOD 悸诀篮 捞傈何磐 窍带 扁夯.
	//
	//DestroyWeaponSkillLOD0SceneLight();
	//CreateWeaponSkillLOD0SceneLight();

	//bInWeaponSkillLOD0 = true;

	//FTimerManager& WTM = GetWorldTimerManager();
	//WTM.ClearTimer(WeaponSkillEndHandlingForceTH);
	//// 捞繁 窿阑 窍瘤 臼酒档 OnEndWeaponSkillLOD0 啊 阂妨具 窍瘤父 父老阑 困茄 救傈厘摹.
	//WTM.SetTimer(WeaponSkillEndHandlingForceTH, this, &ABladeIIPlayer::ForceFinishWeaponSkillLOD0SceneSetup
	//	, 30.0f/*捞 捞惑 啊绰 搬加胶懦捞 乐促搁 函怕摆瘤.*/
	//	, false);
}
void ABladeIIPlayer::OnEndWeaponSkillLOD0()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OnEndWeaponSkillLOD0"));

	//bInWeaponSkillLOD0 = false;

	//// 楷免侩 技泼甸 府悸 
	//if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
	//{ // 咯变 AnimNotify 甫 烹秦 惯悼登绰 镑捞促 焊聪 官肺 SetUseSectionMergedMesh 窍搁 UpdateAnimation 捞 犁蓖龋免 登绢滚府绰 荤怕啊 惯积茄促
	//	DeferredSetUseSectionMergedMesh(true);
	//}
	//SetupLODInfoForInGame(); 
	//DestroyWeaponSkillLOD0SceneLight();

	//GetWorldTimerManager().ClearTimer(WeaponSkillEndHandlingForceTH);

	//DimensionBuffCheckClass<>::GetInstance().Signal();
}
void ABladeIIPlayer::ForceFinishWeaponSkillLOD0SceneSetup()
{
	OnEndWeaponSkillLOD0();
}

#if !UE_BUILD_SHIPPING
float GDevTestWeaponSkillLOD0LightIntensity = -1.0f;
float GDevTestWeaponSkillLOD0LightRadius = -1.0f;
#endif
void ABladeIIPlayer::CreateWeaponSkillLOD0SceneLight()
{
	/* =============================================
	捞 窿阑 窍绰 格利篮 搬加胶懦 矫傈 矫 喊档 扼捞飘 技泼阑 窍瘤 臼栏搁
	赣府 困俊 乐绰 PointLight 啊 倔奔阑 困俊辑 厚眠扁 锭巩俊 沥富 官焊鞍篮 炼疙弧捞 唱坷扁 锭巩捞促.
	概 搬加胶懦 喊肺 技技茄 技泼阑 力傍窍绰 扒 老捞 呈公 目瘤绊 绢蠢 沥档 利寸茄 炼疙捞 唱坷档废 弥家茄狼 贸府客 技泼 力傍.
	============================================= */

	check(IsLocalPlayer());

	static float WeaponSkillLOD0LightIntensity = -1.0f;
	static float WeaponSkillLOD0LightRadius= -1.0f;
	if (WeaponSkillLOD0LightIntensity <= 0.0f || WeaponSkillLOD0LightRadius <= 0.0f)
	{
		GConfig->GetFloat(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("WeaponSkillLOD0LightIntensity"), WeaponSkillLOD0LightIntensity, GGameIni);
		GConfig->GetFloat(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("WeaponSkillLOD0LightRadius"), WeaponSkillLOD0LightRadius, GGameIni);
	}

	WeaponSkillLOD0SceneLight = NewObject<UPointLightComponent>(this, NAME_None, RF_Transient);
	if (WeaponSkillLOD0SceneLight)
	{
		// 技泼阑 芭芒窍霸 力傍且 扒 酒聪瘤父 胶懦喊肺 割啊瘤 加己 沥档绰 力傍窍霸 瞪 荐档 乐促.
		WeaponSkillLOD0SceneLight->Intensity = (WeaponSkillLOD0LightIntensity > 0.0f) ? WeaponSkillLOD0LightIntensity : 50000.0f;
		WeaponSkillLOD0SceneLight->LightColor = FColor(255, 255, 255);
		WeaponSkillLOD0SceneLight->AttenuationRadius = (WeaponSkillLOD0LightRadius > 0.0f) ? WeaponSkillLOD0LightRadius : 1500.0f;
		WeaponSkillLOD0SceneLight->bAffectsWorld = true;

#if !UE_BUILD_SHIPPING
		if (GDevTestWeaponSkillLOD0LightIntensity > 0.0f)
		{
			WeaponSkillLOD0SceneLight->Intensity = GDevTestWeaponSkillLOD0LightIntensity;
		}
		if (GDevTestWeaponSkillLOD0LightRadius > 0.0f)
		{
			WeaponSkillLOD0SceneLight->AttenuationRadius = GDevTestWeaponSkillLOD0LightRadius;
		}
#endif

		// 困摹绰 UpdateWeaponSkillLOD0LightComponentAttachment 俊辑 盎脚凳.
		//WeaponSkillLOD0SceneLight->RelativeLocation = FVector(500.0f, 0.0f, 0.0f); // 捞扒 Tick 俊辑 促矫..
		WeaponSkillLOD0SceneLight->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		WeaponSkillLOD0SceneLight->RegisterComponent();
	}
}
void ABladeIIPlayer::DestroyWeaponSkillLOD0SceneLight()
{
	if (WeaponSkillLOD0SceneLight)
	{
		WeaponSkillLOD0SceneLight->Deactivate();
		WeaponSkillLOD0SceneLight->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
		WeaponSkillLOD0SceneLight->UnregisterComponent();
		WeaponSkillLOD0SceneLight->DestroyComponent();
		WeaponSkillLOD0SceneLight = nullptr;
	}
}
void ABladeIIPlayer::UpdateWeaponSkillLOD0LightComponentAttachment()
{
	check(IsLocalPlayer() && bInWeaponSkillLOD0);

	static FVector WeaponSkillLOD0LightRelPos(0.0f,0.0f,0.0f);
	static bool bWeaponSkillLOD0LightRelPosLoaded = false;
	if (!bWeaponSkillLOD0LightRelPosLoaded)
	{
		GConfig->GetVector(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("WeaponSkillLOD0LightRelPos"), WeaponSkillLOD0LightRelPos, GGameIni);
		bWeaponSkillLOD0LightRelPosLoaded = true;
	}

	USkeletalMeshComponent* SKMeshComp = GetMesh();
	if (WeaponSkillLOD0SceneLight && SKMeshComp)
	{
		// UpdateLightComponentAttachment 俊辑 窍绰 扁夯 PointLight 困摹 盎脚苞 厚搅窍霸 概 平 漂沥 夯阑 蝶扼啊瘤父 
		// 焊促 泅鄂茄 框流烙阑 且 荐 乐栏骨肺 窜鉴洒 Z 蔼父 持瘤绰 臼绰促.

		const FTransform BaseBoneTransform = SKMeshComp->GetBoneTransform(SKMeshComp->GetBoneIndex(B2PLAYER_CENTERBONE_NAME));
		const FVector DesiredWorldLoc = BaseBoneTransform.TransformPosition(WeaponSkillLOD0LightRelPos);
		WeaponSkillLOD0SceneLight->SetWorldLocation(DesiredWorldLoc);
	}
}

EPCClass ABladeIIPlayer::GetTagPlayerClass()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetTagPlayerClass);
	// SkillRTManager 蔼阑 啊廉坷骨肺 destroy 措扁 惑怕牢 怕弊 吝狼 OldPlayer 俊霸辑绰 狼固啊 绝澜.
	return /*GetSkillRTManager() ? CachedSkillRTManager->GetTagPlayerClass() :*/ EPCClass::EPC_End;
}

EPCClass ABladeIIPlayer::GetCurrentPlayerClass() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCurrentPlayerClass);
	//return UB2PCClassInfo::FromCharacterTypeToPCClass(this->CharacterType);
	return /*GetSkillRTManager() ? CachedSkillRTManager->GetTagPlayerClass() :*/ EPCClass::EPC_End;
}

bool ABladeIIPlayer::AmIMainCharacter()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_AmIMainCharacter);
	
	// 辟单 绢瞒乔 possess 救等 惑怕俊辑 GetSkillRTManager 啊 null 捞搁 翠捞 绝焙.. ぬば
	//return GetSkillRTManager() ? CachedSkillRTManager->IsMainPlayerCharacter(this) : false;
	return  false;
}

void ABladeIIPlayer::NotifyTagRetire(ABladeIIPlayer* NewTaggedPlayer)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyTagRetire);
	check(NewTaggedPlayer);

	OnEnterSkillState_Tag(NewTaggedPlayer->GetCurrentPlayerClass());
}

void ABladeIIPlayer::NotifyTagSpawn(class ABladeIIPlayer* OldRetiringPlayer)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyTagSpawn);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::NotifyTagSpawn"));
	check(OldRetiringPlayer);

	TakeOverAutoFromPredecessor(OldRetiringPlayer);

	// 傍拜 档吝俊 tag 甫 沁促搁 捞绢辑 傍拜阑 拌加 窍档废 盒畴炼例厘局甫 蜡惯.
	if (OldRetiringPlayer->IsKeepAttacking())
		SetKeepAttacking(true);

	// 傍拜 规绢 滚瓢惑怕 牢拌
	if (OldRetiringPlayer->IsAttackKeyPressed())
	{
		StartAttackFromInput();
	}

	if (OldRetiringPlayer->IsGuardKeyPressed())
	{
		StartGuardFromInput();
	}

	SetTeamNum(OldRetiringPlayer->GetTeamNum());

	//// RAID AspectRatioAdaptiveFOV, 20180716
	//// 漂沥 葛靛俊辑狼 傈捧甘 叼磊牢 荤沥栏肺 AspectRatioAdaptiveFOV 甫 荤侩窍绰 版快啊 乐澜.
	//if (OldRetiringPlayer->TopDownCameraComponent && OldRetiringPlayer->TopDownCameraComponent->GetAspectRatioAdaptiveFOV())
	//{
	//	if (TopDownCameraComponent)
	//	{
	//		TopDownCameraComponent->SetAspectRatioAdaptiveFOV(true);
	//	}
	//}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 扁夯 摹飘甸 促矫 怕绢唱档 蜡瘤.. 概锅 媚林妨聪 阂祈窍焙
	this->bCheatImmortalMode = OldRetiringPlayer->bCheatImmortalMode;
	this->bCheatHeroMode = OldRetiringPlayer->bCheatHeroMode;
#endif

	//PlayerTaggedClass<ABladeIIPlayer*, ABladeIIPlayer*>::GetInstance().Signal(OldRetiringPlayer, this);
}

void ABladeIIPlayer::OnSpawnEnd()
{
	/*if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		if (DocBattle->GetNeedResurrectBuffText())
		{
			SpawnResurrectBuffTextEffect();
			DocBattle->SetNeedResurrectBuffText(false);
		}
	}	*/
}

void ABladeIIPlayer::NotifyBornAgain(class ABladeIIPlayer* PrevLife)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyBornAgain);
	// 捞扒 Tag 客 付蛮啊瘤狼 UI 局聪皋捞记阑 狼档茄 巴牢单, 酒流 犬沥等 官绰 绝澜. 氢饶 沥秦瘤绰 官俊 蝶扼 捞犯霸 啊带瘤, 刀磊利 备泅阑 窍带瘤.
	// NotifyTagSpawn 阑 流立 阂矾林瘤 臼霸 登搁 弊率俊 备泅茄 巴甸阑 鞘夸俊 蝶扼 捞率俊档
	NotifyTagSpawn(PrevLife);
}

void ABladeIIPlayer::TakeOverAutoFromPredecessor(class ABladeIIPlayer* InPredecessor)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TakeOverAutoFromPredecessor);
	check(InPredecessor);
	// 何劝, 怕弊 殿狼 惑炔. 咯变 肺彬敲饭捞绢 挥 酒聪扼 匙箂敲饭捞绢档 甸绢坷骨肺 GameMode 率 Auto 惑怕甫 荤侩窍搁 救凳.
	if (InPredecessor->IsInAutoPlay()) 
	{
		// Let the new player continue auto play with the same AutoAIController
		StartAutoPlay();
	}
}

EPCSpawnMotionState ABladeIIPlayer::GetSpawnMotionState()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetSpawnMotionState);
	//// 角力 惑怕 函荐绰 Spawn 流饶 Animation 诀单捞飘 鸥捞怪阑 绊妨窍咯 Player Controller 率俊 乐澜
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(Controller);
	//if (B2PC == NULL)
	//{
	//	// 捞巴捞 阂府绰 矫痢捞 spawn 贸府甫 窍绰 档吝捞扼 固贸 possess 啊 救登菌阑 荐 乐澜. 绢驴 荐 绝捞..
	//	// GetNearestPlayerController 甫 窍搁 弥家茄 tag 唱 resurrect 矫痢俊辑 力芭登扁 流傈狼 捞傈 敲饭捞绢俊 possess 等 巴阑 啊廉棵 巴. (B2SkillRTManager 率 曼绊)
	//	// 弊矾唱 父老 敲饭捞绢 某腐磐啊 咯返牢单 钢府 冻绢柳 镑俊 何劝阑 秦具 窍绰 惑炔 殿捞 坷搁 促矫 积阿秦 毫具 窃.
	//	B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetNearestPlayerController(this, this->GetActorLocation()));
	//}

	return /*B2PC ? B2PC->GetPCSpawnMotionState() :*/ (EPCSpawnMotionState)EPCSpawnMotionState::EPST_Normal;
}

void ABladeIIPlayer::EndSpawnMotionState()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_EndSpawnMotionState);
//	// 角力 惑怕 函荐绰 Spawn 流饶 Animation 诀单捞飘 鸥捞怪阑 绊妨窍咯 Player Controller 率俊 乐澜
//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(Controller);
//	if (B2PC)
//	{
//		B2PC->SetPCSpawnMotionState(EPCSpawnMotionState::EPST_Normal);
//
////		EnableInput(B2PC);
//	}
}

void ABladeIIPlayer::OnDeadAnimBegin()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnDeadAnimBegin);
	// 圈肺 巢败廉 绊刀茄 弥饶甫 嘎篮 磊狼 技饭葛聪. 绊烹苞 荐摹肺 啊垫蛮 捞 矫埃捞 瘤唱绊 唱搁 货肺款 矫累阑 困茄 何劝 皋春啊 躲 巴捞促.
	// OnDeath 俊 捞吧 何福搁 角力 局聪皋捞记 惑怕绰 傈捧 吝老 荐 乐栏骨肺 Dead 局聪皋捞记俊 notify 甫 烹秦 何福档废 窃.
	//UGameplayStatics::SetGlobalTimeDilation(this, OnDeathTimeDilation); // 捞扒 老窜 荤侩 救窃.
	//GetWorldTimerManager().SetTimer(OnDeathTimeDilationTH, this, &ABladeIIPlayer::OnDeathTimeDilationTimerCB, OnDeathTimeDilationLength, false);
	//APlayerController* PCont = Cast<APlayerController>(Controller);
	//if (PCont){
		//PCont->PlayDynamicForceFeedback(OnDeathVibrationIntensity, OnDeathVibrationLength, true, true, true, true, EDynamicForceFeedbackAction::Start, FLatentActionInfo());
	//}
	ForceFeedback();
}

void ABladeIIPlayer::SuckedDropItem(class AB2DropBase* InSuckedDrop)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SuckedDropItem);
	// Anything necessary. 啊瓷窍搁 阿 窍困 Drop 努贰胶狼 OnPlayerSuckedMe 俊辑 贸府.
}

int32 ABladeIIPlayer::GetPlayerLevel()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetPlayerLevel);

	return /*CharacterDataStore ? CharacterDataStore->GetCharacterLevel(GetCurrentPlayerClass()) :*/ 1;
}

void ABladeIIPlayer::SyncPCCharacterLevelFromExpManager()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SyncPCCharacterLevelFromExpManager);
	// 弥家茄 BeginPlay 矫痢俊 捞霸 阂妨具 窃. InitializeCombatStats 捞傈俊
	CharacterLevel = GetPlayerLevel();
}

void ABladeIIPlayer::OnSomeMobTermination(ABladeIICharacter* InMobAboutToDestroy)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnSomeMobTermination);
	if (QTEActor && QTEActor == InMobAboutToDestroy) 
	{
		//UE_LOG(LogBladeII, Warning, TEXT("Calling FinalKillQTETarget on QTE target Actor's termination. This is not a desired path."));
		// QTE 鸥百 各 荤噶 贸府甫 捞率辑 刚历 秦 林绰 霸 亮篮单 QTE 各狼 DeadEnd 啊 刚历 坷绊 LifeSpan 付历 促 茄 惑炔捞 积扁帮 秦辑.. 救傈 瞒盔俊辑 贸府
		FinalKillQTETarget();
	}
}

void ABladeIIPlayer::SignalStopAttackImmediate()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SignalStopAttackImmediate);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SignalStopAttackImmediate"));

	bStopAttackSignal = true; // 捞饶 AnimBP 俊辑 贸府等 饶 府悸.

	TryRestoreFromCameraDistZoomNS(); // CameraDistZoom 捞 甸绢啊绊 乐带 惑炔捞扼搁 弊巴档 吝窜.

	StopRepeatingInput();
}

bool ABladeIIPlayer::ConsumeAndCheckStopAttackMotion()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ConsumeAndCheckStopAttackMotion);
	// 茄锅 荤侩窍搁 动.
	bool bRetVal = bStopAttackSignal;
	bStopAttackSignal = false;
	return bRetVal;
}

float ABladeIIPlayer::GetMoveAcceptableRadius()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetMoveAcceptableRadius);
	return MoveToMobAcceptableRadius;
}

float ABladeIIPlayer::GetDamageIncreaseRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDamageIncreaseRate);
	EPCClass CurrentPlayerClass = GetCurrentPlayerClass();
	float Ret = Super::GetDamageIncreaseRate() + IncDamageValueByPassive; // 傍拜 惑怕俊 狼粮窍瘤 臼绰 扁夯利牢 单固瘤 棺 刘啊 可记 荐摹
	
	//// 泅犁 胶懦俊 蝶扼 馆康登绰 Active 胶懦 可记 单固瘤 刘啊 荐摹
	//if (CachedSkillRTManager)
	//{
	//	Ret += IsCounterAttacking() ? CachedSkillRTManager->GetSkillDamageIncreaseRateByActive(EAttackState::ECS_Counter01) :
	//		CachedSkillRTManager->GetSkillDamageIncreaseRateByActive(GetAttackState());
	//}

	//if (IsSkillAttacking())
	//{
	//	Ret += IncSkillDamageValueByPassive;

	//	// 捞巴甸篮 漂沥 胶懦 锅龋俊 利侩登瘤父 Active 酒囱 Passive 己拜..
	//	switch (GetAttackState())
	//	{
	//	case EAttackState::ECS_Skill01:
	//		Ret += CombatStatEval::GetUnitedOptionStatusValue(CurrentPlayerClass, EUnitedCombatOptions::UCO_Skill_IncSkill1Damage, CharacterDataStore);
	//		break;
	//	case EAttackState::ECS_Skill02:
	//		Ret += CombatStatEval::GetUnitedOptionStatusValue(CurrentPlayerClass, EUnitedCombatOptions::UCO_Skill_IncSkill2Damage, CharacterDataStore);
	//		break;
	//	case EAttackState::ECS_Skill03:
	//		Ret += CombatStatEval::GetUnitedOptionStatusValue(CurrentPlayerClass, EUnitedCombatOptions::UCO_Skill_IncSkill3Damage, CharacterDataStore);
	//		break;
	//	case EAttackState::ECS_Skill05:
	//		Ret += CombatStatEval::GetUnitedOptionStatusValue(CurrentPlayerClass, EUnitedCombatOptions::UCO_Skill_IncSkill5Damage, CharacterDataStore);
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//else if(!IsCounterAttacking())
	//{ // 老馆傍拜俊父 利侩登绰 单固瘤 刘啊.
	//	Ret += IncNormalDamageValueByPassive;
	//}

	//// Boss Damage Increase handled in TakeDamage
	//// TODO: Raid Damage Increase
	//// TODO: PVP Damage Increase

	return Ret;
}

float ABladeIIPlayer::GetBossDamageIncreaseRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBossDamageIncreaseRate);
	// Passive + AttackState 俊 蝶弗 Active 可记 荐摹 钦.
	//return IncDamageToBossValueByPassive + (CachedSkillRTManager ? CachedSkillRTManager->GetBossDamageIncreaseRateByActive(GetAttackState()) : 0.f);
	return 0.0f;
}

float ABladeIIPlayer::GetBonusBuffDamageDecreaseAmount() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffDamageDecreaseAmount);
	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_DecReceiveDamage) : */0.f;
}

float ABladeIIPlayer::GetBonusBuffDuration() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffDuration);
	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_BuffTime) :*/ 0.f;
}

float ABladeIIPlayer::GetBuffReflectionDamageRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBuffReflectionDamageRate);
	return/* CachedSkillRTManager ? 1.f + CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncReflectDamageOnBuff) :*/ 1.f;
}

float ABladeIIPlayer::GetBonusBuffHealHPByHit() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffHealHPByHit);
	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_HealHPOnHitOnBuff) :*/ 0.f;
}

float ABladeIIPlayer::GetBonusBuffHealHP() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffHealHP);
	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_HealHP) : */0.f;
}

float ABladeIIPlayer::GetBonusBuffIncMoveSpeed() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffIncMoveSpeed);
	//return CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncMoveSpeedDuring3Sec) : 0.f;
	return 0.f;
}

float ABladeIIPlayer::GetDamageRadiusIncreaseRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDamageRadiusIncreaseRate);
	//return CachedSkillRTManager ? 1.f + CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncArea) : 1.f;
	return 0.f;
}

float ABladeIIPlayer::GetBonusBuffReceiveDamageDecreaseRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffReceiveDamageDecreaseRate);
	//return CachedSkillRTManager ? FMath::Clamp(1.f - CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_DecReceiveDamageOnBuff), 0.f, 1.f) : 1.f;
	return 0.f;
}

float ABladeIIPlayer::GetBonusBuffAttackDamageIncreaseRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffAttackDamageIncreaseRate);
	//return CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncAttackDamageOnBuff) : 0.f;
	return 0.f;
}

float ABladeIIPlayer::GetAttackDamageIncreaseRateByClass(EPCClass TargetClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetAttackDamageIncreaseRateByClass);
	//return (TargetClass != EPCClass::EPC_End) ? IncAttackDamageByPassiveToClass[PCClassToInt(TargetClass)] : 0.0f;
	return 0.f;
}

float ABladeIIPlayer::GetReceiveDamageDecreaseRateByClass(EPCClass AttackerClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetReceiveDamageDecreaseRateByClass);
	//return (AttackerClass != EPCClass::EPC_End) ? DecReceiveDamageByPassiveFromClass[PCClassToInt(AttackerClass)] : 1.0f; // Dec 扼 扁夯蔼 1.0
	return 0.f;
}

int32 ABladeIIPlayer::GetBonusBuffAffectNum() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffAffectNum);
	//return CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncMaxNumCharacterBuff) : 0;
	return 0.f;
}

bool ABladeIIPlayer::CanBeTarget() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CanBeTarget);
	return IsAlive() && !IsBuffActive(EBuffType::Buff_Hide) && GetAttackState() != EAttackState::ECS_WeaponSkill;
}

// Team Buff
float ABladeIIPlayer::GetBonusTeamBuffRate(ETeamBuffType BuffType) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusTeamBuffRate);
	//if (CachedSkillRTManager)
	//{
	//	switch (BuffType)
	//	{
	//	case ETeamBuffType::ETBT_Shield:
	//		return CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncProtectHPRateOnBuff);
	//	case ETeamBuffType::ETBT_IncreaseCriticalRate:
	//		return CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncCriticalRateOnBuff);
	//	case ETeamBuffType::ETBT_IncreaseCriticalDamage:
	//		return CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncCriticalDamageOnBuff);
	//	default:
	//		break;
	//	}
	//}
	return 0.f;
}

float ABladeIIPlayer::GetAdditionalCriticalRateByComboNum() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetAdditionalCriticalRateByComboNum);
	// GetCriticalRate 狼 老何牢单 捞 荐摹绰 漂喊洒 UI 率俊档 谎妨林骨肺 蝶肺 画
	return 0.01f * FMath::Min( FMath::Max(0, CurrentComboNum -1) / 10, 3);
}

float ABladeIIPlayer::GetCriticalRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCriticalRate);
	//////////////////////////////////////////////////
	// ! 傈捧 可记 荐摹 利侩 矫 Inc** 绰 歹窍绊 Dec** 绰 蚌窃. GetEffectiveValueForOptionApplyType 曼绊.

	float CriticalRate = Super::GetCriticalRate();

	//CriticalRate += GetAdditionalCriticalRateByComboNum()
	//	+ IncCriticalRateValueByPassive // ItemOption 棺 passive SkillOption 俊 狼茄 荐摹
	//	+ (CachedSkillRTManager ? CachedSkillRTManager->GetCriticalRateByActive(GetAttackState()) : 0.f); // 泅犁 AttackState 俊 蝶弗 active skill 可记 荐摹.
	//
	return CriticalRate;
}

float ABladeIIPlayer::GetCriticalDamageRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCriticalDamageRate);
	// TODO: need to check equipment option

	float CriticalDamageRate = Super::GetCriticalDamageRate();

	//CriticalDamageRate += IncCriticalDamageValueByPassive // ItemOption 棺 passive SkillOption 俊 狼茄 荐摹
	//	+ (CachedSkillRTManager ? CachedSkillRTManager->GetCriticalDamageRateByActive(GetAttackState()) : 0.f); // 泅犁 AttackState 俊 蝶弗 active skill 可记 荐摹.

// 	if (CurrentTeamBuffType & static_cast<uint8>(ETeamBuffType::ETBT_IncreaseCriticalDamage))
// 	{
// 		CriticalDamageRate += TeamBuffStates[UB2BuffModeEffectInfo::GetTeamBuffIndex(ETeamBuffType::ETBT_IncreaseCriticalDamage)].Amount;
// 	}

	return CriticalDamageRate;
}

float ABladeIIPlayer::GetDamageDecreaseRate() const
{
	return Super::GetDamageDecreaseRate() * BaseDamageDecreaseRate;
}

float ABladeIIPlayer::GetDurationDecreaseRate() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDurationDecreaseRate);
	return StateDurationDecreaseRate;
}

//class UB2AnimInstance* ABladeIIPlayer::GetAnimInstance() const
//{
//	if (GetMesh() != nullptr)
//	{
//		return Cast<UB2AnimInstance>(GetMesh()->GetAnimInstance());
//	}
//
//	return nullptr;
//}

const EAttackState ABladeIIPlayer::GetAnimAttackState() const
{
	//if (auto* AnimInst = GetAnimInstance())
	//	return AnimInst->GetAnimAttackState();

	return GetAttackState();
}

const EAttackType ABladeIIPlayer::GetDamageAnimType() const
{
	//if (auto* AnimInst = GetAnimInstance())
	//	return AnimInst->GetDamageAnimIndex();

	return LastDamageType;
}

float ABladeIIPlayer::GetStaticOptionAppliedWalkSpeed() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetStaticOptionAppliedWalkSpeed);
	//if (CharacterDataStore)
	//{
	//	const float OptionMoveSpeedIncRate = CombatStatEval::GetUnitedOptionStatusValue(GetCurrentPlayerClass(), EUnitedCombatOptions::UCO_Misc_IncMoveSpeed, CharacterDataStore);

	//	return CachedBaseMaxWalkSpeed * (1.0f + OptionMoveSpeedIncRate);
	//}
	return CachedBaseMaxWalkSpeed;
}

float ABladeIIPlayer::GetOptionStatusValue(EItemOption OptionType) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetOptionStatusValue);
	// 啊鞭利 SkillOption 苞 烹钦等 GetUnitedOptionStatusValue 狼 荤侩阑 绊妨. 瞒瞒 傈券..
	//return CharacterDataStore->GetOptionStatusValue(GetCurrentPlayerClass(), OptionType);
	return 0.0f;
}

float ABladeIIPlayer::GetUnitedOptionStatusValue(EUnitedCombatOptions OptionType) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetUnitedOptionStatusValue);
	// 啊鞭利 捞 窃荐狼 龋免篮 某腐磐 spawn 檬扁拳 矫俊 茄沥阑.. 鞘夸茄 蔼篮 某教秦 初绊 静绰 吧 鼻厘.
	//return CombatStatEval::GetUnitedOptionStatusValue(GetCurrentPlayerClass(), OptionType, CharacterDataStore);
	return 0.0f;
}

void ABladeIIPlayer::AddEtherSetEffect(int32 EtherSetID, float TriggerRate, float CoolTime)
{
	//if (GetEtherContainer() && IsAlive())
		//GetEtherContainer()->AddEtherSetEffect(EtherSetID, TriggerRate, CoolTime);
}

//UB2EtherSetEffect* ABladeIIPlayer::GetEtherSetEffect(EEtherSetType SetType)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetEtherSetEffect);
//
//	BII_CHECK(GetEtherContainer());
//
//	switch (SetType)
//	{
//	case EEtherSetType::Offense: return GetEtherContainer()->GetOffenseEtherSet();
//	case EEtherSetType::Defense: return GetEtherContainer()->GetDefenseEtherSet();
//	}
//
//	return nullptr;
//}


bool ABladeIIPlayer::ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ProcessEtherSetEffect);

	//UB2EtherSetEffect* EtherObject = GetEtherSetEffect(EtherSetType);
	//if (Role == ROLE_Authority && EtherObject)
	//{
	//	if (EtherObject->IsActivatable(EtherCauser, CauserDamageInfo, ActualDamage))
	//	{
	//		EtherObject->ActivateEther(EtherCauser);
	//		return true;
	//	}
	//}

	return false;
}

void ABladeIIPlayer::TotalApplyAnimRateScale()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TotalApplyAnimRateScale);
	if (GetMesh())
	{
		GetMesh()->GlobalAnimRateScale = (bReachedAtDeadEnd ? 0.0f : ( // DeadEnd 俊辑狼 沥瘤惑怕
			IsAttacking() ? (1.0f + AdditionalAttackSpeedRatebyPassive) : // 傍拜 惑怕俊辑父 可记 利侩等 蔼栏肺. Active 可记捞唱 Buff 康氢档 咯扁辑 利侩窍搁 瞪 淀..
				1.0f
			));
	}
}

void ABladeIIPlayer::ForceCounterAttack()
{
	bForceCounterAttack = true;
}

void ABladeIIPlayer::OnDeathTimeDilationTimerCB()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnDeathTimeDilationTimerCB);
	GetWorldTimerManager().ClearTimer(OnDeathTimeDilationTH);
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f); // 被捞 OnDeathTimeDilation 柳涝 捞傈 蔼阑 某教秦 初阑 鞘夸绰 绝阑 巴 鞍促..
}

void ABladeIIPlayer::ReserveQTEActorReportMyDeathToBirthplace()
{
	// 啪铰 肯丰矫痢俊辑 胶迄钱率浚 磷澜栏肺 贸府
	QTEActor->ConditionalReportMyDeathToBirthplace();
}

void ABladeIIPlayer::OnQTEMountBegin()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTEMountBegin);
	bool bUseSafeEnableDistance = GetUseQTESafeEnebleDistance();
	ABladeIICharacter* pQTETarget = FindQTEEnableTarget(bUseSafeEnableDistance);
	
	if (!pQTETarget)
		return;
	
	// 呕巴 鸥绰悼救 妮府傈冠胶啊 荤扼廉 EndActiveCamera啊 龋免登绊 岿靛墨皋扼肺 函版凳 弊贰辑 癌磊扁 墨皋扼啊 官差绰 泅惑 阜扁
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	FloorRingPSC->SetHiddenInGame(true);

	// 鸥扁 矫累沁栏搁 QTE措扁 辆丰妮归 秒家.
	pQTETarget->CancleQTEEnableEndCB();	
}

void ABladeIIPlayer::OnQTEMountTargetEnd(const FTransform& QTETransform)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTEMountTargetEnd);
	// 咯扁辑 StopQTEMount 茄 饶俊 mount target 狼 AnimNotify 甫 烹秦 坷绰 芭.
	// Changed to called in StopQTEMount
	FVector NewLocation(QTETransform.GetLocation().X, QTETransform.GetLocation().Y, QTEActor->GetMovementComponent()->GetActorFeetLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator::ZeroRotator);
	SetActorLocationAndRotation(NewLocation, FRotator(0.f, GetActorRotation().Yaw, 0.f), true);

	QTEType = EQTEType::EQT_End;
	QTEStateNum = 0;
	//FinalKillQTETarget();

	/*if (GetGameMode()->IsLocalPlayerAutoBattle())
	{
		StartAutoPlay();
	}*/
}

void ABladeIIPlayer::StopQTEMount()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopQTEMount);
	//
	//if (ReserveQTEActorReportMyDeathToBirthplaceTimerHandle.IsValid())
	//{
	//	GetWorldTimerManager().ClearTimer(ReserveQTEActorReportMyDeathToBirthplaceTimerHandle);
	//}

	//if (QTEActor && QTEActor->IsAlive())
	//{
	//	QTEActor->SetActorEnableCollision(false);
	//	QTEActor->GetMovementComponent()->StopMovementImmediately();
	//	QTEActor->Die(QTEActor->Health, FDamageEvent(UDamageType::StaticClass()), GetController(), NULL);

	//	OnQTEMountTargetEnd(QTEActor->GetMesh()->GetSocketTransform(ABladeIIPlayer::QTESocketName));
	//}

	//// SetActorEnableCollision(true);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);

	//// UI康氢篮 肺拿敲饭捞绢父
	//if (IsLocalPlayer())
	//{
	//	auto* DocBattle = GetDocBattle();
	//	if (DocBattle)
	//	{
	//		DocBattle->SetbIsQTEMounted(false);
	//		DocBattle->SetbIsQTEMountedAndReadyForAttack(false/*this->IsQTEMountingAndReadyForAttack()*/);
	//	}
	//}
}

void ABladeIIPlayer::CountQTEMountTime(float InDeltaSecond)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CountQTEMountTime);
	if (QTEMountRemainingTime <= 0.0f) // StopQTEMount 捞饶俊档 咯扁肺 甸绢坷绰 纳捞胶啊 乐澜. 咯扁辑 楼弗促.
		return;

	checkSlow(IsInQTEState() && QTEType == EQTEType::EQT_Mount);

	//QTEMountRemainingTime -= GetDeltaTimeAsOtherWorldObj(InDeltaSecond, this);

	//auto* DocBattle = GetDocBattle();
	//if (DocBattle) 
	//{
	//	//DocBattle->SetRemainingQTEMountTime(FMath::Max(0.0f, QTEMountRemainingTime));
	//}

	//if (QTEMountRemainingTime <= 0.0f) // Playing time is over.
	//{
	//	StopQTEMount(); // 捞傈俊 鸥捞赣 妮归捞菌澜. 咯瓢 捞吧风 场辰促.
	//}
}

void ABladeIIPlayer::SetupDecalCompForLeftShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupDecalCompForLeftShadowCommon);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupDecalCompForLeftShadowCommon"));

	// 盔贰 BladeIIPlayer 磊脚捞扼搁 InDecalCompToUse 绰 LeftFootShadow 老 巴烙. SetupControlledMatineePuppet 扁瓷阑 困秦 概俺函荐肺 罐绰促. 
	// 咯扁辑 哪器惩飘 register 鳖瘤 窍瘤绰 臼绰促.
	if (InDecalCompToUse && AttachParent)
	{
		//InDecalCompToUse->SetHiddenInGame(false);
		InDecalCompToUse->SetRelativeScale3D(FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot));

		InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
		InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_SHADOW_SOCKET_NAME_LF); // 傈侩 socket 捞 乐澜.
	}
}

void ABladeIIPlayer::SetupDecalCompForRightShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupDecalCompForRightShadowCommon);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupDecalCompForRightShadowCommon"));

	// 盔贰 BladeIIPlayer 磊脚捞扼搁 InDecalCompToUse 绰 RightFootShadow 老 巴烙. SetupControlledMatineePuppet 扁瓷阑 困秦 概俺函荐肺 罐绰促. 
	// 咯扁辑 哪器惩飘 register 鳖瘤 窍瘤绰 臼绰促.

	if (InDecalCompToUse && AttachParent)
	{
		//InDecalCompToUse->SetHiddenInGame(false);
		InDecalCompToUse->SetRelativeScale3D(FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot));

		InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
		InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_SHADOW_SOCKET_NAME_RF); // 傈侩 socket 捞 乐澜.
	}
}

void ABladeIIPlayer::UpdateTargetObject()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateTargetObject);
	//auto* B2SGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//TArray<AB2DestructibleLevelObjBase*> Destructibles = B2SGM->GetDestructibleList();

	//// Could be various conditions, but let's just pick up the closest one.
	//float ClosestDistanceSQ = BIG_NUMBER;
	//AActor* SelectedOne = NULL;

	//for (auto* ThisOne : Destructibles)
	//{
	//	if (!ThisOne || !ThisOne->IsValidObj())
	//		continue;

	//	//const float CurrDistSQ = (mob->GetActorLocation() - PlayerLocation).SizeSquared();
	//	float CurrDistSQ = FLT_MAX;
	//	FHitResult Result;
	//	GetWorld()->LineTraceSingleByChannel(Result, GetActorLocation(), ThisOne->GetActorLocation(), ECC_WorldStatic, FCollisionQueryParams(NAME_None, false, this));
	//	if (Result.bBlockingHit && Result.Actor == ThisOne)
	//	{
	//		if (GetWorld()->GetNavigationSystem()->GetPathLength(GetActorLocation(), ThisOne->GetActorLocation(), CurrDistSQ) == ENavigationQueryResult::Success)
	//		{
	//			if (/*CurrDistSQ < 500 && 鸥百泼 力茄芭府*/CurrDistSQ < ClosestDistanceSQ)
	//			{
	//				ClosestDistanceSQ = CurrDistSQ;
	//				SelectedOne = ThisOne;
	//			}
	//		}
	//	}
	//}
	//TargetObject = SelectedOne;
}
float ABladeIIPlayer::UpdateTarget(bool bInputGuided, bool bOnlyQTEEnabled /* = false */)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateTarget);
	if (IsInQTEState() && !bOnlyQTEEnabled && !IsQTEMounting())
	{
		return 0.f;
	}

	TargetActor = NULL;
	float TargetDist = TargetingDistance;

	//TArray<ABladeIICharacter*> Characters;

	//auto* B2SGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//B2SGM->GetAllWorldEnemyList(Characters, GetTeamNum());

	//for (ABladeIICharacter* ch : Characters)
	//{
	//	if (ch == this)
	//		continue;

	//	if (bOnlyQTEEnabled && !ch->IsTimingForQTEPossession())
	//	{
	//		continue;
	//	}

	//	if (ch->IsInQTEState())
	//	{
	//		continue;
	//	}

	//	if (ch->IsBuffActive(EBuffType::Buff_Hide))
	//	{
	//		continue;
	//	}

	//	FVector Diff = GetTargetLocationVectorFromPlayer(ch);
	//	
	//	float CandidateDist = FLT_MAX;
	//	if (bInputGuided)
	//	{
	//		FVector Direction = GetLastMovementInputVector();
	//		Diff.Z = 0.f;
	//		Direction.Z = 0.f;

	//		if (FVector::DotProduct(Diff, Direction) > 0.f)
	//		{
	//			CandidateDist = FVector::CrossProduct(Diff, Direction).Size();
	//		}
	//		else
	//		{
	//			CandidateDist = Diff.Size2D();
	//		}
	//	}
	//	else
	//	{
	//		CandidateDist = Diff.Size2D();
	//	}

	//	if (/*CandidateDist < 500 && 鸥百泼 力茄芭府*/CandidateDist < TargetDist)
	//	{
	//		/* 荤拱阑 墩绊 傍拜且 荐 乐促绊 秦辑 酒贰 内靛啊 鞘夸绝绢咙..
	//		// Line Trace for visibility check
	//		FVector ViewLoc = GetCapsuleComponent() ? GetActorLocation() + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) : GetActorLocation();
	//		FVector TargetLoc = ch->GetCapsuleComponent() ? ch->GetActorLocation() + FVector(0.f, 0.f, ch->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) : ch->GetActorLocation();

	//		FCollisionQueryParams Params(ABladeIIPlayer::TargetTraceName, false, this);
	//		Params.AddIgnoredActor(ch);
	//		FHitResult Result;
	//		GetWorld()->LineTraceSingleByChannel(Result, ViewLoc, TargetLoc, ECC_Visibility, Params);

	//		if (!Result.bBlockingHit)
	//		*/ 
	//		{
	//			TargetActor = ch;
	//			TargetDist = CandidateDist;
	//		}
	//	}
	//}

	/*
	TArray<AB2DestructibleLevelObjBase*> Destructible = B2SGM->GetDestructibleList();

	float ClosestDistanceSQ = BIG_NUMBER;
	AB2DestructibleLevelObjBase* SelectedOne = NULL;

	for (auto* de : Destructible)
	{
		const float CurrDistSQ = (de->GetActorLocation() - GetActorLocation()).SizeSquared();

		if (CurrDistSQ < ClosestDistanceSQ)
		{
			ClosestDistanceSQ = CurrDistSQ;
			TargetObject = de;
		}
	}
	*/
	return TargetDist;
}

bool ABladeIIPlayer::CanRotateToTarget()
{
	// 1. 辟夯利栏肺 Rotate啊 阂啊瓷且锭
	if (IsStunned() || IsAlive() == false || CustomTimeDilation <= 0.f)
		return false;

	// 2. 鞘荐 蔼 悸泼捞 救登绢 乐阑锭
	if (BP_ConfigMinDashDistance <= 0.f || BP_ConfigMaxDashDistance <= 0.f)
		return false;

	// 3. 老馆 QTE 吝老锭 (付款飘绰 Rotating 秦具窍骨肺 力寇)
	if (IsInQTEState() && IsQTEMounting() == false)
		return false;

	return true;
}

void ABladeIIPlayer::RotateToTarget(float DeltaTime, bool ForceRotateForLowFPS)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_RotateToTarget);

	//bool bTargetActorValid = (TargetActor && TargetActor->IsAlive() && GetCapsuleComponent());
	//bool bTargetObjectValid = TargetObject && GetCapsuleComponent();
	//bool bInputValid = GetLastMovementInputVector() != FVector::ZeroVector && !GetCharacterMovement()->bApplyOptionalVelocityDuringRootmotion && !bDisableRotateToInput;

	//if (!bTargetObjectValid && !bTargetActorValid && !bInputValid)
	//	return;

	//FVector Diff;

	//FRotator CurrentRotation = IsQTEMounting() ? QTEActor->GetActorRotation() : GetActorRotation();
	//float	TargetYaw = CurrentRotation.Yaw;
	//// poikl2212 涝仿乐阑锭父 涝仿规氢栏肺 汲沥. 扁夯蔼篮 泅犁 肺抛捞记栏风
	//if (bInputValid)
	//	TargetYaw = GetLastMovementInputVector().Rotation().Yaw;

	//bool bNeedUpdateRotation = ForceRotateForLowFPS ||
	//	(GetAttackState() != EAttackState::ECS_None
	//		&& GetAttackState() != EAttackState::ECS_GuardStart
	//		&& GetAttackState() != EAttackState::ECS_Guard
	//		&& GetAttackState() != EAttackState::ECS_GuardEnd
	//		&& GetAttackState() != EAttackState::ECS_Evade
	//		&& GetAttackState() != EAttackState::ECS_TagOtherChar
	//		&& !IsQTEMounting());

	//// Find Target Direction (only non normal targeting mode for now)
	//bool bNeedUpdateTargetDir = GetSignalAttack() && GetAttackState() == EAttackState::ECS_None && TargetingMode != ETargetingMode::ETM_Normal;
	//
	////Qte 吝捞搁 公矫..
	//if (!IsQTEMounting() && !bInputValid)
	//{
	//	if (CurrentMoveAttackState != EAutoMoveAttackState::AttackTarget)
	//		return;
	//}
	//
	//if (bNeedUpdateRotation || bNeedUpdateTargetDir)
	//{
	//	//DrawDebugPoint(GetWorld(), TargetActor->GetActorLocation(), 10.f, FColor::Red, true, 10.f);
	//	if (bTargetActorValid)
	//	{
	//		Diff = GetTargetLocationVectorFromPlayer(TargetActor);
	//		if (!bInputValid && GetTargetDistanceInTarget(TargetActor) < BP_ConfigMaxDashDistance)
	//		{				
	//			TargetYaw = Diff.Rotation().Yaw;
	//		}
	//		else
	//		{
	//			bNeedUpdateRotation = false;
	//			bNeedUpdateTargetDir = false;
	//			bTargetActorValid = false;
	//		}
	//	}

	//	if (!bTargetActorValid && bTargetObjectValid)
	//	{
	//		Diff = GetTargetLocationVectorFromPlayer(TargetObject);
	//		if (!bInputValid && GetTargetDistanceInTarget(TargetObject) < BP_ConfigMinDashDistance)
	//		{
	//			TargetYaw = Diff.Rotation().Yaw;
	//		}
	//		else
	//		{
	//			bNeedUpdateRotation = false;
	//			bNeedUpdateTargetDir = false;
	//		}
	//	}

	//	if (Diff.Size() >= BP_ConfigMaxDashDistance)
	//	{
	//		StopDash();
	//	}

	//	if (bNeedUpdateTargetDir)
	//	{
	//		float DiffYaw = FRotator::ClampAxis(TargetYaw - CurrentRotation.Yaw);

	//		if (DiffYaw > 45.f && DiffYaw <= 135.f)
	//		{
	//			TargetDirection = ETargetDirection::ETD_Right;
	//		}
	//		else if (DiffYaw > 135.f && DiffYaw <= 225.f)
	//		{
	//			TargetDirection = ETargetDirection::ETD_Back;
	//		}
	//		else if (DiffYaw > 225.f && DiffYaw <= 315.f)
	//		{
	//			TargetDirection = ETargetDirection::ETD_Left;
	//		}
	//		else
	//		{
	//			TargetDirection = ETargetDirection::ETD_Forward;
	//		}
	//	}

	//	UCharacterMovementComponent* MovementComp = GetCharacterMovement();

	//	if (bNeedUpdateRotation)
	//	{
	//		// Dash Target Homing
	//		if (!ForceRotateForLowFPS && TotalDashDuration > 0.f)
	//		{
	//			if (MovementComp && MovementComp->bApplyOptionalVelocityDuringRootmotion)
	//			{
	//				// Calculate homing...
	//				MovementComp->OptionalVelocity = Diff / CurrentDashDuration;
	//				MovementComp->OptionalVelocity.Z = 0.f;
	//				float OptionalSpeed = MovementComp->OptionalVelocity.SizeSquared2D();
	//				if (OptionalSpeed > MaxDashSpeed)
	//				{
	//					MovementComp->OptionalVelocity = MovementComp->OptionalVelocity / (OptionalSpeed * MaxDashSpeed);
	//				}
	//				MovementComp->OptionalVelocityBlend = FMath::Clamp<float>(1.f - (CurrentDashDuration / TotalDashDuration), 0.f, 1.f);
	//			}
	//		}
	//	}

	//	if ((bInputValid)
	//		|| (bNeedUpdateRotation && (!bDisableRotateToTarget || MovementComp->bApplyOptionalVelocityDuringRootmotion)))
	//	{
	//		//Diff.Z = 0.f;
	//		CurrentRotation.Yaw = Utilities::FBlendHelper::BlendRotationWithSpeed(DeltaTime, CurrentRotation.Yaw, TargetYaw, RotateToTargetSpeed);
	//		if (IsQTEMounting())
	//		{
	//			QTEActor->SetActorRotation(CurrentRotation);
	//		}
	//		else
	//		{
	//			SetActorRotation(CurrentRotation);
	//		}
	//	}
	//}
}

// 肺流绝澜 罐篮 鸥百率栏肺 雀傈
void ABladeIIPlayer::RotateToTarget(class AActor* InTarget)
{
	FVector TargetDir = InTarget->GetActorLocation() - GetActorLocation();
	TargetDir.Z = 0;;

	SetActorRotation(TargetDir.Rotation());
}

void ABladeIIPlayer::UpdateCameraBoom(float DeltaTime)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraBoom);

	// TargetArmLength
	float fLengthClampAmount = CameraLengthValueForBlending.LimitChangeAmountPerSec * DeltaTime;

	CameraBoom->TargetArmLength = FMath::Clamp(CameraLengthValueForBlending.TargetArmLength , CameraBoom->TargetArmLength - fLengthClampAmount, CameraBoom->TargetArmLength + fLengthClampAmount);

	// Fov
	float fFovClampAmount = CameraFovValueForBlending.LimitChangeAmountPerSec * DeltaTime;

	TopDownCameraComponent->SetFieldOfView(FMath::Clamp(CameraFovValueForBlending.CameraFov, TopDownCameraComponent->FieldOfView - fFovClampAmount, TopDownCameraComponent->FieldOfView + fFovClampAmount));
	
}

float ABladeIIPlayer::GetDeadTimeSec()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDeadTimeSec);
	return m_fDeadTimeSec;
}

void ABladeIIPlayer::SetDeadTimeSec(float fDeadTime)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetDeadTimeSec);
	m_fDeadTimeSec = fDeadTime;
}

bool ABladeIIPlayer::GetUseQTESafeEnebleDistance()
{
	//AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	//if (pGM)
	//{
	//	return true;
	//}
	return false;
}

class ABladeIICharacter* ABladeIIPlayer::FindQTEEnableTarget(bool bUseSafeEnableDistance /*= false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_FindQTEEnableTarget);
	ABladeIICharacter* QTEEnableTarget = NULL;

	TArray<ABladeIICharacter*> Characters;

	auto* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//B2GM->GetAllCharacterList(Characters);

	//AB2TutorialGameMode* B2TGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this));
	//float MaxDist = B2TGM ? FLT_MAX : (bUseSafeEnableDistance ? QTE_ENABLE_SAFE_DISTANCE : QTE_ENABLE_DISTANCE);

	//for (auto* ch : Characters)
	//{
	//	if (ch == this || ch->GetTeamNum() == GetTeamNum())
	//		continue;

	//	if (!ch->IsTimingForQTEPossession())
	//	{
	//		continue;
	//	}

	//	if (!ch->IsAlive())
	//	{
	//		continue;
	//	}

	//	FVector Diff = GetTargetLocationVectorFromPlayer(ch);
	//	float fDistance = Diff.Size2D() - ch->GetCapsuleComponent()->GetScaledCapsuleRadius();

	//	if (fDistance <= 0)
	//		fDistance = 0;
	//			
	//	if (Diff.Size2D() < MaxDist)
	//	{
	//		{
	//			QTEEnableTarget = ch;
	//			MaxDist = Diff.Size2D();
	//		}
	//	}
	//}

	return QTEEnableTarget;
}

bool ABladeIIPlayer::IsVisibleFloorRing()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsVisibleFloorRing);
	return IsPlayerControlled();
}

void ABladeIIPlayer::HandleComboEndForQTE()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_HandleComboEndForQTE);
	if (IsInQTEState())
	{ // QTE 惑怕 辆丰.. 
		SetQTEStateFlag(false);
		bQTERun = false;
		bPendingQTENext = false;
		QTEStateNum = 0;
		FinalKillQTETarget();

		//SetActorEnableCollision(true);
		//SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
		//GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator::ZeroRotator);

		if (!FloorRingPSC->IsVisible())
		{
			FloorRingPSC->SetHiddenInGame(false);
		}
		if (!LightComponent->bAffectsWorld && !LightComponent->IsRegistered())
		{ // QTE Mount 矫 Light 甫 掺带 利捞 乐菌绰单 捞力 弊凡 老 绝栏搁 鞘夸绝摆瘤..
			FormalSetPointLightOn(true);
		}

		// QTE辆丰 妮归甸
		for (FQTEOnEnd& QTEOnEndCallBack : arQTEOnEndCallBack)
		{
			if (QTEOnEndCallBack.ExecuteIfBound())
				QTEOnEndCallBack.Unbind();
		}

		arQTEOnEndCallBack.Empty();
		// QTE辆丰 妮归甸 场

		ResetAllQTEUIDocState(); // 老馆利牢 惑炔捞搁 捞 矫痢俊辑 UIDoc 俊档 促 馆康捞 登绢具 窍瘤父 埃趣 救 弊凡 锭啊 乐绰 芭 鞍酒辑 犬角洒 府悸.
	}
}

void ABladeIIPlayer::CheckSendKakaoActionLog_Death()
{
	//EB2GameMode CurrentGameModeType = GetGameMode()->GetB2GameModeType();
	
	// 评措傈篮 公炼扒焊晨, 1:1, 痢飞傈篮 肺拿敲饭捞绢父 焊晨
	//if ((CurrentGameModeType == EB2GameMode::PVP_Team && GetIsLocalPlayerTeam())
	//	|| (CurrentGameModeType == EB2GameMode::PVP_Tag && IsLocalPlayer())
	//	|| (CurrentGameModeType == EB2GameMode::Control && IsLocalPlayer())
	//	)
	//{
	//	//////////////////////////////////////////////////////////////////
	//	//咀记 瘤钎 肺弊 (敲饭捞绢 磷澜)
	//	B2ActionLogSender Sender;
	//	Sender.setCategory(FString("PLAY"))
	//		.setAction(FString("DEATH"))
	//		.setLabel(B2ActionLogType<EB2GameMode>::GetName(CurrentGameModeType))
	//		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//	//////////////////////////////////////////////////////////////////
	//}
}

void ABladeIIPlayer::CheckSendKakaoActionLog_Kill()
{
	//EB2GameMode CurrentGameModeType = GetGameMode()->GetB2GameModeType();

	//// 评措傈篮 公炼扒焊晨, 1:1, 痢飞傈篮 肺拿敲饭捞绢父 焊晨
	//if ((CurrentGameModeType == EB2GameMode::PVP_Team && GetIsLocalPlayerTeam())
	//	|| (CurrentGameModeType == EB2GameMode::PVP_Tag && IsLocalPlayer())
	//	|| (CurrentGameModeType == EB2GameMode::Control && IsLocalPlayer())
	//	)
	//{
	//	//////////////////////////////////////////////////////////////////
	//	//咀记 瘤钎 肺弊 (敲饭捞绢 懦)
	//	B2ActionLogSender Sender;
	//	Sender.setCategory(FString("PLAY"))
	//		.setAction(FString("KILL"))
	//		.setLabel(B2ActionLogType<EB2GameMode>::GetName(CurrentGameModeType))
	//		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//	//////////////////////////////////////////////////////////////////
	//}
}

UMaterialInstanceDynamic* ABladeIIPlayer::GetChainMaterial()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetChainMaterial);
	//if (LeftWeapon)
	//{
	//	return Cast<UMaterialInstanceDynamic>(LeftWeapon->GetMaterial(0));
	//}
	return NULL;
}

ABladeIICharacter* ABladeIIPlayer::GetNearestEnemyCharacter()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetNearestEnemyCharacter);
	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		TArray<ABladeIICharacter*> EnemyCharacters;
		ABladeIICharacter* NearestEnemy = nullptr;
		FVector Location = GetActorLocation();

		//GameMode->GetAllWorldEnemyList(EnemyCharacters, GetTeamNum(), true);

		float MinDistanceSQ = BIG_NUMBER;
		for (auto* Enemy : EnemyCharacters)
		{
			if (Enemy == this)
				continue;

			const float ThisDistanceSQ = (Enemy->GetActorLocation() - Location).SizeSquared();

			if (ThisDistanceSQ < MinDistanceSQ)
			{
				NearestEnemy = Enemy;
				MinDistanceSQ = ThisDistanceSQ;
			}
		}

		return NearestEnemy;
	}

	return Super::GetNearestEnemyCharacter();
}

void ABladeIIPlayer::PostActorCreated()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_PostActorCreated);
	//Super::PostActorCreated();
	//if (GetMesh())
	//{
	//	static FName GameModeName(TEXT("GameModeType"));
	//	if (UAnimInstance *AnimInst = GetMesh()->GetAnimInstance())
	//	{
	//		UByteProperty* ByteProp = FindField<UByteProperty>(AnimInst->GetClass(), GameModeName);
	//		if (ByteProp != NULL)
	//		{
	//			// Currently only check CounterGame
	//			uint8 GameModeType = Cast<AB2CounterAttackGameMode>(UGameplayStatics::GetGameMode(GetWorld())) != NULL ? 1 : 0;
	//			ByteProp->SetPropertyValue_InContainer(AnimInst, GameModeType);
	//		}
	//	}
	//}
}

void ABladeIIPlayer::GetNearestFriendCharacters(TArray<class ABladeIICharacter*>& Friends, float Range, int32 MaxNum)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetNearestFriendCharacters);
	if (MaxNum <= 0 || Range < GetCapsuleComponent()->GetScaledCapsuleRadius())
	{
		return;
	}

	Friends.Empty();
	// All Mobs are friends
	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		TArray<ABladeIICharacter*> FriendsCharacters;

		//GameMode->GetAllMyTeamCharList(FriendsCharacters, GetTeamNum(), true);
		for (ABladeIICharacter* Friend : FriendsCharacters)
		{
			if (Friend != NULL && Friend != this && FVector::DistSquared(Friend->GetActorLocation(), GetActorLocation()) < FMath::Square(Range))
			{
				MaxNum--;
				Friends.Add(Friend);
			}

			if (MaxNum <= 0)
			{
				return;
			}
		}
	}
}

void ABladeIIPlayer::ChangeFloorRingPS(class UParticleSystem* NewFloorRingPS)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ChangeFloorRingPS);

	if (!IsVisibleFloorRing())
		return;
		
	if (NewFloorRingPS)
	{
		FloorRingPSC->SetTemplate(NewFloorRingPS);
		FloorRingPSC->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		FloorRingPSC->ReregisterComponent();
		FloorRingPSC->Activate();		
	}

	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(FloorRingPSC->GetMaterial(0));
	if (DynamicMaterial == NULL)
	{
		DynamicMaterial = FloorRingPSC->CreateAndSetMaterialInstanceDynamic(0);
	}

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(FloorColorParamName, FloorRingColor);
		DynamicMaterial->SetScalarParameterValue(FloorColorMultiplyParamName, FloorRingColorMultiplier);

		int32 MatNum = FloorRingPSC->GetNumMaterials();
		for (int32 i = 1; i < MatNum; ++i)
		{
			FloorRingPSC->SetMaterial(i, DynamicMaterial);
		}
	}
}

void ABladeIIPlayer::AddQTEOnEndCallBack(const FQTEOnEnd& Handler)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_AddQTEOnEndCallBack);
	arQTEOnEndCallBack.Add(Handler);
}

bool ABladeIIPlayer::GetAbleToMove()
{
	return Super::GetAbleToMove() && !IsDashAttackState(GetAnimAttackState());// && GetAnimAttackState() != EAttackState::ECS_GuardEnd;
}

bool ABladeIIPlayer::IsDashAttackState(EAttackState InAttackState)
{
	return InAttackState == EAttackState::ECS_Dash
		|| InAttackState == EAttackState::ECS_Counter01Dash
		|| InAttackState == EAttackState::ECS_Counter02Dash
		|| InAttackState == EAttackState::ECS_Counter03Dash
		|| InAttackState == EAttackState::ECS_Counter04Dash
		|| InAttackState == EAttackState::ECS_Counter05Dash;
}

void ABladeIIPlayer::SetTeamBuffMode(ETeamBuffType BuffMode, ABladeIIPlayer* Buffer, uint8 TargetingType, float Amount, float Duration, float Range, int32 MaxAffectNum, bool bUseEffect, bool bUseTextEffect)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetTeamBuffMode);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetTeamBuffMode"));
	//
	//EnumMaskValue<ETargetSelectType> TargettingTypeMask(TargetingType);

	//if (!B2TargetSelector::GetInstance().IsExceptSelf(TargettingTypeMask))
	//{
	//	float ApplyDuration = Duration + Buffer->GetBonusBuffDuration();

	//	if (ApplyDuration <= 0.f)
	//	{
	//		return;
	//	}

	//	float ApplyAmount = (Amount * 0.01f) + Buffer->GetBonusTeamBuffRate(BuffMode);

	//	switch (BuffMode)
	//	{
	//	case ETeamBuffType::ETBT_Shield:
	//		ApplyAmount *= GetMaxHealth();
	//		AddBuff(EBuffType::Buff_Shield, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
	//		break;
	//	case ETeamBuffType::ETBT_IncreaseCriticalRate:
	//		AddBuff(EBuffType::Buff_IncreaseCriticalRate, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
	//		break;
	//	case ETeamBuffType::ETBT_AbnormalStateImmune:
	//		AddBuff(EBuffType::Buff_AbnormalStateImmune, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
	//		break;
	//	case ETeamBuffType::ETBT_IncreaseCriticalDamage:
	//		AddBuff(EBuffType::Buff_IncreaseCriticalDamage, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
	//		break;
	//	case ETeamBuffType::ETBT_Enhance:
	//		AddBuff(EBuffType::Buff_Enhance, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
	//		break;
	//	case ETeamBuffType::ETBT_Taunt:
	//		AddBuff(EBuffType::Debuff_Taunt, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
	//		break;
	//	case ETeamBuffType::ETBT_Dispell:
	//		AddBuff(EBuffType::Buff_Dispell, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
	//		break;
	//	}
	//}
	//
	//if (Buffer != this || MaxAffectNum <= 0 || Range <= 0.1f || TargetingType == 0)
	//{
	//	return;
	//}

 //	MaxAffectNum += GetBonusBuffAffectNum();
	//
	//// Buff to other team members..
	//TArray<class ABladeIICharacter*> FoundCharacters;
	//B2TargetSelector::GetInstance().FindTargetSelectFlag(FoundCharacters, TargettingTypeMask, this);

	////remove self
	//FoundCharacters.Remove(this);

	//B2TargetSelector::GetInstance().FilterDistance(FoundCharacters, this, Range);
	//B2TargetSelector::GetInstance().FilterMaxNum(FoundCharacters, MaxAffectNum);

	//
	////GetNearestFriendCharacters(TeamMembers, Range, MaxAffectNum);
	////FilterTeamBuffAffectMember(BuffMode, TeamMembers);

	//for (ABladeIICharacter* Member : FoundCharacters)
	//{
	//	ABladeIIPlayer* CastedMember = Cast<ABladeIIPlayer>(Member);
	//	if (CastedMember)
	//	{
	//		// need to broadcasting
	//		CastedMember->SetTeamBuffMode(BuffMode, this, 0, Amount, Duration, 0.f, 0, bUseEffect, bUseTextEffect);
	//	}
	//}
}

void ABladeIIPlayer::TryRestoreFromCameraDistZoomNS(bool bForceRestore)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TryRestoreFromCameraDistZoomNS);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::TryRestoreFromCameraDistZoomNS"));

	//if (CameraBoom && bForceRestore)
	//{
	//	UWorld* TheWorld = GetWorld();
	//	ABladeIIWorldSettings* B2WS = TheWorld ? Cast<ABladeIIWorldSettings>(TheWorld->GetWorldSettings()) : NULL;
	//	ABladeIIPlayerController* MyB2PC = Cast<ABladeIIPlayerController>(this->GetController());

	//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//	AB2ActiveCameraActor* CurrActiveCam = B2GM ? B2GM->GetCurrentActiveActiveCameraActor() : NULL;

	//	if ((!bCachedForCameraDistZoomState && B2WS && MyB2PC) || (B2GM && B2GM->GetB2GameModeType() == EB2GameMode::PVP_Team))
	//	{
	//		B2WS->ApplyDefaultPlayerCameraSetting(MyB2PC, 0.1f, VTBlend_Linear, true);
	//	}
	//	else
	//	{// 捞巴档 历巴档 救登绰 版快父 某教秦 初篮 吧 荤侩.
 //			CameraBoom->SetRelativeRotation(this->CachedOriginalCamBoomRelativeRotation);
 //			CameraBoom->SetRelativeLocation(this->CachedOriginalCamBoomRelativeLocation);
	//		CameraLengthValueForBlending.TargetArmLength = this->CachedOriginalTargetArmLength;
	//	}

	//	const bool bIsInActiveCam = (CurrActiveCam && CurrActiveCam->GetInteractPlayerCharacter() == this);
	//	if (bIsInActiveCam)
	//	{ 
	//		CurrActiveCam->ForceBeginActiveCamera(0.1f, NULL, false);
	//	}
	//	
	//	bCachedForCameraDistZoomState = false;
	//}
}

FVector ABladeIIPlayer::GetVelocity() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetVelocity);
	if (IsQTEMounting())
	{
		return QTEActor->GetVelocity();
	}

	return Super::GetVelocity();
}

void ABladeIIPlayer::SetDefaultCameraRotator()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetDefaultCameraRotator);
	if (TopDownCameraComponent)
	{
		TopDownCameraComponent->SetRelativeRotation(TopDownCameraDefaultRotator);
	}
}

void ABladeIIPlayer::UpdatePlayerBuffChanged()
{
	//if (GetSkillRTManager())
	//	GetSkillRTManager()->UpdatePlayerBuffChanged(*(BuffManager->GetBuffList()));
}

bool ABladeIIPlayer::StartCustomAnim(UAnimSequenceBase * InAnimAsset)
{
//	if (auto* AnimInst = GetAnimInstance())
//		return AnimInst->StartCustomAnim(InAnimAsset);

	return false;
}

void ABladeIIPlayer::AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
{
	Super::AffectToAttacker(Victim, ActualDamage, DamageInfo);

	if (GetAbsorbHPAmountRateOnAttack() > 0.0f)
	{
		if (FMath::FRand() < GetProbableHPAbsorptionProbability())// 咯扁 犬伏篮 绊沥等 巴栏肺 可记 荐摹绰 雀汗樊捞瘤 犬伏捞 酒丛.
		{
			  // GetUnitedOptionStatusValue 俊辑 欺季飘甫 0.01 胶纳老茄 蔼阑 临 巴牢单 HealingHPByPercent 绰 欺季飘蔼阑 罐栏骨肺 促矫 100 蚌窃.
			  HealingHPByPercent(GetAbsorbHPAmountRateOnAttack() * 100.0f, true, true);
		}
	}
}

void ABladeIIPlayer::AffectToAttackerAfterVictimDie(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
{
	Super::AffectToAttackerAfterVictimDie(Victim, ActualDamage, DamageInfo);

	if (GetHealHPAmountRateOnKill() > 0.0f)
	{
		if (FMath::FRand() < GetProbableRecoverHPProbability())// 咯扁 犬伏篮 绊沥等 巴栏肺 可记 荐摹绰 雀汗樊捞瘤 犬伏捞 酒丛.
		{
			// GetUnitedOptionStatusValue 俊辑 欺季飘甫 0.01 胶纳老茄 蔼阑 临 巴牢单 HealingHPByPercent 绰 欺季飘蔼阑 罐栏骨肺 促矫 100 蚌窃.
			HealingHPByPercent(GetHealHPAmountRateOnKill() * 100.0f, true, true);
		}
	}
}

void ABladeIIPlayer::SetInAutoPlay(int32 Value)
{
	//InAutoPlay = Value;
	//BII_CHECK(CachedSkillRTManager);
	//if (CachedSkillRTManager)
	//{
	//	CachedSkillRTManager->SetAutoBattle(Value);
	//}

	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocBattle)
	//{
	//	DocBattle->SetInAutoPlay(InAutoPlay);
	//}
}

bool ABladeIIPlayer::IsImmortalMode() const
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	return bCheatImmortalMode; 
#else
	return false;
#endif
}
bool ABladeIIPlayer::IsHeroMode() const 
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	return bCheatHeroMode;
#else
	return false;
#endif
}

class ABladeIICharacter* ABladeIIPlayer::GetRandomTarget(float TargetingRadius, int32 MaxTargetNum, FVector& RandomTargetDirection)
{
	ABladeIICharacter*	Target = NULL;

	if (RandomTargets.Num() > 0)
	{
		do
		{
			int32 LastIdx = RandomTargets.Num() - 1;
			Target = RandomTargets[LastIdx];
			if (Target == NULL || Target->bIsDying)
			{
				RandomTargets.RemoveAt(LastIdx);
			}
			else
			{
				break;
			}
		} while (RandomTargets.Num() > 0);
	}

	if (Target == NULL || Target->bIsDying)
	{
		// Find new candidates
		TArray<ABladeIICharacter*> Characters;

		auto* B2SGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		//B2SGM->GetAllWorldEnemyList(Characters, GetTeamNum());

		for (auto* ch : Characters)
		{
			if (this == ch)
				continue;

			// 厘局拱 八荤
			FHitResult Result;
			GetWorld()->LineTraceSingleByChannel(Result, GetActorLocation(), ch->GetActorLocation(), GetCapsuleComponent()->GetCollisionObjectType(), FCollisionQueryParams(NAME_None, false, this));
	/*		if (!Result.bBlockingHit || Result.Actor != ch)
				continue;*/

			FVector Diff = GetTargetLocationVectorFromPlayer(ch);
			
			float CandidateDist = Diff.Size2D();
			if (CandidateDist < TargetingRadius)
			{
				Target = ch;
				RandomTargets.Add(ch);
			}

			if (RandomTargets.Num() >= MaxTargetNum)
				break;
		}
	}

	if (Target == NULL || Target->bIsDying)
	{
		// Move forward when no target
		LastRandomTargetDirection = RandomTargetDirection = GetActorForwardVector();
		LastRandomTargetLocation = GetMovementComponent() ? GetMovementComponent()->GetActorFeetLocation() : GetActorLocation();
		return NULL;
	}
	else
	{
		LastRandomTargetDirection = RandomTargetDirection = -Target->GetActorForwardVector();
		LastRandomTargetLocation = Target->GetCharacterMovement() ? Target->GetCharacterMovement()->GetActorFeetLocation() : Target->GetActorLocation();
		return Target;
	}
}

class ABladeIICharacter* ABladeIIPlayer::RemoveRandomTarget()
{
	ABladeIICharacter* Target = NULL;
	if (RandomTargets.Num() > 0)
	{
		int32 LastIdx = RandomTargets.Num() - 1;
		Target = RandomTargets[LastIdx];
		RandomTargets.RemoveAt(LastIdx);
	}
	return Target;
}

void ABladeIIPlayer::ResetRandomTarget()
{
	RandomTargets.Empty();
}

#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
bool ABladeIIPlayer::bHideAllPCs = false;
// 辑滚 救 嘿篮 惑炔俊辑 窜刀 抛胶飘 矫 盔牢阑 舅 荐 绝霸 狐福霸 框流咯辑 泵咯瘤氓. 捻欧 滚傈父 弊烦.
void ABladeIIPlayer::DevCalmDown()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->MaxWalkSpeed = FMath::Clamp(CachedBaseMaxWalkSpeed, 500.0f, 1500.0f);
	}
	if (GetMesh())
	{
		GetMesh()->GlobalAnimRateScale = 1.0f;
		AdditionalAttackSpeedRatebyPassive = 0.0f;
	}
}
#endif

void ABladeIIPlayer::OnEnterAnimState(const FName& NewState)
{
	const FString& StateStr = NewState.ToString();
	const bool IsSkillState = StateStr.Contains(TEXT("Skill"));
	const bool HasReservedSkill = GetReservedSkill() != EAttackState::ECS_None;

	const bool IsQTEState = StateStr.Equals(TEXT("QTE"), ESearchCase::IgnoreCase);
	if (IsQTEState)
	{
		OnQTEBegin();
	}
	else if (IsSkillState)
	{
		OnEnterSkillState(CachedSkill);
	}
}

void ABladeIIPlayer::OnExitAnimState(const FName& PrevState)
{
	//const FString& StateStr = PrevState.ToString();
	//if (StateStr.Contains(TEXT("Skill")))
	//{
	//	EAttackState EndAttackState = EAttackState::ECS_None;

	//	auto* AnimInst = GetAnimInstance();
	//	const ESkillAnimType SkillType = AnimInst ? AnimInst->GetSkillAnimType() : ESkillAnimType::ESA_End;
	//	if (SkillType >= ESkillAnimType::ESA_Weapon_01 && SkillType <= ESkillAnimType::ESA_Weapon_05_LOD)
	//		EndAttackState = EAttackState::ECS_WeaponSkill;

	//	OnExitSkillState(EndAttackState);
	//}
}

void ABladeIIPlayer::OnEnterSkillState(EAttackState SkillState)
{
	//if (auto* SkillRTManager = GetSkillRTManager())
	//	SkillRTManager->OnEnterPlayerSkillState(SkillState);

	//if (auto* AnimInst = GetAnimInstance())
	//	AnimInst->UpdateDamageEnd();

	//ConsumeReservedSkill();
	//CachedSkill = EAttackState::ECS_None;
}

void ABladeIIPlayer::OnExitSkillState(EAttackState SkillState)
{
	if (SkillState == EAttackState::ECS_WeaponSkill && SkillLOD == 0)
	{
		OnEndWeaponSkillLOD0();
	}
}

bool ABladeIIPlayer::IsForceApplyHitAnimation()
{
	//if(auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this)))
	//{
	//	if (auto* GameRule = GameMode->GetGameRule())
	//		return GameRule->ForceApplyHitAnimation();
	//}

	return false;
}

void ABladeIIPlayer::ChangeAutoMoveAttackState(EAutoMoveAttackState InAutoMoveAttack)
{
	PreChangeMoveAttackState(InAutoMoveAttack);

	if (CheckChangeState(InAutoMoveAttack) == false)
		return;
	
	std::shared_ptr<AutoMoveAttackState> ChangeStatePtr = GetState(InAutoMoveAttack);

	if (CurrentAutoMoveAttackStatePtr)
		CurrentAutoMoveAttackStatePtr->Leave();

	CurrentMoveAttackState = InAutoMoveAttack;
	CurrentAutoMoveAttackStatePtr = ChangeStatePtr;
	CurrentAutoMoveAttackStatePtr->Enter();
}

void ABladeIIPlayer::FilterTeamBuffAffectMember(ETeamBuffType TeamBuffType, TArray<class ABladeIICharacter*>& InChracters)
{
	//nothing
}

std::shared_ptr<ABladeIIPlayer::AutoMoveAttackState> ABladeIIPlayer::GetState(EAutoMoveAttackState InAutoMoveAttack)
{
	std::shared_ptr<AutoMoveAttackState> ChangeStatePtr;

	switch (InAutoMoveAttack)
	{
	case NoSearchTarget:
		ChangeStatePtr = NoSearchTargetPtr;
		break;
	case AutoMoveToTarget:
		ChangeStatePtr = AutoMoveToTargetPtr;
		break;
	case AttackTarget:
		ChangeStatePtr = AttackTargetPtr;
		break;
	}
	return ChangeStatePtr;
}

bool ABladeIIPlayer::CheckChangeState(EAutoMoveAttackState InAutoMoveAttack)
{
	return CurrentMoveAttackState != InAutoMoveAttack;
}

void ABladeIIPlayer::PreChangeMoveAttackState(EAutoMoveAttackState & InAutoMoveAttack)
{
	
}

bool ABladeIIPlayer::AutoAttack_AbleSkill()
{
	const bool IsCombo = IsOneOfComboAttackState(GetAnimAttackState());

	return IsCombo || (GetAnimAttackState() == EAttackState::ECS_None);
}
