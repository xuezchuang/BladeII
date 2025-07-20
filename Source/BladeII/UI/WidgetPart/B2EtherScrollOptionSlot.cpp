// Fill out your copyright notice in the Description page of Project Settings.
#include "B2EtherScrollOptionSlot.h"

#include "B2UIManager.h"

#include "B2Inventory.h"

void UB2EtherScrollOptionSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, X_RTB_OptionValue);
	GET_SLOT(UTextBlock, X_RTB_OptionName);
}

void UB2EtherScrollOptionSlot::BindDelegates()
{
	Super::BindDelegates();
}

bool UB2EtherScrollOptionSlot::Initialize()
{
	Super::Initialize();
	
	return true;
}

void UB2EtherScrollOptionSlot::Init()
{
	Super::Init();
	GET_SLOT(UTextBlock, X_RTB_OptionValue);
	GET_SLOT(UTextBlock, X_RTB_OptionName);
}

void UB2EtherScrollOptionSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2EtherScrollOptionSlot::SetInfo(EItemOption Option, float Value, EPCClass PCClass)
{
	if (X_RTB_OptionName.IsValid())
	{
		X_RTB_OptionName->SetText(GetLOCTextOfItemOption(Option, PCClass));
	}

	if (X_RTB_OptionValue.IsValid())
	{
		X_RTB_OptionValue->SetText(FText::Format(FText::FromString(TEXT("+{0}")), GetItemOptionValueDisplayText(Option, Value, true)));
	}
}
