// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPointShopLimitedEventPopup.h"

#include "B2AndroidBackManager.h"
#include "BladeIIUtil.h"
#include "RewardEventManager.h"

void UB2PointShopEventPopupSlotVB::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB);
}

void UB2PointShopEventPopupSlotVB::AddChildWidget(UWidget* Content)
{
	if (VB.IsValid())
	{
		VB->AddChild(Content);
	}
}


void UB2PointShopEventPopupSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_Type);
	GET_SLOT(UB2RichTextBlock, RTB_Text);
}

void UB2PointShopEventPopupSlot::InitSlot(FText ftSlotText, bool IsTitleSlot /*= false*/)
{
	this->Init();

	if (WS_Type.IsValid())
	{
		const int32 iWidgetSwitcherIndex = (int32)IsTitleSlot;
		WS_Type->SetActiveWidgetIndex(iWidgetSwitcherIndex);
	}

	if (RTB_Text.IsValid())
	{
		RTB_Text->SetText(ftSlotText);
	}
}


void UB2UIPointShopLimitedEventPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UB2RichTextBlock, RTB_PopupTitle);
	GET_SLOT(UB2RichTextBlock, RTB_PopupExplain);

	GET_SLOT(UHorizontalBox, HB_Main);
}

void UB2UIPointShopLimitedEventPopup::UpdateStaticText()
{
	if (RTB_PopupTitle.IsValid())
	{
		RTB_PopupTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventLimitedPointShop_GuidePopUpTitle")));
	}

	if (RTB_PopupExplain.IsValid())
	{
		RTB_PopupExplain->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventLimitedPointShop_GuidePopUpDescription")));
	}
}

void UB2UIPointShopLimitedEventPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIPointShopLimitedEventPopup::OnClickBTN_Close);
}

void UB2UIPointShopLimitedEventPopup::CloseWidgetDelegate()
{
	ClosePopup();
}

void UB2UIPointShopLimitedEventPopup::OpenPopup()
{
	if (this->GetVisibility() != ESlateVisibility::Visible)
	{
		this->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
}

void UB2UIPointShopLimitedEventPopup::ClosePopup()
{
	if (this->GetVisibility() != ESlateVisibility::Hidden)
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}

void UB2UIPointShopLimitedEventPopup::InitPointShopLimitedEventPopup(const int32 iEventID)
{
	if (const TArray<b2network::B2mdPointEvent>* pArrayEventConditionInfo =
		RewardEventManager::GetInstance().GetArrayEventConditionInfo_Point(iEventID))
	{
		const int32 iArrayMax = pArrayEventConditionInfo->Num();
		for (int32 i = 0; i < iArrayMax; ++i)
		{
			if (pArrayEventConditionInfo->IsValidIndex(i) == false)
			{
				continue;
			}

			const int32 iEventType = (*pArrayEventConditionInfo)[i].event_type;
			const int32 iEventPoint = (*pArrayEventConditionInfo)[i].point;

			TArray<iEVENT_POINT>& ArrayEventPoint = TMap_EventPoint.FindOrAdd(iEventType);
			ArrayEventPoint.Add(iEventPoint);
		}
	}

	if (HB_Main.IsValid())
	{
		HB_Main->ClearChildren();

		InitStarGradeColumn();
		InitEventPointColumn();
	}
}

void UB2UIPointShopLimitedEventPopup::InitStarGradeColumn()
{
	if (HB_Main.IsValid() == false)
	{
		return;
	}

	int32 iStarGradeColumnMax = GetStarGradeColumnMax();

	auto* pCreateVBWidget = CreateWidget<UB2PointShopEventPopupSlotVB>(GetOwningPlayer(), PointShopEventPopupSlotVBClass);
	if (pCreateVBWidget == nullptr)
	{
		return;
	}
	
	pCreateVBWidget->Init();
	HB_Main->AddChild(pCreateVBWidget);

	//createtitle
	auto* pCreateTitleSlot = CreateWidget<UB2PointShopEventPopupSlot>(GetOwningPlayer(), PointShopEventPopupSlotClass);
	if (pCreateTitleSlot)
	{
		FText ftText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventLimitedPointShop_TableHead1"));

		pCreateTitleSlot->InitSlot(ftText, true);
		pCreateVBWidget->AddChildWidget(pCreateTitleSlot);
	}

	//createother
	for (int32 i = 0; i < iStarGradeColumnMax; ++i)
	{
		auto* pCreateSlot = CreateWidget<UB2PointShopEventPopupSlot>(GetOwningPlayer(), PointShopEventPopupSlotClass);
		if (pCreateSlot)
		{
			const int32 iTextIndex = i + 1;
			FString DescriptionKey = FString::Printf(TEXT("EventLimitedPointShop_TableData1_%d"), iTextIndex);
			FText ftText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, DescriptionKey);

			pCreateSlot->InitSlot(ftText);
			pCreateVBWidget->AddChildWidget(pCreateSlot);
		}
	}
}

void UB2UIPointShopLimitedEventPopup::InitEventPointColumn()
{
	if (HB_Main.IsValid() == false)
	{
		return;
	}

	for (auto& Elem : TMap_EventPoint)
	{
		auto* pCreateVBWidget = CreateWidget<UB2PointShopEventPopupSlotVB>(GetOwningPlayer(), PointShopEventPopupSlotVBClass);
		if (pCreateVBWidget == nullptr)
		{
			continue;
		}

		pCreateVBWidget->Init();
		HB_Main->AddChild(pCreateVBWidget);

		const int32 iEventType = Elem.Key;

		//createtitle
		auto* pCreateTitleSlot = CreateWidget<UB2PointShopEventPopupSlot>(GetOwningPlayer(), PointShopEventPopupSlotClass);
		if (pCreateTitleSlot)
		{
			//FString DescriptionFormat = );
			FString DescriptionKey = FString::Printf(TEXT("EventLimitedPointShop_TableHead%d"), iEventType);
			FText ftText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, DescriptionKey);

			pCreateTitleSlot->InitSlot(ftText, true);
			pCreateVBWidget->AddChildWidget(pCreateTitleSlot);
		}

		//createother
		TArray<iEVENT_POINT>& ArrayEventPoint = Elem.Value;
		for (int32 i = 0; i < ArrayEventPoint.Num(); ++i)
		{
			auto* pCreateSlot = CreateWidget<UB2PointShopEventPopupSlot>(GetOwningPlayer(), PointShopEventPopupSlotClass);
			if (pCreateSlot)
			{
				FText ftText = FText::FromString(FString::Printf(TEXT("%d"), ArrayEventPoint[i]));

				pCreateSlot->InitSlot(ftText);
				pCreateVBWidget->AddChildWidget(pCreateSlot);
			}
		}
	}
}

int32 UB2UIPointShopLimitedEventPopup::GetStarGradeColumnMax()
{
	int32 ColumnMax = 0;
	
	for (auto& Elem : TMap_EventPoint)
	{
		TArray<iEVENT_POINT>& ArrayEventPoint = Elem.Value;
		ColumnMax = ArrayEventPoint.Num();
		break;
	}

	return ColumnMax;
}

void UB2UIPointShopLimitedEventPopup::OnClickBTN_Close()
{
	ClosePopup();
}

