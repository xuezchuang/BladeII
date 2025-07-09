#include "BladeII.h"
#include "B2UIPVPBegin.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2PVPGameMode.h"
#include "B2TMGameMode.h"
#include "B2UIPVP1on1BattleMain.h"

UB2UIPVPBegin::UB2UIPVPBegin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FightFxOffsetTime = 0.f;
	FightFxCamDistance = 100.f;
	FightFxScale = 1.f;
}

void UB2UIPVPBegin::Init()
{
	Super::Init();
}

void UB2UIPVPBegin::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (PSCFightFX.IsValid())
	{
		PSCFightFX->DestroyComponent();
	}
}

void UB2UIPVPBegin::CacheAssets()
{
	GET_SLOT(UImage, IMG_Ready);
}

void UB2UIPVPBegin::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ShowFightFXRemainTime < 0.f)
	{
		if (!PSCFightFX.IsValid() || (PSCFightFX->bWasDeactivated || PSCFightFX->bWasCompleted))
		{
			UB2UIManager::GetInstance()->CloseUI(UIFName::PVPBegin);
		}
	}

	if (ShowFightFXRemainTime > 0.f)
	{
		ShowFightFXRemainTime -= InDeltaTime;

		if (ShowFightFXRemainTime < 0.f)
			ShowFightFX();
	}
}

void UB2UIPVPBegin::SetFightFX_PlayTime(float ToSet)
{
	ShowFightFXRemainTime = FMath::Max(ToSet + FightFxOffsetTime, 0.f);
}

void UB2UIPVPBegin::ShowFightFX()
{
	//if (IMG_Ready.IsValid())
	//	IMG_Ready->SetVisibility(ESlateVisibility::Collapsed);

	//if (FightFX == nullptr)
	//	return;

	//if (auto* PVPSceneBattleMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain))
	//{
	//	PVPSceneBattleMainWidget->StartInGameFight();
	//}

	//if (auto* B2PVPGameMode = Cast<AB2PVPGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	//{
	//	B2PVPGameMode->SetStartTimeCount(true);
	//}

	//if (auto* B2TMGameMode = Cast<AB2TMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	//{
	//	B2TMGameMode->SetStartTimeCount(true);
	//}

	//APlayerController* LocalPlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
	//ULocalPlayer* const LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
	//if (LP && LP->ViewportClient)
	//{
	//	FVector2D Center;
	//	LP->ViewportClient->GetViewportSize(Center);
	//	Center *= 0.5f;

	//	FVector WorldLoc;
	//	FVector WorldDir;
	//	LocalPlayerController->DeprojectScreenPositionToWorld(Center.X, Center.Y, WorldLoc, WorldDir);
	//	
	//	auto* CameraManager = UGameplayStatics::GetLocalPlayerCameraManager(GetWorld());
	//	if (CameraManager)
	//	{
	//		PSCFightFX = NewObject<UParticleSystemComponent>(GetWorld()->GetWorldSettings());
	//		PSCFightFX->bAutoDestroy = true;
	//		PSCFightFX->bAllowAnyoneToDestroyMe = true;
	//		PSCFightFX->SecondsBeforeInactive = 0.0f;
	//		PSCFightFX->bAutoActivate = false;
	//		PSCFightFX->SetTemplate(FightFX);
	//		PSCFightFX->bOverrideLODMethod = false;

	//		PSCFightFX->AttachToComponent(CameraManager->GetTransformComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	//		PSCFightFX->SetAbsolute(false, false, true);
	//		PSCFightFX->SetRelativeTransform(FTransform(FRotator(0.f, 180.f, 0.f), FVector(FightFxCamDistance, 0.f, 0.f), FVector(FightFxScale)));

	//		PSCFightFX->RegisterComponentWithWorld(GetWorld());

	//		PSCFightFX->ActivateSystem(true);

	//		OnPlayFightFx_BP();
	//	}
	//}
}
