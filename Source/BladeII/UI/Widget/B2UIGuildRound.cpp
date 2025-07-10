#include "B2UIGuildRound.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"

UB2UIGuildRound::UB2UIGuildRound(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FightFxOffsetTime = 0.f;
	FightFxCamDistance = 100.f;
	FightFxScale = 1.f;
	bStartRound = false;
	bStart = false;
	EndResultAnim = false;
}

void UB2UIGuildRound::Init()
{
	Super::Init();
}

void UB2UIGuildRound::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (PSCFightFX.IsValid())
	{
		PSCFightFX->DestroyComponent();
	}
}

void UB2UIGuildRound::BindDocAuto()
{
	UnbindDoc();

	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	BII_CHECK(GuildDoc);
	if (!GuildDoc)
		return;

	SetDoc(GuildDoc);

	bWin = GuildDoc->GetCurrentRoundWinner() == ETeamIndex::Player;
	int32 CurrentRound = GuildDoc->GetCurrentRound();
	if (RoundTextures.Contains(CurrentRound))
	{
		if (IMG_Round.IsValid())
			IMG_Round->SetBrushFromTexture(RoundTextures[CurrentRound]);
	}
}

void UB2UIGuildRound::UnbindDoc()
{
	SetDoc(nullptr);
}

void UB2UIGuildRound::StartRoundUI(float Delay, bool InStartRound)
{
	bStartRound = InStartRound;

	if (bStartRound)
		ShowRemainTime = FMath::Max(Delay + FightFxOffsetTime, 0.f);
	else
		ShowRemainTime = FMath::Max(Delay + ResultOffsetTime, 0.f);

	bStart = true;
	StartRoundUI_BP(bStartRound);
}

void UB2UIGuildRound::CloseWidgetDelegate()
{

}

void UB2UIGuildRound::CacheAssets()
{
	GET_SLOT(UImage, IMG_Round);
}

void UB2UIGuildRound::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bStart)
		return;

	if (ShowRemainTime < 0.f)
	{
		if (bStartRound)
		{
			if (!PSCFightFX.IsValid() || (PSCFightFX->bWasDeactivated || PSCFightFX->bWasCompleted))
				UB2UIManager::GetInstance()->CloseUI(UIFName::GuildRound);
		}
		else
		{
			if (EndResultAnim)
				UB2UIManager::GetInstance()->CloseUI(UIFName::GuildRound);
		}
	}

	if (ShowRemainTime > 0.f)
	{
		ShowRemainTime -= InDeltaTime;

		if (ShowRemainTime < 0.f)
		{
			if (bStartRound)
				ShowFightFX();
			else
				PlayResult(bWin);
		}
	}

}

void UB2UIGuildRound::ShowFightFX()
{
	//if (IMG_Round.IsValid())
	//	IMG_Round->SetVisibility(ESlateVisibility::Collapsed);

	//if (FightFX == nullptr)
	//	return;

	//APlayerController* LocalPlayerController = UGameplayStatics::GetLocalPlayerController(UUserWidget::GetWorld());
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
	//	auto* CameraManager = UGameplayStatics::GetLocalPlayerCameraManager(UUserWidget::GetWorld());
	//	if (CameraManager)
	//	{
	//		PSCFightFX = NewObject<UParticleSystemComponent>(UUserWidget::GetWorld()->GetWorldSettings());
	//		PSCFightFX->bAutoDestroy = true;
	//		PSCFightFX->bAllowAnyoneToDestroyMe = true;
	//		PSCFightFX->SecondsBeforeInactive = 0.0f;
	//		PSCFightFX->bAutoActivate = false;
	//		PSCFightFX->SetTemplate(FightFX);
	//		PSCFightFX->bOverrideLODMethod = false;

	//		PSCFightFX->AttachToComponent(CameraManager->GetTransformComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	//		PSCFightFX->SetAbsolute(false, false, true);
	//		PSCFightFX->SetRelativeTransform(FTransform(FRotator(0.f, 180.f, 0.f), FVector(FightFxCamDistance, 0.f, 0.f), FVector(FightFxScale)));

	//		PSCFightFX->RegisterComponentWithWorld(UUserWidget::GetWorld());

	//		PSCFightFX->ActivateSystem(true);

	//		OnPlayFightFx_BP();
	//	}
	//}
}

void UB2UIGuildRound::PlayResult(bool InIsWin)
{
	EndResultAnim = false;
	OnPlayResult_BP(InIsWin);
}
