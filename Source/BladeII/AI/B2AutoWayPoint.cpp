// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square
#include "B2AutoWayPoint.h"
//#include "BladeII.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/ShapeComponent.h"
#include "BladeIIPlayer.h"
#include "B2AutoWayPointMarker.h"
#include "B2AutoAIController.h"
#include "B2MonsterSpawnPool.h"
#include "B2StageManager.h"
#include "BladeIIGameMode.h"
#include "B2JumpEventZone.h"
#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

// Sets default values
AB2AutoWayPoint::AB2AutoWayPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Settings almost from AInfo. Don't want this to be tickable.
	PrimaryActorTick.bCanEverTick	= false;
	bReplicates						= false;
	NetUpdateFrequency				= 10.0f;
	SetReplicateMovement(false);
	SetCanBeDamaged(false);

	// This can check player overlap for EAWPPC_Overlap, as well as being the root component.
	DefaultCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DefaultCollisionComponent"));
	SetRootComponent(DefaultCollisionComponent);
	DefaultCollisionComponent->InitBoxExtent(FVector(200.0f, 200.0f, 100.0f));
	DefaultCollisionComponent->SetCollisionProfileName(CollisionProfileName_OverlapMostDynamic);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	EditorSpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("AutoWayPointSprite"));
	if (!IsRunningCommandlet() && (EditorSpriteComponent != nullptr))
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName ID_B2AutoWayPoint;
			FText NAME_B2AutoWayPoint;
			FConstructorStatics()
				: SpriteTexture(TEXT("/Engine/EditorResources/Waypoint")) // Editor has waypoint texture resource..
				, ID_B2AutoWayPoint(TEXT("B2AutoWayPoint"))
				, NAME_B2AutoWayPoint(FText::FromString(TEXT("B2AutoWayPointEditorSprite")))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		EditorSpriteComponent->Sprite					= ConstructorStatics.SpriteTexture.Get();
		EditorSpriteComponent->SpriteInfo.Category		= ConstructorStatics.ID_B2AutoWayPoint;
		EditorSpriteComponent->SpriteInfo.DisplayName	= ConstructorStatics.NAME_B2AutoWayPoint;
		EditorSpriteComponent->AttachToComponent(DefaultCollisionComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		EditorSpriteComponent->bIsScreenSizeScaled		= true;
		EditorSpriteComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

#endif

	// To not forget setting PassOrder, set default PassOrder as invalid.
	PassOrder				= -1;
	bAttackActionOnArrive	= false;
	bPassed					= false;

#if WITH_EDITOR
	UTextRenderComponent* InfoTextDisplay = CreateEditorOnlyDefaultSubobject<UTextRenderComponent>(*GetInfoTextDisplayName());
	if (InfoTextDisplay)
	{
		InfoTextDisplay->AttachToComponent(DefaultCollisionComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		InfoTextDisplay->SetTextRenderColor(FColor(255, 0, 0));
		InfoTextDisplay->SetText(FText::FromString(FString::Printf(TEXT("WP %d"), PassOrder)));
		InfoTextDisplay->SetRelativeLocation(FVector(0.0f,0.0f,100.0f));
		InfoTextDisplay->SetVisibility(true);
		InfoTextDisplay->SetHiddenInGame(true);
		InfoTextDisplay->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InfoTextDisplay->SetHorizontalAlignment(EHTA_Center);
		InfoTextDisplay->SetWorldSize(50.0f);
	}
#endif
}

void AB2AutoWayPoint::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

#if WITH_EDITOR
	if (GIsEditor)
	{
		// Let's see if this is needed for game too..?
		EnsureUniquePassOrderForCurrentWorld();
		UpdateInfoText();
	}
#endif
}

// Called when the game starts or when spawned
void AB2AutoWayPoint::BeginPlay()
{
	Super::BeginPlay();
#if !WITH_EDITOR
	TArray<UActorComponent*> AllComps = GetComponentsByTag(USphereComponent::StaticClass(), TEXT("PathObject"));
	for (int TI = 0; TI < AllComps.Num(); ++TI)
	{
		USphereComponent* Delete = static_cast<USphereComponent*>(AllComps[TI]);
		Delete->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
		Delete->UnregisterComponent();
		Delete->ConditionalBeginDestroy();
	}
#endif

	bPassed = false;

	FActorSpawnParameters SpawnInfo;
	FTransform trm = GetTransform();

#if WITH_EDITOR
	if (!this->HasValidPassOrder())
		return;

	// Check if we got any "unexpected" duplicated order.
	// If we encounter the warning here, it's probably due to the level streaming..

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	BII_CHECK(B2GM);
	if (B2GM)
	{
		for (auto* waypoint : B2GM->GetWayPointList())
		{
			if (!(waypoint != this && waypoint->HasValidPassOrder() && waypoint->PassOrder == this->PassOrder))
				continue;

#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[AB2AutoWayPoint][WARNING!] Duplicated Waypoint PassOrder!:\n\nObject Name: %s, PassOrder: %d"), *this->GetName(), this->PassOrder)
				));
#endif
			break;
		}
	}
#endif
}

void AB2AutoWayPoint::MarkPassed(bool bMarkPrevious, bool bInAutoPlay, class AB2AutoAIController* AutoController)
{
	bPassed = true;

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	BII_CHECK(B2GM);
	if (bMarkPrevious && B2GM)
	{
		for (auto* waypoint : B2GM->GetWayPointList())
		{
			if (!(waypoint && waypoint->PassOrder < this->PassOrder && waypoint->IsPassed() == false))
				continue;
			waypoint->MarkPassed(false, false, NULL);
		}
	}

	if (bInAutoPlay && AutoController)
	{
		// Do some auto AI stuff if necessary.
		auto* B2Player = Cast<ABladeIIPlayer>(AutoController->GetPawn());
		if (B2Player && bAttackActionOnArrive)
		{
			// Do the bluffing action kk
			B2Player->StartAttack();
		}

		//AutoController->TargetToNextWayPoint();
	}
}


void AB2AutoWayPoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
		
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	if (!B2Player || bPassed)
		return;
	
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GM && B2GM->IsLocalPlayerAutoBattle() == false) // B2Player->IsInAutoPlay() 로 검사하면 연출 도중 통과 시 실제로는 Auto 여도 Auto가 아닌 걸로 판별될 수 있다.
	{		
		// See if we can pass here. There could be additional condition.
		MarkPassed(true, false, NULL); // Auto 아닌 상황에서 통과시에는 이전 꺼 다 통과한 걸로 처리.
	}			
	// In autoplay mode, let it passed only for the current target.
	else if (auto* AutoAIController = B2Player->GetAutoAIController())
	{
		if(AutoAIController->CurrentTargetWP == this)
			MarkPassed(false, true, AutoAIController);
	}
}

#if WITH_EDITOR
void AB2AutoWayPoint::EditorGetAllWayPointsInWorld(class UWorld* InWorld, TArray<AB2AutoWayPoint*>& OutWPArray, bool bSort)
{
	if (!InWorld || !GIsEditor)
		return;
	// Play-in-Editor 도 아닌 에디터 뷰포트 상황에서 사용을 위함.
	for (FActorIterator It(InWorld); It; ++It)
	{
		AB2AutoWayPoint* WP = Cast<AB2AutoWayPoint>(*It);
		if (!WP)
			continue;
		
		OutWPArray.Add(WP);
	}

	if (bSort)
	{
		// Sort the array according to the PassOrder. (ascending)
		OutWPArray.Sort([](const AB2AutoWayPoint& A, const AB2AutoWayPoint& B) { return A.GetPassOrder() < B.GetPassOrder(); });
	}
}
#endif

void AB2AutoWayPoint::EnsureUniquePassOrderForCurrentWorld()
{
	TArray<AB2AutoWayPoint*> WPArray;

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GM)
	{
		WPArray = B2GM->GetWayPointList();
	}
#if WITH_EDITOR
	else if (GIsEditor)
	{
		EditorGetAllWayPointsInWorld(GetWorld(), WPArray, true);
	}
#endif
		
	// Start from second element.
	for (int32 WI = 1; WI < WPArray.Num(); ++WI)
	{
		AB2AutoWayPoint* WPPrev = WPArray[WI - 1];
		AB2AutoWayPoint* WPCurr = WPArray[WI];
		//AB2AutoWayPoint* WPNext = (WI < WPArray.Num() - 1) ? WPArray[WI + 1] : NULL;

		// Increase one if the PassOrder is same to the previous one.
		// This should be enough.. It will propagate to the end until mets some empty slot.
		// Don't adjust for invalid index..
		if (WPCurr->HasValidPassOrder() && WPPrev->PassOrder == WPCurr->PassOrder)
		{
			WPCurr->PassOrder += 1;
#if WITH_EDITOR // This entire function probably only for the editor..?
			if (GIsEditor)
			{
				WPCurr->UpdateInfoText();
				MarkPackageDirty();
			}
#endif
		}
	}
}

#if WITH_EDITOR

UTextRenderComponent* AB2AutoWayPoint::FindInfoTextDisplayComponent()
{
	TArray<UActorComponent*> CompsByClass;
	GetComponents(CompsByClass);
	for (auto* component : CompsByClass)
	{
		auto* TextComponent = Cast<UTextRenderComponent>(component);
		if (!TextComponent && TextComponent->GetName() != GetInfoTextDisplayName())
			continue;
		return TextComponent;
	}

	return NULL;
}

void AB2AutoWayPoint::UpdateInfoText()
{
	UTextRenderComponent* InfoTextDisplay = FindInfoTextDisplayComponent();
	if (!InfoTextDisplay)
		return;
	
	// To maintain the text size regardless of the root scale.
	FVector ActorScale = GetActorScale3D();
	InfoTextDisplay->SetRelativeScale3D(FVector(1.0f/ActorScale.X, 1.0f/ActorScale.Y, 1.0f/ActorScale.Z));
	InfoTextDisplay->SetText(FText::FromString(FString::Printf(TEXT("WP %d"), PassOrder)));
	// Red for invalid index.
	InfoTextDisplay->SetTextRenderColor( HasValidPassOrder() ? FColor(0, 0, 255) : FColor(255, 0, 0));
}

void AB2AutoWayPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName Name_PassOrder		= GET_MEMBER_NAME_CHECKED(AB2AutoWayPoint, PassOrder);
	FProperty* PropertyThatChanged	= PropertyChangedEvent.Property;
	FName PropertyName				= PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if(PropertyName == Name_PassOrder)
	{
		UpdateInfoText();
		if(HasValidPassOrder()) // Ignore for invalid pass order.
		{
			EnsureUniquePassOrderForCurrentWorld();
		}
	}
}

#endif

float AB2AutoWayPoint::FindNearestMarker(class AB2AutoAIController* AutoController, AActor* Target)
{
	//auto* B2Player = Cast<ABladeIIPlayer>(AutoController->GetPawn());
	//FTransform NewTrm = FTransform::Identity;
	//NewTrm.SetLocation(Target->GetTransform().GetLocation());

	//FPathFindingQuery Query(AutoController, *GetWorld()->GetNavigationSystem()->GetNavDataForProps(AutoController->GetNavAgentPropertiesRef()), AutoController->GetPawn()->GetTransform().GetLocation(), Target->GetTransform().GetLocation());
	//Query.SetAllowPartialPaths(false);
	//if (GetWorld()->GetNavigationSystem()->TestPathSync(Query))
	//{
	//	AB2AutoWayPointMarker::GetInstance(GetWorld())->MoveMarker(NewTrm);
	//}
	//else
	//{
	//	FNavLocation NavProjectedLocation;
	//	if (GetWorld()->GetNavigationSystem()->ProjectPointToNavigation(Target->GetActorLocation(), NavProjectedLocation, FVector(500, 500, 300)) == false)
	//	{
	//		AB2AutoWayPointMarker::GetInstance(GetWorld())->MoveMarker(NewTrm);
	//	}
	//	else
	//	{
	//		NewTrm.SetLocation(NavProjectedLocation.Location);
	//		AB2AutoWayPointMarker::GetInstance(GetWorld())->MoveMarker(NewTrm);
	//	}
	//}
	return FVector::DistSquaredXY(AutoController->GetPawn()->GetTransform().GetLocation(), Target->GetTransform().GetLocation());
}

void AB2AutoWayPoint::ForceMarkByJumpEvent(class AB2JumpEventZone* JumpActor, class ABladeIIPlayer* TargetPlayer)
{
	// 강제 마킹은, 현재 웨이포인트의 Passed 플래그와 상관없이 항상 발동을 [보장] 한다. (우선순위에 따라 의도치 않은 결과 방지)
	if ( TargetPlayer )
	{
		auto* AutoAIController = TargetPlayer->GetAutoAIController();
		if ( AutoAIController && AutoAIController->CurrentTargetWP == this )
		{
			// [자동사냥] 유/무에 따라 다음 웨이포인트 계산을 추가적으로 결정 (TargetToNextWayPoint)
			MarkPassed( true, AutoAIController != nullptr ? true : false, AutoAIController );
		}
	}
}
