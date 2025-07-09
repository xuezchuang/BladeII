// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "GameFramework/Actor.h"
#include "B2AutoWayPointMarker.generated.h"

/**  
 * AB2AutoWayPointMarker
 * Way point for auto play.
 * This is to manage necessary play route and ordering stuff. Not for advanced navigation, which will be done by NavMesh.
 */
UCLASS()
class BLADEII_API AB2AutoWayPointMarker : public AActor
{
	GENERATED_BODY()
	
	static class AB2AutoWayPointMarker* Instance;

	/** Default collision component, being a root component, and could do some triggering..? */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DefaultCollisionComponent;

public:	
	static class AB2AutoWayPointMarker* GetInstance(class UWorld* World);

	// Sets default values for this actor's properties
	AB2AutoWayPointMarker(const FObjectInitializer& ObjectInitializer);
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void MoveMarker(const FTransform& trm);
	void DelegateMoveMarker();
	void DelegateBeginOverlap();
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
};
