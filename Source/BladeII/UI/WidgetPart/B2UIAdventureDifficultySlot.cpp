// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIAdventureDifficultySlot.h"

#include "B2UIDocHelper.h"
#include "B2UIDocChapter.h"


void UB2UIAdventureDifficultySlot::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UOverlay, O_LockOverlay);
	GET_SLOT(UOverlay, O_SelectOverlay);

	GET_SLOT(UTextBlock, TB_Unlock1);
	GET_SLOT(UTextBlock, TB_Unlock2);
	GET_SLOT(UTextBlock, TB_Unlock3);
	GET_SLOT(UTextBlock, TB_Reward1);
	GET_SLOT(UTextBlock, TB_Reward2);
	GET_SLOT(UTextBlock, TB_Reward3);
}

void UB2UIAdventureDifficultySlot::BindDelegates()
{
	if (BTN_Select.IsValid())
		BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIAdventureDifficultySlot::OnClickButton);
}

void UB2UIAdventureDifficultySlot::OnClickButton()
{
	FOnClickBTNSelect.ExecuteIfBound(SlotDifficulty);
}

void UB2UIAdventureDifficultySlot::InitDifficulty(EStageDifficulty CurStageDifficulty, EStageDifficulty MaxStageDifficulty)
{
	if (CurStageDifficulty <= EStageDifficulty::ESD_Normal)
		CurStageDifficulty = EStageDifficulty::ESD_Hard;

	SlotDifficulty = CurStageDifficulty;
	SetDifficulty_BP(FMath::Max((int32)SlotDifficulty - 2, 0));	// 喉风橇赴飘俊辑绰 牢郸胶啊 0何磐 矫累窍骨肺 蔼阑 烙狼肺 函版秦霖促.
	SetUnlock((MaxStageDifficulty >= SlotDifficulty), false);
}

void UB2UIAdventureDifficultySlot::SetUnlock(bool isUnLock, bool isAnimation)
{
	// 遏 捞固瘤 力芭 棺 急琶康开 劝己拳
	if (O_LockOverlay.IsValid())
		O_LockOverlay->SetVisibility(isUnLock ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (BTN_Select.IsValid())
		BTN_Select->SetVisibility(isUnLock ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (TB_Unlock1.IsValid())
		TB_Unlock1->SetVisibility(isUnLock ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	if (TB_Unlock2.IsValid())
		TB_Unlock2->SetVisibility(isUnLock ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	if (TB_Unlock3.IsValid())
		TB_Unlock3->SetVisibility(isUnLock ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	
	if (TB_Reward1.IsValid())
		TB_Reward1->SetVisibility(isUnLock ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (TB_Reward2.IsValid())
		TB_Reward2->SetVisibility(isUnLock ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (TB_Reward3.IsValid())
		TB_Reward3->SetVisibility(isUnLock ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	SetUnLock_BP(isAnimation);
}

void UB2UIAdventureDifficultySlot::SetSelectSlot(bool isSelect)
{
	if (O_SelectOverlay.IsValid())
		O_SelectOverlay->SetVisibility(isSelect ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	SetSelect_BP(isSelect);
}

void UB2UIAdventureDifficultySlot::UpdateSlot(EStageDifficulty CurSelectDifficulty)
{
	SetSelectSlot(SlotDifficulty == CurSelectDifficulty);
}

void UB2UIAdventureDifficultySlot::SetDelegate(UB2UIAdventureDifficultySlot_OnClickBTNSelect OnClick_BTNSelect)
{
	FOnClickBTNSelect = OnClick_BTNSelect;
}

