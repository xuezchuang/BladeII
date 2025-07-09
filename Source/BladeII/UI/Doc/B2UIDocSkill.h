// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "UMG.h"
#include "B2UIDocBase.h"
#include "BladeIICharacter.h"
#include "B2UIDocSkill.generated.h"

class UMaterialInterface;

/* Both statically defined and dynamically updated usage state data of a single skill */
UCLASS()
class BLADEII_API UB2UIDocSkill : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocSkill(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocSkill();

	//////////////////////////////////////////////////
	// Statically defined data by SkillInfo

	DECLARE_UIDOC_FIELD(FText, SkillName)

	/** At which level player can use this skill? */
	DECLARE_UIDOC_FIELD(int32, SkillLevel)

	//////////////////////////////////////////////////

	// Docs having valid SkillInputIndex will be used.
	DECLARE_UIDOC_FIELD(int32, SkillInputIndex)

	DECLARE_UIDOC_FIELD(float, SkillStackMaxPoint)

	//////////////////////////////////////////////////
	// Dynamic skill state, to be updated only for skills for current active player character and currently bound to one of inputs.

	DECLARE_UIDOC_FIELD(bool, IsExistingSkill) // Very basic information, but sometimes need to check..
	DECLARE_UIDOC_FIELD(bool, IsDisabledByCooltime) // A little longer than cooltime counting period.
	DECLARE_UIDOC_FIELD(bool, IsDisabledByDebuff)

	DECLARE_UIDOC_FIELD(bool, IsCountingCooltime) // A little shorter than whole disabled by cooltime state.
	DECLARE_UIDOC_FIELD(float, FinalCooltime) // Almost static data come from SkillInfo, but could be different due to some other conditions
	DECLARE_UIDOC_FIELD(float, RemainingCooltime) // Less than FinalCooltime when it is counted.

	DECLARE_UIDOC_FIELD(float, SkillStackPoint)
	//////////////////////////////////////////////////

	DECLARE_UIDOC_FIELD(bool, IsSuggestSkill)
	DECLARE_UIDOC_FIELD(bool, IsDisabledByOtherSuggest)	// force disable this skill when other Suggest(Survive) skill is activated.


private:
	int32 SkillInfoSkillID; // This is like a key within a hero doc.

public:

	void SetAndLoadFromSkillID(int32 InSkillID);

	/* It sets the hero who owns this skill. */
	void SetOwnerHeroDoc(class UB2UIDocHero* OwnerDoc)	{ OwnerHeroDoc = OwnerDoc; }
	class UB2UIDocHero* GetOwnerHeroDoc() const			{ return OwnerHeroDoc; }

	FORCEINLINE int32 GetSkillInfoSkillID() { return SkillInfoSkillID; }

private:
	class UB2UIDocHero* OwnerHeroDoc;
};
