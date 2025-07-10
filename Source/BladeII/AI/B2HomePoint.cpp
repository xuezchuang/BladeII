
#include "B2HomePoint.h"
//#include "BladeII.h"
#include "BladeIIUtil.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Components/BillboardComponent.h"

AB2HomePoint::AB2HomePoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick	= false;
	//bReplicates						= false;
	//bCanBeDamaged					= false;

	// This can check player overlap for EAWPPC_Overlap, as well as being the root component.
	DefaultCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DefaultCollisionComponent"));
	DefaultCollisionComponent->InitSphereRadius(200.0f);
	DefaultCollisionComponent->SetCollisionProfileName(CollisionProfileName_OverlapMostDynamic);

	SetRootComponent(DefaultCollisionComponent);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	EditorSpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("HomePointSprite"));
	if (!IsRunningCommandlet() && (EditorSpriteComponent != nullptr))
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName ID_B2HomePoint;
			FText NAME_B2HomePoint;
			FConstructorStatics()
				: SpriteTexture(TEXT("/Engine/EditorResources/Waypoint")) // Editor has waypoint texture resource..
				, ID_B2HomePoint(TEXT("B2HomePoint"))
				, NAME_B2HomePoint(FText::FromString(TEXT("B2HomePointEditorSprite")))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		EditorSpriteComponent->Sprite = ConstructorStatics.SpriteTexture.Get();
		EditorSpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_B2HomePoint;
		EditorSpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_B2HomePoint;
		EditorSpriteComponent->AttachToComponent(DefaultCollisionComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		EditorSpriteComponent->bIsScreenSizeScaled = true;
		EditorSpriteComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
#endif
}
