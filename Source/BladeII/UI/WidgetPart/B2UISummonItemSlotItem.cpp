// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UISummonItemSlotItem.h"
#include "B2UIManager.h"
#include "B2UIUtil.h"
#include "BladeIIGameImpl.h"

#include "B2UIStoreEventLabel.h"
#include "B2UIDocHelper.h"
#include "BladeIIGameImpl.h"
#include "B2UIShortagePopupManager.h"
#include "CommonStruct.h"


void UB2UISummonItemSlotItem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);

	GET_SLOT(UOverlay, OV_GemBox);
	GET_SLOT(UOverlay, OV_FriendShipBox);

	GET_SLOT(UOverlay, OV_EableMileage);
	GET_SLOT(UTextBlock, TB_EableMileage);

	GET_SLOT(UTextBlock, TB_MinGrade);
	GET_SLOT(UTextBlock, TB_MaxGrade);
	GET_SLOT(UTextBlock, TB_EnableGrade);

	GET_SLOT(UTextBlock, TB_LotteryNotice);

	if (SummonButtonArray.Num() == 0)
	{
		SummonButtonArray.SetNum(static_cast<int32>(ESummonButtonType::Max));

		GET_SLOT_BYNAME(UB2UIWidgetBase, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].UIP_RedDot, UIP_TenRedDot);
		GET_SLOT_BYNAME(UB2UIWidgetBase, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].UIP_RedDot, UIP_OneRedDot);

		GET_SLOT_BYNAME(UTextBlock, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].TB_Summon, TB_TenSummon);
		GET_SLOT_BYNAME(UTextBlock, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].TB_Summon, TB_OneSummon);

		GET_SLOT_BYNAME(UB2RichTextBlock, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].RTB_BuyCount, RTB_TenBuyCount);
		GET_SLOT_BYNAME(UB2RichTextBlock, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].RTB_BuyCount, RTB_OneBuyCount);

		GET_SLOT_BYNAME(UTextBlock, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].TB_SummonDesc, TB_TenSummonDesc);
		GET_SLOT_BYNAME(UTextBlock, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].TB_SummonDesc, TB_OneSummonDesc);

		GET_SLOT_BYNAME(UB2Button, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].BTN_Summon, BTN_TenSummon);
		GET_SLOT_BYNAME(UB2Button, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].BTN_Summon, BTN_OneSummon);

		GET_SLOT_BYNAME(UB2UIStoreProductCost, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].UIP_Cost, UIP_TenCost);
		if (SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].UIP_Cost.IsValid())
			SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].UIP_Cost->Init();
		GET_SLOT_BYNAME(UB2UIStoreProductCost, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].UIP_Cost, UIP_OneCost);
		if (SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].UIP_Cost.IsValid())
			SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].UIP_Cost->Init();

		GET_SLOT_BYNAME(UB2UIStoreEventLabel, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].UIP_Store_Event_Label, UIP_Store_TenEvent_Label);
		if (SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].UIP_Store_Event_Label.IsValid())
			SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].UIP_Store_Event_Label->Init();
		GET_SLOT_BYNAME(UB2UIStoreEventLabel, SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].UIP_Store_Event_Label, UIP_Store_OneEvent_Label);
		if (SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].UIP_Store_Event_Label.IsValid())
			SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].UIP_Store_Event_Label->Init();
	}
}

void UB2UISummonItemSlotItem::UpdateStaticText()
{
	if (TB_EableMileage.IsValid())
		TB_EableMileage->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_EnableMileage")));

	if (TB_LotteryNotice.IsValid())
		TB_LotteryNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_ExceptionItem")));

	if (SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].TB_Summon.IsValid())
		SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].TB_Summon->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_TenSummon")));
	if (SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].TB_Summon.IsValid())
		SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].TB_Summon->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_OneSummon")));
}

void UB2UISummonItemSlotItem::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_Ten)].BTN_Summon, &UB2UISummonItemSlotItem::OnClickBTN_TenSummon);
	BIND_CLICK_FUNC_TO_BTN(SummonButtonArray[static_cast<int32>(ESummonButtonType::SummonButtonType_One)].BTN_Summon, &UB2UISummonItemSlotItem::OnClickBTN_OneSummon);
}

void UB2UISummonItemSlotItem::Init()
{
	Super::Init();

	bStartCountTime = false;
}

void UB2UISummonItemSlotItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateLeftTime();
}

void UB2UISummonItemSlotItem::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UISummonItemSlotItem::UpdateLeftTime()
{
	if (!bStartCountTime)
		return;

	for (int Idx = 0; Idx < SummonButtonArray.Num(); Idx++)
	{
		auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SummonButtonArray[Idx].SlotHashKey);
		int64 LeftTime = Data ? Data->GetNextFreeTimeLeftSeconds() : 0;
		SummonButtonArray[Idx].LeftTime = LeftTime;

		SetFreeLeftTime();
	}
}

void UB2UISummonItemSlotItem::UpdateRedDot()
{
	for (int Idx = 0; Idx < SummonButtonArray.Num(); Idx++)
	{
		if (SummonButtonArray[Idx].UIP_RedDot.IsValid())
		{
			bool bIsFree = IsFreeSummonableSlot(SummonButtonArray[Idx].SlotHashKey);
			SummonButtonArray[Idx].UIP_RedDot->SetVisibility(bIsFree ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

void UB2UISummonItemSlotItem::SetSummonItemSlot(ESummonItemCost InCostType)
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, InCostType == ESummonItemCost::Gem ? TEXT("Store_SummonItem_GemBox") : TEXT("Store_SummonItem_FriendShipBox")));

	if (OV_GemBox.IsValid())
		OV_GemBox->SetVisibility(InCostType == ESummonItemCost::Gem ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	if (OV_FriendShipBox.IsValid())
		OV_FriendShipBox->SetVisibility(InCostType == ESummonItemCost::Gem ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

	if (OV_EableMileage.IsValid())
		OV_EableMileage->SetVisibility(InCostType == ESummonItemCost::Gem ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UISummonItemSlotItem::SetGradeRange(int32 InMin, int32 InMax)
{
	if (TB_MinGrade.IsValid())
		TB_MinGrade->SetText(FText::AsNumber(InMin));
	if (TB_MaxGrade.IsValid())
		TB_MaxGrade->SetText(FText::AsNumber(InMax));
	if (TB_EnableGrade.IsValid())
		TB_EnableGrade->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_EnableGrade")));
}

void UB2UISummonItemSlotItem::SetSummonItem(ESummonButtonType InButtonType, int32 InSlotHashKey, int64 InLeftTime)
{
	SummonButtonArray[static_cast<int32>(InButtonType)].SummonButtonType = InButtonType;
	SummonButtonArray[static_cast<int32>(InButtonType)].SlotHashKey = InSlotHashKey;
	SummonButtonArray[static_cast<int32>(InButtonType)].LeftTime = InLeftTime;
}

void UB2UISummonItemSlotItem::SetSummonButtonItem(ESummonButtonType InButtonType, ESummonItemCost InCostType, int32 InCostCount, int32 InCalculatedCost, int32 InLimitCount, int32 InLimitMaxCount, bool IsTenLotteryTicket)
{
	EStoreItemCost StoreItemCost = EStoreItemCost::End;
	if (IsTenLotteryTicket)
	{
		// 10회 뽑기권
		StoreItemCost = EStoreItemCost::TenLotteryTicket;
	}
	else
	{
		StoreItemCost = (InCostType == ESummonItemCost::Gem) ? EStoreItemCost::Gem : EStoreItemCost::FriendshipPoint;
	}

	int32 CostCount = IsTenLotteryTicket ? 1 : InCostCount;
	int32 CalCost = IsTenLotteryTicket ? 0 : InCalculatedCost;

	if (SummonButtonArray[static_cast<int32>(InButtonType)].UIP_Cost.IsValid())
	{
		SummonButtonArray[static_cast<int32>(InButtonType)].UIP_Cost->SetCost(StoreItemCost, CostCount, CalCost);
		SummonButtonArray[static_cast<int32>(InButtonType)].UIP_Cost->SetSummonDesc(InButtonType);
	}

	if (SummonBuyInfo.Contains(InButtonType) == false)
	{
		SummonBuyInfo.Add(InButtonType);
	}

	SummonBuyInfo[InButtonType].StoreItemCost = StoreItemCost;
	SummonBuyInfo[InButtonType].CostCount = CostCount;
	SummonBuyInfo[InButtonType].CalCost = CalCost;

	SetLimitCount(InButtonType, InLimitCount, InLimitMaxCount);
}

void UB2UISummonItemSlotItem::SetLimitCount(ESummonButtonType InButtonType, int32 InLimitCount, int32 InMaxCount)
{
	if (SummonButtonArray[static_cast<int32>(InButtonType)].RTB_BuyCount.IsValid())
	{
		SummonButtonArray[static_cast<int32>(InButtonType)].RTB_BuyCount->SetVisibility(InMaxCount ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		SummonButtonArray[static_cast<int32>(InButtonType)].RTB_BuyCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_BuyCount")), FText::AsNumber(InMaxCount - InLimitCount), FText::AsNumber(InMaxCount)));
	}
}

void UB2UISummonItemSlotItem::SetVisibleFreeDesc(ESummonButtonType InButtonType, bool bVisible)
{
	if (SummonButtonArray[static_cast<int32>(InButtonType)].TB_SummonDesc.IsValid())
	{
		SummonButtonArray[static_cast<int32>(InButtonType)].TB_SummonDesc->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UISummonItemSlotItem::SetLotterySale(ESummonButtonType InButtonType)
{
	if (SummonButtonArray[static_cast<int32>(InButtonType)].UIP_Store_Event_Label.IsValid())
	{
		SummonButtonArray[static_cast<int32>(InButtonType)].UIP_Store_Event_Label->SetFundsSaleForSummon();
	}
}

void UB2UISummonItemSlotItem::StartFreeLeftTime(bool ToToggle)
{
	if (SummonButtonArray.Num() == ESummonButtonType::Max)
	{
		bStartCountTime = ToToggle;

		UpdateRedDot();
	}
}

void UB2UISummonItemSlotItem::SetFreeLeftTime()
{
	for (int Idx = 0; Idx < SummonButtonArray.Num(); Idx++)
	{
		int64 LeftTime = SummonButtonArray[Idx].LeftTime;
		if (SummonButtonArray[Idx].TB_SummonDesc.IsValid())
		{
			if (LeftTime == 0)
			{
				SummonButtonArray[Idx].TB_SummonDesc->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				SummonButtonArray[Idx].TB_SummonDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
				SummonButtonArray[Idx].TB_SummonDesc->SetText(GetLeftTimeText(LeftTime));
			}
		}
	}
}

FText UB2UISummonItemSlotItem::GetLeftTimeText(int64 Seconds)
{
	const FDateTime& LeftTime = FDateTime::FromUnixTimestamp(Seconds);

	if (Seconds < 60)
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_Free_Sec")), FText::AsNumber(LeftTime.GetSecond()));
	}
	else if (Seconds < 3600)
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_Free_MinSec")), FText::AsNumber(LeftTime.GetMinute()), FText::AsNumber(LeftTime.GetSecond()));
	}
	else if (Seconds < 86400)
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_Free_HourMinSec"))
			, FText::AsNumber(LeftTime.GetHour()), FText::AsNumber(LeftTime.GetMinute()), FText::AsNumber(LeftTime.GetSecond()));
	}

	return FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_Free_DayHourMin"))
		, FText::AsNumber(LeftTime.GetDay()), FText::AsNumber(LeftTime.GetHour()), FText::AsNumber(LeftTime.GetMinute()));
}

bool UB2UISummonItemSlotItem::IsFreeSummonableSlot(int32 SlotHashKey)
{
	if (auto* SummonData = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SlotHashKey))
	{
		if (SummonData->IsMultiple() && SummonData->GetCostType() == int32(ESummonItemCost::Gem))
		{
			// 보석 10개짜리는 Free가 없음
			return false;
		}
		else
		{
			int64 FreeSummonLeftTime = SummonData->GetNextFreeTimeLeftSeconds();
			if (FreeSummonLeftTime == 0)
			{
				return true;
			}
		}
	}

	return false;
}

void UB2UISummonItemSlotItem::OnClickBTN_TenSummon()
{
	const ESummonButtonType SummonButtonType = ESummonButtonType::SummonButtonType_Ten;
	int32 HashKey = SummonButtonArray[SummonButtonType].SlotHashKey;
	bool bUseTenLotteryTicket = (BladeIIGameImpl::GetClientDataStore().GetShopTenLotteryTicket() > 0) && 
								(SummonBuyInfo[SummonButtonType].StoreItemCost == EStoreItemCost::TenLotteryTicket);
	
	if (B2UIUtil::IsLimitedBuySummonSlot(HashKey))
		return;

	if (SummonBuyInfo.Contains(SummonButtonType))
	{
		if (IsSummonItemBuy(SummonBuyInfo[SummonButtonType]))
		{
			SelectCharacterToSummonItemClass<int32, bool>::GetInstance().Signal(HashKey, bUseTenLotteryTicket);
		}
		else
		{
			OpenErrorPopup(SummonBuyInfo[SummonButtonType].StoreItemCost);
		}
	}
}

void UB2UISummonItemSlotItem::OnClickBTN_OneSummon()
{
	const ESummonButtonType SummonButtonType = ESummonButtonType::SummonButtonType_One;
	int32 HashKey = SummonButtonArray[SummonButtonType].SlotHashKey;

	if (B2UIUtil::IsLimitedBuySummonSlot(HashKey))
		return;

	if (SummonBuyInfo.Contains(SummonButtonType))
	{
		if (IsSummonItemBuy(SummonBuyInfo[SummonButtonType]))
		{
			SelectCharacterToSummonItemClass<int32, bool>::GetInstance().Signal(HashKey, false);
		}
		else
		{
			OpenErrorPopup(SummonBuyInfo[SummonButtonType].StoreItemCost);
		}
	}
}

void UB2UISummonItemSlotItem::OpenErrorPopup(EStoreItemCost InCostType)
{
	switch (InCostType)
	{
	case EStoreItemCost::DarkCrystal:
		UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGotoShopForDarkCrystalMessage();
		break;
	case EStoreItemCost::FriendshipPoint:
		UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGotoFriendForFriendShip();
		break;
	case EStoreItemCost::Gem:
		UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGoToShopForGemMessage();
		break;
	default:
		break;
	}
}

bool UB2UISummonItemSlotItem::IsSummonItemBuy(FSummonBuyInfo& InSummonBuyInfo)
{
	bool IsBuy = true;

	if (InSummonBuyInfo.CostCount != 0)
	{
		int32 UserCost = 0;
		if (auto UserInfo = UB2UIDocHelper::GetDocUser())
		{
			switch (InSummonBuyInfo.StoreItemCost)
			{
			case EStoreItemCost::Gem:
				UserCost = UserInfo->GetGem();
				break;
			case EStoreItemCost::DarkCrystal:
				UserCost = UserInfo->GetDarkCrystal();
				break;
			case EStoreItemCost::FriendshipPoint:
				UserCost = UserInfo->GetSocialPoint();
				break;
			case EStoreItemCost::TenLotteryTicket:
				UserCost = BladeIIGameImpl::GetClientDataStore().GetShopTenLotteryTicket();
				break;
			default:
				break;
			}
		}

		if (InSummonBuyInfo.CalCost > 0 && InSummonBuyInfo.CalCost > UserCost)
		{
			IsBuy = false;
		}
		else if (
				(InSummonBuyInfo.StoreItemCost == EStoreItemCost::DarkCrystal ||
				InSummonBuyInfo.StoreItemCost == EStoreItemCost::FriendshipPoint ||
				InSummonBuyInfo.StoreItemCost == EStoreItemCost::Gem)
				&&
				InSummonBuyInfo.CostCount > UserCost
			)
		{
			IsBuy = false;
		}
	}
	return IsBuy;
}
