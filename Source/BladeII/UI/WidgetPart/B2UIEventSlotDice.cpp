// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventSlotDice.h"

#include "B2RewardInfo.h"
#include "BladeIIUtil.h"

void UB2UIEventSlotDice::InitEventSlotDiceCell(const b2network::B2mdDiceEventCell& DiceEventCell)
{
	CachedDiceEventCellMD = DiceEventCell;

	this->Init();

	UpdateIMG_RewardIcon();
}

void UB2UIEventSlotDice::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_Board);

	GET_SLOT(UOverlay, O_Arrive);
	if (O_Arrive.IsValid())
	{
		O_Arrive->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UTextBlock, TB_RewardCount);

	GET_SLOT(UTextBlock, TB_Times);

	GET_SLOT(UTextBlock, TB_Start);

	GET_SLOT(UOverlay, O_RewardIcon);
	GET_SLOT(UImage, IMG_Start);
	GET_SLOT(UB2Image, IMG_RewardIcon);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_RewardIcon);
}

void UB2UIEventSlotDice::BindDelegates()
{
	Super::BindDelegates();

	if (BTN_RewardIcon.IsValid())
	{
		FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetInfoData(CachedDiceEventCellMD.reward_id) : nullptr;
		if (RewardInfo != nullptr)
		{
			BTN_RewardIcon->SetItemInfo(RewardInfo->ItemInfoIndex);
		}
	}
}

void UB2UIEventSlotDice::UpdateStaticText()
{
	if (TB_Times.IsValid())
	{
		FText ftTimes = FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_BonusBlank1")), FText::AsNumber(CachedDiceEventCellMD.effect_value));
		TB_Times->SetText(ftTimes);
	}

	if (TB_Start.IsValid())
	{
		TB_Start->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_StartBlank")));
	}

	if (TB_RewardCount.IsValid())
	{
		FText ftRewardCount = FText::FromString(FString::Printf(TEXT("x %d"), CachedDiceEventCellMD.reward_count));
		TB_RewardCount->SetText(ftRewardCount);
	}
}

void UB2UIEventSlotDice::UpdateVisibleState()
{
	/*
	namespace B2DiceEventCellType
	{
	const int START = 1;
	const int REWARD = 2;
	const int TIMES = 3;
	}
	*/

	if (WS_Board.IsValid())
	{
		if (IMG_Start.IsValid())
		{
			// 한번 Hidden 된 Start 이미지가 다시 표기될 일은 없다.
			if (IMG_Start->GetVisibility() != ESlateVisibility::Hidden)
			{
				IMG_Start->SetVisibility(CachedDiceEventCellMD.cell_type == b2network::B2DiceEventCellType::START ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
			}
		}

		if (O_RewardIcon.IsValid())
		{
			O_RewardIcon->SetVisibility(CachedDiceEventCellMD.cell_type == b2network::B2DiceEventCellType::TIMES ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
		}

		WS_Board->SetActiveWidgetIndex(CachedDiceEventCellMD.cell_type);
	}
}

void UB2UIEventSlotDice::EndAnimMovePosition_BP()
{
	//int32 NextCellNumber = CachedDiceEventCellMD.cell_number + 1;
	//DiceCellFinishAnimationClass<int32>::GetInstance().Signal(NextCellNumber);

	//SetArriveState(false);
}

void UB2UIEventSlotDice::EndAnimArrive_BP()
{
	//DiceCellFinishArriveAnimationClass<int32>::GetInstance().Signal(CachedDiceEventCellMD.cell_number);

	//SetArriveState(true);
}

void UB2UIEventSlotDice::UpdateIMG_RewardIcon()
{
	if (IMG_RewardIcon.IsValid())
	{
		UMaterialInterface* pMaterial = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetRewardIconMaterial(CachedDiceEventCellMD.reward_id) : nullptr;
		if (pMaterial)
		{
			IMG_RewardIcon->SetBrushFromMaterial(pMaterial);
		}
	}
}

void UB2UIEventSlotDice::UpdateArriveState(int32 iPosition)
{
	bool IsArrive = false;

	if (iPosition == CachedDiceEventCellMD.cell_number)
	{
		IsArrive = true;
	}

	SetArriveState(IsArrive);
}

void UB2UIEventSlotDice::SetArriveState(bool IsArrive)
{
	if (O_Arrive.IsValid())
	{
		O_Arrive->SetVisibility(IsArrive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UIEventSlotDice::SetVisibleStartIcon(const bool Visible)
{
	if (IMG_Start.IsValid())
	{
		IMG_Start->SetVisibility(Visible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

int32 UB2UIEventSlotDice::GetCellNumberFromWidgetName()
{
	if (CellNumberFromWidgetName == -1)
	{
		const FString ThisWidgetName = this->GetFName().ToString();

		const TCHAR* DivideChar = TEXT("_");	//ex) EventSlot_1, EventSlot_2 ...
		const int32 DividePos = ThisWidgetName.Find(DivideChar, ESearchCase::IgnoreCase, ESearchDir::FromStart);
		if (DividePos != INDEX_NONE)
		{
			FString StringBoardIndex = ThisWidgetName.Mid(DividePos + 1, ThisWidgetName.Len() - DividePos + 1);
			CellNumberFromWidgetName = FCString::Atoi(*StringBoardIndex);
		}
	}

	return CellNumberFromWidgetName;
}