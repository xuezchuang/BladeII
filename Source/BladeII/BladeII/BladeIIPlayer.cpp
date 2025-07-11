// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "BladeIIPlayer.h"
//#include "BladeIIPlayerController.h"
////#include "BladeIIProjectile.h"
////#include "B2AutoAIController.h"
//#include "BladeIIGameMode.h"
//#include "B2TutorialGameMode.h"
////#include "B2SkillInfo.h"
//#include "B2SkillRTManager.h"
////#include "B2InGameHUDInfo.h"
////#include "B2FloatingGoldAcq.h"
////#include "B2FloatingDamageNum.h"
////#include "B2UIManager_InGameCombat.h"
////#include "GameFramework/WorldSettings.h"
////#include "Components/ChainComponent.h"
////#include "Runtime/Engine/Public/EngineUtils.h"
////#include "Animation/SkeletalMeshActor.h"
////#include "SkeletalRenderPublic.h"
////#include "Components/DecalComponent.h"
////#include "Navigation/PathFollowingComponent.h"
//#include "B2AreaDamageActorBase.h"
//#include "Event.h"
//#include "B2UIDocHelper.h"
//#include "B2CombatStatEvaluator.h"
//#include "B2LobbyInventory.h"
//#include "B2StageManager.h"
//#include "B2StageGameMode.h"
////#include "Camera/CameraAnimInst.h"
//#include "B2PCClassInfoBox.h"
//#include "B2BuffModeEffectInfo.h"
//#include "B2ActiveCameraActor.h"
//#include "BladeIIWorldSettings.h"
////#include "B2DestructibleLevelObjBase.h"
//#include "B2SomeInfo.h"
//#include "BrainComponent.h"
////#include "DailyGameMode/B2CounterAttackGameMode.h"
//#include "B2EtherManager.h"
//#include "B2CharacterBuffManager.h"
//#include "B2Buff_Base.h"
//#include "B2GeneralBuffs.h"
//#include "BladeIIScalabilityControl.h"
//#include "B2EtherSetEffect.h"
//#include "B2EtherContainer.h"
////#include "B2UIManager.h"
////#include "B2UIBattleQTE.h"
////#include "B2CodeTable.h"
//#include "B2NetGameMode.h"
////#include "B2AnimInstance.h"
//#include "FB2ErrorMessage.h"
//#include "B2TargetSelector.h"
////#include "B2CharacterMovementComponent.h"
//#include "BladeIIGameImpl.h"
//#include "B2GameInstance.h"
//#include "B2ControlGameMode.h"
////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
////#include "BladeIITestDummyNPC.h"
////#endif
//#include "BladeIIUtil.h"
//#include "DF.h"
//
//#define FORCE_LOGGING_IN_ANDROID 0
//
//FName ABladeIIPlayer::LeftFootBoneName(TEXT("Bip001-L-Toe0"));
//FName ABladeIIPlayer::RightFootBoneName(TEXT("Bip001-R-Toe0"));
//FName ABladeIIPlayer::TargetTraceName(TEXT("B2TargetTrace"));
//FName ABladeIIPlayer::QTESocketName(TEXT("QTESocket"));
//FName ABladeIIPlayer::FloorColorParamName(TEXT("ColorControl"));
//FName ABladeIIPlayer::FloorColorMultiplyParamName(TEXT("ColorMultiplier"));
//
//float ABladeIIPlayer::AutoMoveAttackState::GetCurrentTargetDistance()
//{
//	AActor* CurrentTarget = GetCurrentTargeActor();
//
//	if (CurrentTarget)
//		return Player->GetTargetLocationVectorFromPlayer(CurrentTarget).Size();
//
//	return -1.0f;
//}
//
//AActor * ABladeIIPlayer::AutoMoveAttackState::GetCurrentTargeActor()
//{
//	bool bTargetActorValid = (Player->TargetActor && Player->TargetActor->IsAlive() && Player->GetCapsuleComponent());
//	bool bTargetObjectValid = Player->TargetObject && Player->GetCapsuleComponent();
//
//	if (bTargetActorValid)
//		return Player->TargetActor;
//	else if (bTargetObjectValid)
//		return Player->TargetObject;
//	else
//		return nullptr;
//}
//
//void ABladeIIPlayer::AutoMoveAttackState::StopAutoMove()
//{
//	Player->AutoMoveTargetActor = nullptr;
//	ABladeIIPlayerController* B2Controller = Cast<ABladeIIPlayerController>(Player->GetController());
//	if (B2Controller)
//		B2Controller->StopMovement();
//}
//
//void ABladeIIPlayer::AutoMoveAttackState::StartAutoMove(AActor * CurrentTarget)
//{
//	//if (Player->m_bAttackKeyPressed)
//	//{
//	//	if (Player->AutoMoveTargetActor != CurrentTarget)
//	//	{
//	//		ABladeIIPlayerController* B2Controller = Cast<ABladeIIPlayerController>(Player->GetController());
//	//		if (B2Controller && B2Controller->MoveToActor(CurrentTarget, Player->DefaultAttackRange))
//	//		{
//	//			Player->StopAttack();
//	//			Player->AutoMoveTargetActor = CurrentTarget;
//	//		}
//	//	}
//	//}
//}
//
//bool ABladeIIPlayer::AutoMoveAttackState::CheckAutoMoveRadius(float InDistance)
//{
//	return InDistance <= (Player->DefaultAttackRange + Player->AutoMoveRange) && InDistance > Player->DefaultAttackRange;
//}
//
//bool ABladeIIPlayer::AutoMoveAttackState::CheckAttackTargetRadius(float InDistance)
//{
//	return InDistance <= Player->DefaultAttackRange;
//}
//
//void ABladeIIPlayer::AutoMoveAttackState::Tick(float delta)
//{
//	//bool bInputValid =  Player->GetLastMovementInputVector() != FVector::ZeroVector && 
//	//				   !Player->GetCharacterMovement()->bApplyOptionalVelocityDuringRootmotion && 
//	//				   !Player->bDisableRotateToInput;
//
//	//if (Player->IsQTEMounting() || bInputValid)
//	//{
//	//	Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::NoSearchTarget);
//	//	return;
//	//}
//
//	//float Distance = GetCurrentTargetDistance();
//	//if (Distance < 0.0f)
//	//{
//	//	Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::NoSearchTarget);
//	//	return;
//	//}
//
//	//bool bInAutoMoveRadius = CheckAutoMoveRadius(Distance);
//	//if (bInAutoMoveRadius)
//	//{
//	//	if (Player->IsInAutoPlay())
//	//	{
//	//		Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::AttackTarget);
//	//		return;
//	//	}
//	//	Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::AutoMoveToTarget);
//	//	return;
//	//}
//
//	//bool bInAttackRadius = CheckAttackTargetRadius(Distance);
//	//if (bInAttackRadius)
//	//{
//	//	Player->ChangeAutoMoveAttackState(EAutoMoveAttackState::AttackTarget);
//	//	return;
//	//}
//}
//
//
//void ABladeIIPlayer::NoSearchTargetState::Enter()
//{
//
//}
//void ABladeIIPlayer::NoSearchTargetState::Leave()
//{
//
//}
//void ABladeIIPlayer::NoSearchTargetState::Tick(float delta)
//{
//	AutoMoveAttackState::Tick(delta);
//}
//
//void ABladeIIPlayer::AutoMoveToTargetState::Enter()
//{
//	AActor* CurrentTargetActor = GetCurrentTargeActor();
//	
//	StartAutoMove(CurrentTargetActor);
//}
//void ABladeIIPlayer::AutoMoveToTargetState::Leave()
//{
//	StopAutoMove();
//}
//void ABladeIIPlayer::AutoMoveToTargetState::Tick(float delta)
//{
//	AutoMoveAttackState::Tick(delta);
//
//	if (Player->CurrentMoveAttackState != EAutoMoveAttackState::AutoMoveToTarget)
//		return;
//
//	AActor* CurrentTargetActor = GetCurrentTargeActor();
//	if (Player->AutoMoveTargetActor != CurrentTargetActor)
//		StartAutoMove(CurrentTargetActor);
//}
//
//void ABladeIIPlayer::AttackTargetState::Enter()
//{
//	if (Player->m_bAttackKeyPressed)
//		Player->StartAttackFromInput();
//}
//void ABladeIIPlayer::AttackTargetState::Leave()
//{
//
//}
//void ABladeIIPlayer::AttackTargetState::Tick(float delta)
//{
//	AutoMoveAttackState::Tick(delta);
//}
//
//ABladeIIPlayer::ABladeIIPlayer(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
//{
//	// TODO : Init Data for Character Type
//	CharacterType = ECharacterType::ECT_UnknownPC;
//	Health = 500.f;
//	MaxHealth = 500.f;
//	//MaxArmor = 100.f;
//
//	TargetingMode = ETargetingMode::ETM_Normal;
//	TagAttackMode = ETagAttackMode::EAM_DownAttack;
//
//	CameraPitch = -50.f;
//	CameraDistance = 700.f;
//	CameraYaw = -130.f;
//
//	MaxDistanceToAllowAutoAttack = 450.0f;
//
//	TotalDashDuration = -1.f;
//	CurrentDashDuration = -1.f;
//	MaxDashDistance = 1000.f;
//	MaxDashSpeed = 10000.f;
//
//	BP_ConfigMinDashDistance = -1.f;
//	BP_ConfigMaxDashDistance = -1.f;
//
//	TargetActor = NULL;
//	TargetObject = NULL;
//	TargetingDistance = 2000.f;
//	bDisableRotateToTarget = false;
//	bDisableRotateToInput = false;
//	RotateToTargetSpeed = 0.f;
//	CounterCheckTime = 0.25f;
//	EvadeCheckTime = 0.4f;
//
//	bIsInCriticalCondition = false;
//	CriticalConditionHealthRatio = 0.2f;
//	bInvincibleByStageEvent = false;
//	bReachedAtDeadEnd = false;
//
//	bStopAttackSignal = false;
//
//	bSkillCancel = false;
//
//	CameraCullingRadius = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
//
//	// Don't rotate character to camera direction
//	bUseControllerRotationPitch = false;
//	bUseControllerRotationYaw = false;
//	bUseControllerRotationRoll = false;
//
//	// Create a camera boom...
//	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
//	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	//CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
//	UpdateCameraDistance(CameraDistance);
//	//CameraBoom->RelativeRotation = FRotator(CameraPitch, CameraYaw, 0.f);
//	//CameraBoom->bDoCollisionTest = true;
//
//	// Create a camera...
//	ActionCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("ActionCamera"));
//	ActionCameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	ActionCameraComponent->bUsePawnControlRotation = false;
//	ActionCameraComponent->bAutoActivate = false;
//
//	TopDownCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopDownCamera"));
//	TopDownCameraComponent->AttachToComponent(CameraBoom, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), USpringArmComponent::SocketName);
//	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
//
//	TopDownCameraDefaultRotator = TopDownCameraComponent->GetRelativeTransform().GetRotation().Rotator();
//	
//	// LightComponent �� attachment �� �ٽ� �����ϰ� �� �� �ִ�.
//	LightComponent = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("PointLightComponent"));
//	LightComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	LightComponentAttachDistZ = 425.0f;
//	//LightComponent->RelativeLocation = FVector(0.f, 0.f, 425.f);
//	LightComponent->Intensity = 45000.f;
//	LightComponent->LightColor = FColor(255, 222, 164);
//	LightComponent->AttenuationRadius = 800.f;
//
//	bHeadPointLightIsFormallyOn = false; // �Ϲ����� ��η� �� LightComponent �� �Ѱ� �� ������ bHeadPointLightIsFormallyOn �� ���� �����ؾ� �Ѵ�.
//
//	//GetCharacterMovement()->bIgnoreForceDuringRootmotion = true;
//
//	//// Create chain components
//	//LeftWeapon = ObjectInitializer.CreateDefaultSubobject<UChainComponent>(this, TEXT("LeftWeaponComponent"));
//	//LeftWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	//LeftWeapon->Deactivate();
//
//	//RightWeapon = ObjectInitializer.CreateDefaultSubobject<UChainComponent>(this, TEXT("RightWeaponComponent"));
//	//RightWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	//RightWeapon->Deactivate();
//
//	ShadowSizeFoot = 75.f;
//
//	DefaultAttackRange = 350.0f;
//	AutoMoveRange = 200.0f;
//
//	AutoMoveTargetActor = nullptr;
//
//#if 0
//	// Create foot shadow components
//	LeftFootShadow = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("LeftFootShadowComponent"));
//	//LeftFootShadow->bAbsoluteLocation = true;
//	LeftFootShadow->bAbsoluteRotation = true;
//	LeftFootShadow->RelativeLocation = FVector::ZeroVector;
//	LeftFootShadow->RelativeScale3D = FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot);
//	LeftFootShadow->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
//	LeftFootShadow->DecalSize = FVector(1.f, 1.f, 1.f); // for 4.11
//	//LeftFootShadow->bHiddenInGame = true;
//	LeftFootShadow->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	LeftFootShadow->Deactivate();
//
//	RightFootShadow = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("RightFootShadowComponent"));
//	//RightFootShadow->bAbsoluteLocation = true;
//	RightFootShadow->bAbsoluteRotation = true;
//	RightFootShadow->RelativeLocation = FVector::ZeroVector;
//	RightFootShadow->RelativeScale3D = FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot);
//	RightFootShadow->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
//	RightFootShadow->DecalSize = FVector(1.f, 1.f, 1.f); // for 4.11
//	//RightFootShadow->bHiddenInGame = true;
//	RightFootShadow->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	RightFootShadow->Deactivate();
//#endif
//
//	FloorRingPSC = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("FloorRingPSC"));
//	FloorRingPSC->SecondsBeforeInactive = 0.f;
//	FloorRingPSC->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
// 	FloorRingPSC->Deactivate();	
//
//	// ���ο�, ���ᶧ�� ���ӵ��� ����
//	//FloorRingPSC->bUseOwnerCustomTimeDilation = false;
//
//	// �� FloorRing �� ������� ���Խ�Ű�� �ʵ��� ������Ʈ�� �߰���.
//	EventPuppetSetupBlacklist.Add(FloorRingPSC);
//
//	FloorRingColorMultiplier = 1.f;
//
//	ComboMaintainDuration = 3.0f;
//	
//	bLastInvokedAnySkillAttack = false;
//	
//	////////////////////////////////////////////////////////////////////////////////
//	//>>> Widget classes specification begin
//
//	// They will be managed not to be created, but let's null out anyway.
//	//FloatingHPBarWidgetClass = NULL;
//	HiddenMobPointerPSTemplate = NULL;
//
//
//	//if (!GMinimalDLCFrontMode)
//	//{ // DLC Front ��� ���ҽ��ε� �ִ��� ����
//	//	FString DefaultHiddenMobPointerPSTemplatePath;
//	//	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultHiddenMobPointerPSTemplate"), DefaultHiddenMobPointerPSTemplatePath, GGameIni);
//	//	HiddenMobPointerPSTemplate = LoadObject<UParticleSystem>(NULL, *DefaultHiddenMobPointerPSTemplatePath);
//	//	BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("HiddenMobPointerPSTemplate"))); // MinimalDLCFrontMode �� ���� ������� BaseBP �� ���� �������� �ʱ� ���� �̸� �߰�
//
//	//	FString DefaultForceFeedbackPath;
//	//	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultForceFeedbackPath"), DefaultForceFeedbackPath, GGameIni);
//	//	ForceFeedbackSetting = LoadObject<UForceFeedbackEffect>(NULL, *DefaultForceFeedbackPath);
//	//	BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("ForceFeedbackSetting"))); // MinimalDLCFrontMode �� ���� ������� BaseBP �� ���� �������� �ʱ� ���� �̸� �߰�
//	//}
//
//	//<<< Widget classes specification end
//	////////////////////////////////////////////////////////////////////////////////
//
//	//CachedSkillRTManager = NULL;
//	//HUDInfoClass = NULL;
//	//
//	//bFinalizedByPCClassInfo = false;
//	//
//	//PlayerTopDownCamInitialFOV = 90.0f;
//
//	//CharacterLevel = 10;
//
//	//bCheatImmortalMode = false;
//	//bCheatHeroMode = false;
//
//	//InAutoPlay = AutoBattleType::NONE;
//
//	bEventsSubscribed = false;
//
//	bPendingTag = false;
//	bPendingQTENext = false;
//
//	SwipeTouchIndex = ETouchIndex::Touch1;
//	LastTouchLocation = FVector::ZeroVector;
//	bIsSwiping = false;
//	bIsTagPossible = true;
//
//	// Anim Notify Data
//	CachedFOV = 80.f;
//	CachedOriginalTargetArmLength = 0.f;
//	CachedOriginalCamBoomRelativeRotation = FRotator::ZeroRotator;
//	CachedOriginalCamBoomRelativeLocation = FVector::ZeroVector;
//	CameraDistZoomEstimatedPlayTime = 0.f;
//	//bCachedForCameraDistZoomState = false;
//	//CameraAnimPlayInstance = NULL;
//	//RemainCameraAnims = 0;
//	//CurrentCameraAnimTime = 0.f;
//	//CameraShakePlayInstance = NULL;
//
//	CachedLocation = FVector::ZeroVector;
//	CurrentLocation = FVector::ZeroVector;
//	DeltaPerTick = FVector::ZeroVector;
//	B2RandomStartLocation = FVector::ZeroVector;
//
//	OriginalCameraLagSpeed = 0.f;
//	OriginalCameraLagMaxDist = 0.f;
//	OriginalLightColor = FColor(255, 222, 164);
//	OriginalLightIntensity = 40000.f;
//	LightPrevColorBlend = 0.f;
//
//	bKeepAttacking = false;
//	bCachedPressedInCinematic = false;
//	LastGuardTime = 0.f;
//	//CachedBaseMaxWalkSpeed = 0.f; // PCClassInfo �� ���õ� ���� �� �־�� ��. Elevated to BladeIICharacter
//	
//	WeaponSkillPoint = 0;
//	CurrentSkillAnimIdx = ESkillAnimType::ESA_End;
//	SkillLOD = 0;
//	
//	RandDamageAnimType = EAttackType::EAT_Default;
//
//	bEnableNormalAttack = true;
//
//	QTENPCClass = ENPCClass::ENC_End;
//	QTEMountRemainingTime = 0.0f;
//
//	// �÷��̾� �⺻���� ������
//	SetTeamNum(int32(ETeamType::ETT_Red));
//
//	CharacterDataStore = NULL;
//
//	m_fDeadTimeSec = -1;
//
//	BaseDamageDecreaseRate = 1.0f;
//	MeleeDamageDecreaseRate = 1.0f;
//	RangeDamageDecreaseRate = 1.0f;
//	BossDamageDecreaseRate = 1.0f;
//	ResistCriticalRate = 1.0f;
//	StateDurationDecreaseRate = 1.f;
//	IncDamageValueByPassive = 0.0f;
//	IncNormalDamageValueByPassive = 0.0f;
//	IncSkillDamageValueByPassive = 0.0f;
//	IncDamageToBossValueByPassive = 0.0f;
//	IncCriticalRateValueByPassive = 0.0f;
//	IncCriticalDamageValueByPassive = 0.0f;
//	HealHPAmountRateOnCounter = 0.0f;
//	HealHPAmountRateOnKill = 0.0f;
//	AbsorbHPAmountRateOnAttack = 0.0f;
//	AdditionalAttackSpeedRatebyPassive = 0.0f;
//	FPlatformMemory::Memzero(IncAttackDamageByPassiveToClass, sizeof(IncAttackDamageByPassiveToClass));
//	FPlatformMemory::Memzero(DecReceiveDamageByPassiveFromClass, sizeof(DecReceiveDamageByPassiveFromClass)); // �⺻�� 1 �� �±� �ѵ� �ʱⰪ �־��ֱ� �����̴�..
//
//	InjectedGameMode = nullptr;
//
//	LastRandomTargetLocation = FVector::ZeroVector;
//	LastRandomTargetDirection = FVector::ZeroVector;
//
//	m_bAttackKeyPressed = false;
//	m_bGuardKeyPressed = false;
//
//	bInWeaponSkillLOD0 = false;
//	WeaponSkillLOD0SceneLight = nullptr;
//
//	IsUnityTick = false;
//	bOverUnityPoint = true;
//
//	//AutoSkillPriority = { WEAPON_6STAR_SKILL_INDEX/*3*/, 0, 1, 2 };
//
//	NoSearchTargetPtr = std::make_shared<NoSearchTargetState>(this);
//	AutoMoveToTargetPtr = std::make_shared<AutoMoveToTargetState>(this);
//	AttackTargetPtr = std::make_shared<AttackTargetState>(this);
//
//	CurrentAutoMoveAttackStatePtr = NoSearchTargetPtr;
//	ChangeAutoMoveAttackState(EAutoMoveAttackState::NoSearchTarget);
//
//	PrevPosition = FVector::ZeroVector;
//	StayTime = 0.0f;
//	bRestartLevel = false;
//	
//	GuardCoolTime = 0.0f;
//	HasNotTagDefence = false;
//}
//
//bool ABladeIIPlayer::CheckPlayerStopTime(float DeltaTime)
//{
//	if (bRestartLevel)
//		return false;
//
//	//AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this));
//	//if (!StageGM)
//	//	return false;
//
//	//if (StageGM->GetStageManager()->IsStageCleared())
//	//	return false;
//
//	//if (IsInAutoPlay() == 0)
//	//	return false;
//
//	//if (!IsAlive())
//	//	return false;
//
//	//if (UGameplayStatics::IsGamePaused(GetWorld()) == true)
//	//	return false;
//
//	//if (GetHadInput())
//	//{
//	//	StayTime = 0.0f;
//	//	return false;
//	//}
//
//	//FVector CurPosision = GetActorLocation();
//	//if (CurPosision.Equals(PrevPosition))
//	//{
//	//	StayTime += DeltaTime;
//	//}
//	//else
//	//{
//	//	StayTime = 0.0f;
//	//	PrevPosition = CurPosision;
//	//}
//
//	return StayTime > RESTART_TIME;
//}
//
//void ABladeIIPlayer::AddGuardCoolTime(float InAddCoolTime)
//{
//	if (InAddCoolTime > 0)
//	{
//		SetGuardCoolTime(GetGuardCoolTime() + InAddCoolTime);
//		SetHasNotTagDefence(true);
//	}
//}
//
//void ABladeIIPlayer::SetGuardCoolTime(float InCoolTime)
//{
//	GuardCoolTime = InCoolTime;
//	UB2UIDocBattle* DocBattle = GetDocBattle();
//	if (DocBattle)
//	{
//		//DocBattle->SetGuardCoolTime(GuardCoolTime);
//	}
//}
//
//float ABladeIIPlayer::GetGuardCoolTime()
//{
//	return GuardCoolTime;
//}
//
//bool ABladeIIPlayer::GetHasNotTagDefence()
//{
//	return HasNotTagDefence;
//}
//
//void ABladeIIPlayer::SetHasNotTagDefence(bool InHasNotTagDefence)
//{
//	UB2UIDocBattle* DocBattle = GetDocBattle();
//	if (DocBattle)
//	{
//		HasNotTagDefence = InHasNotTagDefence;
//		//DocBattle->SetHasNotTagDefence(InHasNotTagDefence);
//	}
//}
//
//void ABladeIIPlayer::UpdateGuardCoolTime(float InDeltaSeconds)
//{
//	if (GetGuardCoolTime() > 0.0f)
//	{
//		SetGuardCoolTime(GetGuardCoolTime() - InDeltaSeconds);
//
//		if (GetGuardCoolTime() <= 0.0f)
//		{
//			SetGuardCoolTime(0.0f);
//			SetHasNotTagDefence(false);
//		}
//	}
//}
//
//void ABladeIIPlayer::SetReservedSkill(EAttackState InSkillState)
//{
//	if (GetReservedSkill() == EAttackState::ECS_None && IsOneOfSkillAttackState(InSkillState))
//	{
//		ReservedSkill = InSkillState;
//		CachedSkill = InSkillState;
//		if (auto* World = GetWorld())
//			LastSkillInputTime = World->GetTimeSeconds();
//#ifdef FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLog ReservedSkillState : %d / time : %f threadId : %d\n"), static_cast<int32>(InSkillState), LastSkillInputTime, FPlatformTLS::GetCurrentThreadId());
//#endif
//	}
//}
//
//void ABladeIIPlayer::ConsumeReservedSkill()
//{
//	ReservedSkill = EAttackState::ECS_None;
//	LastSkillInputTime = 0.f;
//}
//
//ESkillAnimType ABladeIIPlayer::GetCurrentSKillAnimIdx()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCurrentSKillAnimIdx);
//
//	return GetCurrentSKillAnimIdx_Native();	
//}
//
//ESkillAnimType ABladeIIPlayer::GetCurrentSKillAnimIdx_Native()
//{
//	//[@AKI, 170815] ���� �м� �� SkillLOD�� ���� ���� �κ� �� �� ��.
//	if (SkillLOD != 0 && CurrentSkillAnimIdx >= ESkillAnimType::ESA_Weapon_01)
//	{
//		return static_cast<ESkillAnimType>(static_cast<int32>(ESkillAnimType::ESA_Weapon_Normal_End) + static_cast<int32>(CurrentSkillAnimIdx) - static_cast<int32>(ESkillAnimType::ESA_Weapon_01) + 1);
//	}
//	return CurrentSkillAnimIdx;
//}
//
//void ABladeIIPlayer::BeginPlay()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_BeginPlay);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::BeginPlay"));
//
//	// �� ������ PlayerController �� possess �� �Ǿ� �־�� ��. UB2SkillRTManager::DynSpawnNewPlayerCommon �̳� ABladeIIGameMode::StaticSpawnPCClassInfoPlayer ����
//	//GetSkillRTManager();
//	SetupMeshesMain(); // ��� ���տ� ���� �޽� �¾�
//
//	SyncPCCharacterLevelFromExpManager(); // InitializeCombatStats ������..
//
//	// ĸ�� ������Ʈ �������� [�÷��̾� ��ŸƮ] �����ϰ��� ������ ����
//	// �����ϰ� �ʱ�ȭ
//	GetCapsuleComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
//
//	//if (EtherContainer == nullptr)
//	//	EtherContainer = NewObject<UB2EtherContainer>(this, UB2EtherContainer::StaticClass()->GetFName());
//
//	//Super::BeginPlay();
//
//	//USkeletalMeshComponent* SKMeshComp = GetMesh();
//	//if (LeftWeapon)
//	//{
//	//	LeftWeapon->TargetBoneNames.Add(FName(TEXT("L_Weapon_Cp_01")));
//	//	LeftWeapon->TargetBoneNames.Add(FName(TEXT("L_Weapon_Cp_02")));
//	//	LeftWeapon->TargetBoneNames.Add(FName(TEXT("L_Weapon_Cp_03")));
//	//	LeftWeapon->TargetBoneNames.Add(FName(TEXT("L_Weapon_Cp_04")));
//
//	//	LeftWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//	//	LeftWeapon->AttachToComponent(SKMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	//	LeftWeapon->ReregisterComponent();
//	//	if (LeftWeapon->SkelComp)
//	//	{
//	//		LeftWeapon->Activate();
//	//	}
//	//}
//
//	//if (RightWeapon)
//	//{
//	//	RightWeapon->TargetBoneNames.Add(FName(TEXT("R_Weapon_Cp_01")));
//	//	RightWeapon->TargetBoneNames.Add(FName(TEXT("R_Weapon_Cp_02")));
//	//	RightWeapon->TargetBoneNames.Add(FName(TEXT("R_Weapon_Cp_03")));
//	//	RightWeapon->TargetBoneNames.Add(FName(TEXT("R_Weapon_Cp_04")));
//
//	//	RightWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//	//	RightWeapon->AttachToComponent(SKMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//
//	//	RightWeapon->ReregisterComponent();
//	//	if (RightWeapon->SkelComp)
//	//	{
//	//		RightWeapon->Activate();
//	//	}
//	//}
//
//	//if (ActionCameraComponent)
//	//{
//	//	ActionCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
//	//}
//
//#if 0
//	if (ShadowMaterial)
//	{
//		if (SKMeshComp->GetSocketByName(B2PLAYER_SHADOW_SOCKET_NAME_LF))
//		{
//			SetupDecalCompForLeftShadowCommon(LeftFootShadow, SKMeshComp); // SetupControlledMatineePuppet ������ ����ϴ� ���� �¾� �κ�.. �̾��µ� �ᱹ ���⼭�� ���
//
//			LeftFootShadow->ReregisterComponent();
//			LeftFootShadow->SetDecalMaterial(ShadowMaterial);
//			LeftFootShadow->Activate();
//		}
//		else
//		{
//			LeftFootShadow->Deactivate(); // ������ �����Ǿ��ٸ� ���� ���ܾ� �̻��� �� �� ����.
//			LeftFootShadow->SetVisibility(false);
//		}
//
//		if (SKMeshComp->GetSocketByName(B2PLAYER_SHADOW_SOCKET_NAME_RF))
//		{
//			SetupDecalCompForRightShadowCommon(RightFootShadow, SKMeshComp);  // SetupControlledMatineePuppet ������ ����ϴ� ���� �¾� �κ�.. �̾��µ� �ᱹ ���⼭�� ���
//
//			RightFootShadow->ReregisterComponent();
//			RightFootShadow->SetDecalMaterial(ShadowMaterial);
//			RightFootShadow->Activate();
//		}
//		else
//		{
//			RightFootShadow->Deactivate();
//			RightFootShadow->SetVisibility(false);
//		}
//	}
//#endif
//
//	auto* InGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	check(InGameMode);
//
//	if (InGameMode->bOverrideFloorRingColor && InGameMode->FloorRingColors.Num() > GetTeamNum())
//	{
//		FloorRingColor = InGameMode->FloorRingColors[GetTeamNum()];
//	}
//
//	if (InGameMode->bOverrideFloorRingColor && InGameMode->FloorRingColorMultipliers.Num() > GetTeamNum())
//	{
//		FloorRingColorMultiplier = FMath::Max(1.f, InGameMode->FloorRingColorMultipliers[GetTeamNum()]);
//	}
//
//	if (FloorRingPS)
//	{
//		ChangeFloorRingPS(FloorRingPS);	
//	}
//
//	SetupLightComponentAttachment();
//	// GameRule ����ؼ� ��..
//	FormalSetPointLightOn(true);
//	BII_CHECK(GetInjectedGameRule());
//
//	SubscribeEvents();
//
//	// UpdateCamera** �� ���� ������ ������ Camera** ������ ���� ĳ�̵�.
//	CameraDistance = CameraBoom->TargetArmLength;
//	//CameraPitch = CameraBoom->RelativeRotation.Pitch;
//	//CameraYaw = CameraBoom->RelativeRotation.Yaw;
//	PlayerTopDownCamInitialFOV = TopDownCameraComponent->FieldOfView;
//
//	CameraLengthValueForBlending.TargetArmLength = CameraDistance;
//	CameraFovValueForBlending.CameraFov = PlayerTopDownCamInitialFOV;
//
//	LastAttackSuccessTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
//	CurrentComboNum = 0;
//
//	CanEvade = true;
//	EvadeTimer = 0.0f;
//
//	InitUpdateUIDoc(); // ���� Ŭ�������� �θ��� �Ұǵ� ���⼭ �� �ؼ� ���� �� ����. �׻� ������Ʈ �Ǵ� �� ���ü���
//
//
//#if !UE_BUILD_SHIPPING
//	extern bool gUseCharacterDefaultMaterial; // ĳ���� default material �׽�Ʈ
//	if (gUseCharacterDefaultMaterial)
//	{
//		USkeletalMeshComponent* MainMeshComp = GetMesh();
//		for (int32 MI = 0; MI < MainMeshComp->GetNumMaterials(); ++MI)
//		{
//			//MainMeshComp->SetMaterial(MI, UMaterial::GetDefaultMaterial(MD_Surface));
//		}
//	}
//#endif
//}
//
//void ABladeIIPlayer::BeginDestroy()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_BeginDestroy);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::BeginDestroy"));
//
//	// Subscribe �� �� Player ��.. Unsubscribe �� �˴� �����ع���.
//	//ABladeIIPlayer* CurrentLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	//if ((!CurrentLocalB2Player || CurrentLocalB2Player == this)) // ���⼭�� LocalPlayerCharacter �˻縦 �� �ʿ䰡 �ִ�. �±� ���� ��� ����/�ı� ������ �������� ���� �� ����.
//	//{
//	//	UnsubscribeEvents();
//	//}
//
//	Super::BeginDestroy();
//}
//
//void ABladeIIPlayer::Destroyed()
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::Destroyed"));
//
//	// This route is what we can expect from tagging.
//	DestroyPlayerUIManager();
//	UnsubscribeEvents();
//
//	Super::Destroyed();
//}
//
//void ABladeIIPlayer::PreSave(FObjectPreSaveContext SaveContext)
//{
//	Super::PreSave(SaveContext);
//
//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		// ��� ĳ���� spawn �� ���̴� ���̽� �������Ʈ���� �⺻�� ���ҽ����� ���� �ִµ� �̰� �������� ���� ������ Ŭ �� �ִ�. ��ŷ ���̺� �� ����.
//		//if (IsSavingForCookingOnPreSave(TargetPlatform) && GetMesh())
//		//{
//		//	// ���⼭ null ��Ų �� PCClassInfo �� ���� ������ �Ǿ��.
//		//	FloorRingPS = nullptr;
//		//	ReadyWeaponSkillFX = nullptr;
//		//	HUDInfoClass = nullptr;
//
//		//	UE_LOG(LogBladeII, Log, TEXT("NULL out resouce properties of %s"), *this->GetPathName());
//		//}
//	}
//#endif
//}
//
//void ABladeIIPlayer::EnableInput(APlayerController* controller)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_EnableInput);
//	SubscribeEvents();
//	Super::EnableInput(controller);
//}
//
//void ABladeIIPlayer::DisableInput(APlayerController* controller)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_DisableInput);
//	UnsubscribeEvents();
//	Super::DisableInput(controller);
//}
//
//void ABladeIIPlayer::SubscribeEvents()
//{
//	if (bEventsSubscribed)
//		return;
//
//	// ���� ���� Player �� ���� �̺�Ʈ�� �޾ƾ� �Ѵٸ� Subscribe ���ϰ��� �����ߴٰ� Unsubscribe �� ���.
//	CAPTURE_UOBJECT(ABladeIIPlayer);
//
//	PlayerStartAttackTicket = PlayerStartAttackClass<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartAttack)
//			Capture->StartAttackFromInput();
//		END_CAPTURE_OBJECT()
//	);
//	PlayerStopAttackTicket = PlayerStopAttackClass<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStopAttack)
//			Capture->StopAttackFromInput();
//		END_CAPTURE_OBJECT()
//	);
//	PlayerStartSkill01Ticket = PlayerStartSkill01Class<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartSkill01)
//			Capture->StartSkill01();
//		END_CAPTURE_OBJECT()
//	);
//	PlayerStartSkill02Ticket = PlayerStartSkill02Class<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartSkill02)
//			Capture->StartSkill02();
//		END_CAPTURE_OBJECT()
//	);
//	PlayerStartSkill03Ticket = PlayerStartSkill03Class<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartSkill03)
//			Capture->StartSkill03();
//		END_CAPTURE_OBJECT()
//	);
//	PlayerStartWeaponSkillTicket = PlayerStartWeaponSkillClass<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartWeaponSkill)
//			Capture->StartWeaponSkill();
//		END_CAPTURE_OBJECT()
//	);
//	PlayerStartGuardTicket = PlayerStartGuardClass<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartGuard)
//			Capture->StartGuardFromInput();
//		END_CAPTURE_OBJECT()
//	);
//	PlayerStopGuardTicket = PlayerStopGuardClass<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStopGuard)
//			Capture->StopGuardFromInput();
//		END_CAPTURE_OBJECT()
//	);
//	PlayerStartTagTicket = PlayerStartTagClass<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PlayerStartTag)
//			Capture->StartTag();
//		END_CAPTURE_OBJECT()
//	);
//	QTEAttackTicket = QTEAttackClass<>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(QTEAttack)
//			Capture->StartQTE();
//		END_CAPTURE_OBJECT()
//	);
//	PleaseForgetMeTicket = PleaseForgetMeClass<ABladeIICharacter*>::GetInstance().Subscribe(
//		USE_CAPTURE_OBJECT_AND_TICKET(PleaseForgetMe, ABladeIICharacter* CharAboutTobeDestroyed)
//			Capture->OnSomeMobTermination(CharAboutTobeDestroyed);
//		END_CAPTURE_OBJECT()
//	);
//
//	bEventsSubscribed = true;
//}
//
//void ABladeIIPlayer::UnsubscribeEvents()
//{
//	if (!bEventsSubscribed)
//		return;
//
//	PlayerStartAttackClass<>::GetInstance().Unsubscribe(PlayerStartAttackTicket);
//	PlayerStopAttackClass<>::GetInstance().Unsubscribe(PlayerStopAttackTicket);
//	PlayerStartSkill01Class<>::GetInstance().Unsubscribe(PlayerStartSkill01Ticket);
//	PlayerStartSkill02Class<>::GetInstance().Unsubscribe(PlayerStartSkill02Ticket);
//	PlayerStartSkill03Class<>::GetInstance().Unsubscribe(PlayerStartSkill03Ticket);
//	PlayerStartWeaponSkillClass<>::GetInstance().Unsubscribe(PlayerStartWeaponSkillTicket);
//	PlayerStartGuardClass<>::GetInstance().Unsubscribe(PlayerStartGuardTicket);
//	PlayerStopGuardClass<>::GetInstance().Unsubscribe(PlayerStopGuardTicket);
//	PlayerStartTagClass<>::GetInstance().Unsubscribe(PlayerStartTagTicket);
//	QTEAttackClass<>::GetInstance().Unsubscribe(QTEAttackTicket);
//	PleaseForgetMeClass<ABladeIICharacter*>::GetInstance().Unsubscribe(PleaseForgetMeTicket);
//
//	bEventsSubscribed = false;
//}
//
//void ABladeIIPlayer::FinalizePCClassInfoSpawn(bool bFinalizeWithoutInGameInfo)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_FinalizePCClassInfoSpawn);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::FinalizePCClassInfoSpawn"));
//
//	if (bFinalizedByPCClassInfo == false)
//	{
//		ABladeIIGameMode* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//		//if (B2GameMode)
//		//{
//		//	// Override by B2PCClassInfo. 
//		//	// BladeIIGameMode may set PCClassEnum during its own spawning operation, and it will be picked up here. 
//		//	this->OverrideByPCClassInfo(B2GameMode, nullptr, bFinalizeWithoutInGameInfo);
//		//				
//		//	B2GameMode->FinishSpawnByPCClassInfo(); // �ѹ� ���ǰ� ���� �ٷ� ����
//
//		//	bFinalizedByPCClassInfo = true;
//
//		//	InjectGameMode(B2GameMode);
//		//}
//	}
//}
//
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
///** ���� ĳ���� ������ ���� ġƮ */
//float gLocalPCCombatStatScale = -1.0f;
//#endif
//
//void ABladeIIPlayer::InitializeCombatStats()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_InitializeCombatStats);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::InitializeCombatStats"));
//
//	// Super::InitializeCombatStats �� ������� �ʴ´�.
//	
//	// ���⼱ �������� ����� ������
//	// GetCurrentPlayerClass �� ����� �� ���� �����Ϸ��� OverrideByPCClassInfo �� ó���� ���Ŀ��� ��.
////	GetSkillRTManager();
////	GetCharacterDataStore();
////
////	if (CachedSkillRTManager == nullptr || CharacterDataStore == nullptr) //�Ϲ����� �÷��̾ �ƴ� �����ֱ��(�¸��������� �±�ĳ����..)
////		return; 
////
////	LocalizedUserNickName = FText::FromString(CharacterDataStore->GetUserNickName());
////	
////	auto* BladeGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
////
////	TotalAttack = BladeGameMode->GetTotalAttack(GetCharacterDataStore());
////	TotalDefense = BladeGameMode->GetTotalDefense(GetCharacterDataStore());
////
////	//��� ���� ���� Doc���� �ҷ��´�
////	auto GuildDoc = UB2UIDocHelper::GetDocGuild();
////
////	check(GuildDoc)
////
////	float GuildAttack = GuildDoc->GetAttackGuildBuff();
////	float GuildDefense = GuildDoc->GetDefenseGuildBuff();
////	float GuildHealth = GuildDoc->GetHealthGuildBuff();
////
////	GuildAttack = TotalAttack * (GuildAttack * 0.01f);
////	GuildDefense = TotalDefense * (GuildDefense * 0.01f);
////
////	// ��Ȱ ���� ����
////	//TMap<EResurrectBuffType, int32> AppliedResurrectBuff;
////	//CachedSkillRTManager->GetAppliedResurrectionBuffValues(AppliedResurrectBuff);
////	int32* AttackBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Attack);
////	if (AttackBuffIfAny && *AttackBuffIfAny > 0)
////	{
////		TotalAttack = TotalAttack + TotalAttack * ((float)(*AttackBuffIfAny) * 0.01f);
////	}
////	int32* DefenseBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Defense);
////	if (DefenseBuffIfAny && *DefenseBuffIfAny > 0)
////	{
////		TotalDefense = TotalDefense + TotalDefense * ((float)(*DefenseBuffIfAny) * 0.01f);
////	}
////
////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ġƮ�� ���� ���� ����
////	if (gLocalPCCombatStatScale > 0.0f && IsLocalPlayer())
////	{
////		TotalAttack *= gLocalPCCombatStatScale;
////		TotalDefense *= gLocalPCCombatStatScale;
////	}
////#endif
////
////	float AttackCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
////	float DefenseCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
////	if (GetGameMode())
////	{
////		AttackCoefficientValue = GetGameMode()->GetAttackCoefficient();
////		DefenseCoefficientValue = GetGameMode()->GetDefenseCoefficient();
////	}
////
////	AttackLevel = CombatStatEval::GetADLevel(TotalAttack, AttackCoefficientValue);
////	DefenseLevel = CombatStatEval::GetADLevel(TotalDefense / 3.f, DefenseCoefficientValue);
////
////	// Need to be change to use formula, skill/item %, etc...
////	MaxHealth = CombatStatEval::GetPCHealth(GetCurrentPlayerClass(), CharacterDataStore);
////	MaxHealth += CombatStatEval::GetPCHealthByMod(GetCurrentPlayerClass(), BladeGameMode->GetB2GameModeType(), CharacterDataStore);
////
////	// ��庰 ü�±��� �ջ� �� �⺻ ü�º��� ���� ü���� �ȴٸ� �⺻ ü�±����� �������ش�.
////	MaxHealth = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseHealth(CharacterDataStore->GetCharacterLevel(GetCurrentPlayerClass()))), MaxHealth);
////
////	GuildHealth = MaxHealth * (GuildHealth * 0.01f);			//��� ���� ž��
////
////	int32* HealthBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Health);
////	if (HealthBuffIfAny && *HealthBuffIfAny > 0)
////	{
////		MaxHealth = MaxHealth + MaxHealth * ((float)(*HealthBuffIfAny) * 0.01f); // ü�� ������ MaxHealth ��? �ƴ� MaxHealth ���� Health �� Ŀ����?
////	}
////
////	TotalAttack += GuildAttack;
////	TotalDefense += GuildDefense;
////	MaxHealth += GuildHealth;
////	Health = MaxHealth;
////
////	// No formula for armor for now
////	Armor = MaxArmor;
////
////	// ��Ȱ ������ �����Ѵٸ� ��Ȱ ��Ȳ�̶�� ���̱� ������ ��Ȱ�ÿ��� ����Ʈ�� �����͸� �������� �ʴ´�.
////	if (AB2StageManager::GetCacheStageKeepEssentialData().IsPlayInExtraLevel()
////		&& AppliedResurrectBuff.Num() == 0)
////	{ // ExtraMap ��ȯ �� ó�� �Ϻ� ���⵵..
////		// Health �� �Ƹ� �����ؼ� SkillRTManager �ʿ�����
////		AB2StageManager::GetCacheStageKeepEssentialData().SetPostExtraMapLoadB2Player(this);
////	}
////		
////	// �⺻ ��/��/ü �̿��� ���� �ɼ� ��ġ�� ĳ��.
////	InitializeCombatOptionCacheValues();
////
////	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
////	if (MovementComp)
////	{ // PCClassInfo ������ �⺻������ �ְ� �������̶� ��ų �ɼ� ���� �ݿ��� walk speed ����
////		MovementComp->MaxWalkSpeed = GetStaticOptionAppliedWalkSpeed();
////	}
////
////	TotalApplyAnimRateScale(); // ���Ŀ��� AttackState �� ���� �����.
////
////	//GameMode�� ���� ���Ⱥ���
////	if (BladeGameMode)
////	{
////		CharacterDamageRate *= BladeGameMode->GetDamageRateByGameModeType();
////		Health *= BladeGameMode->GetHealthRateByGameModeType();
////		MaxHealth *= BladeGameMode->GetHealthRateByGameModeType();
////	}
////
////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ġƮ�� ���� ���� ����
////	if (gLocalPCCombatStatScale > 0.0f && IsLocalPlayer())
////	{
////		Health *= gLocalPCCombatStatScale;
////		MaxHealth *= gLocalPCCombatStatScale;
////	}
////#endif
////
////	if (CachedSkillRTManager)
////	{//Update InactivePlayerState
////		CachedSkillRTManager->InitInactiveCharacterMaxHealth(MaxHealth);
////	}
////
////	if (EtherContainer)
////		EtherContainer->InitializeContainer(this);
//}
//
//
//void ABladeIIPlayer::InitializeCombatOptionCacheValues()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_InitializeCombatOptionCacheValues);
//	const EPCClass CurrPCClassEnum = GetCurrentPlayerClass();
//	checkSlow(CharacterDataStore); // ���� ȣ�� Ÿ�̹� �� �̰� �̹� �ʱ�ȭ �� ���¿� ���� �ɷ�.
//
//	//// ���� �ɼ� ��ġ�� ĳ��. ���� ���� �ÿ� Inc** �� ���ϰ� Dec** �� ����. GetEffectiveValueForOptionApplyType ����.
//
//	//BaseDamageDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecDamage, CharacterDataStore);
//	//MeleeDamageDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecMeleeDamage, CharacterDataStore);
//	//RangeDamageDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecRangedDamage, CharacterDataStore);
//	//BossDamageDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecBossDamage, CharacterDataStore);
//	//ResistCriticalRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_ResistCritical, CharacterDataStore);
//	//StateDurationDecreaseRate = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecAbnormalStateTime, CharacterDataStore);
//	//DecAdditionalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_DecAdditionalDamage, CharacterDataStore);
//
//	//IncDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncDamage, CharacterDataStore);
//	//IncNormalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncNormalDamage, CharacterDataStore);
//	//IncSkillDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Skill_IncSkillDamage, CharacterDataStore);
//	//// �Ʒ��� ������ ID �� Ư�� ActiveSkill ���� ����Ǵ� �ɼǵ鵵 �־ �װ͵��� Ư�� AttackState �� ���� ���� ���� �����ͼ� ����.
//	//IncCriticalRateValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncCriticalRate, CharacterDataStore);
//	//IncCriticalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncCriticalDamage, CharacterDataStore);
//	//IncDamageToBossValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncBossDamage, CharacterDataStore);
//	//IncAdditionalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncAdditionalDamage, CharacterDataStore);
//
//	//// �Ʒ� Heal/Absorb HP ��ġ���� �ִ� health �� ��� ȸ�� ����.
//	//HealHPAmountRateOnCounter = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_HealHPByCountAttack, CharacterDataStore);;
//	//HealHPAmountRateOnKill = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Health_ProbableRecoverHP, CharacterDataStore);
//	//AbsorbHPAmountRateOnAttack = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Health_ProbableHPAbsorption, CharacterDataStore);
//
//	//AdditionalAttackSpeedRatebyPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncAttackSpeed, CharacterDataStore);
//
//	//// PC Ŭ���� ���� �ĺ��ڰ� ���еǴ� �ɼǵ�.
//	//int32 AttackDamageByClassBaseIdx = static_cast<int32>(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToGladiator);
//	//int32 ReceiveDamageByClassBaseIdx = static_cast<int32>(EUnitedCombatOptions::UCO_Defense_DecDamageByGladiator);
//	//for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
//	//{
//	//	IncAttackDamageByPassiveToClass[i] = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, static_cast<EUnitedCombatOptions>(AttackDamageByClassBaseIdx + i), CharacterDataStore);
//	//	DecReceiveDamageByPassiveFromClass[i] = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, static_cast<EUnitedCombatOptions>(ReceiveDamageByClassBaseIdx + i), CharacterDataStore);
//	//}
//
//	////SetRegenerateHPRate(
//	////	// �̰� �ۼ�Ʈ �ƴ� ���밪 �����.
//	////	CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Health_PerSecRecoverHP, CharacterDataStore),
//	////	false
//	////);
//}
//
////////////////////////////////////////////////////////////////////////////
//// Input
//
//void ABladeIIPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupPlayerInputComponent);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupPlayerInputComponent"));
//
//	// Set up gameplay key bindings
//	check(PlayerInputComponent);
//	InputComponent->BindAxis("MoveForward", this, &ABladeIIPlayer::MoveForward);
//	InputComponent->BindAxis("MoveRight", this, &ABladeIIPlayer::MoveRight);
//
//	FInputActionBinding& FirePressed = PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABladeIIPlayer::StartAttackFromInput);
//	//FirePressed.bConsumeInput = false;
//	FInputActionBinding& FireReleased = PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABladeIIPlayer::StopAttackFromInput);
//	//FireReleased.bConsumeInput = false;
//
//	FInputActionBinding& SkillPressed_01 = PlayerInputComponent->BindAction("Skill_01", IE_Pressed, this, &ABladeIIPlayer::StartSkill01);
//	FInputActionBinding& SkillPressed_02 = PlayerInputComponent->BindAction("Skill_02", IE_Pressed, this, &ABladeIIPlayer::StartSkill02);
//	FInputActionBinding& SkillPressed_03 = PlayerInputComponent->BindAction("Skill_03", IE_Pressed, this, &ABladeIIPlayer::StartSkill03);
//	FInputActionBinding& SkillPressed_Weapon = PlayerInputComponent->BindAction("Skill_Weapon", IE_Pressed, this, &ABladeIIPlayer::StartWeaponSkill);
//	//FInputActionBinding& SkillReleased = InputComponent->BindAction("Skill", IE_Released, this, &ABladeIIPlayer::StopSkill);
//
//	FInputActionBinding& TagPressed = PlayerInputComponent->BindAction("Tag", IE_Pressed, this, &ABladeIIPlayer::StartTag);
//
//	FInputActionBinding& GuardPressed = PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &ABladeIIPlayer::StartGuardFromInput);
//	FInputActionBinding& GuardReleased = PlayerInputComponent->BindAction("Guard", IE_Released, this, &ABladeIIPlayer::StopGuardFromInput);
//
//	FInputActionBinding& QTEPressed = PlayerInputComponent->BindAction("QTE", IE_Pressed, this, &ABladeIIPlayer::StartQTEByKeyPress);
//
//	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
//	// "turn" handles devices that provide an absolute delta, such as a mouse.
//	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
//	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
//	//InputComponent->BindAxis("TurnRate", this, &ABladeIICharacter::TurnAtRate);
//	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
//	//InputComponent->BindAxis("LookUpRate", this, &ABladeIICharacter::LookUpAtRate);
//
//	// handle touch devices
//	InputComponent->BindTouch(IE_Pressed, this, &ABladeIIPlayer::TouchStarted);
//	InputComponent->BindTouch(IE_Released, this, &ABladeIIPlayer::TouchStopped);
//}
//
//bool ABladeIIPlayer::IsFineToStartAnyNewInputHandling()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsFineToStartAnyNewInputHandling);
//	//ABladeIIPlayerController* B2Controller = Cast<ABladeIIPlayerController>(GetController());
//	//BII_CHECK(B2Controller);
//	//return (IsAlive() && B2Controller != nullptr && B2Controller->IsFineToStartAnyNewInputHandling());
//	return false;
//}
//
//bool ABladeIIPlayer::IsFineToStartAnyNewMoveInputHandling()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsFineToStartAnyNewMoveInputHandling);
//	if (!GetAbleToMove())
//	{
//		return false;
//	}
//	// MoveInput �� ���� ���� ��찡 �־ �߰� �˻�
//	//ABladeIIPlayerController* B2Controller = Cast<ABladeIIPlayerController>(GetController());
//	//BII_CHECK(B2Controller);
//	//return (IsAlive() && B2Controller != nullptr && B2Controller->IsFineToStartAnyNewMoveInputHandling());
//	return false;
//}
//
//void ABladeIIPlayer::OnStartSkill(EAttackState SkillType)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnStartSkill);
//
//	// ���� �޺����忡�� ���ִ°ǵ� ��ų�����Ҷ��� �� ���ش�.
//	// ��ų�� - ���� - ������ų
//	// �̰� �ȳ�����..
//	//GetAnimInstance()->SetComboSkill(false);
//
//	//ClearBuff(EBuffType::Buff_Hide);
//
//	//if (SkillType != EAttackState::ECS_WeaponSkill && Role == ROLE_Authority)
//	//	AddWeaponSkillPoint(1.0f);
//
//	//if (SkillType == EAttackState::ECS_WeaponSkill)
//	//{
//	//	ClearDebuffs();
//
//	//	// ���� Ŭ����ܰ�(����, ����)���� ���� �޺������ �����ϴ°� �ִ�. ���⼱ ���.
//	//	// ���� �޺����� �����س�� �ִ�BP���� ���ý�����Ʈ ������ �ٲ������. ECS_Skill04 ��ų�� �ȳ���.
//	//	ConsumeComboEnd();
//	//	ResetDamageNum();
//	//}
//
//	//if (SkillType == EAttackState::ECS_Skill04)
//	//{
//	//	if (BuffManager && BuffManager->CrowdControlCount > 0)
//	//		SpawnSurviveSkillEffect();
//
//	//	ClearDebuffs();
//
//	//	// ���� Ŭ����ܰ�(����, ����)���� ���� �޺������ �����ϴ°� �ִ�. ���⼱ ���.
//	//	// ���� �޺����� �����س�� �ִ�BP���� ���ý�����Ʈ ������ �ٲ������. ECS_Skill04 ��ų�� �ȳ���.
//	//	ConsumeComboEnd();
//	//	ResetDamageNum();
//	//
//	//	// Apply Cooltime decrease.. Ư���� ��Ÿ�� �����̸� ��Ÿ ��Ÿ�� ���� �ɼǵ��� SkillRTManager �ʿ� �����Ǿ� �ִ�.
//	//	// ���� ECombatOptionApplyType �� Subtract_Value �� �������� OneMinusMultiply �� �ٲ� �� �ִ�.
//	//	if (CachedSkillRTManager && Role == ROLE_Authority)
//	//	{
//	//		const auto& SkillOption = CachedSkillRTManager->GetCachedSkillOption();
//
//	//		const float CooltimeDecrease = SkillOption.GetActiveSkillOptionData(SkillType, ESkillOption::ESO_DecCoolTimeOtherSkills);
//	//		if (CooltimeDecrease > 0.f)
//	//			CachedSkillRTManager->CountRemainingCoolTime(CooltimeDecrease);
//	//	}
//	//}
//}
//
//bool ABladeIIPlayer::IsQTEMounting() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsQTEMounting);
//	return IsInQTEState() && QTEActor && QTEActor->IsControlledByQTEPlayer() && QTEType == EQTEType::EQT_Mount;
//}
//
//bool ABladeIIPlayer::IsQTEMountingAndReadyForAttack() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsQTEMountingAndReadyForAttack);
//	// ����Ʈ���� ���� ���ݱ��� �ٷ� ������ ����..
//	return (IsQTEMounting() && this->QTEStateNum >= 1 && QTEActor->QTEStateNum >= 1);
//}
//
//void ABladeIIPlayer::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TouchStarted);
//	if (IsFineToStartAnyNewInputHandling())
//	{
//		if (!bIsSwiping)
//		{
//			bIsSwiping = true;
//			LastTouchLocation = Location;
//			SwipeTouchIndex = FingerIndex;
//		}
//	}
//	else
//	{
//		/* Cinematic ������ ���� Input�� ���� */
//		//bCachedPressedInCinematic = true;
//	}
//}
//
//void ABladeIIPlayer::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TouchStopped);
//	//if (FingerIndex == SwipeTouchIndex && bIsSwiping && FVector(Location - LastTouchLocation).Size() > 100.f)
//	//{
//	//	bIsSwiping = false;
//	//	BII_CHECK(CachedSkillRTManager);
//	//	if (CachedSkillRTManager && CachedSkillRTManager->IsTagAttackSuggested())
//	//	{
//	//		StartTag();
//	//	}
//	//}
//
//	bCachedPressedInCinematic = false;
//}
//
//void ABladeIIPlayer::TurnAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
//}
//
//void ABladeIIPlayer::LookUpAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
//}
//
//void ABladeIIPlayer::MoveForward(float Value)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_MoveForward);
//
//	//ProcessInputMovement(Value, CameraBoom->RelativeRotation.Yaw, EAxis::X);
//}
//
//void ABladeIIPlayer::MoveRight(float Value)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_MoveRight);
//	
//	//ProcessInputMovement(Value, CameraBoom->RelativeRotation.Yaw, EAxis::Y);
//}
//
//bool ABladeIIPlayer::CanHandleInputMovement(float InputValue)
//{
//	if (InputValue == 0.0f || (!IsFineToStartAnyNewMoveInputHandling() && !IsQTEMounting()))
//		return false;
//
//	return true;
//}
//
//void ABladeIIPlayer::ProcessInputMovement(float Value, float BoomYaw, EAxis::Type StandardAxis)
//{
//	if (CanHandleInputMovement(Value))
//	{
//		// find out which way is right
//		const FRotator YawRotation(0, BoomYaw, 0);
//		// get right vector 
//		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(StandardAxis);
//
//		// add movement in that direction
//		if (IsQTEMounting())
//		{
//			QTEActor->AddMovementInput(Direction, Value, false);
//		}
//		else
//		{
//			AddMovementInput(Direction, Value);
//		}
//
//		OnInputMovement(Direction);
//	}
//}
//
//void ABladeIIPlayer::OnInputMovement(const FVector& MoveDirection)
//{
//	//AB2AutoAIController* AutoAIController = GetAutoAIController();
//	//if (AutoAIController && AutoAIController->IsRunningAI() && IsLocalPlayer())
//	//{
//	//	const bool InAttackMotion = IsOneOfComboAttackState(GetAnimAttackState());
//	//	if (InAttackMotion && m_bAttackKeyPressed == false)
//	//		StopAttack();
//
//	//	AutoAIController->SetPCHadMovementInput(); // Let the AutoBT knows PC had input.
//	//}
//}
//
//void ABladeIIPlayer::UpdateCameraPitch(float InPitch)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraPitch);
//	// CameraZoom ���� �ӽ� ȿ�� �� ���� ������ ���� default ���� ĳ��
//	CameraPitch = InPitch;
//	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CameraRoll));
//}
//
//void ABladeIIPlayer::UpdateCameraYaw(float InYaw)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraYaw);
//	// CameraZoom ���� �ӽ� ȿ�� �� ���� ������ ���� default ���� ĳ��
//	CameraYaw = InYaw;
//	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CameraRoll));
//}
//
//void ABladeIIPlayer::UpdateCameraRoll(float InRoll)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraRoll);
//	// CameraZoom ���� �ӽ� ȿ�� �� ���� ������ ���� default ���� ĳ��
//	CameraRoll = InRoll;
//	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CameraRoll));
//}
//
//float ABladeIIPlayer::GetCameraPitch() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraPitch);
//	return CameraPitch;
//}
//
//float ABladeIIPlayer::GetCameraYaw() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraYaw);
//	return CameraYaw;
//}
//
//float ABladeIIPlayer::GetCameraRoll() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraRoll);
//	return CameraRoll;
//}
//
//void ABladeIIPlayer::UpdateCameraDistance(float InDistance)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraDistance);
//	// CameraZoom ���� �ӽ� ȿ�� �� ���� ������ ���� default ���� ĳ��
//	CameraLengthValueForBlending.TargetArmLength = CameraBoom->TargetArmLength = CameraDistance = InDistance;
//}
//
//void ABladeIIPlayer::UpdateCameraTransform(const FTransform& InTransform)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraTransform);
//	TopDownCameraComponent->SetRelativeTransform(InTransform);
//}
//
//void ABladeIIPlayer::UpdateCameraTargetFocusOffset(const FVector& Offset)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraTargetFocusOffset);
//	CameraBoom->SetRelativeLocation(Offset);
//}
//
//float ABladeIIPlayer::GetCameraDistance() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraDistance);
//	return CameraDistance;
//}
//
//FVector ABladeIIPlayer::GetCameraTargetFocusOffset()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraTargetFocusOffset);
//	return CameraBoom->GetRelativeTransform().GetTranslation();
//}
//
//void ABladeIIPlayer::UpdateCameraBoomLockedViewTarget(bool bLockedViewTarget)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraBoomLockedViewTarget);
//	CameraBoomLockedViewTarget = bLockedViewTarget;
//
//	//CameraBoom->bAbsoluteRotation = !bLockedViewTarget;
//
//	CameraBoom->UpdateComponentToWorld();
//}
//
//bool ABladeIIPlayer::GetCameraBoomLockedViewTarget()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCameraBoomLockedViewTarget);
//	return CameraBoomLockedViewTarget;
//}
//
//void ABladeIIPlayer::NotifyActorCustomEvent(FName EventName, UObject* OptionalNotifyingObject)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyActorCustomEvent);
//	//if (EventName == CAMERAANIM_STOP_ACTOR_CUSTOM_EVENT)
//	{
//		if (--RemainCameraAnims < 0)
//		{
//			// Reset to original camera
//			ActionCameraComponent->Deactivate();
//			TopDownCameraComponent->Activate();
//			CurrentCameraAnimTime = 0.f;
//			RemainCameraAnims = 0;
//		}
//	}
//
//	Super::NotifyActorCustomEvent(EventName, OptionalNotifyingObject);
//}
//
//void ABladeIIPlayer::StartAttackFromInput()
//{
//	m_bAttackKeyPressed = true;
//	if(CurrentMoveAttackState != EAutoMoveAttackState::AutoMoveToTarget)
//		StartAttack();
//}
//
//void ABladeIIPlayer::StopAttackFromInput()
//{
//	m_bAttackKeyPressed = false;
//	StopAttack();
//}
//
//void ABladeIIPlayer::StartGuardFromInput()
//{
//	if (m_bGuardKeyPressed)
//		return;
//
//	m_bGuardKeyPressed = true;
//	StartGuard(true);
//}
//
//void ABladeIIPlayer::StopGuardFromInput()
//{
//	m_bGuardKeyPressed = false;
//	StopGuard();
//}
//
//void ABladeIIPlayer::StartAttack()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAttack);
//    //	B2_SCOPED_TRACK_LOG( FString::Printf(TEXT("ABladeIIPlayer::StartAttack, Current AttackState %d"), static_cast<int32>(GetAttackState())) );
//
//	SetKeepAttacking(true);
//
//	if (!IsFineToStartAnyNewInputHandling() || !GetAbleToControl())
//		return;
//
//	// GuardBreak ����߿��� �ȵ�
//	if (InGuardBreak())
//		return;
//
//	StartAttackExecute();
//}
//
//void ABladeIIPlayer::StartAttackExecute()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAttackExecute);
//	if (!bEnableNormalAttack)
//		return;
//
//	if (IsInQTEState())
//	{
//		if (IsQTEMounting())
//		{
//			bPendingQTENext = true; // Handled by OnQTENext
//			bNeedUpdateTarget = true;
//			if (QTEStateNum == 1)
//			{
//				OnQTENext();
//			}
//		}
//	}
//	else
//	{
//		if (!GetSignalAttack())
//		{
//			bNeedUpdateTarget = true;
//		}
//
//		SetSignalAttack(true);
//	}
//
//	RotateToTarget(0.f, true);
//
//	ClearBuff(EBuffType::Buff_Hide);
//}
//
//void ABladeIIPlayer::StopAttack()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopAttack);
//
//	if (m_bAttackKeyPressed && CurrentMoveAttackState != EAutoMoveAttackState::AutoMoveToTarget)
//		return;
//
//	SetKeepAttacking(false);
//	SetSignalAttack(false);
//}
//
//void ABladeIIPlayer::StopRepeatingInput(bool bFromInput/* = false*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopRepeatingInput);
//	if (bFromInput)
//	{
//		StopAttackFromInput();
//		StopGuardFromInput();
//	}
//	else
//	{
//		StopAttack();
//		StopGuard();
//	}	
//}
//
//void ABladeIIPlayer::ResetSkillCooltime()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAnySkill);
//	BII_CHECK(CachedSkillRTManager);
//	//if (CachedSkillRTManager)
//	//{
//	//	CachedSkillRTManager->ResetSkillCoolTime();
//	//}
//}
//
//void ABladeIIPlayer::StartAnySkill()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAnySkill);
//	BII_CHECK(CachedSkillRTManager);
//
//	//const bool CanStartSkill = CachedSkillRTManager && TargetActor != nullptr;
//	//if (CanStartSkill)
//	//{
//	//	const int32 SuitableSkillIdx = GetHighPrioritySkillIndex();
//
//	//	if (SuitableSkillIdx != INDEX_NONE)
//	//	{
//	//		const float SkillShootingRange = CachedSkillRTManager->GetSkillShootingRangeForAI(this, SuitableSkillIdx);
//	//		const float DistToTarget = GetTargetLocationVectorFromPlayer(TargetActor).Size();
//
//	//		if (DistToTarget > SkillShootingRange)
//	//			StartAttack();
//
//	//		else if (SuitableSkillIdx != INDEX_NONE)
//	//			StartSkillInner(SuitableSkillIdx, false);
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::StartSkill01()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartSkill01);
//	StartSkillInner(0, true);
//}
//
//void ABladeIIPlayer::StartSkill02()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartSkill02);
//	StartSkillInner(1, true);
//}
//
//void ABladeIIPlayer::StartSkill03()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartSkill03);
//	StartSkillInner(2, true);
//}
//
//void ABladeIIPlayer::StartWeaponSkill()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartWeaponSkill);
//	//StartSkillInner(WEAPON_6STAR_SKILL_INDEX, true);
//	//bOverUnityPoint = true;
//}
//
//void ABladeIIPlayer::AddWeaponSkillPoint(float ToAddPoint)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_AddWeaponSkillPoint);
//	SetWeaponSkillPoint(WeaponSkillPoint + ToAddPoint);	
//}
//
//void ABladeIIPlayer::SetWeaponSkillPoint(float ToSetPoint)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetWeaponSkillPoint);
//
//	if (!IsAlive())
//		return;
//
//	float MaxWeaponSkillPoint = 0.0f;
//	//GetSkillRTManager();
//	//if (CachedSkillRTManager)
//	//{
//	//	MaxWeaponSkillPoint = CachedSkillRTManager->GetActiveCharacterMaxWeaponSkillPoint();
//	//	WeaponSkillPoint = FMath::Clamp(ToSetPoint, 0.0f, MaxWeaponSkillPoint);
//	//	
//	//	if (WeaponSkillPoint == MaxWeaponSkillPoint)
//	//	{
//	//		bOverUnityPoint = false;
//	//	}
//
//	//	CachedSkillRTManager->UpdateWeaponSkillPoint(WeaponSkillPoint);
//	//}
//
//	AttachReadyWeaponSkillFX(MaxWeaponSkillPoint == WeaponSkillPoint);
//}
//
//void ABladeIIPlayer::SetWeaponSkillPointMax()
//{
//	//if (CachedSkillRTManager)
//		//SetWeaponSkillPoint(CachedSkillRTManager->GetActiveCharacterMaxWeaponSkillPoint());
//}
//
//bool ABladeIIPlayer::IsWeaponSkillPointMax()
//{
//	//if (CachedSkillRTManager)
//	//	return GetWeaponSkillPoint() == CachedSkillRTManager->GetActiveCharacterMaxWeaponSkillPoint();
//
//	return false;
//}
//
//void ABladeIIPlayer::AttachReadyWeaponSkillFX(bool bAttach)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_AttachReadyWeaponSkillFX);
//	//if (CachedSkillRTManager)
//	//{
//	//	EAttackState CurrentInputSkill = CachedSkillRTManager->GetSkillOfInputIndex(WEAPON_6STAR_SKILL_INDEX);
//	//	if (CurrentInputSkill != EAttackState::ECS_WeaponSkill)
//	//		bAttach = false;
//	//}
//
//
//	//if (bAttach)
//	//{
//	//	if (ReadyWeaponSkillPSC == nullptr)
//	//	{
//	//		ReadyWeaponSkillPSC = NewObject<UParticleSystemComponent>(this, TEXT("ReadyWeaponSkillPSC"), RF_Transient);
//	//		ReadyWeaponSkillPSC->SecondsBeforeInactive = 0.f;
//	//		ReadyWeaponSkillPSC->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	//		ReadyWeaponSkillPSC->SetTemplate(ReadyWeaponSkillFX);
//	//		ReadyWeaponSkillPSC->RegisterComponent();
//	//	}
//	//	
//	//	ReadyWeaponSkillPSC->Activate();
//	//}
//	//else if (ReadyWeaponSkillPSC && !bAttach)
//	//{
//	//	ReadyWeaponSkillPSC->Deactivate();
//	//}
//}
//
//EAttackState ABladeIIPlayer::StartSkillInner(int32 SkillInputIndex, bool IsPlayerInput)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartSkillInner);
//	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartSkillInner, Current AttackState %d"), static_cast<int32>(GetAttackState())));
//
//	BII_CHECK(CachedSkillRTManager);
////
////	if (!IsFineToStartAnyNewInputHandling() || !CachedSkillRTManager)
////	{
////#if FORCE_LOGGING_IN_ANDROID
////		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Skill - IsFineToStartAnyNewInputHandling false"));
////#endif
////		return EAttackState::ECS_None;
////	}
////
////	EAttackState CurrentInputSkill = CachedSkillRTManager->GetSkillOfInputIndex(SkillInputIndex);
////	if (IsAvailableStartSkill(CurrentInputSkill) == false)
////		return EAttackState::ECS_None;
////
////	EAttackState FinalAttackState = CachedSkillRTManager->GetStartableSkill(SkillInputIndex, IsPlayerInput);
////	if (FinalAttackState == EAttackState::ECS_None)
////	{
////#if FORCE_LOGGING_IN_ANDROID
////		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Skill Can't Start- StartSkillByInputIndex Failed"));
////#endif
////		return EAttackState::ECS_None;
////	}
////	
////	SetAttackState(FinalAttackState); // Does not actually start by StartSkillByInputIndex call, must set the return value here.
////	CurrentSkillAnimIdx = CachedSkillRTManager->GetSkillAnimIndex(SkillInputIndex); // AnimBP ���� ����ϵ���, GetAnimStateOverrideSequence ���� ����ϵ���.
////	bLastInvokedAnySkillAttack = true;
////
////	OnStartSkill(FinalAttackState);
////	SetReservedSkill(FinalAttackState);
////
////	if (SkillInputIndex == WEAPON_6STAR_SKILL_INDEX)
////	{
////		if (SkillLOD == 0)
////		{ // �÷��̾ �����̼� Ŭ����� �ϹǷ� LOD �� ����� �¾���
////			OnBeginWeaponSkillLOD0();
////		}
////
////		SetWeaponSkillPoint(0);
////	}
////
////	ForceFeedback();
////
////	// ���Ӹ�忡 �˸�(����Ʈ �÷��̾�� ���� ��Ž)
////	GetGameMode()->NotifyPlayerStartSkill(this, FinalAttackState);
//
//	//return FinalAttackState;
//	return EAttackState::ECS_None;
//}
//#if !UE_BUILD_SHIPPING
//// StartSkillInner ���� �ʿ��� �͵� ����. ġƮ ����̴� �Ӹ��� �����鼭 �Ȱ��� ���� �ʿ���� ����.. �ϸ� ����..
//void ABladeIIPlayer::DevTestStartWeaponSkill(ESkillAnimType WantedAnimType)
//{
//	if (WantedAnimType >= ESkillAnimType::ESA_Weapon_01 && WantedAnimType <= ESkillAnimType::ESA_Weapon_05_LOD)
//	{
//		SetAttackState(EAttackState::ECS_WeaponSkill);
//
//		CurrentSkillAnimIdx = WantedAnimType;
//		bLastInvokedAnySkillAttack = true;
//
//		OnStartSkill(EAttackState::ECS_WeaponSkill);
//		SetReservedSkill(EAttackState::ECS_WeaponSkill);
//
//		if (SkillLOD == 0)
//		{
//			OnBeginWeaponSkillLOD0();
//		}
//
//		ForceFeedback();
//
//		// ���Ӹ�忡 �˸�(����Ʈ �÷��̾�� ���� ��Ž)
//		//GetGameMode()->NotifyPlayerStartSkill(this, EAttackState::ECS_WeaponSkill);
//	}
//}
//#endif
//
//bool ABladeIIPlayer::IsAvailableStartSkill(EAttackState InAttackState)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsAvailableStartSkill);
//	if (IsInQTEState())
//	{
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- IsInQTEState"));
//#endif
//		return false; // For now
//	}
//
//	// ���� ����ó��
//	if (IsBuffActive(EBuffType::Debuff_Freezing) && InAttackState == EAttackState::ECS_Skill04)
//	{
//		return true;
//	}
//
//	// �⺻������ �������̳� �ݰ���, �����̻��߿��� ��ų ����� �Ұ���
//	if (IsSkillAttacking())
//	{
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- IsSkillAttacking"));
//#endif
//		return false;
//	} 
//	if (IsCounterAttacking())
//	{
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- IsCounterAttacking"));
//#endif
//		return false;
//
//	}
//	if (GetReservedSkill() != EAttackState::ECS_None)
//	{
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- HasReservedSkill"));
//#endif
//		return false;
//
//	} 
//
//	// �˴ٿ� or �и��߿��� �Ұ���
//	if (HadHugeDamaged())
//	{
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- HadHugeDamaged"));
//#endif
//		return false;
//	}
//
//
//	// ����, ������ �⺻������ �Ұ����ϳ�
//	bool bMovableDebuff = IsStunned() || IsFrozen();
//	if (bMovableDebuff)
//	{
//		if (InAttackState == EAttackState::ECS_Skill04 || InAttackState == EAttackState::ECS_WeaponSkill) // ������ų�� ��� ����
//			return true;
//
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- InMoveableDebuff"));
//#endif
//		return false;
//	}
//
//	if (GetSpawnMotionState() != EPCSpawnMotionState::EPST_Normal)
//		return false;
//
//	return true;
//}
//
//int32 ABladeIIPlayer::GetHighPrioritySkillIndex()
//{
//	//for (int32 PriorityIdx = 0; PriorityIdx < MAX_SKILL_INPUT; PriorityIdx++)
//	//{
//	//	if (AutoSkillPriority.IsValidIndex(PriorityIdx))
//	//	{
//	//		const int32 SkillIndex = AutoSkillPriority[PriorityIdx];
//	//		//if (CachedSkillRTManager->GetStartableSkill(SkillIndex, false) != EAttackState::ECS_None)
//	//		//	return SkillIndex;
//	//	}
//	//}
//
//	return INDEX_NONE;
//}
//
//void ABladeIIPlayer::StartTag()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartTag);
//	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartTag, Current AttackState %d"), static_cast<int32>(GetAttackState())));
//
//	//check(CachedSkillRTManager);
//
//	//if (CanTag() == false)
//	//	return;
//
//	//// ������ IsTagPossible �� �ƴ϶�� ��ư�� Ȱ��ȭ���� ���ƾ� ��.
//	//if (CachedSkillRTManager == NULL || CachedSkillRTManager->IsTagPossible() == false || !IsFineToStartAnyNewInputHandling())
//	//	return;
//
//	//if (CachedSkillRTManager->IsTagPossibleButDeferred())
//	//{
//	//	bPendingTag = true; // ���� �������� �ʴٸ� (���� ���� ��) ���ุ �Ѵ�.
//	//}
//	//else
//	//{
//	//	StartTagCommon();
//	//}
//}
//
//void ABladeIIPlayer::StartTagCommon()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartTagCommon);
//	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartTagCommon, Current AttackState %d"), static_cast<int32>(GetAttackState())));
//
//	if (CanTag() == false)
//		return;
//
//	if (IsLocalPlayer())
//	{
//		auto* DocBattle = GetDocBattle();
//		if (DocBattle)
//		{// ���� �÷��̾� ĳ������ ���� �±� ���θ� UIDoc �� ����. �±� ��ÿ� UI �ʿ��� ���� �ϰ� �±� �� ���µ� ��.
//			//DocBattle->SetbTagAttackPending(CachedSkillRTManager && CachedSkillRTManager->IsTagAttackSuggested());
//		}
//
//		// �±��ص� ���׸� ��Ÿ�� ����
//		/*if (UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass()))
//		{
//			UB2EtherSetEffect* OffenseEtherObject = GetEtherSetEffect(EEtherSetType::Offense);
//			if (OffenseEtherObject)
//			{
//				float OffenseCoolTime = OffenseEtherObject->GetCurrentCoolTime();
//				DocHero->SetCurOffenseEtherCoolTime(OffenseCoolTime);
//			}
//			UB2EtherSetEffect* DefenseEtherObject = GetEtherSetEffect(EEtherSetType::Defense);
//			if (DefenseEtherObject)
//			{
//				float DefenseCoolTime = DefenseEtherObject->GetCurrentCoolTime();
//				DocHero->SetCurDefenseEtherCoolTime(DefenseCoolTime);
//			}
//		}*/
//	}
//
//	if (TagAttackMode > ETagAttackMode::EAM_Disable)
//	{
//		bTagAttackTriggered = true;
//		AddWeaponSkillPoint(1.0f);
//	}
//
//	// ���⼭ �����ϸ� AnimBlueprint, Animation, B2TagNotify ���ļ� UB2SkillRTManager::TagToOther �� �� ��.
//	SetAttackState(EAttackState::ECS_TagOtherChar);
//	bLastInvokedAnySkillAttack = true;
//
//	SetReservedSkill(EAttackState::ECS_TagOtherChar);
//}
//
//void ABladeIIPlayer::ExecutePendingTag()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ExecutePendingTag);
//	//check(CachedSkillRTManager);
//	//if (bPendingTag && CachedSkillRTManager->IsTagPossibleRightNow())
//	//{ // �� ��� ���� ���߿� tag ��û�� ���� ��.
//	//	StartTagCommon();
//	//	bPendingTag = false;
//	//}
//}
//
//bool ABladeIIPlayer::InGuardBreak()
//{
//	return LastGuardBreakType != EGuardBreakLogicType::EGLT_AbleToGuard;
//}
//
//bool ABladeIIPlayer::IsGuardStartable()
//{
//	if (!CanEvade)
//		return false;
//
//	if (!IsFineToStartAnyNewInputHandling())
//		return false;
//
//	if (!GetAbleToControl())
//		return false;
//
//	if (IsInQTEState() || GetAttackState() == EAttackState::ECS_Evade) // || DamagedNum != 0)
//		return false; // For now
//
//	if (InGuardBreak())
//		return false;
//
//	if (GetReservedSkill() != EAttackState::ECS_None)
//		return false;
//
//	return true;
//}
//
//// bFromInput ƽ���ؼ� ���°Ŵ� �����⳪ �ݰ�Ÿ�̸� üũ ���ҷ��� �߰�
//void ABladeIIPlayer::StartGuard(bool bFromInput/* = false*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartGuard);
//	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartGuard, Current AttackState %d"), static_cast<int32>(GetAttackState())));
//
//	BII_CHECK(GetWorld());
//
//
//	//const float PrevGuardTime = LastGuardTime;
//	//if (bFromInput)
//	//	LastGuardTime = GetWorld()->GetTimeSeconds();
//
//	//if (IsGuardStartable() == false)
//	//	return;
//
//	//const float GuardDelta = LastGuardTime - PrevGuardTime;
//	//const bool IsEvadeInput = bFromInput && PrevGuardTime != -1.f;
//	//const bool IsEvadeTime = GuardDelta > 0.f && GuardDelta < EvadeCheckTime;
//
//	//if (IsEvadeInput && IsEvadeTime)
//	//{
//	//	const bool IsCancelableState = IsSkillAttacking() || IsCounterAttacking();
//	//	if (IsCancelableState)
//	//	{
//	//		bSkillCancel = true;
//	//	}
//	//	else
//	//	{
//	//		bNeedUpdateTarget = true;
//
//	//		const auto CanEvadeState = GetAttackState() == EAttackState::ECS_None || GetAttackState() == EAttackState::ECS_GuardEnd;
//	//		if (CanEvadeState)
//	//		{
//	//			SetAttackState(EAttackState::ECS_Evade);
//	//			LastGuardTime = -1.f;
//	//		}
//	//	}
//	//}
//
//	//else
//	//{
//	//	if (auto* AnimInst = GetAnimInstance())
//	//	{
//	//		const auto AnimAttackState = AnimInst->GetAnimAttackState();
//	//		const bool IsGuardableState = (AnimAttackState >= EAttackState::ECS_None && AnimAttackState <= EAttackState::ECS_Combo06);
//	//		const bool IsInDamageAnim = AnimInst->IsDamaged() && AnimInst->GetDamageAnimIndex() > EAttackType::EAT_Default;
//
//	//		if (IsInDamageAnim == false && (IsGuardableState || GetAttackState() == EAttackState::ECS_GuardEnd))
//	//		{
//	//			SetAttackState(EAttackState::ECS_GuardStart);
//	//		}
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::StopGuard(bool bForce/* = false*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopGuard);
//
//	if (m_bGuardKeyPressed && !bForce)
//		return;
//
//	// Don't try guard during skill attack, to get wanted comboend notify 
//	if ((IsGuarding() || GetAttackState() == EAttackState::ECS_GuardStart) && GetReservedSkill() == EAttackState::ECS_None)
//	{
//		SetAttackState(EAttackState::ECS_GuardEnd);
//		//::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);
//	}
//}
//
//void ABladeIIPlayer::StartQTEByKeyPress()
//{
//	// ��ưUI������ �װ����ؼ� ����
//	//UB2UIBattleMain* pBattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain);
//	//UB2UIBattleQTE* CreatedQTEUIP = pBattleMainUI ? pBattleMainUI->GetCreatedQTEUIP() : nullptr;
//	//	
//	//if (CreatedQTEUIP)
//	//	CreatedQTEUIP->OnPressedBtnArea();
//	//else
//	//	StartQTE();
//}
//
//void ABladeIIPlayer::StartQTE(bool bForce/* = false*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartQTE);
//	if (!bForce && !IsQTEStartAble())
//	{
//		return;
//	}
//
//	if (IsInQTEState())
//	{
//		// QTE Next (QTE ���¿��� ���� 2�� QTE �� ���)
//		if (GetQTEEnabled() && QTEActor->IsAlive())
//		{
//			SetQTEEnabled(false);
//			bPendingQTENext = true;
//		}
//		return;
//	}
//
//	// Find QTE Enabled Target
//	bNeedUpdateTarget = false;
//	UpdateTarget(GetLastMovementInputVector() != FVector::ZeroVector && bDisableRotateToInput, true);
//
//	// Only in the control mode And Starting QTE situation, apply a safe QTE distance.
//	bool bUseSafeEnableDistance = GetUseQTESafeEnebleDistance();
//	ABladeIICharacter* pQTETargetActor = FindQTEEnableTarget(bUseSafeEnableDistance);
//
//	if (pQTETargetActor && pQTETargetActor->IsAlive() && pQTETargetActor->GetDamagedNum() == 0 && pQTETargetActor->QTEType != EQTEType::EQT_Break)
//	{
//		SetAttackState(EAttackState::ECS_None);
//		// Start QTE Dash
//		StopRepeatingInput();
//		// QTE Dash ���� �� QTEAttack �� True �� �ٲ��� �ϴµ� �� ���� �̸� True �� �Ǿ��ִٸ� False �� �ٲ۴�.
//		//if (pQTETargetActor->QTEType == EQTEType::EQT_Mount)
//		//{
//		//	if (auto* AnimInst = GetAnimInstance())
//		//	{
//		//		AnimInst->SetQTEAttack(false);
//		//	}
//		//}
//		SetQTEStateFlag(true);
//		bQTERun = false;
//		QTEStateNum = 0;
//		pQTETargetActor->QTEPCClassEnum = GetCurrentPlayerClass();
//		FVector Diff = GetTargetLocationVectorFromPlayer(pQTETargetActor, false);
//		float ToTargetYaw = FRotator::ClampAxis(FMath::Atan2(Diff.Y, Diff.X) * 180.f / PI);
//		SetActorRotation(FRotator(0.f, ToTargetYaw, 0.f));
//		if (pQTETargetActor->QTEType == EQTEType::EQT_Mount)
//		{
//			float fYawForQTEMount = ToTargetYaw - 180.0f;
//			fYawForQTEMount = fYawForQTEMount < 0.0f ? fYawForQTEMount + 360.0f : fYawForQTEMount;
//			pQTETargetActor->SetActorRotation(FRotator(0.f, fYawForQTEMount, 0.f));
//		}
//		else
//		{
//			pQTETargetActor->SetActorRotation(FRotator(0.f, ToTargetYaw, 0.f));
//		}
//
//		// Start mount timer count
//		if (pQTETargetActor->QTEType == EQTEType::EQT_Mount)
//		{
//			OnQTEMountBegin();
//		}
//
//		QTEActor = pQTETargetActor;
//		QTENPCClass = pQTETargetActor->GetNPCClass();
//		pQTETargetActor = NULL;
//	}
//}
//
//void ABladeIIPlayer::OnQTENext()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTENext);
//	//if ((bPendingQTENext || (QTEType == EQTEType::EQT_Mount && QTEStateNum == 0)) && IsInQTEState() && QTEActor && QTEActor->IsAlive())
//	//{
//	//	// EQT_Mount Ÿ�̸� ����������ϴ� ����. ���Ӹ�忡 �˸�
//	//	if (QTEType == EQTEType::EQT_Mount && QTEStateNum == 0)
//	//	{
//	//		// UI�� QTE�����ڰ� �����÷��̾��϶��� ����
//	//		if (IsLocalPlayer())
//	//		{
//	//			auto* DocBattle = GetDocBattle();
//	//			if (DocBattle)
//	//			{
//
//	//				const float FinalMountTime = FMath::Max(5.f, QTEActor->QTEMountTime);
//	//				const float InvalidMountTIme = -1.0f;
//	//				QTEMountRemainingTime = QTEActor->GetQTELimitEnabled() ? InvalidMountTIme : FinalMountTime;
//
//	//				DocBattle->SetMaxQTEMountTime(FinalMountTime); // MaxQTEMountTIme �� ���� �ݹ��� ���� ���� ���̹Ƿ� �̰� ���� ����.
//	//				DocBattle->SetRemainingQTEMountTime(QTEMountRemainingTime);
//	//				DocBattle->SetbIsQTEMounted(true);
//	//			}
//	//		}
//
//	//		GetWorldTimerManager().SetTimer(ReserveQTEActorReportMyDeathToBirthplaceTimerHandle, this, &ABladeIIPlayer::ReserveQTEActorReportMyDeathToBirthplace, 0.2f, false);
//	//		QTEActor->OnMountStartComplete();
//	//		
//	//		ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//
//	//		// ����Ʈ���� ���̵�� �ٲ���.
//	//		// CountQTEMountTime() ���� ���̵��̻󿡼� Ÿ�̸� ���ҽ�Ŵ
//	//		if (pGM)
//	//			pGM->NotifyQTEMountBeginToIdle(this);
//	//	}
//
//	//	bPendingQTENext = false;
//	//	QTEStateNum++;
//	//	QTEActor->QTEStateNum++;
//	//	
//	//	// QTE mount �� ���� ���ݱ��� ���� ���ΰ� ����� �� �ִ� Ÿ�̹����� ����.
//	//	auto* DocBattle = GetDocBattle();
//	//	if (DocBattle)
//	//	{
//	//		DocBattle->SetbIsQTEMountedAndReadyForAttack(this->IsQTEMountingAndReadyForAttack());
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::OnQTEAttackEnd()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTEAttackEnd);
//	if (QTEType == EQTEType::EQT_Mount)
//	{
//		// QTE Attack mode during mount, need to return idle
//		QTEStateNum = 1;
//		QTEActor->QTEStateNum = 1;
//	}
//}
//
//bool ABladeIIPlayer::GetQTEInputEnableReady() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetQTEInputEnableReady);
//	return (IsQTEMounting() && QTEStateNum > 1);
//}
//
//void ABladeIIPlayer::SetQTEState()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetQTEState);
//	SetQTEEnabled(false);
//	if (IsInQTEState() && QTEActor && QTEActor->IsAlive())
//	{
//		QTEActor->SetQTEState();
//		QTEType = QTEActor->QTEType;
//	}
//}
//
//void ABladeIIPlayer::SetQTEStateFlag(bool bInState)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetQTEStateFlag);
//	Super::SetQTEStateFlag(bInState); // �ܼ� �� ����.
//
//	auto* DocBattle = GetDocBattle();
//
//	if (IsLocalPlayer() && DocBattle)
//	{ // UIDoc �� ���°� ������ �ַ��� ���� �̷��� ���� ��.
//		//DocBattle->SetbIsPlayerInQTEState(IsInQTEState());
//	}
//}
//
//void ABladeIIPlayer::ForcedChangeQTEStepMount()
//{
//	StartQTE();
//	//if (QTEActor && QTEActor->QTEType == EQTEType::EQT_Mount)
//	//{
//	//	OnQTEBegin();
//	//	// �ش� ������ Ÿ�� �ʴ� ���� BP ( AnimNotify_QTEDashEnd ) �� �����ϰ� ����
//	//	if (auto* AnimInst = GetAnimInstance())
//	//	{
//	//		AnimInst->SetQTEAttack(true);
//	//	}
//	//	SetQTEState();
//	//}
//}
//
//bool ABladeIIPlayer::IsTimingForSuccessiveQTE() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsTimingForSuccessiveQTE);
//	// �̹� QTE �� ���¿���..
//	return (QTEActor && QTEActor->IsInQTEState() && this->IsInQTEState() && this->GetQTEEnabled());
//}
//
//void ABladeIIPlayer::OnEvadeEnd()
//{
//	//// �𵹸���. �ʿ��ϸ�. ���Ӹ�忡���� �ٸ�.
//	//if (GetGameMode()->GetGameRule()->IsEvadeCoolTimeCheck() && EvadeTimer == 0.0f)
//	//	SetEvadeTimer();
//}
//
//void ABladeIIPlayer::OnQTEBegin()
//{
//	//if (QTEActor)
//	//{
//	//	AAIController* TargetController = Cast<AAIController>(QTEActor->GetController());
//	//	if (TargetController && TargetController->BrainComponent)
//	//		TargetController->BrainComponent->StopLogic(FString(TEXT("QTE")));
//	//	
//	//	// Remove AI during QTE
//	//	QTEActor->SetSignalAttack(false);
//	//	QTEActor->HideFloatingHPBar(true); // ���� ����̳� �ٸ������� HP �ٴ� ���ش�.
//
//	//	// Start mount timer count
//	//	if (QTEActor->QTEType == EQTEType::EQT_Mount)
//	//	{
//	//		// ������ �ΰ�
//	//		QTEActor->SetTeamNum(GetTeamNum());
//
//	//		ECollisionChannel PlayerCollisionChannel = GetCapsuleComponent()->GetCollisionObjectType();
//	//		QTEActor->GetCapsuleComponent()->SetCollisionObjectType(PlayerCollisionChannel);
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::StartDash(float InDashDuration, float InMaxDistance)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartDash);
//	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::StartDash, Current AttackState %d"), static_cast<int32>(GetAttackState())));
//
//	if (!IsFineToStartAnyNewInputHandling())
//		return;
//	
//	//TotalDashDuration = InDashDuration;
//	//CurrentDashDuration = InDashDuration;
//	//MaxDashDistance = InMaxDistance;
//	//GetCharacterMovement()->bApplyOptionalVelocityDuringRootmotion = true;
//	//GetCharacterMovement()->OptionalVelocity = FVector::ZeroVector;
//	//GetCharacterMovement()->OptionalVelocityBlend = 0.f;
//}
//
//void ABladeIIPlayer::StopDash()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopDash);
//	//TotalDashDuration = -1.f;
//	//CurrentDashDuration = -1.f;
//	//GetCharacterMovement()->bApplyOptionalVelocityDuringRootmotion = false;
//	//GetCharacterMovement()->OptionalVelocity = FVector::ZeroVector;
//	//GetCharacterMovement()->OptionalVelocityBlend = 0.f;
//}
//
//void ABladeIIPlayer::SetAttackState(EAttackState InAttackState)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetAttackState);
//	bool bGuardStart = InAttackState == EAttackState::ECS_GuardStart && InAttackState != GetAttackState();
//
//	Super::SetAttackState(InAttackState);
//
//	TotalApplyAnimRateScale(); // �ַ� AttackState �� ���� ���� ���ΰ� �޶����Ƿ� SetAttackState ���� �ϸ� ������ ��.
//
//	if (bGuardStart)
//		PlayerGuardStartedClass<>::GetInstance().Signal();
//}
//
//FVector ABladeIIPlayer::GetDirectionToTargetCharacter()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDirectionToTargetCharacter);
//	if (TargetActor)
//	{
//		FVector ToTarget = TargetActor->GetActorLocation() - this->GetActorLocation();
//		ToTarget.Normalize();
//		return ToTarget;
//	}
//	else if (TargetObject)
//	{
//		FVector ToTarget = TargetObject->GetActorLocation() - this->GetActorLocation();
//		ToTarget.Normalize();
//		return ToTarget;
//	}
//	return FVector(1.0f, 0.0f, 0.0f);
//}
//
//FVector ABladeIIPlayer::GetDirectionToTargetCharacter(FVector& BoneLocation)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDirectionToTargetCharacter_BoneLocation);
//	if (TargetActor)
//	{
//		FVector ToTarget = TargetActor->GetActorLocation() - BoneLocation;
//		ToTarget.Normalize();
//		return ToTarget;
//	}
//	else if (TargetObject)
//	{
//		FVector ToTarget = TargetObject->GetActorLocation() - BoneLocation;
//		ToTarget.Normalize();
//		return ToTarget;
//	}
//	return FVector(1.0f, 0.0f, 0.0f);
//}
//
//UAnimSequenceBase* ABladeIIPlayer::GetAnimStateOverrideSequence()
//{
//	//
//	// Animation Blueprint ���� AllowSequenceDynamicChange �� üũ�� �ִ� state �� ���� �� �Ҹ��� ��.
//	// ���⼭ ���� ���¿� ���� Ư�� AnimSequence �� �����ϸ� AnimBP �� ���� state �� ���� ��ġ�� �ִϸ��̼��� �ƴ� ���⼭ ������ �ִϸ��̼��� �÷����ϰ� �� ��.
//	// NULL ���� �� �⺻�� �÷���
//	//
//
//	//ESkillAnimType FinalCurrentSkillAnimType = GetCurrentSKillAnimIdx(); // CurrentSkillAnimIdx ���� GetCurrentSKillAnimIdx �� ���
//	//if (IsSkillAttacking() && FinalCurrentSkillAnimType != ESkillAnimType::ESA_End)
//	//{ // AnimBP �� �ƴ� ���� ���̺� ��ϵ� skill �ִϸ��̼��� �ҷ��鿩�� AnimBP �� �Կ���.
//	//	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
//	//	UB2SkillAnimInfo* SkillAnimInfo = B2GI ? B2GI->GetSkillAnimInfo() : nullptr;
//	//	UAnimSequenceBase* WantedSkillAnim = SkillAnimInfo ? SkillAnimInfo->GetSingleSkillAnim(GetCurrentPlayerClass(), FinalCurrentSkillAnimType) : nullptr;
//	//	return WantedSkillAnim;
//	//}
//
//	return nullptr;
//}
//
//float ABladeIIPlayer::GetTargetDistance() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetTargetDistance);
//	if (TargetActor)
//		return GetTargetDistanceInTarget(TargetActor);
//
//	return -1.f;
//}
//
//int32 ABladeIIPlayer::GetCounterMotionIncrease() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCounterMotionIncrease);
//	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCounterMotionInc() :*/ 0;
//}
//
//void ABladeIIPlayer::ForceFeedback() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ForceFeedback);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::ForceFeedback"));
//
//	//ABladeIIPlayerController* PC = Cast<ABladeIIPlayerController>(GetController());
//	//if (PC && IsPlayerControlled())
//	//{
//	//	if (IsVibration())
//	//	{
//	//		PC->ClientPlayForceFeedback(ForceFeedbackSetting, false, FName());
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::StopForceFeedback() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopForceFeedback);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::StopForceFeedback"));
//
//	//ABladeIIPlayerController* PC = Cast<ABladeIIPlayerController>(GetController());
//	//if (PC && IsPlayerControlled())
//	{
//		/*if (IsVibration())
//		{
//			PC->ClientStopForceFeedback(ForceFeedbackSetting, FName());
//		}*/
//	}
//}
//
//bool ABladeIIPlayer::GetHadInput() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetHadInput);
//
//	bool bMoveInput = false;
//
//	if (IsQTEMounting())
//		bMoveInput = QTEActor->GetLastMovementInputVector() != FVector::ZeroVector;
//	else
//		bMoveInput = GetLastMovementInputVector() != FVector::ZeroVector;
//
//	return bMoveInput || m_bGuardKeyPressed || m_bAttackKeyPressed;
//}
//
//bool ABladeIIPlayer::IsTagAttackEnabled() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsTagAttackEnabled);
//	if (IsSkillAttacking())
//	{
//		return true;
//	}
//
//	//if (DamagedNum > 0 && CachedSkillRTManager)
//	//{
//	//	if (LastDamageType == EAttackType::EAT_KnockBack && CachedSkillRTManager->GetPassiveSkillOptionValue(GetCurrentPlayerClass(), EPassiveType::EPassiveType_Tag, ESkillOption::ESO_EnableTagOnKnockBack) > 0.f)
//	//	{
//	//		return true;
//	//	}
//	//	if (LastDamageType == EAttackType::EAT_BigBounce && CachedSkillRTManager->GetPassiveSkillOptionValue(GetCurrentPlayerClass(), EPassiveType::EPassiveType_Tag, ESkillOption::ESO_EnableTagOnBigBounce) > 0.f)
//	//	{
//	//		return true;
//	//	}
//	//}
//	return false;
//}
//
//void ABladeIIPlayer::SetKeepAttacking(bool bKeep)
//{
//	bKeepAttacking = bKeep;
//}
//
//bool ABladeIIPlayer::HadTagDisabledDamage() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_HadTagDisabledDamage);
//	// Prevent tag during KnockBack and BigBounce
//	
//	const bool IsDamaged = DamagedNum > 0;
//	const bool HasHugeDamage = IsDamaged && IsInHugeDamage();
//
//	const bool IsAbnormal = IsFrozen() || IsStunned();
//
//	if (IsAbnormal || HasHugeDamage)
//		return true;
//
//	return false;
//}
//
//void ABladeIIPlayer::SetTagEnable(bool IsEnable)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetTagEnable);
//	bIsTagPossible = IsEnable;
//}
//
//bool ABladeIIPlayer::CanTag()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CanTag);	
//
//	//if (Role == ROLE_SimulatedProxy)
//	//	return true;
//
//	//if (GetGameMode()->IsAllowTag() == false || bIsTagPossible == false)
//	//	return false;
//
//	if (IsInQTEState())
//		return false;
//
//	if (HadHugeDamaged())
//		return false;
//
//	//if (IsSkillAttacking())
//	//{
//	//	if (auto* DocBattle = Cast<UB2UIDocBattle>(UB2UIDocHelper::GetDocBattle()))
//	//		if (DocBattle->GetIsTagSuggested() == false)
//	//			return false;
//	//	
//	//}
//
//	if (bIsDying)
//		return false;
//
//	return true;
//}
//
//bool ABladeIIPlayer::CanPlayerHaveMoveMotion() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CanPlayerHaveMoveMotion);
//	//ABladeIIPlayerController*	B2PC				= Cast<ABladeIIPlayerController>(GetController());
//	//AB2AutoAIController*		AutoAIController	= GetAutoAIController();
//
//	//return (GetHadInput() || 
//	//	// B2StageEventDirector ���� ��Ƽ�� ���� ��ġ���� ��ũ ��Ȳ, ActiveCameraActor ��迡���� ó�� ��Ȳ.
//	//	(B2PC && B2PC->CanPlayerHaveMoveMotion()) || 
//	//	(AutoAIController && AutoAIController->CanPlayerHaveMoveMotion()));
//	return true;
//}
//
//void ABladeIIPlayer::NotifyPlayerDeadEnd()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyPlayerDeadEnd);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::NotifyPlayerDeadEnd"));
//
//	if (GetMesh())
//	{
//		// Dead �ִϸ��̼��� ���� ���·� ������Ų��. ��Ȱ�� �ϰ� �ߴ��� �ϰ�, ������ ���� spawn �ϰ� �� ��. �ƴ� �ٽ� ���� ��
//		bReachedAtDeadEnd = true;
//		TotalApplyAnimRateScale();
//
//		// �׸���.. ���뿡�� ��Ȱ �޴��� �����ϵ��� GameMode �ʿ� ��ȣ
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//		BII_CHECK(B2GM);
//
//		//if (B2GM)
//		//{
//		//	B2GM->NotifyPlayerDeadEnd(this);
//		//}
//
//		// ���� �ð�(���� �Ϸ�) ����.
//		m_fDeadTimeSec = GetWorld()->GetTimeSeconds();
//	}
//}
//
//void ABladeIIPlayer::OnCounterStart()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnCounterStart);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OnCounterStart"));
//
//	AddWeaponSkillPoint(2.0f);
//	ForceFeedback();
//
//	// �ݰݽ� HP ȸ�� �ɼ��� �ݰ� ���� �� Ÿ�� ���� ������ ������� ������ ����.
//	if (HealHPAmountRateOnCounter > 0.0f) 
//	{
//		// GetUnitedOptionStatusValue ���� �ۼ�Ʈ�� 0.01 �������� ���� �� ���ε� HealingHPByPercent �� �ۼ�Ʈ���� �����Ƿ� �ٽ� 100 ����.
//		HealingHPByPercent(HealHPAmountRateOnCounter * 100.0f);
//	}
//
//	bForceCounterAttack = false;
//	LastGuardBreakType = EGuardBreakLogicType::EGLT_AbleToGuard;
//	PlayerCounterAttackSucceedClass<>::GetInstance().Signal();
//}
//
//bool ABladeIIPlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const FDamageInfo* DamageType)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CanTakeDamage);
//	if (IsCounterAttacking())
//		return false;
//
//	if (IsInQTEState())
//		return false;
//
//	if (bInvincibleByStageEvent)
//		return false;
//
//	if (bbInvincibleByTutorial)
//		return false;
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (Cast<ABladeIITestDummyNPC>(GetAttacker(DamageCauser)))
//	{ // TestDummyNPC ��ü���� Damage �� �����ϵ��� ó���ϴ� �͵� ������ �װ͸����δ� �Ѱ谡 ����
//		return false;
//	}
//#endif
//
//	return Super::CanTakeDamage(DamageCauser, DamageEvent, DamageType);
//}
//
//void ABladeIIPlayer::ProcessTakeDamageAnimation(const FDamageInfo& DamageInfo, AActor* DamageCauser)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ProcessTakeDamageAnimation);
//	Super::ProcessTakeDamageAnimation(DamageInfo, DamageCauser);
//}
//
//void ABladeIIPlayer::ProcessGuard(const FDamageInfo& DamageInfo, AActor* DamageCauser)
//{
//	if (IsGuarding() == false)
//		return;
//
//	if (auto* Attacker = GetAttacker(DamageCauser))
//		RotateToTarget(Attacker);
//
//	if (DamageInfo.GuardBreakLogicType != EGuardBreakLogicType::EGLT_AbleToGuard)
//	{
//		// Guard Break Handling
//		LastGuardBreakType = DamageInfo.GuardBreakLogicType;
//		StopGuard(true);
//	}
//}
//
//void ABladeIIPlayer::PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak)
//{
//	auto ArmorBreakResponse = GetSuitableDamageType(DamageInfo, IsArmorBreak);
//
//	// Dash�߿��� ������ �ִϸ��̼� ���� X
//	if (IsTinyDamage(ArmorBreakResponse) && IsDashAttackState(GetAnimAttackState()))
//		return;
//
//	ApplyDamageAnimation(ArmorBreakResponse);
//	UpdateDamageNum(DamageInfo);
//}
//
//void ABladeIIPlayer::Tick(float DeltaSeconds)
//{
//	Super::Tick(DeltaSeconds);
//
//	Tick_InputAndMovement(DeltaSeconds);
//	//Tick_QTE(DeltaSeconds);
//	//Tick_UnitySkill(DeltaSeconds);
//	//
//	//if (EtherContainer)
//	//	EtherContainer->UpdateCoolTime(GetWorld()->GetDeltaSeconds());
//
//	//if (CurrentAutoMoveAttackStatePtr)
//	//	CurrentAutoMoveAttackStatePtr->Tick(DeltaSeconds);
//
//	//if (CachedSkillRTManager)
//	//	CachedSkillRTManager->CountRemainingCoolTime(GetWorld()->GetDeltaSeconds());
//
//	//UpdateCameraBoom(DeltaSeconds);
//	//UpdateLightComponentAttachment();
//
//
//	//if (GetReservedSkill() != EAttackState::ECS_None && GetWorld())
//	//{
//	//	const float ReserveSkillRefreshTime = (Role == ROLE_Authority) ? 0.2f : 0.5f;
//	//	if (GetWorld()->GetTimeSeconds() - LastSkillInputTime >= ReserveSkillRefreshTime)
//	//		ConsumeReservedSkill();
//	//}
//
//	//UpdatePlayerBuffChanged();
//
//	//if (CheckPlayerStopTime(DeltaSeconds))
//	//{
//	//	bRestartLevel = true;
//
//	//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
//	//	if (B2PC)
//	//	{
//	//		B2PC->RestartLevel();
//	//	}
//	//}
//
//	//UpdateGuardCoolTime(DeltaSeconds);
//}
//
//void ABladeIIPlayer::Tick_InputAndMovement(float DeltaSeconds)
//{
//	// Dash Target Homing
//	if (TotalDashDuration > 0.f)
//	{
//		CurrentDashDuration -= DeltaSeconds;
//		if (CurrentDashDuration <= 0.f)
//			StopDash();
//	}
//
//	if(CanRotateToTarget())
//		RotateToTarget(DeltaSeconds);
//
//	// To simulate repeat attack button
//	if (IsKeepAttacking() && IsFineToStartAnyNewInputHandling() && GetAbleToControl() && !InGuardBreak())
//		SetSignalAttack(true);
//
//	else if (m_bGuardKeyPressed && !IsGuarding() && IsGuardStartable())
//		StartGuard();
//
//	/*if (GetGameMode()->GetGameRule()->IsEvadeCoolTimeCheck() && !CanEvade)
//	{
//		EvadeTimer -= DeltaSeconds;
//		EvadeTimer = FMath::Max(EvadeTimer, 0.0f);
//
//		CanEvade = (EvadeTimer == 0.0f);
//	}*/
//
//	if (bPendingTag) // ���� ���� �� �±� ������ ���
//		ExecutePendingTag();
//
//	if (bInWeaponSkillLOD0 && WeaponSkillLOD0SceneLight)
//	{
//		UpdateWeaponSkillLOD0LightComponentAttachment();
//	}
//	else
//	{
//		UpdateLightComponentAttachment();
//	}
//}
//
//void ABladeIIPlayer::Tick_QTE(float DeltaSeconds)
//{
//	// QTE target handling
//	if (IsInQTEState() && QTEActor && QTEActor->IsInQTEState())
//	{
//		// Sync to QTE host
//		if (QTEActor->IsControlledByQTEPlayer())
//		{
//			if (QTEType == EQTEType::EQT_Mount)
//			{
//				const bool IsCurrentQTERun = QTEActor->GetVelocity().SizeSquared() > 100.f;
//				bQTERun = QTEActor->bQTERun = IsCurrentQTERun;
//
//				const FTransform& QTETransform = QTEActor->GetMesh()->GetSocketTransform(QTESocketName);
//				SetActorLocationAndRotation(QTEActor->GetActorLocation(), QTEActor->GetActorRotation(), true);
//				GetMesh()->SetWorldLocationAndRotation(QTETransform.GetLocation(), QTETransform.GetRotation());
//
//				if (QTEStateNum >= 1) // QTE Idle/Attack
//					CountQTEMountTime(DeltaSeconds);
//
//				if (IsQTEMounting())
//				{
//					const int32 nQTEStateNum = GetQTEStateNum();
//					if (nQTEStateNum > 0 && nQTEStateNum != QTEActor->QTEStateNum)
//					{
//						QTEActor->QTEStateNum = nQTEStateNum;
//					}
//				}
//			}
//		}
//		else if (QTEActor->IsAlive())
//		{
//			FTransform QTETransform = GetMesh()->GetSocketTransform(QTESocketName);
//			//QTETransform.RemoveScaling();
//			QTETransform.AddToTranslation(FVector(0.f, 0.f, QTEActor->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
//			QTEActor->SetActorTransform(QTETransform);
//		}
//	}
//}
//
//void ABladeIIPlayer::SetActorHiddenInGame(bool bNewHidden)
//{
//#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
//	if (bHideAllPCs){
//		bNewHidden = true;
//	}
//#endif
//
//	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetActorHiddenInGame);
//	//const bool bWasHidden = bHidden;
//
//	//Super::SetActorHiddenInGame(bNewHidden);
//
//	//if (bWasHidden == bNewHidden) // ���� �������� ������ �̰� �Ҹ� ���� �־ �Ʒ� �߰� �ڵ���� Ȯ���� ������ �Ǿ��� ���� ����ǵ���
//	//	return;
//
//	//FormalSetPointLightOn(!bNewHidden); // �÷��̾� ĳ���͸� ����� ���⿡ ���� ����Ʈ�� ���� �� ������ �̷�����?
//
//	//// Ư�� Hidden ���Ŀ� �ٽ� ���̵��� ���� �� Particle ���� �͵��� �Ⱥ��̴� ��찡 �־.. �̷��� �� �ִ� �� �����µ� �ѵ�..
//	//if (!bNewHidden)
//	//{
//	//	EnsureActivateStaticallyAttachedPSCs(this);
//	//}	
//}
//
//void ABladeIIPlayer::FormalSetPointLightOn(bool bIsOn)
//{
//	// �Ϲ����� ���� ��� ���� �������� �÷��̾� �Ӹ� �� PointLight On/Off �ÿ��� �̰� ������.
//	// ABladeIIPlayer ���ο�����.
//	if (LightComponent) 
//	{
//		// �Ϻ� ��忡�� �Ӹ� �� PointLight �� ������� �ʴ� ��찡 ����.
//		const bool bAllowedByGameRule = (GetInjectedGameRule() && GetInjectedGameRule()->AllowPlayerPointLight());
//
//		// �꿡 ���� �㰡���� �ʴ� ���..
//		if (!bAllowedByGameRule || !IsHeadPointLightAllowed())
//		{
//			bHeadPointLightIsFormallyOn = false;
//			LightComponent->bAffectsWorld = false;
//			LightComponent->UnregisterComponent();
//			LightComponent->Deactivate();
//		}
//		else
//		{
//			if (bIsOn)
//			{
//				bHeadPointLightIsFormallyOn = true;
//				LightComponent->bAffectsWorld = true;
//				LightComponent->SetVisibility(true);
//				LightComponent->RegisterComponent();
//				LightComponent->Activate();
//			}
//			else
//			{
//				bHeadPointLightIsFormallyOn = false;
//				LightComponent->bAffectsWorld = false;
//				LightComponent->SetVisibility(false); // ���� ����� �Ӹ�ī�� ���϶�?
//				LightComponent->UnregisterComponent();
//			}
//
//		}
//
//#if !UE_BUILD_SHIPPING
//		extern bool gbTestTurnOffPlayerLight;
//		if (gbTestTurnOffPlayerLight) // Off �� ��츸 ���� ó��
//		{
//			bHeadPointLightIsFormallyOn = false;
//			LightComponent->bAffectsWorld = false;
//			LightComponent->UnregisterComponent();
//			LightComponent->Deactivate();
//		}
//#endif
//
//		LightComponent->MarkRenderStateDirty();
//	}
//}
//
//void ABladeIIPlayer::SetupLightComponentAttachment()
//{
//	USkeletalMeshComponent* SKMeshComp = GetMesh();
//	if (LightComponent && SKMeshComp)
//	{
//#if !UE_BUILD_SHIPPING
//		extern int32 gTestAttachLightCompMode; // �̰� ��� ��� �ٿ��� ������ ���� ���� �׽�Ʈ ����
//		if (gTestAttachLightCompMode == 2) // 0, 1 ���� �����ڿ��� ������ ��� ĸ�� attach ����
//		{
//			LightComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//			LightComponent->AttachToComponent(SKMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_ROOTBONE_NAME);
//			// ��Ʈ���� ĳ���� �ϴ� �߽��̰� ĸ���� �߾��̶� �߰� offset �� ����
//			//LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, LightComponentAttachDistZ + ZOffsetFromMatchingSKActorToBIICharacter(this)));
//		}
//		else
//#endif
//		{
//			LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, LightComponentAttachDistZ));
//		}
//	}
//}
//
//void ABladeIIPlayer::UpdateLightComponentAttachment()
//{
//#if !UE_BUILD_SHIPPING
//	extern int32 gTestAttachLightCompMode; // ������ �׽�Ʈ ��..
//	if (gTestAttachLightCompMode == 0)
//#endif
//	{
//		USkeletalMeshComponent* SKMeshComp = GetMesh();
//		if (LightComponent && SKMeshComp)
//		{
//			// ���̴� �� ĸ�� ������Ʈ�� �������� �߽�(Bip001)�� ���� ������� ��ġ�� �ֵ���
//			// �̷��� �ؼ� ĳ���� �ִϸ��̼ǿ� ���� �޽� ��ġ offset �� ���󰡵��� �Ѵ�. 
//			// �׳� ���� �ٿ� ������ ȸ������ ���󰡰� �Ǿ �ݷ��� ��ǿ��� ��ġ�� ũ�� �䵿�ϰ� �Ǵ� ���� �������� �ʰ� ��ġ�� ���󰡰� �� ��.
//			FVector UpdatedLocation = SKMeshComp->GetBoneLocation(B2PLAYER_CENTERBONE_NAME, EBoneSpaces::WorldSpace);
//
//			if (QTEActor && (QTEActor->IsInQTEState() || QTEActor->QTEType == EQTEType::EQT_Mount))
//			{
//				UpdatedLocation = QTEActor->GetMesh()->GetBoneLocation(B2PLAYER_MOUNT_CENTERBONE_NAME, EBoneSpaces::WorldSpace);
//			}
//
//			UpdatedLocation.Z += LightComponentAttachDistZ;
//			LightComponent->SetWorldLocation(UpdatedLocation);
//		}
//	}
//}
//
//int32 ABladeIIPlayer::IsInAutoPlay() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsInAutoPlay);
//	// ���� ������ Auto ���� ���ημ� GameMode ���� Maintained ���¿� �޸� �Ͻ������� ���� �� ����.
//	return InAutoPlay;
//}
//
////AB2AutoAIController* ABladeIIPlayer::GetAutoAIController() const
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetAutoAIController);
////	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
////	if (B2PC == nullptr)
////		return nullptr;
////
////	return B2PC->GetAutoAIController();
////}
//
//// ������ Start/StopAutoPlay �� ���� ����ϴ� ���� �Ͻ������� Auto �� On/Off �ϰų� AI �÷��̾��� ����̰�
//// �Ϲ����� ���Ӹ�忡�� Auto �� On/Off �� ������ GameMode �� �������̽��� ����ؾ� ���°� �����ȴ�.
//void ABladeIIPlayer::StartAutoPlay()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StartAutoPlay);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::StartAutoPlay"));
//
//	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
//	//if (B2PC != nullptr)
//	//{
//	//	if (B2PC->StartAutoPlay(this))
//	//	{
//	//		if (this->IsLocalCharacter())
//	//		{
//	//			auto* InGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	//			check(InGameMode);
//	//			SetInAutoPlay(InGameMode->GetPlayerAutoBattleType());
//	//		}
//	//		else
//	//		{
//	//			SetInAutoPlay(AutoBattleType::Skill);
//	//		}
//	//	}
//	//	else
//	//	{
//	//		SetInAutoPlay(AutoBattleType::NONE);
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::StopAutoPlay()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopAutoPlay);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::StopAutoPlay"));
//
//	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
//	//if (B2PC == nullptr)
//	//	return;
//
//	//AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this));
//	//if (StageGM && StageGM->IsRepeatBattleOn()){
//	//	return; // �ݺ����������� �ڵ� ���¸� �ѳ��� ä ����.
//	//}
//
//	//InAutoPlay = AutoBattleType::NONE;
//
//	//B2PC->StopAutoPlay();
//	//StopRepeatingInput();
//}
//
//void ABladeIIPlayer::FinalKillQTETarget()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_FinalKillQTETarget);
//	//// Kill target after final QTE action or when take off mount 
//	//if (QTEActor && (QTEActor->IsInQTEState() || QTEActor->QTEType != EQTEType::EQT_Mount))
//	//{
//	//	if (QTEActor->GetLifeSpan() < 0.f) // for safety, if there is no deadend in QTE animation
//	//	{
//	//		QTEActor->Die(QTEActor->Health, FDamageEvent(UDamageType::StaticClass()), GetController(), NULL);
//	//		QTEActor->Destroy();
//	//	}
//	//	else
//	//	{ // �Ϲ����� ���. QTE �� �ɸ� ���� �ڵ�󿡼� Die �� ������ �ʰ� �ִϸ��̼ǿ��� DeadEnd �� ���� ó���� �ȴ�. (OnAnimMobDeadEnd)
//	//		QTEActor->DieForQTE(); // ���� ���� ���� �� �ʿ��� ����Ű� ���� ó���� ���� �� �ش�.
//	//	}
//	//	QTEActor = NULL;
//	//	QTENPCClass = ENPCClass::ENC_End;
//	//	QTEType = EQTEType::EQT_End;
//	//}
//}
//
//void ABladeIIPlayer::ResetAllQTEUIDocState()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ResetAllQTEUIDocState);
//	// ���� UI visibility �� �ΰ��ϰ� �����ϴ� �κ��� �� �ִµ� �̷����� Ÿ�̹��̶� ���� ���� �ִ� �� ���� ��� �������� Ȯ�λ�� �� ȣ���� �뵵.
//	//auto* DocBattle = GetDocBattle();
//	//if (DocBattle)
//	//{
//	//	DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::None);
//	//	DocBattle->SetbIsPlayerInQTEState(false);
//	//	DocBattle->SetbIsQTEMounted(false);
//	//	DocBattle->SetbIsQTEMountedAndReadyForAttack(false);
//	//}
//}
//
//void ABladeIIPlayer::ReceiveComboEnd(EAttackState InLastAttackState, bool IsAttackCanceled)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ReceiveComboEnd);
//	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIPlayer::ReceiveComboEnd, LastAttackState %d, CurrentAttackState"), static_cast<int32>(InLastAttackState), static_cast<int32>(GetAttackState())));
//
//	//// QTE Data Handling
//	//HandleComboEndForQTE();	
//
//	//if (InLastAttackState >= EAttackState::ECS_GuardStart && InLastAttackState <= EAttackState::ECS_Evade)
//	//{
//	//	::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);
//	//}
//
//	//if (InLastAttackState == EAttackState::ECS_Evade)
//	//	OnEvadeEnd();
//
//	//if (!((InLastAttackState >= EAttackState::ECS_Skill01 && InLastAttackState < EAttackState::ECS_GuardStart) // Tag �� ���� ���Խ�Ű�µ� Tag �δ� �ȿ� �� �ִ�. ���� ó���� �ٸ� ��Ʈ�ε� �� �ֵ���.
//	//	// Intended to see if this is comboend notification for skill end, but it is not that easy to ensure, so check additional condition.
//	//	// At worst, it could just set the timer in wrong situation, better than not setting it.
//	//	// See StartGuard/StopGuard, we do some to get relevant ComboEnd for skill attack anyway.
//	//	|| bLastInvokedAnySkillAttack))
//	//	return;
//
//	//BII_CHECK(CachedSkillRTManager);
//
//	//// Do the cooltime timer count for skill end.
//	//if (CachedSkillRTManager)
//	//{
//	//	CachedSkillRTManager->SetSkillCoolTimeTimer(InLastAttackState);
//	//}
//	//bLastInvokedAnySkillAttack = false;
//}
//
//void ABladeIIPlayer::OnProcessComboEnd()
//{
//	bSkillCancel = false;
//	bComboEnabled = false;
//	SetAttackState(EAttackState::ECS_None);
//	TagAttackMode = ETagAttackMode::EAM_DownAttack;
//	SetReceiveNetMotionCancleSignal(false);
//	LastGuardBreakType = EGuardBreakLogicType::EGLT_AbleToGuard;
//
//	//if (GetStageEventShowState() == EStageEventShowState::ESES_None)
//	//{
//	//	if (auto* PlayerCameraManager = UGameplayStatics::GetLocalPlayerCameraManager(this))
//	//		PlayerCameraManager->StopAllCameraAnims(true);
//	//}
//}
//
//bool ABladeIIPlayer::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
//{
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (bCheatImmortalMode) // Damage �� ������ ������ ����.
//	{
//		return false;
//	}
//#endif
//	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
//	//if (B2PC && B2PC->IsStageEndNonCombatState()){
//	//	return false; // �������� ���� ���������� ���� ó�� �߰���.
//	//}
//
//	return Super::CanDie(KillingDamage, DamageEvent, Killer, DamageCauser);
//}
//
//void ABladeIIPlayer::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnDeath);
//
//	// �θ��Լ��� ���Ʒ� �±װ�ġ�鼭 �����÷��̾ �ƴϰԵ� �̸�üũ
//	CheckSendKakaoActionLog_Death();
//
//	ABladeIIPlayer* KillerPlayer = Cast<ABladeIIPlayer>(DamageCauser);
//	if (KillerPlayer)
//		KillerPlayer->CheckSendKakaoActionLog_Kill();
//
//	Super::OnDeath(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser);
//
//	TargetActor = nullptr;
//
//	//// �±װ� ���� ���Ӹ���̰� �±װ� �����ϴٸ� �ڵ��±�
//	//// �ݰ��Ӹ��� ����. pvp���Ӹ�� ���� NotifyCharacterDead ���ؼ� �±��ϴ� ������ ����
//	//AB2NetGameMode* pCastedNetGM = Cast<AB2NetGameMode>(GetGameMode());
//
//	//if (!pCastedNetGM && GetInjectedGameRule()->AllowToTag() && CachedSkillRTManager && CachedSkillRTManager->IsTagPossibleAtDeadMoment())
//	//{
//	//	CachedSkillRTManager->TagToOther((int32)ETagAttackMode::EAM_DownAttack, EAttackState::ECS_None);
//	//}
//
//	//SetActorEnableCollision(false);	
//}
//
//int64 ABladeIIPlayer::GetAccountId()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetAccountId);
//	//if (IsLocalPlayer())
//	//	return BladeIIGameImpl::GetClientDataStore().GetAccountId();
//
//	return 0;
//}
//
//bool ABladeIIPlayer::IsQTEStartAble()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsQTEStartAble);
//	return GetInjectedGameRule()->IsQTEEnabled() && GetDamagedNum() == 0 && !IsStunned() && !IsFrozen() && GetAttackState() < EAttackState::ECS_Skill01 && !IsInAutoPlay();
//}
//
//float ABladeIIPlayer::GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds)
//{
//	// ��ӽ�ų ����߿� ����
//	if (UnitySkillCoolTime != 0 && GetAttackState() != EAttackState::ECS_WeaponSkill)
//	{
//		return (DeltaSeconds / UnitySkillCoolTime) * UnitySkillAddPoint;
//	}
//
//	return 0.0f;
//}
//
//void ABladeIIPlayer::Tick_UnitySkill(float DeltaSeconds)
//{
//	if (IsUnityTick == true && bOverUnityPoint == true)
//	{
//		float fWorldDeltaSeconds = GetWorld()->GetDeltaSeconds();
//		float fAddPoint = GetUnitySkillPointDeltaSecondsRate(fWorldDeltaSeconds);
//		if (fAddPoint != 0.0f)
//		{
//			AddWeaponSkillPoint(fAddPoint);
//		}
//	}
//}
//
//float ABladeIIPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TakeDamage);
//
////	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
////	if (B2PC && B2PC->IsStageEndNonCombatState()){
////		return 0.0f; // �������� ���� ���������� ���� ó��. bCanBeDamaged �� �ᵵ ��ɸ鿡���� �� �Ŵ� ������ Ȥ �ٸ� �Ŷ� ������ �𸣰ڱ�.
////	}
////
////	const FDamageInfo* DamageType = static_cast<const FDamageInfo*>(DamageEvent.DamageInfo);
////	const FDamageInfo& DamageInfo = *(DamageType);
////	if (!DamageType) {
////#if !UE_BUILD_SHIPPING
////		// ��ü�� DamageType �� ���� ��Ȳ�� ������ ���� �÷��� �Ǵ� attack �ִϸ��̼ǿ��� damage notify �� ������ ��. InterpTrackAnimControl �� SkipDamageNotifiersOnly �� üũ.
////		//UE_LOG(LogBladeII, Warning, TEXT("NULL DamageType is detected of DamageCauser %s"), DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"));
////#endif
////		return 0.0f;
////	}
//
//	//ProcessGuard(DamageInfo, DamageCauser);
//	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
//
////	CounterCheckTime = DamageType->bCountable ? FMath::Abs(CounterCheckTime) : -FMath::Abs(CounterCheckTime);
////
////	// �ݰ� ������
////	bCounterCauseStun = DamageType->bStunCountable;
////
////	if (ActualDamage > 0.f || GetB2GameModeType(GetWorld()) == EB2GameMode::CounterDungeon)
////	{
////		PlayerTakeAttackClass<float>::GetInstance().Signal(0.1f);
////	}
////
////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
////	if (bCheatImmortalMode)
////	{// CanDie ���� return false �Ͽ����� Health ���� 0���� �˻��ؼ� ���� �� ó�� ó���� �� �����Ƿ� 1�� ���ܵ�.
////		Health = FMath::Max(Health, 1.0f); 
////	}
////#endif
////
////	AB2AutoAIController*	AutoAIController	= GetAutoAIController();
////	ABladeIIProjectile*		ProjectileCauser	= Cast<ABladeIIProjectile>(DamageCauser);
////	ABladeIICharacter*		ProjectileOwner		= ProjectileCauser ? Cast<ABladeIICharacter>(ProjectileCauser->GetOwner()) : nullptr;
////
////	if (AutoAIController && ProjectileCauser && ProjectileOwner && ProjectileOwner->IsAlive())
////	{
////		// To track the character who fired the projectile out of player's target range in autoplay mode..
////		AutoAIController->CachedRangeAttackedMob = ProjectileOwner;
////	}
////
////	// ��� ����
////	bIsInCriticalCondition = (Health / MaxHealth <= CriticalConditionHealthRatio) ? true : false;
//
//	return ActualDamage;
//}
//
//void ABladeIIPlayer::NotifyTargetDamaged(class ABladeIICharacter* DamagedTarget, float DamageAmount)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyTargetDamaged);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::NotifyTargetDamaged"));
//
//	Super::NotifyTargetDamaged(DamagedTarget, DamageAmount);
//
//	// Someday might need check Target for friendly fire..
//
//	++CurrentComboNum; // Unconditionally increase ComboNum.
//
//	if (!GetWorld())
//		return;
//
//	LastAttackSuccessTime = GetWorld()->GetTimeSeconds();
//	if (ResetComboNumTimerHandle.IsValid())
//	{
//		GetWorldTimerManager().ClearTimer(ResetComboNumTimerHandle);
//	}
//	GetWorldTimerManager().SetTimer(ResetComboNumTimerHandle, this, &ABladeIIPlayer::ResetComboNumTimerCallback, ComboMaintainDuration, false);
//
//	TargetDamagedClass<ABladeIICharacter*, int32, float>::GetInstance().Signal(this, CurrentComboNum, DamageAmount); // UI �� ��ȣ
//	UpdateUIDocOnTargetDamaged();
//}
//
//void ABladeIIPlayer::ResetComboNumTimerCallback()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ResetComboNumTimerCallback);
//	CurrentComboNum = 0;
//	auto* DocBattle = GetDocBattle();
//	if (DocBattle)
//	{
//		//DocBattle->SetComboNum(CurrentComboNum);
//	}
//}
//
//void ABladeIIPlayer::InitUpdateUIDoc()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_InitUpdateUIDoc);
//	// ��� ó���� ���õǾ�� �� �͵�..
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::InitUpdateUIDoc"));
//
//	Super::InitUpdateUIDoc();
//		
//	// ABladeIIPlayer::SpawnOtherAsPuppet ���� �ɷ� ������ ��� update �� ���� ���� �ȵ�.
//	//if (IsLocalPlayer())
//	//{
//	//	// UIDoc �� static �̹Ƿ� �� ƽ ������Ʈ �Ǵ� �� �ƴ� ���� �����鿡 ���� ���¸� �ʱ�ȭ�� �� �ʿ䰡 �ִ�.
//	//	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
//	//	InjectDocBattle(DocBattle);
//
//	//	if (DocBattle)
//	//	{
//	//		DocBattle->SetCurrPCMaxHealth(MaxHealth);
//	//		DocBattle->SetComboNum(CurrentComboNum);
//
//	//		// �±� �������� �ᱹ ����� �� ���̹Ƿ� �±� Ŭ���� ���� ���⼭ ����
//	//		// UB2SkillRTManager::DynSpawnNewPlayerCommon ����. �� ������ TagPlayerClass �� ���� �� ���� ��.
//	//		// �ΰ��ӿ����� ���� Ȱ��/��Ȱ�� ĳ���Ϳ� ���� main/sub �� ������ �ؾ� ��. Main �� Ȯ���� ���� Ȱ���� ���� �±׸� ���� ���� ���� ���� ���� �ʹݻ���.
//	//		DocBattle->SetIsCurPCMain(AmIMainCharacter());
//	//		DocBattle->SetCurPCClass((int32)GetCurrentPlayerClass());
//	//		DocBattle->SetTagPCClass((int32)GetTagPlayerClass());
//	//		DocBattle->SetInAutoPlay(IsInAutoPlay()); // AutoPlay ���δ� ó������ ���⼭ �����ϰ� ���Ŀ��� GameMode �ʿ���
//
//	//		DocBattle->SetbTagAttackPending(false); // �ش� �÷��״� �̹� ���� ���� ���̴�.
//	//	}
//
//	//	ResetAllQTEUIDocState();
//
//	//	SetGuardCoolTime(0.0f);
//	//	SetHasNotTagDefence(false);
//	//}
//}
//
//void ABladeIIPlayer::UpdateUIDocOnTargetDamaged()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateUIDocOnTargetDamaged);
//	//auto* DocBattle = GetDocBattle();
//	//if (DocBattle)
//	//{
//	//	DocBattle->SetComboNum(CurrentComboNum);
//	//	DocBattle->SetCriticalRateByComboNum(GetAdditionalCriticalRateByComboNum());
//	//}
//}
//
//void ABladeIIPlayer::CharacterBuffUpdateUIdoc()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CharacterBuffUpdateUIdoc);
//	//auto* DocBattle = GetDocBattle();
//	//if (DocBattle && IsLocalPlayer())
//	//{
//	//	TArray<FUISkillBuffDisplayInfo_Character> AllCharacterBuffInfo; // ���� �ߵ��� �͵鸸 �ֱ�
//
//	//	for (UB2Buff_Base* pBuff : *(GetBuffManager()->GetBuffList()))
//	//	{
//	//		if (pBuff->IsActive())
//	//		{
//	//			FUISkillBuffDisplayInfo_Character ThisInfo;
//	//			ThisInfo.MyType = pBuff->GetBuffType();
//	//			ThisInfo.LastStartedTime = pBuff->GetBuffBeginTime();
//	//			ThisInfo.RemainingTime = pBuff->GetRemainTime();
//	//			AllCharacterBuffInfo.Add(ThisInfo);
//	//		}
//	//	}
//	//	DocBattle->SetCharacterBuffState(AllCharacterBuffInfo);
//	//}
//}
//
//void ABladeIIPlayer::SpawnSurviveSkillEffect()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnSurviveSkillEffect);
//	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
//	//UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : nullptr;
//
//	//UParticleSystem* RestoreParticle = SomeInfo ? SomeInfo->GetSurviveSkillRestoreEffect() : nullptr;
//	//if (RestoreParticle)
//	//{
//	//	UGameplayStatics::SpawnEmitterAttached(RestoreParticle, GetBaseMesh(), FName(TEXT("Bip001-Pelvis")));
//	//}
//}
//
//float ABladeIIPlayer::GetShield() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetShield);
//
//	/*if (BuffManager != nullptr)
//	{
//		if (UB2Buff_Shield* ShieldBuff = Cast<UB2Buff_Shield>(BuffManager->FindBuff(EBuffType::Buff_Shield)))
//		{
//			return ShieldBuff->GetShieldAmount();
//		}
//	}*/
//
//	return 0.0f;
//}
//
//void ABladeIIPlayer::SetShield(const float NewShieldAmount)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetShield);
//	/*if (UB2Buff_Shield* ShieldBuff = Cast<UB2Buff_Shield>(BuffManager->FindBuff(EBuffType::Buff_Shield)))
//	{
//		ShieldBuff->SetShieldAmount(NewShieldAmount);		
//
//		if (IsLocalPlayer())
//		{
//			auto* DocBattle = GetDocBattle();
//			DocBattle->SetCurrPCShield(NewShieldAmount);
//		}
//	}*/
//}
//
//bool ABladeIIPlayer::IsComboAttacking() const
//{
//	const auto CurrAttackState = GetAnimAttackState();
//	return IsOneOfComboAttackState(CurrAttackState) || CurrAttackState == EAttackState::ECS_Dash;
//}
//
//bool ABladeIIPlayer::IsSkillAttacking() const
//{
//	return IsOneOfSkillAttackState(GetAnimAttackState());
//}
//
//bool ABladeIIPlayer::IsCounterAttacking() const
//{
//	return IsCounterAttackingState(GetAnimAttackState());
//}
//
//bool ABladeIIPlayer::HadHugeDamaged()
//{
//	//if (auto* AnimInst = GetAnimInstance())
//	//{
//	//	const EAttackType CurrDamageAnim = AnimInst->GetDamageAnimIndex();
//	//	if (CurrDamageAnim == EAttackType::EAT_BigBounce || CurrDamageAnim == EAttackType::EAT_KnockBack)
//	//		return true;
//	//}
//	return false;
//}
//
//bool ABladeIIPlayer::IsGuarding()
//{
//	return IsGuardingState(GetAnimAttackState());
//}
//
//void ABladeIIPlayer::SpawnResurrectBuffTextEffect()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnResurrectBuffTextEffect);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SpawnResurrectBuffTextEffect"));
//	/*
//		UB2BuffModeEffectInfo* BuffModeEffectInfo = GetBuffModeEffectInfo();
//		if (BuffModeEffectInfo)
//		{
//			TMap<EResurrectBuffType, int32> AppliedResurrectBuff;
//			CachedSkillRTManager->GetAppliedResurrectionBuffValues(AppliedResurrectBuff);
//			int32* AttackBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Attack);
//			if (AttackBuffIfAny && *AttackBuffIfAny > 0)
//			{
//				BuffModeEffectInfo->SpawnResurrectTextEffect(EResurrectBuffType::RBT_Attack, this);
//			}
//			int32* DefenseBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Defense);
//			if (DefenseBuffIfAny && *DefenseBuffIfAny > 0)
//			{
//				BuffModeEffectInfo->SpawnResurrectTextEffect(EResurrectBuffType::RBT_Defense, this);
//			}
//			int32* HealthBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Health);
//			if (HealthBuffIfAny && *HealthBuffIfAny > 0)
//			{
//				BuffModeEffectInfo->SpawnResurrectTextEffect(EResurrectBuffType::RBT_Health, this);
//			}
//		}*/
//}
//
//void ABladeIIPlayer::DestroyPlayerUIManager()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_DestroyPlayerUIManager);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::DestroyPlayerUIManager"));
//
//	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	//if (B2GM && B2GM->GetUIManager_InGameCombat())
//	//{
//	//	B2GM->GetUIManager_InGameCombat()->DestroyHUDForDyingPlayer(this); // �÷��̾� ���� ����.
//	//}
//}
//
//UB2SkillRTManager* ABladeIIPlayer::GetSkillRTManager()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetSkillRTManager);
//	if (CachedSkillRTManager == NULL)
//	{
//		// Controller �� ���� Possess �Ǿ�� ����
//		auto* B2PC = Cast<ABladeIIPlayerController>(GetController());
//		CachedSkillRTManager = B2PC ? B2PC->GetPlayerSkillRTManager() : NULL;
//
//		if (CachedSkillRTManager)
//			CachedSkillRTManager->InitSkillRTManager(this);
//	}
//	BII_CHECK(!Cast<ABladeIIPlayerController>(Controller) || CachedSkillRTManager); // Controller �� ���� ��Ȳ�� ABladeIIPlayer::SpawnOtherAsPuppet ���� ���� �� ����.
//	return CachedSkillRTManager;
//}
//
//ICharacterDataStore* ABladeIIPlayer::GetCharacterDataStore()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCharacterDataStore);
//	//����� ���� ���� Local�� Player character�� �����Ѵ�.
//	if (CharacterDataStore == nullptr)
//		CharacterDataStore = &BladeIIGameImpl::GetLocalCharacterData();
//
//	return CharacterDataStore;
//}
//
////UB2InGameHUDInfo* ABladeIIPlayer::GetHUDInfoObject()
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetHUDInfoObject);
////	if (HUDInfoClass)
////	{
////		return Cast<UB2InGameHUDInfo>(HUDInfoClass->GetDefaultObject());
////	}
////	return NULL;
////}
//
//void ABladeIIPlayer::SetupControlledMatineePuppet(ASkeletalMeshActor* InMatineePuppet, bool bSetupLightComponent, USkeletalMesh* OptionalSKMeshToSet)
//{
//	check(!OptionalSKMeshToSet); // ��� ���� �ܺο��� �̰� ���� �־��ִ� ���� ���� �ɷ�.. Ȥ ���� �� ���� �� check �� �����ϴµ� �Ʒ� �ڵ�� ������ �ʴ��� ����..
//
//	//// ���� ���� ���� ����ϴ� ���Ӹ���ε� ���⿡���� �װ� ���������� ����ϴ� �� �ƴϸ� ���� ���� �и��� �� ����
//	//USkeletalMesh* ExplicitNonMergedMesh = nullptr;
//	///*if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this) && !UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this))
//	//{
//	//	ExplicitNonMergedMesh = GetMergedMesh(false);
//	//}*/
//
//	//USkeletalMeshComponent* MyBaseMeshComp = this->GetBaseMesh();
//	//USkeletalMeshComponent* PuppetBaseMeshComp = InMatineePuppet ? InMatineePuppet->GetSkeletalMeshComponent() : nullptr;
//
//	//////////////////////////////////////////
//
//	//BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupControlledMatineePuppet);
//	//Super::SetupControlledMatineePuppet(InMatineePuppet, bSetupLightComponent, 
//	//	ExplicitNonMergedMesh ? ExplicitNonMergedMesh : OptionalSKMeshToSet // OptionalSKMeshToSet ���ڸ� ���⼭ ���� �Ѱ��ְ� ��..
//	//);
//
//	//// ��ű� ���� �� ������ �߰��� ������ �ַ��� �ߴµ� ������ �͵��� ������ �Լ����� �⺻ ó��.
//	//
//	//if (MyBaseMeshComp && PuppetBaseMeshComp)
//	//{
//	//	// Scalability ��Ʈ���� �Ϻημ� ForcedLOD ���� �͵� ������ �ش�. ��� ������� 0 �ϰŶ� �ʿ�� �������� ���Ծ ���� �� ����.
//	//	PuppetBaseMeshComp->SetForcedLOD(MyBaseMeshComp->ForcedLodModel);
//	//}
//}
//
///** ��ü�� ���� ������ ���� �±� ĳ���� spawn. ���̶�� �ؼ� �׽�Ʈ ����� �ƴϴ�.
// * PossessByAIController �� AI �� ���� �� �ƴ϶� ���� �⺻���� �߷� ���� ���� �� ó�� ����. */
//ABladeIIPlayer* ABladeIIPlayer::SpawnOtherAsPuppet(bool bPossessByAIController /*= true*/, bool bTurnOnPointLight /*= false*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnOtherAsPuppet);
//	BII_CHECK(GetTagPlayerClass() != EPCClass::EPC_End);
//	ABladeIIPlayer* Spawned = SpawnPuppetInternal(GetTagPlayerClass(), bPossessByAIController);
//	if (Spawned)
//	{ // ����� pointlight ���ѵ� �ְ�.. �±����� ������ ������.. �̰� �ʿ��� ��Ȳ�� ��������..
//		Spawned->FormalSetPointLightOn(bTurnOnPointLight);
//	}
//	return Spawned;
//}
//
//ABladeIIPlayer* ABladeIIPlayer::SpawnMyselfAsPuppet(bool bPossessByAIController /*= true*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnMyselfAsPuppet);
//	return SpawnPuppetInternal(GetCurrentPlayerClass(), bPossessByAIController);
//}
//
//ABladeIIPlayer* ABladeIIPlayer::SpawnPuppetInternal(EPCClass InSpawnClass, bool bPossessByAIController /*= true*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnPuppetInternal);
//	//BII_CHECK(this->IsValidObj());
//
//	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
//	//if (InSpawnClass != EPCClass::EPC_End && B2PC && B2PC->IsLocalController())
//	//{
//	//	const FTransform& CurrTransform = GetTransform();
//	//	const float AsideDistance = GetCapsuleComponent() ? GetCapsuleComponent()->GetScaledCapsuleRadius() : 50.0f;
//	//	// ��¦ ������ spawn ��Ų��. �̰� ���� ��Ȳ���� ������ �������� ��ġ�� ���� Ʈ�� ������ ������ �����ؾ� ��.
//	//	FTransform NewTransform(
//	//		CurrTransform.GetRotation(),
//	//		CurrTransform.GetTranslation() + FVector(AsideDistance, AsideDistance, 50.0f),
//	//		CurrTransform.GetScale3D()
//	//	);
//
//	//	// ���� ���� �� ���شٵ��� �� �ʿ� ���� �״�� ����. ���� �ʿ��� �� ó��. 
//	//	// PlayerController �� possess ������ ������ GetLocalPlayerCharacter ���� �ǵ��� �޸� ���⼭ ������ ���� �����Ѵٵ��� �ϴ� ���� ���� ��.
//	//	return ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, InSpawnClass, NewTransform, bPossessByAIController);
//	//}
//	return nullptr;
//}
//
//void ABladeIIPlayer::SetEvadeTimer()
//{
//	CanEvade = false;
//	EvadeTimer = EVADE_COOL_TIME;
//}
//
//// �Ʒ� SetupLODInfo~~ ������ �� ���� ���
//static void SetB2PCMergedMeshCompLODInfoInternal(USkeletalMeshComponent* InMeshComp, float InScreenSize_0, float InScreenSize_1)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(Global_SetB2PCMergedMeshCompLODInfoInternal);
//	//// ���� �����Ϳ��� ���� ���ҽ��� �ƴ� ��Ÿ�ӿ� merge �� �޽��� ������. ���⼭ Mesh ���ҽ��� �Ӽ����� �ٲ� ���̱� ������ �Ϲ������� �����Ϳ��� ���� �� ���ҽ������� ����ϴ�.
//	//USkeletalMesh* MeshResource = InMeshComp ? InMeshComp->SkeletalMesh : NULL;
//	////FSkeletalMeshObject* MeshResourceObj = InMeshComp ? InMeshComp->MeshObject : NULL;
//
//	//if (!MeshResource /*|| !MeshResourceObj*/) {
//	//	return;
//	//}
//
//	//bool bScreenSizeChanged = false;
//	//for (int32 LODIdx = 0; LODIdx < MeshResource->LODInfo.Num(); ++LODIdx)
//	//{
//	//	FSkeletalMeshLODInfo& MeshLODInfo = MeshResource->LODInfo[LODIdx];
//	//	const float PrevScreenSize = MeshLODInfo.ScreenSize;
//
//	//	// �ϴ��� 0 �� 1 �̻��� LOD �� ����. �� �̻��� �ʿ信 ����
//	//	MeshLODInfo.ScreenSize = (LODIdx == 0 ? InScreenSize_0 : InScreenSize_1);
//	//	
//	//	if (PrevScreenSize != MeshLODInfo.ScreenSize)
//	//	{
//	//		bScreenSizeChanged = true;
//	//	}
//	//}
//	//if (bScreenSizeChanged)
//	//{
//	//	InMeshComp->UpdateLODStatus(); // �̷��� �Ѵٰ� ���⼭ �ٷ� ������ �� �� ������ ������..
//	//	InMeshComp->MarkRenderStateDirty();
//	//}
//}
//
///** 
// * Scalability �ý����� GameplayOnlyLODQuality �� ���� �׸����� ����ϱ� ����. 
// * ������, ����� (�κ�����) �� �η��� ������
// * ForceLODLevel �̶� ForcedLODLevel �� 0 �� �� ����� ScreenSize �� �ִ�.
// * ��� ��ü�� �� ������ ForceLODLevel �� ����ŭ Ŀ�� �����ѵ� ForceLODLevel �� �����ϸ� r.SkeletalMeshLODBias �� ���� �׽�Ʈ�� ��������� �� ���� ��. 
// * �׸��� ��Ȥ ����ġ ���� �ް��� ī�޶�p�� ����ؾ� �� �� ���� ���� �ְ�.
// */
//int32 GB2PlayerCharForcedLODLevelCombat = 0;
//FAutoConsoleVariableRef CVarB2PlayerCharForcedLODLevelCombat(
//	TEXT("r.B2PlayerCharForcedLODLevelCombat"),
//	GB2PlayerCharForcedLODLevelCombat,
//	TEXT("Forced LOD level for BladeII player character meshes in combat scene, intended to be controlled by Scalability system."),
//	ECVF_Scalability
//);
//int32 GB2PlayerCharForcedLODLevelCinematic = 0;
//FAutoConsoleVariableRef CVarB2PlayerCharForcedLODLevelCinematic(
//	TEXT("r.B2PlayerCharForcedLODLevelCinematic"),
//	GB2PlayerCharForcedLODLevelCinematic,
//	TEXT("Forced LOD level for BladeII player character meshes in cinematic scene, intended to be controlled by Scalability system."),
//	ECVF_Scalability
//);
//// ������ LOD 1 ScreenSize �� ���� ū ������ �ش�. LOD1 �� ��� ScreenSize �� Ŀ������ �ٰŸ����� LOD1 �� ����.
//float GB2PlayerCharLOD1ScreenSizeCombat = 0.5f;
//FAutoConsoleVariableRef CVarB2PlayerCharLOD1ScreenSizeCombat(
//	TEXT("r.B2PlayerCharLOD1ScreenSizeCombat"),
//	GB2PlayerCharLOD1ScreenSizeCombat,
//	TEXT("LOD 1 ScreenSize for BladeII player character meshes in combat scene, intended to be controlled by Scalability system."),
//	ECVF_Scalability
//);
//// ����� LOD 1 ScreenSize �� ���� ���� ������ �ش�. LOD1 �� ��� ScreenSize �� �۾������� ���Ÿ����� LOD1 �� ����.
//float GB2PlayerCharLOD1ScreenSizeCinematic = 0.1f;
//FAutoConsoleVariableRef CVarB2PlayerCharLOD1ScreenSizeCinematic(
//	TEXT("r.B2PlayerCharLOD1ScreenSizeCinematic"),
//	GB2PlayerCharLOD1ScreenSizeCinematic,
//	TEXT("LOD 1 ScreenSize for BladeII player character meshes in cinematic scene, intended to be controlled by Scalability system."),
//	ECVF_Scalability
//);
//
//void ABladeIIPlayer::SetupLODInfoForInGame()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupLODInfoForInGame);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupLODInfoForInGame"));
//	
//	// Scalability �ý��ۿ� ���� LOD ���� ����. ForceLOD Ȥ�� LODScreenSize
//	SetB2PCMergedMeshCompLODInfoInternal(GetBaseMesh(), 0.0f, GB2PlayerCharLOD1ScreenSizeCombat);
//	GetBaseMesh()->SetForcedLOD(GB2PlayerCharForcedLODLevelCombat);
//}
//void ABladeIIPlayer::SetupLODInfoForCinematic()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupLODInfoForCinematic);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupLODInfoForCinematic"));
//
//#if !UE_BUILD_SHIPPING
//	// �� �ڵ� ��ü�� ��ǻ� BladeIIScalabilityOverride �� �ƴϴ�.
//	// ��, ������ ��� ���� ����ϰ� ���� ���� �� spike �м� ��� �ʿ��ؼ�.. ���⵵ bUseBladeIIScalabilityOverride �� ���� �����÷��� �� �޽� LOD ������ ���������� �� �� �ֵ��� ��.
//	/*if (!B2Scalability::bUseBladeIIScalabilityOverride) {
//		SetupLODInfoForInGame();
//		return;
//	}*/
//#endif
//
//	// SetUseSectionMergedMesh ���õ� ���⿡ �����ֱ� ������ ���� �� ������ �׷��� �� ��.
//	// �װ� ��Ȳ�� ���� �� �� �ٸ��� ����.
//
//	// Scalability �ý��ۿ� ���� LOD ���� ����. ForceLOD Ȥ�� LODScreenSize
//	// ��� ���⿡�� �������� �� �޽� ���ҽ��� ���� �� ������ �ְ����� ������Ʈ�� �ٸ��� ForceLOD �� �������� ������ �־�� �Ѵ�.
//	SetB2PCMergedMeshCompLODInfoInternal(GetBaseMesh(), 0.0f, GB2PlayerCharLOD1ScreenSizeCinematic);
//	GetBaseMesh()->SetForcedLOD(GB2PlayerCharForcedLODLevelCinematic);
//}
//void ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(class ASkeletalMeshActor* InLobbySKActor)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupLODInfoForLobbyRepresentative);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupLODInfoForLobbyRepresentative"));
//
//	check(InLobbySKActor);
//	//
//	//// �κ񿡼��� �ʱ⿡ Merge ���ٴ� Component ������ �����ϰ� �־��� ������ ��� �������� �ߴµ�.. �ᱹ �׷��� �ȵ� ��.
//	//// ��� Component attach ������ �ϰ� �Ǹ� �Ʒ� SetB2PCMergedMeshCompLODInfoInternal �� ����ϱⰡ ���������. ��Ÿ�� ������ �޽��� �ƴ϶� ����Ʈ �� ���ҽ��� �ǵ帮�� �� ���̹Ƿ�.
//	//TArray<UActorComponent*> AllSKComps = InLobbySKActor->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
//
//	//for (UActorComponent* CurrComp : AllSKComps)
//	//{
//	//	USkeletalMeshComponent* CurrSKComp = Cast<USkeletalMeshComponent>(CurrComp);
//	//	// �κ� Cinematic ������ ��������.		
//	//	SetB2PCMergedMeshCompLODInfoInternal(CurrSKComp, 0.0f, GB2PlayerCharLOD1ScreenSizeCinematic); // ForcedLOD 0 �̸� ����.
//	//	if (CurrSKComp)
//	//	{
//	//		CurrSKComp->SetForcedLOD(GB2PlayerCharForcedLODLevelCinematic); // �κ� CinematicLODLevel �� ����
//	//	}
//	//}
//}
//void ABladeIIPlayer::OnBeginWeaponSkillLOD0()
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OnBeginWeaponSkillLOD0"));
//
//	checkSlow(IsLocalPlayer()); // �ƴ� ��� �ִ���..?
//	//
//	//// �ΰ��� ���������� ���� ���� ���� ������ ���⿡���� �ƴ� ���.. �Ͻ������� ���� �и� �� ��� ����.
//	//if (/*UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this) &&*/ // �� ������ �� �����.. ��ġ ������ ���°� ���̴��� ��ӽ�ų���� ���յ� ���� ������ ��찡 �ִ�.
//	//	!UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this))
//	//{
//	//	SetUseSectionMergedMesh(false);
//	//}
//
//	//SetupLODInfoForCinematic(); // ����� LOD �¾��� �������� �ϴ� �⺻.
//	//
//	//DestroyWeaponSkillLOD0SceneLight();
//	//CreateWeaponSkillLOD0SceneLight();
//
//	//bInWeaponSkillLOD0 = true;
//
//	//FTimerManager& WTM = GetWorldTimerManager();
//	//WTM.ClearTimer(WeaponSkillEndHandlingForceTH);
//	//// �̷� ���� ���� �ʾƵ� OnEndWeaponSkillLOD0 �� �ҷ��� ������ ������ ���� ������ġ.
//	//WTM.SetTimer(WeaponSkillEndHandlingForceTH, this, &ABladeIIPlayer::ForceFinishWeaponSkillLOD0SceneSetup
//	//	, 30.0f/*�� �̻� ���� ��ӽ�ų�� �ִٸ� ���°���.*/
//	//	, false);
//}
//void ABladeIIPlayer::OnEndWeaponSkillLOD0()
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OnEndWeaponSkillLOD0"));
//
//	//bInWeaponSkillLOD0 = false;
//
//	//// ����� ���õ� ���� 
//	//if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
//	//{ // ���� AnimNotify �� ���� �ߵ��Ǵ� ���̴� ���� �ٷ� SetUseSectionMergedMesh �ϸ� UpdateAnimation �� ���ȣ�� �Ǿ������ ���°� �߻��Ѵ�
//	//	DeferredSetUseSectionMergedMesh(true);
//	//}
//	//SetupLODInfoForInGame(); 
//	//DestroyWeaponSkillLOD0SceneLight();
//
//	//GetWorldTimerManager().ClearTimer(WeaponSkillEndHandlingForceTH);
//
//	//DimensionBuffCheckClass<>::GetInstance().Signal();
//}
//void ABladeIIPlayer::ForceFinishWeaponSkillLOD0SceneSetup()
//{
//	OnEndWeaponSkillLOD0();
//}
//
//#if !UE_BUILD_SHIPPING
//float GDevTestWeaponSkillLOD0LightIntensity = -1.0f;
//float GDevTestWeaponSkillLOD0LightRadius = -1.0f;
//#endif
//void ABladeIIPlayer::CreateWeaponSkillLOD0SceneLight()
//{
//	/* =============================================
//	�� ���� �ϴ� ������ ��ӽ�ų ���� �� ���� ����Ʈ ������ ���� ������
//	�Ӹ� ���� �ִ� PointLight �� ���� ������ ���߱� ������ ���� �ٺ����� ������ ������ �����̴�.
//	�� ��ӽ�ų ���� ������ ������ �����ϴ� �� ���� �ʹ� Ŀ���� ��� ���� ������ ������ �������� �ּ����� ó���� ���� ����.
//	============================================= */
//
//	check(IsLocalPlayer());
//
//	static float WeaponSkillLOD0LightIntensity = -1.0f;
//	static float WeaponSkillLOD0LightRadius= -1.0f;
//	if (WeaponSkillLOD0LightIntensity <= 0.0f || WeaponSkillLOD0LightRadius <= 0.0f)
//	{
//		GConfig->GetFloat(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("WeaponSkillLOD0LightIntensity"), WeaponSkillLOD0LightIntensity, GGameIni);
//		GConfig->GetFloat(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("WeaponSkillLOD0LightRadius"), WeaponSkillLOD0LightRadius, GGameIni);
//	}
//
//	WeaponSkillLOD0SceneLight = NewObject<UPointLightComponent>(this, NAME_None, RF_Transient);
//	if (WeaponSkillLOD0SceneLight)
//	{
//		// ������ ��â�ϰ� ������ �� �ƴ����� ��ų���� ��� �Ӽ� ������ �����ϰ� �� ���� �ִ�.
//		WeaponSkillLOD0SceneLight->Intensity = (WeaponSkillLOD0LightIntensity > 0.0f) ? WeaponSkillLOD0LightIntensity : 50000.0f;
//		WeaponSkillLOD0SceneLight->LightColor = FColor(255, 255, 255);
//		WeaponSkillLOD0SceneLight->AttenuationRadius = (WeaponSkillLOD0LightRadius > 0.0f) ? WeaponSkillLOD0LightRadius : 1500.0f;
//		WeaponSkillLOD0SceneLight->bAffectsWorld = true;
//
//#if !UE_BUILD_SHIPPING
//		if (GDevTestWeaponSkillLOD0LightIntensity > 0.0f)
//		{
//			WeaponSkillLOD0SceneLight->Intensity = GDevTestWeaponSkillLOD0LightIntensity;
//		}
//		if (GDevTestWeaponSkillLOD0LightRadius > 0.0f)
//		{
//			WeaponSkillLOD0SceneLight->AttenuationRadius = GDevTestWeaponSkillLOD0LightRadius;
//		}
//#endif
//
//		// ��ġ�� UpdateWeaponSkillLOD0LightComponentAttachment ���� ���ŵ�.
//		//WeaponSkillLOD0SceneLight->RelativeLocation = FVector(500.0f, 0.0f, 0.0f); // �̰� Tick ���� �ٽ�..
//		WeaponSkillLOD0SceneLight->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
//		WeaponSkillLOD0SceneLight->RegisterComponent();
//	}
//}
//void ABladeIIPlayer::DestroyWeaponSkillLOD0SceneLight()
//{
//	if (WeaponSkillLOD0SceneLight)
//	{
//		WeaponSkillLOD0SceneLight->Deactivate();
//		WeaponSkillLOD0SceneLight->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//		WeaponSkillLOD0SceneLight->UnregisterComponent();
//		WeaponSkillLOD0SceneLight->DestroyComponent();
//		WeaponSkillLOD0SceneLight = nullptr;
//	}
//}
//void ABladeIIPlayer::UpdateWeaponSkillLOD0LightComponentAttachment()
//{
//	check(IsLocalPlayer() && bInWeaponSkillLOD0);
//
//	static FVector WeaponSkillLOD0LightRelPos(0.0f,0.0f,0.0f);
//	static bool bWeaponSkillLOD0LightRelPosLoaded = false;
//	if (!bWeaponSkillLOD0LightRelPosLoaded)
//	{
//		GConfig->GetVector(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("WeaponSkillLOD0LightRelPos"), WeaponSkillLOD0LightRelPos, GGameIni);
//		bWeaponSkillLOD0LightRelPosLoaded = true;
//	}
//
//	USkeletalMeshComponent* SKMeshComp = GetMesh();
//	if (WeaponSkillLOD0SceneLight && SKMeshComp)
//	{
//		// UpdateLightComponentAttachment ���� �ϴ� �⺻ PointLight ��ġ ���Ű� ����ϰ� �� ƽ Ư�� ���� �������� 
//		// ���� ������ �������� �� �� �����Ƿ� �ܼ��� Z ���� ������ �ʴ´�.
//
//		const FTransform BaseBoneTransform = SKMeshComp->GetBoneTransform(SKMeshComp->GetBoneIndex(B2PLAYER_CENTERBONE_NAME));
//		const FVector DesiredWorldLoc = BaseBoneTransform.TransformPosition(WeaponSkillLOD0LightRelPos);
//		WeaponSkillLOD0SceneLight->SetWorldLocation(DesiredWorldLoc);
//	}
//}
//
//EPCClass ABladeIIPlayer::GetTagPlayerClass()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetTagPlayerClass);
//	// SkillRTManager ���� �������Ƿ� destroy ��� ������ �±� ���� OldPlayer ���Լ��� �ǹ̰� ����.
//	return /*GetSkillRTManager() ? CachedSkillRTManager->GetTagPlayerClass() :*/ EPCClass::EPC_End;
//}
//
//EPCClass ABladeIIPlayer::GetCurrentPlayerClass() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCurrentPlayerClass);
//	//return UB2PCClassInfo::FromCharacterTypeToPCClass(this->CharacterType);
//	return /*GetSkillRTManager() ? CachedSkillRTManager->GetTagPlayerClass() :*/ EPCClass::EPC_End;
//}
//
//bool ABladeIIPlayer::AmIMainCharacter()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_AmIMainCharacter);
//	
//	// �ٵ� ������ possess �ȵ� ���¿��� GetSkillRTManager �� null �̸� ���� ����.. �̤�
//	//return GetSkillRTManager() ? CachedSkillRTManager->IsMainPlayerCharacter(this) : false;
//	return  false;
//}
//
//void ABladeIIPlayer::NotifyTagRetire(ABladeIIPlayer* NewTaggedPlayer)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyTagRetire);
//	check(NewTaggedPlayer);
//
//	OnEnterSkillState_Tag(NewTaggedPlayer->GetCurrentPlayerClass());
//}
//
//void ABladeIIPlayer::NotifyTagSpawn(class ABladeIIPlayer* OldRetiringPlayer)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyTagSpawn);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::NotifyTagSpawn"));
//	check(OldRetiringPlayer);
//
//	TakeOverAutoFromPredecessor(OldRetiringPlayer);
//
//	// ���� ���߿� tag �� �ߴٸ� �̾ ������ ��� �ϵ��� �г�������ָ� ����.
//	if (OldRetiringPlayer->IsKeepAttacking())
//		SetKeepAttacking(true);
//
//	// ���� ��� ��ư���� �ΰ�
//	if (OldRetiringPlayer->IsAttackKeyPressed())
//	{
//		StartAttackFromInput();
//	}
//
//	if (OldRetiringPlayer->IsGuardKeyPressed())
//	{
//		StartGuardFromInput();
//	}
//
//	SetTeamNum(OldRetiringPlayer->GetTeamNum());
//
//	//// RAID AspectRatioAdaptiveFOV, 20180716
//	//// Ư�� ��忡���� ������ ������ �������� AspectRatioAdaptiveFOV �� ����ϴ� ��찡 ����.
//	//if (OldRetiringPlayer->TopDownCameraComponent && OldRetiringPlayer->TopDownCameraComponent->GetAspectRatioAdaptiveFOV())
//	//{
//	//	if (TopDownCameraComponent)
//	//	{
//	//		TopDownCameraComponent->SetAspectRatioAdaptiveFOV(true);
//	//	}
//	//}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // �⺻ ġƮ�� �ٽ� �¾�� ����.. �Ź� ���ַ��� �����ϱ�
//	this->bCheatImmortalMode = OldRetiringPlayer->bCheatImmortalMode;
//	this->bCheatHeroMode = OldRetiringPlayer->bCheatHeroMode;
//#endif
//
//	PlayerTaggedClass<ABladeIIPlayer*, ABladeIIPlayer*>::GetInstance().Signal(OldRetiringPlayer, this);
//}
//
//void ABladeIIPlayer::OnSpawnEnd()
//{
//	/*if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
//	{
//		if (DocBattle->GetNeedResurrectBuffText())
//		{
//			SpawnResurrectBuffTextEffect();
//			DocBattle->SetNeedResurrectBuffText(false);
//		}
//	}	*/
//}
//
//void ABladeIIPlayer::NotifyBornAgain(class ABladeIIPlayer* PrevLife)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_NotifyBornAgain);
//	// �̰� Tag �� ���������� UI �ִϸ��̼��� �ǵ��� ���ε�, ���� Ȯ���� �ٴ� ����. ���� �������� �ٿ� ���� �̷��� ������, ������ ������ �ϴ���.
//	// NotifyTagSpawn �� ���� �ҷ����� �ʰ� �Ǹ� ���ʿ� ������ �͵��� �ʿ信 ���� ���ʿ���
//	NotifyTagSpawn(PrevLife);
//}
//
//void ABladeIIPlayer::TakeOverAutoFromPredecessor(class ABladeIIPlayer* InPredecessor)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TakeOverAutoFromPredecessor);
//	check(InPredecessor);
//	// ��Ȱ, �±� ���� ��Ȳ. ���� �α��÷��̾� �� �ƴ϶� �׹s�÷��̾ �����Ƿ� GameMode �� Auto ���¸� ����ϸ� �ȵ�.
//	if (InPredecessor->IsInAutoPlay()) 
//	{
//		// Let the new player continue auto play with the same AutoAIController
//		StartAutoPlay();
//	}
//}
//
//EPCSpawnMotionState ABladeIIPlayer::GetSpawnMotionState()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetSpawnMotionState);
//	//// ���� ���� ������ Spawn ���� Animation ������Ʈ Ÿ�̹��� ����Ͽ� Player Controller �ʿ� ����
//	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(Controller);
//	//if (B2PC == NULL)
//	//{
//	//	// �̰��� �Ҹ��� ������ spawn ó���� �ϴ� �����̶� ��ó possess �� �ȵǾ��� �� ����. ��¿ �� ����..
//	//	// GetNearestPlayerController �� �ϸ� �ּ��� tag �� resurrect �������� ���ŵǱ� ������ ���� �÷��̾ possess �� ���� ������ ��. (B2SkillRTManager �� ����)
//	//	// �׷��� ���� �÷��̾� ĳ���Ͱ� �����ε� �ָ� ������ ���� ��Ȱ�� �ؾ� �ϴ� ��Ȳ ���� ���� �ٽ� ������ ���� ��.
//	//	B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetNearestPlayerController(this, this->GetActorLocation()));
//	//}
//
//	return /*B2PC ? B2PC->GetPCSpawnMotionState() :*/ (EPCSpawnMotionState)EPCSpawnMotionState::EPST_Normal;
//}
//
//void ABladeIIPlayer::EndSpawnMotionState()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_EndSpawnMotionState);
////	// ���� ���� ������ Spawn ���� Animation ������Ʈ Ÿ�̹��� ����Ͽ� Player Controller �ʿ� ����
////	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(Controller);
////	if (B2PC)
////	{
////		B2PC->SetPCSpawnMotionState(EPCSpawnMotionState::EPST_Normal);
////
//////		EnableInput(B2PC);
////	}
//}
//
//void ABladeIIPlayer::OnDeadAnimBegin()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnDeadAnimBegin);
//	// Ȧ�� ������ ���� ���ĸ� ���� ���� �������. ����� ��ġ�� ������ �� �ð��� ������ ���� ���ο� ������ ���� ��Ȱ �޴��� �� ���̴�.
//	// OnDeath �� �̰� �θ��� ���� �ִϸ��̼� ���´� ���� ���� �� �����Ƿ� Dead �ִϸ��̼ǿ� notify �� ���� �θ����� ��.
//	//UGameplayStatics::SetGlobalTimeDilation(this, OnDeathTimeDilation); // �̰� �ϴ� ��� ����.
//	//GetWorldTimerManager().SetTimer(OnDeathTimeDilationTH, this, &ABladeIIPlayer::OnDeathTimeDilationTimerCB, OnDeathTimeDilationLength, false);
//	//APlayerController* PCont = Cast<APlayerController>(Controller);
//	//if (PCont){
//		//PCont->PlayDynamicForceFeedback(OnDeathVibrationIntensity, OnDeathVibrationLength, true, true, true, true, EDynamicForceFeedbackAction::Start, FLatentActionInfo());
//	//}
//	ForceFeedback();
//}
//
//void ABladeIIPlayer::SuckedDropItem(class AB2DropBase* InSuckedDrop)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SuckedDropItem);
//	// Anything necessary. �����ϸ� �� ���� Drop Ŭ������ OnPlayerSuckedMe ���� ó��.
//}
//
//int32 ABladeIIPlayer::GetPlayerLevel()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetPlayerLevel);
//
//	return /*CharacterDataStore ? CharacterDataStore->GetCharacterLevel(GetCurrentPlayerClass()) :*/ 1;
//}
//
//void ABladeIIPlayer::SyncPCCharacterLevelFromExpManager()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SyncPCCharacterLevelFromExpManager);
//	// �ּ��� BeginPlay ������ �̰� �ҷ��� ��. InitializeCombatStats ������
//	CharacterLevel = GetPlayerLevel();
//}
//
//void ABladeIIPlayer::OnSomeMobTermination(ABladeIICharacter* InMobAboutToDestroy)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnSomeMobTermination);
//	if (QTEActor && QTEActor == InMobAboutToDestroy) 
//	{
//		//UE_LOG(LogBladeII, Warning, TEXT("Calling FinalKillQTETarget on QTE target Actor's termination. This is not a desired path."));
//		// QTE Ÿ�� �� ��� ó���� ���ʼ� ���� �� �ִ� �� ������ QTE ���� DeadEnd �� ���� ���� LifeSpan ���� �� �� ��Ȳ�� ����� �ؼ�.. ���� �������� ó��
//		FinalKillQTETarget();
//	}
//}
//
//void ABladeIIPlayer::SignalStopAttackImmediate()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SignalStopAttackImmediate);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SignalStopAttackImmediate"));
//
//	bStopAttackSignal = true; // ���� AnimBP ���� ó���� �� ����.
//
//	TryRestoreFromCameraDistZoomNS(); // CameraDistZoom �� ���� �ִ� ��Ȳ�̶�� �װ͵� �ߴ�.
//
//	StopRepeatingInput();
//}
//
//bool ABladeIIPlayer::ConsumeAndCheckStopAttackMotion()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ConsumeAndCheckStopAttackMotion);
//	// �ѹ� ����ϸ� ��.
//	bool bRetVal = bStopAttackSignal;
//	bStopAttackSignal = false;
//	return bRetVal;
//}
//
//float ABladeIIPlayer::GetMoveAcceptableRadius()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetMoveAcceptableRadius);
//	return MoveToMobAcceptableRadius;
//}
//
//float ABladeIIPlayer::GetDamageIncreaseRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDamageIncreaseRate);
//	EPCClass CurrentPlayerClass = GetCurrentPlayerClass();
//	float Ret = Super::GetDamageIncreaseRate() + IncDamageValueByPassive; // ���� ���¿� �������� �ʴ� �⺻���� ������ �� ���� �ɼ� ��ġ
//	
//	//// ���� ��ų�� ���� �ݿ��Ǵ� Active ��ų �ɼ� ������ ���� ��ġ
//	//if (CachedSkillRTManager)
//	//{
//	//	Ret += IsCounterAttacking() ? CachedSkillRTManager->GetSkillDamageIncreaseRateByActive(EAttackState::ECS_Counter01) :
//	//		CachedSkillRTManager->GetSkillDamageIncreaseRateByActive(GetAttackState());
//	//}
//
//	//if (IsSkillAttacking())
//	//{
//	//	Ret += IncSkillDamageValueByPassive;
//
//	//	// �̰͵��� Ư�� ��ų ��ȣ�� ��������� Active �ƴ� Passive ����..
//	//	switch (GetAttackState())
//	//	{
//	//	case EAttackState::ECS_Skill01:
//	//		Ret += CombatStatEval::GetUnitedOptionStatusValue(CurrentPlayerClass, EUnitedCombatOptions::UCO_Skill_IncSkill1Damage, CharacterDataStore);
//	//		break;
//	//	case EAttackState::ECS_Skill02:
//	//		Ret += CombatStatEval::GetUnitedOptionStatusValue(CurrentPlayerClass, EUnitedCombatOptions::UCO_Skill_IncSkill2Damage, CharacterDataStore);
//	//		break;
//	//	case EAttackState::ECS_Skill03:
//	//		Ret += CombatStatEval::GetUnitedOptionStatusValue(CurrentPlayerClass, EUnitedCombatOptions::UCO_Skill_IncSkill3Damage, CharacterDataStore);
//	//		break;
//	//	case EAttackState::ECS_Skill05:
//	//		Ret += CombatStatEval::GetUnitedOptionStatusValue(CurrentPlayerClass, EUnitedCombatOptions::UCO_Skill_IncSkill5Damage, CharacterDataStore);
//	//		break;
//	//	default:
//	//		break;
//	//	}
//	//}
//	//else if(!IsCounterAttacking())
//	//{ // �Ϲݰ��ݿ��� ����Ǵ� ������ ����.
//	//	Ret += IncNormalDamageValueByPassive;
//	//}
//
//	//// Boss Damage Increase handled in TakeDamage
//	//// TODO: Raid Damage Increase
//	//// TODO: PVP Damage Increase
//
//	return Ret;
//}
//
//float ABladeIIPlayer::GetBossDamageIncreaseRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBossDamageIncreaseRate);
//	// Passive + AttackState �� ���� Active �ɼ� ��ġ ��.
//	//return IncDamageToBossValueByPassive + (CachedSkillRTManager ? CachedSkillRTManager->GetBossDamageIncreaseRateByActive(GetAttackState()) : 0.f);
//	return 0.0f;
//}
//
//float ABladeIIPlayer::GetBonusBuffDamageDecreaseAmount() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffDamageDecreaseAmount);
//	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_DecReceiveDamage) : */0.f;
//}
//
//float ABladeIIPlayer::GetBonusBuffDuration() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffDuration);
//	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_BuffTime) :*/ 0.f;
//}
//
//float ABladeIIPlayer::GetBuffReflectionDamageRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBuffReflectionDamageRate);
//	return/* CachedSkillRTManager ? 1.f + CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncReflectDamageOnBuff) :*/ 1.f;
//}
//
//float ABladeIIPlayer::GetBonusBuffHealHPByHit() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffHealHPByHit);
//	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_HealHPOnHitOnBuff) :*/ 0.f;
//}
//
//float ABladeIIPlayer::GetBonusBuffHealHP() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffHealHP);
//	return /*CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_HealHP) : */0.f;
//}
//
//float ABladeIIPlayer::GetBonusBuffIncMoveSpeed() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffIncMoveSpeed);
//	//return CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncMoveSpeedDuring3Sec) : 0.f;
//	return 0.f;
//}
//
//float ABladeIIPlayer::GetDamageRadiusIncreaseRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDamageRadiusIncreaseRate);
//	//return CachedSkillRTManager ? 1.f + CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncArea) : 1.f;
//	return 0.f;
//}
//
//float ABladeIIPlayer::GetBonusBuffReceiveDamageDecreaseRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffReceiveDamageDecreaseRate);
//	//return CachedSkillRTManager ? FMath::Clamp(1.f - CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_DecReceiveDamageOnBuff), 0.f, 1.f) : 1.f;
//	return 0.f;
//}
//
//float ABladeIIPlayer::GetBonusBuffAttackDamageIncreaseRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffAttackDamageIncreaseRate);
//	//return CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncAttackDamageOnBuff) : 0.f;
//	return 0.f;
//}
//
//float ABladeIIPlayer::GetAttackDamageIncreaseRateByClass(EPCClass TargetClass) const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetAttackDamageIncreaseRateByClass);
//	//return (TargetClass != EPCClass::EPC_End) ? IncAttackDamageByPassiveToClass[PCClassToInt(TargetClass)] : 0.0f;
//	return 0.f;
//}
//
//float ABladeIIPlayer::GetReceiveDamageDecreaseRateByClass(EPCClass AttackerClass) const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetReceiveDamageDecreaseRateByClass);
//	//return (AttackerClass != EPCClass::EPC_End) ? DecReceiveDamageByPassiveFromClass[PCClassToInt(AttackerClass)] : 1.0f; // Dec �� �⺻�� 1.0
//	return 0.f;
//}
//
//int32 ABladeIIPlayer::GetBonusBuffAffectNum() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusBuffAffectNum);
//	//return CachedSkillRTManager ? CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncMaxNumCharacterBuff) : 0;
//	return 0.f;
//}
//
//bool ABladeIIPlayer::CanBeTarget() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CanBeTarget);
//	return IsAlive() && !IsBuffActive(EBuffType::Buff_Hide) && GetAttackState() != EAttackState::ECS_WeaponSkill;
//}
//
//// Team Buff
//float ABladeIIPlayer::GetBonusTeamBuffRate(ETeamBuffType BuffType) const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetBonusTeamBuffRate);
//	//if (CachedSkillRTManager)
//	//{
//	//	switch (BuffType)
//	//	{
//	//	case ETeamBuffType::ETBT_Shield:
//	//		return CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncProtectHPRateOnBuff);
//	//	case ETeamBuffType::ETBT_IncreaseCriticalRate:
//	//		return CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncCriticalRateOnBuff);
//	//	case ETeamBuffType::ETBT_IncreaseCriticalDamage:
//	//		return CachedSkillRTManager->GetCachedSkillOption().GetActiveSkillOptionData(GetAttackState(), ESkillOption::ESO_IncCriticalDamageOnBuff);
//	//	default:
//	//		break;
//	//	}
//	//}
//	return 0.f;
//}
//
//float ABladeIIPlayer::GetAdditionalCriticalRateByComboNum() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetAdditionalCriticalRateByComboNum);
//	// GetCriticalRate �� �Ϻ��ε� �� ��ġ�� Ư���� UI �ʿ��� �ѷ��ֹǷ� ���� ��
//	return 0.01f * FMath::Min( FMath::Max(0, CurrentComboNum -1) / 10, 3);
//}
//
//float ABladeIIPlayer::GetCriticalRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCriticalRate);
//	//////////////////////////////////////////////////
//	// ! ���� �ɼ� ��ġ ���� �� Inc** �� ���ϰ� Dec** �� ����. GetEffectiveValueForOptionApplyType ����.
//
//	float CriticalRate = Super::GetCriticalRate();
//
//	//CriticalRate += GetAdditionalCriticalRateByComboNum()
//	//	+ IncCriticalRateValueByPassive // ItemOption �� passive SkillOption �� ���� ��ġ
//	//	+ (CachedSkillRTManager ? CachedSkillRTManager->GetCriticalRateByActive(GetAttackState()) : 0.f); // ���� AttackState �� ���� active skill �ɼ� ��ġ.
//	//
//	return CriticalRate;
//}
//
//float ABladeIIPlayer::GetCriticalDamageRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCriticalDamageRate);
//	// TODO: need to check equipment option
//
//	float CriticalDamageRate = Super::GetCriticalDamageRate();
//
//	//CriticalDamageRate += IncCriticalDamageValueByPassive // ItemOption �� passive SkillOption �� ���� ��ġ
//	//	+ (CachedSkillRTManager ? CachedSkillRTManager->GetCriticalDamageRateByActive(GetAttackState()) : 0.f); // ���� AttackState �� ���� active skill �ɼ� ��ġ.
//
//// 	if (CurrentTeamBuffType & static_cast<uint8>(ETeamBuffType::ETBT_IncreaseCriticalDamage))
//// 	{
//// 		CriticalDamageRate += TeamBuffStates[UB2BuffModeEffectInfo::GetTeamBuffIndex(ETeamBuffType::ETBT_IncreaseCriticalDamage)].Amount;
//// 	}
//
//	return CriticalDamageRate;
//}
//
//float ABladeIIPlayer::GetDamageDecreaseRate() const
//{
//	return Super::GetDamageDecreaseRate() * BaseDamageDecreaseRate;
//}
//
//float ABladeIIPlayer::GetDurationDecreaseRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDurationDecreaseRate);
//	return StateDurationDecreaseRate;
//}
//
////class UB2AnimInstance* ABladeIIPlayer::GetAnimInstance() const
////{
////	if (GetMesh() != nullptr)
////	{
////		return Cast<UB2AnimInstance>(GetMesh()->GetAnimInstance());
////	}
////
////	return nullptr;
////}
//
//const EAttackState ABladeIIPlayer::GetAnimAttackState() const
//{
//	//if (auto* AnimInst = GetAnimInstance())
//	//	return AnimInst->GetAnimAttackState();
//
//	return GetAttackState();
//}
//
//const EAttackType ABladeIIPlayer::GetDamageAnimType() const
//{
//	//if (auto* AnimInst = GetAnimInstance())
//	//	return AnimInst->GetDamageAnimIndex();
//
//	return LastDamageType;
//}
//
//float ABladeIIPlayer::GetStaticOptionAppliedWalkSpeed() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetStaticOptionAppliedWalkSpeed);
//	//if (CharacterDataStore)
//	//{
//	//	const float OptionMoveSpeedIncRate = CombatStatEval::GetUnitedOptionStatusValue(GetCurrentPlayerClass(), EUnitedCombatOptions::UCO_Misc_IncMoveSpeed, CharacterDataStore);
//
//	//	return CachedBaseMaxWalkSpeed * (1.0f + OptionMoveSpeedIncRate);
//	//}
//	return CachedBaseMaxWalkSpeed;
//}
//
//float ABladeIIPlayer::GetOptionStatusValue(EItemOption OptionType) const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetOptionStatusValue);
//	// ������ SkillOption �� ���յ� GetUnitedOptionStatusValue �� ����� ���. ���� ��ȯ..
//	//return CharacterDataStore->GetOptionStatusValue(GetCurrentPlayerClass(), OptionType);
//	return 0.0f;
//}
//
//float ABladeIIPlayer::GetUnitedOptionStatusValue(EUnitedCombatOptions OptionType) const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetUnitedOptionStatusValue);
//	// ������ �� �Լ��� ȣ���� ĳ���� spawn �ʱ�ȭ �ÿ� ������.. �ʿ��� ���� ĳ���� ���� ���� �� ����.
//	//return CombatStatEval::GetUnitedOptionStatusValue(GetCurrentPlayerClass(), OptionType, CharacterDataStore);
//	return 0.0f;
//}
//
//void ABladeIIPlayer::AddEtherSetEffect(int32 EtherSetID, float TriggerRate, float CoolTime)
//{
//	//if (GetEtherContainer() && IsAlive())
//		//GetEtherContainer()->AddEtherSetEffect(EtherSetID, TriggerRate, CoolTime);
//}
//
////UB2EtherSetEffect* ABladeIIPlayer::GetEtherSetEffect(EEtherSetType SetType)
////{
////	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetEtherSetEffect);
////
////	BII_CHECK(GetEtherContainer());
////
////	switch (SetType)
////	{
////	case EEtherSetType::Offense: return GetEtherContainer()->GetOffenseEtherSet();
////	case EEtherSetType::Defense: return GetEtherContainer()->GetDefenseEtherSet();
////	}
////
////	return nullptr;
////}
//
//
//bool ABladeIIPlayer::ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ProcessEtherSetEffect);
//
//	//UB2EtherSetEffect* EtherObject = GetEtherSetEffect(EtherSetType);
//	//if (Role == ROLE_Authority && EtherObject)
//	//{
//	//	if (EtherObject->IsActivatable(EtherCauser, CauserDamageInfo, ActualDamage))
//	//	{
//	//		EtherObject->ActivateEther(EtherCauser);
//	//		return true;
//	//	}
//	//}
//
//	return false;
//}
//
//void ABladeIIPlayer::TotalApplyAnimRateScale()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TotalApplyAnimRateScale);
//	if (GetMesh())
//	{
//		GetMesh()->GlobalAnimRateScale = (bReachedAtDeadEnd ? 0.0f : ( // DeadEnd ������ ��������
//			IsAttacking() ? (1.0f + AdditionalAttackSpeedRatebyPassive) : // ���� ���¿����� �ɼ� ����� ������. Active �ɼ��̳� Buff ���⵵ ���⼭ �����ϸ� �� ��..
//				1.0f
//			));
//	}
//}
//
//void ABladeIIPlayer::ForceCounterAttack()
//{
//	bForceCounterAttack = true;
//}
//
//void ABladeIIPlayer::OnDeathTimeDilationTimerCB()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnDeathTimeDilationTimerCB);
//	GetWorldTimerManager().ClearTimer(OnDeathTimeDilationTH);
//	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f); // ���� OnDeathTimeDilation ���� ���� ���� ĳ���� ���� �ʿ�� ���� �� ����..
//}
//
//void ABladeIIPlayer::ReserveQTEActorReportMyDeathToBirthplace()
//{
//	// ž�� �Ϸ�������� ����Ǯ�ʿ� �������� ó��
//	QTEActor->ConditionalReportMyDeathToBirthplace();
//}
//
//void ABladeIIPlayer::OnQTEMountBegin()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTEMountBegin);
//	bool bUseSafeEnableDistance = GetUseQTESafeEnebleDistance();
//	ABladeIICharacter* pQTETarget = FindQTEEnableTarget(bUseSafeEnableDistance);
//	
//	if (!pQTETarget)
//		return;
//	
//	// Ż�� Ÿ�µ��� �ݸ����ڽ��� ����� EndActiveCamera�� ȣ��ǰ� ����ī�޶�� ����� �׷��� ���ڱ� ī�޶� �ٲ�� ���� ����
//	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
//	FloorRingPSC->SetHiddenInGame(true);
//
//	// Ÿ�� ���������� QTE��� �����ݹ� ���.
//	pQTETarget->CancleQTEEnableEndCB();	
//}
//
//void ABladeIIPlayer::OnQTEMountTargetEnd(const FTransform& QTETransform)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTEMountTargetEnd);
//	// ���⼭ StopQTEMount �� �Ŀ� mount target �� AnimNotify �� ���� ���� ��.
//	// Changed to called in StopQTEMount
//	FVector NewLocation(QTETransform.GetLocation().X, QTETransform.GetLocation().Y, QTEActor->GetMovementComponent()->GetActorFeetLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
//	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator::ZeroRotator);
//	SetActorLocationAndRotation(NewLocation, FRotator(0.f, GetActorRotation().Yaw, 0.f), true);
//
//	QTEType = EQTEType::EQT_End;
//	QTEStateNum = 0;
//	//FinalKillQTETarget();
//
//	/*if (GetGameMode()->IsLocalPlayerAutoBattle())
//	{
//		StartAutoPlay();
//	}*/
//}
//
//void ABladeIIPlayer::StopQTEMount()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_StopQTEMount);
//	//
//	//if (ReserveQTEActorReportMyDeathToBirthplaceTimerHandle.IsValid())
//	//{
//	//	GetWorldTimerManager().ClearTimer(ReserveQTEActorReportMyDeathToBirthplaceTimerHandle);
//	//}
//
//	//if (QTEActor && QTEActor->IsAlive())
//	//{
//	//	QTEActor->SetActorEnableCollision(false);
//	//	QTEActor->GetMovementComponent()->StopMovementImmediately();
//	//	QTEActor->Die(QTEActor->Health, FDamageEvent(UDamageType::StaticClass()), GetController(), NULL);
//
//	//	OnQTEMountTargetEnd(QTEActor->GetMesh()->GetSocketTransform(ABladeIIPlayer::QTESocketName));
//	//}
//
//	//// SetActorEnableCollision(true);
//	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
//
//	//// UI������ �����÷��̾
//	//if (IsLocalPlayer())
//	//{
//	//	auto* DocBattle = GetDocBattle();
//	//	if (DocBattle)
//	//	{
//	//		DocBattle->SetbIsQTEMounted(false);
//	//		DocBattle->SetbIsQTEMountedAndReadyForAttack(false/*this->IsQTEMountingAndReadyForAttack()*/);
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::CountQTEMountTime(float InDeltaSecond)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_CountQTEMountTime);
//	if (QTEMountRemainingTime <= 0.0f) // StopQTEMount ���Ŀ��� ����� ������ ���̽��� ����. ���⼭ ¥����.
//		return;
//
//	checkSlow(IsInQTEState() && QTEType == EQTEType::EQT_Mount);
//
//	//QTEMountRemainingTime -= GetDeltaTimeAsOtherWorldObj(InDeltaSecond, this);
//
//	//auto* DocBattle = GetDocBattle();
//	//if (DocBattle) 
//	//{
//	//	//DocBattle->SetRemainingQTEMountTime(FMath::Max(0.0f, QTEMountRemainingTime));
//	//}
//
//	//if (QTEMountRemainingTime <= 0.0f) // Playing time is over.
//	//{
//	//	StopQTEMount(); // ������ Ÿ�̸� �ݹ��̾���. ��ư �̰ɷ� ������.
//	//}
//}
//
//void ABladeIIPlayer::SetupDecalCompForLeftShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupDecalCompForLeftShadowCommon);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupDecalCompForLeftShadowCommon"));
//
//	// ���� BladeIIPlayer �ڽ��̶�� InDecalCompToUse �� LeftFootShadow �� ����. SetupControlledMatineePuppet ����� ���� �Ű������� �޴´�. 
//	// ���⼭ ������Ʈ register ���� ������ �ʴ´�.
//	if (InDecalCompToUse && AttachParent)
//	{
//		//InDecalCompToUse->SetHiddenInGame(false);
//		InDecalCompToUse->SetRelativeScale3D(FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot));
//
//		InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//		InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_SHADOW_SOCKET_NAME_LF); // ���� socket �� ����.
//	}
//}
//
//void ABladeIIPlayer::SetupDecalCompForRightShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupDecalCompForRightShadowCommon);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupDecalCompForRightShadowCommon"));
//
//	// ���� BladeIIPlayer �ڽ��̶�� InDecalCompToUse �� RightFootShadow �� ����. SetupControlledMatineePuppet ����� ���� �Ű������� �޴´�. 
//	// ���⼭ ������Ʈ register ���� ������ �ʴ´�.
//
//	if (InDecalCompToUse && AttachParent)
//	{
//		//InDecalCompToUse->SetHiddenInGame(false);
//		InDecalCompToUse->SetRelativeScale3D(FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot));
//
//		InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//		InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_SHADOW_SOCKET_NAME_RF); // ���� socket �� ����.
//	}
//}
//
//void ABladeIIPlayer::UpdateTargetObject()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateTargetObject);
//	//auto* B2SGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//	//TArray<AB2DestructibleLevelObjBase*> Destructibles = B2SGM->GetDestructibleList();
//
//	//// Could be various conditions, but let's just pick up the closest one.
//	//float ClosestDistanceSQ = BIG_NUMBER;
//	//AActor* SelectedOne = NULL;
//
//	//for (auto* ThisOne : Destructibles)
//	//{
//	//	if (!ThisOne || !ThisOne->IsValidObj())
//	//		continue;
//
//	//	//const float CurrDistSQ = (mob->GetActorLocation() - PlayerLocation).SizeSquared();
//	//	float CurrDistSQ = FLT_MAX;
//	//	FHitResult Result;
//	//	GetWorld()->LineTraceSingleByChannel(Result, GetActorLocation(), ThisOne->GetActorLocation(), ECC_WorldStatic, FCollisionQueryParams(NAME_None, false, this));
//	//	if (Result.bBlockingHit && Result.Actor == ThisOne)
//	//	{
//	//		if (GetWorld()->GetNavigationSystem()->GetPathLength(GetActorLocation(), ThisOne->GetActorLocation(), CurrDistSQ) == ENavigationQueryResult::Success)
//	//		{
//	//			if (/*CurrDistSQ < 500 && Ÿ���� ���ѰŸ�*/CurrDistSQ < ClosestDistanceSQ)
//	//			{
//	//				ClosestDistanceSQ = CurrDistSQ;
//	//				SelectedOne = ThisOne;
//	//			}
//	//		}
//	//	}
//	//}
//	//TargetObject = SelectedOne;
//}
//float ABladeIIPlayer::UpdateTarget(bool bInputGuided, bool bOnlyQTEEnabled /* = false */)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateTarget);
//	if (IsInQTEState() && !bOnlyQTEEnabled && !IsQTEMounting())
//	{
//		return 0.f;
//	}
//
//	TargetActor = NULL;
//	float TargetDist = TargetingDistance;
//
//	//TArray<ABladeIICharacter*> Characters;
//
//	//auto* B2SGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//	//B2SGM->GetAllWorldEnemyList(Characters, GetTeamNum());
//
//	//for (ABladeIICharacter* ch : Characters)
//	//{
//	//	if (ch == this)
//	//		continue;
//
//	//	if (bOnlyQTEEnabled && !ch->IsTimingForQTEPossession())
//	//	{
//	//		continue;
//	//	}
//
//	//	if (ch->IsInQTEState())
//	//	{
//	//		continue;
//	//	}
//
//	//	if (ch->IsBuffActive(EBuffType::Buff_Hide))
//	//	{
//	//		continue;
//	//	}
//
//	//	FVector Diff = GetTargetLocationVectorFromPlayer(ch);
//	//	
//	//	float CandidateDist = FLT_MAX;
//	//	if (bInputGuided)
//	//	{
//	//		FVector Direction = GetLastMovementInputVector();
//	//		Diff.Z = 0.f;
//	//		Direction.Z = 0.f;
//
//	//		if (FVector::DotProduct(Diff, Direction) > 0.f)
//	//		{
//	//			CandidateDist = FVector::CrossProduct(Diff, Direction).Size();
//	//		}
//	//		else
//	//		{
//	//			CandidateDist = Diff.Size2D();
//	//		}
//	//	}
//	//	else
//	//	{
//	//		CandidateDist = Diff.Size2D();
//	//	}
//
//	//	if (/*CandidateDist < 500 && Ÿ���� ���ѰŸ�*/CandidateDist < TargetDist)
//	//	{
//	//		/* �繰�� �հ� ������ �� �ִٰ� �ؼ� �Ʒ� �ڵ尡 �ʿ������..
//	//		// Line Trace for visibility check
//	//		FVector ViewLoc = GetCapsuleComponent() ? GetActorLocation() + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) : GetActorLocation();
//	//		FVector TargetLoc = ch->GetCapsuleComponent() ? ch->GetActorLocation() + FVector(0.f, 0.f, ch->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) : ch->GetActorLocation();
//
//	//		FCollisionQueryParams Params(ABladeIIPlayer::TargetTraceName, false, this);
//	//		Params.AddIgnoredActor(ch);
//	//		FHitResult Result;
//	//		GetWorld()->LineTraceSingleByChannel(Result, ViewLoc, TargetLoc, ECC_Visibility, Params);
//
//	//		if (!Result.bBlockingHit)
//	//		*/ 
//	//		{
//	//			TargetActor = ch;
//	//			TargetDist = CandidateDist;
//	//		}
//	//	}
//	//}
//
//	/*
//	TArray<AB2DestructibleLevelObjBase*> Destructible = B2SGM->GetDestructibleList();
//
//	float ClosestDistanceSQ = BIG_NUMBER;
//	AB2DestructibleLevelObjBase* SelectedOne = NULL;
//
//	for (auto* de : Destructible)
//	{
//		const float CurrDistSQ = (de->GetActorLocation() - GetActorLocation()).SizeSquared();
//
//		if (CurrDistSQ < ClosestDistanceSQ)
//		{
//			ClosestDistanceSQ = CurrDistSQ;
//			TargetObject = de;
//		}
//	}
//	*/
//	return TargetDist;
//}
//
//bool ABladeIIPlayer::CanRotateToTarget()
//{
//	// 1. �ٺ������� Rotate�� �Ұ����Ҷ�
//	if (IsStunned() || IsAlive() == false || CustomTimeDilation <= 0.f)
//		return false;
//
//	// 2. �ʼ� �� ������ �ȵǾ� ������
//	if (BP_ConfigMinDashDistance <= 0.f || BP_ConfigMaxDashDistance <= 0.f)
//		return false;
//
//	// 3. �Ϲ� QTE ���϶� (����Ʈ�� Rotating �ؾ��ϹǷ� ����)
//	if (IsInQTEState() && IsQTEMounting() == false)
//		return false;
//
//	return true;
//}
//
//void ABladeIIPlayer::RotateToTarget(float DeltaTime, bool ForceRotateForLowFPS)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_RotateToTarget);
//
//	//bool bTargetActorValid = (TargetActor && TargetActor->IsAlive() && GetCapsuleComponent());
//	//bool bTargetObjectValid = TargetObject && GetCapsuleComponent();
//	//bool bInputValid = GetLastMovementInputVector() != FVector::ZeroVector && !GetCharacterMovement()->bApplyOptionalVelocityDuringRootmotion && !bDisableRotateToInput;
//
//	//if (!bTargetObjectValid && !bTargetActorValid && !bInputValid)
//	//	return;
//
//	//FVector Diff;
//
//	//FRotator CurrentRotation = IsQTEMounting() ? QTEActor->GetActorRotation() : GetActorRotation();
//	//float	TargetYaw = CurrentRotation.Yaw;
//	//// poikl2212 �Է��������� �Է¹������� ����. �⺻���� ���� �����̼�����
//	//if (bInputValid)
//	//	TargetYaw = GetLastMovementInputVector().Rotation().Yaw;
//
//	//bool bNeedUpdateRotation = ForceRotateForLowFPS ||
//	//	(GetAttackState() != EAttackState::ECS_None
//	//		&& GetAttackState() != EAttackState::ECS_GuardStart
//	//		&& GetAttackState() != EAttackState::ECS_Guard
//	//		&& GetAttackState() != EAttackState::ECS_GuardEnd
//	//		&& GetAttackState() != EAttackState::ECS_Evade
//	//		&& GetAttackState() != EAttackState::ECS_TagOtherChar
//	//		&& !IsQTEMounting());
//
//	//// Find Target Direction (only non normal targeting mode for now)
//	//bool bNeedUpdateTargetDir = GetSignalAttack() && GetAttackState() == EAttackState::ECS_None && TargetingMode != ETargetingMode::ETM_Normal;
//	//
//	////Qte ���̸� ����..
//	//if (!IsQTEMounting() && !bInputValid)
//	//{
//	//	if (CurrentMoveAttackState != EAutoMoveAttackState::AttackTarget)
//	//		return;
//	//}
//	//
//	//if (bNeedUpdateRotation || bNeedUpdateTargetDir)
//	//{
//	//	//DrawDebugPoint(GetWorld(), TargetActor->GetActorLocation(), 10.f, FColor::Red, true, 10.f);
//	//	if (bTargetActorValid)
//	//	{
//	//		Diff = GetTargetLocationVectorFromPlayer(TargetActor);
//	//		if (!bInputValid && GetTargetDistanceInTarget(TargetActor) < BP_ConfigMaxDashDistance)
//	//		{				
//	//			TargetYaw = Diff.Rotation().Yaw;
//	//		}
//	//		else
//	//		{
//	//			bNeedUpdateRotation = false;
//	//			bNeedUpdateTargetDir = false;
//	//			bTargetActorValid = false;
//	//		}
//	//	}
//
//	//	if (!bTargetActorValid && bTargetObjectValid)
//	//	{
//	//		Diff = GetTargetLocationVectorFromPlayer(TargetObject);
//	//		if (!bInputValid && GetTargetDistanceInTarget(TargetObject) < BP_ConfigMinDashDistance)
//	//		{
//	//			TargetYaw = Diff.Rotation().Yaw;
//	//		}
//	//		else
//	//		{
//	//			bNeedUpdateRotation = false;
//	//			bNeedUpdateTargetDir = false;
//	//		}
//	//	}
//
//	//	if (Diff.Size() >= BP_ConfigMaxDashDistance)
//	//	{
//	//		StopDash();
//	//	}
//
//	//	if (bNeedUpdateTargetDir)
//	//	{
//	//		float DiffYaw = FRotator::ClampAxis(TargetYaw - CurrentRotation.Yaw);
//
//	//		if (DiffYaw > 45.f && DiffYaw <= 135.f)
//	//		{
//	//			TargetDirection = ETargetDirection::ETD_Right;
//	//		}
//	//		else if (DiffYaw > 135.f && DiffYaw <= 225.f)
//	//		{
//	//			TargetDirection = ETargetDirection::ETD_Back;
//	//		}
//	//		else if (DiffYaw > 225.f && DiffYaw <= 315.f)
//	//		{
//	//			TargetDirection = ETargetDirection::ETD_Left;
//	//		}
//	//		else
//	//		{
//	//			TargetDirection = ETargetDirection::ETD_Forward;
//	//		}
//	//	}
//
//	//	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
//
//	//	if (bNeedUpdateRotation)
//	//	{
//	//		// Dash Target Homing
//	//		if (!ForceRotateForLowFPS && TotalDashDuration > 0.f)
//	//		{
//	//			if (MovementComp && MovementComp->bApplyOptionalVelocityDuringRootmotion)
//	//			{
//	//				// Calculate homing...
//	//				MovementComp->OptionalVelocity = Diff / CurrentDashDuration;
//	//				MovementComp->OptionalVelocity.Z = 0.f;
//	//				float OptionalSpeed = MovementComp->OptionalVelocity.SizeSquared2D();
//	//				if (OptionalSpeed > MaxDashSpeed)
//	//				{
//	//					MovementComp->OptionalVelocity = MovementComp->OptionalVelocity / (OptionalSpeed * MaxDashSpeed);
//	//				}
//	//				MovementComp->OptionalVelocityBlend = FMath::Clamp<float>(1.f - (CurrentDashDuration / TotalDashDuration), 0.f, 1.f);
//	//			}
//	//		}
//	//	}
//
//	//	if ((bInputValid)
//	//		|| (bNeedUpdateRotation && (!bDisableRotateToTarget || MovementComp->bApplyOptionalVelocityDuringRootmotion)))
//	//	{
//	//		//Diff.Z = 0.f;
//	//		CurrentRotation.Yaw = Utilities::FBlendHelper::BlendRotationWithSpeed(DeltaTime, CurrentRotation.Yaw, TargetYaw, RotateToTargetSpeed);
//	//		if (IsQTEMounting())
//	//		{
//	//			QTEActor->SetActorRotation(CurrentRotation);
//	//		}
//	//		else
//	//		{
//	//			SetActorRotation(CurrentRotation);
//	//		}
//	//	}
//	//}
//}
//
//// �������� ���� Ÿ�������� ȸ��
//void ABladeIIPlayer::RotateToTarget(class AActor* InTarget)
//{
//	FVector TargetDir = InTarget->GetActorLocation() - GetActorLocation();
//	TargetDir.Z = 0;;
//
//	SetActorRotation(TargetDir.Rotation());
//}
//
//void ABladeIIPlayer::UpdateCameraBoom(float DeltaTime)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraBoom);
//
//	// TargetArmLength
//	float fLengthClampAmount = CameraLengthValueForBlending.LimitChangeAmountPerSec * DeltaTime;
//
//	CameraBoom->TargetArmLength = FMath::Clamp(CameraLengthValueForBlending.TargetArmLength , CameraBoom->TargetArmLength - fLengthClampAmount, CameraBoom->TargetArmLength + fLengthClampAmount);
//
//	// Fov
//	float fFovClampAmount = CameraFovValueForBlending.LimitChangeAmountPerSec * DeltaTime;
//
//	TopDownCameraComponent->SetFieldOfView(FMath::Clamp(CameraFovValueForBlending.CameraFov, TopDownCameraComponent->FieldOfView - fFovClampAmount, TopDownCameraComponent->FieldOfView + fFovClampAmount));
//	
//}
//
//float ABladeIIPlayer::GetDeadTimeSec()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetDeadTimeSec);
//	return m_fDeadTimeSec;
//}
//
//void ABladeIIPlayer::SetDeadTimeSec(float fDeadTime)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetDeadTimeSec);
//	m_fDeadTimeSec = fDeadTime;
//}
//
//bool ABladeIIPlayer::GetUseQTESafeEnebleDistance()
//{
//	//AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
//	//if (pGM)
//	//{
//	//	return true;
//	//}
//	return false;
//}
//
//class ABladeIICharacter* ABladeIIPlayer::FindQTEEnableTarget(bool bUseSafeEnableDistance /*= false*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_FindQTEEnableTarget);
//	ABladeIICharacter* QTEEnableTarget = NULL;
//
//	TArray<ABladeIICharacter*> Characters;
//
//	auto* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//	//B2GM->GetAllCharacterList(Characters);
//
//	//AB2TutorialGameMode* B2TGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this));
//	//float MaxDist = B2TGM ? FLT_MAX : (bUseSafeEnableDistance ? QTE_ENABLE_SAFE_DISTANCE : QTE_ENABLE_DISTANCE);
//
//	//for (auto* ch : Characters)
//	//{
//	//	if (ch == this || ch->GetTeamNum() == GetTeamNum())
//	//		continue;
//
//	//	if (!ch->IsTimingForQTEPossession())
//	//	{
//	//		continue;
//	//	}
//
//	//	if (!ch->IsAlive())
//	//	{
//	//		continue;
//	//	}
//
//	//	FVector Diff = GetTargetLocationVectorFromPlayer(ch);
//	//	float fDistance = Diff.Size2D() - ch->GetCapsuleComponent()->GetScaledCapsuleRadius();
//
//	//	if (fDistance <= 0)
//	//		fDistance = 0;
//	//			
//	//	if (Diff.Size2D() < MaxDist)
//	//	{
//	//		{
//	//			QTEEnableTarget = ch;
//	//			MaxDist = Diff.Size2D();
//	//		}
//	//	}
//	//}
//
//	return QTEEnableTarget;
//}
//
//bool ABladeIIPlayer::IsVisibleFloorRing()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_IsVisibleFloorRing);
//	return IsPlayerControlled();
//}
//
//void ABladeIIPlayer::HandleComboEndForQTE()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_HandleComboEndForQTE);
//	if (IsInQTEState())
//	{ // QTE ���� ����.. 
//		SetQTEStateFlag(false);
//		bQTERun = false;
//		bPendingQTENext = false;
//		QTEStateNum = 0;
//		FinalKillQTETarget();
//
//		//SetActorEnableCollision(true);
//		//SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
//		//GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator::ZeroRotator);
//
//		if (!FloorRingPSC->IsVisible())
//		{
//			FloorRingPSC->SetHiddenInGame(false);
//		}
//		if (!LightComponent->bAffectsWorld && !LightComponent->IsRegistered())
//		{ // QTE Mount �� Light �� ���� ���� �־��µ� ���� �׷� �� ������ �ʿ������..
//			FormalSetPointLightOn(true);
//		}
//
//		// QTE���� �ݹ��
//		for (FQTEOnEnd& QTEOnEndCallBack : arQTEOnEndCallBack)
//		{
//			if (QTEOnEndCallBack.ExecuteIfBound())
//				QTEOnEndCallBack.Unbind();
//		}
//
//		arQTEOnEndCallBack.Empty();
//		// QTE���� �ݹ�� ��
//
//		ResetAllQTEUIDocState(); // �Ϲ����� ��Ȳ�̸� �� �������� UIDoc ���� �� �ݿ��� �Ǿ�� ������ ��Ȥ �� �׷� ���� �ִ� �� ���Ƽ� Ȯ���� ����.
//	}
//}
//
//void ABladeIIPlayer::CheckSendKakaoActionLog_Death()
//{
//	//EB2GameMode CurrentGameModeType = GetGameMode()->GetB2GameModeType();
//	
//	// �������� �����Ǻ���, 1:1, �������� �����÷��̾ ����
//	//if ((CurrentGameModeType == EB2GameMode::PVP_Team && GetIsLocalPlayerTeam())
//	//	|| (CurrentGameModeType == EB2GameMode::PVP_Tag && IsLocalPlayer())
//	//	|| (CurrentGameModeType == EB2GameMode::Control && IsLocalPlayer())
//	//	)
//	//{
//	//	//////////////////////////////////////////////////////////////////
//	//	//�׼� ��ǥ �α� (�÷��̾� ����)
//	//	B2ActionLogSender Sender;
//	//	Sender.setCategory(FString("PLAY"))
//	//		.setAction(FString("DEATH"))
//	//		.setLabel(B2ActionLogType<EB2GameMode>::GetName(CurrentGameModeType))
//	//		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
//	//	//////////////////////////////////////////////////////////////////
//	//}
//}
//
//void ABladeIIPlayer::CheckSendKakaoActionLog_Kill()
//{
//	//EB2GameMode CurrentGameModeType = GetGameMode()->GetB2GameModeType();
//
//	//// �������� �����Ǻ���, 1:1, �������� �����÷��̾ ����
//	//if ((CurrentGameModeType == EB2GameMode::PVP_Team && GetIsLocalPlayerTeam())
//	//	|| (CurrentGameModeType == EB2GameMode::PVP_Tag && IsLocalPlayer())
//	//	|| (CurrentGameModeType == EB2GameMode::Control && IsLocalPlayer())
//	//	)
//	//{
//	//	//////////////////////////////////////////////////////////////////
//	//	//�׼� ��ǥ �α� (�÷��̾� ų)
//	//	B2ActionLogSender Sender;
//	//	Sender.setCategory(FString("PLAY"))
//	//		.setAction(FString("KILL"))
//	//		.setLabel(B2ActionLogType<EB2GameMode>::GetName(CurrentGameModeType))
//	//		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
//	//	//////////////////////////////////////////////////////////////////
//	//}
//}
//
//UMaterialInstanceDynamic* ABladeIIPlayer::GetChainMaterial()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetChainMaterial);
//	//if (LeftWeapon)
//	//{
//	//	return Cast<UMaterialInstanceDynamic>(LeftWeapon->GetMaterial(0));
//	//}
//	return NULL;
//}
//
//ABladeIICharacter* ABladeIIPlayer::GetNearestEnemyCharacter()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetNearestEnemyCharacter);
//	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	if (GameMode)
//	{
//		TArray<ABladeIICharacter*> EnemyCharacters;
//		ABladeIICharacter* NearestEnemy = nullptr;
//		FVector Location = GetActorLocation();
//
//		//GameMode->GetAllWorldEnemyList(EnemyCharacters, GetTeamNum(), true);
//
//		float MinDistanceSQ = BIG_NUMBER;
//		for (auto* Enemy : EnemyCharacters)
//		{
//			if (Enemy == this)
//				continue;
//
//			const float ThisDistanceSQ = (Enemy->GetActorLocation() - Location).SizeSquared();
//
//			if (ThisDistanceSQ < MinDistanceSQ)
//			{
//				NearestEnemy = Enemy;
//				MinDistanceSQ = ThisDistanceSQ;
//			}
//		}
//
//		return NearestEnemy;
//	}
//
//	return Super::GetNearestEnemyCharacter();
//}
//
//void ABladeIIPlayer::PostActorCreated()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_PostActorCreated);
//	//Super::PostActorCreated();
//	//if (GetMesh())
//	//{
//	//	static FName GameModeName(TEXT("GameModeType"));
//	//	if (UAnimInstance *AnimInst = GetMesh()->GetAnimInstance())
//	//	{
//	//		UByteProperty* ByteProp = FindField<UByteProperty>(AnimInst->GetClass(), GameModeName);
//	//		if (ByteProp != NULL)
//	//		{
//	//			// Currently only check CounterGame
//	//			uint8 GameModeType = Cast<AB2CounterAttackGameMode>(UGameplayStatics::GetGameMode(GetWorld())) != NULL ? 1 : 0;
//	//			ByteProp->SetPropertyValue_InContainer(AnimInst, GameModeType);
//	//		}
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::GetNearestFriendCharacters(TArray<class ABladeIICharacter*>& Friends, float Range, int32 MaxNum)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetNearestFriendCharacters);
//	if (MaxNum <= 0 || Range < GetCapsuleComponent()->GetScaledCapsuleRadius())
//	{
//		return;
//	}
//
//	Friends.Empty();
//	// All Mobs are friends
//	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	if (GameMode)
//	{
//		TArray<ABladeIICharacter*> FriendsCharacters;
//
//		//GameMode->GetAllMyTeamCharList(FriendsCharacters, GetTeamNum(), true);
//		for (ABladeIICharacter* Friend : FriendsCharacters)
//		{
//			if (Friend != NULL && Friend != this && FVector::DistSquared(Friend->GetActorLocation(), GetActorLocation()) < FMath::Square(Range))
//			{
//				MaxNum--;
//				Friends.Add(Friend);
//			}
//
//			if (MaxNum <= 0)
//			{
//				return;
//			}
//		}
//	}
//}
//
//void ABladeIIPlayer::ChangeFloorRingPS(class UParticleSystem* NewFloorRingPS)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ChangeFloorRingPS);
//
//	if (!IsVisibleFloorRing())
//		return;
//		
//	if (NewFloorRingPS)
//	{
//		FloorRingPSC->SetTemplate(NewFloorRingPS);
//		FloorRingPSC->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//		FloorRingPSC->ReregisterComponent();
//		FloorRingPSC->Activate();		
//	}
//
//	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(FloorRingPSC->GetMaterial(0));
//	if (DynamicMaterial == NULL)
//	{
//		DynamicMaterial = FloorRingPSC->CreateAndSetMaterialInstanceDynamic(0);
//	}
//
//	if (DynamicMaterial)
//	{
//		DynamicMaterial->SetVectorParameterValue(FloorColorParamName, FloorRingColor);
//		DynamicMaterial->SetScalarParameterValue(FloorColorMultiplyParamName, FloorRingColorMultiplier);
//
//		int32 MatNum = FloorRingPSC->GetNumMaterials();
//		for (int32 i = 1; i < MatNum; ++i)
//		{
//			FloorRingPSC->SetMaterial(i, DynamicMaterial);
//		}
//	}
//}
//
//void ABladeIIPlayer::AddQTEOnEndCallBack(const FQTEOnEnd& Handler)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_AddQTEOnEndCallBack);
//	arQTEOnEndCallBack.Add(Handler);
//}
//
//bool ABladeIIPlayer::GetAbleToMove()
//{
//	return Super::GetAbleToMove() && !IsDashAttackState(GetAnimAttackState());// && GetAnimAttackState() != EAttackState::ECS_GuardEnd;
//}
//
//bool ABladeIIPlayer::IsDashAttackState(EAttackState InAttackState)
//{
//	return InAttackState == EAttackState::ECS_Dash
//		|| InAttackState == EAttackState::ECS_Counter01Dash
//		|| InAttackState == EAttackState::ECS_Counter02Dash
//		|| InAttackState == EAttackState::ECS_Counter03Dash
//		|| InAttackState == EAttackState::ECS_Counter04Dash
//		|| InAttackState == EAttackState::ECS_Counter05Dash;
//}
//
//void ABladeIIPlayer::SetTeamBuffMode(ETeamBuffType BuffMode, ABladeIIPlayer* Buffer, uint8 TargetingType, float Amount, float Duration, float Range, int32 MaxAffectNum, bool bUseEffect, bool bUseTextEffect)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetTeamBuffMode);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetTeamBuffMode"));
//	//
//	//EnumMaskValue<ETargetSelectType> TargettingTypeMask(TargetingType);
//
//	//if (!B2TargetSelector::GetInstance().IsExceptSelf(TargettingTypeMask))
//	//{
//	//	float ApplyDuration = Duration + Buffer->GetBonusBuffDuration();
//
//	//	if (ApplyDuration <= 0.f)
//	//	{
//	//		return;
//	//	}
//
//	//	float ApplyAmount = (Amount * 0.01f) + Buffer->GetBonusTeamBuffRate(BuffMode);
//
//	//	switch (BuffMode)
//	//	{
//	//	case ETeamBuffType::ETBT_Shield:
//	//		ApplyAmount *= GetMaxHealth();
//	//		AddBuff(EBuffType::Buff_Shield, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
//	//		break;
//	//	case ETeamBuffType::ETBT_IncreaseCriticalRate:
//	//		AddBuff(EBuffType::Buff_IncreaseCriticalRate, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
//	//		break;
//	//	case ETeamBuffType::ETBT_AbnormalStateImmune:
//	//		AddBuff(EBuffType::Buff_AbnormalStateImmune, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
//	//		break;
//	//	case ETeamBuffType::ETBT_IncreaseCriticalDamage:
//	//		AddBuff(EBuffType::Buff_IncreaseCriticalDamage, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
//	//		break;
//	//	case ETeamBuffType::ETBT_Enhance:
//	//		AddBuff(EBuffType::Buff_Enhance, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
//	//		break;
//	//	case ETeamBuffType::ETBT_Taunt:
//	//		AddBuff(EBuffType::Debuff_Taunt, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
//	//		break;
//	//	case ETeamBuffType::ETBT_Dispell:
//	//		AddBuff(EBuffType::Buff_Dispell, ApplyDuration, ApplyAmount, Buffer, bUseEffect, bUseTextEffect);
//	//		break;
//	//	}
//	//}
//	//
//	//if (Buffer != this || MaxAffectNum <= 0 || Range <= 0.1f || TargetingType == 0)
//	//{
//	//	return;
//	//}
//
// //	MaxAffectNum += GetBonusBuffAffectNum();
//	//
//	//// Buff to other team members..
//	//TArray<class ABladeIICharacter*> FoundCharacters;
//	//B2TargetSelector::GetInstance().FindTargetSelectFlag(FoundCharacters, TargettingTypeMask, this);
//
//	////remove self
//	//FoundCharacters.Remove(this);
//
//	//B2TargetSelector::GetInstance().FilterDistance(FoundCharacters, this, Range);
//	//B2TargetSelector::GetInstance().FilterMaxNum(FoundCharacters, MaxAffectNum);
//
//	//
//	////GetNearestFriendCharacters(TeamMembers, Range, MaxAffectNum);
//	////FilterTeamBuffAffectMember(BuffMode, TeamMembers);
//
//	//for (ABladeIICharacter* Member : FoundCharacters)
//	//{
//	//	ABladeIIPlayer* CastedMember = Cast<ABladeIIPlayer>(Member);
//	//	if (CastedMember)
//	//	{
//	//		// need to broadcasting
//	//		CastedMember->SetTeamBuffMode(BuffMode, this, 0, Amount, Duration, 0.f, 0, bUseEffect, bUseTextEffect);
//	//	}
//	//}
//}
//
//void ABladeIIPlayer::TryRestoreFromCameraDistZoomNS(bool bForceRestore)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TryRestoreFromCameraDistZoomNS);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::TryRestoreFromCameraDistZoomNS"));
//
//	//if (CameraBoom && bForceRestore)
//	//{
//	//	UWorld* TheWorld = GetWorld();
//	//	ABladeIIWorldSettings* B2WS = TheWorld ? Cast<ABladeIIWorldSettings>(TheWorld->GetWorldSettings()) : NULL;
//	//	ABladeIIPlayerController* MyB2PC = Cast<ABladeIIPlayerController>(this->GetController());
//
//	//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	//	AB2ActiveCameraActor* CurrActiveCam = B2GM ? B2GM->GetCurrentActiveActiveCameraActor() : NULL;
//
//	//	if ((!bCachedForCameraDistZoomState && B2WS && MyB2PC) || (B2GM && B2GM->GetB2GameModeType() == EB2GameMode::PVP_Team))
//	//	{
//	//		B2WS->ApplyDefaultPlayerCameraSetting(MyB2PC, 0.1f, VTBlend_Linear, true);
//	//	}
//	//	else
//	//	{// �̰͵� ���͵� �ȵǴ� ��츸 ĳ���� ���� �� ���.
// //			CameraBoom->SetRelativeRotation(this->CachedOriginalCamBoomRelativeRotation);
// //			CameraBoom->SetRelativeLocation(this->CachedOriginalCamBoomRelativeLocation);
//	//		CameraLengthValueForBlending.TargetArmLength = this->CachedOriginalTargetArmLength;
//	//	}
//
//	//	const bool bIsInActiveCam = (CurrActiveCam && CurrActiveCam->GetInteractPlayerCharacter() == this);
//	//	if (bIsInActiveCam)
//	//	{ 
//	//		CurrActiveCam->ForceBeginActiveCamera(0.1f, NULL, false);
//	//	}
//	//	
//	//	bCachedForCameraDistZoomState = false;
//	//}
//}
//
//FVector ABladeIIPlayer::GetVelocity() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetVelocity);
//	if (IsQTEMounting())
//	{
//		return QTEActor->GetVelocity();
//	}
//
//	return Super::GetVelocity();
//}
//
//void ABladeIIPlayer::SetDefaultCameraRotator()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetDefaultCameraRotator);
//	if (TopDownCameraComponent)
//	{
//		TopDownCameraComponent->SetRelativeRotation(TopDownCameraDefaultRotator);
//	}
//}
//
//void ABladeIIPlayer::UpdatePlayerBuffChanged()
//{
//	//if (GetSkillRTManager())
//	//	GetSkillRTManager()->UpdatePlayerBuffChanged(*(BuffManager->GetBuffList()));
//}
//
//bool ABladeIIPlayer::StartCustomAnim(UAnimSequenceBase * InAnimAsset)
//{
////	if (auto* AnimInst = GetAnimInstance())
////		return AnimInst->StartCustomAnim(InAnimAsset);
//
//	return false;
//}
//
//void ABladeIIPlayer::AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
//{
//	Super::AffectToAttacker(Victim, ActualDamage, DamageInfo);
//
//	if (GetAbsorbHPAmountRateOnAttack() > 0.0f)
//	{
//		if (FMath::FRand() < GetProbableHPAbsorptionProbability())// ���� Ȯ���� ������ ������ �ɼ� ��ġ�� ȸ�������� Ȯ���� �ƴ�.
//		{
//			  // GetUnitedOptionStatusValue ���� �ۼ�Ʈ�� 0.01 �������� ���� �� ���ε� HealingHPByPercent �� �ۼ�Ʈ���� �����Ƿ� �ٽ� 100 ����.
//			  HealingHPByPercent(GetAbsorbHPAmountRateOnAttack() * 100.0f, true, true);
//		}
//	}
//}
//
//void ABladeIIPlayer::AffectToAttackerAfterVictimDie(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
//{
//	Super::AffectToAttackerAfterVictimDie(Victim, ActualDamage, DamageInfo);
//
//	if (GetHealHPAmountRateOnKill() > 0.0f)
//	{
//		if (FMath::FRand() < GetProbableRecoverHPProbability())// ���� Ȯ���� ������ ������ �ɼ� ��ġ�� ȸ�������� Ȯ���� �ƴ�.
//		{
//			// GetUnitedOptionStatusValue ���� �ۼ�Ʈ�� 0.01 �������� ���� �� ���ε� HealingHPByPercent �� �ۼ�Ʈ���� �����Ƿ� �ٽ� 100 ����.
//			HealingHPByPercent(GetHealHPAmountRateOnKill() * 100.0f, true, true);
//		}
//	}
//}
//
//void ABladeIIPlayer::SetInAutoPlay(int32 Value)
//{
//	//InAutoPlay = Value;
//	//BII_CHECK(CachedSkillRTManager);
//	//if (CachedSkillRTManager)
//	//{
//	//	CachedSkillRTManager->SetAutoBattle(Value);
//	//}
//
//	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();
//	//if (DocBattle)
//	//{
//	//	DocBattle->SetInAutoPlay(InAutoPlay);
//	//}
//}
//
//bool ABladeIIPlayer::IsImmortalMode() const
//{
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	return bCheatImmortalMode; 
//#else
//	return false;
//#endif
//}
//bool ABladeIIPlayer::IsHeroMode() const 
//{
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	return bCheatHeroMode;
//#else
//	return false;
//#endif
//}
//
//class ABladeIICharacter* ABladeIIPlayer::GetRandomTarget(float TargetingRadius, int32 MaxTargetNum, FVector& RandomTargetDirection)
//{
//	ABladeIICharacter*	Target = NULL;
//
//	if (RandomTargets.Num() > 0)
//	{
//		do
//		{
//			int32 LastIdx = RandomTargets.Num() - 1;
//			Target = RandomTargets[LastIdx];
//			if (Target == NULL || Target->bIsDying)
//			{
//				RandomTargets.RemoveAt(LastIdx);
//			}
//			else
//			{
//				break;
//			}
//		} while (RandomTargets.Num() > 0);
//	}
//
//	if (Target == NULL || Target->bIsDying)
//	{
//		// Find new candidates
//		TArray<ABladeIICharacter*> Characters;
//
//		auto* B2SGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//		//B2SGM->GetAllWorldEnemyList(Characters, GetTeamNum());
//
//		for (auto* ch : Characters)
//		{
//			if (this == ch)
//				continue;
//
//			// ��ֹ� �˻�
//			FHitResult Result;
//			GetWorld()->LineTraceSingleByChannel(Result, GetActorLocation(), ch->GetActorLocation(), GetCapsuleComponent()->GetCollisionObjectType(), FCollisionQueryParams(NAME_None, false, this));
//	/*		if (!Result.bBlockingHit || Result.Actor != ch)
//				continue;*/
//
//			FVector Diff = GetTargetLocationVectorFromPlayer(ch);
//			
//			float CandidateDist = Diff.Size2D();
//			if (CandidateDist < TargetingRadius)
//			{
//				Target = ch;
//				RandomTargets.Add(ch);
//			}
//
//			if (RandomTargets.Num() >= MaxTargetNum)
//				break;
//		}
//	}
//
//	if (Target == NULL || Target->bIsDying)
//	{
//		// Move forward when no target
//		LastRandomTargetDirection = RandomTargetDirection = GetActorForwardVector();
//		LastRandomTargetLocation = GetMovementComponent() ? GetMovementComponent()->GetActorFeetLocation() : GetActorLocation();
//		return NULL;
//	}
//	else
//	{
//		LastRandomTargetDirection = RandomTargetDirection = -Target->GetActorForwardVector();
//		LastRandomTargetLocation = Target->GetCharacterMovement() ? Target->GetCharacterMovement()->GetActorFeetLocation() : Target->GetActorLocation();
//		return Target;
//	}
//}
//
//class ABladeIICharacter* ABladeIIPlayer::RemoveRandomTarget()
//{
//	ABladeIICharacter* Target = NULL;
//	if (RandomTargets.Num() > 0)
//	{
//		int32 LastIdx = RandomTargets.Num() - 1;
//		Target = RandomTargets[LastIdx];
//		RandomTargets.RemoveAt(LastIdx);
//	}
//	return Target;
//}
//
//void ABladeIIPlayer::ResetRandomTarget()
//{
//	RandomTargets.Empty();
//}
//
//#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
//bool ABladeIIPlayer::bHideAllPCs = false;
//// ���� �� ���� ��Ȳ���� �ܵ� �׽�Ʈ �� ������ �� �� ���� ������ �������� �ÿ���å. ��ŷ ������ �׷�.
//void ABladeIIPlayer::DevCalmDown()
//{
//	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
//	if (MovementComp)
//	{
//		MovementComp->MaxWalkSpeed = FMath::Clamp(CachedBaseMaxWalkSpeed, 500.0f, 1500.0f);
//	}
//	if (GetMesh())
//	{
//		GetMesh()->GlobalAnimRateScale = 1.0f;
//		AdditionalAttackSpeedRatebyPassive = 0.0f;
//	}
//}
//#endif
//
//void ABladeIIPlayer::OnEnterAnimState(const FName& NewState)
//{
//	const FString& StateStr = NewState.ToString();
//	const bool IsSkillState = StateStr.Contains(TEXT("Skill"));
//	const bool HasReservedSkill = GetReservedSkill() != EAttackState::ECS_None;
//
//	const bool IsQTEState = StateStr.Equals(TEXT("QTE"), ESearchCase::IgnoreCase);
//	if (IsQTEState)
//	{
//		OnQTEBegin();
//	}
//	else if (IsSkillState)
//	{
//		OnEnterSkillState(CachedSkill);
//	}
//}
//
//void ABladeIIPlayer::OnExitAnimState(const FName& PrevState)
//{
//	//const FString& StateStr = PrevState.ToString();
//	//if (StateStr.Contains(TEXT("Skill")))
//	//{
//	//	EAttackState EndAttackState = EAttackState::ECS_None;
//
//	//	auto* AnimInst = GetAnimInstance();
//	//	const ESkillAnimType SkillType = AnimInst ? AnimInst->GetSkillAnimType() : ESkillAnimType::ESA_End;
//	//	if (SkillType >= ESkillAnimType::ESA_Weapon_01 && SkillType <= ESkillAnimType::ESA_Weapon_05_LOD)
//	//		EndAttackState = EAttackState::ECS_WeaponSkill;
//
//	//	OnExitSkillState(EndAttackState);
//	//}
//}
//
//void ABladeIIPlayer::OnEnterSkillState(EAttackState SkillState)
//{
//	//if (auto* SkillRTManager = GetSkillRTManager())
//	//	SkillRTManager->OnEnterPlayerSkillState(SkillState);
//
//	//if (auto* AnimInst = GetAnimInstance())
//	//	AnimInst->UpdateDamageEnd();
//
//	//ConsumeReservedSkill();
//	//CachedSkill = EAttackState::ECS_None;
//}
//
//void ABladeIIPlayer::OnExitSkillState(EAttackState SkillState)
//{
//	if (SkillState == EAttackState::ECS_WeaponSkill && SkillLOD == 0)
//	{
//		OnEndWeaponSkillLOD0();
//	}
//}
//
//bool ABladeIIPlayer::IsForceApplyHitAnimation()
//{
//	//if(auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this)))
//	//{
//	//	if (auto* GameRule = GameMode->GetGameRule())
//	//		return GameRule->ForceApplyHitAnimation();
//	//}
//
//	return false;
//}
//
//void ABladeIIPlayer::ChangeAutoMoveAttackState(EAutoMoveAttackState InAutoMoveAttack)
//{
//	PreChangeMoveAttackState(InAutoMoveAttack);
//
//	if (CheckChangeState(InAutoMoveAttack) == false)
//		return;
//	
//	std::shared_ptr<AutoMoveAttackState> ChangeStatePtr = GetState(InAutoMoveAttack);
//
//	if (CurrentAutoMoveAttackStatePtr)
//		CurrentAutoMoveAttackStatePtr->Leave();
//
//	CurrentMoveAttackState = InAutoMoveAttack;
//	CurrentAutoMoveAttackStatePtr = ChangeStatePtr;
//	CurrentAutoMoveAttackStatePtr->Enter();
//}
//
//void ABladeIIPlayer::FilterTeamBuffAffectMember(ETeamBuffType TeamBuffType, TArray<class ABladeIICharacter*>& InChracters)
//{
//	//nothing
//}
//
//std::shared_ptr<ABladeIIPlayer::AutoMoveAttackState> ABladeIIPlayer::GetState(EAutoMoveAttackState InAutoMoveAttack)
//{
//	std::shared_ptr<AutoMoveAttackState> ChangeStatePtr;
//
//	switch (InAutoMoveAttack)
//	{
//	case NoSearchTarget:
//		ChangeStatePtr = NoSearchTargetPtr;
//		break;
//	case AutoMoveToTarget:
//		ChangeStatePtr = AutoMoveToTargetPtr;
//		break;
//	case AttackTarget:
//		ChangeStatePtr = AttackTargetPtr;
//		break;
//	}
//	return ChangeStatePtr;
//}
//
//bool ABladeIIPlayer::CheckChangeState(EAutoMoveAttackState InAutoMoveAttack)
//{
//	return CurrentMoveAttackState != InAutoMoveAttack;
//}
//
//void ABladeIIPlayer::PreChangeMoveAttackState(EAutoMoveAttackState & InAutoMoveAttack)
//{
//	
//}
//
//bool ABladeIIPlayer::AutoAttack_AbleSkill()
//{
//	const bool IsCombo = IsOneOfComboAttackState(GetAnimAttackState());
//
//	return IsCombo || (GetAnimAttackState() == EAttackState::ECS_None);
//}
