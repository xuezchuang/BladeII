
#include "B2LobbySkeletalMeshActor.h"
//#include "BladeII.h"
#include "B2CharPartTouchComponent.h"
#include "B2LobbyGameMode.h"

AB2LobbySkeletalMeshActor::AB2LobbySkeletalMeshActor(const FObjectInitializer& ObjectInitializer)
	:	Super(ObjectInitializer)
{
	bCloseUpState = false;
	bPendingCloseUp = false;

	PrimaryActorTick.bCanEverTick = true;
}

void AB2LobbySkeletalMeshActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
/*	
	if (bPendingCloseUp)
	{
		bPendingCloseUp = false;

		auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));

		if (LobbyGameMode)
			LobbyGameMode->SelectLobbyMainCharacter(PCClass);
	}*/	
}

void AB2LobbySkeletalMeshActor::PlayTouchAnimation(UAnimSequenceBase* TouchAnim)
{
	//if (bCloseUpState && TouchAnim && !bPlayingRotateAnim && bPlayingActionAnim == false)
	//{
	//	auto* Mesh = GetSkeletalMeshComponent();
	//	check(Mesh);

	//	Mesh->SetAnimInstanceClass(nullptr);
	//	Mesh->SetAnimInstanceClass(AnimClass);

	//	PlayAnimation(TouchAnim);

	//	for (auto* AttachedComp : Mesh->GetAttachChildren())
	//	{
	//		auto* AudioComp = Cast<UAudioComponent>(AttachedComp);
	//		if (AudioComp)
	//		{
	//			AudioComp->Stop();
	//		}
	//	}

	//	bPlayingActionAnim = bActionAnimTriggered = true;
	//}
}

void AB2LobbySkeletalMeshActor::SetCloseUpState(bool bSet, bool bPlaySelectionAnim, bool bForceSetState)
{
	if (bCloseUpState == bSet && !bForceSetState)
		return;

	bCloseUpState = bSet;

	TArray<UActorComponent*> Components;
	GetComponents(Components);

	for (auto* Component : Components)
	{
		auto* PartTouchComponent = Cast<UB2CharPartTouchComponent>(Component);
		if (PartTouchComponent)
		{
			PartTouchComponent->SetEnable(bCloseUpState);
		}
	}

	if (bCloseUpState)
	{
		if (bPlaySelectionAnim) {
			PlaySelectAnimation(); // Anim 敲饭捞绰 急琶荤亲捞 瞪 荐 乐澜.
		}
		OnCloseUP();
	}
		
}

void AB2LobbySkeletalMeshActor::NotifyActorOnReleased(FKey ButtonReleased)
{
	Super::NotifyActorOnReleased(ButtonReleased);

	OnTouchedActor();
}

void AB2LobbySkeletalMeshActor::NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchEnd(FingerIndex);

	OnTouchedActor();
}

void AB2LobbySkeletalMeshActor::OnTouchedActor()
{
	//auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	//if (LobbyGameMode && !LobbyGameMode->IsIgnoreTouchEnd())
	//{
	//	if (!bCloseUpState)
	//	{
	//		bPendingCloseUp = true;
	//		return;
	//	}

	//	//饭捞甫 促矫 谨辑 弥利狼 TouchComponent甫 茫磊.
	//	auto* PlayerController = UGameplayStatics::GetLocalPlayerController(this);
	//	check(PlayerController);

	//	FVector2D MousePosition;
	//	UGameViewportClient* ViewportClient = CastChecked<ULocalPlayer>(PlayerController->Player)->ViewportClient;
	//	if (ViewportClient && ViewportClient->GetMousePosition(MousePosition))
	//	{
	//		FHitResult HitResult;
	//		
	//		FVector WorldOrigin;
	//		FVector WorldDirection;
	//		if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldOrigin, WorldDirection) == true)
	//		{
	//			FHitResult SkelMeshHit;
	//			FCollisionQueryParams Params;

	//			auto* SkelMeshComponent = GetSkeletalMeshComponent();
	//			SkelMeshComponent->LineTraceComponent(SkelMeshHit, WorldOrigin, WorldDirection * 1000.f + WorldOrigin, Params);

	//			UB2CharPartTouchComponent* HighestPriorityComponent = nullptr;

	//			TArray<UActorComponent*> Components;
	//			GetComponents(Components);

	//			for (auto* Component : Components)
	//			{
	//				auto* PartTouchComponent = Cast<UB2CharPartTouchComponent>(Component);
	//				if (PartTouchComponent && (HighestPriorityComponent == nullptr || HighestPriorityComponent->TouchPriority > PartTouchComponent->TouchPriority))
	//				{
	//					FHitResult Hit;
	//					
	//					if(PartTouchComponent->LineTraceComponent(Hit, WorldOrigin, WorldDirection * 1000.f + WorldOrigin, Params))
	//					{//Priority啊 臭绊 SkeletalMeshComponent焊促 菊辑 乐绢具窃.
	//						if (HighestPriorityComponent == nullptr || !(SkelMeshHit.bBlockingHit && Hit.Distance > SkelMeshHit.Distance))
	//						HighestPriorityComponent = PartTouchComponent;
	//					}
	//				}
	//			}

	//			if (HighestPriorityComponent != nullptr)
	//			{
	//				PlayTouchAnimation(HighestPriorityComponent->GetInteractionAnim());
	//			}					
	//		}
	//	}		
	//}	
}