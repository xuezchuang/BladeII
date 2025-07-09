// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIStoreMileageRewardPopupSlot.h"
#include "BladeIIGameImpl.h"
#include "B2UIRewardEventSet.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"


void UB2UIStoreMileageRewardPopupSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_ContentSelect);
	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UTextBlock, TB_GetReward);
	GET_SLOT(UHorizontalBox, HB_Rewards);
	GET_SLOT(UOverlay, P_ReceiveComplete);
	GET_SLOT(UTextBlock, TB_Complete);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UProgressBar, PB_Mileage);
}

void UB2UIStoreMileageRewardPopupSlot::UpdateStaticText()
{
	if (TB_GetReward.IsValid())
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));
	if (TB_Complete.IsValid())
		TB_Complete->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));
}

void UB2UIStoreMileageRewardPopupSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIStoreMileageRewardPopupSlot::OnClickBTN_GetReward);
}

void UB2UIStoreMileageRewardPopupSlot::Init()
{
	Super::Init();
}

void UB2UIStoreMileageRewardPopupSlot::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIStoreMileageRewardPopupSlot::CreateRewardBox(int32 MileageType, int32 BoxGrade)
{
	CurrentMileageType = MileageType;

	if (HB_Rewards.IsValid())
		HB_Rewards->ClearChildren();

	SetProgressBar(BoxGrade);
	SetBoxText(BoxGrade);

	int32 HashKey = FStoreMileageData::GetHashkey(CurrentMileageType, static_cast<uint32>(BoxGrade));
	auto* MileageData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(HashKey);

	if (MileageData)
	{
		UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
		for (auto& Elem : MileageData->RewardIds)
		{
			auto* CreatedWidget = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardIconClass);

			if (CreatedWidget)
			{
				CreatedWidget->Init();
				CreatedWidget->UpdateRewardIcon(Elem.Id, FText::AsNumber(Elem.Count));
				HB_Rewards->AddChild(CreatedWidget);
			}
		}
	}
}

void UB2UIStoreMileageRewardPopupSlot::SetProgressBar(int32 BoxGrade)
{
	auto* BClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(CurrentMileageType, static_cast<uint32>(EUIStoreMileageGrade::BClass)));
	auto* AClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(CurrentMileageType, static_cast<uint32>(EUIStoreMileageGrade::AClass)));
	auto* SClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(CurrentMileageType, static_cast<uint32>(EUIStoreMileageGrade::SClass)));

	int32 BMaxMileagePoint = BClassData ? BClassData->MaxMileage : 0;
	int32 AMaxMileagePoint = AClassData ? AClassData->MaxMileage : 0;
	int32 SMaxMileagePoint = SClassData ? SClassData->MaxMileage : 0;

	int32 CurrentMileagePoint = 0;

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		CurrentMileagePoint = DocStore->GetMileagePoint(CurrentMileageType);
		if (PB_Mileage.IsValid())
		{
			switch (BoxGrade)
			{
			case EStoreMileageBoxType::MileageBoxB:
				PB_Mileage->SetPercent(CurrentMileagePoint >= BMaxMileagePoint ? 1.0f : CurrentMileagePoint / (float)BMaxMileagePoint);
				break;

			case EStoreMileageBoxType::MileageBoxA:
				if (CurrentMileagePoint >= AMaxMileagePoint)
				{
					PB_Mileage->SetPercent(1.0f);
				}
				else
				{
					PB_Mileage->SetPercent(CurrentMileagePoint < BMaxMileagePoint ?
						0.0f : (float)(CurrentMileagePoint - BMaxMileagePoint) / (float)(AMaxMileagePoint - BMaxMileagePoint));
				}
				break;

			case EStoreMileageBoxType::MileageBoxS:
				if (CurrentMileagePoint >= SMaxMileagePoint)
				{
					PB_Mileage->SetPercent(1.0f);
				}
				else
				{
					PB_Mileage->SetPercent(CurrentMileagePoint < AMaxMileagePoint ?
						0.0f : (float)(CurrentMileagePoint - AMaxMileagePoint) / (float)(SMaxMileagePoint - AMaxMileagePoint));
				}
				break;
			}
		}
	}
}

void UB2UIStoreMileageRewardPopupSlot::SetBoxText(int32 BoxGrade)
{
	if (TB_Title.IsValid())
	{
		switch (BoxGrade)
		{
		case EStoreMileageBoxType::MileageBoxB:
			TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Mileage_BClass")));
			break;

		case EStoreMileageBoxType::MileageBoxA:
			TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Mileage_AClass")));
			break;

		case EStoreMileageBoxType::MileageBoxS:
			TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Mileage_SClass")));
			break;
		}
	}
}

void UB2UIStoreMileageRewardPopupSlot::SetRewardState(EUIStoreMileageRewardState RewardState, bool EnableState)
{
	if (WS_ContentSelect.IsValid())
		WS_ContentSelect->SetActiveWidgetIndex(RewardState == EUIStoreMileageRewardState::Reached ? 1 : 0);

	if (P_ReceiveComplete.IsValid())
		P_ReceiveComplete->SetVisibility(RewardState == EUIStoreMileageRewardState::Rewarded ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (BTN_GetReward.IsValid())
		BTN_GetReward->SetIsEnabled(EnableState);
}

void UB2UIStoreMileageRewardPopupSlot::OnClickBTN_GetReward()
{
	data_trader::Retailer::GetInstance().RequestRewardShopMileage(CurrentMileageType);
}
