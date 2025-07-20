// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "GameFramework/Actor.h"
#include "B2AutoWayPoint.generated.h"

/**  
 * AB2AutoWayPoint
 * Way point for auto play.
 * This is to manage necessary play route and ordering stuff. Not for advanced navigation, which will be done by NavMesh.
 */

/*
USTRUCT()
struct FWayPoint
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere)
	FTransform Transform;
	UPROPERTY(VisibleAnywhere)
	FVector	BoxExtent;

	FWayPoint() {};
	FWayPoint(FTransform trm, FVector box) : Transform(trm), BoxExtent(box) {};
};
*/

UCLASS()
class BLADEII_API AB2AutoWayPoint : public AActor
{
	GENERATED_BODY()
	
	/** Default collision component, being a root component, and could do some triggering..? */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DefaultCollisionComponent;

#if WITH_EDITORONLY_DATA
private :
	/** Billboard Component displayed in editor */
	UPROPERTY()
	class UBillboardComponent* EditorSpriteComponent;

#endif

	//UPROPERTY(Transient)
	//class AB2StageManager* CachedStageManager;
	

public:
	/**
	 * The order among loaded waypoints in a single stage. 
	 * Negative value is invalid one.
	 * It's supposed to be unique in a single stage. (but we might have to consider duplicated case when a level is dynamically streamed)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WayPoint")
	int32 PassOrder;

	/**
	 * If true, do a bluffing attack gesture (regardless of the presence of enemy) after arriving this point.
	 */
	UPROPERTY(EditAnywhere, Category = "WayPoint")
	bool bAttackActionOnArrive;
protected:

	/** Being set to true if player has passed this WP */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "WayPoint")
	bool bPassed;

public:	
	// Sets default values for this actor's properties
	AB2AutoWayPoint(const FObjectInitializer& ObjectInitializer);

	virtual void PostRegisterAllComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** To be called when this WP is passed by a player with meeting condition. 
	 * bMarkPrevious : Set all WPs in previous order as being passed. 
	 * bInAutoPlay : True if passed during Auto AI play. It could be passed in player controlled mode. */
	void MarkPassed(bool bMarkPrevious, bool bInAutoPlay, class AB2AutoAIController* AutoController);

	/**  
	 * Check PassConditionCollection whether we can finally pass this waypoint.
	 */
	void FinalCheckForPass();

	inline bool IsPassed() { return bPassed; }

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	float FindNearestMarker(class AB2AutoAIController* AutoController, AActor* Target);

	void ForceMarkByJumpEvent(class AB2JumpEventZone* JumpActor, class ABladeIIPlayer* TargetPlayer);

#if WITH_EDITOR
	/** Simply iterates all actors in world and returns waypoint actors.
	 * You can get sorted array by PassOrder if bSort is true. */
	static void EditorGetAllWayPointsInWorld(class UWorld* InWorld, TArray<AB2AutoWayPoint*>& OutWPArray, bool bSort);
#endif

	/** Check other WayPoint actors in current world and ensure each of them has unique order..
	 * It can affects all loaded AutoWayPoint instances. 
	 * It iterates a lot, so be careful to NOT use this in tick. 
	 * Could be editor only functionality... */
	void EnsureUniquePassOrderForCurrentWorld();

	inline int32 GetPassOrder() const { return PassOrder; }

	inline bool HasValidPassOrder() { return (PassOrder >= 0); }

	//FORCEINLINE void SetStageManager(class AB2StageManager* InManager) { CachedStageManager = InManager; }

#if WITH_EDITOR

	inline FString GetInfoTextDisplayName() { return FString(TEXT("InfoTextDisplayComponent")); }

	class UTextRenderComponent* FindInfoTextDisplayComponent();

	void UpdateInfoText();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
