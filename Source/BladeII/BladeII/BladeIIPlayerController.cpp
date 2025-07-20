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
				//FinalTargetYaw = FRotator::ClampAxis(FMath::Atan2(Diff.Y, Diff.X) * 180 / PI); // FVector::Rotation 俊辑 窍绰 扒单.. 拱沸 Yaw 父 鞘夸窍促搁 捞犯霸 窍搁 己瓷俊 档框捞 登摆瘤..
			}
			FRotator CamBoomRotationToTarget = Diff.Rotation();
				
			if (bCameraTargetSetByCameraBoom)
			{
				// SetCameraTarget 俊辑 CameraBoom 阑 荤侩窍档废 沁促搁 CameraTargetBoomRotOffset 狼 Yaw 蔼阑 咯扁辑档 利侩秦具.
				// UpdateCameraTransform 阑 沁促搁.. 肋 葛福摆促 ぇぇ
				CamBoomRotationToTarget.Yaw += CameraTargetActor->CameraTargetBoomRotOffset.Yaw;
			}
				
			//B2Char->UpdateCameraYaw(Utilities::FBlendHelper::BlendRotation(DeltaTime, B2Char->GetCameraYaw(), CamBoomRotationToTarget.Yaw));

			if (bCameraTargetFollowPitchToo == true)
			{
				// Pitch 档 盎脚. 泅犁肺急 CameraTargetSetByCameraBoom 老 锭父 蜡瓤且 淀?
				CamBoomRotationToTarget.Pitch += CameraTargetActor->CameraTargetBoomRotOffset.Pitch;
				B2Char->UpdateCameraPitch(CamBoomRotationToTarget.Pitch);
			}
		}
		else
		{
			CameraTargetActor = NULL;
		}
	}
	
#if !UE_BUILD_SHIPPING // 窍唱挥牢 肺拿 敲饭捞绢 牧飘费矾俊 措秦 阂府绰 Tick 捞骨肺 咯扁啊 利例且 淀.
	ABladeIIPlayer* PossedPlayer = Cast<ABladeIIPlayer>(GetPawn()); // 匙筗葛靛俊急 捞霸 NULL 老 荐 乐促. 弊凡 订 蝶肺 UB2LobbyUI_CharStatDetail 率俊 瘤沥茄 吧肺 荤侩. 辟单 弊订 酒抗 咯扁肺 救 甸绢棵 荐档..
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

		// QTE 牢 版快 乞家俊 矫急瞒窜 困氰拱 汲沥捞 救等 巴档 见辨 荐 乐档废 CustomDurationThreshold 阑 撤霸 霖促. -1 捞搁 DrawInGameCustomDuration 狼 扁夯蔼烙.
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
	// 肺弊 呈公腹捞 朵
	//B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayerController::SetCameraTarget"));

	if (CameraTargetActor != CameraTarget)
	{
		CameraTargetActor = CameraTarget;
		if (CameraTargetActor)
		{
			bExplicitCameraBoomYawSet = false; // CameraTargetActor 啊 技泼登菌促搁 利绢档 Yaw 绰 拌加 弊率阑 氢秦 盎脚瞪 巴捞骨肺 捞扒 荤侩窍瘤 臼澜.
			bCameraTargetSetByCameraBoom = bSetByCameraBoom;
			// 捞扒 荤角惑 SetByCameraBoom 老 锭父?
			bCameraTargetFollowPitchToo = bTargetPitchToo;

			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn());
			if (B2Player)
			{
				// 葛靛俊 蝶扼 檬扁 技泼捞 哎府绰单 捞饶俊绰 PlayerTick 俊辑 付蛮啊瘤 拌魂 利侩.
				if (bCameraTargetSetByCameraBoom)
				{
					// CameraTargetActor 甫 氢窍档废 CameraBoom 阑 炼例. CameraBoom 俊 扁馆茄 墨皋扼 瓤苞客 鞍捞 荤侩 啊瓷
					FVector TargetToPlayerDir = CameraTargetActor->GetActorLocation() - B2Player->GetActorLocation();
					TargetToPlayerDir.Normalize();

					FRotator CamBoomRotationToTarget = TargetToPlayerDir.Rotation();
					if (bCameraTargetFollowPitchToo == false)
					{
						CamBoomRotationToTarget.Pitch = 0.0f; // 霸烙 漂己惑 yaw 父 蝶扼哎 鞘夸啊 乐阑 巴.
					}
					CamBoomRotationToTarget += CameraTargetActor->CameraTargetBoomRotOffset; // Character 率俊 汲沥茄 蔼俊 蝶扼 眠啊炼例.

					B2Player->UpdateCameraPitch(CamBoomRotationToTarget.Pitch);
					B2Player->UpdateCameraYaw(CamBoomRotationToTarget.Yaw);
				}
				else
				{
					// Top-down 墨皋扼 哪器惩飘 transform 阑 流立 函版.
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
	SetCameraBoomYaw(InRotYaw); // 咯扁急 CameraTargetActor 啊 公瓤拳瞪 巴.
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
		SetCameraTarget(NULL); // CameraTarget 捞 乐促搁 捞霸 救冈洒聪 公瓤拳茄促.
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
		SetCameraTarget(NULL); // CameraTarget 捞 乐促搁 捞霸 救冈洒聪 公瓤拳茄促.
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
		// ExplicitCameraBoom** 捞 冈囚 乐瘤 臼促搁 泅犁 敲饭捞绢啊 甸绊 乐绰 蔼捞扼档 啊廉客 焊磊.
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

	SetCameraTarget(CameraTargetBackup); // 捞傈 CameraTarget 汗备
	// 捞巴甸档 付蛮啊瘤肺 捞傈俊 技泼等 利捞 乐促搁 货肺款 pawn 俊档 付蛮啊瘤肺 技泼秦辑 蜡瘤登档废 茄促.
	// CameraTargetActor 啊 乐歹扼档 老何绰 冈鳃 巴.
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

	//// 矫匙付平 葛靛 墨款泼
	//if (bInCinematicMode)
	//	nCinematicModeCount++;
	//else
	//	nCinematicModeCount--;

	//// 矫匙付平 辆丰绰 墨款飘0老锭父 角青
	//if (!bInCinematicMode && nCinematicModeCount != 0)
	//	return;

	//// 娄救嘎眠绊 辆丰父 龋免沁阑鳖毫.. 檬扁拳
	//if (nCinematicModeCount < 0)
	//	nCinematicModeCount = 0;

	//bHUDHidingCinematicMode = (bInCinematicMode && bAffectsHUD); // 捞犯霸 蝶肺 技泼秦辑 Widget 率俊辑 曼炼窍档废.
	//bInputDisabledCinematicMode = (bInCinematicMode && bAffectsMovement); // AffectsMovement 矫 傈眉 涝仿 disable 肺 荤侩

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

	//// FloorRing 篮 HUD 客 鞍捞 见扁绰 己拜栏肺 摹绊.. 拱沸 付萍聪飘发栏肺 SkeletalMeshActor 甫 备悼矫虐绰 芭扼搁 弊率档 颊毫拎具 茄促. (AB2StageEventDirector::ActivateMatineePuppetPSCs)
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

	//// 傈捧 UI 焊捞扁 咯何俊 蝶扼 矫唱府坷 措拳惑磊 困摹甫 技泼秦具 秦辑. CinematicMode 汲沥捞 扁夯 汲沥焊促 快急矫登骨肺 咯扁辑 公炼扒 技泼
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
	// 扁夯 内靛俊辑绰 饭骇 辆丰矫 阂府瘤 臼绰 淀茄单, VirtualJoystick 篮 概锅 货肺 积己. 饭骇 郴妨哎 锭 捞吧 阂矾临 鞘夸啊 乐促. 窜 咯扁狼 BeginDestroy 富绊.
	if (VirtualJoystick.IsValid())
	{
		ActivateTouchInterface(nullptr);
	}
}

bool ABladeIIPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	//磐摹 Began 捞亥飘 UI墩绊 轰器飘肺 涝仿 逞绢吭唱.
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
	//// 捞 矫痢俊辑 绝绰 版快啊 惯斑登绰单 (Extra 甘栏肺 捞悼秦辑 官肺 Auto 矫累) 敲阀汽捞唱 呼靛 汲沥 蝶扼 BeginPlay 鸥捞怪捞 促弗 扒瘤档 葛福摆促.
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
