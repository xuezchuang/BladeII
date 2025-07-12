// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIEventAttendacnePart.h"
#include "B2UIManager.h"

#include "Retailer.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIDocHelper.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIEventAttendacnePart::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Image, IMG_ITEM);
	GET_SLOT(UImage, IMG_Day);
	GET_SLOT(UImage, IMG_BackGround);
	GET_SLOT(UImage, IMG_BackGroundSpecial);
	GET_SLOT(UImage, IMG_ButtonEffect);
	GET_SLOT(UB2Button, BTN_Receive);
	GET_SLOT(UTextBlock, TB_RewardCount);
	GET_SLOT(UTextBlock, TB_Day);
	GET_SLOT(UB2RichTextBlock, TB_ItemName);
	GET_SLOT(UB2RichTextBlock, TB_Receive);
	GET_SLOT(UPanelWidget, P_Complete);
	GET_SLOT(UPanelWidget, P_ReceiveButton);
	GET_SLOT(UPanelWidget, P_BackGroundSpecial);

	GET_SLOT(UTextBlock, TB_GetReward);

	Anim_Recieve = GetAnimation(this, "Anim_Recieve");
}

void UB2UIEventAttendacnePart::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_GetReward.IsValid())
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));

	UpdateGetRewardBtnText(IsDoubleReward);
}

void UB2UIEventAttendacnePart::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Receive, &UB2UIEventAttendacnePart::OnClickReceive);
}

void UB2UIEventAttendacnePart::Init()
{
	Super::Init();

	PartsType = EAttendanceTab::EventAttendance;
	RewardEventItem = nullptr;
	IsSpecialDay = false;
}

void UB2UIEventAttendacnePart::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	RewardEventItem = nullptr;
}

void UB2UIEventAttendacnePart::UpdateRewardState(EEventRewardState eRewardState, b2network::B2RewardPtr Reward)
{
	//RewardEventItem = Reward;

	//auto Func = [this](const ESlateVisibility& Receive, const ESlateVisibility& Effect, const ESlateVisibility& ReceivePanel, const bool VisibleButton)
	//{
	//	if (BTN_Receive.IsValid())
	//		BTN_Receive->SetIsEnabled(VisibleButton);
	//	if (P_Complete.IsValid())
	//		P_Complete->SetVisibility(Receive);
	//	if (IMG_ButtonEffect.IsValid())
	//		IMG_ButtonEffect->SetVisibility(Effect);
	//	if (P_ReceiveButton.IsValid())
	//		P_ReceiveButton->SetVisibility(ReceivePanel);
	//};

	//switch (eRewardState)
	//{
	//case EEventRewardState::REWARD_WAIT:
	//{
	//	Func(ESlateVisibility::Hidden, ESlateVisibility::Hidden, ESlateVisibility::SelfHitTestInvisible, false);
	//}
	//break;
	//case EEventRewardState::REWARD_ENABLE:
	//{
	//	Func(ESlateVisibility::Hidden, ESlateVisibility::HitTestInvisible, ESlateVisibility::SelfHitTestInvisible, true);
	//}
	//break;
	//case EEventRewardState::REWARD_COMPLETE:
	//{
	//	Func(ESlateVisibility::SelfHitTestInvisible, ESlateVisibility::Hidden, ESlateVisibility::Hidden, false);

	//	if(Reward && Anim_Recieve.IsValid())
	//		PlayAnimation(Anim_Recieve.Get());
	//	if(Reward)
	//		OnStartRewardAnimation_BP();
	//	ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(false);
	//}
	//break;
	//default:
	//	break;
	//}
}

void UB2UIEventAttendacnePart::InitRewardSlotInfo(const FEventAttendanceReward& RewardInfo)
{
	CachedRewardInfo = RewardInfo;
	
	if (TB_RewardCount.IsValid())
		TB_RewardCount->SetText(FText::AsNumber(RewardInfo.reward_count));
	if (IMG_ITEM.IsValid())
		IMG_ITEM->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(RewardInfo.reward_id));
	if (TB_ItemName.IsValid())
		TB_ItemName->SetText(BladeIIGameImpl::GetClientDataStore().GetRewardName(RewardInfo.reward_id));
	if (TB_Day.IsValid())
		TB_Day->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventAttendance_Day")), FText::FromString(FString::FromInt(RewardInfo.day))));

	if (IsSpecialDay)
	{
		if (IMG_BackGround.IsValid())
			IMG_BackGround->SetVisibility(ESlateVisibility::Hidden);
		if (IMG_BackGroundSpecial.IsValid())
			IMG_BackGroundSpecial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (TB_Day.IsValid())
			TB_Day->SetVisibility(ESlateVisibility::Hidden);
		if (P_BackGroundSpecial.IsValid())
			P_BackGroundSpecial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UIEventAttendacnePart::SetNewUserInfo(const FAttendanceDayData& InDayInfo)
{
	if(TB_RewardCount.IsValid())
	{
		TB_RewardCount->SetText(FText::AsNumber(InDayInfo.nRewardCount));
	}

	if(IMG_ITEM.IsValid())
	{
		IMG_ITEM->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(InDayInfo.nRewardId));
	}

	if(TB_ItemName.IsValid())
	{
		TB_ItemName->SetText(BladeIIGameImpl::GetClientDataStore().GetRewardName(InDayInfo.nRewardId));
	}

	if (TB_Day.IsValid())
	{
		TB_Day->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventAttendance_Day")), FText::FromString(FString::FromInt(InDayInfo.nDay))));
	}

	if (IsSpecialDay)
	{
		if (IMG_BackGround.IsValid())
			IMG_BackGround->SetVisibility(ESlateVisibility::Hidden);
		if (IMG_BackGroundSpecial.IsValid())
			IMG_BackGroundSpecial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (TB_Day.IsValid())
			TB_Day->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (P_BackGroundSpecial.IsValid())
			P_BackGroundSpecial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UIEventAttendacnePart::SetCachedEventInfo(const b2network::B2mdEventAttendance& rEventInfo)
{
	CachedEventInfo = rEventInfo;
}

void UB2UIEventAttendacnePart::UpdateGetRewardBtnText(const bool& IsPossibleDoubleReward)
{
	FText ReciveText;

	if (IsPossibleDoubleReward)
	{
		ReciveText = BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("Attendance_DoubleReward"));
	}
	else
	{
		ReciveText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward"));
	}

	if (TB_Receive.IsValid())
	{
		TB_Receive->SetText(ReciveText);
	}
}

void UB2UIEventAttendacnePart::FinishRecieveAnimation()
{
	//if (RewardEventItem)
	//{
	//	if (!UB2UIManager::GetInstance()->OpenRewardMailPopUp(RewardEventItem))
	//	{
	//		if (auto* PopUp = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
	//		{
	//			TArray<b2network::B2RewardPtr> Items;
	//			Items.Add(RewardEventItem);
	//			PopUp->AddRewardItems(Items, true);
	//		}
	//	}

	//	ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(true);

	//	RewardEventItem = nullptr;
	//}
}

void UB2UIEventAttendacnePart::OnClickAttendanceEvent()
{
	bool IsPopupOpen = false;

	switch (CachedEventInfo.bonus_type)
	{
	case EEventBonusType::NORMAL:
	{
	}break;
	case EEventBonusType::DOUBLE:
	{
		if (IsDoubleReward == false)
		{
			IsPopupOpen = true;
		}
	}break;
	}

	if (IsPopupOpen)
	{
		OpenPopupBuyPackageOrRequestReward();
	}
	else
	{
		data_trader::Retailer::GetInstance().RequestEventAttendance(CachedEventInfo.event_id);
	}
}

void UB2UIEventAttendacnePart::OnClickNewUserEvent()
{
	data_trader::Retailer::GetInstance().RequestBeginnerAttendance();
}

void UB2UIEventAttendacnePart::OnClickReceive()
{
	switch (PartsType)
	{
	case EAttendanceTab::EventAttendance:
		OnClickAttendanceEvent();
		break;
	case EAttendanceTab::EventNewUser:
		OnClickNewUserEvent();
		break;
	default:
		break;
	}
}

void UB2UIEventAttendacnePart::OpenPopupBuyPackageOrRequestReward()
{
	UB2UIMsgPopupSimple* pPopupUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(
		EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("Attendance_Description")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]()	//yes
		{
			EnterPackageShop();
		}),
		FMsgPopupOnClick::CreateLambda([this]()	//no
		{
			data_trader::Retailer::GetInstance().RequestEventAttendance(CachedEventInfo.event_id);
		}));

	if (pPopupUI)
	{
		pPopupUI->SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Buy")));
		pPopupUI->SetButtonText(EUIMsgPopupButton::Negative, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));
	}
}

void UB2UIEventAttendacnePart::EnterPackageShop()
{
	//if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	//{
	//	const int32 FlatratePackageTabIndex = 7;
	//	DocStore->SetPackageTabToOpen(FlatratePackageTabIndex);
	//	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::PackageStore));
	//}
}