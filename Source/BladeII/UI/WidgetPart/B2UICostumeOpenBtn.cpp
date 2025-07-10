// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UICostumeOpenBtn.h"

#include "BladeIIGameImpl.h"
#include "B2UIManager.h"

void UB2UICostumeOpenBtn::Init()
{
	Super::Init();

	SubscribeEvents();
}

void UB2UICostumeOpenBtn::DestroySelf(UB2UIManager* InUIManager)
{
	UnSubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UICostumeOpenBtn::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_CostumeOpen.Get()), &UB2UICostumeOpenBtn::HasRedDot_CostumeOpenBtn);
}

void UB2UICostumeOpenBtn::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_CostumeOpen);
	GET_SLOT(UB2RichTextBlock, RTB_CostumeOpen);
}

void UB2UICostumeOpenBtn::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_CostumeOpen, &UB2UICostumeOpenBtn::OnClickBTN_CostumeOpen);
}

void UB2UICostumeOpenBtn::UpdateStaticText()
{
	if (RTB_CostumeOpen.IsValid())
	{
		//RTB_CostumeOpen->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CostumeOpenBtn")));
	}
}

void UB2UICostumeOpenBtn::SubscribeEvents()
{
	UnSubscribeEvents();

	TWeakObjectPtr<UB2UICostumeOpenBtn>	Capture(this);

	Issues.Add(OpenCostumeClass<bool>::GetInstance().Subscribe2(
	[Capture](bool IsOpen)
	{
		if (Capture.IsValid())
		{
			Capture->UpdateVisibility(IsOpen);
		}
	}
	));
}

void UB2UICostumeOpenBtn::UnSubscribeEvents()
{
	Issues.Empty();
}

void UB2UICostumeOpenBtn::UpdateVisibility(const bool& bIsOpen)
{
	if(bIsOpen)
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UICostumeOpenBtn::OnClickBTN_CostumeOpen()
{
	EnterCostumePageClass<ECostumePage>::GetInstance().Signal(ECostumePage::Inven);
}

bool UB2UICostumeOpenBtn::HasRedDot_CostumeOpenBtn()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Costume();
}

void UB2UICostumeOpenBtn::InitCostumeOpenBtn_BP()
{
	this->Init();

	const bool& bIsOpenCostume = BladeIIGameImpl::GetCostumeDataStore().GetIsOpenCostume();

	this->UpdateVisibility(bIsOpenCostume);
}


