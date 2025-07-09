#include "BladeII.h"
#include "B2UITeamEntryCharacter.h"
#include "B2UIInGameBuffIcon.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "BladeIILocText.h"

void UB2UITeamEntryCharacter::Init()
{
	Super::Init();
	EntryPCClass = EPCClass::EPC_Gladiator;
}

void UB2UITeamEntryCharacter::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UITeamEntryCharacter::OnClickBtnSelect);
}

void UB2UITeamEntryCharacter::SetEntryCharacter(EPCClass CharacterClass, int32 CharacterLevel)
{
	if (CharacterTexture.Num() < GetMaxPCClassNum())
		return;

	EntryPCClass = CharacterClass;

	if (IMG_Character.IsValid())
		IMG_Character->SetBrushFromMaterial(CharacterTexture[PCClassToInt(CharacterClass)]);

	if (IMG_DisableCharacter.IsValid())
		IMG_DisableCharacter->SetBrushFromMaterial(CharacterTexture[PCClassToInt(CharacterClass)]);

	if (TBS_LvText.IsValid())
	{
		TBS_LvText->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(CharacterLevel))));
	}

}

void UB2UITeamEntryCharacter::SetVisibilityBtnSelect(bool b)
{
	if (BTN_Select.IsValid())
		BTN_Select->SetVisibility(b ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UITeamEntryCharacter::OnClickBtnSelect()
{
	FOnClickBtnSelect.ExecuteIfBound(EntryPCClass);
}

void UB2UITeamEntryCharacter::CacheAssets()
{
	GET_SLOT(UImage, IMG_Character)
	GET_SLOT(UImage, IMG_DisableCharacter)
	GET_SLOT(UTextBlock, TBS_LvText)
	GET_SLOT(UB2Button, BTN_Select)
}

void UB2UITeamEntryCharacter::SetSelectSlot(bool IsSelect)
{
	SetSelectSlot_BP(IsSelect);
}

