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

	//내부 프로퍼티 변경시 에디터상 표기 할 때 B2CombatCoefficientContainerDetail도 변경 해줘야 함
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


