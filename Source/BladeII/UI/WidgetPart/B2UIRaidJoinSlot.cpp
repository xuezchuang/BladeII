// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRaidJoinSlot.h"
#include "B2UIDocRaid.h"

#include "B2UIManager.h"
#include "B2UIGameInviteList.h"


#include "BladeIIUtil.h"

void UB2UIRaidJoinSlot::Init()
{
	Super::Init();
	IsMySlot = false;
	SlotState = ERaidJoinSlotState::SearchingQuickJoin;
}

void UB2UIRaidJoinSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UImage, IMG_MainCharacter);
	GET_SLOT(UImage, IMG_Me);

	GET_SLOT(UTextBlock, TBS_CharLevel);
	GET_SLOT(UTextBlock, TBS_CharName);
	GET_SLOT(UB2Button, BT_Close_01);
	GET_SLOT(UB2Button, BT_Close_02);

	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, STB_Invite);
	GET_SLOT(UTextBlock, STB_Invite2);
	GET_SLOT(UTextBlock, STB_Search);

	GET_SLOT(UTextBlock, STB_GuildMan);
	GET_SLOT(UPanelWidget, P_GuildMan);
}

void UB2UIRaidJoinSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIRaidJoinSlot::OnClick_BTN_Select);
	BIND_CLICK_FUNC_TO_BTN(BT_Close_01, &UB2UIRaidJoinSlot::OnClick_BTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BT_Close_02, &UB2UIRaidJoinSlot::OnClick_BTN_Close2);
}

void UB2UIRaidJoinSlot::UpdateStaticText()
{
	if (STB_Invite.IsValid())
		STB_Invite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_Invite"))));
	if (STB_Invite2.IsValid())
		STB_Invite2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_Invite"))));
	if (STB_Search.IsValid())
		STB_Search->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_SearchMatch"))));
	if (STB_GuildMan.IsValid())
		STB_GuildMan->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_guildmembers"))));
}

void UB2UIRaidJoinSlot::OnClick_BTN_Select()
{
	FClickBTNSelect.ExecuteIfBound(SlotNum);
}

void UB2UIRaidJoinSlot::OnClick_BTN_Close()
{
	// 초대 취소
}

void UB2UIRaidJoinSlot::OnClick_BTN_Close2()
{
	// 강퇴
}

void UB2UIRaidJoinSlot::SetSlotNum(int32 InSlotNum)
{
	SlotNum = InSlotNum;
}

void UB2UIRaidJoinSlot::SetSlotState(ERaidJoinSlotState InSlotState)
{
	SlotState = InSlotState;
	SetVisibleSelectBtn(InSlotState == ERaidJoinSlotState::Invitable);
	SetSlotState_BP((int32)InSlotState);
	FUpdateSlot.ExecuteIfBound();
}

void UB2UIRaidJoinSlot::SetVisibleSelectBtn(bool IsVisible)
{
	BTN_Select->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIRaidJoinSlot::SetSlotEntryData(FText CharName, int32 CharLevel, EPCClass PCType, int32 CombatPower, bool IsMyGuild)
{
	if (PCType != EPCClass::EPC_End)
	{
		int32 PcIndex = PCClassToInt(PCType);
		if (CharterTexture.IsValidIndex(PcIndex) && IMG_MainCharacter.IsValid())
			IMG_MainCharacter->SetBrushFromMaterial(CharterTexture[PcIndex]);
	}

	/*if (TB_TotalAttack.IsValid())
		TB_TotalAttack->SetText(FText::AsNumber(AttackPoint));

	if (TB_TotalDefense.IsValid())
		TB_TotalDefense->SetText(FText::AsNumber(DefensePoint));*/

	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(CombatPower));

	if (TBS_CharName.IsValid())
		TBS_CharName->SetText(CharName);

	if (TBS_CharLevel.IsValid())
		TBS_CharLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(CharLevel))));

	if (P_GuildMan.IsValid())
		P_GuildMan->SetVisibility(IsMyGuild ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

}

void UB2UIRaidJoinSlot::OnChanagedPCClass(UB2UIDocBase* Sender, EPCClass PCClass, EPCClass PrevPCClass)
{
	if (PCClass != EPCClass::EPC_End)
	{
		int32 PcIndex = PCClassToInt(PCClass);
		if (CharterTexture.IsValidIndex(PcIndex) && IMG_MainCharacter.IsValid())
			IMG_MainCharacter->SetBrushFromMaterial(CharterTexture[PcIndex]);
	}
}

void UB2UIRaidJoinSlot::OnChanagedCombatPoint(UB2UIDocBase* Sender, int32 CombatPower, int32 PreDefensePoint)
{
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(CombatPower));
}

/*
void UB2UIRaidJoinSlot::OnChanagedTotalAttackPoint(UB2UIDocBase* Sender, int32 AttackPoint, int32 PreAttackPoint)
{
/ *
	if (TB_TotalAttack.IsValid())
		TB_TotalAttack->SetText(FText::AsNumber(AttackPoint));* /
}

void UB2UIRaidJoinSlot::OnChanagedTotalDefensePoint(UB2UIDocBase* Sender, int32 DefensePoint, int32 PreDefensePoint)
{
	/ *if (TB_TotalDefense.IsValid())
		TB_TotalDefense->SetText(FText::AsNumber(DefensePoint));* /
}
*/

void UB2UIRaidJoinSlot::OnChanagedSlotState(UB2UIDocBase* Sender, ERaidJoinSlotState CurSlotState, ERaidJoinSlotState PreSlotState)
{
	SetSlotState(CurSlotState);
}

void UB2UIRaidJoinSlot::OnChanagedCharLevel(UB2UIDocBase* Sender, int32 CurLevel, int32 PreLevel)
{
	if (TBS_CharLevel.IsValid())
		TBS_CharLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(CurLevel))));
}

void UB2UIRaidJoinSlot::OnChanagedCharName(UB2UIDocBase* Sender, FText CurName, FText PreName)
{
	if (TBS_CharName.IsValid())
		TBS_CharName->SetText(CurName);
}

void UB2UIRaidJoinSlot::OnChangedMyGuildMember(UB2UIDocBase* Sender, bool CurMyGuild, bool PreMyGuild)
{
	if (P_GuildMan.IsValid())
		P_GuildMan->SetVisibility(CurMyGuild ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

bool UB2UIRaidJoinSlot::IsBattleReady()
{
	return (SlotState == ERaidJoinSlotState::JoinCharacter);
}

void UB2UIRaidJoinSlot::SetIsMySlot(bool InIsMySlot)
{
	IsMySlot = InIsMySlot;

	if (IMG_Me.IsValid())
		IMG_Me->SetVisibility(InIsMySlot ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	SetIsVisibleBTNCancel(false);
}


void UB2UIRaidJoinSlot::SetIsVisibleBTNCancel(bool IsVisble)
{
	// 활성화 가능하고, 내것이 아닐때만 보여지게
	if (BT_Close_02.IsValid())
		BT_Close_02->SetVisibility(IsVisble ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIRaidJoinSlot::OnChanagedMySlotIndex(UB2UIDocBase* Sender, int32 CurMySlotIdx, int32 PreMySlotIdx)
{
	SetIsMySlot(CurMySlotIdx == SlotNum);
}
