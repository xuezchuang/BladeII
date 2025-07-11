#include "B2GeneralBuffs.h"
#include "Event.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "B2NoAnimSkillInfo.h"
#include "BladeIIGameMode.h"
//#include "B2DimensionGameMode.h"
#include "B2CharacterBuffManager.h"
#include "B2DamageEffectInfo.h"
//#include "B2AnimInstance.h"

#include "Engine/DamageEvents.h"


void ClearDebuffGroup(EBuffType CurrentBuff, class ABladeIICharacter* Character)
{
	if (Character)
	{
		for (EBuffType ClearBuffType : DebuffGroup)
		{
			if(ClearBuffType == CurrentBuff)
				continue;
			Character->ClearBuff(ClearBuffType);
		}
	}
}

/*
	�ӹ�
*/
UB2Buff_Restraint::UB2Buff_Restraint()
{
	m_vRestraintPos = FVector(0, 0, 0);
	bTickableBuff = true;

	bDebuff = true;
}

void UB2Buff_Restraint::OnBeginBuff(const float BuffDuration)
{
	m_vRestraintPos = GetOwnerCharacter()->GetActorLocation();
}

void UB2Buff_Restraint::OnTick(const float DeltaSeconds)
{
	GetOwnerCharacter()->SetActorLocation(m_vRestraintPos);
}

/*
	����
*/
UB2Buff_Shield::UB2Buff_Shield()
{
	bShieldClear = false;
	ShieldAmount = 0.f;
}

void UB2Buff_Shield::OnBeginBuff(const float BuffDuration)
{
	bShieldClear = false;

	// UI���������� SetShieldAmount�ٷ� ȣ����ϱ� �ɸ������ؼ� Ž.
	GetOwnerCharacter()->SetShield(BuffAmount);
}

void UB2Buff_Shield::OnClearBuff(bool bTimeOut)
{
	bShieldClear = true;
	GetOwnerCharacter()->SetShield(0.0f);
}

void UB2Buff_Shield::SetShieldAmount(const float Amount)
{
	ShieldAmount = FMath::Max<float>(Amount, 0.f);

	if (ShieldAmount <= 0.f && !bShieldClear)
		BuffClear(false);
}


/*
	DoT ������
*/
UB2Buff_DotDamage::UB2Buff_DotDamage()
{
	DotTickTime = 0.f;
	CurrentDotCount = 0;

	bTickableBuff = true;

	bDebuff = true;

	// <-������ ����
	DamageInfo.bCountable = false;
	DamageInfo.bUseHitSound = false;
	DamageInfo.ArmorBreakAmount = 0.f;
	DamageInfo.WeaponType = EDamageWeapon::EDW_Magic;
	DamageInfo.ElementType = EDamageElement::EDE_Fire;
	// �̰� �⺻ �븻. �ִϸ��̼� ��Ƽ����->TakeDamage���ؼ��� ���� ����(TakeDamageAbnormalState �ȿ��� �����Ŵ��� �̿��ϴ°� ���� ���۾�)
	DamageInfo.DamageLogicType = EDamageLogicType::ELT_Normal;

	// ������ ���� �����ڿ��� �˸�������.
	DamageInfo.bIgnoreNotifyToAttacker = true;

	// Dynamic values
	DamageInfo.CriticalRateOverride = 0.f;
	DamageInfo.CriticalDamageRateOverride = 1.f;
	DamageInfo.DamageIncreaseRateOverride = 1.f;
	DamageInfo.BossDamageIncreaseRateOverride = 1.f;
	// -> ������ ����

	DamageInfo.bIgnoreIncreaseDamageNum = true;

	DotDamageLogicType = EDamageLogicType::ELT_Normal;
}

void UB2Buff_DotDamage::OnBeginBuff(const float BuffDuration)
{
	// ����� DotDamage�� ȭ���̶� �����ϰ�
	DotTickTime = 1.0f;

	CurrentDotCount = 1;

	// ȭ�� ���͸���. �ٸ� ��Ʈ������ ������� �߰��Ǹ� ��ӹ޾Ƽ� ȭ�� ���α���.
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		if (UB2DamageEffectInfo* DamageEffectInfo = OwnerCharacter->GetDamageEffectInfo())
			DamageEffectInfo->OverrideCharacterMaterial(ECharacterMaterialOverrideType::ECMO_Fire, OwnerCharacter);
	}
}

void UB2Buff_DotDamage::OnTick(const float DeltaSeconds)
{
	if (GetElapsedTime() >= DotTickTime * CurrentDotCount)
	{
		CurrentDotCount++;
		OnDotActive(CurrentDotCount);
	}
}

void UB2Buff_DotDamage::OnClearBuff(bool bTimeOut)
{
	//DotTickTime = 0.f;
	//CurrentDotCount = 0;

	//if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	//	OwnerCharacter->RestoreOverrideMaterial(ECharacterMaterialOverrideType::ECMO_Fire);

	//if (auto* OwnerPlayer = Cast<ABladeIIPlayer>(GetOwnerCharacter()))
	//{
	//	if (auto* AnimInst = OwnerPlayer->GetAnimInstance())
	//		AnimInst->UpdateDamageEnd();
	//}

}

void UB2Buff_DotDamage::OnDotActive(const int32 DotTickCount)
{
	//ABladeIICharacter* DotTarget = GetOwnerCharacter();
	//if (DotTarget && DotTarget->IsAlive() && DotTarget->Role == ROLE_Authority)
	//{
	//	DamageInfo.DamageLogicType = DotDamageLogicType;
	//	
	//	// Causer( DotDamage�� �� ���� ) �� GC�� ����� ��� ũ���� ���ɼ��� ���� -> TWeakObjectPtr�� ���� GC�� ���� �����Ǹ� IsValid���� �ʰ� ��
	//	ABladeIICharacter* DamageCauser = nullptr;
	//	if (BuffCauser.IsValid())
	//		DamageCauser = Cast<ABladeIICharacter>(BuffCauser.Get());

	//	DotTarget->RequestDamage(BuffAmount, &DamageInfo, DamageCauser, true);
	//}
}

void UB2Buff_DotDamage::PostInitProperties()
{
	Super::PostInitProperties();

	// ������ ���� ���
	const uint32 DamageInfoHash = DamageInfo.CalculateDamageHash(this);
	FDamageInfoContainer::GetContainer().RegisterDamageInfo(DamageInfoHash, DamageInfo);
}


/*
	����
*/
UB2Buff_Freezing::UB2Buff_Freezing()
{
	bTickableBuff = true;
	bDebuff = true;
	bUseCrowdControlCount = true;
}

void UB2Buff_Freezing::OnBeginBuff(const float BuffDuration)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		ClearDebuffGroup(BuffType, OwnerCharacter);

		if (UB2DamageEffectInfo* DamageEffectInfo = OwnerCharacter->GetDamageEffectInfo())
			DamageEffectInfo->OverrideCharacterMaterial(ECharacterMaterialOverrideType::ECMO_Ice, OwnerCharacter);

		// ����ġ�� ����Ʈ ����(����, �Ӹ�Ż�� ����)
		OwnerCharacter->ClearEffectInAbnormalState();
	}
}

void UB2Buff_Freezing::OnTick(const float DeltaSeconds)
{	
}

void UB2Buff_Freezing::OnClearBuff(bool bTimeOut)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		OwnerCharacter->RestoreOverrideMaterial(ECharacterMaterialOverrideType::ECMO_Ice);
		OwnerCharacter->LastStateDamageType = EStateAttackType::EDT_Normal;

		// ���� �ɸ��Ÿ� �ǰݸ�� ��ŵ + �̹� �ϱ��־ �н�(�˹������ ���� ���� �����Ǵ� ��쵵 ����)
		if (BuffManager->IsBuffActive(EBuffType::Debuff_Stun) == false && OwnerCharacter->GetDamagedNum() == 0)
		{
			OwnerCharacter->UpdateDamageNum(FDamageInfo::GetDefaultDamage());
			OwnerCharacter->ApplyDefaultDamageAnimation();
			OwnerCharacter->LastDamageType = FMath::RandBool() ? EAttackType::EAT_LeftSwing : EAttackType::EAT_RightSwing;
		}

		if (BuffManager && bTimeOut)
			BuffManager->CrowdControlCount = 0;

		OwnerCharacter->SetForceComboEnd();
	}
}

float UB2Buff_Freezing::GetCustomTimeDilation()
{
	if (GetOwnerCharacter())
		return GetOwnerCharacter()->FreezeTimeDilation;

	return 1.f;
}

UB2Buff_Reflect::UB2Buff_Reflect()
{
	// <-������ ����
	DamageInfo.bCountable = false;
	DamageInfo.bUseHitSound = false;
	DamageInfo.ArmorBreakAmount = 0.f;
	DamageInfo.WeaponType = EDamageWeapon::EDW_Magic;
	DamageInfo.ElementType = EDamageElement::EDE_Fire;
	DamageInfo.bIgnoreNotifyToAttacker = true;
	// -> ������ ����
}

void UB2Buff_Reflect::OwnerTakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	//const FDamageInfo* TakeDmgInfo = static_cast<const FDamageInfo*>(DamageEvent.DamageInfo);
	//if(TakeDmgInfo && TakeDmgInfo->bIgnoreNotifyToAttacker)
	//	return;

	//ABladeIICharacter* Owner = GetOwnerCharacter();
	//if(auto* Attacker = Cast<ABladeIICharacter>(Owner->GetAttacker(DamageCauser)))
	//{
	//	Attacker->RequestDamage(BuffAmount, &DamageInfo, Owner);
	//}
}

void UB2Buff_Reflect::PostInitProperties()
{
	Super::PostInitProperties();

	// ������ ���� ���
	const uint32 DamageInfoHash = DamageInfo.CalculateDamageHash(this);
	FDamageInfoContainer::GetContainer().RegisterDamageInfo(DamageInfoHash, DamageInfo);
}

UB2Buff_Stun::UB2Buff_Stun()
{
	bDebuff = true;
	bUseCrowdControlCount = true;
}

void UB2Buff_Stun::OnBeginBuff(const float BuffDuration)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		ClearDebuffGroup(BuffType, OwnerCharacter);

		OwnerCharacter->LastStateDamageType = EStateAttackType::EDT_Stun;
		OwnerCharacter->SetForceComboEnd();
	}
}

void UB2Buff_Stun::OnTick(const float DeltaSeconds)
{

}

void UB2Buff_Stun::OnClearBuff(bool bTimeOut)
{
	//if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	//{
	//	OwnerCharacter->LastStateDamageType = EStateAttackType::EDT_Normal;

	//	if (BuffManager && bTimeOut)
	//		BuffManager->CrowdControlCount = 0;

	//	OwnerCharacter->SetForceComboEnd();

	//	// HugeDamage �߿� Stun�� Clear �� ��� HugeDamageAnim�� DamageEndNotify���� ComboEnd�� ȣ�� �� ��.
	//	if (auto* OwnerPlayer = Cast<ABladeIIPlayer>(OwnerCharacter))
	//	{	
	//		if (IsHugeDamage(OwnerPlayer->GetDamageAnimType()) == false)
	//		{
	//			auto* AnimInst = OwnerPlayer->GetAnimInstance();
	//			if(AnimInst)
	//				AnimInst->UpdateDamageEnd();
	//			if(OwnerPlayer->Role == ROLE_Authority)
	//				OwnerPlayer->ConsumeReservedSkill();
	//		}
	//	}
	//}
}

UB2Buff_Poison::UB2Buff_Poison()
{
	bDebuff = true;
}

void UB2Buff_Poison::OnBeginBuff(const float BuffDuration)
{	
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		if (UB2DamageEffectInfo* DamageEffectInfo = OwnerCharacter->GetDamageEffectInfo())
			DamageEffectInfo->OverrideCharacterMaterial(ECharacterMaterialOverrideType::ECMO_Poison, OwnerCharacter);
	}
}

void UB2Buff_Poison::OnTick(const float DeltaSeconds)
{

}

void UB2Buff_Poison::OnClearBuff(bool bTimeOut)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		OwnerCharacter->RestoreOverrideMaterial(ECharacterMaterialOverrideType::ECMO_Poison);
	}
}

float UB2Buff_Poison::GetCustomTimeDilation()
{
	return FMath::Max((100.f - BuffAmount) * 0.01f, 0.0f);
}

UB2Buff_Hide::UB2Buff_Hide()
{

}

void UB2Buff_Hide::OnBeginBuff(const float BuffDuration)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		if (UB2DamageEffectInfo* DamageEffectInfo = OwnerCharacter->GetDamageEffectInfo())
			DamageEffectInfo->OverrideCharacterMaterial(ECharacterMaterialOverrideType::ECMO_Hide, OwnerCharacter);

		// ����� �÷��̾�� �Ⱥ��̰�(�÷��̾� ���� �ɸ��Ͱ� ���� ����Ұ� �������)
		ABladeIIPlayer* pCastedPlayer = Cast<ABladeIIPlayer>(OwnerCharacter);

		if (pCastedPlayer && !pCastedPlayer->GetIsLocalPlayerTeam())
			pCastedPlayer->SetActorHiddenInGame(true);
	}
}

void UB2Buff_Hide::OnTick(const float DeltaSeconds)
{

}

void UB2Buff_Hide::OnClearBuff(bool bTimeOut)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		OwnerCharacter->RestoreOverrideMaterial(ECharacterMaterialOverrideType::ECMO_Hide);

		// ����� �÷��̾�� �Ⱥ��̰��Ѱ� ����
		ABladeIIPlayer* pCastedPlayer = Cast<ABladeIIPlayer>(OwnerCharacter);

		if (pCastedPlayer && !pCastedPlayer->GetIsLocalPlayerTeam())
			pCastedPlayer->SetActorHiddenInGame(false);
	}
}

UB2Buff_HealHPByAttack::UB2Buff_HealHPByAttack()
{

}

void UB2Buff_HealHPByAttack::AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		OwnerCharacter->HealingHPByPercent(BuffAmount);
	}
}

UB2Buff_AbnormalStateImmune::UB2Buff_AbnormalStateImmune()
{

}

void UB2Buff_AbnormalStateImmune::OnBeginBuff(const float BuffDuration)
{
	GetBuffManager()->ClearDebuffs();
}

void UB2Buff_AbnormalStateImmune::OnClearBuff(bool bTimeOut)
{
	DimensionBuffCheckClass<>::GetInstance().Signal();
}

UB2Buff_IncreaseMoveSpeed::UB2Buff_IncreaseMoveSpeed()
{

}

void UB2Buff_IncreaseMoveSpeed::OnBeginBuff(const float BuffDuration)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->GetStaticOptionAppliedWalkSpeed() * BuffAmount;
	}
	
}

void UB2Buff_IncreaseMoveSpeed::OnTick(const float DeltaSeconds)
{

}

void UB2Buff_IncreaseMoveSpeed::OnClearBuff(bool bTimeOut)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OwnerCharacter->GetStaticOptionAppliedWalkSpeed();
	}
}

UB2Buff_DebuffAttack::UB2Buff_DebuffAttack()
{
	bDebuff = true;
}

float UB2Buff_DebuffAttack::GetDebuffDuration()
{
	if (m_arAdditionalParams.Num() >= 1)
		return m_arAdditionalParams[0];

	return 0;
}

float UB2Buff_DebuffAttack::GetDebuffTriggerRate()
{
	if (m_arAdditionalParams.Num() >= 2)
		return m_arAdditionalParams[1];

	return 0;
}

void UB2Buff_DebuffAttack::AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
{
	if (Victim->IsImmune(&DamageInfo) || Victim->IsGuardSuccess(&DamageInfo))
		return;

	if (ActualDamage > 0.0f && GetDebuffTriggerRate() > FMath::RandRange(0.0f, 1.0f))
	{
		switch (BuffType)
		{
		case EBuffType::Buff_AllFreezeAttack:
			Victim->AddBuff(EBuffType::Debuff_Freezing, GetDebuffDuration(), 0, GetOwnerCharacter());
			break;
		case EBuffType::Buff_AllStunAttack:
			Victim->AddBuff(EBuffType::Debuff_Stun, GetDebuffDuration(), 0, GetOwnerCharacter());
			break;
		case EBuffType::Buff_AllBurningAttack:
			Victim->AddBuff(EBuffType::Debuff_Burning, GetDebuffDuration(), GetBuffAmount(), GetOwnerCharacter());
			break;
		case EBuffType::Buff_AllPoisonAttack:
			Victim->AddBuff(EBuffType::Debuff_Poison, GetDebuffDuration(), GetBuffAmount(), GetOwnerCharacter());
			break;
		}
	}
}


UB2Buff_HitFreeze::UB2Buff_HitFreeze()
{
	
}

const float UB2Buff_Enhance::ChangeScaleSize = 1.25f;
UB2Buff_Enhance::UB2Buff_Enhance()
{
	ScaleDuration = 0.5f;
	NoAnimateScaleUP = true;
}

void UB2Buff_Enhance::OnBeginBuff(const float BuffDuration)
{
	//if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	//{
	//	OwnerCharacter->AddBuff(EBuffType::Buff_IncreaseAttackDamage, BuffDuration, GetBuffAmount());
	//	OwnerCharacter->AddBuff(EBuffType::Buff_DecreaseReceiveDamage, BuffDuration, GetBuffAmount());
	//	//OwnerCharacter->AddBuff(EBuffType::Buff_IncreaseDefense, BuffDuration, GetBuffAmount());
	//	
	//	//Rim shader ������ ���� ������ �ϴ� �ּ�ó��
	//	//if (UB2DamageEffectInfo* DamageEffectInfo = OwnerCharacter->GetDamageEffectInfo())
	//	//	DamageEffectInfo->OverrideCharacterMaterial(ECharacterMaterialOverrideType::ECMO_Enhance, OwnerCharacter);

	//	NoAnimateScaleUP = BuffDuration - (ScaleDuration * 2.0f) <= 0.0f;

	//	if (OwnerCharacter->GetMesh())
	//	{
	//		RestoreScale = OwnerCharacter->GetMesh()->RelativeScale3D;
	//		if(NoAnimateScaleUP)
	//			OwnerCharacter->GetMesh()->SetRelativeScale3D(RestoreScale * ChangeScaleSize);
	//	}
	//}
}

void UB2Buff_Enhance::OnTick(const float DeltaSeconds)
{
	if (NoAnimateScaleUP)
		return;

	auto ScaleFunction = [this](float Time) 
	{
		float CurrentScaleFactor = FMath::LerpStable(1.0f, ChangeScaleSize, Time / ScaleDuration);
		GetOwnerCharacter()->GetMesh()->SetRelativeScale3D(RestoreScale * CurrentScaleFactor);
	};

	if (GetElapsedTime() <= ScaleDuration)
	{
		ScaleFunction(GetElapsedTime());
	}
	else if (GetRemainTime() <= ScaleDuration)
	{
		ScaleFunction(GetRemainTime());
	}
	else
	{
		GetOwnerCharacter()->GetMesh()->SetRelativeScale3D(RestoreScale * ChangeScaleSize);
	}
}

void UB2Buff_Enhance::OnClearBuff(bool bTimeOut)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		//Rim shader ������ ���� ������ �ϴ� �ּ�ó��
		//OwnerCharacter->RestoreOverrideMaterial(ECharacterMaterialOverrideType::ECMO_Enhance);

		if (OwnerCharacter->GetMesh())
			OwnerCharacter->GetMesh()->SetRelativeScale3D(RestoreScale);
	}
}

UB2Buff_IncreaseDefense::UB2Buff_IncreaseDefense()
{

}

void UB2Buff_IncreaseDefense::OnBeginBuff(const float BuffDuration)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		//���� ���
		//������ ������ �ʾ� �̿ϼ�
	}
}

void UB2Buff_IncreaseDefense::OnClearBuff(bool bTimeOut)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		//restore
	}
}

UB2Buff_Taunt::UB2Buff_Taunt()
{

}
void UB2Buff_Taunt::OnBeginBuff(const float BuffDuration)
{
	//aggropriority �ٲܷ� �ߴµ� Player�� ������ �ٸ�������� ó��..
	//���� ������ ���Ϸ�Ʈ�� ���� �ɸ����� �߰�

	AActor* Causer = BuffCauser.IsValid() ? BuffCauser.Get() : nullptr;

	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
		OwnerCharacter->AddBuff(EBuffType::DeBuff_Silent, BuffDuration, 0.0f, Causer);
}

void UB2Buff_Taunt::OnClearBuff(bool bTimeOut)
{
}

UB2Buff_Silent::UB2Buff_Silent()
{
	bDebuff = true;
}
void UB2Buff_Silent::OnBeginBuff(const float BuffDuration)
{
}

void UB2Buff_Silent::OnClearBuff(bool bTimeOut)
{
}

UB2Buff_Seal::UB2Buff_Seal()
{
	bDebuff = true;
	bUseCrowdControlCount = true;
}

void UB2Buff_Seal::OnBeginBuff(const float BuffDuration)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		ClearDebuffGroup(BuffType, OwnerCharacter);

		// ����ġ�� ����Ʈ ����(����, �Ӹ�Ż�� ����)
		OwnerCharacter->ClearEffectInAbnormalState();
	}
}

void UB2Buff_Seal::OnClearBuff(bool bTimeOut)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		OwnerCharacter->LastStateDamageType = EStateAttackType::EDT_Normal;

		if (BuffManager->IsBuffActive(EBuffType::Debuff_Stun) == false && OwnerCharacter->GetDamagedNum() == 0)
		{
			OwnerCharacter->UpdateDamageNum(FDamageInfo::GetDefaultDamage());
			OwnerCharacter->ApplyDefaultDamageAnimation();
			OwnerCharacter->LastDamageType = FMath::RandBool() ? EAttackType::EAT_LeftSwing : EAttackType::EAT_RightSwing;
		}

		if (BuffManager && bTimeOut)
			BuffManager->CrowdControlCount = 0;

		OwnerCharacter->SetForceComboEnd();
	}
}

float UB2Buff_Seal::GetCustomTimeDilation()
{
	return 0.0f;
}

UB2Buff_InstanceBuff::UB2Buff_InstanceBuff()
{

}

void UB2Buff_InstanceBuff::BuffBegin(UB2CharacterBuffManager * pBuffManager, float fDuration, AActor * InBuffCauser, bool bUseEffect, bool bUseTextEffect, TArray<float>* PtrAdditionalParams)
{
	Super::BuffBegin(pBuffManager, fDuration, InBuffCauser, bUseEffect, bUseTextEffect, PtrAdditionalParams);

	BuffClear(false);
}

void UB2Buff_InstanceBuff::UpdateBuffTick(const float DeltaSeconds)
{
	//nothing
}

UB2Buff_Dispell::UB2Buff_Dispell()
{

}

void UB2Buff_Dispell::OnBeginBuff(const float BuffDuration)
{
	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
		OwnerCharacter->ClearDebuffs();
}

UB2Buff_Heal::UB2Buff_Heal()
{
	HealTickTime = 0.5f;
	CurrentHealCount = 0;
	RemainAmount = 0.0f;
}

void UB2Buff_Heal::OnBeginBuff(const float BuffDuration)
{
	HealTickTime = 0.5f;
	CurrentHealCount = 0;
	RemainAmount = BuffAmount;

	if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
	{
		BuffAmount = FMath::Clamp(BuffAmount, 0.0f, 1.0f);
		if (BuffDuration <= 0.0f)
		{
			OwnerCharacter->HealingHPByPercent(BuffAmount * 100.0f);
			RemainAmount = 0.0f;
		}
	}
}

void UB2Buff_Heal::OnTick(const float DeltaSeconds)
{
	if (GetElapsedTime() >= HealTickTime * CurrentHealCount)
	{
		float TickAmount = HealTickTime / Duration;
		TickAmount = FMath::Clamp(TickAmount, 0.0f, 1.0f);
		TickAmount = BuffAmount * TickAmount;
		TickAmount = FMath::Min(TickAmount, RemainAmount);
		RemainAmount -= TickAmount;
		CurrentHealCount++;

		if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
			OwnerCharacter->HealingHPByPercent(TickAmount * 100.0f);
	}
}

void UB2Buff_Heal::OnClearBuff(bool bTimeOut)
{
	if (RemainAmount > 0.0f)
	{
		if (ABladeIICharacter* OwnerCharacter = GetOwnerCharacter())
			OwnerCharacter->HealingHPByPercent(RemainAmount * 100.0f);
	}
}