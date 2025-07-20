// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square


#include "B2InfluenceSkillInfo.h"

FSingleInfluenceSkillInfo::FSingleInfluenceSkillInfo()
{
	SkillID						= SKILL_INVALID_ID;
	Influence					= EInfluenceType::EIT_End;
}

#if WITH_EDITOR
void FSingleInfluenceSkillInfo::EditorOnPostLoad()
{ 

}
#endif

UB2InfluenceSkillInfo::UB2InfluenceSkillInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UB2InfluenceSkillInfo::PostLoad()
{
	for (int32 i = 0; i < SkillInfoArray.Num(); ++i)										
	{																									
		SkillInfoMap.Add(SkillInfoArray[i].Influence, &SkillInfoArray[i]);
	}
	
	Super::PostLoad();

#if WITH_EDITOR
	if (GIsEditor)
	{
		for (FSingleInfluenceSkillInfo& ThisSkillInfo : SkillInfoArray) {
			ThisSkillInfo.EditorOnPostLoad();
		}
	}
#endif
}

const FSingleInfluenceSkillInfo* UB2InfluenceSkillInfo::GetSingleInfoOfID(EInfluenceType fluenceType) const
{
	auto SkillInfo = SkillInfoMap.Find(fluenceType);

	return SkillInfo ? *SkillInfo : nullptr;
}

#if WITH_EDITOR
void UB2InfluenceSkillInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SkillInfoMap.Empty();

	for (int32 i = 0; i < SkillInfoArray.Num(); ++i)
	{
		SkillInfoMap.Add(SkillInfoArray[i].Influence, &SkillInfoArray[i]);
	}
}
#endif
