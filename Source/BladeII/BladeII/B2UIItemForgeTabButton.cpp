// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIItemForgeTabButton.h"
#include "BladeIILocText.h"

#include "B2UIManager.h"
#include "B2UIItemForge.h"
#include "B2UIDocHelper.h"



// B2UICollectBookIcon俊 扁檬窍咯 父甸绢 咙

void UB2UIItemForgeTabButton::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UPanelWidget, p_UnSelected);
	GET_SLOT(UPanelWidget, p_Selected);
	GET_SLOT(UTextBlock, TB_Unselected);
	GET_SLOT(UTextBlock, TB_Selected);
	GET_SLOT(UB2Button, BTN_Tab);
}

void UB2UIItemForgeTabButton::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Tab, &UB2UIItemForgeTabButton::OnClickBTN_Area);
}

void UB2UIItemForgeTabButton::UpdateStaticText()
{
	
}

void UB2UIItemForgeTabButton::Init()
{
	Super::Init();
	YouHaveBeenChosen(false);
}

void UB2UIItemForgeTabButton::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	//ClearDelegate();
}

void UB2UIItemForgeTabButton::OnClickBTN_Area()
{
	if (ItemForgeMainUI)
		ItemForgeMainUI->OnClickedTabButton(this);
}

void UB2UIItemForgeTabButton::SetOwnerItemForgeUI(class UB2UIItemForge* OwnerUI, int32 tabID)
{
	ItemForgeMainUI = OwnerUI;
	TabID = tabID;

	FText name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("TabID_%d"), TabID));

	if (TB_Unselected.IsValid())
		TB_Unselected->SetText(name);
	if (TB_Selected.IsValid())
		TB_Selected->SetText(name);
}

void UB2UIItemForgeTabButton::YouHaveBeenChosen(bool selected)
{
	_IsSelected = selected;
	OnSelected_BP();

	if (p_Selected.IsValid())
		p_Selected->SetVisibility(selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (TB_Selected.IsValid())
		TB_Selected->SetVisibility(selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (p_UnSelected.IsValid())
		p_UnSelected->SetVisibility(!selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (TB_Unselected.IsValid())
		TB_Unselected->SetVisibility(!selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UIItemForgeTabButton::SetVisible(bool bVisible)
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
		YouHaveBeenChosen(bVisible);
	}
}