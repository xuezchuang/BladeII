
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
	//EUniqueBuffType 利侩且 锭付促 扁粮 Player俊辑 窍带 窿 秦拎具窃
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
	//pahse change 饶
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

	//鳃 滚橇 角青
	Super::Excute(ApplyCharacters);
}