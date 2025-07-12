#pragma once

#include "B2UIWidgetBase.h"
#include "../../Common/CommonStruct.h"
#include "B2UITeamEntrySelectSlot.generated.h"

DECLARE_DELEGATE(FTeamMatchSelectCharacter);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamEntrySelectSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetCharacterSlot(EPCClass CharacterClass);
	void SetEnterTeamMatchCheck(EPCClass CharacterClass);
	//FTeamMatchSelectCharacter GetFormationSlotUpdateDelegate() { return FormationSlotUpdateDelegate;}

public:
	UFUNCTION()
	void OnClickSelect();

public:
	//FTeamMatchSelectCharacter FormationSlotUpdateDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInterface*>		CharacterTexture;

private:
	TWeakObjectPtr<UImage>		IMG_Character;
	TWeakObjectPtr<UTextBlock>	TBS_CharacterLv;
	TWeakObjectPtr<UTextBlock>	TBS_CharacterJob;
	TWeakObjectPtr<UTextBlock>	TBS_Att;
	TWeakObjectPtr<UTextBlock>	TBS_Def;
	TWeakObjectPtr<UCanvasPanel>	CP_CharacterCheck;
	TWeakObjectPtr<UButton>			BTN_Select;
	EPCClass						ChacacterClassEnum;
};