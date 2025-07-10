// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIDetailMyInfoPopup.h"
#include "B2UIManager.h"
#include "B2UIGuildMark.h"
#include "B2UIDocHelper.h"

#include "B2LobbyGameMode.h"
#include "B2UILobbyMain.h"

#include "BladeIIGameImpl.h"

UB2UIDetailMyInfoSlot::UB2UIDetailMyInfoSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentPCClass = EPCClass::EPC_End;
	bIsSelected = false;
	SlotDelegate = 0;
}

void UB2UIDetailMyInfoSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Select);
	
	GET_SLOT(UTextBlock, STB_BattleScore);
	GET_SLOT(UTextBlock, TB_Level);
	GET_SLOT(UTextBlock, TB_BattleScore);

	GET_SLOT(UWidgetSwitcher, MainSwitcher);

	GET_SLOT(UProgressBar, ExpProgressBar);
	GET_SLOT(UProgressBar, ExpProgressBar_Max);
}

void UB2UIDetailMyInfoSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIDetailMyInfoSlot::OnClickSelectButton);
}

void UB2UIDetailMyInfoSlot::Init()
{
	Super::Init();
}

void UB2UIDetailMyInfoSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIDetailMyInfoSlot::UpdateStaticText()
{
	if (STB_BattleScore.IsValid())
		STB_BattleScore->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
}

void UB2UIDetailMyInfoSlot::SetCharInfo(EPCClass HeroClass, int32 Lv, int32 CombatPower)
{
	CurrentPCClass = HeroClass;
	bIsSelected = false;

	if (MainSwitcher.IsValid())
	{
		MainSwitcher->SetActiveWidgetIndex(PCClassToInt(CurrentPCClass));

		UWidgetSwitcher* subSwitcher = Cast<UWidgetSwitcher>(MainSwitcher->GetWidgetAtIndex(PCClassToInt(CurrentPCClass)));
		if (subSwitcher)
		{
			subSwitcher->SetActiveWidgetIndex(bIsSelected);
		}
	}

	if (TB_Level.IsValid())
		TB_Level->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(Lv)));
	if (TB_BattleScore.IsValid())
		TB_BattleScore->SetText(FText::AsNumber(CombatPower));

	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	const int32 CharacterMaxLevel = ClientDataStore.GetLocalCharacterData().GetCharacterMaxLevel();
	const float ExpPercent = ClientDataStore.GetLocalCharacterData().GetCharacterExpPercent(HeroClass) * 0.01f;

	if (ExpProgressBar.IsValid() && ExpProgressBar_Max.IsValid())
	{
		// ∏∏∑¶¿Ã∏È ∂ÁøÏ¡‹.
		bool IsMax = CharacterMaxLevel <= Lv;
		ExpProgressBar->SetPercent(IsMax ? 0.0f : ExpPercent);
		ExpProgressBar->SetVisibility(IsMax ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		ExpProgressBar_Max->SetVisibility(IsMax ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UIDetailMyInfoSlot::OnClickSelectButton()
{
	SlotDelegate.ExecuteIfBound(CurrentPCClass);
}

void UB2UIDetailMyInfoSlot::SetSelectedState(bool selected)
{
	bIsSelected = selected;

	if (MainSwitcher.IsValid())
	{
		UWidgetSwitcher* subSwitcher = Cast<UWidgetSwitcher>(MainSwitcher->GetWidgetAtIndex(PCClassToInt(CurrentPCClass)));
		if (subSwitcher)
		{
			subSwitcher->SetActiveWidgetIndex(bIsSelected);
		}
	}
}

bool UB2UIDetailMyInfoSlot::GetIsSelected()
{
	return bIsSelected;
}

void UB2UIDetailMyInfoSlot::SetSlotDelegate(FCharacterSlot Indelegate)
{
	SlotDelegate = Indelegate;
}

///////////////////////////////////////////////////////////
UB2UIDetailMyInfoPopup::UB2UIDetailMyInfoPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectPCClass = EPCClass::EPC_End;
}

void UB2UIDetailMyInfoPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_DefaultMark);
	GET_SLOT(UTextBlock, TB_UserNickName);
	GET_SLOT(UTextBlock, TB_UserGuildName);

	GET_SLOT(UB2UIGuildMark, UIP_GuildMark);
	if(UIP_GuildMark.IsValid()) 
		UIP_GuildMark->Init();

	HeroArr.SetNumUninitialized(4);
	GET_SLOT_BYNAME(UB2UIDetailMyInfoSlot, HeroArr[PCClassToInt(EPCClass::EPC_Gladiator)], UIP_Gladiator);
	GET_SLOT_BYNAME(UB2UIDetailMyInfoSlot, HeroArr[PCClassToInt(EPCClass::EPC_Assassin)], UIP_Assassin);
	GET_SLOT_BYNAME(UB2UIDetailMyInfoSlot, HeroArr[PCClassToInt(EPCClass::EPC_Wizard)], UIP_Wizard);
	GET_SLOT_BYNAME(UB2UIDetailMyInfoSlot, HeroArr[PCClassToInt(EPCClass::EPC_Fighter)], UIP_Fighter);
	for (auto El : HeroArr)
	{
		El->Init();
	}

	GET_SLOT(UB2Button, BTN_ChangeChar);
	GET_SLOT(UTextBlock, STB_ChangeChar);
	GET_SLOT(UB2Button, BTN_Close);
}

void UB2UIDetailMyInfoPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_ChangeChar, &UB2UIDetailMyInfoPopup::OnClickBTN_ChangeChar);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIDetailMyInfoPopup::OnClickBTN_Close);
}

void UB2UIDetailMyInfoPopup::UpdateStaticText()
{
	if (STB_ChangeChar.IsValid())
		STB_ChangeChar->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DetailMyInfoPopup_Change")));
}

void UB2UIDetailMyInfoPopup::Init()
{
	Super::Init();

	SetUserGuild();
	SetUserNickName();
	SetCharInfos();

	EPCClass FirstSelectClass = EPCClass::EPC_Gladiator;

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{ 
		FirstSelectClass = IntToPCClass(DocBattle->GetCurPCClass());
	}

	//SlotDelegateFunction(FirstSelectClass);

	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		if (HeroArr[ClassCnt].IsValid())
		{
			HeroArr[ClassCnt]->SetSelectedState((PCClassToInt(FirstSelectClass) == ClassCnt));
		}
	}
}

void UB2UIDetailMyInfoPopup::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIDetailMyInfoPopup::OnClickBTN_ChangeChar()
{
	//LobbyCharObvForcedChangeClass<EPCClass>::GetInstance().Signal(SelectPCClass);
	
	UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	if (LobbyMainUIPage)
	{
		LobbyMainUIPage->ForcedChangeFocusCharacter();
	}

	this->CloseSelf();
}

void UB2UIDetailMyInfoPopup::OnClickBTN_Close()
{
	// πÈπˆ∆∞ √ﬂ∞° √≥∏Æ « ø‰
	this->CloseSelf();
}

void UB2UIDetailMyInfoPopup::SetCharInfos()
{
	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		auto DocHero = UB2UIDocHelper::GetDocHero(ClassCnt);
		const EPCClass HeroClassEnum = IntToPCClass(ClassCnt);
		if (DocHero)
		{
			const int32 HeroLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(HeroClassEnum);

			if (HeroArr[ClassCnt].IsValid())
			{
				HeroArr[ClassCnt]->SetCharInfo(HeroClassEnum, HeroLevel, DocHero->GetCombatPower());
				//HeroArr[ClassCnt]->SetSlotDelegate(FCharacterSlot::CreateLambda([this](EPCClass InValue) {SlotDelegateFunction(InValue); }));
			}
		}
	}
}

void UB2UIDetailMyInfoPopup::SetUserNickName()
{
	if (TB_UserNickName.IsValid())
		TB_UserNickName->SetText(FText::FromString(BladeIIGameImpl::GetClientDataStore().GetUserNickName()));
}

void UB2UIDetailMyInfoPopup::SetUserGuild()
{
	const FUserGuildInfo& GuildInfo = BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo();

	if (BladeIIGameImpl::GetClientDataStore().GetUserGuildID())
	{
		if (UIP_GuildMark.IsValid()) {
			UIP_GuildMark->SetGuildMark(GuildInfo.Mark_Index, GuildInfo.MarkColor_Index, GuildInfo.MarkBG_Index, GuildInfo.MarkBGColor_Index);
			UIP_GuildMark->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		if (TB_UserGuildName.IsValid()) {
			TB_UserGuildName->SetText(FText::FromString(GuildInfo.GuildName));
			TB_UserGuildName->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (IMG_DefaultMark.IsValid()) {
			IMG_DefaultMark->SetVisibility(ESlateVisibility::Hidden); // GuildMark ∂˚ ∞∞¿∫ ¿ßƒ°.
		}
	}
	else
	{
		if (IMG_DefaultMark.IsValid()) {
			IMG_DefaultMark->SetVisibility(ESlateVisibility::HitTestInvisible); // GuildMark ∂˚ ∞∞¿∫ ¿ßƒ°.
		}
		if (UIP_GuildMark.IsValid()) {
			UIP_GuildMark->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TB_UserGuildName.IsValid()) {
			TB_UserGuildName->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIDetailMyInfoPopup::SlotDelegateFunction(EPCClass InValue)
{
	int32 EPCValue = PCClassToInt(InValue);
	SelectPCClass = InValue;

	for (int32 ClassCnt = 0; ClassCnt < GetMaxPCClassNum(); ++ClassCnt)
	{
		if (HeroArr[ClassCnt].IsValid())
		{
			HeroArr[ClassCnt]->SetSelectedState((EPCValue == ClassCnt));
		}
	}
}


