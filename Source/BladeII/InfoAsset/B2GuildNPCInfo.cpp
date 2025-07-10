
#include "B2GuildNPCInfo.h"
#include "Materials/Material.h"

#if WITH_EDITOR
void FGuildNPCInfo::EditorOnPostLoad()
{
	NPCSlotIcon.LoadSynchronous();
	NPCUnSlotIcon.LoadSynchronous();
	NPCSlotLock.LoadSynchronous();
	NPCSlotCircleIcon.LoadSynchronous();
	SpecialSkill1.LoadSynchronous();
	SpecialSkill2.LoadSynchronous();
	SpecialSkill3.LoadSynchronous();
}

void FGuildMercenarySkill::EditorOnPostLoad()
{
	SkillImageNormal.LoadSynchronous();
	SkillImageDisabled.LoadSynchronous();
}

#endif

FMercenaryMasterySkillInfo::FMercenaryMasterySkillInfo()
{
	TargetingType = 0;
	RestrictExcuteCount = -1;
}

UB2GuildNPCInfo::UB2GuildNPCInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2GuildNPCInfo::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	for (FGuildNPCInfo& NPCInfo : GuildNPCInfoArray)
	{
		NPCInfo.EditorOnPostLoad();
	}

	for (FGuildMercenarySkill& SkillInfo : GuildMercenarySkillArray)
	{
		SkillInfo.EditorOnPostLoad();
	}

#endif
}

const FGuildNPCInfo* UB2GuildNPCInfo::GetGuildNPCInfoOfID(int32 NPCID)
{
	for (FGuildNPCInfo& NPCInfo : GuildNPCInfoArray)
	{
		if (NPCInfo.NPCID == NPCID)
		{
			NPCInfo.NPCSlotIcon.LoadSynchronous();
			NPCInfo.NPCUnSlotIcon.LoadSynchronous();
			NPCInfo.NPCSlotLock.LoadSynchronous();
			NPCInfo.NPCSlotCircleIcon.LoadSynchronous();
			NPCInfo.SpecialSkill1.LoadSynchronous();
			NPCInfo.SpecialSkill2.LoadSynchronous();
			NPCInfo.SpecialSkill3.LoadSynchronous();

			return &NPCInfo;
		}
	}

	return nullptr;
}

const FGuildMercenarySkill* UB2GuildNPCInfo::GetMercenarySkillOfID(int32 SkillID)
{

	for (FGuildMercenarySkill& SkillInfo : GuildMercenarySkillArray)
	{
		if (SkillInfo.SkillID == SkillID)
		{
			SkillInfo.SkillImageNormal.LoadSynchronous();
			SkillInfo.SkillImageDisabled.LoadSynchronous();

			return &SkillInfo;
		}
	}


	return nullptr;
}

UMaterialInterface * UB2GuildNPCInfo::GetNPCSlotIcon(int32 NPCID)
{
	FGuildNPCInfo* GuildNpcInfo = GuildNPCInfoArray.FindByPredicate([NPCID](const FGuildNPCInfo& NpcInfo) { return NpcInfo.NPCID == NPCID; });
	
	if (GuildNpcInfo)
		return Cast<UMaterialInterface>(GuildNpcInfo->NPCSlotIcon.LoadSynchronous());

	return nullptr;
}

const FMercenaryMasterySkillInfo* UB2GuildNPCInfo::GetMasterySkillInfo(EMercenarySkillOption InOption)
{
	if (MasterySkillMap.Contains(InOption))
		return &(MasterySkillMap[InOption]);

	return nullptr;
}


