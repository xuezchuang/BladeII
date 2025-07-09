#pragma once

#include "B2UIWidgetBase.h"
#include "B2UITeamEntryCharacter.generated.h"

class UB2UIInGameBuffIcon;

DECLARE_DELEGATE_OneParam(FB2UITeamEntryCharacter_OnClickSelect, EPCClass)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamEntryCharacter : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BindDelegates() override;
	void SetEntryCharacter(EPCClass CharacterClass, int32 CharacterLevel);
	EPCClass GetPCClass() { return EntryPCClass; }

	void SetVisibilityBtnSelect(bool b);
	void SetSelectSlot(bool IsSelect);

	UFUNCTION()
	void OnClickBtnSelect();

	UFUNCTION(BlueprintImplementableEvent)
	void SetSelectSlot_BP(bool IsSelect);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UImage>			IMG_Character;
	TWeakObjectPtr<UImage>			IMG_DisableCharacter;
	TWeakObjectPtr<UTextBlock>		TBS_LvText;
	TWeakObjectPtr<UB2Button>		BTN_Select;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInterface*>		CharacterTexture;

	FB2UITeamEntryCharacter_OnClickSelect	FOnClickBtnSelect;

private:
	EPCClass								EntryPCClass;
	TArray<UB2UIInGameBuffIcon*>			AllResurrectionBuffIcons;
};