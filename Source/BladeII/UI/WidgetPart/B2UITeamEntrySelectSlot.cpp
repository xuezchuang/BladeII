#include "BladeII.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2UITeamEntrySelectSlot.h"
#include "B2TMGameMode.h"


#include "BladeIIGameImpl.h"

void UB2UITeamEntrySelectSlot::Init()
{
	Super::Init();
}

void UB2UITeamEntrySelectSlot::CacheAssets()
{
	GET_SLOT(UImage,		IMG_Character)
	GET_SLOT(UTextBlock,	TBS_CharacterLv)
	GET_SLOT(UTextBlock,	TBS_CharacterJob)
	GET_SLOT(UTextBlock,	TBS_Att)
	GET_SLOT(UTextBlock,	TBS_Def)
	GET_SLOT(UCanvasPanel,	CP_CharacterCheck)
	GET_SLOT(UButton,		BTN_Select)
}

void UB2UITeamEntrySelectSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UITeamEntrySelectSlot::OnClickSelect);
}

void UB2UITeamEntrySelectSlot::SetCharacterSlot(EPCClass CharacterClass)
{
	ChacacterClassEnum = CharacterClass;
	UB2UIDocHero* HeroDoc = UB2UIDocHelper::GetDocHero(PCClassToInt(CharacterClass));

	if (!HeroDoc)
		return;

	if (CharacterTexture.Num() < GetMaxPCClassNum())
		return;

	if (IMG_Character.IsValid())
		IMG_Character->SetBrushFromMaterial(CharacterTexture[PCClassToInt(CharacterClass)]);

	if (TBS_CharacterLv.IsValid())
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
		//TBS_CharacterLv->SetText(
		//	FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(HeroDoc->GetCurrentLevel())))
		//	);
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
		const int32 CharLevel = CharacterDataStore.GetCharacterLevel(HeroDoc->GetHeroClass());
		TBS_CharacterLv->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(CharLevel)))
		);
	}

	if (TBS_CharacterJob.IsValid())
		TBS_CharacterJob->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(HeroDoc->GetHeroClass()));


	if (TBS_Att.IsValid())
		TBS_Att->SetText(FText::AsNumber(HeroDoc->GetAttackPower()));

	if (TBS_Def.IsValid())
		TBS_Def->SetText(FText::AsNumber(HeroDoc->GetDefensePower()));

	SetEnterTeamMatchCheck(CharacterClass);
}

void UB2UITeamEntrySelectSlot::SetEnterTeamMatchCheck(EPCClass CharacterClass)
{
	UB2UIDocTeamMatch* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamDoc)
		return;

	for (int32 idx = 0; idx < /*AB2TMGameMode::GetNumCharactersOfTeam()*/3; ++idx)//임시
	{
		if (TeamDoc->GetCharacterClass(true, idx) == CharacterClass)
		{
			CP_CharacterCheck->SetVisibility(ESlateVisibility::Visible);
			return;
		}			
	}

	CP_CharacterCheck->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UITeamEntrySelectSlot::OnClickSelect()
{
	UB2UIDocTeamMatch* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamDoc)
		return;

	for (int32 idx = 0; idx < /*AB2TMGameMode::GetNumCharactersOfTeam()*/3; ++idx)//임시
	{
		if (TeamDoc->GetCharacterClass(true, idx) == ChacacterClassEnum)
		{
			TeamDoc->SetCharacterClass(true, idx, EPCClass::EPC_End);
			CP_CharacterCheck->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		else if (TeamDoc->GetCharacterClass(true, idx) == EPCClass::EPC_End)
		{
			if (CP_CharacterCheck->GetVisibility() == ESlateVisibility::Hidden)
			{
				TeamDoc->SetCharacterClass(true, idx, ChacacterClassEnum);
				CP_CharacterCheck->SetVisibility(ESlateVisibility::Visible);
				break;
			}
		}
	}

	if (FormationSlotUpdateDelegate.IsBound())
		FormationSlotUpdateDelegate.ExecuteIfBound();
}
