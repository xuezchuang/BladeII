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
//				: SpriteTexture(TEXT("/Engine/EditorResources/S_SceneCaptureIcon")) // �� ī�޶� ��� �������� �ֱ�.
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
//	// ��ܿ� CameraMovementMode �� CameraTargetingMode ���� �� SetEditConditionProperties ���� �����ϰ�
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
			FString::Printf(TEXT("[���] ActiveCameraActor %s �� DualCameraBlending �� �����Ǿ����� �̸� ���� �߰� ī�޶� ������Ʈ�� ����� ��ġ Ȥ�� �����Ǿ� ���� �ʽ��ϴ�."), *GetName())
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
		SyncFromEditingCompPlacement(); // ������ ������Ʈ�� ������ ��ġ ��ũ
	}
#endif
}

void AB2ActiveCameraActor::BeginDestroy()
{
	Super::BeginDestroy();

	RemoveComponentOverlapCallback(); // �� �� �ʿ䰡 �ֳ�..?

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
	// �����Ϳ��� ��ġ�� ���� ��ġ�μ�.. Fixed �� ScaledFollowPlayer ���� ���.
	InitialActiveCameraSpringArmTransform = ActiveCameraSpringArm->GetComponentTransform();

	RegisterComponentOverlapCallback();

	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ManualActive)
		return;

	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_BeginStage)
		ForceBeginActiveCamera();
	else
		// ���ۺ��� �÷��̾�� ������ ��� �ٷ� ����. �� ���Ŀ� ABladeIIWorldSettings::BeginPlay �� �Ҹ��ٸ� ApplyDefaultPlayerCameraSetting �� ���� ���� ��.
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
		// ���� ī�޶� �̿�
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
				// DualCameraBlending �� �������� ���.

				FMinimalViewInfo ViewInfoA;
				CamA->GetCameraView(DeltaTime, ViewInfoA);
				FMinimalViewInfo ViewInfoB;
				CamB->GetCameraView(DeltaTime, ViewInfoB);

				const float BlendWeight = GetDualCameraBlendingWeight(); // B ���� weight �� �ǰڴ�.
				// OutResult ���� ViewInfoA �� ViewInfoB �� ����.
				OutResult = ViewInfoA;
				OutResult.BlendViewInfo(ViewInfoB, BlendWeight);

				CachedDualCameraBlendedYaw = OutResult.Rotation.Yaw; // �̰� �Է� �ڵ鸵�� ����..
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

	//// ���� ��Ƽ��ī�޶� ����Ȱ��ȭ�ȰŸ� ������ ����
	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (pGM)
	//{
	//	AB2ActiveCameraActor* pCurAcriveCamActor = pGM->GetCurrentActiveActiveCameraActor();

	//	if (pCurAcriveCamActor && pCurAcriveCamActor->GetTriggerMode() == EActiveCameraTriggerMode::EACTRG_ManualActive)
	//		return;
	//}

	//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));

	//// �÷��̾� overlap ���� �ߵ�.
	//if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ActorArea && B2PC &&
	//	B2Player && B2Player == UGameplayStatics::GetLocalPlayerCharacter(this))
	//{
	//	BeginActiveCamera(B2PC);
	//}
}

void AB2ActiveCameraActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	//// ���� ��Ƽ��ī�޶� ����Ȱ��ȭ�ȰŸ� ������ ����
	//ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (pGM)
	//{
	//	AB2ActiveCameraActor* pCurAcriveCamActor = pGM->GetCurrentActiveActiveCameraActor();

	//	if (pCurAcriveCamActor && pCurAcriveCamActor->GetTriggerMode() == EActiveCameraTriggerMode::EACTRG_ManualActive)
	//		return;
	//}

	//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	//ABladeIIPlayer* B2LocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//// �÷��̾ ������ �ߴ�.
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
	//	// BeginComponent/EndComponent �� ��� ������ ����� �Ϳ� overlap �ߴ��� ���ο� ����.
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
	//	// ���⼱ ���� ó�� ���ص� �� ��.
	//}
}

void AB2ActiveCameraActor::RegisterComponentOverlapCallback()
{
	//// ShapeComponent �� Overlap callback �� ���
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
	//// ShapeComponent ��
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

	//// ��Ÿ ������ �߰��� �� �ִ�. TriggerMode �� EACTRG_ToggleByComponent �� ��쿡 ����? 
	//// ������ �װ� �������� overlap �� �ƴ� ������ touch �� ���� toggle �Ǵ� ���ε� ������ ������ ��� �Ǵ�����?

	return false;
}

void AB2ActiveCameraActor::ForceBeginActiveCamera(float BlendInTimeOverride /*= -1.0f*/, AController* InOptionalInteractingController /*= NULL*/, bool bUseConditionalBeginTransitionInputDisable /*= true*/)
{
	//// ��Ÿ ���� �˻� ���� �׳� ����. ���� Ư���� ��쿡�� ����ؾ߰ڴ�.
	//ABladeIIPlayerController* B2LocalPC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (B2LocalPC)
	//{
	//	BeginActiveCamera(B2LocalPC, BlendInTimeOverride, InOptionalInteractingController, bUseConditionalBeginTransitionInputDisable);
	//}
}

void AB2ActiveCameraActor::CustomEndActiveCamera()
{
	EndActiveCamera(); // �ϴ� ���ٸ� �� ����.
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
	//// �켱, ������ Ȱ��ȭ �� ���� �ִٸ� ����. �׷��� ī�޶� ������ �� ����.
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	//if (B2GM && !B2GM->IsAllowCameraWorking())
	//	return;

	//if (B2GM && B2GM->GetCurrentActiveActiveCameraActor() && B2GM->GetCurrentActiveActiveCameraActor() != this)
	//{
	//	B2GM->GetCurrentActiveActiveCameraActor()->CustomEndActiveCamera();
	//}

	//// ���� ���ڸ� ���ؼ� BlendInTime �� override �� �� �ִµ� ��ü�� ���� ����� ���� ��� �ܺο��� ���� ���� �õ��� �� ���� ����� �ǵ�.
	//const float FinalBlendTime = BlendInTimeOverride >= 0.0f ? BlendInTimeOverride : BlendInTime;

	//BeginForNewInteractionCommon(InLocalPC, FinalBlendTime, InOptionalInteractingController);

	//if(bUseConditionalBeginTransitionInputDisable)
	//	ConditionalBeginTransitionInputDisable(true);

	//bIsActivated = true;

	//// GameMode �� ���
	//if (B2GM)
	//{
	//	B2GM->SetCurrentActiveActiveCameraActor(this);
	//}
}

void AB2ActiveCameraActor::BeginForNewInteractionCommon(class ABladeIIPlayerController* InLocalPC, float FinalBlendInTime, class AController* InOptionalInteractingController /*= NULL*/)
{
	//check(InLocalPC && InLocalPC->IsValidObj());
	//// InOptionalInteractingController �� LocalPC ó�� ���� �ε��� ���� �����Ǵ� �ְ� �ƴ϶� invalid �� ���ɼ��� ����� ������.
	//checkSlow(!InOptionalInteractingController || InOptionalInteractingController->IsValidObj());

	//CachedLocalPC = InLocalPC;
	//CachedInteractingController = InOptionalInteractingController ? InOptionalInteractingController : CachedLocalPC; // Stage ���� ��忡���� CachedLocalPC �� CachedInteractingController �� ���� ��..
	//LastInteractBeginPlayer = Cast<ABladeIIPlayer>(CachedInteractingController->GetPawn()); // �̷��� ĳ���ؼ� ���߿� üũ�� �ϰ� ���� ����� ���� �ʴ´�.
	//
	//if (bDualCameraBlending && CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
	//{
	//	UCameraComponent* CamA = ActiveCameraComp;
	//	UCameraComponent* CamB = FindExtraCamCompForDualCameraBlending();
	//	if (CamA && CamB)
	//	{
	//		// �׳� ActiveCameraComp �� Fixed �� ����ϴ� �Ŷ� ����� �� �ִµ� ���� �����ϰ� �ϴ� ���� �ٸ� �ϳ��� �����ǵ��� �� ���� �־ Ư���� ��û�� �ƴ� ���� �׳� ��.
	//		// �̰͵� ForceFromLastView�� �ؾ� ����� �����׿�.
	//		InLocalPC->SetViewTargetWithBlend(this, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic, 0, false, true);
	//		// ��Ÿ Dual Blending �� �ٽ� �۵��� CalcCamera ����.
	//	}
	//}
	//else
	//{
	//	if (CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
	//	{
	//		// CameraTargetingMode �� Fixed �� �ƴ϶�� rotation �� �ٲ� �� �ִ�.
	//		ActiveCameraSpringArm->SetWorldTransform(InitialActiveCameraSpringArmTransform);
	//	}
	//	if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	//	{
	//		// FixedCameraTargetPosition �� relative �� ���.
	//		FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
	//		SetActiveCameraRotationToTarget(FixedTargetWorldTransform.GetTranslation());
	//	}

	//	if (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer && CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
	//	{
	//		// �⺻���� player top-down ī�޶� ������ �ٲ㼭 ����ϴ� ��尡 ��. ���� ó������ ����.
	//		SetupForConsistentPlayerTopDownCamera();
	//	}

	//	UCameraComponent* FinalCameraComponent = NULL;
	//	if (IsUsingActiveCameraComponent())
	//	{
	//		// ���� ������ �� �𸣰ڴµ� �ᱹ ForceFromLastView �� �ؾ� ����� �����±�
	//		CachedLocalPC->SetViewTargetWithBlend(this, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic, 0, false, true);
	//		FinalCameraComponent = ActiveCameraComp;
	//	}
	//	else
	//	{
	//		if (LastInteractBeginPlayer)
	//		{
	//			// �ƴ϶�� �÷��̾� top-down ī�޶� ����Ѵٴ� ���� �Ǵµ� ���� ViewTarget ���� �����̸� PlayerCameraManager ������ Ư���ϰ� ó���ؼ� ���� �� ��.
	//			CachedLocalPC->SetViewTargetWithBlend(LastInteractBeginPlayer, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic);
	//			
	//			if (LastInteractBeginPlayer->HasActorBegunPlay())
	//			{
	//				FinalCameraComponent = LastInteractBeginPlayer->GetTopDownCameraComponent();
	//			}
	//		}
	//	}

	//	if (bOverrideFOVAngle && FinalCameraComponent) // ���������� ���Ǵ� CameraComponent ���ٰ� FOV ���� override. EndActiveCamera ���� �������� ��.
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
	//if (CachedLocalPC && InteractLocalPlayer) // NULL �̸� �̹� End ó�� �Ǿ��ų� �ƴ� ���� �߸��Ǿ��ų�. �ι� �Ҹ� ���� �����Ƿ� �ߺ� ó������ �ʵ���.
	//{
	//	if (bRestoreCamera)
	//	{
	//		// �÷��̾� �⺻ ī�޶�� back
	//		// Cubic �̿��� �ɼ��� �ʿ��ϴٸ� ������ ����
	//		if (bOverrideFOVAngle)
	//		{
	//			// Beginplay �� ĳ���� ���� ������ ���ư�.
	//			InteractLocalPlayer->GetTopDownCameraComponent()->FieldOfView = InteractLocalPlayer->GetTopDownCamIntialFOV();
	//		}
	//		RestoreSpringArmSettings();
	//		CachedLocalPC->ApplyWorldDefaultCamera(BlendOutTime, EViewTargetBlendFunction::VTBlend_Cubic);
	//	}

	//	// GameMode ��� ����
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
			{// �÷��̾� top-down ī�޶��� ���� ���� �Ӽ������� �ǵ����� ���� ���� ���� ĳ���� �д�.
				CopyFromSpringArmComponentToSelectedSettings(InteractLocalPlayer->GetCameraBoom(), PlayerCameraBoomSettingBackup);
				CopyFromSelectedSettingsToSpringArmComponent(SpringArmSettings, InteractLocalPlayer->GetCameraBoom());
			}
		}
	}
}
void AB2ActiveCameraActor::RestoreSpringArmSettings()
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	// �÷��̾� ĳ���Ϳ� �޸� Top-down ī�޶� �����. ���⿡�� �����ϰ� �ִ� ActiveCameraComp �� ���� �ǵ��� �ʿ䰡 ����..
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
		// �̰� �뷫 � ���ĸ�, ActiveCamera ���� �ȿ��� �±׸� �ߴٰų� �ؼ� �̰� Ʈ������ BladeIIPlayer �� Destroy �ǰ� ���� spawn �� ���.
		// �װ͵� ���������� ActorArea�� Ʈ���� �ϴ� ����� Overlap ������ ���� BeginActiveCamera �� �Ǿ��� �Ű� ToggleByComponent �� Ʈ���� �ϴ� ��� ����� ���� �� ��.
		// ActorArea �� Ʈ���� �ϴ� ��쵵 �̰ɷ� ó���� �� �ִٸ� �װ� �� ���� ��.
		BeginForNewInteractionCommon(CachedLocalPC, 0.0f, CachedInteractingController);
	}
}

void AB2ActiveCameraActor::UpdateActiveCamera(float DeltaSeconds)
{
	//ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	//if (InteractLocalPlayer && CachedLocalPC)
	//{
	//	bool bFallbackToWorldDefault = false; // ��� ���� ������ �̰� true �� ����.

	//	if (IsUsingActiveCameraComponent())
	//	{
	//		float ExpectedPlayerInputHandlingYaw = ActiveCameraSpringArm->GetComponentRotation().Yaw; // �Ϲ����� ActiveCameraComponent ����ϴ� ��Ȳ������ �Է� �ڵ鸵 �⺻�� ��.
	//		
	//		if (bDualCameraBlending)
	//		{
	//			ExpectedPlayerInputHandlingYaw = CachedDualCameraBlendedYaw;
	//			// ��Ÿ ���� ���⼭ �ʿ��� �� ���� ��.. ī�޶� ��ü�� �� �����̶�.
	//		}
	//		else
	//		{
	//			if (CameraMovementMode == EActiveCameraMovementMode::EACMM_ScaledFollowPlayer)
	//			{
	//				FVector BasePosToPlayer = InteractLocalPlayer->GetActorLocation() - InitialActiveCameraSpringArmTransform.GetTranslation();
	//				float PlayerToBaseDistance = BasePosToPlayer.Size();
	//				BasePosToPlayer.Normalize();

	//				// CameraPlayerFollowingScale ��ŭ �������ؼ� �÷��̾� ��ġ�� ���󰡵���
	//				FVector ScaledCameraLocation = InitialActiveCameraSpringArmTransform.GetTranslation() + BasePosToPlayer * CameraPlayerFollowingScale * PlayerToBaseDistance;
	//				ActiveCameraSpringArm->SetWorldLocation(ScaledCameraLocation);
	//			}
	//			// CameraMovementMode �� FollowPlayer �̰ų� Fixed �� ���⼭ ó�� �� ��.

	//			if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
	//			{
	//				SetActiveCameraRotationToTarget(InteractLocalPlayer->GetActorLocation());
	//			}
	//			else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob)
	//			{
	//				if (CachedTargetMob == NULL) // Begin ������ ���� ���� �����Ƿ� Update ���� �˻�
	//				{
	//					CachedTargetMob = GetDesiredCameraTargetWaveMob();
	//				}
	//				if (CachedTargetMob && CachedTargetMob->IsPendingKill() == false)
	//				{
	//					SetActiveCameraRotationToTarget(CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset);
	//				}
	//				else
	//				{
	//					// ���� ���� ��Ȳ�̶�� �⺻ ī�޶�� ����.
	//					bFallbackToWorldDefault = true;
	//				}
	//			}
	//			else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	//			{
	//				// FixedCameraTargetPosition �� relative �� ���.
	//				FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
	//				SetActiveCameraRotationToTarget(FixedTargetWorldTransform.GetTranslation());
	//			}
	//		}

	//		// �÷��̾� movement �Է� ó���� ���� CameraBoom �� ���ϰ� ȸ����Ų��. ABladeIIPlayer::MoveForward/MoveRight ����
	//		// ActiveCameraComp �� Yaw �� �������� �����ϸ� yaw �� �ް��ϰ� ���ϴ� �Ϻ� ��Ȳ���� ������ �ǹǷ� ������ �� �ϴµ� PlayerMoveInputAdjustBoomSyncAlpha ���� ���� ���� ��ǥ������ �־��� ����.
	//		// �׸��� ������ ������ ��ǻ� ������ �ʴ� ��Ȳ���� �������� ������ �ϱ⵵ �ؼ�.. �����ϸ� PlayerMoveInputAdjustBoomSyncAlpha �� 1.0 ����..
	//		float InterpolatedYaw = 
	//			bUsePlacedActiveCamYawAsInputBase ? InitialActiveCameraSpringArmTransform.Rotator().Yaw : // �׷���.. yaw �� �ް��ϰ� ���ϴ� ��Ȳ�� ���� ������å.
	//			FMath::Lerp(InteractLocalPlayer->
	//			GetCameraYaw(), ExpectedPlayerInputHandlingYaw, PlayerMoveInputAdjustBoomSyncAlpha);
	//		InteractLocalPlayer->UpdateCameraYaw(InterpolatedYaw);
	//	}
	//	else
	//	{
	//		// �÷��̾� top-down ī�޶��� CameraBoom �� ����. BladeIIPlayerController �ʿ� ������ �� �������̽��� ���ؼ�.
	//		// CameraMovementMode �� ������ FollowPlayer �� �װ� (��, �ǵ帱 �ʿ� ����) CameraTarget �� ���� CameraBoom �� rotation �� �����Ѵ�.
	//		if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
	//		{
	//			// �� ���� ���� ���� top-down ī�޶�� ���̰� ���� ���ε�, PlayerTopDownCameraSettings �� ���� �ʱ� ���� ��?

	//			// ž�ٿ� ī�޶� �̿��� ��������
	//			if (bDualCameraBlending)
	//			{
	//				if (CachedLocalPC)
	//				{
	//					// PlayerTopDownCameraSettings �� �� ��Һ��� override �� �͸� ����
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

	//					// ī�޶� �����̼� ����
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
	//			if (CachedTargetMob == NULL) // Begin ������ ���� ���� �����Ƿ� Update ���� �˻�
	//			{
	//				CachedTargetMob = GetDesiredCameraTargetWaveMob();
	//			}

	//			if (CachedTargetMob && CachedTargetMob->IsPendingKill() == false)
	//			{
	//				SetPlayerCameraBoomRotationToTarget(
	//					CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset,
	//					// ���⿣ ���� �Ķ���Ϳ� ���� offset �� overriding �� �� ����.
	//					TargetMobCameraAdjustSettings.BoomRotOffsetPitch, TargetMobCameraAdjustSettings.BoomRotOffsetYaw,
	//					(TargetMobCameraAdjustSettings.bOverrideCameraBoomArmLength ? 
	//						GetExtraAppliedCameraDistance(TargetMobCameraAdjustSettings.CameraBoomArmLength, TargetMobCameraAdjustSettings.CameraBoomArmLength_Extra)
	//						: 0.0f)
	//					);

	//				// �� �Ĵٺ���..
	//				SetPlayerCameraRotationToTarget(
	//					CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset,
	//					TargetMobCameraAdjustSettings.CamRotOffsetPitch, TargetMobCameraAdjustSettings.CamRotOffsetYaw
	//					);
	//				
	//			}
	//			else
	//			{
	//				// ���� ���� ��Ȳ�̶�� �⺻ ī�޶�� ����.
	//				//bFallbackToWorldDefault = true;
	//			}
	//		}
	//		else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	//		{
	//			// FixedCameraTargetPosition �� relative �� ���.
	//			FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
	//			SetPlayerCameraBoomRotationToTarget(FixedTargetWorldTransform.GetTranslation());

	//			// CameraMovementMode �� FollowPlayer �鼭 TargetingMode �� Fixed �� ��� FixedCameraTargetPosition ��ġ�� �÷��̾ ���� ��� ī�޶� �Ҿ����ϰ� ��.
	//			// �ƿ� �ش� ������ ������� �ʴ� �ɷ� �� ���� ������, FixedCameraTargetPosition �� �� �����ؼ� ����� �� ����.
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
	//	// PlayerTopDownCameraSettings �� �� ��Һ��� override �� �͸� ����
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
	//	// ī�޶� �����̼� ����
	//	if (ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter())
	//	{
	//		FRotator CamRotation = InteractLocalPlayer->TopDownCameraComponent->GetRelativeTransform().GetRotation().Rotator();

	//		if (PlayerTopDownCameraSettings.bOverrideCameraPitch)
	//			CamRotation.Pitch = CamRotation.Pitch + PlayerTopDownCameraSettings.CameraPitch;

	//		if (PlayerTopDownCameraSettings.bOverrideCameraYaw)
	//			CamRotation.Yaw = CamRotation.Yaw + PlayerTopDownCameraSettings.CameraYaw;

	//		//ActiveCameraSpringArm->SetWorldRotation(CamRotation);
	//		InteractLocalPlayer->TopDownCameraComponent->SetRelativeRotation(CamRotation);

	//		// SetViewTargetWithBlend �� �� ���Ŀ� ���� �ҷ��� ��.
	//	}
	//}
}

bool AB2ActiveCameraActor::IsUsingActiveCameraComponent()
{
	if (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer)
	{
		// �ϴ�, FollowPlayer ��忡���� �÷��̾� top-down ī�޶� ����Ѵٴ� �����ε�, ����鼭 �ٲ� ���� �ִ�.
		return false;
	}
	return true;
}

void AB2ActiveCameraActor::SetActiveCameraRotationToTarget(FVector InViewTargetLocation)
{
	FVector ToTarget = (InViewTargetLocation - ActiveCameraSpringArm->GetComponentLocation());
	ToTarget.Normalize();
	// �����δ� SpringArm �� ����.
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

		// RotOffset �� �׳� ����
		CamBoomRotation.Pitch = CamBoomRotation.Pitch + RotOffsetPitch;

		// Boom Ptich ���׼� ���Ѽ�
		if (bUseCameraBoomRotationClamp)
			CamBoomRotation.Pitch = FMath::Clamp<float>(CamBoomRotation.Pitch, MinCameraBoomRotationClamp, MaxCameraBoomRotationClamp);
		
		InteractLocalPlayer->UpdateCameraPitch(CamBoomRotation.Pitch);
		InteractLocalPlayer->UpdateCameraYaw(CamBoomRotation.Yaw + RotOffsetYaw);
		if (ArmLengthOverride > 0.0f) // ArmLengthOverride �� 0 ���� Ŭ ����
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
	AB2MonsterSpawnPool* TargetMobSP = B2GM ? B2GM->GetActiveSpawnPool() : NULL; // ���� ���� �� ������.

	if (TargetMobSP)
	{
		int32 WaveNum = 0;
		int32 MobIndex = 0;
		ParseForWaveMobSelectKeyword(TargetMobSelectKeyword, WaveNum, MobIndex); // ���ڿ��� ������ �ɼǿ��� ��ȣ�� ����.

		TArray<ABladeIICharacter*> InterestedWaveMobs;
		TargetMobSP->GetWaveMobList(WaveNum, InterestedWaveMobs, false); // �׾�� �༮�� �����ؼ� ���������� ���⼭ ȣ���ϴ� Ÿ�ֿ̹� �װ� �ɱ�..?

		for (int32 WMI = 0; WMI < InterestedWaveMobs.Num(); ++WMI)
		{
			ABladeIICharacter* CurrMob = InterestedWaveMobs[WMI];
			if (CurrMob)
			{
				if (CurrMob->GetSpawnPoolWaveObjIndex() == MobIndex) // ��ǻ� Wave �� ���۵� ��Ȳ�� �ƴϸ� ã�� �ε����� mob �� ���� ���� �ִ�.
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

	//// ī�޶� ��ũ ������ ���� �÷��̾� �̵� �Է¿� ȥ���� �� �� �����Ƿ� ���� �ð� ���� �Է��� ���´�.
	//// �÷��̾� Top-down ī�޶� ����ϴ� ��忡�� �ʿ���� ��.. �ߴµ� movement, target ��� player �� �ƴ� �̻��� �ʿ��� ��.
	//// ���ī�޶� ��忡���� ���
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
	//			// �Է��� ��Ȱ��ȭ �� ���� ���� ��ġ���� �ణ ������ �̵���Ų��. ��迡�� �� �־������� ��
	//							
	//			const float MoveDistance = InteractLocalPlayer->GetDefaultHalfHeight() * 2.0f * TransitionAutoMoveScale;
	//			const FVector MoveTargetLocation = InteractLocalPlayer->GetActorLocation() + (InteractLocalPlayer->GetVelocity().GetSafeNormal() * MoveDistance);
	//			
	//			UNavigationSystem::SimpleMoveToLocation(CachedLocalPC, MoveTargetLocation);
	//		}
	//	}
	//	else
	//	{
	//		// Ȥ�ö� TransitIn �����̶�� �ߴ�
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
	//// CachedLocalPC �� ������ ���ϰ� bDisableMoveInput �� ó���ؾ�..
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
	//// CachedLocalPC �� ������ ���ϰ� bDisableMoveInput �� ó���ؾ�..
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
	//	//���� ���� ������
	//	FVector vPointA;
	//	//���� ���� ������
	//	FVector vPointB;

	//	// �÷��� �����Ǿ����� ����ġ�� �ϰ� �ƴϸ� ī�޶���ġ �̿���
	//	if (FlagA)
	//		vPointA = FlagA->GetComponentLocation();
	//	else
	//		vPointA = CamA->GetComponentLocation();
	//			
	//	if (FlagB)
	//		vPointB = FlagB->GetComponentLocation();
	//	else
	//		vPointB = CamB->GetComponentLocation();

	//	// ���ڿ��������� �ɸ��� ��ġ���� A, B �� �������̷� �����Ѵ�(���� �̿�).
	//	FVector vGoalPos = vPointB - vPointA;
	//	FVector vCurPos = InteractLocalPlayer->GetActorLocation() - vPointA;

	//	// ������ ������ġ ���� ������
	//	float fCurPosSize = FVector::DotProduct(vGoalPos.GetSafeNormal(KINDA_SMALL_NUMBER), vCurPos);
	//			
	//	float fWeight = fCurPosSize / vGoalPos.Size();

	//	if (fWeight < KINDA_SMALL_NUMBER) // ���� �������� ��Ÿ�� �� �ִ� ��Ȳ�� �ƴϰ�����..
	//		return 0.0f;

	//	if (fWeight >= 1.0f) // Ȥ�� 1�Ѵ°� �ʿ��ϸ� �׶� ����
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

	//	//Ȱ��ȭ �ѹ� ����
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

	//		// ���� �����ϴ� �ɼ��� �ƴ϶�� ���� �� ���
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
	//			EditingComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)); // EditingComp �� RelativeLocation �� ����� ���̹Ƿ� �ݵ�� root �� attach
	//			EditingComp->RegisterComponent();
	//			EditingComp->SetStaticMesh(EditingCompMesh);
	//			EditingComp->SetVisibility(true);
	//			// ������ ���ӿ��� �������� ���� �ֵ������� 
	//			EditingComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//			EditingComp->SetHiddenInGame(true);
	//			EditingComp->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")));
	//			
	//			// Movement, Target ��� Fixed �̸� ���� ���� CameraTargetPosition �� �� �� ���� ActiveCameraComp �� ��ġ�� ��ġ �� ȸ������ �״�� ����ϸ� �ǰ���..
	//			if(CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
	//			{					
	//				EditingComp->SetWorldLocation(ActiveCameraSpringArm->GetComponentTransform().TransformPosition(FVector(300.0f, 0.0f, 0.0f)));
	//				FixedCameraTargetPosition = EditingComp->RelativeLocation;
	//			}
	//			else
	//			{
	//				EditingComp->SetRelativeLocation(FixedCameraTargetPosition); // FixedCameraTargetPosition �� relative ���
	//			}
	//		}
	//	}
	//}

	//// ShapeComponent �鿡 ���� �ȳ��ؽ�Ʈ�� ����. ToggleMode_BeginComponents, ToggleMode_EndComponents �� ����� �͵��� Ư���� �߰� ����..
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
	//	NewInfoTextComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // ��ġ�� ������ �����ش�
	//	
	//	const bool bIsOneOfBeginToggleComp = IsOneOfBeginToggleComp(CurrShapeComp);
	//	const bool bIsOneOfEndToggleComp = IsOneOfEndToggleComp(CurrShapeComp);

	//	// Component �뵵 ���� �ٸ��� ǥ���� ������.
	//	FColor CompTextColor(255, 0, 0);
	//	FString CompInfoStr = CurrShapeComp->GetName() + TEXT("\n");

	//	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent)
	//	{
	//		if (bIsOneOfBeginToggleComp && bIsOneOfEndToggleComp)
	//		{
	//			CompInfoStr += TEXT("WRONG Setting!!"); // Begin/End �� �� ���ÿ� ���õ� �� �߸���.
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

	// ��Ÿ ������Ʈ�� Ȥ�� �߰��� �Ǹ�..
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
//		// ��ٸ� ���� ����
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
//				FString::Printf(TEXT("[���] ToggleMode_BeginComponents �� ToggleMode_EndComponents �� ���ÿ� ��ϵ� ������Ʈ�� �ֽ��ϴ�."))
//				));
//#endif
//		}
//
//
//		TArray<UActorComponent*> AllShapeComps = GetComponentsByClass(UShapeComponent::StaticClass());
//		// ������ ���� ������Ʈ �̸��� �־����� �˻�.
//		for (int32 BCI = 0; BCI < ToggleMode_BeginComponents.Num(); ++BCI)
//		{
//			if (ToggleMode_BeginComponents[BCI] == NAME_None) // ���� �������� ���� �� �н�.
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
//					FString::Printf(TEXT("[���] %s (��)��� �̸��� Shape (Collision) Component �� �����ϴ�."), *(ToggleMode_BeginComponents[BCI].ToString()))
//					));
//#endif
//			}
//		}
//
//		for (int32 BCI = 0; BCI < ToggleMode_EndComponents.Num(); ++BCI)
//		{
//			if (ToggleMode_EndComponents[BCI] == NAME_None) // ���� �������� ���� �� �н�.
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
//					FString::Printf(TEXT("[���] %s (��)��� �̸��� Shape (Collision) Component �� �����ϴ�."), *(ToggleMode_EndComponents[BCI].ToString()))
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
	//	// ActiveCameraComp ��� SpringArm �� �����ؾ� ��.
	//	// �׷��� ActiveCameraComp �� Ŭ���ϱ⿡�� ������ ActiveCameraComp ����Ʈ�� SpringArm ����Ʈ�� �ǵ��� ���� �����ֵ��� ����.
	//	if(ActiveCameraComp->RelativeLocation != FVector(0.0f,0.0f,0.0f))
	//	{
	//		// �������δ� SocketOffset �� TargetOffset �� �����־�� ������ ���� �׷��Ա���..
	//		FTransform SupposedSpringArmTransform = FTransform(FVector(ActiveCameraSpringArm->TargetArmLength, 0.0f, 0.0f)) * ActiveCameraComp->GetComponentTransform();
	//		ActiveCameraSpringArm->SetWorldLocation(SupposedSpringArmTransform.GetTranslation());
	//		ActiveCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//	}
	//	if(ActiveCameraComp->RelativeRotation != FRotator(0.0f,0.0f,0.0f))
	//	{
	//		ActiveCameraSpringArm->SetWorldRotation(ActiveCameraComp->GetComponentRotation());
	//		ActiveCameraComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	//	}

	//	// ��ġ ��ũ �� Refresh
	//	SyncFromEditingCompPlacement();
	//	// Regenerate ���� ��ġ�� ��ũ�ϴ� �ɼ����� call. ���� ������ �ƴ϶� �̷��� ���� ������ �� �̵����� ���� ��Ŀ���� �Ҿ������ ���� ������.
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