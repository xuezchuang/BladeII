// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/NoExportTypes.h"
#include "CommonStruct.h"
#include "B2CombatConstantInfo.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FCombatCoefficientContainer 
{
	GENERATED_BODY()

	//郴何 橇肺欺萍 函版矫 俊叼磐惑 钎扁 且 锭 B2CombatCoefficientContainerDetail档 函版 秦拎具 窃
	FCombatCoefficientContainer();
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EB2GameMode GameMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackCoefficient;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DefenseCoefficient;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HPCoefficient;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PVPDamageScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PVPMaxDamageRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PVEProportionalConstant;
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2CombatConstantInfo : public UObject
{
	GENERATED_BODY()

public:
	float GetAttackCoefficient(EB2GameMode InGameMode);
	float GetDefenseCoefficient(EB2GameMode InGameMode);
	float GetHPCoefficient(EB2GameMode InGameMode);
	float GetPVEProportionalConstant(EB2GameMode InGameMode);
	float GetPVPDamageScale(EB2GameMode InGameMode);
	float GetPVPMaxDamageRatio(EB2GameMode InGameMode);

private:
	enum class ECombatCoefficientType
	{
		ECOT_Attack,
		ECOT_Defense,
		ECOT_HP,
		ECOT_PVPDamageScale,
		ECOT_PVPMaxDamageRatio,
		ECOT_PVEProportionalConstant,
	};

	float GetCombatCoefficient(EB2GameMode InGameMode, ECombatCoefficientType CombatCoefficientType);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FCombatCoefficientContainer> CombatCoefficients;
};


