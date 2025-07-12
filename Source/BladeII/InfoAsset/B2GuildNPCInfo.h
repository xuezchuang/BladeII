#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "UMG.h"
#include "BladeIIPlayer.h"
#include "B2SkillInfo.h"
#include "../PassiveSkill/B2SkillParameter.h"
//#include "B2SkillParameter.h"
#include "B2GuildNPCInfo.generated.h"


USTRUCT(BlueprintType)
struct FGuildNPCInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	int32 NPCID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildNPCInfo")
	ENPCClass NPCClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildNPCInfo")
	ESkillType SpecialType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> NPCSlotIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> NPCUnSlotIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> NPCSlotLock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> NPCSlotCircleIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> SpecialSkill1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> SpecialSkill2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> SpecialSkill3;

#if WITH_EDITOR
	void EditorOnPostLoad();
#endif

};

USTRUCT(BlueprintType)
struct FGuildMercenarySkill
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FGuildMercenarySkill")
	int32 SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FGuildMercenarySkill")
	TSoftObjectPtr<class UMaterialInterface> SkillImageNormal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FGuildMercenarySkill")
	TSoftObjectPtr<class UMaterialInterface> SkillImageHover;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FGuildMercenarySkill")
	TSoftObjectPtr<class UMaterialInterface> SkillImagePress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FGuildMercenarySkill")
	TSoftObjectPtr<class UMaterialInterface> SkillImageDisabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FGuildMercenarySkill")
	bool bUseSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FGuildMercenarySkill")
	int32 SkillAttackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildNPCInfo")
	ESkillType SkillType;

#if WITH_EDITOR
	void EditorOnPostLoad();
#endif

};

USTRUCT()
struct FMercenaryMasterySkillInfo
{
	GENERATED_USTRUCT_BODY()

	FMercenaryMasterySkillInfo();
	~FMercenaryMasterySkillInfo() {}

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2SkillTrigger> SkillTriggerType;

	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = ETargetSelectType))
	uint8 TargetingType;

	UPROPERTY(EditAnywhere)
	int32 RestrictExcuteCount;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2SkillCommand> SkillCommandType;

	UPROPERTY(EditAnywhere)
	FB2SkillParameterContainer SkillParamters;
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2GuildNPCInfo : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGuildNPCInfo> GuildNPCInfoArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGuildMercenarySkill> GuildMercenarySkillArray;

	UPROPERTY(EditAnywhere)
	TMap<EMercenarySkillOption, FMercenaryMasterySkillInfo> MasterySkillMap;


public:
	UB2GuildNPCInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;
	const FGuildNPCInfo* GetGuildNPCInfoOfID(int32 NPCID);
	const FGuildMercenarySkill* GetMercenarySkillOfID(int32 SkillID);
	UMaterialInterface* GetNPCSlotIcon(int32 NPCID);
	const FMercenaryMasterySkillInfo* GetMasterySkillInfo(EMercenarySkillOption InOption);
};