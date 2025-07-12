// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIConnectRewardPart.h"
#include "B2UIManager.h"

#include "BladeIIGameMode.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2RewardInfo.h"
#include "B2GameInstance.h"

#include "BladeIIGameImpl.h"
#include "../../Common/Event.h"

void UB2UIConnectRewardPart::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Image, IMG_RewardDone);
	GET_SLOT(UB2Image, IMG_ITEM);
	GET_SLOT(UB2Image, IMG_ActiveInner);
	GET_SLOT(UB2Image, IMG_ActiveOuter);
	GET_SLOT(UImage, IMG_Base);
	GET_SLOT(UImage, IMG_RewardEnd);
	GET_SLOT(UImage, IMG_RewardDone);
	GET_SLOT(UTextBlock, TB_RewardCount);
	GET_SLOT(UB2RichTextBlock, TB_Minute);
	GET_SLOT(UB2RichTextBlock, TB_ItemName);
	GET_SLOT(UPanelWidget, P_ReceiveComplete);
	GET_SLOT(UPanelWidget, P_CanReceive);
	GET_SLOT(UTextBlock, TB_GetReward);

	Anim_Receive = GetAnimation(this, "Anim_Receive");
}

void UB2UIConnectRewardPart::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIConnectRewardPart::Init()
{
	Super::Init();
	CacheAssets();
	RewardTime = 0.0f;
}

void UB2UIConnectRewardPart::UpdateStaticText()
{
	if (TB_GetReward.IsValid())
	{
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));
	}
}

void UB2UIConnectRewardPart::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

float UB2UIConnectRewardPart::GetRewardTime() const
{
	return static_cast<float>(RewardTime);
}

void UB2UIConnectRewardPart::SetCanReceiveEffect(bool Enable)
{
	if (P_CanReceive.IsValid())
	{
		if (Enable)
		{
			if (P_CanReceive->GetVisibility() == ESlateVisibility::Collapsed)
			{
				P_CanReceive->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		else
		{
			if (P_CanReceive->GetVisibility() == ESlateVisibility::HitTestInvisible)
			{
				P_CanReceive->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

bool UB2UIConnectRewardPart::GetReward() const
{
	return IsReward;
}

int32 UB2UIConnectRewardPart::GetRewardItemIndex() const
{
	 return RewardItemIndex;
}

float UB2UIConnectRewardPart::GetInterval() const
{
	return Interval;
}

void UB2UIConnectRewardPart::SetRewardState(int32 InItemIndex, int32 InRewardTime, float InInterval, int32 InRewardCount, bool InReward, int32 InRewardOrder)
{
	IsReward		= InReward;
	Interval        = InInterval;
	RewardTime      = InRewardTime;
	RewardItemIndex = InItemIndex;
	RewardOrder     = InRewardOrder;

	if (IMG_ITEM.IsValid())
		IMG_ITEM->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(RewardItemIndex));
	if (TB_ItemName.IsValid())
		TB_ItemName->SetText(BladeIIGameImpl::GetClientDataStore().GetRewardName(RewardItemIndex));
	if (TB_Minute.IsValid())
		TB_Minute->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AccessReward_Gift")), FText::FromString(FString::FromInt(RewardTime))));
	if (TB_RewardCount.IsValid())
		TB_RewardCount->SetText(FText::AsNumber(InRewardCount));
	if (P_ReceiveComplete.IsValid())
		P_ReceiveComplete->SetVisibility(IsReward ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (IMG_ActiveInner.IsValid())
		IMG_ActiveInner->SetVisibility(IsReward ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	if (IMG_ActiveOuter.IsValid())
		IMG_ActiveOuter->SetVisibility(IsReward ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	if (P_CanReceive.IsValid())
	{
		const auto CanRecieve = (!IsReward) && (UB2GameInstance::GetPlayTime() > GetRewardTime() * 60.0f);
		P_CanReceive->SetVisibility(CanRecieve ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UIConnectRewardPart::FinishRecieveAnimation()
{
	//if (P_ReceiveComplete.IsValid())
	//	P_ReceiveComplete->SetVisibility(ESlateVisibility::Visible);
	//if (IMG_ActiveInner.IsValid())
	//	IMG_ActiveInner->SetVisibility(ESlateVisibility::Collapsed);
	//if (IMG_ActiveOuter.IsValid())
	//	IMG_ActiveOuter->SetVisibility(ESlateVisibility::Collapsed);

	////ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(true);
	//FinishConnectRewardClass<>::GetInstance().Signal();
}

void UB2UIConnectRewardPart::SetReward(const bool _Reward)
{
	IsReward = _Reward;
	if (P_ReceiveComplete.IsValid())
	{
		P_ReceiveComplete->SetVisibility(ESlateVisibility::Visible);
	}
	if (Anim_Receive.IsValid())
	{
		PlayAnimation(Anim_Receive.Get());
	}
	if(IsReward)
		OnStartRewardAnimation_BP();

	SetCanReceiveEffect(false);
	ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(false);
}

