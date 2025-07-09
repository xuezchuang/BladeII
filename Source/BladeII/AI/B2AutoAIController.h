// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "BladeIIPlayerAIController.h"
#include "B2AutoAIController.generated.h"

/**
 * AIController for auto player, using AutoWayPoint
 * The special point of this controller class is, this coexists with player character's player controller.
 */
UCLASS()
class BLADEII_API AB2AutoAIController : public ABladeIIPlayerAIController
{
	GENERATED_BODY()
	
protected:
	/** We have done all */
	UPROPERTY(BlueprintReadOnly)
	uint32 bPassedAllWayPoint : 1;
	
private:
	/** This is set from outside, as a raw value. 
	 * BladeIIPlayer's bHadInput might be intended to do the same thing..?  */
	uint32 bRawPCHadMovementInput : 1;
	/** To maintain the movement input flag for one tick. */
	uint32 bPCHadMovementInput : 1;

	uint32 bLastPCHadMovementInput : 1;

	float fLastPCHadMovementInputFalseTime;

	uint32 bPCHadBattleInput : 1;

public:

	UPROPERTY(BlueprintReadOnly)
	class AB2AutoWayPoint* CurrentTargetWP;

	UPROPERTY(BlueprintReadOnly)
	class AB2AutoWayPointMarker* CurrentTargetMK;

	bool StageCompleted;

public:

	AB2AutoAIController(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;
	virtual void Destroyed() override;

	virtual FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr);

	void UnsubscribeEvents();
	
	/** Right after Possess, check loaded waypoints and set the initial goal,.. */
	virtual void BeginAutoPlay() override;
	void FinishAutoPlay();
	void ResetAutoPlay();

	void TargetToNextWayPoint();

	/** Either required wave mob or general target mob.. Check for both, then set it to CurrentTargetMob. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool UpdateTargetMobInPriority();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool UpdateTargetObjectInPriority(float minDistance, float maxDistance);

private:

	/** Pick one of live mobs without any wave condition in general circumstances.
	 * It will returns null if nothing can be spotted within certain distance. 
	 * Additionally pick up a ranged mob attacked the player recently. */
	class ABladeIICharacter* PickGeneralTargetMob();
	
	/** Pick one of live mobs (probably the closest one) in the interested wave, when the wave is the complete condition of CurentTargetWP. 
	 * If CurrentTargetWP does not have wave complete condition or all mobs in the interested wave died, it will return NULL. 
	 * Like GetInterestedWaveMobList, consider the result is valid only for one frame. */
	class ABladeIICharacter* PickOneInterestedWaveMob();

	class AActor* PickOneInterestedDestructible();
	
public:
	
	inline void SetPCHadMovementInput() { bRawPCHadMovementInput = true; }

	/** Query if PC had movement input. It lasts for one tick. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool HadMovementInput();
	
	// 공격 방어버튼 입력중인가
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool HadBattleInput();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool ShouldStopAutoDueToCinematicMode();
	
	/** Possess does not let existing player controller to unpossess the player pawn */
	//virtual void Possess(APawn* InPawn) override;
	//virtual void UnPossess() override;

	bool CanPlayerHaveMoveMotion() const {
		return GetMoveStatus() == EPathFollowingStatus::Moving;
	}

private:
	
	void OnPassedAllWayPoint();

	bool	bAlreadySubscribeEvent;
	uint32	StageCompleteTicket;
};
