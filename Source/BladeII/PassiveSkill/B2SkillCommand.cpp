
#include "B2SkillCommand.h"
#include "B2PassiveSkill.h"

#include "BladeIIAIController.h"


void UB2SkillCommand::InitCommand(class UB2PassiveSkill* InOwnerPassiveSkill, const FB2SkillParameterContainer& InSkillParamContainer)
{
	OwnerPassiveSkill = InOwnerPassiveSkill;
	SetSkillParameter(InSkillParamContainer);
}

void UB2SkillCommand::SetSkillParameter(const FB2SkillParameterContainer & InSkillParamContainer)
{
	FB2SkillParameter::InitializeParameter(this, InSkillParamContainer);
}

void UB2SkillCommand_AddBuff::Excute(TArray<class ABladeIICharacter*>& ApplyCharacters)
{
	for (ABladeIICharacter* Chracter : ApplyCharacters)
	{
		float BonusDuration = 0.0f;
		float BonusAmount = 0.0f;
		AddBonusProperty(Chracter, BonusDuration, BonusAmount);

		Chracter->AddBuff(BuffType, ApplyDuration.GetValue() + BonusDuration, ApplyAmount.GetValue() + BonusAmount, OwnerPassiveSkill ? OwnerPassiveSkill->GetOwner() : nullptr);
	}
}

void UB2SkillCommand_AddBuff::AddBonusProperty(ABladeIICharacter * ApplyCharacter, float & OutBonusDuration, float & OutBonusAmount)
{
	//EUniqueBuffType 적용할 때마다 기존 Player에서 하던 짓 해줘야함
	OutBonusDuration = 0.0f;
	OutBonusAmount = 0.0f;

	if (ApplyCharacter == nullptr)
		return;

	switch (BuffType)
	{
	case EBuffType::Buff_HealHPByAttack:
		OutBonusAmount += ApplyCharacter->GetBonusBuffHealHPByHit() * 100.f;
		break;
	}
}

void UB2SkillCommand_Transfusion::Excute(TArray<class ABladeIICharacter*>& ApplyCharacters)
{
	//pahse change 후
	for (ABladeIICharacter* Chracter : ApplyCharacters)
	{
		if (Chracter && Chracter->IsAlive())
		{
			Chracter->bInPhaseAnimation = true;
			ABladeIIAIController* Controller = Cast<ABladeIIAIController>(Chracter->GetController());
			if(Controller)
				Controller->StopMovement();
		}
	}

	//힐 버프 실행
	Super::Excute(ApplyCharacters);
}