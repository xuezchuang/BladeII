// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIInteractiveActor.h"
#include "BladeIIDamageType.h"
#include "B2AreaDamageActorBase.generated.h"

/**
*
*/
UCLASS(abstract, hidecategories = InteractTrigger)
class BLADEII_API AB2AreaDamageActorBase : public AContinuousInAreaInteractiveActor
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float FadeInOutTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float DamageFirstImpactTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	class UParticleSystem* WarningFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	class UParticleSystem* ActivatingFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	struct FDamageInfo DamageInfo;

	//void SetDamageAreaData(const struct FDamageInfo& InDamageInfo, const ABladeIICharacter* B2Char, bool bOverrideDamageInfo, float DamageOfTick, float Radius, float DelayTime, float Interval = 0.f, float Life = 0.f, float FirstImpactTime = -1.f);
	void SetDamageAreaFXData(class UParticleSystem* InWarningFX, class UParticleSystem* InActivateFX, const FLinearColor& Color, const FVector& InWarningFxScale = FVector(1.f, 1.f, 1.f));

	virtual void BeginPlay() override;
	virtual void BeginInteract(class AActor* InInteractingActor) override;
	virtual void InteractAction() override;
	virtual void EndInteract() override;

	virtual void BeginDestroy() override;
	virtual void Destroyed() override;

	virtual void Tick(float DeltaSeconds) override;

	const void* GetCustomDamageInfo();

	void CancelCastAreaDamage_CallBack();

	//static AB2AreaDamageActorBase* SpawnDamageAreaActor(
	//	TSubclassOf<AB2AreaDamageActorBase> Class,
	//	ABladeIICharacter* Owner,
	//	const struct FDamageInfo& InDamageInfo,
	//	bool bOverrideDamageInfo, const FVector& SpawnLocation, float DamageOfTick, float Radius, float DelayTime, float Interval, float Life, float FirstImpactTime = -1.f,
	//	class UParticleSystem* InWarningFX = nullptr, class UParticleSystem* InActivateFX = nullptr, const FLinearColor& Color = FLinearColor::Black,
	//	const FVector& InWarningFxScale = FVector(1.f, 1.f, 1.f));

	FORCEINLINE void SetCancelSubscribeKey(uint32 Key) {
		EventOwnerCancelAreaDamageSubscribeKey = Key;
	}
	FORCEINLINE uint32 GetCancelSubscribeKey() {
		return EventOwnerCancelAreaDamageSubscribeKey;
	}

	void ToggleDamageImpact();

protected:
	float Damage;
	float DamageDelay;
	float DamageRadius;
	float DamageInterval;
	bool bDamageFirstImpactTimeTriggeredTimer;
	FTimerHandle DamageFirstImpactTimerHandle;

	FLinearColor WarningFXColor;
	FVector WarningFxScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeII, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* PSCComponent;

	float ElapsedTimeSinceActivate;

	uint32 EventOwnerCancelAreaDamageSubscribeKey;

	static FName OldFxParamCountTimeName;
	static FName OldFxParamRadiusName;
	static FName OldFxParamFadeName;

	bool bCancelCastAreaDamage;
};

UCLASS()
class BLADEII_API AB2AreaDamageActor : public AB2AreaDamageActorBase
{
	GENERATED_BODY()

public:
	virtual void BeginInteract(class AActor* InInteractingActor) override;

private:
	static FName NewFxParamLifeTimeName;
	static FName NewFxParamRadiusName;
	static FName NewFxParamColorName;
};
