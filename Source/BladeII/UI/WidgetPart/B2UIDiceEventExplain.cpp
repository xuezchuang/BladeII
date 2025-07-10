// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDiceEventExplain.h"

#include "BladeIIUtil.h"
#include "RewardEventManager.h"
#include "B2GameInstance.h"
#include "B2UIDiceEventExplainPopup.h"
#include "B2UIUtil.h"

void UB2UIDiceEventExplain::DestroySelf(class UB2UIManager* InUIManager)
{
	if (pDiceEventExplainPopup.IsValid())
	{
		pDiceEventExplainPopup->DestroySelf(InUIManager);
		pDiceEventExplainPopup = nullptr;
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIDiceEventExplain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_RemaningTimeExplain);
	GET_SLOT(UTextBlock, TB_RemaningTime);

	GET_SLOT(UB2Button, BTN_Tip);
	GET_SLOT(UTextBlock, TB_BtnTip);

	pDiceEventExplainPopup = nullptr;
}

void UB2UIDiceEventExplain::UpdateStaticText()
{
	if (TB_RemaningTimeExplain.IsValid())
	{
		TB_RemaningTimeExplain->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_TimeTitle")));
	}

	if (TB_BtnTip.IsValid())
	{
		TB_BtnTip->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_Tip")));
	}
}

void UB2UIDiceEventExplain::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Tip, &UB2UIDiceEventExplain::OnClickBTN_Tip);
}

void UB2UIDiceEventExplain::OnClickBTN_Tip()
{
	OpenDiceEventExplainPopup();
}

void UB2UIDiceEventExplain::OpenDiceEventExplainPopup()
{
	if (pDiceEventExplainPopup.IsValid())
	{
		pDiceEventExplainPopup->UpdateFinishRewardState(CachedFinishCount);
		pDiceEventExplainPopup->OpenPopup();
	}
	else
	{
		if (auto* pCreateWidget = CreateWidget<UB2UIDiceEventExplainPopup>(GetWorld(), UIDiceEventExplainPopupClass))
		{
			pDiceEventExplainPopup = pCreateWidget;
			pDiceEventExplainPopup->InitDiceEventExplainPopup(CachedEventID);
			pDiceEventExplainPopup->UpdateFinishRewardState(CachedFinishCount);
			pDiceEventExplainPopup->OpenPopup();
		}
	}
}

void UB2UIDiceEventExplain::UpdateEventExplain(int32 iEventID, int32 iFinishCount)
{
	CachedEventID = iEventID;
	CachedFinishCount = iFinishCount; 

	UpdateRemaningTimeText();
}

void UB2UIDiceEventExplain::UpdateRemaningTimeText()
{
	if (TB_RemaningTime.IsValid() == false)
	{
		return;
	}

	if (const b2network::B2mdEventInfo* pEventInfo = RewardEventManager::GetInstance().GetEnableEventInfo(CachedEventID))
	{
		FTimespan RemainTime = FDateTime::FromUnixTimestamp(pEventInfo->end_time / 1000) - UB2GameInstance::GetUTCNow();

		FText ftRemainingTime;

		if (RemainTime.GetTicks() > 0)
		{
			ftRemainingTime = B2UIUtil::TwoStepTimeSpanToString(RemainTime);
		}
		else
		{
			//기간 만료
		}
		
		TB_RemaningTime->SetText(ftRemainingTime);
	}	
}