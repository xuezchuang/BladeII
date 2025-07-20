// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFilterCheckBox.h"

UB2UIFilterCheckBox::UB2UIFilterCheckBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIFilterCheckBox::Init()
{
	Super::Init();

	SetIsChecked(false);
	if (BTN_PreSelect.IsValid())
	{
		BTN_PreSelect->SetVisibility(ESlateVisibility::Collapsed);
		BIND_CLICK_FUNC_TO_BTN(BTN_PreSelect, &UB2UIFilterCheckBox::OnClickPreSelect)
		PreSelectFilter.Unbind();
	}
}

void UB2UIFilterCheckBox::SetIsChecked(bool InIsChecked, bool bBroadcast)
{
	if (CB_Select.IsValid())
		CB_Select->SetIsChecked(InIsChecked);

	if (TB_SelectLabel.IsValid())
		TB_SelectLabel->SetVisibility(!InIsChecked ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	if (TB_UnSelectLabel.IsValid())
		TB_UnSelectLabel->SetVisibility(InIsChecked ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	if (bBroadcast)
		OnChangeState(InIsChecked);
}

bool UB2UIFilterCheckBox::IsChecked() const
{
	if (CB_Select.IsValid())
		return CB_Select->IsChecked();
	return false;
}

void UB2UIFilterCheckBox::SetLabel(FText LabelName)
{
	if (TB_SelectLabel.IsValid())
		TB_SelectLabel->SetText(LabelName);
	if (TB_UnSelectLabel.IsValid())
		TB_UnSelectLabel->SetText(LabelName);
}

void UB2UIFilterCheckBox::SetPreFilterCheck(OnPreSelectFilter InOnPreSelectFilter)
{
	if (BTN_PreSelect.IsValid())
	{
		BTN_PreSelect->SetVisibility(ESlateVisibility::Visible);
		PreSelectFilter = InOnPreSelectFilter;
	}
}

void UB2UIFilterCheckBox::CacheAssets()
{
	GET_SLOT(UCheckBox, CB_Select);
	GET_SLOT(UTextBlock, TB_SelectLabel);
	GET_SLOT(UTextBlock, TB_UnSelectLabel);
	GET_SLOT(UB2Button, BTN_PreSelect);
}

void UB2UIFilterCheckBox::BindDelegates()
{
	if (CB_Select.IsValid())
		CB_Select->OnCheckStateChanged.AddDynamic(this, &UB2UIFilterCheckBox::OnChangeState);
}

void UB2UIFilterCheckBox::OnChangeState(bool State)
{
	if (BTN_PreSelect.IsValid() && PreSelectFilter.IsBound())
	{
		BTN_PreSelect->SetVisibility(State ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}

	if (TB_SelectLabel.IsValid())
		TB_SelectLabel->SetVisibility(!State ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	if (TB_UnSelectLabel.IsValid())
		TB_UnSelectLabel->SetVisibility(State ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	OnCheckStateChanged.Broadcast(State);

	ButtonDelegate.ExecuteIfBound();
}

void UB2UIFilterCheckBox::OnClickPreSelect()
{
	PreSelectFilter.ExecuteIfBound(this);
}
