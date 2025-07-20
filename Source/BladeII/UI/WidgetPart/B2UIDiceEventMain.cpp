// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDiceEventMain.h"

#include "BladeIIUtil.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
#include "B2ClientDataStore.h"

void UB2UIDiceEventMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_DiceRoll);
	GET_SLOT(UTextBlock, TB_BtnDiceRoll);

	GET_SLOT(UTextBlock, TB_EventExplain);

	GET_SLOT(UTextBlock, TB_RolledDiceExplain);
	GET_SLOT(UTextBlock, TB_RolledDiceCount);

	GET_SLOT(UProgressBar, PB_ProgressGetDicePoint);
	GET_SLOT(UTextBlock, TB_ProgressGetDicePoint);
	GET_SLOT(UTextBlock, TB_PossessionDiceCount);

	GET_SLOT(UTextBlock, TB_FinishCount);
}

void UB2UIDiceEventMain::UpdateStaticText()
{
	if (TB_EventExplain.IsValid())
	{
		TB_EventExplain->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_FreeDiceGuide")), FText::AsNumber(GetDiceFreeCountMax())));
	}

	if (TB_RolledDiceExplain.IsValid())
	{
		//TB_RolledDiceExplain->SetText();
	}
}

void UB2UIDiceEventMain::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_DiceRoll, &UB2UIDiceEventMain::OnClickBTN_DiceRoll);
}

void UB2UIDiceEventMain::OnClickBTN_DiceRoll()
{
	// 滚瓢 捞固瘤 盎脚 捞酱啊 乐绢 掉饭捞 饶 肺流 贸府..
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UIDiceEventMain::OnDelayDiceRollCallback, 0.1f, false);
}
void UB2UIDiceEventMain::OnDelayDiceRollCallback()
{
	data_trader::Retailer::GetInstance().RequestRollDiceEvent(CachedEventID, IsFreeDice());
}

void UB2UIDiceEventMain::UpdateDiceEventMain(int32 iEventID, int32 iFreeDiceCount, int32 iGetDicePoint, int32 iRolledDiceCount, int32 iFinishCount)
{
	CachedEventID = iEventID;
	CachedIsFreeDice = iFreeDiceCount;
	CachedGetDicePoint = iGetDicePoint;

	UpdateDynamicText(iGetDicePoint, iRolledDiceCount, iFinishCount);
	UpdateProgressBarGetDicePoint(iGetDicePoint);
	UpdateDiceRollBtn(iFreeDiceCount, iGetDicePoint);
}

void UB2UIDiceEventMain::UpdateDynamicText(int32 iGetDicePoint, int32 iRolledDiceCount, int32 iFinishCount)
{
	if (TB_RolledDiceCount.IsValid())		//带柳 穿利 林荤困 荐
	{
		FText ftRolledDiceCount = FText::FromString(FString::Printf(TEXT("%d"), iRolledDiceCount));

		TB_RolledDiceCount->SetText(ftRolledDiceCount);
	}

	if (TB_ProgressGetDicePoint.IsValid())	//林荤困 裙垫 器牢飘/弥措 器牢飘
	{
		FText ftProgressGetDicePoint = FText::FromString(FString::Printf(TEXT("%d/%d"), GetProgressGetDicePoint(), GetDicePointMax()));

		TB_ProgressGetDicePoint->SetText(ftProgressGetDicePoint);
	}

	if (TB_PossessionDiceCount.IsValid())	//焊蜡 林荤困 荐
	{
		FText ftPossessionDiceCount = FText::FromString(FString::Printf(TEXT("%d"), GetPossessionDiceCount()));

		TB_PossessionDiceCount->SetText(ftPossessionDiceCount);
	}

	if (TB_FinishCount.IsValid())	//肯林 冉荐
	{
		FText ftFinishText = BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_FinishTitle"));
		FText ftCountText = FText::FromString(FString::Printf(TEXT(" : %d"), iFinishCount));

		FString FinalString = ftFinishText.ToString();
		FinalString.Append(ftCountText.ToString());

		TB_FinishCount->SetText(FText::FromString(FinalString));
	}
}

void UB2UIDiceEventMain::UpdateProgressBarGetDicePoint(int32 iGetDicePoint)
{
	if (PB_ProgressGetDicePoint.IsValid())
	{
		float ProgressRate = static_cast<float>(GetProgressGetDicePoint()) / static_cast<float>(GetDicePointMax());

		if (ProgressRate >= 1.f)
		{
			ProgressRate = 0.f;
		}

		PB_ProgressGetDicePoint->SetPercent(ProgressRate);
	}
}

void UB2UIDiceEventMain::UpdateDiceRollBtn(int32 iFreeDiceCount, int32 iGetDicePoint)
{
	FText	ftBtnDiceRoll;
	bool	IsPossibleRollDice = false;

	if (iFreeDiceCount)
	{
		FText ftDiceFree = BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_DiceButton_Free"));
		FText ftFreeCount = FText::FromString(FString::Printf(TEXT("%d/%d"), iFreeDiceCount, GetDiceFreeCountMax()));

		FString FinalString = ftDiceFree.ToString();
		FinalString.Append(ftFreeCount.ToString());

		ftBtnDiceRoll = FText::FromString(FinalString);
	
		IsPossibleRollDice = true;
	}
	else
	{
		ftBtnDiceRoll = BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_DiceButton_Point"));

		if (GetPossessionDiceCount() > 0)
		{
			IsPossibleRollDice = true;
		}
	}

	if (TB_BtnDiceRoll.IsValid())
	{
		TB_BtnDiceRoll->SetText(ftBtnDiceRoll);
	}

	if (BTN_DiceRoll.IsValid())
	{
		BTN_DiceRoll->SetIsEnabled(IsPossibleRollDice);
	}
}

int32 UB2UIDiceEventMain::GetProgressGetDicePoint()
{ 
	return CachedGetDicePoint % GetDicePointMax();
}

int32 UB2UIDiceEventMain::GetPossessionDiceCount()
{
	return CachedGetDicePoint / GetDicePointMax();
}

int32 UB2UIDiceEventMain::GetDicePointMax()
{
	return BladeIIGameImpl::GetClientDataStore().GetEventDicePointPerOneDice();
}

int32 UB2UIDiceEventMain::GetDiceFreeCountMax()
{
	return BladeIIGameImpl::GetClientDataStore().GetEventDiceFreeCount();
}