// Fill out your copyright notice in the Description page of Project Settings.
#include "B2ActiveCameraActor.h"
//#include "BladeII.h"
#if WITH_EDITOR
#include "Editor.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Components/TextRenderComponent.h"
#endif

#include "BladeIIPlayer.h"
#include "BladeIIPlayerController.h"
#include "BladeIIGameMode.h"
#include "B2TMGameMode.h"
#include "BladeIIWorldSettings.h"
#include "B2MonsterSpawnPool.h"
#include "GameFramework/SpringArmComponent.h"
#include "BladeIIScalabilityControl.h"

#include "FB2ErrorMessage.h"


ABladeIIPlayer* AB2ActiveCameraActor::GetInteractPlayerCharacter()
{ 
	return (CachedInteractingController ? Cast<ABladeIIPlayer>(CachedInteractingController->GetPawn()) : NULL);
}

// Sets default values
AB2ActiveCameraActor::AB2ActiveCameraActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//	bReplicates = false;
//	NetUpdateFrequency = 10.0f;
//	bReplicateMovement = false;
//	bCanBeDamaged = false;
//
//	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
//	SetRootComponent(DefaultRootComponent);
//
//#if WITH_EDITOR && WITH_EDITORONLY_DATA
//	EditorSpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("ActiveCameraActorSprite"));
//	if (!IsRunningCommandlet() && (EditorSpriteComponent != nullptr))
//	{
//		// Structure to hold one-time initialization
//		struct FConstructorStatics
//		{
//			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
//			FName ID_B2ActiveCameraActor;
//			FText NAME_B2ActiveCameraActor;
//			FConstructorStatics()
//				: SpriteTexture(TEXT("/Engine/EditorResources/S_SceneCaptureIcon")) // 딱 카메라 모양 아이콘이 있군.
//				, ID_B2ActiveCameraActor(TEXT("B2ActiveCameraActor"))
//				, NAME_B2ActiveCameraActor(FText::FromString(TEXT("B2ActiveCameraActorEditorSprite")))
//			{
//			}
//		};
//		static FConstructorStatics ConstructorStatics;
//
//		EditorSpriteComponent->Sprite = ConstructorStatics.SpriteTexture.Get();
//		EditorSpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_B2ActiveCameraActor;
//		EditorSpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_B2ActiveCameraActor;
//		EditorSpriteComponent->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//		EditorSpriteComponent->bIsScreenSizeScaled = true;
//		EditorSpriteComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
//	}
//#endif
//
//	CameraMovementMode = EActiveCameraMovementMode::EACMM_FollowPlayer;
//	CameraTargetingMode = EActiveCameraTargetingMode::EACTM_Player;
//
//	// 상단에 CameraMovementMode 랑 CameraTargetingMode 설정 및 SetEditConditionProperties 따라 적절하게
//	bDualCameraBlending_Inverse = true;
//	bCameraMovementMode_ScaledFollowingPlayer = false;
//	bCameraTargetingMode_Mob = false;
//	bCameraTargetingMode_Mob_with_PlayerCameraBoom = false;
//	bCameraTargetingMode_Fixed = false;
//	bCameraMovementTarget_BothPlayer = true;
//	bUseActiveCameraComponentMode = false;
//	bTriggerMode_ToggleByComponent = false;
//
//	bIsActivated = false;
//
//	CameraPlayerFollowingScale = 0.5f;
//	BlendInTime = 1.0f;
//	BlendOutTime = 1.0f;
//	TransitionInputDisableTime = 0.6f;
//	TransitionAutoMoveScale = 1.0f;
//	bTransitInInputDisabled = false;
//	bTransitOutInputDisabled = false;
//	
//	bOverrideFOVAngle = false;
//	FOV_Override = 90.0f;
//	DCBExtraFOV_Override = 90.0f;
//
//	bDualCameraBlending = false;
//	DCBExtraCamName = NAME_None;
//	DCBFlagName = NAME_None;
//	DCBExtraFlagName = NAME_None;
//	bRestoreCamera = true;
//	CachedDualCameraBlendedYaw = 0.0f;
//
//	bOverrideSpringArmSettings = false;
//
//	bUseCameraBoomRotationClamp = false;
//
//	TriggerMode = EActiveCameraTriggerMode::EACTRG_ActorArea;
//
//	FixedCameraTargetPosition = FVector(0.0f, 0.0f, 0.0f);
//
//	PlayerMoveInputAdjustBoomSyncAlpha = 1.0f;
//
//	ActiveCameraSpringArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("ActiveCameraSpringArm"));
//	ActiveCameraSpringArm->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	ActiveCameraSpringArm->TargetArmLength = 100.0f;
//	ActiveCameraSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
//	ActiveCameraSpringArm->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
//
//	ActiveCameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("ActiveCameraComp"));
//	ActiveCameraComp->AttachToComponent(ActiveCameraSpringArm, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//	ActiveCameraComp->bAutoActivate = true;
//	ActiveCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
//	ActiveCameraComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
//
//	bUsePlacedActiveCamYawAsInputBase = false;
//	LastInteractBeginPlayer = NULL;
//	CachedLocalPC = NULL;
//	CachedInteractingController = NULL;
//	CachedTargetMob = NULL;
//
//	bActivedByAnimation = false;
}

void AB2ActiveCameraActor::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	SetEditConditionProperties();

	if (bDualCameraBlending && CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed && !FindExtraCamCompForDualCameraBlending())
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("[경고] ActiveCameraActor %s 에 DualCameraBlending 이 설정되었으나 이를 위한 추가 카메라 컴포넌트가 제대로 설치 혹은 설정되어 있지 않습니다."), *GetName())
			));
#endif
	}
#endif
}

void AB2ActiveCameraActor::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
#if WITH_EDITOR
	if (GIsEditor)
	{
		RefreshEditingComp();
	}
#endif
}

void AB2ActiveCameraActor::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	if (GIsEditor && GIsPlayInEditorWorld == false)
	{
		SyncFromEditingCompPlacement(); // 프리뷰 컴포넌트로 편집한 위치 싱크
	}
#endif
}

void AB2ActiveCameraActor::BeginDestroy()
{
	Super::BeginDestroy();

	RemoveComponentOverlapCallback(); // 꼭 할 필요가 있나..?

#if WITH_EDITOR
	// Need to manually destroy them..?
	DestroyEditingComponents(true);
#endif
}

// Called when the game starts or when spawned
void AB2ActiveCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
	check(ActiveCameraSpringArm && ActiveCameraComp);
	// 에디터에서 배치한 기준 위치로서.. Fixed 나 ScaledFollowPlayer 에서 사용.
	InitialActiveCameraSpringArmTransform = ActiveCameraSpringArm->GetComponentTransform();

	RegisterComponentOverlapCallback();

	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ManualActive)
		return;

	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_BeginStage)
		ForceBeginActiveCamera();
	else
		// 시작부터 플레이어와 접촉한 경우 바로 시작. 이 이후에 ABladeIIWorldSettings::BeginPlay 가 불린다면 ApplyDefaultPlayerCameraSetting 를 하지 않을 것.
		ManualTestBeginActiveCamera();
}

// Called every frame
void AB2ActiveCameraActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bIsActivated)
	{
		CheckForOutdatedInteraction();
		UpdateActiveCamera(DeltaTime);
	}
}

void AB2ActiveCameraActor::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	if (bDualCameraBlending)
	{		
		// 고정 카메라 이용
		if (CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
		{
			UCameraComponent* CamA = ActiveCameraComp;
			UCameraComponent* CamB = FindExtraCamCompForDualCameraBlending();

			if (!CamA || !CamB)
			{
				Super::CalcCamera(DeltaTime, OutResult);
			}
			else
			{
				// DualCameraBlending 의 실질적인 기능.

				FMinimalViewInfo ViewInfoA;
				CamA->GetCameraView(DeltaTime, ViewInfoA);
				FMinimalViewInfo ViewInfoB;
				CamB->GetCameraView(DeltaTime, ViewInfoB);

				const float BlendWeight = GetDualCameraBlendingWeight(); // B 로의 weight 가 되겠다.
				// OutResult 에는 ViewInfoA 와 ViewInfoB 를 섞음.
				OutResult = ViewInfoA;
				OutResult.BlendViewInfo(ViewInfoB, BlendWeight);

				CachedDualCameraBlendedYaw = OutResult.Rotation.Yaw; // 이건 입력 핸들링을 위해..
			}
		}
	}
	else
	{
		Super::CalcCamera(DeltaTime, OutResult);
	}
}

void AB2ActiveCameraActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//// 현재 액티브카메라 수동활성화된거면 오버랩 무시
	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (pGM)
	//{
	//	AB2ActiveCameraActor* pCurAcriveCamActor = pGM->GetCurrentActiveActiveCameraActor();

	//	if (pCurAcriveCamActor && pCurAcriveCamActor->GetTriggerMode() == EActiveCameraTriggerMode::EACTRG_ManualActive)
	//		return;
	//}

	//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));

	//// 플레이어 overlap 에서 발동.
	//if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ActorArea && B2PC &&
	//	B2Player && B2Player == UGameplayStatics::GetLocalPlayerCharacter(this))
	//{
	//	BeginActiveCamera(B2PC);
	//}
}

void AB2ActiveCameraActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	//// 현재 액티브카메라 수동활성화된거면 오버랩 무시
	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (pGM)
	//{
	//	AB2ActiveCameraActor* pCurAcriveCamActor = pGM->GetCurrentActiveActiveCameraActor();

	//	if (pCurAcriveCamActor && pCurAcriveCamActor->GetTriggerMode() == EActiveCameraTriggerMode::EACTRG_ManualActive)
	//		return;
	//}

	//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	//ABladeIIPlayer* B2LocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//// 플레이어가 떠나면 중단.
	//if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ActorArea &&
	//	B2Player && B2LocalPlayer && B2Player == B2LocalPlayer)
	//{
	//	EndActiveCamera();
	//}
}

void AB2ActiveCameraActor::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	//ABladeIIPlayer* B2LocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));

	//if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent &&
	//	B2PC && B2Player && B2LocalPlayer && B2Player == B2LocalPlayer)
	//{
	//	// BeginComponent/EndComponent 중 어느 것으로 등록한 것에 overlap 했는지 여부에 따라.
	//	if (IsCurrentlyActive())
	//	{
	//		UShapeComponent* SweepComp = Cast<UShapeComponent>(SweepResult.GetComponent());
	//		if (IsOneOfEndToggleComp(SweepComp))
	//		{
	//			EndActiveCamera();
	//		}
	//	}
	//	else
	//	{
	//		UShapeComponent* SweepComp = Cast<UShapeComponent>(SweepResult.GetComponent());
	//		if (IsOneOfBeginToggleComp(SweepComp))
	//		{
	//			BeginActiveCamera(B2PC);
	//		}
	//	}
	//}
}

void AB2ActiveCameraActor::OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	//ABladeIIPlayer* B2LocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));

	//if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent &&
	//	B2Player && B2LocalPlayer && B2Player == B2LocalPlayer)
	//{
	//	// 여기선 딱히 처리 안해도 될 듯.
	//}
}

void AB2ActiveCameraActor::RegisterComponentOverlapCallback()
{
	//// ShapeComponent 만 Overlap callback 에 등록
	//TArray<UActorComponent*> AllComps = GetComponentsByClass(UShapeComponent::StaticClass());
	//for (int32 CI = 0; CI < AllComps.Num(); ++CI)
	//{
	//	UShapeComponent* CurrComp = Cast<UShapeComponent>(AllComps[CI]);
	//	if (CurrComp)
	//	{
	//		CurrComp->OnComponentBeginOverlap.AddDynamic(this, &AB2ActiveCameraActor::OnComponentBeginOverlapCallback);
	//		CurrComp->OnComponentEndOverlap.AddDynamic(this, &AB2ActiveCameraActor::OnComponentEndOverlapCallback);
	//	}
	//}
}
void AB2ActiveCameraActor::RemoveComponentOverlapCallback()
{
	//// ShapeComponent 만
	//TArray<UActorComponent*> AllComps = GetComponentsByClass(UShapeComponent::StaticClass());
	//for (int32 CI = 0; CI < AllComps.Num(); ++CI)
	//{
	//	UShapeComponent* CurrComp = Cast<UShapeComponent>(AllComps[CI]);
	//	if (CurrComp)
	//	{
	//		CurrComp->OnComponentBeginOverlap.RemoveDynamic(this, &AB2ActiveCameraActor::OnComponentBeginOverlapCallback);
	//		CurrComp->OnComponentEndOverlap.RemoveDynamic(this, &AB2ActiveCameraActor::OnComponentEndOverlapCallback);
	//	}
	//}
}

bool AB2ActiveCameraActor::ManualTestBeginActiveCamera(float BlendInTimeOverride /*= -1.0f*/)
{
	//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ActorArea && B2PC && B2Player && IsOverlappingActor(B2Player))
	//{
	//	BeginActiveCamera(B2PC, BlendInTimeOverride);

	//	return true;
	//}

	//// 기타 조건이 추가될 수 있다. TriggerMode 가 EACTRG_ToggleByComponent 인 경우에 대해? 
	//// 하지만 그건 지속적인 overlap 이 아닌 순간의 touch 에 의해 toggle 되는 것인데 임의의 시점에 어떻게 판단하지?

	return false;
}

void AB2ActiveCameraActor::ForceBeginActiveCamera(float BlendInTimeOverride /*= -1.0f*/, AController* InOptionalInteractingController /*= NULL*/, bool bUseConditionalBeginTransitionInputDisable /*= true*/)
{
	//// 여타 조건 검사 없이 그냥 시작. 물론 특별한 경우에만 사용해야겠다.
	//ABladeIIPlayerController* B2LocalPC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (B2LocalPC)
	//{
	//	BeginActiveCamera(B2LocalPC, BlendInTimeOverride, InOptionalInteractingController, bUseConditionalBeginTransitionInputDisable);
	//}
}

void AB2ActiveCameraActor::CustomEndActiveCamera()
{
	EndActiveCamera(); // 일단 별다를 건 없군.
}

bool AB2ActiveCameraActor::IsBaseActiveCamera()
{
	return TriggerMode == EActiveCameraTriggerMode::EACTRG_BeginStage;
}

void AB2ActiveCameraActor::BeginActiveCamera(class ABladeIIPlayerController* InLocalPC, float BlendInTimeOverride /*= -1.0f*/, class AController* InOptionalInteractingController /*= NULL*/, bool bUseConditionalBeginTransitionInputDisable /*= true*/)
{
	check(ActiveCameraSpringArm && ActiveCameraComp);
	//check(InLocalPC && InLocalPC->IsValidObj());
	//	
	//// 우선, 기존에 활성화 된 것이 있다면 종료. 그래야 카메라 블렌딩이 안 꼬임.
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	//if (B2GM && !B2GM->IsAllowCameraWorking())
	//	return;

	//if (B2GM && B2GM->GetCurrentActiveActiveCameraActor() && B2GM->GetCurrentActiveActiveCameraActor() != this)
	//{
	//	B2GM->GetCurrentActiveActiveCameraActor()->CustomEndActiveCamera();
	//}

	//// 여기 인자를 통해서 BlendInTime 을 override 할 수 있는데 대체로 연출 종료와 같은 경우 외부에서 직접 적용 시도를 할 때에 사용할 의도.
	//const float FinalBlendTime = BlendInTimeOverride >= 0.0f ? BlendInTimeOverride : BlendInTime;

	//BeginForNewInteractionCommon(InLocalPC, FinalBlendTime, InOptionalInteractingController);

	//if(bUseConditionalBeginTransitionInputDisable)
	//	ConditionalBeginTransitionInputDisable(true);

	//bIsActivated = true;

	//// GameMode 에 등록
	//if (B2GM)
	//{
	//	B2GM->SetCurrentActiveActiveCameraActor(this);
	//}
}

void AB2ActiveCameraActor::BeginForNewInteractionCommon(class ABladeIIPlayerController* InLocalPC, float FinalBlendInTime, class AController* InOptionalInteractingController /*= NULL*/)
{
	//check(InLocalPC && InLocalPC->IsValidObj());
	//// InOptionalInteractingController 는 LocalPC 처럼 레벨 로딩된 내내 유지되는 애가 아니라서 invalid 할 가능성이 상당히 존재함.
	//checkSlow(!InOptionalInteractingController || InOptionalInteractingController->IsValidObj());

	//CachedLocalPC = InLocalPC;
	//CachedInteractingController = InOptionalInteractingController ? InOptionalInteractingController : CachedLocalPC; // Stage 게임 모드에서는 CachedLocalPC 와 CachedInteractingController 가 같을 것..
	//LastInteractBeginPlayer = Cast<ABladeIIPlayer>(CachedInteractingController->GetPawn()); // 이렇게 캐싱해서 나중에 체크만 하고 실제 사용은 하지 않는다.
	//
	//if (bDualCameraBlending && CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
	//{
	//	UCameraComponent* CamA = ActiveCameraComp;
	//	UCameraComponent* CamB = FindExtraCamCompForDualCameraBlending();
	//	if (CamA && CamB)
	//	{
	//		// 그냥 ActiveCameraComp 만 Fixed 로 사용하는 거랑 비슷할 수 있는데 설정 전달하고 하다 보면 다른 하나가 누락되든지 할 수도 있어서 특별한 요청이 아닌 한은 그냥 둠.
	//		// 이것도 ForceFromLastView를 해야 제대로 먹히네요.
	//		InLocalPC->SetViewTargetWithBlend(this, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic, 0, false, true);
	//		// 기타 Dual Blending 의 핵심 작동은 CalcCamera 에서.
	//	}
	//}
	//else
	//{
	//	if (CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
	//	{
	//		// CameraTargetingMode 도 Fixed 가 아니라면 rotation 은 바뀔 수 있다.
	//		ActiveCameraSpringArm->SetWorldTransform(InitialActiveCameraSpringArmTransform);
	//	}
	//	if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	//	{
	//		// FixedCameraTargetPosition 은 relative 로 취급.
	//		FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
	//		SetActiveCameraRotationToTarget(FixedTargetWorldTransform.GetTranslation());
	//	}

	//	if (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer && CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
	//	{
	//		// 기본적인 player top-down 카메라를 설정만 바꿔서 사용하는 모드가 됨. 거의 처음에만 세팅.
	//		SetupForConsistentPlayerTopDownCamera();
	//	}

	//	UCameraComponent* FinalCameraComponent = NULL;
	//	if (IsUsingActiveCameraComponent())
	//	{
	//		// 뭐가 문젠지 잘 모르겠는데 결국 ForceFromLastView 를 해야 제대로 먹히는군
	//		CachedLocalPC->SetViewTargetWithBlend(this, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic, 0, false, true);
	//		FinalCameraComponent = ActiveCameraComp;
	//	}
	//	else
	//	{
	//		if (LastInteractBeginPlayer)
	//		{
	//			// 아니라면 플레이어 top-down 카메라를 사용한다는 뜻이 되는데 같은 ViewTarget 간의 블렌딩이면 PlayerCameraManager 내에서 특수하게 처리해서 블렌딩 할 것.
	//			CachedLocalPC->SetViewTargetWithBlend(LastInteractBeginPlayer, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic);
	//			
	//			if (LastInteractBeginPlayer->HasActorBegunPlay())
	//			{
	//				FinalCameraComponent = LastInteractBeginPlayer->GetTopDownCameraComponent();
	//			}
	//		}
	//	}

	//	if (bOverrideFOVAngle && FinalCameraComponent) // 최종적으로 사용되는 CameraComponent 에다가 FOV 각도 override. EndActiveCamera 에서 돌려놓을 것.
	//	{
	//		FinalCameraComponent->FieldOfView = FOV_Override;
	//	}

	//	OverrideSpringArmSettings();
	//}
}

void AB2ActiveCameraActor::EndActiveCamera()
{
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM && !B2GM->IsAllowCameraWorking())
	//	return;
	//
	//ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	//if (CachedLocalPC && InteractLocalPlayer) // NULL 이면 이미 End 처리 되었거나 아님 뭔가 잘못되었거나. 두번 불릴 수는 있으므로 중복 처리하지 않도록.
	//{
	//	if (bRestoreCamera)
	//	{
	//		// 플레이어 기본 카메라로 back
	//		// Cubic 이외의 옵션이 필요하다면 변수로 노출
	//		if (bOverrideFOVAngle)
	//		{
	//			// Beginplay 때 캐싱해 놓은 값으로 돌아감.
	//			InteractLocalPlayer->GetTopDownCameraComponent()->FieldOfView = InteractLocalPlayer->GetTopDownCamIntialFOV();
	//		}
	//		RestoreSpringArmSettings();
	//		CachedLocalPC->ApplyWorldDefaultCamera(BlendOutTime, EViewTargetBlendFunction::VTBlend_Cubic);
	//	}

	//	// GameMode 등록 해제
	//	if (B2GM && B2GM->GetCurrentActiveActiveCameraActor() == this)
	//	{
	//		B2GM->SetCurrentActiveActiveCameraActor(NULL);
	//	}

	//	ConditionalBeginTransitionInputDisable(false);
	//}

	//CachedLocalPC = NULL;
	//CachedInteractingController = NULL;
	//LastInteractBeginPlayer = NULL;
	//CachedTargetMob = NULL;
	//
	//bIsActivated = false;
}

void CopyFromSelectedSettingsToSpringArmComponent(FActiveCameraSpringArmSelectedSettings& FromSettings, USpringArmComponent* ToComponent)
{
	if (ToComponent)
	{
		ToComponent->bEnableCameraLag = FromSettings.bEnableCameraLag;
		ToComponent->bEnableCameraRotationLag = FromSettings.bEnableCameraRotationLag;
		ToComponent->CameraLagSpeed = FromSettings.CameraLagSpeed;
		ToComponent->CameraRotationLagSpeed = FromSettings.CameraRotationLagSpeed;
		ToComponent->CameraLagMaxDistance = FromSettings.CameraLagMaxDistance;
	}
}

void CopyFromSpringArmComponentToSelectedSettings(USpringArmComponent* FromComponent, FActiveCameraSpringArmSelectedSettings& ToSettings)
{
	if (FromComponent)
	{
		ToSettings.bEnableCameraLag = FromComponent->bEnableCameraLag;
		ToSettings.bEnableCameraRotationLag = FromComponent->bEnableCameraRotationLag;
		ToSettings.CameraLagSpeed = FromComponent->CameraLagSpeed;
		ToSettings.CameraRotationLagSpeed = FromComponent->CameraRotationLagSpeed;
		ToSettings.CameraLagMaxDistance = FromComponent->CameraLagMaxDistance;
	}
}

void AB2ActiveCameraActor::OverrideSpringArmSettings()
{
	if (bOverrideSpringArmSettings)
	{
		if (IsUsingActiveCameraComponent())
		{
			CopyFromSelectedSettingsToSpringArmComponent(SpringArmSettings, ActiveCameraSpringArm);
		}
		else if (CachedInteractingController)
		{
			ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
			if (InteractLocalPlayer)
			{// 플레이어 top-down 카메라의 경우는 추후 속성값들을 되돌리기 위해 현재 값을 캐싱해 둔다.
				CopyFromSpringArmComponentToSelectedSettings(InteractLocalPlayer->GetCameraBoom(), PlayerCameraBoomSettingBackup);
				CopyFromSelectedSettingsToSpringArmComponent(SpringArmSettings, InteractLocalPlayer->GetCameraBoom());
			}
		}
	}
}
void AB2ActiveCameraActor::RestoreSpringArmSettings()
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	// 플레이어 캐릭터에 달린 Top-down 카메라만 대상임. 여기에서 보유하고 있는 ActiveCameraComp 는 굳이 되돌릴 필요가 없지..
	if (bOverrideSpringArmSettings && IsUsingActiveCameraComponent() == false && InteractLocalPlayer)
	{
		CopyFromSelectedSettingsToSpringArmComponent(PlayerCameraBoomSettingBackup, InteractLocalPlayer->GetCameraBoom());
	}
}

void AB2ActiveCameraActor::CheckForOutdatedInteraction()
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	if (InteractLocalPlayer && InteractLocalPlayer != LastInteractBeginPlayer)
	{
		// 이건 대략 어떤 경우냐면, ActiveCamera 영역 안에서 태그를 했다거나 해서 이걸 트리거한 BladeIIPlayer 가 Destroy 되고 새로 spawn 된 경우.
		// 그것도 실질적으로 ActorArea로 트리거 하는 경우라면 Overlap 감지로 인해 BeginActiveCamera 가 되었을 거고 ToggleByComponent 로 트리거 하는 경우 여기로 오게 될 것.
		// ActorArea 로 트리거 하는 경우도 이걸로 처리할 수 있다면 그게 더 좋긴 함.
		BeginForNewInteractionCommon(CachedLocalPC, 0.0f, CachedInteractingController);
	}
}

void AB2ActiveCameraActor::UpdateActiveCamera(float DeltaSeconds)
{
	//ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	//if (InteractLocalPlayer && CachedLocalPC)
	//{
	//	bool bFallbackToWorldDefault = false; // 계산 도중 구리면 이걸 true 로 세팅.

	//	if (IsUsingActiveCameraComponent())
	//	{
	//		float ExpectedPlayerInputHandlingYaw = ActiveCameraSpringArm->GetComponentRotation().Yaw; // 일반적인 ActiveCameraComponent 사용하는 상황에서의 입력 핸들링 기본축 값.
	//		
	//		if (bDualCameraBlending)
	//		{
	//			ExpectedPlayerInputHandlingYaw = CachedDualCameraBlendedYaw;
	//			// 기타 딱히 여기서 필요한 건 없을 듯.. 카메라 자체는 걍 고정이라.
	//		}
	//		else
	//		{
	//			if (CameraMovementMode == EActiveCameraMovementMode::EACMM_ScaledFollowPlayer)
	//			{
	//				FVector BasePosToPlayer = InteractLocalPlayer->GetActorLocation() - InitialActiveCameraSpringArmTransform.GetTranslation();
	//				float PlayerToBaseDistance = BasePosToPlayer.Size();
	//				BasePosToPlayer.Normalize();

	//				// CameraPlayerFollowingScale 만큼 스케일해서 플레이어 위치를 따라가도록
	//				FVector ScaledCameraLocation = InitialActiveCameraSpringArmTransform.GetTranslation() + BasePosToPlayer * CameraPlayerFollowingScale * PlayerToBaseDistance;
	//				ActiveCameraSpringArm->SetWorldLocation(ScaledCameraLocation);
	//			}
	//			// CameraMovementMode 가 FollowPlayer 이거나 Fixed 면 여기서 처리 안 함.

	//			if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
	//			{
	//				SetActiveCameraRotationToTarget(InteractLocalPlayer->GetActorLocation());
	//			}
	//			else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob)
	//			{
	//				if (CachedTargetMob == NULL) // Begin 시점에 없을 수도 있으므로 Update 에서 검사
	//				{
	//					CachedTargetMob = GetDesiredCameraTargetWaveMob();
	//				}
	//				if (CachedTargetMob && CachedTargetMob->IsPendingKill() == false)
	//				{
	//					SetActiveCameraRotationToTarget(CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset);
	//				}
	//				else
	//				{
	//					// 몹이 없는 상황이라면 기본 카메라로 폴백.
	//					bFallbackToWorldDefault = true;
	//				}
	//			}
	//			else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	//			{
	//				// FixedCameraTargetPosition 은 relative 로 취급.
	//				FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
	//				SetActiveCameraRotationToTarget(FixedTargetWorldTransform.GetTranslation());
	//			}
	//		}

	//		// 플레이어 movement 입력 처리를 위해 CameraBoom 을 여하간 회전시킨다. ABladeIIPlayer::MoveForward/MoveRight 참고
	//		// ActiveCameraComp 의 Yaw 를 직빵으로 대입하면 yaw 가 급격하게 변하는 일부 상황에서 문제가 되므로 보간을 좀 하는데 PlayerMoveInputAdjustBoomSyncAlpha 값이 낮을 수록 목표값에서 멀어질 것임.
	//		// 그리고 가끔은 각도가 사실상 변하지 않는 상황에서 쓸데없이 보간을 하기도 해서.. 가능하면 PlayerMoveInputAdjustBoomSyncAlpha 를 1.0 으로..
	//		float InterpolatedYaw = 
	//			bUsePlacedActiveCamYawAsInputBase ? InitialActiveCameraSpringArmTransform.Rotator().Yaw : // 그래서.. yaw 가 급격하게 변하는 상황에 대한 고육지책.
	//			FMath::Lerp(InteractLocalPlayer->
	//			GetCameraYaw(), ExpectedPlayerInputHandlingYaw, PlayerMoveInputAdjustBoomSyncAlpha);
	//		InteractLocalPlayer->UpdateCameraYaw(InterpolatedYaw);
	//	}
	//	else
	//	{
	//		// 플레이어 top-down 카메라의 CameraBoom 을 조작. BladeIIPlayerController 쪽에 구비해 둔 인터페이스를 통해서.
	//		// CameraMovementMode 는 어차피 FollowPlayer 일 테고 (즉, 건드릴 필요 없음) CameraTarget 에 따라 CameraBoom 의 rotation 을 조작한다.
	//		if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
	//		{
	//			// 이 경우는 거의 기존 top-down 카메라와 차이가 없을 것인데, PlayerTopDownCameraSettings 에 의한 초기 세팅 뿐?

	//			// 탑다운 카메라 이용한 듀얼블렌딩시
	//			if (bDualCameraBlending)
	//			{
	//				if (CachedLocalPC)
	//				{
	//					// PlayerTopDownCameraSettings 의 각 요소별로 override 한 것만 적용
	//					if (PlayerTopDownCameraSettings.bOverrideCameraBoomPitch)
	//					{
	//						float fPitchA = PlayerTopDownCameraSettings.CameraBoomPitch;
	//						float fPitchB = DCBExtraPlayerTopDownCameraSettings.CameraBoomPitch;
	//						float fBlendedPitch = fPitchA + ((fPitchB - fPitchA) * GetDualCameraBlendingWeight());
	//						CachedLocalPC->SetCameraBoomPitch(fBlendedPitch);
	//					}

	//					if (PlayerTopDownCameraSettings.bOverrideCameraBoomYaw)
	//					{
	//						float fYawA = PlayerTopDownCameraSettings.CameraBoomYaw;
	//						float fYawB = DCBExtraPlayerTopDownCameraSettings.CameraBoomYaw;
	//						float fBlendedYaw = fYawA + ((fYawB - fYawA) * GetDualCameraBlendingWeight());
	//						CachedLocalPC->SetCameraBoomYaw(fBlendedYaw);
	//					}

	//					if (PlayerTopDownCameraSettings.bOverrideCameraBoomRoll)
	//					{
	//						float fRollA = PlayerTopDownCameraSettings.CameraBoomRoll;
	//						float fRollB = DCBExtraPlayerTopDownCameraSettings.CameraBoomRoll;
	//						float fBlendedRoll = fRollA + ((fRollB - fRollA) * GetDualCameraBlendingWeight());
	//						CachedLocalPC->SetCameraBoomRoll(fBlendedRoll);
	//					}

	//					if (PlayerTopDownCameraSettings.bOverrideCameraBoomArmLength)
	//					{
	//						float fArmLengthA = GetExtraAppliedCameraDistance(PlayerTopDownCameraSettings.CameraBoomArmLength, PlayerTopDownCameraSettings.CameraBoomArmLength_Extra);
	//						float fArmLengthB = GetExtraAppliedCameraDistance(DCBExtraPlayerTopDownCameraSettings.CameraBoomArmLength, DCBExtraPlayerTopDownCameraSettings.CameraBoomArmLength_Extra);
	//						float fBlendedArmLength = fArmLengthA + ((fArmLengthB - fArmLengthA) * GetDualCameraBlendingWeight());
	//						CachedLocalPC->SetCameraBoomArmLength(fBlendedArmLength);
	//					}

	//					if (PlayerTopDownCameraSettings.bOverrideCameraTargetFocusOffset)
	//					{
	//						FVector OffsetA = PlayerTopDownCameraSettings.CameraTargetFocusOffset;
	//						FVector OffsetB = DCBExtraPlayerTopDownCameraSettings.CameraTargetFocusOffset;
	//						FVector Offset = OffsetA + ((OffsetB - OffsetA) * GetDualCameraBlendingWeight());
	//						CachedLocalPC->SetCameraTargetFocusOffset(Offset);
	//					}

	//					if (bOverrideFOVAngle && CachedInteractingController)
	//					{
	//						float fFovA = FOV_Override;
	//						float fFovB = DCBExtraFOV_Override;
	//						float fBlendedFov = fFovA + ((fFovB - fFovA) * GetDualCameraBlendingWeight());
	//						ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(CachedInteractingController->GetPawn());

	//						if (pPlayer)
	//							pPlayer->GetTopDownCameraComponent()->FieldOfView = fBlendedFov;
	//					}

	//					// 카메라 로테이션 조작
	//					FRotator CamRotation = InteractLocalPlayer->TopDownCameraComponent->GetRelativeTransform().GetRotation().Rotator();

	//					if (PlayerTopDownCameraSettings.bOverrideCameraPitch)
	//					{
	//						float fPitchA = PlayerTopDownCameraSettings.CameraPitch;
	//						float fPitchB = DCBExtraPlayerTopDownCameraSettings.CameraPitch;
	//						float fBlendedPitch = fPitchA + ((fPitchB - fPitchA) * GetDualCameraBlendingWeight());
	//						CamRotation.Pitch = CamRotation.Pitch + fBlendedPitch;
	//					}

	//					if (PlayerTopDownCameraSettings.bOverrideCameraYaw)
	//					{
	//						float fYawA = PlayerTopDownCameraSettings.CameraYaw;
	//						float fYawB = DCBExtraPlayerTopDownCameraSettings.CameraYaw;
	//						float fBlendedYaw = fYawA + ((fYawB - fYawA) * GetDualCameraBlendingWeight());
	//						CamRotation.Yaw = CamRotation.Yaw + fBlendedYaw;
	//					}

	//					InteractLocalPlayer->TopDownCameraComponent->SetRelativeRotation(CamRotation);
	//				}					
	//			}
	//		}
	//		else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob)
	//		{
	//			if (CachedTargetMob == NULL) // Begin 시점에 없을 수도 있으므로 Update 에서 검사
	//			{
	//				CachedTargetMob = GetDesiredCameraTargetWaveMob();
	//			}

	//			if (CachedTargetMob && CachedTargetMob->IsPendingKill() == false)
	//			{
	//				SetPlayerCameraBoomRotationToTarget(
	//					CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset,
	//					// 여기엔 개별 파라미터에 대한 offset 과 overriding 이 좀 있음.
	//					TargetMobCameraAdjustSettings.BoomRotOffsetPitch, TargetMobCameraAdjustSettings.BoomRotOffsetYaw,
	//					(TargetMobCameraAdjustSettings.bOverrideCameraBoomArmLength ? 
	//						GetExtraAppliedCameraDistance(TargetMobCameraAdjustSettings.CameraBoomArmLength, TargetMobCameraAdjustSettings.CameraBoomArmLength_Extra)
	//						: 0.0f)
	//					);

	//				// 몹 쳐다보게..
	//				SetPlayerCameraRotationToTarget(
	//					CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset,
	//					TargetMobCameraAdjustSettings.CamRotOffsetPitch, TargetMobCameraAdjustSettings.CamRotOffsetYaw
	//					);
	//				
	//			}
	//			else
	//			{
	//				// 몹이 없는 상황이라면 기본 카메라로 폴백.
	//				//bFallbackToWorldDefault = true;
	//			}
	//		}
	//		else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	//		{
	//			// FixedCameraTargetPosition 은 relative 로 취급.
	//			FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
	//			SetPlayerCameraBoomRotationToTarget(FixedTargetWorldTransform.GetTranslation());

	//			// CameraMovementMode 가 FollowPlayer 면서 TargetingMode 가 Fixed 인 경우 FixedCameraTargetPosition 위치에 플레이어가 있을 경우 카메라가 불안정하게 됨.
	//			// 아예 해당 조합을 사용하지 않는 걸로 할 수도 있지만, FixedCameraTargetPosition 을 잘 세팅해서 사용해 볼 수도.
	//		}
	//	}

	//	if (bFallbackToWorldDefault)
	//	{
	//		CachedLocalPC->ApplyWorldDefaultCamera(0.0f);
	//	}
	//}
}

void AB2ActiveCameraActor::SetupForConsistentPlayerTopDownCamera()
{
	//if (CachedLocalPC)
	//{
	//	// PlayerTopDownCameraSettings 의 각 요소별로 override 한 것만 적용
	//	CachedLocalPC->SetCameraBoomLockedViewTarget(PlayerTopDownCameraSettings.bCameraBoomLockedViewTarget);

	//	if (PlayerTopDownCameraSettings.bOverrideCameraBoomPitch)
	//	{
	//		CachedLocalPC->SetCameraBoomPitch(PlayerTopDownCameraSettings.CameraBoomPitch);
	//	}

	//	if (PlayerTopDownCameraSettings.bOverrideCameraBoomYaw)
	//	{
	//		CachedLocalPC->SetCameraBoomYaw(PlayerTopDownCameraSettings.CameraBoomYaw);
	//	}

	//	if (PlayerTopDownCameraSettings.bOverrideCameraBoomRoll)
	//	{
	//		CachedLocalPC->SetCameraBoomRoll(PlayerTopDownCameraSettings.CameraBoomRoll);
	//	}

	//	if (PlayerTopDownCameraSettings.bOverrideCameraBoomArmLength)
	//	{
	//		CachedLocalPC->SetCameraBoomArmLength(GetExtraAppliedCameraDistance(
	//			PlayerTopDownCameraSettings.CameraBoomArmLength, PlayerTopDownCameraSettings.CameraBoomArmLength_Extra));
	//	}

	//	if (PlayerTopDownCameraSettings.bOverrideCameraTargetFocusOffset)
	//	{
	//		CachedLocalPC->SetCameraTargetFocusOffset(PlayerTopDownCameraSettings.CameraTargetFocusOffset);
	//	}

	//	
	//	// 카메라 로테이션 조작
	//	if (ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter())
	//	{
	//		FRotator CamRotation = InteractLocalPlayer->TopDownCameraComponent->GetRelativeTransform().GetRotation().Rotator();

	//		if (PlayerTopDownCameraSettings.bOverrideCameraPitch)
	//			CamRotation.Pitch = CamRotation.Pitch + PlayerTopDownCameraSettings.CameraPitch;

	//		if (PlayerTopDownCameraSettings.bOverrideCameraYaw)
	//			CamRotation.Yaw = CamRotation.Yaw + PlayerTopDownCameraSettings.CameraYaw;

	//		//ActiveCameraSpringArm->SetWorldRotation(CamRotation);
	//		InteractLocalPlayer->TopDownCameraComponent->SetRelativeRotation(CamRotation);

	//		// SetViewTargetWithBlend 는 이 이후에 따로 불려야 함.
	//	}
	//}
}

bool AB2ActiveCameraActor::IsUsingActiveCameraComponent()
{
	if (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer)
	{
		// 일단, FollowPlayer 모드에서는 플레이어 top-down 카메라를 사용한다는 가정인데, 만들면서 바뀔 수도 있다.
		return false;
	}
	return true;
}

void AB2ActiveCameraActor::SetActiveCameraRotationToTarget(FVector InViewTargetLocation)
{
	FVector ToTarget = (InViewTargetLocation - ActiveCameraSpringArm->GetComponentLocation());
	ToTarget.Normalize();
	// 실제로는 SpringArm 을 조절.
	ActiveCameraSpringArm->SetWorldRotation(ToTarget.Rotation());
}

void AB2ActiveCameraActor::SetPlayerCameraBoomRotationToTarget(FVector InViewTargetLocation, float RotOffsetPitch /*= 0.0f*/, float RotOffsetYaw /*= 0.0f*/, float ArmLengthOverride /*= 0.0f*/)
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	if (InteractLocalPlayer)
	{
		FVector ToTarget = (InViewTargetLocation - InteractLocalPlayer->GetActorLocation());
		ToTarget.Normalize();
		FRotator CamBoomRotation = ToTarget.Rotation();

		// RotOffset 은 그냥 가함
		CamBoomRotation.Pitch = CamBoomRotation.Pitch + RotOffsetPitch;

		// Boom Ptich 상항선 하한선
		if (bUseCameraBoomRotationClamp)
			CamBoomRotation.Pitch = FMath::Clamp<float>(CamBoomRotation.Pitch, MinCameraBoomRotationClamp, MaxCameraBoomRotationClamp);
		
		InteractLocalPlayer->UpdateCameraPitch(CamBoomRotation.Pitch);
		InteractLocalPlayer->UpdateCameraYaw(CamBoomRotation.Yaw + RotOffsetYaw);
		if (ArmLengthOverride > 0.0f) // ArmLengthOverride 는 0 보다 클 때만
		{
			InteractLocalPlayer->UpdateCameraDistance(ArmLengthOverride);
		}
	}
}

void AB2ActiveCameraActor::SetPlayerCameraRotationToTarget(FVector InViewTargetLocation, float RotOffsetPitch /*= 0.0f*/, float RotOffsetYaw /*= 0.0f*/)
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	if (InteractLocalPlayer)
	{
		if (InteractLocalPlayer->TopDownCameraComponent)
		{
			FVector ToTarget = (InViewTargetLocation - InteractLocalPlayer->TopDownCameraComponent->GetComponentLocation());
			ToTarget.Normalize();
			FRotator CamBoomRotation = ToTarget.Rotation();

			CamBoomRotation.Pitch = CamBoomRotation.Pitch + RotOffsetPitch;
			CamBoomRotation.Yaw = CamBoomRotation.Yaw + RotOffsetYaw;

			InteractLocalPlayer->TopDownCameraComponent->SetWorldRotation(CamBoomRotation);
		}		
	}
}

ABladeIICharacter* AB2ActiveCameraActor::GetDesiredCameraTargetWaveMob()
{
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	AB2MonsterSpawnPool* TargetMobSP = B2GM ? B2GM->GetActiveSpawnPool() : NULL; // 레벨 시작 시 정해짐.

	if (TargetMobSP)
	{
		int32 WaveNum = 0;
		int32 MobIndex = 0;
		ParseForWaveMobSelectKeyword(TargetMobSelectKeyword, WaveNum, MobIndex); // 문자열로 지정한 옵션에서 번호들 빼옴.

		TArray<ABladeIICharacter*> InterestedWaveMobs;
		TargetMobSP->GetWaveMobList(WaveNum, InterestedWaveMobs, false); // 죽어가는 녀석도 포함해서 가져오지만 여기서 호출하는 타이밍에 그게 될까..?

		for (int32 WMI = 0; WMI < InterestedWaveMobs.Num(); ++WMI)
		{
			ABladeIICharacter* CurrMob = InterestedWaveMobs[WMI];
			if (CurrMob)
			{
				if (CurrMob->GetSpawnPoolWaveObjIndex() == MobIndex) // 사실상 Wave 가 시작된 상황이 아니면 찾는 인덱스의 mob 이 없을 수도 있다.
				{
					return CurrMob;
				}
			}
		}
	}
	return NULL;
}

void AB2ActiveCameraActor::ConditionalBeginTransitionInputDisable(bool bTransitIn)
{
	//ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();

	//// 카메라 워크 변동에 따라 플레이어 이동 입력에 혼선이 올 수 있으므로 일정 시간 동안 입력을 막는다.
	//// 플레이어 Top-down 카메라를 사용하는 모드에선 필요없을 듯.. 했는데 movement, target 모두 player 가 아닌 이상은 필요할 듯.
	//// 듀얼카메라 모드에서는 사용
	//if ((bDualCameraBlending || CameraMovementMode != EActiveCameraMovementMode::EACMM_FollowPlayer || CameraTargetingMode != EActiveCameraTargetingMode::EACTM_Player) &&
	//	TransitionInputDisableTime > 0.0f && CachedLocalPC && InteractLocalPlayer)
	//{
	//	if (bTransitIn)
	//	{
	//		CachedLocalPC->bDisableMoveInputForActiveCameraTransition = true;
	//		bTransitInInputDisabled = true;
	//		GetWorldTimerManager().SetTimer(TransitInTimeTimerHandle, this, &AB2ActiveCameraActor::TransitInTimerCB, TransitionInputDisableTime, false);

	//		if (TransitionAutoMoveScale > 0.0f)
	//		{
	//			// 입력이 비활성화 된 동안 현재 위치에서 약간 앞으로 이동시킨다. 경계에서 좀 멀어지도록 ㅋ
	//							
	//			const float MoveDistance = InteractLocalPlayer->GetDefaultHalfHeight() * 2.0f * TransitionAutoMoveScale;
	//			const FVector MoveTargetLocation = InteractLocalPlayer->GetActorLocation() + (InteractLocalPlayer->GetVelocity().GetSafeNormal() * MoveDistance);
	//			
	//			UNavigationSystem::SimpleMoveToLocation(CachedLocalPC, MoveTargetLocation);
	//		}
	//	}
	//	else
	//	{
	//		// 혹시라도 TransitIn 도중이라면 중단
	//		if (bTransitInInputDisabled)
	//		{
	//			GetWorldTimerManager().ClearTimer(TransitInTimeTimerHandle);
	//			bTransitInInputDisabled = false;
	//		}

	//		CachedLocalPC->bDisableMoveInputForActiveCameraTransition = true;
	//		bTransitOutInputDisabled = true;
	//		GetWorldTimerManager().SetTimer(TransitOutTimeTimerHandle, this, &AB2ActiveCameraActor::TransitOutTimerCB, TransitionInputDisableTime, false);
	//	}
	//}
}

void AB2ActiveCameraActor::TransitInTimerCB()
{
	//GetWorldTimerManager().ClearTimer(TransitInTimeTimerHandle);
	//// CachedLocalPC 가 없더라도 여하간 bDisableMoveInput 은 처리해야..
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (B2PC)
	//{
	//	B2PC->bDisableMoveInputForActiveCameraTransition = false;
	//}
	//bTransitInInputDisabled = false;
}

void AB2ActiveCameraActor::TransitOutTimerCB()
{
	//GetWorldTimerManager().ClearTimer(TransitOutTimeTimerHandle);
	//// CachedLocalPC 가 없더라도 여하간 bDisableMoveInput 은 처리해야..
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (B2PC)
	//{
	//	B2PC->bDisableMoveInputForActiveCameraTransition = false;
	//}
	//bTransitOutInputDisabled = false;
}

bool AB2ActiveCameraActor::IsOneOfBeginToggleComp(class UShapeComponent* InTestComp)
{
	if (InTestComp == NULL)
	{
		return false;
	}

	for (int32 BCI = 0; BCI < ToggleMode_BeginComponents.Num(); ++BCI)
	{
		if (InTestComp->GetName() == ToggleMode_BeginComponents[BCI].ToString())
		{
			return true;
		}
	}

	return false;
}

bool AB2ActiveCameraActor::IsOneOfEndToggleComp(class UShapeComponent* InTestComp)
{
	if (InTestComp == NULL)
	{
		return false;
	}

	for (int32 BCI = 0; BCI < ToggleMode_EndComponents.Num(); ++BCI)
	{
		if (InTestComp->GetName() == ToggleMode_EndComponents[BCI].ToString())
		{
			return true;
		}
	}

	return false;
}

UCameraComponent* AB2ActiveCameraActor::FindExtraCamCompForDualCameraBlending()
{
	TInlineComponentArray<UCameraComponent*> AllCameras;
	GetComponents<UCameraComponent>(AllCameras);

	for (UCameraComponent* ThisCam : AllCameras)
	{
		if (ThisCam && ThisCam->GetFName() == DCBExtraCamName)
		{
			return ThisCam;
		}
	}
	return NULL;
}

USceneComponent* AB2ActiveCameraActor::FindSceneComponentForDualCameraBlending(FName CompName)
{
	TInlineComponentArray<USceneComponent*> AllComponents;
	GetComponents<USceneComponent>(AllComponents);

	for (USceneComponent* ThisComp : AllComponents)
	{
		if (ThisComp && ThisComp->GetFName() == CompName)
		{
			return ThisComp;
		}
	}
	return NULL;
}

float AB2ActiveCameraActor::GetDualCameraBlendingWeight()
{
	//UCameraComponent* CamA = ActiveCameraComp;
	//UCameraComponent* CamB = FindExtraCamCompForDualCameraBlending();
	//ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();

	//if (CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed && (!CamA || !CamB))
	//	return 0.0f;

	//if (InteractLocalPlayer && InteractLocalPlayer->IsValidObj())
	//{
	//	USceneComponent* FlagA = NULL;
	//	USceneComponent* FlagB = NULL;

	//	if (DCBFlagName != NAME_None)
	//		FlagA = FindSceneComponentForDualCameraBlending(DCBFlagName);

	//	if (DCBExtraFlagName != NAME_None)
	//		FlagB = FindSceneComponentForDualCameraBlending(DCBExtraFlagName);
	//	
	//	//보간 시작 기준점
	//	FVector vPointA;
	//	//보간 종료 기준점
	//	FVector vPointB;

	//	// 플래그 설정되었으면 그위치로 하고 아니면 카메라위치 이용함
	//	if (FlagA)
	//		vPointA = FlagA->GetComponentLocation();
	//	else
	//		vPointA = CamA->GetComponentLocation();
	//			
	//	if (FlagB)
	//		vPointB = FlagB->GetComponentLocation();
	//	else
	//		vPointB = CamB->GetComponentLocation();

	//	// 부자연스러워서 케릭터 위치값을 A, B 두 지점사이로 보정한다(내적 이용).
	//	FVector vGoalPos = vPointB - vPointA;
	//	FVector vCurPos = InteractLocalPlayer->GetActorLocation() - vPointA;

	//	// 보정된 현재위치 벡터 사이즈
	//	float fCurPosSize = FVector::DotProduct(vGoalPos.GetSafeNormal(KINDA_SMALL_NUMBER), vCurPos);
	//			
	//	float fWeight = fCurPosSize / vGoalPos.Size();

	//	if (fWeight < KINDA_SMALL_NUMBER) // 흔한 설정에서 나타날 수 있는 상황은 아니겠지만..
	//		return 0.0f;

	//	if (fWeight >= 1.0f) // 혹시 1넘는게 필요하면 그때 제거
	//		return 1.0f;

	//	return fWeight;
	//}

	return 0.0f;
}

void AB2ActiveCameraActor::ChangeCameraOwnerPlayer(ABladeIIPlayer* targetPlayer, float OverrideBlendingTime)
{	
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));

	//if (B2PC && targetPlayer && targetPlayer->IsAlive())
	//{
	//	ABladeIIWorldSettings* B2WS = GetWorld() ? Cast<ABladeIIWorldSettings>(GetWorld()->GetWorldSettings()) : NULL;

	//	if (B2WS)
	//	{
	//		B2WS->ApplyDefaultPlayerCameraSettingForPlayerPawn(targetPlayer);

	//		if (PlayerTopDownCameraSettings.bOverrideCameraBoomPitch)
	//			targetPlayer->UpdateCameraPitch(PlayerTopDownCameraSettings.CameraBoomPitch);

	//		if (PlayerTopDownCameraSettings.bOverrideCameraBoomYaw)
	//			targetPlayer->UpdateCameraYaw(PlayerTopDownCameraSettings.CameraBoomYaw);

	//		if (PlayerTopDownCameraSettings.bOverrideCameraBoomRoll)
	//			targetPlayer->UpdateCameraRoll(PlayerTopDownCameraSettings.CameraBoomRoll);

	//		if (PlayerTopDownCameraSettings.bOverrideCameraBoomArmLength)
	//			targetPlayer->UpdateCameraDistance(GetExtraAppliedCameraDistance(
	//				PlayerTopDownCameraSettings.CameraBoomArmLength, PlayerTopDownCameraSettings.CameraBoomArmLength_Extra));

	//		if (PlayerTopDownCameraSettings.bOverrideCameraTargetFocusOffset)
	//			targetPlayer->UpdateCameraTargetFocusOffset(PlayerTopDownCameraSettings.CameraTargetFocusOffset);
	//	}

	//	//활성화 한번 해줌
	//	ForceBeginActiveCamera();
	//	float LastBlendingTime = OverrideBlendingTime >= 0.0f ? OverrideBlendingTime : BlendInTime;
	//	B2PC->SetViewTargetWithBlend(targetPlayer, LastBlendingTime, EViewTargetBlendFunction::VTBlend_Cubic, 0, true, true);
	//}
}

#if WITH_EDITOR
void AB2ActiveCameraActor::RefreshEditingComp(bool bRegenerateStaticComp /*= true*/)
{
	DestroyEditingComponents(bRegenerateStaticComp);

	//if(CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	//{
	//	UStaticMesh* EditingCompMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/BasicShapes/Cone.Cone"));
	//	// TEXT("/Engine/BasicShapes/Cone.Cone")
	//	if(EditingCompMesh)
	//	{
	//		UStaticMeshComponent* EditingComp = NULL;

	//		// 새로 생성하는 옵션이 아니라면 기존 거 사용
	//		if(TargetPositionEditingComp.Num() == 0)
	//		{
	//			EditingComp = NewObject<UStaticMeshComponent>(this, FName(TEXT("TargetPositionEditingComp")), RF_Transient);
	//			if(EditingComp)
	//			{
	//				TargetPositionEditingComp.Add(EditingComp);
	//			}
	//		}
	//		else
	//		{
	//			EditingComp = TargetPositionEditingComp[0];
	//		}

	//		if(EditingComp)
	//		{
	//			EditingComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)); // EditingComp 의 RelativeLocation 을 사용할 것이므로 반드시 root 에 attach
	//			EditingComp->RegisterComponent();
	//			EditingComp->SetStaticMesh(EditingCompMesh);
	//			EditingComp->SetVisibility(true);
	//			// 어차피 게임에선 생기지도 않을 애들이지만 
	//			EditingComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//			EditingComp->SetHiddenInGame(true);
	//			EditingComp->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")));
	//			
	//			// Movement, Target 모두 Fixed 이면 굳이 따로 CameraTargetPosition 을 줄 거 없이 ActiveCameraComp 의 배치된 위치 및 회전값을 그대로 사용하면 되겠지..
	//			if(CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
	//			{					
	//				EditingComp->SetWorldLocation(ActiveCameraSpringArm->GetComponentTransform().TransformPosition(FVector(300.0f, 0.0f, 0.0f)));
	//				FixedCameraTargetPosition = EditingComp->RelativeLocation;
	//			}
	//			else
	//			{
	//				EditingComp->SetRelativeLocation(FixedCameraTargetPosition); // FixedCameraTargetPosition 은 relative 취급
	//			}
	//		}
	//	}
	//}

	//// ShapeComponent 들에 대한 안내텍스트들 생성. ToggleMode_BeginComponents, ToggleMode_EndComponents 에 등록한 것들은 특별히 추가 정보..
	//TArray<UActorComponent*> ShapeComps = GetComponentsByClass(UShapeComponent::StaticClass());
	//for (int32 SCI = 0; SCI < ShapeComps.Num(); ++SCI)
	//{
	//	UShapeComponent* CurrShapeComp = Cast<UShapeComponent>(ShapeComps[SCI]);
	//	if (CurrShapeComp == NULL)
	//	{
	//		continue;
	//	}

	//	UTextRenderComponent* NewInfoTextComp = NewObject<UTextRenderComponent>(this, *FString::Printf(TEXT("ShapeCompInfoText_%d"), SCI), RF_Transient);
	//	if (NewInfoTextComp == NULL)
	//	{
	//		continue;
	//	}

	//	NewInfoTextComp->AttachToComponent(CurrShapeComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//	NewInfoTextComp->RegisterComponent();
	//	NewInfoTextComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	NewInfoTextComp->SetVisibility(true);
	//	NewInfoTextComp->SetHiddenInGame(true);
	//	NewInfoTextComp->SetHorizontalAlignment(EHTA_Center);
	//	NewInfoTextComp->SetWorldSize(30.0f);
	//	NewInfoTextComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	//	NewInfoTextComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // 위치는 적당히 맞춰준다
	//	
	//	const bool bIsOneOfBeginToggleComp = IsOneOfBeginToggleComp(CurrShapeComp);
	//	const bool bIsOneOfEndToggleComp = IsOneOfEndToggleComp(CurrShapeComp);

	//	// Component 용도 따라 다르게 표현할 정보들.
	//	FColor CompTextColor(255, 0, 0);
	//	FString CompInfoStr = CurrShapeComp->GetName() + TEXT("\n");

	//	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent)
	//	{
	//		if (bIsOneOfBeginToggleComp && bIsOneOfEndToggleComp)
	//		{
	//			CompInfoStr += TEXT("WRONG Setting!!"); // Begin/End 둘 다 동시에 세팅된 건 잘못임.
	//			CompTextColor = FColor(255, 0, 127);
	//		}
	//		else
	//		{
	//			if (bIsOneOfBeginToggleComp)
	//			{
	//				CompInfoStr += TEXT("Begin Toggle");
	//				CompTextColor = FColor(255, 0, 0);
	//			}
	//			if (bIsOneOfEndToggleComp)
	//			{
	//				CompInfoStr += TEXT("End Toggle");
	//				CompTextColor = FColor(0, 0, 255);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		CompInfoStr += TEXT("ActiveCamera Area");
	//		CompTextColor = FColor(0, 255, 127);
	//	}
	//			
	//	
	//	NewInfoTextComp->SetTextRenderColor(CompTextColor);
	//	NewInfoTextComp->SetText(FText::FromString(CompInfoStr));
	//	
	//	ShapeCompInfoText.Add(NewInfoTextComp);
	//}

}

void AB2ActiveCameraActor::SyncFromEditingCompPlacement()
{
	//if(CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	//{
	//	if(TargetPositionEditingComp.Num() > 0 && TargetPositionEditingComp[0])
	//	{
	//		FixedCameraTargetPosition = TargetPositionEditingComp[0]->RelativeLocation;
	//	}
	//}
}

void AB2ActiveCameraActor::DestroyEditingComponents(bool bDestroyStaticCompToo)
{
	if(bDestroyStaticCompToo)
	{
		for(int32 CI = 0; CI < TargetPositionEditingComp.Num(); ++CI)
		{
			if(TargetPositionEditingComp[CI])
			{
				TargetPositionEditingComp[CI]->DestroyComponent();
			}
		}
		TargetPositionEditingComp.Empty();
	}

	for (int32 SCI = 0; SCI < ShapeCompInfoText.Num(); ++SCI)
	{
		if (ShapeCompInfoText[SCI])
		{
			ShapeCompInfoText[SCI]->DestroyComponent();
		}
	}
	ShapeCompInfoText.Empty();

	// 기타 컴포넌트가 혹시 추가가 되면..
}

void AB2ActiveCameraActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	const FName Name_ToggleMode_BeginComponents = GET_MEMBER_NAME_CHECKED(AB2ActiveCameraActor, ToggleMode_BeginComponents);
//	const FName Name_ToggleMode_EndComponents = GET_MEMBER_NAME_CHECKED(AB2ActiveCameraActor, ToggleMode_EndComponents);
//
//	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
//	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;
//
//	PlayerMoveInputAdjustBoomSyncAlpha = FMath::Clamp(PlayerMoveInputAdjustBoomSyncAlpha, 0.0f, 1.0f);
//	CameraPlayerFollowingScale = FMath::Clamp(CameraPlayerFollowingScale, 0.0f, 1.0f);
//
//	if (PropertyName == Name_ToggleMode_BeginComponents || PropertyName == Name_ToggleMode_EndComponents)
//	{
//		// 양다리 금지 ㄷㄷ
//		bool bCompNameExistForBothBeginAndEnd = false;
//		for (int32 BCI = 0; BCI < ToggleMode_BeginComponents.Num(); ++BCI)
//		{
//			for (int32 ECI = 0; ECI < ToggleMode_EndComponents.Num(); ++ECI)
//			{
//				if (ToggleMode_BeginComponents[BCI] == ToggleMode_EndComponents[ECI])
//				{
//					bCompNameExistForBothBeginAndEnd = true;
//					break;
//				}
//			}
//		}
//
//
//		if (bCompNameExistForBothBeginAndEnd)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[경고] ToggleMode_BeginComponents 와 ToggleMode_EndComponents 에 동시에 등록된 컴포넌트가 있습니다."))
//				));
//#endif
//		}
//
//
//		TArray<UActorComponent*> AllShapeComps = GetComponentsByClass(UShapeComponent::StaticClass());
//		// 실제로 없는 컴포넌트 이름을 넣었는지 검사.
//		for (int32 BCI = 0; BCI < ToggleMode_BeginComponents.Num(); ++BCI)
//		{
//			if (ToggleMode_BeginComponents[BCI] == NAME_None) // 아직 세팅하지 않은 건 패스.
//			{
//				continue;
//			}
//
//			bool bNameNotExist = true;
//			for (int32 SCI = 0; SCI < AllShapeComps.Num(); ++SCI)
//			{
//				if (AllShapeComps[SCI]->GetName() == ToggleMode_BeginComponents[BCI].ToString())
//				{
//					bNameNotExist = false;
//					break;
//				}
//			}
//			if (bNameNotExist == true)
//			{
//#if !PLATFORM_MAC
//				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//					FString::Printf(TEXT("[경고] %s (이)라는 이름의 Shape (Collision) Component 가 없습니다."), *(ToggleMode_BeginComponents[BCI].ToString()))
//					));
//#endif
//			}
//		}
//
//		for (int32 BCI = 0; BCI < ToggleMode_EndComponents.Num(); ++BCI)
//		{
//			if (ToggleMode_EndComponents[BCI] == NAME_None) // 아직 세팅하지 않은 건 패스.
//			{
//				continue;
//			}
//
//			bool bNameNotExist = true;
//			for (int32 SCI = 0; SCI < AllShapeComps.Num(); ++SCI)
//			{
//				if (AllShapeComps[SCI]->GetName() == ToggleMode_EndComponents[BCI].ToString())
//				{
//					bNameNotExist = false;
//					break;
//				}
//			}
//			if (bNameNotExist == true)
//			{
//#if !PLATFORM_MAC
//				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//					FString::Printf(TEXT("[경고] %s (이)라는 이름의 Shape (Collision) Component 가 없습니다."), *(ToggleMode_EndComponents[BCI].ToString()))
//					));
//#endif
//			}
//		}
//	}
//
//	SetEditConditionProperties();
//
//	RefreshEditingComp();
}

void AB2ActiveCameraActor::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	//if (bFinished)
	//{
	//	// ActiveCameraComp 대신 SpringArm 을 제어해야 함.
	//	// 그래도 ActiveCameraComp 가 클릭하기에는 좋으니 ActiveCameraComp 에디트가 SpringArm 에디트로 되도록 값을 맞춰주도록 하자.
	//	if(ActiveCameraComp->RelativeLocation != FVector(0.0f,0.0f,0.0f))
	//	{
	//		// 정식으로는 SocketOffset 과 TargetOffset 도 맞춰주어야 하지만 굳이 그렇게까지..
	//		FTransform SupposedSpringArmTransform = FTransform(FVector(ActiveCameraSpringArm->TargetArmLength, 0.0f, 0.0f)) * ActiveCameraComp->GetComponentTransform();
	//		ActiveCameraSpringArm->SetWorldLocation(SupposedSpringArmTransform.GetTranslation());
	//		ActiveCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//	}
	//	if(ActiveCameraComp->RelativeRotation != FRotator(0.0f,0.0f,0.0f))
	//	{
	//		ActiveCameraSpringArm->SetWorldRotation(ActiveCameraComp->GetComponentRotation());
	//		ActiveCameraComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	//	}

	//	// 위치 싱크 후 Refresh
	//	SyncFromEditingCompPlacement();
	//	// Regenerate 없이 위치만 싱크하는 옵션으로 call. 성능 때문이 아니라 이렇게 하지 않으면 매 이동마다 선택 포커스를 잃어버려서 존나 불편함.
	//	RefreshEditingComp(false);
	//}
}

void AB2ActiveCameraActor::SetEditConditionProperties()
{
	bDualCameraBlending_Inverse = !bDualCameraBlending;
	bCameraMovementMode_ScaledFollowingPlayer = (CameraMovementMode == EActiveCameraMovementMode::EACMM_ScaledFollowPlayer && !bDualCameraBlending);
	bCameraTargetingMode_Mob = (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob && !bDualCameraBlending);
	bCameraTargetingMode_Mob_with_PlayerCameraBoom = (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer && CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob && !bDualCameraBlending);
	bCameraTargetingMode_Fixed = (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed && !bDualCameraBlending);
	bCameraMovementTarget_BothPlayer = (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer && CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player);
	bUseActiveCameraComponentMode = IsUsingActiveCameraComponent();
	bTriggerMode_ToggleByComponent = (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent);
	bUseDCBExtraPlayerTopDownCameraSettings = (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer&& CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player && bDualCameraBlending);
	bUseDCBExtraFOV_Override = bUseDCBExtraPlayerTopDownCameraSettings && bOverrideFOVAngle;
}
#endif