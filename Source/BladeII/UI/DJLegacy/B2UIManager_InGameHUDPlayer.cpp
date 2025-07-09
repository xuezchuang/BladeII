// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager_InGameHUDPlayer.h"
#include "BladeIINetPlayer.h"
#include "BladeIIPlayerController.h"
#include "B2FloatingGoldAcq.h"
#include "B2FloatingDamageNum.h"
#include "BladeIIGameMode.h"
#include "B2StageGameMode.h"
#include "B2StageManager.h"
#include "B2FloatingHPBar.h"
#include "B2ControlGameMode.h"
#include "B2PVPGameMode.h"
#include "B2RaidGameMode.h"
#include "B2UIManager_InGameCombat.h"
#include "BladeIIUtil.h"

UB2UIManager_InGameHUDPlayer::UB2UIManager_InGameHUDPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GConfig->GetFloat(TEXT("/Script/BladeII.B2HiddenMobPointer"), TEXT("RadiusFromPlayer"), HMP_RadiusFromPlayer, GGameIni);
	CurrentHMPTarget = NULL;
	HiddenMobPointerOnlyVisiblityFlag = true;
}

bool UB2UIManager_InGameHUDPlayer::InitUIManager(class ABladeIICharacter* InOwner, AB2UIManager_InGameCombat* InMasterManager)
{
	if(Super::InitUIManager(InOwner, InMasterManager) == false)
	{
		return false;
	}

	OwnerPlayer = Cast<ABladeIIPlayer>(OwnerCharacter);
	if (OwnerPlayer == NULL)
	{
		return false;
	}
	
	// 이건 파티클시스템 기반인데, 여하간 Widget 과 같이 취급. 이전엔 UserWidget 으로 구현되어 있었음.
	if (OwnerPlayer->HiddenMobPointerPSTemplate != NULL)
	{
		HiddenMobPointerPSInst = UGameplayStatics::SpawnEmitterAttached(OwnerPlayer->HiddenMobPointerPSTemplate, OwnerPlayer->GetRootComponent());
	}

	// Static 하게 추가로 생성할 게 있으며 여기서.

	// 처음부터 Cinematic mode 인 경우가 있는데 UpdateManual 에서 캐치하지 못하므로 한번 체크
	if (bCachedHUDHidingCinematicMode)
	{
		HideAll();
	}
#if !UE_BUILD_SHIPPING
	extern bool gbHACKInGameUIHiddenByDevCommand;
	if (gbHACKInGameUIHiddenByDevCommand) // 개발용 커맨드로 숨긴 경우에 대해
	{
		HideAll();
	}
#endif

	return true;
}

void UB2UIManager_InGameHUDPlayer::UpdateManual(float DeltaSeconds)
{
	Super::UpdateManual(DeltaSeconds);
	
	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget)
		{
			GoldAcqWidget->UpdatePosition();
		}
	}

	if (HiddenMobPointerPSInst)
	{
		auto CurrentGameMode = GetB2GameModeType(UGameplayStatics::GetGameMode(OwnerPlayer));
		if (CurrentGameMode == EB2GameMode::Control)
			UpdateConquestAreaPointerForControl();
		else if(CurrentGameMode == EB2GameMode::PVP_Tag)
			UpdateConquestAreaPointerForPvP();
		else
			UpdateHiddenMobPointer();
	}

}

void UB2UIManager_InGameHUDPlayer::DestroyPlayerWidgets()
{
	// DestroyUserWidgets 은 아무것도 안하고 이건 다른 조건에서 불리게 될 것.
	if (FloatingHPBarWidget)
	{
		FloatingHPBarWidget->DestroySelf();
		FloatingHPBarWidget = NULL;
	}

	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget && MasterUIManager)
		{	// 반납
			MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_GoldAcq, GoldAcqWidget);
		}
	}
	GoldAcqWidgetSetArray.Empty();

	for (int32 DWI = 0; DWI < DamageNumWidgetSetArray.Num(); ++DWI)
	{
		UB2FloatingWidget* DamageNumWidget = DamageNumWidgetSetArray[DWI].FloatingWidget;
		if (DamageNumWidget && MasterUIManager)
		{	// 반납
			MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_DamageNum_PC, DamageNumWidget);
			OwnerCharacter->GetWorldTimerManager().ClearTimer(DamageNumWidgetSetArray[DWI].WidgetDestroyTimerHandle);
		}
	}
	DamageNumWidgetSetArray.Empty();

	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->ConditionalBeginDestroy();
		HiddenMobPointerPSInst = NULL;
	}
}

void UB2UIManager_InGameHUDPlayer::HideAll()
{
	Super::HideAll();

	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget)
		{
			GoldAcqWidget->ForceHide();
		}
	}
	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->SetVisibility(false);
	}

	CachedPlayerController->SetVirtualJoystickVisibility(false); // VirtualJoystick 은 다른 widget 과는 살짝 다름.
}

void UB2UIManager_InGameHUDPlayer::RestoreVisibilityAll()
{
	Super::RestoreVisibilityAll();

	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget)
		{
			GoldAcqWidget->RestoreFromForceHidden();
		}
	}
	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->SetVisibility(true); // 일단 켜 준다. 어차피 bCurrentlyHideAll 이 아니면 매틱 검사하면서 visibility 세팅하게 될 것.
	}

	CachedPlayerController->SetVirtualJoystickVisibility(!HiddenVirtualJoystick);
}

void UB2UIManager_InGameHUDPlayer::HideMobPointer(bool bInHide)
{
	HiddenMobPointerOnlyVisiblityFlag = !bInHide;

	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->SetVisibility(HiddenMobPointerOnlyVisiblityFlag); // 어차피 bCurrentlyHideAll 이 아니면 매틱 검사하면서 visibility 세팅하게 될 것.
	}
}

void UB2UIManager_InGameHUDPlayer::NotifyBeginPlay()
{
	Super::NotifyBeginPlay();

}
void UB2UIManager_InGameHUDPlayer::NotifyStageBegin()
{

}

void UB2UIManager_InGameHUDPlayer::NotifyTargetDamaged(int32 SuccessiveComboNum, float DamageAmount)
{

}

void UB2UIManager_InGameHUDPlayer::NotifyAutoStateChange(bool bInAutoNow)
{

}
void UB2UIManager_InGameHUDPlayer::NotifyPlayerTagged(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer)
{
}

void UB2UIManager_InGameHUDPlayer::NotifyPlayerSuckedGold(int32 AcquiredAmount)
{
	// Create a new one for floating gold acquire notification text
	CreateSingleGoldAcqWidget(AcquiredAmount);

}

UB2FloatingGoldAcq* UB2UIManager_InGameHUDPlayer::CreateSingleGoldAcqWidget(int32 AcquiredAmount)
{
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
	extern bool gDisableInGameFloatingWidgets;
	if (gDisableInGameFloatingWidgets) {
		return nullptr;
	}
#endif

	if (MasterUIManager)
	{		
		// 이제 생성이 아니라 Rent 이다. 생성이 될 수도 있긴 하지.
		UB2FloatingGoldAcq* NewGoldAcqWidget = Cast<UB2FloatingGoldAcq>(MasterUIManager->RentFloatingWidget(
			EFloatingWidgetPoolClassType::EFWPCT_GoldAcq, CachedPlayerController, OwnerCharacter
		));
		if (NewGoldAcqWidget)
		{			
			// 원래 GoldAcqWidget 하나만 생성해서 매 SuckedGold 이벤트마다 새로 파라미터 세팅하던 이전 코드 잔재인데 놔둬서 안될 것도 없어 보여서 그대로 씀.
			// -> 놔뒀길래 다행.. 결국 FloatWidget Pooling 을 하면서 이전 방식이 필요해짐
			NewGoldAcqWidget->NotifyPlayerSuckedGold(AcquiredAmount);

			if (bCurrentlyHideAll) // 도중에 생성한다면 숨기는 걸 까먹어선 안되겠다.
			{
				NewGoldAcqWidget->ForceHide();
			}

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = NewGoldAcqWidget;

			// VisibleDuration 으로 작동하는 해제 타이머 설치
			OwnerPlayer->GetWorldTimerManager().SetTimer(NewWidgetSet.WidgetDestroyTimerHandle,
				FTimerDelegate::CreateUObject(this, &UB2UIManager_InGameHUDPlayer::DestroySingleGoldAcqWidgetTimerCB, NewGoldAcqWidget),
				NewGoldAcqWidget->GetVisibleDuration(), false);

			GoldAcqWidgetSetArray.Add(NewWidgetSet);

			return NewGoldAcqWidget;
		}
	}

	return NULL;
}

void UB2UIManager_InGameHUDPlayer::DestroySingleGoldAcqWidgetTimerCB(class UB2FloatingGoldAcq* WidgetToDestroy)
{
	if (WidgetToDestroy && MasterUIManager)
	{
		for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
		{
			UB2FloatingWidget* CurrDW = GoldAcqWidgetSetArray[GAI].FloatingWidget;
			if (CurrDW == WidgetToDestroy)
			{
				// 이름이 옛날 흔적이라 Destroy 인데 실제로 이제는 Destroy 가 아니라 pool 에 반납한다.
				MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_GoldAcq, WidgetToDestroy);
				OwnerCharacter->GetWorldTimerManager().ClearTimer(GoldAcqWidgetSetArray[GAI].WidgetDestroyTimerHandle);
				GoldAcqWidgetSetArray.RemoveAt(GAI);
				break;
			}
		}
	}
}

void UB2UIManager_InGameHUDPlayer::SetCenterMessage(FText& InMessageText, float DisplayTime, int32 AnimIndex /*= -1*/)
{

}

void UB2UIManager_InGameHUDPlayer::HideCenterMessage()
{

}

void UB2UIManager_InGameHUDPlayer::HideCenterMessageTimerCB()
{
	if (OwnerPlayer)
	{
		OwnerPlayer->GetWorldTimerManager().ClearTimer(CenterMessageHideTimerHandle);
	}
	HideCenterMessage();
}

void UB2UIManager_InGameHUDPlayer::HideVirtualJoystick(bool bInHide)
{
	HiddenVirtualJoystick = bInHide;
	CachedPlayerController->SetVirtualJoystickVisibility(!HiddenVirtualJoystick);
}

void UB2UIManager_InGameHUDPlayer::UpdateHiddenMobPointer()
{
	if (bCurrentlyHideAll || !HiddenMobPointerOnlyVisiblityFlag)
	{
		HiddenMobPointerPSInst->SetVisibility(false);
		return;
	}

	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(OwnerPlayer->Controller);

	if (B2PC)
	{
		int32 ScreenSizeX = 0;
		int32 ScreenSizeY = 0;
		B2PC->GetViewportSize(ScreenSizeX, ScreenSizeY);

#if 0 // 저 멀리 있는 target 으로 인해 적절히 갱신되지 않는 채로 있는 경우가 있어서 걍 매 프레임 새로 찾고 대신 과하게 FActorIterator 를 돌지는 않도록..
		bool bFindNewHMPTarget = false;
		// CurrentHMPTarget 을 유지할지 검사. 매번 CurrentHMPTarget 을 갱신하기는 비싸다..
		//if (CurrentHMPTarget == NULL || CurrentHMPTarget->IsPendingKill() || CurrentHMPTarget->IsAlive() == false)
		{
			bFindNewHMPTarget = true;
		}
		else
		{
			FVector2D CurrentTargetScreenLocation(0.0f, 0.0f);
			if (B2PC->ProjectWorldLocationToScreen(CurrentHMPTarget->GetActorLocation(), CurrentTargetScreenLocation))
			{
				if (CurrentTargetScreenLocation.X >= 0.0f && CurrentTargetScreenLocation.X <= (float)ScreenSizeX &&
					CurrentTargetScreenLocation.Y >= 0.0f && CurrentTargetScreenLocation.Y <= (float)ScreenSizeY)
				{
					bFindNewHMPTarget = true; // 화면 안으로 들어왔으니 새로 찾는다.
				}
			}
			else
			{
				bFindNewHMPTarget = true;
			}
		}

		if (bFindNewHMPTarget)
#endif
		{
			CurrentHMPTarget = NULL; // 일단 NULL. 못찾으면 안 뿌리는 거.

			float ClosestOutScreenDistSquared2D = MAX_FLT; // 화면 밖에 있는 몹 중 가장 가까운 것의 거리 제곱.

														   // FActorIterator 를 돌기는 비싸니 SpawnPool 에서 spawn 된 애들 리스트만 추려서 검사.
			TArray<class ABladeIICharacter*> AllWaveMobList;
			ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(OwnerPlayer));
			if (B2GM)
			{
				B2GM->GetAllWorldMobList(AllWaveMobList);
			}
			for (int32 MI = 0; MI < AllWaveMobList.Num(); ++MI)
			{
				ABladeIICharacter* B2Char = AllWaveMobList[MI];
				if (B2Char)
				{
					FVector2D CandidScreenLocation(0.0f, 0.0f);
					if (B2PC->ProjectWorldLocationToScreen(B2Char->GetActorLocation(), CandidScreenLocation))
					{
						if (CandidScreenLocation.X < 0.0f || CandidScreenLocation.X >(float)ScreenSizeX || CandidScreenLocation.Y < 0.0f || CandidScreenLocation.Y >(float)ScreenSizeY)
						{
							// 화면 밖에 있음. XY 평면에서 가장 가까운 걸로 선택.

							float CurrCandidDistSquared2D = (B2Char->GetActorLocation() - OwnerPlayer->GetActorLocation()).SizeSquared2D();

							if (CurrCandidDistSquared2D < ClosestOutScreenDistSquared2D)
							{
								CurrentHMPTarget = B2Char;
								ClosestOutScreenDistSquared2D = CurrCandidDistSquared2D;
							}
						}
						else
						{
							CurrentHMPTarget = NULL; // 단 하나라도 시야에 들어온 것이 있다면 뿌리지 않는다.
							break;
						}
					}
				}
			}
		}
		VisiblePointer();
	}
}

void UB2UIManager_InGameHUDPlayer::UpdateConquestAreaPointerForPvP()
{
	if (auto* pGM = Cast<AB2PVPGameMode>(UGameplayStatics::GetGameMode(OwnerPlayer)))
	{
		auto* B2Char = pGM->GetOpponent();
		auto* B2PC = pGM->GetLocalController();

		CurrentHMPTarget = NULL;

		if (B2Char && B2PC)
		{
			FVector2D CandidScreenLocation(0.0f, 0.0f);

			if (B2PC->ProjectWorldLocationToScreen(B2Char->GetActorLocation(), CandidScreenLocation))
			{
				int32 ScreenSizeX = 0;
				int32 ScreenSizeY = 0;

				B2PC->GetViewportSize(ScreenSizeX, ScreenSizeY);

				if (CandidScreenLocation.X < 0.0f || CandidScreenLocation.X >(float)ScreenSizeX || CandidScreenLocation.Y < 0.0f || CandidScreenLocation.Y >(float)ScreenSizeY)
				{
					CurrentHMPTarget = B2Char;
				}
			}
		}

		VisiblePointer();
	}
}

void UB2UIManager_InGameHUDPlayer::VisiblePointer()
{
	if (CurrentHMPTarget)
	{
		FVector PlayerToTargetVector = (CurrentHMPTarget->GetActorLocation() - OwnerPlayer->GetActorLocation());
		PlayerToTargetVector.Z = 0.0f; // Z 값은 따로 계산
		FVector PlayerToHMPVector = PlayerToTargetVector.GetSafeNormal() * HMP_RadiusFromPlayer;

		// Z 값은 플레이어 캐릭터 발바닥 부근으로. 약간 올린다.
		FVector HMPLocation = OwnerPlayer->GetActorLocation() + PlayerToHMPVector +
			FVector(0.0f, 0.0f, -1.0f * OwnerPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 1.0f);

		HiddenMobPointerPSInst->SetWorldLocation(HMPLocation);
		HiddenMobPointerPSInst->SetWorldRotation(PlayerToTargetVector.Rotation());
		HiddenMobPointerPSInst->SetVisibility(true);
	}
	else
	{
		HiddenMobPointerPSInst->SetVisibility(false);
	}
}

void UB2UIManager_InGameHUDPlayer::UpdateConquestAreaPointerForControl()
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(OwnerPlayer));

	if (pGM)
	{
		FVector PlayerToTargetVector = (pGM->GetControlObjectLocation() - OwnerPlayer->GetActorLocation());
		
		PlayerToTargetVector.Z = 0.0f; // Z 값은 따로 계산

		// 일정거리 이하면 안표시
		if (PlayerToTargetVector.Size() <= pGM->GetControlObjectViewLimitLength())
		{
			HiddenMobPointerPSInst->SetVisibility(false);
			return;
		}

		FVector PlayerToHMPVector = PlayerToTargetVector.GetSafeNormal() * HMP_RadiusFromPlayer;

		// Z 값은 플레이어 캐릭터 발바닥 부근으로. 약간 올린다.
		FVector HMPLocation = OwnerPlayer->GetActorLocation() + PlayerToHMPVector +
			FVector(0.0f, 0.0f, -1.0f * OwnerPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 1.0f);

		HiddenMobPointerPSInst->SetWorldLocation(HMPLocation);
		HiddenMobPointerPSInst->SetWorldRotation(PlayerToTargetVector.Rotation());
		HiddenMobPointerPSInst->SetVisibility(true);
	}
}
