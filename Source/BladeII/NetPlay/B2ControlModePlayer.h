#pragma once
//#include "BladeII.h"
#include "BladeIINetPlayer.h"
#include "B2ControlModePlayer.generated.h"

class ABladeIIPlayerAIController;

UCLASS()
class AB2ControlModePlayer : public ABladeIINetPlayer
{
	GENERATED_UCLASS_BODY()

public:
	// AActor interface
	virtual void BeginPlay() override;
	virtual void Tick(float) override;
		
	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartQTE(bool bForce = false) override;

	virtual void StopQTEMount() override;

	// BladeIIPlayer interfaces...
	virtual float GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds) override;
	virtual void AddWeaponSkillPoint(float ToAddPoint) override;

	virtual bool CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType) override;

	virtual void MoveForward(float InValue) override;
	virtual void MoveRight(float InValue) override;

	// My interfaces...
	void PromoteNetStatus();

	class AB2ControlGameMode* GetControlGameMode();

	void SuperStartQTE() { Super::StartQTE(true); }
	void SuperStopQTEMount() { Super::StopQTEMount(); }

	void ResetRequestStartQTEFlag() { bRequestStartQTE = false; }

private:
	UPROPERTY(Transient)
	ABladeIIPlayerAIController*	MyAIController;
	
	bool		bRequestStartQTE;
};