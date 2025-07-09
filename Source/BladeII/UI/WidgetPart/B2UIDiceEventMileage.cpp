// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIDiceEventMileage.h"

#include "BladeIIUtil.h"
#include "B2UIDiceEventMileageRewardPopup.h"
#include "B2UIDiceEventMileageSlot.h"
#include "RewardEventManager.h"
#include "B2UIManager.h"

void UB2UIDiceEventMileage::DestroySelf(class UB2UIManager* InUIManager)
{
	if (pDiceEventMileageRewardPopup.IsValid())
	{
		pDiceEventMileageRewardPopup->DestroySelf(InUIManager);
		pDiceEventMileageRewardPopup = nullptr;
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIDiceEventMileage::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_DiceEventMileage);
	GET_SLOT(UHorizontalBox, HB_MileageSlotList);

	pDiceEventMileageRewardPopup = nullptr;
}

void UB2UIDiceEventMileage::UpdateStaticText()
{

}

void UB2UIDiceEventMileage::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_DiceEventMileage, &UB2UIDiceEventMileage::OnClickBTN_DiceEventMileage);
}

void UB2UIDiceEventMileage::InitMasterData_Mileage(const int32 iEventID)
{
	const TArray<b2network::B2mdDiceEventMileage>* pArrayMileage = RewardEventManager::GetInstance().GetArrayEventDiceInfo_Mileage(iEventID);

	if (pArrayMileage == nullptr)
	{
		return;
	}

	CachedArrayMileageInfo.Empty();

	const int32 iArrayMax = pArrayMileage->Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (pArrayMileage->IsValidIndex(i) == false)
		{
			continue;
		}

		CachedArrayMileageInfo.Add((*pArrayMileage)[i]);
	}

	InitMileage();
}

void UB2UIDiceEventMileage::InitMileage()
{
	InitMileageSlotList();
	UpdateMileage(0, 0);
}

void UB2UIDiceEventMileage::InitMileageSlotList()
{
	if (HB_MileageSlotList.IsValid() == false)
	{
		return;
	}

	ArrayMileageSlot.Empty();

	const int32 iArrayMax = CachedArrayMileageInfo.Num();
	const int32 iLastSlotIndex = iArrayMax - 1;
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (CachedArrayMileageInfo.IsValidIndex(i) == false)
		{
			continue;
		}

		auto* pCreateMileageSlot = CreateWidget<UB2UIDiceEventMileageSlot>(GetOwningPlayer(), UIDiceEventMileageSlotClass);

		int32 iProgressBarMax = CachedArrayMileageInfo[i].dice_count;
		const int32 iPrevIndex = i - 1;
		if (ArrayMileageSlot.IsValidIndex(iPrevIndex))
		{
			iProgressBarMax -= CachedArrayMileageInfo[iPrevIndex].dice_count;
		}
		
		if (UHorizontalBoxSlot* pHorizontalSlot = Cast<UHorizontalBoxSlot>(HB_MileageSlotList->AddChild(pCreateMileageSlot)))
		{
			pHorizontalSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}

		pCreateMileageSlot->InitMileageSlot(CachedArrayMileageInfo[i]);
		pCreateMileageSlot->SetProgressBarMax(iProgressBarMax);

		ArrayMileageSlot.Add(pCreateMileageSlot);
	}
}

void UB2UIDiceEventMileage::UpdateMileage(int32 iRolledDiceCount, int32 MileageRewardedValue)
{
	CachedRolledDiceCount = iRolledDiceCount;
	CachedMileageRewardValue = MileageRewardedValue;

	const int32 iArrayMax = ArrayMileageSlot.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayMileageSlot.IsValidIndex(i) == false)
		{
			continue;
		}

		int32 iProgressBarCount = iRolledDiceCount;
		const int32 iPrevIndex = i - 1;
		if (ArrayMileageSlot.IsValidIndex(iPrevIndex))
		{
			iProgressBarCount -= ArrayMileageSlot[iPrevIndex]->GetGoalDiceCount();
		}

		ArrayMileageSlot[i]->UpdateMileageSlot(iRolledDiceCount, MileageRewardedValue);
		ArrayMileageSlot[i]->UpdateMileageProgressBar(iProgressBarCount);
	}

	if (pDiceEventMileageRewardPopup.IsValid())
	{
		pDiceEventMileageRewardPopup->UpdateMileageRewardPopup(CachedRolledDiceCount, CachedMileageRewardValue);
	}
}

void UB2UIDiceEventMileage::OnClickBTN_DiceEventMileage()
{
	OpenDiceEventMileageRewardPopup();
}

void UB2UIDiceEventMileage::OpenDiceEventMileageRewardPopup()
{
	if (pDiceEventMileageRewardPopup.IsValid())
	{
		pDiceEventMileageRewardPopup->UpdateMileageRewardPopup(CachedRolledDiceCount, CachedMileageRewardValue);
		pDiceEventMileageRewardPopup->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
		{
			pDiceEventMileageRewardPopup = pUIManagerInstance->OpenUI<UB2UIDiceEventMileageRewardPopup>(UIFName::DiceEventMileageRewardPopup);
			pDiceEventMileageRewardPopup->InitMileageRewardPopup(CachedArrayMileageInfo);
			pDiceEventMileageRewardPopup->UpdateMileageRewardPopup(CachedRolledDiceCount, CachedMileageRewardValue);
			pDiceEventMileageRewardPopup->SetVisibility(ESlateVisibility::Visible);
		}
	}
}