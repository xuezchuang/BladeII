// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICommonTab.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"

#include "B2UIStoreEventLabel.h"

DECLARE_DELEGATE(FTabButtonDelegate)
void UB2UICommonTab::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UPanelWidget, p_UnSelected);
	GET_SLOT(UPanelWidget, p_Selected);
	GET_SLOT(UTextBlock, TB_Unselected);
	GET_SLOT(UTextBlock, TB_Selected);
	GET_SLOT(UB2Button, BTN_Tab);

	GET_SLOT(UB2UIStoreEventLabel, UIP_Store_Event_Label);
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->Init();
}

void UB2UICommonTab::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Tab, &UB2UICommonTab::OnClickBTN_Tab);
}

void UB2UICommonTab::Init()
{
	Super::Init();
	SetTabSelected(false);
}

void UB2UICommonTab::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	ClearDelegate();
}

void UB2UICommonTab::OnClickBTN_Tab()
{
	ButtonDelegate.ExecuteIfBound();
}

void UB2UICommonTab::SetTabSelected(bool selected)
{
	_IsSelected = selected;

	if (p_Selected.IsValid())
		p_Selected->SetVisibility(selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (TB_Selected.IsValid())
		TB_Selected->SetVisibility(selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (p_UnSelected.IsValid())
		p_UnSelected->SetVisibility(!selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (TB_Unselected.IsValid())
		TB_Unselected->SetVisibility(!selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UICommonTab::SetTabName(FText TabName)
{
	//内靛俊辑 静芭唱, UI困连俊辑 静芭唱 笛吝 窍唱父 荤侩秦具窃.
	CurrentTabName = TabName;
	if (TB_Unselected.IsValid())
		TB_Unselected->SetText(CurrentTabName);
	if (TB_Selected.IsValid())
		TB_Selected->SetText(CurrentTabName);
}

void UB2UICommonTab::SetVisible(bool bVisible)
{
	if (!bVisible)
	{
		if (p_Selected.IsValid())
			p_Selected->SetVisibility(ESlateVisibility::Collapsed);
		if (TB_Selected.IsValid())
			TB_Selected->SetVisibility(ESlateVisibility::Collapsed);
		if (p_UnSelected.IsValid())
			p_UnSelected->SetVisibility(ESlateVisibility::Collapsed);
		if (TB_Unselected.IsValid())
			TB_Unselected->SetVisibility(ESlateVisibility::Collapsed);
		if (BTN_Tab.IsValid())
			BTN_Tab->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (BTN_Tab.IsValid())
			BTN_Tab->SetVisibility(ESlateVisibility::Visible);
		SetTabSelected(bVisible);
	}
}

void UB2UICommonTab::SetTabHeadImage(UMaterialInstance* image)
{
	TabHeaderImage = image;
}

void UB2UICommonTab::SetTabEvent(bool bFundsSale)
{
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->SetLobbyEventLabel(bFundsSale);
}

void UB2UICommonTab::SetTabStoreEventLabel(int32 ProductId)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int32 PackageProductMark = DocStore->GetPackageProductMark(ProductId);

		if (UIP_Store_Event_Label.IsValid())
			UIP_Store_Event_Label->SetStoreEventLabel(PackageProductMark);
	}
}

void UB2UICommonTab::SetTabLimitLevel(bool bIsLimitLevel)
{
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->SetLimitLevel(bIsLimitLevel);
}

void UB2UICommonTab::SetIsTabEnable(bool InValue)
{
	if (BTN_Tab.IsValid())
	{
		BTN_Tab->SetIsEnabled(InValue);
	}
}

void UB2UICommonTab::ClearDelegate()
{
	ButtonDelegate.Unbind();
}

UB2UnitedWidgetBase * UB2UICommonTab::GetMyRedDotWidget()
{
	return GetRedDotWidget();
}
