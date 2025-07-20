// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_HeroMgmtCommon.h"
//#include "BladeII.h"

#include "B2LobbyUISwitcher.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2UIManager_Lobby.h"
#include "Event.h"
#include "B2UIDocHelper.h"
#include "B2UIHeader.h"
#include "B2UIWidgetBase.h"
#include "B2UIPHeroMgmtPreset.h"
#include "B2Airport.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyUI_InventoryMain.h"
#include "B2LobbyUI_TotemInventoryMain.h"

TArray<UImage*> TSelectImg;

UB2LobbyUI_HeroMgmtCommon::UB2LobbyUI_HeroMgmtCommon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentHeroMgmtSubMode = EHeroMgmtSubMode::EHMSM_Inventory;
}

void UB2LobbyUI_HeroMgmtCommon::SelectPcClass(EPCClass pc_class)
{
	SelectedHeroMgmtPCClass = pc_class;

	// 肺厚 GM 率栏肺 烹瘤窍搁 芭扁辑档 促弗 UI (牢亥配府 扼带瘤..) 率栏肺 函版烹瘤 且 巴.
	LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Signal(SelectedHeroMgmtPCClass);

	OnHeroPCChanged(SelectedHeroMgmtPCClass);
}

void UB2LobbyUI_HeroMgmtCommon::OnHeroPCChanged(EPCClass NewClass)
{
	// 趣矫扼档 酒捞袍 惑技 芒捞 唱客乐绰 惑怕扼搁 摧绰促.
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_StoredItemDetail);
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_CharStatDetail);

	// 傍/规/眉 傈捧仿 荐摹狼 UI 馆康牢单 趣矫 初摹绰 何盒捞 乐阑鳖 酵绢辑 康旷包府 芒 某腐磐 官拆 锭 茄锅 秦 林绰 芭.
	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(SelectedHeroMgmtPCClass, false);

	SetCharacterTextColor(SelectedHeroMgmtPCClass);

	EventChangedHeroMgmtPCClass(SelectedHeroMgmtPCClass);// 唱赣瘤 BP 率 贸府.

	if (UIP_ShowWingSet.IsValid()) {
		UIP_ShowWingSet->UpdateShowWingBtnImage();
	}

	if (CB_ShowHead.IsValid())
		CB_ShowHead->SetIsChecked(BladeIIGameImpl::GetLocalCharacterData().IsShowHeadItem(NewClass));
}

void UB2LobbyUI_HeroMgmtCommon::CyclePCSelection(bool bForward)
{
	// 沥秦柳 鉴辑啊 乐澜. 八捧荤 - 绢截脚 - 付过荤 - 拜捧啊.

	if (bForward)
	{
		if (SelectedHeroMgmtPCClass != EPCClass::EPC_End)
		{
			SelectedHeroMgmtPCClass = IntToPCClass((int32)SelectedHeroMgmtPCClass + 1);
		}
		if (SelectedHeroMgmtPCClass == EPCClass::EPC_End)
		{
			SelectedHeroMgmtPCClass = EPCClass::EPC_Gladiator;
		}
	}
	else
	{
		if (SelectedHeroMgmtPCClass != EPCClass::EPC_Gladiator)
		{
			SelectedHeroMgmtPCClass = IntToPCClass((int32)SelectedHeroMgmtPCClass - 1);
		}
		else
		{
			SelectedHeroMgmtPCClass = EPCClass::EPC_Fighter;
		}
	}

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
	
	// 肺厚 GM 率栏肺 烹瘤窍搁 芭扁辑档 促弗 UI (牢亥配府 扼带瘤..) 率栏肺 函版烹瘤 且 巴.
	LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Signal(SelectedHeroMgmtPCClass);

	// 趣矫扼档 酒捞袍 惑技 芒捞 唱客乐绰 惑怕扼搁 摧绰促.
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_StoredItemDetail);
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);
}

void UB2LobbyUI_HeroMgmtCommon::SubscribeEvents()
{
	if (bEventSubscribed)
		return;

	InternalSubscribeEvents();

	bEventSubscribed = true;
}

void UB2LobbyUI_HeroMgmtCommon::UnsubscribeEvents()
{
	if (!bEventSubscribed)
		return;

	InternalUnsubscribeEvents();
	
	bEventSubscribed = false;
}

void UB2LobbyUI_HeroMgmtCommon::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	SubscribeEvents();

	SelectedHeroMgmtPCClass = CachedLobbyGM->GetHeroMgmtModePCClass(); // 贸澜俊 茄锅篮 鞘夸窍焙 ぱ.ぱ 搬惫 促弗 侥栏肺 蔼阑 掘绢坷霸 登摆瘤父 
	if (SelectedHeroMgmtPCClass == EPCClass::EPC_End)
		SelectedHeroMgmtPCClass = EPCClass::EPC_Gladiator;

	const int32 MaxPresetSlot = static_cast<uint8>(EItemPresetSlotType::EIPS_End);
	for (int i = 0; i < MaxPresetSlot; ++i)
	{
		if (EquipPresets[i].IsValid())
		{
			EquipPresets[i]->SetSlotNumber(SelectedHeroMgmtPCClass, (EItemPresetSlotType)i);
		}
		if (SkillPresets[i].IsValid())
		{
			SkillPresets[i]->SetSlotNumber(SelectedHeroMgmtPCClass, (EItemPresetSlotType)i);
		}
	}
	if (UIP_ShowWingSet.IsValid())
	{
		UIP_ShowWingSet->StartFromLobby(InUIManager, InGM); // 捞扒 LobbyUISwitcher 俊辑 葛福聪 流立 秦 林绢具..
	}

	UpdateStaticTexts();
	UpdateDynamicTexts();
	ShowEnhanceItemSetButton();

	SelectPcClass(SelectedHeroMgmtPCClass);

	CurrentHeroMgmtSubMode = CachedLobbyGM->GetHeroMgmtSubMode();

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	if (UIP_CommonHeader.IsValid())
	{
		UIP_CommonHeader->SetIsInDJLegacyHeroMgmt(true, CurrentHeroMgmtSubMode);
		B2AndroidBackManager::GetInstance()->AddBackWidget(UIP_CommonHeader.Get(), FName(TEXT("Header")));
	}
	
	
}

void UB2LobbyUI_HeroMgmtCommon::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIHeader, UIP_CommonHeader)
	if (UIP_CommonHeader.IsValid())
	{		
		UIP_CommonHeader->Init();
		UIP_CommonHeader->SetIsInDJLegacyHeroMgmt(true, EHeroMgmtSubMode::EHMSM_Inventory); // 咯扁急 措钎拜 Inventory 肺 持绢淋
		UIP_CommonHeader->BindDocAuto();
	}

	GET_SLOT(UB2Button, BTN_EnhanceItemSet);
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceItemSet, &UB2LobbyUI_HeroMgmtCommon::OnEnhanceItemSetButtonPressed);

	GET_SLOT(UB2LobbyUI_HeroMgmtShowWingSet, UIP_ShowWingSet);

	GET_SLOT(UCheckBox, CB_ShowHead);
	if (CB_ShowHead.IsValid())
		CB_ShowHead->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_HeroMgmtCommon::OnShowHeadButtonPressed);
	
	GET_SLOT(UTextBlock, TB_GoldAmount)
	GET_SLOT(UTextBlock, TB_GemAmount)

	GET_SLOT(UTextBlock, TB_CharLevel)
	GET_SLOT(UProgressBar, PB_CharExpProgress)
	GET_SLOT(UTextBlock, TB_CharExpText)

	GET_SLOT(UTextBlock, TB_HealthLabel)
	GET_SLOT(UTextBlock, TB_HealthPoint)

	GET_SLOT(UTextBlock, TB_AttackLabel)
	GET_SLOT(UTextBlock, TB_AttackPoint)

	GET_SLOT(UTextBlock, TB_DefenseLabel)
	GET_SLOT(UTextBlock, TB_DefensePoint)

	GET_SLOT(UTextBlock, TB_CombatLable)
	GET_SLOT(UTextBlock, TB_CombatPower)

	GET_SLOT(UImage, IMG_Select_Gladiator)
	GET_SLOT(UImage, IMG_Select_Assassin)
	GET_SLOT(UImage, IMG_Select_Wizard)
	GET_SLOT(UImage, IMG_Select_Fighter)
	GET_SLOT(UImage, IMG_LevelMax)

	GET_SLOT(UPanelWidget, P_HeroGladiator)
	GET_SLOT(UPanelWidget, P_HeroAssassin)
	GET_SLOT(UPanelWidget, P_HeroWizard)
	GET_SLOT(UPanelWidget, P_HeroFighter)

	GET_SLOT(UB2Button, BTN_SelectGladiator)
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectGladiator, &UB2LobbyUI_HeroMgmtCommon::OnGladiatorSelectButtonPressed)
	GET_SLOT(UB2Button, BTN_SelectAssassin)
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectAssassin, &UB2LobbyUI_HeroMgmtCommon::OnAssassinSelectButtonPressed)
	GET_SLOT(UB2Button, BTN_SelectWizard)
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectWizard, &UB2LobbyUI_HeroMgmtCommon::OnWizardSelectButtonPressed)
	GET_SLOT(UB2Button, BTN_SelectFighter)
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectFighter, &UB2LobbyUI_HeroMgmtCommon::OnFighterSelectButtonPressed)
	
	GET_SLOT(UTextBlock, TB_GladiatorFull)
	if (TB_GladiatorFull.IsValid())
	{
		TB_GladiatorFull->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UTextBlock, TB_AssassinFull)
	if (TB_AssassinFull.IsValid())
	{
		TB_AssassinFull->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UTextBlock, TB_WizardFull)
	if (TB_WizardFull.IsValid())
	{
		TB_WizardFull->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UTextBlock, TB_FighterFull)
	if (TB_FighterFull.IsValid())
	{
		TB_FighterFull->SetVisibility(ESlateVisibility::Collapsed);
	}

	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Gladiator)
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Assassin)
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Wizard)
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Fighter)

	GET_SLOT(UB2Button, BTN_DetailedInfo)
	BIND_CLICK_FUNC_TO_BTN(BTN_DetailedInfo, &UB2LobbyUI_HeroMgmtCommon::OnDetailedInfoButtonPressed)
	GET_SLOT(UB2Button, BTN_DetailedInfoExtra)
	BIND_CLICK_FUNC_TO_BTN(BTN_DetailedInfoExtra, &UB2LobbyUI_HeroMgmtCommon::OnDetailedInfoButtonPressed)

	TSelectImg.Empty();
	TSelectImg.Add(IMG_Select_Gladiator.Get());
	TSelectImg.Add(IMG_Select_Assassin.Get());
	TSelectImg.Add(IMG_Select_Wizard.Get());
	TSelectImg.Add(IMG_Select_Fighter.Get());

	GET_SLOT(UPanelWidget, HB_EquipPreset)
	GET_SLOT(UPanelWidget, HB_SkillPreset)

	GET_SLOT(UCanvasPanel, X_LeftPopupPanel)
	GET_SLOT(UCanvasPanel, X_RightPopupPanel)

	GET_SLOT(UOverlay, X_FullPopupPanel)

	const int32 MaxPresetSlot = static_cast<uint8>(EItemPresetSlotType::EIPS_End);
	EquipPresets.SetNum(MaxPresetSlot);
	SkillPresets.SetNum(MaxPresetSlot);

	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, EquipPresets[static_cast<uint8>(EItemPresetSlotType::EIPS_First)], EquipPreset_0);
	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, EquipPresets[static_cast<uint8>(EItemPresetSlotType::EIPS_Second)], EquipPreset_1);
	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, EquipPresets[static_cast<uint8>(EItemPresetSlotType::EIPS_Third)], EquipPreset_2);

	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, SkillPresets[static_cast<uint8>(EItemPresetSlotType::EIPS_First)], SkillPreset_0);
	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, SkillPresets[static_cast<uint8>(EItemPresetSlotType::EIPS_Second)], SkillPreset_1);
	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, SkillPresets[static_cast<uint8>(EItemPresetSlotType::EIPS_Third)], SkillPreset_2);

	for (int i = 0; i <MaxPresetSlot; ++i)
	{
		if (EquipPresets[i].IsValid())
		{
			EquipPresets[i]->Init();
			EquipPresets[i]->SetUsageType(EPresetUsageType::Select);
		}
		if (SkillPresets[i].IsValid())
		{
			SkillPresets[i]->Init();
			EquipPresets[i]->SetUsageType(EPresetUsageType::Select);
		}
	}

	SetHidePresetControls();
}

void UB2LobbyUI_HeroMgmtCommon::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(P_HeroGladiator.Get()), &UB2LobbyUI_HeroMgmtCommon::RedDotCondition_Gladiator);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(P_HeroAssassin.Get()), &UB2LobbyUI_HeroMgmtCommon::RedDotCondition_Assassin);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(P_HeroWizard.Get()), &UB2LobbyUI_HeroMgmtCommon::RedDotCondition_Wizard);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(P_HeroFighter.Get()), &UB2LobbyUI_HeroMgmtCommon::RedDotCondition_Fighter);
}

void UB2LobbyUI_HeroMgmtCommon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 官差绢具 窍绰 版快啊 惑寸洒 乐绢辑 概 橇饭烙 窜困肺.. 货肺款 备炼俊辑绰 促福霸 函版秦具 窃.
	UpdateDynamicTexts();
}

void UB2LobbyUI_HeroMgmtCommon::DestroySelf()
{
	if (UIP_CommonHeader.IsValid())
	{
		UIP_CommonHeader->UnbindDoc();
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(UIP_CommonHeader.Get(), FName(TEXT("Header")));
		UIP_CommonHeader->DestroySelf(NULL); // 咯扁急 UIManager 客 惑包绝捞 荤侩阑 窍霸 登骨肺 NULL 阑 傈崔.
	}
	if (UIP_ShowWingSet.IsValid())
	{
		UIP_ShowWingSet->DestroySelf();
	}

	UnsubscribeEvents();

	Super::DestroySelf();
}

void UB2LobbyUI_HeroMgmtCommon::UpdateStaticTexts()
{
	if (TB_HealthLabel.IsValid())
	{
		TB_HealthLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health")));
	}
	if (TB_AttackLabel.IsValid())
	{
		TB_AttackLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack")));
	}
	if (TB_DefenseLabel.IsValid())
	{
		TB_DefenseLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense")));
	}
	if (TB_CombatLable.IsValid())
	{
		TB_CombatLable->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
	}

	if (TB_GladiatorFull.IsValid())
	{
		TB_GladiatorFull->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("InvenFull")));
	}
		
	if (TB_AssassinFull.IsValid())
	{
		TB_AssassinFull->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("InvenFull")));
	}
		
	if (TB_WizardFull.IsValid())
	{
		TB_WizardFull->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("InvenFull")));
	}
		
	if (TB_FighterFull.IsValid())
	{
		TB_FighterFull->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("InvenFull")));
	}
}

void UB2LobbyUI_HeroMgmtCommon::UpdateDynamicTexts()
{
	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(SelectedHeroMgmtPCClass));

	if (DocHero)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
		float fSelectCharExpPercent = CharacterDataStore.GetCharacterExpPercent(SelectedHeroMgmtPCClass);
		int32 iSelectCharLatestLevel = CharacterDataStore.GetCharacterLevel(SelectedHeroMgmtPCClass);

		if (TB_CharLevel.IsValid())
		{
			TB_CharLevel->SetText(
				FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(iSelectCharLatestLevel)))
				);
		}
		if (PB_CharExpProgress.IsValid() && IMG_LevelMax.IsValid())
		{
			if (iSelectCharLatestLevel >= CharacterDataStore.GetCharacterMaxLevel())
			{
				PB_CharExpProgress->SetVisibility(ESlateVisibility::Collapsed);
				IMG_LevelMax->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				IMG_LevelMax->SetVisibility(ESlateVisibility::Collapsed);
				PB_CharExpProgress->SetVisibility(ESlateVisibility::Visible);
				PB_CharExpProgress->SetPercent(fSelectCharExpPercent * 0.01f);
			}
		}
		if (TB_CharExpText.IsValid())
		{
			TB_CharExpText->SetText(FText::FromString(FString::Printf(TEXT("(%d%%)"), (int32)fSelectCharExpPercent)));
		}

		if (TB_HealthPoint.IsValid())
		{
			TB_HealthPoint->SetText(FText::FromString(FString::FromInt(DocHero->GetHealthPoint())));
		}
		if (TB_AttackPoint.IsValid())
		{
			TB_AttackPoint->SetText(FText::FromString(FString::FromInt(DocHero->GetAttackPower())));
		}
		if (TB_DefensePoint.IsValid())
		{
			TB_DefensePoint->SetText(FText::FromString(FString::FromInt(DocHero->GetDefensePower())));
		}
		if (TB_CombatPower.IsValid())
		{
			TB_CombatPower->SetText(FText::AsNumber(DocHero->GetCombatPower()));
		}
	}

	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{
		if (TB_GoldAmount.IsValid()){
			TB_GoldAmount->SetText(FText::FromString(FString::FormatAsNumber(DocUser->GetGold())));
		}
		if (TB_GemAmount.IsValid()){
			TB_GemAmount->SetText(FText::FromString(FString::FormatAsNumber(DocUser->GetGem())));
		}
	}
}

void UB2LobbyUI_HeroMgmtCommon::OnEnhanceItemSetButtonPressed()
{
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_EnhanceItemSetEffectPopup, FDJLegacySubpopupAddPanelInfo());
}

void UB2LobbyUI_HeroMgmtCommon::OnDetailedInfoButtonPressed()
{
	UB2LobbyUserWidget* OwnerUI = nullptr;
	UPanelWidget* PopupAddPanel = nullptr;

	GetDetailPopupWidgetInfo(CurrentHeroMgmtSubMode, &OwnerUI, &PopupAddPanel);

	// 技何 胶湃 pop-up
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_CharStatDetail, FDJLegacySubpopupAddPanelInfo(OwnerUI, PopupAddPanel));
}

void UB2LobbyUI_HeroMgmtCommon::OnShowHeadButtonPressed(bool IsCheck)
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	
	bool CurrentShowHead = BladeIIGameImpl::GetLocalCharacterData().IsShowHeadItem(SelectedPCClass);
	if (CurrentShowHead != IsCheck)
	{
		BladeIIGameImpl::GetLocalCharacterData().SetCacheShowHeadItem(SelectedPCClass, IsCheck);

		LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(SelectedPCClass);
	}
}

void UB2LobbyUI_HeroMgmtCommon::OnGladiatorSelectButtonPressed()
{
	if (SelectedHeroMgmtPCClass != EPCClass::EPC_Gladiator)
	{
		SelectPcClass(EPCClass::EPC_Gladiator);
	}
}

void UB2LobbyUI_HeroMgmtCommon::OnAssassinSelectButtonPressed()
{
	if (SelectedHeroMgmtPCClass != EPCClass::EPC_Assassin)
	{
		SelectPcClass(EPCClass::EPC_Assassin);
	}
}

void UB2LobbyUI_HeroMgmtCommon::OnWizardSelectButtonPressed()
{
	if (SelectedHeroMgmtPCClass != EPCClass::EPC_Wizard)
	{
		SelectPcClass(EPCClass::EPC_Wizard);
	}
}

void UB2LobbyUI_HeroMgmtCommon::OnFighterSelectButtonPressed()
{
	if (SelectedHeroMgmtPCClass != EPCClass::EPC_Fighter)
	{
		SelectPcClass(EPCClass::EPC_Fighter);
	}
}

FText UB2LobbyUI_HeroMgmtCommon::GetHeroMgmtText() const
{
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyHeroMgmt_Title")));
}

FText UB2LobbyUI_HeroMgmtCommon::GetNickNameText() const
{
	return FText::FromString(BladeIIGameImpl::GetClientDataStore().GetUserNickName());
}

FText UB2LobbyUI_HeroMgmtCommon::GetDetailedInfoButtonText() const
{
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyHeroMgmt_DetailedInfoButton")));
}

bool UB2LobbyUI_HeroMgmtCommon::RedDotCondition_Gladiator()
{
	return RedDotCondition_BySubMode(CurrentHeroMgmtSubMode, EPCClass::EPC_Gladiator);
}

bool UB2LobbyUI_HeroMgmtCommon::RedDotCondition_Assassin()
{
	return RedDotCondition_BySubMode(CurrentHeroMgmtSubMode, EPCClass::EPC_Assassin);
}

bool UB2LobbyUI_HeroMgmtCommon::RedDotCondition_Wizard()
{
	return RedDotCondition_BySubMode(CurrentHeroMgmtSubMode, EPCClass::EPC_Wizard);
}

bool UB2LobbyUI_HeroMgmtCommon::RedDotCondition_Fighter()
{
	return RedDotCondition_BySubMode(CurrentHeroMgmtSubMode, EPCClass::EPC_Fighter);
}

bool UB2LobbyUI_HeroMgmtCommon::RedDotCondition_BySubMode(EHeroMgmtSubMode InSubMode, EPCClass PCClass)
{
	FB2RedDotManager& RedDotManager = BladeIIGameImpl::GetRedDotManager();

	switch (InSubMode)
	{
	case EHeroMgmtSubMode::EHMSM_Inventory: return RedDotManager.HasLobbyRedDot_HeroMgmt_Inventory(PCClass);
	case EHeroMgmtSubMode::EHMSM_Skill: return RedDotManager.HasLobbyRedDot_HeroMgmt_Skill(PCClass);
	case EHeroMgmtSubMode::EHMSM_Brevet: return RedDotManager.HasLobbyRedDot_HeroMgmt_Brevet(PCClass);
	case EHeroMgmtSubMode::EHMSM_Wing: return RedDotManager.HasLobbyRedDot_HeroMgmt_Wing(PCClass);
	case EHeroMgmtSubMode::EHMSM_Relic: return RedDotManager.HasLobbyRedDot_HeroMgmt_Relic(PCClass);
	case EHeroMgmtSubMode::EHMSM_Costume: return RedDotCondition_ByCostumeMode(PCClass);
	case EHeroMgmtSubMode::EHMSM_Totem: return RedDotManager.HasLobbyRedDot_HeroMgmt_Totem();
	}
	
	return false;
}

bool UB2LobbyUI_HeroMgmtCommon::RedDotCondition_ByCostumeMode(EPCClass PCClass)
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_CostumeInventory(PCClass);
}


void UB2LobbyUI_HeroMgmtCommon::SetCharacterTextColor(EPCClass eclass)
{
	for (int i = 0; i < GetMaxPCClassNum(); i++)
	{
		if (i == int(eclass))
		{
			TSelectImg[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			TSelectImg[i]->SetVisibility(ESlateVisibility::Hidden);
		}	
	}
	
}

void UB2LobbyUI_HeroMgmtCommon::GetDetailPopupWidgetInfo(EHeroMgmtSubMode InHeroMgmtSubMode, UB2LobbyUserWidget** OutLobbyUserWIdget, UPanelWidget** OutPopupWidget)
{
	//if (CachedLobbyGM != nullptr)
	//{
	//	switch (CurrentHeroMgmtSubMode)
	//	{
	//	case EHeroMgmtSubMode::EHMSM_Inventory:
	//	{
	//		UB2LobbyUI_InventoryMain* OwnerAsInvenUI = Cast<UB2LobbyUI_InventoryMain>(CachedLobbyGM->DJLegacy_GetCurrLobbyUIPage());
	//		*OutLobbyUserWIdget = OwnerAsInvenUI;
	//		*OutPopupWidget = OwnerAsInvenUI ? OwnerAsInvenUI->GetItemDetailPopupParentPanel(true) : nullptr;
	//		break;
	//	}
	//	case EHeroMgmtSubMode::EHMSM_Totem:
	//	{
	//		UB2LobbyUI_TotemInventoryMain* OwnerAsTotemUI = Cast<UB2LobbyUI_TotemInventoryMain>(CachedLobbyGM->DJLegacy_GetCurrLobbyUIPage());
	//		*OutLobbyUserWIdget = OwnerAsTotemUI;
	//		*OutPopupWidget = OwnerAsTotemUI ? OwnerAsTotemUI->GetItemDetailPopupParentPanel(true) : nullptr;
	//		break;
	//	}
	//	}
	//}

	//if (*OutLobbyUserWIdget != nullptr && *OutPopupWidget != nullptr)
	//	return;

	//*OutLobbyUserWIdget = this;
	//*OutPopupWidget = GetInfoPopupParentPanel(true);
}

void UB2LobbyUI_HeroMgmtCommon::SetHidePresetControls()
{
	if (HB_EquipPreset.IsValid())
		HB_EquipPreset->SetVisibility(ESlateVisibility::Hidden);

	if (HB_SkillPreset.IsValid())
		HB_SkillPreset->SetVisibility(ESlateVisibility::Hidden);
}

void UB2LobbyUI_HeroMgmtCommon::SetInventoryFullText()
{
	if (TB_GladiatorFull.IsValid())
	{
		bool IsFull = UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, EPCClass::EPC_Gladiator);
		TB_GladiatorFull->SetVisibility(IsFull ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (TB_AssassinFull.IsValid())
	{
		bool IsFull = UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, EPCClass::EPC_Assassin);
		TB_AssassinFull->SetVisibility(IsFull ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (TB_WizardFull.IsValid())
	{
		bool IsFull = UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, EPCClass::EPC_Wizard);
		TB_WizardFull->SetVisibility(IsFull ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (TB_FighterFull.IsValid())
	{
		bool IsFull = UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, EPCClass::EPC_Fighter);
		TB_FighterFull->SetVisibility(IsFull ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_HeroMgmtCommon::HidePresetWidget(EInventoryPresetMode PresetMode)
{
	if (PresetMode == EInventoryPresetMode::EIPM_Equipment && HB_EquipPreset.IsValid())
		HB_EquipPreset->SetVisibility(ESlateVisibility::Collapsed);

	else if (PresetMode == EInventoryPresetMode::EIPM_Skill && HB_SkillPreset.IsValid())
		HB_SkillPreset->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2LobbyUI_HeroMgmtCommon::HeaderBackButton()
{
	if (UIP_CommonHeader.IsValid())
	{
		UIP_CommonHeader->CloseWidgetDelegate();
	}
}

void UB2LobbyUI_HeroMgmtCommon::InternalSubscribeEvents()
{
	CAPTURE_UOBJECT(UB2LobbyUI_HeroMgmtCommon);

	SelectPresetSlotTicket = SelectPresetSlotClass<EItemPresetSlotType>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectPresetSlot, const EItemPresetSlotType InPresetSlot)
		Capture->SwitchPresetState(InPresetSlot);
	END_CAPTURE_OBJECT()
		);
}

void UB2LobbyUI_HeroMgmtCommon::InternalUnsubscribeEvents()
{
	SelectPresetSlotClass<EItemPresetSlotType>::GetInstance().Unsubscribe(SelectPresetSlotTicket);
}

void UB2LobbyUI_HeroMgmtCommon::SetPresetMode(EInventoryPresetMode PresetMode)
{
	CurrentPreserMode = PresetMode;

	ESlateVisibility SkillPreset(ESlateVisibility::Hidden);
	ESlateVisibility EquipPreset(ESlateVisibility::Hidden);
	
	bool HasPresets = false;
	const int32 MinPreset = 1;
	switch (PresetMode)
	{
		case EInventoryPresetMode::EIPM_Equipment:
			HasPresets = BladeIIGameImpl::GetClientDataStore().GetItemPresetMaxCount() > MinPreset;
			EquipPreset = HasPresets ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

			break;
		case EInventoryPresetMode::EIPM_Skill:
			HasPresets = BladeIIGameImpl::GetClientDataStore().GetSkillPresetMaxCount() > MinPreset;
			SkillPreset = HasPresets ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
			break;
		case EInventoryPresetMode::EIPM_None:
		case EInventoryPresetMode::EIPM_End:
		default:
			break;
	}

	if (HB_EquipPreset.IsValid())		HB_EquipPreset->SetVisibility(EquipPreset);
	if (HB_SkillPreset.IsValid())		HB_SkillPreset->SetVisibility(SkillPreset);
}

void UB2LobbyUI_HeroMgmtCommon::SwitchPresetState(EItemPresetSlotType SelectPreset)
{

	const int32 MaxPresetSlot = static_cast<uint8>(EItemPresetSlotType::EIPS_End);
	const int32 PresetIndex = (int32)SelectPreset;

	const bool IsEquipPreset = CurrentPreserMode == EInventoryPresetMode::EIPM_Equipment;
	const bool IsSkillPreset = CurrentPreserMode == EInventoryPresetMode::EIPM_Skill;

	const bool IsValidPresetMode	 = IsEquipPreset || IsSkillPreset;
	const bool IsValidPresetSelected = PresetIndex >= 0 && PresetIndex < MaxPresetSlot;

	if (IsValidPresetMode && IsValidPresetSelected)
	{
		auto& PresetWidgets = (IsEquipPreset) ? EquipPresets : SkillPresets;
		for (int32 Preset = 0; Preset < MaxPresetSlot; Preset++)
		{
			const bool IsSelectedPreset = (PresetIndex == Preset);

			auto& PresetWidget = PresetWidgets[Preset];
			if (PresetWidget.IsValid())
				PresetWidget->SwitchState(IsSelectedPreset);
		}

		if (IsEquipPreset)
			UpdateItemPresetClass<EPCClass, EItemPresetSlotType>::GetInstance().Signal(SelectedHeroMgmtPCClass, SelectPreset);

		else if (IsSkillPreset)
			UpdateSkillSlotClass<EPCClass, EItemPresetSlotType>::GetInstance().Signal(SelectedHeroMgmtPCClass, SelectPreset);
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_HeroMgmtCommon::ShowNewMark(EPCClass PCClass, bool bShow)
{
}

void UB2LobbyUI_HeroMgmtCommon::ShowCharacterButtons(bool bShow)
{
	if (P_HeroGladiator.IsValid())
	{
		P_HeroGladiator->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (P_HeroAssassin.IsValid())
	{
		P_HeroAssassin->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (P_HeroWizard.IsValid())
	{
		P_HeroWizard->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (P_HeroFighter.IsValid())
	{
		P_HeroFighter->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_HeroMgmtCommon::ShowEnhanceItemSetButton(bool bShow)
{
	if (BTN_EnhanceItemSet.IsValid())
	{
		BTN_EnhanceItemSet->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_HeroMgmtCommon::ShowWingButton(bool bShow)
{
	if (UIP_ShowWingSet.IsValid())
	{
		UIP_ShowWingSet->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}
void UB2LobbyUI_HeroMgmtCommon::ShowHeadButton(bool bShow)
{
	if (CB_ShowHead.IsValid())
	{
		CB_ShowHead->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_HeroMgmtCommon::ShowDetailedInfoExtraButtons(bool bShow)
{
	if (BTN_DetailedInfo.IsValid())
	{
		BTN_DetailedInfo->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (BTN_DetailedInfoExtra.IsValid())
	{
		BTN_DetailedInfoExtra->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_HeroMgmtCommon::SetExtraDisableButtons(bool bInDisable)
{
	// 俺喊 滚瓢 窜困肺 掺绊 挠 荐档 乐瘤父 老窜篮 窜鉴窍霸 啊磊..
	this->SetVisibility(bInDisable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);
}

////////////////////////////////////////////////////////////////////////////////
// UB2LobbyUI_HeroMgmtShowWingSet
// 窍困 葛靛喊肺 力阿阿 甸绢哎 临 舅绊 蝶肺 颇飘拳 沁绰单 瘤陛篮 傲 康旷包府 傍烹 颇飘俊父 乐绢辑.. 捞凡 鞘夸鳖柳 绝菌促. 窍瘤父 绢痘霸 瞪瘤 趣矫 肚 葛福瘤
////////////////////////////////////////////////////////////////////////////////

UB2LobbyUI_HeroMgmtShowWingSet::UB2LobbyUI_HeroMgmtShowWingSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2LobbyUI_HeroMgmtShowWingSet::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	UpdateShowWingBtnImage();
}

void UB2LobbyUI_HeroMgmtShowWingSet::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_ShowWing);
	GET_SLOT(UImage, IMG_ShowWing_BGOn);
	GET_SLOT(UImage, IMG_ShowWing_IconOn);
	GET_SLOT(UImage, IMG_ShowWing_BGOff);
	GET_SLOT(UImage, IMG_ShowWing_IconOff);
	GET_SLOT(UTextBlock, TB_ShowWing);
	BIND_CLICK_FUNC_TO_BTN(BTN_ShowWing, &UB2LobbyUI_HeroMgmtShowWingSet::OnClickedShowWingBtn);

}

void UB2LobbyUI_HeroMgmtShowWingSet::DestroySelf()
{
	UB2Airport::FlushPendingWingVisibilityReq(); // 朝俺 visibility on/off 茄 芭 葛酒辑 辑滚肺 夸没. 趣 咯扁肺 救棵鳖 酵绢辑 AB2LobbyGameMode::OnQuitHeroMgmtSubModeCommon 俊辑档.

	Super::DestroySelf();
}

void UB2LobbyUI_HeroMgmtShowWingSet::OnClickedShowWingBtn()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);

	if (bGotValidWingData)
	{
		// 滚瓢阑 磊林 On/Off 且 荐 乐栏骨肺 辑滚肺 焊郴绰 菩哦阑 弥家拳窍扁 困秦 咯扁辑绰 官肺 辑滚俊 request 甫 朝府瘤 臼绊 康旷包府 葛靛甫 唱哎 锭 朝赴促.
		UB2Airport::SetWingVisibility(SelectedPCClass, !CurrentWingData.bShouldBeVisible, false);

		UpdateShowWingBtnImage();
	}
}

void UB2LobbyUI_HeroMgmtShowWingSet::UpdateShowWingBtnImage()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);

	if (bGotValidWingData)
	{
		if (TB_ShowWing.IsValid())
		{
			TB_ShowWing->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, CurrentWingData.bShouldBeVisible ? TEXT("CollectBook_ShowWing") : TEXT("CollectBook_HideWing")));
		}

		if (IMG_ShowWing_BGOn.IsValid())
		{
			IMG_ShowWing_BGOn->SetVisibility(CurrentWingData.bShouldBeVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
		if (IMG_ShowWing_IconOn.IsValid())
		{
			IMG_ShowWing_IconOn->SetVisibility(CurrentWingData.bShouldBeVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
		if (IMG_ShowWing_BGOff.IsValid())
		{
			IMG_ShowWing_BGOff->SetVisibility(CurrentWingData.bShouldBeVisible ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		}
		if (IMG_ShowWing_IconOff.IsValid())
		{
			IMG_ShowWing_IconOff->SetVisibility(CurrentWingData.bShouldBeVisible ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		}
	}
}
