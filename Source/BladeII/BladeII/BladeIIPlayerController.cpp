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
				//FinalTargetYaw = FRotator::ClampAxis(FMath::Atan2(Diff.Y, Diff.X) * 180 / PI); // FVector::Rotation 에서 하는 건데.. 물론 Yaw 만 필요하다면 이렇게 하면 성능에 도움이 되겠지..
			}
			FRotator CamBoomRotationToTarget = Diff.Rotation();
				
			if (bCameraTargetSetByCameraBoom)
			{
				// SetCameraTarget 에서 CameraBoom 을 사용하도록 했다면 CameraTargetBoomRotOffset 의 Yaw 값을 여기서도 적용해야.
				// UpdateCameraTransform 을 했다면.. 잘 모르겠다 ㄷㄷ
				CamBoomRotationToTarget.Yaw += CameraTargetActor->CameraTargetBoomRotOffset.Yaw;
			}
				
			//B2Char->UpdateCameraYaw(Utilities::FBlendHelper::BlendRotation(DeltaTime, B2Char->GetCameraYaw(), CamBoomRotationToTarget.Yaw));

			if (bCameraTargetFollowPitchToo == true)
			{
				// Pitch 도 갱신. 현재로선 CameraTargetSetByCameraBoom 일 때만 유효할 듯?
				CamBoomRotationToTarget.Pitch += CameraTargetActor->CameraTargetBoomRotOffset.Pitch;
				B2Char->UpdateCameraPitch(CamBoomRotationToTarget.Pitch);
			}
		}
		else
		{
			CameraTargetActor = NULL;
		}
	}
	
#if !UE_BUILD_SHIPPING // 하나뿐인 로컬 플레이어 컨트롤러에 대해 불리는 Tick 이므로 여기가 적절할 듯.
	ABladeIIPlayer* PossedPlayer = Cast<ABladeIIPlayer>(GetPawn()); // 네퉉모드에선 이게 NULL 일 수 있다. 그럴 땐 따로 UB2LobbyUI_CharStatDetail 쪽에 지정한 걸로 사용. 근데 그땐 아예 여기로 안 들어올 수도..
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

		// QTE 인 경우 평소에 시선차단 위험물 설정이 안된 것도 숨길 수 있도록 CustomDurationThreshold 을 낮게 준다. -1 이면 DrawInGameCustomDuration 의 기본값임.
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
	// 로그 너무많이 뜸
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayerController::SetCameraTarget"));

	if (CameraTargetActor != CameraTarget)
	{
		CameraTargetActor = CameraTarget;
		if (CameraTargetActor)
		{
			bExplicitCameraBoomYawSet = false; // CameraTargetActor 가 세팅되었다면 적어도 Yaw 는 계속 그쪽을 향해 갱신될 것이므로 이건 사용하지 않음.
			bCameraTargetSetByCameraBoom = bSetByCameraBoom;
			// 이건 사실상 SetByCameraBoom 일 때만?
			bCameraTargetFollowPitchToo = bTargetPitchToo;

			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
			if (B2Player)
			{
				// 모드에 따라 초기 세팅이 갈리는데 이후에는 PlayerTick 에서 마찬가지 계산 적용.
				if (bCameraTargetSetByCameraBoom)
				{
					// CameraTargetActor 를 향하도록 CameraBoom 을 조절. CameraBoom 에 기반한 카메라 효과와 같이 사용 가능
					FVector TargetToPlayerDir = CameraTargetActor->GetActorLocation() - B2Player->GetActorLocation();
					TargetToPlayerDir.Normalize();

					FRotator CamBoomRotationToTarget = TargetToPlayerDir.Rotation();
					if (bCameraTargetFollowPitchToo == false)
					{
						CamBoomRotationToTarget.Pitch = 0.0f; // 게임 특성상 yaw 만 따라갈 필요가 있을 것.
					}
					CamBoomRotationToTarget += CameraTargetActor->CameraTargetBoomRotOffset; // Character 쪽에 설정한 값에 따라 추가조절.

					B2Player->UpdateCameraPitch(CamBoomRotationToTarget.Pitch);
					B2Player->UpdateCameraYaw(CamBoomRotationToTarget.Yaw);
				}
				else
				{
					// Top-down 카메라 컴포넌트 transform 을 직접 변경.
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
	SetCameraBoomYaw(InRotYaw); // 여기선 CameraTargetActor 가 무효화될 것.
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
		SetCameraTarget(NULL); // CameraTarget 이 있다면 이게 안먹히니 무효화한다.
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
		SetCameraTarget(NULL); // CameraTarget 이 있다면 이게 안먹히니 무효화한다.
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
		// ExplicitCameraBoom** 이 먹혀 있지 않다면 현재 플레이어가 들고 있는 값이라도 가져와 보자.
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

	SetCameraTarget(CameraTargetBackup); // 이전 CameraTarget 복구
	// 이것들도 마찬가지로 이전에 세팅된 적이 있다면 새로운 pawn 에도 마찬가지로 세팅해서 유지되도록 한다.
	// CameraTargetActor 가 있더라도 일부는 먹힐 것.
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

	//// 시네마틱 모드 카운팅
	//if (bInCinematicMode)
	//	nCinematicModeCount++;
	//else
	//	nCinematicModeCount--;

	//// 시네마틱 종료는 카운트0일때만 실행
	//if (!bInCinematicMode && nCinematicModeCount != 0)
	//	return;

	//// 짝안맞추고 종료만 호출했을까봐.. 초기화
	//if (nCinematicModeCount < 0)
	//	nCinematicModeCount = 0;

	//bHUDHidingCinematicMode = (bInCinematicMode && bAffectsHUD); // 이렇게 따로 세팅해서 Widget 쪽에서 참조하도록.
	//bInputDisabledCinematicMode = (bInCinematicMode && bAffectsMovement); // AffectsMovement 시 전체 입력 disable 로 사용

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

	//// FloorRing 은 HUD 와 같이 숨기는 성격으로 치고.. 물론 마티니트랙으로 SkeletalMeshActor 를 구동시키는 거라면 그쪽도 손봐줘야 한다. (AB2StageEventDirector::ActivateMatineePuppetPSCs)
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

	//// 전투 UI 보이기 여부에 따라 시나리오 대화상자 위치를 세팅해야 해서. CinematicMode 설정이 기본 설정보다 우선시되므로 여기서 무조건 세팅
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
	// 기본 코드에서는 레벨 종료시 불리지 않는 듯한데, VirtualJoystick 은 매번 새로 생성. 레벨 내려갈 때 이걸 불러줄 필요가 있다. 단 여기의 BeginDestroy 말고.
	if (VirtualJoystick.IsValid())
	{
		ActivateTouchInterface(nullptr);
	}
}

bool ABladeIIPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	//터치 Began 이벤트 UI뚫고 뷰포트로 입력 넘어왔나.
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
	//// 이 시점에서 없는 경우가 발견되는데 (Extra 맵으로 이동해서 바로 Auto 시작) 플랫폼이나 빌드 설정 따라 BeginPlay 타이밍이 다른 건지도 모르겠다.
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
