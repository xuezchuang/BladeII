// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIHotTimeBuffPart.h"
#include "CommonStruct.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "B2ItemInfo.h"
#include "B2UIManager.h"
#include "B2UIUtil.h"
#include "B2UIMsgPopupBuyHotTime.h"
#include "B2UIShortagePopupManager.h"
#include "B2GameInstance.h"

void UB2UIHotTimeBuffPart::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UOverlay, O_Active);
	GET_SLOT(UTextBlock, TB_ActiveBuffTitle);
	GET_SLOT(UTextBlock, TB_DeActiveBuffTitle);
	GET_SLOT(UB2Image, IMG_BuffImage);

	GET_SLOT(UTextBlock, TB_IncreaseRatio);
	GET_SLOT(UTextBlock, TB_BuffDescription);

	GET_SLOT(UOverlay, O_Time);
	GET_SLOT(UTextBlock, TB_RemainTime);
	GET_SLOT(UTextBlock, TB_BuffState);

	GET_SLOT(UOverlay, O_CurrentHotTime);
	GET_SLOT(UOverlay, O_NextHotTime);

	GET_SLOT(UTextBlock, TB_CurrentHotTime);
	GET_SLOT(UTextBlock, TB_NextHotTimeTitle);
	GET_SLOT(UTextBlock, TB_NextHotTime);

	GET_SLOT(UB2Button, BTN_BuyProductBuff);
	GET_SLOT(UB2Image, IMG_GoodsImage);
	GET_SLOT(UTextBlock, TB_Price);

	GET_SLOT(UHorizontalBox, HB_IncreaseRatio);
}

void UB2UIHotTimeBuffPart::UpdateBuffUI()
{
	switch (EventType)
	{
	case EHotTimeEventType::Event:
		UpdateEventTypeUI();
		break;
	case EHotTimeEventType::Product:
		UpdateProductTypeUI();
		break;
	}
}	

void UB2UIHotTimeBuffPart::UpdateStaticText()
{
	if (TB_NextHotTimeTitle.IsValid())
		TB_NextHotTimeTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_NextHotTime")));
}

void UB2UIHotTimeBuffPart::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_BuyProductBuff, &UB2UIHotTimeBuffPart::OnClickBuyProductBuff);
}

void UB2UIHotTimeBuffPart::ChangeActiveBuff(bool Active)
{
	if (O_Active.IsValid())
		O_Active->SetVisibility(Active ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (TB_ActiveBuffTitle.IsValid())
		TB_ActiveBuffTitle->SetVisibility(Active ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (TB_DeActiveBuffTitle.IsValid())
		TB_DeActiveBuffTitle->SetVisibility(Active ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIHotTimeBuffPart::SetVisibleUIForEventType()
{
	if (EventType == EHotTimeEventType::Event)
	{
		if(BTN_BuyProductBuff.IsValid())
			BTN_BuyProductBuff->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (EventType == EHotTimeEventType::Product)
	{
		if (O_CurrentHotTime.IsValid())
			O_CurrentHotTime->SetVisibility(ESlateVisibility::Collapsed);
		if (O_NextHotTime.IsValid())
			O_NextHotTime->SetVisibility(ESlateVisibility::Collapsed);
		if (TB_BuffState.IsValid())
			TB_BuffState->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIHotTimeBuffPart::UpdateEventTypeUI()
{
	FDateTime CurrentTime = UB2GameInstance::GetKoreaTimeNow();
	//UE_LOG(LogBladeII, Log, TEXT("%s"), *(CurrentTime.ToString()));
	bool bActiveEventPeriod = HotTimeBuffManager::GetInstance().IsInHotTimeEventPeriod(CurrentTime);
	bool bActiveInHour = HotTimeBuffManager::GetInstance().IsInHotTimeEventPeriod(BuffType, CurrentTime);

	ChangeActiveBuff(bActiveInHour);
	
	if (O_CurrentHotTime.IsValid())
		O_CurrentHotTime->SetVisibility(bActiveInHour ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (O_Time.IsValid())
		O_Time->SetVisibility(bActiveInHour ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	FHotTimeEventDetailInfo CurrentInfo;
	FHotTimeEventDetailInfo NextInfo;
	bool HaveNextInfo = HotTimeBuffManager::GetInstance().GetEventBuffInfoInTime(CurrentTime, BuffType, CurrentInfo, NextInfo);

	if (HB_IncreaseRatio.IsValid())
		HB_IncreaseRatio->SetVisibility((bActiveEventPeriod &&(bActiveInHour || HaveNextInfo)) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (bActiveInHour)
	{
		FTimespan CurrentRemainTime = CurrentInfo.GetEndTime(CurrentTime) - CurrentTime;

		if(TB_CurrentHotTime.IsValid())
			TB_CurrentHotTime->SetText(GetHotTimeHourMinText(CurrentInfo));
		
		if (TB_RemainTime.IsValid())
			TB_RemainTime->SetText(B2UIUtil::TwoStepTimeSpanToString(CurrentRemainTime));

		if (TB_IncreaseRatio.IsValid())
			TB_IncreaseRatio->SetText(FText::AsNumber(CurrentInfo.increase_rate));
	}

	if (O_NextHotTime.IsValid())
		O_NextHotTime->SetVisibility((bActiveEventPeriod && !bActiveInHour && HaveNextInfo) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (bActiveEventPeriod && !bActiveInHour &&  HaveNextInfo)
	{
		if (TB_NextHotTime.IsValid())
			TB_NextHotTime->SetText(GetHotTimeHourMinText(NextInfo));

		if (TB_IncreaseRatio.IsValid())
			TB_IncreaseRatio->SetText(FText::AsNumber(NextInfo.increase_rate));
	}

	if (TB_BuffState.IsValid())
	{
		TB_BuffState->SetVisibility(!bActiveInHour ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		if (!bActiveInHour)
			TB_BuffState->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, bActiveEventPeriod ? TEXT("HotTime_EndTime") : TEXT("HotTime_EndEvent") ));
	}
}

void UB2UIHotTimeBuffPart::UpdateProductTypeUI()
{
	FDateTime CurrentTime = UB2GameInstance::GetUTCNow();
	bool bActiveProduct = HotTimeBuffManager::GetInstance().IsActiveHotTimeProduct(BuffType, CurrentTime);

	FHotTimeProductBuffInfo ProductBuffInfo;
	HotTimeBuffManager::GetInstance().GetProductBuffInfo(BuffType, ProductBuffInfo);

	ChangeActiveBuff(bActiveProduct);

	if (O_Time.IsValid())
		O_Time->SetVisibility(bActiveProduct ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (TB_IncreaseRatio.IsValid())
		TB_IncreaseRatio->SetText(FText::AsNumber(ProductBuffInfo.increaseRate));

	if (bActiveProduct)
	{
		FDateTime EndTime;
		HotTimeBuffManager::GetInstance().GetProductBuffEndTime(BuffType, EndTime);

		FTimespan CurrentRemainTime = EndTime - CurrentTime;

		if (TB_RemainTime.IsValid())
			TB_RemainTime->SetText(B2UIUtil::TwoStepTimeSpanToString(CurrentRemainTime));
	}
	
	int32 price = 0;
	if (HotTimeBuffManager::GetInstance().AvailablePurchaseBuffTicket(BuffType))
	{
		price = HotTimeBuffManager::GetInstance().GetBuffTicketCount(BuffType);

		if (IMG_GoodsImage.IsValid())
			IMG_GoodsImage->SetBrushFromMaterial(StaticFindItemInfo()->GetItemIcon(HotTimeBuffManager::GetInstance().GetBuffTicketID(BuffType)));
	}
	else
	{
		price = ProductBuffInfo.price;

		if (IMG_GoodsImage.IsValid())
			IMG_GoodsImage->SetBrushFromMaterial(StaticFindItemInfo()->GetItemIcon(FItemRefIDHelper::GetGoodsID_Gem()));

		if (TB_Price.IsValid() && !HotTimeBuffManager::GetInstance().AvailablePurchaseGem(BuffType))
			TB_Price->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
	}
	if (TB_Price.IsValid())
		TB_Price->SetText(FText::AsNumber(price));
	
}

FText UB2UIHotTimeBuffPart::GetHotTimeHourMinText(const FHotTimeEventDetailInfo & InEventDetailInfo)
{
	FNumberFormattingOptions Options;
	Options.MinimumIntegralDigits = 2;

	////추후 Local Timezone에 의해 변환해서 표기 해야될 수도 있음
	////-가 나올수도 있으므로 2일
	//FDateTime CompareTime = FDateTime(1, 1, 2);
	//FDateTime BeginTime = InEventDetailInfo.GetBeginTime(CompareTime);
	//FDateTime EndTime = InEventDetailInfo.GetEndTime(CompareTime);

	return FText::Format(FText::FromString(TEXT("{0}:{1} ~ {2}:{3}")),
		FText::AsNumber(InEventDetailInfo.begin_hour, &Options), FText::AsNumber(InEventDetailInfo.begin_min, &Options),
		FText::AsNumber(InEventDetailInfo.end_hour, &Options), FText::AsNumber(InEventDetailInfo.end_min, &Options));
}

void UB2UIHotTimeBuffPart::OnClickBuyProductBuff()
{
	if (!HotTimeBuffManager::GetInstance().AvailablePurchaseBuffTicket(BuffType) && 
		!HotTimeBuffManager::GetInstance().AvailablePurchaseGem(BuffType))
	{
		UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGoToShopForGemMessage();
		return;
	}

	if (HotTimeBuffManager::GetInstance().AvailablePurchaseBuffTicket(BuffType) || 
		HotTimeBuffManager::GetInstance().AvailablePurchaseGem(BuffType))
	{
		FHotTimeProductBuffInfo ProductBuffInfo;
		if (!HotTimeBuffManager::GetInstance().GetProductBuffInfo(BuffType, ProductBuffInfo))
			return;

		bool AvailTicket = HotTimeBuffManager::GetInstance().AvailablePurchaseBuffTicket(BuffType);
		FText ContentText;
		if (AvailTicket)
			ContentText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_BuyTicketDescription")), BladeIIGetLOCText(B2LOC_CAT_EVENT, BuffUIData.TitleLOCKey), ProductBuffInfo.period_hour);
		else
			ContentText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_BuyDescription")), BladeIIGetLOCText(B2LOC_CAT_EVENT, BuffUIData.TitleLOCKey), ProductBuffInfo.period_hour);
		
		UB2UIMsgPopupBuyHotTime* MsgHotTime = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupBuyHotTime>(EUIMsgPopup::BuyHotTime,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			ContentText,
			0.f,
			true);

		if (MsgHotTime != nullptr)
		{
			MsgHotTime->SetHotTimeBuffType(BuffType);
			MsgHotTime->SetHotTimeImage(BuffUIData.IconImg, BuffUIData.ImgSize);
		}
	}
}

void UB2UIHotTimeBuffPart::DestroySelf(UB2UIManager * InUIManager)
{
	Super::DestroySelf(InUIManager);
	//CreateGoodsShortcutTooltipPopup
}

void UB2UIHotTimeBuffPart::SetHotTimeData(EHotTimeEventType InEventType, EHotTimeBuffType InBuffType, const FB2HotTimeBuffUIData& InBuffUIData)
{
	//EventType = InEventType;
	//BuffType = InBuffType;
	//BuffUIData = InBuffUIData;

	//if (TB_ActiveBuffTitle.IsValid())
	//	TB_ActiveBuffTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, BuffUIData.TitleLOCKey));
	//if(TB_DeActiveBuffTitle.IsValid())
	//	TB_DeActiveBuffTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, BuffUIData.TitleLOCKey));

	//if (IMG_BuffImage.IsValid())
	//{
	//	IMG_BuffImage->SetBrushFromTexture(BuffUIData.IconImg);
	//	IMG_BuffImage->GetBrush().ImageSize = InBuffUIData.ImgSize;
	//}
	//
	//if (TB_BuffDescription.IsValid())
	//	TB_BuffDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, BuffUIData.DescriptionLOCKey));

	//SetVisibleUIForEventType();
	//UpdateBuffUI();
}

