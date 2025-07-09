#pragma once

#include "B2UIWidgetBase.h"
#include "UnitySkillManager.h"
#include "B2UIGuildMercenary.h"
#include "B2UISkillOptionList.generated.h"

UENUM()
enum class ESkillOptionTitleType : uint8
{
	CurrentSkillOption,
	NextSkillOption,
	SpecialOption
};

USTRUCT()
struct FSkillOptionListTitle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ESkillOptionTitleType		Type;
	
	UPROPERTY(EditAnywhere)
	class UMaterialInterface*	TitleImage;
	
	UPROPERTY(EditAnywhere)
	FSlateColor					FontColor;
	
	UPROPERTY(EditAnywhere)
	FString						TextCodeName;
};

UCLASS(BlueprintType)
class BLADEII_API UB2UISkillOptionList : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	void SetSkillOptionType(ESkillOptionTitleType InOptionType, int32 SkillLevel = 0);

	void InitializeList(int32 SkillId, int32 SkillLevel, bool OnlyNew = false);
	void InitializeUnitySkillList(int32 SkillId, EPCClass MainClass);
	void InitializeMercenaryOptions(int32 NPCID, int32 InLv, ENPCSpecialSKill InSpecialSkillTpye);
	void SetSpecialOptionList(int32 SkillId, int32 SkillLevel);

protected:
	virtual void CacheAssets() override;

	TArray<FSkillOptionData> GetSkillOptions(int32 SkillId, int32 SkillLevel, bool OnlyNew = false);
	void DisplaySkillOptions(int32 SkillId, TArray<FSkillOptionData> & SkillOptionDataArray);
	void DisplayUnitySkillOptions(int32 SkillId, const TArray<FUnitySkillOption>& UnityDataArray);
protected:
	UPROPERTY(EditAnywhere)
	TArray<FSkillOptionListTitle>	SkillOptionListTitleSkin;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2UISkillOptionText> SkillOptionTextClass;

private:
	TWeakObjectPtr<UVerticalBox>	VB_SkillOptionList;
	TWeakObjectPtr<UImage>			IMG_Title;
	TWeakObjectPtr<UTextBlock>		TB_Title;

	ESkillOptionTitleType			OptionType;
};
