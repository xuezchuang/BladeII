// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIModRewardBoxDetailInfo.h"
#include "B2UIManager.h"

#include "B2UIRewardEventSet.h"
#include "B2UIManager.h"

void UB2UIModRewardBoxDetailInfo::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UUniformGridPanel, X_UniformPanel);
	GET_SLOT(UTextBlock, TB_RewardCount);

	GET_SLOT(UTextBlock, TB_Desc);
	
}

void UB2UIModRewardBoxDetailInfo::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIModRewardBoxDetailInfo::OnClickBTN_Close);
}

void UB2UIModRewardBoxDetailInfo::Init()
{
	Super::Init();
}

void UB2UIModRewardBoxDetailInfo::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}


void UB2UIModRewardBoxDetailInfo::SetRewardCount(int32 Count)
{
	if (TB_RewardCount.IsValid())
		TB_RewardCount->SetText(FText::FromString(FString::Printf(TEXT("X%d"), Count)));
}

void UB2UIModRewardBoxDetailInfo::SetPanelClear()
{
	if (X_UniformPanel.IsValid())
		X_UniformPanel->ClearChildren();

	for (int32 i = 0; i < 5; ++i)
	{

		auto RewardIcon = CreateWidget<UB2UIRewardIcon>(GetWorld(), RewardIconClass);

		if (!RewardIcon)
			return;

		auto PanelSlot = Cast<UUniformGridSlot>(X_UniformPanel->AddChild(RewardIcon));

		if (!PanelSlot)
			return;

		RewardIcon->SetVisibility(ESlateVisibility::Hidden);
		PanelSlot->SetRow(i / 5);
		PanelSlot->SetColumn(i % 5);
	}

}

void UB2UIModRewardBoxDetailInfo::SetRewardIcon(int32 ItemId, int32 MinCount, int32 MaxCount, int32 PanelIndex)
{
	if (!X_UniformPanel.IsValid() && !RewardIconClass)
		return;

	auto RewardIcon = CreateWidget<UB2UIRewardIcon>(GetWorld(), RewardIconClass);

	if (!RewardIcon)
		return;

	auto PanelSlot =  Cast<UUniformGridSlot>(X_UniformPanel->AddChild(RewardIcon));

	if (!PanelSlot)
		return;

	RewardIcon->Init();
	PanelSlot->SetRow(PanelIndex / 5);
	PanelSlot->SetColumn(PanelIndex % 5);
	FText IconText;
	if(MinCount != MaxCount)
		IconText = FText::FromString(FString::Printf(TEXT("%d ~ %d"), MinCount, MaxCount));
	else
		IconText = FText::FromString(FString::Printf(TEXT("%d"), MaxCount));

	RewardIcon->UpdateRewardIcon(ItemId, IconText);


}

void UB2UIModRewardBoxDetailInfo::UpdateStaticText()
{
	if (TB_Desc.IsValid())
		TB_Desc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("duelRewardBox_RewardNotice")));
}

void UB2UIModRewardBoxDetailInfo::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UIModRewardBoxDetailInfo::OnClickBTN_Close()
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		UIManager->CloseUI(UIFName::ModRewardBoxDetail);
	}
}


