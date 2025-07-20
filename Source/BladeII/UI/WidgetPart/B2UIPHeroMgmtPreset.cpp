// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPHeroMgmtPreset.h"
#include "BladeIIGameMode.h"
#include "BladeIILocText.h"
#include "Event.h"


void UB2UIPHeroMgmtPreset::Init()
{
	Super::Init();
}

void UB2UIPHeroMgmtPreset::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Select);
	GET_SLOT(UTextBlock, TB_Unselect);
	GET_SLOT(UImage, IMG_Select);
	GET_SLOT(UImage, IMG_Unselect)
	GET_SLOT(UB2Button, BTN_Area);
}

void UB2UIPHeroMgmtPreset::UpdateStaticText()
{
}

void UB2UIPHeroMgmtPreset::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIPHeroMgmtPreset::OnPressedArea);
}

void UB2UIPHeroMgmtPreset::SetUsageType(EPresetUsageType UsageType)
{
	PresetUsageType = UsageType;
}

void UB2UIPHeroMgmtPreset::SetSlotNumber(EPCClass SelectPCClass, EItemPresetSlotType SlotNumber)
{
	SelectedHeroMgmtPCClass = SelectPCClass;
	PresetSlot = SlotNumber;

	IsPressed = false;

	int32 SlotNum = (int32)SlotNumber + 1;
	if (TB_Select.IsValid())
		TB_Select->SetText(FText::AsNumber(SlotNum));
	if (TB_Unselect.IsValid())
		TB_Unselect->SetText(FText::AsNumber(SlotNum));
}

void UB2UIPHeroMgmtPreset::OnPressedArea()
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_StoredItemDetail);
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_CharStatDetail);

	switch (PresetUsageType)
	{
	case EPresetUsageType::Select:
	{
		bool beforeIsPressed = IsPressed;

		SelectPresetSlotClass<EItemPresetSlotType>::GetInstance().Signal(PresetSlot);

		if (beforeIsPressed == false)
			LobbySlotChangeEffectClass<>::GetInstance().Signal();
		break;
	}
	case EPresetUsageType::Preview:
		SelectPreviewPresetSlotClass<EItemPresetSlotType>::GetInstance().Signal(PresetSlot);
		break;
	default:
		break;
	}
}

void UB2UIPHeroMgmtPreset::SwitchState(bool bEnabled)
{
	IsPressed = bEnabled;

	if (IMG_Select.IsValid())
		IMG_Select->SetVisibility(bEnabled ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	if (IMG_Unselect.IsValid())
		IMG_Unselect->SetVisibility(bEnabled ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (TB_Select.IsValid())
		TB_Select->SetVisibility(bEnabled ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	if (TB_Unselect.IsValid())
		TB_Unselect->SetVisibility(bEnabled ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIPHeroMgmtPreset::ShowSlotPressedMessage()
{
	FText Format = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PresetSlotChangeNotice"));

	const int32 DisplaySlotNumber = static_cast<int32>(PresetSlot) + 1;
	FText SlotNoticeText = FText::Format(Format, FText::AsNumber(DisplaySlotNumber));

	ABladeIIGameMode::SetHUDCenterMessage(SlotNoticeText, 1.0f);
}
