// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "Styling/SlateBrush.h"
#include "B2UIDocBase.h"
#include "CommonStruct.h"
#include "B2UIDocHero.generated.h"

/* This is a document that reflects the data of the respective heroes. */
UCLASS()
class BLADEII_API UB2UIDocHero : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocHero(const FObjectInitializer& ObjectInitializer);

	DECLARE_UIDOC_FIELD(FText, ClassDesc)

	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore refactoring
	//DECLARE_UIDOC_FIELD(int32, BeforeTotalExp)
	//DECLARE_UIDOC_FIELD(int32, AfterTotalExp)
	//DECLARE_UIDOC_FIELD(int32, CurrentExp)
	//DECLARE_UIDOC_FIELD(int32, CurrentExpPercent) // Percent in its matching level's exp range. 0 ~ 100
	//											  /** Exp point that the hero will get as a result of stage clear. */
	//DECLARE_UIDOC_FIELD(int32, BasicUpgradeExp)
	//DECLARE_UIDOC_FIELD(int32, BasicUpgradeExpPercent) // Percent in its matching level's exp range. 0 ~ 100
	//												   /** Exp point that the hero will get as a result of stage bonus. */
	//DECLARE_UIDOC_FIELD(int32, ExtraUpgradeExp)
	//DECLARE_UIDOC_FIELD(int32, ExtraUpgradeExpPercent) // Percent in its matching level's exp range. 0 ~ 100

	//												   /** The player's level in most time, before getting upgrade as a result of combat. */
	//DECLARE_UIDOC_FIELD(int32, CurrentLevel)
	///** Level that the hero will get as a result of stage clear. */
	//DECLARE_UIDOC_FIELD(int32, BasicUpgradeLevel)
	///** Level that the hero will get as a result of stage bonus. */
	//DECLARE_UIDOC_FIELD(int32, ExtraUpgradeLevel)


	/** Most recent acquired skill point as a result of level-up */
	DECLARE_UIDOC_FIELD(int32, LastAcquiredSkillPoint)

	/** Three major stats of a hero. */
	DECLARE_UIDOC_FIELD(int32, CombatPower)
	DECLARE_UIDOC_FIELD(int32, PreCombatPower)
	DECLARE_UIDOC_FIELD(int32, AttackPower)
	DECLARE_UIDOC_FIELD(int32, DefensePower)
	DECLARE_UIDOC_FIELD(int32, HealthPoint) // NOT the current health during combat.
	/** They are different from basic stats ONLY WHEN this hero got level-up at stage clear moment. */
	DECLARE_UIDOC_FIELD(int32, PostClearAttackPower)
	DECLARE_UIDOC_FIELD(int32, PostClearDefensePower)
	DECLARE_UIDOC_FIELD(int32, PostClearHealthPoint)

	/** If the combat power has been changed */
	DECLARE_UIDOC_FIELD(bool, ChangedCombatPower)

	/** Ether Cooltime in battle **/
	DECLARE_UIDOC_FIELD(float, CurOffenseEtherCoolTime);
	DECLARE_UIDOC_FIELD(float, CurDefenseEtherCoolTime)

public:
	virtual void Init() override;
	virtual void Destroy() override;
	void InitResources();
	void ReleaseResources();

	FORCEINLINE EPCClass GetHeroClass() { return IntToPCClass(this->GetDocId()); }

	class UB2PCClassInfo* GetRelevantPCClassInfo();
	//class UB2InGameHUDInfo_Player* GetRelevantHUDInfo(); // HUD info contains something like button style, image brush stuff which can be defined differently per character.
	
	//static class UB2InGameHUDInfo_Player* StaticGetRelevantHUDInfo(EPCClass InPClass);
	
	// It will replace PortraitIcon.. This UIDoc is not supposed to have resource reference. Just helper functions to replace old code.
	static class UMaterialInterface* GetPlayerPortraitMaterial(UObject* WorldContextObject, EPCClass InPCClass); 
	static class UMaterialInterface* GetPlayerDeathPortraitMaterial(UObject* WorldContextObject, EPCClass InPCClass);
	static class UTexture2D* GetPlayerLargePortraitTexture(EPCClass InPCClass);

	void AddSkillDoc(class UB2UIDocSkill* NewSkillDoc, int32 InSkillInfoSkillID, int32 ToPlaceInput);
	
	class UB2UIDocSkill* GetSkillDocOfID(int32 InSkillInfoSkillID);
	class UB2UIDocSkill* GetSkillDocByInputIndex(int32 InputIndex);

	//void AddEquipDoc(class UB2UIDocEquip* NewEquipDoc, int32 EquipPlaceIndex);
	//class UB2UIDocEquip* GetEquipDocByPlaceIndex(int32 EquipPlaceIndex);

	//void InitExp();
	//void UpdateExp();

	void SetPowers(const int32 iLevel);
	void SetPostClearPowers(const int32 iLevel);

private:
	TArray<class UB2UIDocSkill*> SkillDocs;
	TArray<class UB2UIDocEquip*> EquipDocs;
};
