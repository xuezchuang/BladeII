#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "UMG.h"
#include "BladeIIPlayer.h"
#include "B2GuildBuffInfo.generated.h"

UENUM(BlueprintType)
enum class EGuildSkillType : uint8
{
	EGuildSkillType_Mod,
	EGuildSkillType_GuildMod,
};

USTRUCT(BlueprintType)
struct FSingleGuildSkillInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	int32 SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	EGuildSkillType GuildSkillType;

	/* Button icon State*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> ButtonIcon_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GuildSkillInfo")
	TSoftObjectPtr<class UMaterialInterface> ButtonIcon_Disabled;

#if WITH_EDITOR
	void EditorOnPostLoad();
#endif

};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2GuildSkillInfo : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FSingleGuildSkillInfo> SkillInfoArray;

public:
	UB2GuildSkillInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;
	const FSingleGuildSkillInfo* GetSingleInfoOfID(int32 SkillInfoID);
};