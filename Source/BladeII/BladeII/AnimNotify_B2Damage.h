// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BladeIIDamageType.h"
#include "AnimNotify_B2Damage.generated.h"

/**
 * 
 */

UCLASS()
class BLADEII_API UAnimNotify_B2Damage : public UAnimNotify
{
public:
	GENERATED_BODY()

	UAnimNotify_B2Damage(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostInitProperties() override;

	void SerializeBPProperties();

	uint32 DamageHash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageInfo")
	FDamageInfo DamageInfo;
};
