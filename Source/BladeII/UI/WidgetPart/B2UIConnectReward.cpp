// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIConnectRewardPart.h"
#include "B2UIConnectReward.h"
#include "BladeIIGameMode.h"
#include "B2UIMsgPopupReward.h"
#include "B2ClientDataStore.h"
#include "Retailer.h"
#include "B2GameInstance.h"


#include "BladeIIGameImpl.h"

void UB2UIConnectReward::CacheAssets()
{
	Super::CacheAssets();

	if (ConnectRewardParts.Num() == 0)
	{
		ConnectRewardParts.SetNum(6);
		GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[0], UIP_ConnectRewardPart0);
		GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[1], UIP_ConnectRewardPart1);
		GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[2], UIP_ConnectRewardPart2);
		GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[3], UIP_ConnectRewardPart3);
		GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[4], UIP_ConnectRewardPart4);
		GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[5], UIP_ConnectRewardPart5);

		GET_SLOT(UB2Button, BTN_Recieve);
		GET_SLOT(UTextBlock, TB_Receive);
		GET_SLOT(UImage, IMG_ProgressBar);
		GET_SLOT(UImage, IMG_ButtonEffect);
		GET_SLOT(UB2RichTextBlock, TB_TimeMessage);
		GET_SLOT(UB2RichTextBlock, TB_CumulativeTime);
	}
}

void UB2UIConnectReward::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Recieve, &UB2UIConnectReward::OnClickBTN_Recieve);
}

void UB2UIConnectReward::UpdateStaticText()
{
	if (TB_TimeMessage.IsValid())
		TB_TimeMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AccessReward_Time")));
}

void UB2UIConnectReward::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(ReceivePlayTimeRewardClass<const TArray<b2network::B2RewardPtr>&>::GetInstance().Subscribe2(
		[this](const TArray<b2network::B2RewardPtr>& Rewards)
	{
		this->StartReward(Rewards);
	}
	));

	Issues.Add(FinishConnectRewardClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->FinishRewardAnim();
	}
	));

	Issues.Add(FirstRequestUpdateDailyPlayTimeClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->UpdateRewardState();
		this->RenderProgressBar(UB2GameInstance::GetPlayTime());
		UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
	}
	));
}

void UB2UIConnectReward::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIConnectReward::Init()
{
	Super::Init();

	SubscribeEvents();

	InitConnectRewardPart();
	UpdateRewardState();

	if (TB_Receive.IsValid())
		TB_Receive->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));
	if(BTN_Recieve.IsValid())
		BTN_Recieve->SetIsEnabled(true);
	if (IMG_ButtonEffect.IsValid())
		IMG_ButtonEffect->SetVisibility(ESlateVisibility::HitTestInvisible);

	CalculateProgressBar = true;
	IsWatingReward		 = false;

}

void UB2UIConnectReward::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (auto ConnectPart : ConnectRewardParts)
	{
		ConnectPart->DestroySelf(InUIManager);
	}
}

/*
 ���󸶴� �ð� ������ �����ϸ� ����ð� / �����ð� = Progress���� �����Ϸ� �ϸ�Ǵµ�
 ���� ������ ������ 6����� ġ��, �� ���󸶴��� �ð� ������ �޶� ó���� ���� ������Ѵ�.
 ù��°�� 3�п� 1/6�� �����ϰ�, �������� 60�е��� 1/6�� �����Ѵ�.
  ������ 6����� �����ϰ� ������ 60���� ä��ٰ� �ϸ�
 ProgressBar = 5/6 + ((5������� ������ �����ð� / 5��°~6��° ����) * 1/6)
 ������ 6����� �����ϰ� ó�� 3���� ä��ٰ� �ϸ�
 ProgressBar = 0/6 + ((0������� ������ �����ð� / 0��°~1��° ����) * 1/6)
*/
void UB2UIConnectReward::RenderProgressBar(const float PlayTime)
{
	int32 PlayTimeToInt = 0;
	float ProgressBar = 0.0f;

	if (MaxRewardPlayTime * 60.0f > PlayTime)
	{
		auto AddProgressRatio = 1.0f;       //  6���� 5��° ���� ���̸� 4 / 6
		auto GoalTime = 0.0f;               //  ���� ���� �ջ� �� �ð��� ���� �ϰ�, ���� ĭ���� 1�� �Ǳ� ���� �ð�
		auto CalculatedPlayTime = PlayTime; //  �ѽð� - ����ĭ ������ �ð�

		const auto ONE_MINUTE = 60.0f;
		const auto RewardCount = ConnectRewardParts.Num();

		for (auto i = RewardCount - 1; i >= 0; --i)
		{
			float BeforeRewardTime = (ConnectRewardParts[i]->GetRewardTime() - ConnectRewardParts[i]->GetInterval()) * ONE_MINUTE;
			if (CalculatedPlayTime >= BeforeRewardTime)
			{
				AddProgressRatio = static_cast<float>(i) / RewardCount;
				GoalTime = ConnectRewardParts[i]->GetInterval();
				CalculatedPlayTime -= BeforeRewardTime;
				break;
			}
		}

		ProgressBar = AddProgressRatio + (CalculatedPlayTime / (GoalTime * ONE_MINUTE)) * 1.0f / RewardCount;
		PlayTimeToInt = static_cast<int32>((PlayTime / ONE_MINUTE));
		CalculateProgressBar = true;
	}
	else
	{
		ProgressBar = 1.0f;
		PlayTimeToInt = static_cast<int32>(MaxRewardPlayTime);
		CalculateProgressBar = false;
		CheckRewardState();
	}

	if(IMG_ProgressBar.IsValid())
		IMG_ProgressBar->SetRenderScale(FVector2D(ProgressBar, 1.0f));
	if(TB_CumulativeTime.IsValid())
		TB_CumulativeTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::FromString(FString::FromInt(MaxRewardPlayTime < PlayTimeToInt ? MaxRewardPlayTime : PlayTimeToInt))));

}

void UB2UIConnectReward::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	if (CalculateProgressBar)
	{
		RenderProgressBar(UB2GameInstance::GetPlayTime());
	}
	CheckRewardState();
}


bool UB2UIConnectReward::IsNewReward() const
{
	bool IsNew = false;

	for (auto& RewardPart : ConnectRewardParts)
	{
		if (!RewardPart->GetReward() && UB2GameInstance::GetPlayTime() > RewardPart->GetRewardTime() * 60.0f)
		{
			RewardPart->SetCanReceiveEffect(true);
			IsNew = true;
		}

	}
	return IsNew;
}

void UB2UIConnectReward::CheckRewardState()
{
	if (BTN_Recieve.IsValid() && IsWatingReward == false)
	{
		if (BTN_Recieve->GetIsEnabled() && !IsNewReward())
		{
			BTN_Recieve->SetIsEnabled(false);
			if (IMG_ButtonEffect.IsValid())
				IMG_ButtonEffect->SetVisibility(ESlateVisibility::Collapsed);
			UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
		}
		else if (!BTN_Recieve->GetIsEnabled() && IsNewReward())
		{
			BTN_Recieve->SetIsEnabled(true);
			if (IMG_ButtonEffect.IsValid())
				IMG_ButtonEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
			UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
		}
	}
}

void UB2UIConnectReward::InitConnectRewardPart()
{
	for (int32 i = 0; i < ConnectRewardParts.Num(); ++i)
	{
		if (ConnectRewardParts[i].IsValid())
			ConnectRewardParts[i]->Init();
	}
}

void UB2UIConnectReward::UpdateRewardState()
{
	int BeforeTime = 0;
	const auto& RewardInfo = BladeIIGameImpl::GetClientDataStore().GetPlayTimeRewardData();
	const auto& PlayTimeStatus = BladeIIGameImpl::GetClientDataStore().GetPlayTimeStatus();

	for (int32 i = 0; i < ConnectRewardParts.Num(); ++i)
	{
		if (!RewardInfo.IsValidIndex(i) || !ConnectRewardParts[i].IsValid())
			continue;

		ConnectRewardParts[i]->SetRewardState(RewardInfo[i].reward_id, RewardInfo[i].play_time_inmin, RewardInfo[i].play_time_inmin - BeforeTime
			, RewardInfo[i].reward_count, i < PlayTimeStatus.daily_play_time_reward_index, i + 1);

		BeforeTime = RewardInfo[i].play_time_inmin;
		MaxRewardPlayTime = RewardInfo[i].play_time_inmin;
	}
}

void UB2UIConnectReward::PopUpConnectReward(const TArray<b2network::B2RewardPtr>& Rewards)
{
	if (Rewards.Num() > 0)
	{
		if (!UB2UIManager::GetInstance()->OpenRewardMailPopUp(Rewards))
		{
			if (auto PopUp = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
			{
				PopUp->AddRewardItems(Rewards, true);
			}
		}
		UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
	}
}

TWeakObjectPtr<UB2UIConnectRewardPart>* UB2UIConnectReward::FindRewardItem(const b2network::B2RewardPtr& RewardItem)
{
	return ConnectRewardParts.FindByPredicate([&RewardItem](TWeakObjectPtr<UB2UIConnectRewardPart>& RewardPart)
	{    // �ȹ��� ���°�, ������ ����(1��°, 3��°�� ���� ������ ��� ó��), ������ Index�� ���� ��
		if (!RewardPart->GetReward() && RewardPart->GetRewardItemIndex() == RewardItem->raw_reward->id)
			return true;
		return false;
	});
}

void UB2UIConnectReward::FinishRewardAnim()
{
	StartReward(SavedRewards);
	//if (SavedRewards.Num() > 0)
	//{
	//	PopUpConnectReward(SavedRewards);
	//	SavedRewards.Empty();
	//}
}

void UB2UIConnectReward::StartReward(const TArray<b2network::B2RewardPtr>& Rewards)
{
	SavedRewards = Rewards;

	for (int32 i = 0; i < Rewards.Num(); ++i)
	{
		if (Rewards[i] == nullptr)
		{
			continue;
		}

		if (auto* PartReward = FindRewardItem(Rewards[i]))
		{
			(*PartReward)->SetReward(true);
			BTN_Recieve->SetIsEnabled(false);
			if (IMG_ButtonEffect.IsValid())
				IMG_ButtonEffect->SetVisibility(ESlateVisibility::Collapsed);
			//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
			return;
		}
	}

	IsWatingReward = false;;

	if (SavedRewards.Num() > 0)
	{
		PopUpConnectReward(SavedRewards);
		SavedRewards.Empty();
	}
	
	CheckRewardState();
	ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(true);
	DoMarkCombineRedDotClass<>::GetInstance().Signal();
}

void UB2UIConnectReward::OnClickBTN_Recieve()
{
	if (IsWatingReward)
		return;

	IsWatingReward = true;
	// ����ȭ���� ���������� �ð� ����ȭ ��û����
	if ((UB2GameInstance::GetUTCNow() - UB2GameInstance::GetPlayTimeAfterRequest()).GetTotalSeconds() > 1.1)
	{
		BladeIIGameImpl::GetClientDataStore().SetConnectRewardButtonPush(true);
		UB2GameInstance::RequestUpdateDailyPlayTime();
	}
	// �ƴϸ� �ٷ� Reward��û
	else
	{
		data_trader::Retailer::GetInstance().RequestReceiveDailyPlayTimeReward();
	}

}



