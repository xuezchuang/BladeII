#include "B2PassiveSkillFactory.h"
#include <type_traits>
#include "B2SkillTrigger.h"
#include "B2SkillCommand.h"


UB2PassiveSkill* PassiveSkillFactory::CreateMasteryPassiveSkill(class ABladeIICharacter* SkillOwner, const FMercenaryMasterySkillInfo & Info, float ServerValue)
{
	//validation
	if (SkillOwner == nullptr)
		return nullptr;

	if (Info.SkillTriggerType.Get() == nullptr)
		return nullptr;

	if (Info.SkillCommandType.Get() == nullptr)
		return nullptr;

	UB2PassiveSkill* NewPassiveSkill = NewObject<UB2PassiveSkill>(SkillOwner, NAME_None, RF_Transient);
	UB2SkillTrigger* NewSkillTrigger = NewObject<UB2SkillTrigger>(NewPassiveSkill, Info.SkillTriggerType, NAME_None, RF_Transient);
	UB2SkillCommand* NewSkillCommand = NewObject<UB2SkillCommand>(NewPassiveSkill, Info.SkillCommandType, NAME_None, RF_Transient);

	if (NewPassiveSkill == nullptr || NewSkillTrigger == nullptr || NewSkillCommand == nullptr)
		return nullptr;				  

	//Copy Param Container
	FB2SkillParameterContainer CopyContainer = Info.SkillParamters;
	CopyContainer.InitContainer(ServerValue);

	NewPassiveSkill->InitSkill(SkillOwner, Info.RestrictExcuteCount, EnumMaskValue<ETargetSelectType>(Info.TargetingType), NewSkillTrigger, NewSkillCommand, CopyContainer);

	return NewPassiveSkill;
}
