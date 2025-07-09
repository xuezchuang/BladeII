#include "BladeII.h"
#include "BladeIICharacter.h"
#include "B2SkillTrigger.h"
#include "B2SkillCommand.h"
#include "B2TargetSelector.h"
#include "B2PassiveSkill.h"

void UB2PassiveSkill::BeginDestroy()
{
	SkillTrigger = nullptr;
	SkillCommand = nullptr;
	OwnerCharacter = nullptr;

	Super::BeginDestroy();
}

void UB2PassiveSkill::InitSkill(ABladeIICharacter* InOwnerCharacter, int32 InRestrictExcuteCount, EnumMaskValue<ETargetSelectType>&& InTargetSelectType, UB2SkillTrigger * InSkillTrigger, UB2SkillCommand * InSkillCommand, const FB2SkillParameterContainer& InSkillParamContainer)
{
	if (!InSkillTrigger || !InSkillCommand )
		return;

	OwnerCharacter = InOwnerCharacter;
	RestrictExcuteCount = InRestrictExcuteCount;
	TargetSelectType = std::forward<EnumMaskValue<ETargetSelectType>>(InTargetSelectType);

	SkillTrigger = InSkillTrigger;
	SkillCommand = InSkillCommand;
	SkillTrigger->InitTrigger(this, InSkillParamContainer);
	SkillCommand->InitCommand(this, InSkillParamContainer);
}

void UB2PassiveSkill::StartSkill()
{
	if (RestrictExcuteCount == 0)
	{
		if (SkillTrigger)
			SkillTrigger->UnRegistEvent();
		return;
	}
	
	--RestrictExcuteCount;
	TArray<ABladeIICharacter*> FoundTargets;
	B2TargetSelector::GetInstance().FindTargetSelectFlag(FoundTargets, TargetSelectType, GetOwner());

	if (SkillCommand)
		SkillCommand->Excute(FoundTargets);
}

ABladeIICharacter * UB2PassiveSkill::GetOwner() const
{
	return OwnerCharacter;
}
