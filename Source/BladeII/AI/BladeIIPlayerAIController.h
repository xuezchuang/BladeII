#pragma once

#include "AIController.h"
#include "Knowledge.h"
#include "BladeIIPlayerAIController.generated.h"

UCLASS()
class BLADEII_API ABladeIIPlayerAIController : public AAIController
{
	GENERATED_BODY()

	/** Total skill management. Normal skill, resurrection, tag, etc.. */
	UPROPERTY(Transient)
	class UB2SkillRTManager* SkillRTManager;

public:
	// My properties...
	UPROPERTY(BlueprintReadOnly)
	class ABladeIICharacter* CurrentTargetMob;

	// My properties...
	UPROPERTY(BlueprintReadOnly)
	class AActor* CurrentTargetObject;

	/** Cached range attacking mob reference, which really attacked the player recently,
	* to trace it even when it is out of target range. */
	UPROPERTY(BlueprintReadOnly)
	class ABladeIICharacter* CachedRangeAttackedMob;

	float TotalDashDuration;
	float CurrentDashDuration;
	float MaxDashDistance;

	ABladeIIPlayerAIController(const FObjectInitializer& ObjectInitializer);

	// AActor interfaces...
	virtual void BeginPlay() override;
	virtual void Tick(float) override;

	// AController interfaces...
	//virtual void Possess(APawn*) override;
	//virtual void UnPossess() override;

	// My interfaces...
	virtual void BeginAutoPlay();
	void StopAutoPlay();
	bool IsRunningAI();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual bool UpdateTargetAndLocation(FString const& KeyName);

	virtual bool UpdateTarget();
	
	FORCEINLINE void SetTeamNum(int8 InTeamNum){ TeamNum = InTeamNum; }
	FORCEINLINE int8 GetTeamNum(){ return TeamNum; }
	FAIMemory&	GetMyMemory()
	{
		return MyMemory;
	}

	// Clear all except for condition...
	void	ClearMyMemory()
	{
		MyMemory.MyTeam.Empty();
		MyMemory.OtherTeam.Empty();
	}

	virtual class ABladeIICharacter* PickOpponent();

protected:
	UPROPERTY()
	UBehaviorTree*			DefaultBT;

	FAIMemory				MyMemory;

	void FindTarget();
	
	int8 TeamNum;

	void LoadBehaviorTreeFromAsset();

	IGameRule*				InjectedGameRule;
};

UCLASS()
class BLADEII_API AB2TeamMatchPlayerAIController : public ABladeIIPlayerAIController
{
	GENERATED_BODY()

public:

	virtual class ABladeIICharacter* PickOpponent() override;
};


UCLASS()
class BLADEII_API AB2GuildPlayerAIController : public ABladeIIPlayerAIController
{
	GENERATED_BODY()

public:

	virtual class ABladeIICharacter* PickOpponent() override;
};