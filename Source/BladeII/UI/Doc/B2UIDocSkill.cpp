#include "B2UIDocSkill.h"
#include "B2SkillInfo.h"
#include "B2SkillRTManager.h"

UB2UIDocSkill::UB2UIDocSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMultiple = true;
	bAnonymous = false;
	DocType = EUIDocType::Skill;
	SkillInfoSkillID = SKILL_INVALID_ID;
	IsExistingSkill = true;
}

UB2UIDocSkill::~UB2UIDocSkill()
{

}

void UB2UIDocSkill::SetAndLoadFromSkillID(int32 InSkillInfoSkillID)
{
	//SkillInfoSkillID = InSkillInfoSkillID;

	//SkillLevel = UB2SkillInfo::GetEnablePlayerLevel(InSkillInfoSkillID);

	//SkillInputIndex = -1;
}