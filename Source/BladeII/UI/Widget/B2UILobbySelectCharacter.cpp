// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UILobbySelectCharacter.h"
#include "Event.h"
#include "B2LobbySceneManager.h"

#include "BladeIIGameImpl.h"
////////////////////////////////////////////////////////////
/// AlotWidget
///////////////////////////////////////////////////////////
void UB2UILobbySelectCharacterSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UWidgetSwitcher, WS_ClassImage);

	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, TB_Level);

	GET_SLOT(UProgressBar, PB_Exp);
	GET_SLOT(UProgressBar, PB_ExpMax);
}

void UB2UILobbySelectCharacterSlot::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UILobbySelectCharacterSlot::OnClickBTN_Select);
}

void UB2UILobbySelectCharacterSlot::Init()
{
	Super::Init();
}

void UB2UILobbySelectCharacterSlot::SetInfo(EPCClass HeroClass, int32 Lv, int32 CombatPower, int32 MaxLevel, float ExpPercent)
{
	CurrentPCClass = HeroClass;
	bIsSelected = false;

	if (WS_ClassImage.IsValid())
	{
		WS_ClassImage->SetActiveWidgetIndex(PCClassToInt(CurrentPCClass));
	}

	if (TB_Level.IsValid())
		TB_Level->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(Lv)));
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(CombatPower));

	if (PB_Exp.IsValid() && PB_ExpMax.IsValid())
	{
		// ∏∏∑¶¿Ã∏È ∂ÁøÏ¡‹.
		bool IsMax = MaxLevel <= Lv;
		PB_Exp->SetPercent(IsMax ? 0.0f : ExpPercent);
		PB_Exp->SetVisibility(IsMax ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		PB_ExpMax->SetVisibility(IsMax ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UILobbySelectCharacterSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UILobbySelectCharacterSlot::OnClickBTN_Select()
{
	SlotDelegate.ExecuteIfBound(CurrentPCClass);
}

void UB2UILobbySelectCharacterSlot::SetSelectedState(bool selected)
{
	bIsSelected = selected;
	
	if (WS_ClassImage.IsValid())
	{
		UImage* CharImage = Cast<UImage>(WS_ClassImage->GetWidgetAtIndex(static_cast<int32>(CurrentPCClass)));
		if (CharImage)
		{
			FLinearColor Color = bIsSelected ? FLinearColor(1.0f, 1.0f, 1.0f, 1.0f) : FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);
			CharImage->SetColorAndOpacity(Color);
		}
	}
}

bool UB2UILobbySelectCharacterSlot::GetIsSelected()
{
	return bIsSelected;
}

void UB2UILobbySelectCharacterSlot::UpdateStaticText()
{
	if (STB_CombatPower.IsValid())
		STB_CombatPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
}

void UB2UILobbySelectCharacterSlot::SetSlotDelegate(FCharacterSlot Indelegate)
{
	SlotDelegate = Indelegate;
}

////////////////////////////////////////////////////////////
/// PageWidget
///////////////////////////////////////////////////////////
void UB2UILobbySelectCharacter::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button,  BTN_Back);
	GET_SLOT(UTextBlock, TB_SelectCharacter);

	CharSlots.SetNum(4);
	GET_SLOT_BYNAME(UB2UILobbySelectCharacterSlot, CharSlots[0], UIP_SelectSlot0);
	GET_SLOT_BYNAME(UB2UILobbySelectCharacterSlot, CharSlots[1], UIP_SelectSlot1);
	GET_SLOT_BYNAME(UB2UILobbySelectCharacterSlot, CharSlots[2], UIP_SelectSlot2);
	GET_SLOT_BYNAME(UB2UILobbySelectCharacterSlot, CharSlots[3], UIP_SelectSlot3);
}

void UB2UILobbySelectCharacter::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Back, &UB2UILobbySelectCharacter::OnClickBTN_Back);
}

void UB2UILobbySelectCharacter::Init()
{
	Super::Init();

	SetCharSlotInfos();
}

void UB2UILobbySelectCharacter::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UILobbySelectCharacter::OnClickBTN_Back()
{
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_CharacterObserve);
}

void UB2UILobbySelectCharacter::UpdateStaticText()
{
	if (TB_SelectCharacter.IsValid())
		TB_SelectCharacter->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_SelectHero")));
}

void UB2UILobbySelectCharacter::CloseWidgetDelegate()
{
	OnClickBTN_Back();
}

void UB2UILobbySelectCharacter::SetCharSlotInfos()
{
	TArray<EPCClass> PcClassArray = {EPCClass::EPC_Fighter,EPCClass::EPC_Assassin,EPCClass::EPC_Gladiator,EPCClass::EPC_Wizard};

	for (int32 i = 0; i < 4; ++i)
	{
		EPCClass PCClass = PcClassArray[i];
		int32 PCClassNum = static_cast<int32>(PCClass);

		if (auto DocHero = UB2UIDocHelper::GetDocHero(PCClassNum))
		{
			const int32 HeroLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(PCClass);
			const int32 CharacterMaxLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterMaxLevel();
			const float ExpPercent = BladeIIGameImpl::GetLocalCharacterData().GetCharacterExpPercent(PCClass) * 0.01f;

			if (CharSlots[i].IsValid())
			{
				CharSlots[i]->Init();
				CharSlots[i]->SetInfo(PCClass, HeroLevel, DocHero->GetCombatPower(), CharacterMaxLevel, ExpPercent);
				CharSlots[i]->SetSlotDelegate(FCharacterSlot::CreateLambda([this](EPCClass InValue) {SlotDelegateFunction(InValue); }));
			}
		}
	}
}

void UB2UILobbySelectCharacter::SlotDelegateFunction(EPCClass InValue)
{
	int32 EPCValue = PCClassToInt(InValue);
	SelectPCClass = InValue;

	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		if (CharSlots[ClassCnt].IsValid())
		{
			CharSlots[ClassCnt]->SetSelectedState((EPCValue == ClassCnt));
		}
	}

	LobbyCharObvForcedChangeClass<EPCClass>::GetInstance().Signal(SelectPCClass);

	CloseWidgetDelegate();
}
