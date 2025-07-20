// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIChangeEquipItemOrCostumeBtn.h"
#include "Event.h"

void UB2UIChangeEquipItemOrCostumeBtn::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCheckBox, CB_ChangeEquipItemOrCostume);
}

void UB2UIChangeEquipItemOrCostumeBtn::BindDelegates()
{
	Super::BindDelegates();

	if (CB_ChangeEquipItemOrCostume.IsValid())
	{
		CB_ChangeEquipItemOrCostume->OnCheckStateChanged.AddDynamic(this, &UB2UIChangeEquipItemOrCostumeBtn::OnChangeEquipItemOrCostumeBtnPressed);
	}
}

void UB2UIChangeEquipItemOrCostumeBtn::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateInputDelay(InDeltaTime);
}

void UB2UIChangeEquipItemOrCostumeBtn::InitInputDelay()
{
	fCurrentInputDelay = fMaxInputDelay;
}

void UB2UIChangeEquipItemOrCostumeBtn::UpdateInputDelay(const float& InDeltaTime)
{
	if (fCurrentInputDelay < fMaxInputDelay)
	{
		fCurrentInputDelay += InDeltaTime;
	}
}

bool UB2UIChangeEquipItemOrCostumeBtn::IsEnableInputDelay()
{
	bool IsEnableInput = false;

	if (fCurrentInputDelay >= fMaxInputDelay)
	{
		IsEnableInput = true;
		fCurrentInputDelay = 0.f;
	}

	return IsEnableInput;
}

void UB2UIChangeEquipItemOrCostumeBtn::InitEquipSlotVisibleBroadCast()
{
	CostumeEquipSlotVisibleClass<bool>::GetInstance().Signal(false);
}

void UB2UIChangeEquipItemOrCostumeBtn::OnChangeEquipItemOrCostumeBtnPressed(bool IsCheck)
{
	bool bFinalCheckState = IsCheck ? false : true;

	if (IsEnableInputDelay())
	{
		bFinalCheckState = IsCheck;

		CostumeEquipSlotVisibleClass<bool>::GetInstance().Signal(IsCheck);
	}

	CB_ChangeEquipItemOrCostume->SetIsChecked(bFinalCheckState);
}

void UB2UIChangeEquipItemOrCostumeBtn::InitChangeEquipItemOrCostumeBtn_BP()
{
	this->Init();

	InitInputDelay();

	InitEquipSlotVisibleBroadCast();
}