// Fill out your copyright notice in the Description page of Project Settings.

#include "B2CombatConstantInfo.h"
#include "B2CombatStatEvaluator.h"
#include "../BladeII/BladeIIUtil.h"



FCombatCoefficientContainer::FCombatCoefficientContainer()
	//:AttackCoefficient(CombatStatEval::GetDefaultCombatCoefficient()),
	//DefenseCoefficient(CombatStatEval::GetDefaultCombatCoefficient()), 
	//HPCoefficient(CombatStatEval::GetDefaultCombatCoefficient()),
	//PVPDamageScale(CombatStatEval::GetDefaultPVPDamageScale()), 
	//PVPMaxDamageRatio(CombatStatEval::GetDefaultPVPMaxDamageRatio()),
	//PVEProportionalConstant(CombatStatEval::GetDefaultPVEProportionalConstant())
{ }


float UB2CombatConstantInfo::GetAttackCoefficient(EB2GameMode InGameMode)
{
	return GetCombatCoefficient(InGameMode, ECombatCoefficientType::ECOT_Attack);
}

float UB2CombatConstantInfo::GetDefenseCoefficient(EB2GameMode InGameMode)
{
	return GetCombatCoefficient(InGameMode, ECombatCoefficientType::ECOT_Defense);
}

float UB2CombatConstantInfo::GetHPCoefficient(EB2GameMode InGameMode)
{
	return GetCombatCoefficient(InGameMode, ECombatCoefficientType::ECOT_HP);
}

float UB2CombatConstantInfo::GetPVEProportionalConstant(EB2GameMode InGameMode)
{
	return GetCombatCoefficient(InGameMode, ECombatCoefficientType::ECOT_PVEProportionalConstant);
}

float UB2CombatConstantInfo::GetPVPDamageScale(EB2GameMode InGameMode)
{
	return GetCombatCoefficient(InGameMode, ECombatCoefficientType::ECOT_PVPDamageScale);
}

float UB2CombatConstantInfo::GetPVPMaxDamageRatio(EB2GameMode InGameMode)
{
	return GetCombatCoefficient(InGameMode, ECombatCoefficientType::ECOT_PVPMaxDamageRatio);
}

float UB2CombatConstantInfo::GetCombatCoefficient(EB2GameMode InGameMode, ECombatCoefficientType CombatCoefficientType)
{
	const FCombatCoefficientContainer* CombatCoefficient = CombatCoefficients.FindByPredicate([InGameMode](FCombatCoefficientContainer& Container)
	{
		return Container.GameMode == InGameMode;
	});

	BII_CHECK(CombatCoefficient != nullptr)

	switch (CombatCoefficientType)
	{
	case ECombatCoefficientType::ECOT_PVPDamageScale:
		return CombatCoefficient != nullptr ? CombatCoefficient->PVPDamageScale : CombatStatEval::GetDefaultPVPDamageScale();
	case ECombatCoefficientType::ECOT_PVPMaxDamageRatio:
		return CombatCoefficient != nullptr ? CombatCoefficient->PVPMaxDamageRatio : CombatStatEval::GetDefaultPVPMaxDamageRatio();
	case ECombatCoefficientType::ECOT_PVEProportionalConstant:
		return CombatCoefficient != nullptr ? CombatCoefficient->PVEProportionalConstant : CombatStatEval::GetDefaultPVEProportionalConstant();
	}

	if (CombatCoefficient != nullptr)
	{
		switch (CombatCoefficientType)
		{
		case ECombatCoefficientType::ECOT_Attack:
			return CombatCoefficient->AttackCoefficient;
		case ECombatCoefficientType::ECOT_Defense:
			return CombatCoefficient->DefenseCoefficient;
		case ECombatCoefficientType::ECOT_HP:
			return CombatCoefficient->HPCoefficient;
		}
	}
	return CombatStatEval::GetDefaultCombatCoefficient();
	
}
