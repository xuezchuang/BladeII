// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "BladeIIPlayerController.h"
#include "BladeIIPlayer.h"
#include "BladeIINetPlayer.h"
#include "B2AutoAIController.h"
//#include "AI/Navigation/NavigationSystem.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "BladeIIWorldSettings.h"
#include "BladeIIGameMode.h"
#include "B2StageGameMode.h"
#include "B2StageManager.h"
#include "B2TutorialGameMode.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "BladeIIUtil.h"
#include "B2SkillRTManager.h"
#if !UE_BUILD_SHIPPING
#include "B2LobbyUI_CharStatDetail.h"
#endif
#include "BladeIIAIController.h"

#include "BladeIIGameImpl.h"

//AB2StageManager* GetStageManager(UWorld* InWorld)
//{
//	if (!InWorld)
//		return NULL;
//
//	auto* B2SGM			= Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(InWorld));
//	auto* StageManager	= B2SGM ? B2SGM->GetStageManager() : NULL;
//
//	return StageManager;
//}

ABladeIIPlayerController::ABladeIIPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bHUDHidingCinematicMode = false;
	bInputDisabledCinematicMode = false;
	bStageEndNonCombatState = false;
	bDisableMoveInputForActiveCameraTransition = false;

	bCameraTargetSetByCameraBoom = false;
	bCameraTargetFollowPitchToo = false;
	ExplicitCameraBoomPitch = 0.0f;
	bExplicitCameraBoomPitchSet = false;
	ExplicitCameraBoomYaw = 0.0f;
	bExplicitCameraBoomYawSet = false;
	ExplicitCameraBoomArmLength = 0.0f;
	bExplicitCameraBoomArmLengthSet = false;
	ExplicitCameraTargetFocusOffset = FVector::ZeroVector;
	bExplicitCameraTargetFocusOffsetSet = false;
	bExplicitCameraBoomLockedViewTarget = false;
	bExplicitCameraBoomLockedViewTargetSet = false;
	
	PlayerSpawnMotionState = EPCSpawnMotionState::EPST_Normal;

	nCinematicModeCount = 0;
}

void ABladeIIPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	HideViewBlockingObjects();

	//// keep updating the destination every tick while desired
	//if (bMoveToMouseCursor)
	//{
	//	MoveToMouseCursor();
	//}
	ABladeIIPlayer* B2Char = Cast<ABladeIIPlayer>(GetPawn());

	if (B2Char == NULL || (B2Char->IsInQTEState() && !B2Char->IsQTEMounting()))
	{
		return;
	}
	// Validate Target
	//if (!B2Char->bNeedUpdateTarget)
	//{
	//	if (B2Char->TargetActor == NULL)
	//	{
	//		B2Char->bNeedUpdateTarget = true;
	//	}
	//	else if (B2Char->TargetActor->bIsDying)
	//	{
	//		B2Char->TargetActor = NULL; // Prevent last attack rotation
	//	}
	//	else if (B2Char->GetLastMovementInputVector() != FVector::ZeroVector && B2Char->bDisableRotateToInput)
	//	{
	//		// Guided target change
	//		B2Char->bNeedUpdateTarget = true;
	//	}
	//	else if (B2Char->TargetingMode == ETargetingMode::ETM_RetargetNearest && B2Char->TotalDashDuration <= 0.f)
	//	{
	//		B2Char->bNeedUpdateTarget = true;
	//	}
	//	else if (B2Char->TotalDashDuration <= 0.f) // Don't update target during dash
	//	{
	//		FVector PlayerLoc = B2Char->GetCapsuleComponent() ? B2Char->GetActorLocation() + B2Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : B2Char->GetActorLocation();
	//		FVector TargetLoc = B2Char->TargetActor->GetBaseMesh() ? B2Char->TargetActor->GetBaseMesh()->GetComponentLocation() : B2Char->TargetActor->GetActorLocation();

	//		if ((PlayerLoc - TargetLoc).Size() > B2Char->TargetingDistance)
	//		{
	//			B2Char->bNeedUpdateTarget = true;
	//		}
	//		else
	//		{
	//			FCollisionQueryParams Params(ABladeIIPlayer::TargetTraceName, false, this);
	//			Params.AddIgnoredActor(B2Char->TargetActor);
	//			FHitResult Result;
	//			GetWorld()->LineTraceSingleByChannel(Result, PlayerLoc, TargetLoc, ECC_Visibility, Params);

	//			B2Char->bNeedUpdateTarget = Result.bBlockingHit;
	//		}
	//	}
	//}

	if (B2Char->bNeedUpdateTarget)
	{
		B2Char->bNeedUpdateTarget = false;
		FindTarget(B2Char->GetLastMovementInputVector() != FVector::ZeroVector && B2Char->bDisableRotateToInput);
	}

	FVector Diff = FVector::ZeroVector;
	//float TargetYaw = B2Char->GetLastMovementInputVector().Rotation().Yaw;
		
	if (CameraTargetActor)
	{
		if (CameraTargetActor->IsAlive())
		{
			if (CameraTargetActor != B2Char->TargetActor || Diff == FVector::ZeroVector)
			{
				Diff = CameraTargetActor->GetActorLocation() - B2Char->GetActorLocation();
				//FinalTargetYaw = FRotator::ClampAxis(FMath::Atan2(Diff.Y, Diff.X) * 180 / PI); // FVector::Rotation ���� �ϴ� �ǵ�.. ���� Yaw �� �ʿ��ϴٸ� �̷��� �ϸ� ���ɿ� ������ �ǰ���..
			}
			FRotator CamBoomRotationToTarget = Diff.Rotation();
				
			if (bCameraTargetSetByCameraBoom)
			{
				// SetCameraTarget ���� CameraBoom �� ����ϵ��� �ߴٸ� CameraTargetBoomRotOffset �� Yaw ���� ���⼭�� �����ؾ�.
				// UpdateCameraTransform �� �ߴٸ�.. �� �𸣰ڴ� ����
				CamBoomRotationToTarget.Yaw += CameraTargetActor->CameraTargetBoomRotOffset.Yaw;
			}
				
			//B2Char->UpdateCameraYaw(Utilities::FBlendHelper::BlendRotation(DeltaTime, B2Char->GetCameraYaw(), CamBoomRotationToTarget.Yaw));

			if (bCameraTargetFollowPitchToo == true)
			{
				// Pitch �� ����. ����μ� CameraTargetSetByCameraBoom �� ���� ��ȿ�� ��?
				CamBoomRotationToTarget.Pitch += CameraTargetActor->CameraTargetBoomRotOffset.Pitch;
				B2Char->UpdateCameraPitch(CamBoomRotationToTarget.Pitch);
			}
		}
		else
		{
			CameraTargetActor = NULL;
		}
	}
	
#if !UE_BUILD_SHIPPING // �ϳ����� ���� �÷��̾� ��Ʈ�ѷ��� ���� �Ҹ��� Tick �̹Ƿ� ���Ⱑ ������ ��.
	ABladeIIPlayer* PossedPlayer = Cast<ABladeIIPlayer>(GetPawn()); // �׹W��忡�� �̰� NULL �� �� �ִ�. �׷� �� ���� UB2LobbyUI_CharStatDetail �ʿ� ������ �ɷ� ���. �ٵ� �׶� �ƿ� ����� �� ���� ����..
	EPCClass CurrentLocalMainPlayerClass = PossedPlayer ? PossedPlayer->GetCurrentPlayerClass() : EPCClass::EPC_End;
	EPCClass CurrentLocalSubPlayerClass = PossedPlayer ? PossedPlayer->GetTagPlayerClass() : EPCClass::EPC_End;
	//UB2LobbyUI_CharStatDetail::DebugDrawPlayerStatOnScreen(CurrentLocalMainPlayerClass, CurrentLocalSubPlayerClass);
#endif
}

void ABladeIIPlayerController::HideViewBlockingObjects()
{
	ABladeIIPlayer* B2Char = Cast<ABladeIIPlayer>(GetPawn());

	if (IsLocalController() && B2Char && B2Char->bUseCameraCulling)
	{
		FVector PlayerLoc = B2Char->GetCapsuleComponent() ? B2Char->GetActorLocation() + B2Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*1.5f : B2Char->GetActorLocation();

		static FName NAME_B2CameraCulling = FName(TEXT("CameraCulling"));
		FCollisionQueryParams CameraParams(NAME_B2CameraCulling, false, B2Char);

		TArray<FHitResult> Hits;
		// UE4 MERGE 4.9 djsong: SweepMulti is deprecated. I replace it with SweepMultiByObjectType based on our parameter usage.
		GetWorld()->SweepMultiByObjectType(Hits, B2Char->TopDownCameraComponent->GetComponentLocation(), PlayerLoc, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllStaticObjects), FCollisionShape::MakeCapsule(B2Char->CameraCullingRadius, B2Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), CameraParams);
		//FCollisionShape::MakeSphere(B2Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())

		// QTE �� ��� ��ҿ� �ü����� ���蹰 ������ �ȵ� �͵� ���� �� �ֵ��� CustomDurationThreshold �� ���� �ش�. -1 �̸� DrawInGameCustomDuration �� �⺻����.
		float CustomDurationThreshold = B2Char->IsInQTEState() ? -1.f : 0.f;

		// collate into per-actor list of hit components
		for (int32 Idx = 0; Idx < Hits.Num(); ++Idx)
		{
			UPrimitiveComponent* Comp = Hits[Idx].GetComponent();

			//if (Comp->bVisible && !Comp->bHiddenInGame && (Comp->DrawInGameCustomDuration >= CustomDurationThreshold))
			{
				//Comp->PushDrawInGameToProxy(false);
			}
		}
	}
}

void ABladeIIPlayerController::SetCameraTarget(ABladeIICharacter* CameraTarget, bool bSetByCameraBoom, bool bTargetPitchToo)
{
	// �α� �ʹ����� ��
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayerController::SetCameraTarget"));

	if (CameraTargetActor != CameraTarget)
	{
		CameraTargetActor = CameraTarget;
		if (CameraTargetActor)
		{
			bExplicitCameraBoomYawSet = false; // CameraTargetActor �� ���õǾ��ٸ� ��� Yaw �� ��� ������ ���� ���ŵ� ���̹Ƿ� �̰� ������� ����.
			bCameraTargetSetByCameraBoom = bSetByCameraBoom;
			// �̰� ��ǻ� SetByCameraBoom �� ����?
			bCameraTargetFollowPitchToo = bTargetPitchToo;

			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
			if (B2Player)
			{
				// ��忡 ���� �ʱ� ������ �����µ� ���Ŀ��� PlayerTick ���� �������� ��� ����.
				if (bCameraTargetSetByCameraBoom)
				{
					// CameraTargetActor �� ���ϵ��� CameraBoom �� ����. CameraBoom �� ����� ī�޶� ȿ���� ���� ��� ����
					FVector TargetToPlayerDir = CameraTargetActor->GetActorLocation() - B2Player->GetActorLocation();
					TargetToPlayerDir.Normalize();

					FRotator CamBoomRotationToTarget = TargetToPlayerDir.Rotation();
					if (bCameraTargetFollowPitchToo == false)
					{
						CamBoomRotationToTarget.Pitch = 0.0f; // ���� Ư���� yaw �� ���� �ʿ䰡 ���� ��.
					}
					CamBoomRotationToTarget += CameraTargetActor->CameraTargetBoomRotOffset; // Character �ʿ� ������ ���� ���� �߰�����.

					B2Player->UpdateCameraPitch(CamBoomRotationToTarget.Pitch);
					B2Player->UpdateCameraYaw(CamBoomRotationToTarget.Yaw);
				}
				else
				{
					// Top-down ī�޶� ������Ʈ transform �� ���� ����.
					B2Player->UpdateCameraTransform(CameraTargetActor->CameraTargetTransform);
				}
			}
		}
	}
}

void ABladeIIPlayerController::SetCameraBoomPosture(float InRotPitch, float InRotYaw, float InArmLength)
{
	// All in one.
	SetCameraBoomPitch(InRotPitch);
	SetCameraBoomYaw(InRotYaw); // ���⼱ CameraTargetActor �� ��ȿȭ�� ��.
	SetCameraBoomArmLength(InArmLength);
}

void ABladeIIPlayerController::SetCameraBoomPitch(float InRotPitch)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Player)
	{
		bExplicitCameraBoomPitchSet = true;
		ExplicitCameraBoomPitch = InRotPitch;
		B2Player->UpdateCameraPitch(ExplicitCameraBoomPitch);
	}
}

void ABladeIIPlayerController::SetCameraBoomYaw(float InRotYaw)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Player)
	{
		SetCameraTarget(NULL); // CameraTarget �� �ִٸ� �̰� �ȸ����� ��ȿȭ�Ѵ�.
		bExplicitCameraBoomYawSet = true;
		ExplicitCameraBoomYaw = InRotYaw;
		B2Player->UpdateCameraYaw(ExplicitCameraBoomYaw);
	}
}

void ABladeIIPlayerController::SetCameraBoomRoll(float InRotRoll)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Player)
	{
		SetCameraTarget(NULL); // CameraTarget �� �ִٸ� �̰� �ȸ����� ��ȿȭ�Ѵ�.
		bExplicitCameraBoomRollSet = true;
		ExplicitCameraBoomRoll = InRotRoll;
		B2Player->UpdateCameraRoll(ExplicitCameraBoomRoll);
	}
}

void ABladeIIPlayerController::SetCameraBoomArmLength(float InArmLength)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Player)
	{
		bExplicitCameraBoomArmLengthSet = true;
		ExplicitCameraBoomArmLength = InArmLength;
		B2Player->UpdateCameraDistance(ExplicitCameraBoomArmLength);
	}
}

void ABladeIIPlayerController::SetCameraTargetFocusOffset(const FVector& Offset)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Player)
	{
		bExplicitCameraTargetFocusOffsetSet = true;
		ExplicitCameraTargetFocusOffset = Offset;
		B2Player->UpdateCameraTargetFocusOffset(ExplicitCameraTargetFocusOffset);
	}
}

void ABladeIIPlayerController::SetCameraBoomLockedViewTarget(bool bLockedViewTarget)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Player)
	{
		bExplicitCameraBoomLockedViewTargetSet = true;
		bExplicitCameraBoomLockedViewTarget = bLockedViewTarget;
		B2Player->UpdateCameraBoomLockedViewTarget(bExplicitCameraBoomLockedViewTarget);
	}
}

void ABladeIIPlayerController::SetDefaultCameraRotator()
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Player)
	{
		B2Player->SetDefaultCameraRotator();
	}
}

void ABladeIIPlayerController::ApplyExplicitCameraBoomParams()
{
	if (bExplicitCameraBoomPitchSet)
	{
		SetCameraBoomPitch(ExplicitCameraBoomPitch);
	}
	if (bExplicitCameraBoomYawSet)
	{
		SetCameraBoomYaw(ExplicitCameraBoomYaw);
	}
	if (bExplicitCameraBoomArmLengthSet)
	{
		SetCameraBoomArmLength(ExplicitCameraBoomArmLength);
	}
	if (bExplicitCameraTargetFocusOffsetSet)
	{
		SetCameraTargetFocusOffset(ExplicitCameraTargetFocusOffset);
	}
	if (bExplicitCameraBoomLockedViewTargetSet)
	{
		SetCameraBoomLockedViewTarget(bExplicitCameraBoomLockedViewTarget);
	}
}

void ABladeIIPlayerController::ApplyWorldDefaultCamera(float BlendTime, enum EViewTargetBlendFunction BlendFunc)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayerController::ApplyWorldDefaultCamera"));

	//UWorld* TheWorld = GetWorld();
	//ABladeIIWorldSettings* B2WS = TheWorld ? Cast<ABladeIIWorldSettings>(TheWorld->GetWorldSettings()) : NULL;
	//if (B2WS)
	//{
	//	B2WS->ApplyDefaultPlayerCameraSetting(this, BlendTime, BlendFunc);
	//}

	//ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(GetPawn());

	//if (pPlayer)
	//	pPlayer->bCachedForCameraDistZoomState = false;
}

void ABladeIIPlayerController::ApplyWorldPlayerLightSetting()
{
	//UWorld* TheWorld = GetWorld();
	//ABladeIIWorldSettings* B2WS = TheWorld ? Cast<ABladeIIWorldSettings>(TheWorld->GetWorldSettings()) : NULL;
	//ABladeIIPlayer* MyPlayer = Cast<ABladeIIPlayer>(GetPawn());
	//if (B2WS && MyPlayer)
	//{
	//	B2WS->ApplyPlayerLightSettingForPlayerPawn(MyPlayer);
	//}	
}

void ABladeIIPlayerController::SetPawn(APawn* InPawn)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayerController::SetPawn"));

	// To maintain the camera target when the possessed pawn is switched.
	ABladeIICharacter* CameraTargetBackup = CameraTargetActor;
	SetCameraTarget(NULL); // To set the CameraTargetActor which is actually the same, see SetCameraTarget.

	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(InPawn);
	if (B2Player)
	{
		// ExplicitCameraBoom** �� ���� ���� �ʴٸ� ���� �÷��̾ ��� �ִ� ���̶� ������ ����.
		if (bExplicitCameraBoomPitchSet == false)
		{
			ExplicitCameraBoomPitch = B2Player->GetCameraPitch();
		}
		if (bExplicitCameraBoomYawSet == false)
		{
			ExplicitCameraBoomYaw = B2Player->GetCameraYaw();
		}
		if (bExplicitCameraBoomRollSet == false)
		{
			ExplicitCameraBoomRoll = B2Player->GetCameraRoll();
		}
		if (bExplicitCameraBoomArmLengthSet == false)
		{
			ExplicitCameraBoomArmLength = B2Player->GetCameraDistance();
		}
		if (bExplicitCameraTargetFocusOffsetSet == false)
		{
			ExplicitCameraTargetFocusOffset = B2Player->GetCameraTargetFocusOffset();
		}
		if (bExplicitCameraBoomLockedViewTargetSet == false)
		{
			bExplicitCameraBoomLockedViewTarget = B2Player->GetCameraBoomLockedViewTarget();
		}
	}

	//////////////////////////////
	Super::SetPawn(InPawn);
	//////////////////////////////

	SetCameraTarget(CameraTargetBackup); // ���� CameraTarget ����
	// �̰͵鵵 ���������� ������ ���õ� ���� �ִٸ� ���ο� pawn ���� ���������� �����ؼ� �����ǵ��� �Ѵ�.
	// CameraTargetActor �� �ִ��� �Ϻδ� ���� ��.
	ApplyExplicitCameraBoomParams();

	ApplyWorldPlayerLightSetting();
}

bool ABladeIIPlayerController::IsSkippedByCustomLogic() const
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Player && !B2Player->CanBeTarget())
	{
		return true;
	}

	return false;
}

void ABladeIIPlayerController::FindTarget(bool bInputGuided)
{
	// Find Target...
	ABladeIIPlayer* B2Char	= Cast<ABladeIIPlayer>(GetPawn());

	if (!B2Char)
	{
		return;
	}

	auto* PrevActor = B2Char->TargetActor;
	float TargetDist = B2Char->UpdateTarget(bInputGuided);
	B2Char->UpdateTargetObject();

	/*if (B2Char->TargetActor == NULL || PrevActor != B2Char->TargetActor || TargetDist > B2Char->MaxDashDistance)
	{
		B2Char->StopDash();
	}*/
}

void ABladeIIPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &ABladeIIPlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &ABladeIIPlayerController::OnSetDestinationReleased);

	//// support touch devices 
	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABladeIIPlayerController::MoveToTouchLocation);
	//InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ABladeIIPlayerController::MoveToTouchLocation);
}

void ABladeIIPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ABladeIIPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ABladeIIPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	BII_CHECK(MyPawn);

	//if (MyPawn)
	//{
	//	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	//	float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

	//	// We need to issue move command only if far enough in order for walk animation to play correctly
	//	if (NavSys && (Distance > 120.0f))
	//	{
	//		NavSys->SimpleMoveToLocation(this, DestLocation);
	//	}
	//}
}

void ABladeIIPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ABladeIIPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ABladeIIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//MyAssetStore.SetStartingGold(BladeIIGameImpl::GetClientDataStore().GetGoldAmount());

	//if (!AutoAIController)
	//	AutoAIController = SpawnAutoAIController(FVector::ZeroVector, FRotator::ZeroRotator);

	//UWorld* TheWorld = GetWorld();
	//ABladeIIWorldSettings* B2WS = TheWorld ? Cast<ABladeIIWorldSettings>(TheWorld->GetWorldSettings()) : NULL;
	//if (B2WS)
	//	B2WS->InitDefaultPlayerCameraSetting();
}

void ABladeIIPlayerController::BeginDestroy()
{
	Super::BeginDestroy();

}

void ABladeIIPlayerController::Destroyed()
{
	/*if (AutoAIController)
	{
		AutoAIController->Destroy();
		AutoAIController = nullptr;
	}	*/
}

//ALevelSequenceActor * ABladeIIPlayerController::GetMatineeActor()
//{
//	return MatineeActor;
//}

void ABladeIIPlayerController::SetCinematicMode(bool bInCinematicMode, bool bHidePlayer, bool bAffectsHUD, bool bAffectsMovement, bool bAffectsTurning)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayerController::SetCinematicMode"));

	//// �ó׸�ƽ ��� ī����
	//if (bInCinematicMode)
	//	nCinematicModeCount++;
	//else
	//	nCinematicModeCount--;

	//// �ó׸�ƽ ����� ī��Ʈ0�϶��� ����
	//if (!bInCinematicMode && nCinematicModeCount != 0)
	//	return;

	//// ¦�ȸ��߰� ���Ḹ ȣ���������.. �ʱ�ȭ
	//if (nCinematicModeCount < 0)
	//	nCinematicModeCount = 0;

	//bHUDHidingCinematicMode = (bInCinematicMode && bAffectsHUD); // �̷��� ���� �����ؼ� Widget �ʿ��� �����ϵ���.
	//bInputDisabledCinematicMode = (bInCinematicMode && bAffectsMovement); // AffectsMovement �� ��ü �Է� disable �� ���

	//UB2UIManager::GetInstance()->SetHUDHidingCinematicMode(bInCinematicMode, bHUDHidingCinematicMode);

	//Super::SetCinematicMode(bInCinematicMode, bHidePlayer, bAffectsHUD, bAffectsMovement, bAffectsTurning);

	//ABladeIIPlayer* MyPlayer = Cast<ABladeIIPlayer>(GetPawn());
	//if (MyPlayer && MyPlayer->TargetActor)
	//{
	//	if (bInCinematicMode && bHidePlayer)
	//	{
	//		MyPlayer->TargetActor->SetActorHiddenInGame(true);
	//	}
	//	else if (!bCinematicMode)
	//	{
	//		MyPlayer->TargetActor->SetActorHiddenInGame(false);
	//	}
	//}

	//// FloorRing �� HUD �� ���� ����� �������� ġ��.. ���� ��Ƽ��Ʈ������ SkeletalMeshActor �� ������Ű�� �Ŷ�� ���ʵ� �պ���� �Ѵ�. (AB2StageEventDirector::ActivateMatineePuppetPSCs)
	//if (MyPlayer && MyPlayer->FloorRingPSC)
	//{
	//	if (bHUDHidingCinematicMode){
	//		MyPlayer->FloorRingPSC->Deactivate();
	//		MyPlayer->FloorRingPSC->SetHiddenInGame(true);
	//	}
	//	else{
	//		MyPlayer->FloorRingPSC->SetHiddenInGame(false);
	//		MyPlayer->FloorRingPSC->Activate();
	//	}
	//}

	//// ���� UI ���̱� ���ο� ���� �ó����� ��ȭ���� ��ġ�� �����ؾ� �ؼ�. CinematicMode ������ �⺻ �������� �켱�õǹǷ� ���⼭ ������ ����
	//UB2UIDocScenarioDialog* DocSDG = UB2UIDocHelper::GetDocScenarioDialog();
	//if (DocSDG)
	//{
	//	DocSDG->SetbLastBattleUIHidden(bHUDHidingCinematicMode);
	//}

	//if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//{
	//	DocBattle->SetIsCenematicMode(bHUDHidingCinematicMode);
	//}

	//AB2TutorialGameMode* TutorialGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this));
	//if (TutorialGM)
	//	SetVirtualJoystickVisibility(!bInCinematicMode);//SetVirtualJoystickVisibility(TutorialGM->IsVisibleVirtualJoystick());
}

void ABladeIIPlayerController::DeactivateVitualJoystick()
{
	// �⺻ �ڵ忡���� ���� ����� �Ҹ��� �ʴ� ���ѵ�, VirtualJoystick �� �Ź� ���� ����. ���� ������ �� �̰� �ҷ��� �ʿ䰡 �ִ�. �� ������ BeginDestroy ����.
	if (VirtualJoystick.IsValid())
	{
		ActivateTouchInterface(nullptr);
	}
}

bool ABladeIIPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	//��ġ Began �̺�Ʈ UI�հ� ����Ʈ�� �Է� �Ѿ�Գ�.
	//if (Type == ETouchType::Began)
	//{
	//	NotifyInputTouchBeganInViewPortClass<>::GetInstance().Signal();
	//}

	return Super::InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);
}

void ABladeIIPlayerController::SetStageEndNonCombatState(bool bInValue)
{
	if (bStageEndNonCombatState == bInValue)
		return;

	if (bInValue)
	{
		ABladeIIPlayer* MyPlayer = Cast<ABladeIIPlayer>(GetPawn());
		if (MyPlayer)
		{
			MyPlayer->StopRepeatingInput(true);
		}
	}

	bStageEndNonCombatState = bInValue;
}

void ABladeIIPlayerController::KillAllEnemies()
{
	TArray<ABladeIICharacter*> Characters;
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//BII_CHECK(B2GM);
	//if (B2GM){
	//	B2GM->GetAllWorldMobList(Characters);
	//}

	for (auto* ch : Characters)
	{
		ch->Suicide();
	}
}

//AB2AutoAIController* ABladeIIPlayerController::SpawnAutoAIController(FVector const& InSpawnLocation, FRotator const& InSpawnRotation)
//{
//	FActorSpawnParameters SpawnInfo;
//	SpawnInfo.Instigator = Instigator;
//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
//	SpawnInfo.bDeferConstruction = true;
//	// Fixed class.
//	AB2AutoAIController* NewAC = GetWorld()->SpawnActor<AB2AutoAIController>(AB2AutoAIController::StaticClass(), InSpawnLocation, InSpawnRotation, SpawnInfo);
//	if (NewAC)
//	{
//		UGameplayStatics::FinishSpawningActor(NewAC, FTransform(InSpawnRotation, InSpawnLocation));
//	}
//	BII_CHECK(NewAC);
//	return NewAC;
//}

bool ABladeIIPlayerController::StartAutoPlay(ABladeIIPlayer* avatar)
{
	//// �� �������� ���� ��찡 �߰ߵǴµ� (Extra ������ �̵��ؼ� �ٷ� Auto ����) �÷����̳� ���� ���� ���� BeginPlay Ÿ�̹��� �ٸ� ������ �𸣰ڴ�.
	//if (!AutoAIController){
	//	AutoAIController = SpawnAutoAIController(FVector::ZeroVector, FRotator::ZeroRotator);
	//}
	//
	//BII_CHECK(AutoAIController);

	//if (!AutoAIController)
	//	return false;
	//
	//// It does not make existing player controller unpossesses the player.
	//AutoAIController->Possess(avatar);

	//ABladeIIPlayer* B2Char = Cast<ABladeIIPlayer>(GetPawn());

	//if (B2Char->IsQTEMounting() && B2Char->QTEActor)
	//{
	//	ABladeIIAIController* pAC = Cast<ABladeIIAIController>(B2Char->QTEActor->GetController());

	//	if (pAC)
	//	{
	//		AutoAIController->StopAutoPlay();
	//		pAC->ApplyMountBehaviorTree();
	//	}
	//}

	return true;
}

void ABladeIIPlayerController::StopAutoPlay()
{
	BII_CHECK(AutoAIController);

	//if (!AutoAIController)
	//	return;

	//ABladeIIPlayer* B2Char = Cast<ABladeIIPlayer>(GetPawn());

	//if (B2Char->IsQTEMounting() && B2Char->QTEActor)
	//{
	//	ABladeIIAIController* pAC = Cast<ABladeIIAIController>(B2Char->QTEActor->GetController());

	//	if (pAC)
	//		pAC->StopAutoPlay();
	//}
	//
	//// Now only player controller possesses the player.
	//AutoAIController->UnPossess();
}

void ABladeIIPlayerController::ResetAutoAiController()
{
	//if (AutoAIController)
	//	AutoAIController->ResetAutoPlay();
}

bool ABladeIIPlayerController::MoveToActor(AActor * Goal, float AcceptanceRadius)
{
	//UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
	//if (NavSys == nullptr || Goal == nullptr || GetPawn() == nullptr)
		//return false;

	//UPathFollowingComponent* PFollowComp = nullptr;
	////InitNavigationControl(PFollowComp);
	//if (CachePathFollowingComp == nullptr)
	//	CachePathFollowingComp = FindComponentByClass<UPathFollowingComponent>();

	//if (PFollowComp == nullptr)
	//	return false;

	//if (!PFollowComp->IsPathFollowingAllowed())
		return false;

	//const bool bAlreadyAtGoal = PFollowComp->HasReached(*Goal, EPathFollowingReachMode::OverlapAgentAndGoal);

	//if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	//{
	//	PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
	//		, FAIRequestID::AnyRequest, bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
	//}

	//if (bAlreadyAtGoal)
	//{
	//	PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
	//	return true;
	//}
	//else
	//{
	//	const ANavigationData* NavData = NavSys->GetNavDataForProps(GetNavAgentPropertiesRef());
	//	if (NavData)
	//	{
	//		FPathFindingQuery Query(this, *NavData, GetNavAgentLocation(), Goal->GetActorLocation());
	//		FPathFindingResult Result = NavSys->FindPathSync(Query);
	//		if (Result.IsSuccessful())
	//		{
	//			Result.Path->SetGoalActorObservation(*Goal, 100.0f);

	//			FAIMoveRequest MoveReq(Goal);
	//			MoveReq.SetUsePathfinding(true);
	//			MoveReq.SetAllowPartialPath(true);
	//			MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	//			MoveReq.SetReachTestIncludesAgentRadius(true);
	//			MoveReq.SetCanStrafe(true);

	//			PFollowComp->RequestMove(MoveReq, Result.Path);
	//			return true;
	//		}
	//		else if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	//		{
	//			PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
	//		}
	//	}
	//}
	return false;
}

bool ABladeIIPlayerController::CanPlayerHaveMoveMotion() const
{
	bool bPathFindMove = false;
	//if(CachePathFollowingComp)
	//	bPathFindMove = CachePathFollowingComp->GetStatus() == EPathFollowingStatus::Moving;

	return (IsInCinematicMode() || bDisableMoveInputForActiveCameraTransition || bPathFindMove);
}

UB2SkillRTManager* ABladeIIPlayerController::GetPlayerSkillRTManager()
{
	//if (SkillRTManager == NULL)
	//{
	//	auto* PlayerCharacter = Cast<ABladeIIPlayer>(GetPawn());
	//	if (PlayerCharacter)
	//	{
	//		SkillRTManager = NewObject<UB2SkillRTManager>(this, *FString::Printf(TEXT("SkillRTManager_%s"), *this->GetName()), RF_Transient);
	//	}
	//}
	return SkillRTManager;
}
