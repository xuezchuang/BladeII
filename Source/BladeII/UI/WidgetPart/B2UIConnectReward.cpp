// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIConnectReward.h"
#include "B2UIManager.h"
#include "B2UIConnectRewardPart.h"

#include "BladeIIGameMode.h"
#include "B2UIMsgPopupReward.h"
#include "B2ClientDataStore.h"
#include "Retailer.h"
#include "B2GameInstance.h"


#include "BladeIIGameImpl.h"

void UB2UIConnectReward::CacheAssets()
{
	Super::CacheAssets();

	//if (ConnectRewardParts.Num() == 0)
	//{
	//	ConnectRewardParts.SetNum(6);
	//	GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[0], UIP_ConnectRewardPart0);
	//	GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[1], UIP_ConnectRewardPart1);
	//	GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[2], UIP_ConnectRewardPart2);
	//	GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[3], UIP_ConnectRewardPart3);
	//	GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[4], UIP_ConnectRewardPart4);
	//	GET_SLOT_BYNAME(UB2UIConnectRewardPart, ConnectRewardParts[5], UIP_ConnectRewardPart5);

	//	GET_SLOT(UB2Button, BTN_Recieve);
	//	GET_SLOT(UTextBlock, TB_Receive);
	//	GET_SLOT(UImage, IMG_ProgressBar);
	//	GET_SLOT(UImage, IMG_ButtonEffect);
	//	GET_SLOT(UB2RichTextBlock, TB_TimeMessage);
	//	GET_SLOT(UB2RichTextBlock, TB_CumulativeTime);
	//}
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
	//UnsubscribeEvents();

	//Issues.Add(ReceivePlayTimeRewardClass<const TArray<b2network::B2RewardPtr>&>::GetInstance().Subscribe2(
	//	[this](const TArray<b2network::B2RewardPtr>& Rewards)
	//{
	//	this->StartReward(Rewards);
	//}
	//));

	//Issues.Add(FinishConnectRewardClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->FinishRewardAnim();
	//}
	//));

	//Issues.Add(FirstRequestUpdateDailyPlayTimeClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->UpdateRewardState();
	//	this->RenderProgressBar(UB2GameInstance::GetPlayTime());
	//	UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
	//}
	//));
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
 焊惑付促 矫埃 埃拜捞 老沥窍搁 泅犁矫埃 / 弥辆矫埃 = Progress官狼 胶纳老肺 窍搁登绰单
 立加 焊惑狼 急拱捞 6俺扼绊 摹搁, 阿 焊惑付促狼 矫埃 埃拜捞 崔扼辑 贸府甫 蝶肺 秦拎具茄促.
 霉锅掳绰 3盒俊 1/6捞 瞒具窍绊, 付瘤阜篮 60盒悼救 1/6捞 瞒具茄促.
  焊惑捞 6俺扼绊 啊沥窍绊 付瘤阜 60盒阑 盲款促绊 窍搁
 ProgressBar = 5/6 + ((5锅犁何磐 矫累茄 穿利矫埃 / 5锅掳~6锅掳 埃拜) * 1/6)
 焊惑捞 6俺扼绊 啊沥窍绊 贸澜 3盒阑 盲款促绊 窍搁
 ProgressBar = 0/6 + ((0锅犁何磐 矫累茄 穿利矫埃 / 0锅掳~1锅掳 埃拜) * 1/6)
*/
void UB2UIConnectReward::RenderProgressBar(const float PlayTime)
{
	int32 PlayTimeToInt = 0;
	float ProgressBar = 0.0f;

	if (MaxRewardPlayTime * 60.0f > PlayTime)
	{
		auto AddProgressRatio = 1.0f;       //  6俺吝 5锅掳 焊惑 吝捞搁 4 / 6
		auto GoalTime = 0.0f;               //  捞傈 鳖瘤 钦魂 茄 矫埃阑 力寇 窍绊, 瘤陛 沫俊辑 1捞 登扁 困茄 矫埃
		auto CalculatedPlayTime = PlayTime; //  醚矫埃 - 捞傈沫 鳖瘤狼 矫埃

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
	//if (BTN_Recieve.IsValid() && IsWatingReward == false)
	//{
	//	if (BTN_Recieve->GetIsEnabled() && !IsNewReward())
	//	{
	//		BTN_Recieve->SetIsEnabled(false);
	//		if (IMG_ButtonEffect.IsValid())
	//			IMG_ButtonEffect->SetVisibility(ESlateVisibility::Collapsed);
	//		UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
	//	}
	//	else if (!BTN_Recieve->GetIsEnabled() && IsNewReward())
	//	{
	//		BTN_Recieve->SetIsEnabled(true);
	//		if (IMG_ButtonEffect.IsValid())
	//			IMG_ButtonEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
	//		UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
	//	}
	//}
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
	//if (Rewards.Num() > 0)
	//{
	//	if (!UB2UIManager::GetInstance()->OpenRewardMailPopUp(Rewards))
	//	{
	//		if (auto PopUp = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
	//		{
	//			PopUp->AddRewardItems(Rewards, true);
	//		}
	//	}
	//	UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
	//}
}

TWeakObjectPtr<UB2UIConnectRewardPart>* UB2UIConnectReward::FindRewardItem(const b2network::B2RewardPtr& RewardItem)
{
	return ConnectRewardParts.FindByPredicate([&RewardItem](TWeakObjectPtr<UB2UIConnectRewardPart>& RewardPart)
	{    // 救罐篮 惑怕绊, 鉴辑啊 鞍绊(1锅掳, 3锅掳啊 鞍篮 焊惑老 版快 贸府), 酒捞袍 Index啊 鞍阑 锭
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
	//SavedRewards = Rewards;

	//for (int32 i = 0; i < Rewards.Num(); ++i)
	//{
	//	if (Rewards[i] == nullptr)
	//	{
	//		continue;
	//	}

	//	if (auto* PartReward = FindRewardItem(Rewards[i]))
	//	{
	//		(*PartReward)->SetReward(true);
	//		BTN_Recieve->SetIsEnabled(false);
	//		if (IMG_ButtonEffect.IsValid())
	//			IMG_ButtonEffect->SetVisibility(ESlateVisibility::Collapsed);
	//		//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
	//		return;
	//	}
	//}

	//IsWatingReward = false;;

	//if (SavedRewards.Num() > 0)
	//{
	//	PopUpConnectReward(SavedRewards);
	//	SavedRewards.Empty();
	//}
	//
	//CheckRewardState();
	//ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(true);
	//DoMarkCombineRedDotClass<>::GetInstance().Signal();
}

void UB2UIConnectReward::OnClickBTN_Recieve()
{
	if (IsWatingReward)
		return;

	IsWatingReward = true;
	// 悼扁拳茄瘤 坷贰灯栏搁 矫埃 悼扁拳 夸没何磐
	if ((UB2GameInstance::GetUTCNow() - UB2GameInstance::GetPlayTimeAfterRequest()).GetTotalSeconds() > 1.1)
	{
		BladeIIGameImpl::GetClientDataStore().SetConnectRewardButtonPush(true);
		UB2GameInstance::RequestUpdateDailyPlayTime();
	}
	// 酒聪搁 官肺 Reward夸没
	else
	{
		data_trader::Retailer::GetInstance().RequestReceiveDailyPlayTimeReward();
	}

}



