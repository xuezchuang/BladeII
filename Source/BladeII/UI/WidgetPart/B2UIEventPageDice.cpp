// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPageDice.h"

#include "RewardEventManager.h"
#include "B2UIEventSlotDice.h"
#include "B2UIDiceEventMain.h"
#include "B2UIDiceEventExplain.h"
#include "B2UIDiceEventMileage.h"
#include "B2UIManager.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
#include "B2ClientDataStore.h"

void UB2UIDiceEventRollEffect::InitDiceEventRollEffect(TWeakObjectPtr<class UB2UIEventPageDice> pWidget)
{
	pEventPageDice = pWidget;
	this->Init();
	this->AddToViewport(iDiceEventRollEffectZOrder_BP);
}

void UB2UIDiceEventRollEffect::EndAnimRollEffect_BP()
{
	if (pEventPageDice.IsValid())
	{
		pEventPageDice->UpdateDiceEventPageAfterRollDiceAnim();
	}
}



void UB2UIEventPageDice::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UIP_DiceEventMain.IsValid())
	{
		UIP_DiceEventMain->DestroySelf(InUIManager);
	}

	if (UIP_DiceEventExplain.IsValid())
	{
		UIP_DiceEventExplain->DestroySelf(InUIManager);
	}

	if (UIP_DiceEventMileage.IsValid())
	{
		UIP_DiceEventMileage->DestroySelf(InUIManager);
	}

	if (pDiceEventRollEffect.IsValid())
	{
		pDiceEventRollEffect->DestroySelf(InUIManager);
		pDiceEventRollEffect = nullptr;
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIEventPageDice::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIDiceEventMain, UIP_DiceEventMain);
	if (UIP_DiceEventMain.IsValid())
	{
		UIP_DiceEventMain->Init();
	}

	GET_SLOT(UB2UIDiceEventExplain, UIP_DiceEventExplain);
	if (UIP_DiceEventExplain.IsValid())
	{
		UIP_DiceEventExplain->Init();
	}

	GET_SLOT(UB2UIDiceEventMileage, UIP_DiceEventMileage);
	if (UIP_DiceEventMileage.IsValid())
	{
		UIP_DiceEventMileage->Init();
	}

	CacheAssetsEventSlotDiceCell();

	GET_SLOT(UB2Button, BTN_BuyDicePoint);
	GET_SLOT(UTextBlock, TB_BtnBuyDicePoint);
	GET_SLOT(UTextBlock, TB_NpcDialog);

	pDiceEventRollEffect = nullptr;
}

void UB2UIEventPageDice::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_BuyDicePoint, &UB2UIEventPageDice::OnClickBTN_BuyDicePoint);
}

void UB2UIEventPageDice::UpdateStaticText()
{
	//[@SameLine, 180905]
	//글로벌 나갈시 타임존과 초기화 시간내려주는거 작업 예정 서버와 협의
	//임시로 24 넣음 == 무료 주사위는 매일 24시에 지급 됩니다.
	if (TB_NpcDialog.IsValid())
	{
		TB_NpcDialog->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_NPCMessage")), FText::AsNumber(24)));
	}
}

void UB2UIEventPageDice::SubscribeEvents_EventPage()
{
	//UnsubscribeEvents_EventPage();

	//CAPTURE_UOBJECT(UB2UIEventPageDice);

	//Issues_EventPage.Add(DeliveryRollDiceEventClass<FB2ResponseRollDiceEventPtr>::GetInstance().Subscribe2(
	//[Capture](FB2ResponseRollDiceEventPtr ReciveEventReward)
	//{
	//	if (Capture.IsValid())
	//	{
	//		Capture->ResponseRollDiceEvent(ReciveEventReward);
	//	}
	//}
	//));

	//Issues_EventPage.Add(DeliveryBuyDiceEventPointClass<FB2ResponseBuyDiceEventPointPtr>::GetInstance().Subscribe2(
	//	[Capture](FB2ResponseBuyDiceEventPointPtr ReciveDiceEventPoint)
	//{
	//	if (Capture.IsValid())
	//	{
	//		Capture->ResponseBuyDiceEventPoint(ReciveDiceEventPoint);
	//	}
	//}
	//));

	//Issues_EventPage.Add(DeliveryRewardDiceEventMileageClass<FB2ResponseRewardDiceEventMileagePtr>::GetInstance().Subscribe2(
	//	[Capture](FB2ResponseRewardDiceEventMileagePtr ReciveDiceEventMileage)
	//{
	//	if (Capture.IsValid())
	//	{
	//		Capture->ResponseRewardDiceEventMileage(ReciveDiceEventMileage);
	//	}
	//}
	//));

	//Issues_EventPage.Add(DiceCellFinishAnimationClass<int32>::GetInstance().Subscribe2(
	//	[Capture](int32 iCellNumber)
	//{
	//	if (Capture.IsValid())
	//	{
	//		Capture->UpdateDiceMovePositionAnim(iCellNumber);
	//	}
	//}
	//));

	//Issues_EventPage.Add(DiceCellFinishArriveAnimationClass<int32>::GetInstance().Subscribe2(
	//	[Capture](int32 iCellNumber)
	//{
	//	if (Capture.IsValid())
	//	{
	//		Capture->UpdateFromFinishArriveAnim(iCellNumber);
	//	}
	//}
	//));
}

void UB2UIEventPageDice::InitEventPageMasterData()
{
	InitDiceEventMasterData_Cell();
	InitDiceEventMasterData_Mileage();
	InitDiceEventMasterData_BuyPoint();
}

void UB2UIEventPageDice::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
	{
		return;
	}

	/*
	REFLECTION_REGISTER_BEGIN(B2DiceEventStatus)
	REFLECTION_REGISTER_FIELD(int32, event_id)
	REFLECTION_REGISTER_FIELD(int32, point)
	REFLECTION_REGISTER_FIELD(int32, free_dice)
	REFLECTION_REGISTER_FIELD(int32, position)
	REFLECTION_REGISTER_FIELD(TArray<B2DiceEventMissionPtr>, missions)
	REFLECTION_REGISTER_FIELD(int32, finish_count)
	REFLECTION_REGISTER_FIELD(int32, buy_point_count)
	REFLECTION_REGISTER_FIELD(int32, roll_dice_count)
	REFLECTION_REGISTER_END(8)
	*/

	const int32 iUseOnlyThisIndex = 0;
	const TArray<b2network::B2DiceEventStatusPtr> ArrayEventStatus = ReciveGetEvent->dice_event_statuses;
	if (ArrayEventStatus.IsValidIndex(iUseOnlyThisIndex))
	{
		EventStatus.point.Set(ArrayEventStatus[iUseOnlyThisIndex]->point);
		EventStatus.free_dice.Set(ArrayEventStatus[iUseOnlyThisIndex]->free_dice);
		EventStatus.position.Set(ArrayEventStatus[iUseOnlyThisIndex]->position);
		EventStatus.finish_count.Set(ArrayEventStatus[iUseOnlyThisIndex]->finish_count);
		EventStatus.buy_point_count.Set(ArrayEventStatus[iUseOnlyThisIndex]->buy_point_count);
		EventStatus.roll_dice_count.Set(ArrayEventStatus[iUseOnlyThisIndex]->roll_dice_count);
		EventStatus.mileage_rewarded_value.Set(ArrayEventStatus[iUseOnlyThisIndex]->mileage_reward_value);
		EventStatus.VisibleStartIcon.Set(true);
	}

	UpdateDiceEventMain(EventStatus.free_dice.Get(), EventStatus.point.Get(), EventStatus.roll_dice_count.Get(), EventStatus.finish_count.Get());
	UpdateDiceEventExplain(EventStatus.finish_count.Get());
	UpdateDiceEventSlot(EventStatus.position.Get());
	UpdateDiceMileage(EventStatus.roll_dice_count.Get(), EventStatus.mileage_rewarded_value.Get());
	UpateDiceBuyPointBtn(EventStatus.buy_point_count.Get());
	UpdateDiceSlotStartIcon(EventStatus.roll_dice_count.Get() > 0);
}

void UB2UIEventPageDice::ResponseRollDiceEvent(FB2ResponseRollDiceEventPtr pReciveEventReward)
{
	if (pReciveEventReward == nullptr)
	{
		return;
	}

	/*
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseRollDiceEvent)
	REFLECTION_REGISTER_FIELD(B2RewardPtr, cell_reward)
	REFLECTION_REGISTER_FIELD(B2RewardPtr, finish_reward)
	REFLECTION_REGISTER_FIELD(int32, current_position)
	REFLECTION_REGISTER_FIELD(int32, dice_number)
	REFLECTION_REGISTER_FIELD(int32, dice_cell_type)
	REFLECTION_REGISTER_FIELD(bool, is_finish)
	REFLECTION_REGISTER_FIELD(int32, reward_count_times)
	REFLECTION_REGISTER_FIELD(int32, free_dice_count)
	REFLECTION_REGISTER_FIELD(int32, finish_count)
	REFLECTION_REGISTER_FIELD(int32, roll_dice_count)
	DECLARE_FIELD(10, int32, current_point)
	REFLECTION_REGISTER_END(11)
	*/

	EventStatus.free_dice.Set(pReciveEventReward->free_dice_count);
	EventStatus.roll_dice_count.Set(pReciveEventReward->roll_dice_count);
	EventStatus.finish_count.Set(pReciveEventReward->finish_count);
	EventStatus.dice_number.Set(pReciveEventReward->dice_number);
	EventStatus.current_position.Set(pReciveEventReward->current_position);
	EventStatus.point.Set(pReciveEventReward->current_point);

	bool IsReward = false;
	if (pReciveEventReward->cell_reward != nullptr ||
		pReciveEventReward->finish_reward != nullptr)
	{
		IsReward = true;
	}
	
	EventStatus.IsReward.Set(IsReward);

	PlayAnimDiceRollEffect();
}

void UB2UIEventPageDice::ResponseBuyDiceEventPoint(FB2ResponseBuyDiceEventPointPtr pReciveDiceEventPoint)
{
	if (pReciveDiceEventPoint == nullptr)
	{
		return;
	}

	/*
	REFLECTION_REGISTER_BEGIN(FB2MessageInfoResponseBuyDiceEventPoint)
	REFLECTION_REGISTER_FIELD(int32, current_gem)
	REFLECTION_REGISTER_FIELD(int32, buy_count)
	REFLECTION_REGISTER_FIELD(int32, point)
	REFLECTION_REGISTER_END(3)
	*/

	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_EVENT });

	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, pReciveDiceEventPoint->current_gem);

	EventStatus.point.Set(pReciveDiceEventPoint->point);
	EventStatus.buy_point_count.Set(pReciveDiceEventPoint->buy_count);
	
	UpdateDiceEventMain(EventStatus.free_dice.Get(), EventStatus.point.Get(), EventStatus.roll_dice_count.Get(), EventStatus.finish_count.Get());
	UpateDiceBuyPointBtn(EventStatus.buy_point_count.Get());
}

void UB2UIEventPageDice::ResponseRewardDiceEventMileage(FB2ResponseRewardDiceEventMileagePtr pReciveDiceEventMileage)
{
	if (pReciveDiceEventMileage == nullptr)
	{
		return;
	}

	EventStatus.mileage_rewarded_value.Set(pReciveDiceEventMileage->reward_value);

	UpdateDiceMileage(EventStatus.roll_dice_count.Get(), EventStatus.mileage_rewarded_value.Get());

	OpenMailPopup();
}

void UB2UIEventPageDice::UpdateDiceEventPageAfterRollDiceAnim()
{
	UpdateDiceEventMain(EventStatus.free_dice.Get(), EventStatus.point.Get(), EventStatus.roll_dice_count.Get(), EventStatus.finish_count.Get());
	UpdateDiceEventExplain(EventStatus.finish_count.Get());
	UpdateDiceMileage(EventStatus.roll_dice_count.Get(), EventStatus.mileage_rewarded_value.Get());
	UpdateDiceMovePositionAnim(EventStatus.position.Get(), true);
	UpdateDiceSlotStartIcon(EventStatus.roll_dice_count.Get() > 0);
}

void UB2UIEventPageDice::CacheAssetsEventSlotDiceCell()
{
	const TArray<b2network::B2mdDiceEventCell>* pArrayDiceCell = RewardEventManager::GetInstance().GetArrayEventDiceInfo_Cell(iEventID);
	if (pArrayDiceCell == nullptr)
	{
		return;
	}

	const int32 iCellMax = pArrayDiceCell->Num();
	ArrayEventSlotList.SetNum(iCellMax);
	for (int32 i = 0; i < iCellMax; ++i)
	{
		ArrayEventSlotList[i] = GetCachedWidget<UB2UIEventSlotDice>(FName(*FString::Printf(TEXT("EventSlotDice_%d"), i)));
	}
}

void UB2UIEventPageDice::InitDiceEventMasterData_Cell()
{
	const TArray<b2network::B2mdDiceEventCell>* pArrayDiceCell = RewardEventManager::GetInstance().GetArrayEventDiceInfo_Cell(iEventID);
	if (pArrayDiceCell == nullptr)
	{
		return;
	}

	const int32 iArrayMax = pArrayDiceCell->Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (pArrayDiceCell->IsValidIndex(i) == false)
		{
			continue;
		}

		const int32 iArraySlotMax = ArrayEventSlotList.Num();
		for (int32 j = 0; j < iArraySlotMax; ++j)
		{
			if (ArrayEventSlotList[j].IsValid() == false)
			{
				continue;
			}

			if (auto* pEventSlotDice = Cast<UB2UIEventSlotDice>(ArrayEventSlotList[j].Get()))
			{
				if (pEventSlotDice->GetCellNumberFromWidgetName() == ((*pArrayDiceCell)[i]).cell_number)
				{
					if (((*pArrayDiceCell)[i]).cell_type == b2network::B2DiceEventCellType::START)
					{
						CachedStartSlotIndex = j;
					}

					pEventSlotDice->InitEventSlotDiceCell((*pArrayDiceCell)[i]);
					break;
				}
			}	
		}
	}
}

void UB2UIEventPageDice::InitDiceEventMasterData_Mileage()
{
	if (UIP_DiceEventMileage.IsValid())
	{
		UIP_DiceEventMileage->InitMasterData_Mileage(iEventID);
	}
}

void UB2UIEventPageDice::InitDiceEventMasterData_BuyPoint()
{
	const TArray<b2network::B2mdDiceEventBuyPoint>* pArrayDiceBuyPointMD = RewardEventManager::GetInstance().GetArrayEventDiceInfo_BuyPoint(iEventID);
	if (pArrayDiceBuyPointMD)
	{
		CachedDiceBuyPointMD.Empty();
		
		const int32 iArrayMax = pArrayDiceBuyPointMD->Num();
		for (int32 i = 0; i < iArrayMax; ++i)
		{
			if (pArrayDiceBuyPointMD->IsValidIndex(i) == false)
			{
				continue;
			}

			CachedDiceBuyPointMD.Add((*pArrayDiceBuyPointMD)[i]);
		}
	}
}

void UB2UIEventPageDice::PlayAnimDiceRollEffect()
{
	if (pDiceEventRollEffect.IsValid())
	{
		pDiceEventRollEffect->SetVisibility(ESlateVisibility::Visible);
		pDiceEventRollEffect->PlayAnimRollEffect_BP(EventStatus.dice_number.Get());
	}
	else
	{
		if (auto* pCreateWidget = CreateWidget<UB2UIDiceEventRollEffect>(GetWorld(), UIDiceEventRollEffectClass))
		{
			pDiceEventRollEffect = pCreateWidget;
			pDiceEventRollEffect->InitDiceEventRollEffect(this);
			pDiceEventRollEffect->SetVisibility(ESlateVisibility::Visible);
			pDiceEventRollEffect->PlayAnimRollEffect_BP(EventStatus.dice_number.Get());
		}
	}
}

void UB2UIEventPageDice::UpdateDiceMovePositionAnim(int32 iCellNumber, bool IsFirstStart /*= false*/)
{
	if (iCellNumber >= ArrayEventSlotList.Num())	//start지점 통과
	{
		iCellNumber = 0;
	}

	if (ArrayEventSlotList[iCellNumber].IsValid() == false)
	{
		return;
	}

	if (auto* pEventSlotDice = Cast<UB2UIEventSlotDice>(ArrayEventSlotList[iCellNumber].Get()))
	{
		if (iCellNumber == EventStatus.current_position.Get())
		{
			pEventSlotDice->StartAnimArrive_BP();
		}
		else
		{
			if (IsFirstStart)
			{
				pEventSlotDice->EndAnimMovePosition_BP();
			}
			else
			{
				pEventSlotDice->StartAnimMovePosition_BP();
			}
		}
	}
}

void UB2UIEventPageDice::UpdateFromFinishArriveAnim(int32 iCellNumber)
{
	EventStatus.position.Set(iCellNumber);

	if (pDiceEventRollEffect.IsValid())
	{
		pDiceEventRollEffect->SetVisibility(ESlateVisibility::Hidden);
	}

	if (EventStatus.IsReward.Get() > 0)
	{
		OpenMailPopup();
	}
}

void UB2UIEventPageDice::UpdateDiceEventMain(int32 iFreeDiceCount, int32 iGetDicePoint, int32 iRollDiceCount, int32 iFinishCount)
{
	if (UIP_DiceEventMain.IsValid())
	{
		UIP_DiceEventMain->UpdateDiceEventMain(iEventID, iFreeDiceCount, iGetDicePoint, iRollDiceCount, iFinishCount);
	}
}

void UB2UIEventPageDice::UpdateDiceEventExplain(int32 iFinishCount)
{
	if (UIP_DiceEventExplain.IsValid())
	{
		UIP_DiceEventExplain->UpdateEventExplain(iEventID, iFinishCount);
	}
}

void UB2UIEventPageDice::UpdateDiceEventSlot(int32 iPosition)
{
	const int32 iArraySlotMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < iArraySlotMax; ++i)
	{
		if (ArrayEventSlotList[i].IsValid() == false)
		{
			continue;
		}

		if (auto* pEventSlotDice = Cast<UB2UIEventSlotDice>(ArrayEventSlotList[i].Get()))
		{
			pEventSlotDice->UpdateArriveState(iPosition);
		}
	}
}

void UB2UIEventPageDice::UpdateDiceSlotStartIcon(bool IsStarted)
{
	if (IsStarted == false || EventStatus.VisibleStartIcon.Get() == false)
	{
		return;
	}

	if (ArrayEventSlotList.IsValidIndex(CachedStartSlotIndex) == false)
	{
		return;
	}

	if (auto* pEventSlotDice = Cast<UB2UIEventSlotDice>(ArrayEventSlotList[CachedStartSlotIndex].Get()))
	{
		if (pEventSlotDice->GetCachedCellType() == b2network::B2DiceEventCellType::START)
		{
			EventStatus.VisibleStartIcon.Set(false);
			pEventSlotDice->SetVisibleStartIcon(false);
		}
	}
}

void UB2UIEventPageDice::UpdateDiceMileage(int32 iRollDiceCount, int32 MileageRewardedValue)
{
	if (UIP_DiceEventMileage.IsValid())
	{
		UIP_DiceEventMileage->UpdateMileage(iRollDiceCount, MileageRewardedValue);
	}
}

void UB2UIEventPageDice::UpateDiceBuyPointBtn(int32 iBuyPointCount)
{
	if (TB_BtnBuyDicePoint.IsValid())
	{
		const int32 iMaxBuyPointCount = CachedDiceBuyPointMD.Num();
		int32 iPossibleBuyPointCount = iMaxBuyPointCount - iBuyPointCount;
		if (iPossibleBuyPointCount < 0)
		{
			iPossibleBuyPointCount = 0;
		}

		FText ftBuyPoint = BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_BuyPointButton"));
		FText ftBtnBuyDicePoint = FText::FromString(FString::Printf(TEXT("%d/%d"), iPossibleBuyPointCount, iMaxBuyPointCount));

		FString FinalString = ftBuyPoint.ToString();
		FinalString.Append(TEXT("\n"));
		FinalString.Append(ftBtnBuyDicePoint.ToString());

		TB_BtnBuyDicePoint->SetText(FText::FromString(FinalString));
	}

	const int32 iMaxBuyPointCount = CachedDiceBuyPointMD.Num();
	bool IsPossibleBuyPointCount = iBuyPointCount < iMaxBuyPointCount ? true : false;

	PlayAnimPossibleBuyPoint_BP(IsPossibleBuyPointCount);
}

void UB2UIEventPageDice::OnClickBTN_BuyDicePoint()
{
	const int32 iBuyPointCount = EventStatus.buy_point_count.Get();
	const int32 iMaxBuyPointCount = CachedDiceBuyPointMD.Num();

	bool IsPossibleBuyPointCount = iBuyPointCount < iMaxBuyPointCount ? true : false;

	if (IsPossibleBuyPointCount)
	{
		FText ftBuyPoint;

		if (CachedDiceBuyPointMD.IsValidIndex(iBuyPointCount))
		{
			ftBuyPoint = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_EVENT), FString(TEXT("DiceEvent_BuyPointDesc"))),
				FText::FromString(FString::FromInt(CachedDiceBuyPointMD[iBuyPointCount].gem)), FText::FromString(FString::FromInt(CachedDiceBuyPointMD[iBuyPointCount].point)));
		}

		UB2UIMsgPopupBuyDicePoint* pUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupBuyDicePoint>(
			EUIMsgPopup::MsgPopupBuyDicePoint,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			ftBuyPoint,
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda(
			[this]()
			{
				this->RequestBuyDiceEventPoint();

			}
			));
	}
	else
	{
		UB2UIMsgPopupSimple* pPopupUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DiceEvent_SoldOutDesc")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm);
	}
}

void UB2UIEventPageDice::RequestBuyDiceEventPoint()
{
	data_trader::Retailer::GetInstance().RequestBuyDiceEventPoint(iEventID);
}