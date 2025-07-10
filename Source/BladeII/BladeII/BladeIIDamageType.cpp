// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeIIDamageType.h"

//#include "BladeIIPlayer.h"
//#include "BladeIIGameMode.h"
//#include "AnimNotify_B2Damage.h"
//#include "B2EtherSetEffect.h"

FDamageInfo::FDamageInfo()
{
	DamageType = EAttackType::EAT_Default;
	ElementType = EDamageElement::EDE_None;
	StateType = EStateAttackType::EDT_Normal;
	WeaponType = EDamageWeapon::EDW_Blade;

	DamageLogicType = EDamageLogicType::ELT_Normal;
	ArmorBreakLogicType = EArmorBreakLogicType::EALT_Normal;
	GuardBreakLogicType = EGuardBreakLogicType::EGLT_AbleToGuard;

	ArmorBreakAmount = 30.f;

	StateDamageLogicType = EDamageLogicType::ELT_PctByTargetTotalHP;

	StateDamageTriggerRate = 1.0f;
	StateDamageDuration = 0.f;
	StateDamageAmount = 0.f;

	// -1 means no overrides
	DamageIncreaseRateOverride = -1.f;
	BossDamageIncreaseRateOverride = -1.f;
	CriticalRateOverride = -1.f;
	CriticalDamageRateOverride = -1.f;

	bCountable = true;
	bStunCountable = false;
	bUseHitSound = true;
	bIgnoreNotifyToAttacker = false;
	bOffenseEtherActivatable = false;
	bDefenseEtherActivatable = false;
	bForceApplyStateDamage = false;
	bIgnoreIncreaseDamageNum = false;

	HitDirectionOverride = EHitDirection::EHD_None;
	HitRandomAngleOverride = -1.f;

	DamageHash = 0;

	Amplitude = FVector::ZeroVector;
	Frequency = FVector::ZeroVector;
}

void FDamageInfo::SetDamageInfo(const FDamageInfo* DamageInfo, const ABladeIICharacter* B2Char)
{
	if (B2Char == NULL)
	{
		return;
	}

	if (B2Char->IsControlledByQTEPlayer())
	{
		B2Char = B2Char->FindQteOwnerPlayer();

		if (B2Char == NULL)
		{
			return;
		}
	}

	if (DamageInfo != NULL && DamageInfo != this)
	{
		*this = *DamageInfo;
	}

	//auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(B2Char));

	//if (!GameMode)
	//	return;

	//GameMode->OverrideDamageInfo(*this, B2Char);
}

uint32 FDamageInfo::CalculateDamageHash(UObject* Object) const
{
	if (DamageHash != 0)
		return DamageHash;

	FString UniqueAssetPath;
	
	// 1. AnimNotify_B2Damage
	//if (UAnimNotify_B2Damage* DamageNotify = Cast<UAnimNotify_B2Damage>(Object))
	//{
	//	UniqueAssetPath = DamageNotify->GetFullName(nullptr);	// [Asset].[AnimSequence].[Notify == Object]
	//}

	//else if (UB2EtherSetEffect* EtherObject = Cast<UB2EtherSetEffect>(Object))
	//{
	//	UniqueAssetPath = EtherObject->GetFullName(nullptr);	// [Asset].[EtherClass]
	//}
	//else
	//{
	//	Object->StaticClass()->GetFName().ToString(UniqueAssetPath);
	//}

	DamageHash = GetTypeHash(UniqueAssetPath);
	return DamageHash;
}

const FDamageInfo* FDamageInfo::GetDefaultDamagePtr()
{
	return &GetDefaultDamage();
}

const FDamageInfo& FDamageInfo::GetDefaultDamage()
{
	static FDamageInfo DamageInfo;
	return DamageInfo;
}

bool FDamageInfoContainer::RegisterDamageInfo(uint32 HashValue, const FDamageInfo& DamageInfo)
{
	const FDamageInfo* FindDamageInfo = DamageInfoMap.Find(HashValue);
	if (HashValue != 0)
	{
		DamageInfoMap.Add(HashValue, DamageInfo);
		return true;
	}

	return false;
}

const FDamageInfo* FDamageInfoContainer::GetDamageInfo(uint32 HashValue)
{
	return GetContainer().DamageInfoMap.Find(HashValue);
}

FDamageInfoContainer& FDamageInfoContainer::GetContainer()
{
	static FDamageInfoContainer Container;
	return Container;
}

EBuffType GetBuffTypeByStateAttack(EStateAttackType InType)
{
	switch (InType)
	{
	case EStateAttackType::EDT_Freezing:	return EBuffType::Debuff_Freezing;
	case EStateAttackType::EDT_Stun:		return EBuffType::Debuff_Stun;
	case EStateAttackType::EDT_Burning:		return EBuffType::Debuff_Burning;
	case EStateAttackType::EDT_Poison:		return EBuffType::Debuff_Poison;
	case EStateAttackType::EDT_Immobilize:	return EBuffType::Debuff_Immobilize;
	case EStateAttackType::EDT_Seal:		return EBuffType::Debuff_Seal;
	}

	return EBuffType::Buff_None;
}
