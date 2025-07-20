#include "B2UICharacterSelectItem.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2UICharacterSelectItem::UB2UICharacterSelectItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectModeType = EUIHeroSelectModeType::None;
	SelectType = EUIHeroSelectType::None;
}

void UB2UICharacterSelectItem::CacheAssets()
{
	//GET_SLOT(UButton, BTN_SelectStartMain)
	//GET_SLOT(UButton, BTN_SelectStartSub)
	GET_SLOT(UButton, BTN_Area)
	GET_SLOT(UImage, IMG_Hero)
	GET_SLOT(UTextBlock, TB_HeroName)
	GET_SLOT(UTextBlock, TB_HeroLevel)
	GET_SLOT(UTextBlock, TB_HeroLevel2)
	GET_SLOT(UTextBlock, TB_CombatPower)
}

void UB2UICharacterSelectItem::UpdateStaticText()
{
}

void UB2UICharacterSelectItem::BindDelegates()
{
	//BIND_CLICK_FUNC_TO_BTN(BTN_SelectStartMain, &UB2UICharacterSelectItem::OnClickBtnArea)
	//BIND_CLICK_FUNC_TO_BTN(BTN_SelectStartSub, &UB2UICharacterSelectItem::OnClickBtnArea)
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UICharacterSelectItem::OnClickBtnArea)
}

void UB2UICharacterSelectItem::BindDoc(class UB2UIDocHero* DocHero)
{
	UnbindDoc();
	SetDoc(DocHero);

	if (DocHero)
	{
		SetHeroImage(DocHero->GetHeroClass());
		SetHeroName(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(DocHero->GetHeroClass()));
		//SetHeroLevel(DocHero->GetCurrentLevel());
		SetHeroCombatPower(DocHero->GetCombatPower());

		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
		SetHeroLevel(BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetCharacterLevel(DocHero->GetHeroClass()));
		//DocHero->OnCurrentLevelChanged.AddUObject(this, &UB2UICharacterSelectItem::OnChangedHeroLevel);
	}
}

void UB2UICharacterSelectItem::UnbindDoc()
{
	//UB2UIDocHero* BoundDocHero = Cast<UB2UIDocHero>(GetDoc());
	//if (BoundDocHero)
	//{
	//	//BoundDocHero->OnCurrentLevelChanged.RemoveAll(this);
	//}

	SetDoc(nullptr);
}

void UB2UICharacterSelectItem::SetHeroImage(EPCClass CharType)
{
	CharacterClass = CharType;
	SetHeroImage_BP(CharType);
}

void UB2UICharacterSelectItem::SetHeroName(const FText& InText)
{
	if (TB_HeroName.IsValid())
		TB_HeroName->SetText(InText);
}

void UB2UICharacterSelectItem::SetHeroLevel(int32 InValue)
{
	if (TB_HeroLevel.IsValid())
		TB_HeroLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Hero_Level"))), FText::AsNumber(InValue)));

	if (TB_HeroLevel2.IsValid())
		TB_HeroLevel2->SetText(FText::AsNumber(InValue));
}

void UB2UICharacterSelectItem::SetHeroCombatPower(int32 InValue)
{
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(InValue));
}

void UB2UICharacterSelectItem::SetSelectMode(EUIHeroSelectModeType InSelectMode)
{
	SelectModeType = InSelectMode;

	SetSelectMode_BP(InSelectMode);
}

void UB2UICharacterSelectItem::SetSelectedType(EUIHeroSelectType InSelectType)
{
	SelectType = InSelectType;

	SetSelectedType_BP(InSelectType);

	
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UICharacterSelectItem::OnClickBtnArea()
{
	if (SelectModeType == EUIHeroSelectModeType::None)
		return;

	OnCharacterSelectItemClickedDelgate.ExecuteIfBound(this);
}

//====================================================================================
//									Doc Delegate
//====================================================================================

void UB2UICharacterSelectItem::OnChangedHeroLevel(class UB2UIDocBase* Sender, int32 CurLevel, int32 PrevLevel)
{
	if (CurLevel != PrevLevel)
	{
		SetHeroLevel(CurLevel);
	}
}