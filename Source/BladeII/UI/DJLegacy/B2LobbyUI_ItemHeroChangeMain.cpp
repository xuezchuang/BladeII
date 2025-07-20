
#include "B2LobbyUI_ItemHeroChangeMain.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2DynItemIcon.h"
#include "B2UIStoreProductCost.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"

void UB2LobbyUI_ItemHeroChangeMain::Init()
{
	Super::Init();
	
	AB2LobbyGameMode* LobbyGameModed = Cast<AB2LobbyGameMode>(GetWorld()->GetAuthGameMode());

	if (LobbyGameModed)
	{
		CachedInventory = LobbyGameModed->GetLobbyInventory();
		check(CachedInventory);

		if (!CachedInventory->GetItemOPTargetItemData(TargetItem) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_HeroChange)
		{
			CloseAndQuitItemOpMode(); // 父俊 窍唱 角菩茄促搁.. 绢驴 荐 绝瘤.
			return;
		}

	}

	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
	{
		SetCharacterSlotState(static_cast<EPCClass>(i), false);
	}

	ChoicePCClass = EPCClass::EPC_End;

}

#define GET_SLOT_CHARSETTING(InSlot, Name)														\
	GET_SLOT_BYNAME(UOverlay,InSlot.O_CharacterOverlay, O_##Name)								\
	GET_SLOT_BYNAME(UB2Button,InSlot.BTN_Character, BTN_##Name)									\
	GET_SLOT_BYNAME(UOverlay,InSlot.O_CharacterOnOverlay, O_##Name##On)							\
	GET_SLOT_BYNAME(UTextBlock,InSlot.TB_CharacterName, TB_##Name)								\
	GET_SLOT_BYNAME(UTextBlock,InSlot.TB_CharacterName2, TB_##Name##2)							\
	GET_SLOT_BYNAME(UOverlay,InSlot.O_CharacterInActive, O_##Name##InActive)					\
	BIND_CLICK_FUNC_TO_BTN(InSlot.BTN_Character, &UB2LobbyUI_ItemHeroChangeMain::OnClick##Name)	\

void UB2LobbyUI_ItemHeroChangeMain::CacheAssets()
{
	

	GET_SLOT(UTextBlock, TB_EnhanceChangeCost);
	GET_SLOT(UTextBlock, TB_EnhanceChangeTitle);
	GET_SLOT(UTextBlock, TB_EnhanceChangeBtn);

	GET_SLOT(UCanvasPanel, CP_TargetItemIconSet);
	GET_SLOT(UCanvasPanel, CP_IngredItemIconSet);

	GET_SLOT(UB2Button, BTN_EnhanceChange);
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceChange, &UB2LobbyUI_ItemHeroChangeMain::OnClickChangeItem)
	GET_SLOT(UB2Button, BTN_BackgroundBlock);
	BIND_CLICK_FUNC_TO_BTN(BTN_BackgroundBlock, &UB2LobbyUI_ItemHeroChangeMain::OnClickClose)
	GET_SLOT(UB2Button, BTN_Close)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemHeroChangeMain::OnClickClose)
	GET_SLOT(UB2Button, BTN_SkipResult)
	BIND_CLICK_FUNC_TO_BTN(BTN_SkipResult, &UB2LobbyUI_ItemHeroChangeMain::OnClickClose)

	GET_SLOT(UB2DynItemIcon_LobbyInven, TargetItemIcon)
	GET_SLOT(UB2DynItemIcon_LobbyInven, IngredItemIcon)

	GET_SLOT(UTextBlock, TB_HeroChangeTitle)
	GET_SLOT(UTextBlock, TB_HeroChangeBtn)
	GET_SLOT(UTextBlock, TB_Confirm)
	GET_SLOT(UTextBlock, TB_HeroChageGuide)

	CharacterSlot.Empty();
	CharacterSlot.SetNum(static_cast<int32>(EPCClass::EPC_End));

	GET_SLOT_CHARSETTING(CharacterSlot[static_cast<int32>(EPCClass::EPC_Gladiator)], Gladiator);
	GET_SLOT_CHARSETTING(CharacterSlot[static_cast<int32>(EPCClass::EPC_Assassin)], Assasin);
	GET_SLOT_CHARSETTING(CharacterSlot[static_cast<int32>(EPCClass::EPC_Wizard)], Wizard);
	GET_SLOT_CHARSETTING(CharacterSlot[static_cast<int32>(EPCClass::EPC_Fighter)], Fighter);

	ChangItemResultAni = GetAnimation(this, "ChangeItemResultAni");


}

void UB2LobbyUI_ItemHeroChangeMain::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2LobbyUI_ItemHeroChangeMain::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	SubscribeEvents();
}

void UB2LobbyUI_ItemHeroChangeMain::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);


	UnSubscribeEvents();
}

void UB2LobbyUI_ItemHeroChangeMain::InitBase()
{
	if (IngredItemIcon.IsValid())
		IngredItemIcon->SetVisibility(ESlateVisibility::Collapsed);

	if (BTN_SkipResult.IsValid())
	{
		BTN_SkipResult->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (BTN_EnhanceChange.IsValid())
	{
		BTN_EnhanceChange->SetIsEnabled(false);
	}

	if (TB_EnhanceChangeCost.IsValid())
		TB_EnhanceChangeCost->SetText(FText::FromString(TEXT("-")));
}

void UB2LobbyUI_ItemHeroChangeMain::SubscribeEvents()
{
	ChangeItemCharacterTypeTicket = DeliveryChangeItemCharacterTypeClass<FB2ResponseChangeItemCharacterTypePtr>::GetInstance().Subscribe(
		[this](const FB2ResponseChangeItemCharacterTypePtr& Info)
	{
		this->ServerResultItemHeroChangeMain(Info);
	}
	);
}

void UB2LobbyUI_ItemHeroChangeMain::UnSubscribeEvents()
{
	DeliveryChangeItemCharacterTypeClass<FB2ResponseChangeItemCharacterTypePtr>::GetInstance().Unsubscribe(ChangeItemCharacterTypeTicket);
}

void UB2LobbyUI_ItemHeroChangeMain::UpdateStaticText()
{
	if (TB_HeroChangeBtn.IsValid())
		TB_HeroChangeBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ExchangeHero_Title")));

	if(TB_HeroChangeTitle.IsValid())
		TB_HeroChangeTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ExchangeHero_Title")));

	if(TB_Confirm.IsValid())
		TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));

	if (CharacterSlot[static_cast<int32>(EPCClass::EPC_Gladiator)].TB_CharacterName.IsValid() &&
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Gladiator)].TB_CharacterName2.IsValid())
	{
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Gladiator)].TB_CharacterName->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Gladiator")));
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Gladiator)].TB_CharacterName2->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Gladiator")));
	}

	if (CharacterSlot[static_cast<int32>(EPCClass::EPC_Assassin)].TB_CharacterName.IsValid() &&
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Assassin)].TB_CharacterName2.IsValid())
	{
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Assassin)].TB_CharacterName->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Assassin")));
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Assassin)].TB_CharacterName2->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Assassin")));
	}

	if (CharacterSlot[static_cast<int32>(EPCClass::EPC_Wizard)].TB_CharacterName.IsValid() &&
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Wizard)].TB_CharacterName2.IsValid())
	{
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Wizard)].TB_CharacterName->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Wizard")));
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Wizard)].TB_CharacterName2->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Wizard")));
	}

	if (CharacterSlot[static_cast<int32>(EPCClass::EPC_Fighter)].TB_CharacterName.IsValid() &&
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Fighter)].TB_CharacterName2.IsValid())
	{
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Fighter)].TB_CharacterName->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Fighter")));
		CharacterSlot[static_cast<int32>(EPCClass::EPC_Fighter)].TB_CharacterName2->SetText(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Fighter")));
	}

	if (TB_HeroChageGuide.IsValid())
		TB_HeroChageGuide->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_SelectCharacter_FullText")));
}

void UB2LobbyUI_ItemHeroChangeMain::CloseWidgetDelegate()
{
	CloseAndQuitItemOpMode();
}

void UB2LobbyUI_ItemHeroChangeMain::OnClickClose()
{
	CloseAndQuitItemOpMode();
}


void UB2LobbyUI_ItemHeroChangeMain::OnClickGladiator()
{
	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
	{
		if (static_cast<EPCClass>(i) == EPCClass::EPC_Gladiator)
			CharacterSlot[i].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Visible);
		else
			CharacterSlot[i].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	IngredItemUpdate(EPCClass::EPC_Gladiator);

}

void UB2LobbyUI_ItemHeroChangeMain::OnClickAssasin()
{
	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
	{
		if(static_cast<EPCClass>(i) == EPCClass::EPC_Assassin)
			CharacterSlot[i].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Visible);
		else
			CharacterSlot[i].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}

	IngredItemUpdate(EPCClass::EPC_Assassin);
}

void UB2LobbyUI_ItemHeroChangeMain::OnClickWizard()
{
	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
	{
		if (static_cast<EPCClass>(i) == EPCClass::EPC_Wizard)
			CharacterSlot[i].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Visible);
		else
			CharacterSlot[i].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}

	IngredItemUpdate(EPCClass::EPC_Wizard);
}

void UB2LobbyUI_ItemHeroChangeMain::OnClickFighter()
{
	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
	{
		if (static_cast<EPCClass>(i) == EPCClass::EPC_Fighter)
			CharacterSlot[i].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Visible);
		else
			CharacterSlot[i].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}

	IngredItemUpdate(EPCClass::EPC_Fighter);
}

void UB2LobbyUI_ItemHeroChangeMain::OnClickChangeItem()
{
	data_trader::Retailer::GetInstance().RequestChangeItemCharacterType(TargetItem.InstanceUID, CliToSvrPCClassType(ChoicePCClass));
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TimeHandler);
		TheWorld->GetTimerManager().SetTimer(TimeHandler, this, &UB2LobbyUI_ItemHeroChangeMain::ChangeButtonActive ,1.0f, true);

	}
}

void UB2LobbyUI_ItemHeroChangeMain::ResultFunction()
{
	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
	{
		SetInActive(static_cast<EPCClass>(i));
	}

	if(BTN_EnhanceChange.IsValid())
		BTN_EnhanceChange->SetVisibility(ESlateVisibility::Hidden);

	if (BTN_SkipResult.IsValid())
		BTN_SkipResult->SetVisibility(ESlateVisibility::Visible);

	if (TargetItemIcon.IsValid())
		TargetItemIcon->SetIsEnabled(false);
}

void UB2LobbyUI_ItemHeroChangeMain::DestroyTargetItemIcon()
{

}

void UB2LobbyUI_ItemHeroChangeMain::CloseAndQuitItemOpMode()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ItemHeroChangeMain);
}

void UB2LobbyUI_ItemHeroChangeMain::UpdateItemIcons()
{
	DestroyTargetItemIcon();

	if (!CP_TargetItemIconSet.IsValid() || TargetItem.InstanceUID <= 0 || TargetItem.ItemRefID <= 0) {
		return;
	}

	if (TargetItemIcon.IsValid())
	{
		TargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget);
		TargetItemIcon->UpdateItemData(TargetItem);


	}
}

void UB2LobbyUI_ItemHeroChangeMain::SetCharacterSlotState(EPCClass InClass, bool InState)
{
	if (CharacterSlot.Num() > static_cast<int32>(InClass))
	{

		CharacterSlot[static_cast<int32>(InClass)].O_CharacterInActive->SetVisibility(ESlateVisibility::Collapsed);
		if (InState)
		{
			CharacterSlot[static_cast<int32>(InClass)].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Visible);
		}
		else
			CharacterSlot[static_cast<int32>(InClass)].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Collapsed);

	}
}

void UB2LobbyUI_ItemHeroChangeMain::SetInActive(EPCClass InClass)
{
	if (CharacterSlot.Num() > static_cast<int32>(InClass))
	{
		CharacterSlot[static_cast<int32>(InClass)].O_CharacterOnOverlay->SetVisibility(ESlateVisibility::Collapsed);
		CharacterSlot[static_cast<int32>(InClass)].O_CharacterInActive->SetVisibility(ESlateVisibility::Visible);
		CharacterSlot[static_cast<int32>(InClass)].BTN_Character->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UB2LobbyUI_ItemHeroChangeMain::SetCostInfo(int32 InDarkCrystalCost, TArray<FB2Item> InChangeItem)
{
	DarkCrystalCost = InDarkCrystalCost;

	ChangeItem = InChangeItem;

}

void UB2LobbyUI_ItemHeroChangeMain::SetCostText(int32 InCost)
{
	if (TB_EnhanceChangeCost.IsValid())
	{
		TB_EnhanceChangeCost->SetText(FText::AsNumber(InCost));
		auto UserInfo = UB2UIDocHelper::GetDocUser();
		int32 UserCost = UserInfo->GetDarkCrystal();

		TB_EnhanceChangeCost->SetColorAndOpacity(InCost <= UserCost ? FLinearColor::White : FLinearColor::Red);
	}

	if (BTN_EnhanceChange.IsValid())
		BTN_EnhanceChange->SetIsEnabled(true);
}

void UB2LobbyUI_ItemHeroChangeMain::IngredItemUpdate(EPCClass InPCClass)
{
	if (IngredItemIcon.IsValid())
	{
		for (int32 i = 0; i < ChangeItem.Num(); ++i)
		{
			if (ChangeItem[i].AllowedPCClass == static_cast<int32>(InPCClass))
			{
				SourceItem = ChangeItem[i];
				SourceItem.IsNew = false;
				IngredItemIcon->UpdateItemData(SourceItem);
				IngredItemIcon->SetVisibility(ESlateVisibility::Visible);
				SetCostText(DarkCrystalCost);

				ChoicePCClass = InPCClass;
			}
		}
	}
}

void UB2LobbyUI_ItemHeroChangeMain::ServerResultItemHeroChangeMain(const FB2ResponseChangeItemCharacterTypePtr& InSeverPtr)
{
	//ResultStartAnimation();

	if (ChangItemResultAni.IsValid())
	{
		PlayAnimation(ChangItemResultAni.Get());
		if (Sfx_Sound)
			PlaySound(Sfx_Sound);
	}
	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();

	//酒捞袍 持绢林扁
	ResultItem = InSeverPtr->updated_item;
	BladeIIGameImpl::GetClientDataStore().AddChangeItem(ResultItem);

	BladeIIGameImpl::GetClientDataStore().SetDarkCrystal(InSeverPtr->current_dark_crystal);

	if (DocUser)
		DocUser->SetDarkCrystal(InSeverPtr->current_dark_crystal);

	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_StoredItemDetail);
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);

	//FB2Item AllItem = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().FindItem(ResultItem.InstanceUID);
}

void UB2LobbyUI_ItemHeroChangeMain::ChangeButtonActive()
{
	if (BTN_EnhanceChange.IsValid())
		BTN_EnhanceChange->SetIsEnabled(true);
}

