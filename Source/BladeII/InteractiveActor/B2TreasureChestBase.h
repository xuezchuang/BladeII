// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "BladeIIInteractiveActor.h"
#include "B2TreasureChestBase.generated.h"

class AB2StageManager;

/**
 * 
 */
UCLASS(hidecategories = InteractTrigger)
class BLADEII_API AB2TreasureChestBase : public ASkeletalMeshInteractiveActor
{
	GENERATED_UCLASS_BODY()	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	int32 RewardGoldWeight;

	virtual void BeginDestroy() override;

	FORCEINLINE	void SetStageManager(AB2StageManager* inManager) { CachedStageManager = inManager; }

private:
	virtual void InteractAction() override;

	UPROPERTY(Transient)
	AB2StageManager* CachedStageManager;
};
