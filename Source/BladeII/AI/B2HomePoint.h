#pragma once

#include "GameFramework/Actor.h"
#include "B2HomePoint.generated.h"

UCLASS()
class BLADEII_API AB2HomePoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DefaultCollisionComponent;

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UBillboardComponent* EditorSpriteComponent;
#endif

	AB2HomePoint(const FObjectInitializer& ObjectInitializer);
};