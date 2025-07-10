#include "B2SkillTrigger.h"
#include "Event.h"

#include "B2PassiveSkill.h"

void UB2SkillTrigger::UnRegistEvent()
{
	Issues.Empty();
}

void UB2SkillTrigger::SetSkillParameter(const FB2SkillParameterContainer & InSkillParamContainer)
{
	FB2SkillParameter::InitializeParameter(this, InSkillParamContainer);
}

void UB2SkillTrigger::InitTrigger(class UB2PassiveSkill* InOwnerPassiveSkill, const FB2SkillParameterContainer& InSkillParamContainer)
{
	OwnerPassiveSkill = InOwnerPassiveSkill;
	RegistEvent();
	SetSkillParameter(InSkillParamContainer);
}

void UB2SkillTrigger::BeginDestroy()
{
	UnRegistEvent();
	Super::BeginDestroy();
}

void UB2SkillTrigger::OnTriggeringPassiveSkill()
{
	if (OwnerPassiveSkill)
		OwnerPassiveSkill->StartSkill();
}


void UB2SkillTrigger_StartGame::RegistEvent()
{
	Issues.Add(FightStartClass<int32>::GetInstance().Subscribe2(
		[this](int32 RoundNumber)
	{
		OnTriggeringPassiveSkill();
	}
	));
}

void UB2SkillTrigger_LowHealth::RegistEvent()
{
	Issues.Add(CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Subscribe2(
		[this](ABladeIICharacter* DamagedCharacter, float ActualDamage, bool bCriticalDamage)
	{
		if (DamagedCharacter && DamagedCharacter->IsAlive() && 
			OwnerPassiveSkill && OwnerPassiveSkill->GetOwner() && 
			OwnerPassiveSkill->GetOwner() == DamagedCharacter)
		{
			ABladeIICharacter* OwnerCharacter = OwnerPassiveSkill->GetOwner();
			float const CurrHealthRate = OwnerCharacter->GetHealth() / OwnerCharacter->GetMaxHealth();
			if (TriggerHealtRatio.GetValue() >= CurrHealthRate)
			{
				OnTriggeringPassiveSkill();
			}
		}
	}
	));
}