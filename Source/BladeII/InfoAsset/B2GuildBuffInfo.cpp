
#include "B2GuildBuffInfo.h"
#include "Materials/Material.h"

#if WITH_EDITOR
void FSingleGuildSkillInfo::EditorOnPostLoad()
{
	ButtonIcon_Normal.LoadSynchronous();
	ButtonIcon_Disabled.LoadSynchronous();
}
#endif

UB2GuildSkillInfo::UB2GuildSkillInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2GuildSkillInfo::PostLoad()
{
	Super::PostLoad();


#if WITH_EDITOR

	for (FSingleGuildSkillInfo& SkillInfo : SkillInfoArray)
	{
		SkillInfo.EditorOnPostLoad();
	}
#endif
}

const FSingleGuildSkillInfo* UB2GuildSkillInfo::GetSingleInfoOfID(int32 SkillInfoID)
{
	for (FSingleGuildSkillInfo& SkillInfo : SkillInfoArray)
	{
		if (SkillInfo.SkillID == SkillInfoID)
		{
			SkillInfo.ButtonIcon_Normal.LoadSynchronous();
			SkillInfo.ButtonIcon_Disabled.LoadSynchronous();
			return &SkillInfo;
		}
	}

	return nullptr;
}
