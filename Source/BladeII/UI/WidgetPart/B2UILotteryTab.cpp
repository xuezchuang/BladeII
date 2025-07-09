// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UILotteryTab.h"


void UB2UILotteryTab::Init()
{
	Super::Init();

	UMaterialInterface* TabMaterial = nullptr;
	TabMaterial = TabActiveIcon.LoadSynchronous();;

	if (IMG_Active.IsValid() && TabMaterial != nullptr)
	{
		IMG_Active->SetBrushFromMaterial(TabMaterial);
	}

	TabMaterial = nullptr;
	TabMaterial = TabPassiveIcon.LoadSynchronous();;

	if (IMG_Passive.IsValid() && TabMaterial != nullptr)
	{
		IMG_Passive->SetBrushFromMaterial(TabMaterial);
	}
}

void UB2UILotteryTab::DestroySelf(class UB2UIManager* InUIManager)
{
	OnClickLotteryTabDelegate.Unbind();
}

void UB2UILotteryTab::SetTabPCClass(EPCClass InEPCClass)
{
	TabPCClass = InEPCClass;
}

void UB2UILotteryTab::SetOnClickLotteryTabDelegate(OnClickLotteryTab InDelegate)
{
	OnClickLotteryTabDelegate = InDelegate;
}

void UB2UILotteryTab::SetTabVisible(bool IsSelect)
{
	if (IMG_Active.IsValid())
	{
		IMG_Active->SetVisibility(IsSelect ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UILotteryTab::CacheAssets()
{
	GET_SLOT(UButton, BTN_Tab);
	GET_SLOT(UImage, IMG_Active);
	GET_SLOT(UImage, IMG_Passive);
}

void UB2UILotteryTab::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Tab, &UB2UILotteryTab::OnClickTab);
}

void UB2UILotteryTab::OnClickTab()
{
	OnClickLotteryTabDelegate.ExecuteIfBound(TabPCClass);
}