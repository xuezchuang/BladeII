// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "UObject/Object.h"
#include "BladeIIPlayer.h"
#include "B2InfluenceSkillInfo.generated.h"

USTRUCT(BlueprintType)
struct FSingleInfluenceSkillInfo
{
	GENERATED_USTRUCT_BODY()

	FSingleInfluenceSkillInfo();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	EInfluenceType Influence;

	/** The final skill ID applied in AnimBP. See also EAttackState */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	int32 SkillID;

#if WITH_EDITOR
	void EditorOnPostLoad();
#endif
};

/**
* Skill Information for player character.. and possibly for NPCs too? (such as boss mob..)
* Intended to make an object of this class for each character class that uses skill.
*/
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2InfluenceSkillInfo : public UObject
{
	GENERATED_BODY()

private:
	TMap<EInfluenceType, FSingleInfluenceSkillInfo*> SkillInfoMap;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FSingleInfluenceSkillInfo> SkillInfoArray;
public:
	UB2InfluenceSkillInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;

	const FSingleInfluenceSkillInfo* GetSingleInfoOfID(EInfluenceType fluenceType) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
