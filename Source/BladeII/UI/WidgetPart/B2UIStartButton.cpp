// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIStartButton.h"
#include "B2UIManager.h"



void UB2UIStartButton::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_BattleStart);
	GET_SLOT(UTextBlock, TB_BattleStart_D);
	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UImage, IMG_Icon_D);
	GET_SLOT(UImage, IMG_Icon_Second);
	GET_SLOT(UImage, IMG_Icon_D_Second);
	GET_SLOT(UB2Button, BTN_Start);
	GET_SLOT(UTextBlock, TB_GoodsNumber);
	GET_SLOT(UTextBlock, TB_GoodsNumber_D);
	GET_SLOT(UTextBlock, TB_GoodsNumber_Second);
	GET_SLOT(UTextBlock, TB_GoodsNumber_D_Second);
	GET_SLOT(UWidgetSwitcher, ButtonSwitcher);
}

void UB2UIStartButton::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Start, &UB2UIStartButton::OnClickBTN_Start);
}

void UB2UIStartButton::Init()
{
	Super::Init();

	OpenSetting();
	OpenAnimation();

	ClickState = false;
}

void UB2UIStartButton::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIStartButton::OpenSetting()
{
	if (GoodsIMG && IMG_Icon.IsValid())
		IMG_Icon->SetBrushFromMaterial(GoodsIMG);

	if (TB_GoodsNumber.IsValid())
		TB_GoodsNumber->SetVisibility(GoodsHide ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	if (IMG_Icon.IsValid())
		IMG_Icon->SetVisibility(GoodsHide ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	if (GoodsIMG && IMG_Icon_D.IsValid())
		IMG_Icon_D->SetBrushFromMaterial(GoodsIMG);

	if (TB_GoodsNumber_D.IsValid())
		TB_GoodsNumber_D->SetVisibility(GoodsHide ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	if (IMG_Icon_D.IsValid())
		IMG_Icon_D->SetVisibility(GoodsHide ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UB2UIStartButton::SetStartButtonDelegate(FStartButtonDelegate Delegate)
{
	ButtonDelegate = Delegate;
}

void UB2UIStartButton::SetGoodsNumber(int32 Number, bool bUsedYellowColor)
{
	if (TB_GoodsNumber.IsValid())
	{
		TB_GoodsNumber->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Number)));
		TB_GoodsNumber->SetColorAndOpacity(bUsedYellowColor ? UB2UIManager::GetInstance()->TextColor_Gold : FLinearColor::White);
	}

	if (TB_GoodsNumber_D.IsValid())
	{
		TB_GoodsNumber_D->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Number)));
	}
}

void UB2UIStartButton::SetSecondGoodsNumber(int32 Number, bool bUsedYellowColor /*= false*/)
{
	auto EnableSecondGoods = Number > 0;

	if (TB_GoodsNumber_Second.IsValid())
	{
		TB_GoodsNumber_Second->SetVisibility(EnableSecondGoods ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (TB_GoodsNumber_D_Second.IsValid())
	{
		TB_GoodsNumber_D_Second->SetVisibility(EnableSecondGoods ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (IMG_Icon_Second.IsValid())
	{
		IMG_Icon_Second->SetVisibility(EnableSecondGoods ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (IMG_Icon_D_Second.IsValid())
	{
		IMG_Icon_D_Second->SetVisibility(EnableSecondGoods ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if(EnableSecondGoods)
	{
		if (GoodsIMG_Second && IMG_Icon_Second.IsValid())
			IMG_Icon_Second->SetBrushFromMaterial(GoodsIMG_Second);

		if (GoodsIMG_Second && IMG_Icon_D_Second.IsValid())
			IMG_Icon_D_Second->SetBrushFromMaterial(GoodsIMG_Second);

		if (TB_GoodsNumber_Second.IsValid())
		{
			TB_GoodsNumber_Second->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Number)));
			TB_GoodsNumber_Second->SetColorAndOpacity(bUsedYellowColor ? UB2UIManager::GetInstance()->TextColor_Gold : FLinearColor::White);
		}

		if (TB_GoodsNumber_D_Second.IsValid())
		{
			TB_GoodsNumber_D_Second->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Number)));
		}
	}
}

void UB2UIStartButton::StartCloseAni()
{
	CloseAnimation();
}

void UB2UIStartButton::SetButtonEnabled(bool bInIsEnabled)
{
	BTN_Start->SetIsEnabled(bInIsEnabled);
	
	if (ButtonSwitcher.IsValid())
		ButtonSwitcher->SetActiveWidgetIndex(!bInIsEnabled);
}

void UB2UIStartButton::StartGame()
{
	ButtonDelegate.ExecuteIfBound();
}

void UB2UIStartButton::OnClickBTN_Start()
{
	// 滚瓢 努腐窃荐
	// 角龙利栏肺 官牢靛等 窃荐绰 局聪皋捞记捞 场唱绊 StartGame() 龋免凳
	// 酒付档 滚瓢 局聪皋捞记捞 怖 免仿登绊 拳搁 傈券阑 矫难具 茄促绰 聪令啊 乐菌带淀.

	if (ClickState)
		return;

	ClickState = true;

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(StartGameTimemer, this, &UB2UIStartButton::SetClickState, 1.f, false);
	}

	if(StartAnimation)
		PlayAnimation(StartAnimation);

	// 矫累 滚瓢捞 喘府绊 局聪皋捞记捞 免仿登绰 荤捞矫埃悼救 拳搁 喉遏捞扼带瘤 贸府甫 困秦
	ClickDirectDelegate.ExecuteIfBound();
}

void UB2UIStartButton::SetStartButtonText(FText ButtonName)
{
	if (TB_BattleStart.IsValid())
		TB_BattleStart->SetText(ButtonName);
	if (TB_BattleStart_D.IsValid())
		TB_BattleStart_D->SetText(ButtonName);
}

void UB2UIStartButton::HideAllIcon()
{
	if (IMG_Icon.IsValid())		IMG_Icon->SetVisibility(ESlateVisibility::Collapsed);
	if (IMG_Icon_D.IsValid())	IMG_Icon_D->SetVisibility(ESlateVisibility::Collapsed);

	if (TB_GoodsNumber.IsValid()) TB_GoodsNumber->SetVisibility(ESlateVisibility::Collapsed);
	if (TB_GoodsNumber_D.IsValid()) TB_GoodsNumber_D->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_Icon_Second.IsValid()) IMG_Icon_Second->SetVisibility(ESlateVisibility::Collapsed);
	if (IMG_Icon_D_Second.IsValid()) IMG_Icon_D_Second->SetVisibility(ESlateVisibility::Collapsed);

	if (TB_GoodsNumber_Second.IsValid()) TB_GoodsNumber_Second->SetVisibility(ESlateVisibility::Collapsed);
	if (TB_GoodsNumber_D_Second.IsValid()) TB_GoodsNumber_D_Second->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIStartButton::SetStartAnimation(UWidgetAnimation* Inani)
{
	StartAnimation = Inani;
}