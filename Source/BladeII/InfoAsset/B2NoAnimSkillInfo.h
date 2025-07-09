// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "UObject/Object.h"
#include "BladeIIDamageType.h"
#include "B2NoAnimSkillInfo.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2NoAnimSkillInfo : public UObject
{
	GENERATED_BODY()

public: 
	UB2NoAnimSkillInfo(const FObjectInitializer& ObjectInitializer);	

public:
	// NoAnimSkill - Thunderstorm(���׸� ��Ʈ Ÿ�� ������ + ����Ʈ ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thunderstorm")
	float ThunderstormWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thunderstorm")
	TSoftObjectPtr<class UParticleSystem> ThunderstormFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thunderstorm")
	FDamageInfo ThunderstormDamageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thunderstorm")
	float ThunderstormDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thunderstorm")
	float ThunderstormFreezeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thunderstorm")
	float ThunderstormImpulseScale;

	// NoAnimSkill - Limitation(���׸� ��Ʈ �ӹ�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Limitation")
	EBuffType LimitationBuffType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Limitation")
	float LimitationDuration;

	// NoAnimSkill - Firewall(���׸� ��Ʈ ȭ����(?) ���� ��ġ�� ����Ʈ ��� + �ֺ� �� ȭ��������Ʈ ������)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firewall")
	TSoftObjectPtr<class UParticleSystem> FirewallFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firewall")
	FDamageInfo FirewallDamageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firewall")
	float FirewallDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firewall")
	float FirewallRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firewall")
	float FirewallFreezeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firewall")
	float FirewallImpulseScale;

};
