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
//	// LightComponent 는 attachment 를 다시 세팅하게 될 수 있다.
//	LightComponent = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("PointLightComponent"));
//	LightComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	LightComponentAttachDistZ = 425.0f;
//	//LightComponent->RelativeLocation = FVector(0.f, 0.f, 425.f);
//	LightComponent->Intensity = 45000.f;
//	LightComponent->LightColor = FColor(255, 222, 164);
//	LightComponent->AttenuationRadius = 800.f;
//
//	bHeadPointLightIsFormallyOn = false; // 일반적인 경로로 요 LightComponent 를 켜고 끌 때에는 bHeadPointLightIsFormallyOn 를 같이 세팅해야 한다.
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
//	// 슬로우, 빙결때도 제속도로 돌게
//	//FloorRingPSC->bUseOwnerCustomTimeDilation = false;
//
//	// 이 FloorRing 은 연출장면 포함시키지 않도록 블랙리스트에 추가함.
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
//	//{ // DLC Front 모드 리소스로딩 최대한 제거
//	//	FString DefaultHiddenMobPointerPSTemplatePath;
//	//	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultHiddenMobPointerPSTemplate"), DefaultHiddenMobPointerPSTemplatePath, GGameIni);
//	//	HiddenMobPointerPSTemplate = LoadObject<UParticleSystem>(NULL, *DefaultHiddenMobPointerPSTemplatePath);
//	//	BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("HiddenMobPointerPSTemplate"))); // MinimalDLCFrontMode 로 인해 비어있을 BaseBP 쪽 값을 가져오지 않기 위해 이름 추가
//
//	//	FString DefaultForceFeedbackPath;
//	//	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("DefaultForceFeedbackPath"), DefaultForceFeedbackPath, GGameIni);
//	//	ForceFeedbackSetting = LoadObject<UForceFeedbackEffect>(NULL, *DefaultForceFeedbackPath);
//	//	BaseBPCopyExcludeProperties.AddUnique(FName(TEXT("ForceFeedbackSetting"))); // MinimalDLCFrontMode 로 인해 비어있을 BaseBP 쪽 값을 가져오지 않기 위해 이름 추가
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
//	//CachedBaseMaxWalkSpeed = 0.f; // PCClassInfo 에 세팅된 값이 들어가 있어야 함. Elevated to BladeIICharacter
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
//	// 플레이어 기본설정 레드팀
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
//	FPlatformMemory::Memzero(DecReceiveDamageByPassiveFromClass, sizeof(DecReceiveDamageByPassiveFromClass)); // 기본값 1 이 맞긴 한데 초기값 넣어주기 차원이니..
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
//	//[@AKI, 170815] 전투 분석 중 SkillLOD로 가져 오는 부분 인 듯 함.
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
//	// 이 시점에 PlayerController 의 possess 가 되어 있어야 함. UB2SkillRTManager::DynSpawnNewPlayerCommon 이나 ABladeIIGameMode::StaticSpawnPCClassInfoPlayer 참고
//	//GetSkillRTManager();
//	SetupMeshesMain(); // 장비 조합에 따른 메쉬 셋업
//
//	SyncPCCharacterLevelFromExpManager(); // InitializeCombatStats 이전에..
//
//	// 캡슐 컴포넌트 스케일이 [플레이어 스타트] 스케일값에 영향을 받음
//	// 안전하게 초기화
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
//			SetupDecalCompForLeftShadowCommon(LeftFootShadow, SKMeshComp); // SetupControlledMatineePuppet 에서도 사용하는 공통 셋업 부분.. 이었는데 결국 여기서만 사용
//
//			LeftFootShadow->ReregisterComponent();
//			LeftFootShadow->SetDecalMaterial(ShadowMaterial);
//			LeftFootShadow->Activate();
//		}
//		else
//		{
//			LeftFootShadow->Deactivate(); // 소켓이 누락되었다면 차라리 숨겨야 이상한 게 안 나옴.
//			LeftFootShadow->SetVisibility(false);
//		}
//
//		if (SKMeshComp->GetSocketByName(B2PLAYER_SHADOW_SOCKET_NAME_RF))
//		{
//			SetupDecalCompForRightShadowCommon(RightFootShadow, SKMeshComp);  // SetupControlledMatineePuppet 에서도 사용하는 공통 셋업 부분.. 이었는데 결국 여기서만 사용
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
//	// GameRule 고려해서 켬..
//	FormalSetPointLightOn(true);
//	BII_CHECK(GetInjectedGameRule());
//
//	SubscribeEvents();
//
//	// UpdateCamera** 을 통해 세팅할 때에는 Camera** 값들이 같이 캐싱됨.
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
//	InitUpdateUIDoc(); // 상위 클래스에서 부르긴 할건데 여기서 또 해서 나쁠 거 없지. 그새 업데이트 되는 게 나올수도
//
//
//#if !UE_BUILD_SHIPPING
//	extern bool gUseCharacterDefaultMaterial; // 캐릭터 default material 테스트
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
//	// Subscribe 를 한 Player 만.. Unsubscribe 시 죄다 해제해버림.
//	//ABladeIIPlayer* CurrentLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	//if ((!CurrentLocalB2Player || CurrentLocalB2Player == this)) // 여기서도 LocalPlayerCharacter 검사를 할 필요가 있다. 태그 같은 경우 생성/파괴 시점이 엇갈려서 꼬일 수 있음.
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
//		// 모든 캐릭터 spawn 에 쓰이는 베이스 블루프린트에는 기본값 리소스들이 박혀 있는데 이게 무시하지 못할 정도로 클 수 있다. 쿠킹 세이브 시 제거.
//		//if (IsSavingForCookingOnPreSave(TargetPlatform) && GetMesh())
//		//{
//		//	// 여기서 null 시킨 건 PCClassInfo 를 통해 세팅이 되어야.
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
//	// 만일 여러 Player 가 각자 이벤트를 받아야 한다면 Subscribe 리턴값을 저장했다가 Unsubscribe 시 사용.
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
//		//	B2GameMode->FinishSpawnByPCClassInfo(); // 한번 사용되고 나면 바로 리셋
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
///** 로컬 캐릭터 전투력 보정 치트 */
//float gLocalPCCombatStatScale = -1.0f;
//#endif
//
//void ABladeIIPlayer::InitializeCombatStats()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_InitializeCombatStats);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::InitializeCombatStats"));
//
//	// Super::InitializeCombatStats 를 사용하지 않는다.
//	
//	// 여기선 아이템이 적용된 것으로
//	// GetCurrentPlayerClass 가 제대로 된 값을 리턴하려면 OverrideByPCClassInfo 가 처리된 이후여야 함.
////	GetSkillRTManager();
////	GetCharacterDataStore();
////
////	if (CachedSkillRTManager == nullptr || CharacterDataStore == nullptr) //일반적인 플레이어가 아닌 보여주기용(승리씬에서의 태그캐릭터..)
////		return; 
////
////	LocalizedUserNickName = FText::FromString(CharacterDataStore->GetUserNickName());
////	
////	auto* BladeGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
////
////	TotalAttack = BladeGameMode->GetTotalAttack(GetCharacterDataStore());
////	TotalDefense = BladeGameMode->GetTotalDefense(GetCharacterDataStore());
////
////	//길드 버프 적용 Doc에서 불러온다
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
////	// 부활 버프 적용
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
////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 치트에 따른 스탯 보정
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
////	// 모드별 체력까지 합산 후 기본 체력보다 낮은 체력이 된다면 기본 체력까지는 보정해준다.
////	MaxHealth = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseHealth(CharacterDataStore->GetCharacterLevel(GetCurrentPlayerClass()))), MaxHealth);
////
////	GuildHealth = MaxHealth * (GuildHealth * 0.01f);			//길드 버프 탑승
////
////	int32* HealthBuffIfAny = AppliedResurrectBuff.Find(EResurrectBuffType::RBT_Health);
////	if (HealthBuffIfAny && *HealthBuffIfAny > 0)
////	{
////		MaxHealth = MaxHealth + MaxHealth * ((float)(*HealthBuffIfAny) * 0.01f); // 체력 버프는 MaxHealth 에? 아님 MaxHealth 보다 Health 가 커지게?
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
////	// 부활 버프가 존재한다면 부활 상황이라는 뜻이기 때문에 부활시에는 엑스트라 데이터를 가져오지 않는다.
////	if (AB2StageManager::GetCacheStageKeepEssentialData().IsPlayInExtraLevel()
////		&& AppliedResurrectBuff.Num() == 0)
////	{ // ExtraMap 전환 시 처리 일부 여기도..
////		// Health 는 아마 웬만해선 SkillRTManager 쪽에서도
////		AB2StageManager::GetCacheStageKeepEssentialData().SetPostExtraMapLoadB2Player(this);
////	}
////		
////	// 기본 공/방/체 이외의 전투 옵션 수치들 캐싱.
////	InitializeCombatOptionCacheValues();
////
////	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
////	if (MovementComp)
////	{ // PCClassInfo 에서는 기본값만을 넣고 아이템이랑 스킬 옵션 등이 반영된 walk speed 적용
////		MovementComp->MaxWalkSpeed = GetStaticOptionAppliedWalkSpeed();
////	}
////
////	TotalApplyAnimRateScale(); // 이후에도 AttackState 에 따라 적용됨.
////
////	//GameMode에 따른 스탯보정
////	if (BladeGameMode)
////	{
////		CharacterDamageRate *= BladeGameMode->GetDamageRateByGameModeType();
////		Health *= BladeGameMode->GetHealthRateByGameModeType();
////		MaxHealth *= BladeGameMode->GetHealthRateByGameModeType();
////	}
////
////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 치트에 따른 스탯 보정
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
//	checkSlow(CharacterDataStore); // 현재 호출 타이밍 상 이걸 이미 초기화 한 상태에 쓰는 걸로.
//
//	//// 전투 옵션 수치들 캐싱. 실제 적용 시에 Inc** 는 더하고 Dec** 는 곱함. GetEffectiveValueForOptionApplyType 참고.
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
//	//// 아래는 동일한 ID 로 특정 ActiveSkill 에만 적용되는 옵션들도 있어서 그것들은 특정 AttackState 에 따라 값을 따로 가져와서 적용.
//	//IncCriticalRateValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncCriticalRate, CharacterDataStore);
//	//IncCriticalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncCriticalDamage, CharacterDataStore);
//	//IncDamageToBossValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncBossDamage, CharacterDataStore);
//	//IncAdditionalDamageValueByPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncAdditionalDamage, CharacterDataStore);
//
//	//// 아래 Heal/Absorb HP 수치들은 최대 health 값 대비 회복 비율.
//	//HealHPAmountRateOnCounter = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Defense_HealHPByCountAttack, CharacterDataStore);;
//	//HealHPAmountRateOnKill = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Health_ProbableRecoverHP, CharacterDataStore);
//	//AbsorbHPAmountRateOnAttack = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Health_ProbableHPAbsorption, CharacterDataStore);
//
//	//AdditionalAttackSpeedRatebyPassive = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, EUnitedCombatOptions::UCO_Offense_IncAttackSpeed, CharacterDataStore);
//
//	//// PC 클래스 별로 식별자가 구분되는 옵션들.
//	//int32 AttackDamageByClassBaseIdx = static_cast<int32>(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToGladiator);
//	//int32 ReceiveDamageByClassBaseIdx = static_cast<int32>(EUnitedCombatOptions::UCO_Defense_DecDamageByGladiator);
//	//for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
//	//{
//	//	IncAttackDamageByPassiveToClass[i] = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, static_cast<EUnitedCombatOptions>(AttackDamageByClassBaseIdx + i), CharacterDataStore);
//	//	DecReceiveDamageByPassiveFromClass[i] = CombatStatEval::GetUnitedOptionStatusValue(CurrPCClassEnum, static_cast<EUnitedCombatOptions>(ReceiveDamageByClassBaseIdx + i), CharacterDataStore);
//	//}
//
//	////SetRegenerateHPRate(
//	////	// 이건 퍼센트 아닌 절대값 취급임.
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
//	// MoveInput 만 막고 싶은 경우가 있어서 추가 검사
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
//	// 원래 콤보엔드에서 해주는건데 스킬시작할때도 함 해준다.
//	// 스킬중 - 빙결 - 생존스킬
//	// 이게 안나가서..
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
//	//	// 버프 클리어단계(빙결, 스턴)에서 강제 콤보엔드로 설정하는게 있다. 여기선 취소.
//	//	// 강제 콤보엔드 설정해노면 애님BP에서 어택스테이트 논으로 바꿔버린다. ECS_Skill04 스킬이 안나감.
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
//	//	// 버프 클리어단계(빙결, 스턴)에서 강제 콤보엔드로 설정하는게 있다. 여기선 취소.
//	//	// 강제 콤보엔드 설정해노면 애님BP에서 어택스테이트 논으로 바꿔버린다. ECS_Skill04 스킬이 안나감.
//	//	ConsumeComboEnd();
//	//	ResetDamageNum();
//	//
//	//	// Apply Cooltime decrease.. 특수한 쿨타임 감소이며 기타 쿨타임 감소 옵션들은 SkillRTManager 쪽에 구현되어 있다.
//	//	// 현재 ECombatOptionApplyType 이 Subtract_Value 인 구현으로 OneMinusMultiply 로 바뀔 수 있다.
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
//	// 마운트에서 실제 공격까지 바로 가능한 상태..
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
//		/* Cinematic 이전에 눌린 Input은 무시 */
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
//	// CameraZoom 등의 임시 효과 후 상태 복원을 위한 default 값도 캐싱
//	CameraPitch = InPitch;
//	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CameraRoll));
//}
//
//void ABladeIIPlayer::UpdateCameraYaw(float InYaw)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraYaw);
//	// CameraZoom 등의 임시 효과 후 상태 복원을 위한 default 값도 캐싱
//	CameraYaw = InYaw;
//	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CameraRoll));
//}
//
//void ABladeIIPlayer::UpdateCameraRoll(float InRoll)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_UpdateCameraRoll);
//	// CameraZoom 등의 임시 효과 후 상태 복원을 위한 default 값도 캐싱
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
//	// CameraZoom 등의 임시 효과 후 상태 복원을 위한 default 값도 캐싱
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
//	// GuardBreak 모션중에는 안됨
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
////	CurrentSkillAnimIdx = CachedSkillRTManager->GetSkillAnimIndex(SkillInputIndex); // AnimBP 에서 사용하든지, GetAnimStateOverrideSequence 에서 사용하든지.
////	bLastInvokedAnySkillAttack = true;
////
////	OnStartSkill(FinalAttackState);
////	SetReservedSkill(FinalAttackState);
////
////	if (SkillInputIndex == WEAPON_6STAR_SKILL_INDEX)
////	{
////		if (SkillLOD == 0)
////		{ // 플레이어를 가까이서 클로즈업 하므로 LOD 등 연출용 셋업들
////			OnBeginWeaponSkillLOD0();
////		}
////
////		SetWeaponSkillPoint(0);
////	}
////
////	ForceFeedback();
////
////	// 게임모드에 알림(리모트 플레이어는 여기 안탐)
////	GetGameMode()->NotifyPlayerStartSkill(this, FinalAttackState);
//
//	//return FinalAttackState;
//	return EAttackState::ECS_None;
//}
//#if !UE_BUILD_SHIPPING
//// StartSkillInner 에서 필요한 것들 발췌. 치트 기능이니 머리를 쥐어뜯어가면서 똑같이 맞출 필요까진 없고.. 하면 좋고..
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
//		// 게임모드에 알림(리모트 플레이어는 여기 안탐)
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
//	// 빙결 예외처리
//	if (IsBuffActive(EBuffType::Debuff_Freezing) && InAttackState == EAttackState::ECS_Skill04)
//	{
//		return true;
//	}
//
//	// 기본적으로 공격중이나 반격중, 상태이상중에는 스킬 사용이 불가능
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
//	// 넉다운 or 밀림중에도 불가능
//	if (HadHugeDamaged())
//	{
//#if FORCE_LOGGING_IN_ANDROID
//		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLOG Skill Can't Start- HadHugeDamaged"));
//#endif
//		return false;
//	}
//
//
//	// 스턴, 빙결은 기본적으로 불가능하나
//	bool bMovableDebuff = IsStunned() || IsFrozen();
//	if (bMovableDebuff)
//	{
//		if (InAttackState == EAttackState::ECS_Skill04 || InAttackState == EAttackState::ECS_WeaponSkill) // 생존스킬은 사용 가능
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
//	//// 어차피 IsTagPossible 이 아니라면 버튼이 활성화되지 말아야 함.
//	//if (CachedSkillRTManager == NULL || CachedSkillRTManager->IsTagPossible() == false || !IsFineToStartAnyNewInputHandling())
//	//	return;
//
//	//if (CachedSkillRTManager->IsTagPossibleButDeferred())
//	//{
//	//	bPendingTag = true; // 당장 가능하지 않다면 (전투 동작 중) 예약만 한다.
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
//		{// 로컬 플레이어 캐릭터의 연계 태그 여부를 UIDoc 에 세팅. 태그 당시에 UI 쪽에서 참고를 하고 태그 후 리셋될 것.
//			//DocBattle->SetbTagAttackPending(CachedSkillRTManager && CachedSkillRTManager->IsTagAttackSuggested());
//		}
//
//		// 태그해도 에테르 쿨타임 유지
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
//	// 여기서 세팅하면 AnimBlueprint, Animation, B2TagNotify 거쳐서 UB2SkillRTManager::TagToOther 로 갈 것.
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
//	//{ // 이 경우 공격 도중에 tag 요청을 했을 것.
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
//// bFromInput 틱통해서 들어온거는 구르기나 반격타이머 체크 안할려구 추가
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
//	// 버튼UI있으면 그거통해서 실행
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
//		// QTE Next (QTE 상태에서 들어가는 2차 QTE 의 경우)
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
//		// QTE Dash 종료 시 QTEAttack 이 True 로 바뀌어야 하는데 그 전에 미리 True 로 되어있다면 False 로 바꾼다.
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
//	//	// EQT_Mount 타이머 돌리기시작하는 시점. 게임모드에 알림
//	//	if (QTEType == EQTEType::EQT_Mount && QTEStateNum == 0)
//	//	{
//	//		// UI는 QTE실행자가 로컬플레이어일때만 갱신
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
//	//				DocBattle->SetMaxQTEMountTime(FinalMountTime); // MaxQTEMountTIme 은 변경 콜백을 받지 않을 것이므로 이걸 먼저 세팅.
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
//	//		// 마운트에서 아이들로 바꼈다.
//	//		// CountQTEMountTime() 보면 아이들이상에서 타이머 감소시킴
//	//		if (pGM)
//	//			pGM->NotifyQTEMountBeginToIdle(this);
//	//	}
//
//	//	bPendingQTENext = false;
//	//	QTEStateNum++;
//	//	QTEActor->QTEStateNum++;
//	//	
//	//	// QTE mount 에 실제 공격까지 가능 여부가 변경될 수 있는 타이밍으로 간주.
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
//	Super::SetQTEStateFlag(bInState); // 단순 값 세팅.
//
//	auto* DocBattle = GetDocBattle();
//
//	if (IsLocalPlayer() && DocBattle)
//	{ // UIDoc 에 상태값 전달해 주려고 따로 이렇게 만든 거.
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
//	//	// 해당 로직을 타지 않는 기존 BP ( AnimNotify_QTEDashEnd ) 와 동일하게 적용
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
//	// 이미 QTE 인 상태에서..
//	return (QTEActor && QTEActor->IsInQTEState() && this->IsInQTEState() && this->GetQTEEnabled());
//}
//
//void ABladeIIPlayer::OnEvadeEnd()
//{
//	//// 쿨돌린다. 필요하면. 게임모드에따라 다름.
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
//	//	QTEActor->HideFloatingHPBar(true); // 죽은 목숨이나 다름없으니 HP 바는 없앤다.
//
//	//	// Start mount timer count
//	//	if (QTEActor->QTEType == EQTEType::EQT_Mount)
//	//	{
//	//		// 팀정보 인계
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
//	TotalApplyAnimRateScale(); // 주로 AttackState 에 따라 적용 여부가 달라지므로 SetAttackState 에서 하면 적절할 듯.
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
//	// Animation Blueprint 에서 AllowSequenceDynamicChange 를 체크한 애님 state 로 이전 시 불리게 됨.
//	// 여기서 게임 상태에 따라 특정 AnimSequence 를 리턴하면 AnimBP 의 현재 state 는 원래 배치된 애니메이션이 아닌 여기서 리턴한 애니메이션을 플레이하게 될 것.
//	// NULL 리턴 시 기본값 플레이
//	//
//
//	//ESkillAnimType FinalCurrentSkillAnimType = GetCurrentSKillAnimIdx(); // CurrentSkillAnimIdx 말고 GetCurrentSKillAnimIdx 를 써야
//	//if (IsSkillAttacking() && FinalCurrentSkillAnimType != ESkillAnimType::ESA_End)
//	//{ // AnimBP 가 아닌 별도 테이블에 등록된 skill 애니메이션을 불러들여서 AnimBP 에 먹여줌.
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
//	//	// B2StageEventDirector 에서 마티니 시작 위치로의 싱크 상황, ActiveCameraActor 경계에서의 처리 상황.
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
//		// Dead 애니메이션이 끝난 상태로 유지시킨다. 부활을 하건 중단을 하건, 어차피 새로 spawn 하게 될 것. 아님 다시 생각 ㅋ
//		bReachedAtDeadEnd = true;
//		TotalApplyAnimRateScale();
//
//		// 그리고.. 이쯤에서 부활 메뉴를 시작하도록 GameMode 쪽에 신호
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//		BII_CHECK(B2GM);
//
//		//if (B2GM)
//		//{
//		//	B2GM->NotifyPlayerDeadEnd(this);
//		//}
//
//		// 죽은 시간(연출 완료) 저장.
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
//	// 반격시 HP 회복 옵션은 반격 성공 시 타격 등의 유무와 상관없이 무조건 적용.
//	if (HealHPAmountRateOnCounter > 0.0f) 
//	{
//		// GetUnitedOptionStatusValue 에서 퍼센트를 0.01 스케일한 값을 줄 것인데 HealingHPByPercent 는 퍼센트값을 받으므로 다시 100 곱함.
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
//	{ // TestDummyNPC 자체에서 Damage 를 제거하도록 처리하는 것도 있으나 그것만으로는 한계가 있음
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
//	// Dash중에는 데미지 애니메이션 실행 X
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
//	if (bPendingTag) // 전투 동작 중 태그 예약한 경우
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
//	//if (bWasHidden == bNewHidden) // 같은 조건으로 여러번 이게 불릴 때가 있어서 아래 추가 코드들은 확실히 변경이 되었을 때만 실행되도록
//	//	return;
//
//	//FormalSetPointLightOn(!bNewHidden); // 플레이어 캐릭터만 숨기면 여기에 붙인 라이트가 같이 안 꺼져서 이랬었나?
//
//	//// 특히 Hidden 이후에 다시 보이도록 했을 때 Particle 붙인 것들이 안보이는 경우가 있어서.. 이렇게 해 주니 잘 나오는듯 한데..
//	//if (!bNewHidden)
//	//{
//	//	EnsureActivateStaticallyAttachedPSCs(this);
//	//}	
//}
//
//void ABladeIIPlayer::FormalSetPointLightOn(bool bIsOn)
//{
//	// 일반적인 게임 기능 상의 목적으로 플레이어 머리 위 PointLight On/Off 시에는 이걸 쓰도록.
//	// ABladeIIPlayer 내부에서도.
//	if (LightComponent) 
//	{
//		// 일부 모드에서 머리 위 PointLight 를 허용하지 않는 경우가 있음.
//		const bool bAllowedByGameRule = (GetInjectedGameRule() && GetInjectedGameRule()->AllowPlayerPointLight());
//
//		// 룰에 의해 허가되지 않는 경우..
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
//				LightComponent->SetVisibility(false); // 꼭꼭 숨어라 머리카락 보일라?
//				LightComponent->UnregisterComponent();
//			}
//
//		}
//
//#if !UE_BUILD_SHIPPING
//		extern bool gbTestTurnOffPlayerLight;
//		if (gbTestTurnOffPlayerLight) // Off 의 경우만 따로 처리
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
//		extern int32 gTestAttachLightCompMode; // 이걸 어디에 어떻게 붙여야 좋을지 아직 몰라서 테스트 제공
//		if (gTestAttachLightCompMode == 2) // 0, 1 번은 생성자에서 세팅한 대로 캡슐 attach 유지
//		{
//			LightComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//			LightComponent->AttachToComponent(SKMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_ROOTBONE_NAME);
//			// 루트본은 캐릭터 하단 중심이고 캡슐은 중앙이라 추가 offset 을 가함
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
//	extern int32 gTestAttachLightCompMode; // 아직은 테스트 중..
//	if (gTestAttachLightCompMode == 0)
//#endif
//	{
//		USkeletalMeshComponent* SKMeshComp = GetMesh();
//		if (LightComponent && SKMeshComp)
//		{
//			// 붙이는 건 캡슐 컴포넌트에 붙이지만 중심(Bip001)본 에서 상대적인 위치에 있도록
//			// 이렇게 해서 캐릭터 애니메이션에 따른 메쉬 위치 offset 을 따라가도록 한다. 
//			// 그냥 본에 붙여 버리면 회전까지 따라가게 되어서 격렬한 모션에서 위치가 크게 요동하게 되니 본에 붙이지는 않고 위치만 따라가게 해 줌.
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
//	// 지금 당장의 Auto 상태 여부로서 GameMode 쪽의 Maintained 상태와 달리 일시적으로 꺼질 수 있음.
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
//// 이쪽의 Start/StopAutoPlay 를 직접 사용하는 것은 일시적으로 Auto 를 On/Off 하거나 AI 플레이어의 경우이고
//// 일반적인 게임모드에서 Auto 를 On/Off 할 때에는 GameMode 쪽 인터페이스를 사용해야 상태가 유지된다.
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
//	//	return; // 반복전투에서는 자동 상태를 켜놓은 채 유지.
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
//	//	{ // 일반적인 경우. QTE 에 걸린 몹은 코드상에서 Die 를 통하지 않고 애니메이션에서 DeadEnd 를 통한 처리만 된다. (OnAnimMobDeadEnd)
//	//		QTEActor->DieForQTE(); // 따라서 게임 로직 상 필요한 사망신고 등의 처리를 따로 해 준다.
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
//	// 전투 UI visibility 를 민감하게 제어하는 부분이 좀 있는데 이런저런 타이밍이랑 엮일 때가 있는 거 같아 몇몇 군데에서 확인사살 차 호출할 용도.
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
//	//if (!((InLastAttackState >= EAttackState::ECS_Skill01 && InLastAttackState < EAttackState::ECS_GuardStart) // Tag 도 여기 포함시키는데 Tag 로는 안올 수 있다. 여기 처리를 다른 루트로도 해 주도록.
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
//	if (bCheatImmortalMode) // Damage 는 입지만 죽지는 않음.
//	{
//		return false;
//	}
//#endif
//	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetController());
//	//if (B2PC && B2PC->IsStageEndNonCombatState()){
//	//	return false; // 스테이지 종료 시점에서의 무적 처리 추가로.
//	//}
//
//	return Super::CanDie(KillingDamage, DamageEvent, Killer, DamageCauser);
//}
//
//void ABladeIIPlayer::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnDeath);
//
//	// 부모함수나 저아래 태그거치면서 로컬플레이어가 아니게됨 미리체크
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
//	//// 태그가 허용된 게임모드이고 태그가 가능하다면 자동태그
//	//// 넷게임모드는 제외. pvp게임모드 보면 NotifyCharacterDead 통해서 태그하는 로직이 있음
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
//	// 결속스킬 사용중엔 안참
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
////		return 0.0f; // 스테이지 종료 시점에서의 무적 처리. bCanBeDamaged 를 써도 기능면에서는 될 거는 같은데 혹 다른 거랑 엮일지 모르겠군.
////	}
////
////	const FDamageInfo* DamageType = static_cast<const FDamageInfo*>(DamageEvent.DamageInfo);
////	const FDamageInfo& DamageInfo = *(DamageType);
////	if (!DamageType) {
////#if !UE_BUILD_SHIPPING
////		// 대체로 DamageType 이 없는 상황은 연출을 통해 플레이 되는 attack 애니메이션에서 damage notify 가 나가는 거. InterpTrackAnimControl 의 SkipDamageNotifiersOnly 를 체크.
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
////	// 반격 던전용
////	bCounterCauseStun = DamageType->bStunCountable;
////
////	if (ActualDamage > 0.f || GetB2GameModeType(GetWorld()) == EB2GameMode::CounterDungeon)
////	{
////		PlayerTakeAttackClass<float>::GetInstance().Signal(0.1f);
////	}
////
////#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
////	if (bCheatImmortalMode)
////	{// CanDie 에서 return false 하였지만 Health 값이 0인지 검사해서 죽은 것 처럼 처리할 수 있으므로 1은 남겨둠.
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
////	// 빈사 상태
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
//	TargetDamagedClass<ABladeIICharacter*, int32, float>::GetInstance().Signal(this, CurrentComboNum, DamageAmount); // UI 로 신호
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
//	// 몇가지 처음에 세팅되어야 할 것들..
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::InitUpdateUIDoc"));
//
//	Super::InitUpdateUIDoc();
//		
//	// ABladeIIPlayer::SpawnOtherAsPuppet 같은 걸로 생성된 경우 update 에 끼어 들어가선 안됨.
//	//if (IsLocalPlayer())
//	//{
//	//	// UIDoc 이 static 이므로 매 틱 업데이트 되는 게 아닌 많은 변수들에 대해 상태를 초기화해 줄 필요가 있다.
//	//	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
//	//	InjectDocBattle(DocBattle);
//
//	//	if (DocBattle)
//	//	{
//	//		DocBattle->SetCurrPCMaxHealth(MaxHealth);
//	//		DocBattle->SetComboNum(CurrentComboNum);
//
//	//		// 태그 시점에도 결국 여기로 올 것이므로 태그 클래스 등을 여기서 세팅
//	//		// UB2SkillRTManager::DynSpawnNewPlayerCommon 참고. 이 시점에 TagPlayerClass 를 얻어올 수 있을 것.
//	//		// 인게임에서는 현재 활성/비활성 캐릭터와 실제 main/sub 의 구분을 해야 함. Main 이 확실히 현재 활성인 경우는 태그를 아직 하지 않은 게임 시작 초반뿐임.
//	//		DocBattle->SetIsCurPCMain(AmIMainCharacter());
//	//		DocBattle->SetCurPCClass((int32)GetCurrentPlayerClass());
//	//		DocBattle->SetTagPCClass((int32)GetTagPlayerClass());
//	//		DocBattle->SetInAutoPlay(IsInAutoPlay()); // AutoPlay 여부는 처음에만 여기서 세팅하고 이후에는 GameMode 쪽에서
//
//	//		DocBattle->SetbTagAttackPending(false); // 해당 플래그는 이미 사용된 후일 것이다.
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
//	//	TArray<FUISkillBuffDisplayInfo_Character> AllCharacterBuffInfo; // 현재 발동한 것들만 넣기
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
//	//	B2GM->GetUIManager_InGameCombat()->DestroyHUDForDyingPlayer(this); // 플레이어 전용 제거.
//	//}
//}
//
//UB2SkillRTManager* ABladeIIPlayer::GetSkillRTManager()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetSkillRTManager);
//	if (CachedSkillRTManager == NULL)
//	{
//		// Controller 에 의해 Possess 되어야 가능
//		auto* B2PC = Cast<ABladeIIPlayerController>(GetController());
//		CachedSkillRTManager = B2PC ? B2PC->GetPlayerSkillRTManager() : NULL;
//
//		if (CachedSkillRTManager)
//			CachedSkillRTManager->InitSkillRTManager(this);
//	}
//	BII_CHECK(!Cast<ABladeIIPlayerController>(Controller) || CachedSkillRTManager); // Controller 가 없는 상황이 ABladeIIPlayer::SpawnOtherAsPuppet 에서 나올 수 있음.
//	return CachedSkillRTManager;
//}
//
//ICharacterDataStore* ABladeIIPlayer::GetCharacterDataStore()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCharacterDataStore);
//	//여기로 오는 것은 Local용 Player character로 가정한다.
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
//	check(!OptionalSKMeshToSet); // 적어도 여기 외부에서 이걸 직접 넣어주는 경우는 없는 걸로.. 혹 있을 거 같음 이 check 를 제거하는데 아래 코드와 엮이지 않는지 참고..
//
//	//// 섹션 병합 모델을 사용하는 게임모드인데 연출에서도 그걸 연속적으로 사용하는 게 아니면 새로 섹션 분리된 모델 세팅
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
//	//	ExplicitNonMergedMesh ? ExplicitNonMergedMesh : OptionalSKMeshToSet // OptionalSKMeshToSet 인자를 여기서 만들어서 넘겨주게 됨..
//	//);
//
//	//// 장신구 같은 거 붙으면 추가로 세팅해 주려고 했는데 웬만한 것들은 상위단 함수에서 기본 처리.
//	//
//	//if (MyBaseMeshComp && PuppetBaseMeshComp)
//	//{
//	//	// Scalability 컨트롤의 일부로서 ForcedLOD 먹인 것도 전달해 준다. 사실 연출용은 0 일거라 필요는 없겠지만 빼먹어서 좋을 꺼 없지.
//	//	PuppetBaseMeshComp->SetForcedLOD(MyBaseMeshComp->ForcedLodModel);
//	//}
//}
//
///** 대체로 연출 용으로 더미 태그 캐릭터 spawn. 더미라고 해서 테스트 기능이 아니다.
// * PossessByAIController 는 AI 를 위한 게 아니라 보다 기본적인 중력 적용 같은 거 처리 차원. */
//ABladeIIPlayer* ABladeIIPlayer::SpawnOtherAsPuppet(bool bPossessByAIController /*= true*/, bool bTurnOnPointLight /*= false*/)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SpawnOtherAsPuppet);
//	BII_CHECK(GetTagPlayerClass() != EPCClass::EPC_End);
//	ABladeIIPlayer* Spawned = SpawnPuppetInternal(GetTagPlayerClass(), bPossessByAIController);
//	if (Spawned)
//	{ // 모바일 pointlight 제한도 있고.. 태그쪽은 가급적 끄도록.. 이게 필요한 상황이 있으려나..
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
//	//	// 살짝 옆에서 spawn 시킨다. 이걸 쓰는 상황에서 실제로 보여지는 위치는 연출 트랙 등으로 별도로 조절해야 함.
//	//	FTransform NewTransform(
//	//		CurrTransform.GetRotation(),
//	//		CurrTransform.GetTranslation() + FVector(AsideDistance, AsideDistance, 50.0f),
//	//		CurrTransform.GetScale3D()
//	//	);
//
//	//	// 딱히 이전 걸 없앤다든지 할 필요 없이 그대로 리턴. 받은 쪽에서 잘 처리. 
//	//	// PlayerController 에 possess 하지는 않으니 GetLocalPlayerCharacter 등이 의도와 달리 여기서 생성한 것을 리턴한다든지 하는 일은 없을 것.
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
//// 아래 SetupLODInfo~~ 씨리즈 용 공통 기능
//static void SetB2PCMergedMeshCompLODInfoInternal(USkeletalMeshComponent* InMeshComp, float InScreenSize_0, float InScreenSize_1)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(Global_SetB2PCMergedMeshCompLODInfoInternal);
//	//// 실제 에디터에서 보는 리소스가 아닌 런타임에 merge 된 메쉬를 상정함. 여기서 Mesh 리소스의 속성값을 바꿀 것이기 떄문에 일반적으로 에디터에서 보는 그 리소스여서는 곤란하다.
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
//	//	// 일단은 0 과 1 이상의 LOD 만 구분. 그 이상은 필요에 따라
//	//	MeshLODInfo.ScreenSize = (LODIdx == 0 ? InScreenSize_0 : InScreenSize_1);
//	//	
//	//	if (PrevScreenSize != MeshLODInfo.ScreenSize)
//	//	{
//	//		bScreenSizeChanged = true;
//	//	}
//	//}
//	//if (bScreenSizeChanged)
//	//{
//	//	InMeshComp->UpdateLODStatus(); // 이렇게 한다고 여기서 바로 변경이 될 거 같지는 않은데..
//	//	InMeshComp->MarkRenderStateDirty();
//	//}
//}
//
///** 
// * Scalability 시스템의 GameplayOnlyLODQuality 의 세부 항목으로 사용하기 위함. 
// * 전투씬, 연출씬 (로비포함) 두 부류로 나눠서
// * ForceLODLevel 이랑 ForcedLODLevel 이 0 일 때 사용할 ScreenSize 가 있다.
// * 기능 자체만 볼 때에는 ForceLODLevel 로 웬만큼 커버 가능한데 ForceLODLevel 에 의존하면 r.SkeletalMeshLODBias 를 통한 테스트도 어려워져서 덜 쓰게 됨. 
// * 그리고 간혹 예상치 못한 급격한 카메라웤에 대비해야 할 것 같은 때도 있고.
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
//// 전투용 LOD 1 ScreenSize 는 비교적 큰 값으로 준다. LOD1 의 허용 ScreenSize 가 커질수록 근거리에서 LOD1 이 적용.
//float GB2PlayerCharLOD1ScreenSizeCombat = 0.5f;
//FAutoConsoleVariableRef CVarB2PlayerCharLOD1ScreenSizeCombat(
//	TEXT("r.B2PlayerCharLOD1ScreenSizeCombat"),
//	GB2PlayerCharLOD1ScreenSizeCombat,
//	TEXT("LOD 1 ScreenSize for BladeII player character meshes in combat scene, intended to be controlled by Scalability system."),
//	ECVF_Scalability
//);
//// 연출용 LOD 1 ScreenSize 는 비교적 작은 값으로 준다. LOD1 의 허용 ScreenSize 가 작아질수록 원거리에서 LOD1 이 적용.
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
//	// Scalability 시스템에 따른 LOD 설정 조절. ForceLOD 혹은 LODScreenSize
//	SetB2PCMergedMeshCompLODInfoInternal(GetBaseMesh(), 0.0f, GB2PlayerCharLOD1ScreenSizeCombat);
//	GetBaseMesh()->SetForcedLOD(GB2PlayerCharForcedLODLevelCombat);
//}
//void ABladeIIPlayer::SetupLODInfoForCinematic()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupLODInfoForCinematic);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupLODInfoForCinematic"));
//
//#if !UE_BUILD_SHIPPING
//	// 이 코드 자체는 사실상 BladeIIScalabilityOverride 는 아니다.
//	// 단, 성격이 어느 정도 비슷하고 연출 진입 시 spike 분석 등에도 필요해서.. 여기도 bUseBladeIIScalabilityOverride 에 따라 게임플레이 용 메쉬 LOD 설정과 마찬가지로 갈 수 있도록 함.
//	/*if (!B2Scalability::bUseBladeIIScalabilityOverride) {
//		SetupLODInfoForInGame();
//		return;
//	}*/
//#endif
//
//	// SetUseSectionMergedMesh 세팅도 여기에 끼워넣기 적절해 보일 수 있지만 그러지 말 것.
//	// 그건 상황에 따라 좀 더 다르게 들어간다.
//
//	// Scalability 시스템에 따른 LOD 설정 조절. ForceLOD 혹은 LODScreenSize
//	// 사실 연출에서 보여지는 건 메쉬 리소스는 여기 껄 전달해 주겠지만 컴포넌트는 다르니 ForceLOD 는 그쪽으로 전달해 주어야 한다.
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
//	//// 로비에서는 초기에 Merge 보다는 Component 구성을 생각하고 있었기 때문에 모두 가져오게 했는데.. 결국 그렇게 안될 듯.
//	//// 사실 Component attach 구성을 하게 되면 아래 SetB2PCMergedMeshCompLODInfoInternal 을 사용하기가 곤란해진다. 런타임 생성된 메쉬가 아니라 임포트 된 리소스를 건드리게 될 것이므로.
//	//TArray<UActorComponent*> AllSKComps = InLobbySKActor->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
//
//	//for (UActorComponent* CurrComp : AllSKComps)
//	//{
//	//	USkeletalMeshComponent* CurrSKComp = Cast<USkeletalMeshComponent>(CurrComp);
//	//	// 로비도 Cinematic 설정을 가져간다.		
//	//	SetB2PCMergedMeshCompLODInfoInternal(CurrSKComp, 0.0f, GB2PlayerCharLOD1ScreenSizeCinematic); // ForcedLOD 0 이면 적용.
//	//	if (CurrSKComp)
//	//	{
//	//		CurrSKComp->SetForcedLOD(GB2PlayerCharForcedLODLevelCinematic); // 로비도 CinematicLODLevel 을 적용
//	//	}
//	//}
//}
//void ABladeIIPlayer::OnBeginWeaponSkillLOD0()
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OnBeginWeaponSkillLOD0"));
//
//	checkSlow(IsLocalPlayer()); // 아닌 경우 있는지..?
//	//
//	//// 인게임 전투에서는 섹션 병합 모델을 쓰지만 연출에서는 아닌 경우.. 일시적으로 섹션 분리 모델 사용 세팅.
//	//if (/*UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this) &&*/ // 요 조건은 안 보기로.. 흔치 않은데 상태가 꼬이는지 결속스킬에서 병합된 모델이 나오는 경우가 있다.
//	//	!UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this))
//	//{
//	//	SetUseSectionMergedMesh(false);
//	//}
//
//	//SetupLODInfoForCinematic(); // 연출용 LOD 셋업은 이전부터 하던 기본.
//	//
//	//DestroyWeaponSkillLOD0SceneLight();
//	//CreateWeaponSkillLOD0SceneLight();
//
//	//bInWeaponSkillLOD0 = true;
//
//	//FTimerManager& WTM = GetWorldTimerManager();
//	//WTM.ClearTimer(WeaponSkillEndHandlingForceTH);
//	//// 이런 짓을 하지 않아도 OnEndWeaponSkillLOD0 가 불려야 하지만 만일을 위한 안전장치.
//	//WTM.SetTimer(WeaponSkillEndHandlingForceTH, this, &ABladeIIPlayer::ForceFinishWeaponSkillLOD0SceneSetup
//	//	, 30.0f/*이 이상 가는 결속스킬이 있다면 변태겠지.*/
//	//	, false);
//}
//void ABladeIIPlayer::OnEndWeaponSkillLOD0()
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OnEndWeaponSkillLOD0"));
//
//	//bInWeaponSkillLOD0 = false;
//
//	//// 연출용 세팅들 리셋 
//	//if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
//	//{ // 여긴 AnimNotify 를 통해 발동되는 곳이다 보니 바로 SetUseSectionMergedMesh 하면 UpdateAnimation 이 재귀호출 되어버리는 사태가 발생한다
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
//	이 짓을 하는 목적은 결속스킬 시전 시 별도 라이트 세팅을 하지 않으면
//	머리 위에 있는 PointLight 가 얼굴을 위에서 비추기 때문에 정말 바보같은 조명빨이 나오기 때문이다.
//	매 결속스킬 별로 세세한 세팅을 제공하는 건 일이 너무 커지고 어느 정도 적당한 조명이 나오도록 최소한의 처리와 세팅 제공.
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
//		// 세팅을 거창하게 제공할 건 아니지만 스킬별로 몇가지 속성 정도는 제공하게 될 수도 있다.
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
//		// 위치는 UpdateWeaponSkillLOD0LightComponentAttachment 에서 갱신됨.
//		//WeaponSkillLOD0SceneLight->RelativeLocation = FVector(500.0f, 0.0f, 0.0f); // 이건 Tick 에서 다시..
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
//		// UpdateLightComponentAttachment 에서 하는 기본 PointLight 위치 갱신과 비슷하게 매 틱 특정 본을 따라가지만 
//		// 보다 현란한 움직임을 할 수 있으므로 단순히 Z 값만 넣지는 않는다.
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
//	// SkillRTManager 값을 가져오므로 destroy 대기 상태인 태그 중의 OldPlayer 에게서는 의미가 없음.
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
//	// 근데 어차피 possess 안된 상태에서 GetSkillRTManager 가 null 이면 답이 없군.. ㅜㅠ
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
//	// 공격 도중에 tag 를 했다면 이어서 공격을 계속 하도록 분노조절장애를 유발.
//	if (OldRetiringPlayer->IsKeepAttacking())
//		SetKeepAttacking(true);
//
//	// 공격 방어 버튼상태 인계
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
//	//// 특정 모드에서의 전투맵 디자인 사정으로 AspectRatioAdaptiveFOV 를 사용하는 경우가 있음.
//	//if (OldRetiringPlayer->TopDownCameraComponent && OldRetiringPlayer->TopDownCameraComponent->GetAspectRatioAdaptiveFOV())
//	//{
//	//	if (TopDownCameraComponent)
//	//	{
//	//		TopDownCameraComponent->SetAspectRatioAdaptiveFOV(true);
//	//	}
//	//}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 기본 치트들 다시 태어나도 유지.. 매번 쳐주려니 불편하군
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
//	// 이건 Tag 와 마찬가지의 UI 애니메이션을 의도한 것인데, 아직 확정된 바는 없음. 향후 정해지는 바에 따라 이렇게 가던지, 독자적 구현을 하던지.
//	// NotifyTagSpawn 을 직접 불러주지 않게 되면 그쪽에 구현한 것들을 필요에 따라 이쪽에도
//	NotifyTagSpawn(PrevLife);
//}
//
//void ABladeIIPlayer::TakeOverAutoFromPredecessor(class ABladeIIPlayer* InPredecessor)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_TakeOverAutoFromPredecessor);
//	check(InPredecessor);
//	// 부활, 태그 등의 상황. 여긴 로깔플레이어 뿐 아니라 네퉠플레이어도 들어오므로 GameMode 쪽 Auto 상태를 사용하면 안됨.
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
//	//// 실제 상태 변수는 Spawn 직후 Animation 업데이트 타이밍을 고려하여 Player Controller 쪽에 있음
//	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(Controller);
//	//if (B2PC == NULL)
//	//{
//	//	// 이것이 불리는 시점이 spawn 처리를 하는 도중이라 미처 possess 가 안되었을 수 있음. 어쩔 수 없이..
//	//	// GetNearestPlayerController 를 하면 최소한 tag 나 resurrect 시점에서 제거되기 직전의 이전 플레이어에 possess 된 것을 가져올 것. (B2SkillRTManager 쪽 참고)
//	//	// 그러나 만일 플레이어 캐릭터가 여럿인데 멀리 떨어진 곳에 부활을 해야 하는 상황 등이 오면 다시 생각해 봐야 함.
//	//	B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetNearestPlayerController(this, this->GetActorLocation()));
//	//}
//
//	return /*B2PC ? B2PC->GetPCSpawnMotionState() :*/ (EPCSpawnMotionState)EPCSpawnMotionState::EPST_Normal;
//}
//
//void ABladeIIPlayer::EndSpawnMotionState()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_EndSpawnMotionState);
////	// 실제 상태 변수는 Spawn 직후 Animation 업데이트 타이밍을 고려하여 Player Controller 쪽에 있음
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
//	// 홀로 남겨져 고독한 최후를 맞은 자의 세레모니. 고통과 수치로 가득찬 이 시간이 지나고 나면 새로운 시작을 위한 부활 메뉴가 뜰 것이다.
//	// OnDeath 에 이걸 부르면 실제 애니메이션 상태는 전투 중일 수 있으므로 Dead 애니메이션에 notify 를 통해 부르도록 함.
//	//UGameplayStatics::SetGlobalTimeDilation(this, OnDeathTimeDilation); // 이건 일단 사용 안함.
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
//	// Anything necessary. 가능하면 각 하위 Drop 클래스의 OnPlayerSuckedMe 에서 처리.
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
//	// 최소한 BeginPlay 시점에 이게 불려야 함. InitializeCombatStats 이전에
//	CharacterLevel = GetPlayerLevel();
//}
//
//void ABladeIIPlayer::OnSomeMobTermination(ABladeIICharacter* InMobAboutToDestroy)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnSomeMobTermination);
//	if (QTEActor && QTEActor == InMobAboutToDestroy) 
//	{
//		//UE_LOG(LogBladeII, Warning, TEXT("Calling FinalKillQTETarget on QTE target Actor's termination. This is not a desired path."));
//		// QTE 타겟 몹 사망 처리를 이쪽서 먼저 해 주는 게 좋은데 QTE 몹의 DeadEnd 가 먼저 오고 LifeSpan 마저 다 한 상황이 생기곤 해서.. 안전 차원에서 처리
//		FinalKillQTETarget();
//	}
//}
//
//void ABladeIIPlayer::SignalStopAttackImmediate()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SignalStopAttackImmediate);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SignalStopAttackImmediate"));
//
//	bStopAttackSignal = true; // 이후 AnimBP 에서 처리된 후 리셋.
//
//	TryRestoreFromCameraDistZoomNS(); // CameraDistZoom 이 들어가고 있던 상황이라면 그것도 중단.
//
//	StopRepeatingInput();
//}
//
//bool ABladeIIPlayer::ConsumeAndCheckStopAttackMotion()
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_ConsumeAndCheckStopAttackMotion);
//	// 한번 사용하면 땡.
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
//	float Ret = Super::GetDamageIncreaseRate() + IncDamageValueByPassive; // 공격 상태에 의존하지 않는 기본적인 데미지 및 증가 옵션 수치
//	
//	//// 현재 스킬에 따라 반영되는 Active 스킬 옵션 데미지 증가 수치
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
//	//	// 이것들은 특정 스킬 번호에 적용되지만 Active 아닌 Passive 성격..
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
//	//{ // 일반공격에만 적용되는 데미지 증가.
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
//	// Passive + AttackState 에 따른 Active 옵션 수치 합.
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
//	//return (AttackerClass != EPCClass::EPC_End) ? DecReceiveDamageByPassiveFromClass[PCClassToInt(AttackerClass)] : 1.0f; // Dec 라 기본값 1.0
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
//	// GetCriticalRate 의 일부인데 이 수치는 특별히 UI 쪽에도 뿌려주므로 따로 뺌
//	return 0.01f * FMath::Min( FMath::Max(0, CurrentComboNum -1) / 10, 3);
//}
//
//float ABladeIIPlayer::GetCriticalRate() const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetCriticalRate);
//	//////////////////////////////////////////////////
//	// ! 전투 옵션 수치 적용 시 Inc** 는 더하고 Dec** 는 곱함. GetEffectiveValueForOptionApplyType 참고.
//
//	float CriticalRate = Super::GetCriticalRate();
//
//	//CriticalRate += GetAdditionalCriticalRateByComboNum()
//	//	+ IncCriticalRateValueByPassive // ItemOption 및 passive SkillOption 에 의한 수치
//	//	+ (CachedSkillRTManager ? CachedSkillRTManager->GetCriticalRateByActive(GetAttackState()) : 0.f); // 현재 AttackState 에 따른 active skill 옵션 수치.
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
//	//CriticalDamageRate += IncCriticalDamageValueByPassive // ItemOption 및 passive SkillOption 에 의한 수치
//	//	+ (CachedSkillRTManager ? CachedSkillRTManager->GetCriticalDamageRateByActive(GetAttackState()) : 0.f); // 현재 AttackState 에 따른 active skill 옵션 수치.
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
//	// 가급적 SkillOption 과 통합된 GetUnitedOptionStatusValue 의 사용을 고려. 차차 전환..
//	//return CharacterDataStore->GetOptionStatusValue(GetCurrentPlayerClass(), OptionType);
//	return 0.0f;
//}
//
//float ABladeIIPlayer::GetUnitedOptionStatusValue(EUnitedCombatOptions OptionType) const
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_GetUnitedOptionStatusValue);
//	// 가급적 이 함수의 호출은 캐릭터 spawn 초기화 시에 한정을.. 필요한 값은 캐싱해 놓고 쓰는 걸 권장.
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
//		GetMesh()->GlobalAnimRateScale = (bReachedAtDeadEnd ? 0.0f : ( // DeadEnd 에서의 정지상태
//			IsAttacking() ? (1.0f + AdditionalAttackSpeedRatebyPassive) : // 공격 상태에서만 옵션 적용된 값으로. Active 옵션이나 Buff 영향도 여기서 적용하면 될 듯..
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
//	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f); // 굳이 OnDeathTimeDilation 진입 이전 값을 캐싱해 놓을 필요는 없을 것 같다..
//}
//
//void ABladeIIPlayer::ReserveQTEActorReportMyDeathToBirthplace()
//{
//	// 탑승 완료시점에서 스폰풀쪽엔 죽음으로 처리
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
//	// 탈것 타는동안 콜리전박스가 사라져 EndActiveCamera가 호출되고 월드카메라로 변경됨 그래서 갑자기 카메라가 바뀌는 현상 막기
//	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
//	FloorRingPSC->SetHiddenInGame(true);
//
//	// 타기 시작했으면 QTE대기 종료콜백 취소.
//	pQTETarget->CancleQTEEnableEndCB();	
//}
//
//void ABladeIIPlayer::OnQTEMountTargetEnd(const FTransform& QTETransform)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_OnQTEMountTargetEnd);
//	// 여기서 StopQTEMount 한 후에 mount target 의 AnimNotify 를 통해 오는 거.
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
//	//// UI영향은 로컬플레이어만
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
//	if (QTEMountRemainingTime <= 0.0f) // StopQTEMount 이후에도 여기로 들어오는 케이스가 있음. 여기서 짜른다.
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
//	//	StopQTEMount(); // 이전에 타이머 콜백이었음. 여튼 이걸루 끝낸다.
//	//}
//}
//
//void ABladeIIPlayer::SetupDecalCompForLeftShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupDecalCompForLeftShadowCommon);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupDecalCompForLeftShadowCommon"));
//
//	// 원래 BladeIIPlayer 자신이라면 InDecalCompToUse 는 LeftFootShadow 일 것임. SetupControlledMatineePuppet 기능을 위해 매개변수로 받는다. 
//	// 여기서 컴포넌트 register 까지 하지는 않는다.
//	if (InDecalCompToUse && AttachParent)
//	{
//		//InDecalCompToUse->SetHiddenInGame(false);
//		InDecalCompToUse->SetRelativeScale3D(FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot));
//
//		InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//		InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_SHADOW_SOCKET_NAME_LF); // 전용 socket 이 있음.
//	}
//}
//
//void ABladeIIPlayer::SetupDecalCompForRightShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIPlayer_SetupDecalCompForRightShadowCommon);
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupDecalCompForRightShadowCommon"));
//
//	// 원래 BladeIIPlayer 자신이라면 InDecalCompToUse 는 RightFootShadow 일 것임. SetupControlledMatineePuppet 기능을 위해 매개변수로 받는다. 
//	// 여기서 컴포넌트 register 까지 하지는 않는다.
//
//	if (InDecalCompToUse && AttachParent)
//	{
//		//InDecalCompToUse->SetHiddenInGame(false);
//		InDecalCompToUse->SetRelativeScale3D(FVector(ShadowMaxDistance, ShadowSizeFoot, ShadowSizeFoot));
//
//		InDecalCompToUse->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//		InDecalCompToUse->AttachToComponent(AttachParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_SHADOW_SOCKET_NAME_RF); // 전용 socket 이 있음.
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
//	//			if (/*CurrDistSQ < 500 && 타겟팅 제한거리*/CurrDistSQ < ClosestDistanceSQ)
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
//	//	if (/*CandidateDist < 500 && 타겟팅 제한거리*/CandidateDist < TargetDist)
//	//	{
//	//		/* 사물을 뚫고 공격할 수 있다고 해서 아래 코드가 필요없어짐..
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
//	// 1. 근본적으로 Rotate가 불가능할때
//	if (IsStunned() || IsAlive() == false || CustomTimeDilation <= 0.f)
//		return false;
//
//	// 2. 필수 값 셋팅이 안되어 있을때
//	if (BP_ConfigMinDashDistance <= 0.f || BP_ConfigMaxDashDistance <= 0.f)
//		return false;
//
//	// 3. 일반 QTE 중일때 (마운트는 Rotating 해야하므로 제외)
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
//	//// poikl2212 입력있을때만 입력방향으로 설정. 기본값은 현재 로테이션으루
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
//	////Qte 중이면 무시..
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
//// 로직없음 받은 타겟쪽으로 회전
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
//	{ // QTE 상태 종료.. 
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
//		{ // QTE Mount 시 Light 를 끄던 적이 있었는데 이제 그럴 일 없으면 필요없겠지..
//			FormalSetPointLightOn(true);
//		}
//
//		// QTE종료 콜백들
//		for (FQTEOnEnd& QTEOnEndCallBack : arQTEOnEndCallBack)
//		{
//			if (QTEOnEndCallBack.ExecuteIfBound())
//				QTEOnEndCallBack.Unbind();
//		}
//
//		arQTEOnEndCallBack.Empty();
//		// QTE종료 콜백들 끝
//
//		ResetAllQTEUIDocState(); // 일반적인 상황이면 이 시점에서 UIDoc 에도 다 반영이 되어야 하지만 간혹 안 그럴 때가 있는 거 같아서 확실히 리셋.
//	}
//}
//
//void ABladeIIPlayer::CheckSendKakaoActionLog_Death()
//{
//	//EB2GameMode CurrentGameModeType = GetGameMode()->GetB2GameModeType();
//	
//	// 팀대전은 무조건보냄, 1:1, 점령전은 로컬플레이어만 보냄
//	//if ((CurrentGameModeType == EB2GameMode::PVP_Team && GetIsLocalPlayerTeam())
//	//	|| (CurrentGameModeType == EB2GameMode::PVP_Tag && IsLocalPlayer())
//	//	|| (CurrentGameModeType == EB2GameMode::Control && IsLocalPlayer())
//	//	)
//	//{
//	//	//////////////////////////////////////////////////////////////////
//	//	//액션 지표 로그 (플레이어 죽음)
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
//	//// 팀대전은 무조건보냄, 1:1, 점령전은 로컬플레이어만 보냄
//	//if ((CurrentGameModeType == EB2GameMode::PVP_Team && GetIsLocalPlayerTeam())
//	//	|| (CurrentGameModeType == EB2GameMode::PVP_Tag && IsLocalPlayer())
//	//	|| (CurrentGameModeType == EB2GameMode::Control && IsLocalPlayer())
//	//	)
//	//{
//	//	//////////////////////////////////////////////////////////////////
//	//	//액션 지표 로그 (플레이어 킬)
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
//	//	{// 이것도 저것도 안되는 경우만 캐싱해 놓은 걸 사용.
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
//		if (FMath::FRand() < GetProbableHPAbsorptionProbability())// 여기 확률은 고정된 것으로 옵션 수치는 회복량이지 확률이 아님.
//		{
//			  // GetUnitedOptionStatusValue 에서 퍼센트를 0.01 스케일한 값을 줄 것인데 HealingHPByPercent 는 퍼센트값을 받으므로 다시 100 곱함.
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
//		if (FMath::FRand() < GetProbableRecoverHPProbability())// 여기 확률은 고정된 것으로 옵션 수치는 회복량이지 확률이 아님.
//		{
//			// GetUnitedOptionStatusValue 에서 퍼센트를 0.01 스케일한 값을 줄 것인데 HealingHPByPercent 는 퍼센트값을 받으므로 다시 100 곱함.
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
//			// 장애물 검사
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
//// 서버 안 붙은 상황에서 단독 테스트 시 원인을 알 수 없게 빠르게 움직여서 궁여지책. 쿠킹 버전만 그럼.
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
