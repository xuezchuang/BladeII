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
	
	// �̰� ��ƼŬ�ý��� ����ε�, ���ϰ� Widget �� ���� ���. ������ UserWidget ���� �����Ǿ� �־���.
	if (OwnerPlayer->HiddenMobPointerPSTemplate != NULL)
	{
		HiddenMobPointerPSInst = UGameplayStatics::SpawnEmitterAttached(OwnerPlayer->HiddenMobPointerPSTemplate, OwnerPlayer->GetRootComponent());
	}

	// Static �ϰ� �߰��� ������ �� ������ ���⼭.

	// ó������ Cinematic mode �� ��찡 �ִµ� UpdateManual ���� ĳġ���� ���ϹǷ� �ѹ� üũ
	if (bCachedHUDHidingCinematicMode)
	{
		HideAll();
	}
#if !UE_BUILD_SHIPPING
	extern bool gbHACKInGameUIHiddenByDevCommand;
	if (gbHACKInGameUIHiddenByDevCommand) // ���߿� Ŀ�ǵ�� ���� ��쿡 ����
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
	// DestroyUserWidgets �� �ƹ��͵� ���ϰ� �̰� �ٸ� ���ǿ��� �Ҹ��� �� ��.
	if (FloatingHPBarWidget)
	{
		FloatingHPBarWidget->DestroySelf();
		FloatingHPBarWidget = NULL;
	}

	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget && MasterUIManager)
		{	// �ݳ�
			MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_GoldAcq, GoldAcqWidget);
		}
	}
	GoldAcqWidgetSetArray.Empty();

	for (int32 DWI = 0; DWI < DamageNumWidgetSetArray.Num(); ++DWI)
	{
		UB2FloatingWidget* DamageNumWidget = DamageNumWidgetSetArray[DWI].FloatingWidget;
		if (DamageNumWidget && MasterUIManager)
		{	// �ݳ�
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

	CachedPlayerController->SetVirtualJoystickVisibility(false); // VirtualJoystick �� �ٸ� widget ���� ��¦ �ٸ�.
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
		HiddenMobPointerPSInst->SetVisibility(true); // �ϴ� �� �ش�. ������ bCurrentlyHideAll �� �ƴϸ� ��ƽ �˻��ϸ鼭 visibility �����ϰ� �� ��.
	}

	CachedPlayerController->SetVirtualJoystickVisibility(!HiddenVirtualJoystick);
}

void UB2UIManager_InGameHUDPlayer::HideMobPointer(bool bInHide)
{
	HiddenMobPointerOnlyVisiblityFlag = !bInHide;

	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->SetVisibility(HiddenMobPointerOnlyVisiblityFlag); // ������ bCurrentlyHideAll �� �ƴϸ� ��ƽ �˻��ϸ鼭 visibility �����ϰ� �� ��.
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
		// ���� ������ �ƴ϶� Rent �̴�. ������ �� ���� �ֱ� ����.
		UB2FloatingGoldAcq* NewGoldAcqWidget = Cast<UB2FloatingGoldAcq>(MasterUIManager->RentFloatingWidget(
			EFloatingWidgetPoolClassType::EFWPCT_GoldAcq, CachedPlayerController, OwnerCharacter
		));
		if (NewGoldAcqWidget)
		{			
			// ���� GoldAcqWidget �ϳ��� �����ؼ� �� SuckedGold �̺�Ʈ���� ���� �Ķ���� �����ϴ� ���� �ڵ� �����ε� ���ּ� �ȵ� �͵� ���� ������ �״�� ��.
			// -> ���ױ淡 ����.. �ᱹ FloatWidget Pooling �� �ϸ鼭 ���� ����� �ʿ�����
			NewGoldAcqWidget->NotifyPlayerSuckedGold(AcquiredAmount);

			if (bCurrentlyHideAll) // ���߿� �����Ѵٸ� ����� �� ��Ծ �ȵǰڴ�.
			{
				NewGoldAcqWidget->ForceHide();
			}

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = NewGoldAcqWidget;

			// VisibleDuration ���� �۵��ϴ� ���� Ÿ�̸� ��ġ
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
				// �̸��� ���� �����̶� Destroy �ε� ������ ������ Destroy �� �ƴ϶� pool �� �ݳ��Ѵ�.
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

#if 0 // �� �ָ� �ִ� target ���� ���� ������ ���ŵ��� �ʴ� ä�� �ִ� ��찡 �־ �� �� ������ ���� ã�� ��� ���ϰ� FActorIterator �� ������ �ʵ���..
		bool bFindNewHMPTarget = false;
		// CurrentHMPTarget �� �������� �˻�. �Ź� CurrentHMPTarget �� �����ϱ�� ��δ�..
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
					bFindNewHMPTarget = true; // ȭ�� ������ �������� ���� ã�´�.
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
			CurrentHMPTarget = NULL; // �ϴ� NULL. ��ã���� �� �Ѹ��� ��.

			float ClosestOutScreenDistSquared2D = MAX_FLT; // ȭ�� �ۿ� �ִ� �� �� ���� ����� ���� �Ÿ� ����.

														   // FActorIterator �� ����� ��δ� SpawnPool ���� spawn �� �ֵ� ����Ʈ�� �߷��� �˻�.
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
							// ȭ�� �ۿ� ����. XY ��鿡�� ���� ����� �ɷ� ����.

							float CurrCandidDistSquared2D = (B2Char->GetActorLocation() - OwnerPlayer->GetActorLocation()).SizeSquared2D();

							if (CurrCandidDistSquared2D < ClosestOutScreenDistSquared2D)
							{
								CurrentHMPTarget = B2Char;
								ClosestOutScreenDistSquared2D = CurrCandidDistSquared2D;
							}
						}
						else
						{
							CurrentHMPTarget = NULL; // �� �ϳ��� �þ߿� ���� ���� �ִٸ� �Ѹ��� �ʴ´�.
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
		PlayerToTargetVector.Z = 0.0f; // Z ���� ���� ���
		FVector PlayerToHMPVector = PlayerToTargetVector.GetSafeNormal() * HMP_RadiusFromPlayer;

		// Z ���� �÷��̾� ĳ���� �߹ٴ� �α�����. �ణ �ø���.
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
		
		PlayerToTargetVector.Z = 0.0f; // Z ���� ���� ���

		// �����Ÿ� ���ϸ� ��ǥ��
		if (PlayerToTargetVector.Size() <= pGM->GetControlObjectViewLimitLength())
		{
			HiddenMobPointerPSInst->SetVisibility(false);
			return;
		}

		FVector PlayerToHMPVector = PlayerToTargetVector.GetSafeNormal() * HMP_RadiusFromPlayer;

		// Z ���� �÷��̾� ĳ���� �߹ٴ� �α�����. �ణ �ø���.
		FVector HMPLocation = OwnerPlayer->GetActorLocation() + PlayerToHMPVector +
			FVector(0.0f, 0.0f, -1.0f * OwnerPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 1.0f);

		HiddenMobPointerPSInst->SetWorldLocation(HMPLocation);
		HiddenMobPointerPSInst->SetWorldRotation(PlayerToTargetVector.Rotation());
		HiddenMobPointerPSInst->SetVisibility(true);
	}
}
